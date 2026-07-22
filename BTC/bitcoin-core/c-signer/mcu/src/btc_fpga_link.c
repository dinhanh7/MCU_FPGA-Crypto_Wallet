#include "../include/btc_fpga_link.h"

#include <string.h>

static void secure_zero(void *memory, size_t length)
{
    volatile uint8_t *p = (volatile uint8_t *)memory;
    while (length--) *p++ = 0;
}

int btc_mcu_fpga_exchange(
    const struct btc_mcu_uart *uart,
    const struct btc_fpga_bip143_request *request,
    struct btc_fpga_signature_response *response,
    uint32_t timeout_ms)
{
    uint8_t tx[BTC_FPGA_MAX_REQUEST_FRAME];
    uint8_t rx[BTC_FPGA_MAX_RESPONSE_FRAME];
    size_t tx_len = 0;
    int result = -1;

    if (!uart || !uart->write || !uart->read || !request || !response)
        return -1;
    if (btc_fpga_encode_bip143_request(request, tx, sizeof(tx), &tx_len) != 0)
        goto out;
    if (uart->flush) uart->flush(uart->context);
    if (uart->write(uart->context, tx, tx_len, timeout_ms) != 0)
        goto out;
    if (uart->read(uart->context, rx, 8, timeout_ms) != 0)
        goto out;
    size_t payload_len = ((size_t)rx[6] << 8) | rx[7];
    if (payload_len > BTC_FPGA_SIGN_RESPONSE_BYTES)
        goto out;
    if (uart->read(uart->context, rx + 8, payload_len + 2U, timeout_ms) != 0)
        goto out;
    result = btc_fpga_decode_signature_response(
        rx, 8U + payload_len + 2U, request->sequence,
        request->freeze_id, response);
out:
    secure_zero(tx, sizeof(tx));
    secure_zero(rx, sizeof(rx));
    return result;
}
