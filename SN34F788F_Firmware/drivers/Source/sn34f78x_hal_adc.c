/***********************
 * SN34F78X SDK V2.1.4 *
 ***********************/

#include "sn34f78x_hal_adc.h"
#if (configUSE_ADC_PERIPHERAL == 1)
/* Initialization/de-initialization functions  ********************************/
/**
 * @brief  Initializes the ADC mode according to the specified parameters in the ADC_Init_T and create the associated handle.
 * @param  hadc Pointer to a ADC_Handle_T structure that contains the configuration information for the specified ADC module.
 * @retval HAL status
 */
HAL_Status_T HAL_ADC_Init(ADC_Handle_T *hadc)
{
    /* Check the ADC handle */
    ASSERT_PARAM(hadc);

    /* Check the ADC instance */
    ASSERT_PARAM(IS_ADC_INSTANCE(hadc));

    /* Check ADC converter mode */
    ASSERT_PARAM(IS_ADC_CONV_MODE(hadc));

    /* Check ADC scan conversion channel numbers */
    ASSERT_PARAM(IS_ADC_SCAN_NUM(hadc));

    /* Check ADC AUTOPWDN */
    ASSERT_PARAM(IS_ADC_AUTOPWDN(hadc));

    /* Check ADC prescaler clock */
    ASSERT_PARAM(IS_ADC_MCLK(hadc));

    /* Check ADC SCANNUM when single step mode */
    RET_FLAG_FALSE(CHECK_SCANNUM(hadc), HAL_ERROR);

    /* AUTOPWDN is only invalid when conv_mode is ADC_CONVMODE_SINGLE_STEP or ADC_CONVMODE_SINGLE_SCAN. */
    RET_FLAG_FALSE(CHECK_AUTOPWDN_IN_SINGLE_MODE(hadc), HAL_ERROR);

    /* Protect the process */
    TAKE_MUTEX(hadc);

    /* Check whether is already initialized */
    if (HAL_FLAG_EQU(hadc->state, HAL_STATE_RESET))
    {
#if (configUSE_ADC_REGISTER_CALLBACKS == 1)
        if (hadc->MspInitCallback == NULL)
        {
            hadc->MspInitCallback = HAL_ADC_MspInit;
        }
        if (hadc->MspDeInitCallback == NULL)
        {
            hadc->MspDeInitCallback = HAL_ADC_MspDeInit;
        }
        if (hadc->ConvCpltCallback == NULL)
        {
            hadc->ConvCpltCallback = HAL_ADC_ConvCpltCallback;
        }
        if (hadc->StopCallback == NULL)
        {
            hadc->StopCallback = HAL_ADC_StopCallback;
        }
        if (hadc->LevelOutOfWindowCallback == NULL)
        {
            hadc->LevelOutOfWindowCallback = HAL_ADC_LevelOutOfWindowCallback;
        }
        if (hadc->ErrorCallback == NULL)
        {
            hadc->ErrorCallback = HAL_ADC_ErrorCallback;
        }
        hadc->MspInitCallback(hadc);
#else
        HAL_ADC_MspInit(hadc);
#endif /* configUSE_ADC_REGISTER_CALLBACKS */

#if (configUSE_ADC_NONBLOCKING_DMA == 1U)
        for (uint8_t i = 0; i <= ADC_DMA_CHANNEL_3; i++)
        {
            if (hadc->hdma[i])
            {
                hadc->hdma[i]->parent = (void *)hadc;
                HAL_DMA_RegisterCallback(hadc->hdma[i], HAL_DMA_XFER_CPLT_CB_ID, _adc_dma_xfer_done_callback);
                HAL_DMA_RegisterCallback(hadc->hdma[i], HAL_DMA_XFER_ABORT_CB_ID, _adc_dma_xfer_abort_callback);
                HAL_DMA_RegisterCallback(hadc->hdma[i], HAL_DMA_XFER_ERROR_CB_ID, _adc_dma_xfer_error_callback);
            }
        }

#endif /* configUSE_ADC_NONBLOCKING_DMA */

        /* Update the state */
        HAL_FLAG_WRITE(hadc->state, HAL_STATE_BUSY);
        /* Set scan prescaler clock */
        HAL_REG_WRITE(hadc->instance->PRE_b.MCLKDIV, hadc->init.pre_clock);
        /* Set converter mode */
        HAL_REG_WRITE(hadc->instance->CTRL_b.COVNMODE, hadc->init.conv_mode);
        /* Set AUTOPWDN */
        HAL_REG_WRITE(hadc->instance->CTRL_b.AUTOPWDN, hadc->init.autopwdn);
        /* Set power-down control signal select by auto or manual */
        HAL_REG_WRITE(hadc->instance->MISC_b.XPD_LDOSEL, (~hadc->init.autopwdn) & 0x1);
        HAL_REG_WRITE(hadc->instance->MISC_b.XPD_ADCSEL, (~hadc->init.autopwdn) & 0x1);
        /* Set scan channel number */
        if (hadc->init.scan_ch_num == 0)
            hadc->init.scan_ch_num = 1;
        HAL_REG_WRITE(hadc->instance->CTRL_b.SCANNUM, hadc->init.scan_ch_num - 1);

        /* Set ADC normal operating */
        HAL_REG_WRITE(hadc->instance->MISC_b.XPD_ADC, 0);
    }
    else
    {
        /* Update the state */
        hadc->error_code = HAL_ERROR_REINIT;
        return HAL_ERROR;
    }

    /* ADC offset calibration */
    _offset_calibration(hadc);

    /* update state to ready */
    hadc->error_code = HAL_OK;
    HAL_FLAG_WRITE(hadc->state, HAL_STATE_READY);
    /* init mutex */
    GIVE_MUTEX(hadc);
    return HAL_OK;
}

/**
 * @brief  DeInitializes the ADC peripheral.
 * @param  hadc Pointer to a ADC_Handle_T structure that contains the configuration information for the specified ADC.
 * @retval HAL status
 */
HAL_Status_T HAL_ADC_DeInit(ADC_Handle_T *hadc)
{
    /* Check the ADC handle */
    ASSERT_PARAM(hadc);

    /* Check the ADC instance */
    ASSERT_PARAM(IS_ADC_INSTANCE(hadc));

    /* Protect the process */
    TAKE_MUTEX(hadc);

    /* Disable ADC */
    HAL_REG_WRITE(hadc->instance->CTRL_b.ADCEN, HAL_DISABLE);

#if (configUSE_ADC_REGISTER_CALLBACKS == 1)
    if (hadc->MspDeInitCallback == NULL)
    {
        hadc->MspDeInitCallback = HAL_ADC_MspDeInit;
    }

    hadc->MspDeInitCallback(hadc);

    /* Reset default callback function */
    _reset_callback(hadc);

#else
    HAL_ADC_MspDeInit(hadc);
#endif /* configUSE_ADC_REGISTER_CALLBACKS */

    /* Reset ADC state */
    _reset_base(hadc);

    /* Clear error code */
    CLEAR_ERROR_CODE(hadc);

    /* Release the process protection */
    GIVE_MUTEX(hadc);

    return HAL_OK;
}

/**
 * @brief  ADC Msp Init
 * @param  hadc Pointer to a ADC_Handle_T structure that contains the configuration information for the specified ADC.
 * @retval None
 */
__weak void HAL_ADC_MspInit(ADC_Handle_T *hadc)
{
    /* Check the ADC handle */
    ASSERT_PARAM(hadc);
    /* Check the ADC instance */
    ASSERT_PARAM(IS_ADC_INSTANCE(hadc));
    /* Prevent unused argument(s) compilation warning */
    UNUSED(hadc);

    /* NOTE : This function should not be modified, when the callback is needed,
              the HAL_ADC_MspInit could be implemented in the user file
     */
}

/**
 * @brief  ADC Msp DeInit callback
 * @param  hadc Pointer to a ADC_Handle_T structure that contains the configuration information for the specified ADC.
 * @retval None
 */
__weak void HAL_ADC_MspDeInit(ADC_Handle_T *hadc)
{
    /* Prevent unused argument(s) compilation warning */
    UNUSED(hadc);
    /* NOTE: This function should not be modified, when the callback is needed, the HAL_I2C_MspDeInit could be implemented in the user file */
}

/* Callbacks Register/UnRegister functions ************************************/
#if (configUSE_ADC_REGISTER_CALLBACKS == 1)
/**
 * @brief  Register a User ADC callback to be used instead of the weak predefined callback.
 * @param  hadc Pointer to a ADC_Handle_T structure that contains the configuration information for the specified ADC module.
 * @param  callback_id ID of the callback to be registered.
 * @param  callback pointer to the Callback function
 * @retval HAL status
 */
HAL_Status_T HAL_ADC_RegisterCallback(ADC_Handle_T *hadc, HAL_ADC_CallbackID_T callback_id, ADC_Callback_T callback)
{
    /* Check the ADC handle */
    ASSERT_PARAM(hadc);
    /* Check the ADC instance */
    ASSERT_PARAM(IS_ADC_INSTANCE(hadc));
    /* Protect the process */
    TAKE_MUTEX(hadc);
    HAL_Status_T status = HAL_OK;
    switch (callback_id)
    {
        case HAL_ADC_MSPINIT_CB_ID:
            hadc->MspInitCallback = callback;
            break;
        case HAL_ADC_MSPDEINIT_CB_ID:
            hadc->MspDeInitCallback = callback;
            break;
        case HAL_ADC_CONV_CPLT_CB_ID:
            hadc->ConvCpltCallback = callback;
            break;
        case HAL_ADC_STOP_CB_ID:
            hadc->StopCallback = callback;
            break;
        case HAL_ADC_DETECT_CB_ID:
            hadc->LevelOutOfWindowCallback = callback;
            break;
        case HAL_ADC_ERROR_CB_ID:
            hadc->ErrorCallback = callback;
            break;

        default:
            status = HAL_ERROR;
            break;
    }

    /* Release the process protection */
    GIVE_MUTEX(hadc);
    return status;
}

/**
 * @brief  Unregister an ADC callback, ADC callback is redirected to the weak predefined callback.
 * @param  hdma Pointer to a ADC_Handle_T structure that contains the configuration information for the specified ADC module.
 * @param  callback_id ID of the callback to be unregistered.
 *         This parameter can be one of the following values:
 *           @arg @ref HAL_ADC_TIMEOUT_CB_ID ADC Timeout Callback ID
 * @retval HAL status
 */
HAL_Status_T HAL_ADC_UnRegisterCallback(ADC_Handle_T *hadc, HAL_ADC_CallbackID_T callback_id)
{
    /* Check the ADC handle */
    ASSERT_PARAM(hadc);
    /* Check the ADC instance */
    ASSERT_PARAM(IS_ADC_INSTANCE(hadc));
    /* Protect the process */
    TAKE_MUTEX(hadc);
    HAL_Status_T status = HAL_OK;
    switch (callback_id)
    {
        case HAL_ADC_MSPINIT_CB_ID:
            hadc->MspInitCallback = HAL_ADC_MspInit;
            break;
        case HAL_ADC_MSPDEINIT_CB_ID:
            hadc->MspDeInitCallback = HAL_ADC_MspDeInit;
            break;
        case HAL_ADC_CONV_CPLT_CB_ID:
            hadc->ConvCpltCallback = HAL_ADC_ConvCpltCallback;
            break;
        case HAL_ADC_STOP_CB_ID:
            hadc->StopCallback = HAL_ADC_StopCallback;
            break;
        case HAL_ADC_DETECT_CB_ID:
            hadc->LevelOutOfWindowCallback = HAL_ADC_LevelOutOfWindowCallback;
            break;
        case HAL_ADC_ERROR_CB_ID:
            hadc->ErrorCallback = HAL_ADC_ErrorCallback;
            break;

        default:
            status = HAL_ERROR;
            break;
    }
    /* Release the process protection */
    GIVE_MUTEX(hadc);

    return status;
}

#endif /* configUSE_ADC_REGISTER_CALLBACKS */

/* I/O operation functions ******************************************************/
#if (configUSE_ADC_NONBLOCKING_IT == 1)
/**
 * @brief  Start conversion of the regular channels in interrupt mode .
 * @param  hadc Pointer to a ADC_Handle_T structure that contains the configuration information for the specified ADC module.
 * @retval HAL status
 */
HAL_Status_T HAL_ADC_Start_IT(ADC_Handle_T *hadc)
{
    /* Check the ADC handle */
    ASSERT_PARAM(hadc);
    /* Check the ADC instance */
    ASSERT_PARAM(IS_ADC_INSTANCE(hadc));

    /* Check ADC state */
    RET_FLAG_FALSE(HAL_FLAG_EQU(hadc->state, HAL_STATE_READY), HAL_ERROR);

    /* Check ADC is not in power_dowm */
    RET_FLAG_TRUE(HAL_FLAG_HAS(hadc->state_ext, ADC_EXT_STATE_CORE_POWER_DOWN), HAL_ERROR);

    /* When ADC reference option is not VDD/GND, AIN0 can not participate in conversion */
    RET_FLAG_TRUE(HAL_FLAG_EQU(_check_ref_option(hadc), 1), HAL_ERROR);

    /* Protect the process */
    TAKE_MUTEX(hadc);

    /* Enable ADC scan conversion finish interrupt and be set channel interrupt */
    _start_adc(hadc, HAL_ENABLE);

    /* Set ADC busy */
    HAL_FLAG_WRITE(hadc->state, HAL_STATE_BUSY);

    /* Release the process protection */
    GIVE_MUTEX(hadc);

    return HAL_OK;
}

/**
 * @brief  Stop conversion of the regular channels in interrupt mode .
 * @param  hadc Pointer to a ADC_Handle_T structure that contains the configuration information for the specified ADC module.
 * @retval HAL status
 */
HAL_Status_T HAL_ADC_Stop_IT(ADC_Handle_T *hadc)
{
    /* Check the ADC handle */
    ASSERT_PARAM(hadc);
    /* Check the ADC instance */
    ASSERT_PARAM(IS_ADC_INSTANCE(hadc));

    /* Protect the process */
    TAKE_MUTEX(hadc);

    _stop_adc(hadc);

    /* Set ADC ready */
    HAL_FLAG_WRITE(hadc->state, HAL_STATE_READY);

    /* Release the process protection */
    GIVE_MUTEX(hadc);

    return HAL_OK;
}

#endif /* configUSE_ADC_NONBLOCKING_IT */

#if (configUSE_ADC_NONBLOCKING_DMA == 1)
/**
 * @brief  Start conversion of the regular channels in DMA mode .
 * @param  hadc Pointer to a ADC_Handle_T structure that contains the configuration information for the specified ADC module.
 * @retval HAL status
 */
HAL_Status_T HAL_ADC_Start_DMA(ADC_Handle_T *hadc, uint32_t dma_channel, uint32_t *data, uint32_t length)
{
    /* Check the ADC handle */
    ASSERT_PARAM(hadc);
    /* Check the ADC instance */
    ASSERT_PARAM(IS_ADC_INSTANCE(hadc));
    /* Check the ADC DMA handle */
    ASSERT_PARAM((hadc->hdma[dma_channel]));

    /* Check the ADC state */
    RET_FLAG_TRUE(HAL_FLAG_EQU(hadc->state, HAL_STATE_RESET), HAL_ERROR);

    /* Check ADC is not in power_dowm */
    RET_FLAG_TRUE(HAL_FLAG_HAS(hadc->state_ext, ADC_EXT_STATE_CORE_POWER_DOWN), HAL_ERROR);

    /* Check the ADC is continue scan mode */
    RET_FLAG_FALSE(IS_ADC_LOOP_SCAN(hadc), HAL_ERROR);

    /* Check the ADC DMA channel state */
    RET_FLAG_LACKS(hadc->dma_channel_state, ADC_CHANNEL_STATE_READY << (dma_channel * 0x08), HAL_ERROR);

    /* When ADC reference option is not VDD/GND, AIN0 can not participate in conversion */
    RET_FLAG_TRUE(HAL_FLAG_EQU(_check_ref_option(hadc), 1), HAL_ERROR);

    /* Protect the process */
    TAKE_MUTEX(hadc);

    /* Enable ADC DMA */
    uint32_t *dam_ch_reg = (uint32_t *)((&hadc->instance->DMA0CTRL_b) + (dma_channel * 0x04));
    HAL_REG_SBIT(*dam_ch_reg, 1 << 0x10);

    /* Set value from ADC DMA FIFO to data */
    uint32_t    *fifo_addr = (uint32_t *)((hadc->instance->DMA0DAT + 0x04 * dma_channel));
    HAL_Status_T status    = HAL_DMA_Start_IT(hadc->hdma[dma_channel], (uint32_t)(((uint8_t *)(hadc->instance)) + _adc_dma_ch_offset_addr[dma_channel]), (uint32_t)data, length);
    if (status != HAL_OK)
    {
        return status;
    }

    /* Set the ADC DMA channel busy state */
    HAL_FLAG_SET(hadc->dma_channel_state, ADC_CHANNEL_STATE_BUSY << (dma_channel * 0x08));

    /* Enable ADC if it is disable */
    _start_adc(hadc, HAL_DISABLE);

    /* Release the process protection */
    GIVE_MUTEX(hadc);

    return HAL_OK;
}

/**
 * @brief  Stop conversion of the regular channels in DMA mode .
 * @param  hadc Pointer to a ADC_Handle_T structure that contains the configuration information for the specified ADC module.
 * @retval HAL status
 */
HAL_Status_T HAL_ADC_Stop_DMA(ADC_Handle_T *hadc, uint32_t dma_channel)
{
    /* Check the ADC handle */
    ASSERT_PARAM(hadc);
    /* Check the ADC instance */
    ASSERT_PARAM(IS_ADC_INSTANCE(hadc));
    /* Check the ADC DMA handle */
    ASSERT_PARAM((hadc->hdma[dma_channel]));

    /* Protect the process */
    TAKE_MUTEX(hadc);

    /* Disable ADC DMA */
    uint32_t *dam_ch_reg = (uint32_t *)((&hadc->instance->DMA0CTRL_b) + (dma_channel * 0x04));
    HAL_REG_CBIT(*dam_ch_reg, 1 << 0x10);

    /* Abort DMA */
    HAL_Status_T status = HAL_DMA_Abort_IT(hadc->hdma[dma_channel]);
    if (status != HAL_OK)
    {
        return status;
    }

    /* Clear the ADC DMA channel busy state */
    HAL_FLAG_CLR(hadc->dma_channel_state, ADC_CHANNEL_STATE_BUSY << (dma_channel * 0x08));

    /* Release the process protection */
    GIVE_MUTEX(hadc);

    return HAL_OK;
}

#endif /* configUSE_ADC_NONBLOCKING_DMA */

/**
 * @brief  Gets the converted channel value from data register of regular channel.
 * @param  hadc pointer to a ADC_Handle_T structure that contains
 *         the configuration information for the specified ADC.
 * @param  ch Selected ADC channel
 * @retval Converted value
 */
uint32_t HAL_ADC_GetValue(ADC_Handle_T *hadc, uint32_t ch)
{
    /* Return the selected ADC channel converted value */
    return (uint32_t)*((uint32_t *)hadc->instance + ch);
}

/* Peripheral Control functions *************************************************/
/**
 * @brief  ADC timing config .
 * @param  hadc Pointer to a ADC_Handle_T structure that contains the configuration information for the specified ADC module.
 * @param  timing Pointer to a ADC_Timing_T structure that contains the configuration information for the specified timing.
 * @retval HAL status
 */
HAL_Status_T HAL_ADC_ConfigTiming(ADC_Handle_T *hadc, ADC_Timing_T *timing)
{
    /* Check the ADC handle */
    ASSERT_PARAM(hadc);

    /* Check the ADC instance */
    ASSERT_PARAM(IS_ADC_INSTANCE(hadc));

    /* Check ADC power_on time */
    ASSERT_PARAM(IS_ADC_PWRON_TIME(timing));

    /* Check ADC channel delay time */
    ASSERT_PARAM(IS_ADC_CH_DELAY_TIME(timing));

    /* Check ADC channel sel time */
    ASSERT_PARAM(IS_ADC_CH_SEL_TIME(timing));

    /* Check ADC hold time */
    ASSERT_PARAM(IS_ADC_HOLD_TIME(timing));

    /* Check ADC sample time */
    ASSERT_PARAM(IS_ADC_SAMPL_TIME(timing));

    /* Check ADC sensing discharge time */
    ASSERT_PARAM(IS_ADC_DISCH_TIME(timing));

    /* Check ADCEN is 0 */
    RET_FLAG_FALSE(HAL_FLAG_EQU(hadc->instance->CTRL_b.ADCEN, 0), HAL_ERROR);

    /* Set ADC power enable cycle number of MCLK */
    HAL_REG_WRITE(hadc->instance->TPARM_b.PWRENTIME, timing->power_en_time);
    /* Set ADC delay time cycle number of MCLK for each channel sampling */
    HAL_REG_WRITE(hadc->instance->SMPR_b.CHDLYTIME, timing->ch_delay_time);
    /* Set ADC analog input channel change discharge cycle number of MCLK */
    HAL_REG_WRITE(hadc->instance->SMPR_b.CHSELTIME, timing->ch_sel_time);
    /* Set ADC analog input channel hold cycle number of MCLK */
    HAL_REG_WRITE(hadc->instance->SMPR_b.HOLDTIME, timing->hold_time);
    /* Set ADC analog input channel sample clock high level cycle number of MCLK */
    HAL_REG_WRITE(hadc->instance->SMPR_b.SAMPLTIME, timing->sampl_time);
    /* Set ADC analog input channel settling cycle number of MCLK */
    HAL_REG_WRITE(hadc->instance->SMPR_b.SETTLTIME, timing->settl_time);
    /* Set ADC sensing discharge cycle number of MCLK */
    HAL_REG_WRITE(hadc->instance->DISCHTIME_b.DISCHTIME, timing->disch_time);
    return HAL_OK;
}
/**
 * @brief  ADC channel config .
 * @param  hadc Pointer to a ADC_Handle_T structure that contains the configuration information for the specified ADC module.
 * @param  ch Pointer to a ADC_Channel_T structure that contains the configuration information for the specified channel.
 * @retval HAL status
 */
HAL_Status_T HAL_ADC_ConfigChannel(ADC_Handle_T *hadc, ADC_Channel_T *ch)
{
    /* Check the ADC handle */
    ASSERT_PARAM(hadc);
    /* Check the ADC instance */
    ASSERT_PARAM(IS_ADC_INSTANCE(hadc));
    /* Check the channel config */
    ASSERT_PARAM(IS_CHANNEL_CONFIG(ch));

    /* Check channel rank when single step mode */
    RET_FLAG_FALSE(CHECK_CHANNEL_RANK(hadc, ch), HAL_ERROR);

    /* Protect the process */
    TAKE_MUTEX(hadc);

    /* Set channel sequence */
    if (HAL_FLAG_EQU(hadc->init.conv_mode, ADC_CONVMODE_SINGLE_STEP))
    {
        HAL_REG_WRITE(hadc->instance->SQR0_b.SQC1, ch->channel);
    }
    else
    {
        _set_channel_sequence(hadc, ch);
    }

    /* Release the process protection */
    GIVE_MUTEX(hadc);

    return HAL_OK;
}

/**
 * @brief  ADC analog  watchdog config .
 * @param  hadc Pointer to a ADC_Handle_T structure that contains the configuration information for the specified ADC module.
 * @param  analog_wdg Pointer to a ADC_AnalogWDG_T structure that contains the configuration information for the specified abalog watchdog.
 * @retval HAL status
 */
HAL_Status_T HAL_ADC_ConfigAnalogWDG(ADC_Handle_T *hadc, ADC_AnalogWDG_T *analog_wdg)
{
    /* Check the ADC handle */
    ASSERT_PARAM(hadc);
    /* Check the ADC instance */
    ASSERT_PARAM(IS_ADC_INSTANCE(hadc));
    /* Check the analog watchdog config */
    ASSERT_PARAM(IS_ANALOG_WDG(analog_wdg));

    /* Protect the process */
    TAKE_MUTEX(hadc);

    uint32_t *th_detect_reg = (uint32_t *)((&hadc->instance->THRHOLD0) + (analog_wdg->channel * 2));

    uint32_t *th_detect_ie_reg = (uint32_t *)((&hadc->instance->THRDIE0_b) + (analog_wdg->channel / 16));
    if (HAL_FLAG_EQU(analog_wdg->enable, HAL_ENABLE))
    {
        if (analog_wdg->watchdog_mode & ADC_DETECT_LTR)
        {
            /* Enable the lower threshold detection function and set threshold config */
            uint32_t th_de_value = (analog_wdg->lower_threshold) | (analog_wdg->channel << 16);
            HAL_REG_WRITE(*th_detect_reg, th_de_value);

            /* Enable threshold interrupt */
            if (analog_wdg->channel < ADC_CHANNEL_VDD)
            {
                HAL_REG_SBIT(*th_detect_ie_reg, 1 << (analog_wdg->channel * 2));
            }
            else
            {
                HAL_REG_SBIT(*(th_detect_ie_reg + 1), 1 << ((analog_wdg->channel - ADC_CHANNEL_VDD) * 2));
            }
        }
        if (analog_wdg->watchdog_mode & ADC_DETECT_HTR)
        {
            /* Enable the higher threshold detection function and set threshold config */
            uint32_t th_de_value = (analog_wdg->higher_threshold) | (analog_wdg->channel << 16) | 1U << 31;
            HAL_REG_WRITE(*(th_detect_reg + 1), th_de_value);

            /* Enable threshold interrupt */
            if (analog_wdg->channel < ADC_CHANNEL_VDD)
            {
                HAL_REG_SBIT(*th_detect_ie_reg, 1 << (analog_wdg->channel * 2 + 1));
            }
            else
            {
                HAL_REG_SBIT(*(th_detect_ie_reg + 1), 1 << ((analog_wdg->channel - ADC_CHANNEL_VDD) * 2 + 1));
            }
        }
    }
    else
    {
        uint32_t *th_detect_reg = (uint32_t *)((&hadc->instance->THRHOLD0) + (analog_wdg->channel * 2));
        if (analog_wdg->watchdog_mode & ADC_DETECT_LTR)
        {
            /* Disable the lower threshold detection function */
            HAL_REG_CBIT(*th_detect_reg, 0xFFF);
            /* Disable threshold interrupt */
            if (analog_wdg->channel < ADC_CHANNEL_VDD)
            {
                HAL_REG_CBIT(*th_detect_ie_reg, 1 << (analog_wdg->channel * 2));
            }
            else
            {
                HAL_REG_CBIT(*(th_detect_ie_reg + 1), 1 << ((analog_wdg->channel - ADC_CHANNEL_VDD) * 2));
            }
        }
        if (analog_wdg->watchdog_mode & ADC_DETECT_HTR)
        {
            /* Disable the higher threshold detection function */
            HAL_REG_SBIT(*(th_detect_reg + 1), 0xFFF);
            /* Disable threshold interrupt */
            if (analog_wdg->channel < ADC_CHANNEL_VDD)
            {
                HAL_REG_CBIT(*th_detect_ie_reg, 1 << (analog_wdg->channel * 2 + 1));
            }
            else
            {
                HAL_REG_CBIT(*(th_detect_ie_reg + 1), 1 << ((analog_wdg->channel - ADC_CHANNEL_VDD) * 2 + 1));
            }
        }
    }

    /* Release the process protection */
    GIVE_MUTEX(hadc);
    return HAL_OK;
}

/**
 * @brief  ADC reference option select .
 * @param  hadc Pointer to a ADC_Handle_T structure that contains the configuration information for the specified ADC module.
 * @param  ref_option Reference option select.
 *                    This parameter can be a value of @ref adc_ref_option
 * @retval HAL status
 */
HAL_Status_T HAL_ADC_SelectRefOption(ADC_Handle_T *hadc, uint32_t ref_option)
{
    /* Check the ADC handle */
    ASSERT_PARAM(hadc);
    /* Check the ADC instance */
    ASSERT_PARAM(IS_ADC_INSTANCE(hadc));
    /* Check the reference option select */
    ASSERT_PARAM(IS_REF_OPTION(ref_option));

    /* Protect the process */
    TAKE_MUTEX(hadc);

    if (ref_option & 0x2)
    {
        HAL_REG_WRITE(hadc->instance->MISC_b.XPD_LDO, 0);
    }
    else
    {
        HAL_REG_WRITE(hadc->instance->MISC_b.XPD_LDO, 1);
    }

    /* Select reference option */
    HAL_REG_WRITE(hadc->instance->MISC_b.XREFSEL, (ref_option & 0x3));
    HAL_REG_WRITE(hadc->instance->MISC_b.XSEL_LDO, (ref_option & 0xC) >> 2);

    /* ADC offset calibration */
    _offset_calibration(hadc);

    /* Release the process protection */
    GIVE_MUTEX(hadc);

    return HAL_OK;
}

/**
 * @brief  ADC wakeup time config .
 * @param  hadc Pointer to a ADC_Handle_T structure that contains the configuration information for the specified ADC module.
 * @param  wakeup_time Wakeup time.
 *                     This parameter can be a value of @ref adc_wakeup_cycles
 * @retval HAL status
 */
HAL_Status_T HAL_ADC_ConfigWakeup(ADC_Handle_T *hadc, uint32_t wakeup_time)
{
    /* Check the ADC handle */
    ASSERT_PARAM(hadc);
    /* Check the ADC instance */
    ASSERT_PARAM(IS_ADC_INSTANCE(hadc));
    /* Check the wakeup time */
    ASSERT_PARAM(IS_WAKEUP_TIME(wakeup_time));

    /* Protect the process */
    TAKE_MUTEX(hadc);

    /* Set wakeup time */
    HAL_REG_WRITE(hadc->instance->TPARM_b.WKUPTIME, wakeup_time);

    /* Release the process protection */
    GIVE_MUTEX(hadc);

    return HAL_OK;
}

#if (configUSE_ADC_NONBLOCKING_DMA == 1)
/**
 * @brief  ADC DMA channel config .
 * @param  hadc Pointer to a ADC_Handle_T structure that contains the configuration information for the specified ADC module.
 * @param  wakeup_time Wakeup time.
 *                     This parameter can be a value of @ref adc_wakeup_cycles
 * @retval HAL status
 */
HAL_Status_T HAL_ADC_ConfigDMA_Channel(ADC_Handle_T *hadc, ADC_DMA_Channel_T *dma_channel)
{
    /* Check the ADC handle */
    ASSERT_PARAM(hadc);
    /* Check the ADC instance */
    ASSERT_PARAM(IS_ADC_INSTANCE(hadc));
    /* Check the ADC DMA channel config */
    ASSERT_PARAM(IS_DMA_CHANNEL(dma_channel));

    uint32_t dma_ch = dma_channel->dma_channel;

    /* Check the ADC DMA channel state is not busy */
    RET_FLAG_HAS(hadc->dma_channel_state, ADC_CHANNEL_STATE_BUSY << (dma_ch * 8), HAL_BUSY);

    /* Set ADC DMA channel config */
    uint32_t *dam_ch_reg   = (uint32_t *)((&hadc->instance->DMA0CTRL_b) + (dma_channel->dma_channel * 0x04));
    uint32_t  dma_ch_value = dma_channel->injected_ch | (dma_channel->fifo_threshold << 8) | (dma_channel->injected_mode << 24);
    HAL_REG_WRITE(*dam_ch_reg, dma_ch_value);

    /* Set the ADC DMA channel ready state */
    HAL_FLAG_SET(hadc->dma_channel_state, ADC_CHANNEL_STATE_READY << (dma_ch * 8));

    return HAL_OK;
}
#endif /* configUSE_ADC_NONBLOCKING_DMA */

/* Power-on and power-down function *************************************************/
/**
 * @brief  ADC Wakeup.The function can not be used when ADC is auto power down mode.
 * @param  hadc Pointer to a ADC_Handle_T structure that contains the configuration information for the specified ADC module.
 * @retval HAL status
 */
HAL_Status_T HAL_ADC_Wakeup(ADC_Handle_T *hadc)
{
    /* Check the ADC handle */
    ASSERT_PARAM(hadc);
    /* Check the ADC instance */
    ASSERT_PARAM(IS_ADC_INSTANCE(hadc));

    /* Check the ADC is not auto power down mode */
    RET_FLAG_FALSE(HAL_FLAG_EQU(hadc->init.autopwdn, HAL_DISABLE), HAL_ERROR);

    /* Protect the process */
    TAKE_MUTEX(hadc);

    /* Set ADC core to wake up mode */
    HAL_REG_WRITE(hadc->instance->MISC_b.XPD_ADC, 0);
    HAL_REG_WRITE(hadc->instance->MISC_b.XPD_ADCSEL, 0);
    /* Set ADC wake up state */
    HAL_FLAG_CLR(hadc->state_ext, ADC_EXT_STATE_CORE_POWER_DOWN);

    /* Release the process protection */
    GIVE_MUTEX(hadc);

    return HAL_OK;
}

/**
 * @brief  ADC Power dowm.The function can not be used when ADC is auto power down mode.
 * @param  hadc Pointer to a ADC_Handle_T structure that contains the configuration information for the specified ADC module.
 * @retval HAL status
 */
HAL_Status_T HAL_ADC_PowerDown(ADC_Handle_T *hadc)
{
    /* Check the ADC handle */
    ASSERT_PARAM(hadc);
    /* Check the ADC instance */
    ASSERT_PARAM(IS_ADC_INSTANCE(hadc));

    /* Check the ADC is not running and in continue scan mode */
    RET_FLAG_TRUE(HAL_FLAG_EQU(hadc->init.conv_mode, ADC_CONVMODE_LOOP_SCAN) && HAL_FLAG_EQU(hadc->instance->CTRL_b.ADCEN, HAL_ENABLE), HAL_ERROR);

    /* Check the ADC is not auto power down mode */
    RET_FLAG_FALSE(HAL_FLAG_EQU(hadc->init.autopwdn, HAL_DISABLE), HAL_ERROR);

    /* Protect the process */
    TAKE_MUTEX(hadc);

    /* Set ADC core to power-down mode */
    HAL_REG_WRITE(hadc->instance->MISC_b.XPD_ADCSEL, 1);
    HAL_REG_WRITE(hadc->instance->MISC_b.XPD_ADC, 1);
    /* Set ADC power-down state */
    HAL_FLAG_SET(hadc->state_ext, ADC_EXT_STATE_CORE_POWER_DOWN);

    /* Release the process protection */
    GIVE_MUTEX(hadc);

    return HAL_OK;
}

/* Peripheral State functions ***************************************************/
/**
 * @brief  Return the ADC state.
 * @param  hadc Pointer to a ADC_Handle_T structure that contains the configuration information for the specified ADC.
 * @retval HAL_State_T
 */
HAL_State_T HAL_ADC_GetState(ADC_Handle_T *hadc)
{
    /* Check the ADC handle */
    ASSERT_PARAM(hadc);
    return hadc->state;
}

/**
 * @brief  Return ADC error code.
 * @param  hadc Pointer to a ADC_Handle_T structure that contains the configuration information for the specified ADC.
 * @retval HAL_State_T
 */
uint32_t HAL_ADC_GetError(ADC_Handle_T *hadc)
{
    /* Check the ADC handle */
    ASSERT_PARAM(hadc);
    return hadc->error_code;
}

/**
 * @brief  Return ADC channel trigger interrupt when end of conversion or the result of conversion beyond threshold.
 * @param  hadc Pointer to a ADC_Handle_T structure that contains the configuration information for the specified ADC.
 * @retval HAL_State_T
 */
uint32_t HAL_ADC_GetActiveChannel(ADC_Handle_T *hadc)
{
    /* Check the ADC handle */
    ASSERT_PARAM(hadc);
    return hadc->active_channel;
}

/* IRQ Handler functions ******************************************************/
__weak ATTR_ISR void HAL_ADC_IRQHandler(ADC_Handle_T *hadc)
{
    /* Check the ADC handle */
    ASSERT_PARAM(hadc);
    /* Check the ADC instance */
    ASSERT_PARAM(IS_ADC_INSTANCE(hadc));

    uint32_t eocis = HAL_REG_READ(hadc->instance->EOCIS);
    uint32_t ris   = HAL_REG_READ(hadc->instance->RIS);

#if (configUSE_ADC_REGISTER_CALLBACKS == 1)
    /* ADC channel single channel conversion complete interrupt */
    if (eocis != 0)
    {
        uint8_t channel = POSITION_VAL(eocis);

        /* Clear ADC channel conversion complete flag */
        HAL_REG_WRITE(hadc->instance->EOCIS, 1 << channel);
    }

    if (ris & (1 << 0))
    {
        /* Clear ADC conversion complete flag */
        HAL_REG_WRITE(hadc->instance->RIS, 1 << 0);

        if (hadc->init.conv_mode != ADC_CONVMODE_LOOP_SCAN)
        {
            /* Set ADC ready */
            HAL_FLAG_WRITE(hadc->state, HAL_STATE_READY);
            if (HAL_FLAG_EQU(hadc->init.autopwdn, HAL_ENABLE))
            {
                HAL_FLAG_SET(hadc->state_ext, ADC_EXT_STATE_AUTO_POWER_DOWN);
            }
        }

        /* Call ADC conversion complete Callback */
        hadc->ConvCpltCallback(hadc);
    }
    if (ris & (1 << 1))
    {
        /* Clear ADC conversion stop flag */
        HAL_REG_WRITE(hadc->instance->RIS, 1 << 1);

        /* Set ADC ready */
        HAL_FLAG_WRITE(hadc->state, HAL_STATE_READY);

        /* Call ADC conversion stop Callback */
        hadc->StopCallback(hadc);
    }

    /* ADC threshold interrupt */
    if (HAL_REG_READ(hadc->instance->THRDIS0) != 0)
    {
        uint8_t th_index = POSITION_VAL(HAL_REG_READ(hadc->instance->THRDIS0));
        /* Clear threshold flag */
        HAL_REG_WRITE(hadc->instance->THRDIS0, 1 << th_index);

        hadc->active_channel = th_index / 2;

        /* Call ADC channel single channel conversion Callback */
        hadc->LevelOutOfWindowCallback(hadc);
    }
    else if (HAL_REG_READ(hadc->instance->THRDIS1) != 0)
    {
        uint8_t th_index = POSITION_VAL(HAL_REG_READ(hadc->instance->THRDIS1));

        /* Clear threshold flag */
        HAL_REG_WRITE(hadc->instance->THRDIS1, 1 << th_index);

        hadc->active_channel = (th_index / 2) + 16;

        /* Call ADC channel single channel conversion Callback */
        hadc->LevelOutOfWindowCallback(hadc);
    }
#else
    if (HAL_REG_READ(hadc->instance->EOCIS) != 0)
    {
        uint32_t channel = POSITION_VAL(HAL_REG_READ(hadc->instance->EOCIS));

        /* Clear ADC channel conversion complete flag */
        HAL_REG_WRITE(hadc->instance->EOCIS, 1 << channel);
    }
    if (HAL_REG_READ(hadc->instance->RIS_b.ADC_DONEIF))
    {
        /* Clear ADC conversion complete flag */
        HAL_REG_WRITE(hadc->instance->RIS, 1 << 0);

        if (hadc->init.conv_mode != ADC_CONVMODE_LOOP_SCAN)
        {
            /* Set ADC ready */
            HAL_FLAG_WRITE(hadc->state, HAL_STATE_READY);
        }

        /* Call ADC conversion complete Callback */
        HAL_ADC_ConvCpltCallback(hadc);
    }
    if (HAL_REG_READ(hadc->instance->RIS_b.ADC_STOPIF))
    {
        /* Clear ADC conversion stop flag */
        HAL_REG_WRITE(hadc->instance->RIS, 1 << 1);

        /* Set ADC ready */
        HAL_FLAG_WRITE(hadc->state, HAL_STATE_READY);

        /* Call ADC conversion stop Callback */
        HAL_ADC_StopCallback(hadc);
    }

    /* ADC threshold interrupt */
    if (HAL_REG_READ(hadc->instance->THRDIS0) != 0)
    {
        uint8_t th_index = POSITION_VAL(HAL_REG_READ(hadc->instance->THRDIS0));
        /* Clear threshold flag */
        HAL_REG_WRITE(hadc->instance->THRDIS0, 1 << th_index);

        hadc->active_channel = th_index / 2;

        /* Call ADC channel single channel conversion Callback */
        HAL_ADC_LevelOutOfWindowCallback(hadc);
    }
    else if (HAL_REG_READ(hadc->instance->THRDIS1) != 0)
    {
        uint8_t th_index = POSITION_VAL(HAL_REG_READ(hadc->instance->THRDIS1));
        /* Clear threshold flag */
        HAL_REG_WRITE(hadc->instance->THRDIS1, 1 << th_index);

        hadc->active_channel = (th_index / 2) + 16;

        /* Call ADC channel single channel conversion Callback */
        HAL_ADC_LevelOutOfWindowCallback(hadc);
    }
#endif
}

/* Event Callback functions ***************************************************/
/**
 * @brief  ADC converter done callback
 * @param  hadc Pointer to a ADC_Handle_T structure that contains the configuration information for the specified ADC.
 * @retval None
 */
__weak void HAL_ADC_ConvCpltCallback(ADC_Handle_T *hadc)
{
    /* Check the ADC handle */
    ASSERT_PARAM(hadc);
    /* Check the ADC instance */
    ASSERT_PARAM(IS_ADC_INSTANCE(hadc));
    /* prevent unused argument(s) compilation warning */
    UNUSED(hadc);
    /* NOTE: This function should not be modified, when the callback is needed,
         the HAL_ADC_ConvCpltCallback could be implemented in the user file */
}

/**
 * @brief  ADC converter stop callback
 * @param  hadc Pointer to a ADC_Handle_T structure that contains the configuration information for the specified ADC.
 * @retval None
 */
__weak void HAL_ADC_StopCallback(ADC_Handle_T *hadc)
{
    /* Check the ADC handle */
    ASSERT_PARAM(hadc);
    /* Check the ADC instance */
    ASSERT_PARAM(IS_ADC_INSTANCE(hadc));
    /* prevent unused argument(s) compilation warning */
    UNUSED(hadc);
    /* NOTE: This function should not be modified, when the callback is needed,
         the HAL_ADC_StopCallback could be implemented in the user file */
}

/**
 * @brief  ADC analog detect interrupt
 * @param  hadc Pointer to a ADC_Handle_T structure that contains the configuration information for the specified ADC.
 * @retval None
 */
__weak void HAL_ADC_LevelOutOfWindowCallback(ADC_Handle_T *hadc)
{
    /* Check the ADC handle */
    ASSERT_PARAM(hadc);
    /* Check the ADC instance */
    ASSERT_PARAM(IS_ADC_INSTANCE(hadc));
    /* prevent unused argument(s) compilation warning */
    UNUSED(hadc);
    /* NOTE: This function should not be modified, when the callback is needed,
         the HAL_ADC_LevelOutOfWindowCallback could be implemented in the user file */
}

/**
 * @brief  ADC error callback
 * @param  hadc Pointer to a ADC_Handle_T structure that contains the configuration information for the specified ADC.
 * @retval None
 */
__weak void HAL_ADC_ErrorCallback(ADC_Handle_T *hadc)
{
    /* Check the ADC handle */
    ASSERT_PARAM(hadc);
    /* Check the ADC instance */
    ASSERT_PARAM(IS_ADC_INSTANCE(hadc));
    /* prevent unused argument(s) compilation warning */
    UNUSED(hadc);
    /* NOTE: This function should not be modified, when the callback is needed,
         the HAL_ADC_ErrorCallback could be implemented in the user file */
}

/**
 * @brief  Reset ADC state.
 * @param  hadc Pointer to a ADC_Handle_T structure that contains the configuration information for the specified ADC.
 * @retval None
 */
static void _reset_base(ADC_Handle_T *hadc)
{
    /* Reset ext state */
    hadc->state_ext = ADC_EXT_STATE_WAKE_UP;
#if (configUSE_ADC_NONBLOCKING_DMA == 1)
    /* Reset state */
    HAL_FLAG_WRITE(hadc->dma_channel_state, ADC_CHANNEL_STATE_RESET);
#endif
    /* Reset state */
    HAL_FLAG_WRITE(hadc->state, HAL_STATE_RESET);
}

/**
 * @brief  Enable ADC
 * @param  hadc Pointer to a ADC_Handle_T structure that contains the configuration information for the specified ADC.
 * @retval None
 */
static void _enable_adc(ADC_Handle_T *hadc)
{
    /* If ADCEN == 0, set ADCEN to 1 then delay for ADC warm-up time */
    if (HAL_FLAG_EQU(HAL_REG_READ(hadc->instance->CTRL_b.ADCEN), 0))
    {
        /* Enable ADC */
        HAL_REG_WRITE(hadc->instance->CTRL_b.ADCEN, HAL_ENABLE);
    }
}

/**
 * @brief  start ADC and set adc interrupt
 * @param  hadc Pointer to a ADC_Handle_T structure that contains the configuration information for the specified ADC.
 * @param  ie ADC interrupt flag
 * @retval None
 */
static void _start_adc(ADC_Handle_T *hadc, uint8_t ie)
{
    /* Enable ADC scan conversion finish interrupt */
    HAL_REG_WRITE(hadc->instance->IE_b.ADC_DONEIE, ie);

    for (uint8_t i = 0; i < (hadc->init.scan_ch_num); i++)
    {
        uint8_t channel = _get_channel_form_sqc(hadc, i + 1);
        /* Enable SQCn channel end of conversion interrupt */
        HAL_REG_SBIT(hadc->instance->EOCIE, ie << channel);
    }

    /* Enable ADC */
    _enable_adc(hadc);

    /* Start ADC conversion */
    HAL_REG_WRITE(hadc->instance->CTRL_b.SWSTART, HAL_ENABLE);
}

#if (configUSE_ADC_NONBLOCKING_IT == 1)
/**
 * @brief  stop ADC , disable ADC conversion interrupt and enable ADC stop interrupt flag
 * @param  hadc Pointer to a ADC_Handle_T structure that contains the configuration information for the specified ADC.
 * @retval None
 */
static void _stop_adc(ADC_Handle_T *hadc)
{
    /* Disable ADC scan conversion finish interrupt */
    HAL_REG_WRITE(hadc->instance->IE_b.ADC_DONEIE, 0);

    /* Enable ADC scan conversion stop interrupt */
    HAL_REG_WRITE(hadc->instance->IE_b.ADC_STOPIE, 1);

    /* Disable ADC channel end of conversion interrupt */
    HAL_REG_WRITE(hadc->instance->EOCIE, 0);

    /* Clear ADC channel interrupt */
    HAL_REG_WRITE(hadc->instance->EOCIS, 0x3FFFF);

    /* Stop conversion */
    HAL_REG_WRITE(hadc->instance->CTRL_b.SWSTART, HAL_DISABLE);

    /* Disable ADC */
    HAL_REG_WRITE(hadc->instance->CTRL_b.ADCEN, HAL_DISABLE);
}
#endif /* configUSE_ADC_NONBLOCKING_IT */

/**
 * @brief  Get channel from scan sequence
 * @param  hadc Pointer to a ADC_Handle_T structure that contains the configuration information for the specified ADC.
 * @param  sequence scan sequence
 * @retval None
 */
static uint8_t _get_channel_form_sqc(ADC_Handle_T *hadc, uint8_t sequence)
{
    uint8_t channel = 0;

    uint8_t seq_reg_index = (sequence - 1) / 4;
    uint8_t seq_reg_bit   = 8 * (((sequence - 1) % 4));
    switch (seq_reg_index)
    {
        case 0:
            channel = (HAL_REG_RBIT(hadc->instance->SQR0, (0x1F << (seq_reg_bit))) >> seq_reg_bit);
            break;
        case 1:
            channel = (HAL_REG_RBIT(hadc->instance->SQR1, (0x1F << (seq_reg_bit))) >> seq_reg_bit);
            break;
        case 2:
            channel = (HAL_REG_RBIT(hadc->instance->SQR2, (0x1F << (seq_reg_bit))) >> seq_reg_bit);
            break;
        case 3:
            channel = (HAL_REG_RBIT(hadc->instance->SQR3, (0x1F << (seq_reg_bit))) >> seq_reg_bit);
            break;
        case 4:
            channel = (HAL_REG_RBIT(hadc->instance->SQR4, (0x1F << (seq_reg_bit))) >> seq_reg_bit);
            break;

        default:
            break;
    }
    return channel;
}

/**
 * @brief  Set channel sequence to scan
 * @param  hadc Pointer to a ADC_Handle_T structure that contains the configuration information for the specified ADC.
 * @retval None
 */
static void _set_channel_sequence(ADC_Handle_T *hadc, ADC_Channel_T *ch)
{
    switch (((ch->rank - 1) / 4))
    {
        case 0:
            HAL_REG_CBIT(hadc->instance->SQR0, 0x1F << (8 * ((ch->rank - 1) % 4)));
            HAL_REG_SBIT(hadc->instance->SQR0, ch->channel << (8 * ((ch->rank - 1) % 4)));
            break;
        case 1:
            HAL_REG_CBIT(hadc->instance->SQR1, 0x1F << (8 * ((ch->rank - 1) % 4)));
            HAL_REG_SBIT(hadc->instance->SQR1, ch->channel << (8 * ((ch->rank - 1) % 4)));
            break;
        case 2:
            HAL_REG_CBIT(hadc->instance->SQR2, 0x1F << (8 * ((ch->rank - 1) % 4)));
            HAL_REG_SBIT(hadc->instance->SQR2, ch->channel << (8 * ((ch->rank - 1) % 4)));
            break;
        case 3:
            HAL_REG_CBIT(hadc->instance->SQR3, 0x1F << (8 * ((ch->rank - 1) % 4)));
            HAL_REG_SBIT(hadc->instance->SQR3, ch->channel << (8 * ((ch->rank - 1) % 4)));
            break;
        case 4:
            HAL_REG_CBIT(hadc->instance->SQR4, 0x1F << (8 * ((ch->rank - 1) % 4)));
            HAL_REG_SBIT(hadc->instance->SQR4, ch->channel << (8 * ((ch->rank - 1) % 4)));
            break;
        default:
            break;
    }
}

/**
 * @brief  ADC offset calibration
 * @param  hadc Pointer to a ADC_Handle_T structure that contains the configuration information for the specified ADC.
 * @retval None
 * @note Enable calibration when \ref HAL_ADC_Init or \ref HAL_ADC_SelectRefOption
 */
static void _offset_calibration(ADC_Handle_T *hadc)
{
    uint16_t data_total    = 0;
    uint16_t data_avg      = 0;
    uint16_t first_conv_ch = HAL_REG_READ(hadc->instance->SQR0_b.SQC1);
    /* Set ADC single step conversion mode */
    uint32_t conv_mode = HAL_REG_READ(hadc->instance->CTRL_b.COVNMODE);
    HAL_REG_WRITE(hadc->instance->CTRL_b.COVNMODE, ADC_CONVMODE_SINGLE_STEP);

    uint8_t scan_num = hadc->init.scan_ch_num - 1;
    HAL_REG_WRITE(hadc->instance->CTRL_b.SCANNUM, 0);

    /* Enable calibration */
    HAL_REG_WRITE(hadc->instance->MISC_b.XCAL, HAL_ENABLE);
    /* Update XBVOS when enable calibration */
    HAL_REG_WRITE(hadc->instance->MISC_b.XBVOS, 0);
    /* Enable ADC */
    _enable_adc(hadc);
    /* Save 10 timers data from CH0 conversion */
    for (size_t i = 0; i < ADC_CALIBRATION_TIMES; i++)
    {
        /* Start ADC conversion */
        HAL_REG_WRITE(hadc->instance->CTRL_b.SWSTART, HAL_ENABLE);
        /* Wait The 1st conversion channel finish */
        while (HAL_REG_RBIT(hadc->instance->EOCIS, (1 << first_conv_ch)) == 0)
            ;
        /* Clear channel flag */
        HAL_REG_WRITE(hadc->instance->EOCIS, (1 << first_conv_ch));
        data_total += HAL_ADC_GetValue(hadc, first_conv_ch);
    }
    /* Disable calibration */
    HAL_REG_WRITE(hadc->instance->MISC_b.XCAL, HAL_DISABLE);

    data_avg = data_total / ADC_CALIBRATION_TIMES;
    if (data_avg > 127)
    {
        /* Set calibration value */
        data_avg = 127;
    }
    HAL_REG_WRITE(hadc->instance->MISC_b.XBVOS, data_avg);
    /* Rollback ADC conversion mode */
    HAL_REG_WRITE(hadc->instance->CTRL_b.COVNMODE, conv_mode);
    HAL_REG_WRITE(hadc->instance->CTRL_b.SCANNUM, scan_num);
    /* Disable ADC */
    HAL_REG_WRITE(hadc->instance->CTRL_b.ADCEN, HAL_DISABLE);
    /* Clear ADC conversion done and stop flag */
    HAL_REG_WRITE(hadc->instance->RIS, 0x3 << 0);
}

/**
 * @brief  when ADC reference option is not VDD/GND , AIN0 can not participate in conversion
 * @param  hadc Pointer to a ADC_Handle_T structure that contains the configuration information for the specified ADC.
 * @retval 0: when ADC in ADC reference option is not VDD/GND, AIN0 not participate in conversion
 *         1: when ADC in ADC reference option is not VDD/GND, AIN0 participate in conversion
 */
uint8_t _check_ref_option(ADC_Handle_T *hadc)
{
    if (HAL_FLAG_NEQ(hadc->instance->MISC_b.XREFSEL, 0x1))
    {
        for (size_t i = 0; i < hadc->init.scan_ch_num; i++)
        {
            if (HAL_FLAG_EQU(_get_channel_form_sqc(hadc, i + 1), ADC_CHANNEL_0))
            {
                return 1;
            }
        }
    }
    return 0;
}

#if (configUSE_ADC_REGISTER_CALLBACKS == 1)
/**
 * @brief  Reset ADC all callback
 * @param  hadc Pointer to a ADC_Handle_T structure that contains the configuration information for the specified ADC module.
 * @retval None
 */
static void _reset_callback(ADC_Handle_T *hadc)
{
    hadc->MspInitCallback          = HAL_ADC_MspInit;
    hadc->MspDeInitCallback        = HAL_ADC_MspDeInit;
    hadc->ConvCpltCallback         = HAL_ADC_ConvCpltCallback;
    hadc->StopCallback             = HAL_ADC_StopCallback;
    hadc->LevelOutOfWindowCallback = HAL_ADC_LevelOutOfWindowCallback;
    hadc->ErrorCallback            = HAL_ADC_ErrorCallback;
}
#endif /* configUSE_ADC_REGISTER_CALLBACKS */

#if (configUSE_ADC_NONBLOCKING_DMA == 1)
/**
 * @brief  stop ADC in DMA mode
 * @param  hadc Pointer to a ADC_Handle_T structure that contains the configuration information for the specified ADC.
 * @retval None
 */
static void _stop_adc_dma(ADC_Handle_T *hadc)
{
    /* Stop conversion */
    HAL_REG_WRITE(hadc->instance->CTRL_b.SWSTART, HAL_DISABLE);

    /* Disable ADC */
    HAL_REG_WRITE(hadc->instance->CTRL_b.ADCEN, HAL_DISABLE);
}

/**
 * @brief  Get DMA channel from ADC
 * @param  hadc Pointer to a ADC_Handle_T structure that contains the configuration information for the specified ADC.
 * @param  hdma Pointer to a DMA_Handle_T structure that contains the configuration information for the specified DMA.
 * @retval DMA channel from ADC
 */
static int32_t get_adc_dma_ch(ADC_Handle_T *hadc, DMA_Handle_T *hdma)
{
    int32_t ch = ADC_DMA_CHANNEL_0;
    for (; ch <= ADC_DMA_CHANNEL_3; ch++)
    {
        if (hdma == hadc->hdma[ch])
        {
            return ch;
        }
    }
    return -1;
}

/**
 * @brief  DMA transfer completed callback
 * @param  hdma Pointer to a DMA_Handle_T structure that contains the configuration information for the specified DMA.
 * @retval void
 */
static void _adc_dma_xfer_done_callback(DMA_Handle_T *hdma)
{
    ADC_Handle_T *hadc   = (ADC_Handle_T *)hdma->parent;
    int32_t       dma_ch = get_adc_dma_ch(hadc, hdma);
    if (dma_ch != -1)
    {
        /* Disable ADC DMA */
        uint32_t *dam_ch_reg = (uint32_t *)((&hadc->instance->DMA0CTRL_b) + (dma_ch * 0x04));
        HAL_REG_CBIT(*dam_ch_reg, 1 << 0x10);
    }
#if (configUSE_ADC_REGISTER_CALLBACKS == 1)
    if (hadc->ConvCpltCallback != NULL)
    {
        (hadc->ConvCpltCallback)(hadc);
    }
#else
    HAL_ADC_ConvCpltCallback(hadc);
#endif
}

/**
 * @brief  DMA transfer stop callback
 * @param  hdma Pointer to a DMA_Handle_T structure that contains the configuration information for the specified DMA.
 * @retval void
 */
static void _adc_dma_xfer_abort_callback(DMA_Handle_T *hdma)
{
    ADC_Handle_T *hadc   = (ADC_Handle_T *)hdma->parent;
    int32_t       dma_ch = get_adc_dma_ch(hadc, hdma);
    if (dma_ch != -1)
    {
        /* Disable ADC DMA */
        uint32_t *dam_ch_reg = (uint32_t *)((&hadc->instance->DMA0CTRL_b) + (dma_ch * 0x04));
        HAL_REG_CBIT(*dam_ch_reg, 1 << 0x10);

        /* Clear the ADC DMA channel busy state */
        HAL_FLAG_CLR(hadc->dma_channel_state, ADC_CHANNEL_STATE_BUSY << (dma_ch * 0x08));

        /* If all ADC DMA channel is ready, stop ADC conversion */
        if (hadc->dma_channel_state == 0x01010101)
        {
            _stop_adc_dma(hadc);
        }
    }

#if (configUSE_ADC_REGISTER_CALLBACKS == 1)
    if (hadc->StopCallback != NULL)
    {
        (hadc->StopCallback)(hadc);
    }
#else
    HAL_ADC_StopCallback(hadc);
#endif
}

/**
 * @brief  DMA transfer error callback
 * @param  hdma Pointer to a DMA_Handle_T structure that contains the configuration information for the specified DMA.
 * @retval void
 */
static void _adc_dma_xfer_error_callback(DMA_Handle_T *hdma)
{
    ADC_Handle_T *hadc = (ADC_Handle_T *)hdma->parent;
    HAL_FLAG_WRITE(hadc->state, HAL_STATE_READY);
    hadc->error_code = HAL_DMA_GetError(hdma);
    _stop_adc_dma(hadc);
#if (configUSE_ADC_REGISTER_CALLBACKS == 1)
    if (hadc->ErrorCallback != NULL)
    {
        (hadc->ErrorCallback)(hadc);
    }
#else
    HAL_ADC_ErrorCallback(hadc);
#endif
}

#endif /* configUSE_ADC_NONBLOCKING_DMA */

#endif /* configUSE_ADC_PERIPHERAL */
