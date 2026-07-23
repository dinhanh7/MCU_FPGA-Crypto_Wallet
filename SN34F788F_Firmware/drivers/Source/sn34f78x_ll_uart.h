/***********************
 * SN34F78X SDK V2.1.4 *
 ***********************/

#ifndef _SN34F78X_LL_UART_H_
#define _SN34F78X_LL_UART_H_

/* Includes ------------------------------------------------------------------*/
#include "sn34f78x_ll.h"

/* Private types -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Private constants ---------------------------------------------------------*/
/* UART Interrupt Identification */
#define LL_UART_II_INTSTATUS_MASK (0x01)
#define LL_UART_II_INTID_MASK     (0x0E)
#define LL_UART_II_INTID_POS      (0x01)
#define LL_UART_II_INTID_THRE     (0x01)
#define LL_UART_II_INTID_RDA      (0x02)
#define LL_UART_II_INTID_RLS      (0x03)
#define LL_UART_II_INTID_CTI      (0x06)
#define LL_UART_II_INTID_MS       (0x00)

/* Private macros ------------------------------------------------------------*/
#define IS_LL_UART_INSTANCE(INSTANCE) (((INSTANCE) == SN_UART0) || \
                                       ((INSTANCE) == SN_UART1) || \
                                       ((INSTANCE) == SN_UART2) || \
                                       ((INSTANCE) == SN_UART3) || \
                                       ((INSTANCE) == SN_UART4) || \
                                       ((INSTANCE) == SN_UART5))

#define IS_LL_UART_BAUD_RATE(BAUDRATE) (((BAUDRATE) >= LL_UART_BAUD_RATE_MIN) || \
                                        ((BAUDRATE) <= LL_UART_BAUD_RATE_MAX))

#define IS_LL_UART_WORD_LENGTH(__VALUE__) (((__VALUE__) == LL_UART_WORD_LENGTH_5) || \
                                           ((__VALUE__) == LL_UART_WORD_LENGTH_6) || \
                                           ((__VALUE__) == LL_UART_WORD_LENGTH_7) || \
                                           ((__VALUE__) == LL_UART_WORD_LENGTH_8))

#define IS_LL_UART_STOP_BITS(__VALUE__) (((__VALUE__) == LL_UART_STOP_BITS_1) || \
                                         ((__VALUE__) == LL_UART_STOP_BITS_2))

#define IS_LL_UART_PARITY(__VALUE__) (((__VALUE__) == LL_UART_PARITY_NONE) || \
                                      ((__VALUE__) == LL_UART_PARITY_EVEN) || \
                                      ((__VALUE__) == LL_UART_PARITY_ODD))

#define IS_LL_UART_HWCONTROL(__VALUE__) (((__VALUE__) == LL_UART_HWCONTROL_NONE) || \
                                         ((__VALUE__) == LL_UART_HWCONTROL_RTS) ||  \
                                         ((__VALUE__) == LL_UART_HWCONTROL_CTS) ||  \
                                         ((__VALUE__) == LL_UART_HWCONTROL_RTS_CTS))

/* Private functions prototypes ----------------------------------------------*/

#endif /* _SN34F78X_LL_UART_H_ */
