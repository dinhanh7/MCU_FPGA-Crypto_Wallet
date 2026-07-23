#include "wallet_gateway.h"

#include "wallet_fpga_link.h"
#include "wallet_fpga_protocol.h"
#include "init.h"
#include "hal_uart.h"

#include <string.h>

static UART_Handle_T host_uart;
static uint8_t request[WALLET_FPGA_MAX_REQUEST_FRAME];
static uint8_t response[WALLET_FPGA_MAX_RESPONSE_FRAME];
static size_t request_used;
static size_t request_expected;
static WalletGatewayState gateway_state;

static void send_error_response(uint8_t status)
{
    uint16_t crc;
    if (request_used < 7U) return;
    response[0] = 0x5aU; response[1] = 0xa5U; response[2] = WALLET_FPGA_VERSION;
    response[3] = request[3]; response[4] = request[4]; response[5] = status;
    response[6] = 0U; response[7] = 0U;
    crc = WalletFpga_Crc16(response + 2, 6U);
    response[8] = (uint8_t)(crc >> 8); response[9] = (uint8_t)crc;
    (void)HAL_UART_Transmit(&host_uart, response, 10U, 1000U);
}

int WalletGateway_Init(void)
{
    __HAL_RCC_GPIO_P2_CLK_ENABLE();
    HAL_GPIO_SetAFIO(SN_GPIO2, GPIO_PIN_15, GPIO_P215_UART_URXD2);
    HAL_GPIO_SetAFIO(SN_GPIO2, GPIO_PIN_14, GPIO_P214_UART_UTXD2);
    __HAL_RCC_UART2_CLK_ENABLE();
    host_uart.instance = SN_UART2;
    host_uart.init.baud_rate = 115200U;
    host_uart.init.word_length = UART_WORD_LENGTH_8;
    host_uart.init.stop_bits = UART_STOP_BITS_1;
    host_uart.init.parity = UART_PARITY_NONE;
    host_uart.init.hw_flow_ctl = UART_HWCONTROL_NONE;
    request_used = 0U; request_expected = 0U; gateway_state = WALLET_GATEWAY_IDLE;
    return HAL_UART_Init(&host_uart) == HAL_OK ? 0 : -1;
}

static void consume(uint8_t byte)
{
    uint16_t payload, crc;
    if (request_used == 0U && byte != 0xa5U) return;
    if (request_used == 1U && byte != 0x5aU) {
        request_used = byte == 0xa5U ? 1U : 0U; return;
    }
    if (request_used >= sizeof(request)) { request_used = 0U; gateway_state = WALLET_GATEWAY_FRAME_ERROR; return; }
    request[request_used++] = byte;
    if (request_used == 7U) {
        payload = (uint16_t)(((uint16_t)request[5] << 8) | request[6]);
        request_expected = (size_t)payload + 9U;
        if (request_expected > sizeof(request)) {
            request_used = 0U; gateway_state = WALLET_GATEWAY_FRAME_ERROR; return;
        }
    }
    if (request_expected != 0U && request_used == request_expected) {
        crc = (uint16_t)(((uint16_t)request[request_used - 2U] << 8) |
                         request[request_used - 1U]);
        if (request[2] != WALLET_FPGA_VERSION ||
            WalletFpga_Crc16(request + 2, request_used - 4U) != crc) {
            request_used = 0U; request_expected = 0U;
            gateway_state = WALLET_GATEWAY_FRAME_ERROR; return;
        }
        if (request[4] == WALLET_FPGA_CMD_PING) {
            /* PING is non-sensitive and can pass without physical approval. */
            (void)WalletGateway_Approve();
        } else if (request[4] == WALLET_FPGA_CMD_SIGN_ETH_HASH && request[5] == 0U && request[6] == 32U) {
            gateway_state = WALLET_GATEWAY_PENDING_ETH;
        } else if (request[4] == WALLET_FPGA_CMD_SIGN_BIP143 && request_expected >= 123U) {
            gateway_state = WALLET_GATEWAY_PENDING_BTC;
        } else {
            request_used = 0U; request_expected = 0U; gateway_state = WALLET_GATEWAY_FRAME_ERROR;
        }
    }
}

void WalletGateway_Poll(void)
{
    uint8_t byte;
    if (gateway_state != WALLET_GATEWAY_IDLE) return;
    while (HAL_UART_Receive(&host_uart, &byte, 1U, 0U) == HAL_OK) consume(byte);
}

WalletGatewayState WalletGateway_State(void) { return gateway_state; }

const uint8_t *WalletGateway_ReviewId(void)
{
    if (gateway_state == WALLET_GATEWAY_PENDING_ETH) return request + 7;
    if (gateway_state == WALLET_GATEWAY_PENDING_BTC) return request + 7; /* freeze_id */
    return NULL;
}

int WalletGateway_Approve(void)
{
    size_t response_length;
    int result;
    if (request_used == 0U) return -1;
    result = WalletFpgaLink_TransactRaw(request, request_used, response,
                                        sizeof(response), &response_length, 10000U);
    if (result == 0)
        result = HAL_UART_Transmit(&host_uart, response, (uint16_t)response_length,
                                   1000U) == HAL_OK ? 0 : -1;
    memset(request, 0, request_used);
    request_used = 0U; request_expected = 0U; gateway_state = WALLET_GATEWAY_IDLE;
    return result;
}

void WalletGateway_Reject(void)
{
    /* FPGA status VERIFY_ERROR: fail fast without ever returning a signature. */
    send_error_response(0x07U);
    memset(request, 0, request_used);
    request_used = 0U; request_expected = 0U; gateway_state = WALLET_GATEWAY_IDLE;
}

void WalletGateway_ClearError(void)
{
    request_used = 0U; request_expected = 0U; gateway_state = WALLET_GATEWAY_IDLE;
}
