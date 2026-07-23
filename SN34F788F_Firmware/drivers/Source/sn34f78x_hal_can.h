/***********************
 * SN34F78X SDK V2.1.4 *
 ***********************/

#ifndef _SN34F78X_HAL_CAN_H_
#define _SN34F78X_HAL_CAN_H_
#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "sn34f78x_hal.h"

/* Private types -------------------------------------------------------------*/

/* Private macros ------------------------------------------------------------*/

#define HAL_CAN_FLAG_GEQU(flag1, flag2) ((flag1) >= (flag2)) /**< 'flag1' >= 'flag2' */
#define HAL_CAN_FLAG_LEQU(flag1, flag2) ((flag1) <= (flag2)) /**< 'flag1' <= 'flag2' */

/**
 * @brief  is can instance?
 * @param  __HANDLE__ specifies the CAN Handle.
 */
#define IS_CAN_INSTANCE(__HANDLE__) (((__HANDLE__)->instance) == SN_CAN0 || \
                                     ((__HANDLE__)->instance) == SN_CAN1)

/**
 * @brief  is can valid transmission buffer type?
 * @param  __TYPE__ specifies the CAN Transmission buffer type.
 */
#define IS_CAN_PTB_STB(__TYPE__) (HAL_FLAG_EQU(__TYPE__, CAN_MAILBOX_PTB) || \
                                  HAL_FLAG_EQU(__TYPE__, CAN_MAILBOX_STB))

/**
 * @brief  is can valid fillter scale type?
 * @param  __TYPE__ specifies the CAN Fillter scale type.
 */
#define IS_CAN_FILLTER_SCALE(__TYPE__) (HAL_FLAG_EQU(__TYPE__, CAN_FILTER_SCALE_STD) || \
                                        HAL_FLAG_EQU(__TYPE__, CAN_FILTER_SCALE_EXT) || \
                                        HAL_FLAG_EQU(__TYPE__, CAN_FILTER_SCALE_BOTH))

/**
 * @brief  is can valid timing setting?
 * @param  __TYPE__ specifies the CAN Timing setting.
 */
#define IS_CAN_TIMING_CFG(__PRESCALER__, __SEG1__, __SEG2__, __SJW__) \
    (HAL_CAN_FLAG_LEQU(__PRESCALER__, CAN_TIMING_PRESCALER_MAX) &&    \
     HAL_CAN_FLAG_GEQU(__SEG1__, CAN_TIMING_SEG1_MIN) &&              \
     HAL_CAN_FLAG_LEQU(__SEG1__, CAN_TIMING_SEG1_MAX) &&              \
     HAL_CAN_FLAG_LEQU(__SEG2__, CAN_TIMING_SEG2_MAX) &&              \
     HAL_CAN_FLAG_LEQU(__SJW__, CAN_TIMING_SJW_MAX) &&                \
     HAL_CAN_FLAG_GEQU(__SEG1__, __SEG2__ + 1) &&                     \
     HAL_CAN_FLAG_GEQU(__SEG2__, __SJW__))

/**
 * @brief  default cb
 * @param  __HCB__ cb pointer in the CAN Handle
 * @param  __PCB__ cb function
 */
#define CAN_DEFAULT_CB(__HCB__, __PCB__) \
    do                                   \
    {                                    \
        if (__HCB__ == NULL)             \
        {                                \
            __HCB__ = __PCB__;           \
        }                                \
    } while (0)

/**
 * @brief  reset cb
 * @param  __HCB__ cb pointer in the CAN Handle
 * @param  __PCB__ cb function
 */
#define CAN_RESET_CB(__HCB__, __PCB__) \
    do                                 \
    {                                  \
        if (__HCB__ != NULL)           \
        {                              \
            __HCB__ = __PCB__;         \
        }                              \
    } while (0)

#define CAN_FLAG_C1R(REG, KEEP_MASK, CLR_MASK) (REG = ((REG) & (KEEP_MASK)) | (CLR_MASK))

/**
 * @brief  Check whether the specified CAN flag is set or not.
 * @param  __HANDLE__ specifies the CAN Handle.
 * @param  __FLAG__ specifies the flag to check.
 */
#define CAN_FLAG_EPIF                       (1U << 20) // Busy flag
#define CAN_FLAG_ALIF                       (1U << 18) // Data in TX FIFO ≤ TXFIFOTH
#define CAN_FLAG_BEIF                       (1U << 16) // Data in RX FIFO >= RXFIFOTH
#define CAN_FLAG_RIF                        (1U << 15) // Data in RX FIFO >= RXFIFOTH
#define CAN_FLAG_ROIF                       (1U << 14) // Data in RX FIFO >= RXFIFOTH
#define CAN_FLAG_RFIF                       (1U << 13) // Data in RX FIFO >= RXFIFOTH
#define CAN_FLAG_RAFIF                      (1U << 12) // Data in RX FIFO >= RXFIFOTH
#define CAN_FLAG_TPIF                       (1U << 11) // Data in RX FIFO >= RXFIFOTH
#define CAN_FLAG_TSIF                       (1U << 10) // Data in RX FIFO >= RXFIFOTH
#define CAN_FLAG_EIF                        (1U << 9)  // Data in RX FIFO >= RXFIFOTH
#define CAN_FLAG_AIF                        (1U << 8)  // Data in RX FIFO >= RXFIFOTH
#define CAN_FLAG_TSFF                       (1U << 0)  // read only
#define CAN_FLAG_ALL                        (CAN_FLAG_EPIF | CAN_FLAG_ALIF | CAN_FLAG_BEIF | CAN_FLAG_RIF | CAN_FLAG_ROIF | CAN_FLAG_RFIF | CAN_FLAG_RAFIF | CAN_FLAG_TPIF | CAN_FLAG_TSIF | CAN_FLAG_EIF | CAN_FLAG_AIF | CAN_FLAG_TSFF)
#define CAN_FLAG_HAS(__HANDLE__, __FLAG__)  (HAL_FLAG_HAS(((__HANDLE__)->instance->INTCFG), (__FLAG__)))
#define CAN_FLAG_LACK(__HANDLE__, __FLAG__) (HAL_FLAG_LACKS(((__HANDLE__)->instance->INTCFG), (__FLAG__)))
#define CAN_FLAG_CLR(__HANDLE__, __FLAG__)  (CAN_FLAG_C1R(((__HANDLE__)->instance->INTCFG), ~CAN_FLAG_ALL, __FLAG__))

/**
 * @brief  Check whether the specified TTCAN flag is set or not.
 * @param  __HANDLE__ specifies the CAN Handle.
 * @param  __FLAG__ specifies the flag to check.
 */
#define TTCAN_FLAG_WTIF                       (1U << 30) // Busy flag
#define TTCAN_FLAG_TEIF                       (1U << 29) // Data in TX FIFO ≤ TXFIFOTH
#define TTCAN_FLAG_TTIF                       (1U << 27) // Data in RX FIFO >= RXFIFOTH
#define TTCAN_FLAG_ALL                        (TTCAN_FLAG_WTIF | TTCAN_FLAG_TEIF | TTCAN_FLAG_TTIF)
#define TTCAN_FLAG_HAS(__HANDLE__, __FLAG__)  (HAL_FLAG_HAS(((__HANDLE__)->instance->TTCTRL), (__FLAG__)))
#define TTCAN_FLAG_LACK(__HANDLE__, __FLAG__) (HAL_FLAG_LACKS(((__HANDLE__)->instance->TTCTRL), (__FLAG__)))
#define TTCAN_FLAG_CLR(__HANDLE__, __FLAG__)  (CAN_FLAG_C1R(((__HANDLE__)->instance->INTCFG), ~TTCAN_FLAG_ALL, __FLAG__))

/**
 * @brief  Enable/Disable the specified CAN interrupts.
 * @param  __HANDLE__ specifies the CAN Handle.
 * @param  __INTERRUPT__ specifies the interrupt source to enable or disable.
 */
#define CAN_IE_AFWL_POS                           (28U)      // Receive buffer Almost Full Warning Limit
#define CAN_IE_EWL_POS                            (24U)      // Programmable Error Warning Limit = (EWL+1)*8
#define CAN_IE_EPIE                               (1U << 21) // TX Data Complete Interrupt Enable
#define CAN_IE_ALIE                               (1U << 19) // TX dma interrupt enable
#define CAN_IE_BEIE                               (1U << 17) // TX dma interrupt enable
#define CAN_IE_RIE                                (1U << 7)  // TX FIFO threshold interrupt enable
#define CAN_IE_ROIE                               (1U << 6)  // RX FIFO threshold interrupt enable
#define CAN_IE_RFIE                               (1U << 5)  // TX Underflow interrupt enable
#define CAN_IE_RAFIE                              (1U << 4)  // TX Overflow interrupt enable
#define CAN_IE_TPIE                               (1U << 3)  // TX Overflow interrupt enable
#define CAN_IE_TSIE                               (1U << 2)  // TX Overflow interrupt enable
#define CAN_IE_EIE                                (1U << 1)  // TX Overflow interrupt enable
#define CAN_IE_ALL                                (CAN_IE_EIE | CAN_IE_EPIE | CAN_IE_ALIE | CAN_IE_BEIE | CAN_IE_RIE | CAN_IE_ROIE | CAN_IE_RFIE | CAN_IE_RAFIE | CAN_IE_TPIE | CAN_IE_TSIE)
#define CAN_IE_INIT                               (CAN_IE_EIE | CAN_IE_RIE | CAN_IE_ROIE | CAN_IE_RFIE | CAN_IE_RAFIE | CAN_IE_TPIE | CAN_IE_TSIE)
#define CAN_ENABLE_IE(__HANDLE__, __INTERRUPT__)  (HAL_FLAG_SET((__HANDLE__)->instance->INTCFG, (__INTERRUPT__)))
#define CAN_DISABLE_IE(__HANDLE__, __INTERRUPT__) (HAL_FLAG_CLR((__HANDLE__)->instance->INTCFG, (__INTERRUPT__)))
#define CAN_IE_HAS(__HANDLE__, __INTERRUPT__)     (HAL_FLAG_HAS((__HANDLE__)->instance->INTCFG, (__INTERRUPT__)))
#define CAN_IE_LACK(__HANDLE__, __INTERRUPT__)    (HAL_FLAG_LACKS((__HANDLE__)->instance->INTCFG, (__INTERRUPT__)))

/**
 * @brief  Enable/Disable the specified CAN clock.
 * @param  __HANDLE__ specifies the CAN Handle.
 */
#define CAN_ENABLE_CLK(__HANDLE__)                    \
    do                                                \
    {                                                 \
        if (((__HANDLE__)->instance) == SN_CAN0)      \
        {                                             \
            SN_SCU->APB0CLKG_b.CAN0CLKEN = 1U;        \
        }                                             \
        else if (((__HANDLE__)->instance) == SN_CAN1) \
        {                                             \
            SN_SCU->APB0CLKG_b.CAN1CLKEN = 1U;        \
        }                                             \
    } while (0)

#define CAN_DISABLE_CLK(__HANDLE__)                   \
    do                                                \
    {                                                 \
        if (((__HANDLE__)->instance) == SN_CAN0)      \
        {                                             \
            SN_SCU->APB0CLKG_b.CAN0CLKEN = 0U;        \
        }                                             \
        else if (((__HANDLE__)->instance) == SN_CAN1) \
        {                                             \
            SN_SCU->APB0CLKG_b.CAN1CLKEN = 0U;        \
        }                                             \
    } while (0)

/**
 * @brief  Enable/Disable the specified CAN interrupts.
 * @param  __HANDLE__ specifies the CAN Handle.
 * @param  __INTERRUPT__ specifies the interrupt source to enable or disable.
 */
#define TTCAN_IE_WTIE                               (1U << 31) // TX Data Complete Interrupt Enable
#define TTCAN_IE_TTIE                               (1U << 28) // TX dma interrupt enable
#define TTCAN_IE_ALL                                (TTCAN_IE_WTIE | TTCAN_IE_TTIE)
#define TTCAN_ENABLE_IE(__HANDLE__, __INTERRUPT__)  (HAL_FLAG_SET((__HANDLE__)->instance->TTCTRL, (__INTERRUPT__)))
#define TTCAN_DISABLE_IE(__HANDLE__, __INTERRUPT__) (HAL_FLAG_CLR((__HANDLE__)->instance->TTCTRL, (__INTERRUPT__)))
#define TTCAN_IE_HAS(__HANDLE__, __INTERRUPT__)     (HAL_FLAG_HAS((__HANDLE__)->instance->TTCTRL, (__INTERRUPT__)))
#define TTCAN_IE_LACK(__HANDLE__, __INTERRUPT__)    (HAL_FLAG_LACKS((__HANDLE__)->instance->TTCTRL, (__INTERRUPT__)))

/**
 * @brief  can inner state.
 * @param  __HANDLE__ specifies the CAN Handle.
 * @param  __STATE_EX__ specifies the state_ex source to enable or disable.
 */
#define CAN_STATE_EX_PTB                            (1U << 0) // can has pending PTB buffer
#define CAN_STATE_EX_STB                            (1U << 1) // can has pending STB buffer
#define CAN_STATE_EX_ABORT                          (1U << 2) // abort is ongoing
#define CAN_STATE_EX_SLEEP                          (1U << 3) // can is sleep mode
#define CAN_STATE_EX_SET(__HANDLE__, __STATE_EX__)  HAL_FLAG_SET((__HANDLE__)->state_ex, (__STATE_EX__))
#define CAN_STATE_EX_CLR(__HANDLE__, __STATE_EX__)  HAL_FLAG_CLR((__HANDLE__)->state_ex, (__STATE_EX__))
#define CAN_STATE_EX_HAS(__HANDLE__, __STATE_EX__)  HAL_FLAG_HAS((__HANDLE__)->state_ex, (__STATE_EX__))
#define CAN_STATE_EX_LACK(__HANDLE__, __STATE_EX__) HAL_FLAG_LACKS((__HANDLE__)->state_ex, (__STATE_EX__))

/* Private constants ---------------------------------------------------------*/
#define CAN_DEFAULT_TIMEOUT (10)     /* timeout 10 ms after             */
#define CAN_RX_ALMOST_THD   (15 / 2) /* receive fifo almost thd (half)  */
#define CAN_EW_LIMIT        (15)     /* error Warning Limit = (EWL+1)*8 */

/* Private function ----------------------------------------------------------*/
static uint32_t _can_set_config(CAN_Handle_T *hcan);
static void     _can_init_callbacks_to_default(CAN_Handle_T *hcan);

#ifdef __cplusplus
}
#endif
#endif /* _SN34F78X_HAL_CAN_H_ */
