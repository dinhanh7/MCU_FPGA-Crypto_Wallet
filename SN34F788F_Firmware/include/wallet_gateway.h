#ifndef WALLET_GATEWAY_H
#define WALLET_GATEWAY_H

#include <stdint.h>

typedef enum {
    WALLET_GATEWAY_IDLE = 0,
    WALLET_GATEWAY_PENDING_ETH,
    WALLET_GATEWAY_PENDING_BTC,
    WALLET_GATEWAY_FRAME_ERROR
} WalletGatewayState;

/* Board wiring: USB-UART TX -> P2.15 (UART2 RX), RX <- P2.14 (UART2 TX). */
int WalletGateway_Init(void);
void WalletGateway_Poll(void);
WalletGatewayState WalletGateway_State(void);
const uint8_t *WalletGateway_ReviewId(void);
int WalletGateway_Approve(void);
void WalletGateway_Reject(void);
void WalletGateway_ClearError(void);

#endif
