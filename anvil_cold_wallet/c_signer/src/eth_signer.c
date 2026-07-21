#include <errno.h>
#include <inttypes.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#ifndef __linux__
#include "hal_spi.h"
// extern SPI_Handle_T hspi0; // Uncomment khi ghép vào project Sonix
#endif

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
    bool yes;
} TransactionOptions;

static void secure_zero(void *pointer, size_t length) {
    volatile uint8_t *bytes = (volatile uint8_t *)pointer;
    while (length-- > 0U) {
        *bytes++ = 0U;
    }
}

static void fail(const char *message) {
    fprintf(stderr, "Error: %s\n", message);
    exit(EXIT_FAILURE);
}

static void buffer_append(Buffer *buffer, const uint8_t *data, size_t length) {
    if (length > BUFFER_CAPACITY - buffer->length) {
        fail("internal buffer capacity exceeded");
    }
    memcpy(buffer->data + buffer->length, data, length);
    buffer->length += length;
}

static void buffer_byte(Buffer *buffer, uint8_t value) {
    buffer_append(buffer, &value, 1U);
}

static int hex_nibble(char character) {
    if (character >= '0' && character <= '9') {
        return character - '0';
    }
    if (character >= 'a' && character <= 'f') {
        return character - 'a' + 10;
    }
    if (character >= 'A' && character <= 'F') {
        return character - 'A' + 10;
    }
    return -1;
}

static bool parse_hex_exact(const char *text, uint8_t *output, size_t output_length) {
    size_t offset = 0U;
    if (text[0] == '0' && (text[1] == 'x' || text[1] == 'X')) {
        offset = 2U;
    }
    if (strlen(text + offset) != output_length * 2U) {
        return false;
    }
    for (size_t index = 0U; index < output_length; ++index) {
        int high = hex_nibble(text[offset + index * 2U]);
        int low = hex_nibble(text[offset + index * 2U + 1U]);
        if (high < 0 || low < 0) {
            return false;
        }
        output[index] = (uint8_t)((high << 4) | low);
    }
    return true;
}

static bool parse_hex_variable(
    const char *text, uint8_t *output, size_t capacity, size_t *output_length
) {
    size_t offset = 0U;
    if (text[0] == '0' && (text[1] == 'x' || text[1] == 'X')) {
        offset = 2U;
    }
    size_t characters = strlen(text + offset);
    if ((characters & 1U) != 0U || characters / 2U > capacity) {
        return false;
    }
    *output_length = characters / 2U;
    for (size_t index = 0U; index < *output_length; ++index) {
        int high = hex_nibble(text[offset + index * 2U]);
        int low = hex_nibble(text[offset + index * 2U + 1U]);
        if (high < 0 || low < 0) {
            return false;
        }
        output[index] = (uint8_t)((high << 4) | low);
    }
    return true;
}

static bool decimal_to_uint256(const char *text, uint8_t output[32]) {
    if (text == NULL || text[0] == '\0') {
        return false;
    }
    memset(output, 0, 32U);
    for (const char *cursor = text; *cursor != '\0'; ++cursor) {
        if (*cursor < '0' || *cursor > '9') {
            return false;
        }
        unsigned carry = (unsigned)(*cursor - '0');
        for (size_t index = 32U; index-- > 0U;) {
            unsigned value = (unsigned)output[index] * 10U + carry;
            output[index] = (uint8_t)(value & 0xffU);
            carry = value >> 8U;
        }
        if (carry != 0U) {
            return false;
        }
    }
    return true;
}

static size_t minimal_integer(const uint8_t value[32], const uint8_t **start) {
    size_t offset = 0U;
    while (offset < 32U && value[offset] == 0U) {
        ++offset;
    }
    *start = value + offset;
    return 32U - offset;
}

static void append_length(Buffer *output, size_t length, uint8_t short_base, uint8_t long_base) {
    if (length <= 55U) {
        buffer_byte(output, (uint8_t)(short_base + length));
        return;
    }
    uint8_t encoded[sizeof(size_t)];
    size_t encoded_length = 0U;
    size_t remaining = length;
    while (remaining > 0U) {
        encoded[sizeof(encoded) - 1U - encoded_length] = (uint8_t)(remaining & 0xffU);
        remaining >>= 8U;
        ++encoded_length;
    }
    buffer_byte(output, (uint8_t)(long_base + encoded_length));
    buffer_append(output, encoded + sizeof(encoded) - encoded_length, encoded_length);
}

static void rlp_bytes(Buffer *output, const uint8_t *data, size_t length) {
    if (length == 1U && data[0] < 0x80U) {
        buffer_byte(output, data[0]);
        return;
    }
    append_length(output, length, 0x80U, 0xb7U);
    if (length > 0U) {
        buffer_append(output, data, length);
    }
}

static void rlp_list(Buffer *output, const Buffer *payload) {
    append_length(output, payload->length, 0xc0U, 0xf7U);
    buffer_append(output, payload->data, payload->length);
}

static void rlp_decimal(Buffer *output, const char *decimal) {
    uint8_t value[32];
    const uint8_t *start = NULL;
    if (!decimal_to_uint256(decimal, value)) {
        fail("invalid or overflowing decimal integer");
    }
    size_t length = minimal_integer(value, &start);
    rlp_bytes(output, start, length);
}

static uint64_t rotate_left(uint64_t value, unsigned shift) {
    return shift == 0U ? value : (value << shift) | (value >> (64U - shift));
}

static uint64_t load64_le(const uint8_t *input) {
    uint64_t value = 0U;
    for (unsigned index = 0U; index < 8U; ++index) {
        value |= (uint64_t)input[index] << (8U * index);
    }
    return value;
}

static void store64_le(uint8_t *output, uint64_t value) {
    for (unsigned index = 0U; index < 8U; ++index) {
        output[index] = (uint8_t)(value >> (8U * index));
    }
}

static void keccak_permute(uint64_t state[25]) {
    static const uint64_t round_constants[24] = {
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
        UINT64_C(0x0000000080000001), UINT64_C(0x8000000080008008)
    };
    static const unsigned rotations[24] = {
        1U, 3U, 6U, 10U, 15U, 21U, 28U, 36U,
        45U, 55U, 2U, 14U, 27U, 41U, 56U, 8U,
        25U, 43U, 62U, 18U, 39U, 61U, 20U, 44U
    };
    static const unsigned pi_lanes[24] = {
        10U, 7U, 11U, 17U, 18U, 3U, 5U, 16U,
        8U, 21U, 24U, 4U, 15U, 23U, 19U, 13U,
        12U, 2U, 20U, 14U, 22U, 9U, 6U, 1U
    };

    for (unsigned round = 0U; round < 24U; ++round) {
        uint64_t column[5];
        for (unsigned x = 0U; x < 5U; ++x) {
            column[x] = state[x] ^ state[x + 5U] ^ state[x + 10U]
                      ^ state[x + 15U] ^ state[x + 20U];
        }
        for (unsigned x = 0U; x < 5U; ++x) {
            uint64_t theta = column[(x + 4U) % 5U]
                           ^ rotate_left(column[(x + 1U) % 5U], 1U);
            for (unsigned y = 0U; y < 25U; y += 5U) {
                state[y + x] ^= theta;
            }
        }

        uint64_t current = state[1];
        for (unsigned index = 0U; index < 24U; ++index) {
            unsigned lane = pi_lanes[index];
            uint64_t next = state[lane];
            state[lane] = rotate_left(current, rotations[index]);
            current = next;
        }

        for (unsigned y = 0U; y < 25U; y += 5U) {
            uint64_t row[5];
            memcpy(row, state + y, sizeof(row));
            for (unsigned x = 0U; x < 5U; ++x) {
                state[y + x] = row[x] ^ ((~row[(x + 1U) % 5U]) & row[(x + 2U) % 5U]);
            }
        }
        state[0] ^= round_constants[round];
    }
}

static void keccak256(const uint8_t *input, size_t length, uint8_t output[32]) {
    enum { RATE = 136 };
    uint64_t state[25] = {0U};
    while (length >= RATE) {
        for (size_t index = 0U; index < RATE / 8U; ++index) {
            state[index] ^= load64_le(input + index * 8U);
        }
        keccak_permute(state);
        input += RATE;
        length -= RATE;
    }

    uint8_t final_block[RATE] = {0U};
    if (length > 0U) {
        memcpy(final_block, input, length);
    }
    final_block[length] ^= 0x01U;
    final_block[RATE - 1U] ^= 0x80U;
    for (size_t index = 0U; index < RATE / 8U; ++index) {
        state[index] ^= load64_le(final_block + index * 8U);
    }
    keccak_permute(state);
    for (size_t index = 0U; index < 4U; ++index) {
        store64_le(output + index * 8U, state[index]);
    }
    secure_zero(state, sizeof(state));
    secure_zero(final_block, sizeof(final_block));
}

static void print_hex(const uint8_t *data, size_t length) {
    static const char alphabet[] = "0123456789abcdef";
    fputs("0x", stdout);
    for (size_t index = 0U; index < length; ++index) {
        fputc(alphabet[data[index] >> 4U], stdout);
        fputc(alphabet[data[index] & 0x0fU], stdout);
    }
}

static void append_common_fields(Buffer *payload, const TransactionOptions *options) {
    uint8_t address[20];
    uint8_t call_data[MAX_DATA_BYTES];
    size_t call_data_length = 0U;
    if (!parse_hex_exact(options->to, address, sizeof(address))) {
        fail("recipient must be a 20-byte hexadecimal Ethereum address");
    }
    if (!parse_hex_variable(options->data, call_data, sizeof(call_data), &call_data_length)) {
        fail("data must be even-length hexadecimal and no more than 2048 bytes");
    }

    rlp_decimal(payload, options->chain_id);
    rlp_decimal(payload, options->nonce);
    rlp_decimal(payload, options->max_priority_fee);
    rlp_decimal(payload, options->max_fee);
    rlp_decimal(payload, options->gas_limit);
    rlp_bytes(payload, address, sizeof(address));
    rlp_decimal(payload, options->value);
    rlp_bytes(payload, call_data, call_data_length);
    buffer_byte(payload, 0xc0U); /* Empty access list. */
}

static Buffer typed_transaction(const Buffer *list_payload) {
    Buffer encoded_list = {{0U}, 0U};
    Buffer typed = {{0U}, 0U};
    rlp_list(&encoded_list, list_payload);
    buffer_byte(&typed, 0x02U);
    buffer_append(&typed, encoded_list.data, encoded_list.length);
    return typed;
}

static void show_request(const TransactionOptions *options, const uint8_t address[20]) {
    fputs("\nOffline EIP-1559 signing request\n", stderr);
    fputs("Signer:      ", stderr);
    static const char alphabet[] = "0123456789abcdef";
    fputs("0x", stderr);
    for (size_t index = 0U; index < 20U; ++index) {
        fputc(alphabet[address[index] >> 4U], stderr);
        fputc(alphabet[address[index] & 0x0fU], stderr);
    }
    fprintf(stderr, "\nTo:          %s\n", options->to);
    fprintf(stderr, "Value (wei): %s\n", options->value);
    fprintf(stderr, "Chain ID:    %s\n", options->chain_id);
    fprintf(stderr, "Nonce:       %s\n", options->nonce);
    fprintf(stderr, "Gas limit:   %s\n", options->gas_limit);
    fprintf(stderr, "Data:        %s\n", options->data);
}

static void confirm_request(const TransactionOptions *options) {
    if (options->yes) {
        return;
    }
    char answer[16];
    fputs("Type 'yes' to sign: ", stderr);
    fflush(stderr);
    if (fgets(answer, sizeof(answer), stdin) == NULL || strcmp(answer, "yes\n") != 0) {
        fail("signing cancelled");
    }
}

static void spi_fpga_sign(const uint8_t message_hash[32], const uint8_t pin[4], uint8_t signature_r[32], uint8_t signature_s[32], int *recovery_id) {
#ifdef __linux__
    fprintf(stderr, "[Mock SPI] Sending Command 0x01 (SIGN_REQ) to FPGA...\n");
    fprintf(stderr, "[Mock SPI] Hash: 0x");
    for (int i = 0; i < 32; i++) fprintf(stderr, "%02x", message_hash[i]);
    fprintf(stderr, "\n[Mock SPI] PIN: %02x%02x%02x%02x\n", pin[0], pin[1], pin[2], pin[3]);
    
    fprintf(stderr, "[Mock SPI] Waiting for FPGA (AES + ECDSA)...\n");
    fprintf(stderr, "[Mock SPI] Sending Command 0x02 (READ_SIG) to FPGA...\n");
    
    memset(signature_r, 0x11, 32); // Mock R
    memset(signature_s, 0x22, 32); // Mock S
    *recovery_id = 1;
    fprintf(stderr, "[Mock SPI] Received Signature from FPGA!\n\n");
#else
    uint8_t cmd_sign[1] = {0x01};
    uint8_t cmd_read[1] = {0x02};
    uint8_t sig_buf[64];
    
    // 1. Kéo CS xuống mức thấp
    // HAL_GPIO_WritePin(SPI_CS_PORT, SPI_CS_PIN, GPIO_PIN_RESET);
    
    // 2. Gửi Lệnh SIGN_REQ
    HAL_SPI_Transmit(&hspi0, cmd_sign, 1, 1000);
    // Gửi Hash
    HAL_SPI_Transmit(&hspi0, (uint8_t*)message_hash, 32, 1000);
    // Gửi PIN
    HAL_SPI_Transmit(&hspi0, (uint8_t*)pin, 4, 1000);
    
    // 3. Kéo CS lên mức cao
    // HAL_GPIO_WritePin(SPI_CS_PORT, SPI_CS_PIN, GPIO_PIN_SET);
    
    // 4. Đợi FPGA xử lý (Polling hoặc Interrupt)
    // HAL_Delay(100); 
    
    // 5. Đọc chữ ký
    // HAL_GPIO_WritePin(SPI_CS_PORT, SPI_CS_PIN, GPIO_PIN_RESET);
    HAL_SPI_Transmit(&hspi0, cmd_read, 1, 1000);
    HAL_SPI_Receive(&hspi0, sig_buf, 64, 1000);
    // HAL_GPIO_WritePin(SPI_CS_PORT, SPI_CS_PIN, GPIO_PIN_SET);
    
    memcpy(signature_r, sig_buf, 32);
    memcpy(signature_s, sig_buf + 32, 32);
    *recovery_id = 0; // Cần FPGA trả về y_parity
#endif
}

static void sign_transaction(const TransactionOptions *options) {
    uint8_t signer_address[20] = {0x01, 0x23, 0x45, 0x67, 0x89, 0xab, 0xcd, 0xef, 0x01, 0x23, 0x45, 0x67, 0x89, 0xab, 0xcd, 0xef, 0x01, 0x23, 0x45, 0x67}; // Mock Address
    uint8_t signing_hash[32];
    uint8_t transaction_hash[32];
    uint8_t signature_r[32];
    uint8_t signature_s[32];
    int recovery_id = 0;
    uint8_t dummy_pin[4] = {0x12, 0x34, 0x56, 0x78}; // Mã PIN người dùng nhập

    show_request(options, signer_address);
    confirm_request(options);

    Buffer unsigned_payload = {{0U}, 0U};
    append_common_fields(&unsigned_payload, options);
    Buffer unsigned_transaction = typed_transaction(&unsigned_payload);
    keccak256(unsigned_transaction.data, unsigned_transaction.length, signing_hash);

    // Giao phó cho FPGA ký qua SPI
    spi_fpga_sign(signing_hash, dummy_pin, signature_r, signature_s, &recovery_id);

    Buffer signed_payload = {{0U}, 0U};
    append_common_fields(&signed_payload, options);
    uint8_t parity = (uint8_t)recovery_id;
    rlp_bytes(&signed_payload, &parity, parity == 0U ? 0U : 1U);
    const uint8_t *r_start = NULL;
    const uint8_t *s_start = NULL;
    size_t r_length = minimal_integer(signature_r, &r_start);
    size_t s_length = minimal_integer(signature_s, &s_start);
    rlp_bytes(&signed_payload, r_start, r_length);
    rlp_bytes(&signed_payload, s_start, s_length);
    Buffer raw_transaction = typed_transaction(&signed_payload);
    keccak256(raw_transaction.data, raw_transaction.length, transaction_hash);

    fputs("{\n  \"format\": \"ethereum-c-signer-v1\",\n  \"from\": \"", stdout);
    print_hex(signer_address, sizeof(signer_address));
    fputs("\",\n  \"messageHash\": \"", stdout);
    print_hex(signing_hash, sizeof(signing_hash));
    fprintf(stdout, "\",\n  \"yParity\": %d,\n  \"r\": \"", recovery_id);
    print_hex(signature_r, 32U);
    fputs("\",\n  \"s\": \"", stdout);
    print_hex(signature_s, 32U);
    fputs("\",\n  \"rawTransaction\": \"", stdout);
    print_hex(raw_transaction.data, raw_transaction.length);
    fputs("\",\n  \"transactionHash\": \"", stdout);
    print_hex(transaction_hash, sizeof(transaction_hash));
    fputs("\"\n}\n", stdout);
}

static void usage(const char *program) {
    fprintf(
        stderr,
        "Usage:\n"
        "  %s address\n"
        "  %s sign-hash --hash 0x32_BYTE_HASH [--yes]\n"
        "  %s sign --chain-id N --nonce N --max-priority-fee-per-gas N \\\n+"
        "      --max-fee-per-gas N --gas-limit N --to 0xADDRESS --value WEI \\\n+"
        "      [--data 0x] [--yes]\n",
        program,
        program,
        program
    );
}

static const char *required_value(int argc, char **argv, int *index) {
    if (*index + 1 >= argc) {
        fail("option requires a value");
    }
    ++(*index);
    return argv[*index];
}

int main(int argc, char **argv) {
    if (argc < 2 || strcmp(argv[1], "sign") != 0) {
        usage(argv[0]);
        return EXIT_FAILURE;
    }

    TransactionOptions options = {0};
    options.data = "0x";
    for (int index = 2; index < argc; ++index) {
        const char *argument = argv[index];
        if (strcmp(argument, "--chain-id") == 0) {
            options.chain_id = required_value(argc, argv, &index);
        } else if (strcmp(argument, "--nonce") == 0) {
            options.nonce = required_value(argc, argv, &index);
        } else if (strcmp(argument, "--max-priority-fee-per-gas") == 0) {
            options.max_priority_fee = required_value(argc, argv, &index);
        } else if (strcmp(argument, "--max-fee-per-gas") == 0) {
            options.max_fee = required_value(argc, argv, &index);
        } else if (strcmp(argument, "--gas-limit") == 0) {
            options.gas_limit = required_value(argc, argv, &index);
        } else if (strcmp(argument, "--to") == 0) {
            options.to = required_value(argc, argv, &index);
        } else if (strcmp(argument, "--value") == 0) {
            options.value = required_value(argc, argv, &index);
        } else if (strcmp(argument, "--data") == 0) {
            options.data = required_value(argc, argv, &index);
        } else if (strcmp(argument, "--yes") == 0) {
            options.yes = true;
        } else {
            usage(argv[0]);
            fail("unknown command-line option");
        }
    }

    if (options.chain_id == NULL || options.nonce == NULL
        || options.max_priority_fee == NULL || options.max_fee == NULL
        || options.gas_limit == NULL || options.to == NULL || options.value == NULL) {
        usage(argv[0]);
        fail("missing required transaction option");
    }
    sign_transaction(&options);
    return EXIT_SUCCESS;
}
