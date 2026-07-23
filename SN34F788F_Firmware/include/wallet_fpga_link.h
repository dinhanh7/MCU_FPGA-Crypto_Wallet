#ifndef WALLET_FPGA_LINK_H
#define WALLET_FPGA_LINK_H

#include <stdint.h>
#include <stddef.h>
#include "wallet_fpga_protocol.h"

/* Board wiring: FPGA TX -> P1.8 (UART1 RX), FPGA RX <- P1.9 (UART1 TX). */
int WalletFpgaLink_Init(void);
int WalletFpgaLink_Ping(uint32_t timeout_ms);
int WalletFpgaLink_TransactRaw(const uint8_t *request, size_t request_length,
                               uint8_t *response, size_t response_capacity,
                               size_t *response_length, uint32_t timeout_ms);
int WalletFpgaLink_SignEth(const uint8_t hash[32], WalletEthSignature *signature,
                           uint32_t timeout_ms);
int WalletFpgaLink_SignBip143(const WalletBip143Request *request,
                              WalletBip143Signature *signature,
                              uint32_t timeout_ms);

#endif
