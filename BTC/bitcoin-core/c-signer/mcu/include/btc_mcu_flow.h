#ifndef BTC_MCU_FLOW_H
#define BTC_MCU_FLOW_H

#include <stddef.h>
#include <stdint.h>

#include "btc_fpga_link.h"

enum btc_mcu_decision {
    BTC_MCU_CANCEL = 0,
    BTC_MCU_APPROVE = 1
};

struct btc_mcu_review {
    const char *source_address;
    const char *recipient_address;
    const char *change_address;
    uint64_t input_sat;
    uint64_t recipient_sat;
    uint64_t change_sat;
    uint64_t fee_sat;
};

struct btc_mcu_platform {
    void *context;
    /* Return zero on success. lock_buffer may program the Cortex-M MPU. */
    int (*lock_buffer)(void *context, const uint8_t *buffer, size_t len);
    void (*unlock_buffer)(void *context);
    void (*sha256)(void *context, const uint8_t *data, size_t len,
                   uint8_t digest[32]);
    void (*display_review)(void *context, const struct btc_mcu_review *review,
                           const uint8_t freeze_id[32]);
    /* Verify locally; never forward PIN/passkey to FPGA. */
    int (*verify_passkey)(void *context, unsigned attempt);
    enum btc_mcu_decision (*wait_decision)(void *context);
    void (*secure_zero)(void *context, void *data, size_t len);
    struct btc_mcu_uart fpga_uart;
};

/*
 * The caller parses and validates the PSBT before entering this function.
 * On success, PSBT remains logically frozen until the caller inserts r/s.
 * On rejection/failure, the complete PSBT buffer is wiped.
 */
int btc_mcu_authorize_fpga_signature(
    const struct btc_mcu_platform *platform,
    uint8_t *psbt, size_t psbt_len,
    const struct btc_mcu_review *review,
    struct btc_fpga_bip143_request *fpga_request,
    struct btc_fpga_signature_response *signature);

void btc_mcu_release_frozen_buffer(const struct btc_mcu_platform *platform);

#endif
