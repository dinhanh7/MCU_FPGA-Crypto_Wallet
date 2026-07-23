#ifndef _SN34F78X_HAL_DEF_H_
#define _SN34F78X_HAL_DEF_H_

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "sn34f78x_def.h"

/* Exported types ------------------------------------------------------------*/
/**
 * \brief Define the return status for HAL API calling.
 * \ingroup execution_status
 */
typedef enum
{
    HAL_OK      = 0x00U, /**< Operation succeeded */
    HAL_ERROR   = 0x01U, /**< Operation failed */
    HAL_BUSY    = 0x02U, /**< Driver is busy */
    HAL_TIMEOUT = 0x03U, /**< Operation timeout */
} HAL_Status_T;

/**
 * \brief Define the runtime states for HAL_%IP%_GetState interface.
 * \ingroup hal_runtime_state
 */
typedef enum
{
    HAL_STATE_RESET      = 0x00U, /**< Peripheral is not yet initialized */
    HAL_STATE_READY      = 0x01U, /**< Peripheral initialized and ready for use */
    HAL_STATE_BUSY_TX    = 0x04U, /**< Peripheral TX process is ongoing */
    HAL_STATE_BUSY_RX    = 0x08U, /**< Peripheral RX process is ongoing */
    HAL_STATE_BUSY_TX_RX = 0x10U, /**< Peripheral TX and RX processes are ongoing */
    HAL_STATE_BUSY       = 0x1CU, /**< Peripheral internal process is ongoing */
} HAL_State_T;

/* Exported constants --------------------------------------------------------*/
/**
 * \brief Define the progress protection mutex
 */
typedef volatile uint8_t HAL_MUTEX; /**< Progress protection mutex */

/**
 * \brief Define HAL enable / disable value
 * \ingroup hal_macro
 * @{
 */
#define HAL_DISABLE 0x0U /**< General disable value */
#define HAL_ENABLE  0x1U /**< General enable  value */
/**
 * @}
 */

/**
 * \brief HAL max delay
 * \ingroup hal_macro
 */
#define HAL_MAX_DELAY 0xFFFFFFFFU /**< Max delay for timeout usage */

/**
 * \brief HAL error code
 * \ingroup hal_error_code
 * @{
 */
#define HAL_ERROR_NONE    0UL  /**< No Error */
#define HAL_ERROR_TIMEOUT 1UL  /**< Timeout Error */
#define HAL_ERROR_REINIT  2UL  /**< Re-init Error */
#define HAL_ERROR_SPECIFY 10UL /**< Specifying Error */
/**
 * @}
 */

/* Exported macros -----------------------------------------------------------*/
/**
 * \brief Define the HAL flag check macros
 * \ingroup hal_macro
 * @{
 */
#define HAL_FLAG_EQU(flag1, flag2) FLAG_EQU(flag1, flag2) /**< 'flag1' == 'flag2' */
#define HAL_FLAG_NEQ(flag1, flag2) FLAG_NEQ(flag1, flag2) /**< 'flag1' != 'flag2' */
#define HAL_FLAG_HAS(flag, e)      FLAG_HAS(flag, e)      /**< check current 'flag' whether has 'e' */
#define HAL_FLAG_LACKS(flag, e)    FLAG_LACKS(flag, e)    /**< check current 'flag' whether lacks 'e' */
#define HAL_FLAG_SET(flag, e)      FLAG_SET(flag, e)      /**< set 'flag' with 'e' */
#define HAL_FLAG_CLR(flag, e)      FLAG_CLR(flag, e)      /**< clear 'e' from 'flag' */
#define HAL_FLAG_RESET(flag)       FLAG_RESET(flag)       /**< zero 'flag' content */
#define HAL_FLAG_WRITE(flag, v)    FLAG_WRITE(flag, v)    /**< write 'v' to  'flag' */
#define HAL_FLAG_AND(flag, e)      FLAG_AND(flag, e)      /**< do 'AND' operator between 'flag' and 'e' */
/**
 * @}
 */

/**
 * \brief Define the HAL register operation macros
 * \ingroup hal_macro
 * @{
 */
#define HAL_REG_RBIT(reg, bit)      REG_RBIT(reg, bit)      /**< read register bit */
#define HAL_REG_SBIT(reg, bit)      REG_SBIT(reg, bit)      /**< set register bit */
#define HAL_REG_CBIT(reg, bit)      REG_CBIT(reg, bit)      /**< clear register bit */
#define HAL_REG_MBIT(reg, msk, bit) REG_MBIT(reg, msk, bit) /**< modify register with bits */
#define HAL_REG_READ(reg)           REG_READ(reg)           /**< read register */
#define HAL_REG_WRITE(reg, val)     REG_WRITE(reg, val)     /**< write val to register */
#define HAL_REG_CLEAR(reg)          REG_CLEAR(reg)          /**< clear register */
/**
 * @}
 */

/**
 * \brief Check if the value can be divided with no remainder by 4
 * \ingroup hal_macro
 */
#define HAL_IS_ALIGNED_TO_4(e) (HAL_FLAG_LACKS((uint32_t)e, 0x03)) /**< Check if the value can be divided with no remainder by 4 */

/**
 * \brief HAL bit mask
 * \ingroup hal_macro
 */
#define HAL_BIT(n) (0x1UL << (n)) /**< Calculate bit (from bit 0) mask */

/**
 * \brief HAL link DMA
 * \ingroup hal_macro
 */
#define HAL_LINKDMA(__HANDLE__, __PPP_DMA_FIELD__, __DMA_HANDLE__) /**< Link DMA */ \
    do                                                                              \
    {                                                                               \
        (__HANDLE__)->__PPP_DMA_FIELD__ = &(__DMA_HANDLE__);                        \
        (__DMA_HANDLE__).parent         = (__HANDLE__);                             \
    } while (0U) /**< Link the peripheral DMA filed with specified DMA handler and also link current DMA parent with current handler */

/**
 * \brief Check current process whether timeout; this macro should be called under loop scope.
 * \ingroup hal_macro
 */
#define BREAK_PROC_TIMEOUT(__HANDLE__, __TICK_START__, __DURATION__) /**< Timeout Return */           \
    if (((__DURATION__) != HAL_MAX_DELAY) && (((HAL_GetTick() - (__TICK_START__)) > (__DURATION__)))) \
    {                                                                                                 \
        __HANDLE__->error_code = HAL_ERROR_TIMEOUT;                                                   \
        break;                                                                                        \
    }

/**
 * \brief Clear current error code
 * \ingroup hal_macro
 */
#define CLEAR_ERROR_CODE(handle) (handle)->error_code = HAL_ERROR_NONE /**< Clear Error Code */

/**
 * \brief Check if current state is ready
 * \ingroup hal_macro
 */
#define RET_STATE_NOT_READY(handle) /**< State not ready */ \
    do                                                      \
    {                                                       \
        if ((handle)->state != HAL_STATE_READY)             \
        {                                                   \
            return HAL_ERROR;                               \
        }                                                   \
    } while (0)

/**
 * \brief Keep workflow protected
 * \ingroup hal_macro
 */
#define TAKE_MUTEX(handle) /**< Take Mutex */ \
    do                                        \
    {                                         \
        if ((handle)->mutex == 1)             \
        {                                     \
            return HAL_BUSY;                  \
        }                                     \
        else                                  \
        {                                     \
            (handle)->mutex = 1;              \
        }                                     \
    } while (0U)

/**
 * \brief Release workflow protected
 * \ingroup hal_macro
 */
#define GIVE_MUTEX(handle) /**< Give Mutex */ \
    do                                        \
    {                                         \
        (handle)->mutex = 0;                  \
    } while (0U)

/**
 * \brief define the constant express or value express for register operation
 * \ingroup hal_macro
 */
#define REG_CONST_EXPR(VAL) (*((uint32_t *)VAL)) /**< Constant Value express for register operation */

#ifdef __cplusplus
}
#endif

#endif /* _SN34F78X_HAL_DEF_H_ */
