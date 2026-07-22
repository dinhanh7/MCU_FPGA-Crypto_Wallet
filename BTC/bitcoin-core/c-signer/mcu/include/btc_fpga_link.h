#ifndef BTC_MCU_FPGA_LINK_H
#define BTC_MCU_FPGA_LINK_H

#include <stddef.h>
#include <stdint.h>

#include "../../src/fpga_protocol.h"

struct btc_mcu_uart {
    void *context;
    int (*write)(void *context, const uint8_t *data, size_t len,
                 uint32_t timeout_ms);
    int (*read)(void *context, uint8_t *data, size_t len,
                uint32_t timeout_ms);
    void (*flush)(void *context);
};

int btc_mcu_fpga_exchange(
    const struct btc_mcu_uart *uart,
    const struct btc_fpga_bip143_request *request,
    struct btc_fpga_signature_response *response,
    uint32_t timeout_ms);

#endif
