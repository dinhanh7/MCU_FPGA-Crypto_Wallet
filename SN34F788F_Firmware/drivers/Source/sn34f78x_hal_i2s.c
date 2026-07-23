/***********************
 * SN34F78X SDK V2.1.4 *
 ***********************/

#include "sn34f78x_hal_i2s.h"

#if (configUSE_I2S_PERIPHERAL == 1)
uint32_t audio_freq[2][2] = {
    // 48M 96M
    {0x17, 0x00}, // 16K
    {0x7, 0x00},  // 48K
};

/**
 * @brief Initializes the I2S according to the specified parameters in the I2S_Init_T and create the associated handle.
 * @param hi2s pointer to a I2S_Handle_T structure that contains the configuration information for I2S module
 * @return HAL_Status_T
 */
HAL_Status_T HAL_I2S_Init(I2S_Handle_T *hi2s)
{
    /* Check the I2S handle */
    ASSERT_PARAM(hi2s);
    /* Check the instance */
    ASSERT_PARAM(IS_I2S_INSTANCE(hi2s));

    /* Check the init parameters */
    ASSERT_PARAM(IS_I2S_MODE(hi2s));
    ASSERT_PARAM(IS_I2S_STD(hi2s));
    ASSERT_PARAM(IS_I2S_CHANNEL(hi2s));
    ASSERT_PARAM(IS_I2S_DATA_LEN(hi2s));
    ASSERT_PARAM(IS_I2S_CH_LEN(hi2s));
    // ASSERT_PARAM(IS_I2S_CLK_DIV(hi2s));

    if (HAL_FLAG_EQU(hi2s->state, HAL_STATE_RESET))
    {
        /* Allocate lock resource and initialize it */
        GIVE_MUTEX(hi2s);

#if (configUSE_I2S_REGISTER_CALLBACKS == 1)
        /* Reset interrupt callbacks to legacy weak callbacks */
        _i2s_init_callbacks_to_default(hi2s);

        /* Init default callback */
        if (hi2s->MspInitCallback == NULL)
        {
            hi2s->MspInitCallback = HAL_I2S_MspInit;
        }
        /* Init the low level hardware */
        hi2s->MspInitCallback(hi2s);
#else
        /* Init the low level hardware */
        HAL_I2S_MspInit(hi2s);
#endif /* configUSE_I2S_REGISTER_CALLBACKS */

#if (configUSE_I2S_NONBLOCKING_DMA == 1)
        if (hi2s->hdmatx != NULL)
        {
            hi2s->hdmatx->parent = (void *)hi2s;
            HAL_DMA_RegisterCallback(hi2s->hdmatx, HAL_DMA_XFER_CPLT_CB_ID, _i2s_tx_dma_cb);
            HAL_DMA_RegisterCallback(hi2s->hdmatx, HAL_DMA_XFER_ERROR_CB_ID, _i2s_tx_dma_err_cb);
        }

        if (hi2s->hdmarx != NULL)
        {
            hi2s->hdmarx->parent = (void *)hi2s;
            HAL_DMA_RegisterCallback(hi2s->hdmarx, HAL_DMA_XFER_CPLT_CB_ID, _i2s_rx_dma_cb);
            HAL_DMA_RegisterCallback(hi2s->hdmarx, HAL_DMA_XFER_ERROR_CB_ID, _i2s_rx_dma_err_cb);
        }
#endif
    }
    else
    {
        HAL_FLAG_WRITE(hi2s->error_code, HAL_ERROR_REINIT);
        return HAL_ERROR;
    }

    /* Set the I2S configuration */
    RET_FLAG_FALSE(_i2s_set_config(hi2s) == HAL_OK, HAL_ERROR);

    /* Update the state */
    HAL_FLAG_WRITE(hi2s->state, HAL_STATE_READY);
    HAL_FLAG_WRITE(hi2s->rx_state, HAL_STATE_READY);

    return HAL_OK;
}

/**
 * @brief DeInitializes the I2S peripheral.
 * @param hi2s pointer to a I2S_Handle_T structure that contains the configuration information for I2S module
 * @return HAL_Status_T
 */
HAL_Status_T HAL_I2S_DeInit(I2S_Handle_T *hi2s)
{
    /* Check the I2S handle */
    ASSERT_PARAM(hi2s);
    /* Check the instance */
    ASSERT_PARAM(IS_I2S_INSTANCE(hi2s));

    HAL_FLAG_WRITE(hi2s->state, HAL_STATE_BUSY);

    /* Disable I2S */
    HAL_REG_CLEAR(hi2s->instance->CTRL2);

#if (configUSE_I2S_REGISTER_CALLBACKS == 1)
    /* Init default callback */
    if (hi2s->MspDeInitCallback == NULL)
    {
        hi2s->MspDeInitCallback = HAL_I2S_MspDeInit;
    }
    /* DeInit the low level hardware */
    hi2s->MspDeInitCallback(hi2s);
#else
    /* DeInit the low level hardware */
    HAL_I2S_MspDeInit(hi2s);
#endif /* configUSE_I2S_REGISTER_CALLBACKS */

#if (configUSE_I2S_NONBLOCKING_DMA == 1)
    if (hi2s->hdmatx != NULL)
    {
        hi2s->hdmatx->parent = NULL;
        HAL_DMA_UnRegisterCallback(hi2s->hdmatx, HAL_DMA_XFER_CPLT_CB_ID);
        HAL_DMA_UnRegisterCallback(hi2s->hdmatx, HAL_DMA_XFER_ERROR_CB_ID);
    }

    if (hi2s->hdmarx != NULL)
    {
        hi2s->hdmarx->parent = NULL;
        HAL_DMA_UnRegisterCallback(hi2s->hdmarx, HAL_DMA_XFER_CPLT_CB_ID);
        HAL_DMA_UnRegisterCallback(hi2s->hdmarx, HAL_DMA_XFER_ERROR_CB_ID);
    }
#endif /* configUSE_I2S_NONBLOCKING_DMA */

    /* Update the state */
    HAL_FLAG_WRITE(hi2s->state, HAL_STATE_RESET);
    HAL_FLAG_WRITE(hi2s->rx_state, HAL_STATE_RESET);
    CLEAR_ERROR_CODE(hi2s);

    /* Release the process protection */
    GIVE_MUTEX(hi2s);

    return HAL_OK;
}

/**
 * @brief I2S MSP Init
 * @param  hi2s pointer to a I2S_Handle_T structure that contains the configuration information for I2S module
 * @retval None
 */
__weak void HAL_I2S_MspInit(I2S_Handle_T *hi2s)
{
    /* Prevent unused argument(s) compilation warning */
    UNUSED(hi2s);

    /* NOTE : This function Should not be modified, when the callback is needed, the HAL_I2S_MspInit could be implemented in the user file */
}

/**
 * @brief I2S MSP DeInit
 * @param  hi2s pointer to a I2S_Handle_T structure that contains the configuration information for I2S module
 * @retval None
 */
__weak void HAL_I2S_MspDeInit(I2S_Handle_T *hi2s)
{
    /* Prevent unused argument(s) compilation warning */
    UNUSED(hi2s);

    /* NOTE : This function Should not be modified, when the callback is needed, the HAL_I2S_MspDeInit could be implemented in the user file */
}

/*
================================================================================
            ##### Callbacks Register/UnRegister functions #####
================================================================================
*/
#if (configUSE_I2S_REGISTER_CALLBACKS == 1)
/**
 * @brief  Register a User I2S Callback To be used instead of the weak predefined callback
 * @param  hi2s Pointer to a I2S_Handle_T structure that contains the configuration information for the specified I2S module.
 * @param  callback_id ID of the callback to be registered
 *         This parameter can be one of the following values:
 *           @arg @ref HAL_I2S_TX_COMPLETE_CB_ID I2S Tx Complete Callback ID
 *           @arg @ref HAL_I2S_RX_COMPLETE_CB_ID I2S Rx Complete Callback ID
 *           @arg @ref HAL_I2S_ERROR_CB_ID I2S Error Callback ID
 *           @arg @ref HAL_I2S_MSP_INIT_CB_ID MspInit Callback ID
 *           @arg @ref HAL_I2S_MSP_DEINIT_CB_ID MspDeInit Callback ID
 * @param  callback pointer to the Callback function
 * @retval HAL status
 */
HAL_Status_T HAL_I2S_RegisterCallback(I2S_Handle_T *hi2s, HAL_I2S_CallbackID_T callback_id, I2S_Callback_T callback)
{
    HAL_Status_T status = HAL_OK;

    /* Check the I2S handle */
    ASSERT_PARAM(hi2s);
    /* Check the instance */
    ASSERT_PARAM(IS_I2S_INSTANCE(hi2s));
    /* Check the parameters */
    ASSERT_PARAM(callback);

    /* Protect the process */
    TAKE_MUTEX(hi2s);

    switch (callback_id)
    {
        case HAL_I2S_MSP_INIT_CB_ID:
        {
            hi2s->MspInitCallback = callback;
        }
        break;

        case HAL_I2S_MSP_DEINIT_CB_ID:
        {
            hi2s->MspDeInitCallback = callback;
        }
        break;

        case HAL_I2S_TX_COMPLETE_CB_ID:
        {
            hi2s->TxCpltCallback = callback;
        }
        break;

        case HAL_I2S_RX_COMPLETE_CB_ID:
        {
            hi2s->RxCpltCallback = callback;
        }
        break;

        case HAL_I2S_ERROR_CB_ID:
        {
            hi2s->ErrorCallback = callback;
        }
        break;

        default:
        {
            /* Update the error code */
            HAL_FLAG_WRITE(hi2s->error_code, HAL_I2S_ERROR_INVALID_CALLBACK);
            /* Return error status */
            status = HAL_ERROR;
        }
        break;
    }

    /* Release the process protection */
    GIVE_MUTEX(hi2s);

    return status;
}

/**
 * @brief  Unregister an I2S Callback, I2S callback is redirected to the weak predefined callback
 * @param  hi2s Pointer to a I2S_Handle_T structure that contains the configuration information for the specified I2S module.
 * @param  callback_id ID of the callback to be unregistered
 *         This parameter can be one of the following values:
 *           @arg @ref HAL_I2S_TX_COMPLETE_CB_ID I2S Tx Complete Callback ID
 *           @arg @ref HAL_I2S_RX_COMPLETE_CB_ID I2S Rx Complete Callback ID
 *           @arg @ref HAL_I2S_ERROR_CB_ID I2S Error Callback ID
 *           @arg @ref HAL_I2S_MSP_INIT_CB_ID MspInit Callback ID
 *           @arg @ref HAL_I2S_MSP_DEINIT_CB_ID MspDeInit Callback ID
 * @retval HAL status
 */
HAL_Status_T HAL_I2S_UnRegisterCallback(I2S_Handle_T *hi2s, HAL_I2S_CallbackID_T callback_id)
{
    HAL_Status_T status = HAL_OK;

    /* Check the I2S handle */
    ASSERT_PARAM(hi2s);
    /* Check the instance */
    ASSERT_PARAM(IS_I2S_INSTANCE(hi2s));

    /* Protect the process */
    TAKE_MUTEX(hi2s);

    switch (callback_id)
    {
        case HAL_I2S_MSP_INIT_CB_ID:
        {
            hi2s->MspInitCallback = HAL_I2S_MspInit;
        }
        break;

        case HAL_I2S_MSP_DEINIT_CB_ID:
        {
            hi2s->MspDeInitCallback = HAL_I2S_MspDeInit;
        }
        break;

        case HAL_I2S_TX_COMPLETE_CB_ID:
        {
            hi2s->TxCpltCallback = HAL_I2S_TxCpltCallback;
        }
        break;

        case HAL_I2S_RX_COMPLETE_CB_ID:
        {
            hi2s->RxCpltCallback = HAL_I2S_RxCpltCallback;
        }
        break;

        case HAL_I2S_ERROR_CB_ID:
        {
            hi2s->ErrorCallback = HAL_I2S_ErrorCallback;
        }
        break;

        default:
        {
            /* Update the error code */
            HAL_FLAG_WRITE(hi2s->error_code, HAL_I2S_ERROR_INVALID_CALLBACK);
            /* Return error status */
            status = HAL_ERROR;
        }
        break;
    }

    /* Release the process protection */
    GIVE_MUTEX(hi2s);

    return status;
}
#endif /* configUSE_I2S_REGISTER_CALLBACKS */
/*
================================================================================
            ##### Peripheral State and Error functions #####
================================================================================
*/
/**
 * @brief  Return the I2S state
 * @param  hi2s pointer to a I2S_Handle_T structure that contains
 *         the configuration information for I2S module
 * @retval HAL state
 */
HAL_State_T HAL_I2S_GetState(I2S_Handle_T *hi2s)
{
    /* Check the I2S handle */
    ASSERT_PARAM(hi2s);
    /* Check the instance */
    ASSERT_PARAM(IS_I2S_INSTANCE(hi2s));

    return (HAL_State_T)(hi2s->state | hi2s->rx_state);
}

/**
 * @brief  Return the I2S error code
 * @param  hi2s pointer to a I2S_Handle_T structure that contains
 *         the configuration information for I2S module
 * @retval I2S Error Code
 */
uint32_t HAL_I2S_GetError(I2S_Handle_T *hi2s)
{
    /* Check the I2S handle */
    ASSERT_PARAM(hi2s);
    /* Check the instance */
    ASSERT_PARAM(IS_I2S_INSTANCE(hi2s));

    return hi2s->error_code;
}

/*
================================================================================
            ##### I/O Operation functions #####
================================================================================
*/
#if (configUSE_I2S_BLOCKING == 1)
/**
 * @brief  Transmit an amount of data in blocking mode
 * @param  hi2s pointer to a I2S_Handle_T structure that contains
 *         the configuration information for I2S module
 * @param  data a 8-bit pointer to data buffer.
 * @param  size number of data sample to be sent:
 * @param  timeout Timeout duration
 * @note   The I2S is kept enabled at the end of transaction to avoid the clock de-synchronization
 *         between Master and Slave(example: audio streaming).
 * @retval HAL status
 */
HAL_Status_T HAL_I2S_Transmit(I2S_Handle_T *hi2s, uint8_t *data, uint16_t size, uint32_t timeout)
{
    HAL_Status_T status = HAL_OK;

    /* Check the I2S handle */
    ASSERT_PARAM(hi2s);
    /* Check the instance */
    ASSERT_PARAM(IS_I2S_INSTANCE(hi2s));
    /* Check the parameters */
    ASSERT_PARAM(data);
    ASSERT_PARAM(size);

    if (HAL_FLAG_EQU(hi2s->state, HAL_STATE_READY))
    {
        /* Protect the process */
        TAKE_MUTEX(hi2s);

        /* Update the state */
        HAL_FLAG_WRITE(hi2s->state, HAL_STATE_BUSY_TX);
        /* Clear I2S error code */
        CLEAR_ERROR_CODE(hi2s);
        /* Set the function for transmit */
        _i2s_xfer_info(hi2s, data, NULL, size, 0);
        /* Set the first data */
        hi2s->I2S_DataTxHandler(hi2s, 1);
        /* Enable the I2S */
        I2S_ENABLE_TX_FUC(hi2s);
        I2S_ENABLE_RX_FUC(hi2s); // Due to the hardware design, both TXEN and RXEN must be enabled to maintain normal data transmission.
        PERIPHERAL_I2S_ENABLE(hi2s);
        /* Transmit data */
        uint32_t      tick_start = HAL_GetTick();
        uint16_t      cnt        = I2S_TX_FIFO_DEPTH;
        SN_SPI0_Type *reg        = hi2s->instance;
        while (hi2s->tx.xfer_count > 0U)
        {
            cnt = I2S_TX_FIFO_DEPTH - HAL_REG_READ(reg->STATUS_b.TFVE);
            if (cnt > 0)
            {
                if (cnt > hi2s->tx.xfer_count)
                {
                    cnt = hi2s->tx.xfer_count;
                }
                hi2s->I2S_DataTxHandler(hi2s, cnt);
            }
            BREAK_PROC_TIMEOUT(hi2s, tick_start, timeout);
        }

        if (hi2s->error_code == HAL_ERROR_TIMEOUT)
        {
            status = HAL_TIMEOUT;
        }

        /* Update the state */
        HAL_FLAG_WRITE(hi2s->state, HAL_STATE_READY);
        /* Release the process protection */
        GIVE_MUTEX(hi2s);
    }
    else if (HAL_FLAG_EQU(hi2s->state, HAL_STATE_BUSY_TX))
    {
        /* Return busy status */
        status = HAL_BUSY;
    }
    else
    {
        /* Return error status */
        status = HAL_ERROR;
    }

    return status;
}

/**
 * @brief  Receive an amount of data in blocking mode
 * @param  hi2s pointer to a I2S_Handle_T structure that contains
 *         the configuration information for I2S module
 * @param  data a 8-bit pointer to data buffer.
 * @param  size number of data sample to be sent:
 * @param  timeout Timeout duration
 * @note   The I2S is kept enabled at the end of transaction to avoid the clock de-synchronization
 *         between Master and Slave(example: audio streaming).
 * @note   In I2S Master Receiver mode, just after enabling the peripheral the clock will be generate
 *         in continuous way and as the I2S is not disabled at the end of the I2S transaction.
 * @retval HAL status
 */
HAL_Status_T HAL_I2S_Receive(I2S_Handle_T *hi2s, uint8_t *data, uint16_t size, uint32_t timeout)
{
    HAL_Status_T status = HAL_OK;

    /* Check the I2S handle */
    ASSERT_PARAM(hi2s);
    /* Check the instance */
    ASSERT_PARAM(IS_I2S_INSTANCE(hi2s));
    /* Check the parameters */
    ASSERT_PARAM(data);
    ASSERT_PARAM(size);

    if (HAL_FLAG_EQU(hi2s->rx_state, HAL_STATE_READY))
    {
        /* Protect the process */
        TAKE_MUTEX(hi2s);

        /* Update the state */
        HAL_FLAG_WRITE(hi2s->rx_state, HAL_STATE_BUSY_RX);
        /* Clear I2S error code */
        CLEAR_ERROR_CODE(hi2s);
        /* Set the function for receive */
        _i2s_xfer_info(hi2s, NULL, data, 0, size);
        /* Enable the I2S */
        I2S_ENABLE_RX_FUC(hi2s);
        I2S_ENABLE_TX_FUC(hi2s); // Due to the hardware design, both TXEN and RXEN must be enabled to maintain normal data transmission.
        PERIPHERAL_I2S_ENABLE(hi2s);
        /* Receive data */
        uint32_t tick_start = HAL_GetTick();
        uint16_t cnt        = I2S_RX_FIFO_DEPTH;
        while (hi2s->rx.xfer_count > 0U)
        {
            cnt = HAL_REG_READ(hi2s->instance->STATUS_b.RFVE);
            if (cnt > 0U)
            {
                if (cnt > hi2s->rx.xfer_count)
                {
                    cnt = hi2s->rx.xfer_count;
                }
                hi2s->I2S_DataRxHandler(hi2s, cnt);
            }
            BREAK_PROC_TIMEOUT(hi2s, tick_start, timeout);
        }

        if (hi2s->error_code == HAL_ERROR_TIMEOUT)
        {
            status = HAL_TIMEOUT;
        }

        /* Update the state */
        HAL_FLAG_WRITE(hi2s->rx_state, HAL_STATE_READY);
        /* Release the process protection */
        GIVE_MUTEX(hi2s);
    }
    else if (HAL_FLAG_EQU(hi2s->rx_state, HAL_STATE_BUSY_RX))
    {
        /* Return busy status */
        status = HAL_BUSY;
    }
    else
    {
        /* Return error status */
        status = HAL_ERROR;
    }

    return status;
}
#endif /* configUSE_I2S_BLOCKING */

#if (configUSE_I2S_NONBLOCKING_IT == 1)
/**
 * @brief  Transmit an amount of data in non-blocking mode with Interrupt
 * @param  hi2s pointer to a I2S_Handle_T structure that contains
 *         the configuration information for I2S module
 * @param  data a 8-bit pointer to data buffer.
 * @param  size number of data sample to be sent:
 * @note   The I2S is kept enabled at the end of transaction to avoid the clock de-synchronization
 *         between Master and Slave(example: audio streaming).
 * @retval HAL status
 */
HAL_Status_T HAL_I2S_Transmit_IT(I2S_Handle_T *hi2s, uint8_t *data, uint16_t size)
{
    HAL_Status_T status = HAL_OK;

    /* Check the I2S handle */
    ASSERT_PARAM(hi2s);
    /* Check the instance */
    ASSERT_PARAM(IS_I2S_INSTANCE(hi2s));
    /* Check the parameters */
    ASSERT_PARAM(data);
    ASSERT_PARAM(size);

    if (HAL_FLAG_EQU(hi2s->state, HAL_STATE_READY))
    {
        /* Protect the process */
        TAKE_MUTEX(hi2s);

        /* Update the state */
        HAL_FLAG_WRITE(hi2s->state, HAL_STATE_BUSY_TX);
        /* Clear I2S error code */
        CLEAR_ERROR_CODE(hi2s);
        /* Set the function for IT transmit */

        if (HAL_REG_READ(hi2s->instance->CTRL2_b.TXEN) == HAL_DISABLE)
        {
            /* start tx dma */
            status = _i2s_it_xfer_kernel(hi2s, data, NULL, size, 0);
        }
        else
        {
            uint32_t ictrl = HAL_REG_READ(hi2s->instance->ICTRL) & ~(I2S_ICTRL_TFTHOD_MASK | I2S_ICTRL_TFTHIEN_MASK);

            HAL_REG_WRITE(hi2s->instance->CTRL2_b.TXFCLR, 1);
            HAL_REG_WRITE(hi2s->instance->CTRL2_b.RESET, 1);

            /* Set TX Data */
            hi2s->tx.buffer_ptr = (void *)data;
            hi2s->tx.xfer_size  = size;
            hi2s->tx.xfer_count = size;

            ictrl |= (1 << I2S_ICTRL_TFTHIEN_BIT) | (I2S_TFTHI_VAL << I2S_ICTRL_TFTHOD_BIT);
            HAL_REG_WRITE(hi2s->instance->ICTRL, ictrl);
        }

        /* Release the process protection */
        GIVE_MUTEX(hi2s);
    }
    else if (HAL_FLAG_EQU(hi2s->state, HAL_STATE_BUSY_TX))
    {
        /* Return busy status */
        status = HAL_BUSY;
    }
    else
    {
        /* Return error status */
        status = HAL_ERROR;
    }

    return status;
}

/**
 * @brief  Receive an amount of data in non-blocking mode with Interrupt
 * @param  hi2s pointer to a I2S_Handle_T structure that contains
 *         the configuration information for I2S module
 * @param  data a 8-bit pointer to the Receive data buffer.
 * @param  size number of data sample to be sent:
 * @note   The I2S is kept enabled at the end of transaction to avoid the clock de-synchronization
 *         between Master and Slave(example: audio streaming).
 * @note   It is recommended to use DMA for the I2S receiver to avoid de-synchronization
 *         between Master and Slave otherwise the I2S interrupt should be optimized.
 * @retval HAL status
 */
HAL_Status_T HAL_I2S_Receive_IT(I2S_Handle_T *hi2s, uint8_t *data, uint16_t size)
{
    HAL_Status_T status = HAL_OK;

    /* Check the I2S handle */
    ASSERT_PARAM(hi2s);
    /* Check the instance */
    ASSERT_PARAM(IS_I2S_INSTANCE(hi2s));
    /* Check the parameters */
    ASSERT_PARAM(data);
    ASSERT_PARAM(size);

    if (HAL_FLAG_EQU(hi2s->rx_state, HAL_STATE_READY))
    {
        /* Protect the process */
        TAKE_MUTEX(hi2s);

        /* Update the state */
        HAL_FLAG_WRITE(hi2s->rx_state, HAL_STATE_BUSY_RX);
        /* Clear I2S error code */
        CLEAR_ERROR_CODE(hi2s);
        /* Set the function for IT receive */

        if (HAL_REG_READ(hi2s->instance->CTRL2_b.TXEN) == HAL_DISABLE)
        {
            /* start tx dma */
            status = _i2s_it_xfer_kernel(hi2s, NULL, data, 0, size);
        }
        else
        {
            uint32_t ictrl = HAL_REG_READ(hi2s->instance->ICTRL) & ~(I2S_ICTRL_RFTHOD_MASK | I2S_ICTRL_RFTHIEN_MASK);

            HAL_REG_WRITE(hi2s->instance->CTRL2_b.RXFCLR, 1);
            HAL_REG_WRITE(hi2s->instance->CTRL2_b.RESET, 1);

            /* Set RX Data */
            hi2s->rx.buffer_ptr = (void *)data;
            hi2s->rx.xfer_size  = size;
            hi2s->rx.xfer_count = size;

            ictrl |= (1 << I2S_ICTRL_RFTHIEN_BIT) | (I2S_RFTHI_VAL << I2S_ICTRL_RFTHOD_BIT);
            HAL_REG_WRITE(hi2s->instance->ICTRL, ictrl);
        }

        /* Release the process protection */
        GIVE_MUTEX(hi2s);
    }
    else if (HAL_FLAG_EQU(hi2s->rx_state, HAL_STATE_BUSY_RX))
    {
        /* Return busy status */
        status = HAL_BUSY;
    }
    else
    {
        /* Return error status */
        status = HAL_ERROR;
    }

    return status;
}
#endif /* configUSE_I2S_NONBLOCKING_IT */

#if (configUSE_I2S_BLOCKING == 1 || configUSE_I2S_NONBLOCKING_IT == 1)
/**
 * @brief  Stop Blocking & IT mode transfer
 * @param  hi2s pointer to a I2S_Handle_T structure that contains
 *              the configuration information for I2S module
 * @return HAL_Status_T
 */
HAL_Status_T HAL_I2S_Stop(I2S_Handle_T *hi2s)
{
    /* Check the I2S handle */
    ASSERT_PARAM(hi2s);
    /* Check the instance */
    ASSERT_PARAM(IS_I2S_INSTANCE(hi2s));
    /* Check the state */
    RET_FLAG_FALSE(HAL_REG_READ(hi2s->instance->CTRL2_b.SPIEN) == HAL_ENABLE, HAL_ERROR);

    /* Disable I2S */
    PERIPHERAL_I2S_DISABLE(hi2s);
    I2S_DISABLE_TX_FUC(hi2s);
    I2S_DISABLE_RX_FUC(hi2s);

    /* clear RIS */
    HAL_REG_READ(hi2s->instance->RIS);

    /* Update the state */
    HAL_FLAG_WRITE(hi2s->state, HAL_STATE_READY);
    HAL_FLAG_WRITE(hi2s->rx_state, HAL_STATE_READY);

    return HAL_OK;
}
#endif /* configUSE_I2S_BLOCKING || configUSE_I2S_NONBLOCKING_IT */

#if (configUSE_I2S_NONBLOCKING_DMA == 1)
/**
 * @brief  Transmit an amount of data in non-blocking mode with DMA
 * @param  hi2s pointer to a I2S_Handle_T structure that contains
 *         the configuration information for I2S module
 * @param  data a 8-bit pointer to the Transmit data buffer.
 * @param  size number of data sample to be sent:
 * @note   The I2S is kept enabled at the end of transaction to avoid the clock de-synchronization
 *         between Master and Slave(example: audio streaming).
 * @retval HAL status
 */
HAL_Status_T HAL_I2S_Transmit_DMA(I2S_Handle_T *hi2s, uint8_t *data, uint16_t size)
{
    HAL_Status_T status = HAL_OK;

    /* Check the I2S handle */
    ASSERT_PARAM(hi2s);
    /* Check the instance */
    ASSERT_PARAM(IS_I2S_INSTANCE(hi2s));
    /* Check the DMA instance */
    ASSERT_PARAM(hi2s->hdmatx);
    /* Check the parameters */
    ASSERT_PARAM(data);
    ASSERT_PARAM(size);

    if (HAL_FLAG_EQU(hi2s->state, HAL_STATE_READY))
    {
        /* Protect the process */
        TAKE_MUTEX(hi2s);

        /* Update the state */
        HAL_FLAG_WRITE(hi2s->state, HAL_STATE_BUSY_TX);
        /* Clear I2S error code */
        CLEAR_ERROR_CODE(hi2s);

        if (HAL_REG_READ(hi2s->instance->CTRL2_b.TXEN) == HAL_DISABLE)
        {
            /* start tx dma */
            status = _i2s_dma_xfer_kernel(hi2s, data, NULL, size, 0);
            if (status == HAL_OK)
            {
                /* Enable the I2S */
                I2S_ENABLE_TX_FUC(hi2s);
                PERIPHERAL_I2S_ENABLE(hi2s);
            }
        }
        else
        {
            uint32_t ictrl = HAL_REG_READ(hi2s->instance->ICTRL);
            ictrl |= (1 << I2S_ICTRL_TFDMAEN_BIT) | (I2S_SET_FIFO_TX_VAL(1) << I2S_ICTRL_TFTHOD_BIT);
            HAL_REG_WRITE(hi2s->instance->ICTRL, ictrl);

            status = HAL_DMA_Start_IT(hi2s->hdmatx, (uint32_t)data, (uint32_t)&hi2s->instance->DATA, size);
        }

        /* Release the process protection */
        GIVE_MUTEX(hi2s);
    }
    else if (HAL_FLAG_EQU(hi2s->state, HAL_STATE_BUSY_TX))
    {
        /* Return busy status */
        status = HAL_BUSY;
    }
    else
    {
        /* Return error status */
        status = HAL_ERROR;
    }

    return status;
}

/**
 * @brief  Receive an amount of data in non-blocking mode with DMA
 * @param  hi2s pointer to a I2S_Handle_T structure that contains
 *         the configuration information for I2S module
 * @param  data a 8-bit pointer to the Receive data buffer.
 * @param  size number of data sample to be sent:
 * @note   The I2S is kept enabled at the end of transaction to avoid the clock de-synchronization
 *         between Master and Slave(example: audio streaming).
 * @retval HAL status
 */
HAL_Status_T HAL_I2S_Receive_DMA(I2S_Handle_T *hi2s, uint8_t *data, uint16_t size)
{
    HAL_Status_T status = HAL_OK;

    /* Check the I2S handle */
    ASSERT_PARAM(hi2s);
    /* Check the instance */
    ASSERT_PARAM(IS_I2S_INSTANCE(hi2s));
    /* Check the DMA instance */
    ASSERT_PARAM(hi2s->hdmarx);
    /* Check the parameters */
    ASSERT_PARAM(data);
    ASSERT_PARAM(size);

    if (HAL_FLAG_EQU(hi2s->rx_state, HAL_STATE_READY))
    {
        /* Protect the process */
        TAKE_MUTEX(hi2s);

        /* Update the state */
        HAL_FLAG_WRITE(hi2s->rx_state, HAL_STATE_BUSY_RX);
        /* Clear I2S error code */
        CLEAR_ERROR_CODE(hi2s);

        if (HAL_REG_READ(hi2s->instance->CTRL2_b.RXEN) == HAL_DISABLE)
        {
            /* start rx dma */
            status = _i2s_dma_xfer_kernel(hi2s, NULL, data, 0, size);
            if (status == HAL_OK)
            {
                /* Enable the I2S */
                I2S_ENABLE_RX_FUC(hi2s);
                PERIPHERAL_I2S_ENABLE(hi2s);
            }
        }
        else
        {
            uint32_t ictrl = HAL_REG_READ(hi2s->instance->ICTRL);
            ictrl |= (1 << I2S_ICTRL_RFDMAEN_BIT) | (I2S_SET_FIFO_RX_VAL(1) << I2S_ICTRL_RFTHOD_BIT);
            HAL_REG_WRITE(hi2s->instance->ICTRL, ictrl);

            status = HAL_DMA_Start_IT(hi2s->hdmarx, (uint32_t)&hi2s->instance->DATA, (uint32_t)data, size);
        }

        /* Release the process protection */
        GIVE_MUTEX(hi2s);
    }
    else if (HAL_FLAG_EQU(hi2s->rx_state, HAL_STATE_BUSY_RX))
    {
        /* Return busy status */
        status = HAL_BUSY;
    }
    else
    {
        /* Return error status */
        status = HAL_ERROR;
    }

    return status;
}

/**
 * @brief  Pause DMA transfer
 * @param  hi2s pointer to a I2S_Handle_T structure that contains
 *         the configuration information for I2S module
 * @retval HAL status
 */
HAL_Status_T HAL_I2S_DMAPause(I2S_Handle_T *hi2s)
{
    /* Check the I2S handle */
    ASSERT_PARAM(hi2s);
    /* Check the instance */
    ASSERT_PARAM(IS_I2S_INSTANCE(hi2s));
    /* Check the DMA instance */
    ASSERT_PARAM(hi2s->hdmatx != NULL || hi2s->hdmarx != NULL);

    /* Protect the process */
    TAKE_MUTEX(hi2s);

    if ((hi2s->hdmatx != NULL) || (hi2s->state == HAL_STATE_BUSY_TX))
    {
        HAL_DMA_Abort(hi2s->hdmatx);
    }
    if ((hi2s->hdmarx != NULL) || (hi2s->rx_state == HAL_STATE_BUSY_RX))
    {
        HAL_DMA_Abort(hi2s->hdmarx);
    }

    /* Release the process protection */
    GIVE_MUTEX(hi2s);

    return HAL_OK;
}

/**
 * @brief  Resume DMA transfer
 * @param  hi2s pointer to a I2S_Handle_T structure that contains
 *         the configuration information for I2S module
 * @retval HAL status
 */
HAL_Status_T HAL_I2S_DMAResume(I2S_Handle_T *hi2s)
{
    /* Check the I2S handle */
    ASSERT_PARAM(hi2s);
    /* Check the instance */
    ASSERT_PARAM(IS_I2S_INSTANCE(hi2s));
    /* Check the DMA instance */
    ASSERT_PARAM(hi2s->hdmatx != NULL || hi2s->hdmarx != NULL);

    /* Protect the process */
    TAKE_MUTEX(hi2s);

    if ((hi2s->hdmatx != NULL) || (hi2s->state == HAL_STATE_BUSY_TX))
    {
        HAL_DMA_Resume_IT(hi2s->hdmatx);
    }
    if ((hi2s->hdmarx != NULL) || (hi2s->rx_state == HAL_STATE_BUSY_RX))
    {
        HAL_DMA_Resume_IT(hi2s->hdmarx);
    }

    /* Release the process protection */
    GIVE_MUTEX(hi2s);

    return HAL_OK;
}

/**
 * @brief  Stop DMA transfer
 * @param  hi2s pointer to a I2S_Handle_T structure that contains
 *         the configuration information for I2S module
 * @retval HAL status
 */
HAL_Status_T HAL_I2S_DMAStop(I2S_Handle_T *hi2s)
{
    /* Check the I2S handle */
    ASSERT_PARAM(hi2s);
    /* Check the instance */
    ASSERT_PARAM(IS_I2S_INSTANCE(hi2s));
    /* Check the DMA instance */
    ASSERT_PARAM(hi2s->hdmatx != NULL || hi2s->hdmarx != NULL);

    /* Protect the process */
    TAKE_MUTEX(hi2s);

    /* Abort DMA */
    _i2s_dma_abort(hi2s);
    I2S_DISABLE_IE(hi2s, I2S_IE_TFDMAEN);
    I2S_DISABLE_IE(hi2s, I2S_IE_RFDMAEN);
    /* Disable I2S */
    PERIPHERAL_I2S_DISABLE(hi2s);
    I2S_DISABLE_TX_FUC(hi2s);
    I2S_DISABLE_RX_FUC(hi2s);
    /* Update the state */
    HAL_FLAG_WRITE(hi2s->state, HAL_STATE_READY);
    HAL_FLAG_WRITE(hi2s->rx_state, HAL_STATE_READY);

    /* Release the process protection */
    GIVE_MUTEX(hi2s);

    return HAL_OK;
}
#endif /* configUSE_I2S_NONBLOCKING_DMA */

/*
================================================================================
            ##### IRQHandler and Event Callback functions #####
================================================================================
*/
#if (configUSE_I2S_NONBLOCKING_IT == 1)
/**
 * @brief  This function handles I2S interrupt request.
 * @param  hi2s pointer to a I2S_Handle_T structure that contains
 *         the configuration information for I2S module
 * @retval None
 */
__weak void ATTR_ISR HAL_I2S_IRQHandler(I2S_Handle_T *hi2s)
{
    uint32_t status = HAL_REG_READ(hi2s->instance->STATUS);
    uint16_t cnt    = 0;

    if (hi2s->rx.buffer_ptr != NULL)
    {
        cnt = (status >> I2S_STATUS_RFVE_BIT) & I2S_STATUS_RFVE_MASK;
        if (cnt > hi2s->rx.xfer_count)
        {
            cnt = hi2s->rx.xfer_count;
        }

        hi2s->I2S_DataRxHandler(hi2s, cnt);

        /* 2023/12/28 If the current RX data is less than the threshold, reset the threshold */
        if (hi2s->rx.xfer_count < I2S_RFTHI_VAL)
        {
            HAL_REG_WRITE(hi2s->instance->ICTRL_b.RFTHOD, hi2s->rx.xfer_count);
        }

        /* Error callback*/
        uint32_t ris = HAL_REG_READ(hi2s->instance->RIS);
        if (HAL_FLAG_HAS(ris, 1 << I2S_RIS_RFORI))
        {
            HAL_FLAG_WRITE(hi2s->error_code, HAL_I2S_ERROR_RX_OVERFLOW);

#if (configUSE_I2S_REGISTER_CALLBACKS == 1)
            if (hi2s->ErrorCallback != NULL)
            {
                hi2s->ErrorCallback(hi2s);
            }
#else
            HAL_I2S_ErrorCallback(hi2s);
#endif
        }
    }

    if (hi2s->tx.buffer_ptr != NULL)
    {
        cnt = I2S_TX_FIFO_DEPTH - (status >> I2S_STATUS_TFVE_BIT) & I2S_STATUS_TFVE_MASK;
        if (cnt > hi2s->tx.xfer_count)
        {
            cnt = hi2s->tx.xfer_count;
        }

        hi2s->I2S_DataTxHandler(hi2s, cnt);

        /* Error callback*/
        uint32_t ris = HAL_REG_READ(hi2s->instance->RIS);
        if (HAL_FLAG_HAS(ris, 1 << I2S_RIS_TFURI))
        {
            HAL_FLAG_WRITE(hi2s->error_code, HAL_I2S_ERROR_TX_UNDERFLOW);

#if (configUSE_I2S_REGISTER_CALLBACKS == 1)
            if (hi2s->ErrorCallback != NULL)
            {
                hi2s->ErrorCallback(hi2s);
            }
#else
            HAL_I2S_ErrorCallback(hi2s);
#endif
        }
    }

    if (hi2s->rx.xfer_size != 0U && hi2s->rx.xfer_count == 0U)
    {
        I2S_DISABLE_IE(hi2s, I2S_IE_RFTHIEN);
        /* Update the state */
        HAL_FLAG_WRITE(hi2s->rx_state, HAL_STATE_READY);

        /* Call user RX complete callback */
#if (configUSE_I2S_REGISTER_CALLBACKS == 1)
        hi2s->RxCpltCallback(hi2s);
#else
        HAL_I2S_RxCpltCallback(hi2s);
#endif
    }
    if (hi2s->tx.xfer_size != 0U && hi2s->tx.xfer_count == 0)
    {
        I2S_DISABLE_IE(hi2s, I2S_IE_TFTHIEN);
        /* Update the state */
        HAL_FLAG_WRITE(hi2s->state, HAL_STATE_READY);

        /* Call user TX complete callback */
#if (configUSE_I2S_REGISTER_CALLBACKS == 1)
        hi2s->TxCpltCallback(hi2s);
#else
        HAL_I2S_TxCpltCallback(hi2s);
#endif
    }
}
#endif

/**
 * @brief  I2S Tx Transfer Completed Callback.
 * @param  hi2s Pointer to a I2S_Handle_T structure that contains the configuration information for the specified I2S module.
 * @retval None
 */
__weak ATTR_ISR void HAL_I2S_TxCpltCallback(I2S_Handle_T *hi2s)
{
    /* Prevent unused argument(s) compilation warning */
    UNUSED(hi2s);

    /* NOTE: This function should not be modified, when the callback is needed, the HAL_I2S_TxCpltCallback could be implemented in the user file */
}

/**
 * @brief  I2S Rx Transfer Completed Callback.
 * @param  hi2s Pointer to a I2S_Handle_T structure that contains the configuration information for the specified I2S module.
 * @retval None
 */
__weak ATTR_ISR void HAL_I2S_RxCpltCallback(I2S_Handle_T *hi2s)
{
    /* Prevent unused argument(s) compilation warning */
    UNUSED(hi2s);

    /* NOTE: This function should not be modified, when the callback is needed, the HAL_I2S_RxCpltCallback could be implemented in the user file */
}

/**
 * @brief  I2S Error Callback.
 * @param  hi2s Pointer to a I2S_Handle_T structure that contains the configuration information for the specified I2S module.
 * @retval None
 */
__weak ATTR_ISR void HAL_I2S_ErrorCallback(I2S_Handle_T *hi2s)
{
    /* Prevent unused argument(s) compilation warning */
    UNUSED(hi2s);

    /* NOTE: This function should not be modified, when the callback is needed, the HAL_I2S_ErrorCallback could be implemented in the user file */
}

/* Private functions ---------------------------------------------------------*/
static HAL_Status_T _i2s_set_config(I2S_Handle_T *hi2s)
{
    /* switch to i2s mode */
    HAL_REG_WRITE(hi2s->instance->CTRL0_b.FFMT, 3);

    /* data frame format */
    HAL_REG_WRITE(hi2s->instance->CTRL0_b.LBM, 0);

    /* Master/Slaver mode select */
    switch (hi2s->init.mode)
    {
        case I2S_MODE_MASTER: /* i2s Master, default */
        {
            HAL_REG_WRITE(hi2s->instance->CTRL0_b.FSFDBK, 1);
            HAL_REG_WRITE(hi2s->instance->CTRL0_b.SCLKFDBK, 1);
            HAL_REG_WRITE(hi2s->instance->CTRL0_b.OPM, ((hi2s->init.mode << 1) | hi2s->init.channel));
        }
        break;
        case I2S_MODE_SLAVE: /* i2s Slave */
        {
            HAL_REG_WRITE(hi2s->instance->CTRL0_b.OPM, ((hi2s->init.mode << 1) | hi2s->init.channel));
        }
        break;

        default:
            break;
    }

    /* justify mode select */
    HAL_REG_WRITE(hi2s->instance->CTRL0_b.FSPO, hi2s->init.cpol);
    switch (hi2s->init.standard)
    {
        case I2S_STANDARD_PHILIPS:
        {
            HAL_REG_WRITE(hi2s->instance->CTRL0_b.FSJSTFY, 0);
            HAL_REG_WRITE(hi2s->instance->CTRL0_b.FSDIST, 1);
        }
        break;

        case I2S_STANDARD_LSB:
        {
            HAL_REG_WRITE(hi2s->instance->CTRL0_b.FSJSTFY, 1);
            HAL_REG_WRITE(hi2s->instance->CTRL0_b.FSDIST, 0);
        }
        break;

        case I2S_STANDARD_MSB:
        {
            HAL_REG_WRITE(hi2s->instance->CTRL0_b.FSJSTFY, 0);
            HAL_REG_WRITE(hi2s->instance->CTRL0_b.FSDIST, 0);
        }
        break;

        default:
            break;
    }

    /* i2s word length */
    uint32_t sdl = hi2s->init.data_format;
    uint32_t pdl = hi2s->init.channel_len - sdl;
    HAL_REG_WRITE(hi2s->instance->CTRL1_b.PDL, pdl);
    HAL_REG_WRITE(hi2s->instance->CTRL1_b.SDL, sdl - 1);

    /* i2s sample clock */
    uint8_t i2s_index = 0;
    switch ((uint32_t)hi2s->instance)
    {
        case SN_SPI0_BASE:
        {
            i2s_index = 0;
        }
        break;

        case SN_SPI1_BASE:
        {
            i2s_index = 1;
        }
        break;

        case SN_SPI2_BASE:
        {
            i2s_index = 2;
        }
        break;
    }
    uint32_t source_clock = 0;
    if (__HAL_RCC_GET_SSPn_CLKSEL(i2s_index) == RCC_SPI_CLKSEL_HCLK)
    {
        source_clock = HAL_RCC_GetHCLKFreq();
    }
    else // RCC_SPI_CLKSEL_PLLCLK
    {
        source_clock = HAL_RCC_GetPLLCLKFreq();
    }
    uint32_t rel_sclk = hi2s->init.audio_freq * hi2s->init.channel_len * 2;
    uint32_t div_sclk = (source_clock / rel_sclk - 2) >> 1;

    /* div should >= 2 */
    if (div_sclk < 2)
    {
        return HAL_ERROR;
    }

    /* 2024/01/24 verify if satisfying the inequation "(3.5 x Tpclk) < (Tsclk x SDL_Length)" */
    uint32_t pclk = HAL_RCC_GetAPB1CLKFreq();
    if (7 * rel_sclk >= 2 * pclk * hi2s->init.data_format)
    {
        return HAL_ERROR;
    }

#if (configUSE_I2S_BLOCKING == 1 || configUSE_I2S_NONBLOCKING_IT == 1)
    uint32_t data_bits = HAL_REG_READ(hi2s->instance->CTRL1_b.SDL) + 1;
    uint8_t  data_size = data_bits / 8;
    if (data_bits % 8 != 0)
    {
        data_size++;
    }
    switch (data_size)
    {
        case 1:
        {
            hi2s->I2S_DataTxHandler = _i2s_write_data_1b;
            hi2s->I2S_DataRxHandler = _i2s_read_data_1b;
        }
        break;

        case 2:
        {
            hi2s->I2S_DataTxHandler = _i2s_write_data_2b;
            hi2s->I2S_DataRxHandler = _i2s_read_data_2b;
        }
        break;

        case 3:
        {
            hi2s->I2S_DataTxHandler = _i2s_write_data_3b;
            hi2s->I2S_DataRxHandler = _i2s_read_data_3b;
        }
        break;

        case 4:
        {
            hi2s->I2S_DataTxHandler = _i2s_write_data_4b;
            hi2s->I2S_DataRxHandler = _i2s_read_data_4b;
        }
        break;

        default:
        {
            hi2s->I2S_DataTxHandler = _i2s_write_data_1b;
            hi2s->I2S_DataRxHandler = _i2s_read_data_1b;
        }
        break;
    }
#endif /* configUSE_I2S_BLOCKING == 1 || configUSE_I2S_NONBLOCKING_IT == 1 */
    HAL_REG_WRITE(hi2s->instance->CTRL1_b.SCLKDIV, div_sclk);
    HAL_REG_WRITE(hi2s->instance->CTRL2_b.TXDOE, 1);

    /* i2s DPDL setting */
    HAL_REG_WRITE(hi2s->instance->CTRL3_b.DPDLEN, 0);
    HAL_REG_WRITE(hi2s->instance->CTRL3_b.DPDL, 4);

    /* Clear fifo status */
    HAL_REG_WRITE(hi2s->instance->CTRL2_b.RESET, 1);
    I2S_ENABLE_TX_FIFO(hi2s);
    I2S_ENABLE_RX_FIFO(hi2s);
    I2S_RESET_FIFO(hi2s);

    return HAL_OK;
}

#if (configUSE_I2S_REGISTER_CALLBACKS == 1)
static void _i2s_init_callbacks_to_default(I2S_Handle_T *hi2s)
{
    hi2s->TxCpltCallback = HAL_I2S_TxCpltCallback;
    hi2s->RxCpltCallback = HAL_I2S_RxCpltCallback;
    hi2s->ErrorCallback  = HAL_I2S_ErrorCallback;
}
#endif /* configUSE_I2S_REGISTER_CALLBACKS */

#if (configUSE_I2S_NONBLOCKING_IT == 1)
static HAL_Status_T _i2s_it_xfer_kernel(I2S_Handle_T *hi2s, uint8_t *tx_data, uint8_t *rx_data, uint32_t tx_size, uint32_t rx_size)
{
    uint32_t ictrl = 0;
    uint32_t ctrl2 = HAL_REG_READ(hi2s->instance->CTRL2) & ~(I2S_CTRL2_SPIEN_MASK | I2S_CTRL2_TXEN_MASK | I2S_CTRL2_RXEN_MASK);
    HAL_REG_WRITE(hi2s->instance->CTRL2, ctrl2);

    /* set xfer info */
    _i2s_xfer_info(hi2s, tx_data, rx_data, tx_size, rx_size);

    /* Set the function for rx */
    if (hi2s->rx.xfer_size)
    {
        ictrl = HAL_REG_READ(hi2s->instance->ICTRL) & ~(I2S_ICTRL_RFTHOD_MASK | I2S_ICTRL_RFTHIEN_MASK);
        HAL_REG_WRITE(hi2s->instance->ICTRL, ictrl);

        HAL_REG_WRITE((hi2s)->instance->ICTRL_b.RFTHOD_UNIT, 0);
        if (hi2s->rx.xfer_size >= I2S_RFTHI_VAL)
        {
            ictrl |= (1 << I2S_ICTRL_RFTHIEN_BIT) | (I2S_RFTHI_VAL << I2S_ICTRL_RFTHOD_BIT);
        }
        else
        {
            ictrl |= (1 << I2S_ICTRL_RFTHIEN_BIT) | (hi2s->rx.xfer_size << I2S_ICTRL_RFTHOD_BIT);
        }
    }

    /* Set the function for tx */
    if (hi2s->tx.xfer_size)
    {
        ictrl = HAL_REG_READ(hi2s->instance->ICTRL) & ~(I2S_ICTRL_TFTHOD_MASK | I2S_ICTRL_TFTHIEN_MASK);
        HAL_REG_WRITE(hi2s->instance->ICTRL, ictrl);

        uint32_t _width = HAL_REG_READ(hi2s->instance->CTRL1_b.SDL) + 1;

        if (hi2s->instance->CTRL0_b.OPM < 2U) // Slave
        {
            if (_width == I2S_DATA_LEN_8B)
            {
                HAL_REG_WRITE(hi2s->instance->DATA, hi2s->tx.buffer_ptr[0]);
                hi2s->tx.buffer_ptr++;
            }
            else if (_width == I2S_DATA_LEN_16B)
            {
                HAL_REG_WRITE(hi2s->instance->DATA, ((uint16_t *)hi2s->tx.buffer_ptr)[0]);
                hi2s->tx.buffer_ptr += 2;
            }
            else if (_width == I2S_DATA_LEN_24B)
            {
                HAL_REG_WRITE(hi2s->instance->DATA, ((uint32_t *)hi2s->tx.buffer_ptr)[0]);
                hi2s->tx.buffer_ptr += 3;
            }
            else // (_width == I2S_DATA_LEN_32B)
            {
                HAL_REG_WRITE(hi2s->instance->DATA, ((uint32_t *)hi2s->tx.buffer_ptr)[0]);
                hi2s->tx.buffer_ptr += 4;
            }
            hi2s->tx.xfer_count--;
        }

        ictrl |= (1 << I2S_ICTRL_TFTHIEN_BIT) | (I2S_TFTHI_VAL << I2S_ICTRL_TFTHOD_BIT);
    }

    ctrl2 |= I2S_CTRL2_TXEN_MASK | I2S_CTRL2_RXEN_MASK;
    HAL_REG_WRITE(hi2s->instance->CTRL2, ctrl2);
    HAL_REG_WRITE(hi2s->instance->ICTRL, ictrl);
    PERIPHERAL_I2S_ENABLE(hi2s);
    return HAL_OK;
}
#endif /* configUSE_I2S_NONBLOCKING_IT */

#if (configUSE_I2S_NONBLOCKING_DMA == 1)
static HAL_Status_T _i2s_dma_xfer_kernel(I2S_Handle_T *hi2s, uint8_t *tx_data, uint8_t *rx_data, uint32_t tx_size, uint32_t rx_size)
{
    HAL_Status_T status = HAL_OK;
    uint32_t     ictrl  = 0;
    uint32_t     ctrl2  = HAL_REG_READ(hi2s->instance->CTRL2) & ~(I2S_CTRL2_SPIEN_MASK | I2S_CTRL2_TXEN_MASK | I2S_CTRL2_RXEN_MASK);
    HAL_REG_WRITE(hi2s->instance->CTRL2, ctrl2);

    /* set xfer info */
    _i2s_xfer_info(hi2s, tx_data, rx_data, tx_size, rx_size);

    if (rx_size > 0U)
    {
        /* clear I2S DMA RX config */
        ictrl = HAL_REG_READ(hi2s->instance->ICTRL) & ~(I2S_ICTRL_RFTHOD_MASK | I2S_ICTRL_RFDMAEN_MASK);
        HAL_REG_WRITE(hi2s->instance->ICTRL, ictrl);

        status = HAL_DMA_Start_IT(hi2s->hdmarx, (uint32_t)&hi2s->instance->DATA, (uint32_t)rx_data, rx_size);
        if (status != HAL_OK)
        {
            /* Update the state */
            HAL_FLAG_WRITE(hi2s->rx_state, HAL_STATE_READY);
            HAL_FLAG_WRITE(hi2s->error_code, HAL_I2S_ERROR_DMA);
            return status;
        }
        ictrl |= (1 << I2S_ICTRL_RFDMAEN_BIT) | (I2S_SET_FIFO_RX_VAL(1) << I2S_ICTRL_RFTHOD_BIT);
    }

    if (tx_size > 0U)
    {
        ictrl = HAL_REG_READ(hi2s->instance->ICTRL) & ~(I2S_ICTRL_TFTHOD_MASK | I2S_ICTRL_TFDMAEN_MASK);
        HAL_REG_WRITE(hi2s->instance->ICTRL, ictrl);

        uint32_t _width   = HAL_REG_READ(hi2s->instance->CTRL1_b.SDL) + 1;
        uint8_t *buff_ptr = tx_data;
        if (hi2s->instance->CTRL0_b.OPM < 2U) // Slave
        {
            if (_width == I2S_DATA_LEN_8B)
            {
                HAL_REG_WRITE(hi2s->instance->DATA, tx_data[0]);
                buff_ptr++;
            }
            else if (_width == I2S_DATA_LEN_16B)
            {
                HAL_REG_WRITE(hi2s->instance->DATA, ((uint16_t *)buff_ptr)[0]);
                buff_ptr += 2;
            }
            else if (_width == I2S_DATA_LEN_24B)
            {
                HAL_REG_WRITE(hi2s->instance->DATA, ((uint32_t *)buff_ptr)[0]);
                buff_ptr += 3;
            }
            else if (_width == I2S_DATA_LEN_32B)
            {
                HAL_REG_WRITE(hi2s->instance->DATA, ((uint32_t *)buff_ptr)[0]);
                buff_ptr += 4;
            }

            if (tx_size > 1)
            {
                status = HAL_DMA_Start_IT(hi2s->hdmatx, (uint32_t)(buff_ptr), (uint32_t)&hi2s->instance->DATA, tx_size - 1);
            }
        }
        else
        {
            status = HAL_DMA_Start_IT(hi2s->hdmatx, (uint32_t)(buff_ptr), (uint32_t)&hi2s->instance->DATA, tx_size);
        }

        if (status != HAL_OK)
        {
            /* Update the state */
            HAL_FLAG_WRITE(hi2s->state, HAL_STATE_READY);
            HAL_FLAG_WRITE(hi2s->error_code, HAL_I2S_ERROR_DMA);
            return status;
        }
        ictrl |= (1 << I2S_ICTRL_TFDMAEN_BIT) | (I2S_SET_FIFO_TX_VAL(1) << I2S_ICTRL_TFTHOD_BIT);
    }

    /* Write ICTRL lastly */
    ctrl2 |= I2S_CTRL2_TXEN_MASK | I2S_CTRL2_RXEN_MASK;
    HAL_REG_WRITE(hi2s->instance->ICTRL, ictrl);
    HAL_REG_WRITE(hi2s->instance->CTRL2, ctrl2);
    PERIPHERAL_I2S_ENABLE(hi2s);
    return status;
}

static void _i2s_dma_abort(I2S_Handle_T *hi2s)
{
    if (hi2s->hdmatx != NULL)
    {
        I2S_DISABLE_IE(hi2s, I2S_IE_TFDMAEN);
        HAL_DMA_Abort(hi2s->hdmatx);
    }

    if (hi2s->hdmarx != NULL)
    {
        I2S_DISABLE_IE(hi2s, I2S_IE_RFDMAEN);
        HAL_DMA_Abort(hi2s->hdmarx);
    }
}

static void _i2s_tx_dma_cb(DMA_Handle_T *hdma)
{
    I2S_Handle_T *hi2s  = (I2S_Handle_T *)hdma->parent;
    hi2s->tx.xfer_count = 0;
    HAL_FLAG_WRITE(hi2s->state, HAL_STATE_READY);

#if (configUSE_I2S_REGISTER_CALLBACKS == 1)
    if (hi2s->TxCpltCallback != NULL)
    {
        hi2s->TxCpltCallback(hi2s);
    }
#else
    HAL_I2S_TxCpltCallback(hi2s);
#endif
}

static void _i2s_rx_dma_cb(DMA_Handle_T *hdma)
{
    I2S_Handle_T *hi2s  = (I2S_Handle_T *)hdma->parent;
    hi2s->rx.xfer_count = 0;
    HAL_FLAG_WRITE(hi2s->rx_state, HAL_STATE_READY);

#if (configUSE_I2S_REGISTER_CALLBACKS == 1)
    if (hi2s->RxCpltCallback != NULL)
    {
        hi2s->RxCpltCallback(hi2s);
    }
#else
    HAL_I2S_RxCpltCallback(hi2s);
#endif
}

static void _i2s_tx_dma_err_cb(DMA_Handle_T *hdma)
{
    I2S_Handle_T *hi2s = (I2S_Handle_T *)hdma->parent;
    /* Abort DMA */
    _i2s_dma_abort(hi2s);
    /* Disable I2S */
    PERIPHERAL_I2S_DISABLE(hi2s);
    I2S_DISABLE_TX_FUC(hi2s);
    /* Update the state */
    HAL_FLAG_WRITE(hi2s->error_code, HAL_I2S_ERROR_DMA_TX);
    HAL_FLAG_WRITE(hi2s->state, HAL_STATE_READY);

#if (configUSE_I2S_REGISTER_CALLBACKS == 1)
    if (hi2s->ErrorCallback != NULL)
    {
        hi2s->ErrorCallback(hi2s);
    }
#else
    HAL_I2S_ErrorCallback(hi2s);
#endif
}

static void _i2s_rx_dma_err_cb(DMA_Handle_T *hdma)
{
    I2S_Handle_T *hi2s = (I2S_Handle_T *)hdma->parent;
    /* Abort DMA */
    _i2s_dma_abort(hi2s);
    /* Disable I2S */
    PERIPHERAL_I2S_DISABLE(hi2s);
    I2S_DISABLE_RX_FUC(hi2s);
    /* Update the state */
    HAL_FLAG_WRITE(hi2s->error_code, HAL_I2S_ERROR_DMA_RX);
    HAL_FLAG_WRITE(hi2s->rx_state, HAL_STATE_READY);

#if (configUSE_I2S_REGISTER_CALLBACKS == 1)
    if (hi2s->ErrorCallback != NULL)
    {
        hi2s->ErrorCallback(hi2s);
    }
#else
    HAL_I2S_ErrorCallback(hi2s);
#endif
}
#endif /* configUSE_I2S_NONBLOCKING_DMA */

#endif /* configUSE_I2S_PERIPHERAL */
