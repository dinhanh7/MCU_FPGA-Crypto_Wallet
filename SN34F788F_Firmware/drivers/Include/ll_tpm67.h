/***********************
 * SN34F78X SDK V2.1.4 *
 ***********************/

#ifndef _LL_TPM67_H_
#define _LL_TPM67_H_

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "ll_tpm.h"

/* Exported types ------------------------------------------------------------*/

/* Exported constants --------------------------------------------------------*/

/* Exported macros --------------------------------------------------------*/

// tpm calibration times

/* Exported functions --------------------------------------------------------*/
/**
 * @brief  Enable Counter
 * @param  tpm TPM instance
 * @retval None
 */
__STATIC_INLINE void LL_TPM67_Enable(SN_CT16B6_Type *tpm)
{
    LL_REG_SBIT(tpm->TMRCTRL, TPM_34678_TMRCTRL_CEN);
}

/**
 * @brief  Disable Counter
 * @param  tpm TPM instance
 * @retval None
 */
__STATIC_INLINE void LL_TPM67_Disable(SN_CT16B6_Type *tpm)
{
    LL_REG_CBIT(tpm->TMRCTRL, TPM_34678_TMRCTRL_CEN);
}

/**
 * @brief  Judge if enabled TPM67
 * @param  tpm TPM instance
 * @retval 1: Enabled;   0: Disabled
 */
__STATIC_INLINE uint32_t LL_TPM67_IsEnabled(SN_CT16B6_Type *tpm)
{
    return ((LL_REG_READ(tpm->TMRCTRL) & TPM_34678_TMRCTRL_CEN) == TPM_34678_TMRCTRL_CEN);
}

/**
 * @brief  Reset TPM
 * @param  tpm TPM instance
 * @retval None
 */
__STATIC_INLINE void LL_TPM67_Reset(SN_CT16B6_Type *tpm)
{
    LL_REG_SBIT(tpm->TMRCTRL, TPM_34678_TMRCTRL_CRST);
}

/**
 * @brief  Get Reset State
 * @param  tpm TPM instance
 * @retval 0 : Reset finished,  1 : Resetting
 */
__STATIC_INLINE uint32_t LL_TPM67_GetResetState(SN_CT16B6_Type *tpm)
{
    return ((LL_REG_READ(tpm->TMRCTRL) & TPM_34678_TMRCTRL_CRST) >> TPM_34678_TMRCTRL_CRST_Pos);
}

/**
 * @brief  Set Counter value
 * @param  tpm TPM instance
 * @param  value Counter value range from 0x0 to 0xFFFF
 * @retval None
 */
__STATIC_INLINE void LL_TPM67_SetTimeCounter(SN_CT16B6_Type *tpm, uint32_t value)
{
    LL_REG_CBIT(tpm->TC, TPM_TC_TC);
    LL_REG_SBIT(tpm->TC, value << TPM_TC_TC_Pos);
}

/**
 * @brief  Get Counter value
 * @param  tpm TPM instance
 * @retval Counter value
 */
__STATIC_INLINE uint32_t LL_TPM67_GetTimeCounter(SN_CT16B6_Type *tpm)
{
    return ((LL_REG_READ(tpm->TC) & TPM_TC_TC) >> TPM_TC_TC_Pos);
}

/**
 * @brief  Set Prescaler value
 * @param  tpm TPM instance
 * @param  value Prescaler value range from 0x0 to 0xFF
 * @retval None
 */
__STATIC_INLINE void LL_TPM67_SetPrescaler(SN_CT16B6_Type *tpm, uint32_t value)
{
    LL_REG_CBIT(tpm->PRE, TPM_PRE_PRE);
    LL_REG_SBIT(tpm->PRE, value << TPM_PRE_PRE_Pos);
}

/**
 * @brief  Get Prescaler value
 * @param  tpm TPM instance
 * @retval Prescaler value
 */
__STATIC_INLINE uint32_t LL_TPM67_GetPrescaler(SN_CT16B6_Type *tpm)
{
    return ((LL_REG_READ(tpm->PRE) & TPM_PRE_PRE) >> TPM_PRE_PRE_Pos);
}

/**
 * @brief  Set PC Prescaler value
 * @param  tpm TPM instance
 * @param  value Prescaler value range from 0x0 to 0xFF
 * @retval None
 */
__STATIC_INLINE void LL_TPM67_SetPrescalerPC(SN_CT16B6_Type *tpm, uint32_t value)
{
    LL_REG_CBIT(tpm->PC, TPM_PC_PC);
    LL_REG_SBIT(tpm->PC, value << TPM_PC_PC_Pos);
}

/**
 * @brief  Get PC Prescaler value
 * @param  tpm TPM instance
 * @retval PC Prescaler value
 */
__STATIC_INLINE uint32_t LL_TPM67_GetPrescalerPC(SN_CT16B6_Type *tpm)
{
    return ((LL_REG_READ(tpm->PC) & TPM_PC_PC) >> TPM_PC_PC_Pos);
}

/**
 * @brief  Enable generating an interrupt when MR0 matches TC
 * @param  tpm TPM instance
 * @retval None
 */
__STATIC_INLINE void LL_TPM67_EnableIT_MR0IE(SN_CT16B6_Type *tpm)
{
    LL_REG_SBIT(tpm->MCTRL, TPM_67_MCTRL_MR0IE);
}

/**
 * @brief  Disable generating an interrupt when MR0 matches TC
 * @param  tpm TPM instance
 * @retval None
 */
__STATIC_INLINE void LL_TPM67_DisableIT_MR0IE(SN_CT16B6_Type *tpm)
{
    LL_REG_CBIT(tpm->MCTRL, TPM_67_MCTRL_MR0IE);
}

/**
 * @brief  Judge if MR0IE  Interrupt is enabled
 * @param  tpm TPM instance
 * @retval 0: disabled, 1: enabled.
 */
__STATIC_INLINE uint32_t LL_TPM67_IsEnabledIT_MR0IE(SN_CT16B6_Type *tpm)
{
    return ((LL_REG_READ(tpm->MCTRL) & TPM_67_MCTRL_MR0IE) == TPM_67_MCTRL_MR0IE);
}

/**
 * @brief  Enable reset TC when MR0 matches TC
 * @param  tpm TPM instance
 * @retval None
 */
__STATIC_INLINE void LL_TPM67_Enable_MR0RST(SN_CT16B6_Type *tpm)
{
    LL_REG_SBIT(tpm->MCTRL, TPM_67_MCTRL_MR0RST);
}

/**
 * @brief  Disable reset TC when MR0 matches TC
 * @param  tpm TPM instance
 * @retval None
 */
__STATIC_INLINE void LL_TPM67_Disable_MR0RST(SN_CT16B6_Type *tpm)
{
    LL_REG_CBIT(tpm->MCTRL, TPM_67_MCTRL_MR0RST);
}

/**
 * @brief  Judge if MR0RST is enabled
 * @param  tpm TPM instance
 * @retval 0: disabled, 1: enabled.
 */
__STATIC_INLINE uint32_t LL_TPM67_IsEnabled_MR0RST(SN_CT16B6_Type *tpm)
{
    return ((LL_REG_READ(tpm->MCTRL) & TPM_67_MCTRL_MR0RST) == TPM_67_MCTRL_MR0RST);
}

/**
 * @brief  Stop TC and PC and clear CEN bit when MR0 matches TC
 * @param  tpm TPM instance
 * @retval None
 */
__STATIC_INLINE void LL_TPM67_Enable_MR0STOP(SN_CT16B6_Type *tpm)
{
    LL_REG_SBIT(tpm->MCTRL, TPM_67_MCTRL_MR0STOP);
}

/**
 * @brief  Disable MR0STOP
 * @param  tpm TPM instance
 * @retval None
 */
__STATIC_INLINE void LL_TPM67_Disable_MR0STOP(SN_CT16B6_Type *tpm)
{
    LL_REG_CBIT(tpm->MCTRL, TPM_67_MCTRL_MR0STOP);
}

/**
 * @brief  Judge if MR0STOP is enabled
 * @param  tpm TPM instance
 * @retval 0: disabled, 1: enabled.
 */
__STATIC_INLINE uint32_t LL_TPM67_IsEnabled_MR0STOP(SN_CT16B6_Type *tpm)
{
    return ((LL_REG_READ(tpm->MCTRL) & TPM_67_MCTRL_MR0STOP) == TPM_67_MCTRL_MR0STOP);
}

/**
 * @brief  Set Timer counter match value to MR0 register
 * @param  tpm TPM instance
 * @param  value Timer counter match value range from 0 to 0xFFFF
 * @retval None
 */
__STATIC_INLINE void LL_TPM67_SetTCMatchValueToMR0(SN_CT16B6_Type *tpm, uint32_t value)
{
    LL_REG_CBIT(tpm->MR0, TPM_01234567_MR0_MR);
    LL_REG_SBIT(tpm->MR0, value << TPM_01234567_MR0_MR_Pos);
}

/**
 * @brief  Get Timer counter (TC) match value
 * @param  tpm TPM instance
 * @retval Timer counter (TC) match value
 */
__STATIC_INLINE uint32_t LL_TPM67_GetTCMatchValueFromMR0(SN_CT16B6_Type *tpm)
{
    return ((LL_REG_READ(tpm->MR0) & TPM_01234567_MR0_MR) >> TPM_01234567_MR0_MR_Pos);
}

/**
 * @brief  Judge if has Match channel 0 interrupt flag
 * @param  tpm TPM instance
 * @retval 0: No interrupt on match channel 0
 *         1: Interrupt requirements met on match channel 0.
 */
__STATIC_INLINE uint32_t LL_TPM67_IsActiveFlag_MR0IF(SN_CT16B6_Type *tpm)
{
    return ((LL_REG_READ(tpm->RIS) & TPM_67_RIS_MR0IF) == TPM_67_RIS_MR0IF);
}

/**
 * @brief  Clear MR0IF
 * @param  tpm TPM instance
 * @retval None
 */
__STATIC_INLINE void LL_TPM67_ClearFlag_MR0IF(SN_CT16B6_Type *tpm)
{
    LL_REG_SBIT(tpm->IC, TPM_67_IC_MR0IC);
}

/**
 * @brief  MR0 DMA request active enable
 * @param  tpm TPM instance
 * @retval None
 */
__STATIC_INLINE void LL_TPM67_Enable_DMAMR0(SN_CT16B6_Type *tpm)
{
    LL_REG_SBIT(tpm->DMA, TPM_67_DMA_DMAMR0);
}
/**
 * @brief  MR0 DMA request active disable
 * @param  tpm TPM instance
 * @retval None
 */
__STATIC_INLINE void LL_TPM67_Disable_DMAMR0(SN_CT16B6_Type *tpm)
{
    LL_REG_CBIT(tpm->DMA, TPM_67_DMA_DMAMR0);
}
/**
 * @brief  Judge if enabled MR0 DMA request active
 * @param  tpm TPM instance
 * @retval 1: Enabled;   0: Disabled
 */
__STATIC_INLINE uint32_t LL_TPM67_IsEnabled_DMAMR0(SN_CT16B6_Type *tpm)
{
    return ((LL_REG_READ(tpm->DMA) & TPM_67_DMA_DMAMR0) == TPM_67_DMA_DMAMR0);
}

#ifdef __cplusplus
}
#endif

#endif /* _LL_tpm_H_ */
