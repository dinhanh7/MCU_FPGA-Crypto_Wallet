/***********************
 * SN34F78X SDK V2.1.4 *
 ***********************/

#ifndef _LL_TPM125_H_
#define _LL_TPM125_H_

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "ll_tpm.h"

/* private macro ------------------------------------------------------------*/
/// \cond
#define TPM_0125_MCTRL_MRnIE(channel)   (1 << (channel * 3 + (channel == 9 ? 2 : 0)))
#define TPM_0125_MCTRL_MRnRST(channel)  (2 << (channel * 3 + (channel == 9 ? 2 : 0)))
#define TPM_0125_MCTRL_MRnSTOP(channel) (4 << (channel * 3 + (channel == 9 ? 2 : 0)))
#define TPM_0125_EM_EMn(channel)        (1 << channel)

#define TPM_125_PWMCTRL_PWMnEN(channel)   (1 << channel)
#define TPM_125_PWMCTRL_PWMnIOEN(channel) (1 << (20 + channel))
#define TPM_125_RIS_MRnIF(channel)        (1 << (channel + (channel == 9 ? 0 : 4) - 4))
#define TPM_125_IC_MRnIC(channel)         (1 << (channel + (channel == 9 ? 0 : 4) - 4))
#define TPM_0125_DMA_DMAMRn(channel)      (1 << (channel + (channel == 9 ? 0 : 4) - 4))
/// \endcond
/* Exported types ------------------------------------------------------------*/
/* Exported constants --------------------------------------------------------*/

/**
 * \defgroup tpm125_cm_mode CT16B1/2/5 Counting Mode
 * \ingroup tpm_control
 * @{
 */
#define LL_TPM125_CM_COUNTING_UP          TPM_0125_TMRCTRL_CM_EdgeUpCounting        /**< Edge-aligned Up-counting mode.  */
#define LL_TPM125_CM_COUNTING_DOWN        TPM_0125_TMRCTRL_CM_EdgeDownCounting      /**< Edge-aligned Down-counting mode.  */
#define LL_TPM125_CM_COUNTING_CENTER_DOWN TPM_0125_TMRCTRL_CM_DownCountingSetFlag   /**< The match interrupt flag is set during the down-counting period. */
#define LL_TPM125_CM_COUNTING_CENTER_UP   TPM_0125_TMRCTRL_CM_UpCountingSetFlag     /**< The match interrupt flag is set during the up-counting period. */
#define LL_TPM125_CM_COUNTING_CENTER_BOTH TPM_0125_TMRCTRL_CM_UpDownCountingSetFlag /**< The match interrupt flag is set during both up and down counting period. */
/**
 * @}
 */

/**
 * \defgroup tpm125_ctm_mode CT16B1/2/5 Counter Time Mode
 * \ingroup tpm_control
 * @{
 */
#define LL_TPM125_CTM_TIMER              TPM_01245_CNTCTRL_CTM_EveryRisingEdge /*!< Every rising edge of the input source */
#define LL_TPM125_CTM_COUNTING_UP_CAP0   TPM_01245_CNTCTRL_CTM_RisingEdgeCAP0  /*!< TC is incremented on rising edges on the CAP0 input selected */
#define LL_TPM125_CTM_COUNTING_DOWN_CAP0 TPM_01245_CNTCTRL_CTM_FallingEdgeCAP0 /*!< TC is incremented on falling edges on the CAP0 input selected */
#define LL_TPM125_CTM_COUNTING_BOTH_CAP0 TPM_01245_CNTCTRL_CTM_BothEdgeCAP0    /*!< TC is incremented on both edges on the CAP0 input selected */
/**
 * @}
 */

/**
 * \defgroup tpm125_pwm_pin_functionality CT16B1/2/5 PWM functionality when MR0=TC
 * \ingroup tpm_control
 * @{
 */
#define LL_TPM125_PWM_PIN_NONE   0 /*!< Do nothing */
#define LL_TPM125_PWM_PIN_LOW    1 /*!< CT16Bn_PWMn pin is Low when MR0=TC*/
#define LL_TPM125_PWM_PIN_HIGH   2 /*!< CT16Bn_PWMn pin is High when MR0=TC */
#define LL_TPM125_PWM_PIN_TOGGLE 3 /*!< Toggle CT16Bn_PWMn pin hen MR0=TC*/
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
 * \defgroup tpm125_pwm_mode CT16B1/2/5 PWM output mode
 * \ingroup tpm_control
 * @{
 */
#define LL_TPM125_PWM_MODE_1 0 /*!< PWM is 0 when TC <(<=) MR3 during Up-counting(Down-counting) period */
#define LL_TPM125_PWM_MODE_2 1 /*!< PWM is 1 when TC <(<=) MR3 during Up-counting(Down-counting) period */
#define LL_TPM125_PWM_MODE_3 2 /*!< PWM is forced to 0 */
#define LL_TPM125_PWM_MODE_4 3 /*!< PWM is forced to 1 */
/**
 * @}
 */

/**
 * \defgroup tpm125_mr_loadmode CT16B1/2/5 MRn load mode selection in center-aligned mode
 * \ingroup tpm_control
 * @{
 */
#define LL_TPM125_NO_EFFECT        TPM_0125_LOADCTRL_LOADMR0_None          /*!< No effect */
#define LL_TPM125_LOAD_TC_EQ_0     TPM_0125_LOADCTRL_LOADMR0_LoadAndFixed  /*!< Load MRn value at TC=0 and MRn value is unchanged at TC=MR9 */
#define LL_TPM125_LOAD_TC_EQ_0_MR9 TPM_0125_LOADCTRL_LOADMR0_LoadAndChange /*!< Load MRn value at TC=0 and TC=MR9 */
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
__STATIC_INLINE void LL_TPM125_Enable(SN_CT16B2_Type *tpm)
{
    LL_REG_SBIT(tpm->TMRCTRL, TPM_0125_TMRCTRL_CEN);
}

/**
 * @brief  Disable Counter
 * @param  tpm TPM instance
 * @retval None
 */
__STATIC_INLINE void LL_TPM125_Disable(SN_CT16B2_Type *tpm)
{
    LL_REG_CBIT(tpm->TMRCTRL, TPM_0125_TMRCTRL_CEN);
}

/**
 * @brief  Judge if enabled TPM125
 * @param  tpm TPM instance
 * @retval 1: Enabled;   0: Disabled
 */
__STATIC_INLINE uint32_t LL_TPM125_IsEnabled(SN_CT16B2_Type *tpm)
{
    return ((LL_REG_READ(tpm->TMRCTRL) & TPM_0125_TMRCTRL_CEN) >> TPM_0125_TMRCTRL_CEN);
}

/**
 * @brief  Reset TPM
 * @param  tpm TPM instance
 * @retval None
 */
__STATIC_INLINE void LL_TPM125_Reset(SN_CT16B2_Type *tpm)
{
    LL_REG_SBIT(tpm->TMRCTRL, TPM_0125_TMRCTRL_CRST);
}

/**
 * @brief  Get Reset State
 * @param  tpm TPM instance
 * @retval 0 : Reset finished,  1 : Resetting
 */
__STATIC_INLINE uint32_t LL_TPM125_GetResetState(SN_CT16B2_Type *tpm)
{
    return ((LL_REG_READ(tpm->TMRCTRL) & TPM_0125_TMRCTRL_CRST) >> TPM_0125_TMRCTRL_CRST_Pos);
}

/**
 * @brief  Counting mode selection
 * @param  tpm TPM instance
 * @param  mode Counting mode \ref tpm125_cm_mode
 * @retval None
 */
__STATIC_INLINE void LL_TPM125_SetCountingMode(SN_CT16B2_Type *tpm, uint32_t mode)
{
    LL_REG_CBIT(tpm->TMRCTRL, TPM_0125_TMRCTRL_CM);
    LL_REG_SBIT(tpm->TMRCTRL, mode);
}

/**
 * @brief  Counting mode selection
 * @param  tpm TPM instance
 * @retval Counting mode
 *         0: Edge-aligned Up-counting mode
 *         1: Edge-aligned Down-counting mode
 *         2: The match interrupt flag is set during the down-counting period.
 *         4: The match interrupt flag is set during the up-counting period.
 *         6: The match interrupt flag is set during both up and down counting period.
 */
__STATIC_INLINE uint32_t LL_TPM125_GetCountingMode(SN_CT16B2_Type *tpm)
{
    return ((LL_REG_READ(tpm->TMRCTRL) & TPM_0125_TMRCTRL_CM) >> TPM_0125_TMRCTRL_CM_Pos);
}

/**
 * @brief  Set Counter value
 * @param  tpm TPM instance
 * @param  value Counter value range from 0x0 to 0xFFFF
 * @retval None
 */
__STATIC_INLINE void LL_TPM125_SetTimeCounter(SN_CT16B2_Type *tpm, uint32_t value)
{
    LL_REG_CBIT(tpm->TC, TPM_TC_TC);
    LL_REG_SBIT(tpm->TC, value << TPM_TC_TC_Pos);
}

/**
 * @brief  Get Counter value
 * @param  tpm TPM instance
 * @retval Counter value
 */
__STATIC_INLINE uint32_t LL_TPM125_GetTimeCounter(SN_CT16B2_Type *tpm)
{
    return ((LL_REG_READ(tpm->TC) & TPM_TC_TC) >> TPM_TC_TC_Pos);
}

/**
 * @brief  Set Prescaler value
 * @param  tpm TPM instance
 * @param  value Prescaler value range from 0x0 to 0xFF
 * @retval None
 */
__STATIC_INLINE void LL_TPM125_SetPrescaler(SN_CT16B2_Type *tpm, uint32_t value)
{
    LL_REG_CBIT(tpm->PRE, TPM_PRE_PRE);
    LL_REG_SBIT(tpm->PRE, value << TPM_PRE_PRE_Pos);
}

/**
 * @brief  Get Prescaler value
 * @param  tpm TPM instance
 * @retval Prescaler value
 */
__STATIC_INLINE uint32_t LL_TPM125_GetPrescaler(SN_CT16B2_Type *tpm)
{
    return ((LL_REG_READ(tpm->PRE) & TPM_PRE_PRE) >> TPM_PRE_PRE_Pos);
}

/**
 * @brief  Set PC Prescaler value
 * @param  tpm TPM instance
 * @param  value Prescaler value range from 0x0 to 0xFF
 * @retval None
 */
__STATIC_INLINE void LL_TPM125_SetPrescalerPC(SN_CT16B2_Type *tpm, uint32_t value)
{
    LL_REG_CBIT(tpm->PC, TPM_PC_PC);
    LL_REG_SBIT(tpm->PC, value << TPM_PC_PC_Pos);
}

/**
 * @brief  Get PC Prescaler value
 * @param  tpm TPM instance
 * @retval PC Prescaler value
 */
__STATIC_INLINE uint32_t LL_TPM125_GetPrescalerPC(SN_CT16B2_Type *tpm)
{
    return ((LL_REG_READ(tpm->PC) & TPM_PC_PC) >> TPM_PC_PC_Pos);
}

/**
 * @brief  Set Counter/Timer Mode
 * @param  tpm TPM instance
 * @param  mode Counter/Timer Mode \ref tpm125_ctm_mode
 * @retval None
 */
__STATIC_INLINE void LL_TPM125_SetCounterTimeMode(SN_CT16B2_Type *tpm, uint32_t mode)
{
    LL_REG_CBIT(tpm->CNTCTRL, TPM_01245_CNTCTRL_CTM);
    LL_REG_SBIT(tpm->CNTCTRL, mode << TPM_01245_CNTCTRL_CTM_Pos);
}

/**
 * @brief  Get Counter/Timer Mode
 * @param  tpm TPM instance
 * @retval Counter/Timer Mode
 */
__STATIC_INLINE uint32_t LL_TPM125_GetCounterTimeMode(SN_CT16B2_Type *tpm)
{
    return ((LL_REG_READ(tpm->CNTCTRL) & TPM_01245_CNTCTRL_CTM) >> TPM_01245_CNTCTRL_CTM_Pos);
}

/**
 * @brief  Enable generating an interrupt when MRn matches TC
 * @param  tpm TPM instance
 * @param  channel channel number. \ref tpm_channel
 *          tpm 1,2,5 just support channel 0,1,2,3,9
 * @retval None
 */
__STATIC_INLINE void LL_TPM125_EnableIT_MRnIE(SN_CT16B2_Type *tpm, uint32_t channel)
{
    LL_REG_SBIT(tpm->MCTRL, TPM_0125_MCTRL_MRnIE(channel));
}

/**
 * @brief  Disable generating an interrupt when MRn matches TC
 * @param  tpm TPM instance
 * @param  channel channel number. \ref tpm_channel
 *          tpm 1,2,5 just support channel 0,1,2,3,9
 * @retval None
 */
__STATIC_INLINE void LL_TPM125_DisableIT_MRnIE(SN_CT16B2_Type *tpm, uint32_t channel)
{
    LL_REG_CBIT(tpm->MCTRL, TPM_0125_MCTRL_MRnIE(channel));
}

/**
 * @brief  Judge if MRnIE  Interrupt is enabled
 * @param  tpm TPM instance
 * @param  channel channel number. \ref tpm_channel
 *          tpm 1,2,5 just support channel 0,1,2,3,9
 * @retval 0: disabled, 1: enabled.
 */
__STATIC_INLINE uint32_t LL_TPM125_IsEnabledIT_MRnIE(SN_CT16B2_Type *tpm, uint32_t channel)
{
    return ((LL_REG_READ(tpm->MCTRL) & TPM_0125_MCTRL_MRnIE(channel)) == TPM_0125_MCTRL_MRnIE(channel));
}

/**
 * @brief  Enable reset TC when MRn matches TC
 * @param  tpm TPM instance
 * @param  channel channel number. \ref tpm_channel
 *          tpm 1,2,5 just support channel 0,1,2,3,9
 * @retval None
 */
__STATIC_INLINE void LL_TPM125_Enable_MRnRST(SN_CT16B2_Type *tpm, uint32_t channel)
{
    LL_REG_SBIT(tpm->MCTRL, TPM_0125_MCTRL_MRnRST(channel));
}

/**
 * @brief  Disable reset TC when MRn matches TC
 * @param  tpm TPM instance
 * @param  channel channel number. \ref tpm_channel
 *          tpm 1,2,5 just support channel 0,1,2,3,9
 * @retval None
 */
__STATIC_INLINE void LL_TPM125_Disable_MRnRST(SN_CT16B2_Type *tpm, uint32_t channel)
{
    LL_REG_CBIT(tpm->MCTRL, TPM_0125_MCTRL_MRnRST(channel));
}

/**
 * @brief  Judge if MRnRST is enabled
 * @param  tpm TPM instance
 * @param  channel channel number. \ref tpm_channel
 *          tpm 1,2,5 just support channel 0,1,2,3,9
 * @retval 1: Enabled, 0: Disabled
 */
__STATIC_INLINE uint32_t LL_TPM125_IsEnabled_MRnRST(SN_CT16B2_Type *tpm, uint32_t channel)
{
    return ((LL_REG_READ(tpm->MCTRL) & TPM_0125_MCTRL_MRnRST(channel)) == TPM_0125_MCTRL_MRnRST(channel));
}

/**
 * @brief  Stop TC and PC and clear CEN bit when MRn matches TC
 * @param  tpm TPM instance
 * @param  channel channel number. \ref tpm_channel
 *          tpm 1,2,5 just support channel 0,1,2,3,9
 * @retval None
 */
__STATIC_INLINE void LL_TPM125_Enable_MRnSTOP(SN_CT16B2_Type *tpm, uint32_t channel)
{
    LL_REG_SBIT(tpm->MCTRL, TPM_0125_MCTRL_MRnSTOP(channel));
}

/**
 * @brief  Disable MRnSTOP
 * @param  tpm TPM instance
 * @param  channel channel number. \ref tpm_channel
 *          tpm 1,2,5 just support channel 0,1,2,3,9
 * @retval None
 */
__STATIC_INLINE void LL_TPM125_Disable_MRnSTOP(SN_CT16B2_Type *tpm, uint32_t channel)
{
    LL_REG_CBIT(tpm->MCTRL, TPM_0125_MCTRL_MRnSTOP(channel));
}

/**
 * @brief  Judge if MRnSTOP is enabled
 * @param  tpm TPM instance
 * @param  channel channel number. \ref tpm_channel
 *          tpm 1,2,5 just support channel 0,1,2,3,9
 * @retval 0: disabled, 1: enabled.
 */
__STATIC_INLINE uint32_t LL_TPM125_IsEnabled_MRnSTOP(SN_CT16B2_Type *tpm, uint32_t channel)
{
    return ((LL_REG_READ(tpm->MCTRL) & TPM_0125_MCTRL_MRnSTOP(channel)) == TPM_0125_MCTRL_MRnSTOP(channel));
}

/**
 * @brief  Set Timer counter match value to MRn register
 * @param  tpm TPM instance
 * @param  value Timer counter match value range from 0 to 0xFFFF
 * @param  mrx which MR number to set, just supports mrx = 0,1,2,3,9
 * @retval None
 */
__STATIC_INLINE void LL_TPM125_SetTCMatchValueToMRn(SN_CT16B2_Type *tpm, uint32_t value, uint32_t mrx)
{
    LL_REG_CBIT(*(&tpm->MR0 + mrx + (mrx == 9 ? 1 : 0)), 0xFFFF << 0U);
    LL_REG_SBIT(*(&tpm->MR0 + mrx + (mrx == 9 ? 1 : 0)), value << 0U);
}

/**
 * @brief  Get Timer counter (TC) match value
 * @param  tpm TPM instance
 * @param  mrx which MR number to set, just supports mrx = 0,1,2,3,9
 * @retval Timer counter (TC) match value
 */
__STATIC_INLINE uint32_t LL_TPM125_GetTCMatchValueFromMRn(SN_CT16B2_Type *tpm, uint32_t mrx)
{
    return ((*(&tpm->MR0 + mrx + (mrx == 9 ? 1 : 0)) & 0xFFFF) >> 0U);
}

/**
 * @brief  Capture/Reset on CT16Bn_CAP0 signal rising edge
 * @param  tpm TPM instance
 * @retval None
 */
__STATIC_INLINE void LL_TPM125_Enable_CAP0RE(SN_CT16B2_Type *tpm)
{
    LL_REG_SBIT(tpm->CAPCTRL, TPM_01245_CAPCTRL_CAP0RE);
}

/**
 * @brief  Disable CAP0RE
 * @param  tpm TPM instance
 * @retval None
 */
__STATIC_INLINE void LL_TPM125_Disable_CAP0RE(SN_CT16B2_Type *tpm)
{
    LL_REG_CBIT(tpm->CAPCTRL, TPM_01245_CAPCTRL_CAP0RE);
}

/**
 * @brief  Judge if CAP0RE is enabled
 * @param  tpm TPM instance
 * @retval 0: disabled, 1: enabled.
 */
__STATIC_INLINE uint32_t LL_TPM125_IsEnabled_CAP0RE(SN_CT16B2_Type *tpm)
{
    return ((LL_REG_READ(tpm->CAPCTRL) & TPM_01245_CAPCTRL_CAP0RE) == TPM_01245_CAPCTRL_CAP0RE);
}

/**
 * @brief  Capture/Reset on CT16Bn_CAP0 signal falling edge
 * @param  tpm TPM instance
 * @retval None
 */
__STATIC_INLINE void LL_TPM125_Enable_CAP0FE(SN_CT16B2_Type *tpm)
{
    LL_REG_SBIT(tpm->CAPCTRL, TPM_01245_CAPCTRL_CAP0FE);
}

/**
 * @brief  Disable CAP0FE
 * @param  tpm TPM instance
 * @retval None
 */
__STATIC_INLINE void LL_TPM125_Disable_CAP0FE(SN_CT16B2_Type *tpm)
{
    LL_REG_CBIT(tpm->CAPCTRL, TPM_01245_CAPCTRL_CAP0FE);
}

/**
 * @brief  Judge if CAP0FE is enabled
 * @param  tpm TPM instance
 * @retval 0: disabled, 1: enabled.
 */
__STATIC_INLINE uint32_t LL_TPM125_IsEnabled_CAP0FE(SN_CT16B2_Type *tpm)
{
    return ((LL_REG_READ(tpm->CAPCTRL) & TPM_01245_CAPCTRL_CAP0FE) == TPM_01245_CAPCTRL_CAP0FE);
}

/**
 * @brief  Interrupt on CT16Bn_CAP0 event : a CAP0 load due to a CT16Bn_CAP0 event will generate an interrupt.
 * @param  tpm TPM instance
 * @retval None
 */
__STATIC_INLINE void LL_TPM125_EnableIT_CAP0IE(SN_CT16B2_Type *tpm)
{
    LL_REG_SBIT(tpm->CAPCTRL, TPM_01245_CAPCTRL_CAP0IE);
}

/**
 * @brief  Disable CAP0IE
 * @param  tpm TPM instance
 * @retval None
 */
__STATIC_INLINE void LL_TPM125_DisableIT_CAP0IE(SN_CT16B2_Type *tpm)
{
    LL_REG_CBIT(tpm->CAPCTRL, TPM_01245_CAPCTRL_CAP0IE);
}

/**
 * @brief  Judge if CAP0IE is enabled
 * @param  tpm TPM instance
 * @retval 0: disabled, 1: enabled.
 */
__STATIC_INLINE uint32_t LL_TPM125_IsEnabledIT_CAP0IE(SN_CT16B2_Type *tpm)
{
    return ((LL_REG_READ(tpm->CAPCTRL) & TPM_01245_CAPCTRL_CAP0IE) == TPM_01245_CAPCTRL_CAP0IE);
}

/**
 * @brief  CAP0 function enable
 * @param  tpm TPM instance
 * @retval None
 */
__STATIC_INLINE void LL_TPM125_Enable_CAP0EN(SN_CT16B2_Type *tpm)
{
    LL_REG_SBIT(tpm->CAPCTRL, TPM_01245_CAPCTRL_CAP0EN);
}

/**
 * @brief  Disable CAP0EN
 * @param  tpm TPM instance
 * @retval None
 */
__STATIC_INLINE void LL_TPM125_Disable_CAP0EN(SN_CT16B2_Type *tpm)
{
    LL_REG_CBIT(tpm->CAPCTRL, TPM_01245_CAPCTRL_CAP0EN);
}

/**
 * @brief  Judge if CAP0EN is enabled
 * @param  tpm TPM instance
 * @retval 0: disabled, 1: enabled.
 */
__STATIC_INLINE uint32_t LL_TPM125_IsEnabled_CAP0EN(SN_CT16B2_Type *tpm)
{
    return ((LL_REG_READ(tpm->CAPCTRL) & TPM_01245_CAPCTRL_CAP0EN) == TPM_01245_CAPCTRL_CAP0EN);
}

/**
 * @brief  Get Timer counter (TC) match value from CAP0
 * @param  tpm TPM instance
 * @retval Timer counter (TC) match value
 */
__STATIC_INLINE uint32_t LL_TPM125_GetTCMatchValueFromCAP0(SN_CT16B2_Type *tpm)
{
    return ((LL_REG_READ(tpm->CAP0) & TPM_01245_CAP0_CAP0) >> TPM_01245_CAP0_CAP0_Pos);
}

/**
 * @brief  When EMC0!=00b and MRn!=TC, it will drive the state of CT16Bn_PWM0 output High
 * @param  tpm TPM instance
 * @param  channel channel number. \ref tpm_channel
 *          tpm 1,2,5 just support channel 0,1,2,3
 * @retval None
 */
__STATIC_INLINE void LL_TPM125_SetPWMnHigh(SN_CT16B2_Type *tpm, uint32_t channel)
{
    LL_REG_SBIT(tpm->EM, TPM_0125_EM_EMn(channel));
}

/**
 * @brief  When EMC0!=00b and MRn!=TC, it will drive the state of CT16Bn_PWM0 output Low
 * @param  tpm TPM instance
 * @param  channel channel number. \ref tpm_channel
 *          tpm 1,2,5 just support channel 0,1,2,3
 * @retval None
 */
__STATIC_INLINE void LL_TPM125_SetPWMnLow(SN_CT16B2_Type *tpm, uint32_t channel)
{
    LL_REG_CBIT(tpm->EM, TPM_0125_EM_EMn(channel));
}

/**
 * @brief  Get CT16Bn_PWM0 output state when EMC0!=00b and MRn!=TC
 * @param  tpm TPM instance
 * @param  channel channel number. \ref tpm_channel
 *          tpm 1,2,5 just support channel 0,1,2,3
 * @retval 1: high, 0: low
 */
__STATIC_INLINE uint32_t LL_TPM125_GetPWMnState(SN_CT16B2_Type *tpm, uint32_t channel)
{
    return ((LL_REG_READ(tpm->EM) & TPM_0125_EM_EMn(channel)) >> TPM_0125_EM_EMn(channel));
}

/**
 * @brief  Set CT16Bn_PWM0 functionality when MRn=TC
 * @param  tpm TPM instance
 * @param  channel channel number. \ref tpm_channel
 *          tpm 1,2,5 just support channel 0,1,2,3
 * @param  function PWM0 functionality \ref tpm125_pwm_pin_functionality
 * @retval None
 */
__STATIC_INLINE void LL_TPM125_SetPWMnFunction(SN_CT16B2_Type *tpm, uint32_t function, uint32_t channel)
{
    LL_REG_CBIT(tpm->EM, 48 << (channel * 2));
    LL_REG_SBIT(tpm->EM, function << (4 + channel * 2));
}

/**
 * @brief  Get CT16Bn_PWM0 functionality when MRn=TC
 * @param  tpm TPM instance
 * @param  channel channel number. \ref tpm_channel
 *          tpm 1,2,5 just support channel 0,1,2,3
 * @retval
 *         0: Do nothing
 *         1: CT16Bn_PWM0 pin is low when MRn=TC
 *         2: CT16Bn_PWM0 pin is high when MRn=TC
 *         3: CT16Bn_PWM0 pin is toggled when MRn=TC
 */
__STATIC_INLINE uint32_t LL_TPM125_GetPWMnFunction(SN_CT16B2_Type *tpm, uint32_t channel)
{
    return ((LL_REG_READ(tpm->EM) & (48 << (channel * 2))) >> (4 + channel * 2));
}

/**
 * @brief  Enable PWMn
 * @param  tpm TPM instance
 * @param  channel channel number. \ref tpm_channel
 *          tpm 1,2,5 just support channel 0,1,2,3
 * @retval None
 */
__STATIC_INLINE void LL_TPM125_Enable_PWMn(SN_CT16B2_Type *tpm, uint32_t channel)
{
    LL_REG_SBIT(tpm->PWMCTRL, TPM_125_PWMCTRL_PWMnEN(channel));
}

/**
 * @brief  Disable PWMn
 * @param  tpm TPM instance
 * @param  channel channel number. \ref tpm_channel
 *          tpm 1,2,5 just support channel 0,1,2,3
 * @retval None
 */
__STATIC_INLINE void LL_TPM125_Disable_PWMn(SN_CT16B2_Type *tpm, uint32_t channel)
{
    LL_REG_CBIT(tpm->PWMCTRL, TPM_125_PWMCTRL_PWMnEN(channel));
}

/**
 * @brief  Judge if enabled PWMn
 * @param  tpm TPM instance
 * @param  channel channel number. \ref tpm_channel
 *          tpm 1,2,5 just support channel 0,1,2,3
 * @retval 1: Enabled;   0: Disabled
 */
__STATIC_INLINE uint32_t LL_TPM125_IsEnabled_PWMn(SN_CT16B2_Type *tpm, uint32_t channel)
{
    return ((LL_REG_READ(tpm->PWMCTRL) & TPM_125_PWMCTRL_PWMnEN(channel)) == TPM_125_PWMCTRL_PWMnEN(channel));
}

/**
 * @brief  Set PWMn mode
 * @param  tpm TPM instance
 * @param  mode PWMn mode \ref tpm125_pwm_mode
 * @param  channel channel number. \ref tpm_channel
 *          tpm 1,2,5 just support channel 0,1,2,3
 * @retval None
 */
__STATIC_INLINE void LL_TPM125_SetPWMnMode(SN_CT16B2_Type *tpm, uint32_t mode, uint32_t channel)
{
    LL_REG_CBIT(tpm->PWMCTRL, 48 << (channel * 2));
    LL_REG_SBIT(tpm->PWMCTRL, mode << (4 + channel * 2));
}

/**
 * @brief  Get PWMn mode
 * @param  tpm TPM instance
 * @param  channel channel number. \ref tpm_channel
 *          tpm 1,2,5 just support channel 0,1,2,3
 * @retval PWMn mode \ref tpm125_pwm_mode
 */
__STATIC_INLINE uint32_t LL_TPM125_GetPWMnMode(SN_CT16B2_Type *tpm, uint32_t channel)
{
    return ((LL_REG_READ(tpm->PWMCTRL) & (48 << (channel * 2))) >> (4 + channel * 2));
}

/**
 * @brief  Set CT16Bn_PWMn pin act as match output, and output depends on PWMnEN bit
 * @param  tpm TPM instance
 * @param  channel channel number. \ref tpm_channel
 *          tpm 1,2,5 just support channel 0,1,2,3
 * @retval None
 */
__STATIC_INLINE void LL_TPM125_Enable_PWMnIOEN(SN_CT16B2_Type *tpm, uint32_t channel)
{
    LL_REG_SBIT(tpm->PWMCTRL, TPM_125_PWMCTRL_PWMnIOEN(channel));
}

/**
 * @brief  Set CT16Bn_PWMn pin is act as GPIO
 * @param  tpm TPM instance
 * @param  channel channel number. \ref tpm_channel
 *          tpm 1,2,5 just support channel 0,1,2,3
 * @retval None
 */
__STATIC_INLINE void LL_TPM125_Disable_PWMnIOEN(SN_CT16B2_Type *tpm, uint32_t channel)
{
    LL_REG_CBIT(tpm->PWMCTRL, TPM_125_PWMCTRL_PWMnIOEN(channel));
}

/**
 * @brief  Judge if enabled PWMnIOEN
 * @param  tpm TPM instance
 * @param  channel channel number. \ref tpm_channel
 *          tpm 1,2,5 just support channel 0,1,2,3
 * @retval 1: Enabled;   0: Disabled
 */
__STATIC_INLINE uint32_t LL_TPM125_IsEnabled_PWMnIOEN(SN_CT16B2_Type *tpm, uint32_t channel)
{
    return ((LL_REG_READ(tpm->PWMCTRL) & TPM_125_PWMCTRL_PWMnIOEN(channel)) == TPM_125_PWMCTRL_PWMnIOEN(channel));
}

/**
 * @brief  Judge if has Match channel n interrupt flag
 * @param  tpm TPM instance
 * @param  channel channel number. \ref tpm_channel
 *          tpm 1,2,5 just support channel 0,1,2,3,9
 * @retval 0: No interrupt on match channel n
 *         1: Interrupt requirements met on match channel n.
 */
__STATIC_INLINE uint32_t LL_TPM125_IsActiveFlag_MRnIF(SN_CT16B2_Type *tpm, uint32_t channel)
{
    return ((LL_REG_READ(tpm->RIS) & TPM_125_RIS_MRnIF(channel)) == TPM_125_RIS_MRnIF(channel));
}

/**
 * @brief  Judge if has Capture channel 0 interrupt flag
 * @param  tpm TPM instance
 * @retval 0: No interrupt on CAP0
 *         1: Interrupt requirements met on CAP0
 */
__STATIC_INLINE uint32_t LL_TPM125_IsActiveFlag_CAP0IF(SN_CT16B2_Type *tpm)
{
    return ((LL_REG_READ(tpm->RIS) & TPM_125_RIS_CAP0IF) == TPM_125_RIS_CAP0IF);
}

/**
 * @brief  Clear MRnIF
 * @param  tpm TPM instance
 * @param  channel channel number. \ref tpm_channel
 *          tpm 1,2,5 just support channel 0,1,2,3,9
 * @retval None
 */
__STATIC_INLINE void LL_TPM125_ClearFlag_MRnIF(SN_CT16B2_Type *tpm, uint32_t channel)
{
    LL_REG_SBIT(tpm->IC, TPM_125_IC_MRnIC(channel));
}
/**
 * @brief  Clear CAP0IF
 * @param  tpm TPM instance
 * @retval None
 */
__STATIC_INLINE void LL_TPM125_ClearFlag_CAP0IF(SN_CT16B2_Type *tpm)
{
    LL_REG_SBIT(tpm->IC, TPM_125_IC_CAP0IC);
}

/**
 * @brief  Set MRn load mode selection in center-aligned mode
 * @param  tpm TPM instance
 * @param  selection Load mode selection \ref tpm125_mr_loadmode
 * @param  channel Channel number. \ref tpm_channel
 *          tpm 1,2,5 just support channel 0,1,2,3
 * @retval None
 */
__STATIC_INLINE void LL_TPM125_SetMRnLoadMode(SN_CT16B2_Type *tpm, uint32_t selection, uint32_t channel)
{
    LL_REG_CBIT(tpm->LOADCTRL, (0x03 << (channel * 2)));
    LL_REG_SBIT(tpm->LOADCTRL, selection << (channel * 2));
}

/**
 * @brief  Get MRn load mode selection in center-aligned mode
 * @param  tpm TPM instance
 * @param  channel channel number. \ref tpm_channel
 *          tpm 1,2,5 just support channel 0,1,2,3
 * @retval 0: No effect
 *         1: Load MRn value at TC=0 and MRn value is unchanged at TC=MR9
 *         2: Reserved
 *         3: Load MRn value at TC=0 and TC=MR9
 */
__STATIC_INLINE uint32_t LL_TPM125_GetMRnLoadMode(SN_CT16B2_Type *tpm, uint32_t channel)
{
    return ((LL_REG_READ(tpm->LOADCTRL) & (0x03 << (channel * 2))) >> (channel * 2));
}

/**
 * @brief  MRn DMA request active enable
 * @param  tpm TPM instance
 * @param  channel channel number. \ref tpm_channel
 *          tpm 1,2,5 just support channel 0,1,2,3,9
 * @retval None
 */
__STATIC_INLINE void LL_TPM125_Enable_DMAMRn(SN_CT16B2_Type *tpm, uint32_t channel)
{
    LL_REG_SBIT(tpm->DMA, TPM_0125_DMA_DMAMRn(channel));
}

/**
 * @brief  MRn DMA request active disable
 * @param  tpm TPM instance
 * @param  channel channel number. \ref tpm_channel
 *          tpm 1,2,5 just support channel 0,1,2,3,9
 * @retval None
 */
__STATIC_INLINE void LL_TPM125_Disable_DMAMRn(SN_CT16B2_Type *tpm, uint32_t channel)
{
    LL_REG_CBIT(tpm->DMA, TPM_0125_DMA_DMAMRn(channel));
}

/**
 * @brief  Judge if enabled MRn DMA request active
 * @param  tpm TPM instance
 * @param  channel channel number. \ref tpm_channel
 *          tpm 1,2,5 just support channel 0,1,2,3,9
 * @retval 1: Enabled;   0: Disabled
 */
__STATIC_INLINE uint32_t LL_TPM125_IsEnabled_DMAMRn(SN_CT16B2_Type *tpm, uint32_t channel)
{
    return ((LL_REG_READ(tpm->DMA) & TPM_0125_DMA_DMAMRn(channel)) == TPM_0125_DMA_DMAMRn(channel));
}

/**
 * @brief  CAP0 DMA request active enable
 * @param  tpm TPM instance
 * @retval None
 */
__STATIC_INLINE void LL_TPM125_Enable_DMACAP0(SN_CT16B2_Type *tpm)
{
    LL_REG_SBIT(tpm->DMA, TPM_0125_DMA_DMACAP0);
}
/**
 * @brief  CAP0 DMA request active disable
 * @param  tpm TPM instance
 * @retval None
 */
__STATIC_INLINE void LL_TPM125_Disable_DMACAP0(SN_CT16B2_Type *tpm)
{
    LL_REG_CBIT(tpm->DMA, TPM_0125_DMA_DMACAP0);
}
/**
 * @brief  Judge if enabled CAP0 DMA request active
 * @param  tpm TPM instance
 * @retval 1: Enabled;   0: Disabled
 */
__STATIC_INLINE uint32_t LL_TPM125_IsEnabled_DMACAP0(SN_CT16B2_Type *tpm)
{
    return ((LL_REG_READ(tpm->DMA) & TPM_0125_DMA_DMACAP0) == TPM_0125_DMA_DMACAP0);
}

/**
 * @brief  Set MRn alias for DMA access
 * @param  tpm TPM instance
 * @param  channel channel number \ref tpm_channel
 *         tpm 1,2,5 just supports channel 0~3
 * @param  alias alias
 * @retval None
 */
__STATIC_INLINE void LL_TPM125_SetMRnAlias(SN_CT16B2_Type *tpm, uint16_t alias, uint32_t channel)
{
    LL_REG_CBIT(*(&tpm->DMAMRA1 + channel * 2), 0xFFFF << 0U);
    LL_REG_SBIT(*(&tpm->DMAMRA1 + channel * 2), alias << 0U);
}

#ifdef __cplusplus
}
#endif

#endif /* _LL_tpm_H_ */
