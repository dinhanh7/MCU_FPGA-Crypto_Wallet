/***********************
 * SN34F78X SDK V2.1.4 *
 ***********************/

#ifndef _LL_TPM3_H_
#define _LL_TPM3_H_

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "ll_tpm.h"

/* Exported types ------------------------------------------------------------*/

/* Exported constants --------------------------------------------------------*/

/**
 * \defgroup tpm3_pwm_pin_functionality CT16B3 PWM functionality when MRn=TC
 * \ingroup tpm_control
 * @{
 */
#define LL_TPM3_PWM_PIN_NONE   (0x00) /*!< Do nothing */
#define LL_TPM3_PWM_PIN_LOW    (0x01) /*!< CT16Bn_PWMn pin is Low when MRn=TC*/
#define LL_TPM3_PWM_PIN_HIGH   (0x02) /*!< CT16Bn_PWMn pin is High when MRn=TC */
#define LL_TPM3_PWM_PIN_TOGGLE (0x03) /*!< Toggle CT16Bn_PWMn pin hen MRn=TC*/
/**
 * @}
 */

/*!< PWM output mode reference, n=0,1,2,3
    < PWM mode 1 :
       PWMx is 0 when TC<=MRx during Up-counting period
       PWMx is 0 when TC<=MRx during Down-counting period
    < PWM mode 2 :
       PWMx is 1 when TC<MRx during Up-counting period
       PWMx is 1 when TC<=MRx during Down-counting period
    < PWM mode 3 :
       PWMx is forced to 0
    < PWM mode 4 :
       PWMx is forced to 1
*/
/**
 * \defgroup tpm3_pwm_mode CT16B3 PWM output mode
 * \ingroup tpm_control
 * @{
 */
#define LL_TPM3_PWM_MODE_1 (0x00) /*!< PWM is 0 when TC <(<=) MR3 during Up-counting(Down-counting) period */
#define LL_TPM3_PWM_MODE_2 (0x01) /*!< PWM is 1 when TC <(<=) MR3 during Up-counting(Down-counting) period */
#define LL_TPM3_PWM_MODE_3 (0x02) /*!< PWM is forced to 0 */
#define LL_TPM3_PWM_MODE_4 (0x03) /*!< PWM is forced to 1 */
/**
 * @}
 */

/**
 * \defgroup tpm3_pwmn_gpio_selection CT16B3 PWMN/GPIO selection
 * \ingroup tpm_control
 * @{
 */
#define LL_TPM3_PWMN_SEL_GPIO         (0X00) /*!< CT16Bn_PWMN pin is act as GPIO */
#define LL_TPM3_PWMN_SEL_INVERSE_HIGH (0X01) /*!< PWMN pin outputs the inverse signal with dead-band of PWM,but same High during dead-band period */
#define LL_TPM3_PWMN_SEL_INVERSE_LOW  (0X02) /*!< PWMN pin outputs the inverse signal with dead-band of PWM,but same Low during dead-band period */
#define LL_TPM3_PWMN_SEL_SAME         (0X03) /*!< PWMN pin outputs the same signal with dead-band of PWM */
/**
 * @}
 */

/* Exported macros --------------------------------------------------------*/

/* Private Macro -------------------------------------------------------------*/
/// \cond
#define TPM_3_MCTRL_MRnIE(channel)   ((channel * 3 + 0 + (channel == 9 ? 2 : 0)))
#define TPM_3_MCTRL_MRnRST(channel)  ((channel * 3 + 1 + (channel == 9 ? 2 : 0)))
#define TPM_3_MCTRL_MRnSTOP(channel) ((channel * 3 + 2 + (channel == 9 ? 2 : 0)))

#define TPM_3_EM_EMn(channel)  (channel)
#define TPM_3_EM_EMCn(channel) (channel * 2 + 4)

#define TPM_3_PWMCTRL_PWMnEN(channel)    (channel)
#define TPM_3_PWMCTRL_PWMnMODE(channel)  (channel * 2 + 4)
#define TPM_3_PWMCTRL_PWMnIOEN(channel)  (channel + 20)
#define TPM_3_PWMCTRL_PWMnNIOEN(channel) (channel * 2 + 24)

#define TPM_3_RIS_MRnIF(channel) (channel + ((channel == 9 ? -6 : 0)))

#define TPM_3_IC_MRnIC(channel) (channel + ((channel == 9 ? -6 : 0)))

#define TPM_3_DMA_DMA_MRn(channel) (channel + ((channel == 9 ? -6 : 0)))
/// \endcond

/* Exported functions --------------------------------------------------------*/
/**
 * @brief  Enable Counter
 * @param  tpm TPM instance
 * @retval None
 */
__STATIC_INLINE void LL_TPM3_Enable(SN_CT16B3_Type *tpm)
{
    LL_REG_SBIT(tpm->TMRCTRL, TPM_34678_TMRCTRL_CEN);
}

/**
 * @brief  Disable Counter
 * @param  tpm TPM instance
 * @retval None
 */
__STATIC_INLINE void LL_TPM3_Disable(SN_CT16B3_Type *tpm)
{
    LL_REG_CBIT(tpm->TMRCTRL, TPM_34678_TMRCTRL_CEN);
}

/**
 * @brief  Judge if enabled TPM3
 * @param  tpm TPM instance
 * @retval 1: Enabled;   0: Disabled
 */
__STATIC_INLINE uint32_t LL_TPM3_IsEnabled(SN_CT16B3_Type *tpm)
{
    return ((LL_REG_READ(tpm->TMRCTRL) & TPM_34678_TMRCTRL_CEN) == TPM_34678_TMRCTRL_CEN);
}

/**
 * @brief  Reset TPM
 * @param  tpm TPM instance
 * @retval None
 */
__STATIC_INLINE void LL_TPM3_Reset(SN_CT16B3_Type *tpm)
{
    LL_REG_SBIT(tpm->TMRCTRL, TPM_34678_TMRCTRL_CRST);
}

/**
 * @brief  Get Reset State
 * @param  tpm TPM instance
 * @retval 0 : Reset finished,  1 : Resetting
 */
__STATIC_INLINE uint32_t LL_TPM3_GetResetState(SN_CT16B3_Type *tpm)
{
    return ((LL_REG_READ(tpm->TMRCTRL) & TPM_34678_TMRCTRL_CRST) >> TPM_34678_TMRCTRL_CRST_Pos);
}

/**
 * @brief  Set Counter value
 * @param  tpm TPM instance
 * @param  value Counter value range from 0x0 to 0xFFFF
 * @retval None
 */
__STATIC_INLINE void LL_TPM3_SetTimeCounter(SN_CT16B3_Type *tpm, uint32_t value)
{
    LL_REG_CBIT(tpm->TC, TPM_TC_TC);
    LL_REG_SBIT(tpm->TC, value << TPM_TC_TC_Pos);
}

/**
 * @brief  Get Counter value
 * @param  tpm TPM instance
 * @retval Counter value
 */
__STATIC_INLINE uint32_t LL_TPM3_GetTimeCounter(SN_CT16B3_Type *tpm)
{
    return ((LL_REG_READ(tpm->TC) & TPM_TC_TC) >> TPM_TC_TC_Pos);
}

/**
 * @brief  Set Prescaler value
 * @param  tpm TPM instance
 * @param  value Prescaler value range from 0x0 to 0xFF
 * @retval None
 */
__STATIC_INLINE void LL_TPM3_SetPrescaler(SN_CT16B3_Type *tpm, uint32_t value)
{
    LL_REG_CBIT(tpm->PRE, TPM_PRE_PRE);
    LL_REG_SBIT(tpm->PRE, value << TPM_PRE_PRE_Pos);
}

/**
 * @brief  Get Prescaler value
 * @param  tpm TPM instance
 * @retval Prescaler value
 */
__STATIC_INLINE uint32_t LL_TPM3_GetPrescaler(SN_CT16B3_Type *tpm)
{
    return ((LL_REG_READ(tpm->PRE) & TPM_PRE_PRE) >> TPM_PRE_PRE_Pos);
}

/**
 * @brief  Set PC Prescaler value
 * @param  tpm TPM instance
 * @param  value Prescaler value range from 0x0 to 0xFF
 * @retval None
 */
__STATIC_INLINE void LL_TPM3_SetPrescalerPC(SN_CT16B3_Type *tpm, uint32_t value)
{
    LL_REG_CBIT(tpm->PC, TPM_PC_PC);
    LL_REG_SBIT(tpm->PC, value << TPM_PC_PC_Pos);
}

/**
 * @brief  Get PC Prescaler value
 * @param  tpm TPM instance
 * @retval PC Prescaler value
 */
__STATIC_INLINE uint32_t LL_TPM3_GetPrescalerPC(SN_CT16B3_Type *tpm)
{
    return ((LL_REG_READ(tpm->PC) & TPM_PC_PC) >> TPM_PC_PC_Pos);
}

/**
 * @brief  Enable generating an interrupt when MRn matches TC
 * @param  tpm TPM instance
 * @param  channel Channel number, \ref tpm_channel
 *         tpm3 just support channel 0,1,9
 * @retval None
 */
__STATIC_INLINE void LL_TPM3_EnableIT_MRnIE(SN_CT16B3_Type *tpm, uint32_t channel)
{
    LL_REG_SBIT(tpm->MCTRL, 1 << TPM_3_MCTRL_MRnIE(channel));
}

/**
 * @brief  Disable generating an interrupt when MRn matches TC
 * @param  tpm TPM instance
 * @param  channel Channel number, \ref tpm_channel
 *         tpm3 just support channel 0,1,9
 * @retval None
 */
__STATIC_INLINE void LL_TPM3_DisableIT_MRnIE(SN_CT16B3_Type *tpm, uint32_t channel)
{
    LL_REG_CBIT(tpm->MCTRL, 1 << TPM_3_MCTRL_MRnIE(channel));
}

/**
 * @brief  Judge if MR0IE  Interrupt is enabled
 * @param  tpm TPM instance
 * @param  channel Channel number, \ref tpm_channel
 *         tpm3 just support channel 0,1,9
 * @retval 0: disabled, 1: enabled.
 */
__STATIC_INLINE uint32_t LL_TPM3_IsEnabledIT_MRnIE(SN_CT16B3_Type *tpm, uint32_t channel)
{
    return ((LL_REG_READ(tpm->MCTRL) >> TPM_3_MCTRL_MRnIE(channel)) & 0x01U);
}

/**
 * @brief  Enable reset TC when MRn matches TC
 * @param  tpm TPM instance
 * @param  channel Channel number, \ref tpm_channel
 *         tpm3 just support channel 0,1,9
 * @retval None
 */
__STATIC_INLINE void LL_TPM3_Enable_MRnRST(SN_CT16B3_Type *tpm, uint32_t channel)
{
    LL_REG_SBIT(tpm->MCTRL, 1 << TPM_3_MCTRL_MRnRST(channel));
}

/**
 * @brief  Disable reset TC when MRn matches TC
 * @param  tpm TPM instance
 * @param  channel Channel number, \ref tpm_channel
 *         tpm3 just support channel 0,1,9
 * @retval None
 */
__STATIC_INLINE void LL_TPM3_Disable_MRnRST(SN_CT16B3_Type *tpm, uint32_t channel)
{
    LL_REG_CBIT(tpm->MCTRL, 1 << TPM_3_MCTRL_MRnRST(channel));
}

/**
 * @brief  Judge if MRnRST is enabled
 * @param  tpm TPM instance
 * @param  channel Channel number, \ref tpm_channel
 *         tpm3 just support channel 0,1,9
 * @retval 0: disabled, 1: enabled.
 */
__STATIC_INLINE uint32_t LL_TPM3_IsEnabled_MRnRST(SN_CT16B3_Type *tpm, uint32_t channel)
{
    return ((LL_REG_READ(tpm->MCTRL) >> TPM_3_MCTRL_MRnRST(channel)) & 0x01U);
}

/**
 * @brief  Stop TC and PC and clear CEN bit when MRn matches TC
 * @param  tpm TPM instance
 * @param  channel Channel number, \ref tpm_channel
 *         tpm3 just support channel 0,1,9
 * @retval None
 */
__STATIC_INLINE void LL_TPM3_Enable_MRnSTOP(SN_CT16B3_Type *tpm, uint32_t channel)
{
    LL_REG_SBIT(tpm->MCTRL, 1 << TPM_3_MCTRL_MRnSTOP(channel));
}

/**
 * @brief  Disable MRnSTOP
 * @param  tpm TPM instance
 * @param  channel Channel number, \ref tpm_channel
 *         tpm3 just support channel 0,1,9
 * @retval None
 */
__STATIC_INLINE void LL_TPM3_Disable_MRnSTOP(SN_CT16B3_Type *tpm, uint32_t channel)
{
    LL_REG_CBIT(tpm->MCTRL, 1 << TPM_3_MCTRL_MRnSTOP(channel));
}

/**
 * @brief  Judge if MRnSTOP is enabled
 * @param  tpm TPM instance
 * @param  channel Channel number, \ref tpm_channel
 *         tpm3 just support channel 0,1,9
 * @retval 0: disabled, 1: enabled.
 */
__STATIC_INLINE uint32_t LL_TPM3_IsEnabled_MRnSTOP(SN_CT16B3_Type *tpm, uint32_t channel)
{
    return ((LL_REG_READ(tpm->MCTRL) >> TPM_3_MCTRL_MRnSTOP(channel)) & 0x01U);
}

/**
 * @brief  Set Timer counter match value to MRn register
 * @param  tpm TPM instance
 * @param  value Timer counter match value range from 0 to 0xFFFF
 * @param  mrx which MR number to set, just supports mrx = 0,1,9
 * @retval None
 */
__STATIC_INLINE void LL_TPM3_SetTCMatchValueToMRn(SN_CT16B3_Type *tpm, uint32_t value, uint32_t mrx)
{
    LL_REG_CBIT(*(&tpm->MR0 + mrx + (mrx == 9 ? 1 : 0)), 0xFFFF << 0U);
    LL_REG_SBIT(*(&tpm->MR0 + mrx + (mrx == 9 ? 1 : 0)), value << 0U);
}

/**
 * @brief  Get Timer counter (TC) match value
 * @param  tpm TPM instance
 * @param  mrx which MR number to set, just supports mrx = 0,1,9
 * @retval Timer counter (TC) match value
 */
__STATIC_INLINE uint32_t LL_TPM3_GetTCMatchValueFromMRn(SN_CT16B3_Type *tpm, uint32_t mrx)
{
    return ((*(&tpm->MR0 + mrx + (mrx == 9 ? 1 : 0)) & 0xFFFF) >> 0U);
}

/**
 * @brief  When EMCn!=00b and MRn!=TC, it will drive the state of CT16Bn_PWMn output High
 * @param  tpm TPM instance
 * @param  channel Channel number, \ref tpm_channel
 *         tpm3 just support channel 0,1
 * @retval None
 */
__STATIC_INLINE void LL_TPM3_SetPWMnHigh(SN_CT16B3_Type *tpm, uint32_t channel)
{
    LL_REG_SBIT(tpm->EM, 1 << TPM_3_EM_EMn(channel));
}

/**
 * @brief  When EMCn!=00b and MRn!=TC, it will drive the state of CT16Bn_PWMn output Low
 * @param  tpm TPM instance
 * @param  channel Channel number, \ref tpm_channel
 *         tpm3 just support channel 0,1
 * @retval None
 */
__STATIC_INLINE void LL_TPM3_SetPWMnLow(SN_CT16B3_Type *tpm, uint32_t channel)
{
    LL_REG_CBIT(tpm->EM, 1 << TPM_3_EM_EMn(channel));
}

/**
 * @brief  Get CT16Bn_PWMn output state when EMCn!=00b and MRn!=TC
 * @param  tpm TPM instance
 * @param  channel Channel number, \ref tpm_channel
 *         tpm3 just support channel 0,1
 * @retval 1: high, 0: low
 */
__STATIC_INLINE uint32_t LL_TPM3_GetPWMnState(SN_CT16B3_Type *tpm, uint32_t channel)
{
    return ((LL_REG_READ(tpm->EM) >> TPM_3_EM_EMn(channel)) & 0x01U);
}

/**
 * @brief  Set CT16Bn_PWMn functionality when MRn=TC
 * @param  tpm TPM instance
 * @param  function PWM functionality \ref tpm3_pwm_pin_functionality
 * @param  channel Channel number, \ref tpm_channel
 *         tpm3 just support channel 0,1
 * @retval None
 */
__STATIC_INLINE void LL_TPM3_SetPWMnFunction(SN_CT16B3_Type *tpm, uint32_t function, uint32_t channel)
{
    LL_REG_CBIT(tpm->EM, 0x03U << TPM_3_EM_EMCn(channel));
    LL_REG_SBIT(tpm->EM, function << TPM_3_EM_EMCn(channel));
}

/**
 * @brief  Get CT16Bn_PWMn functionality when MRn=TC
 * @param  tpm TPM instance
 * @param  channel Channel number, \ref tpm_channel
 *         tpm3 just support channel 0,1
 * @retval
 *         0: Do nothing
 *         1: CT16Bn_PWMn pin is low when MRn=TC
 *         2: CT16Bn_PWMn pin is high when MRn=TC
 *         3: CT16Bn_PWMn pin is toggled when MRn=TC
 */
__STATIC_INLINE uint32_t LL_TPM3_GetPWMnFunction(SN_CT16B3_Type *tpm, uint32_t channel)
{
    return ((LL_REG_READ(tpm->EM) >> TPM_3_EM_EMCn(channel)) & 0x03U);
}

/**
 * @brief  Enable PWMn
 * @param  tpm TPM instance
 * @param  channel Channel number, \ref tpm_channel
 *         tpm3 just support channel 0,1
 * @retval None
 */
__STATIC_INLINE void LL_TPM3_Enable_PWMn(SN_CT16B3_Type *tpm, uint32_t channel)
{
    LL_REG_SBIT(tpm->PWMCTRL, 1 << TPM_3_PWMCTRL_PWMnEN(channel));
}

/**
 * @brief  Disable PWMn
 * @param  tpm TPM instance
 * @param  channel Channel number, \ref tpm_channel
 *         tpm3 just support channel 0,1
 * @retval None
 */
__STATIC_INLINE void LL_TPM3_Disable_PWMn(SN_CT16B3_Type *tpm, uint32_t channel)
{
    LL_REG_CBIT(tpm->PWMCTRL, 1 << TPM_3_PWMCTRL_PWMnEN(channel));
}

/**
 * @brief  Judge if enabled PWMn
 * @param  tpm TPM instance
 * @param  channel Channel number, \ref tpm_channel
 *         tpm3 just support channel 0,1
 * @retval 1: Enabled;   0: Disabled
 */
__STATIC_INLINE uint32_t LL_TPM3_IsEnabled_PWMn(SN_CT16B3_Type *tpm, uint32_t channel)
{
    return ((LL_REG_READ(tpm->PWMCTRL) >> TPM_3_PWMCTRL_PWMnEN(channel)) & 0x01U);
}

/**
 * @brief  Set PWMn mode
 * @param  tpm TPM instance
 * @param  mode PWM0 mode \ref tpm3_pwm_mode
 * @param  channel Channel number, \ref tpm_channel
 *         tpm3 just support channel 0,1
 * @retval None
 */
__STATIC_INLINE void LL_TPM3_SetPWMnMode(SN_CT16B3_Type *tpm, uint32_t mode, uint32_t channel)
{
    LL_REG_CBIT(tpm->PWMCTRL, 0x03U << TPM_3_PWMCTRL_PWMnMODE(channel));
    LL_REG_SBIT(tpm->PWMCTRL, mode << TPM_3_PWMCTRL_PWMnMODE(channel));
}

/**
 * @brief  Get PWMn mode
 * @param  tpm TPM instance
 * @param  channel Channel number, \ref tpm_channel
 *         tpm3 just support channel 0,1
 * @retval PWM0 mode \ref tpm3_pwm_mode
 */
__STATIC_INLINE uint32_t LL_TPM3_GetPWM0Mode(SN_CT16B3_Type *tpm, uint32_t channel)
{
    return ((LL_REG_READ(tpm->PWMCTRL) >> TPM_3_PWMCTRL_PWMnMODE(channel)) & 0x03U);
}

/**
 * @brief  Set CT16Bn_PWMn pin act as match output, and output depends on PWMnEN bit
 * @param  tpm TPM instance
 * @param  channel Channel number, \ref tpm_channel
 *         tpm3 just support channel 0,1
 * @retval None
 */
__STATIC_INLINE void LL_TPM3_Enable_PWMnIOEN(SN_CT16B3_Type *tpm, uint32_t channel)
{
    LL_REG_SBIT(tpm->PWMCTRL, 1 << TPM_3_PWMCTRL_PWMnIOEN(channel));
}

/**
 * @brief  Set CT16Bn_PWMn pin is act as GPIO
 * @param  tpm TPM instance
 * @param  channel Channel number, \ref tpm_channel
 *         tpm3 just support channel 0,1
 * @retval None
 */
__STATIC_INLINE void LL_TPM3_Disable_PWMnIOEN(SN_CT16B3_Type *tpm, uint32_t channel)
{
    LL_REG_CBIT(tpm->PWMCTRL, 1 << TPM_3_PWMCTRL_PWMnIOEN(channel));
}

/**
 * @brief  Judge if enabled PWMnIOEN
 * @param  tpm TPM instance
 * @param  channel Channel number, \ref tpm_channel
 *         tpm3 just support channel 0,1
 * @retval 1: Enabled;   0: Disabled
 */
__STATIC_INLINE uint32_t LL_TPM3_IsEnabled_PWMnIOEN(SN_CT16B3_Type *tpm, uint32_t channel)
{
    return ((LL_REG_READ(tpm->PWMCTRL) >> TPM_3_PWMCTRL_PWMnIOEN(channel)) & 0x01U);
}

/**
 * @brief  Set CT16Bn_PWMnN/GPIO selection
 * @param  tpm TPM instance
 * @param  channel Channel number, \ref tpm_channel
 *         tpm3 just support channel 0,1
 * @param  selection selection \ref tpm3_pwmn_gpio_selection
 * @retval None
 */
__STATIC_INLINE void LL_TPM3_SetPWMNnSelection(SN_CT16B3_Type *tpm, uint32_t selection, uint32_t channel)
{
    LL_REG_CBIT(tpm->PWMCTRL, 0X03U << TPM_3_PWMCTRL_PWMnNIOEN(channel));
    LL_REG_SBIT(tpm->PWMCTRL, selection << TPM_3_PWMCTRL_PWMnNIOEN(channel));
}

/**
 * @brief  Get CT16Bn_PWMnN/GPIO selection
 * @param  tpm TPM instance
 * @param  channel Channel number, \ref tpm_channel
 *         tpm3 just support channel 0,1
 * @retval selection \ref tpm3_pwmn_gpio_selection
 */
__STATIC_INLINE uint32_t LL_TPM3_GetPWMNnSelection(SN_CT16B3_Type *tpm, uint32_t channel)
{
    return ((LL_REG_READ(tpm->PWMCTRL) >> TPM_3_PWMCTRL_PWMnNIOEN(channel)) & 0x03U);
}

/**
 * @brief  Judge if has Match channel n interrupt flag
 * @param  tpm TPM instance
 * @param  channel Channel number, \ref tpm_channel
 *         tpm3 just support channel 0,1
 * @retval 0: No interrupt on match channel n
 *         1: Interrupt requirements met on match channel n.
 */
__STATIC_INLINE uint32_t LL_TPM3_IsActiveFlag_MRnIF(SN_CT16B3_Type *tpm, uint32_t channel)
{
    return ((LL_REG_READ(tpm->RIS) >> TPM_3_RIS_MRnIF(channel)) & 0x01U);
}

/**
 * @brief  Clear MRnIF
 * @param  tpm TPM instance
 * @param  channel channel number \ref tpm_channel
 *         tpm0 just supports channel 0,1,9
 * @retval None
 */
__STATIC_INLINE void LL_TPM3_ClearFlag_MRnIF(SN_CT16B3_Type *tpm, uint32_t channel)
{
    LL_REG_SBIT(tpm->IC, 1 << TPM_3_IC_MRnIC(channel));
}

/**
 * @brief  Set Count of PWMmN output dead-band period time
 *         PWM0N output dead-band period time=DB*CT16Bn_PCLK*(PR+1) cycle
 * @param  tpm TPM instance
 * @param  pwmx which pwm number to set, range from 0 to 1
 * @param  db_period count of dead-band period time
 * @retval None
 */
__STATIC_INLINE void LL_TPM3_SetDBPeriod(SN_CT16B3_Type *tpm, uint32_t pwmx, uint32_t db_period)
{
    LL_REG_CBIT(*(&tpm->PWM0NDB + pwmx), TPM_034_PWMmNDB_DB);
    LL_REG_SBIT(*(&tpm->PWM0NDB + pwmx), db_period << TPM_034_PWMmNDB_DB_Pos);
}

/**
 * @brief  Get Count of PWMmN output dead-band period time
 * @param  tpm TPM instance
 * @param  pwmx which pwm number to set, range from 0 to 1
 * @retval Dead-band period time
 */
__STATIC_INLINE uint32_t LL_TPM3_GetDBPeriod(SN_CT16B3_Type *tpm, uint32_t pwmx)
{
    return (LL_REG_RBIT(*(&tpm->PWM0NDB + pwmx), TPM_034_PWMmNDB_DB) >> TPM_034_PWMmNDB_DB_Pos);
}

/**
 * @brief  MRn DMA request active enable
 * @param  tpm TPM instance
 * @param  channel channel number \ref tpm_channel
 *         tpm0 just supports channel 0,1,9
 * @retval None
 */
__STATIC_INLINE void LL_TPM3_Enable_DMAMRn(SN_CT16B3_Type *tpm, uint32_t channel)
{
    LL_REG_SBIT(tpm->DMA, 1 << TPM_3_DMA_DMA_MRn(channel));
}
/**
 * @brief  MR0 DMA request active disable
 * @param  tpm TPM instance
 * @param  channel channel number \ref tpm_channel
 *         tpm0 just supports channel 0,1,9
 * @retval None
 */
__STATIC_INLINE void LL_TPM3_Disable_DMAMRn(SN_CT16B3_Type *tpm, uint32_t channel)
{
    LL_REG_CBIT(tpm->DMA, 1 << TPM_3_DMA_DMA_MRn(channel));
}
/**
 * @brief  Judge if enabled MR0 DMA request active
 * @param  tpm TPM instance
 * @param  channel channel number \ref tpm_channel
 *         tpm0 just supports channel 0,1,9
 * @retval 1: Enabled;   0: Disabled
 */
__STATIC_INLINE uint32_t LL_TPM3_IsEnabled_DMAMRn(SN_CT16B3_Type *tpm, uint32_t channel)
{
    return ((LL_REG_READ(tpm->DMA) >> TPM_3_DMA_DMA_MRn(channel)) & 0x01U);
}

/**
 * @brief  Set MRn alias for DMA access
 * @param  tpm TPM instance
 * @param  channel channel number \ref tpm_channel
 *         tpm0 just supports channel 0,1
 * @param  alias alias
 * @retval None
 */
__STATIC_INLINE void LL_TPM3_SetMRnAlias(SN_CT16B3_Type *tpm, uint16_t alias, uint32_t channel)
{
    LL_REG_CBIT(*(&tpm->DMAMRA1 + channel * 2), 0xFFFF << 0U);
    LL_REG_SBIT(*(&tpm->DMAMRA1 + channel * 2), alias << 0U);
}

#ifdef __cplusplus
}
#endif

#endif /* _LL_tpm_H_ */
