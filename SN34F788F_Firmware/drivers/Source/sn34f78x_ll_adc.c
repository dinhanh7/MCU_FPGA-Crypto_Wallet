/***********************
 * SN34F78X SDK V2.1.4 *
 ***********************/

#include "sn34f78x_ll_adc.h"

#if defined(USE_FULL_LL_DRIVER)
/* Exported functions --------------------------------------------------------*/
/**
 * @brief  Initialize ADC registers according to the specified parameters in adc_init.
 * @param  adc ADC base Address
 * @param  adc_init pointer to a @ref LL_ADC_Init_T structure
 *         that contains the configuration information for the specified ADC peripheral.
 * @retval An ErrorStatus enumeration value:
 *           - LL_OK:     ADC registers are initialized according to adc_init content
 *           - LL_ERROR:  Not applicable
 */
LL_Status_T LL_ADC_Init(SN_ADC0_Type *adc, LL_ADC_Init_T *adc_init)
{
    /* Check the parameters */
    RET_FLAG_FALSE(IS_LL_ADC_INSTANCE(adc), LL_ERROR);
    RET_FLAG_FALSE(IS_LL_ADC_CONV_MODE(adc_init->conv_mode), LL_ERROR);
    RET_FLAG_FALSE(IS_LL_ADC_SCAN_NUM(adc_init->scan_ch_num), LL_ERROR);
    RET_FLAG_FALSE(IS_LL_ADC_AUTOPWDN(adc_init->autopwdn), LL_ERROR);
    RET_FLAG_FALSE(IS_LL_ADC_MCLK(adc_init->pre_clock), LL_ERROR);
    RET_FLAG_FALSE(CHECK_AUTOPWDN_IN_SINGLE_MODE(adc_init->conv_mode, adc_init->autopwdn), LL_ERROR);

    /* Set scan prescaler clock */
    LL_ADC_SetClockPrescaler(adc, adc_init->pre_clock);
    /* Set converter mode */
    LL_ADC_SetConverterMode(adc, adc_init->conv_mode);
    /* Set AUTOPWDN */
    if (adc_init->autopwdn == LL_ENABLE)
    {
        LL_ADC_Enable_AutoPowerDown(adc);
        /* Set power-down control signal select by auto */
        LL_ADC_SetADCCorePDSignalSource(adc, LL_ADC_CORE_PD_SIGNAL_FROM_ADCEN);
        LL_ADC_SetLDOPDSignalSource(adc, LL_ADC_LDO_PD_SIGNAL_FROM_ADCEN);
    }
    else
    {
        LL_ADC_Disable_AutoPowerDown(adc);
        /* Set power-down control signal select by manual */
        LL_ADC_SetADCCorePDSignalSource(adc, LL_ADC_CORE_PD_SIGNAL_FROM_XPD_ADC);
        LL_ADC_SetLDOPDSignalSource(adc, LL_ADC_LDO_PD_SIGNAL_FROM_XPD_LDO);
    }

    /* Set scan channel number */
    if (adc_init->scan_ch_num == 0)
    {
        adc_init->scan_ch_num = 1;
    }
    LL_ADC_SetScanChannelNumber(adc, adc_init->scan_ch_num);

    /* Set ADC normal operating */
    LL_ADC_SetADCCorePDControlMode(adc, LL_ADC_CORE_NORMAL_OP_MODE);

    /* ADC offset calibration */
    LL_ADC_Calibration(adc, adc_init);

    return LL_OK;
}

/**
 * @brief  De-initialize ADC registers .
 * @param  adc ADC base Address
 * @retval An ErrorStatus enumeration value:
 *           - LL_OK:     ADC registers are de-initialized
 *           - LL_ERROR:  de-initialized fail
 */
LL_Status_T LL_ADC_DeInit(SN_ADC0_Type *adc)
{
    /* Disable ADC */
    LL_ADC_Disable(adc);

    return LL_OK;
}

/**
 * @brief  Set each @ref LL_ADC_Init_T field to default value.
 * @param  adc_init pointer to a @ref LL_ADC_Init_T structure  whose fields will be set to default values.
 * @retval None
 */
void LL_ADC_StructInit(LL_ADC_Init_T *adc_init)
{
    if (adc_init != NULL)
    {
        adc_init->conv_mode   = LL_ADC_CONVMODE_SINGLE_STEP;
        adc_init->scan_ch_num = 0x11;
        adc_init->autopwdn    = LL_DISABLE;
        adc_init->pre_clock   = 0x5;
    }
}

void LL_ADC_Calibration(SN_ADC0_Type *adc, LL_ADC_Init_T *adc_init)
{
    uint16_t data_total    = 0;
    uint16_t data_avg      = 0;
    uint16_t first_conv_ch = LL_ADC_GetSequencerRanks(adc, LL_ADC_REG_RANK_1);

    /* Set ADC single step conversion mode */
    LL_ADC_SetConverterMode(adc, LL_ADC_CONVMODE_SINGLE_STEP);

    /* Set channel num to 1 */
    LL_ADC_SetScanChannelNumber(adc, 1);

    /* Enable calibration */
    LL_ADC_Enable_CalibrationMode(adc);

    /* Clear XBVOS when enable calibration */
    LL_ADC_SetCalibrationCode(adc, 0);

    /* If ADCEN == 0, set ADCEN to 1 then delay for ADC warm-up time */
    if (LL_FLAG_EQU(LL_ADC_IsEnabled(adc), 0U))
    {
        /* Enable ADC */
        LL_ADC_Enable(adc);
    }

    /* Save 10 timers data from CH0 conversion */
    for (int i = 0; i < ADC_CALIBRATION_TIMES; i++)
    {
        /* Start ADC conversion */
        LL_ADC_StartConversionSW(adc);
        /* Wait The 1st conversion channel finish */
        while (LL_ADC_IsActiveFlag_EOC(adc, first_conv_ch) == 0)
            ;
        /* Clear channel flag */
        LL_ADC_ClearFlag_EOC(adc, first_conv_ch);
        data_total += LL_ADC_ReadChannelData(adc, first_conv_ch);
    }

    /* Disable calibration */
    LL_ADC_Disable_CalibrationMode(adc);

    data_avg = data_total / ADC_CALIBRATION_TIMES;
    if (data_avg > 127)
    {
        /* Set calibration value */
        data_avg = 127;
    }

    /* Set XBVOS  */
    LL_ADC_SetCalibrationCode(adc, data_avg);

    /* Recovery ADC previous conversion mode */
    LL_ADC_SetConverterMode(adc, adc_init->conv_mode);

    /* Recovery ADC previous scan number */
    LL_ADC_SetScanChannelNumber(adc, adc_init->scan_ch_num);

    /* Disable ADC */
    LL_ADC_Disable(adc);

    /* Clear ADC conversion done and stop flag */
    LL_ADC_ClearFlag_DONE(adc);
    LL_ADC_ClearFlag_STOP(adc);
}

#endif /* USE_FULL_LL_DRIVER */
