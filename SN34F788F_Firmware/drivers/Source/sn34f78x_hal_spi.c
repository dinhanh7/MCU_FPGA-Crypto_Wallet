/***********************
 * SN34F78X SDK V2.1.4 *
 ***********************/

#include "sn34f78x_hal_spi.h"

#if (configUSE_SPI_PERIPHERAL == 1U)
/*
================================================================================
            ##### Initialization/De-initialization functions #####
================================================================================
*/
/**
 * @brief  Initializes the SPI base Unit according to the specified parameters in the SPI_Handle_T and initialize the associated handle.
 * @param  hspi pointer to a SPI_Handle_T structure that contains the configuration information for SPI module.
 * @retval HAL_Status_T
 */
HAL_Status_T HAL_SPI_Init(SPI_Handle_T *hspi)
{
    /* Check the SPI handle allocation */
    ASSERT_PARAM(hspi);

    /* Check the instance */
    ASSERT_PARAM(IS_SPI_INSTANCE(hspi));

    /* Check the init parameters */
    ASSERT_PARAM(IS_SPI_FFMT(hspi));
    ASSERT_PARAM(IS_SPI_MODE(hspi));
    ASSERT_PARAM(IS_SPI_DATA_LEN(hspi));
    ASSERT_PARAM(IS_SPI_FS_POL(hspi));
    ASSERT_PARAM(IS_SPI_CLK_POL(hspi));
    ASSERT_PARAM(IS_SPI_CLK_PHA(hspi));
    ASSERT_PARAM(IS_SPI_MSB_LSB(hspi));

    // Slave mode is not necessary for div setting
    if (hspi->init.mode == SPI_MODE_MASTER)
    {
        if (hspi->instance == SN_SPI0)
        {
            ASSERT_PARAM(IS_SPI0_CLK_DIV(hspi));
        }
        else
        {
            ASSERT_PARAM(IS_SPI12_CLK_DIV(hspi));
        }
    }

    /* clear error code*/
    CLEAR_ERROR_CODE(hspi);

    if (HAL_FLAG_HAS(hspi->state, HAL_STATE_RESET))
    {
        /* Allocate lock resource and initialize it */
        GIVE_MUTEX(hspi);

#if (configUSE_SPI_REGISTER_CALLBACKS == 1)
        /* Reset interrupt callbacks to legacy weak callbacks */
        _spi_reset_callback(hspi);

        /* Init default callback */
        if (hspi->MspInitCallback == NULL)
        {
            hspi->MspInitCallback = HAL_SPI_MspInit;
        }

        /* Init the low level hardware : GPIO, CLOCK, NVIC */
        (hspi->MspInitCallback)(hspi);
#else
        /* Init the low level hardware : GPIO, CLOCK, NVIC */
        HAL_SPI_MspInit(hspi);
#endif /* configUSE_SPI_REGISTER_CALLBACKS */

#if (configUSE_SPI_NONBLOCKING_DMA == 1U)
        /* register tx dma callback */
        if (hspi->hdmatx)
        {
            hspi->hdmatx->parent = (void *)hspi;
            HAL_DMA_RegisterCallback(hspi->hdmatx, HAL_DMA_XFER_CPLT_CB_ID, _spi_tx_dma_cb);
            HAL_DMA_RegisterCallback(hspi->hdmatx, HAL_DMA_XFER_ERROR_CB_ID, _spi_tx_dma_err_cb);
        }

        /* register rx dma callback */
        if (hspi->hdmarx)
        {
            hspi->hdmarx->parent = (void *)hspi;
            HAL_DMA_RegisterCallback(hspi->hdmarx, HAL_DMA_XFER_CPLT_CB_ID, _spi_rx_dma_cb);
            HAL_DMA_RegisterCallback(hspi->hdmarx, HAL_DMA_XFER_ERROR_CB_ID, _spi_rx_dma_err_cb);
        }
#endif
    }
    else
    {
        hspi->error_code = HAL_ERROR_REINIT;
        return HAL_ERROR;
    }

    /* Set the SPI configuration */
    HAL_FLAG_WRITE(hspi->error_code, _spi_set_config(hspi));
    RET_FLAG_TRUE(hspi->error_code, HAL_ERROR);

    /* Enable the SPI function */
    HAL_REG_WRITE(hspi->instance->CTRL2_b.SPIEN, 1);

    /* Change HAL SPI state to READY */
    HAL_FLAG_WRITE(hspi->state, HAL_STATE_READY);

    return HAL_OK;
}

/**
 * @brief  DeInitializes the SPI peripheral
 * @param  hspi SPI handle
 * @retval HAL_Status_T
 */
HAL_Status_T HAL_SPI_DeInit(SPI_Handle_T *hspi)
{
    /* Check the SPI handle allocation */
    ASSERT_PARAM(hspi);

    /* Check the parameters */
    ASSERT_PARAM(IS_SPI_INSTANCE(hspi));

    /* Change SPI state to BUSY */
    HAL_FLAG_WRITE(hspi->state, HAL_STATE_BUSY);

    /* Disable the SPI function */
    HAL_FLAG_WRITE(hspi->state_ex, SPI_STATE_EX_NONE);
    HAL_REG_CLEAR(hspi->instance->CTRL2);

    /* Change SPI state to RESET */
    HAL_FLAG_WRITE(hspi->state, HAL_STATE_RESET);

#if (configUSE_SPI_REGISTER_CALLBACKS == 1U)
    /* Init default callback */
    if (hspi->MspDeInitCallback == NULL)
    {
        hspi->MspDeInitCallback = HAL_SPI_MspDeInit;
    }

    /* DeInit the low level hardware: GPIO, CLOCK, NVIC */
    (hspi->MspDeInitCallback)(hspi);
#else
    /* DeInit the low level hardware: GPIO, CLOCK, NVIC */
    HAL_SPI_MspDeInit(hspi);
#endif /* configUSE_SPI_REGISTER_CALLBACKS */

#if (configUSE_SPI_NONBLOCKING_DMA == 1U)
    if (hspi->hdmatx)
    {
        hspi->hdmatx->parent = NULL;
        HAL_DMA_UnRegisterCallback(hspi->hdmatx, HAL_DMA_XFER_CPLT_CB_ID);
        HAL_DMA_UnRegisterCallback(hspi->hdmatx, HAL_DMA_XFER_ERROR_CB_ID);
        hspi->hdmatx = NULL;
    }

    if (hspi->hdmarx)
    {
        hspi->hdmarx->parent = NULL;
        HAL_DMA_UnRegisterCallback(hspi->hdmarx, HAL_DMA_XFER_CPLT_CB_ID);
        HAL_DMA_UnRegisterCallback(hspi->hdmarx, HAL_DMA_XFER_ERROR_CB_ID);
        hspi->hdmarx = NULL;
    }
#endif

    /* Release Mutex */
    GIVE_MUTEX(hspi);

    return HAL_OK;
}

/**
 * @brief  Initializes SPI MSP.
 * @param  hspi SPI handle
 * @retval None
 */
__weak void HAL_SPI_MspInit(SPI_Handle_T *hspi)
{
    /* Prevent unused argument(s) compilation warning */
    UNUSED(hspi);

    /* NOTE : This function should not be modified, when the callback is needed,
              the HAL_SPI_MspInit could be implemented in the user file
     */
}

/**
 * @brief  DeInitializes SPI MSP.
 * @param  hspi SPI handle
 * @retval None
 */
__weak void HAL_SPI_MspDeInit(SPI_Handle_T *hspi)
{
    /* Prevent unused argument(s) compilation warning */
    UNUSED(hspi);

    /* NOTE : This function should not be modified, when the callback is needed,
              the HAL_SPI_MspDeInit could be implemented in the user file
     */
}

#if (configUSE_SPI_REGISTER_CALLBACKS == 1U)
/**
 * @brief  Register a User SPI Callback
 *         To be used instead of the weak predefined callback
 * @param  hspi  Pointer to a SPI_Handle_T structure that contains
 *               the configuration information for the specified SPI.
 * @param  cb_id ID of the callback to be registered
 * @param  pcb pointer to the Callback function
 * @return HAL_Status_T HAL status
 */
HAL_Status_T HAL_SPI_RegisterCallback(SPI_Handle_T *hspi, HAL_SPI_CallbackID_T cb_id, SPI_Callback_T pcb)
{
    HAL_Status_T status = HAL_OK;

    /* Check the valid of SPI handle */
    ASSERT_PARAM(hspi);

    if (pcb == NULL)
    {
        return HAL_ERROR;
    }

    /* Process locked */
    TAKE_MUTEX(hspi);

    if (HAL_FLAG_EQU(hspi->state, HAL_STATE_READY))
    {
        switch (cb_id)
        {
            case HAL_SPI_MSP_INIT_CB_ID:
                hspi->MspInitCallback = pcb;
                break;
            case HAL_SPI_MSP_DEINIT_CB_ID:
                hspi->MspDeInitCallback = pcb;
                break;
            case HAL_SPI_TX_COMPLETE_CB_ID:
                hspi->TxCpltCallback = pcb;
                break;
            case HAL_SPI_RX_COMPLETE_CB_ID:
                hspi->RxCpltCallback = pcb;
                break;
            case HAL_SPI_TX_RX_COMPLETE_CB_ID:
                hspi->TxRxCpltCallback = pcb;
                break;
            case HAL_SPI_ERROR_CB_ID:
                hspi->ErrorCallback = pcb;
                break;
            case HAL_SPI_ABORT_CB_ID:
                hspi->AbortCpltCallback = pcb;
                break;
            default:
                /* Update the error code */
                hspi->error_code = HAL_SPI_ERROR_INVALID_CALLBACK;
                /* Return error status */
                status = HAL_ERROR;
                break;
        }
    }
    else if (HAL_FLAG_EQU(hspi->state, HAL_STATE_RESET))
    {
        switch (cb_id)
        {
            case HAL_SPI_MSP_INIT_CB_ID:
                hspi->MspInitCallback = pcb;
                break;
            case HAL_SPI_MSP_DEINIT_CB_ID:
                hspi->MspDeInitCallback = pcb;
                break;
            default:
                /* Update the error code */
                hspi->error_code = HAL_SPI_ERROR_INVALID_CALLBACK;
                /* Return error status */
                status = HAL_ERROR;
                break;
        }
    }
    else
    {
        status = HAL_ERROR;
    }

    /* Free the mutex */
    GIVE_MUTEX(hspi);

    return status;
}

/**
 * @brief  Unregister an SPI Callback
 *         SPI callback is redirected to the weak predefined callback
 * @param  hspi  Pointer to a SPI_Handle_T structure that contains
 *               the configuration information for the specified SPI.
 * @param  cb_id ID of the callback to be unregistered
 * @return HAL_Status_T
 */
HAL_Status_T HAL_SPI_UnRegisterCallback(SPI_Handle_T *hspi, HAL_SPI_CallbackID_T cb_id)
{
    HAL_Status_T status = HAL_OK;

    /* Check the valid of SPI handle */
    ASSERT_PARAM(hspi);

    /* Process locked */
    TAKE_MUTEX(hspi);

    if (HAL_FLAG_EQU(hspi->state, HAL_STATE_READY))
    {
        switch (cb_id)
        {
            case HAL_SPI_MSP_INIT_CB_ID:
                hspi->MspInitCallback = HAL_SPI_MspInit;
                break;
            case HAL_SPI_MSP_DEINIT_CB_ID:
                hspi->MspDeInitCallback = HAL_SPI_MspDeInit;
                break;
            case HAL_SPI_TX_COMPLETE_CB_ID:
                hspi->TxCpltCallback = HAL_SPI_TxCpltCallback;
                break;
            case HAL_SPI_RX_COMPLETE_CB_ID:
                hspi->RxCpltCallback = HAL_SPI_RxCpltCallback;
                break;
            case HAL_SPI_TX_RX_COMPLETE_CB_ID:
                hspi->TxRxCpltCallback = HAL_SPI_TxRxCpltCallback;
                break;
            case HAL_SPI_ERROR_CB_ID:
                hspi->ErrorCallback = HAL_SPI_ErrorCallback;
                break;
            case HAL_SPI_ABORT_CB_ID:
                hspi->AbortCpltCallback = HAL_SPI_AbortCpltCallback;
                break;
            default:
                /* Update the error code */
                hspi->error_code = HAL_SPI_ERROR_INVALID_CALLBACK;
                /* Return error status */
                status = HAL_ERROR;
                break;
        }
    }
    else if (HAL_FLAG_EQU(hspi->state, HAL_STATE_RESET))
    {
        switch (cb_id)
        {
            case HAL_SPI_MSP_INIT_CB_ID:
                hspi->MspInitCallback = HAL_SPI_MspInit;
                break;
            case HAL_SPI_MSP_DEINIT_CB_ID:
                hspi->MspDeInitCallback = HAL_SPI_MspDeInit;
                break;
            default:
                /* Update the error code */
                hspi->error_code = HAL_SPI_ERROR_INVALID_CALLBACK;
                /* Return error status */
                status = HAL_ERROR;
                break;
        }
    }
    else
    {
        status = HAL_ERROR;
    }

    /* Free the mutex */
    GIVE_MUTEX(hspi);

    return status;
}
#endif /* configUSE_SPI_REGISTER_CALLBACKS */

/**
 * @brief Return the SPI handle state.
 *
 * @param hspi pointer to a SPI_Handle_T structure that contains
 *             the configuration information for SPI module.
 * @return SPI_State_T
 */
HAL_State_T HAL_SPI_GetState(SPI_Handle_T *hspi)
{
    /* Check the valid of SPI handle */
    ASSERT_PARAM(hspi);
    return hspi->state;
}

/**
 * @brief Return the SPI error code.
 *
 * @param hspi pointer to a SPI_Handle_T structure that contains
 *             the configuration information for SPI module.
 * @return Error codes
 */
uint32_t HAL_SPI_GetError(SPI_Handle_T *hspi)
{
    /* Check the valid of SPI handle */
    ASSERT_PARAM(hspi);
    return hspi->error_code;
}
/*
================================================================================
            ##### I/O Operation functions #####
================================================================================
*/
#if (configUSE_SPI_BLOCKING == 1U)
/**
 * @brief Transmit an amount of data in blocking mode.
 * @param hspi pointer to a SPI_Handle_T structure that contains
 *             the configuration information for SPI module.
 * @param data pointer to data buffer
 * @param size amount of data to be sent
 * @param timeout Timeout duration
 * @return HAL_Status_T
 */
HAL_Status_T HAL_SPI_Transmit(SPI_Handle_T *hspi, uint8_t *data, uint32_t size, uint32_t timeout)
{
    HAL_Status_T status = HAL_OK;

    /* Check parameters */
    ASSERT_PARAM(hspi);
    ASSERT_PARAM(data);
    ASSERT_PARAM(size);
    ASSERT_PARAM(!((hspi->base == 1) && (size & 1)));

    /* Check whether SPI is ready */
    RET_FLAG_TRUE(HAL_FLAG_LACKS(hspi->state, HAL_STATE_READY), HAL_ERROR);
    RET_FLAG_TRUE(HAL_FLAG_HAS(hspi->state, HAL_STATE_BUSY), HAL_BUSY);

    /* Process locked */
    TAKE_MUTEX(hspi);

    /* Change SPI peripheral state */
    HAL_FLAG_WRITE(hspi->state, HAL_STATE_BUSY_TX);
    /* Clear SPI error_code */
    CLEAR_ERROR_CODE(hspi);
    /* Set the function for transmit */
    _spi_xfer_kernel(hspi, data, NULL, size, 0);

    /* Transmit data */
    uint32_t tick_start = HAL_GetTick(); /* Init tick_start for timeout management */
    uint32_t cnt        = SPI_TX_FIFO_DEPTH;

    while (hspi->tx.xfer_count > 0)
    {
        if (IS_TX_FIFO_EMPTY(hspi))
        {
            /* Data to be transmitted in every round can't be greater than remaining data count to be transmitted */
            if (cnt > hspi->tx.xfer_count)
            {
                /* Or we need adjust transmitting count*/
                cnt = hspi->tx.xfer_count;
            }
            _spi_tx_fifo_data(hspi, cnt);
        }
        BREAK_PROC_TIMEOUT(hspi, tick_start, timeout);
    }

    /* Wait for finishing transmitting the last data*/
    while (HAL_REG_READ(hspi->instance->STATUS_b.TFVE) > 0)
    {
        BREAK_PROC_TIMEOUT(hspi, tick_start, timeout);
    }

    while (HAL_REG_READ(hspi->instance->STATUS_b.BUSY) == 1)
    {
        BREAK_PROC_TIMEOUT(hspi, tick_start, timeout);
    }

    /* close Tx function */
    SPI_DISABLE_TX_FUC(hspi);

    /* If timeout occurs, return HAL_TIMEOUT */
    if (hspi->error_code == HAL_ERROR_TIMEOUT)
    {
        status = HAL_TIMEOUT;
    }

    /* Set SPI Ready state after transmit */
    HAL_FLAG_WRITE(hspi->state, HAL_STATE_READY);

    /* Process unlocked */
    GIVE_MUTEX(hspi);

    return status;
}

/**
 * @brief Receive an amount of data in blocking mode.
 * @param hspi pointer to a SPI_Handle_T structure that contains
 *             the configuration information for SPI module.
 * @param data pointer to data buffer
 * @param size amount of data to be received
 * @param timeout Timeout duration
 * @return HAL_Status_T
 */
HAL_Status_T HAL_SPI_Receive(SPI_Handle_T *hspi, uint8_t *data, uint32_t size, uint32_t timeout)
{
    HAL_Status_T status = HAL_OK;

    /* Check parameter */
    ASSERT_PARAM(hspi);
    ASSERT_PARAM(data);
    ASSERT_PARAM(size);
    ASSERT_PARAM(!((hspi->base == 1) && (size & 1)));

    /* Check whether SPI is ready */
    RET_FLAG_TRUE(HAL_FLAG_LACKS(hspi->state, HAL_STATE_READY), HAL_ERROR);
    RET_FLAG_TRUE(HAL_FLAG_HAS(hspi->state, HAL_STATE_BUSY), HAL_BUSY);

    /* Process locked */
    TAKE_MUTEX(hspi);

    /* Change SPI peripheral state */
    HAL_FLAG_WRITE(hspi->state, HAL_STATE_BUSY_RX);
    /* Clear SPI error_code */
    CLEAR_ERROR_CODE(hspi);
    /* Set the function for receive */
    if (IS_SPI_MASTER(hspi))
    {
        _spi_xfer_kernel(hspi, NULL, data, size, size);
    }
    else
    {
        _spi_xfer_kernel(hspi, NULL, data, 0, size);
    }

    /* Transmit data */
    uint32_t tick_start = HAL_GetTick(); /* Init tick_start for timeout management */
    uint32_t cnt        = SPI_TX_FIFO_DEPTH;
    if (IS_SPI_MASTER(hspi))
    {
        while (hspi->rx.xfer_count > 0 || hspi->tx.xfer_count > 0)
        {
            if (IS_TX_FIFO_EMPTY(hspi))
            {
                /* Data to be transmitted in every round can't be greater than remaining data count to be transmitted */
                if (cnt > hspi->tx.xfer_count)
                {
                    /* Or we need adjust transmitting count*/
                    cnt = hspi->tx.xfer_count;
                }
                _spi_tx_fifo_dummy(hspi, cnt);
            }
            _spi_rx_data(hspi);
            BREAK_PROC_TIMEOUT(hspi, tick_start, timeout);
        }
    }
    else
    {
        while (hspi->rx.xfer_count)
        {
            _spi_rx_data(hspi);
            BREAK_PROC_TIMEOUT(hspi, tick_start, timeout);
        }
    }

    /* If timeout occurs, return HAL_TIMEOUT */
    if (hspi->error_code == HAL_ERROR_TIMEOUT)
    {
        status = HAL_TIMEOUT;
    }

    /* Set SPI Ready state after receive */
    HAL_FLAG_WRITE(hspi->state, HAL_STATE_READY);

    /* Free the mutex */
    GIVE_MUTEX(hspi);

    return status;
}

/**
 * @brief Transmit and Receive an amount of data in blocking mode.
 * @param hspi pointer to a SPI_Handle_T structure that contains
 *             the configuration information for SPI module.
 * @param tx_data pointer to transmission data buffer
 * @param rx_data pointer to reception data buffer
 * @param size amount of data to be sent and received
 * @param timeout Timeout duration
 * @return HAL_Status_T HAL status
 */
HAL_Status_T HAL_SPI_TransmitReceive(SPI_Handle_T *hspi, uint8_t *tx_data, uint8_t *rx_data, uint32_t size, uint32_t timeout)
{
    HAL_Status_T status = HAL_OK;

    /* Check parameter */
    ASSERT_PARAM(hspi);
    ASSERT_PARAM(tx_data);
    ASSERT_PARAM(rx_data);
    ASSERT_PARAM(size);
    ASSERT_PARAM(!((hspi->base == 1) && (size & 1)));

    /* Check whether SPI is ready */
    RET_FLAG_TRUE(HAL_FLAG_LACKS(hspi->state, HAL_STATE_READY), HAL_ERROR);
    RET_FLAG_TRUE(HAL_FLAG_HAS(hspi->state, HAL_STATE_BUSY), HAL_BUSY);

    /* Process locked */
    TAKE_MUTEX(hspi);

    /* Change SPI peripheral state */
    HAL_FLAG_WRITE(hspi->state, HAL_STATE_BUSY_TX_RX);
    /* Clear SPI error_code */
    CLEAR_ERROR_CODE(hspi);
    /* Set the function for transmit & receive */
    _spi_xfer_kernel(hspi, tx_data, rx_data, size, size);

    /* Transfer Data */
    uint32_t tick_start = HAL_GetTick(); /* Init tick_start for timeout management */
    uint32_t cnt        = SPI_TX_FIFO_DEPTH;
    while (hspi->tx.xfer_count > 0 || hspi->rx.xfer_count > 0)
    {
        if (IS_TX_FIFO_EMPTY(hspi))
        {
            /* Data to be transmitted in every round can't be greater than remaining data count to be transmitted */
            if (cnt > hspi->tx.xfer_count)
            {
                /* Or we need adjust transmitting count*/
                cnt = hspi->tx.xfer_count;
            }
            _spi_tx_fifo_data(hspi, cnt);
        }
        _spi_rx_data(hspi);
        BREAK_PROC_TIMEOUT(hspi, tick_start, timeout);
    }

    /* Wait for finishing transmitting the last data*/
    while (HAL_REG_READ(hspi->instance->STATUS_b.TFVE) > 0)
    {
        BREAK_PROC_TIMEOUT(hspi, tick_start, timeout);
    }

    while (HAL_REG_READ(hspi->instance->STATUS_b.BUSY) == 1)
    {
        BREAK_PROC_TIMEOUT(hspi, tick_start, timeout);
    }

    /* If timeout occurs, return HAL_TIMEOUT */
    if (hspi->error_code == HAL_ERROR_TIMEOUT)
    {
        status = HAL_TIMEOUT;
    }

    /* Set SPI Ready state after transmit & receive */
    HAL_FLAG_WRITE(hspi->state, HAL_STATE_READY);

    /* Free the mutex */
    GIVE_MUTEX(hspi);

    return status;
}
#endif /* configUSE_SPI_BLOCKING */

#if (configUSE_SPI_NONBLOCKING_IT == 1U)
/**
 * @brief Transmit an amount of data in non-blocking mode with Interrupt.
 * @param hspi pointer to a SPI_Handle_T structure that contains
 *             the configuration information for SPI module.
 * @param data pointer to data buffer
 * @param size amount of data to be sent
 * @param timeout Timeout duration
 * @return HAL_Status_T
 */
HAL_Status_T HAL_SPI_Transmit_IT(SPI_Handle_T *hspi, uint8_t *data, uint32_t size)
{
    HAL_Status_T status = HAL_OK;

    /* Check the valid of parameter */
    ASSERT_PARAM(hspi);
    ASSERT_PARAM(data);
    ASSERT_PARAM(size);
    ASSERT_PARAM(!((hspi->base == 1) && (size & 1)));

    /* Check whether SPI is ready */
    RET_FLAG_TRUE(HAL_FLAG_LACKS(hspi->state, HAL_STATE_READY), HAL_ERROR);
    RET_FLAG_TRUE(HAL_FLAG_HAS(hspi->state, HAL_STATE_BUSY), HAL_BUSY);

    /* Process locked */
    TAKE_MUTEX(hspi);

    /* Change SPI peripheral state */
    HAL_FLAG_WRITE(hspi->state, HAL_STATE_BUSY_TX);
    /* mark transfer mode as tx only */
    SPI_STATE_EX_SET(hspi, SPI_STATE_EX_TX_ONLY);
    /* Clear SPI error_code */
    CLEAR_ERROR_CODE(hspi);
    /* Set the function for IT transmit */
    _spi_it_xfer_kernel(hspi, data, NULL, size, 0);

    /* Free the mutex */
    GIVE_MUTEX(hspi);

    return status;
}

/**
 * @brief Receive an amount of data in non-blocking mode with Interrupt.
 * @param hspi pointer to a SPI_Handle_T structure that contains
 *             the configuration information for SPI module.
 * @param data pointer to data buffer
 * @param size amount of data to be received
 * @param timeout Timeout duration
 * @return HAL_Status_T
 */
HAL_Status_T HAL_SPI_Receive_IT(SPI_Handle_T *hspi, uint8_t *data, uint32_t size)
{
    HAL_Status_T status = HAL_OK;

    /* Check the valid of SPI handle */
    ASSERT_PARAM(hspi);
    ASSERT_PARAM(data);
    ASSERT_PARAM(size);
    ASSERT_PARAM(!((hspi->base == 1) && (size & 1)));

    /* Check whether SPI is ready */
    RET_FLAG_TRUE(HAL_FLAG_LACKS(hspi->state, HAL_STATE_READY), HAL_ERROR);
    RET_FLAG_TRUE(HAL_FLAG_HAS(hspi->state, HAL_STATE_BUSY), HAL_BUSY);

    /* Process locked */
    TAKE_MUTEX(hspi);

    /* Change SPI peripheral state */
    HAL_FLAG_WRITE(hspi->state, HAL_STATE_BUSY_RX);
    /* mark transfer mode as rx only */
    SPI_STATE_EX_SET(hspi, SPI_STATE_EX_RX_ONLY);
    /* Clear SPI error_code */
    CLEAR_ERROR_CODE(hspi);
    /* Set the function for IT receive */
    if (IS_SPI_MASTER(hspi))
    {
        _spi_it_xfer_kernel(hspi, NULL, data, size, size);
    }
    else
    {
        _spi_it_xfer_kernel(hspi, NULL, data, 0, size);
    }

    /* Free the mutex */
    GIVE_MUTEX(hspi);

    return status;
}

/**
 * @brief Transmit and Receive an amount of data in non-blocking mode with Interrupt.
 * @param hspi pointer to a SPI_Handle_T structure that contains
 *             the configuration information for SPI module.
 * @param tx_data pointer to transmission data buffer
 * @param rx_data pointer to reception data buffer
 * @param size amount of data to be sent and received
 * @param timeout Timeout duration
 * @return HAL_Status_T HAL status
 */
HAL_Status_T HAL_SPI_TransmitReceive_IT(SPI_Handle_T *hspi, uint8_t *tx_data, uint8_t *rx_data, uint32_t size)
{
    HAL_Status_T status = HAL_OK;

    /* Check the valid of SPI handle */
    ASSERT_PARAM(hspi);
    ASSERT_PARAM(tx_data);
    ASSERT_PARAM(rx_data);
    ASSERT_PARAM(size);
    ASSERT_PARAM(!((hspi->base == 1) && (size & 1)));

    /* Check whether SPI is ready */
    RET_FLAG_TRUE(HAL_FLAG_LACKS(hspi->state, HAL_STATE_READY), HAL_ERROR);
    RET_FLAG_TRUE(HAL_FLAG_HAS(hspi->state, HAL_STATE_BUSY), HAL_BUSY);

    /* Process locked */
    TAKE_MUTEX(hspi);

    /* Change SPI peripheral state */
    HAL_FLAG_WRITE(hspi->state, HAL_STATE_BUSY_TX_RX);
    /* mark transfer mode as transfer */
    SPI_STATE_EX_SET(hspi, SPI_STATE_EX_BOTH);
    /* Clear SPI error_code */
    CLEAR_ERROR_CODE(hspi);
    /* Set the function for IT transmit & receive */
    _spi_it_xfer_kernel(hspi, tx_data, rx_data, size, size);

    /* Free the mutex */
    GIVE_MUTEX(hspi);

    return status;
}
#endif /* configUSE_SPI_NONBLOCKING_IT */

#if (configUSE_SPI_NONBLOCKING_DMA == 1U)
/**
 * @brief  Transmit an amount of data in non-blocking mode with DMA.
 * @param  hspi pointer to a SPI_Handle_T structure that contains
 *              the configuration information for SPI module.
 * @param  data pointer to data buffer
 * @param  size amount of data to be sent
 * @retval HAL status
 */
HAL_Status_T HAL_SPI_Transmit_DMA(SPI_Handle_T *hspi, uint8_t *data, uint32_t size)
{
    HAL_Status_T status = HAL_OK;

    /* check para is valid */
    ASSERT_PARAM(hspi);
    ASSERT_PARAM(hspi->hdmatx);
    ASSERT_PARAM(data);

    /* Check whether SPI is ready */
    RET_FLAG_TRUE(HAL_FLAG_LACKS(hspi->state, HAL_STATE_READY), HAL_ERROR);
    RET_FLAG_TRUE(HAL_FLAG_HAS(hspi->state, HAL_STATE_BUSY), HAL_BUSY);

    /* Process locked */
    TAKE_MUTEX(hspi);

    /* Change SPI peripheral state */
    HAL_FLAG_WRITE(hspi->state, HAL_STATE_BUSY_TX);
    /* Clear SPI error_code */
    CLEAR_ERROR_CODE(hspi);
    /* start tx dma */
    status = _spi_dma_xfer_kernel(hspi, data, NULL, size, 0);

    /* Free the mutex */
    GIVE_MUTEX(hspi);

    return status;
}

/**
 * @brief  Receive an amount of data in non-blocking mode with DMA.
 * @note   In case of MASTER mode and SPI_DIRECTION_2LINES direction, hdmatx shall be defined.
 * @param  hspi pointer to a SPI_Handle_T structure that contains
 *              the configuration information for SPI module.
 * @param  data pointer to data buffer
 * @param  size amount of data to be sent
 * @retval HAL status
 */
HAL_Status_T HAL_SPI_Receive_DMA(SPI_Handle_T *hspi, uint8_t *data, uint32_t size)
{
    HAL_Status_T status = HAL_OK;

    /* check para is valid */
    ASSERT_PARAM(hspi);
    ASSERT_PARAM(hspi->hdmarx);
    ASSERT_PARAM(data);

    /* Check whether SPI is ready */
    RET_FLAG_TRUE(HAL_FLAG_LACKS(hspi->state, HAL_STATE_READY), HAL_ERROR);
    RET_FLAG_TRUE(HAL_FLAG_HAS(hspi->state, HAL_STATE_BUSY), HAL_BUSY);

    /* Process locked */
    TAKE_MUTEX(hspi);

    /* Change SPI peripheral state */
    HAL_FLAG_WRITE(hspi->state, HAL_STATE_BUSY_RX);
    /* Clear SPI error_code */
    CLEAR_ERROR_CODE(hspi);
    /* start rx dma */
    status = _spi_dma_xfer_kernel(hspi, NULL, data, 0, size);

    /* Free the mutex */
    GIVE_MUTEX(hspi);

    return status;
}

/**
 * @brief  Transmit and Receive an amount of data in non-blocking mode with DMA.
 * @param  hspi pointer to a SPI_Handle_T structure that contains
 *              the configuration information for SPI module.
 * @param  tx_data pointer to transmission data buffer
 * @param  rx_data pointer to reception data buffer
 * @param  size amount of data to be sent
 * @retval HAL status
 */
HAL_Status_T HAL_SPI_TransmitReceive_DMA(SPI_Handle_T *hspi, uint8_t *tx_data, uint8_t *rx_data, uint32_t size)
{
    HAL_Status_T status = HAL_OK;

    /* check para is valid */
    ASSERT_PARAM(hspi);
    ASSERT_PARAM(hspi->hdmatx);
    ASSERT_PARAM(hspi->hdmarx);
    ASSERT_PARAM(tx_data);
    ASSERT_PARAM(rx_data);

    /* Check whether SPI is ready */
    RET_FLAG_TRUE(HAL_FLAG_LACKS(hspi->state, HAL_STATE_READY), HAL_ERROR);
    RET_FLAG_TRUE(HAL_FLAG_HAS(hspi->state, HAL_STATE_BUSY), HAL_BUSY);

    /* Process locked */
    TAKE_MUTEX(hspi);

    /* Change SPI peripheral state */
    HAL_FLAG_WRITE(hspi->state, HAL_STATE_BUSY_TX_RX);
    /* Clear SPI error_code */
    CLEAR_ERROR_CODE(hspi);
    /* start tx/rx dma */
    status = _spi_dma_xfer_kernel(hspi, tx_data, rx_data, size, size);

    /* Free the mutex */
    GIVE_MUTEX(hspi);

    return status;
}

/**
 * @brief  Pause the DMA Transfer.
 * @param  hspi pointer to a SPI_Handle_T structure that contains
 *              the configuration information for the specified SPI module.
 * @retval HAL status
 */
HAL_Status_T HAL_SPI_DMAPause(SPI_Handle_T *hspi)
{
    HAL_Status_T status = HAL_OK;

    /* check para is valid */
    ASSERT_PARAM(hspi);

    /* Process locked */
    TAKE_MUTEX(hspi);

    if (hspi->hdmatx == NULL && hspi->hdmarx == NULL)
        return HAL_ERROR;

    if (hspi->hdmatx)
    {
        if (hspi->instance->ICTRL_b.TFTHOD == 1)
        {
            HAL_FLAG_WRITE(hspi->instance->ICTRL_b.TFTHOD, 0);
        }
        else
        {
            status = HAL_ERROR;
        }
    }

    if (hspi->hdmarx)
    {
        if (hspi->instance->ICTRL_b.RFTHOD == 1)
        {
            HAL_FLAG_WRITE(hspi->instance->ICTRL_b.RFTHOD, 0);
        }
        else
        {
            status = HAL_ERROR;
        }
    }

    /* Free the mutex */
    GIVE_MUTEX(hspi);

    return status;
}

/**
 * @brief  Resume the DMA Transfer.
 * @param  hspi pointer to a SPI_Handle_T structure that contains
 *              the configuration information for the specified SPI module.
 * @retval HAL status
 */
HAL_Status_T HAL_SPI_DMAResume(SPI_Handle_T *hspi)
{
    HAL_Status_T status = HAL_OK;

    ASSERT_PARAM(hspi);

    /* Process locked */
    TAKE_MUTEX(hspi);

    if (hspi->hdmatx == NULL && hspi->hdmarx == NULL)
        return HAL_ERROR;

    if (hspi->hdmatx)
    {
        /* Resumption completed only When the SPI_DMA is in the pausing/aborting status*/
        if (hspi->instance->ICTRL_b.TFTHOD == 0)
        {
            HAL_FLAG_WRITE(hspi->instance->ICTRL_b.TFTHOD, 1);
        }
        else
        {
            status = HAL_ERROR;
        }
    }

    if (hspi->hdmarx)
    {
        /* Resumption completed only When the SPI_DMA is in the pausing/aborting status*/
        if (hspi->instance->ICTRL_b.RFTHOD == 0)
        {
            HAL_FLAG_WRITE(hspi->instance->ICTRL_b.RFTHOD, 1);
        }
        else
        {
            status = HAL_ERROR;
        }
    }

    /* Free the mutex */
    GIVE_MUTEX(hspi);

    return status;
}

/**
 * @brief  Stop the DMA Transfer.
 * @param  hspi pointer to a SPI_Handle_T structure that contains
 *              the configuration information for the specified SPI module.
 * @retval HAL status
 */
HAL_Status_T HAL_SPI_DMAStop(SPI_Handle_T *hspi)
{
    HAL_Status_T status = HAL_OK;

    ASSERT_PARAM(hspi);

    /* Process locked */
    TAKE_MUTEX(hspi);

    if (hspi->hdmatx == NULL && hspi->hdmarx == NULL)
        return HAL_ERROR;

    status = _spi_dma_abort(hspi);

    HAL_FLAG_WRITE(hspi->state, HAL_STATE_READY);

    /* Free the mutex */
    GIVE_MUTEX(hspi);

    return status;
}
#endif /* configUSE_SPI_NONBLOCKING_DMA */
/**
 * @brief  Abort ongoing transfer (blocking mode).
 * @param  hspi SPI handle.
 * @note   This procedure could be used for aborting any ongoing transfer (Tx and Rx),
 *         started in Interrupt or DMA mode.
 *         This procedure performs following operations :
 *           - Disable SPI Interrupts (depending of transfer direction)
 *           - Disable the DMA transfer in the peripheral register (if enabled)
 *           - Abort DMA transfer by calling HAL_DMA_Abort (in case of transfer in DMA mode)
 *           - Set handle State to READY
 * @note   This procedure is executed in blocking mode : when exiting function, Abort is considered as completed.
 * @retval HAL status
 */
HAL_Status_T HAL_SPI_Abort(SPI_Handle_T *hspi)
{
    HAL_Status_T status = HAL_OK;

    /* Check the valid of SPI handle */
    ASSERT_PARAM(hspi);

    /* It will return error if you call the abort function in SPI idle time */
    RET_FLAG_FALSE(HAL_FLAG_HAS(hspi->state, HAL_STATE_BUSY), HAL_ERROR);

    /* Process locked */
    TAKE_MUTEX(hspi);

#if (configUSE_SPI_BLOCKING == 1U)
    if (SPI_STATE_EX_LACK(hspi, SPI_STATE_EX_DMA))
    {
        if (HAL_REG_READ(hspi->instance->ICTRL_b.TXCIEN) == DISABLE && HAL_REG_READ(hspi->instance->ICTRL_b.RFTHIEN) == DISABLE && HAL_REG_READ(hspi->instance->ICTRL_b.TFTHOD) == DISABLE)
        {
            hspi->tx.xfer_count = 0;
            hspi->rx.xfer_count = 0;
            while (HAL_FLAG_HAS(hspi->state, HAL_STATE_BUSY))
            {
                __NOP();
            }
        }
    }
#endif

    /* Disable the SPI IT Tx/Rx request if enabled  */
#if (configUSE_SPI_NONBLOCKING_IT == 1U)
    if (SPI_STATE_EX_LACK(hspi, SPI_STATE_EX_DMA))
    {
        if (HAL_REG_READ(hspi->instance->ICTRL_b.TXCIEN) == ENABLE || HAL_REG_READ(hspi->instance->ICTRL_b.RFTHIEN) == ENABLE || HAL_REG_READ(hspi->instance->ICTRL_b.TFTHOD) == ENABLE)
        {
            SPI_DISABLE_IE(hspi, SPI_IE_TFTHIEN);
            SPI_DISABLE_IE(hspi, SPI_IE_RFTHIEN);
            SPI_DISABLE_IE(hspi, SPI_IE_TXCIEN);
            SPI_DISABLE_TX_FIFO(hspi);
            SPI_DISABLE_RX_FIFO(hspi);
            SPI_RESET_FIFO(hspi);
            /* If there is data in shifting register, we need to wait for completion*/
            while (HAL_REG_READ(hspi->instance->STATUS_b.BUSY) == 1)
            {
                __NOP();
            }
        }
    }
#endif

    /* Disable the SPI DMA Tx/Rx request if enabled  */
#if (configUSE_SPI_NONBLOCKING_DMA == 1U)
    if (SPI_STATE_EX_HAS(hspi, SPI_STATE_EX_DMA))
    {
        status = _spi_dma_abort(hspi);
        if (status != HAL_OK)
        {
            HAL_FLAG_SET(hspi->error_code, HAL_SPI_ERROR_ABORT);
        }
    }
#endif

    /* Check error during Abort procedure */
    if (HAL_FLAG_HAS(hspi->error_code, HAL_SPI_ERROR_ABORT))
    {
        /* return HAL_ERROR in case of error during Abort procedure */
        status = HAL_ERROR;
    }
    else
    {
        /* Reset errorCode */
        HAL_FLAG_WRITE(hspi->error_code, HAL_ERROR_NONE);
    }

    /* Change SPI peripheral state */
    HAL_FLAG_WRITE(hspi->state, HAL_STATE_READY);

    /* Free the mutex */
    GIVE_MUTEX(hspi);

    return status;
}

/**
 * @brief  Abort ongoing transfer (Interrupt mode).
 * @param  hspi SPI handle.
 * @note   This procedure could be used for aborting any ongoing transfer (Tx and Rx),
 *         started in Interrupt or DMA mode.
 *         This procedure performs following operations :
 *           - Disable SPI Interrupts (depending of transfer direction)
 *           - Disable the DMA transfer in the peripheral register (if enabled)
 *           - Abort DMA transfer by calling HAL_DMA_Abort_IT (in case of transfer in DMA mode)
 *           - Set handle State to READY
 *           - At abort completion, call user abort complete callback
 * @note   This procedure is executed in Interrupt mode, meaning that abort procedure could be
 *         considered as completed only when user abort complete callback is executed (not when exiting function).
 * @retval HAL status
 */
HAL_Status_T HAL_SPI_Abort_IT(SPI_Handle_T *hspi)
{
    // blocking wait to abort successfully
    HAL_Status_T status = HAL_SPI_Abort(hspi);
    RET_FLAG_FALSE(status == HAL_OK, status);

    // call abort callback
#if (configUSE_SPI_REGISTER_CALLBACKS == 1U)
    hspi->AbortCpltCallback(hspi);
#else
    HAL_SPI_AbortCpltCallback(hspi);
#endif

    return status;
}

/*
================================================================================
            ##### IRQHandler and Event Callback functions #####
================================================================================
*/
/**
 * @brief  Tx Transfer completed callback.
 * @param  hspi pointer to a SPI_Handle_T structure that contains
 *               the configuration information for SPI module.
 * @retval None
 */
__weak void ATTR_ISR HAL_SPI_TxCpltCallback(SPI_Handle_T *hspi)
{
    /* Prevent unused argument(s) compilation warning */
    UNUSED(hspi);

    /* NOTE : This function should not be modified, when the callback is needed,
              the HAL_SPI_TxCpltCallback should be implemented in the user file
     */
}

/**
 * @brief  Rx Transfer completed callback.
 * @param  hspi pointer to a SPI_Handle_T structure that contains
 *              the configuration information for SPI module.
 * @retval None
 */
__weak void ATTR_ISR HAL_SPI_RxCpltCallback(SPI_Handle_T *hspi)
{
    /* Prevent unused argument(s) compilation warning */
    UNUSED(hspi);

    /* NOTE : This function should not be modified, when the callback is needed,
              the HAL_SPI_RxCpltCallback should be implemented in the user file
     */
}

/**
 * @brief  Tx and Rx Transfer completed callback.
 * @param  hspi pointer to a SPI_Handle_T structure that contains
 *              the configuration information for SPI module.
 * @retval None
 */
__weak void ATTR_ISR HAL_SPI_TxRxCpltCallback(SPI_Handle_T *hspi)
{
    /* Prevent unused argument(s) compilation warning */
    UNUSED(hspi);

    /* NOTE : This function should not be modified, when the callback is needed,
              the HAL_SPI_TxRxCpltCallback should be implemented in the user file
     */
}

/**
 * @brief  SPI error callback.
 * @param  hspi pointer to a SPI_Handle_T structure that contains
 *               the configuration information for SPI module.
 * @retval None
 */
__weak void ATTR_ISR HAL_SPI_ErrorCallback(SPI_Handle_T *hspi)
{
    /* Prevent unused argument(s) compilation warning */
    UNUSED(hspi);

    /* NOTE : This function should not be modified, when the callback is needed,
              the HAL_SPI_ErrorCallback should be implemented in the user file
     */
    /* NOTE : The ErrorCode parameter in the hspi handle is updated by the SPI processes
              and user can use HAL_SPI_GetError() API to check the latest error occurred
     */
}

/**
 * @brief  SPI Abort Complete callback.
 * @param  hspi SPI handle.
 * @retval None
 */
__weak void ATTR_ISR HAL_SPI_AbortCpltCallback(SPI_Handle_T *hspi)
{
    /* Prevent unused argument(s) compilation warning */
    UNUSED(hspi);

    /* NOTE : This function should not be modified, when the callback is needed,
              the HAL_SPI_AbortCpltCallback can be implemented in the user file.
     */
}

#if (configUSE_SPI_NONBLOCKING_IT == 1U)
/**
 * @brief  Handle SPI interrupt request.
 * @param  hspi pointer to a SPI_Handle_T structure that contains
 *               the configuration information for the specified SPI module.
 * @retval None
 */
__weak ATTR_ISR void HAL_SPI_IRQHandler(SPI_Handle_T *hspi)
{
    /* The step of reading status must place in the beginning, Make sure every IRQ handling is according with expectation */
    uint32_t status = HAL_REG_READ(hspi->instance->STATUS);
    uint32_t ris    = HAL_REG_READ(hspi->instance->RIS);
    uint32_t cnt    = 0;

    if (HAL_FLAG_HAS(ris, SPI_RIS_RFTHI_MASK))
    {
        cnt = (status >> SPI_STATUS_RFVE_BIT) & SPI_STATUS_RFVE_MASK;
        if (cnt >= hspi->rx.xfer_count)
        {
            /* Arrive here, it means that Rx function will finish in this round, and ready to go to callback */
            SPI_DISABLE_IE(hspi, SPI_IE_TXCIEN);
            cnt = hspi->rx.xfer_count;
        }
        if (cnt > 0)
        {
            _spi_rx_fifo_data(hspi, cnt);
            if (hspi->rx.xfer_count <= SLAVE_RX_THOD)
            {
                /* If the count of remaining data is less than RFTHOD, we need to adjust it for next interrupt */
                SPI_SET_SLAVE_RX_FIFO(hspi, hspi->rx.xfer_count);
            }
        }
    }

    if (HAL_FLAG_HAS(ris, SPI_RIS_TFTHI_MASK)) // TFTHI Mode
    {
        uint32_t tfve = (status >> SPI_STATUS_TFVE_BIT) & SPI_STATUS_TFVE_MASK;
        if (hspi->tx.xfer_size == hspi->tx.xfer_count)
        {
            /* When use TXCI to transfer, we need to close the TFTHI interrupt in the first round*/
            if (HAL_REG_READ(hspi->instance->ICTRL_b.TXCIEN) == 1)
            {
                SPI_DISABLE_TX_FIFO(hspi);
                SPI_DISABLE_IE(hspi, SPI_IE_TFTHIEN);
            }
        }

        if (hspi->tx.xfer_count == 0)
        {
            /* When arrive here, it means that the instance has finished the transmission and ready to go to callback */
            if (tfve == 0 && HAL_REG_READ(hspi->instance->STATUS_b.BUSY) == 0)
            {
                SPI_DISABLE_IE(hspi, SPI_IE_TXCIEN);
            }
        }
        else
        {
            cnt = SPI_TX_FIFO_DEPTH - tfve;
            if (cnt >= hspi->tx.xfer_count)
            {
                /* Use the TXCI interrupt to handle callback in the Final round */
                SPI_ENABLE_IE(hspi, SPI_IE_TXCIEN);

                /* Avoid pendding a meanless Tx interrupt in the Final round when using TFTHI to trigger interrupt*/
                SPI_DISABLE_IE(hspi, SPI_IE_TFTHIEN);
                SPI_DISABLE_TX_FIFO(hspi);

                cnt = hspi->tx.xfer_count;
            }

            if (IS_SPI_MASTER(hspi) && hspi->tx.buffer == NULL)
            {
                _spi_tx_fifo_dummy(hspi, cnt);
            }
            else
            {
                _spi_tx_fifo_data(hspi, cnt);
            }
        }
    }
    else if (HAL_FLAG_HAS(ris, SPI_RIS_TXCI_MASK)) // TXCI Mode
    {
        uint32_t tfve = (status >> SPI_STATUS_TFVE_BIT) & SPI_STATUS_TFVE_MASK;
        if (hspi->tx.xfer_count == 0)
        {
            if (tfve == 0 && HAL_REG_READ(hspi->instance->STATUS_b.BUSY) == 0)
            {
                /* When arrive here, it means that the instance has finished the transmission and ready to go to callback */
                SPI_DISABLE_IE(hspi, SPI_IE_TXCIEN);
            }
        }
        else
        {
            cnt = SPI_TX_FIFO_DEPTH - tfve;
            if (cnt >= hspi->tx.xfer_count)
            {
                cnt = hspi->tx.xfer_count;
            }

            if (IS_SPI_MASTER(hspi) && hspi->tx.buffer == NULL)
            {
                _spi_tx_fifo_dummy(hspi, cnt);
            }
            else
            {
                _spi_tx_fifo_data(hspi, cnt);
            }
        }
    }

    /*Error Check*/
    if (HAL_FLAG_HAS(ris, SPI_RIS_TFURI_MASK))
    {
        HAL_FLAG_WRITE(hspi->state, HAL_STATE_READY);
        hspi->error_code = HAL_SPI_ERROR_TX_UNDERFLOW;
#if (configUSE_SPI_REGISTER_CALLBACKS == 1U)
        hspi->ErrorCallback(hspi);
#else
        HAL_SPI_ErrorCallback(hspi);
#endif
    }
    if (HAL_FLAG_HAS(ris, SPI_RIS_RFORI_MASK))
    {
        HAL_FLAG_WRITE(hspi->state, HAL_STATE_READY);
        hspi->error_code = HAL_SPI_ERROR_RX_OVERFLOW;
#if (configUSE_SPI_REGISTER_CALLBACKS == 1U)
        hspi->ErrorCallback(hspi);
#else
        HAL_SPI_ErrorCallback(hspi);
#endif
    }

    if (hspi->rx.xfer_count == 0 && hspi->tx.xfer_count == 0)
    {
        if (HAL_REG_READ(hspi->instance->ICTRL_b.TXCIEN) == 0)
        {
            _spi_switch_cb(hspi);
        }
    }
}
#endif /* configUSE_SPI_NONBLOCKING_IT */

/* Private functions ---------------------------------------------------------*/
#if (configUSE_SPI_NONBLOCKING_DMA == 1U)
/**
 * @brief spi tx dma error callback
 * @param hspi SPI handle.
 */
static void _spi_tx_dma_err_cb(DMA_Handle_T *hdma)
{
    SPI_Handle_T *hspi = (SPI_Handle_T *)hdma->parent;
    HAL_FLAG_WRITE(hspi->state, HAL_STATE_READY);
    hspi->error_code = HAL_SPI_ERROR_DMA_TX;
#if (configUSE_SPI_REGISTER_CALLBACKS == 1U)
    hspi->ErrorCallback(hspi);
#else
    HAL_SPI_ErrorCallback(hspi);
#endif
}

/**
 * @brief spi rx dma error callback
 * @param hspi SPI handle.
 */
static void _spi_rx_dma_err_cb(DMA_Handle_T *hdma)
{
    SPI_Handle_T *hspi = (SPI_Handle_T *)hdma->parent;
    HAL_FLAG_WRITE(hspi->state, HAL_STATE_READY);
    hspi->error_code = HAL_SPI_ERROR_DMA_RX;
#if (configUSE_SPI_REGISTER_CALLBACKS == 1U)
    hspi->ErrorCallback(hspi);
#else
    HAL_SPI_ErrorCallback(hspi);
#endif
}

/**
 * @brief spi tx dma complete callback
 * @param hspi SPI handle.
 */
static void _spi_tx_dma_cb(DMA_Handle_T *hdma)
{
    SPI_Handle_T *hspi  = (SPI_Handle_T *)hdma->parent;
    hspi->tx.xfer_count = 0;
    if (hspi->rx.xfer_count == 0)
    {
        _spi_switch_dma_cb(hspi);
    }
}

/**
 * @brief spi rx dma complete callback
 * @param hspi SPI handle.
 */
static void _spi_rx_dma_cb(DMA_Handle_T *hdma)
{
    SPI_Handle_T *hspi  = (SPI_Handle_T *)hdma->parent;
    hspi->rx.xfer_count = 0;
    if (hspi->tx.xfer_count == 0)
    {
        _spi_switch_dma_cb(hspi);
    }
}

/**
 * @brief  dma transfer kernel
 * @param  hspi pointer to a SPI_Handle_T structure that contains
 *              the configuration information for SPI module.
 * @param  tx_data pointer to tx data buffer
 * @param  rx_data pointer to rx data buffer
 * @param  tx_size amount of data to be tx
 * @param  rx_size amount of data to be rx
 * @return HAL_Status_T
 */
static HAL_Status_T _spi_dma_xfer_kernel(SPI_Handle_T *hspi, uint8_t *tx_data, uint8_t *rx_data, uint32_t tx_size, uint32_t rx_size)
{
    HAL_Status_T status = HAL_OK;

    /* Close setting related to DMA and Transfer first */
    uint32_t ictrl = HAL_REG_READ(hspi->instance->ICTRL) & ~(SPI_ICTRL_TFTHOD_MASK | SPI_ICTRL_RFTHOD_MASK | SPI_ICTRL_TFDMAEN_MASK | SPI_ICTRL_RFDMAEN_MASK);
    uint32_t ctrl2 = HAL_REG_READ(hspi->instance->CTRL2) & ~(SPI_CTRL2_TXEN_MASK | SPI_CTRL2_RXEN_MASK);
    HAL_REG_WRITE(hspi->instance->ICTRL, ictrl);
    HAL_REG_WRITE(hspi->instance->CTRL2, ctrl2);

    /* set xfer info */
    SPI_STATE_EX_SET(hspi, SPI_STATE_EX_DMA);
    _spi_xfer_info(hspi, tx_data, rx_data, tx_size, rx_size);

    /* Reset fifo */
    SPI_RESET_FIFO(hspi);

    /* start rx dma */
    if (rx_size)
    {
        status = HAL_DMA_Start_IT(hspi->hdmarx, (uint32_t)&hspi->instance->DATA, (uint32_t)rx_data, rx_size);
        if (status != HAL_OK)
        {
            HAL_FLAG_WRITE(hspi->state, HAL_STATE_READY);
            hspi->error_code = HAL_SPI_ERROR_DMA;
            return status;
        }
        ictrl |= (1 << SPI_ICTRL_RFDMAEN_BIT) | (1 << SPI_ICTRL_RFTHOD_BIT);
        ctrl2 |= (1 << SPI_CTRL2_RXEN_BIT);
    }

    /* start tx dma */
    if (tx_size)
    {
        status = HAL_DMA_Start_IT(hspi->hdmatx, (uint32_t)tx_data, (uint32_t)&hspi->instance->DATA, tx_size);
        if (status != HAL_OK)
        {
            HAL_FLAG_WRITE(hspi->state, HAL_STATE_READY);
            hspi->error_code = HAL_SPI_ERROR_DMA;
            return status;
        }
        ictrl |= (1 << SPI_ICTRL_TFDMAEN_BIT) | (1 << SPI_ICTRL_TFTHOD_BIT);
        ctrl2 |= (1 << SPI_CTRL2_TXEN_BIT);
    }

    /* Write ICTRL lastly */
    HAL_REG_WRITE(hspi->instance->CTRL2, ctrl2);
    HAL_REG_WRITE(hspi->instance->ICTRL, ictrl);

    return HAL_OK;
}

/**
 * @brief  abort dma transfer
 * @param  hspi pointer to a SPI_Handle_T structure that contains
 *              the configuration information for the specified SPI module.
 * @return HAL_Status_T
 */
static HAL_Status_T _spi_dma_abort(SPI_Handle_T *hspi)
{
    HAL_Status_T status = HAL_OK;
    /* Disable the SPI function */
    HAL_REG_WRITE(hspi->instance->CTRL2_b.SPIEN, 0);

    if (hspi->hdmatx)
    {
        SPI_DISABLE_IE(hspi, SPI_IE_TFDMAEN);
        SPI_DISABLE_TX_FIFO(hspi);
        SPI_DISABLE_TX_FUC(hspi);
        status = HAL_DMA_Abort(hspi->hdmatx);
        if (status != HAL_OK)
        {
            HAL_FLAG_WRITE(hspi->state, HAL_STATE_READY);
            hspi->error_code = HAL_SPI_ERROR_DMA;
            return status;
        }
    }

    if (hspi->hdmarx)
    {
        SPI_DISABLE_IE(hspi, SPI_IE_RFDMAEN);
        SPI_DISABLE_RX_FIFO(hspi);
        SPI_DISABLE_RX_FUC(hspi);
        status = HAL_DMA_Abort(hspi->hdmarx);
        if (status != HAL_OK)
        {
            HAL_FLAG_WRITE(hspi->state, HAL_STATE_READY);
            hspi->error_code = HAL_SPI_ERROR_DMA;
            return status;
        }
    }

    /* Enable the SPI function */
    HAL_REG_WRITE(hspi->instance->CTRL2_b.SPIEN, 1);
    return HAL_OK;
}

/**
 * @brief spi call callback
 * @param hspi SPI handle.
 */
static void _spi_switch_dma_cb(SPI_Handle_T *hspi)
{
    /* Set SPI Ready state after transfer done */
    HAL_FLAG_WRITE(hspi->state, HAL_STATE_READY);

    if (hspi->tx.buffer != NULL && hspi->rx.buffer == NULL)
    {
        while (HAL_REG_READ(hspi->instance->STATUS_b.BUSY) == 1)
        {
            __NOP();
        }

        SPI_DISABLE_TX_FIFO(hspi);
        SPI_DISABLE_IE(hspi, SPI_IE_TFDMAEN);

        SPI_STATE_EX_CLR(hspi, SPI_STATE_EX_TX_ONLY);
        /* Call user TX complete callback */
#if (configUSE_SPI_REGISTER_CALLBACKS == 1U)
        hspi->TxCpltCallback(hspi);
#else
        HAL_SPI_TxCpltCallback(hspi);
#endif
    }
    else if (hspi->rx.buffer != NULL && hspi->tx.buffer == NULL)
    {
        SPI_DISABLE_RX_FIFO(hspi);
        SPI_DISABLE_IE(hspi, SPI_IE_RFDMAEN);
        SPI_STATE_EX_CLR(hspi, SPI_STATE_EX_RX_ONLY);
        /* Call user RX complete callback */
#if (configUSE_SPI_REGISTER_CALLBACKS == 1U)
        hspi->RxCpltCallback(hspi);
#else
        HAL_SPI_RxCpltCallback(hspi);
#endif
    }
    else if (hspi->rx.buffer != NULL && hspi->tx.buffer != NULL)
    {
        while (HAL_REG_READ(hspi->instance->STATUS_b.BUSY) == 1)
        {
            __NOP();
        }

        SPI_DISABLE_TX_FIFO(hspi);
        SPI_DISABLE_RX_FIFO(hspi);
        SPI_DISABLE_IE(hspi, SPI_IE_TFDMAEN);
        SPI_DISABLE_IE(hspi, SPI_IE_RFDMAEN);
        SPI_STATE_EX_CLR(hspi, SPI_STATE_EX_BOTH);
        /* Call user TXRX complete callback */
#if (configUSE_SPI_REGISTER_CALLBACKS == 1U)
        hspi->TxRxCpltCallback(hspi);
#else
        HAL_SPI_TxRxCpltCallback(hspi);
#endif
    }
}

#endif /* configUSE_SPI_NONBLOCKING_DMA */

#if (configUSE_SPI_BLOCKING == 1U)
/**
 * @brief read fifo level by level
 * @param hspi pointer to a SPI_Handle_T structure that contains
 *             the configuration information for SPI module.
 */
static void _spi_rx_data(SPI_Handle_T *hspi)
{
    if (hspi->instance->STATUS_b.RFVE)
    {
        /* read the received data */
        if (0U == hspi->base)
        {
            *((uint8_t *)hspi->rx.buffer) = (uint8_t)HAL_REG_READ(hspi->instance->DATA) & (uint8_t)hspi->data_mask;
            hspi->rx.buffer               = hspi->rx.buffer + 1;
        }
        /* read the received data */
        else
        {
            *((uint16_t *)hspi->rx.buffer) = (uint16_t)HAL_REG_READ(hspi->instance->DATA) & hspi->data_mask;
            hspi->rx.buffer                = hspi->rx.buffer + 2;
        }
        hspi->rx.xfer_count--;
    }
}

/**
 * @brief  transfer kernel
 * @param  hspi pointer to a SPI_Handle_T structure that contains
 *              the configuration information for SPI module.
 * @param  tx_data pointer to tx data buffer
 * @param  rx_data pointer to rx data buffer
 * @param  tx_size amount of data to be tx
 * @param  rx_size amount of data to be rx
 * @return HAL_Status_T
 */
static HAL_Status_T _spi_xfer_kernel(SPI_Handle_T *hspi, uint8_t *tx_data, uint8_t *rx_data, uint32_t tx_size, uint32_t rx_size)
{
    /* Close setting related to IT and Transfer first */
    uint32_t ictrl = HAL_REG_READ(hspi->instance->ICTRL) & ~(SPI_ICTRL_TFTHOD_MASK | SPI_ICTRL_RFTHOD_MASK);
    uint32_t ctrl2 = HAL_REG_READ(hspi->instance->CTRL2) & ~(SPI_CTRL2_TXEN_MASK | SPI_CTRL2_RXEN_MASK);
    HAL_REG_WRITE(hspi->instance->ICTRL, ictrl);
    HAL_REG_WRITE(hspi->instance->CTRL2, ctrl2);

    /* set xfer info */
    SPI_STATE_EX_CLR(hspi, SPI_STATE_EX_DMA);
    _spi_xfer_info(hspi, tx_data, rx_data, tx_size, rx_size);

    /* Reset fifo */
    SPI_RESET_FIFO(hspi);

    /* Set the function for rx */
    if (hspi->rx.xfer_size)
    {
        ctrl2 |= (1 << SPI_CTRL2_RXEN_BIT);
    }

    /* Set the function for tx */
    if (hspi->tx.xfer_size)
    {
        ctrl2 |= (1 << SPI_CTRL2_TXEN_BIT);
    }

    /* Write ICTRL lastly */
    HAL_REG_WRITE(hspi->instance->CTRL2, ctrl2);
    HAL_REG_WRITE(hspi->instance->ICTRL, ictrl);

    return HAL_OK;
}
#endif /* configUSE_SPI_BLOCKING */

#if (configUSE_SPI_NONBLOCKING_IT == 1U)
/**
 * @brief  it transfer kernel
 * @param  hspi pointer to a SPI_Handle_T structure that contains
 *              the configuration information for SPI module.
 * @param  tx_data pointer to tx data buffer
 * @param  rx_data pointer to rx data buffer
 * @param  tx_size amount of data to be tx
 * @param  rx_size amount of data to be rx
 * @return HAL_Status_T
 */
static HAL_Status_T _spi_it_xfer_kernel(SPI_Handle_T *hspi, uint8_t *tx_data, uint8_t *rx_data, uint32_t tx_size, uint32_t rx_size)
{
    /* Close setting related to IT and Transfer first */
    uint32_t ictrl = HAL_REG_READ(hspi->instance->ICTRL) & ~(SPI_ICTRL_TFTHOD_MASK | SPI_ICTRL_RFTHOD_MASK | SPI_ICTRL_TFTHIEN_MASK | SPI_ICTRL_RFTHIEN_MASK);
    uint32_t ctrl2 = HAL_REG_READ(hspi->instance->CTRL2) & ~(SPI_CTRL2_TXEN_MASK | SPI_CTRL2_RXEN_MASK);
    HAL_REG_WRITE(hspi->instance->ICTRL, ictrl);
    HAL_REG_WRITE(hspi->instance->CTRL2, ctrl2);

    /* Set xfer info */
    SPI_STATE_EX_CLR(hspi, SPI_STATE_EX_DMA);
    _spi_xfer_info(hspi, tx_data, rx_data, tx_size, rx_size);

    /* Reset fifo */
    SPI_RESET_FIFO(hspi);

    /* Set the function for rx */
    if (hspi->rx.xfer_size)
    {
        uint8_t rx_th = 0;
        if (IS_SPI_MASTER(hspi))
        {
            rx_th = SPI_SET_MASTER_FIFO_RX_VAL(hspi->rx.xfer_size);
        }
        else
        {
            rx_th = SPI_SET_SLAVE_FIFO_RX_VAL(hspi->rx.xfer_size);
        }
        ictrl |= (1 << SPI_ICTRL_RFTHIEN_BIT) | (rx_th << SPI_ICTRL_RFTHOD_BIT) | (1 << SPI_ICTRL_RFORIEN_BIT);
        ctrl2 |= (1 << SPI_CTRL2_RXEN_BIT);
    }

    /* Set the function for tx */
    if (hspi->tx.xfer_size)
    {
        uint8_t tx_th = 0;
        if (IS_SPI_MASTER(hspi))
        {
            tx_th = MASTER_TX_THOD;
        }
        else
        {
            tx_th = SLAVE_TX_THOD;
        }
        /* If TX_THOD = 0, we apply TXCI to trigger interrupt*/
        if (tx_th == 0)
        {
            /* Even use TXCI, we need to use TFTHI to trigger first interrupt, and it will be shut down in IRQ handler in the first round */
            ictrl |= (1 << SPI_ICTRL_TFTHIEN_BIT) | (1 << SPI_ICTRL_TFTHOD_BIT) | (1 << SPI_ICTRL_TXCIEN_BIT) | (1 << SPI_ICTRL_TFURIEN_BIT);
        }
        else
        {
            ictrl |= (1 << SPI_ICTRL_TFTHIEN_BIT) | (tx_th << SPI_ICTRL_TFTHOD_BIT) | (1 << SPI_ICTRL_TFURIEN_BIT);
        }
        ctrl2 |= (1 << SPI_CTRL2_TXEN_BIT);
    }

    /* Write ICTRL lastly */
    HAL_REG_WRITE(hspi->instance->CTRL2, ctrl2);
    HAL_REG_WRITE(hspi->instance->ICTRL, ictrl);

    return HAL_OK;
}
/**
 * @brief spi call callback
 * @param hspi SPI handle.
 */
static void _spi_switch_cb(SPI_Handle_T *hspi)
{
    if (hspi->tx.buffer != NULL && hspi->rx.buffer == NULL)
    {
        SPI_DISABLE_IE(hspi, SPI_IE_TFURIEN);
        HAL_REG_WRITE(hspi->instance->CTRL2_b.TXEN, 0);
        SPI_STATE_EX_CLR(hspi, SPI_STATE_EX_TX_ONLY);
        /* Call user TX complete callback */
#if (configUSE_SPI_REGISTER_CALLBACKS == 1U)
        hspi->TxCpltCallback(hspi);
#else
        HAL_SPI_TxCpltCallback(hspi);
#endif
    }
    else if (hspi->rx.buffer != NULL && hspi->tx.buffer == NULL)
    {
        SPI_DISABLE_IE(hspi, SPI_IE_RFORIEN);
        SPI_DISABLE_RX_FIFO(hspi);
        SPI_DISABLE_IE(hspi, SPI_IE_RFTHIEN);
        HAL_REG_WRITE(hspi->instance->CTRL2_b.RXEN, 0);
        SPI_STATE_EX_CLR(hspi, SPI_STATE_EX_RX_ONLY);
        /* Call user RX complete callback */
#if (configUSE_SPI_REGISTER_CALLBACKS == 1U)
        hspi->RxCpltCallback(hspi);
#else
        HAL_SPI_RxCpltCallback(hspi);
#endif
    }
    else if (hspi->rx.buffer != NULL && hspi->tx.buffer != NULL)
    {
        SPI_DISABLE_IE(hspi, SPI_IE_TFURIEN);
        SPI_DISABLE_IE(hspi, SPI_IE_RFORIEN);
        SPI_DISABLE_TX_FIFO(hspi);
        SPI_DISABLE_RX_FIFO(hspi);
        SPI_DISABLE_IE(hspi, SPI_IE_RFTHIEN);
        HAL_REG_WRITE(hspi->instance->CTRL2_b.RXEN, 0);
        HAL_REG_WRITE(hspi->instance->CTRL2_b.TXEN, 0);
        SPI_STATE_EX_CLR(hspi, SPI_STATE_EX_BOTH);
        /* Call user TXRX complete callback */
#if (configUSE_SPI_REGISTER_CALLBACKS == 1U)
        hspi->TxRxCpltCallback(hspi);
#else
        HAL_SPI_TxRxCpltCallback(hspi);
#endif
    }

    /* Set SPI Ready state after transfer done */
    HAL_FLAG_WRITE(hspi->state, HAL_STATE_READY);
}
/**
 * @brief spi rx fifo length data
 * @param hspi SPI handle.
 */
static void _spi_rx_fifo_data(SPI_Handle_T *hspi, uint32_t cnt)
{
    SN_SPI0_Type *reg = hspi->instance;

    if (0U == hspi->base)
    {
        switch (cnt)
        {
            case 8:
            {
                hspi->rx.buffer[0] = HAL_REG_READ(reg->DATA) & (uint8_t)hspi->data_mask;
                hspi->rx.buffer[1] = HAL_REG_READ(reg->DATA) & (uint8_t)hspi->data_mask;
                hspi->rx.buffer[2] = HAL_REG_READ(reg->DATA) & (uint8_t)hspi->data_mask;
                hspi->rx.buffer[3] = HAL_REG_READ(reg->DATA) & (uint8_t)hspi->data_mask;
                hspi->rx.buffer[4] = HAL_REG_READ(reg->DATA) & (uint8_t)hspi->data_mask;
                hspi->rx.buffer[5] = HAL_REG_READ(reg->DATA) & (uint8_t)hspi->data_mask;
                hspi->rx.buffer[6] = HAL_REG_READ(reg->DATA) & (uint8_t)hspi->data_mask;
                hspi->rx.buffer[7] = HAL_REG_READ(reg->DATA) & (uint8_t)hspi->data_mask;
            }
            break;

            case 7:
            {
                hspi->rx.buffer[0] = HAL_REG_READ(reg->DATA) & (uint8_t)hspi->data_mask;
                hspi->rx.buffer[1] = HAL_REG_READ(reg->DATA) & (uint8_t)hspi->data_mask;
                hspi->rx.buffer[2] = HAL_REG_READ(reg->DATA) & (uint8_t)hspi->data_mask;
                hspi->rx.buffer[3] = HAL_REG_READ(reg->DATA) & (uint8_t)hspi->data_mask;
                hspi->rx.buffer[4] = HAL_REG_READ(reg->DATA) & (uint8_t)hspi->data_mask;
                hspi->rx.buffer[5] = HAL_REG_READ(reg->DATA) & (uint8_t)hspi->data_mask;
                hspi->rx.buffer[6] = HAL_REG_READ(reg->DATA) & (uint8_t)hspi->data_mask;
            }
            break;

            case 6:
            {
                hspi->rx.buffer[0] = HAL_REG_READ(reg->DATA) & (uint8_t)hspi->data_mask;
                hspi->rx.buffer[1] = HAL_REG_READ(reg->DATA) & (uint8_t)hspi->data_mask;
                hspi->rx.buffer[2] = HAL_REG_READ(reg->DATA) & (uint8_t)hspi->data_mask;
                hspi->rx.buffer[3] = HAL_REG_READ(reg->DATA) & (uint8_t)hspi->data_mask;
                hspi->rx.buffer[4] = HAL_REG_READ(reg->DATA) & (uint8_t)hspi->data_mask;
                hspi->rx.buffer[5] = HAL_REG_READ(reg->DATA) & (uint8_t)hspi->data_mask;
            }
            break;

            case 5:
            {
                hspi->rx.buffer[0] = HAL_REG_READ(reg->DATA) & (uint8_t)hspi->data_mask;
                hspi->rx.buffer[1] = HAL_REG_READ(reg->DATA) & (uint8_t)hspi->data_mask;
                hspi->rx.buffer[2] = HAL_REG_READ(reg->DATA) & (uint8_t)hspi->data_mask;
                hspi->rx.buffer[3] = HAL_REG_READ(reg->DATA) & (uint8_t)hspi->data_mask;
                hspi->rx.buffer[4] = HAL_REG_READ(reg->DATA) & (uint8_t)hspi->data_mask;
            }
            break;

            case 4:
            {
                hspi->rx.buffer[0] = HAL_REG_READ(reg->DATA) & (uint8_t)hspi->data_mask;
                hspi->rx.buffer[1] = HAL_REG_READ(reg->DATA) & (uint8_t)hspi->data_mask;
                hspi->rx.buffer[2] = HAL_REG_READ(reg->DATA) & (uint8_t)hspi->data_mask;
                hspi->rx.buffer[3] = HAL_REG_READ(reg->DATA) & (uint8_t)hspi->data_mask;
            }
            break;

            case 3:
            {
                hspi->rx.buffer[0] = HAL_REG_READ(reg->DATA) & (uint8_t)hspi->data_mask;
                hspi->rx.buffer[1] = HAL_REG_READ(reg->DATA) & (uint8_t)hspi->data_mask;
                hspi->rx.buffer[2] = HAL_REG_READ(reg->DATA) & (uint8_t)hspi->data_mask;
            }
            break;

            case 2:
            {
                hspi->rx.buffer[0] = HAL_REG_READ(reg->DATA) & (uint8_t)hspi->data_mask;
                hspi->rx.buffer[1] = HAL_REG_READ(reg->DATA) & (uint8_t)hspi->data_mask;
            }
            break;

            case 1:
            {
                hspi->rx.buffer[0] = HAL_REG_READ(reg->DATA) & (uint8_t)hspi->data_mask;
            }
            break;

            default:
                break;
        }
        hspi->rx.buffer += cnt;
    }
    else
    {
        switch (cnt)
        {
            case 8:
            {
                ((uint16_t *)hspi->rx.buffer)[0] = HAL_REG_READ(reg->DATA) & hspi->data_mask;
                ((uint16_t *)hspi->rx.buffer)[1] = HAL_REG_READ(reg->DATA) & hspi->data_mask;
                ((uint16_t *)hspi->rx.buffer)[2] = HAL_REG_READ(reg->DATA) & hspi->data_mask;
                ((uint16_t *)hspi->rx.buffer)[3] = HAL_REG_READ(reg->DATA) & hspi->data_mask;
                ((uint16_t *)hspi->rx.buffer)[4] = HAL_REG_READ(reg->DATA) & hspi->data_mask;
                ((uint16_t *)hspi->rx.buffer)[5] = HAL_REG_READ(reg->DATA) & hspi->data_mask;
                ((uint16_t *)hspi->rx.buffer)[6] = HAL_REG_READ(reg->DATA) & hspi->data_mask;
                ((uint16_t *)hspi->rx.buffer)[7] = HAL_REG_READ(reg->DATA) & hspi->data_mask;
            }
            break;

            case 7:
            {
                ((uint16_t *)hspi->rx.buffer)[0] = HAL_REG_READ(reg->DATA) & hspi->data_mask;
                ((uint16_t *)hspi->rx.buffer)[1] = HAL_REG_READ(reg->DATA) & hspi->data_mask;
                ((uint16_t *)hspi->rx.buffer)[2] = HAL_REG_READ(reg->DATA) & hspi->data_mask;
                ((uint16_t *)hspi->rx.buffer)[3] = HAL_REG_READ(reg->DATA) & hspi->data_mask;
                ((uint16_t *)hspi->rx.buffer)[4] = HAL_REG_READ(reg->DATA) & hspi->data_mask;
                ((uint16_t *)hspi->rx.buffer)[5] = HAL_REG_READ(reg->DATA) & hspi->data_mask;
                ((uint16_t *)hspi->rx.buffer)[6] = HAL_REG_READ(reg->DATA) & hspi->data_mask;
            }
            break;

            case 6:
            {
                ((uint16_t *)hspi->rx.buffer)[0] = HAL_REG_READ(reg->DATA) & hspi->data_mask;
                ((uint16_t *)hspi->rx.buffer)[1] = HAL_REG_READ(reg->DATA) & hspi->data_mask;
                ((uint16_t *)hspi->rx.buffer)[2] = HAL_REG_READ(reg->DATA) & hspi->data_mask;
                ((uint16_t *)hspi->rx.buffer)[3] = HAL_REG_READ(reg->DATA) & hspi->data_mask;
                ((uint16_t *)hspi->rx.buffer)[4] = HAL_REG_READ(reg->DATA) & hspi->data_mask;
                ((uint16_t *)hspi->rx.buffer)[5] = HAL_REG_READ(reg->DATA) & hspi->data_mask;
            }
            break;

            case 5:
            {
                ((uint16_t *)hspi->rx.buffer)[0] = HAL_REG_READ(reg->DATA) & hspi->data_mask;
                ((uint16_t *)hspi->rx.buffer)[1] = HAL_REG_READ(reg->DATA) & hspi->data_mask;
                ((uint16_t *)hspi->rx.buffer)[2] = HAL_REG_READ(reg->DATA) & hspi->data_mask;
                ((uint16_t *)hspi->rx.buffer)[3] = HAL_REG_READ(reg->DATA) & hspi->data_mask;
                ((uint16_t *)hspi->rx.buffer)[4] = HAL_REG_READ(reg->DATA) & hspi->data_mask;
            }
            break;

            case 4:
            {
                ((uint16_t *)hspi->rx.buffer)[0] = HAL_REG_READ(reg->DATA) & hspi->data_mask;
                ((uint16_t *)hspi->rx.buffer)[1] = HAL_REG_READ(reg->DATA) & hspi->data_mask;
                ((uint16_t *)hspi->rx.buffer)[2] = HAL_REG_READ(reg->DATA) & hspi->data_mask;
                ((uint16_t *)hspi->rx.buffer)[3] = HAL_REG_READ(reg->DATA) & hspi->data_mask;
            }
            break;

            case 3:
            {
                ((uint16_t *)hspi->rx.buffer)[0] = HAL_REG_READ(reg->DATA) & hspi->data_mask;
                ((uint16_t *)hspi->rx.buffer)[1] = HAL_REG_READ(reg->DATA) & hspi->data_mask;
                ((uint16_t *)hspi->rx.buffer)[2] = HAL_REG_READ(reg->DATA) & hspi->data_mask;
            }
            break;

            case 2:
            {
                ((uint16_t *)hspi->rx.buffer)[0] = HAL_REG_READ(reg->DATA) & hspi->data_mask;
                ((uint16_t *)hspi->rx.buffer)[1] = HAL_REG_READ(reg->DATA) & hspi->data_mask;
            }
            break;

            case 1:
            {
                ((uint16_t *)hspi->rx.buffer)[0] = HAL_REG_READ(reg->DATA) & hspi->data_mask;
            }
            break;

            default:
                break;
        }
        hspi->rx.buffer += cnt * 2;
    }
    hspi->rx.xfer_count -= cnt;
}
#endif /* configUSE_SPI_NONBLOCKING_IT */

#if (configUSE_SPI_REGISTER_CALLBACKS == 1U)
/**
 * @brief spi reset callback
 * @param hspi SPI handle.
 */
static void inline _spi_reset_callback(SPI_Handle_T *hspi)
{
    hspi->TxCpltCallback    = HAL_SPI_TxCpltCallback;
    hspi->RxCpltCallback    = HAL_SPI_RxCpltCallback;
    hspi->TxRxCpltCallback  = HAL_SPI_TxRxCpltCallback;
    hspi->ErrorCallback     = HAL_SPI_ErrorCallback;
    hspi->AbortCpltCallback = HAL_SPI_AbortCpltCallback;
}
#endif /* configUSE_SPI_REGISTER_CALLBACKS */
#if (configUSE_SPI_BLOCKING == 1U || configUSE_SPI_NONBLOCKING_IT == 1U)
/**
 * @brief spi tx fifo length data
 * @param hspi SPI handle.
 */
static void _spi_tx_fifo_data(SPI_Handle_T *hspi, uint32_t cnt)
{
    SN_SPI0_Type *reg = hspi->instance;

    if (0U == hspi->base)
    {
        switch (cnt)
        {
            case 8:
            {
                HAL_REG_WRITE(reg->DATA, hspi->tx.buffer[0]);
                HAL_REG_WRITE(reg->DATA, hspi->tx.buffer[1]);
                HAL_REG_WRITE(reg->DATA, hspi->tx.buffer[2]);
                HAL_REG_WRITE(reg->DATA, hspi->tx.buffer[3]);
                HAL_REG_WRITE(reg->DATA, hspi->tx.buffer[4]);
                HAL_REG_WRITE(reg->DATA, hspi->tx.buffer[5]);
                HAL_REG_WRITE(reg->DATA, hspi->tx.buffer[6]);
                HAL_REG_WRITE(reg->DATA, hspi->tx.buffer[7]);
            }
            break;

            case 7:
            {
                HAL_REG_WRITE(reg->DATA, hspi->tx.buffer[0]);
                HAL_REG_WRITE(reg->DATA, hspi->tx.buffer[1]);
                HAL_REG_WRITE(reg->DATA, hspi->tx.buffer[2]);
                HAL_REG_WRITE(reg->DATA, hspi->tx.buffer[3]);
                HAL_REG_WRITE(reg->DATA, hspi->tx.buffer[4]);
                HAL_REG_WRITE(reg->DATA, hspi->tx.buffer[5]);
                HAL_REG_WRITE(reg->DATA, hspi->tx.buffer[6]);
            }
            break;

            case 6:
            {
                HAL_REG_WRITE(reg->DATA, hspi->tx.buffer[0]);
                HAL_REG_WRITE(reg->DATA, hspi->tx.buffer[1]);
                HAL_REG_WRITE(reg->DATA, hspi->tx.buffer[2]);
                HAL_REG_WRITE(reg->DATA, hspi->tx.buffer[3]);
                HAL_REG_WRITE(reg->DATA, hspi->tx.buffer[4]);
                HAL_REG_WRITE(reg->DATA, hspi->tx.buffer[5]);
            }
            break;

            case 5:
            {
                HAL_REG_WRITE(reg->DATA, hspi->tx.buffer[0]);
                HAL_REG_WRITE(reg->DATA, hspi->tx.buffer[1]);
                HAL_REG_WRITE(reg->DATA, hspi->tx.buffer[2]);
                HAL_REG_WRITE(reg->DATA, hspi->tx.buffer[3]);
                HAL_REG_WRITE(reg->DATA, hspi->tx.buffer[4]);
            }
            break;

            case 4:
            {
                HAL_REG_WRITE(reg->DATA, hspi->tx.buffer[0]);
                HAL_REG_WRITE(reg->DATA, hspi->tx.buffer[1]);
                HAL_REG_WRITE(reg->DATA, hspi->tx.buffer[2]);
                HAL_REG_WRITE(reg->DATA, hspi->tx.buffer[3]);
            }
            break;

            case 3:
            {
                HAL_REG_WRITE(reg->DATA, hspi->tx.buffer[0]);
                HAL_REG_WRITE(reg->DATA, hspi->tx.buffer[1]);
                HAL_REG_WRITE(reg->DATA, hspi->tx.buffer[2]);
            }
            break;

            case 2:
            {
                HAL_REG_WRITE(reg->DATA, hspi->tx.buffer[0]);
                HAL_REG_WRITE(reg->DATA, hspi->tx.buffer[1]);
            }
            break;

            case 1:
            {
                HAL_REG_WRITE(reg->DATA, hspi->tx.buffer[0]);
            }
            break;

            default:
                break;
        }
        hspi->tx.buffer += cnt;
    }
    else
    {
        uint16_t *buff = (uint16_t *)hspi->tx.buffer;
        switch (cnt)
        {
            case 8:
            {
                HAL_REG_WRITE(reg->DATA, buff[0]);
                HAL_REG_WRITE(reg->DATA, buff[1]);
                HAL_REG_WRITE(reg->DATA, buff[2]);
                HAL_REG_WRITE(reg->DATA, buff[3]);
                HAL_REG_WRITE(reg->DATA, buff[4]);
                HAL_REG_WRITE(reg->DATA, buff[5]);
                HAL_REG_WRITE(reg->DATA, buff[6]);
                HAL_REG_WRITE(reg->DATA, buff[7]);
            }
            break;

            case 7:
            {
                HAL_REG_WRITE(reg->DATA, buff[0]);
                HAL_REG_WRITE(reg->DATA, buff[1]);
                HAL_REG_WRITE(reg->DATA, buff[2]);
                HAL_REG_WRITE(reg->DATA, buff[3]);
                HAL_REG_WRITE(reg->DATA, buff[4]);
                HAL_REG_WRITE(reg->DATA, buff[5]);
                HAL_REG_WRITE(reg->DATA, buff[6]);
            }
            break;

            case 6:
            {
                HAL_REG_WRITE(reg->DATA, buff[0]);
                HAL_REG_WRITE(reg->DATA, buff[1]);
                HAL_REG_WRITE(reg->DATA, buff[2]);
                HAL_REG_WRITE(reg->DATA, buff[3]);
                HAL_REG_WRITE(reg->DATA, buff[4]);
                HAL_REG_WRITE(reg->DATA, buff[5]);
            }
            break;

            case 5:
            {
                HAL_REG_WRITE(reg->DATA, buff[0]);
                HAL_REG_WRITE(reg->DATA, buff[1]);
                HAL_REG_WRITE(reg->DATA, buff[2]);
                HAL_REG_WRITE(reg->DATA, buff[3]);
                HAL_REG_WRITE(reg->DATA, buff[4]);
            }
            break;

            case 4:
            {
                HAL_REG_WRITE(reg->DATA, buff[0]);
                HAL_REG_WRITE(reg->DATA, buff[1]);
                HAL_REG_WRITE(reg->DATA, buff[2]);
                HAL_REG_WRITE(reg->DATA, buff[3]);
            }
            break;

            case 3:
            {
                HAL_REG_WRITE(reg->DATA, buff[0]);
                HAL_REG_WRITE(reg->DATA, buff[1]);
                HAL_REG_WRITE(reg->DATA, buff[2]);
            }
            break;

            case 2:
            {
                HAL_REG_WRITE(reg->DATA, buff[0]);
                HAL_REG_WRITE(reg->DATA, buff[1]);
            }
            break;

            case 1:
            {
                HAL_REG_WRITE(reg->DATA, buff[0]);
            }
            break;

            default:
                break;
        }
        hspi->tx.buffer += cnt * 2;
    }
    hspi->tx.xfer_count -= cnt;
}

/**
 * @brief spi tx fifo length dummy
 * @param hspi SPI handle.
 */
static void _spi_tx_fifo_dummy(SPI_Handle_T *hspi, uint32_t cnt)
{
    SN_SPI0_Type *reg = hspi->instance;

    if (0U == hspi->base)
    {
        switch (cnt)
        {
            case 8:
                HAL_REG_WRITE(reg->DATA, (uint8_t)hspi->tx.dummy);
            case 7:
                HAL_REG_WRITE(reg->DATA, (uint8_t)hspi->tx.dummy);
            case 6:
                HAL_REG_WRITE(reg->DATA, (uint8_t)hspi->tx.dummy);
            case 5:
                HAL_REG_WRITE(reg->DATA, (uint8_t)hspi->tx.dummy);
            case 4:
                HAL_REG_WRITE(reg->DATA, (uint8_t)hspi->tx.dummy);
            case 3:
                HAL_REG_WRITE(reg->DATA, (uint8_t)hspi->tx.dummy);
            case 2:
                HAL_REG_WRITE(reg->DATA, (uint8_t)hspi->tx.dummy);
            case 1:
                HAL_REG_WRITE(reg->DATA, (uint8_t)hspi->tx.dummy);
        }
    }
    else
    {
        switch (cnt)
        {
            case 8:
                HAL_REG_WRITE(reg->DATA, (uint16_t)hspi->tx.dummy);
            case 7:
                HAL_REG_WRITE(reg->DATA, (uint16_t)hspi->tx.dummy);
            case 6:
                HAL_REG_WRITE(reg->DATA, (uint16_t)hspi->tx.dummy);
            case 5:
                HAL_REG_WRITE(reg->DATA, (uint16_t)hspi->tx.dummy);
            case 4:
                HAL_REG_WRITE(reg->DATA, (uint16_t)hspi->tx.dummy);
            case 3:
                HAL_REG_WRITE(reg->DATA, (uint16_t)hspi->tx.dummy);
            case 2:
                HAL_REG_WRITE(reg->DATA, (uint16_t)hspi->tx.dummy);
            case 1:
                HAL_REG_WRITE(reg->DATA, (uint16_t)hspi->tx.dummy);
        }
    }
    hspi->tx.xfer_count -= cnt;
}
#endif /* configUSE_SPI_BLOCKING || configUSE_SPI_NONBLOCKING_IT */

/**
 * @brief spi set config
 * @param hspi SPI handle.
 */
static uint32_t _spi_set_config(SPI_Handle_T *hspi)
{
    /* verify if satisfying the inequation "(3.5 x Tpclk) < (Tsclk x SDL_Length)" */
    uint32_t sspclk = 0;
    uint32_t pclk   = HAL_RCC_GetAPB1CLKFreq() / 1000;
    if (hspi->instance == SN_SPI0)
    {
        sspclk = (__HAL_RCC_GET_SSP0_CLKSEL() == 0 ? HAL_RCC_GetHCLKFreq() / 1000 : HAL_RCC_GetPLLCLKFreq() / 1000);
    }
    else if (hspi->instance == SN_SPI1)
    {
        sspclk = (__HAL_RCC_GET_SSP1_CLKSEL() == 0 ? HAL_RCC_GetHCLKFreq() / 1000 : HAL_RCC_GetPLLCLKFreq() / 1000);
    }
    else
    {
        sspclk = (__HAL_RCC_GET_SSP2_CLKSEL() == 0 ? HAL_RCC_GetHCLKFreq() / 1000 : HAL_RCC_GetPLLCLKFreq() / 1000);
    }
    if (7 * sspclk >= 2 * 2 * (hspi->init.sclk_div + 1) * pclk * hspi->init.data_size)
    {
        // hspi->error_code = HAL_ERROR_SPECIFY;
        return HAL_ERROR;
    }

    /* spi/micro-wire mode select */
    switch (hspi->init.frame_format)
    {
        default:
            return HAL_ERROR;
        case 0:            // default is spi mode currently
        case SPI_FFMT_SPI: /* switch to SPI mode */
            HAL_REG_WRITE(hspi->instance->CTRL0_b.FFMT, 1);
            HAL_REG_WRITE(hspi->instance->CTRL0_b.SPIFSPO, hspi->init.fs_polarity);
            /* Padding Cycle Length for SPI mode only. In the master
               mode, SPI will wait (PCL + 1) SCLK cycles between each
               successive transfer. Maybe user adjust it. */
            HAL_REG_WRITE(hspi->instance->CTRL3_b.PCL, 0);
            // HAL_REG_WRITE(hspi->instance->CTRL2_b.TXDOE, 1);
            /* data frame format */
            switch (hspi->init.clk_polarity)
            {
                case SPI_CLK_POLARITY_LOW:  // default
                case SPI_CLK_POLARITY_HIGH: //
                    HAL_REG_WRITE(hspi->instance->CTRL0_b.SCLKPO, hspi->init.clk_polarity);
                    break;
                default:
                    return HAL_ERROR;
            }
            switch (hspi->init.clk_phase)
            {
                case SPI_CLK_PHASE_1EDGE: // default
                case SPI_CLK_PHASE_2EDGE: //
                    HAL_REG_WRITE(hspi->instance->CTRL0_b.SCLKPH, hspi->init.clk_phase);
                    break;
                default:
                    return HAL_ERROR;
            }
            break;
        case SPI_FFMT_MWR: /* switch to micro-wire mode */
            HAL_REG_WRITE(hspi->instance->CTRL0_b.FFMT, 2);
            HAL_REG_WRITE(hspi->instance->CTRL0_b.FSPO, hspi->init.fs_polarity);
            break;
    }

    /* Master/Slaver mode select */
    switch (hspi->init.mode)
    {
        default:
            return HAL_ERROR;

        case SPI_MODE_MASTER: /* SPI Master (Output on MOSI, Input on MISO), default */
        {
            HAL_REG_WRITE(hspi->instance->CTRL0_b.FSFDBK, 1);
            HAL_REG_WRITE(hspi->instance->CTRL0_b.SCLKFDBK, 1);
            HAL_REG_WRITE(hspi->instance->CTRL1_b.SCLKDIV, hspi->init.sclk_div);
            HAL_REG_WRITE(hspi->instance->CTRL0_b.OPM, 2);
            HAL_REG_WRITE(hspi->instance->CTRL0_b.SPICONTX, 1);
        }
        break;

        case SPI_MODE_SLAVE: /* SPI Slave  (Output on MISO, Input on MOSI) */
        {
            HAL_REG_WRITE(hspi->instance->CTRL0_b.FSFDBK, 0);
            HAL_REG_WRITE(hspi->instance->CTRL0_b.SCLKFDBK, 0);
            HAL_REG_WRITE(hspi->instance->CTRL0_b.OPM, 0);
            HAL_REG_WRITE(hspi->instance->CTRL2_b.TXDOE, 1);
        }
        break;
    }

    /* Config bit order */
    switch (hspi->init.first_bit)
    {
        default:
            return HAL_ERROR;

        case SPI_FIRST_BIT_MSB: // default
        {
            HAL_REG_WRITE(hspi->instance->CTRL0_b.LSB, 0);
        }
        break;

        case SPI_FIRST_BIT_LSB:
        {
            HAL_REG_WRITE(hspi->instance->CTRL0_b.LSB, 1);
            HAL_REG_WRITE(hspi->instance->CTRL2_b.TXDOE, 1);
        }
        break;
    }

    /* config data bits */
    if (hspi->init.data_size >= SPI_DATA_LEN_MIN && hspi->init.data_size <= SPI_DATA_LEN_MAX)
    {
        HAL_REG_WRITE(hspi->instance->CTRL1_b.SDL, hspi->init.data_size - 1);
        hspi->base      = (hspi->init.data_size - 1) / 8;
        hspi->data_mask = ((1 << (HAL_REG_READ(hspi->instance->CTRL1_b.SDL) + 1)) - 1);
    }
    else
    {
        return HAL_ERROR;
    }

    /* Set Tx default value */
    hspi->tx.dummy = hspi->init.tx_default;

    /* Clear fifo status */
    HAL_REG_WRITE(hspi->instance->CTRL2_b.RESET, 1);

    return HAL_OK;
}

/**
 * @brief  transfer kernel info setting
 * @param  hspi pointer to a SPI_Handle_T structure that contains
 *              the configuration information for SPI module.
 * @param  tx_data pointer to tx data buffer
 * @param  rx_data pointer to rx data buffer
 * @param  tx_size amount of data to be tx
 * @param  rx_size amount of data to be rx
 * @return HAL_Status_T
 */
static void inline _spi_xfer_info(SPI_Handle_T *hspi, uint8_t *tx_data, uint8_t *rx_data, uint32_t tx_size, uint32_t rx_size)
{
    /* clear error code*/
    CLEAR_ERROR_CODE(hspi);

    /* reset fifo */
    SPI_RESET_FIFO(hspi);

    /* Size to half-word when not dma. */
    if (SPI_STATE_EX_LACK(hspi, SPI_STATE_EX_DMA))
    {
        tx_size /= (hspi->base + 1);
        rx_size /= (hspi->base + 1);
    }

    /* Set TX Data */
    hspi->tx.buffer     = (void *)tx_data;
    hspi->tx.xfer_size  = tx_size;
    hspi->tx.xfer_count = tx_size;

    /* Set RX Data */
    hspi->rx.buffer     = (void *)rx_data;
    hspi->rx.xfer_size  = rx_size;
    hspi->rx.xfer_count = rx_size;
}
#endif /* configUSE_SPI_PERIPHERAL */
