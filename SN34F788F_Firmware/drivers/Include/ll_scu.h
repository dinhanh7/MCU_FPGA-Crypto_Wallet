/***********************
 * SN34F78X SDK V2.1.4 *
 ***********************/

#ifndef _LL_SCU_H_
#define _LL_SCU_H_

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "sn34f78x_ll_def.h"
#include "ll_reg_msk.h"

/* Exported types ------------------------------------------------------------*/
/* Exported constants --------------------------------------------------------*/

/* Exported functions --------------------------------------------------------*/

/**
 * @brief  Check enters Deep sleep mode wakeup interrupt status
 * @retval Deep sleep mode wakeup interrupt status.
 */
__STATIC_INLINE uint32_t LL_SCU_IsActiveFlag_DS_WAKEUP(void)
{
    return (LL_REG_RBIT(SN_SCU->RIS, SCU_RIS_INT_DS_WAKEUP) == SCU_RIS_INT_DS_WAKEUP);
}

/**
 * @brief  clear enters deep sleep mode wakeup interrupt status
 * @retval None.
 */
__STATIC_INLINE void LL_SCU_ClearFlag_DS_WAKEUP(void)
{
    LL_REG_WRITE(SN_SCU->RIS, SCU_RIS_INT_DS_WAKEUP);
}

/**
 * @brief  Check enters sleep mode wakeup interrupt status
 * @retval Sleep mode wakeup interrupt status.
 */
__STATIC_INLINE uint32_t LL_SCU_IsActiveFlag_WAKEUP(void)
{
    return (LL_REG_RBIT(SN_SCU->RIS, SCU_RIS_INT_WAKEUP) == SCU_RIS_INT_WAKEUP);
}

/**
 * @brief  clear enters sleep mode wakeup interrupt status
 * @retval None.
 */
__STATIC_INLINE void LL_SCU_ClearFlag_WAKEUP(void)
{
    LL_REG_WRITE(SN_SCU->RIS, SCU_RIS_INT_WAKEUP);
}

/**
 * @brief  Check enters FCS command finish interrupt status
 * @retval FCS command finish interrupt status.
 */
__STATIC_INLINE uint32_t LL_SCU_IsActiveFlag_FCS(void)
{
    return (LL_REG_RBIT(SN_SCU->RIS, SCU_RIS_INT_FCS) == SCU_RIS_INT_FCS);
}

/**
 * @brief  clear enters FCS command finish interrupt status
 * @retval None.
 */
__STATIC_INLINE void LL_SCU_ClearFlag_FCS(void)
{
    LL_REG_WRITE(SN_SCU->RIS, SCU_RIS_INT_FCS);
}

/**
 * @brief  Check enters Remap is changed status after reboot command
 * @retval Remap is changed status after reboot command.
 */
__STATIC_INLINE uint32_t LL_SCU_IsActiveFlag_REMAPCHG(void)
{
    return (LL_REG_RBIT(SN_SCU->RIS, SCU_RIS_INT_REMAPCHG) == SCU_RIS_INT_REMAPCHG);
}

/**
 * @brief  clear enters Remap is changed status after reboot command
 * @retval None.
 */
__STATIC_INLINE void LL_SCU_ClearFlag_REMAPCHG(void)
{
    LL_REG_WRITE(SN_SCU->RIS, SCU_RIS_INT_REMAPCHG);
}

/**
 * @brief  Check RTC enters  alarm interrupt status
 * @retval RTC alarm interrupt status.
 */
__STATIC_INLINE uint32_t LL_SCU_IsActiveFlag_RTC_ALARM(void)
{
    return (LL_REG_RBIT(SN_SCU->RIS, SCU_RIS_INT_RTC_ALARM) == SCU_RIS_INT_RTC_ALARM);
}

/**
 * @brief  clear RTC enters RTC alarm interrupt status
 * @retval None.
 */
__STATIC_INLINE void LL_SCU_ClearFlag_RTC_ALARM(void)
{
    LL_REG_WRITE(SN_SCU->RIS, SCU_RIS_INT_RTC_ALARM);
}

/**
 * @brief  Check RTC enters  RTC periodic interrupt status
 * @retval RTC periodic interrupt status.
 */
__STATIC_INLINE uint32_t LL_SCU_IsActiveFlag_RTC_PER(void)
{
    return (LL_REG_RBIT(SN_SCU->RIS, SCU_RIS_INT_RTC_PER) == SCU_RIS_INT_RTC_PER);
}

/**
 * @brief  clear RTC enters RTC periodic interrupt status
 * @retval None.
 */
__STATIC_INLINE void LL_SCU_ClearFlag_RTC_PER(void)
{
    LL_REG_WRITE(SN_SCU->RIS, SCU_RIS_INT_RTC_PER);
}

/**
 * @brief  Check RTC enters  RTC second out interrupt status
 * @retval RTC second out interrupt status.
 */
__STATIC_INLINE uint32_t LL_SCU_IsActiveFlag_RTC_SEC(void)
{
    return (LL_REG_RBIT(SN_SCU->RIS, SCU_RIS_INT_RTC_SEC) == SCU_RIS_INT_RTC_SEC);
}

/**
 * @brief  clear RTC enters RTC second out interrupt status
 * @retval None.
 */
__STATIC_INLINE void LL_SCU_ClearFlag_RTC_SEC(void)
{
    LL_REG_WRITE(SN_SCU->RIS, SCU_RIS_INT_RTC_SEC);
}

/**
 * @brief  Enable Deep sleep mode Wake-up event interrupt enable bit
 * @retval None.
 */
__STATIC_INLINE void LL_SCU_EnableIT_DS_WAKEUP(void)
{
    LL_REG_SBIT(SN_SCU->IE, SCU_IE_DS_WAKEUP_EINT);
}

/**
 * @brief  Disable Deep sleep mode Wake-up event interrupt enable bit
 * @retval None.
 */
__STATIC_INLINE void LL_SCU_DisableIT_DS_WAKEUP(void)
{
    LL_REG_CBIT(SN_SCU->IE, SCU_IE_DS_WAKEUP_EINT);
}

/**
 * @brief  Determine if Deep sleep mode Wake-up event interrupt is enabled
 * @retval Deep sleep mode Wake-up event interrupt enable state.
 */
__STATIC_INLINE uint32_t LL_SCU_IsEnabledIT_DS_WAKEUP(void)
{
    return (LL_REG_RBIT(SN_SCU->IE, SCU_IE_DS_WAKEUP_EINT) == SCU_IE_DS_WAKEUP_EINT);
}

/**
 * @brief  Enable sleep mode Wake-up event interrupt enable bit
 * @retval None.
 */
__STATIC_INLINE void LL_SCU_EnableIT_WAKEUP(void)
{
    LL_REG_SBIT(SN_SCU->IE, SCU_IE_WAKEUP_EINT);
}

/**
 * @brief  Disable sleep mode Wake-up event interrupt enable bit
 * @retval None.
 */
__STATIC_INLINE void LL_SCU_DisableIT_WAKEUP(void)
{
    LL_REG_CBIT(SN_SCU->IE, SCU_IE_WAKEUP_EINT);
}

/**
 * @brief  Determine if sleep mode Wake-up event interrupt is enabled
 * @retval sleep mode Wake-up event interrupt enable state.
 */
__STATIC_INLINE uint32_t LL_SCU_IsEnabledIT_WAKEUP(void)
{
    return (LL_REG_RBIT(SN_SCU->IE, SCU_IE_WAKEUP_EINT) == SCU_IE_WAKEUP_EINT);
}

/**
 * @brief  Enable FCS command finish interrupt
 * @retval None.
 */
__STATIC_INLINE void LL_SCU_EnableIT_FCS(void)
{
    LL_REG_SBIT(SN_SCU->IE, SCU_IE_FCS_EINT);
}

/**
 * @brief  Disable FCS command finish interrupt
 * @retval None.
 */
__STATIC_INLINE void LL_SCU_DisableIT_FCS(void)
{
    LL_REG_CBIT(SN_SCU->IE, SCU_IE_FCS_EINT);
}

/**
 * @brief  Determine if FCS command finish interrupt is enabled
 * @retval FCS command finish interrupt enable state.
 */
__STATIC_INLINE uint32_t LL_SCU_IsEnabledIT_FCS(void)
{
    return (LL_REG_RBIT(SN_SCU->IE, SCU_IE_FCS_EINT) == SCU_IE_FCS_EINT);
}

/**
 * @brief  Enable REMAP change interrupt
 * @retval None.
 */
__STATIC_INLINE void LL_SCU_EnableIT_REMAPCHG(void)
{
    LL_REG_SBIT(SN_SCU->IE, SCU_IE_REMAPCHG_EINT);
}

/**
 * @brief  Disable REMAP change interrupt
 * @retval None.
 */
__STATIC_INLINE void LL_SCU_DisableIT_REMAPCHG(void)
{
    LL_REG_CBIT(SN_SCU->IE, SCU_IE_REMAPCHG_EINT);
}

/**
 * @brief  Determine if REMAP change interrupt is enabled
 * @retval REMAP change interrupt enable state.
 */
__STATIC_INLINE uint32_t LL_SCU_IsEnabledIT_REMAPCHG(void)
{
    return (LL_REG_RBIT(SN_SCU->IE, SCU_IE_REMAPCHG_EINT) == SCU_IE_REMAPCHG_EINT);
}

/**
 * @brief  Enable RTC alarm interrupt
 * @retval None.
 */
__STATIC_INLINE void LL_SCU_EnableIT_RTC_ALARM(void)
{
    LL_REG_SBIT(SN_SCU->IE, SCU_IE_RTC_ALARM_EINT);
}

/**
 * @brief  Disable RTC alarm interrupt
 * @retval None.
 */
__STATIC_INLINE void LL_SCU_DisableIT_RTC_ALARM(void)
{
    LL_REG_CBIT(SN_SCU->IE, SCU_IE_RTC_ALARM_EINT);
}

/**
 * @brief  Determine if RTC alarm interrupt is enabled
 * @retval RTC alarm interrupt enable state.
 */
__STATIC_INLINE uint32_t LL_SCU_IsEnabledIT_RTC_ALARM(void)
{
    return (LL_REG_RBIT(SN_SCU->IE, SCU_IE_RTC_ALARM_EINT) == SCU_IE_RTC_ALARM_EINT);
}

/**
 * @brief  Enable RTC periodic interrupt
 * @retval None.
 */
__STATIC_INLINE void LL_SCU_EnableIT_RTC_PER(void)
{
    LL_REG_SBIT(SN_SCU->IE, SCU_IE_RTC_PER_EINT);
}

/**
 * @brief  Disable RTC periodic interrupt
 * @retval None.
 */
__STATIC_INLINE void LL_SCU_DisableIT_RTC_PER(void)
{
    LL_REG_CBIT(SN_SCU->IE, SCU_IE_RTC_PER_EINT);
}

/**
 * @brief  Determine if RTC periodic interrupt is enabled
 * @retval RTC periodic interrupt enable state.
 */
__STATIC_INLINE uint32_t LL_SCU_IsEnabledIT_RTC_PER(void)
{
    return (LL_REG_RBIT(SN_SCU->IE, SCU_IE_RTC_PER_EINT) == SCU_IE_RTC_PER_EINT);
}

/**
 * @brief  Enable RTC second interrupt
 * @retval None.
 */
__STATIC_INLINE void LL_SCU_EnableIT_RTC_SEC(void)
{
    LL_REG_SBIT(SN_SCU->IE, SCU_IE_RTC_SEC_EINT);
}

/**
 * @brief  Disable RTC second interrupt
 * @retval None.
 */
__STATIC_INLINE void LL_SCU_DisableIT_RTC_SEC(void)
{
    LL_REG_CBIT(SN_SCU->IE, SCU_IE_RTC_SEC_EINT);
}

/**
 * @brief  Determine if RTC second interrupt is enabled
 * @retval RTC second interrupt enable state.
 */
__STATIC_INLINE uint32_t LL_SCU_IsEnabledIT_RTC_SEC(void)
{
    return (LL_REG_RBIT(SN_SCU->IE, SCU_IE_RTC_SEC_EINT) == SCU_IE_RTC_SEC_EINT);
}

/**
 * @brief  get RTC Chip ID register
 * @retval chip id
 */
__STATIC_INLINE uint32_t LL_SCU_GetChipID(void)
{
    return ((LL_REG_READ(SN_SCU->CHIPID) & SCU_CHIPID_CHIP_ID) >> SCU_CHIPID_CHIP_ID_Pos);
}

/**
 * @brief  Get RTC VERSION register
 * @retval chip version
 */
__STATIC_INLINE uint32_t LL_SCU_GetVersion(void)
{
    return ((LL_REG_READ(SN_SCU->VERSION) & SCU_VERSION_VERSION) >> SCU_VERSION_VERSION_Pos);
}

/**
 * @brief  Enable eFlash standby.
 * @retval None
 */
__STATIC_INLINE void LL_SCU_EnableEFCSTB(void)
{
    LL_REG_SBIT(SN_SCU->PWRMODE, SCU_PWRMODE_EFC_STB_OFF);
}

/**
 * @brief  Disable eFlash standby.
 * @retval None
 */
__STATIC_INLINE void LL_SCU_DisableEFCSTB(void)
{
    LL_REG_CBIT(SN_SCU->PWRMODE, SCU_PWRMODE_EFC_STB_OFF);
}

/**
 * @brief  Return if eFlash standby is enabled.
 * @retval State of bit (1 or 0).
 */
__STATIC_INLINE uint32_t LL_SCU_IsEnabledEFCSTB(void)
{
    return !(LL_REG_RBIT(SN_SCU->PWRMODE, SCU_PWRMODE_EFC_STB_OFF) == (SCU_PWRMODE_EFC_STB_OFF));
}

#if defined(USE_FULL_LL_DRIVER)
void LL_PWR_Callback(uint32_t ris);
void LL_RCC_Callback(uint32_t ris);
void LL_RTC_Callback(uint32_t ris);
void LL_SCU_IRQHandler(void);
#endif /* USE_FULL_LL_DRIVER */

#ifdef __cplusplus
}
#endif

#endif /* _LL_SCU_H_ */
