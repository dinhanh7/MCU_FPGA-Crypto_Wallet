/***********************
 * SN34F78X SDK V2.1.4 *
 ***********************/

#ifndef _LL_TPM8_H_
#define _LL_TPM8_H_

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "ll_tpm.h"

/* private macro ------------------------------------------------------------*/
/// \cond
#define TPM_8_MCTRL_MRnIE(channel)   (1 << ((channel > 9 ? (channel - 10) : channel) * 3))
#define TPM_8_MCTRL_MRnRST(channel)  (2 << ((channel > 9 ? (channel - 10) : channel) * 3))
#define TPM_8_MCTRL_MRnSTOP(channel) (4 << ((channel > 9 ? (channel - 10) : channel) * 3))

#define TPM_8_EM_EMn(channel) (1 << channel)

#define TPM_8_PWMCTRL_PWMnEN(channel)   (1 << channel)
#define TPM_8_PWMCTRL_PWMnIOEN(channel) (1 << channel)
#define TPM_8_RIS_MRnIF(channel)        (1 << channel)
#define TPM_8_IC_MRnIC(channel)         (1 << channel)
/// \endcond
/* Exported types ------------------------------------------------------------*/
/* Exported constants --------------------------------------------------------*/

/**
 * \defgroup tpm8_pwm_pin_functionality CT16B8 PWM functionality when MRn=TC
 * \ingroup tpm_control
 * @{
 */
#define LL_TPM8_PWM_PIN_NONE   0 /*!< Do nothing */
#define LL_TPM8_PWM_PIN_LOW    1 /*!< CT16Bn_PWMn pin is Low when MR0=TC*/
#define LL_TPM8_PWM_PIN_HIGH   2 /*!< CT16Bn_PWMn pin is High when MR0=TC */
#define LL_TPM8_PWM_PIN_TOGGLE 3 /*!< Toggle CT16Bn_PWMn pin hen MR0=TC*/
/**
 * @}
 */

/*!< PWM output mode reference, n=0,1,2,3
    < PWM mode 1 :
       PWMx is 0 when TC<=MRx during Up-counting period
       PWMx is 0 when TC<=MRx during Down-counting period
    < PWM mode 2 :
       PWMx is 1 when TC<MRx during Up-counting period
       PWMx is 1 when TC��MRx during Down-counting period
    < PWM mode 3 :
       PWMx is forced to 0
    < PWM mode 4 :
       PWMx is forced to 1
*/
/**
 * \defgroup tpm8_pwm_mode CT16B8 PWM output mode
 * \ingroup tpm_control
 * @{
 */
#define LL_TPM8_PWM_MODE_1 0 /*!< PWM is 0 when TC <(<=) MR3 during Up-counting(Down-counting) period */
#define LL_TPM8_PWM_MODE_2 1 /*!< PWM is 1 when TC <(<=) MR3 during Up-counting(Down-counting) period */
#define LL_TPM8_PWM_MODE_3 2 /*!< PWM is forced to 0 */
#define LL_TPM8_PWM_MODE_4 3 /*!< PWM is forced to 1 */
/**
 * @}
 */

/* Exported macros --------------------------------------------------------*/

// tpm calibration times

/* Exported functions --------------------------------------------------------*/
/**
 * @brief  Enable Counter
 * @param  tpm TPM instance
 * @retval None
 */
__STATIC_INLINE void LL_TPM8_Enable(SN_CT16B8_Type *tpm)
{
    LL_REG_SBIT(tpm->TMRCTRL, TPM_34678_TMRCTRL_CEN);
}

/**
 * @brief  Disable Counter
 * @param  tpm TPM instance
 * @retval None
 */
__STATIC_INLINE void LL_TPM8_Disable(SN_CT16B8_Type *tpm)
{
    LL_REG_CBIT(tpm->TMRCTRL, TPM_34678_TMRCTRL_CEN);
}

/**
 * @brief  Judge if enabled TPM8
 * @param  tpm TPM instance
 * @retval 1: Enabled;   0: Disabled
 */
__STATIC_INLINE uint32_t LL_TPM8_IsEnabled(SN_CT16B8_Type *tpm)
{
    return ((LL_REG_READ(tpm->TMRCTRL) & TPM_34678_TMRCTRL_CEN) == TPM_34678_TMRCTRL_CEN);
}

/**
 * @brief  Reset TPM
 * @param  tpm TPM instance
 * @retval None
 */
__STATIC_INLINE void LL_TPM8_Reset(SN_CT16B8_Type *tpm)
{
    LL_REG_SBIT(tpm->TMRCTRL, TPM_34678_TMRCTRL_CRST);
}

/**
 * @brief  Get Reset State
 * @param  tpm TPM instance
 * @retval 0 : Reset finished,  1 : Resetting
 */
__STATIC_INLINE uint32_t LL_TPM8_GetResetState(SN_CT16B8_Type *tpm)
{
    return ((LL_REG_READ(tpm->TMRCTRL) & TPM_34678_TMRCTRL_CRST) >> TPM_34678_TMRCTRL_CRST_Pos);
}

/**
 * @brief  Set Counter value
 * @param  tpm TPM instance
 * @param  value Counter value range from 0x0 to 0xFFFF
 * @retval None
 */
__STATIC_INLINE void LL_TPM8_SetTimeCounter(SN_CT16B8_Type *tpm, uint32_t value)
{
    LL_REG_CBIT(tpm->TC, TPM_TC_TC);
    LL_REG_SBIT(tpm->TC, value << TPM_TC_TC_Pos);
}

/**
 * @brief  Get Counter value
 * @param  tpm TPM instance
 * @retval Counter value
 */
__STATIC_INLINE uint32_t LL_TPM8_GetTimeCounter(SN_CT16B8_Type *tpm)
{
    return ((LL_REG_READ(tpm->TC) & TPM_TC_TC) >> TPM_TC_TC_Pos);
}

/**
 * @brief  Set Prescaler value
 * @param  tpm TPM instance
 * @param  value Prescaler value range from 0x0 to 0xFF
 * @retval None
 */
__STATIC_INLINE void LL_TPM8_SetPrescaler(SN_CT16B8_Type *tpm, uint32_t value)
{
    LL_REG_CBIT(tpm->PRE, TPM_PRE_PRE);
    LL_REG_SBIT(tpm->PRE, value << TPM_PRE_PRE_Pos);
}

/**
 * @brief  Get Prescaler value
 * @param  tpm TPM instance
 * @retval Prescaler value
 */
__STATIC_INLINE uint32_t LL_TPM8_GetPrescaler(SN_CT16B8_Type *tpm)
{
    return ((LL_REG_READ(tpm->PRE) & TPM_PRE_PRE) >> TPM_PRE_PRE_Pos);
}

/**
 * @brief  Set PC Prescaler value
 * @param  tpm TPM instance
 * @param  value Prescaler value range from 0x0 to 0xFF
 * @retval None
 */
__STATIC_INLINE void LL_TPM8_SetPrescalerPC(SN_CT16B8_Type *tpm, uint32_t value)
{
    LL_REG_CBIT(tpm->PC, TPM_PC_PC);
    LL_REG_SBIT(tpm->PC, value << TPM_PC_PC_Pos);
}

/**
 * @brief  Get PC Prescaler value
 * @param  tpm TPM instance
 * @retval PC Prescaler value
 */
__STATIC_INLINE uint32_t LL_TPM8_GetPrescalerPC(SN_CT16B8_Type *tpm)
{
    return ((LL_REG_READ(tpm->PC) & TPM_PC_PC) >> TPM_PC_PC_Pos);
}

/**
 * @brief  Enable generating an interrupt when MRn matches TC
 * @param  tpm TPM instance
 * @param  channel channel number. \ref tpm_channel
 *          tpm 8 just support channel 0~11
 * @retval None
 */
__STATIC_INLINE void LL_TPM8_EnableIT_MRnIE(SN_CT16B8_Type *tpm, uint32_t channel)
{
    LL_REG_SBIT(*(&tpm->MCTRL + (channel / 10)), TPM_8_MCTRL_MRnIE(channel));
}

/**
 * @brief  Disable generating an interrupt when MRn matches TC
 * @param  tpm TPM instance
 * @param  channel channel number. \ref tpm_channel
 *          tpm 8 just support channel 0~11
 * @retval None
 */
__STATIC_INLINE void LL_TPM8_DisableIT_MRnIE(SN_CT16B8_Type *tpm, uint32_t channel)
{
    LL_REG_CBIT(*(&tpm->MCTRL + (channel / 10)), TPM_8_MCTRL_MRnIE(channel));
}

/**
 * @brief  Judge if MRnIE  Interrupt is enabled
 * @param  tpm TPM instance
 * @param  channel channel number. \ref tpm_channel
 *          tpm 8 just support channel 0~11
 * @retval 0: disabled, 1: enabled.
 */
__STATIC_INLINE uint32_t LL_TPM8_IsEnabledIT_MRnIE(SN_CT16B8_Type *tpm, uint32_t channel)
{
    return ((LL_REG_READ(*(&tpm->MCTRL + (channel / 10))) & TPM_8_MCTRL_MRnIE(channel)) == TPM_8_MCTRL_MRnIE(channel));
}

/**
 * @brief  Enable reset TC when MRn matches TC
 * @param  tpm TPM instance
 * @param  channel channel number. \ref tpm_channel
 *          tpm 8 just support channel 0~11
 * @retval None
 */
__STATIC_INLINE void LL_TPM8_Enable_MRnRST(SN_CT16B8_Type *tpm, uint32_t channel)
{
    LL_REG_SBIT(*(&tpm->MCTRL + (channel / 10)), TPM_8_MCTRL_MRnRST(channel));
}

/**
 * @brief  Disable reset TC when MRn matches TC
 * @param  tpm TPM instance
 * @param  channel channel number. \ref tpm_channel
 *          tpm 8 just support channel 0~11
 * @retval None
 */
__STATIC_INLINE void LL_TPM8_Disable_MRnRST(SN_CT16B8_Type *tpm, uint32_t channel)
{
    LL_REG_CBIT(*(&tpm->MCTRL + (channel / 10)), TPM_8_MCTRL_MRnRST(channel));
}

/**
 * @brief  Judge if MRnRST is enabled
 * @param  tpm TPM instance
 * @param  channel channel number. \ref tpm_channel
 *          tpm 8 just support channel 0~11
 * @retval 1: Enabled,   0: Disabled
 */
__STATIC_INLINE uint32_t LL_TPM8_IsEnabled_MRnRST(SN_CT16B8_Type *tpm, uint32_t channel)
{
    return ((LL_REG_READ(*(&tpm->MCTRL + (channel / 10))) & TPM_8_MCTRL_MRnRST(channel)) == TPM_8_MCTRL_MRnRST(channel));
}

/**
 * @brief  Stop TC and PC and clear CEN bit when MRn matches TC
 * @param  tpm TPM instance
 * @param  channel channel number. \ref tpm_channel
 *          tpm 8 just support channel 0~11
 * @retval None
 */
__STATIC_INLINE void LL_TPM8_Enable_MRnSTOP(SN_CT16B8_Type *tpm, uint32_t channel)
{
    LL_REG_SBIT(*(&tpm->MCTRL + (channel / 10)), TPM_8_MCTRL_MRnSTOP(channel));
}

/**
 * @brief  Disable MRnSTOP
 * @param  tpm TPM instance
 * @param  channel channel number. \ref tpm_channel
 *          tpm 8 just support channel 0~11
 * @retval None
 */
__STATIC_INLINE void LL_TPM8_Disable_MRnSTOP(SN_CT16B8_Type *tpm, uint32_t channel)
{
    LL_REG_CBIT(*(&tpm->MCTRL + (channel / 10)), TPM_8_MCTRL_MRnSTOP(channel));
}

/**
 * @brief  Judge if MRnSTOP is enabled
 * @param  tpm TPM instance
 * @param  channel channel number. \ref tpm_channel
 *          tpm 8 just support channel 0~11
 * @retval 0: disabled, 1: enabled.
 */
__STATIC_INLINE uint32_t LL_TPM8_IsEnabled_MRnSTOP(SN_CT16B8_Type *tpm, uint32_t channel)
{
    return ((LL_REG_READ(*(&tpm->MCTRL + (channel / 10))) & TPM_8_MCTRL_MRnSTOP(channel)) == TPM_8_MCTRL_MRnSTOP(channel));
}

/**
 * @brief  Enable generating an interrupt based on CM[2:0] when MR_PERIOD matches the value in the TC
 * @param  tpm TPM instance
 * @retval None
 */
__STATIC_INLINE void LL_TPM8_Enable_MRPERIODIE(SN_CT16B8_Type *tpm)
{
    LL_REG_SBIT(tpm->MCTRL2, TPM_8_MCTRL2_MRPERIODIE);
}

/**
 * @brief  Disable MRPERIODIE
 * @param  tpm TPM instance
 * @retval None
 */
__STATIC_INLINE void LL_TPM8_Disable_MRPERIODIE(SN_CT16B8_Type *tpm)
{
    LL_REG_CBIT(tpm->MCTRL2, TPM_8_MCTRL2_MRPERIODIE);
}

/**
 * @brief  Judge if MRPERIODIE is enabled
 * @param  tpm TPM instance
 * @retval 0: disabled, 1: enabled.
 */
__STATIC_INLINE uint32_t LL_TPM8_IsEnabled_MRPERIODIE(SN_CT16B8_Type *tpm)
{
    return ((LL_REG_READ(tpm->MCTRL2) & TPM_8_MCTRL2_MRPERIODIE) == TPM_8_MCTRL2_MRPERIODIE);
}

/**
 * @brief  Enable reset TC when MR_PERIOD matches TC
 * @param  tpm TPM instance
 * @retval None
 */
__STATIC_INLINE void LL_TPM8_Enable_MRPERIODRST(SN_CT16B8_Type *tpm)
{
    LL_REG_SBIT(tpm->MCTRL2, TPM_8_MCTRL2_MRPERIODRST);
}

/**
 * @brief  Disable MRPERIODRST
 * @param  tpm TPM instance
 * @retval None
 */
__STATIC_INLINE void LL_TPM8_Disable_MRPERIODRST(SN_CT16B8_Type *tpm)
{
    LL_REG_CBIT(tpm->MCTRL2, TPM_8_MCTRL2_MRPERIODRST);
}

/**
 * @brief  Judge if MRPERIODRST is enabled
 * @param  tpm TPM instance
 * @retval 0: disabled, 1: enabled.
 */
__STATIC_INLINE uint32_t LL_TPM8_IsEnabled_MRPERIODRST(SN_CT16B8_Type *tpm)
{
    return ((LL_REG_READ(tpm->MCTRL2) & TPM_8_MCTRL2_MRPERIODRST) == TPM_8_MCTRL2_MRPERIODRST);
}

/**
 * @brief  Stop MR_PERIOD: TC and PC will stop and CEN bit will be cleared if MR_PERIOD matches TC
 * @param  tpm TPM instance
 * @retval None
 */
__STATIC_INLINE void LL_TPM8_Enable_MRPERIODSTOP(SN_CT16B8_Type *tpm)
{
    LL_REG_SBIT(tpm->MCTRL2, TPM_8_MCTRL2_MRPERIODSTOP);
}

/**
 * @brief  Disable MRPERIODSTOP
 * @param  tpm TPM instance
 * @retval None
 */
__STATIC_INLINE void LL_TPM8_Disable_MRPERIODSTOP(SN_CT16B8_Type *tpm)
{
    LL_REG_CBIT(tpm->MCTRL2, TPM_8_MCTRL2_MRPERIODSTOP);
}

/**
 * @brief  Judge if MRPERIODSTOP is enabled
 * @param  tpm TPM instance
 * @retval 0: disabled, 1: enabled.
 */
__STATIC_INLINE uint32_t LL_TPM8_IsEnabled_MRPERIODSTOP(SN_CT16B8_Type *tpm)
{
    return ((LL_REG_READ(tpm->MCTRL2) & TPM_8_MCTRL2_MRPERIODSTOP) == TPM_8_MCTRL2_MRPERIODSTOP);
}

/**
 * @brief  Set MRn (n = 0,1,...,11) Timer counter match value
 * @param  tpm TPM instance
 * @param  mrx which MR number to set, range from 0 to 11
 * @param  value Timer counter match value
 * @retval None
 */
__STATIC_INLINE void LL_TPM8_SetTCMatchValueToMRn(SN_CT16B8_Type *tpm, uint32_t mrx, uint32_t value)
{
    LL_REG_CBIT(tpm->MR0, TPM_8_MR0To11_MR);
    LL_REG_SBIT(*(&tpm->MR0 + mrx), value << TPM_8_MR0To11_MR_Pos);
}

/**
 * @brief  Set MRx (x = 0,1,...,11) Timer counter match value
 * @param  tpm TPM instance
 * @param  value Timer counter match value
 * @retval None
 */
__STATIC_INLINE void LL_TPM8_SetTCMatchValueToMRPERIOD(SN_CT16B8_Type *tpm, uint32_t value)
{
    LL_REG_CBIT(tpm->MR_PERIOD, TPM_8_MRPERIOD_MR);
    LL_REG_SBIT(tpm->MR_PERIOD, value << TPM_8_MRPERIOD_MR_Pos);
}

/**
 * @brief  When EMC0!=00b and MRn!=TC, it will drive the state of CT16Bn_PWM0 output High
 * @param  tpm TPM instance
 * @param  channel channel number. \ref tpm_channel
 *          tpm 8 just support channel 0~11
 * @retval None
 */
__STATIC_INLINE void LL_TPM8_SetPWMnHigh(SN_CT16B8_Type *tpm, uint32_t channel)
{
    LL_REG_SBIT(tpm->EM, TPM_8_EM_EMn(channel));
}

/**
 * @brief  When EMC0!=00b and MRn!=TC, it will drive the state of CT16Bn_PWM0 output Low
 * @param  tpm TPM instance
 * @param  channel channel number. \ref tpm_channel
 *          tpm 8 just support channel 0~11
 * @retval None
 */
__STATIC_INLINE void LL_TPM8_SetPWMnLow(SN_CT16B8_Type *tpm, uint32_t channel)
{
    LL_REG_CBIT(tpm->EM, TPM_8_EM_EMn(channel));
}

/**
 * @brief  Get CT16Bn_PWM0 output state when EMC0!=00b and MRn!=TC
 * @param  tpm TPM instance
 * @param  channel channel number. \ref tpm_channel
 *          tpm 8 just support channel 0~11
 * @retval 1: high, 0: low
 */
__STATIC_INLINE uint32_t LL_TPM8_GetPWMnState(SN_CT16B8_Type *tpm, uint32_t channel)
{
    return ((LL_REG_READ(tpm->EM) & TPM_8_EM_EMn(channel)) >> TPM_8_EM_EMn(channel));
}

/**
 * @brief  Set CT16Bn_PWM0 functionality when MRn=TC
 * @param  tpm TPM instance
 * @param  channel channel number. \ref tpm_channel
 *          tpm 8 just support channel 0~11
 * @param  function PWM0 functionality \ref tpm8_pwm_pin_functionality
 * @retval None
 */
__STATIC_INLINE void LL_TPM8_SetPWMnFunction(SN_CT16B8_Type *tpm, uint32_t function, uint32_t channel)
{
    LL_REG_CBIT(tpm->EMC, 0x03 << (channel * 2));
    LL_REG_SBIT(tpm->EMC, function << (channel * 2));
}

/**
 * @brief  Get CT16Bn_PWM0 functionality when MRn=TC
 * @param  tpm TPM instance
 * @param  channel channel number. \ref tpm_channel
 *          tpm 8 just support channel 0~11
 * @retval
 *         0: Do nothing
 *         1: CT16Bn_PWM0 pin is low when MRn=TC
 *         2: CT16Bn_PWM0 pin is high when MRn=TC
 *         3: CT16Bn_PWM0 pin is toggled when MRn=TC
 */
__STATIC_INLINE uint32_t LL_TPM8_GetPWMnFunction(SN_CT16B8_Type *tpm, uint32_t channel)
{
    return ((LL_REG_READ(tpm->EMC) & (0x03 << (channel * 2))) >> (channel * 2));
}

/**
 * @brief  Set PWM mode
 * @param  tpm TPM instance
 * @param  mode PWMn mode \ref tpm8_pwm_mode
 * @param  channel channel number. \ref tpm_channel
 *          tpm 8 just support channel 0~11
 * @retval None
 */
__STATIC_INLINE void LL_TPM8_SetPWMnMode(SN_CT16B8_Type *tpm, uint32_t mode, uint32_t channel)
{
    LL_REG_CBIT(tpm->PWMCTRL, 0x03 << (channel * 2));
    LL_REG_SBIT(tpm->PWMCTRL, mode << (channel * 2));
}

/**
 * @brief  Get PWMn mode
 * @param  tpm TPM instance
 * @param  channel channel number. \ref tpm_channel
 *          tpm 8 just support channel 0~11
 * @retval PWMn mode \ref tpm8_pwm_mode
 */
__STATIC_INLINE uint32_t LL_TPM8_GetPWMnMode(SN_CT16B8_Type *tpm, uint32_t channel)
{
    return ((LL_REG_READ(tpm->PWMCTRL) & (0x03 << (channel * 2))) >> (channel * 2));
}

/**
 * @brief  Enable PWMn
 * @param  tpm TPM instance
 * @param  channel channel number. \ref tpm_channel
 *          tpm 8 just support channel 0~11
 * @retval None
 */
__STATIC_INLINE void LL_TPM8_Enable_PWMn(SN_CT16B8_Type *tpm, uint32_t channel)
{
    LL_REG_SBIT(tpm->PWMENB, TPM_8_PWMCTRL_PWMnEN(channel));
}

/**
 * @brief  Disable PWMn
 * @param  tpm TPM instance
 * @param  channel channel number. \ref tpm_channel
 *          tpm 8 just support channel 0~11
 * @retval None
 */
__STATIC_INLINE void LL_TPM8_Disable_PWMn(SN_CT16B8_Type *tpm, uint32_t channel)
{
    LL_REG_CBIT(tpm->PWMENB, TPM_8_PWMCTRL_PWMnEN(channel));
}

/**
 * @brief  Judge if enabled PWMn
 * @param  tpm TPM instance
 * @param  channel channel number. \ref tpm_channel
 *          tpm 8 just support channel 0~11
 * @retval 1: Enabled;   0: Disabled
 */
__STATIC_INLINE uint32_t LL_TPM8_IsEnabled_PWMn(SN_CT16B8_Type *tpm, uint32_t channel)
{
    return ((LL_REG_READ(tpm->PWMENB) & TPM_8_PWMCTRL_PWMnEN(channel)) == TPM_8_PWMCTRL_PWMnEN(channel));
}

/**
 * @brief  Set CT16Bn_PWMn pin act as match output, and output depends on PWMnEN bit
 * @param  tpm TPM instance
 * @param  channel channel number. \ref tpm_channel
 *          tpm 8 just support channel 0~11
 * @retval None
 */
__STATIC_INLINE void LL_TPM8_Enable_PWMnIOEN(SN_CT16B8_Type *tpm, uint32_t channel)
{
    LL_REG_SBIT(tpm->PWMIOENB, TPM_8_PWMCTRL_PWMnIOEN(channel));
}

/**
 * @brief  Set CT16Bn_PWMn pin is act as GPIO
 * @param  tpm TPM instance
 * @param  channel channel number. \ref tpm_channel
 *          tpm 8 just support channel 0~11
 * @retval None
 */
__STATIC_INLINE void LL_TPM8_Disable_PWMnIOEN(SN_CT16B8_Type *tpm, uint32_t channel)
{
    LL_REG_CBIT(tpm->PWMIOENB, TPM_8_PWMCTRL_PWMnIOEN(channel));
}

/**
 * @brief  Judge if enabled PWMnIOEN
 * @param  tpm TPM instance
 * @param  channel channel number. \ref tpm_channel
 *          tpm 8 just support channel 0~11
 * @retval 1: Enabled;   0: Disabled
 */
__STATIC_INLINE uint32_t LL_TPM8_IsEnabled_PWMnIOEN(SN_CT16B8_Type *tpm, uint32_t channel)
{
    return ((LL_REG_READ(tpm->PWMIOENB) & TPM_8_PWMCTRL_PWMnIOEN(channel)) == TPM_8_PWMCTRL_PWMnIOEN(channel));
}

/**
 * @brief  Judge if has Match channel n interrupt flag
 * @param  tpm TPM instance
 * @param  channel channel number. \ref tpm_channel
 *          tpm 8 just support channel 0~11
 * @retval 0: No interrupt on match channel n
 *         1: Interrupt requirements met on match channel n.
 */
__STATIC_INLINE uint32_t LL_TPM8_IsActiveFlag_MRnIF(SN_CT16B8_Type *tpm, uint32_t channel)
{
    return ((LL_REG_READ(tpm->RIS) & TPM_8_RIS_MRnIF(channel)) == TPM_8_RIS_MRnIF(channel));
}

/**
 * @brief  Judge if has Match channel of Period interrupt flag
 * @param  tpm TPM instance
 * @retval 0: No Match condition occurs
 *         1: Match condition occurs
 */
__STATIC_INLINE uint32_t LL_TPM8_IsActiveFlag_MRPERIODIF(SN_CT16B8_Type *tpm)
{
    return ((LL_REG_READ(tpm->RIS) & TPM_8_RIS_MRPERIODIF) == TPM_8_RIS_MRPERIODIF);
}

/**
 * @brief  Clear MRnIF
 * @param  tpm TPM instance
 * @param  channel channel number. \ref tpm_channel
 *          tpm8 supports channel 0~11 and period
 * @retval None
 */
__STATIC_INLINE void LL_TPM8_ClearFlag_MRnIF(SN_CT16B8_Type *tpm, uint32_t channel)
{
    LL_REG_SBIT(tpm->IC, TPM_8_IC_MRnIC(channel));
}

/**
 * @brief  Clear MRPERIODIF
 * @param  tpm TPM instance
 * @retval None
 */
__STATIC_INLINE void LL_TPM8_ClearFlag_MRPERIODIF(SN_CT16B8_Type *tpm)
{
    LL_REG_SBIT(tpm->IC, TPM_8_IC_MRPERIODIC);
}

/**
 * @brief  MR0 DMA request active enable
 * @param  tpm TPM instance
 * @retval None
 */
__STATIC_INLINE void LL_TPM8_Enable_DMAMRPERIOD(SN_CT16B8_Type *tpm)
{
    LL_REG_SBIT(tpm->DMA, TPM_8_DMA_DMAMRPERIOD);
}
/**
 * @brief  MR0 DMA request active disable
 * @param  tpm TPM instance
 * @retval None
 */
__STATIC_INLINE void LL_TPM8_Disable_DMAMRPERIOD(SN_CT16B8_Type *tpm)
{
    LL_REG_CBIT(tpm->DMA, TPM_8_DMA_DMAMRPERIOD);
}
/**
 * @brief  Judge if enabled MR0 DMA request active
 * @param  tpm TPM instance
 * @retval 1: Enabled;   0: Disabled
 */
__STATIC_INLINE uint32_t LL_TPM8_IsEnabled_DMAMRPERIOD(SN_CT16B8_Type *tpm)
{
    return ((LL_REG_READ(tpm->DMA) & TPM_8_DMA_DMAMRPERIOD) == TPM_8_DMA_DMAMRPERIOD);
}

/**
 * @brief  Set MRn alias for DMA access
 * @param  tpm TPM instance
 * @param  channel channel number \ref tpm_channel
 *         tpm 8 just supports channel 0~11
 * @param  alias alias
 * @retval None
 */
__STATIC_INLINE void LL_TPM8_SetMRnAlias(SN_CT16B8_Type *tpm, uint16_t alias, uint32_t channel)
{
    LL_REG_CBIT(*(&tpm->DMAMRA1 + channel * 2), 0xFFFF);
    LL_REG_SBIT(*(&tpm->DMAMRA1 + channel * 2), alias << 0U);
}

#ifdef __cplusplus
}
#endif

#endif /* _LL_tpm_H_ */
