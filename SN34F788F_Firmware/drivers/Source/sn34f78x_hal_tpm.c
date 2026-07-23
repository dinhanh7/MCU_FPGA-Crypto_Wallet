/***********************
 * SN34F78X SDK V2.1.4 *
 ***********************/

#include "sn34f78x_hal_tpm.h"
#if (configUSE_TPM_PERIPHERAL == 1)
/* Initialization/de-initialization functions  ********************************/
/**
 * @brief  Initializes the TPM mode according to the specified parameters in the TPM_Init_T and create the associated handle.
 * @param  htpm Pointer to a TPM_Handle_T structure that contains the configuration information for the specified TPM module.
 * @retval HAL status
 */
HAL_Status_T HAL_TPM_Init(TPM_Handle_T *htpm)
{
    /* Check the TPM handle */
    ASSERT_PARAM(htpm);
    /* Check the TPM instance */
    ASSERT_PARAM(IS_TPM_INSTANCE(htpm));
    /* Check the TPM PRE */
    ASSERT_PARAM(IS_TPM_PRE(htpm));
    /* Check the TPM mode */
    ASSERT_PARAM(IS_TPM_MODE(htpm));
    /* Check the TPM counting mode */
    ASSERT_PARAM(IS_TPM_COUNTING_MODE(htpm));
    /* Check TPM instance support counting mode: Only CT16B0\SN_CT16B1\SN_CT16B2\SN_CT16B5 support all counting mode option, and other instance only support edge-aligned Up-counting mode. */
    RET_FLAG_FALSE(CHECK_SUPPORT_COUNTING_MODE(htpm), HAL_ERROR);
    /* Check TPM instance support Counter/Timer mode: Only CT16B0\SN_CT16B1\SN_CT16B2\SN_CT16B4\SN_CT16B5 support Counter/Timer mode.  */
    RET_FLAG_FALSE(CHECK_SUPPORT_CTM(htpm), HAL_ERROR);
    /* Check whether is already initialized */
    if (HAL_FLAG_EQU(htpm->state, HAL_STATE_RESET))
    {
        /* Protect the process */
        TAKE_MUTEX(htpm);
#if (configUSE_TPM_REGISTER_CALLBACKS == 1)
        if (htpm->MspInitCallback == NULL)
        {
            htpm->MspInitCallback = HAL_TPM_MspInit;
        }
        if (htpm->MspDeInitCallback == NULL)
        {
            htpm->MspDeInitCallback = HAL_TPM_MspDeInit;
        }
        if (htpm->OC_DelayElapsedCallback == NULL)
        {
            htpm->OC_DelayElapsedCallback = HAL_TPM_OC_DelayElapsedCallback;
        }
        if (htpm->PWM_PulseFinishedCallback == NULL)
        {
            htpm->PWM_PulseFinishedCallback = HAL_TPM_PWM_PulseFinishedCallback;
        }
        if (htpm->BreakCallback == NULL)
        {
            htpm->BreakCallback = HAL_TPM_BreakCallback;
        }
        if (htpm->CaptureCallback == NULL)
        {
            htpm->CaptureCallback = HAL_TPM_CaptureCallback;
        }
        if (htpm->ErrorCallback == NULL)
        {
            htpm->ErrorCallback = HAL_TPM_ErrorCallback;
        }
        htpm->MspInitCallback(htpm);
#else
        HAL_TPM_MspInit(htpm);
#endif /* configUSE_TPM_REGISTER_CALLBACKS */

#if (configUSE_TPM_DMA == 1U)
        _register_dma_callback(htpm);
#endif
        /* Set the state to busy */
        HAL_FLAG_WRITE(htpm->state, HAL_STATE_BUSY);
        /* Reset counting */
        HAL_REG_SBIT(*REG_TMRCTRL(htpm), 1 << 1);
        /* Counting mode selection */
        if (function_table[htpm->tpm_index] & TPM_CM)
        {
            HAL_REG_SBIT(*REG_TMRCTRL(htpm), (htpm->init.cm << 4));
        }
        /* Counting mode selection */
        if (function_table[htpm->tpm_index] & TPM_CTM)
        {
            HAL_REG_SBIT(*REG_CNTCTRL(htpm), htpm->init.mode);
            /* Enable capture when TPM CTM is not Timer */
            if (!HAL_FLAG_EQU(htpm->init.mode, TPM_CTM_TIMER))
            {
                HAL_REG_SBIT(*REG_CAP_CONTROL(htpm), (1 << 3));
            }
        }
        /* Counting mode selection */
        HAL_REG_WRITE(*REG_PRE(htpm), htpm->init.pre_scaler);
        /* update state to ready */
        htpm->error_code = HAL_OK;
        HAL_FLAG_WRITE(htpm->state, HAL_STATE_READY);
        _tpm_set_index(htpm);
        /* Update active channel */
        htpm->active_channel = TPM_CHANNEL_CLEARED;
        /* Clear error code */
        CLEAR_ERROR_CODE(htpm);
        /* init mutex */
        GIVE_MUTEX(htpm);
    }
    else
    {
        /* update the state */
        htpm->error_code = HAL_ERROR_REINIT;
        return HAL_ERROR;
    }
    return HAL_OK;
}

/**
 * @brief  DeInitializes the TPM peripheral.
 * @param  htpm Pointer to a TPM_Handle_T structure that contains the configuration information for the specified TPM.
 * @retval HAL status
 */
HAL_Status_T HAL_TPM_DeInit(TPM_Handle_T *htpm)
{
    /* Check the TPM handle */
    ASSERT_PARAM(htpm);
    /* Check the TPM instance */
    ASSERT_PARAM(IS_TPM_INSTANCE(htpm));
    /* Protect the process */
    TAKE_MUTEX(htpm);

    /* Disable TPM */
    HAL_REG_CBIT(*REG_TMRCTRL(htpm), 1);

#if (configUSE_TPM_REGISTER_CALLBACKS == 1)
    if (htpm->MspDeInitCallback == NULL)
    {
        htpm->MspDeInitCallback = HAL_TPM_MspDeInit;
    }
    htpm->MspDeInitCallback(htpm);
#else
    HAL_TPM_MspDeInit(htpm);
#endif /* configUSE_TPM_REGISTER_CALLBACKS */

#if (configUSE_TPM_REGISTER_CALLBACKS == 1)
    /* Reset default callback function */
    _reset_callback(htpm);
#endif /* configUSE_TPM_REGISTER_CALLBACKS */
    /* Reset TPM base */
    _reset_base(htpm);
    /* Clear error code */
    CLEAR_ERROR_CODE(htpm);
    /* Release the process protection */
    GIVE_MUTEX(htpm);
    return HAL_OK;
}

/**
 * @brief  TPM Msp Init
 * @param  htpm Pointer to a TPM_Handle_T structure that contains the configuration information for the specified TPM.
 * @retval None
 */
__weak void HAL_TPM_MspInit(TPM_Handle_T *htpm)
{
    /* Check the TPM handle */
    ASSERT_PARAM(htpm);
    /* Check the TPM instance */
    ASSERT_PARAM(IS_TPM_INSTANCE(htpm));
    /* Prevent unused argument(s) compilation warning */
    UNUSED(htpm);

    /* NOTE : This function should not be modified, when the callback is needed,
              the HAL_TPM_MspInit could be implemented in the user file
     */
}

/**
 * @brief  TPM Msp DeInit callback
 * @param  htpm Pointer to a TPM_Handle_T structure that contains the configuration information for the specified TPM.
 * @retval None
 */
__weak void HAL_TPM_MspDeInit(TPM_Handle_T *htpm)
{
    /* Check the TPM handle */
    ASSERT_PARAM(htpm);
    /* Check the TPM instance */
    ASSERT_PARAM(IS_TPM_INSTANCE(htpm));
    /* Prevent unused argument(s) compilation warning */
    UNUSED(htpm);

    /* NOTE : This function should not be modified, when the callback is needed,
              the HAL_TPM_MspDeInit could be implemented in the user file
     */
}

/* Callbacks Register/UnRegister functions ************************************/
#if (configUSE_TPM_REGISTER_CALLBACKS == 1)
/**
 * @brief  Register a User TPM callback to be used instead of the weak predefined callback.
 * @param  htpm Pointer to a TPM_Handle_T structure that contains the configuration information for the specified TPM module.
 * @param  callback_id ID of the callback to be registered.
 * @param  callback pointer to the Callback function
 * @retval HAL status
 */
HAL_Status_T HAL_TPM_RegisterCallback(TPM_Handle_T *htpm, HAL_TPM_CallbackID_T callback_id, TPM_Callback_T callback)
{
    /* Check the TPM handle */
    ASSERT_PARAM(htpm);
    /* Check the TPM instance */
    ASSERT_PARAM(IS_TPM_INSTANCE(htpm));
    /* Protect the process */
    TAKE_MUTEX(htpm);
    /* Clear error code */
    CLEAR_ERROR_CODE(htpm);
    HAL_Status_T status = HAL_OK;
    switch (callback_id)
    {
        case HAL_TPM_MSPINIT_CB_ID:
            htpm->MspInitCallback = callback;
            break;
        case HAL_TPM_MSPDEINIT_CB_ID:
            htpm->MspDeInitCallback = callback;
            break;
        case HAL_TPM_OC_DELAY_ELAPSED_CB_ID:
            htpm->OC_DelayElapsedCallback = callback;
            break;
        case HAL_TPM_PWM_PULSE_FINISHED_CB_ID:
            htpm->PWM_PulseFinishedCallback = callback;
            break;
        case HAL_TPM_BREAK_CB_ID:
            htpm->BreakCallback = callback;
            break;
        case HAL_TPM_CAP0_CB_ID:
            htpm->CaptureCallback = callback;
            break;
        case HAL_TPM_ERROR_CB_ID:
            htpm->ErrorCallback = callback;
            break;
        default:
            status = HAL_ERROR;
    }
    /* Release the process protection */
    GIVE_MUTEX(htpm);
    return status;
}

/**
 * @brief  Unregister an TPM callback, TPM callback is redirected to the weak predefined callback.
 * @param  hdma Pointer to a TPM_Handle_T structure that contains the configuration information for the specified TPM module.
 * @param  callback_id ID of the callback to be unregistered.
 *         This parameter can be one of the following values:
 *           @arg @ref HAL_TPM_TIMEOUT_CB_ID TPM Timeout Callback ID
 * @retval HAL status
 */
HAL_Status_T HAL_TPM_UnRegisterCallback(TPM_Handle_T *htpm, HAL_TPM_CallbackID_T callback_id)
{
    /* Check the TPM handle */
    ASSERT_PARAM(htpm);
    /* Check the TPM instance */
    ASSERT_PARAM(IS_TPM_INSTANCE(htpm));
    /* Protect the process */
    TAKE_MUTEX(htpm);
    /* Clear error code */
    CLEAR_ERROR_CODE(htpm);
    HAL_Status_T status = HAL_OK;
    switch (callback_id)
    {
        case HAL_TPM_MSPINIT_CB_ID:
            htpm->MspInitCallback = HAL_TPM_MspInit;
            break;
        case HAL_TPM_MSPDEINIT_CB_ID:
            htpm->MspDeInitCallback = HAL_TPM_MspDeInit;
            break;
        case HAL_TPM_OC_DELAY_ELAPSED_CB_ID:
            htpm->OC_DelayElapsedCallback = HAL_TPM_OC_DelayElapsedCallback;
            break;
        case HAL_TPM_PWM_PULSE_FINISHED_CB_ID:
            htpm->PWM_PulseFinishedCallback = HAL_TPM_PWM_PulseFinishedCallback;
            break;
        case HAL_TPM_BREAK_CB_ID:
            htpm->BreakCallback = HAL_TPM_BreakCallback;
            break;
        case HAL_TPM_CAP0_CB_ID:
            htpm->CaptureCallback = HAL_TPM_CaptureCallback;
            break;
        case HAL_TPM_ERROR_CB_ID:
            htpm->ErrorCallback = HAL_TPM_ErrorCallback;
            break;
        case HAL_TPM_ALL_CB_ID:
            htpm->MspInitCallback           = HAL_TPM_MspInit;
            htpm->MspDeInitCallback         = HAL_TPM_MspDeInit;
            htpm->OC_DelayElapsedCallback   = HAL_TPM_OC_DelayElapsedCallback;
            htpm->PWM_PulseFinishedCallback = HAL_TPM_PWM_PulseFinishedCallback;
            htpm->BreakCallback             = HAL_TPM_BreakCallback;
            htpm->CaptureCallback           = HAL_TPM_CaptureCallback;
            htpm->ErrorCallback             = HAL_TPM_ErrorCallback;
            break;
        default:
            status = HAL_ERROR;
            break;
    }
    /* Release the process protection */
    GIVE_MUTEX(htpm);
    return status;
}

#endif /* configUSE_TPM_REGISTER_CALLBACKS */
/* Event Callback functions ***************************************************/
/**
 * @brief  Input Capture callback
 * @param  htpm Pointer to a TPM_Handle_T structure that contains the configuration information for the specified TPM.
 * @retval None
 */
__weak void HAL_TPM_CaptureCallback(TPM_Handle_T *htpm)
{
    /* Check the TPM handle */
    ASSERT_PARAM(htpm);
    /* Check the TPM instance */
    ASSERT_PARAM(IS_TPM_INSTANCE(htpm));
    /* prevent unused argument(s) compilation warning */
    UNUSED(htpm);
    /* NOTE: This function should not be modified, when the callback is needed,
         the HAL_TPM_CaptureCallback could be implemented in the user file */
}

/**
 * @brief  Output Compare callback
 * @param  htpm Pointer to a TPM_Handle_T structure that contains the configuration information for the specified TPM.
 * @retval None
 */
__weak void HAL_TPM_OC_DelayElapsedCallback(TPM_Handle_T *htpm)
{
    /* Check the TPM handle */
    ASSERT_PARAM(htpm);
    /* Check the TPM instance */
    ASSERT_PARAM(IS_TPM_INSTANCE(htpm));
    /* prevent unused argument(s) compilation warning */
    UNUSED(htpm);
    /* NOTE: This function should not be modified, when the callback is needed,
         the HAL_TPM_OC_DelayElapsedCallback could be implemented in the user file */
}

/**
 * @brief  PWM Pulse finished callback
 * @param  htpm Pointer to a TPM_Handle_T structure that contains the configuration information for the specified TPM.
 * @retval None
 */
__weak void HAL_TPM_PWM_PulseFinishedCallback(TPM_Handle_T *htpm)
{
    /* Check the TPM handle */
    ASSERT_PARAM(htpm);
    /* Check the TPM instance */
    ASSERT_PARAM(IS_TPM_INSTANCE(htpm));
    /* prevent unused argument(s) compilation warning */
    UNUSED(htpm);
    /* NOTE: This function should not be modified, when the callback is needed,
         the HAL_TPM_PWM_PulseFinishedCallback could be implemented in the user file */
}

/**
 * @brief  Break Function Callback
 * @param  htpm Pointer to a TPM_Handle_T structure that contains the configuration information for the specified TPM.
 * @retval None
 */
__weak void HAL_TPM_BreakCallback(TPM_Handle_T *htpm)
{
    /* Check the TPM handle */
    ASSERT_PARAM(htpm);
    /* Check the TPM instance */
    ASSERT_PARAM(IS_TPM_INSTANCE(htpm));
    /* prevent unused argument(s) compilation warning */
    UNUSED(htpm);
    /* NOTE: This function should not be modified, when the callback is needed,
         the HAL_TPM_BreakCallback could be implemented in the user file */
}

/**
 * @brief  TPM Error Callback
 * @param  htpm Pointer to a TPM_Handle_T structure that contains the configuration information for the specified TPM.
 * @retval None
 */
__weak void HAL_TPM_ErrorCallback(TPM_Handle_T *htpm)
{
    /* Check the TPM handle */
    ASSERT_PARAM(htpm);
    /* Check the TPM instance */
    ASSERT_PARAM(IS_TPM_INSTANCE(htpm));
    /* prevent unused argument(s) compilation warning */
    UNUSED(htpm);
    /* NOTE: This function should not be modified, when the callback is needed,
         the HAL_TPM_ErrorCallback could be implemented in the user file */
}

/* Base functions ********************************************************/
/**
 * @brief  Start the TPM signal generation in polling mode.
 * @param  htpm Pointer to a TPM_Handle_T structure that contains the configuration information for the specified TPM.
 * @retval HAL_Status_T
 */
HAL_Status_T HAL_TPM_Base_Start(TPM_Handle_T *htpm)
{
    /* Check the TPM handle */
    ASSERT_PARAM(htpm);
    /* Check the TPM instance */
    ASSERT_PARAM(IS_TPM_INSTANCE(htpm));
    /* Clear error code */
    CLEAR_ERROR_CODE(htpm);
    /* Check the TPM state */
    RET_FLAG_TRUE(HAL_FLAG_EQU(htpm->state, HAL_STATE_RESET), HAL_ERROR);
    /* Protect the process */
    TAKE_MUTEX(htpm);
    /* Set the state to busy */
    HAL_FLAG_WRITE(htpm->state, HAL_STATE_BUSY);
    /* Enable Timer Counter and Prescale Counter for counting */
    HAL_REG_SBIT(*REG_TMRCTRL(htpm), 1);
    /* Release the process protection */
    GIVE_MUTEX(htpm);
    return HAL_OK;
}

/**
 * @brief  Stop the TPM signal generation in polling mode.
 * @param  htpm Pointer to a TPM_Handle_T structure that contains the configuration information for the specified TPM.
 * @retval HAL_Status_T
 */
HAL_Status_T HAL_TPM_Base_Stop(TPM_Handle_T *htpm)
{
    /* Check the TPM handle */
    ASSERT_PARAM(htpm);
    /* Check the TPM instance */
    ASSERT_PARAM(IS_TPM_INSTANCE(htpm));
    /* Clear error code */
    CLEAR_ERROR_CODE(htpm);
    /* Check the TPM state */
    RET_FLAG_TRUE(HAL_FLAG_EQU(htpm->state, HAL_STATE_RESET), HAL_ERROR);
    /* Protect the process */
    TAKE_MUTEX(htpm);
    /* Set the state to ready */
    HAL_FLAG_WRITE(htpm->state, HAL_STATE_READY);
    /* Disable Timer Counter and Prescale Counter for counting */
    HAL_REG_CBIT(*REG_TMRCTRL(htpm), 1);
    /* Release the process protection */
    GIVE_MUTEX(htpm);
    return HAL_OK;
}

/**
 * @brief  Start the TPM signal generation in interrupt mode.
 * @param  htpm Pointer to a TPM_Handle_T structure that contains the configuration information for the specified TPM.
 * @retval HAL_Status_T
 */
HAL_Status_T HAL_TPM_Base_Start_IT(TPM_Handle_T *htpm)
{
    /* Check the TPM handle */
    ASSERT_PARAM(htpm);
    /* Check the TPM instance */
    ASSERT_PARAM(IS_TPM_INSTANCE(htpm));
    /* Clear error code */
    CLEAR_ERROR_CODE(htpm);
    /* Check the TPM state */
    RET_FLAG_TRUE(HAL_FLAG_EQU(htpm->state, HAL_STATE_RESET), HAL_ERROR);
    /* Protect the process */
    TAKE_MUTEX(htpm);
    /* Set the state to busy */
    HAL_FLAG_WRITE(htpm->state, HAL_STATE_BUSY);
    /* Enable match interrupt when channel is Timer */
    for (uint8_t i = 0; i <= TPM_CHANNEL_PERIOD; i++)
    {
        uint8_t channel_state = GET_CH_STATE(htpm->channel_info.state, i);
        uint8_t channel_mode  = GET_CH_MODE(htpm->channel_info.mode[i / TPM_CHANNEL_8], i);
        if (HAL_FLAG_EQU(channel_state, CH_STATE_READY) && HAL_FLAG_EQU(channel_mode, CH_TIMER))
        {
            _start_channel_base(htpm, i, HAL_ENABLE);
        }
    }
    /* Release the process protection */
    GIVE_MUTEX(htpm);
    return HAL_OK;
}

/**
 * @brief  Stop the TPM signal generation in interrupt mode.
 * @param  htpm Pointer to a TPM_Handle_T structure that contains the configuration information for the specified TPM.
 * @retval HAL_Status_T
 */
HAL_Status_T HAL_TPM_Base_Stop_IT(TPM_Handle_T *htpm)
{
    /* Check the TPM handle */
    ASSERT_PARAM(htpm);
    /* Check the TPM instance */
    ASSERT_PARAM(IS_TPM_INSTANCE(htpm));
    /* Clear error code */
    CLEAR_ERROR_CODE(htpm);
    /* Check the TPM state */
    RET_FLAG_TRUE(HAL_FLAG_EQU(htpm->state, HAL_STATE_RESET), HAL_ERROR);
    /* Protect the process */
    TAKE_MUTEX(htpm);
    /* Enable match interrupt when channel is Timer */
    for (uint8_t i = 0; i <= TPM_CHANNEL_PERIOD; i++)
    {
        uint8_t channel_state = GET_CH_STATE(htpm->channel_info.state, i);
        uint8_t channel_mode  = GET_CH_MODE(htpm->channel_info.mode[i / TPM_CHANNEL_8], i);
        if (HAL_FLAG_EQU(channel_state, CH_STATE_BUSY) && HAL_FLAG_EQU(channel_mode, CH_TIMER))
        {
            _stop_channel_base(htpm, i);
        }
    }
    /* Set the state to ready */
    HAL_FLAG_WRITE(htpm->state, HAL_STATE_READY);
    /* Disable Timer Counter and Prescale Counter for counting */
    HAL_REG_CBIT(*REG_TMRCTRL(htpm), 1);
    /* Release the process protection */
    GIVE_MUTEX(htpm);
    return HAL_OK;
}

/* PWM functions ********************************************************/
#if (configUSE_TPM_PWM == 1)
/**
 * @brief  PWM config
 * @param  htpm Pointer to a TPM_Handle_T structure that contains the configuration information for the specified TPM.
 * @param  config PWM config structural
 * @param  channel TPM channel to configure
 * @retval HAL_Status_T
 * @note SN_CT16B6&SN_CT16B7 not support PWM
 */
HAL_Status_T HAL_TPM_PWM_ConfigChannel(TPM_Handle_T *htpm, TPM_OC_T *config, uint32_t channel)
{
    /* Check the TPM handle */
    ASSERT_PARAM(htpm);
    /* Check the TPM instance */
    ASSERT_PARAM(IS_TPM_INSTANCE(htpm));
    /* Check the Channle config */
    ASSERT_PARAM(IS_TPM_CHANNEL_COUNTING(config));
    /* Check channel is valid */
    ASSERT_PARAM(IS_TPM_CHANNEL(channel));
    /* Check is PWM mode */
    ASSERT_PARAM(IS_PWM_MODE(config->mode));
    /* Clear error code */
    CLEAR_ERROR_CODE(htpm);
    /* Check instance support PWM */
    RET_FLAG_FALSE((channel_table[htpm->tpm_index][channel] & CH_PWM), HAL_ERROR);
    /* Not suport PWM mode 2 when center mode */
    RET_FLAG_TRUE(CHECK_PWM2_IN_CENTER_MODE(htpm->init.cm, config->mode), HAL_ERROR);
    /* Check TPM state */
    RET_FLAG_FALSE(HAL_FLAG_EQU(htpm->state, HAL_STATE_READY), HAL_ERROR);
    /* Check TPM CTM is not counter mode */
    if (function_table[htpm->tpm_index] & TPM_CTM)
    {
        RET_FLAG_FALSE(HAL_FLAG_EQU(htpm->init.mode, 0), HAL_ERROR);
    }
    /* Protect the process */
    TAKE_MUTEX(htpm);
    /*  Set channel PWM config */
    _config_channel_pwm(htpm, config, channel);
    /* Set channel info */
    WRITE_CH_MODE(htpm->channel_info.mode[channel / TPM_CHANNEL_8], channel, CH_PWM);
    /* Release the process protection */
    GIVE_MUTEX(htpm);
    return HAL_OK;
}

/**
 * @brief  Start PWM in polling mode
 * @param  htpm Pointer to a TPM_Handle_T structure that contains the configuration information for the specified TPM.
 * @param  config capture config structural
 * @param  channel TPM channel
 * @retval HAL_Status_T
 */
HAL_Status_T HAL_TPM_PWM_Start(TPM_Handle_T *htpm, uint32_t channel)
{
    return _start_channel_pwm(htpm, channel, HAL_DISABLE);
}

/**
 * @brief  Stop PWM in polling mode
 * @param  htpm Pointer to a TPM_Handle_T structure that contains the configuration information for the specified TPM.
 * @param  channel TPM channel
 * @retval HAL_Status_T
 */
HAL_Status_T HAL_TPM_PWM_Stop(TPM_Handle_T *htpm, uint32_t channel)
{
    return _stop_channel_pwm(htpm, channel);
}

/**
 * @brief  Start PWM in interrupt mode
 * @param  htpm Pointer to a TPM_Handle_T structure that contains the configuration information for the specified TPM.
 * @param  config capture config structural
 * @param  channel TPM channel
 * @retval HAL_Status_T
 */
HAL_Status_T HAL_TPM_PWM_Start_IT(TPM_Handle_T *htpm, uint32_t channel)
{
    return _start_channel_pwm(htpm, channel, HAL_ENABLE);
}

/**
 * @brief  Stop PWM in interrupt mode
 * @param  htpm Pointer to a TPM_Handle_T structure that contains the configuration information for the specified TPM.
 * @param  channel TPM channel
 * @retval HAL_Status_T
 */
HAL_Status_T HAL_TPM_PWM_Stop_IT(TPM_Handle_T *htpm, uint32_t channel)
{
    return _stop_channel_pwm(htpm, channel);
}

#if (configUSE_TPM_DMA == 1)
/**
 * @brief  Start PWM in DMA mode
 * @param  htpm Pointer to a TPM_Handle_T structure that contains the configuration information for the specified TPM.
 * @param  config capture config structural
 * @param  channel TPM channel
 * @retval HAL_Status_T
 */
HAL_Status_T HAL_TPM_PWM_Start_DMA(TPM_Handle_T *htpm, uint32_t channel, uint32_t *data, uint32_t length)
{
    /* Check the TPM handle */
    ASSERT_PARAM(htpm);
    /* Check the TPM instance */
    ASSERT_PARAM(IS_TPM_INSTANCE(htpm));
    /* Check DMA instance */
    ASSERT_PARAM(IS_DMA_INSTANCE(htpm->hdma[GET_DMA_ID(channel)]));
    /* Clear error code */
    CLEAR_ERROR_CODE(htpm);
    /* Check instance support PWM */
    RET_FLAG_FALSE((channel_table[htpm->tpm_index][channel] & CH_PWM), HAL_ERROR);
    /* Check channel request for DMA */
    RET_FLAG_FALSE((channel_table[htpm->tpm_index][channel] & CH_DMA), HAL_ERROR);
    /* Check channel mode */
    uint8_t channel_mode = GET_CH_MODE(htpm->channel_info.mode[channel / TPM_CHANNEL_8], channel);
    RET_FLAG_FALSE(HAL_FLAG_EQU(channel_mode, CH_PWM), HAL_ERROR);
    /* Check channel state */
    uint8_t channel_state = GET_CH_STATE(htpm->channel_info.state, channel);
    RET_FLAG_FALSE((HAL_FLAG_EQU(channel_state, CH_STATE_READY) || CH_IN_BREAK(htpm, channel)), HAL_ERROR);
    /* Protect the process */
    TAKE_MUTEX(htpm);
    /* Set channel dma request */
    HAL_REG_SBIT(*REG_DMA(htpm), 1 << GET_DMA_REQ(dma_table[htpm->tpm_index][GET_DMA_ID(channel)]));
    /* Set value of MR from data */
    HAL_Status_T status = _start_channel_dma(htpm, channel, data, length);
    if (status == HAL_OK)
    {
        /* Clear channel RIS */
        HAL_REG_SBIT(*REG_IC(htpm), 1 << _get_ch_ic_bit(htpm, channel));
        /* Enable output pin */
        HAL_REG_SBIT(*REG_PWM_IO_ENABLE(htpm), HAL_ENABLE << PWM_IO_EN_BIT(htpm, channel));
        /* Enable PWMN output if current channel support PWMN and config PWMN */
        if (channel_table[htpm->tpm_index][channel] & CH_PWMN)
        {
            uint8_t pwmn_mode = GET_CH_PWMN_MODE(htpm->channel_info.pwmn_mode, channel);
            HAL_REG_WBIT(*REG_PWMCTRL(htpm), 0x03 << (24 + 2 * channel), pwmn_mode << (24 + 2 * channel));
        }
        status = _start_channel_base(htpm, channel, HAL_DISABLE);
    }
    /* Release the process protection */
    GIVE_MUTEX(htpm);
    return status;
}

/**
 * @brief  Stop PWM in DMA mode
 * @param  htpm Pointer to a TPM_Handle_T structure that contains the configuration information for the specified TPM.
 * @param  channel TPM channel
 * @retval HAL_Status_T
 */
HAL_Status_T HAL_TPM_PWM_Stop_DMA(TPM_Handle_T *htpm, uint32_t channel)
{
    /* Check the TPM handle */
    ASSERT_PARAM(htpm);
    /* Check the TPM instance */
    ASSERT_PARAM(IS_TPM_INSTANCE(htpm));
    /* Check DMA instance */
    ASSERT_PARAM(IS_DMA_INSTANCE(htpm->hdma[GET_DMA_ID(channel)]));
    /* Clear error code */
    CLEAR_ERROR_CODE(htpm);
    /* Check instance support PWM */
    RET_FLAG_FALSE((channel_table[htpm->tpm_index][channel] & CH_OC), HAL_ERROR);
    /* Check channel request for DMA */
    RET_FLAG_FALSE((channel_table[htpm->tpm_index][channel] & CH_DMA), HAL_ERROR);
    /* Check channel mode */
    uint8_t channel_mode = GET_CH_MODE(htpm->channel_info.mode[channel / TPM_CHANNEL_8], channel);
    RET_FLAG_FALSE(HAL_FLAG_EQU(channel_mode, CH_PWM), HAL_ERROR);
    /* Check channel state */
    uint8_t channel_state = GET_CH_STATE(htpm->channel_info.state, channel);
    RET_FLAG_FALSE(HAL_FLAG_EQU(channel_state, CH_STATE_BUSY), HAL_ERROR);
    /* Protect the process */
    TAKE_MUTEX(htpm);
    /* Clear channel dma request */
    HAL_REG_CBIT(*REG_DMA(htpm), 1 << GET_DMA_REQ(dma_table[htpm->tpm_index][GET_DMA_ID(channel)]));

    HAL_Status_T status = _stop_channel_dma(htpm, channel);
    if (status == HAL_OK)
    {
        /* Disable output pin */
        HAL_REG_CBIT(*REG_PWM_IO_ENABLE(htpm), 1 << PWM_IO_EN_BIT(htpm, channel));
        /* Enable PWMN output if current channel support PWMN and config PWMN */
        if (channel_table[htpm->tpm_index][channel] & CH_PWMN)
        {
            HAL_REG_CBIT(*REG_PWMCTRL(htpm), 0x03 << (24 + 2 * channel));
        }

        status = _stop_channel_base(htpm, channel);
    }
    /* Release the process protection */
    GIVE_MUTEX(htpm);
    return status;
}
#endif /* configUSE_TPM_DMA */

/**
 * @brief  PWMN config
 * @param  htpm Pointer to a TPM_Handle_T structure that contains the configuration information for the specified TPM.
 * @param  config PWMN config structural
 * @param  channel channel
 * @retval HAL_Status_T
 * @note
 *      1.Only SN_CT16B0&SN_CT16B3&SN_CT16B4 support PWMN
 *      2.PWMN config can not be used after TPM start
 */
HAL_Status_T HAL_TPM_PWM_ConfigPWMN(TPM_Handle_T *htpm, TPM_PWMN_T *config, uint32_t channel)
{
    /* Check the TPM handle */
    ASSERT_PARAM(htpm);
    /* Check the TPM instance */
    ASSERT_PARAM(IS_TPM_INSTANCE(htpm));
    /* Check the PWMN config */
    ASSERT_PARAM(IS_TPM_PWMN_COUNTING(config));
    /* Check channel is valid */
    ASSERT_PARAM(IS_TPM_CHANNEL(channel));
    /* Clear error code */
    CLEAR_ERROR_CODE(htpm);
    /* Check instance support PWMN */
    RET_FLAG_FALSE((channel_table[htpm->tpm_index][channel] & CH_PWMN), HAL_ERROR);
    /* Check TPM state */
    RET_FLAG_FALSE(HAL_FLAG_EQU(htpm->state, HAL_STATE_READY), HAL_ERROR);
    /* Check channel state */
    uint8_t channel_state = GET_CH_STATE(htpm->channel_info.state, channel);
    RET_FLAG_FALSE((HAL_FLAG_EQU(channel_state, CH_STATE_READY)), HAL_ERROR);
    /* Check channel mode */
    uint8_t channel_mode = GET_CH_MODE(htpm->channel_info.mode[channel / TPM_CHANNEL_8], channel);
    RET_FLAG_FALSE(HAL_FLAG_EQU(channel_mode, CH_PWM), HAL_ERROR);
    /* Protect the process */
    TAKE_MUTEX(htpm);
    /* set PWMN mode */
    WRITE_CH_PWMN_MODE(htpm->channel_info.pwmn_mode, channel, config->mode);
    /* set PWMN dead-band time */
    HAL_REG_WRITE(*(REG_PWM0NDB(htpm) + channel), (config->db));
    /* Release the process protection */
    GIVE_MUTEX(htpm);
    return HAL_OK;
}

/**
 * @brief  PWM break function config
 * @param  htpm Pointer to a TPM_Handle_T structure that contains the configuration information for the specified TPM.
 * @param  config PWM breakfunction config structural
 * @retval HAL_Status_T
 * @note
 *      1.Only SN_CT16B0 support break function
 */
HAL_Status_T HAL_TPM_ConfigBreak(TPM_Handle_T *htpm, TPM_Break_T *config)
{
    /* Check the TPM handle */
    ASSERT_PARAM(htpm);
    /* Check the TPM instance */
    ASSERT_PARAM(IS_TPM_INSTANCE(htpm));
    /* Check the PWMN config */
    ASSERT_PARAM(IS_TPM_BREAK_COUNTING(config));
    /* Clear error code */
    CLEAR_ERROR_CODE(htpm);
    /* Check instance support break funtion */
    RET_FLAG_FALSE((function_table[htpm->tpm_index] & TPM_BREAK), HAL_ERROR);
    /* Check TPM state */
    RET_FLAG_FALSE(HAL_FLAG_EQU(htpm->state, HAL_STATE_READY), HAL_ERROR);
    /* Protect the process */
    TAKE_MUTEX(htpm);
    /* set break function config */
    uint32_t break_value = (config->break_mode & 0xF) | (((config->break_mode & 0x10) >> 4) << 3) | (config->break_db << 4) | (config->break_ie << 30);
    HAL_REG_WRITE(*REG_BRKCTRL(htpm), break_value);
    /* Release the process protection */
    GIVE_MUTEX(htpm);
    return HAL_OK;
}

/**
 * @brief  PWM load mode function config
 * @param  htpm Pointer to a TPM_Handle_T structure that contains the configuration information for the specified TPM.
 * @param  config PWM load mode function config structural
 * @retval HAL_Status_T
 * @note
 *      1.Only SN_CT16B0/SN_CT16B1/SN_CT16B2/SN_CT16B5 support this function
 */
HAL_Status_T HAL_TPM_PWM_ConfigLoadMode(TPM_Handle_T *htpm, TPM_LoadMode_T *config)
{
    /* Check the TPM handle */
    ASSERT_PARAM(htpm);
    /* Check the TPM instance */
    ASSERT_PARAM(IS_TPM_INSTANCE(htpm));
    /* Check the load mode config */
    ASSERT_PARAM(IS_TPM_LOAD_MODE_CONFIG(config));
    /* Clear error code */
    CLEAR_ERROR_CODE(htpm);
    /* Check instance support load mode funtion */
    RET_FLAG_FALSE((channel_table[htpm->tpm_index][config->channel] & CH_LOAD), HAL_ERROR);
    /* Check TPM state */
    RET_FLAG_FALSE(HAL_FLAG_EQU(htpm->state, HAL_STATE_READY), HAL_ERROR);
    /* Protect the process */
    TAKE_MUTEX(htpm);
    /* Set load mode */
    SN_CT16B0_Type *ct16_instance = (SN_CT16B0_Type *)htpm->instance;
    switch (config->channel)
    {
        case TPM_CHANNEL_0:
            HAL_REG_WRITE(ct16_instance->LOADCTRL_b.LOAD_MR0, config->load_mode);
            break;
        case TPM_CHANNEL_1:
            HAL_REG_WRITE(ct16_instance->LOADCTRL_b.LOAD_MR1, config->load_mode);
            break;
        case TPM_CHANNEL_2:
            HAL_REG_WRITE(ct16_instance->LOADCTRL_b.LOAD_MR2, config->load_mode);
            break;
        case TPM_CHANNEL_3:
            HAL_REG_WRITE(ct16_instance->LOADCTRL_b.LOAD_MR3, config->load_mode);
            break;
        default:
            break;
    }
    /* Release the process protection */
    GIVE_MUTEX(htpm);
    return HAL_OK;
}

/**
 * @brief  PWM update match value
 * @param  htpm Pointer to a TPM_Handle_T structure that contains the configuration information for the specified TPM.
 * @param  match_value match value
 * @param  channel channel
 * @retval HAL_Status_T
 */
HAL_Status_T HAL_TPM_PWM_ConfigMatchValue(TPM_Handle_T *htpm, uint32_t match_value, uint32_t channel)
{
    /* Check the TPM handle */
    ASSERT_PARAM(htpm);
    /* Check the TPM instance */
    ASSERT_PARAM(IS_TPM_INSTANCE(htpm));
    /* Clear error code */
    CLEAR_ERROR_CODE(htpm);
    /* Check instance support load mode funtion */
    RET_FLAG_FALSE((channel_table[htpm->tpm_index][channel] & CH_REG_UPDATE), HAL_ERROR);
    /* Check TPM state */
    RET_FLAG_FALSE(!HAL_FLAG_EQU(htpm->state, HAL_STATE_RESET), HAL_ERROR);
    /* Protect the process */
    TAKE_MUTEX(htpm);
    /* Update MRn value */
    HAL_REG_WRITE(*(REG_MR0(htpm) + channel), match_value);
    /* Release the process protection */
    GIVE_MUTEX(htpm);
    return HAL_OK;
}

#endif /* configUSE_TPM_PWM */

/* Output Compare_ functions ********************************************************/
#if (configUSE_TPM_OC == 1)
/**
 * @brief  OC config, involve Timer\EMC\Forced
 * @param  htpm Pointer to a TPM_Handle_T structure that contains the configuration information for the specified TPM.
 * @param  config OC config structural
 * @param  channel TPM channel
 * @retval HAL_Status_T
 * @note
 *      1.SN_CT16B6&SN_CT16B7 not support EMC and Forced
 *      2.If config->mode is Forced, the function can not used after TPM start
 */
HAL_Status_T HAL_TPM_OC_ConfigChannel(TPM_Handle_T *htpm, TPM_OC_T *config, uint32_t channel)
{
    /* Check the TPM handle */
    ASSERT_PARAM(htpm);
    /* Check the TPM instance */
    ASSERT_PARAM(IS_TPM_INSTANCE(htpm));
    /* Check the Channle config */
    ASSERT_PARAM(IS_TPM_CHANNEL_COUNTING(config));
    /* Check channel is valid */
    ASSERT_PARAM(IS_TPM_CHANNEL(channel));
    /* Check is OC mode */
    ASSERT_PARAM(IS_OC_MODE(config->mode));
    /* Clear error code */
    CLEAR_ERROR_CODE(htpm);
    /* Check TPM state */
    RET_FLAG_FALSE(HAL_FLAG_EQU(htpm->state, HAL_STATE_READY), HAL_ERROR);
    /* Check instance support OC or Timer */
    RET_FLAG_FALSE((channel_table[htpm->tpm_index][channel] & (CH_OC | CH_TIMER)), HAL_ERROR);
    /* Check TPM CTM is not counter mode when channel mode is not TPM_OCMODE_TIMER */
    if ((function_table[htpm->tpm_index] & TPM_CTM) && HAL_FLAG_NEQ(config->mode, TPM_OCMODE_TIMER))
    {
        RET_FLAG_FALSE(HAL_FLAG_EQU(htpm->init.mode, 0), HAL_ERROR);
    }
    /* Check channel support output */
    RET_FLAG_TRUE((HAL_FLAG_NEQ(config->mode, TPM_OCMODE_TIMER) && (!(channel_table[htpm->tpm_index][channel] & CH_OC))), HAL_ERROR);
    /* Protect the process */
    TAKE_MUTEX(htpm);
    /* Set channel OC config */
    _config_channel_oc(htpm, config, channel);
    /* Set channel info */
    if (HAL_FLAG_EQU(config->mode, TPM_OCMODE_TIMER))
    {
        WRITE_CH_MODE(htpm->channel_info.mode[channel / TPM_CHANNEL_8], channel, CH_TIMER);
    }
    else
    {
        WRITE_CH_MODE(htpm->channel_info.mode[channel / TPM_CHANNEL_8], channel, CH_OC);
    }
    /* Release the process protection */
    GIVE_MUTEX(htpm);
    return HAL_OK;
}

/**
 * @brief  Start OC in polling mode
 * @param  htpm Pointer to a TPM_Handle_T structure that contains the configuration information for the specified TPM.
 * @param  config capture config structural
 * @param  channel TPM channel
 * @retval HAL_Status_T
 */
HAL_Status_T HAL_TPM_OC_Start(TPM_Handle_T *htpm, uint32_t channel)
{
    return _start_channel_oc(htpm, channel, HAL_DISABLE);
}

/**
 * @brief  Stop OC in polling mode
 * @param  htpm Pointer to a TPM_Handle_T structure that contains the configuration information for the specified TPM.
 * @param  channel TPM channel
 * @retval HAL_Status_T
 */
HAL_Status_T HAL_TPM_OC_Stop(TPM_Handle_T *htpm, uint32_t channel)
{
    return _stop_channel_oc(htpm, channel);
}

/**
 * @brief  Start OC in interrupt mode
 * @param  htpm Pointer to a TPM_Handle_T structure that contains the configuration information for the specified TPM.
 * @param  config capture config structural
 * @param  channel TPM channel
 * @retval HAL_Status_T
 */
HAL_Status_T HAL_TPM_OC_Start_IT(TPM_Handle_T *htpm, uint32_t channel)
{
    return _start_channel_oc(htpm, channel, HAL_ENABLE);
}

/**
 * @brief  Stop OC in interrupt mode
 * @param  htpm Pointer to a TPM_Handle_T structure that contains the configuration information for the specified TPM.
 * @param  channel TPM channel
 * @retval HAL_Status_T
 */
HAL_Status_T HAL_TPM_OC_Stop_IT(TPM_Handle_T *htpm, uint32_t channel)
{
    return _stop_channel_oc(htpm, channel);
}

#if (configUSE_TPM_DMA == 1)
/**
 * @brief  Start OC in DMA mode
 * @param  htpm Pointer to a TPM_Handle_T structure that contains the configuration information for the specified TPM.
 * @param  config capture config structural
 * @param  channel TPM channel
 * @retval HAL_Status_T
 */
HAL_Status_T HAL_TPM_OC_Start_DMA(TPM_Handle_T *htpm, uint32_t channel, uint32_t *data, uint32_t length)
{
    /* Check the TPM handle */
    ASSERT_PARAM(htpm);
    /* Check the TPM instance */
    ASSERT_PARAM(IS_TPM_INSTANCE(htpm));
    /* Check DMA instance */
    ASSERT_PARAM(IS_DMA_INSTANCE(htpm->hdma[GET_DMA_ID(channel)]));
    /* Check channel is valid */
    ASSERT_PARAM(IS_TPM_CHANNEL(channel));
    /* Clear error code */
    CLEAR_ERROR_CODE(htpm);
    /* Check instance support OC or Timer */
    RET_FLAG_FALSE((channel_table[htpm->tpm_index][channel] & (CH_OC | CH_TIMER)), HAL_ERROR);
    /* Check channel request for DMA */
    RET_FLAG_FALSE((channel_table[htpm->tpm_index][channel] & CH_DMA), HAL_ERROR);
    /* Check channel mode */
    uint8_t channel_mode = GET_CH_MODE(htpm->channel_info.mode[channel / TPM_CHANNEL_8], channel);
    RET_FLAG_FALSE((channel_mode & (CH_OC | CH_TIMER)), HAL_ERROR);
    /* Check channel state */
    uint8_t channel_state = GET_CH_STATE(htpm->channel_info.state, channel);
    RET_FLAG_FALSE((HAL_FLAG_EQU(channel_state, CH_STATE_READY) || CH_IN_BREAK(htpm, channel)), HAL_ERROR);
    /* Protect the process */
    TAKE_MUTEX(htpm);
    /* Set channel dma request */
    HAL_REG_SBIT(*REG_DMA(htpm), 1 << GET_DMA_REQ(dma_table[htpm->tpm_index][GET_DMA_ID(channel)]));
    /* Set value of MR from data */
    HAL_Status_T status = _start_channel_dma(htpm, channel, data, length);
    if (status == HAL_OK)
    {
        /* Clear channel RIS */
        HAL_REG_SBIT(*REG_IC(htpm), 1 << _get_ch_ic_bit(htpm, channel));
        /* Enable output pin when channel is not timer */
        if (!HAL_FLAG_EQU(channel_mode, CH_TIMER))
        {
            HAL_REG_SBIT(*REG_PWM_IO_ENABLE(htpm), HAL_ENABLE << PWM_IO_EN_BIT(htpm, channel));
        }
        status = _start_channel_base(htpm, channel, HAL_DISABLE);
    }
    /* Release the process protection */
    GIVE_MUTEX(htpm);
    return status;
}

/**
 * @brief  Stop OC in DMA mode
 * @param  htpm Pointer to a TPM_Handle_T structure that contains the configuration information for the specified TPM.
 * @param  channel TPM channel
 * @retval HAL_Status_T
 */
HAL_Status_T HAL_TPM_OC_Stop_DMA(TPM_Handle_T *htpm, uint32_t channel)
{
    /* Check the TPM handle */
    ASSERT_PARAM(htpm);
    /* Check the TPM instance */
    ASSERT_PARAM(IS_TPM_INSTANCE(htpm));
    /* Check DMA instance */
    ASSERT_PARAM(IS_DMA_INSTANCE(htpm->hdma[GET_DMA_ID(channel)]));
    /* Clear error code */
    CLEAR_ERROR_CODE(htpm);
    /* Check instance support OC or Timer */
    RET_FLAG_FALSE((channel_table[htpm->tpm_index][channel] & (CH_OC | CH_TIMER)), HAL_ERROR);
    /* Check channel request for DMA */
    RET_FLAG_FALSE((channel_table[htpm->tpm_index][channel] & CH_DMA), HAL_ERROR);
    /* Check channel mode */
    uint8_t channel_mode = GET_CH_MODE(htpm->channel_info.mode[channel / TPM_CHANNEL_8], channel);
    RET_FLAG_FALSE((HAL_FLAG_EQU(channel_mode, CH_TIMER)) || (HAL_FLAG_EQU(channel_mode, CH_OC)), HAL_ERROR);
    /* Check channel state */
    uint8_t channel_state = GET_CH_STATE(htpm->channel_info.state, channel);
    RET_FLAG_FALSE(HAL_FLAG_EQU(channel_state, CH_STATE_BUSY), HAL_ERROR);
    /* Protect the process */
    TAKE_MUTEX(htpm);
    /* Clear channel dma request */
    HAL_REG_CBIT(*REG_DMA(htpm), 1 << GET_DMA_REQ(dma_table[htpm->tpm_index][GET_DMA_ID(channel)]));

    HAL_Status_T status = _stop_channel_dma(htpm, channel);
    if (status == HAL_OK)
    {
        /* Disable output pin */
        if (!HAL_FLAG_EQU(channel_mode, CH_TIMER))
        {
            HAL_REG_CBIT(*REG_PWM_IO_ENABLE(htpm), 1 << PWM_IO_EN_BIT(htpm, channel));
        }

        status = _stop_channel_base(htpm, channel);
    }
    /* Release the process protection */
    GIVE_MUTEX(htpm);
    return status;
}

#endif /* configUSE_TPM_DMA */

#endif /* configUSE_TPM_OC */

/* Input Capture functions **********************************************/
#if (configUSE_TPM_CAP == 1)
/**
 * @brief  Capture config
 * @param  htpm Pointer to a TPM_Handle_T structure that contains the configuration information for the specified TPM.
 * @param  config capture config structural
 * @retval HAL_Status_T
 */
HAL_Status_T HAL_TPM_CAP_Config(TPM_Handle_T *htpm, TPM_Capture_T *config)
{
    /* Check the TPM handle */
    ASSERT_PARAM(htpm);
    /* Check the TPM instance */
    ASSERT_PARAM(IS_TPM_INSTANCE(htpm));
    /* Check the TPM config */
    ASSERT_PARAM(IS_TPM_CAP(config));
    /* Clear error code */
    CLEAR_ERROR_CODE(htpm);
    /* Check instance support Capture */
    RET_FLAG_FALSE((function_table[htpm->tpm_index] & TPM_CAPTURE0), HAL_ERROR);
    /* Check TPM CTM is not counter mode */
    RET_FLAG_FALSE(HAL_FLAG_EQU(htpm->init.mode, 0), HAL_ERROR);
    /* Check the TPM state */
    RET_FLAG_TRUE(HAL_FLAG_EQU(htpm->state, HAL_STATE_RESET), HAL_ERROR);
    /* Check the TPM capture state */
    RET_FLAG_TRUE(HAL_FLAG_EQU(htpm->cap_state, HAL_STATE_BUSY), HAL_ERROR);
    /* Protect the process */
    TAKE_MUTEX(htpm);
    /* Capture/Reset on CT16Bn_CAP0 capture output signal falling edge */
    HAL_REG_SBIT(*REG_CAP_CONTROL(htpm), config->falling << 1);
    /* Capture/Reset on CT16Bn_CAP0 capture output signal rising edge */
    HAL_REG_SBIT(*REG_CAP_CONTROL(htpm), config->rising);
    /* Set CAP ready state */
    HAL_FLAG_WRITE(htpm->cap_state, HAL_STATE_READY);
    /* Release the process protection */
    GIVE_MUTEX(htpm);
    return HAL_OK;
}

/**
 * @brief  Start Capture in polling mode
 * @param  htpm Pointer to a TPM_Handle_T structure that contains the configuration information for the specified TPM.
 * @retval HAL_Status_T
 */
HAL_Status_T HAL_TPM_CAP_Start(TPM_Handle_T *htpm)
{
    /* Protect the process */
    TAKE_MUTEX(htpm);
    /* Clear error code */
    CLEAR_ERROR_CODE(htpm);
    HAL_Status_T status = _start_capture(htpm, HAL_DISABLE);
    /* Release the process protection */
    GIVE_MUTEX(htpm);
    return status;
}

/**
 * @brief  Stop Capture in polling mode
 * @param  htpm Pointer to a TPM_Handle_T structure that contains the configuration information for the specified TPM.
 * @retval HAL_Status_T
 */
HAL_Status_T HAL_TPM_CAP_Stop(TPM_Handle_T *htpm)
{
    /* Protect the process */
    TAKE_MUTEX(htpm);
    /* Clear error code */
    CLEAR_ERROR_CODE(htpm);
    HAL_Status_T status = _stop_capture(htpm);
    /* Release the process protection */
    GIVE_MUTEX(htpm);
    return status;
}

/**
 * @brief  Start Capture in interrupt mode
 * @param  htpm Pointer to a TPM_Handle_T structure that contains the configuration information for the specified TPM.
 * @retval HAL_Status_T
 */
HAL_Status_T HAL_TPM_CAP_Start_IT(TPM_Handle_T *htpm)
{
    /* Protect the process */
    TAKE_MUTEX(htpm);
    /* Clear error code */
    CLEAR_ERROR_CODE(htpm);
    HAL_Status_T status = _start_capture(htpm, HAL_ENABLE);
    /* Release the process protection */
    GIVE_MUTEX(htpm);
    return status;
}

/**
 * @brief  Stop Capture in polling mode
 * @param  htpm Pointer to a TPM_Handle_T structure that contains the configuration information for the specified TPM.
 * @retval HAL_Status_T
 */
HAL_Status_T HAL_TPM_CAP_Stop_IT(TPM_Handle_T *htpm)
{
    /* Protect the process */
    TAKE_MUTEX(htpm);
    /* Clear error code */
    CLEAR_ERROR_CODE(htpm);
    HAL_Status_T status = _stop_capture(htpm);
    /* Release the process protection */
    GIVE_MUTEX(htpm);
    return status;
}

#if (configUSE_TPM_DMA == 1)
/**
 * @brief  Start Capture in DMA mode
 * @param  htpm Pointer to a TPM_Handle_T structure that contains the configuration information for the specified TPM.
 * @retval HAL_Status_T
 */
HAL_Status_T HAL_TPM_CAP_Start_DMA(TPM_Handle_T *htpm, uint32_t *data, uint16_t length)
{
    /* Check the TPM handle */
    ASSERT_PARAM(htpm);
    /* Check the TPM instance */
    ASSERT_PARAM(IS_TPM_INSTANCE(htpm));
    /* Clear error code */
    CLEAR_ERROR_CODE(htpm);
    /* Check instance support Capture */
    RET_FLAG_FALSE((function_table[htpm->tpm_index] & TPM_CAPTURE0), HAL_ERROR);
    /* Check the TPM capture state */
    RET_FLAG_FALSE(HAL_FLAG_EQU(htpm->cap_state, HAL_STATE_READY), HAL_ERROR);
    /* Check DMA instance */
    RET_FLAG_FALSE(htpm->hdma[TPM_DMA_ID_CAP], HAL_ERROR);
    /* Protect the process */
    TAKE_MUTEX(htpm);
    /* Set capture dma request */
    HAL_REG_SBIT(*REG_DMA(htpm), 1 << GET_DMA_REQ(dma_table[htpm->tpm_index][TPM_DMA_ID_CAP]));
    /* Set value from CAP0 to data */
    HAL_Status_T status = HAL_DMA_Start_IT(htpm->hdma[TPM_DMA_ID_CAP], (uint32_t)REG_CAP0(htpm), (uint32_t)data, length);
    if (status == HAL_OK)
    {
        /* clear capture RIS */
        HAL_REG_SBIT(*REG_IC(htpm), 1 << _get_ch_ic_bit(htpm, TPM_CHANNEL_CLEARED));
        status = _start_capture(htpm, HAL_DISABLE);
    }
    /* Release the process protection */
    GIVE_MUTEX(htpm);

    return status;
}

/**
 * @brief  Stop Capture in DMA mode
 * @param  htpm Pointer to a TPM_Handle_T structure that contains the configuration information for the specified TPM.
 * @retval HAL_Status_T
 */
HAL_Status_T HAL_TPM_CAP_Stop_DMA(TPM_Handle_T *htpm)
{
    /* Check the TPM handle */
    ASSERT_PARAM(htpm);
    /* Check the TPM instance */
    ASSERT_PARAM(IS_TPM_INSTANCE(htpm));
    /* Clear error code */
    CLEAR_ERROR_CODE(htpm);
    /* Check instance support Capture */
    RET_FLAG_FALSE((function_table[htpm->tpm_index] & TPM_CAPTURE0), HAL_ERROR);
    /* Check DMA instance */
    RET_FLAG_FALSE(htpm->hdma[TPM_DMA_ID_CAP], HAL_ERROR);
    /* Protect the process */
    TAKE_MUTEX(htpm);
    /* Clear capture dma request */
    HAL_REG_CBIT(*REG_DMA(htpm), 1 << GET_DMA_REQ(dma_table[htpm->tpm_index][TPM_DMA_ID_CAP]));
    /* Abort DMA */
    HAL_Status_T status = HAL_DMA_Abort_IT(htpm->hdma[TPM_DMA_ID_CAP]);
    if (status == HAL_OK)
    {
        status = _stop_capture(htpm);
    }
    /* Release the process protection */
    GIVE_MUTEX(htpm);
    return status;
}

#endif /* configUSE_TPM_DMA */

/**
 * @brief  Return the captured value.
 * @param  htpm Pointer to a TPM_Handle_T structure that contains the configuration information for the specified TPM.
 * @retval Captured value
 */
uint32_t HAL_TPM_ReadCapturedValue(TPM_Handle_T *htpm)
{
    /* Check the TPM handle */
    ASSERT_PARAM(htpm);
    /* Check the TPM instance */
    ASSERT_PARAM(IS_TPM_INSTANCE(htpm));
    /* Clear error code */
    CLEAR_ERROR_CODE(htpm);
    /* Check instance support capture , if not return 0 */
    RET_FLAG_FALSE((function_table[htpm->tpm_index] & TPM_CAPTURE0), 0);

    return *REG_CAP0(htpm);
}

#endif /* configUSE_TPM_CAP */

/* Peripheral IRQ functions  **************************************************/
/**
 * @brief  This function handles TPM interrupt request.
 * @param  htpm Pointer to a TPM_Handle_T structure that contains the configuration information for the specified TPM module.
 * @retval None
 */
__weak ATTR_ISR void HAL_TPM_IRQHandler(TPM_Handle_T *htpm)
{
    /* Clear error code */
    CLEAR_ERROR_CODE(htpm);
    tpm_irq[htpm->tpm_index](htpm);
}

/* Peripheral State and Errors functions **************************************/
/**
 * @brief  Return the TPM state.
 * @param  htpm Pointer to a TPM_Handle_T structure that contains the configuration information for the specified TPM.
 * @retval HAL_State_T
 */
HAL_State_T HAL_TPM_GetState(TPM_Handle_T *htpm)
{
    /* Check the TPM handle */
    ASSERT_PARAM(htpm);
    /* Clear error code */
    CLEAR_ERROR_CODE(htpm);
    return htpm->state;
}
/**
 * @brief  Return the Channel state of the TPM handle.
 * @param  htpm Pointer to a TPM_Handle_T structure that contains the configuration information for the specified TPM.
 * @retval HAL_State_T
 */
HAL_State_T HAL_TPM_GetChannelState(TPM_Handle_T *htpm, uint32_t channel)
{
    /* Check the TPM handle */
    ASSERT_PARAM(htpm);

    /* Check channel is valid */
    ASSERT_PARAM(IS_TPM_CHANNEL(channel));
    /* Clear error code */
    CLEAR_ERROR_CODE(htpm);

    HAL_State_T channel_state = (HAL_State_T)((((htpm->channel_info.state & (0x03 << (channel * 2))) >> (channel * 2)) & 0x3) << 4);
    return channel_state;
}
/**
 * @brief  Return TPM error code.
 * @param  htpm Pointer to a TPM_Handle_T structure that contains the configuration information for the specified TPM.
 * @retval Errro code
 */
uint32_t HAL_TPM_GetError(TPM_Handle_T *htpm)
{
    /* Check the TPM handle */
    ASSERT_PARAM(htpm);

    return htpm->error_code;
}

/**
 * @brief  Return TPM channel trigger interrupt.
 * @param  htpm Pointer to a TPM_Handle_T structure that contains the configuration information for the specified TPM.
 * @retval Channel
 */
uint32_t HAL_TPM_GetActiveChannel(TPM_Handle_T *htpm)
{
    /* Check the TPM handle */
    ASSERT_PARAM(htpm);
    /* Clear error code */
    CLEAR_ERROR_CODE(htpm);
    return htpm->active_channel;
}

/**
 * @brief  Set TPM index by instance.
 * @param  htpm Pointer to a TPM_Handle_T structure that contains the configuration information for the specified TPM.
 * @retval None
 */
static void _tpm_set_index(TPM_Handle_T *htpm)
{
    switch ((uint32_t)htpm->instance)
    {
        case SN_CT16B0_BASE:
            htpm->tpm_index = CT16B0;
            break;
        case SN_CT16B1_BASE:
            htpm->tpm_index = CT16B1;
            break;
        case SN_CT16B2_BASE:
            htpm->tpm_index = CT16B2;
            break;
        case SN_CT16B3_BASE:
            htpm->tpm_index = CT16B3;
            break;
        case SN_CT16B4_BASE:
            htpm->tpm_index = CT16B4;
            break;
        case SN_CT16B5_BASE:
            htpm->tpm_index = CT16B5;
            break;
        case SN_CT16B6_BASE:
            htpm->tpm_index = CT16B6;
            break;
        case SN_CT16B7_BASE:
            htpm->tpm_index = CT16B7;
            break;
        case SN_CT16B8_BASE:
            htpm->tpm_index = CT16B8;
            break;
        default:
            break;
    }
}

/**
 * @brief  Reset TPM state.
 * @param  htpm Pointer to a TPM_Handle_T structure that contains the configuration information for the specified TPM.
 * @retval None
 */
static void _reset_base(TPM_Handle_T *htpm)
{
    htpm->active_channel         = TPM_CHANNEL_CLEARED;
    htpm->cap_state              = HAL_STATE_RESET;
    htpm->channel_info.state     = 0;
    htpm->channel_info.mode[0]   = 0;
    htpm->channel_info.mode[1]   = 0;
    htpm->channel_info.pwmn_mode = 0;
    htpm->error_code             = HAL_OK;
    /* Update state to reset */
    HAL_FLAG_WRITE(htpm->state, HAL_STATE_RESET);
}

/**
 * @brief  Check TPM state whether busy.
 * @param  htpm Pointer to a TPM_Handle_T structure that contains the configuration information for the specified TPM.
 * @retval None
 */
static uint8_t _tpm_is_busy(TPM_Handle_T *htpm)
{
    if (HAL_FLAG_EQU(htpm->cap_state, HAL_STATE_BUSY))
    {
        return 1;
    }

    for (size_t i = 0; i <= TPM_CHANNEL_PERIOD; i++)
    {
        uint8_t channel_state = GET_CH_STATE(htpm->channel_info.state, i);
        if (HAL_FLAG_EQU(channel_state, CH_STATE_BUSY))
        {
            return 1;
        }
    }
    return 0;
}

/**
 * @brief  Disable TPM when all channel and cap is ready
 * @param  htpm Pointer to a TPM_Handle_T structure that contains the configuration information for the specified TPM.
 * @retval None
 */
static void _tpm_disable(TPM_Handle_T *htpm)
{
    /* Disable TC and PC counter. */
    HAL_REG_CBIT(*REG_TMRCTRL(htpm), 1);
    /* Clear TC and PC */
    HAL_REG_SBIT(*REG_TMRCTRL(htpm), 1 << 1);
    while (SN_CT16B0->TMRCTRL_b.CRST == 1)
        ;
    /* Update TPM state */
    HAL_FLAG_WRITE(htpm->state, HAL_STATE_READY);
}

/**
 * @brief  Set channel base config
 * @param  htpm Pointer to a TPM_Handle_T structure that contains the configuration information for the specified TPM.
 * @param  config OC config structural
 * @param  channel TPM channel
 * @retval None
 */
static void _config_channel_base(TPM_Handle_T *htpm, TPM_OC_T *config, uint32_t channel)
{
    /* Setting Match Value */
    HAL_REG_WRITE(*(REG_MR(htpm, channel)), config->period);
    /* Set Match reset and stop config */
    HAL_REG_SBIT(*(REG_MCTRL(htpm) + (channel / TPM_CHANNEL_10)), config->reset << (GET_CHANNEL_MCTRL(channel_table[htpm->tpm_index][channel]) + 1));
    HAL_REG_SBIT(*(REG_MCTRL(htpm) + (channel / TPM_CHANNEL_10)), config->stop << (GET_CHANNEL_MCTRL(channel_table[htpm->tpm_index][channel]) + 2));
    /* Set channel state */
    WRITE_CH_STATE(htpm->channel_info.state, channel, CH_STATE_READY);
}

/**
 * @brief  Start channel in base
 * @param  htpm Pointer to a TPM_Handle_T structure that contains the configuration information for the specified TPM.
 * @param  channel TPM channel
 * @param  ie Interrupt flag
 * @retval HAL_Status_T
 */
static HAL_Status_T _start_channel_base(TPM_Handle_T *htpm, uint32_t channel, uint8_t ie)
{
    /* Set TPM interrupt flag */
    HAL_REG_SBIT(*(REG_MCTRL(htpm) + (channel / TPM_CHANNEL_10)), ie << (GET_CHANNEL_MCTRL(channel_table[htpm->tpm_index][channel])));

    /* Set TPM and channel busy */
    WRITE_CH_STATE(htpm->channel_info.state, channel, CH_STATE_BUSY);

    HAL_FLAG_WRITE(htpm->state, HAL_STATE_BUSY);
    if (!HAL_REG_RBIT(*REG_TMRCTRL(htpm), 1))
    {
        /* Enable TPM */
        HAL_REG_SBIT(*REG_TMRCTRL(htpm), 1);
    }
    return HAL_OK;
}

#if ((configUSE_TPM_PWM == 1) || (configUSE_TPM_OC == 1))
/**
 * @brief  Set channel PWM config
 * @param  htpm Pointer to a TPM_Handle_T structure that contains the configuration information for the specified TPM.
 * @param  config OC config structural
 * @param  channel TPM channel
 * @retval None
 */
static void _config_channel_pwm(TPM_Handle_T *htpm, TPM_OC_T *config, uint32_t channel)
{
    /* Output pin by PWM */
    HAL_REG_SBIT(*REG_PWM_ENABLE(htpm), 1 << channel);
    /* Set PWM out mode */
    HAL_REG_WBIT(*REG_PWMCTRL(htpm), 0x3 << PWM_EMC_MODE_BIT(htpm, channel), (config->mode - TPM_OCMODE_PWM1) << PWM_EMC_MODE_BIT(htpm, channel));
    /* Set channel base config */
    _config_channel_base(htpm, config, channel);
}

#endif
#if (configUSE_TPM_PWM == 1)
/**
 * @brief  Start channel in PWM
 * @param  htpm Pointer to a TPM_Handle_T structure that contains the configuration information for the specified TPM.
 * @param  channel TPM channel
 * @param  ie Interrupt flag
 * @retval HAL_Status_T
 */
static HAL_Status_T _start_channel_pwm(TPM_Handle_T *htpm, uint32_t channel, uint8_t ie)
{
    /* Check the TPM handle */
    ASSERT_PARAM(htpm);
    /* Check the TPM instance */
    ASSERT_PARAM(IS_TPM_INSTANCE(htpm));
    /* Clear error code */
    CLEAR_ERROR_CODE(htpm);
    /* Check instance support PWM */
    RET_FLAG_FALSE((channel_table[htpm->tpm_index][channel] & CH_PWM), HAL_ERROR);
    /* Check TPM state */
    RET_FLAG_TRUE(HAL_FLAG_EQU(htpm->state, HAL_STATE_RESET), HAL_ERROR);
    /* Check channel mode */
    uint8_t channel_mode = GET_CH_MODE(htpm->channel_info.mode[channel / TPM_CHANNEL_8], channel);
    RET_FLAG_FALSE(HAL_FLAG_EQU(channel_mode, CH_PWM), HAL_ERROR);
    /* Check channel state */
    uint8_t channel_state = GET_CH_STATE(htpm->channel_info.state, channel);
    RET_FLAG_FALSE((HAL_FLAG_EQU(channel_state, CH_STATE_READY) || CH_IN_BREAK(htpm, channel)), HAL_ERROR);
    /* Protect the process */
    TAKE_MUTEX(htpm);
    /* Enable output pin */
    HAL_REG_SBIT(*REG_PWM_IO_ENABLE(htpm), HAL_ENABLE << PWM_IO_EN_BIT(htpm, channel));
    /* Enable PWMN output if current channel support PWMN and config PWMN */
    if (channel_table[htpm->tpm_index][channel] & CH_PWMN)
    {
        uint8_t pwmn_mode = GET_CH_PWMN_MODE(htpm->channel_info.pwmn_mode, channel);
        HAL_REG_WBIT(*REG_PWMCTRL(htpm), 0x03 << (24 + 2 * channel), pwmn_mode << (24 + 2 * channel));
    }
    HAL_Status_T status = _start_channel_base(htpm, channel, ie);
    /* Release the process protection */
    GIVE_MUTEX(htpm);
    return status;
}

/**
 * @brief  Stop channel in PWM
 * @param  htpm Pointer to a TPM_Handle_T structure that contains the configuration information for the specified TPM.
 * @param  channel TPM channel
 * @retval HAL_Status_T
 */
static HAL_Status_T _stop_channel_pwm(TPM_Handle_T *htpm, uint32_t channel)
{
    /* Check the TPM handle */
    ASSERT_PARAM(htpm);
    /* Check the TPM instance */
    ASSERT_PARAM(IS_TPM_INSTANCE(htpm));
    /* Clear error code */
    CLEAR_ERROR_CODE(htpm);
    /* Check instance support PWM */
    RET_FLAG_FALSE((channel_table[htpm->tpm_index][channel] & CH_PWM), HAL_ERROR);
    /* Check channel mode */
    uint8_t channel_mode = GET_CH_MODE(htpm->channel_info.mode[channel / TPM_CHANNEL_8], channel);
    RET_FLAG_FALSE(HAL_FLAG_EQU(channel_mode, CH_PWM), HAL_ERROR);
    /* Check channel state */
    uint8_t channel_state = GET_CH_STATE(htpm->channel_info.state, channel);
    RET_FLAG_FALSE(HAL_FLAG_EQU(channel_state, CH_STATE_BUSY), HAL_ERROR);
    /* Protect the process */
    TAKE_MUTEX(htpm);
    /* Disable output pin */
    HAL_REG_CBIT(*REG_PWM_IO_ENABLE(htpm), 1 << PWM_IO_EN_BIT(htpm, channel));
    /* Disable PWMN output if current channel support PWMN and config PWMN */
    if (channel_table[htpm->tpm_index][channel] & CH_PWMN)
    {
        HAL_REG_CBIT(*REG_PWMCTRL(htpm), 0x03 << (24 + 2 * channel));
    }

    HAL_Status_T status = _stop_channel_base(htpm, channel);
    /* Release the process protection */
    GIVE_MUTEX(htpm);
    return status;
}

#endif

#if (configUSE_TPM_OC == 1)

/**
 * @brief  Set channel OC config
 * @param  htpm Pointer to a TPM_Handle_T structure that contains the configuration information for the specified TPM.
 * @param  config OC config structural
 * @param  channel TPM channel
 * @retval None
 */
static void _config_channel_oc(TPM_Handle_T *htpm, TPM_OC_T *config, uint32_t channel)
{
    switch (config->mode)
    {
        case TPM_OCMODE_TIMER:
            /* Set channel base config */
            _config_channel_base(htpm, config, channel);
            break;
        case TPM_OCMODE_LOW:
        case TPM_OCMODE_HIGH:
        case TPM_OCMODE_TOGGLE:
            /* Output pin by EMC */
            HAL_REG_SBIT(*REG_PWM_ENABLE(htpm), 0 << channel);
            /* Set output pin state during MR0 == TC*/
            HAL_REG_WBIT(*REG_EMC(htpm), 0x03 << PWM_EMC_MODE_BIT(htpm, channel), (config->mode - TPM_OCMODE_TIMER) << PWM_EMC_MODE_BIT(htpm, channel));
            /* Set output pin state during MR0 != TC*/
            HAL_REG_SBIT(*REG_EM(htpm), ((config->idle_state) << channel));
            /* Set channel base config */
            _config_channel_base(htpm, config, channel);
            break;
        case TPM_OCMODE_FORCED_LOW:
        case TPM_OCMODE_FORCED_HIGH:
            /* Set channel PWM config */
            _config_channel_pwm(htpm, config, channel);
            break;
        default:
            break;
    }
}

/**
 * @brief  Start channel in OC
 * @param  htpm Pointer to a TPM_Handle_T structure that contains the configuration information for the specified TPM.
 * @param  channel TPM channel
 * @param  ie Interrupt flag
 * @retval HAL_Status_T
 */
static HAL_Status_T _start_channel_oc(TPM_Handle_T *htpm, uint32_t channel, uint8_t ie)
{
    /* Check the TPM handle */
    ASSERT_PARAM(htpm);
    /* Check the TPM instance */
    ASSERT_PARAM(IS_TPM_INSTANCE(htpm));
    /* Check channel is valid */
    ASSERT_PARAM(IS_TPM_CHANNEL(channel));
    /* Clear error code */
    CLEAR_ERROR_CODE(htpm);
    /* Check instance support OC or Timer */
    RET_FLAG_FALSE((channel_table[htpm->tpm_index][channel] & (CH_OC | CH_TIMER)), HAL_ERROR);
    /* Check channel mode */
    uint8_t channel_mode = GET_CH_MODE(htpm->channel_info.mode[channel / TPM_CHANNEL_8], channel);
    RET_FLAG_FALSE((HAL_FLAG_EQU(channel_mode, CH_TIMER)) || (HAL_FLAG_EQU(channel_mode, CH_OC)), HAL_ERROR);
    /* Check channel state */
    uint8_t channel_state = GET_CH_STATE(htpm->channel_info.state, channel);
    RET_FLAG_FALSE((HAL_FLAG_EQU(channel_state, CH_STATE_READY) || CH_IN_BREAK(htpm, channel)), HAL_ERROR);
    /* Protect the process */
    TAKE_MUTEX(htpm);
    /* Enable output pin when channel is not timer */
    if (!HAL_FLAG_EQU(channel_mode, CH_TIMER))
    {
        HAL_REG_SBIT(*REG_PWM_IO_ENABLE(htpm), HAL_ENABLE << PWM_IO_EN_BIT(htpm, channel));
    }

    HAL_Status_T status = _start_channel_base(htpm, channel, ie);
    /* Release the process protection */
    GIVE_MUTEX(htpm);
    return status;
}

/**
 * @brief  Stop channel in OC
 * @param  htpm Pointer to a TPM_Handle_T structure that contains the configuration information for the specified TPM.
 * @param  channel TPM channel
 * @retval HAL_Status_T
 */
static HAL_Status_T _stop_channel_oc(TPM_Handle_T *htpm, uint32_t channel)
{
    /* Check the TPM handle */
    ASSERT_PARAM(htpm);
    /* Check the TPM instance */
    ASSERT_PARAM(IS_TPM_INSTANCE(htpm));
    /* Check channel is valid */
    ASSERT_PARAM(IS_TPM_CHANNEL(channel));
    /* Clear error code */
    CLEAR_ERROR_CODE(htpm);
    /* Check instance support OC or Timer */
    RET_FLAG_FALSE((channel_table[htpm->tpm_index][channel] & (CH_OC | CH_TIMER)), HAL_ERROR);
    /* Check channel mode */
    uint8_t channel_mode = GET_CH_MODE(htpm->channel_info.mode[channel / TPM_CHANNEL_8], channel);
    RET_FLAG_FALSE((HAL_FLAG_EQU(channel_mode, CH_TIMER)) || (HAL_FLAG_EQU(channel_mode, CH_OC)), HAL_ERROR);
    /* Check channel state */
    uint8_t channel_state = GET_CH_STATE(htpm->channel_info.state, channel);
    RET_FLAG_FALSE(HAL_FLAG_EQU(channel_state, CH_STATE_BUSY), HAL_ERROR);
    /* Protect the process */
    TAKE_MUTEX(htpm);
    /* Disable output pin */
    if (!HAL_FLAG_EQU(channel_mode, CH_TIMER))
    {
        HAL_REG_CBIT(*REG_PWM_IO_ENABLE(htpm), 1 << PWM_IO_EN_BIT(htpm, channel));
    }

    HAL_Status_T status = _stop_channel_base(htpm, channel);
    /* Release the process protection */
    GIVE_MUTEX(htpm);
    return status;
}
#endif
/**
 * @brief  Stop channel in base
 * @param  htpm Pointer to a TPM_Handle_T structure that contains the configuration information for the specified TPM.
 * @param  channel TPM channel
 * @retval HAL_Status_T
 */
static HAL_Status_T _stop_channel_base(TPM_Handle_T *htpm, uint32_t channel)
{
    /* Disable TPM interrupt flag */
    HAL_REG_CBIT(*(REG_MCTRL(htpm) + (channel / TPM_CHANNEL_10)), 1 << (GET_CHANNEL_MCTRL(channel_table[htpm->tpm_index][channel])));

    /* Set TPM channel ready */
    WRITE_CH_STATE(htpm->channel_info.state, channel, CH_STATE_READY);
    if (!_tpm_is_busy(htpm))
    {
        _tpm_disable(htpm);
    }
    return HAL_OK;
}

#if (configUSE_TPM_DMA == 1)
/**
 * @brief  Start channel in DMA
 * @param  htpm Pointer to a TPM_Handle_T structure that contains the configuration information for the specified TPM.
 * @param  channel TPM channel
 * @retval HAL_Status_T
 */
static HAL_Status_T _start_channel_dma(TPM_Handle_T *htpm, uint32_t channel, uint32_t *data, uint32_t length)
{
    /* Get TPM DMA handle index */
    uint8_t dma_index = GET_DMA_ID(channel);
    /* Transfer value from data to MRn */
    HAL_Status_T status = HAL_DMA_Start_IT(htpm->hdma[dma_index], (uint32_t)data, (uint32_t)(((uint8_t *)htpm->instance) + GET_DMA_ADDR(dma_table[htpm->tpm_index][dma_index])), length);
    return status;
}

/**
 * @brief  Stop channel in DMA
 * @param  htpm Pointer to a TPM_Handle_T structure that contains the configuration information for the specified TPM.
 * @param  channel TPM channel
 * @retval HAL_Status_T
 */
static HAL_Status_T _stop_channel_dma(TPM_Handle_T *htpm, uint32_t channel)
{
    HAL_Status_T statue = HAL_OK;
    /* Get TPM DMA handle index */
    uint8_t dma_index = GET_DMA_ID(channel);
    if (HAL_DMA_GetState(htpm->hdma[dma_index]) == HAL_STATE_BUSY)
    {
        /* DMA abort */
        statue = HAL_DMA_Abort_IT(htpm->hdma[dma_index]);
    }

    return statue;
}

#endif /* configUSE_TPM_DMA */

#if (configUSE_TPM_CAP == 1)
/**
 * @brief  Start Capture
 * @param  htpm Pointer to a TPM_Handle_T structure that contains the configuration information for the specified TPM.
 * @param  ie Capture interrupt flag
 * @retval HAL_Status_T
 */
static HAL_Status_T _start_capture(TPM_Handle_T *htpm, uint8_t ie)
{
    /* Check the TPM handle */
    ASSERT_PARAM(htpm);
    /* Check the TPM instance */
    ASSERT_PARAM(IS_TPM_INSTANCE(htpm));
    /* Check instance support Capture */
    RET_FLAG_FALSE((function_table[htpm->tpm_index] & TPM_CAPTURE0), HAL_ERROR);
    /* Check the TPM capture state */
    RET_FLAG_FALSE(HAL_FLAG_EQU(htpm->cap_state, HAL_STATE_READY), HAL_ERROR);
    /* Interrupt on CT16Bn_CAP0 capture output signal event: a CAP0 load due to a CT16Bn_CAP0 capture
     * output signal event will generate an interrupt. */
    HAL_REG_SBIT(*REG_CAP_CONTROL(htpm), ie << 2);
    /* Enable capture 0 function for external capture pin. HW switches I/O configuration directly */
    HAL_REG_SBIT(*REG_CAP_CONTROL(htpm), HAL_ENABLE << 3);
    /* Enable timer counter and pre-scale counter. */
    HAL_REG_SBIT(*REG_TMRCTRL(htpm), 1);
    /* Set CAP busy state */
    HAL_FLAG_WRITE(htpm->cap_state, HAL_STATE_BUSY);
    /* Update TPM state */
    HAL_FLAG_WRITE(htpm->state, HAL_STATE_BUSY);
    return HAL_OK;
}

/**
 * @brief  Stop Capture
 * @param  htpm Pointer to a TPM_Handle_T structure that contains the configuration information for the specified TPM.
 * @retval HAL_Status_T
 */
static HAL_Status_T _stop_capture(TPM_Handle_T *htpm)
{
    /* Check the TPM handle */
    ASSERT_PARAM(htpm);
    /* Check the TPM instance */
    ASSERT_PARAM(IS_TPM_INSTANCE(htpm));
    /* Check instance support Capture */
    RET_FLAG_FALSE((function_table[htpm->tpm_index] & TPM_CAPTURE0), HAL_ERROR);
    /* Disable capture 0 function for external capture pin. HW switches I/O configuration directly */
    HAL_REG_CBIT(*REG_CAP_CONTROL(htpm), 1 << 3);
    HAL_REG_CBIT(*REG_CAP_CONTROL(htpm), 1 << 2);
    /* Set CAP ready state */
    HAL_FLAG_WRITE(htpm->cap_state, HAL_STATE_READY);
    if (!_tpm_is_busy(htpm))
    {
        _tpm_disable(htpm);
    }
    return HAL_OK;
}
#endif /* configUSE_TPM_CAP */

/**
 * @brief  IRQ function kernel
 * @param  htpm Pointer to a TPM_Handle_T structure that contains the configuration information for the specified TPM.
 * @param  irq_table CT16Bn IRQ para array
 * @param  irq_size CT16Bn IRQ para  size
 * @retval None
 */
static void _tpm_irq_kernel(TPM_Handle_T *htpm, const uint32_t *irq_table, uint8_t irq_size)
{
    for (uint8_t i = 0; i < irq_size; i++)
    {
        uint32_t irq_value = irq_table[i];
        if (CHECK_IRQ_IE(htpm, irq_value) && CHECK_IRQ_IF(htpm, irq_value))
        {
            uint8_t func_id      = GET_IRQ_FUNC_ID(irq_value);
            uint8_t ic_bit       = GET_IRQ_RIS_IC_BIT(irq_value);
            htpm->active_channel = GET_IRQ_CH(irq_value);
            func_irq[func_id](htpm, ic_bit);
        }
    }
}

/**
 * @brief  IRQ function for CT16B0/CT16B1/CT16B2/CT16B5
 * @param  htpm Pointer to a TPM_Handle_T structure that contains the configuration information for the specified TPM.
 * @retval None
 */
static void _tpm_0_1_2_5_irq(TPM_Handle_T *htpm)
{
    _tpm_irq_kernel(htpm, tpm_0_1_2_5_irq_table, 7);
}

/**
 * @brief  IRQ function for CT16B3
 * @param  htpm Pointer to a TPM_Handle_T structure that contains the configuration information for the specified TPM.
 * @retval None
 */
static void _tpm_3_irq(TPM_Handle_T *htpm)
{
    _tpm_irq_kernel(htpm, _tpm_3_irq_table, 3);
}

/**
 * @brief  IRQ function for CT16B4
 * @param  htpm Pointer to a TPM_Handle_T structure that contains the configuration information for the specified TPM.
 * @retval None
 */
static void _tpm_4_irq(TPM_Handle_T *htpm)
{
    _tpm_irq_kernel(htpm, _tpm_4_irq_table, 4);
}

/**
 * @brief  IRQ function for CT16B6/CT16B7
 * @param  htpm Pointer to a TPM_Handle_T structure that contains the configuration information for the specified TPM.
 * @retval None
 */
static void _tpm_6_7_irq(TPM_Handle_T *htpm)
{
    if (HAL_REG_RBIT(*REG_MCTRL(htpm), 1 << (3 * 0)) && HAL_REG_RBIT(*REG_RIS(htpm), 1U << 0))
    {
        htpm->active_channel = TPM_CHANNEL_0;
        _mr_irq(htpm, 0);
    }
}

/**
 * @brief  IRQ function for CT16B8
 * @param  htpm Pointer to a TPM_Handle_T structure that contains the configuration information for the specified TPM.
 * @retval None
 */
static void _tpm_8_irq(TPM_Handle_T *htpm)
{
    _tpm_irq_kernel(htpm, tpm_8_irq_table, 13);
}

/**
 * @brief  Trigger interrupt when capture load
 * @param  htpm Pointer to a TPM_Handle_T structure that contains the configuration information for the specified TPM.
 * @param  ic_bit TPM interrupt clear bit
 * @retval None
 */
static void _cap_irq(TPM_Handle_T *htpm, uint8_t ic_bit)
{
#if (configUSE_TPM_REGISTER_CALLBACKS == 1)
    /* call capture callback */
    if (htpm->CaptureCallback != NULL)
    {
        (htpm->CaptureCallback)(htpm);
    }
#else
    HAL_TPM_CaptureCallback(htpm);
#endif
    /* clear compare channel status */
    HAL_REG_SBIT(*REG_IC(htpm), 1 << ic_bit);
}

/**
 * @brief  Trigger interrupt when MRn == TC
 * @param  htpm Pointer to a TPM_Handle_T structure that contains the configuration information for the specified TPM.
 * @param  ic_bit TPM interrupt clear bit
 * @retval None
 */
static void _mr_irq(TPM_Handle_T *htpm, uint8_t ic_bit)
{
    uint8_t channel_mode = GET_CH_MODE(htpm->channel_info.mode[htpm->active_channel / TPM_CHANNEL_8], htpm->active_channel);
#if (configUSE_TPM_REGISTER_CALLBACKS == 1)
    /* Call output compare delay elapsed callback */
    if (htpm->OC_DelayElapsedCallback != NULL)
    {
        (htpm->OC_DelayElapsedCallback)(htpm);
    }

    if (HAL_FLAG_EQU(channel_mode, CH_PWM))
    {
        /* call PWM pulse finished callback */
        if (htpm->PWM_PulseFinishedCallback != NULL)
        {
            (htpm->PWM_PulseFinishedCallback)(htpm);
        }
    }
#else
    HAL_TPM_OC_DelayElapsedCallback(htpm);
    if (HAL_FLAG_EQU(channel_mode, CH_PWM))
    {
        HAL_TPM_PWM_PulseFinishedCallback(htpm);
    }
#endif
    /* clear compare channel status */
    HAL_REG_SBIT(*REG_IC(htpm), 1 << ic_bit);
    /* clear active channel */
    htpm->active_channel = TPM_CHANNEL_CLEARED;
}

/**
 * @brief  Trigger interrupt when break condition occurs
 * @param  htpm Pointer to a TPM_Handle_T structure that contains the configuration information for the specified TPM.
 * @param  ic_bit TPM interrupt clear bit
 * @retval None
 */
static void _break_irq(TPM_Handle_T *htpm, uint8_t ic_bit)
{
#if (configUSE_TPM_REGISTER_CALLBACKS == 1)
    /* call capture callback */
    if (htpm->BreakCallback != NULL)
    {
        (htpm->BreakCallback)(htpm);
    }
#else
    HAL_TPM_BreakCallback(htpm);
#endif
    /* clear break function status */
    HAL_REG_SBIT(*REG_IC(htpm), 1U << ic_bit);
}

#if (configUSE_TPM_DMA == 1)
/**
 * @brief  Get channel ic bit offset
 * @param  htpm Pointer to a TPM_Handle_T structure that contains the configuration information for the specified TPM.
 * @param  channel TPM chanel
 * @retval channel ic bit offset
 */
static uint32_t _get_ch_ic_bit(TPM_Handle_T *htpm, uint8_t channel)
{
    switch (channel)
    {
        case TPM_CHANNEL_9:
            switch (htpm->tpm_index)
            {
                case CT16B3:
                case CT16B4:
                    return 3;

                default:
                    return 5;
            }
        case TPM_CHANNEL_PERIOD:
        {
            return 13;
        }

        case TPM_CHANNEL_CLEARED:
        {
            switch (htpm->tpm_index)
            {
                case CT16B4:
                    return 2;

                default:
                    return 4;
            }
        }
        default:
            break;
    }
    return channel;
}
#endif

#if (configUSE_TPM_REGISTER_CALLBACKS == 1)
/**
 * @brief  Reset TPM all callback
 * @param  htpm Pointer to a TPM_Handle_T structure that contains the configuration information for the specified TPM module.
 * @retval None
 */
static void _reset_callback(TPM_Handle_T *htpm)
{
    htpm->MspInitCallback           = HAL_TPM_MspInit;
    htpm->MspDeInitCallback         = HAL_TPM_MspDeInit;
    htpm->OC_DelayElapsedCallback   = HAL_TPM_OC_DelayElapsedCallback;
    htpm->PWM_PulseFinishedCallback = HAL_TPM_PWM_PulseFinishedCallback;
    htpm->BreakCallback             = HAL_TPM_BreakCallback;
    htpm->CaptureCallback           = HAL_TPM_CaptureCallback;
    htpm->ErrorCallback             = HAL_TPM_ErrorCallback;
}
#endif /* configUSE_TPM_REGISTER_CALLBACKS */

#if (configUSE_TPM_DMA == 1)
/**
 * @brief  Register DMA callback
 * @param  htpm Pointer to a TPM_Handle_T structure that contains the configuration information for the specified TPM.
 * @retval None
 */
static void _register_dma_callback(TPM_Handle_T *htpm)
{
    for (uint8_t i = 0; i <= TPM_DMA_ID_CAP; i++)
    {
        if (htpm->hdma[i])
        {
            htpm->hdma[i]->parent = (void *)htpm;
            if (i == TPM_DMA_ID_CAP)
            {
                HAL_DMA_RegisterCallback(htpm->hdma[i], HAL_DMA_XFER_CPLT_CB_ID, _tpm_dma_xfer_capture_callback);
            }
            else
            {
                HAL_DMA_RegisterCallback(htpm->hdma[i], HAL_DMA_XFER_CPLT_CB_ID, _tpm_dma_xfer_delay_pulse_callback);
            }
            HAL_DMA_RegisterCallback(htpm->hdma[i], HAL_DMA_XFER_ERROR_CB_ID, _tpm_dma_xfer_error_callback);
        }
    }
}

/**
 * @brief  DMA transfer completed callback for MRn
 * @param  hdma Pointer to a DMA_Handle_T structure that contains the configuration information for the specified DMA.
 * @retval None
 */
static void _tpm_dma_xfer_delay_pulse_callback(DMA_Handle_T *hdma)
{
    TPM_Handle_T *htpm = (TPM_Handle_T *)hdma->parent;
    if (hdma == htpm->hdma[TPM_DMA_ID_MR_0])
    {
        htpm->active_channel = TPM_CHANNEL_0;
    }
    else if (hdma == htpm->hdma[TPM_DMA_ID_MR_1])
    {
        htpm->active_channel = TPM_CHANNEL_1;
    }
    else if (hdma == htpm->hdma[TPM_DMA_ID_MR_2])
    {
        htpm->active_channel = TPM_CHANNEL_2;
    }
    else if (hdma == htpm->hdma[TPM_DMA_ID_MR_3])
    {
        htpm->active_channel = TPM_CHANNEL_3;
    }
    else if (hdma == htpm->hdma[TPM_DMA_ID_MR_9])
    {
        htpm->active_channel = TPM_CHANNEL_9;
    }
    else if (hdma == htpm->hdma[TPM_DMA_ID_MR_PERIOD])
    {
        htpm->active_channel = TPM_CHANNEL_PERIOD;
    }

#if (configUSE_TPM_REGISTER_CALLBACKS == 1)
    if (htpm->PWM_PulseFinishedCallback != NULL)
    {
        (htpm->PWM_PulseFinishedCallback)(htpm);
    }
#else
    HAL_TPM_PWM_PulseFinishedCallback(htpm);
#endif
    htpm->active_channel = TPM_CHANNEL_CLEARED;
}
/**
 * @brief  DMA transfer completed callback for capture
 * @param  hdma Pointer to a DMA_Handle_T structure that contains the configuration information for the specified DMA.
 * @retval None
 */
static void _tpm_dma_xfer_capture_callback(DMA_Handle_T *hdma)
{
    TPM_Handle_T *htpm = (TPM_Handle_T *)hdma->parent;
#if (configUSE_TPM_CAP == 1)
    /* Stop capture */
    _stop_capture(htpm);
#endif
#if (configUSE_TPM_REGISTER_CALLBACKS == 1)
    if (htpm->CaptureCallback != NULL)
    {
        (htpm->CaptureCallback)(htpm);
    }
#else
    HAL_TPM_CaptureCallback(htpm);
#endif
}

/**
 * @brief  DMA transfer error callback
 * @param  hdma Pointer to a DMA_Handle_T structure that contains the configuration information for the specified DMA.
 * @retval None
 */
static void _tpm_dma_xfer_error_callback(DMA_Handle_T *hdma)
{
    TPM_Handle_T *htpm = (TPM_Handle_T *)hdma->parent;
    HAL_FLAG_WRITE(htpm->state, HAL_STATE_READY);
    htpm->error_code = HAL_DMA_GetError(hdma);
#if (configUSE_TPM_REGISTER_CALLBACKS == 1)
    if (htpm->ErrorCallback != NULL)
    {
        (htpm->ErrorCallback)(htpm);
    }
#else
    HAL_TPM_ErrorCallback(htpm);
#endif
}
#endif /* configUSE_TPM_DMA */

#endif /* configUSE_TPM_PERIPHERAL */
