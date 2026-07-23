#include "wallet_fpga_protocol.h"

#include <assert.h>
#include <stdio.h>
#include <string.h>

static void finish_response(uint8_t *frame, size_t length)
{
    uint16_t crc = WalletFpga_Crc16(frame + 2, length - 4U);
    frame[length - 2U] = (uint8_t)(crc >> 8);
    frame[length - 1U] = (uint8_t)crc;
}

int main(void)
{
    uint8_t frame[WALLET_FPGA_MAX_REQUEST_FRAME];
    uint8_t hash[32] = {0};
    uint8_t response[138] = {0};
    uint8_t freeze[32] = {0};
    uint8_t outputs[512] = {0};
    size_t length = 0U;
    WalletEthSignature eth;
    WalletBip143Request btc;
    WalletBip143Signature btc_sig;

    assert(WalletFpga_Crc16((const uint8_t *)"123456789", 9U) == 0x29b1U);
    assert(WalletFpga_EncodePing(7U, frame, sizeof(frame), &length) == 0);
    assert(length == 9U && frame[0] == 0xa5U && frame[4] == 0U);
    assert(WalletFpga_EncodePing(7U, frame, 8U, &length) < 0);
    assert(WalletFpga_EncodeEth(9U, hash, frame, sizeof(frame), &length) == 0);
    assert(length == 41U && frame[5] == 0U && frame[6] == 32U);

    response[0] = 0x5aU; response[1] = 0xa5U; response[2] = 1U;
    response[3] = 9U; response[4] = WALLET_FPGA_CMD_SIGN_ETH_HASH;
    response[5] = 0U; response[6] = 0U; response[7] = 65U; response[8] = 1U;
    finish_response(response, 75U);
    assert(WalletFpga_DecodeEth(response, 75U, 9U, &eth) == 0);
    assert(eth.y_parity == 1U);
    response[74] ^= 1U;
    assert(WalletFpga_DecodeEth(response, 75U, 9U, &eth) == WALLET_FPGA_ERR_CRC);
    response[74] ^= 1U;
    assert(WalletFpga_DecodeEth(response, 75U, 8U, &eth) == WALLET_FPGA_ERR_SEQUENCE);

    memset(&btc, 0, sizeof(btc));
    btc.freeze_id[0] = 0x42U; btc.outputs = outputs; btc.outputs_len = 1U;
    assert(WalletFpga_EncodeBip143(&btc, frame, sizeof(frame), &length) == 0);
    assert(length == 124U);
    btc.outputs_len = 512U;
    assert(WalletFpga_EncodeBip143(&btc, frame, sizeof(frame), &length) == 0);
    assert(length == 635U);
    btc.outputs_len = 513U;
    assert(WalletFpga_EncodeBip143(&btc, frame, sizeof(frame), &length) < 0);
    btc.outputs_len = 0U;
    assert(WalletFpga_EncodeBip143(&btc, frame, sizeof(frame), &length) < 0);

    memset(response, 0, sizeof(response));
    response[0] = 0x5aU; response[1] = 0xa5U; response[2] = 1U;
    response[3] = 3U; response[4] = WALLET_FPGA_CMD_SIGN_BIP143;
    response[6] = 0U; response[7] = 128U; response[8] = 0x42U;
    freeze[0] = 0x42U;
    finish_response(response, sizeof(response));
    assert(WalletFpga_DecodeBip143(response, sizeof(response), 3U, freeze, &btc_sig) == 0);
    freeze[0] = 0x41U;
    assert(WalletFpga_DecodeBip143(response, sizeof(response), 3U, freeze, &btc_sig) == WALLET_FPGA_ERR_FORMAT);

    puts("wallet_fpga_protocol: all tests passed");
    return 0;
}
