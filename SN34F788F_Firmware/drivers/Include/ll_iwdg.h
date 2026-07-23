/***********************
 * SN34F78X SDK V2.1.4 *
 ***********************/

#ifndef _LL_IWDG_H_
#define _LL_IWDG_H_

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "sn34f78x_ll_def.h"
#include "ll_reg_msk.h"

/* Exported types ------------------------------------------------------------*/
/**
 * @brief IWDG Init Structure Definition
 * \ingroup iwdg_struct_define
 */
typedef struct
{
    uint32_t ie;             /**< Specifies interrupt when IWDG timeout.
                                  This parameter can be a value of LL_DISABLE of LL_ENABLE */
    uint32_t reset_en;       /**< Specifies reset when IWDG timeout.
                                  This parameter can be a value of LL_DISABLE of LL_ENABLE */
    uint32_t timer_constant; /**< Specifies the timeout value of the IWDG.
                                  This parameter can be a value in range of @ref iwdg_timer_const */

} LL_IWDG_Init_T;

/**
 * \defgroup iwdg_timer_const IWDG Timer Constant Range (Unit: Watch dog clock cycle)
 * \ingroup iwdg_control
 * @{
 */
#define LL_IWDG_TIMER_MIN 0x00000001U /**< IWDG timer constant min */
#define LL_IWDG_TIMER_MAX 0xFFFFFFFFU /**< IWDG timer constant max */
/**
 * @}
 */

/* Private types -------------------------------------------------------------*/
#define LL_IWDG_FEED_KEY 0x00005AB9U /*!< IWDG Feed Register Key */

/* Exported functions --------------------------------------------------------*/
/**
 * @brief  Start the Independent Watchdog
 * @note   Except if the hardware watchdog option is selected
 * @param  iwdg IWDG Instance
 * @retval None
 */
__STATIC_INLINE void LL_IWDG_Enable(SN_WDT_Type *iwdg)
{
    LL_REG_SBIT(iwdg->CTRL, WDT_CTRL_WDTEN);
}

/**
 * @brief  Disable the Independent Watchdog
 * @param  iwdg IWDG Instance
 * @retval None
 */
__STATIC_INLINE void LL_IWDG_Disable(SN_WDT_Type *iwdg)
{
    /* disable IWDG */
    LL_REG_CBIT(iwdg->CTRL, WDT_CTRL_WDTEN);
}

/**
 * @brief  check the Independent Watchdog enable
 * @param  iwdg IWDG Instance
 * @retval Watchdog enable state
 */
__STATIC_INLINE uint32_t LL_IWDG_IsEnabled(SN_WDT_Type *iwdg)
{
    return (LL_REG_READ(iwdg->CTRL) & WDT_CTRL_WDTEN);
}

/**
 * @brief  Reloads IWDG counter with value defined in the reload register
 * @param  iwdg IWDG Instance
 * @retval None
 */
__STATIC_INLINE void LL_IWDG_ReloadCounter(SN_WDT_Type *iwdg)
{
    LL_REG_WRITE(iwdg->RESTART, LL_IWDG_FEED_KEY);
}

/**
 * @brief  Specify the IWDG down-counter reload value
 * @param  iwdg IWDG Instance
 * @param  counter This parameter can be a value in range of @ref iwdg_timer_const
 * @retval None
 */
__STATIC_INLINE void LL_IWDG_SetReloadCounter(SN_WDT_Type *iwdg, uint32_t counter)
{
    LL_REG_WRITE(iwdg->LOAD, counter);
}

/**
 * @brief  Get the specified IWDG down-counter reload value
 * @param  iwdg IWDG Instance
 * @retval Value between Min_Data=1 and Max_Data=256
 */
__STATIC_INLINE uint32_t LL_IWDG_GetReloadCounter(SN_WDT_Type *iwdg)
{
    return (LL_REG_READ(iwdg->LOAD));
}

/**
 * @brief  Get current counter value
 * @param  iwdg IWDG Instance
 * @retval current counter value
 */
__STATIC_INLINE uint32_t LL_IWDG_GetCurrentCounter(SN_WDT_Type *iwdg)
{
    return (LL_REG_READ(iwdg->COUNTER));
}

/**
 * @brief  Enable Watchdog Timeout interrupt
 * @param  iwdg IWDG Instance
 * @retval None.
 */
__STATIC_INLINE void LL_IWDG_EnableIT_Timeout(SN_WDT_Type *iwdg)
{
    LL_REG_SBIT(iwdg->CTRL, WDT_CTRL_IE);
}

/**
 * @brief  Disable Watchdog Timeout interrupt
 * @param  iwdg IWDG Instance
 * @retval None.
 */
__STATIC_INLINE void LL_IWDG_DisableIT_Timeout(SN_WDT_Type *iwdg)
{
    LL_REG_CBIT(iwdg->CTRL, WDT_CTRL_IE);
}

/**
 * @brief  check the Independent Watchdog Timeout interrupt enable
 * @param  iwdg IWDG Instance
 * @retval Watchdog interrupt enable state
 */
__STATIC_INLINE uint32_t LL_IWDG_IsEnabledIT_Timeout(SN_WDT_Type *iwdg)
{
    return (LL_REG_READ(iwdg->CTRL) & WDT_CTRL_IE);
}

/**
 * @brief  Enable Watchdog reset
 * @param  iwdg IWDG Instance
 * @retval None.
 */
__STATIC_INLINE void LL_IWDG_Enable_Reset(SN_WDT_Type *iwdg)
{
    LL_REG_SBIT(iwdg->CTRL, WDT_CTRL_RSTEN);
}

/**
 * @brief  Disable Watchdog reset
 * @param  iwdg IWDG Instance
 * @retval None.
 */
__STATIC_INLINE void LL_IWDG_Disable_Reset(SN_WDT_Type *iwdg)
{
    LL_REG_CBIT(iwdg->CTRL, WDT_CTRL_RSTEN);
}

/**
 * @brief  check the Independent Watchdog Reset
 * @param  iwdg IWDG Instance
 * @retval Watchdog Reset state
 */
__STATIC_INLINE uint32_t LL_IWDG_IsEnabled_Reset(SN_WDT_Type *iwdg)
{
    return (LL_REG_READ(iwdg->CTRL) & WDT_CTRL_RSTEN);
}

/**
 * @brief  Check Watchdog cause a interrupt or chip reset when watch dog time out.
 * @param  iwdg IWDG Instance
 * @retval Watchdog Count state.
 */
__STATIC_INLINE uint32_t LL_IWDG_IsActiveFlag_Count(SN_WDT_Type *iwdg)
{
    return (LL_REG_RBIT(iwdg->STATUS, WDT_STATUS_COUNT) == WDT_STATUS_COUNT);
}

/**
 * @brief  Check Watchdog cause a interrupt or chip reset when watch dog time out.
 * @param  iwdg IWDG Instance
 * @retval None.
 */
__STATIC_INLINE void LL_IWDG_ClearFlag_Status(SN_WDT_Type *iwdg)
{
    LL_REG_SBIT(iwdg->CLEAR, WDT_CLEAR_STATUS);
}

#if defined(USE_FULL_LL_DRIVER)
LL_Status_T LL_IWDG_Init(SN_WDT_Type *iwdg, LL_IWDG_Init_T *iwdg_init);
LL_Status_T LL_IWDG_DeInit(SN_WDT_Type *iwdg);
void        LL_IWDG_StructInit(LL_IWDG_Init_T *iwdg_init);
#endif /* USE_FULL_LL_DRIVER */

#ifdef __cplusplus
}
#endif

#endif
