#ifndef WALLET_FPGA_PROTOCOL_H
#define WALLET_FPGA_PROTOCOL_H

#include <stddef.h>
#include <stdint.h>

#define WALLET_FPGA_VERSION             0x01U
#define WALLET_FPGA_CMD_PING            0x00U
#define WALLET_FPGA_CMD_SIGN_ETH_HASH   0x01U
#define WALLET_FPGA_CMD_SIGN_BIP143     0x10U

#define WALLET_FPGA_MAX_OUTPUT_BYTES    512U
#define WALLET_FPGA_MAX_REQUEST_FRAME   635U
#define WALLET_FPGA_MAX_RESPONSE_FRAME  138U

typedef enum {
    WALLET_FPGA_OK = 0,
    WALLET_FPGA_ERR_ARGUMENT = -1,
    WALLET_FPGA_ERR_FORMAT = -2,
    WALLET_FPGA_ERR_CRC = -3,
    WALLET_FPGA_ERR_SEQUENCE = -4,
    WALLET_FPGA_ERR_STATUS = -5
} WalletFpgaResult;

typedef struct {
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
} WalletBip143Request;

typedef struct {
    uint8_t status;
    uint8_t freeze_id[32];
    uint8_t digest[32];
    uint8_t r[32];
    uint8_t s[32];
} WalletBip143Signature;

typedef struct {
    uint8_t status;
    uint8_t y_parity;
    uint8_t r[32];
    uint8_t s[32];
} WalletEthSignature;

uint16_t WalletFpga_Crc16(const uint8_t *data, size_t length);
int WalletFpga_EncodePing(uint8_t sequence, uint8_t *frame,
                          size_t capacity, size_t *frame_length);
int WalletFpga_EncodeEth(uint8_t sequence, const uint8_t hash[32],
                         uint8_t *frame, size_t capacity,
                         size_t *frame_length);
int WalletFpga_DecodeEth(const uint8_t *frame, size_t frame_length,
                         uint8_t expected_sequence,
                         WalletEthSignature *signature);
int WalletFpga_EncodeBip143(const WalletBip143Request *request,
                            uint8_t *frame, size_t capacity,
                            size_t *frame_length);
int WalletFpga_DecodeBip143(const uint8_t *frame, size_t frame_length,
                            uint8_t expected_sequence,
                            const uint8_t expected_freeze_id[32],
                            WalletBip143Signature *signature);
int WalletFpga_ResponseLength(const uint8_t header[8], size_t *frame_length);

#endif
