#ifndef BTC_FPGA_PROTOCOL_H
#define BTC_FPGA_PROTOCOL_H

#include <stddef.h>
#include <stdint.h>

#define BTC_FPGA_PROTOCOL_VERSION 0x01U
#define BTC_FPGA_COMMAND_PING 0x00U
#define BTC_FPGA_COMMAND_SIGN_ETH_HASH 0x01U
#define BTC_FPGA_COMMAND_SIGN_BIP143 0x10U

#define BTC_FPGA_MAX_OUTPUT_BYTES 512U
#define BTC_FPGA_REQUEST_FIXED_BYTES 114U
#define BTC_FPGA_MAX_REQUEST_PAYLOAD \
    (BTC_FPGA_REQUEST_FIXED_BYTES + BTC_FPGA_MAX_OUTPUT_BYTES)
#define BTC_FPGA_MAX_REQUEST_FRAME (7U + BTC_FPGA_MAX_REQUEST_PAYLOAD + 2U)
#define BTC_FPGA_SIGN_RESPONSE_BYTES 128U
#define BTC_FPGA_ETH_HASH_BYTES 32U
#define BTC_FPGA_ETH_RESPONSE_BYTES 65U
#define BTC_FPGA_MAX_RESPONSE_FRAME (8U + BTC_FPGA_SIGN_RESPONSE_BYTES + 2U)

enum btc_fpga_status {
    BTC_FPGA_STATUS_OK = 0x00,
    BTC_FPGA_STATUS_CRC_ERROR = 0x01,
    BTC_FPGA_STATUS_LENGTH_ERROR = 0x02,
    BTC_FPGA_STATUS_COMMAND_ERROR = 0x03,
    BTC_FPGA_STATUS_BUSY = 0x04,
    BTC_FPGA_STATUS_PRIVATE_KEY_ERROR = 0x05,
    BTC_FPGA_STATUS_BIP143_ERROR = 0x06,
    BTC_FPGA_STATUS_VERIFY_ERROR = 0x07
};

/* Multi-byte Bitcoin fields are already in their wire byte order. */
struct btc_fpga_bip143_request {
    uint8_t sequence;
    uint8_t freeze_id[32];
    uint8_t tx_version[4];
    uint8_t outpoint[36];
    uint8_t input_sequence[4];
    uint8_t pubkey_hash[20];
    uint8_t prevout_amount[8];
    const uint8_t *outputs;
    uint16_t outputs_len;
    uint8_t locktime[4];
    uint8_t sighash_type[4];
};

struct btc_fpga_signature_response {
    uint8_t sequence;
    uint8_t status;
    uint8_t freeze_id[32];
    uint8_t bip143_digest[32];
    uint8_t r[32];
    uint8_t s[32];
};

/* Recoverable Ethereum signature: y_parity || r || s. */
struct btc_fpga_eth_signature_response {
    uint8_t sequence;
    uint8_t status;
    uint8_t y_parity;
    uint8_t r[32];
    uint8_t s[32];
};

uint16_t btc_fpga_crc16_ccitt(const uint8_t *data, size_t len);

int btc_fpga_encode_bip143_request(
    const struct btc_fpga_bip143_request *request,
    uint8_t *frame, size_t frame_capacity, size_t *frame_len);

int btc_fpga_decode_signature_response(
    const uint8_t *frame, size_t frame_len,
    uint8_t expected_sequence, const uint8_t expected_freeze_id[32],
    struct btc_fpga_signature_response *response);

int btc_fpga_encode_eth_hash_request(
    uint8_t sequence, const uint8_t message_hash[BTC_FPGA_ETH_HASH_BYTES],
    uint8_t *frame, size_t frame_capacity, size_t *frame_len);

int btc_fpga_decode_eth_signature_response(
    const uint8_t *frame, size_t frame_len, uint8_t expected_sequence,
    struct btc_fpga_eth_signature_response *response);

#endif
