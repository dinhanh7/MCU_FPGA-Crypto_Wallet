#include "wallet_fpga_link.h"

#include "init.h"
#include "hal_uart.h"

static UART_Handle_T fpga_uart;
static uint8_t fpga_sequence;
static uint8_t request_frame[WALLET_FPGA_MAX_REQUEST_FRAME];
static uint8_t response_frame[WALLET_FPGA_MAX_RESPONSE_FRAME];

int WalletFpgaLink_Init(void)
{
    __HAL_RCC_GPIO_P1_CLK_ENABLE();
    HAL_GPIO_SetAFIO(SN_GPIO1, GPIO_PIN_8, GPIO_P18_UART_URXD1);
    HAL_GPIO_SetAFIO(SN_GPIO1, GPIO_PIN_9, GPIO_P19_UART_UTXD1);
    __HAL_RCC_UART1_CLK_ENABLE();
    fpga_uart.instance = SN_UART1;
    fpga_uart.init.baud_rate = 115200U;
    fpga_uart.init.word_length = UART_WORD_LENGTH_8;
    fpga_uart.init.stop_bits = UART_STOP_BITS_1;
    fpga_uart.init.parity = UART_PARITY_NONE;
    fpga_uart.init.hw_flow_ctl = UART_HWCONTROL_NONE;
    fpga_sequence = 0U;
    return HAL_UART_Init(&fpga_uart) == HAL_OK ? 0 : -1;
}

static int transact(size_t request_length, uint8_t sequence, uint8_t command,
                    uint32_t timeout_ms, size_t *response_length)
{
    uint8_t byte;
    size_t total;
    uint32_t start;
    int sync_state = 0;
    if (HAL_UART_Transmit(&fpga_uart, request_frame, (uint16_t)request_length,
                          timeout_ms) != HAL_OK) return -1;

    /* Resynchronise instead of assuming that the next byte starts a frame. */
    start = HAL_GetTick();
    while ((HAL_GetTick() - start) <= timeout_ms) {
        if (HAL_UART_Receive(&fpga_uart, &byte, 1U, 2U) != HAL_OK) continue;
        if (sync_state == 0) sync_state = byte == 0x5aU ? 1 : 0;
        else if (byte == 0xa5U) { response_frame[0] = 0x5aU; response_frame[1] = 0xa5U; break; }
        else sync_state = byte == 0x5aU ? 1 : 0;
    }
    if (sync_state != 1 || response_frame[1] != 0xa5U) return -1;
    if (HAL_UART_Receive(&fpga_uart, response_frame + 2, 6U, timeout_ms) != HAL_OK)
        return -1;
    if (response_frame[3] != sequence || response_frame[4] != command) return -1;
    if (WalletFpga_ResponseLength(response_frame, &total) != WALLET_FPGA_OK)
        return -1;
    if (HAL_UART_Receive(&fpga_uart, response_frame + 8,
                         (uint16_t)(total - 8U), timeout_ms) != HAL_OK) return -1;
    *response_length = total;
    return 0;
}

int WalletFpgaLink_Ping(uint32_t timeout_ms)
{
    size_t request_length, response_length;
    uint8_t sequence = ++fpga_sequence;
    uint16_t crc;
    if (WalletFpga_EncodePing(sequence, request_frame, sizeof(request_frame),
                              &request_length) != WALLET_FPGA_OK) return -1;
    if (transact(request_length, sequence, WALLET_FPGA_CMD_PING,
                 timeout_ms, &response_length) != 0) return -1;
    if (response_length != 10U || response_frame[5] != 0U) return -1;
    crc = (uint16_t)(((uint16_t)response_frame[8] << 8) | response_frame[9]);
    return WalletFpga_Crc16(response_frame + 2, 6U) == crc ? 0 : -1;
}

int WalletFpgaLink_TransactRaw(const uint8_t *request, size_t request_length,
                               uint8_t *response, size_t response_capacity,
                               size_t *response_length, uint32_t timeout_ms)
{
    size_t received;
    uint16_t payload_length, received_crc;
    uint8_t sequence, command;
    if (request == NULL || response == NULL || response_length == NULL ||
        request_length < 9U || request_length > sizeof(request_frame)) return -1;
    if (request[0] != 0xa5U || request[1] != 0x5aU ||
        request[2] != WALLET_FPGA_VERSION) return -1;
    payload_length = (uint16_t)(((uint16_t)request[5] << 8) | request[6]);
    if (request_length != (size_t)payload_length + 9U) return -1;
    received_crc = (uint16_t)(((uint16_t)request[request_length - 2U] << 8) |
                              request[request_length - 1U]);
    if (WalletFpga_Crc16(request + 2, request_length - 4U) != received_crc)
        return -1;
    command = request[4];
    if (command != WALLET_FPGA_CMD_PING && command != WALLET_FPGA_CMD_SIGN_ETH_HASH &&
        command != WALLET_FPGA_CMD_SIGN_BIP143) return -1;
    sequence = request[3];
    memcpy(request_frame, request, request_length);
    if (transact(request_length, sequence, command, timeout_ms, &received) != 0)
        return -1;
    if (received > response_capacity) return -1;
    memcpy(response, response_frame, received);
    *response_length = received;
    return 0;
}

int WalletFpgaLink_SignEth(const uint8_t hash[32], WalletEthSignature *signature,
                           uint32_t timeout_ms)
{
    size_t request_length, response_length;
    uint8_t sequence = ++fpga_sequence;
    if (WalletFpga_EncodeEth(sequence, hash, request_frame, sizeof(request_frame),
                             &request_length) != WALLET_FPGA_OK) return -1;
    if (transact(request_length, sequence, WALLET_FPGA_CMD_SIGN_ETH_HASH,
                 timeout_ms, &response_length) != 0) return -1;
    return WalletFpga_DecodeEth(response_frame, response_length, sequence,
                                signature) == WALLET_FPGA_OK ? 0 : -1;
}

int WalletFpgaLink_SignBip143(const WalletBip143Request *request,
                              WalletBip143Signature *signature,
                              uint32_t timeout_ms)
{
    size_t request_length, response_length;
    WalletBip143Request local;
    uint8_t sequence = ++fpga_sequence;
    if (request == NULL) return -1;
    local = *request; local.sequence = sequence;
    if (WalletFpga_EncodeBip143(&local, request_frame, sizeof(request_frame),
                                &request_length) != WALLET_FPGA_OK) return -1;
    if (transact(request_length, sequence, WALLET_FPGA_CMD_SIGN_BIP143,
                 timeout_ms, &response_length) != 0) return -1;
    return WalletFpga_DecodeBip143(response_frame, response_length, sequence,
                                   local.freeze_id, signature) == WALLET_FPGA_OK ? 0 : -1;
}
