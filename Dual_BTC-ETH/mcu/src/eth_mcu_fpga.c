#define _POSIX_C_SOURCE 200809L

#include <errno.h>
#include <fcntl.h>
#include <inttypes.h>
#include <openssl/crypto.h>
#include <openssl/evp.h>
#include <openssl/sha.h>
#include <poll.h>
#include <secp256k1.h>
#include <secp256k1_recovery.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <termios.h>
#include <unistd.h>

#include "fpga_protocol.h"

#define BUFFER_CAPACITY 8192U
#define MAX_DATA_BYTES 2048U

typedef struct {
    uint8_t data[BUFFER_CAPACITY];
    size_t length;
} Buffer;

typedef struct {
    const char *chain_id;
    const char *nonce;
    const char *max_priority_fee;
    const char *max_fee;
    const char *gas_limit;
    const char *to;
    const char *value;
    const char *data;
    const char *pubkey_path;
    const char *passkey_path;
    const char *serial_path;
    bool yes;
} TransactionOptions;

static void fail(const char *message)
{
    fprintf(stderr, "error: %s\n", message);
    exit(EXIT_FAILURE);
}

static void secure_zero(void *pointer, size_t length)
{
    OPENSSL_cleanse(pointer, length);
}

static void buffer_append(Buffer *buffer, const uint8_t *data, size_t length)
{
    if (length > BUFFER_CAPACITY - buffer->length)
        fail("internal buffer capacity exceeded");
    memcpy(buffer->data + buffer->length, data, length);
    buffer->length += length;
}

static void buffer_byte(Buffer *buffer, uint8_t value)
{
    buffer_append(buffer, &value, 1U);
}

static int hex_nibble(char c)
{
    if (c >= '0' && c <= '9') return c - '0';
    if (c >= 'a' && c <= 'f') return c - 'a' + 10;
    if (c >= 'A' && c <= 'F') return c - 'A' + 10;
    return -1;
}

static bool parse_hex_exact(const char *text, uint8_t *out, size_t out_len)
{
    size_t offset = 0;
    if (!text) return false;
    if (text[0] == '0' && (text[1] == 'x' || text[1] == 'X')) offset = 2;
    if (strlen(text + offset) != out_len * 2U) return false;
    for (size_t i = 0; i < out_len; ++i) {
        int hi = hex_nibble(text[offset + 2U * i]);
        int lo = hex_nibble(text[offset + 2U * i + 1U]);
        if (hi < 0 || lo < 0) return false;
        out[i] = (uint8_t)((hi << 4) | lo);
    }
    return true;
}

static bool parse_hex_variable(const char *text, uint8_t *out,
                               size_t capacity, size_t *out_len)
{
    size_t offset = 0;
    if (!text) return false;
    if (text[0] == '0' && (text[1] == 'x' || text[1] == 'X')) offset = 2;
    size_t chars = strlen(text + offset);
    if ((chars & 1U) || chars / 2U > capacity) return false;
    *out_len = chars / 2U;
    for (size_t i = 0; i < *out_len; ++i) {
        int hi = hex_nibble(text[offset + 2U * i]);
        int lo = hex_nibble(text[offset + 2U * i + 1U]);
        if (hi < 0 || lo < 0) return false;
        out[i] = (uint8_t)((hi << 4) | lo);
    }
    return true;
}

static void print_hex(FILE *stream, const uint8_t *data, size_t length,
                      bool prefix)
{
    static const char alphabet[] = "0123456789abcdef";
    if (prefix) fputs("0x", stream);
    for (size_t i = 0; i < length; ++i) {
        fputc(alphabet[data[i] >> 4U], stream);
        fputc(alphabet[data[i] & 0x0fU], stream);
    }
}

static bool decimal_to_uint256(const char *text, uint8_t output[32])
{
    if (!text || !*text) return false;
    memset(output, 0, 32);
    for (const char *p = text; *p; ++p) {
        if (*p < '0' || *p > '9') return false;
        unsigned carry = (unsigned)(*p - '0');
        for (size_t i = 32; i-- > 0;) {
            unsigned v = (unsigned)output[i] * 10U + carry;
            output[i] = (uint8_t)v;
            carry = v >> 8U;
        }
        if (carry) return false;
    }
    return true;
}

static size_t minimal_integer(const uint8_t value[32], const uint8_t **start)
{
    size_t offset = 0;
    while (offset < 32 && value[offset] == 0) ++offset;
    *start = value + offset;
    return 32U - offset;
}

static void append_length(Buffer *out, size_t length,
                          uint8_t short_base, uint8_t long_base)
{
    if (length <= 55U) {
        buffer_byte(out, (uint8_t)(short_base + length));
        return;
    }
    uint8_t encoded[sizeof(size_t)];
    size_t n = 0, remaining = length;
    while (remaining) {
        encoded[sizeof(encoded) - 1U - n++] = (uint8_t)remaining;
        remaining >>= 8U;
    }
    buffer_byte(out, (uint8_t)(long_base + n));
    buffer_append(out, encoded + sizeof(encoded) - n, n);
}

static void rlp_bytes(Buffer *out, const uint8_t *data, size_t length)
{
    if (length == 1U && data[0] < 0x80U) {
        buffer_byte(out, data[0]);
        return;
    }
    append_length(out, length, 0x80U, 0xb7U);
    if (length) buffer_append(out, data, length);
}

static void rlp_decimal(Buffer *out, const char *decimal)
{
    uint8_t value[32];
    const uint8_t *start;
    if (!decimal_to_uint256(decimal, value))
        fail("invalid or overflowing decimal integer");
    size_t length = minimal_integer(value, &start);
    rlp_bytes(out, start, length);
}

static void rlp_list(Buffer *out, const Buffer *payload)
{
    append_length(out, payload->length, 0xc0U, 0xf7U);
    buffer_append(out, payload->data, payload->length);
}

static uint64_t rotate_left(uint64_t v, unsigned shift)
{
    return shift ? (v << shift) | (v >> (64U - shift)) : v;
}

static uint64_t load64_le(const uint8_t *input)
{
    uint64_t value = 0;
    for (unsigned i = 0; i < 8; ++i) value |= (uint64_t)input[i] << (8U * i);
    return value;
}

static void store64_le(uint8_t *output, uint64_t value)
{
    for (unsigned i = 0; i < 8; ++i) output[i] = (uint8_t)(value >> (8U * i));
}

static void keccak_permute(uint64_t state[25])
{
    static const uint64_t rc[24] = {
        UINT64_C(0x0000000000000001), UINT64_C(0x0000000000008082),
        UINT64_C(0x800000000000808a), UINT64_C(0x8000000080008000),
        UINT64_C(0x000000000000808b), UINT64_C(0x0000000080000001),
        UINT64_C(0x8000000080008081), UINT64_C(0x8000000000008009),
        UINT64_C(0x000000000000008a), UINT64_C(0x0000000000000088),
        UINT64_C(0x0000000080008009), UINT64_C(0x000000008000000a),
        UINT64_C(0x000000008000808b), UINT64_C(0x800000000000008b),
        UINT64_C(0x8000000000008089), UINT64_C(0x8000000000008003),
        UINT64_C(0x8000000000008002), UINT64_C(0x8000000000000080),
        UINT64_C(0x000000000000800a), UINT64_C(0x800000008000000a),
        UINT64_C(0x8000000080008081), UINT64_C(0x8000000000008080),
        UINT64_C(0x0000000080000001), UINT64_C(0x8000000080008008)};
    static const unsigned rotations[24] = {
        1,3,6,10,15,21,28,36,45,55,2,14,27,41,56,8,25,43,62,18,39,61,20,44};
    static const unsigned pi[24] = {
        10,7,11,17,18,3,5,16,8,21,24,4,15,23,19,13,12,2,20,14,22,9,6,1};
    for (unsigned round = 0; round < 24; ++round) {
        uint64_t column[5];
        for (unsigned x = 0; x < 5; ++x)
            column[x] = state[x] ^ state[x+5] ^ state[x+10] ^ state[x+15] ^ state[x+20];
        for (unsigned x = 0; x < 5; ++x) {
            uint64_t theta = column[(x+4)%5] ^ rotate_left(column[(x+1)%5], 1);
            for (unsigned y = 0; y < 25; y += 5) state[y+x] ^= theta;
        }
        uint64_t current = state[1];
        for (unsigned i = 0; i < 24; ++i) {
            uint64_t next = state[pi[i]];
            state[pi[i]] = rotate_left(current, rotations[i]);
            current = next;
        }
        for (unsigned y = 0; y < 25; y += 5) {
            uint64_t row[5];
            memcpy(row, state + y, sizeof(row));
            for (unsigned x = 0; x < 5; ++x)
                state[y+x] = row[x] ^ ((~row[(x+1)%5]) & row[(x+2)%5]);
        }
        state[0] ^= rc[round];
    }
}

static void keccak256(const uint8_t *input, size_t length, uint8_t output[32])
{
    enum { RATE = 136 };
    uint64_t state[25] = {0};
    while (length >= RATE) {
        for (size_t i = 0; i < RATE/8U; ++i) state[i] ^= load64_le(input + i*8U);
        keccak_permute(state); input += RATE; length -= RATE;
    }
    uint8_t last[RATE] = {0};
    memcpy(last, input, length); last[length] ^= 0x01U; last[RATE-1] ^= 0x80U;
    for (size_t i = 0; i < RATE/8U; ++i) state[i] ^= load64_le(last + i*8U);
    keccak_permute(state);
    for (size_t i = 0; i < 4; ++i) store64_le(output + i*8U, state[i]);
    secure_zero(state, sizeof(state)); secure_zero(last, sizeof(last));
}

static void append_common_fields(Buffer *payload, const TransactionOptions *o)
{
    uint8_t to[20], data[MAX_DATA_BYTES];
    size_t data_len;
    if (!parse_hex_exact(o->to, to, sizeof(to))) fail("recipient is not a 20-byte address");
    if (!parse_hex_variable(o->data, data, sizeof(data), &data_len)) fail("invalid transaction data");
    rlp_decimal(payload, o->chain_id);
    rlp_decimal(payload, o->nonce);
    rlp_decimal(payload, o->max_priority_fee);
    rlp_decimal(payload, o->max_fee);
    rlp_decimal(payload, o->gas_limit);
    rlp_bytes(payload, to, sizeof(to));
    rlp_decimal(payload, o->value);
    rlp_bytes(payload, data, data_len);
    buffer_byte(payload, 0xc0U);
}

static Buffer typed_transaction(const Buffer *payload)
{
    Buffer list = {{0},0}, typed = {{0},0};
    rlp_list(&list, payload); buffer_byte(&typed, 0x02U);
    buffer_append(&typed, list.data, list.length); return typed;
}

static bool read_pubkey(const char *path, uint8_t pubkey[33])
{
    FILE *f = fopen(path, "r");
    if (!f) return false;
    char text[128]; size_t n = fread(text, 1, sizeof(text)-1, f); fclose(f);
    text[n] = 0;
    while (n && (text[n-1] == '\n' || text[n-1] == '\r' || text[n-1] == ' ')) text[--n] = 0;
    return parse_hex_exact(text, pubkey, 33);
}

static bool pubkey_identity(const uint8_t serialized[33], secp256k1_pubkey *pub,
                            uint8_t address[20])
{
    secp256k1_context *ctx = secp256k1_context_create(SECP256K1_CONTEXT_VERIFY);
    uint8_t uncompressed[65], hash[32]; size_t length = sizeof(uncompressed);
    bool ok = ctx && secp256k1_ec_pubkey_parse(ctx, pub, serialized, 33) &&
              secp256k1_ec_pubkey_serialize(ctx, uncompressed, &length, pub,
                                            SECP256K1_EC_UNCOMPRESSED) && length == 65;
    if (ok) { keccak256(uncompressed + 1, 64, hash); memcpy(address, hash + 12, 20); }
    secure_zero(hash, sizeof(hash)); secure_zero(uncompressed, sizeof(uncompressed));
    if (ctx) secp256k1_context_destroy(ctx);
    return ok;
}

static int open_uart(const char *path)
{
    int fd = open(path, O_RDWR | O_NOCTTY | O_CLOEXEC);
    if (fd < 0) fail("cannot open FPGA UART");
    struct termios io;
    if (tcgetattr(fd, &io) != 0) { close(fd); fail("cannot read UART attributes"); }
    io.c_iflag = IGNPAR; io.c_oflag = 0; io.c_lflag = 0;
    io.c_cflag = CS8 | CLOCAL | CREAD; io.c_cc[VMIN] = 0; io.c_cc[VTIME] = 0;
    if (cfsetispeed(&io, B115200) || cfsetospeed(&io, B115200) ||
        tcsetattr(fd, TCSANOW, &io)) { close(fd); fail("cannot configure UART 115200 8N1"); }
    tcflush(fd, TCIOFLUSH); return fd;
}

static bool write_all(int fd, const uint8_t *data, size_t length)
{
    size_t off = 0;
    while (off < length) { ssize_t n = write(fd, data + off, length - off); if (n < 0 && errno == EINTR) continue; if (n <= 0) return false; off += (size_t)n; }
    return true;
}

static bool read_exact(int fd, uint8_t *data, size_t length, int timeout_ms)
{
    size_t off = 0;
    while (off < length) {
        struct pollfd p = {.fd=fd,.events=POLLIN}; int ready;
        do ready = poll(&p, 1, timeout_ms); while (ready < 0 && errno == EINTR);
        if (ready <= 0 || !(p.revents & POLLIN)) return false;
        ssize_t n = read(fd, data + off, length - off);
        if (n < 0 && errno == EINTR) continue;
        if (n <= 0) return false;
        off += (size_t)n;
    }
    return true;
}

static void fpga_sign_hash(const char *serial, const uint8_t hash[32],
                           struct btc_fpga_eth_signature_response *response)
{
    uint8_t frame[64], rx[8 + BTC_FPGA_ETH_RESPONSE_BYTES + 2]; size_t frame_len;
    uint8_t sequence = (uint8_t)(hash[0] ^ hash[15] ^ hash[31]);
    if (btc_fpga_encode_eth_hash_request(sequence, hash, frame, sizeof(frame), &frame_len))
        fail("cannot encode FPGA ETH frame");
    int fd = open_uart(serial);
    if (!write_all(fd, frame, frame_len) || tcdrain(fd)) { close(fd); fail("UART write failed"); }
    if (!read_exact(fd, rx, 8, 30000)) { close(fd); fail("FPGA response timeout"); }
    size_t payload_len = ((size_t)rx[6] << 8) | rx[7];
    if (payload_len > BTC_FPGA_ETH_RESPONSE_BYTES ||
        !read_exact(fd, rx + 8, payload_len + 2, 30000)) { close(fd); fail("incomplete FPGA response"); }
    close(fd);
    int decoded = btc_fpga_decode_eth_signature_response(rx, 8 + payload_len + 2,
                                                          sequence, response);
    if (decoded == 1) { fprintf(stderr, "FPGA status=0x%02x\n", response->status); fail("FPGA rejected ETH signing"); }
    if (decoded) fail("invalid FPGA ETH response, CRC, or sequence");
}

static bool verify_recovery(const secp256k1_pubkey *expected,
                            const uint8_t hash[32], uint8_t parity,
                            const uint8_t r[32], const uint8_t s[32])
{
    secp256k1_context *ctx = secp256k1_context_create(SECP256K1_CONTEXT_VERIFY);
    uint8_t compact[64], recovered_bytes[33], expected_bytes[33];
    size_t recovered_len=33, expected_len=33; secp256k1_pubkey recovered;
    secp256k1_ecdsa_recoverable_signature signature;
    memcpy(compact,r,32); memcpy(compact+32,s,32);
    bool ok = ctx && parity <= 1 &&
        secp256k1_ecdsa_recoverable_signature_parse_compact(ctx,&signature,compact,parity) &&
        secp256k1_ecdsa_recover(ctx,&recovered,&signature,hash) &&
        secp256k1_ec_pubkey_serialize(ctx,recovered_bytes,&recovered_len,&recovered,SECP256K1_EC_COMPRESSED) &&
        secp256k1_ec_pubkey_serialize(ctx,expected_bytes,&expected_len,expected,SECP256K1_EC_COMPRESSED) &&
        recovered_len == expected_len && CRYPTO_memcmp(recovered_bytes,expected_bytes,33) == 0;
    secure_zero(compact,sizeof(compact)); if(ctx) secp256k1_context_destroy(ctx); return ok;
}

static bool unlock_passkey(const char *path)
{
    FILE *f=fopen(path,"r"); char magic[16],salt_hex[64],verifier_hex[96]; int iterations;
    if(!f) fail("cannot open passkey record");
    if(!fgets(magic,sizeof(magic),f)||strcmp(magic,"CSPIN1\n")||
       fscanf(f,"%d\n%63s\n%95s",&iterations,salt_hex,verifier_hex)!=3) { fclose(f); fail("invalid passkey record"); }
    fclose(f); uint8_t salt[16],expected[32],actual[32];
    if(iterations<10000||iterations>2000000||!parse_hex_exact(salt_hex,salt,16)||!parse_hex_exact(verifier_hex,expected,32)) fail("invalid passkey fields");
    char pin[64]; bool unlocked=false;
    for(int attempt=1;attempt<=3;++attempt) {
        struct termios old,newio; bool hidden=isatty(STDIN_FILENO)&&tcgetattr(STDIN_FILENO,&old)==0;
        fputs("Nhap ma mo khoa MCU: ",stderr); fflush(stderr);
        if(hidden){newio=old;newio.c_lflag&=(tcflag_t)~ECHO;tcsetattr(STDIN_FILENO,TCSAFLUSH,&newio);}
        char *ok=fgets(pin,sizeof(pin),stdin); if(hidden){tcsetattr(STDIN_FILENO,TCSAFLUSH,&old);fputc('\n',stderr);} if(!ok) break;
        pin[strcspn(pin,"\r\n")]=0;
        if(PKCS5_PBKDF2_HMAC(pin,(int)strlen(pin),salt,16,iterations,EVP_sha256(),32,actual)!=1) fail("PBKDF2 failed");
        if(CRYPTO_memcmp(actual,expected,32)==0){unlocked=true;break;}
        fprintf(stderr,"Sai ma mo khoa (%d/3).\n",attempt);
    }
    secure_zero(pin,sizeof(pin));secure_zero(salt,sizeof(salt));secure_zero(expected,sizeof(expected));secure_zero(actual,sizeof(actual));return unlocked;
}

static void approve(bool yes)
{
    if (yes) return;
    char line[32];
    fputs("Nhap DONG Y de ky, hoac HUY: ",stderr);fflush(stderr);
    if(!fgets(line,sizeof(line),stdin)||strcmp(line,"DONG Y\n"))fail("transaction cancelled and frozen buffer erased");
}

static void load_identity(const char *path,uint8_t serialized[33],secp256k1_pubkey *pub,uint8_t address[20])
{
    if(!read_pubkey(path,serialized)||!pubkey_identity(serialized,pub,address))fail("invalid compressed public key file");
}

static void sign_hash_command(const char *pubkey_path,const char *passkey_path,
                              const char *serial,const char *hash_text,bool yes)
{
    uint8_t pubbytes[33],address[20],hash[32];secp256k1_pubkey pub;
    if(!parse_hex_exact(hash_text,hash,32))fail("hash must contain exactly 32 bytes");
    load_identity(pubkey_path,pubbytes,&pub,address);
    fprintf(stderr,"\nMCU ETH HASH REVIEW\nSigner: ");print_hex(stderr,address,20,true);fprintf(stderr,"\nHash:   ");print_hex(stderr,hash,32,true);fputc('\n',stderr);
    if(!unlock_passkey(passkey_path))fail("MCU locked: hash erased, FPGA not called");
    approve(yes);
    struct btc_fpga_eth_signature_response sig;fpga_sign_hash(serial,hash,&sig);
    if(!verify_recovery(&pub,hash,sig.y_parity,sig.r,sig.s))fail("FPGA signature does not recover configured public key");
    fputs("{\n  \"format\": \"dual-mcu-ethereum-hash-v1\",\n  \"from\": \"",stdout);print_hex(stdout,address,20,true);
    fputs("\",\n  \"messageHash\": \"",stdout);print_hex(stdout,hash,32,true);
    fprintf(stdout,"\",\n  \"yParity\": %u,\n  \"r\": \"",sig.y_parity);print_hex(stdout,sig.r,32,true);
    fputs("\",\n  \"s\": \"",stdout);print_hex(stdout,sig.s,32,true);fputs("\",\n  \"signatureVerified\": true\n}\n",stdout);
}

static void sign_transaction(const TransactionOptions *o)
{
    uint8_t pubbytes[33],address[20],signing_hash[32],freeze_before[32],freeze_after[32],tx_hash[32];secp256k1_pubkey pub;
    load_identity(o->pubkey_path,pubbytes,&pub,address);
    Buffer unsigned_payload={{0},0};append_common_fields(&unsigned_payload,o);Buffer unsigned_tx=typed_transaction(&unsigned_payload);
    keccak256(unsigned_tx.data,unsigned_tx.length,signing_hash);SHA256(unsigned_tx.data,unsigned_tx.length,freeze_before);
    fprintf(stderr,"\n============================================================\nMAN HINH MCU - ETHEREUM EIP-1559\n============================================================\nSigner:   ");print_hex(stderr,address,20,true);
    fprintf(stderr,"\nTo:       %s\nValue:    %s wei\nChain ID: %s\nNonce:    %s\nGas:      %s\nMax fee:  %s wei/gas\nFreeze:   ",o->to,o->value,o->chain_id,o->nonce,o->gas_limit,o->max_fee);print_hex(stderr,freeze_before,32,false);fputc('\n',stderr);
    if(!unlock_passkey(o->passkey_path))fail("MCU locked: transaction erased, FPGA not called");
    approve(o->yes);
    SHA256(unsigned_tx.data,unsigned_tx.length,freeze_after);
    if(CRYPTO_memcmp(freeze_before,freeze_after,32))fail("frozen ETH buffer changed after review");
    uint8_t hash_after[32];keccak256(unsigned_tx.data,unsigned_tx.length,hash_after);
    if(CRYPTO_memcmp(signing_hash,hash_after,32))fail("ETH signing hash changed after review");
    struct btc_fpga_eth_signature_response sig;fpga_sign_hash(o->serial_path,signing_hash,&sig);
    if(!verify_recovery(&pub,signing_hash,sig.y_parity,sig.r,sig.s))fail("FPGA signature does not recover configured public key");
    Buffer signed_payload={{0},0};append_common_fields(&signed_payload,o);uint8_t parity=sig.y_parity;
    rlp_bytes(&signed_payload,&parity,parity?1U:0U);const uint8_t *rstart,*sstart;size_t rlen=minimal_integer(sig.r,&rstart),slen=minimal_integer(sig.s,&sstart);
    rlp_bytes(&signed_payload,rstart,rlen);rlp_bytes(&signed_payload,sstart,slen);Buffer raw=typed_transaction(&signed_payload);keccak256(raw.data,raw.length,tx_hash);
    fputs("{\n  \"format\": \"dual-mcu-eip1559-v1\",\n  \"from\": \"",stdout);print_hex(stdout,address,20,true);
    fputs("\",\n  \"freezeId\": \"",stdout);print_hex(stdout,freeze_before,32,true);
    fputs("\",\n  \"messageHash\": \"",stdout);print_hex(stdout,signing_hash,32,true);
    fprintf(stdout,"\",\n  \"yParity\": %u,\n  \"r\": \"",sig.y_parity);print_hex(stdout,sig.r,32,true);
    fputs("\",\n  \"s\": \"",stdout);print_hex(stdout,sig.s,32,true);
    fputs("\",\n  \"rawTransaction\": \"",stdout);print_hex(stdout,raw.data,raw.length,true);
    fputs("\",\n  \"transactionHash\": \"",stdout);print_hex(stdout,tx_hash,32,true);
    fputs("\",\n  \"signatureVerified\": true\n}\n",stdout);
    secure_zero(&sig,sizeof(sig));secure_zero(signing_hash,sizeof(signing_hash));secure_zero(hash_after,sizeof(hash_after));
}

static const char *need_value(int argc,char **argv,int *i)
{
    if(*i+1>=argc)fail("option requires a value");
    return argv[++*i];
}

static void usage(const char *p)
{
    fprintf(stderr,"Usage:\n  %s address PUBKEYFILE\n  %s sign-hash-fpga PUBKEYFILE PASSKEY_RECORD SERIAL --hash 0xHASH [--yes]\n  %s sign-fpga PUBKEYFILE PASSKEY_RECORD SERIAL --chain-id N --nonce N --max-priority-fee-per-gas N --max-fee-per-gas N --gas-limit N --to 0xADDRESS --value WEI [--data 0x] [--yes]\n",p,p,p);
}

int eth_mcu_main(int argc,char **argv)
{
    if(argc==3&&!strcmp(argv[1],"address")){uint8_t b[33],a[20];secp256k1_pubkey p;load_identity(argv[2],b,&p,a);print_hex(stdout,a,20,true);putchar('\n');return 0;}
    if(argc>=6&&!strcmp(argv[1],"sign-hash-fpga")){const char *hash=NULL;bool yes=false;for(int i=5;i<argc;++i){if(!strcmp(argv[i],"--hash"))hash=need_value(argc,argv,&i);else if(!strcmp(argv[i],"--yes"))yes=true;else{usage(argv[0]);fail("unknown option");}}if(!hash)fail("missing --hash");sign_hash_command(argv[2],argv[3],argv[4],hash,yes);return 0;}
    if(argc<6||strcmp(argv[1],"sign-fpga")){usage(argv[0]);return 2;}
    TransactionOptions o={0};o.pubkey_path=argv[2];o.passkey_path=argv[3];o.serial_path=argv[4];o.data="0x";
    for(int i=5;i<argc;++i){const char *a=argv[i];if(!strcmp(a,"--chain-id"))o.chain_id=need_value(argc,argv,&i);else if(!strcmp(a,"--nonce"))o.nonce=need_value(argc,argv,&i);else if(!strcmp(a,"--max-priority-fee-per-gas"))o.max_priority_fee=need_value(argc,argv,&i);else if(!strcmp(a,"--max-fee-per-gas"))o.max_fee=need_value(argc,argv,&i);else if(!strcmp(a,"--gas-limit"))o.gas_limit=need_value(argc,argv,&i);else if(!strcmp(a,"--to"))o.to=need_value(argc,argv,&i);else if(!strcmp(a,"--value"))o.value=need_value(argc,argv,&i);else if(!strcmp(a,"--data"))o.data=need_value(argc,argv,&i);else if(!strcmp(a,"--yes"))o.yes=true;else{usage(argv[0]);fail("unknown option");}}
    if(!o.chain_id||!o.nonce||!o.max_priority_fee||!o.max_fee||!o.gas_limit||!o.to||!o.value)
        fail("missing required transaction option");
    sign_transaction(&o);
    return 0;
}
