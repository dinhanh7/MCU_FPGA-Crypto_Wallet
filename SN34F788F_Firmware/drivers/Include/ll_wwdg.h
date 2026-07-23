/***********************
 * SN34F78X SDK V2.1.4 *
 ***********************/

#ifndef _LL_WWDG_H_
#define _LL_WWDG_H_

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "sn34f78x_ll_def.h"
#include "ll_reg_msk.h"

/* Exported types ------------------------------------------------------------*/
/**
 * @brief WWDG Init Structure Definition
 * \ingroup wwdg_struct_define
 */
typedef struct
{
    uint32_t prescaler;    /**< Specifies the pre-scaler value of the WWDG.
                                This parameter can be a value of @ref wwdg_prescaler_const */
    uint32_t counter;      /**< Specifies the timeout value of the WWDG.
                                This parameter can be a value in range of @ref wwdg_counter_range */
    uint32_t window;       /**< Specifies the WWDG window value to be compared to the down-counter.
                                This parameter can be a value in range of @ref wwdg_counter_range */
    uint32_t underflow_en; /**< Specifies underflow when WWDG timeout.
                                This parameter can be a value of LL_DISABLE of LL_ENABLE */
    uint32_t reset_en;     /**< Specifies reset when WWDG timeout.
                                This parameter can be a value of LL_DISABLE of LL_ENABLE */
    uint32_t overflow_en;  /**< Specifies overflow when WWDG timeout.
                                This parameter can be a value of LL_DISABLE of LL_ENABLE */
} LL_WWDG_Init_T;

/**
 * \defgroup wwdg_prescaler_const WWDG prescaler Range
 * \ingroup wwdg_control
 * @{
 */
#define LL_WWDG_PRESCALER_0 (0x00000000U) /**<  WWDT prescaler is divider/4 */
#define LL_WWDG_PRESCALER_1 (0x00000001U) /**<  WWDT prescaler is divider/8 */
#define LL_WWDG_PRESCALER_2 (0x00000002U) /**<  WWDT prescaler is divider/16 */
#define LL_WWDG_PRESCALER_3 (0x00000003U) /**<  WWDT prescaler is divider/32 */
#define LL_WWDG_PRESCALER_4 (0x00000004U) /**<  WWDT prescaler is divider/64 */
#define LL_WWDG_PRESCALER_5 (0x00000005U) /**<  WWDT prescaler is divider/128 */
#define LL_WWDG_PRESCALER_6 (0x00000006U) /**<  WWDT prescaler is divider/256 */
#define LL_WWDG_PRESCALER_7 (0x00000007U) /**<  WWDT prescaler is divider/256 */
/**
 * @}
 */

/**
 * \defgroup wwdg_counter_range WWDG Timer Constant Range (Unit: Watch dog clock cycle)
 * \ingroup wwdg_control
 * @{
 */
#define LL_WWDG_TIMER_MIN 0x00000001U /**< WWDG timer constant min */
#define LL_WWDG_TIMER_MAX 0xFFFFFFFFU /**< WWDG timer constant max */
/**
 * @}
 */

/* Private types -------------------------------------------------------------*/
#define LL_WWDG_FEED_KEY      0xAAAAU /**< define feed key */
#define LL_WWDG_REPROG_KEY    0X5555U /**< define the key which enable access to the WWDT_PRESCALER and WWDT_RELOAD registers */
#define LL_WWDG_START_KEY     0xCCCCU /**< define the start WWDT key */
#define LL_WWDG_DISREPROG_KEY 0X0000U /**< define the key which disable access to the the WWDT_PRESCALER and WWDT_RELOAD registers */

/* Exported functions --------------------------------------------------------*/
/**
 * @brief  Reloads WWDG counter with value defined in the reload register
 * @param  wwdg WWDG Instance
 * @retval None
 */
__STATIC_INLINE void LL_WWDG_ReloadCounter(SN_WWDT_Type *wwdg)
{
    LL_REG_WRITE(wwdg->KEY, LL_WWDG_FEED_KEY);
}

/**
 * @brief  start WWDG
 * @param  wwdg WWDG Instance
 * @retval None
 */
__STATIC_INLINE void LL_WWDG_Start(SN_WWDT_Type *wwdg)
{
    LL_REG_WRITE(wwdg->KEY, LL_WWDG_START_KEY);
}

/**
 * @brief  enable access to the WWDT_PRESCALER and WWDT_RELOAD
 * @param  wwdg WWDG Instance
 * @retval None
 */
__STATIC_INLINE void LL_WWDG_EnableWriteAccess(SN_WWDT_Type *wwdg)
{
    LL_REG_WRITE(wwdg->KEY, LL_WWDG_REPROG_KEY);
}

/**
 * @brief  Gets the current enabled state of WWDG
 * @param  wwdg WWDG Instance
 * @retval 1：WWDT Enable；0：WWDT Disable
 */
__STATIC_INLINE uint32_t LL_WWDG_IsEnabled(SN_WWDT_Type *wwdg)
{
    return (LL_REG_RBIT(wwdg->CTRL, WWDT_CTRL_WWDTEN) == WWDT_CTRL_WWDTEN);
}

/**
 * @brief  disable access to the WWDT_PRESCALER and WWDT_RELOAD
 * @param  wwdg WWDG Instance
 * @retval None
 */
__STATIC_INLINE void LL_WWDG_DisableWriteAccess(SN_WWDT_Type *wwdg)
{
    LL_REG_WRITE(wwdg->KEY, LL_WWDG_DISREPROG_KEY);
}

/**
 * @brief  Set WWDG prescaler
 * @param  wwdg WWDG Instance
 * @param  prescaler This parameter can be a value in range of @ref wwdg_prescaler_const
 * @retval None
 */
__STATIC_INLINE void LL_WWDG_SetPrescaler(SN_WWDT_Type *wwdg, uint32_t prescaler)
{
    LL_REG_WRITE(wwdg->PRESCALER, prescaler);
}

/**
 * @brief  Specify the WWDG down-counter reload value
 * @param  wwdg WWDG Instance
 * @param  counter This parameter can be a value in range of @ref wwdg_counter_range
 * @retval None
 */
__STATIC_INLINE void LL_WWDG_SetReloadCounter(SN_WWDT_Type *wwdg, uint32_t counter)
{
    LL_REG_WRITE(wwdg->RELOAD, counter);
}

/**
 * @brief  Get the specified WWDG down-counter reload value
 * @param  wwdg WWDG Instance
 * @retval Value between Min_Data=1 and Max_Data=256
 */
__STATIC_INLINE uint32_t LL_WWDG_GetReloadCounter(SN_WWDT_Type *wwdg)
{
    return (LL_REG_READ(wwdg->RELOAD));
}

/**
 * @brief  Enable Watchdog interrupt
 * @param  wwdg WWDG Instance
 * @retval None.
 */
__STATIC_INLINE void LL_WWDG_EnableIT_OVF(SN_WWDT_Type *wwdg)
{
    LL_REG_SBIT(wwdg->CTRL, WWDT_CTRL_IE);
}

/**
 * @brief  Disable Watchdog interrupt
 * @param  wwdg WWDG Instance
 * @retval None.
 */
__STATIC_INLINE void LL_WWDG_DisableIT_OVF(SN_WWDT_Type *wwdg)
{
    LL_REG_CBIT(wwdg->CTRL, WWDT_CTRL_IE);
}

/**
 * @brief  Determine if IRQ is enabled
 * @param  wwdg WWDG Instance
 * @retval IRQ state.
 */
__STATIC_INLINE uint32_t LL_WWDG_IsEnabledIT_OVF(SN_WWDT_Type *wwdg)
{
    return (LL_REG_RBIT(wwdg->CTRL, WWDT_CTRL_IE) == WWDT_CTRL_IE);
}

/**
 * @brief  Enable Watchdog reset
 * @param  wwdg WWDG Instance
 * @retval None.
 */
__STATIC_INLINE void LL_WWDG_Enable_Reset(SN_WWDT_Type *wwdg)
{
    LL_REG_SBIT(wwdg->CTRL, WWDT_CTRL_RSTEN);
}

/**
 * @brief  Disable Watchdog reset
 * @param  wwdg WWDG Instance
 * @retval None.
 */
__STATIC_INLINE void LL_WWDG_Disable_Reset(SN_WWDT_Type *wwdg)
{
    LL_REG_CBIT(wwdg->CTRL, WWDT_CTRL_RSTEN);
}

/**
 * @brief  Determine if Reset is enabled
 * @param  wwdg WWDG Instance
 * @retval Reset state.
 */
__STATIC_INLINE uint32_t LL_WWDG_IsEnabled_Reset(SN_WWDT_Type *wwdg)
{
    return (LL_REG_RBIT(wwdg->CTRL, WWDT_CTRL_RSTEN) == WWDT_CTRL_RSTEN);
}

/**
 * @brief  Set up WWDT Windows
 * @param  wwdg WWDG Instance
 * @param  window This parameter can be a value in range of @ref wwdg_counter_range
 * @retval None.
 */
__STATIC_INLINE void LL_WWDG_SetWindow(SN_WWDT_Type *wwdg, uint32_t window)
{
    LL_REG_WRITE(wwdg->UDFV, window);
}

/**
 * @brief  Set the duration of an assertion that interrupts a reset
 * @param  wwdg WWDG Instance
 * @param  len  Duration of an assertion that interrupts a reset
 * @retval None.
 */
__STATIC_INLINE void LL_WWDG_SetAssertionDuration(SN_WWDT_Type *wwdg, uint32_t len)
{
    LL_REG_WRITE(wwdg->INTL, (len & WWDT_INTL_LEN_Msk));
}

/**
 * @brief  Get the duration of an assertion that interrupts a reset
 * @param  wwdg WWDG Instance
 * @retval IRQ state.
 */
__STATIC_INLINE uint32_t LL_WWDG_GetAssertionDuration(SN_WWDT_Type *wwdg)
{
    return (LL_REG_RBIT(wwdg->INTL, WWDT_CTRL_IE) & WWDT_INTL_LEN_Msk);
}

/**
 * @brief  Enable the Early Wakeup Interrupt
 * @param  wwdg WWDG Instance
 * @retval None.
 */
__STATIC_INLINE void LL_WWDG_EnableIT_UDF(SN_WWDT_Type *wwdg)
{
    LL_REG_SBIT(wwdg->CTRL, WWDT_CTRL_UDFIE);
}

/**
 * @brief  Disable the Early Wakeup Interrupt
 * @param  wwdg WWDG Instance
 * @retval None.
 */
__STATIC_INLINE void LL_WWDG_DisableIT_UDF(SN_WWDT_Type *wwdg)
{
    LL_REG_CBIT(wwdg->CTRL, WWDT_CTRL_UDFIE);
}

/**
 * @brief  Check if Early Wakeup Interrupt is enabled
 * @param  wwdg WWDG Instance
 * @retval underflow interrupt Enable state.
 */
__STATIC_INLINE uint32_t LL_WWDG_IsEnabledIT_UDF(SN_WWDT_Type *wwdg)
{
    return (LL_REG_RBIT(wwdg->CTRL, WWDT_CTRL_UDFIE) == WWDT_CTRL_UDFIE);
}

/**
 * @brief  Enable Watchdog underflow
 * @param  wwdg WWDG Instance
 * @retval None.
 */
__STATIC_INLINE void LL_WWDG_Enable_UDF(SN_WWDT_Type *wwdg)
{
    LL_REG_SBIT(wwdg->CTRL, WWDT_CTRL_UDFEN);
}

/**
 * @brief  Disable Watchdog underflow
 * @param  wwdg WWDG Instance
 * @retval None.
 */
__STATIC_INLINE void LL_WWDG_Disable_UDF(SN_WWDT_Type *wwdg)
{
    LL_REG_CBIT(wwdg->CTRL, WWDT_CTRL_UDFEN);
}

/**
 * @brief  Determine if underflow function is enabled
 * @param  wwdg WWDG Instance
 * @retval underflow function Enable state.
 */
__STATIC_INLINE uint32_t LL_WWDG_IsEnabled_UDF(SN_WWDT_Type *wwdg)
{
    return (LL_REG_RBIT(wwdg->CTRL, WWDT_CTRL_UDFEN) == WWDT_CTRL_UDFEN);
}

/**
 * @brief  Check WWDT enters underflow state
 * @param  wwdg WWDG Instance
 * @retval underflow interrupt state.
 */
__STATIC_INLINE uint32_t LL_WWDG_IsActiveFlag_UDF(SN_WWDT_Type *wwdg)
{
    return (LL_REG_RBIT(wwdg->RIS, WWDT_RIS_UDF) == WWDT_RIS_UDF);
}

/**
 * @brief  clear WWDT enters underflow state
 * @param  wwdg WWDG Instance
 * @retval None.
 */
__STATIC_INLINE void LL_WWDG_ClearFlag_UDF(SN_WWDT_Type *wwdg)
{
    LL_REG_SBIT(wwdg->RIS, WWDT_RIS_UDF);
}

/**
 * @brief  Check WWDT enters OVF state
 * @param  wwdg WWDG Instance
 * @retval OVF state.
 */
__STATIC_INLINE uint32_t LL_WWDG_IsActiveFlag_OVF(SN_WWDT_Type *wwdg)
{
    return (LL_REG_RBIT(wwdg->RIS, WWDT_RIS_OVF) == WWDT_RIS_OVF);
}

/**
 * @brief  clear WWDT OVF state
 * @param  wwdg WWDG Instance
 * @retval None.
 */
__STATIC_INLINE void LL_WWDG_ClearFlag_OVF(SN_WWDT_Type *wwdg)
{
    LL_REG_SBIT(wwdg->RIS, WWDT_RIS_OVF);
}

/**
 * @brief  Check WWDG prescaler value update
 * @param  wwdg WWDG Instance
 * @retval prescaler value update state.
 */
__STATIC_INLINE uint32_t LL_WWDG_IsActiveFlag_PrescalerLock(SN_WWDT_Type *wwdg)
{
    return (LL_REG_RBIT(wwdg->LOCKST, WWDT_LOCKST_PRESCALER_LOCK) == WWDT_LOCKST_PRESCALER_LOCK);
}

/**
 * @brief  Check WWDG Reload value update
 * @param  wwdg WWDG Instance
 * @retval Reload value update state.
 */
__STATIC_INLINE uint32_t LL_WWDG_IsActiveFlag_ReloadLock(SN_WWDT_Type *wwdg)
{
    return (LL_REG_RBIT(wwdg->LOCKST, WWDT_LOCKST_RELOAD_LOCK) == WWDT_LOCKST_RELOAD_LOCK);
}

#if defined(USE_FULL_LL_DRIVER)
LL_Status_T LL_WWDG_Init(SN_WWDT_Type *wwdg, LL_WWDG_Init_T *wwdg_init);
LL_Status_T LL_WWDG_DeInit(SN_WWDT_Type *wwdg);
void        LL_WWDG_StructInit(LL_WWDG_Init_T *wwdg_init);
#endif /* USE_FULL_LL_DRIVER */

#ifdef __cplusplus
}
#endif

#endif
