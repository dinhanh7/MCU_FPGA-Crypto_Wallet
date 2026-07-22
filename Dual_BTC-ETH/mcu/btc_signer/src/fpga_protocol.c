#include "fpga_protocol.h"

#include <string.h>

uint16_t btc_fpga_crc16_ccitt(const uint8_t *data, size_t len)
{
    uint16_t crc = 0xffffU;
    for (size_t i = 0; i < len; ++i) {
        crc ^= (uint16_t)data[i] << 8;
        for (unsigned bit = 0; bit < 8; ++bit)
            crc = (crc & 0x8000U) ? (uint16_t)((crc << 1) ^ 0x1021U)
                                  : (uint16_t)(crc << 1);
    }
    return crc;
}

static void put_bytes(uint8_t *dst, size_t *offset,
                      const uint8_t *src, size_t count)
{
    memcpy(dst + *offset, src, count);
    *offset += count;
}

int btc_fpga_encode_bip143_request(
    const struct btc_fpga_bip143_request *request,
    uint8_t *frame, size_t frame_capacity, size_t *frame_len)
{
    if (!request || !frame || !frame_len || !request->outputs ||
        request->outputs_len == 0 ||
        request->outputs_len > BTC_FPGA_MAX_OUTPUT_BYTES)
        return -1;

    const size_t payload_len = BTC_FPGA_REQUEST_FIXED_BYTES +
                               request->outputs_len;
    const size_t total_len = 7U + payload_len + 2U;
    if (frame_capacity < total_len) return -1;

    size_t off = 0;
    frame[off++] = 0xa5;
    frame[off++] = 0x5a;
    frame[off++] = BTC_FPGA_PROTOCOL_VERSION;
    frame[off++] = request->sequence;
    frame[off++] = BTC_FPGA_COMMAND_SIGN_BIP143;
    frame[off++] = (uint8_t)(payload_len >> 8);
    frame[off++] = (uint8_t)payload_len;
    put_bytes(frame, &off, request->freeze_id, 32);
    put_bytes(frame, &off, request->tx_version, 4);
    put_bytes(frame, &off, request->outpoint, 36);
    put_bytes(frame, &off, request->input_sequence, 4);
    put_bytes(frame, &off, request->pubkey_hash, 20);
    put_bytes(frame, &off, request->prevout_amount, 8);
    frame[off++] = (uint8_t)(request->outputs_len >> 8);
    frame[off++] = (uint8_t)request->outputs_len;
    put_bytes(frame, &off, request->outputs, request->outputs_len);
    put_bytes(frame, &off, request->locktime, 4);
    put_bytes(frame, &off, request->sighash_type, 4);

    if (off != 7U + payload_len) return -1;
    const uint16_t crc = btc_fpga_crc16_ccitt(frame + 2, off - 2);
    frame[off++] = (uint8_t)(crc >> 8);
    frame[off++] = (uint8_t)crc;
    *frame_len = off;
    return 0;
}

int btc_fpga_decode_signature_response(
    const uint8_t *frame, size_t frame_len,
    uint8_t expected_sequence, const uint8_t expected_freeze_id[32],
    struct btc_fpga_signature_response *response)
{
    if (!frame || !expected_freeze_id || !response || frame_len < 10U)
        return -1;
    if (frame[0] != 0x5a || frame[1] != 0xa5 ||
        frame[2] != BTC_FPGA_PROTOCOL_VERSION ||
        frame[3] != expected_sequence ||
        frame[4] != BTC_FPGA_COMMAND_SIGN_BIP143)
        return -1;

    const uint16_t payload_len = (uint16_t)((uint16_t)frame[6] << 8) |
                                 frame[7];
    if (frame_len != 8U + payload_len + 2U) return -1;
    const uint16_t received_crc = (uint16_t)((uint16_t)frame[frame_len - 2] << 8) |
                                  frame[frame_len - 1];
    if (btc_fpga_crc16_ccitt(frame + 2, frame_len - 4) != received_crc)
        return -1;

    memset(response, 0, sizeof(*response));
    response->sequence = frame[3];
    response->status = frame[5];
    if (response->status != BTC_FPGA_STATUS_OK)
        return payload_len == 0 ? 1 : -1;
    if (payload_len != BTC_FPGA_SIGN_RESPONSE_BYTES) return -1;

    memcpy(response->freeze_id, frame + 8, 32);
    memcpy(response->bip143_digest, frame + 40, 32);
    memcpy(response->r, frame + 72, 32);
    memcpy(response->s, frame + 104, 32);
    if (memcmp(response->freeze_id, expected_freeze_id, 32) != 0)
        return -1;
    return 0;
}

int btc_fpga_encode_eth_hash_request(
    uint8_t sequence, const uint8_t message_hash[BTC_FPGA_ETH_HASH_BYTES],
    uint8_t *frame, size_t frame_capacity, size_t *frame_len)
{
    const size_t payload_len = BTC_FPGA_ETH_HASH_BYTES;
    const size_t total_len = 7U + payload_len + 2U;
    if (!message_hash || !frame || !frame_len || frame_capacity < total_len)
        return -1;

    size_t off = 0;
    frame[off++] = 0xa5;
    frame[off++] = 0x5a;
    frame[off++] = BTC_FPGA_PROTOCOL_VERSION;
    frame[off++] = sequence;
    frame[off++] = BTC_FPGA_COMMAND_SIGN_ETH_HASH;
    frame[off++] = 0;
    frame[off++] = BTC_FPGA_ETH_HASH_BYTES;
    put_bytes(frame, &off, message_hash, BTC_FPGA_ETH_HASH_BYTES);
    const uint16_t crc = btc_fpga_crc16_ccitt(frame + 2, off - 2);
    frame[off++] = (uint8_t)(crc >> 8);
    frame[off++] = (uint8_t)crc;
    *frame_len = off;
    return 0;
}

int btc_fpga_decode_eth_signature_response(
    const uint8_t *frame, size_t frame_len, uint8_t expected_sequence,
    struct btc_fpga_eth_signature_response *response)
{
    if (!frame || !response || frame_len < 10U)
        return -1;
    if (frame[0] != 0x5a || frame[1] != 0xa5 ||
        frame[2] != BTC_FPGA_PROTOCOL_VERSION ||
        frame[3] != expected_sequence ||
        frame[4] != BTC_FPGA_COMMAND_SIGN_ETH_HASH)
        return -1;

    const uint16_t payload_len = (uint16_t)((uint16_t)frame[6] << 8) |
                                 frame[7];
    if (frame_len != 8U + payload_len + 2U)
        return -1;
    const uint16_t received_crc =
        (uint16_t)((uint16_t)frame[frame_len - 2] << 8) |
        frame[frame_len - 1];
    if (btc_fpga_crc16_ccitt(frame + 2, frame_len - 4) != received_crc)
        return -1;

    memset(response, 0, sizeof(*response));
    response->sequence = frame[3];
    response->status = frame[5];
    if (response->status != BTC_FPGA_STATUS_OK)
        return payload_len == 0U ? 1 : -1;
    if (payload_len != BTC_FPGA_ETH_RESPONSE_BYTES || frame[8] > 1U)
        return -1;
    response->y_parity = frame[8];
    memcpy(response->r, frame + 9, 32);
    memcpy(response->s, frame + 41, 32);
    return 0;
}
