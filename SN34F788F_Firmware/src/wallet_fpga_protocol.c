#include "wallet_fpga_protocol.h"

#include <string.h>

uint16_t WalletFpga_Crc16(const uint8_t *data, size_t length)
{
    uint16_t crc = 0xffffU;
    size_t i;
    unsigned bit;
    if (data == NULL && length != 0U) return 0U;
    for (i = 0; i < length; ++i) {
        crc ^= (uint16_t)data[i] << 8;
        for (bit = 0; bit < 8U; ++bit) {
            crc = (crc & 0x8000U) != 0U
                ? (uint16_t)((crc << 1) ^ 0x1021U)
                : (uint16_t)(crc << 1);
        }
    }
    return crc;
}

static int encode_simple(uint8_t sequence, uint8_t command,
                         const uint8_t *payload, uint16_t payload_length,
                         uint8_t *frame, size_t capacity, size_t *frame_length)
{
    size_t total;
    uint16_t crc;
    if (frame == NULL || frame_length == NULL ||
        (payload == NULL && payload_length != 0U)) return WALLET_FPGA_ERR_ARGUMENT;
    total = 9U + payload_length;
    if (capacity < total) return WALLET_FPGA_ERR_ARGUMENT;
    frame[0] = 0xa5U; frame[1] = 0x5aU;
    frame[2] = WALLET_FPGA_VERSION; frame[3] = sequence; frame[4] = command;
    frame[5] = (uint8_t)(payload_length >> 8); frame[6] = (uint8_t)payload_length;
    if (payload_length != 0U) memcpy(frame + 7, payload, payload_length);
    crc = WalletFpga_Crc16(frame + 2, 5U + payload_length);
    frame[7U + payload_length] = (uint8_t)(crc >> 8);
    frame[8U + payload_length] = (uint8_t)crc;
    *frame_length = total;
    return WALLET_FPGA_OK;
}

int WalletFpga_EncodePing(uint8_t sequence, uint8_t *frame,
                          size_t capacity, size_t *frame_length)
{
    return encode_simple(sequence, WALLET_FPGA_CMD_PING, NULL, 0U,
                         frame, capacity, frame_length);
}

int WalletFpga_EncodeEth(uint8_t sequence, const uint8_t hash[32],
                         uint8_t *frame, size_t capacity, size_t *frame_length)
{
    if (hash == NULL) return WALLET_FPGA_ERR_ARGUMENT;
    return encode_simple(sequence, WALLET_FPGA_CMD_SIGN_ETH_HASH, hash, 32U,
                         frame, capacity, frame_length);
}

static int validate_response(const uint8_t *frame, size_t length,
                             uint8_t sequence, uint8_t command,
                             uint16_t *payload_length)
{
    uint16_t payload;
    uint16_t received_crc;
    if (frame == NULL || payload_length == NULL || length < 10U)
        return WALLET_FPGA_ERR_ARGUMENT;
    if (frame[0] != 0x5aU || frame[1] != 0xa5U ||
        frame[2] != WALLET_FPGA_VERSION || frame[4] != command)
        return WALLET_FPGA_ERR_FORMAT;
    if (frame[3] != sequence) return WALLET_FPGA_ERR_SEQUENCE;
    payload = (uint16_t)(((uint16_t)frame[6] << 8) | frame[7]);
    if (length != 10U + payload) return WALLET_FPGA_ERR_FORMAT;
    received_crc = (uint16_t)(((uint16_t)frame[length - 2U] << 8) |
                              frame[length - 1U]);
    if (WalletFpga_Crc16(frame + 2, length - 4U) != received_crc)
        return WALLET_FPGA_ERR_CRC;
    *payload_length = payload;
    if (frame[5] != 0U) return WALLET_FPGA_ERR_STATUS;
    return WALLET_FPGA_OK;
}

int WalletFpga_DecodeEth(const uint8_t *frame, size_t frame_length,
                         uint8_t expected_sequence,
                         WalletEthSignature *signature)
{
    uint16_t payload;
    int result;
    if (signature == NULL) return WALLET_FPGA_ERR_ARGUMENT;
    memset(signature, 0, sizeof(*signature));
    result = validate_response(frame, frame_length, expected_sequence,
                               WALLET_FPGA_CMD_SIGN_ETH_HASH, &payload);
    if (frame != NULL && frame_length > 5U) signature->status = frame[5];
    if (result != WALLET_FPGA_OK) return result;
    if (payload != 65U || frame[8] > 1U) return WALLET_FPGA_ERR_FORMAT;
    signature->y_parity = frame[8];
    memcpy(signature->r, frame + 9, 32U);
    memcpy(signature->s, frame + 41, 32U);
    return WALLET_FPGA_OK;
}

int WalletFpga_EncodeBip143(const WalletBip143Request *r, uint8_t *frame,
                            size_t capacity, size_t *frame_length)
{
    uint8_t payload[114U + WALLET_FPGA_MAX_OUTPUT_BYTES];
    size_t off = 0U;
    if (r == NULL || r->outputs == NULL || r->outputs_len == 0U ||
        r->outputs_len > WALLET_FPGA_MAX_OUTPUT_BYTES)
        return WALLET_FPGA_ERR_ARGUMENT;
#define PUT(field, count) do { memcpy(payload + off, (field), (count)); off += (count); } while (0)
    PUT(r->freeze_id, 32U); PUT(r->tx_version, 4U); PUT(r->outpoint, 36U);
    PUT(r->input_sequence, 4U); PUT(r->pubkey_hash, 20U);
    PUT(r->prevout_amount, 8U);
    payload[off++] = (uint8_t)(r->outputs_len >> 8);
    payload[off++] = (uint8_t)r->outputs_len;
    PUT(r->outputs, r->outputs_len); PUT(r->locktime, 4U); PUT(r->sighash_type, 4U);
#undef PUT
    return encode_simple(r->sequence, WALLET_FPGA_CMD_SIGN_BIP143,
                         payload, (uint16_t)off, frame, capacity, frame_length);
}

int WalletFpga_DecodeBip143(const uint8_t *frame, size_t frame_length,
                            uint8_t expected_sequence,
                            const uint8_t expected_freeze_id[32],
                            WalletBip143Signature *signature)
{
    uint16_t payload;
    int result;
    if (expected_freeze_id == NULL || signature == NULL)
        return WALLET_FPGA_ERR_ARGUMENT;
    memset(signature, 0, sizeof(*signature));
    result = validate_response(frame, frame_length, expected_sequence,
                               WALLET_FPGA_CMD_SIGN_BIP143, &payload);
    if (frame != NULL && frame_length > 5U) signature->status = frame[5];
    if (result != WALLET_FPGA_OK) return result;
    if (payload != 128U) return WALLET_FPGA_ERR_FORMAT;
    memcpy(signature->freeze_id, frame + 8, 32U);
    memcpy(signature->digest, frame + 40, 32U);
    memcpy(signature->r, frame + 72, 32U);
    memcpy(signature->s, frame + 104, 32U);
    if (memcmp(signature->freeze_id, expected_freeze_id, 32U) != 0)
        return WALLET_FPGA_ERR_FORMAT;
    return WALLET_FPGA_OK;
}

int WalletFpga_ResponseLength(const uint8_t header[8], size_t *frame_length)
{
    uint16_t payload;
    if (header == NULL || frame_length == NULL) return WALLET_FPGA_ERR_ARGUMENT;
    if (header[0] != 0x5aU || header[1] != 0xa5U ||
        header[2] != WALLET_FPGA_VERSION) return WALLET_FPGA_ERR_FORMAT;
    payload = (uint16_t)(((uint16_t)header[6] << 8) | header[7]);
    if ((size_t)payload + 10U > WALLET_FPGA_MAX_RESPONSE_FRAME)
        return WALLET_FPGA_ERR_FORMAT;
    *frame_length = (size_t)payload + 10U;
    return WALLET_FPGA_OK;
}
