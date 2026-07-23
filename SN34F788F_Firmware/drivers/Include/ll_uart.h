/***********************
 * SN34F78X SDK V2.1.4 *
 ***********************/

#ifndef _LL_UART_H_
#define _LL_UART_H_

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "sn34f78x_ll_def.h"
#include "ll_reg_msk.h"

/* Exported types ------------------------------------------------------------*/
/**
 * @brief LL UART Init Structure Definition
 * \ingroup uart_struct_define
 */
typedef struct
{
    uint32_t baud_rate;   /**< Specifies the UART communication speed.
                               This parameter can be a value in range of @ref uart_baud_rate_range */
    uint32_t word_length; /**< Specifies the number of data bits transmitted or received in a frame.
                               This parameter can be a value of @ref uart_word_length              */
    uint32_t stop_bits;   /**< Specifies the number of stop bits transmitted.
                               This parameter can be a value of @ref uart_stop_bits                */
    uint32_t parity;      /**< Specifies the parity mode.
                               This parameter can be a value of @ref uart_parity                   */
    uint32_t hw_flow_ctl; /**< Specifies whether the hardware flow control mode is enabled or disabled.
                               This parameter can be a value of @ref uart_hardware_flow            */
} LL_UART_Init_T;

/* Exported constants --------------------------------------------------------*/
/**
 * \defgroup uart_baud_rate_range UART Baud Rate Range
 * \ingroup uart_control
 * @{
 */
#define LL_UART_BAUD_RATE_MIN 110U     /**< Baud rate min */
#define LL_UART_BAUD_RATE_MAX 3000000U /**< Baud rate max */
/**
 * @}
 */

/**
 * \defgroup uart_word_length UART Word Length
 * \ingroup uart_control
 * @{
 */
#define LL_UART_WORD_LENGTH_5 (0x00U << UART_LC_WLS_Pos) /**< 5 Data bits */
#define LL_UART_WORD_LENGTH_6 (0x01U << UART_LC_WLS_Pos) /**< 6 Data bits */
#define LL_UART_WORD_LENGTH_7 (0x02U << UART_LC_WLS_Pos) /**< 7 Data bits */
#define LL_UART_WORD_LENGTH_8 (0x03U << UART_LC_WLS_Pos) /**< 8 Data bits */
/**
 * @}
 */

/**
 * \defgroup uart_stop_bits UART Stop Bits
 * \ingroup uart_control
 * @{
 */
#define LL_UART_STOP_BITS_1 (0x00U << UART_LC_SBS_Pos) /**< 1 Stop bit */
#define LL_UART_STOP_BITS_2 (0x01U << UART_LC_SBS_Pos) /**< 2 Stop bits (1.5 if Word Length == 5) */
/**
 * @}
 */

/**
 * \defgroup uart_parity UART Parity
 * \ingroup uart_control
 * @{
 */
#define LL_UART_PARITY_NONE (0x00000000U)                             /**< No parity */
#define LL_UART_PARITY_EVEN (((0x1U << UART_LC_PS_Pos)) | UART_LC_PE) /**< Even parity */
#define LL_UART_PARITY_ODD  (UART_LC_PE)                              /**< Odd parity */
/**
 * @}
 */

/**
 * \defgroup uart_hardware_flow UART Hardware Flow
 * \ingroup uart_control
 * @{
 */
#define LL_UART_HWCONTROL_NONE    0x00U /**< No Hardware Control */
#define LL_UART_HWCONTROL_RTS     0x01U /**< RTS Hardware Control */
#define LL_UART_HWCONTROL_CTS     0x02U /**< CTS Hardware Control */
#define LL_UART_HWCONTROL_RTS_CTS 0x03U /**< RTS and CTS Hardware Control */
/**
 * @}
 */

/**
 * \defgroup uart_Trigger_level_RX_FIFO_interrupt UART Trigger level of RX FIFO interrupt
 * \ingroup uart_control
 * @{
 */
#define LL_UART_RXTL_1  UART_FIFOCTRL_RXTL_1  /**< 1 character */
#define LL_UART_RXTL_4  UART_FIFOCTRL_RXTL_4  /**< 4 character */
#define LL_UART_RXTL_8  UART_FIFOCTRL_RXTL_8  /**< 8 character */
#define LL_UART_RXTL_14 UART_FIFOCTRL_RXTL_14 /**< 14 character */
/**
 * @}
 */

/**
 * \defgroup uart_RTSn_pin_output UART RTSn pin output
 * \ingroup uart_control
 * @{
 */
#define LL_UART_RTS_OUTPUT_HIGH 0x00U /**< RTSn pin output High (inactive) */
#define LL_UART_RTS_OUTPUT_LOW  0x01U /**< RTSn pin output Low (active) */
/**
 * @}
 */

/**
 * \defgroup uart_mode UART mode
 * \ingroup uart_control
 * @{
 */
#define LL_UART_MODE_UART     0x00U /**< UART mode */
#define LL_UART_MODE_IRDA_SIR 0x01U /**< IRDA SIR mode */
/**
 * @}
 */

/**
 * \defgroup uart_IRDA_SIR_pulse_invert UART IRDA SIR pulse invert
 * \ingroup uart_control
 * @{
 */
#define LL_UART_IRDA_SIR_PULSE_NOT_INVERT 0x00U /**< IRDA SIR pulse is not inverted */
#define LL_UART_IRDA_SIR_PULSE_INVERT     0x01U /**< IRDA SIR pulse is inverted */
/**
 * @}
 */

/**
 * \defgroup uart_IRDA_SIR_pulse_width UART IRDA SIR pulse width
 * \ingroup uart_control
 * @{
 */
#define LL_UART_IRDA_SIR_PULSE_WIDTH_16       0x00U /**< Pulse width = 3/16 */
#define LL_UART_IRDA_SIR_PULSE_WIDTH_PB16XCLK 0x01U /**< Pulse width = 3/ PB16XCLK (When PB16XCLK = 1.8432MHz, pulse width = 1.6us) */
/**
 * @}
 */

#if defined(USE_FULL_LL_DRIVER)
LL_Status_T LL_UART_Init(void *uart, LL_UART_Init_T *uart_init);
LL_Status_T LL_UART_DeInit(void *uart);
void        LL_UART_StructInit(LL_UART_Init_T *uart_init);
#endif /* USE_FULL_LL_DRIVER */

#ifdef __cplusplus
}
#endif

#endif /* _LL_UART_H_ */
