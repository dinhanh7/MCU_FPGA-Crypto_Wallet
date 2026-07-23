/***********************
 * SN34F78X SDK V2.1.4 *
 ***********************/

#ifndef _SN34F78X_HAL_UART_H_
#define _SN34F78X_HAL_UART_H_

/* Includes ------------------------------------------------------------------*/
#include "sn34f78x_hal.h"

/* private types -------------------------------------------------------------*/
/* private variables ---------------------------------------------------------*/
/* private constants ---------------------------------------------------------*/

/* UART Interrupt Identification */
#define UART_II_INTSTATUS_MASK (0x01)
#define UART_II_INTID_MASK     (0x0E)
#define UART_II_INTID_POS      (0x01)
#define UART_II_INTID_THRE     (0x01)
#define UART_II_INTID_RDA      (0x02)
#define UART_II_INTID_RLS      (0x03)
#define UART_II_INTID_CTI      (0x06)
#define UART_II_INTID_MS       (0x00)

/* UART Line Status */
#define UART_IS_RDR  (0x01)
#define UART_IS_OE   (0x02)
#define UART_IS_PE   (0x04)
#define UART_IS_FE   (0x08)
#define UART_IS_BI   (0x10)
#define UART_IS_THRE (0x20)

/* UART state_ext */
#define EX_STATE_NONE     (0)
#define EX_STATE_TX_ABORT (1U << 0)
#define EX_STATE_RX_ABORT (1U << 1)

/* UART FIFO Depth */
#define UART_FIFO_DEPTH_USAGE_TX (1U)
#define UART_FIFO_DEPTH_USAGE_RX (8U)

/* private macros ------------------------------------------------------------*/
#define IS_PERIPHERAL_UART_INSTANCE(__HANDLE__)   \
    (((SN_UART0_Type *)__HANDLE__) == SN_UART0 || \
     ((SN_UART0_Type *)__HANDLE__) == SN_UART1 || \
     ((SN_UART0_Type *)__HANDLE__) == SN_UART2 || \
     ((SN_UART0_Type *)__HANDLE__) == SN_UART3 || \
     ((SN_UART4_Type *)__HANDLE__) == SN_UART4 || \
     ((SN_UART4_Type *)__HANDLE__) == SN_UART5)

#define IS_UART_HWFLOW_INSTANCE(INSTANCE) (((INSTANCE) == SN_UART0) || \
                                           ((INSTANCE) == SN_UART1) || \
                                           ((INSTANCE) == SN_UART2) || \
                                           ((INSTANCE) == SN_UART3))

#define IS_UART_BAUD_RATE(BAUDRATE) (((BAUDRATE) >= UART_BAUD_RATE_MIN) || \
                                     ((BAUDRATE) <= UART_BAUD_RATE_MAX))

#define IS_UART_WORD_LENGTH(WORD) ((WORD) <= UART_WORD_LENGTH_8)
#define IS_UART_STOP_BITS(STOP)   ((STOP) <= UART_STOP_BITS_2)
#define IS_UART_PARITY_ODD(ODD)   ((ODD) <= UART_PARITY_ODD)

#define IS_UART_HWFLOW_CONTROL(CONTROL)    \
    (((CONTROL) == UART_HWCONTROL_NONE) || \
     ((CONTROL) == UART_HWCONTROL_RTS) ||  \
     ((CONTROL) == UART_HWCONTROL_CTS) ||  \
     ((CONTROL) == UART_HWCONTROL_RTS_CTS))

#define IS_UART_DMA_MODE(MODE)      \
    (((MODE) == UART_DMA_MODE_0) || \
     ((MODE) == UART_DMA_MODE_1) || \
     ((MODE) == UART_DMA_MODE_2))

#define IS_UART_TX_FIFO_LEVEL(LEVEL)   \
    (((LEVEL) == UART_TX_FIFO_TL_0) || \
     ((LEVEL) == UART_TX_FIFO_TL_1) || \
     ((LEVEL) == UART_TX_FIFO_TL_2) || \
     ((LEVEL) == UART_TX_FIFO_TL_3))

#define IS_UART_RX_FIFO_LEVEL(LEVEL)   \
    (((LEVEL) == UART_TX_FIFO_TL_0) || \
     ((LEVEL) == UART_TX_FIFO_TL_1) || \
     ((LEVEL) == UART_TX_FIFO_TL_2) || \
     ((LEVEL) == UART_TX_FIFO_TL_3))

// UART n FIFO Control register (UARTn_FIFOCTRL) (n=0,1,2,3,4,5)
#define UART_FIFOCTRL_FIFOEN      (1U << 0)
#define UART_FIFOCTRL_RXFIFORST   (1U << 1)
#define UART_FIFOCTRL_TXFIFORST   (1U << 2)
#define UART_FIFOCTRL_RXTL(level) (level << 6)

// UART n Interrupt Enable register (UARTn_IE) (n=0,1,2,3)
#define UART_IE_RDAIE  (1U << 0)
#define UART_IE_THREIE (1U << 1)
#define UART_IE_RLSIE  (1U << 2)
#define UART_IE_MSIE   (1U << 3)
#define UART_IE_RTSEN  (1U << 4)
#define UART_IE_CTSEN  (1U << 5)

// UART n Line Control register (UARTn_LC) (n=0,1,2,3,4,5)
#define UART_LC_WLS(length) (length << 0)
#define UART_LC_SBS(bit)    (bit << 2)
#define UART_LC_PE          (1U << 3)
#define UART_LC_PS(parity)  (parity << 4)
#define UART_LC_BC          (1U << 6)
#define UART_LC_DLAB        (1U << 7)

// UART n Line Status register (UARTn_LS) (n=0,1,2,3,4,5)
#define UART_LS_RDR  (1U << 0)
#define UART_LS_OE   (1U << 1)
#define UART_LS_PE   (1U << 2)
#define UART_LS_FE   (1U << 3)
#define UART_LS_BI   (1U << 4)
#define UART_LS_THRE (1U << 5)
#define UART_LS_TEMT (1U << 6)
#define UART_LS_RXFE (1U << 7)

/* Private functions prototypes ----------------------------------------------*/
static void         _uart_init_callbacks_default(UART_Handle_T *huart);
static uint8_t      _uart_get_rxtl(UART_Handle_T *huart);
static HAL_Status_T _uart_set_config(UART_Handle_T *huart);
static HAL_Status_T _uart_set_baudrate(UART_Handle_T *huart, uint32_t baud_rate);

static void _uart_rls_handle(UART_Handle_T *huart);
static void _uart_rda_handle(UART_Handle_T *huart);
static void _uart_thre_handle(UART_Handle_T *huart);
static void _uart_ms_handle(UART_Handle_T *huart);
static void _uart_transmit_complete(UART_Handle_T *huart);
static void _uart_receive_complete(UART_Handle_T *huart);
static void _uart_abort_tx(UART_Handle_T *huart);
static void _uart_abort_rx(UART_Handle_T *huart);

#if (configUSE_UART_NONBLOCKING_DMA == 1)
static HAL_Status_T _uart_dma_tx_check(UART_Handle_T *huart);
static HAL_Status_T _uart_dma_rx_check(UART_Handle_T *huart);
static void         _uart_dma_tx_cplt_callback(DMA_Handle_T *hdma);
static void         _uart_dma_tx_err_callback(DMA_Handle_T *hdma);
static void         _uart_dma_tx_abort_callback(DMA_Handle_T *hdma);
static void         _uart_dma_rx_cplt_callback(DMA_Handle_T *hdma);
static void         _uart_dma_rx_err_callback(DMA_Handle_T *hdma);
static void         _uart_dma_rx_abort_callback(DMA_Handle_T *hdma);
#endif /* configUSE_UART_NONBLOCKING_DMA */

#endif /* _SN34F78X_HAL_UART_H_ */
