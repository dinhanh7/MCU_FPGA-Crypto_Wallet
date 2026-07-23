/***********************
 * SN34F78X SDK V2.1.4 *
 ***********************/

#include "sn34f78x_hal_can.h"

#if (configUSE_CAN_PERIPHERAL == 1U)

/**
 * @brief  Initializes the CAN peripheral according to the specified
 *         parameters in the CAN_Init_T.
 * @param  hcan pointer to a CAN_Handle_T structure that contains
 *         the configuration information for the specified CAN.
 * @return HAL_Status_T
 */
HAL_Status_T HAL_CAN_Init(CAN_Handle_T *hcan)
{
    /* Check the parameters */
    ASSERT_PARAM(hcan);
    ASSERT_PARAM(IS_CAN_INSTANCE(hcan));
    ASSERT_PARAM(IS_CAN_TIMING_CFG(hcan->init.pre_scaler, hcan->init.time_seg1, hcan->init.time_seg2, hcan->init.sync_jump_width));

    /* reset can system control */
    HAL_REG_WRITE(hcan->instance->SYSCTRL_b.RESET, 1);

    if (HAL_FLAG_HAS(hcan->state, HAL_STATE_RESET))
    {
        /* Allocate lock resource and initialize it */
        GIVE_MUTEX(hcan);

#if (configUSE_CAN_REGISTER_CALLBACKS == 1)
        /* Init default callback */
        _can_init_callbacks_to_default(hcan);

        /* Init the low level hardware : GPIO, CLOCK, NVIC */
        (hcan->MspInitCallback)(hcan);
        (hcan->MspInitFilterCallback)(hcan);
#else
        /* Init the low level hardware : GPIO, CLOCK, NVIC */
        HAL_CAN_MspInit(hcan);
        HAL_CAN_MspInitFilter(hcan);
#endif /* configUSE_CAN_REGISTER_CALLBACKS */
    }

    /* Set the CAN state */
    HAL_FLAG_SET(hcan->state, HAL_STATE_BUSY);

    /* Set the CAN configuration */
    HAL_FLAG_WRITE(hcan->error_code, _can_set_config(hcan));
    RET_FLAG_FALSE(hcan->error_code == HAL_OK, HAL_ERROR);

    /* Change CAN state to READY */
    HAL_FLAG_WRITE(hcan->state, HAL_STATE_READY);

    return HAL_OK;
}

/**
 * @brief  De-initializes the CAN peripheral registers to their default
 *         reset values.
 * @param  hcan pointer to a CAN_Handle_T structure that contains
 *         the configuration information for the specified CAN.
 * @retval HAL status
 */
HAL_Status_T HAL_CAN_DeInit(CAN_Handle_T *hcan)
{
    /* Check the CAN handle allocation */
    ASSERT_PARAM(hcan);

    /* Check the parameters */
    ASSERT_PARAM(IS_CAN_INSTANCE(hcan));

    /* Change CAN state to BUSY */
    HAL_FLAG_SET(hcan->state, HAL_STATE_BUSY);

    // CAN_ENABLE_CLK(hcan);

    /* pin control */
    HAL_REG_WRITE(hcan->instance->SYSCTRL_b.RESET, 1);
    HAL_REG_WRITE(hcan->instance->PINCTRL_b.STBYEN, 0);
    HAL_REG_WRITE(hcan->instance->PINCTRL_b.RXEN, 0);
    HAL_REG_WRITE(hcan->instance->PINCTRL_b.TXEN, 0);
    HAL_REG_WRITE(hcan->instance->SYSCTRL_b.RESET, 0);

#if (configUSE_CAN_REGISTER_CALLBACKS == 1U)
    /* Init default callback */
    CAN_DEFAULT_CB(hcan->MspDeInitCallback, HAL_CAN_MspDeInit);

    /* DeInit the low level hardware: GPIO, CLOCK, NVIC */
    (hcan->MspDeInitCallback)(hcan);
#else
    /* DeInit the low level hardware: GPIO, CLOCK, NVIC */
    HAL_CAN_MspDeInit(hcan);
#endif /* configUSE_CAN_REGISTER_CALLBACKS */

    /* Release Mutex */
    GIVE_MUTEX(hcan);

    /* Change CAN state to RESET */
    HAL_FLAG_WRITE(hcan->state, HAL_STATE_RESET);

    return HAL_OK;
}

/**
 * @brief  Initializes the CAN Filter.
 * @param  hcan pointer to a CAN_Handle_T structure that contains
 *         the configuration information for the specified CAN.
 * @retval None
 */
__weak void HAL_CAN_MspInitFilter(CAN_Handle_T *hcan)
{
    /* Prevent unused argument(s) compilation warning */
    UNUSED(hcan);

    /* NOTE : This function Should not be modified, when the callback is needed,
              the HAL_CAN_MspInitFilter could be implemented in the user file
     */
}

/**
 * @brief  Initializes the CAN MSP.
 * @param  hcan pointer to a CAN_Handle_T structure that contains
 *         the configuration information for the specified CAN.
 * @retval None
 */
__weak void HAL_CAN_MspInit(CAN_Handle_T *hcan)
{
    /* Prevent unused argument(s) compilation warning */
    UNUSED(hcan);

    /* NOTE : This function Should not be modified, when the callback is needed,
              the HAL_CAN_MspInit could be implemented in the user file
     */
}

/**
 * @brief  DeInitializes the CAN MSP.
 * @param  hcan pointer to a CAN_Handle_T structure that contains
 *         the configuration information for the specified CAN.
 * @retval None
 */
__weak void HAL_CAN_MspDeInit(CAN_Handle_T *hcan)
{
    /* Prevent unused argument(s) compilation warning */
    UNUSED(hcan);

    /* NOTE : This function Should not be modified, when the callback is needed,
              the HAL_CAN_MspDeInit could be implemented in the user file
     */
}

/**
 * @brief  can set config.
 * @param  hcan pointer to a CAN_Handle_T structure that contains
 *         the configuration information for the specified CAN.
 * @retval result
 */
static uint32_t _can_set_config(CAN_Handle_T *hcan)
{
    /* pin setting: on reset=1 */
    HAL_REG_WRITE(hcan->instance->PINCTRL_b.TXEN, 1);
    HAL_REG_WRITE(hcan->instance->PINCTRL_b.RXEN, 1);
    HAL_REG_WRITE(hcan->instance->PINCTRL_b.STBYEN, 1);

    /* baudrate setting: on reset=1, eg:PCLK = 48M, CAN bit rate = (48M/((3+1)*((20+2)+(1+1)))) = 500K bit/s */
    HAL_REG_WRITE(hcan->instance->SBITCFG_b.S_PRESC, hcan->init.pre_scaler);    // 3
    HAL_REG_WRITE(hcan->instance->SBITCFG_b.S_SEG1, hcan->init.time_seg1);      // 20
    HAL_REG_WRITE(hcan->instance->SBITCFG_b.S_SEG2, hcan->init.time_seg2);      // 1
    HAL_REG_WRITE(hcan->instance->SBITCFG_b.S_SJW, hcan->init.sync_jump_width); // 0

    /* reset can system control */
    HAL_REG_WRITE(hcan->instance->SYSCTRL_b.RESET, 0);

    /* mode setting */
    HAL_REG_WRITE(hcan->instance->SYSCTRL_b.LOM, 0);
    HAL_REG_WRITE(hcan->instance->SYSCTRL_b.LBMI, 0);
    HAL_REG_WRITE(hcan->instance->SYSCTRL_b.LBME, 0);
    HAL_REG_WRITE(hcan->instance->SYSCTRL_b.RBALL, 0);
    switch (hcan->init.mode)
    {
        case CAN_MODE_NORMAL:
            break;
        case CAN_MODE_SILENT:
            HAL_REG_WRITE(hcan->instance->SYSCTRL_b.LOM, 1);
            HAL_REG_WRITE(hcan->instance->SYSCTRL_b.RBALL, 1);
            break;
        case CAN_MODE_LOOPBACK_IN:
            HAL_REG_WRITE(hcan->instance->SYSCTRL_b.LBMI, 1);
            HAL_REG_WRITE(hcan->instance->SYSCTRL_b.RBALL, 1);
            break;
        case CAN_MODE_LOOPBACK_EX:
            HAL_REG_WRITE(hcan->instance->SYSCTRL_b.LBME, 1);
            HAL_REG_WRITE(hcan->instance->SYSCTRL_b.RBALL, 1);
            HAL_REG_WRITE(hcan->instance->SYSCTRL_b.SACK, hcan->init.self_ack);
            break;
        case CAN_MODE_SILENT_LOOPBACK_EX:
            HAL_REG_WRITE(hcan->instance->SYSCTRL_b.LOM, 1);
            HAL_REG_WRITE(hcan->instance->SYSCTRL_b.LBME, 1);
            HAL_REG_WRITE(hcan->instance->SYSCTRL_b.RBALL, 1);
            HAL_REG_WRITE(hcan->instance->SYSCTRL_b.SACK, hcan->init.self_ack);
            break;
        default:
            return HAL_ERROR;
    }

    /* TX Transmission: STB */
    HAL_REG_WRITE(hcan->instance->SYSCTRL_b.TSMODE, hcan->init.transmit_fifo_priority);
    HAL_REG_WRITE(hcan->instance->SYSCTRL_b.TSSS, hcan->init.auto_retransmission);

    /* TX Transmission: PTB */
    HAL_REG_WRITE(hcan->instance->SYSCTRL_b.TPSS, hcan->init.auto_retransmission);

    /* rx overflow mode setting */
    HAL_REG_WRITE(hcan->instance->SYSCTRL_b.ROM, hcan->init.rx_overflow_mode);

    /* TTCAN transmit buffer mode */
    // HAL_REG_WRITE(hcan->instance->SYSCTRL_b.TTTBM, hcan->init.full_ttcan_support);

    /* RX fifo level setting */
    HAL_REG_WRITE(hcan->instance->INTCFG_b.AFWL, CAN_RX_ALMOST_THD);
    HAL_REG_WRITE(hcan->instance->INTCFG_b.EWL, CAN_EW_LIMIT);
    hcan->notification = (CAN_RX_ALMOST_THD << CAN_IE_AFWL_POS) |
                         (CAN_EW_LIMIT << CAN_IE_EWL_POS) |
                         CAN_IE_INIT;

    return HAL_OK;
}

#if (configUSE_CAN_REGISTER_CALLBACKS == 1U)
/**
 * @brief  can initlize default callback.
 * @param  hcan pointer to a CAN_Handle_T structure that contains
 *         the configuration information for the specified CAN.
 * @retval result
 */
static void _can_init_callbacks_to_default(CAN_Handle_T *hcan)
{
    CAN_DEFAULT_CB(hcan->MspInitCallback, HAL_CAN_MspInit);
    CAN_DEFAULT_CB(hcan->MspInitFilterCallback, HAL_CAN_MspInitFilter);
    CAN_DEFAULT_CB(hcan->TxPTBCompleteCallback, HAL_CAN_TxPTBCompleteCallback);
    CAN_DEFAULT_CB(hcan->TxSTBCompleteCallback, HAL_CAN_TxSTBCompleteCallback);
    CAN_DEFAULT_CB(hcan->TxAbortCallback, HAL_CAN_TxAbortCallback);
    CAN_DEFAULT_CB(hcan->RxFifoOneFullCallback, HAL_CAN_RxFifoOneFullCallback);
    CAN_DEFAULT_CB(hcan->RxFifoHalfFullCallback, HAL_CAN_RxFifoHalfFullCallback);
    CAN_DEFAULT_CB(hcan->RxFifoAllFullCallback, HAL_CAN_RxFifoAllFullCallback);
    CAN_DEFAULT_CB(hcan->ErrorCallback, HAL_CAN_ErrorCallback);
}

/**
 * @brief  Register a CAN CallBack.
 *         To be used instead of the weak predefined callback
 * @param  hcan pointer to a CAN_Handle_T structure that contains
 *         the configuration information for CAN module
 * @param  cb_id CallbackID ID of the callback to be registered
 *         This parameter can be one of the following values:
 *           @arg @ref HAL_CAN_TX_STB_FULL_CB_ID Tx Mailbox 0 Complete callback ID
 *           @arg @ref HAL_CAN_TX_ABORT_CB_ID Tx Mailbox 0 Abort callback ID
 *           @arg @ref HAL_CAN_RX_FIFO_FULL_CB_ID Rx Fifo 0 full callback ID
 *           @arg @ref HAL_CAN_SLEEP_CB_ID Sleep callback ID
 *           @arg @ref HAL_CAN_WAKEUP_FROM_RX_MSG_CB_ID Wake Up from Rx message callback ID
 *           @arg @ref HAL_CAN_ERROR_CB_ID Error callback ID
 *           @arg @ref HAL_CAN_MSP_INIT_CB_ID MspInit callback ID
 *           @arg @ref HAL_CAN_MSP_DEINIT_CB_ID MspDeInit callback ID
 * @param  pcb pointer to the Callback function
 * @return HAL_Status_T
 */
HAL_Status_T HAL_CAN_RegisterCallback(CAN_Handle_T *hcan, CAN_CallbackID_T cb_id, CAN_Callback_T pcb)
{
    HAL_Status_T status = HAL_OK;

    if (pcb == NULL)
    {
        /* Update the error code */
        hcan->error_code |= HAL_CAN_ERROR_INVALID_CALLBACK;
        return HAL_ERROR;
    }

    if (hcan->state == HAL_STATE_READY)
    {
        switch (cb_id)
        {
            case HAL_CAN_TX_PTB_CPLT_CB_ID:
                hcan->TxPTBCompleteCallback = pcb;
                break;
            case HAL_CAN_TX_STB_CPLT_CB_ID:
                hcan->TxSTBCompleteCallback = pcb;
                break;
            case HAL_CAN_TX_ABORT_CB_ID:
                hcan->TxAbortCallback = pcb;
                break;
            case HAL_CAN_RX_FIFO_HALF_CB_ID:
                hcan->RxFifoHalfFullCallback = pcb;
                break;
            case HAL_CAN_RX_FIFO_ONE_CB_ID:
                hcan->RxFifoOneFullCallback = pcb;
                break;
            case HAL_CAN_RX_FIFO_FULL_CB_ID:
                hcan->RxFifoAllFullCallback = pcb;
                break;
            case HAL_CAN_ERROR_CB_ID:
                hcan->ErrorCallback = pcb;
                break;
            default:
                /* Update the error code */
                hcan->error_code |= HAL_CAN_ERROR_INVALID_CALLBACK;
                /* Return error status */
                status = HAL_ERROR;
                break;
        }
    }
    else if (hcan->state == HAL_STATE_RESET)
    {
        switch (cb_id)
        {
            case HAL_CAN_MSP_INIT_FILTER_CB_ID:
                hcan->MspInitFilterCallback = pcb;
                break;
            case HAL_CAN_MSP_INIT_CB_ID:
                hcan->MspInitCallback = pcb;
                break;
            case HAL_CAN_MSP_DEINIT_CB_ID:
                hcan->MspDeInitCallback = pcb;
                break;
            default:
                /* Update the error code */
                hcan->error_code |= HAL_CAN_ERROR_INVALID_CALLBACK;
                /* Return error status */
                status = HAL_ERROR;
                break;
        }
    }
    else
    {
        /* Update the error code */
        hcan->error_code |= HAL_CAN_ERROR_INVALID_CALLBACK;
        /* Return error status */
        status = HAL_ERROR;
    }

    return status;
}

/**
 * @brief  Unregister a CAN CallBack.
 *         CAN callback is redirected to the weak predefined callback
 * @param  hcan pointer to a CAN_Handle_T structure that contains
 *         the configuration information for CAN module
 * @param  cb_id ID of the callback to be unregistered
 *         This parameter can be one of the following values:
 *           @arg @ref HAL_CAN_TX_STB_FULL_CB_ID Tx Mailbox 0 Complete callback ID
 *           @arg @ref HAL_CAN_TX_ABORT_CB_ID Tx Mailbox 0 Abort callback ID
 *           @arg @ref HAL_CAN_RX_FIFO_FULL_CB_ID Rx Fifo 0 full callback ID
 *           @arg @ref HAL_CAN_SLEEP_CB_ID Sleep callback ID
 *           @arg @ref HAL_CAN_WAKEUP_FROM_RX_MSG_CB_ID Wake Up from Rx message callback ID
 *           @arg @ref HAL_CAN_ERROR_CB_ID Error callback ID
 *           @arg @ref HAL_CAN_MSP_INIT_CB_ID MspInit callback ID
 *           @arg @ref HAL_CAN_MSP_DEINIT_CB_ID MspDeInit callback ID
 * @retval HAL status
 */
HAL_Status_T HAL_CAN_UnRegisterCallback(CAN_Handle_T *hcan, CAN_CallbackID_T cb_id)
{
    HAL_Status_T status = HAL_OK;

    if (hcan->state == HAL_STATE_READY)
    {
        switch (cb_id)
        {
            case HAL_CAN_TX_PTB_CPLT_CB_ID:
                hcan->TxPTBCompleteCallback = HAL_CAN_TxPTBCompleteCallback;
                break;
            case HAL_CAN_TX_STB_CPLT_CB_ID:
                hcan->TxSTBCompleteCallback = HAL_CAN_TxSTBCompleteCallback;
                break;
            case HAL_CAN_TX_ABORT_CB_ID:
                hcan->TxAbortCallback = HAL_CAN_TxAbortCallback;
                break;
            case HAL_CAN_RX_FIFO_ONE_CB_ID:
                hcan->RxFifoOneFullCallback = HAL_CAN_RxFifoOneFullCallback;
                break;
            case HAL_CAN_RX_FIFO_HALF_CB_ID:
                hcan->RxFifoHalfFullCallback = HAL_CAN_RxFifoHalfFullCallback;
                break;
            case HAL_CAN_RX_FIFO_FULL_CB_ID:
                hcan->RxFifoAllFullCallback = HAL_CAN_RxFifoAllFullCallback;
                break;
            case HAL_CAN_ERROR_CB_ID:
                hcan->ErrorCallback = HAL_CAN_ErrorCallback;
                break;
            case HAL_CAN_MSP_INIT_CB_ID:
                hcan->MspInitCallback = HAL_CAN_MspInit;
                break;
            case HAL_CAN_MSP_INIT_FILTER_CB_ID:
                hcan->MspInitFilterCallback = HAL_CAN_MspInitFilter;
                break;
            default:
                /* Update the error code */
                hcan->error_code |= HAL_CAN_ERROR_INVALID_CALLBACK;
                /* Return error status */
                status = HAL_ERROR;
                break;
        }
    }
    else if (hcan->state == HAL_STATE_RESET)
    {
        switch (cb_id)
        {
            case HAL_CAN_MSP_INIT_FILTER_CB_ID:
                hcan->MspInitFilterCallback = HAL_CAN_MspInitFilter;
                break;
            case HAL_CAN_MSP_INIT_CB_ID:
                hcan->MspInitCallback = HAL_CAN_MspInit;
                break;
            case HAL_CAN_MSP_DEINIT_CB_ID:
                hcan->MspDeInitCallback = HAL_CAN_MspDeInit;
                break;
            default:
                /* Update the error code */
                hcan->error_code |= HAL_CAN_ERROR_INVALID_CALLBACK;
                /* Return error status */
                status = HAL_ERROR;
                break;
        }
    }
    else
    {
        /* Update the error code */
        hcan->error_code |= HAL_CAN_ERROR_INVALID_CALLBACK;
        /* Return error status */
        status = HAL_ERROR;
    }

    return status;
}
#endif

/**
 * @brief  Configures the CAN reception filter according to the specified
 *         parameters in the CAN_Filter_T.
 * @param  hcan pointer to a CAN_Handle_T structure that contains
 *         the configuration information for the specified CAN.
 * @param  filter pointer to a CAN_Filter_T structure that
 *         contains the filter configuration information.
 * @return HAL_Status_T
 */
HAL_Status_T HAL_CAN_ConfigFilter(CAN_Handle_T *hcan, CAN_Filter_T *filter)
{
    /* check para */
    ASSERT_PARAM(hcan);
    ASSERT_PARAM(filter);
    ASSERT_PARAM(IS_CAN_FILLTER_SCALE(filter->filter_scale));

    RET_FLAG_FALSE(hcan->state == HAL_STATE_RESET, HAL_ERROR);

    /* index of be operated currently */
    if (filter->filter_activation)
    {
        /* filter index enable */
        HAL_FLAG_SET(hcan->instance->ACFCTRL, 1 << (filter->filter_index + 16));

        /* filter index address */
        HAL_REG_WRITE(hcan->instance->ACFCTRL_b.ACFADR, filter->filter_index);

        /* filter code setting */
        HAL_REG_WRITE(hcan->instance->ACFCTRL_b.SELMASK, 0);
        HAL_REG_WRITE(hcan->instance->ACFID_b.ACFBUF, filter->filter_code);

        /* filter mask setting */
        HAL_REG_WRITE(hcan->instance->ACFCTRL_b.SELMASK, 1);
        HAL_REG_WRITE(hcan->instance->ACFID_b.ACFBUF, filter->filter_mask);

        /* choice scale mode */
        switch (filter->filter_scale)
        {
            case CAN_FILTER_SCALE_BOTH:
                HAL_REG_WRITE(hcan->instance->ACFID_b.AIDEE, 0);
                break;
            case CAN_FILTER_SCALE_STD:
            case CAN_FILTER_SCALE_EXT:
                HAL_REG_WRITE(hcan->instance->ACFID_b.AIDEE, 1);
                HAL_REG_WRITE(hcan->instance->ACFID_b.AIDE, filter->filter_scale);
                break;
            default:
                return HAL_ERROR;
        }
    }
    else
    {
        /* filter index disable */
        HAL_FLAG_CLR(hcan->instance->ACFCTRL, 1 << (filter->filter_index + 16));
    }

    return HAL_OK;
}

/**
 * @brief  Start the CAN module.
 * @param  hcan pointer to an CAN_Handle_T structure that contains
 *         the configuration information for the specified CAN.
 * @retval HAL_Status_T
 */
HAL_Status_T HAL_CAN_Start(CAN_Handle_T *hcan)
{
    /* check para */
    ASSERT_PARAM(hcan);

    /* check state is ready */
    RET_FLAG_LACKS(hcan->state, HAL_STATE_READY, HAL_ERROR);

    /* Process locked */
    TAKE_MUTEX(hcan);

    // CAN_ENABLE_CLK(hcan);
    /* Clean Rx FIFO, make sure Rx FIFO is empty when re-start after stop */
    while (HAL_REG_READ(hcan->instance->SYSCTRL_b.RSTAT))
    {
        HAL_REG_WRITE(hcan->instance->SYSCTRL_b.RREL, 1);
    }

    /* Abort all mailbox */
    HAL_REG_WRITE(hcan->instance->SYSCTRL_b.TPA, 1);
    HAL_REG_WRITE(hcan->instance->SYSCTRL_b.TSA, 1);

    /* set busy state */
    HAL_FLAG_SET(hcan->state, HAL_STATE_BUSY);

    /* enable ie */
    CAN_ENABLE_IE(hcan, hcan->notification);

    /* Process unlocked */
    GIVE_MUTEX(hcan);

    return HAL_OK;
}

/**
 * @brief  Stop the CAN module and enable access to configuration registers.
 * @param  hcan pointer to an CAN_Handle_T structure that contains
 *         the configuration information for the specified CAN.
 * @return HAL_Status_T
 */
HAL_Status_T HAL_CAN_Stop(CAN_Handle_T *hcan)
{
    /* check para */
    ASSERT_PARAM(hcan);

    /* check state is ready */
    if (HAL_FLAG_LACKS(hcan->state, HAL_STATE_BUSY))
    {
        hcan->error_code = HAL_CAN_ERROR_NOT_STARTED;
        return HAL_ERROR;
    }

    /* Process locked */
    TAKE_MUTEX(hcan);

    /* Abort all mailbox */
    HAL_REG_WRITE(hcan->instance->SYSCTRL_b.TPA, 1);
    HAL_REG_WRITE(hcan->instance->SYSCTRL_b.TSA, 1);

    // CAN_DISABLE_CLK(hcan);

    /* disable ie */
    CAN_DISABLE_IE(hcan, hcan->notification);

    /* clear busy state */
    HAL_FLAG_CLR(hcan->state, HAL_STATE_BUSY);

    /* Process unlocked */
    GIVE_MUTEX(hcan);

    return HAL_OK;
}

/**
 * @brief  Request the sleep mode (low power) entry.
 *         When returning from this function, Sleep mode will be entered
 *         as soon as the current CAN activity (transmission or reception
 *         of a CAN frame) has been completed.
 * @param  hcan pointer to an CAN_Handle_T structure that contains
 *         the configuration information for the specified CAN.
 * @return HAL_Status_T
 */
HAL_Status_T HAL_CAN_RequestSleep(CAN_Handle_T *hcan)
{
    /* check para */
    ASSERT_PARAM(hcan);

    /* check can has been sleep mode */
    if (CAN_STATE_EX_HAS(hcan, CAN_STATE_EX_SLEEP))
    {
        return HAL_OK;
    }

    /* check can is initialized */
    if (HAL_FLAG_LACKS(hcan->state, HAL_STATE_READY))
    {
        hcan->error_code = HAL_CAN_ERROR_NOT_INITIALIZED;
        return HAL_ERROR;
    } /* check can is tx busy */
    else if ((HAL_REG_READ(hcan->instance->SYSCTRL_b.TPE) == 1) ||
             (HAL_REG_READ(hcan->instance->SYSCTRL_b.TSONE) == 1) ||
             (HAL_REG_READ(hcan->instance->SYSCTRL_b.TSALL) == 1))
    {
        hcan->error_code = HAL_CAN_ERROR_STARTED;
        return HAL_ERROR;
    }

    CAN_STATE_EX_SET(hcan, CAN_STATE_EX_SLEEP);
    HAL_REG_WRITE(hcan->instance->SYSCTRL_b.STBY, 1);

    return HAL_OK;
}

/**
 * @brief  Wake up from sleep mode.
 *         When returning with HAL_OK status from this function, Sleep mode
 *         is exited.
 * @param  hcan pointer to an CAN_Handle_T structure that contains
 *         the configuration information for the specified CAN.
 * @return HAL_Status_T
 */
HAL_Status_T HAL_CAN_WakeUp(CAN_Handle_T *hcan)
{
    /* check para */
    ASSERT_PARAM(hcan);

    /* check can is initialized */
    if (HAL_FLAG_LACKS(hcan->state, HAL_STATE_READY))
    {
        hcan->error_code = HAL_CAN_ERROR_NOT_INITIALIZED;
        return HAL_ERROR;
    } /* check can is tx busy */
    else if ((HAL_REG_READ(hcan->instance->SYSCTRL_b.TPE) == 1) ||
             (HAL_REG_READ(hcan->instance->SYSCTRL_b.TSONE) == 1) ||
             (HAL_REG_READ(hcan->instance->SYSCTRL_b.TSALL) == 1))
    {
        hcan->error_code = HAL_CAN_ERROR_STARTED;
        return HAL_ERROR;
    }

    /* check can is sleep mode */
    if (CAN_STATE_EX_LACK(hcan, CAN_STATE_EX_SLEEP))
    {
        return HAL_OK;
    }

    CAN_STATE_EX_CLR(hcan, CAN_STATE_EX_SLEEP);
    HAL_REG_WRITE(hcan->instance->SYSCTRL_b.STBY, 0);

    return HAL_OK;
}

/**
 * @brief  Check is sleep mode is active.
 * @param  hcan pointer to an CAN_Handle_T structure that contains
 *         the configuration information for the specified CAN.
 * @return Status
 *          - HAL_DISABLE : Sleep mode is not active.
 *          - HAL_ENABLE  : Sleep mode is active
 */
HAL_Status_T HAL_CAN_IsSleepActive(CAN_Handle_T *hcan)
{
    /* check para */
    ASSERT_PARAM(hcan);

    if (HAL_REG_READ(hcan->instance->SYSCTRL_b.STBY))
    {
        return HAL_OK;
    }
    else
    {
        CAN_STATE_EX_CLR(hcan, CAN_STATE_EX_SLEEP);
        return HAL_ERROR;
    }
}

/**
 * @brief  Add a message to the first free Tx mailbox and activate the
 *         corresponding transmission request.
 * @param  hcan pointer to a CAN_Handle_T structure that contains
 *         the configuration information for the specified CAN.
 * @param  tx_header pointer to a CAN_TxHeader_T structure.
 * @param  data array containing the payload of the Tx frame.
 * @param  mailbox choice transmission buffer, @ref can_mailbox
 * @retval HAL_Status_T
 */
HAL_Status_T HAL_CAN_AddTxMessage(CAN_Handle_T *hcan, CAN_TxHeader_T *tx_header, uint8_t *data, uint32_t mailbox)
{
    /* check para */
    ASSERT_PARAM(hcan);
    ASSERT_PARAM(tx_header);
    ASSERT_PARAM(data);
    ASSERT_PARAM(IS_CAN_PTB_STB(mailbox));

    /* check can is started */
    if (HAL_FLAG_LACKS(hcan->state, HAL_STATE_BUSY))
    {
        hcan->error_code = HAL_CAN_ERROR_NOT_STARTED;
        return HAL_ERROR;
    }
    /* check can is sleep */
    if (CAN_STATE_EX_HAS(hcan, CAN_STATE_EX_SLEEP))
    {
        hcan->error_code = HAL_CAN_ERROR_TX_SLEEP;
        return HAL_ERROR;
    }

    /* check space ready */
    if ((mailbox == CAN_MAILBOX_STB) && (HAL_REG_READ(hcan->instance->SYSCTRL_b.TSTAT) == 3))
    {
        return HAL_BUSY;
    }
    else if ((mailbox == CAN_MAILBOX_PTB) && (HAL_REG_READ(hcan->instance->SYSCTRL_b.TPE) == 1))
    {
        return HAL_BUSY;
    }

    /* Process locked */
    TAKE_MUTEX(hcan);

    /* tx use PTB or STB */
    HAL_REG_WRITE(hcan->instance->SYSCTRL_b.TBSEL, mailbox);

    /* write data info */
    HAL_REG_WRITE(hcan->instance->TBSTA_b.DLC, tx_header->dlc);
    HAL_REG_WRITE(hcan->instance->TBSTA_b.IDE, tx_header->ide);
    HAL_REG_WRITE(hcan->instance->TBSTA_b.RTR, tx_header->rtr);
    HAL_REG_WRITE(hcan->instance->TBID_b.ID, tx_header->id);
    hcan->instance->TBSTA &= 0xCF; // clear FD FDF bit for fix RTR bug

    /* write data */
    if (HAL_FLAG_EQU(tx_header->rtr, 0))
    {
        switch (tx_header->dlc)
        {
            case 8:
                HAL_REG_WRITE(hcan->instance->TBDATA2_b.D8, data[7]);
            case 7:
                HAL_REG_WRITE(hcan->instance->TBDATA2_b.D7, data[6]);
            case 6:
                HAL_REG_WRITE(hcan->instance->TBDATA2_b.D6, data[5]);
            case 5:
                HAL_REG_WRITE(hcan->instance->TBDATA2_b.D5, data[4]);
            case 4:
                HAL_REG_WRITE(hcan->instance->TBDATA1_b.D4, data[3]);
            case 3:
                HAL_REG_WRITE(hcan->instance->TBDATA1_b.D3, data[2]);
            case 2:
                HAL_REG_WRITE(hcan->instance->TBDATA1_b.D2, data[1]);
            case 1:
                HAL_REG_WRITE(hcan->instance->TBDATA1_b.D1, data[0]);
            case 0:
                break;
            default:
                break;
        }
    }

    if (mailbox == CAN_MAILBOX_PTB)
    {
        CAN_STATE_EX_SET(hcan, CAN_STATE_EX_PTB);
        HAL_REG_WRITE(hcan->instance->SYSCTRL_b.TPE, 1);
    }
    else
    {
        CAN_STATE_EX_SET(hcan, CAN_STATE_EX_STB);
        HAL_REG_WRITE(hcan->instance->SYSCTRL_b.TSNEXT, 1);
        HAL_REG_WRITE(hcan->instance->SYSCTRL_b.TSALL, 1);
    }

    /* Process unlocked */
    GIVE_MUTEX(hcan);

    return HAL_OK;
}

/**
 * @brief  Abort transmission requests
 * @param  hcan pointer to an CAN_Handle_T structure that contains
 *         the configuration information for the specified CAN.
 * @retval HAL status
 */
HAL_Status_T HAL_CAN_AbortTxRequest(CAN_Handle_T *hcan, uint32_t mailbox)
{
    /* check para */
    ASSERT_PARAM(hcan);
    ASSERT_PARAM(IS_CAN_PTB_STB(mailbox));

    if (mailbox == CAN_MAILBOX_PTB)
    {
        HAL_REG_WRITE(hcan->instance->SYSCTRL_b.TPA, 1);
    }
    else
    {
        HAL_REG_WRITE(hcan->instance->SYSCTRL_b.TSA, 1);
    }

    return HAL_OK;
}

/**
 * @brief  Return Tx FIFO free level: number of free Tx Mailboxes.
 * @param  hcan pointer to an CAN_Handle_T structure that contains
 *         the configuration information for the specified CAN.
 * @return uint32_t
 */
CAN_TX_STA_T HAL_CAN_GetTxMailboxesStatus(CAN_Handle_T *hcan, uint32_t mailbox)
{
    /* check para */
    ASSERT_PARAM(hcan);
    ASSERT_PARAM(IS_CAN_PTB_STB(mailbox));

    volatile uint32_t status = HAL_CAN_TX_EMPTY;
    if (mailbox == CAN_MAILBOX_PTB)
    {
        if (HAL_REG_READ(hcan->instance->SYSCTRL_b.TPE) == 1)
        {
            status = HAL_CAN_TX_FULL;
        }
    }
    else if (mailbox == CAN_MAILBOX_STB)
    {
        status = HAL_REG_READ(hcan->instance->SYSCTRL_b.TSTAT);
    }

    return (CAN_TX_STA_T)status;
}

/**
 * @brief  Check if a transmission request is pending on the tx FIFO.
 * @param  hcan pointer to an CAN_Handle_T structure that contains
 *         the configuration information for the specified CAN.
 * @retval Status
 *          - HAL_DISABLE : No pending transmission request on the tx FIFO.
 *          - HAL_ENABLE  : Pending transmission request on the tx FIFO.
 */
HAL_Status_T HAL_CAN_IsTxMessagePending(CAN_Handle_T *hcan)
{
    /* check para */
    ASSERT_PARAM(hcan);

    return HAL_OK;
}

/**
  * @brief  Return timestamp of Tx message sent, if time triggered communication
            mode is enabled.
  * @param  hcan pointer to a CAN_Handle_T structure that contains
  *         the configuration information for the specified CAN.
  * @retval Timestamp of message sent from Tx Mailbox.
  */
uint32_t HAL_CAN_GetTxTimestamp(CAN_Handle_T *hcan, uint32_t mailbox)
{
    /* check para */
    ASSERT_PARAM(hcan);
    ASSERT_PARAM(IS_CAN_PTB_STB(mailbox));

    return 0;
}

/**
 * @brief  Get an CAN frame from the Rx FIFO zone into the message RAM.
 * @param  hcan pointer to an CAN_Handle_T structure that contains
 *         the configuration information for the specified CAN.
 * @param  rx_header pointer to a CAN_RxHeaderTypeDef structure where the header
 *         of the Rx frame will be stored.
 * @param  data array where the payload of the Rx frame will be stored.
 * @retval HAL_Status_T
 */
HAL_Status_T HAL_CAN_GetRxMessage(CAN_Handle_T *hcan, CAN_RxHeader_T *rx_header, uint8_t *data)
{
    /* check para */
    ASSERT_PARAM(hcan);
    ASSERT_PARAM(rx_header);
    ASSERT_PARAM(data);

    volatile uint32_t status = HAL_CAN_TX_EMPTY;

    /* check can is started */
    if (HAL_FLAG_LACKS(hcan->state, HAL_STATE_BUSY))
    {
        hcan->error_code = HAL_CAN_ERROR_NOT_STARTED;
        return HAL_ERROR;
    }

    status = HAL_REG_READ(hcan->instance->SYSCTRL_b.RSTAT);
    /* check data ready */
    if (status == 0)
    {
        return HAL_BUSY;
    }

    /* Process locked */
    TAKE_MUTEX(hcan);

    /* read data info */
    rx_header->dlc = HAL_REG_READ(hcan->instance->RBSTA_b.DLC);
    rx_header->ide = HAL_REG_READ(hcan->instance->RBSTA_b.IDE);
    rx_header->rtr = HAL_REG_READ(hcan->instance->RBSTA_b.RTR);
    rx_header->id  = HAL_REG_READ(hcan->instance->RBID_b.ID);
    if (HAL_REG_READ(hcan->instance->TTCTRL_b.TTEN))
    {
        rx_header->timestamp = HAL_REG_READ(hcan->instance->RBSTA_b.CYCLE_TIME);
    }

    /* read data */
    if (HAL_FLAG_EQU(rx_header->rtr, 0))
    {
        switch (rx_header->dlc)
        {
            case 8:
                data[7] = HAL_REG_READ(hcan->instance->RBDATA2_b.D8);
            case 7:
                data[6] = HAL_REG_READ(hcan->instance->RBDATA2_b.D7);
            case 6:
                data[5] = HAL_REG_READ(hcan->instance->RBDATA2_b.D6);
            case 5:
                data[4] = HAL_REG_READ(hcan->instance->RBDATA2_b.D5);
            case 4:
                data[3] = HAL_REG_READ(hcan->instance->RBDATA1_b.D4);
            case 3:
                data[2] = HAL_REG_READ(hcan->instance->RBDATA1_b.D3);
            case 2:
                data[1] = HAL_REG_READ(hcan->instance->RBDATA1_b.D2);
            case 1:
                data[0] = HAL_REG_READ(hcan->instance->RBDATA1_b.D1);
            case 0:
                break;
            default:
                break;
        }
    }

    HAL_REG_WRITE(hcan->instance->SYSCTRL_b.RREL, 1);

    /* Process unlocked */
    GIVE_MUTEX(hcan);

    return HAL_OK;
}

/**
 * @brief  Return Rx FIFO fill level.
 * @param  hcan pointer to an CAN_Handle_T structure that contains
 *         the configuration information for the specified CAN.
 * @return uint32_t
 */
CAN_RX_STA_T HAL_CAN_GetRxFifoStatus(CAN_Handle_T *hcan)
{
    /* check para */
    ASSERT_PARAM(hcan);

    volatile uint32_t status = HAL_CAN_TX_EMPTY;

    status = HAL_REG_READ(hcan->instance->SYSCTRL_b.RSTAT);

    return (CAN_RX_STA_T)status;
}

/**
 * @brief  Enable interrupts.
 * @param  hcan pointer to an CAN_Handle_T structure that contains
 *         the configuration information for the specified CAN.
 * @param  active_its indicates which interrupts will be enabled.
 *         This parameter can be any combination of @arg CAN_Interrupts.
 * @return HAL_Status_T
 */
HAL_Status_T HAL_CAN_ActivateNotification(CAN_Handle_T *hcan, uint32_t active_its)
{
    /* check para */
    ASSERT_PARAM(hcan);

    uint32_t ies = hcan->notification;
    if (HAL_FLAG_LACKS(hcan->state, HAL_STATE_READY))
    {
        hcan->error_code = HAL_CAN_ERROR_NOT_INITIALIZED;
        return HAL_ERROR;
    }
    else
    {
        if (HAL_FLAG_HAS(active_its, CAN_IT_TX_PTB))
        {
            ies |= CAN_IE_TPIE;
        }
        if (HAL_FLAG_HAS(active_its, CAN_IT_TX_STB))
        {
            ies |= CAN_IE_TSIE;
        }
        if (HAL_FLAG_HAS(active_its, CAN_IT_RX_ONE))
        {
            ies |= CAN_IE_RIE;
        }
        if (HAL_FLAG_HAS(active_its, CAN_IT_RX_HALF))
        {
            ies |= CAN_IE_RAFIE;
        }
        if (HAL_FLAG_HAS(active_its, CAN_IT_RX_FULL))
        {
            ies |= CAN_IE_RFIE;
        }
        if (HAL_FLAG_HAS(active_its, CAN_IT_ERROR_RX_OVERRUN))
        {
            ies |= CAN_IE_ROIE;
        }
        if (HAL_FLAG_HAS(active_its, CAN_IT_ERROR_PASSIVE))
        {
            ies |= CAN_IE_EPIE;
        }
        if (HAL_FLAG_HAS(active_its, CAN_IT_ERROR_BUS))
        {
            ies |= CAN_IE_BEIE;
        }
        if (HAL_FLAG_HAS(active_its, CAN_IT_ERROR_AL))
        {
            ies |= CAN_IE_ALIE;
        }
        if (HAL_FLAG_HAS(active_its, CAN_IT_ERROR))
        {
            ies |= CAN_IE_EIE;
        }

        hcan->notification = ies;
        CAN_ENABLE_IE(hcan, hcan->notification);
        return HAL_OK;
    }
}

/**
 * @brief  Disable interrupts.
 * @param  hcan pointer to an CAN_Handle_T structure that contains
 *         the configuration information for the specified CAN.
 * @param  inactive_its indicates which interrupts will be disabled.
 *         This parameter can be any combination of @arg CAN_Interrupts.
 * @return HAL_Status_T
 */
HAL_Status_T HAL_CAN_DeactivateNotification(CAN_Handle_T *hcan, uint32_t inactive_its)
{
    /* check para */
    ASSERT_PARAM(hcan);

    uint32_t ies   = hcan->notification;
    uint32_t imask = 0;
    if (HAL_FLAG_LACKS(hcan->state, HAL_STATE_READY))
    {
        hcan->error_code = HAL_CAN_ERROR_NOT_INITIALIZED;
        return HAL_ERROR;
    }
    else
    {
        if (HAL_FLAG_HAS(inactive_its, CAN_IT_TX_PTB))
        {
            ies ^= CAN_IE_TPIE;
            imask |= CAN_IE_TPIE;
        }
        if (HAL_FLAG_HAS(inactive_its, CAN_IT_TX_STB))
        {
            ies ^= CAN_IE_TSIE;
            imask |= CAN_IE_TSIE;
        }
        if (HAL_FLAG_HAS(inactive_its, CAN_IT_RX_ONE))
        {
            ies ^= CAN_IE_RIE;
            imask |= CAN_IE_RIE;
        }
        if (HAL_FLAG_HAS(inactive_its, CAN_IT_RX_HALF))
        {
            ies ^= CAN_IE_RAFIE;
            imask |= CAN_IE_RAFIE;
        }
        if (HAL_FLAG_HAS(inactive_its, CAN_IT_RX_FULL))
        {
            ies ^= CAN_IE_RFIE;
            imask |= CAN_IE_RFIE;
        }
        if (HAL_FLAG_HAS(inactive_its, CAN_IT_ERROR_RX_OVERRUN))
        {
            ies ^= CAN_IE_ROIE;
            imask |= CAN_IE_ROIE;
        }
        if (HAL_FLAG_HAS(inactive_its, CAN_IT_ERROR_PASSIVE))
        {
            ies ^= CAN_IE_EPIE;
            imask |= CAN_IE_EPIE;
        }
        if (HAL_FLAG_HAS(inactive_its, CAN_IT_ERROR_BUS))
        {
            ies ^= CAN_IE_BEIE;
            imask |= CAN_IE_BEIE;
        }
        if (HAL_FLAG_HAS(inactive_its, CAN_IT_ERROR_AL))
        {
            ies ^= CAN_IE_ALIE;
            imask |= CAN_IE_ALIE;
        }
        if (HAL_FLAG_HAS(inactive_its, CAN_IT_ERROR))
        {
            ies ^= CAN_IE_EIE;
            imask |= CAN_IE_EIE;
        }

        hcan->notification = ies;
        CAN_DISABLE_IE(hcan, imask);
        return HAL_OK;
    }
}

/**
 * @brief  Transmission PTB complete callback
 * @param  hcan pointer to a CAN_Handle_T structure that contains
 *         the configuration information for the specified CAN.
 * @retval None
 */
__weak ATTR_ISR void HAL_CAN_TxPTBCompleteCallback(CAN_Handle_T *hcan)
{
    /* Prevent unused argument(s) compilation warning */
    UNUSED(hcan);

    /* NOTE : This function Should not be modified, when the callback is needed,
              the HAL_CAN_TxPTBCompleteCallback could be implemented in the user file
     */
}

/**
 * @brief  Transmission STB fifo complete callback
 * @param  hcan pointer to a CAN_Handle_T structure that contains
 *         the configuration information for the specified CAN.
 * @retval None
 */
__weak ATTR_ISR void HAL_CAN_TxSTBCompleteCallback(CAN_Handle_T *hcan)
{
    /* Prevent unused argument(s) compilation warning */
    UNUSED(hcan);

    /* NOTE : This function Should not be modified, when the callback is needed,
              the HAL_CAN_TxSTBCompleteCallback could be implemented in the user file
     */
}

/**
 * @brief  Transmission abort callback
 * @param  hcan pointer to a CAN_Handle_T structure that contains
 *         the configuration information for the specified CAN.
 * @retval None
 */
__weak ATTR_ISR void HAL_CAN_TxAbortCallback(CAN_Handle_T *hcan)
{
    /* Prevent unused argument(s) compilation warning */
    UNUSED(hcan);

    /* NOTE : This function Should not be modified, when the callback is needed,
              the HAL_CAN_TxAbortCallback could be implemented in the user file
     */
}

/**
 * @brief  RX fifo one full callback
 * @param  hcan pointer to a CAN_Handle_T structure that contains
 *         the configuration information for the specified CAN.
 * @retval None
 */
__weak ATTR_ISR void HAL_CAN_RxFifoOneFullCallback(CAN_Handle_T *hcan)
{
    /* Prevent unused argument(s) compilation warning */
    UNUSED(hcan);

    /* NOTE : This function Should not be modified, when the callback is needed,
              the HAL_CAN_RxFifoOneFullCallback could be implemented in the user file
     */
}

/**
 * @brief  RX fifo almost full callback
 * @param  hcan pointer to a CAN_Handle_T structure that contains
 *         the configuration information for the specified CAN.
 * @retval None
 */
__weak ATTR_ISR void HAL_CAN_RxFifoHalfFullCallback(CAN_Handle_T *hcan)
{
    /* Prevent unused argument(s) compilation warning */
    UNUSED(hcan);

    /* NOTE : This function Should not be modified, when the callback is needed,
              the HAL_CAN_RxFifoHalfFullCallback could be implemented in the user file
     */
}

/**
 * @brief  RX FIFO all full callback
 * @param  hcan pointer to a CAN_Handle_T structure that contains
 *         the configuration information for the specified CAN.
 * @retval None
 */
__weak ATTR_ISR void HAL_CAN_RxFifoAllFullCallback(CAN_Handle_T *hcan)
{
    /* Prevent unused argument(s) compilation warning */
    UNUSED(hcan);

    /* NOTE : This function Should not be modified, when the callback is needed,
              the HAL_CAN_RxFifoAllFullCallback could be implemented in the user file
     */
}

/**
 * @brief  Error CAN callback.
 * @param  hcan pointer to a CAN_Handle_T structure that contains
 *         the configuration information for the specified CAN.
 * @retval None
 */
__weak ATTR_ISR void HAL_CAN_ErrorCallback(CAN_Handle_T *hcan)
{
    /* Prevent unused argument(s) compilation warning */
    UNUSED(hcan);

    /* NOTE : This function Should not be modified, when the callback is needed,
              the HAL_CAN_ErrorCallback could be implemented in the user file
     */
}

/**
 * @brief  Return the CAN state.
 * @param  hcan pointer to an CAN_Handle_T structure that contains
 *         the configuration information for the specified CAN.
 * @return CAN_State_T
 */
HAL_State_T HAL_CAN_GetState(CAN_Handle_T *hcan)
{
    /* check para */
    ASSERT_PARAM(hcan);

    return hcan->state;
}

/**
 * @brief  Return the CAN error code.
 * @param  hcan pointer to an CAN_Handle_T structure that contains
 *         the configuration information for the specified CAN.
 * @return uint32_t error code
 */
uint32_t HAL_CAN_GetError(CAN_Handle_T *hcan)
{
    /* check para */
    ASSERT_PARAM(hcan);

    return hcan->error_code;
}

/**
 * @brief  Reset the CAN error code.
 * @param  hcan pointer to an CAN_Handle_T structure that contains
 *         the configuration information for the specified CAN.
 * @return HAL_Status_T
 */
HAL_Status_T HAL_CAN_ResetError(CAN_Handle_T *hcan)
{
    /* check para */
    ASSERT_PARAM(hcan);

    hcan->error_code = HAL_CAN_ERROR_NONE;
    hcan->state      = HAL_STATE_READY;

    return HAL_OK;
}

/**
 * @brief Handles CAN TX interrupt request
 * @param hcan pointer to an CAN_Handle_T structure that contains
 *        the configuration information for the specified CAN.
 */
__weak ATTR_ISR void HAL_CAN_TX_IRQHandler(CAN_Handle_T *hcan)
{
    /* Transmission Primary Int */
    if (CAN_FLAG_HAS(hcan, CAN_FLAG_TPIF) && CAN_IE_HAS(hcan, CAN_IE_TPIE))
    {
        CAN_FLAG_CLR(hcan, CAN_FLAG_TPIF);
#if (configUSE_CAN_REGISTER_CALLBACKS == 1U)
        if (hcan->TxPTBCompleteCallback)
            (hcan->TxPTBCompleteCallback)(hcan);
#else
        HAL_CAN_TxPTBCompleteCallback(hcan);
#endif
        CAN_STATE_EX_CLR(hcan, CAN_STATE_EX_PTB);
    }

    /* Transmission Secondary Int */
    if (CAN_FLAG_HAS(hcan, CAN_FLAG_TSIF) && CAN_IE_HAS(hcan, CAN_IE_TSIE))
    {
        CAN_FLAG_CLR(hcan, CAN_FLAG_TSIF);
#if (configUSE_CAN_REGISTER_CALLBACKS == 1U)
        if (hcan->TxSTBCompleteCallback)
            (hcan->TxSTBCompleteCallback)(hcan);
#else
        HAL_CAN_TxSTBCompleteCallback(hcan);
#endif
        CAN_STATE_EX_CLR(hcan, CAN_STATE_EX_STB);
    }
}

/**
 * @brief Handles CAN RX interrupt request
 * @param hcan pointer to an CAN_Handle_T structure that contains
 *        the configuration information for the specified CAN.
 */
__weak ATTR_ISR void HAL_CAN_RX_IRQHandler(CAN_Handle_T *hcan)
{
    /* Receive buffer Full Int */
    if (CAN_FLAG_HAS(hcan, CAN_FLAG_RFIF) && CAN_IE_HAS(hcan, CAN_IE_RFIE))
    {
        CAN_FLAG_CLR(hcan, CAN_FLAG_RFIF);
#if (configUSE_CAN_REGISTER_CALLBACKS == 1U)
        if (hcan->RxFifoAllFullCallback)
            (hcan->RxFifoAllFullCallback)(hcan);
#else
        HAL_CAN_RxFifoAllFullCallback(hcan);
#endif
    }

    /* Receive buffer Almost Full Int */
    if (CAN_FLAG_HAS(hcan, CAN_FLAG_RAFIF) && CAN_IE_HAS(hcan, CAN_IE_RAFIE))
    {
        CAN_FLAG_CLR(hcan, CAN_FLAG_RAFIF);
#if (configUSE_CAN_REGISTER_CALLBACKS == 1U)
        if (hcan->RxFifoHalfFullCallback)
            (hcan->RxFifoHalfFullCallback)(hcan);
#else
        HAL_CAN_RxFifoHalfFullCallback(hcan);
#endif
    }

    /* Receive buffer Overflow Int */
    if (CAN_FLAG_HAS(hcan, CAN_FLAG_ROIF) && CAN_IE_HAS(hcan, CAN_IE_ROIE))
    {
        CAN_FLAG_CLR(hcan, CAN_FLAG_ROIF);
        HAL_FLAG_SET(hcan->state, HAL_STATE_READY);
        hcan->error_code = HAL_CAN_ERROR_RX_OVERFLOW;
#if (configUSE_CAN_REGISTER_CALLBACKS == 1U)
        if (hcan->ErrorCallback)
            (hcan->ErrorCallback)(hcan);
#else
        HAL_CAN_ErrorCallback(hcan);
#endif
    }

    /* Receive one frame Int */
    if (CAN_FLAG_HAS(hcan, CAN_FLAG_RIF) && CAN_IE_HAS(hcan, CAN_IE_RIE))
    {
        CAN_FLAG_CLR(hcan, CAN_FLAG_RIF);
#if (configUSE_CAN_REGISTER_CALLBACKS == 1U)
        if (hcan->RxFifoOneFullCallback)
            hcan->RxFifoOneFullCallback(hcan);
#else
        HAL_CAN_RxFifoOneFullCallback(hcan);
#endif
    }
}

/**
 * @brief Handles CAN Time-Trigger interrupt request
 * @param hcan pointer to an CAN_Handle_T structure that contains
 *        the configuration information for the specified CAN.
 */
__weak ATTR_ISR void HAL_CAN_TT_IRQHandler(CAN_Handle_T *hcan)
{
    /* TTCAN: Watch Trigger Int */
    if (TTCAN_FLAG_HAS(hcan, TTCAN_FLAG_WTIF) && TTCAN_IE_HAS(hcan, TTCAN_IE_WTIE))
    {
        TTCAN_FLAG_CLR(hcan, TTCAN_FLAG_WTIF);
    }

    /* TTCAN: Time Trigger Int */
    if (TTCAN_FLAG_HAS(hcan, TTCAN_FLAG_TTIF) && TTCAN_IE_HAS(hcan, TTCAN_IE_TTIE))
    {
        TTCAN_FLAG_CLR(hcan, TTCAN_FLAG_TTIF);
    }

    /* Time Trigger CAN error */
    if (TTCAN_FLAG_HAS(hcan, TTCAN_FLAG_TEIF))
    {
        /* TTCAN: Trigger Error Int */
        TTCAN_FLAG_CLR(hcan, TTCAN_FLAG_TEIF);
        HAL_FLAG_SET(hcan->state, HAL_STATE_READY);
        hcan->error_code = HAL_CAN_ERROR_TIME_TRIGGER;
#if (configUSE_CAN_REGISTER_CALLBACKS == 1U)
        if (hcan->ErrorCallback)
            (hcan->ErrorCallback)(hcan);
#else
        HAL_CAN_ErrorCallback(hcan);
#endif
    }
}

/**
 * @brief Handles CAN Error-Worning interrupt request
 * @param hcan pointer to an CAN_Handle_T structure that contains
 *        the configuration information for the specified CAN.
 */
__weak ATTR_ISR void HAL_CAN_EW_IRQHandler(CAN_Handle_T *hcan)
{
    hcan->error_code = HAL_CAN_ERROR_NONE;
    /* Bus Error Int */
    if (CAN_FLAG_HAS(hcan, CAN_FLAG_BEIF) && CAN_IE_HAS(hcan, CAN_IE_BEIE))
    {
        CAN_FLAG_CLR(hcan, CAN_FLAG_BEIF);
        HAL_FLAG_SET(hcan->state, HAL_STATE_READY);
        switch (hcan->instance->ERRSTA_b.KOER)
        {
            case 0:
                break;
            case 1:
                hcan->error_code = HAL_CAN_ERROR_BD;
                break;
            case 2:
                hcan->error_code = HAL_CAN_ERROR_FOR;
                break;
            case 3:
                hcan->error_code = HAL_CAN_ERROR_STF;
                break;
            case 4:
                hcan->error_code = HAL_CAN_ERROR_ACK;
                break;
            case 5:
                hcan->error_code = HAL_CAN_ERROR_CRC;
                break;
            case 6:
            case 7:
            default:
                hcan->error_code = HAL_CAN_ERROR_BUS;
                break;
        }
    }

    /* Error Int */
    if (CAN_FLAG_HAS(hcan, CAN_FLAG_EIF) && CAN_IE_HAS(hcan, CAN_IE_EIE))
    {
        CAN_FLAG_CLR(hcan, CAN_FLAG_EIF);
        HAL_FLAG_SET(hcan->state, HAL_STATE_READY);
        hcan->error_code = HAL_CAN_ERROR_EWG;
    }

    /* Error Passive Int */
    if (CAN_FLAG_HAS(hcan, CAN_FLAG_EPIF) && CAN_IE_HAS(hcan, CAN_IE_EPIE))
    {
        CAN_FLAG_CLR(hcan, CAN_FLAG_EPIF);
        HAL_FLAG_SET(hcan->state, HAL_STATE_READY);
        hcan->error_code = HAL_CAN_ERROR_PASSIVE;
    }

    /* Arbitration Lost Int */
    if (CAN_FLAG_HAS(hcan, CAN_FLAG_ALIF) && CAN_IE_HAS(hcan, CAN_IE_ALIE))
    {
        CAN_FLAG_CLR(hcan, CAN_FLAG_ALIF);
        HAL_FLAG_SET(hcan->state, HAL_STATE_READY);
        hcan->error_code = HAL_CAN_ERROR_TX_ALST;
    }

    /* call error callback when error state */
    if (!HAL_FLAG_EQU(hcan->error_code, HAL_CAN_ERROR_NONE))
    {
#if (configUSE_CAN_REGISTER_CALLBACKS == 1U)
        if (hcan->ErrorCallback)
            (hcan->ErrorCallback)(hcan);
#else
        HAL_CAN_ErrorCallback(hcan);
#endif
        return;
    }

    /* Abort Int */
    if (CAN_FLAG_HAS(hcan, CAN_FLAG_AIF))
    {
        CAN_FLAG_CLR(hcan, CAN_FLAG_AIF);
#if (configUSE_CAN_REGISTER_CALLBACKS == 1U)
        if (hcan->TxAbortCallback)
            (hcan->TxAbortCallback)(hcan);
#else
        HAL_CAN_TxAbortCallback(hcan);
#endif
        return;
    }
}

#endif
