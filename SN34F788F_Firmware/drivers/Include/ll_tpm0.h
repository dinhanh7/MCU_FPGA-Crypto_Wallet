/***********************
 * SN34F78X SDK V2.1.4 *
 ***********************/

#ifndef _LL_TPM0_H_
#define _LL_TPM0_H_

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "ll_tpm.h"

/* Private Macro -------------------------------------------------------------*/
/// \cond
#define TPM_0_MCTRL_MRnIE(channel)   ((channel * 3 + 0 + (channel == 9 ? 2 : 0)))
#define TPM_0_MCTRL_MRnRST(channel)  ((channel * 3 + 1 + (channel == 9 ? 2 : 0)))
#define TPM_0_MCTRL_MRnSTOP(channel) ((channel * 3 + 2 + (channel == 9 ? 2 : 0)))

#define TPM_0_EM_EM(channel)  (channel)
#define TPM_0_EM_EMC(channel) (channel * 2 + 4)

#define TPM_0_PWMCTRL_PWMnEN(channel)    (channel)
#define TPM_0_PWMCTRL_PWMnMODE(channel)  (channel * 2 + 4)
#define TPM_0_PWMCTRL_PWMnIOEN(channel)  (channel + 20)
#define TPM_0_PWMCTRL_PWMnNIOEN(channel) (channel * 2 + 24)

#define TPM_0_RIS_MRnIF(channel) (channel + ((channel == 9 ? -4 : 0)))

#define TPM_0_IC_MRnIC(channel) (channel + ((channel == 9 ? -4 : 0)))

#define TPM_0_LOADCTRL_LOAD_MRn(channel) (channel * 2)

#define TPM_0_DMA_DMA_MRn(channel) (channel + ((channel == 9 ? -4 : 0)))
/// \endcond
/* Exported types ------------------------------------------------------------*/

/* Exported constants --------------------------------------------------------*/

/**
 * \defgroup tpm0_cm_mode CT16B0 Counting Mode
 * \ingroup tpm_control
 * @{
 */
#define LL_TPM0_CM_COUNTING_UP          TPM_0125_TMRCTRL_CM_EdgeUpCounting        /**< Edge-aligned Up-counting mode.  */
#define LL_TPM0_CM_COUNTING_DOWN        TPM_0125_TMRCTRL_CM_EdgeDownCounting      /**< Edge-aligned Down-counting mode.  */
#define LL_TPM0_CM_COUNTING_CENTER_DOWN TPM_0125_TMRCTRL_CM_DownCountingSetFlag   /**< The match interrupt flag is set during the down-counting period. */
#define LL_TPM0_CM_COUNTING_CENTER_UP   TPM_0125_TMRCTRL_CM_UpCountingSetFlag     /**< The match interrupt flag is set during the up-counting period. */
#define LL_TPM0_CM_COUNTING_CENTER_BOTH TPM_0125_TMRCTRL_CM_UpDownCountingSetFlag /**< The match interrupt flag is set during both up and down counting period. */
/**
 * @}
 */

/**
 * \defgroup tpm0_ctm_mode CT16B0 Counter Time Mode
 * \ingroup tpm_control
 * @{
 */
#define LL_TPM0_CTM_TIMER              TPM_01245_CNTCTRL_CTM_EveryRisingEdge /*!< Every rising edge of the input source */
#define LL_TPM0_CTM_COUNTING_UP_CAP0   TPM_01245_CNTCTRL_CTM_RisingEdgeCAP0  /*!< TC is incremented on rising edges on the CAP0 input selected */
#define LL_TPM0_CTM_COUNTING_DOWN_CAP0 TPM_01245_CNTCTRL_CTM_FallingEdgeCAP0 /*!< TC is incremented on falling edges on the CAP0 input selected */
#define LL_TPM0_CTM_COUNTING_BOTH_CAP0 TPM_01245_CNTCTRL_CTM_BothEdgeCAP0    /*!< TC is incremented on both edges on the CAP0 input selected */
/**
 * @}
 */

/**
 * \defgroup tpm0_pwm_pin_functionality CT16B0 PWM functionality when MRn=TC
 * \ingroup tpm_control
 * @{
 */
#define LL_TPM0_PWM_PIN_NONE   (0x00U) /*!< Do nothing */
#define LL_TPM0_PWM_PIN_LOW    (0x01U) /*!< CT16Bn_PWMn pin is Low when MRn=TC*/
#define LL_TPM0_PWM_PIN_HIGH   (0x02U) /*!< CT16Bn_PWMn pin is High when MRn=TC */
#define LL_TPM0_PWM_PIN_TOGGLE (0x03U) /*!< Toggle CT16Bn_PWMn pin hen MRn=TC*/
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
 * \defgroup tpm0_pwm_mode CT16B0 PWM output mode
 * \ingroup tpm_control
 * @{
 */
#define LL_TPM0_PWM_MODE_1 (0x00U) /*!< PWM is 0 when TC <(<=) MR3 during Up-counting(Down-counting) period */
#define LL_TPM0_PWM_MODE_2 (0x01U) /*!< PWM is 1 when TC <(<=) MR3 during Up-counting(Down-counting) period */
#define LL_TPM0_PWM_MODE_3 (0x02U) /*!< PWM is forced to 0 */
#define LL_TPM0_PWM_MODE_4 (0x03U) /*!< PWM is forced to 1 */
/**
 * @}
 */

/**
 * \defgroup tpm0_pwmn_gpio_selection CT16B0 PWMnN/GPIO selection
 * \ingroup tpm_control
 * @{
 */
#define LL_TPM0_PWMN_SEL_GPIO         (0x00U) /*!< CT16Bn_PWMnN pin is act as GPIO */
#define LL_TPM0_PWMN_SEL_INVERSE_HIGH (0x01U) /*!< PWMnN pin outputs the inverse signal with dead-band of PWM0,but same High during dead-band period */
#define LL_TPM0_PWMN_SEL_INVERSE_LOW  (0x02U) /*!< PWMnN pin outputs the inverse signal with dead-band of PWM0,but same Low during dead-band period */
#define LL_TPM0_PWMN_SEL_SAME         (0x03U) /*!< PWMnN pin outputs the same signal with dead-band of PWM0 */
/**
 * @}
 */

/**
 * \defgroup tpm0_mr_loadmode CT16B0 MRn load mode selection in center-aligned mode
 * \ingroup tpm_control
 * @{
 */
#define LL_TPM0_NO_EFFECT        TPM_0125_LOADCTRL_LOADMR0_None          /*!< No effect */
#define LL_TPM0_LOAD_TC_EQ_0     TPM_0125_LOADCTRL_LOADMR0_LoadAndFixed  /*!< Load MR0 value at TC=0 and MR0 value is unchanged at TC=MR9 */
#define LL_TPM0_LOAD_TC_EQ_0_MR9 TPM_0125_LOADCTRL_LOADMR0_LoadAndChange /*!< Load MR0 value at TC=0 and TC=MR9 */
/**
 * @}
 */

/**
 * \defgroup tpm0_breakpin_select CT16B0 trigger source selection of PWM channels break function
 * \ingroup tpm_control
 * @{
 */
#define LL_TPM0_BRK_SEL_DISABLE  TPM_0_BRKCTRL_BRKSEL_Disable  /*!< Disable */
#define LL_TPM0_BRK_SEL_BREAKPIN TPM_0_BRKCTRL_BRKSEL_BreakPin /*!< Break Pin */
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
__STATIC_INLINE void LL_TPM0_Enable(SN_CT16B0_Type *tpm)
{
    LL_REG_SBIT(tpm->TMRCTRL, TPM_0125_TMRCTRL_CEN);
}

/**
 * @brief  Disable Counter
 * @param  tpm TPM instance
 * @retval None
 */
__STATIC_INLINE void LL_TPM0_Disable(SN_CT16B0_Type *tpm)
{
    LL_REG_CBIT(tpm->TMRCTRL, TPM_0125_TMRCTRL_CEN);
}

/**
 * @brief  Judge if enabled TPM0
 * @param  tpm TPM instance
 * @retval 1: Enabled;   0: Disabled
 */
__STATIC_INLINE uint32_t LL_TPM0_IsEnabled(SN_CT16B0_Type *tpm)
{
    return ((LL_REG_READ(tpm->TMRCTRL) & TPM_0125_TMRCTRL_CEN) == TPM_0125_TMRCTRL_CEN);
}

/**
 * @brief  Reset TPM
 * @param  tpm TPM instance
 * @retval None
 */
__STATIC_INLINE void LL_TPM0_Reset(SN_CT16B0_Type *tpm)
{
    LL_REG_SBIT(tpm->TMRCTRL, TPM_0125_TMRCTRL_CRST);
}

/**
 * @brief  Get Reset State
 * @param  tpm TPM instance
 * @retval 0 : Reset finished,  1 : Resetting
 */
__STATIC_INLINE uint32_t LL_TPM0_GetResetState(SN_CT16B0_Type *tpm)
{
    return ((LL_REG_READ(tpm->TMRCTRL) & TPM_0125_TMRCTRL_CRST) >> TPM_0125_TMRCTRL_CRST_Pos);
}

/**
 * @brief  Counting mode selection
 * @param  tpm TPM instance
 * @param  mode Counting mode \ref tpm0_cm_mode
 * @retval None
 */
__STATIC_INLINE void LL_TPM0_SetCountingMode(SN_CT16B0_Type *tpm, uint32_t mode)
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
__STATIC_INLINE uint32_t LL_TPM0_GetCountingMode(SN_CT16B0_Type *tpm)
{
    return ((LL_REG_READ(tpm->TMRCTRL) & TPM_0125_TMRCTRL_CM) >> TPM_0125_TMRCTRL_CM_Pos);
}

/**
 * @brief  Set Counter value
 * @param  tpm TPM instance
 * @param  value Counter value range from 0x0 to 0xFFFF
 * @retval None
 */
__STATIC_INLINE void LL_TPM0_SetTimeCounter(SN_CT16B0_Type *tpm, uint32_t value)
{
    LL_REG_CBIT(tpm->TC, TPM_TC_TC);
    LL_REG_SBIT(tpm->TC, value << TPM_TC_TC_Pos);
}

/**
 * @brief  Get Counter value
 * @param  tpm TPM instance
 * @retval Counter value
 */
__STATIC_INLINE uint32_t LL_TPM0_GetTimeCounter(SN_CT16B0_Type *tpm)
{
    return ((LL_REG_READ(tpm->TC) & TPM_TC_TC) >> TPM_TC_TC_Pos);
}

/**
 * @brief  Set Prescaler value
 * @param  tpm TPM instance
 * @param  value Prescaler value range from 0x0 to 0xFF
 * @retval None
 */
__STATIC_INLINE void LL_TPM0_SetPrescaler(SN_CT16B0_Type *tpm, uint32_t value)
{
    LL_REG_CBIT(tpm->PRE, TPM_PRE_PRE);
    LL_REG_SBIT(tpm->PRE, value << TPM_PRE_PRE_Pos);
}

/**
 * @brief  Get Prescaler value
 * @param  tpm TPM instance
 * @retval Prescaler value
 */
__STATIC_INLINE uint32_t LL_TPM0_GetPrescaler(SN_CT16B0_Type *tpm)
{
    return ((LL_REG_READ(tpm->PRE) & TPM_PRE_PRE) >> TPM_PRE_PRE_Pos);
}

/**
 * @brief  Set PC Prescaler value
 * @param  tpm TPM instance
 * @param  value Prescaler value range from 0x0 to 0xFF
 * @retval None
 */
__STATIC_INLINE void LL_TPM0_SetPrescalerPC(SN_CT16B0_Type *tpm, uint32_t value)
{
    LL_REG_CBIT(tpm->PC, TPM_PC_PC);
    LL_REG_SBIT(tpm->PC, value << TPM_PC_PC_Pos);
}

/**
 * @brief  Get PC Prescaler value
 * @param  tpm TPM instance
 * @retval PC Prescaler value
 */
__STATIC_INLINE uint32_t LL_TPM0_GetPrescalerPC(SN_CT16B0_Type *tpm)
{
    return ((LL_REG_READ(tpm->PC) & TPM_PC_PC) >> TPM_PC_PC_Pos);
}

/**
 * @brief  Set Counter/Timer Mode
 * @param  tpm TPM instance
 * @param  mode Counter/Timer Mode \ref tpm0_ctm_mode
 * @retval None
 */
__STATIC_INLINE void LL_TPM0_SetCounterTimeMode(SN_CT16B0_Type *tpm, uint32_t mode)
{
    LL_REG_CBIT(tpm->CNTCTRL, TPM_01245_CNTCTRL_CTM);
    LL_REG_SBIT(tpm->CNTCTRL, mode << TPM_01245_CNTCTRL_CTM_Pos);
}

/**
 * @brief  Get Counter/Timer Mode
 * @param  tpm TPM instance
 * @retval Counter/Timer Mode
 */
__STATIC_INLINE uint32_t LL_TPM0_GetCounterTimeMode(SN_CT16B0_Type *tpm)
{
    return ((LL_REG_READ(tpm->CNTCTRL) & TPM_01245_CNTCTRL_CTM) >> TPM_01245_CNTCTRL_CTM_Pos);
}

/**
 * @brief  Enable generating an interrupt when MRn matches TC
 * @param  tpm TPM instance
 * @param  channel Channel number, \ref tpm_channel
 *         tpm0 just support channel 0,1,2,3,9
 * @retval None
 */
__STATIC_INLINE void LL_TPM0_EnableIT_MRnIE(SN_CT16B0_Type *tpm, uint32_t channel)
{
    LL_REG_SBIT(tpm->MCTRL, 1 << TPM_0_MCTRL_MRnIE(channel));
}

/**
 * @brief  Disable generating an interrupt when MRn matches TC
 * @param  tpm TPM instance
 * @param  channel Channel number, \ref tpm_channel
 *         tpm0 just support channel 0,1,2,3,9
 * @retval None
 */
__STATIC_INLINE void LL_TPM0_DisableIT_MRnIE(SN_CT16B0_Type *tpm, uint32_t channel)
{
    LL_REG_CBIT(tpm->MCTRL, 1 << TPM_0_MCTRL_MRnIE(channel));
}

/**
 * @brief  Judge if MRnIE  Interrupt is enabled
 * @param  tpm TPM instance
 * @param  channel Channel number, \ref tpm_channel
 *         tpm0 just support channel 0,1,2,3,9
 * @retval 0: disabled, 1: enabled.
 */
__STATIC_INLINE uint32_t LL_TPM0_IsEnabledIT_MRnIE(SN_CT16B0_Type *tpm, uint32_t channel)
{
    return ((LL_REG_READ(tpm->MCTRL) >> TPM_0_MCTRL_MRnIE(channel)) & 0x01);
}

/**
 * @brief  Enable reset TC when MRn matches TC
 * @param  tpm TPM instance
 * @param  channel Channel number, \ref tpm_channel
 *         tpm0 just support channel 0,1,2,3,9
 * @retval None
 */
__STATIC_INLINE void LL_TPM0_Enable_MRnRST(SN_CT16B0_Type *tpm, uint32_t channel)
{
    LL_REG_SBIT(tpm->MCTRL, 1 << TPM_0_MCTRL_MRnRST(channel));
}

/**
 * @brief  Disable reset TC when MRn matches TC
 * @param  tpm TPM instance
 * @param  channel Channel number, \ref tpm_channel
 *         tpm0 just support channel 0,1,2,3,9
 * @retval None
 */
__STATIC_INLINE void LL_TPM0_Disable_MRnRST(SN_CT16B0_Type *tpm, uint32_t channel)
{
    LL_REG_CBIT(tpm->MCTRL, 1 << TPM_0_MCTRL_MRnRST(channel));
}

/**
 * @brief  Judge if MRnRST is enabled
 * @param  tpm TPM instance
 * @param  channel Channel number, \ref tpm_channel
 *         tpm0 just support channel 0,1,2,3,9
 * @retval 0: disabled, 1: enabled.
 */
__STATIC_INLINE uint32_t LL_TPM0_IsEnabled_MRnRST(SN_CT16B0_Type *tpm, uint32_t channel)
{
    return ((LL_REG_READ(tpm->MCTRL) >> TPM_0_MCTRL_MRnRST(channel)) & 0x01);
}

/**
 * @brief  Stop TC and PC and clear CEN bit when MRn matches TC
 * @param  tpm TPM instance
 * @param  channel Channel number, \ref tpm_channel
 *         tpm0 just support channel 0,1,2,3,9
 * @retval None
 */
__STATIC_INLINE void LL_TPM0_Enable_MRnSTOP(SN_CT16B0_Type *tpm, uint32_t channel)
{
    LL_REG_SBIT(tpm->MCTRL, 1 << TPM_0_MCTRL_MRnSTOP(channel));
}

/**
 * @brief  Disable reset TC when MRn matches TC
 * @param  tpm TPM instance
 * @param  channel Channel number, \ref tpm_channel
 *         tpm0 just support channel 0,1,2,3,9
 * @retval None
 */
__STATIC_INLINE void LL_TPM0_Disable_MRnSTOP(SN_CT16B0_Type *tpm, uint32_t channel)
{
    LL_REG_CBIT(tpm->MCTRL, 1 << TPM_0_MCTRL_MRnSTOP(channel));
}

/**
 * @brief  Judge if MRnRST is enabled
 * @param  tpm TPM instance
 * @param  channel Channel number, \ref tpm_channel
 *         tpm0 just support channel 0,1,2,3,9
 * @retval 0: disabled, 1: enabled.
 */
__STATIC_INLINE uint32_t LL_TPM0_IsEnabled_MRnSTOP(SN_CT16B0_Type *tpm, uint32_t channel)
{
    return ((LL_REG_READ(tpm->MCTRL) >> TPM_0_MCTRL_MRnSTOP(channel)) & 0x01);
}

/**
 * @brief  Set Timer counter match value to MRn register
 * @param  tpm TPM instance
 * @param  value Timer counter match value range from 0 to 0xFFFF
 * @param  mrx which MR number to set, just supports mrx = 0,1,2,3,9
 * @retval None
 */
__STATIC_INLINE void LL_TPM0_SetTCMatchValueToMRn(SN_CT16B0_Type *tpm, uint32_t value, uint32_t mrx)
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
__STATIC_INLINE uint32_t LL_TPM0_GetTCMatchValueFromMRn(SN_CT16B0_Type *tpm, uint32_t mrx)
{
    return ((*(&tpm->MR0 + mrx + (mrx == 9 ? 1 : 0)) & 0xFFFF) >> 0U);
}

/**
 * @brief  Capture/Reset on CT16Bn_CAP0 signal rising edge
 * @param  tpm TPM instance
 * @retval None
 */
__STATIC_INLINE void LL_TPM0_Enable_CAP0RE(SN_CT16B0_Type *tpm)
{
    LL_REG_SBIT(tpm->CAPCTRL, TPM_01245_CAPCTRL_CAP0RE);
}

/**
 * @brief  Disable CAP0RE
 * @param  tpm TPM instance
 * @retval None
 */
__STATIC_INLINE void LL_TPM0_Disable_CAP0RE(SN_CT16B0_Type *tpm)
{
    LL_REG_CBIT(tpm->CAPCTRL, TPM_01245_CAPCTRL_CAP0RE);
}

/**
 * @brief  Judge if CAP0RE is enabled
 * @param  tpm TPM instance
 * @retval 0: disabled, 1: enabled.
 */
__STATIC_INLINE uint32_t LL_TPM0_IsEnabled_CAP0RE(SN_CT16B0_Type *tpm)
{
    return ((LL_REG_READ(tpm->CAPCTRL) & TPM_01245_CAPCTRL_CAP0RE) == TPM_01245_CAPCTRL_CAP0RE);
}

/**
 * @brief  Capture/Reset on CT16Bn_CAP0 signal falling edge
 * @param  tpm TPM instance
 * @retval None
 */
__STATIC_INLINE void LL_TPM0_Enable_CAP0FE(SN_CT16B0_Type *tpm)
{
    LL_REG_SBIT(tpm->CAPCTRL, TPM_01245_CAPCTRL_CAP0FE);
}

/**
 * @brief  Disable CAP0FE
 * @param  tpm TPM instance
 * @retval None
 */
__STATIC_INLINE void LL_TPM0_Disable_CAP0FE(SN_CT16B0_Type *tpm)
{
    LL_REG_CBIT(tpm->CAPCTRL, TPM_01245_CAPCTRL_CAP0FE);
}

/**
 * @brief  Judge if CAP0FE is enabled
 * @param  tpm TPM instance
 * @retval 0: disabled, 1: enabled.
 */
__STATIC_INLINE uint32_t LL_TPM0_IsEnabled_CAP0FE(SN_CT16B0_Type *tpm)
{
    return ((LL_REG_READ(tpm->CAPCTRL) & TPM_01245_CAPCTRL_CAP0FE) == TPM_01245_CAPCTRL_CAP0FE);
}

/**
 * @brief  Interrupt on CT16Bn_CAP0 event : a CAP0 load due to a CT16Bn_CAP0 event will generate an interrupt.
 * @param  tpm TPM instance
 * @retval None
 */
__STATIC_INLINE void LL_TPM0_EnableIT_CAP0IE(SN_CT16B0_Type *tpm)
{
    LL_REG_SBIT(tpm->CAPCTRL, TPM_01245_CAPCTRL_CAP0IE);
}

/**
 * @brief  Disable CAP0IE
 * @param  tpm TPM instance
 * @retval None
 */
__STATIC_INLINE void LL_TPM0_DisableIT_CAP0IE(SN_CT16B0_Type *tpm)
{
    LL_REG_CBIT(tpm->CAPCTRL, TPM_01245_CAPCTRL_CAP0IE);
}

/**
 * @brief  Judge if CAP0IE is enabled
 * @param  tpm TPM instance
 * @retval 0: disabled, 1: enabled.
 */
__STATIC_INLINE uint32_t LL_TPM0_IsEnabledIT_CAP0IE(SN_CT16B0_Type *tpm)
{
    return ((LL_REG_READ(tpm->CAPCTRL) & TPM_01245_CAPCTRL_CAP0IE) == TPM_01245_CAPCTRL_CAP0IE);
}

/**
 * @brief  CAP0 function enable
 * @param  tpm TPM instance
 * @retval None
 */
__STATIC_INLINE void LL_TPM0_Enable_CAP0EN(SN_CT16B0_Type *tpm)
{
    LL_REG_SBIT(tpm->CAPCTRL, TPM_01245_CAPCTRL_CAP0EN);
}

/**
 * @brief  Disable CAP0EN
 * @param  tpm TPM instance
 * @retval None
 */
__STATIC_INLINE void LL_TPM0_Disable_CAP0EN(SN_CT16B0_Type *tpm)
{
    LL_REG_CBIT(tpm->CAPCTRL, TPM_01245_CAPCTRL_CAP0EN);
}

/**
 * @brief  Judge if CAP0EN is enabled
 * @param  tpm TPM instance
 * @retval 0: disabled, 1: enabled.
 */
__STATIC_INLINE uint32_t LL_TPM0_IsEnabled_CAP0EN(SN_CT16B0_Type *tpm)
{
    return ((LL_REG_READ(tpm->CAPCTRL) & TPM_01245_CAPCTRL_CAP0EN) == TPM_01245_CAPCTRL_CAP0EN);
}

/**
 * @brief  Get Timer counter (TC) match value from CAP0
 * @param  tpm TPM instance
 * @retval Timer counter (TC) match value
 */
__STATIC_INLINE uint32_t LL_TPM0_GetTCMatchValueFromCAP0(SN_CT16B0_Type *tpm)
{
    return ((LL_REG_READ(tpm->CAP0) & TPM_01245_CAP0_CAP0) >> TPM_01245_CAP0_CAP0_Pos);
}

/**
 * @brief  When EMCn!=00b and MRn!=TC, it will drive the state of CT16Bn_PWMn output High
 * @param  tpm TPM instance
 * @param  channel channel number \ref tpm_channel
 *         tpm0 just supports channel 0,1,2,3
 * @retval None
 */
__STATIC_INLINE void LL_TPM0_SetPWMnHigh(SN_CT16B0_Type *tpm, uint32_t channel)
{
    LL_REG_SBIT(tpm->EM, 1 << TPM_0_EM_EM(channel));
}

/**
 * @brief  When EMCn!=00b and MRn!=TC, it will drive the state of CT16Bn_PWMn output Low
 * @param  tpm TPM instance
 * @param  channel channel number \ref tpm_channel
 *         tpm0 just supports channel 0,1,2,3
 * @retval None
 */
__STATIC_INLINE void LL_TPM0_SetPWMnLow(SN_CT16B0_Type *tpm, uint32_t channel)
{
    LL_REG_CBIT(tpm->EM, 1 << TPM_0_EM_EM(channel));
}

/**
 * @brief  Get CT16Bn_PWMn output state when EMCn!=00b and MRn!=TC
 * @param  tpm TPM instance
 * @param  channel channel number \ref tpm_channel
 *         tpm0 just supports channel 0,1,2,3
 * @retval 1: high, 0: low
 */
__STATIC_INLINE uint32_t LL_TPM0_GetPWMnState(SN_CT16B0_Type *tpm, uint32_t channel)
{
    return ((LL_REG_READ(tpm->EM) >> TPM_0_EM_EM(channel)) & 0x01);
}

/**
 * @brief  Set CT16Bn_PWMn functionality when MR0=TC
 * @param  tpm TPM instance
 * @param  function PWMn functionality \ref tpm0_pwm_pin_functionality
 * @param  channel channel number \ref tpm_channel
 *         tpm0 just supports channel 0,1,2,3
 * @retval None
 */
__STATIC_INLINE void LL_TPM0_SetPWMnFunction(SN_CT16B0_Type *tpm, uint32_t function, uint32_t channel)
{
    LL_REG_CBIT(tpm->EM, 0x03U << TPM_0_EM_EMC(channel));
    LL_REG_SBIT(tpm->EM, function << TPM_0_EM_EMC(channel));
}

/**
 * @brief  Get CT16Bn_PWMn functionality when MRn=TC
 * @param  tpm TPM instance
 * @param  channel channel number \ref tpm_channel
 *         tpm0 just supports channel 0,1,2,3
 * @retval
 *         0: Do nothing
 *         1: CT16Bn_PWMn pin is low when MRn=TC
 *         2: CT16Bn_PWMn pin is high when MRn=TC
 *         3: CT16Bn_PWMn pin is toggled when MRn=TC
 */
__STATIC_INLINE uint32_t LL_TPM0_GetPWM0Function(SN_CT16B0_Type *tpm, uint32_t channel)
{
    return ((LL_REG_READ(tpm->EM) >> TPM_0_EM_EMC(channel)) & 0x03U);
}

/**
 * @brief  Enable PWMn
 * @param  tpm TPM instance
 * @param  channel channel number \ref tpm_channel
 *         tpm0 just supports channel 0,1,2,3
 * @retval None
 */
__STATIC_INLINE void LL_TPM0_Enable_PWMn(SN_CT16B0_Type *tpm, uint32_t channel)
{
    LL_REG_SBIT(tpm->PWMCTRL, 1 << TPM_0_PWMCTRL_PWMnEN(channel));
}

/**
 * @brief  Disable PWMn
 * @param  tpm TPM instance
 * @param  channel channel number \ref tpm_channel
 *         tpm0 just supports channel 0,1,2,3
 * @retval None
 */
__STATIC_INLINE void LL_TPM0_Disable_PWMn(SN_CT16B0_Type *tpm, uint32_t channel)
{
    LL_REG_CBIT(tpm->PWMCTRL, 1 << TPM_0_PWMCTRL_PWMnEN(channel));
}

/**
 * @brief  Judge if enabled PWMn
 * @param  tpm TPM instance
 * @param  channel channel number \ref tpm_channel
 *         tpm0 just supports channel 0,1,2,3
 * @retval 1: Enabled;   0: Disabled
 */
__STATIC_INLINE uint32_t LL_TPM0_IsEnabled_PWMn(SN_CT16B0_Type *tpm, uint32_t channel)
{
    return ((LL_REG_READ(tpm->PWMCTRL) >> TPM_0_PWMCTRL_PWMnEN(channel)) & 0x01);
}

/**
 * @brief  Set PWMn mode
 * @param  tpm TPM instance
 * @param  mode PWMn mode \ref tpm0_pwm_mode
 * @param  channel channel number \ref tpm_channel
 *         tpm0 just supports channel 0,1,2,3
 * @retval None
 */
__STATIC_INLINE void LL_TPM0_SetPWMnMode(SN_CT16B0_Type *tpm, uint32_t mode, uint32_t channel)
{
    LL_REG_CBIT(tpm->PWMCTRL, 0x03U << TPM_0_PWMCTRL_PWMnMODE(channel));
    LL_REG_SBIT(tpm->PWMCTRL, mode << TPM_0_PWMCTRL_PWMnMODE(channel));
}

/**
 * @brief  Get PWMn mode
 * @param  tpm TPM instance
 * @param  channel channel number \ref tpm_channel
 *         tpm0 just supports channel 0,1,2,3
 * @retval PWM0 mode \ref tpm0_pwm_mode
 */
__STATIC_INLINE uint32_t LL_TPM0_GetPWMnMode(SN_CT16B0_Type *tpm, uint32_t channel)
{
    return ((LL_REG_READ(tpm->PWMCTRL) >> TPM_0_PWMCTRL_PWMnMODE(channel)) & 0x03U);
}

/**
 * @brief  Set CT16Bn_PWMn pin act as match output, and output depends on PWMnEN bit
 * @param  tpm TPM instance
 * @param  channel channel number \ref tpm_channel
 *         tpm0 just supports channel 0,1,2,3
 * @retval None
 */
__STATIC_INLINE void LL_TPM0_Enable_PWMnIOEN(SN_CT16B0_Type *tpm, uint32_t channel)
{
    LL_REG_SBIT(tpm->PWMCTRL, 1 << TPM_0_PWMCTRL_PWMnIOEN(channel));
}

/**
 * @brief  Set CT16Bn_PWMn pin is act as GPIO
 * @param  tpm TPM instance
 * @param  channel channel number \ref tpm_channel
 *         tpm0 just supports channel 0,1,2,3
 * @retval None
 */
__STATIC_INLINE void LL_TPM0_Disable_PWMnIOEN(SN_CT16B0_Type *tpm, uint32_t channel)
{
    LL_REG_CBIT(tpm->PWMCTRL, 1 << TPM_0_PWMCTRL_PWMnIOEN(channel));
}

/**
 * @brief  Judge if enabled PWMnIOEN
 * @param  tpm TPM instance
 * @param  channel channel number \ref tpm_channel
 *         tpm0 just supports channel 0,1,2,3
 * @retval 1: Enabled;   0: Disabled
 */
__STATIC_INLINE uint32_t LL_TPM0_IsEnabled_PWMnIOEN(SN_CT16B0_Type *tpm, uint32_t channel)
{
    return ((LL_REG_READ(tpm->PWMCTRL) >> TPM_0_PWMCTRL_PWMnIOEN(channel)) & 0x01);
}

/**
 * @brief  Set CT16Bn_PWMnN/GPIO selection
 * @param  tpm TPM instance
 * @param  selection selection \ref tpm0_pwmn_gpio_selection
 * @param  channel channel number \ref tpm_channel
 *         tpm0 just supports channel 0,1,2,3
 * @retval None
 */
__STATIC_INLINE void LL_TPM0_SetPWMNnSelection(SN_CT16B0_Type *tpm, uint32_t selection, uint32_t channel)
{
    LL_REG_CBIT(tpm->PWMCTRL, 0x03U << TPM_0_PWMCTRL_PWMnNIOEN(channel));
    LL_REG_SBIT(tpm->PWMCTRL, selection << TPM_0_PWMCTRL_PWMnNIOEN(channel));
}

/**
 * @brief  Get CT16Bn_PWM0N/GPIO selection
 * @param  tpm TPM instance
 * @param  channel channel number \ref tpm_channel
 *         tpm0 just supports channel 0,1,2,3
 * @retval selection \ref tpm0_pwmn_gpio_selection
 */
__STATIC_INLINE uint32_t LL_TPM0_GetPWMNnSelection(SN_CT16B0_Type *tpm, uint32_t channel)
{
    return ((LL_REG_READ(tpm->PWMCTRL) >> TPM_0_PWMCTRL_PWMnNIOEN(channel)) & 0x03U);
}

/**
 * @brief  Judge if has Match channel n interrupt flag
 * @param  tpm TPM instance
 * @param  channel channel number \ref tpm_channel
 *         tpm0 just supports channel 0,1,2,3,9
 * @retval 0: No interrupt on match channel n
 *         1: Interrupt requirements met on match channel n.
 */
__STATIC_INLINE uint32_t LL_TPM0_IsActiveFlag_MRnIF(SN_CT16B0_Type *tpm, uint32_t channel)
{
    return ((LL_REG_READ(tpm->RIS) >> TPM_0_RIS_MRnIF(channel)) & 0x01);
}

/**
 * @brief  Judge if has Capture channel 0 interrupt flag
 * @param  tpm TPM instance
 * @retval 0: No interrupt on CAP0
 *         1: Interrupt requirements met on CAP0
 */
__STATIC_INLINE uint32_t LL_TPM0_IsActiveFlag_CAP0IF(SN_CT16B0_Type *tpm)
{
    return ((LL_REG_READ(tpm->RIS) & TPM_0_RIS_CAP0IF) == TPM_0_RIS_CAP0IF);
}

/**
 * @brief  Judge if has Break Interrupt flag
 * @param  tpm TPM instance
 * @retval 0: No break condition occurs
 *         1: Break condition occurs
 */
__STATIC_INLINE uint32_t LL_TPM0_IsActiveFlag_BRKIF(SN_CT16B0_Type *tpm)
{
    return ((LL_REG_READ(tpm->RIS) & TPM_0_RIS_BRKIF) == TPM_0_RIS_BRKIF);
}

/**
 * @brief  Clear MRnIF
 * @param  tpm TPM instance
 * @param  channel channel number \ref tpm_channel
 *         tpm0 just supports channel 0,1,2,3,9
 * @retval None
 */
__STATIC_INLINE void LL_TPM0_ClearFlag_MRnIF(SN_CT16B0_Type *tpm, uint32_t channel)
{
    LL_REG_SBIT(tpm->IC, 1 << TPM_0_IC_MRnIC(channel));
}

/**
 * @brief  Clear CAP0IF
 * @param  tpm TPM instance
 * @retval None
 */
__STATIC_INLINE void LL_TPM0_ClearFlag_CAP0IF(SN_CT16B0_Type *tpm)
{
    LL_REG_SBIT(tpm->IC, TPM_0_IC_CAP0IC);
}

/**
 * @brief  Clear BRKIF
 * @param  tpm TPM instance
 * @retval None
 */
__STATIC_INLINE void LL_TPM0_ClearFlag_BRKIF(SN_CT16B0_Type *tpm)
{
    LL_REG_SBIT(tpm->IC, TPM_0_IC_BRKIC);
}

/**
 * @brief  Set Count of PWMmN output dead-band period time
 *         PWM0N output dead-band period time=DB*CT16Bn_PCLK*(PR+1) cycle
 * @param  tpm TPM instance
 * @param  pwmx which pwm number to set, range from 0 to 3
 * @param  db_period count of dead-band period time
 * @retval None
 */
__STATIC_INLINE void LL_TPM0_SetDBPeriod(SN_CT16B0_Type *tpm, uint32_t pwmx, uint32_t db_period)
{
    LL_REG_CBIT(*(&tpm->PWM0NDB + pwmx), TPM_034_PWMmNDB_DB);
    LL_REG_SBIT(*(&tpm->PWM0NDB + pwmx), db_period << TPM_034_PWMmNDB_DB_Pos);
}

/**
 * @brief  Get Count of PWMmN output dead-band period time
 * @param  tpm TPM instance
 * @param  pwmx which pwm number to set, range from 0 to 3
 * @retval Dead-band period time
 */
__STATIC_INLINE uint32_t LL_TPM0_GetDBPeriod(SN_CT16B0_Type *tpm, uint32_t pwmx)
{
    return (LL_REG_RBIT(*(&tpm->PWM0NDB + pwmx), TPM_034_PWMmNDB_DB) >> TPM_034_PWMmNDB_DB_Pos);
}

/**
 * @brief  Set MRn load mode selection in center-aligned mode
 * @param  tpm TPM instance
 * @param  selection selection \ref tpm0_mr_loadmode
 * @param  channel channel number \ref tpm_channel
 *         tpm0 just supports channel 0,1,2,3
 * @retval None
 */
__STATIC_INLINE void LL_TPM0_SetMRnLoadMode(SN_CT16B0_Type *tpm, uint32_t selection, uint32_t channel)
{
    LL_REG_CBIT(tpm->LOADCTRL, 0x03U << TPM_0_LOADCTRL_LOAD_MRn(channel));
    LL_REG_SBIT(tpm->LOADCTRL, selection << TPM_0_LOADCTRL_LOAD_MRn(channel));
}

/**
 * @brief  Get MRn load mode selection in center-aligned mode
 * @param  tpm TPM instance
 * @param  channel channel number \ref tpm_channel
 *         tpm0 just supports channel 0,1,2,3
 * @retval 0: No effect
 *         1: Load MRn value at TC=0 and MRn value is unchanged at TC=MR9
 *         2: Reserved
 *         3: Load MRn value at TC=0 and TC=MR9
 */
__STATIC_INLINE uint32_t LL_TPM0_GetMRnLoadMode(SN_CT16B0_Type *tpm, uint32_t channel)
{
    return ((LL_REG_READ(tpm->LOADCTRL) >> TPM_0_LOADCTRL_LOAD_MRn(channel)) & 0x3U);
}

/**
 * @brief  MRn DMA request active enable
 * @param  tpm TPM instance
 * @param  channel channel number \ref tpm_channel
 *         tpm0 just supports channel 0,1,2,3,9
 * @retval None
 */
__STATIC_INLINE void LL_TPM0_Enable_DMAMRn(SN_CT16B0_Type *tpm, uint32_t channel)
{
    LL_REG_SBIT(tpm->DMA, 1 << TPM_0_DMA_DMA_MRn(channel));
}
/**
 * @brief  MRn DMA request active disable
 * @param  tpm TPM instance
 * @param  channel channel number \ref tpm_channel
 *         tpm0 just supports channel 0,1,2,3,9
 * @retval None
 */
__STATIC_INLINE void LL_TPM0_Disable_DMAMRn(SN_CT16B0_Type *tpm, uint32_t channel)
{
    LL_REG_CBIT(tpm->DMA, 1 << TPM_0_DMA_DMA_MRn(channel));
}
/**
 * @brief  Judge if enabled MRn DMA request active
 * @param  tpm TPM instance
 * @param  channel channel number \ref tpm_channel
 *         tpm0 just supports channel 0,1,2,3,9
 * @retval 1: Enabled;   0: Disabled
 */
__STATIC_INLINE uint32_t LL_TPM0_IsEnabled_DMAMRn(SN_CT16B0_Type *tpm, uint32_t channel)
{
    return ((LL_REG_READ(tpm->DMA) >> TPM_0_DMA_DMA_MRn(channel)) & 0x1U);
}

/**
 * @brief  CAP0 DMA request active enable
 * @param  tpm TPM instance
 * @retval None
 */
__STATIC_INLINE void LL_TPM0_Enable_DMACAP0(SN_CT16B0_Type *tpm)
{
    LL_REG_SBIT(tpm->DMA, TPM_0125_DMA_DMACAP0);
}
/**
 * @brief  CAP0 DMA request active disable
 * @param  tpm TPM instance
 * @retval None
 */
__STATIC_INLINE void LL_TPM0_Disable_DMACAP0(SN_CT16B0_Type *tpm)
{
    LL_REG_CBIT(tpm->DMA, TPM_0125_DMA_DMACAP0);
}
/**
 * @brief  Judge if enabled CAP0 DMA request active
 * @param  tpm TPM instance
 * @retval 1: Enabled;   0: Disabled
 */
__STATIC_INLINE uint32_t LL_TPM0_IsEnabled_DMACAP0(SN_CT16B0_Type *tpm)
{
    return ((LL_REG_READ(tpm->DMA) & TPM_0125_DMA_DMACAP0) == TPM_0125_DMA_DMACAP0);
}

/**
 * @brief  Set MRn alias for DMA access
 * @param  tpm TPM instance
 * @param  channel channel number \ref tpm_channel
 *         tpm0 just supports channel 0,1,2,3
 * @param  alias alias
 * @retval None
 */
__STATIC_INLINE void LL_TPM0_SetMRnAlias(SN_CT16B0_Type *tpm, uint16_t alias, uint32_t channel)
{
    LL_REG_CBIT(*(&tpm->DMAMRA1 + channel * 2), 0xFFFF << 0U);
    LL_REG_SBIT(*(&tpm->DMAMRA1 + channel * 2), alias << 0U);
}

/**
 * @brief  Set The trigger source of PWM channels break function
 * @param  tpm TPM instance
 * @param  selection selection \ref tpm0_breakpin_select
 * @retval None
 */
__STATIC_INLINE void LL_TPM0_SetBRKSelection(SN_CT16B0_Type *tpm, uint32_t selection)
{
    LL_REG_CBIT(tpm->BRKCTRL, TPM_0_BRKCTRL_BRKSEL);
    LL_REG_SBIT(tpm->BRKCTRL, selection << TPM_0_BRKCTRL_BRKSEL_Pos);
}

/**
 * @brief  Get The trigger source of PWM channels break function
 * @param  tpm TPM instance
 * @retval 4: Break pin;   0: Disabled;  other: Reserved
 */
__STATIC_INLINE uint32_t LL_TPM0_GetBRKSelection(SN_CT16B0_Type *tpm)
{
    return ((LL_REG_READ(tpm->BRKCTRL) & TPM_0_BRKCTRL_BRKSEL) >> TPM_0_BRKCTRL_BRKSEL_Pos);
}

/**
 * @brief  Set The trigger level high of PWM channels break function
 * @param  tpm TPM instance
 * @retval None
 */
__STATIC_INLINE void LL_TPM0_SetBRKTriggerLevelHigh(SN_CT16B0_Type *tpm)
{
    LL_REG_SBIT(tpm->BRKCTRL, TPM_0_BRKCTRL_BRKLEVEL);
}

/**
 * @brief  Set The trigger level low of PWM channels break function
 * @param  tpm TPM instance
 * @retval None
 */
__STATIC_INLINE void LL_TPM0_SetBRKTriggerLevelLow(SN_CT16B0_Type *tpm)
{
    LL_REG_CBIT(tpm->BRKCTRL, TPM_0_BRKCTRL_BRKLEVEL);
}

/**
 * @brief  Get The trigger level of PWM channels break function
 * @param  tpm TPM instance
 * @retval 0: low;  1: high
 */
__STATIC_INLINE uint32_t LL_TPM0_GetBRKTriggerLevel(SN_CT16B0_Type *tpm)
{
    return ((LL_REG_READ(tpm->BRKCTRL) & TPM_0_BRKCTRL_BRKLEVEL) >> TPM_0_BRKCTRL_BRKLEVEL_Pos);
}

/**
 * @brief  Set Break pin (BRK) debounce time
 * @param  tpm TPM instance
 * @param  time debounce time range from 0 to 7
 * @retval None
 */
__STATIC_INLINE void LL_TPM0_SetBRKDebounceTime(SN_CT16B0_Type *tpm, uint32_t time)
{
    LL_REG_CBIT(tpm->BRKCTRL, TPM_0_BRKCTRL_BRKDB);
    LL_REG_SBIT(tpm->BRKCTRL, time << TPM_0_BRKCTRL_BRKDB_Pos);
}

/**
 * @brief  Get Break pin (BRK) debounce time
 * @param  tpm TPM instance
 * @retval debounce time range from 0 to 7
 */
__STATIC_INLINE uint32_t LL_TPM0_GetBRKDebounceTime(SN_CT16B0_Type *tpm)
{
    return ((LL_REG_READ(tpm->BRKCTRL) & TPM_0_BRKCTRL_BRKDB) >> TPM_0_BRKCTRL_BRKDB_Pos);
}

/**
 * @brief  Enable generating an interrupt when BRKIF = 1
 * @param  tpm TPM instance
 * @retval None
 */
__STATIC_INLINE void LL_TPM0_EnableIT_BRKIE(SN_CT16B0_Type *tpm)
{
    LL_REG_SBIT(tpm->BRKCTRL, TPM_0_BRKCTRL_BRKIE);
}

/**
 * @brief  Disable generating an interrupt when BRKIF = 1
 * @param  tpm TPM instance
 * @retval None
 */
__STATIC_INLINE void LL_TPM0_DisableIT_BRKIE(SN_CT16B0_Type *tpm)
{
    LL_REG_CBIT(tpm->BRKCTRL, TPM_0_BRKCTRL_BRKIE);
}

/**
 * @brief  Judge if enabled generating an interrupt when BRKIF = 1
 * @param  tpm TPM instance
 * @retval 1: Enabled;   0: Disabled
 */
__STATIC_INLINE uint32_t LL_TPM0_IsEnabledIT_BRKIE(SN_CT16B0_Type *tpm)
{
    return ((LL_REG_READ(tpm->BRKCTRL) & TPM_0_BRKCTRL_BRKIE) == TPM_0_BRKCTRL_BRKIE);
}

#ifdef __cplusplus
}
#endif

#endif /* _LL_tpm_H_ */
