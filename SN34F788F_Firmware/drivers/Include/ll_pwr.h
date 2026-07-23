/***********************
 * SN34F78X SDK V2.1.4 *
 ***********************/

#ifndef _LL_PWR_H_
#define _LL_PWR_H_

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "ll_scu.h"
#include "ll_reg_msk.h"

/* Exported types ------------------------------------------------------------*/

/* Exported constants --------------------------------------------------------*/
/**
 * \defgroup pwr_power_mode Power Mode
 * \ingroup pwr_control
 * @{
 */
#define LL_PWR_MODE_SLP  SCU_PWRMODE_SLEEP  /**< Power Mode: Sleep */
#define LL_PWR_MODE_DSLP SCU_PWRMODE_DSLEEP /**< Power Mode: Deep Sleep */
#define LL_PWR_MODE_DPD  SCU_PWRMODE_DPD    /**< Power Mode: Deep Power Down */
/**
 * @}
 */

/* Exported functions --------------------------------------------------------*/
/**
 * @brief  Set the power mode.
 * @param  mode This parameter can be one of @ref pwr_power_mode
 * @retval None
 */
__STATIC_INLINE void LL_PWR_SetPowerMode(uint32_t mode)
{
    LL_REG_SBIT(SN_SCU->PWRMODE, mode);
}

/**
 * @brief  Return the power mode.
 * @retval Returned value can be one of @ref pwr_power_mode
 */
__STATIC_INLINE uint32_t LL_PWR_GetPowerMode(void)
{
    return LL_REG_RBIT(SN_SCU->PWRMODE, SCU_PWRMODE_DPD | SCU_PWRMODE_DSLEEP | SCU_PWRMODE_SLEEP);
}

/**
 * @brief  Clear the power mode.
 * @retval None
 */
__STATIC_INLINE void LL_PWR_ClearPowerMode(void)
{
    LL_REG_CBIT(SN_SCU->PWRMODEMISC, SCU_PWRMODEMISC_PWRMODECLR);
}

/**
 * @brief  Enter sleep mode.
 * @retval None
 */
__STATIC_INLINE void LL_PWR_EnterSleep(void)
{
    LL_REG_SBIT(SCB->SCR, SCB_SCR_SLEEPDEEP_Msk);
    LL_PWR_SetPowerMode(LL_PWR_MODE_SLP);
    __WFI();
    LL_PWR_ClearPowerMode();
    LL_REG_CBIT(SCB->SCR, SCB_SCR_SLEEPDEEP_Msk);
}

/**
 * @brief  Enter deep sleep mode.
 * @retval None
 */
__STATIC_INLINE void LL_PWR_EnterDeepSleep(void)
{
    LL_REG_CBIT(SCB->SCR, SCB_SCR_SLEEPDEEP_Msk);
    LL_PWR_SetPowerMode(LL_PWR_MODE_DSLP);
    __WFI();
    LL_PWR_ClearPowerMode();
}

/**
 * @brief  Enter Deep Power Down mode.
 * @retval None
 */
__STATIC_INLINE void LL_PWR_EnterDeepPowerDown(void)
{
    LL_REG_CBIT(SCB->SCR, SCB_SCR_SLEEPDEEP_Msk);
    LL_PWR_SetPowerMode(LL_PWR_MODE_DPD);
    __WFI();
}

/**
 * @brief  Enter Frequency Change Sequence.
 * @retval None
 */
__STATIC_INLINE void LL_PWR_EnterFCS(void)
{
    LL_REG_SBIT(SN_SCU->PWRMODE, SCU_PWRMODE_FCS);
}

/**
 * @brief  Exit Frequency Change Sequence.
 * @retval None
 */
__STATIC_INLINE void LL_PWR_ExitFCS(void)
{
    LL_REG_CBIT(SN_SCU->PWRMODE, SCU_PWRMODE_FCS);
}

/**
 * @brief  Enable sleep mode wakeup for NVIC.
 * @retval None
 */
__STATIC_INLINE void LL_PWR_Enable_SLPWakeupNVIC(void)
{
    LL_REG_SBIT(SN_SCU->SLP_WAKUPEN, SCU_SLP_WAKUPEN_SLP_WAKUP_INTEN);
}

/**
 * @brief  Disable sleep mode wakeup for NVIC.
 * @retval None
 */
__STATIC_INLINE void LL_PWR_Disable_SLPWakeupNVIC(void)
{
    LL_REG_CBIT(SN_SCU->SLP_WAKUPEN, SCU_SLP_WAKUPEN_SLP_WAKUP_INTEN);
}

/**
 * @brief  Return if sleep mode wakeup for NVIC is enabled.
 * @retval State of bit (1 or 0).
 */
__STATIC_INLINE uint32_t LL_PWR_IsEnabled_SLPWakeupNVIC(void)
{
    return (LL_REG_RBIT(SN_SCU->SLP_WAKUPEN, SCU_SLP_WAKUPEN_SLP_WAKUP_INTEN) == (SCU_SLP_WAKUPEN_SLP_WAKUP_INTEN));
}

/**
 * @brief  Return if wakeup from sleep by NVIC.
 * @retval State of bit (1 or 0).
 */
__STATIC_INLINE uint32_t LL_PWR_IsActiveFlag_SLPWakeupNVIC(void)
{
    return (LL_REG_RBIT(SN_SCU->SLP_WAKUPST, SCU_SLP_WAKUPST_SLP_WAKUP_INT) == (SCU_SLP_WAKUPST_SLP_WAKUP_INT));
}

/**
 * @brief  Clear flag of wakeup from sleep by NVIC.
 * @retval None
 */
__STATIC_INLINE void LL_PWR_ClearFlag_SLPWakeupNVIC(void)
{
    LL_REG_WRITE(SN_SCU->SLP_WAKUPST, SCU_SLP_WAKUPST_SLP_WAKUP_INT);
}

/**
 * @brief  Enable sleep mode wakeup interrupt.
 * @retval None
 */
__STATIC_INLINE void LL_PWR_EnableIT_SLPWakeup(void)
{
    LL_REG_SBIT(SN_SCU->IE, SCU_IE_WAKEUP_EINT);
}

/**
 * @brief  Disable sleep mode wakeup interrupt.
 * @retval None
 */
__STATIC_INLINE void LL_PWR_DisableIT_SLPWakeup(void)
{
    LL_REG_CBIT(SN_SCU->IE, SCU_IE_WAKEUP_EINT);
}

/**
 * @brief  Return if sleep mode wakeup interrupt is enabled.
 * @retval State of bit (1 or 0).
 */
__STATIC_INLINE uint32_t LL_PWR_IsEnabledIT_SLPWakeup(void)
{
    return (LL_REG_RBIT(SN_SCU->IE, SCU_IE_WAKEUP_EINT) == (SCU_IE_WAKEUP_EINT));
}

/**
 * @brief  Return if wakeup from sleep.
 * @retval State of bit (1 or 0).
 */
__STATIC_INLINE uint32_t LL_PWR_IsActiveFlag_SLPWakeup(void)
{
    return (LL_REG_RBIT(SN_SCU->RIS, SCU_RIS_INT_WAKEUP) == (SCU_RIS_INT_WAKEUP));
}

/**
 * @brief  Clear flag of wakeup from sleep.
 * @retval None
 */
__STATIC_INLINE void LL_PWR_ClearFlag_SLPWakeup(void)
{
    LL_REG_WRITE(SN_SCU->RIS, SCU_RIS_INT_WAKEUP);
}

/**
 * @brief  Enable deep sleep mode wakeup interrupt.
 * @retval None
 */
__STATIC_INLINE void LL_PWR_EnableIT_DSLPWakeup(void)
{
    LL_REG_SBIT(SN_SCU->IE, SCU_IE_DS_WAKEUP_EINT);
}

/**
 * @brief  Disable deep sleep mode wakeup interrupt.
 * @retval None
 */
__STATIC_INLINE void LL_PWR_DisableIT_DSLPWakeup(void)
{
    LL_REG_CBIT(SN_SCU->IE, SCU_IE_DS_WAKEUP_EINT);
}

/**
 * @brief  Return if deep sleep mode wakeup interrupt is enabled.
 * @retval State of bit (1 or 0).
 */
__STATIC_INLINE uint32_t LL_PWR_IsEnabledIT_DSLPWakeup(void)
{
    return (LL_REG_RBIT(SN_SCU->IE, SCU_IE_DS_WAKEUP_EINT) == (SCU_IE_DS_WAKEUP_EINT));
}

/**
 * @brief  Return if wakeup from deep sleep.
 * @retval State of bit (1 or 0).
 */
__STATIC_INLINE uint32_t LL_PWR_IsActiveFlag_DSLPWakeup(void)
{
    return (LL_REG_RBIT(SN_SCU->RIS, SCU_RIS_INT_DS_WAKEUP) == (SCU_RIS_INT_DS_WAKEUP));
}

/**
 * @brief  Clear flag of wakeup from deep sleep.
 * @retval None
 */
__STATIC_INLINE void LL_PWR_ClearFlag_DSLPWakeup(void)
{
    LL_REG_WRITE(SN_SCU->RIS, SCU_RIS_INT_DS_WAKEUP);
}

/**
 * @brief  Return if wakeup from deep power down.
 * @retval State of bit (1 or 0).
 */
__STATIC_INLINE uint32_t LL_PWR_IsActiveFlag_DPDWakeup(void)
{
    return (LL_REG_RBIT(SN_SCU->BTUP_STS, SCU_BTUP_STS_DPDWKF) == (SCU_BTUP_STS_DPDWKF));
}

/**
 * @brief  Clear flag of wakeup from deep power down.
 * @retval None
 */
__STATIC_INLINE void LL_PWR_ClearFlag_DPDWakeup(void)
{
    LL_REG_WRITE(SN_SCU->BTUP_STS, SCU_BTUP_STS_DPDWKF);
}

/**
 * @brief  Return if GPIO output function was held.
 * @retval State of bit (1 or 0).
 */
__STATIC_INLINE uint32_t LL_PWR_IsActiveFlag_GPIO_HOLD(void)
{
    return (LL_REG_RBIT(SN_SCU->BTUP_STS, SCU_BTUP_STS_GPIO_HOLD) == (SCU_BTUP_STS_GPIO_HOLD));
}

/**
 * @brief  Clear flag of auto hold the GPIO output state.
 * @retval None
 */
__STATIC_INLINE void LL_PWR_ClearFlag_GPIO_HOLD(void)
{
    LL_REG_WRITE(SN_SCU->BTUP_STS, SCU_BTUP_STS_GPIO_HOLD);
}

/**
 * @brief  Enable the wakeup by the RTC alarm.
 * @retval None
 */
__STATIC_INLINE void LL_PWR_Enable_RTCWakeup(void)
{
    LL_REG_SBIT(SN_SCU->BTUP_CTRL, SCU_BTUP_CTRL_RTCWKEN);
}

/**
 * @brief  Disable the wakeup by the RTC alarm.
 * @retval None
 */
__STATIC_INLINE void LL_PWR_Disable_RTCWakeup(void)
{
    LL_REG_CBIT(SN_SCU->BTUP_CTRL, SCU_BTUP_CTRL_RTCWKEN);
}

/**
 * @brief  Return if wakeup by the RTC alarm is enabled.
 * @retval State of bit (1 or 0).
 */
__STATIC_INLINE uint32_t LL_PWR_IsEnabled_RTCWakeup(void)
{
    return (LL_REG_RBIT(SN_SCU->BTUP_CTRL, SCU_BTUP_CTRL_RTCWKEN) == (SCU_BTUP_CTRL_RTCWKEN));
}

/**
 * @brief  Return if wakeup by RTC alarm.
 * @retval State of bit (1 or 0).
 */
__STATIC_INLINE uint32_t LL_PWR_IsActiveFlag_RTCWakeup(void)
{
    return (LL_REG_RBIT(SN_SCU->BTUP_STS, SCU_BTUP_STS_RTCWKF) == (SCU_BTUP_STS_RTCWKF));
}

/**
 * @brief  Clear flag of wakeup by RTC alarm.
 * @retval None
 */
__STATIC_INLINE void LL_PWR_ClearFlag_RTCWakeup(void)
{
    LL_REG_WRITE(SN_SCU->BTUP_STS, SCU_BTUP_STS_RTCWKF);
}

/**
 * @brief  Enable the wakeup by the GPIO0 interrupt.
 * @retval None
 */
__STATIC_INLINE void LL_PWR_Enable_P0Wakeup(void)
{
    LL_REG_SBIT(SN_SCU->BTUP_CTRL, SCU_BTUP_CTRL_P0WKEN);
}

/**
 * @brief  Disable the wakeup by the GPIO0 interrupt.
 * @retval None
 */
__STATIC_INLINE void LL_PWR_Disable_P0Wakeup(void)
{
    LL_REG_CBIT(SN_SCU->BTUP_CTRL, SCU_BTUP_CTRL_P0WKEN);
}

/**
 * @brief  Return if wakeup by the GPIO0 interrupt is enabled.
 * @retval State of bit (1 or 0).
 */
__STATIC_INLINE uint32_t LL_PWR_IsEnabled_P0Wakeup(void)
{
    return (LL_REG_RBIT(SN_SCU->BTUP_CTRL, SCU_BTUP_CTRL_P0WKEN) == (SCU_BTUP_CTRL_P0WKEN));
}

/**
 * @brief  Return if wakeup by GPIO0 interrupt.
 * @retval State of bit (1 or 0).
 */
__STATIC_INLINE uint32_t LL_PWR_IsActiveFlag_P0Wakeup(void)
{
    return (LL_REG_RBIT(SN_SCU->BTUP_STS, SCU_BTUP_STS_P0WKF) == (SCU_BTUP_STS_P0WKF));
}

/**
 * @brief  Clear flag of wakeup by GPIO0 interrupt.
 * @retval None
 */
__STATIC_INLINE void LL_PWR_ClearFlag_P0Wakeup(void)
{
    LL_REG_WRITE(SN_SCU->BTUP_STS, SCU_BTUP_STS_P0WKF);
}

/**
 * @brief  Enable the wakeup by the GPIO1 interrupt.
 * @retval None
 */
__STATIC_INLINE void LL_PWR_Enable_P1Wakeup(void)
{
    LL_REG_SBIT(SN_SCU->BTUP_CTRL, SCU_BTUP_CTRL_P1WKEN);
}

/**
 * @brief  Disable the wakeup by the GPIO1 interrupt.
 * @retval None
 */
__STATIC_INLINE void LL_PWR_Disable_P1Wakeup(void)
{
    LL_REG_CBIT(SN_SCU->BTUP_CTRL, SCU_BTUP_CTRL_P1WKEN);
}

/**
 * @brief  Return if wakeup by the GPIO1 interrupt is enabled.
 * @retval State of bit (1 or 0).
 */
__STATIC_INLINE uint32_t LL_PWR_IsEnabled_P1Wakeup(void)
{
    return (LL_REG_RBIT(SN_SCU->BTUP_CTRL, SCU_BTUP_CTRL_P1WKEN) == (SCU_BTUP_CTRL_P1WKEN));
}

/**
 * @brief  Return if wakeup by GPIO1 interrupt.
 * @retval State of bit (1 or 0).
 */
__STATIC_INLINE uint32_t LL_PWR_IsActiveFlag_P1Wakeup(void)
{
    return (LL_REG_RBIT(SN_SCU->BTUP_STS, SCU_BTUP_STS_P1WKF) == (SCU_BTUP_STS_P1WKF));
}

/**
 * @brief  Clear flag of wakeup by GPIO1 interrupt.
 * @retval None
 */
__STATIC_INLINE void LL_PWR_ClearFlag_P1Wakeup(void)
{
    LL_REG_WRITE(SN_SCU->BTUP_STS, SCU_BTUP_STS_P1WKF);
}

/**
 * @brief  Enable the wakeup by the GPIO2 interrupt.
 * @retval None
 */
__STATIC_INLINE void LL_PWR_Enable_P2Wakeup(void)
{
    LL_REG_SBIT(SN_SCU->BTUP_CTRL, SCU_BTUP_CTRL_P2WKEN);
}

/**
 * @brief  Disable the wakeup by the GPIO2 interrupt.
 * @retval None
 */
__STATIC_INLINE void LL_PWR_Disable_P2Wakeup(void)
{
    LL_REG_CBIT(SN_SCU->BTUP_CTRL, SCU_BTUP_CTRL_P2WKEN);
}

/**
 * @brief  Return if wakeup by the GPIO2 interrupt is enabled.
 * @retval State of bit (1 or 0).
 */
__STATIC_INLINE uint32_t LL_PWR_IsEnable_P2dWakeup(void)
{
    return (LL_REG_RBIT(SN_SCU->BTUP_CTRL, SCU_BTUP_CTRL_P2WKEN) == (SCU_BTUP_CTRL_P2WKEN));
}

/**
 * @brief  Return if wakeup by GPIO2 interrupt.
 * @retval State of bit (1 or 0).
 */
__STATIC_INLINE uint32_t LL_PWR_IsActiveFlag_P2Wakeup(void)
{
    return (LL_REG_RBIT(SN_SCU->BTUP_STS, SCU_BTUP_STS_P2WKF) == (SCU_BTUP_STS_P2WKF));
}

/**
 * @brief  Clear flag of wakeup by GPIO2 interrupt.
 * @retval None
 */
__STATIC_INLINE void LL_PWR_ClearFlag_P2Wakeup(void)
{
    LL_REG_WRITE(SN_SCU->BTUP_STS, SCU_BTUP_STS_P2WKF);
}

/**
 * @brief  Enable the wakeup by the GPIO3 interrupt.
 * @retval None
 */
__STATIC_INLINE void LL_PWR_Enable_P3Wakeup(void)
{
    LL_REG_SBIT(SN_SCU->BTUP_CTRL, SCU_BTUP_CTRL_P3WKEN);
}

/**
 * @brief  Disable the wakeup by the GPIO3 interrupt.
 * @retval None
 */
__STATIC_INLINE void LL_PWR_Disable_P3Wakeup(void)
{
    LL_REG_CBIT(SN_SCU->BTUP_CTRL, SCU_BTUP_CTRL_P3WKEN);
}

/**
 * @brief  Return if wakeup by the GPIO3 interrupt is enabled.
 * @retval State of bit (1 or 0).
 */
__STATIC_INLINE uint32_t LL_PWR_IsEnabled_P3Wakeup(void)
{
    return (LL_REG_RBIT(SN_SCU->BTUP_CTRL, SCU_BTUP_CTRL_P3WKEN) == (SCU_BTUP_CTRL_P3WKEN));
}

/**
 * @brief  Return if wakeup by GPIO3 interrupt.
 * @retval State of bit (1 or 0).
 */
__STATIC_INLINE uint32_t LL_PWR_IsActiveFlag_P3Wakeup(void)
{
    return (LL_REG_RBIT(SN_SCU->BTUP_STS, SCU_BTUP_STS_P3WKF) == (SCU_BTUP_STS_P3WKF));
}

/**
 * @brief  Clear flag of wakeup by GPIO3 interrupt.
 * @retval None
 */
__STATIC_INLINE void LL_PWR_ClearFlag_P3Wakeup(void)
{
    LL_REG_WRITE(SN_SCU->BTUP_STS, SCU_BTUP_STS_P3WKF);
}

/**
 * @brief  Enable the wakeup by the Ethernet Wake-on-LAN Frame.
 * @retval None
 */
__STATIC_INLINE void LL_PWR_Enable_ETHWakeup(void)
{
    LL_REG_SBIT(SN_SCU->BTUP_CTRL, SCU_BTUP_CTRL_ETHWKEN);
}

/**
 * @brief  Disable the wakeup by the Ethernet Wake-on-LAN Frame.
 * @retval None
 */
__STATIC_INLINE void LL_PWR_Disable_ETHWakeup(void)
{
    LL_REG_CBIT(SN_SCU->BTUP_CTRL, SCU_BTUP_CTRL_ETHWKEN);
}

/**
 * @brief  Return if wakeup by the Ethernet Wake-on-LAN Frame.
 * @retval State of bit (1 or 0).
 */
__STATIC_INLINE uint32_t LL_PWR_IsEnabled_ETHWakeup(void)
{
    return (LL_REG_RBIT(SN_SCU->BTUP_CTRL, SCU_BTUP_CTRL_ETHWKEN) == (SCU_BTUP_CTRL_ETHWKEN));
}

/**
 * @brief  Return if wakeup by the Ethernet Wake-on-LAN Frame.
 * @retval State of bit (1 or 0).
 */
__STATIC_INLINE uint32_t LL_PWR_IsActiveFlag_ETHWakeup(void)
{
    return (LL_REG_RBIT(SN_SCU->BTUP_STS, SCU_BTUP_STS_ETHWKF) == (SCU_BTUP_STS_ETHWKF));
}

/**
 * @brief  Clear flag of wakeup by the Ethernet Wake-on-LAN Frame.
 * @retval None
 */
__STATIC_INLINE void LL_PWR_ClearFlag_ETHWakeup(void)
{
    LL_REG_WRITE(SN_SCU->BTUP_STS, SCU_BTUP_STS_ETHWKF);
}


/**
 * @brief  Disable all weakeup event.
 * @retval None
 */
__STATIC_INLINE void LL_PWR_Disable_AllWakeup(void)
{
    LL_REG_CLEAR(SN_SCU->BTUP_CTRL);
}

/**
 * @brief  Clear flag of wakeup by all event.
 * @retval None
 */
__STATIC_INLINE void LL_PWR_ClearFlag_AllWakeup(void)
{
    LL_REG_WRITE(SN_SCU->BTUP_STS, SCU_BTUP_STS_ALLWKF);
}

/**
 * @brief  Enable SRAM1 automatic sleep function.
 * @retval None
 */
__STATIC_INLINE void LL_PWR_Enable_AutoSLP_SRAM1(void)
{
    LL_REG_SBIT(SN_SCU->SRAM1CTRL, SCU_SRAM1CTRL_AUTOSLPEN);
}

/**
 * @brief  Disable SRAM1 automatic sleep function.
 * @retval None
 */
__STATIC_INLINE void LL_PWR_Disable_AutoSLP_SRAM1(void)
{
    LL_REG_CBIT(SN_SCU->SRAM1CTRL, SCU_SRAM1CTRL_AUTOSLPEN);
}

/**
 * @brief  Return if SRAM1 automatic sleep function is enabled.
 * @retval State of bit (1 or 0).
 */
__STATIC_INLINE uint32_t LL_PWR_IsEnabled_AutoSLP_SRAM1(void)
{
    return (LL_REG_RBIT(SN_SCU->SRAM1CTRL, SCU_SRAM1CTRL_AUTOSLPEN) == (SCU_SRAM1CTRL_AUTOSLPEN));
}

/**
 * @brief  Enable SRAM2 automatic sleep function.
 * @retval None
 */
__STATIC_INLINE void LL_PWR_Enable_AutoSLP_SRAM2(void)
{
    LL_REG_SBIT(SN_SCU->SRAM2CTRL, SCU_SRAM2CTRL_AUTOSLPEN);
}

/**
 * @brief  Disable SRAM2 automatic sleep function.
 * @retval None
 */
__STATIC_INLINE void LL_PWR_Disable_AutoSLP_SRAM2(void)
{
    LL_REG_CBIT(SN_SCU->SRAM2CTRL, SCU_SRAM2CTRL_AUTOSLPEN);
}

/**
 * @brief  Return if SRAM2 automatic sleep function is enabled.
 * @retval State of bit (1 or 0).
 */
__STATIC_INLINE uint32_t LL_PWR_IsEnabled_AutoSLP_SRAM2(void)
{
    return (LL_REG_RBIT(SN_SCU->SRAM2CTRL, SCU_SRAM2CTRL_AUTOSLPEN) == (SCU_SRAM2CTRL_AUTOSLPEN));
}

/**
 * @brief  Enable SRAM3 automatic sleep function.
 * @retval None
 */
__STATIC_INLINE void LL_PWR_Enable_AutoSLP_SRAM3(void)
{
    LL_REG_SBIT(SN_SCU->SRAM3CTRL, SCU_SRAM3CTRL_AUTOSLPEN);
}

/**
 * @brief  Disable SRAM3 automatic sleep function.
 * @retval None
 */
__STATIC_INLINE void LL_PWR_Disable_AutoSLP_SRAM3(void)
{
    LL_REG_CBIT(SN_SCU->SRAM3CTRL, SCU_SRAM3CTRL_AUTOSLPEN);
}

/**
 * @brief  Return if SRAM3 automatic sleep function is enabled.
 * @retval State of bit (1 or 0).
 */
__STATIC_INLINE uint32_t LL_PWR_IsEnabled_AutoSLP_SRAM3(void)
{
    return (LL_REG_RBIT(SN_SCU->SRAM3CTRL, SCU_SRAM3CTRL_AUTOSLPEN) == (SCU_SRAM3CTRL_AUTOSLPEN));
}

/**
 * @brief  Enable BKPSRAM automatic sleep function.
 * @retval None
 */
__STATIC_INLINE void LL_PWR_Enable_AutoSLP_BKPSRAM(void)
{
    LL_REG_SBIT(SN_SCU->BKPSRAMCTRL, SCU_BKPSRAMCTRL_AUTOSLPEN);
}

/**
 * @brief  Disable BKPSRAM automatic sleep function.
 * @retval None
 */
__STATIC_INLINE void LL_PWR_Disable_AutoSLP_BKPSRAM(void)
{
    LL_REG_CBIT(SN_SCU->BKPSRAMCTRL, SCU_BKPSRAMCTRL_AUTOSLPEN);
}

/**
 * @brief  Return if BKPSRAM automatic sleep function is enabled.
 * @retval State of bit (1 or 0).
 */
__STATIC_INLINE uint32_t LL_PWR_IsEnabled_AutoSLP_BKPSRAM(void)
{
    return (LL_REG_RBIT(SN_SCU->BKPSRAMCTRL, SCU_BKPSRAMCTRL_AUTOSLPEN) == (SCU_BKPSRAMCTRL_AUTOSLPEN));
}

/**
 * @brief  Enable CAN0 SRAM retention mode.
 * @retval None
 */
__STATIC_INLINE void LL_PWR_Enable_AutoRET_CAN0SRAM(void)
{
    LL_REG_SBIT(SN_SCU->GMAC_SC, SCU_GMAC_SC_CAN0SRAMRET);
}

/**
 * @brief  Disable CAN0 SRAM retention mode.
 * @retval None
 */
__STATIC_INLINE void LL_PWR_Disable_AutoRET_CAN0SRAM(void)
{
    LL_REG_CBIT(SN_SCU->GMAC_SC, SCU_GMAC_SC_CAN0SRAMRET);
}

/**
 * @brief  Return if CAN0 SRAM retention mode is enabled.
 * @retval State of bit (1 or 0).
 */
__STATIC_INLINE uint32_t LL_PWR_IsEnabled_AutoRET_CAN0SRAM(void)
{
    return (LL_REG_RBIT(SN_SCU->GMAC_SC, SCU_GMAC_SC_CAN0SRAMRET) == (SCU_GMAC_SC_CAN0SRAMRET));
}

/**
 * @brief  Enable CAN1 SRAM retention mode.
 * @retval None
 */
__STATIC_INLINE void LL_PWR_Enable_AutoRET_CAN1SRAM(void)
{
    LL_REG_SBIT(SN_SCU->GMAC_SC, SCU_GMAC_SC_CAN1SRAMRET);
}

/**
 * @brief  Disable CAN1 SRAM retention mode.
 * @retval None
 */
__STATIC_INLINE void LL_PWR_Disable_AutoRET_CAN1SRAM(void)
{
    LL_REG_CBIT(SN_SCU->GMAC_SC, SCU_GMAC_SC_CAN1SRAMRET);
}

/**
 * @brief  Return if CAN1 SRAM retention mode is enabled.
 * @retval State of bit (1 or 0).
 */
__STATIC_INLINE uint32_t LL_PWR_IsEnabled_AutoRET_CAN1SRAM(void)
{
    return (LL_REG_RBIT(SN_SCU->GMAC_SC, SCU_GMAC_SC_CAN1SRAMRET) == (SCU_GMAC_SC_CAN1SRAMRET));
}

/**
 * @brief  Enable Ethernet MAC SRAM retention mode.
 * @retval None
 */
__STATIC_INLINE void LL_PWR_Enable_AutoRET_ETHSRAM(void)
{
    LL_REG_SBIT(SN_SCU->GMAC_SC, SCU_GMAC_SC_ETHSRAMRET);
}

/**
 * @brief  Disable Ethernet MAC SRAM retention mode.
 * @retval None
 */
__STATIC_INLINE void LL_PWR_Disable_AutoRET_ETHSRAM(void)
{
    LL_REG_CBIT(SN_SCU->GMAC_SC, SCU_GMAC_SC_ETHSRAMRET);
}

/**
 * @brief  Return if Ethernet MAC SRAM retention mode is enabled.
 * @retval State of bit (1 or 0).
 */
__STATIC_INLINE uint32_t LL_PWR_IsEnabled_AutoRET_ETHSRAM(void)
{
    return (LL_REG_RBIT(SN_SCU->GMAC_SC, SCU_GMAC_SC_ETHSRAMRET) == (SCU_GMAC_SC_ETHSRAMRET));
}

#ifdef __cplusplus
}
#endif

#endif /* _LL_PWR_H_ */
