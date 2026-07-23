/***********************
 * SN34F78X SDK V2.1.4 *
 ***********************/

#include "sn34f78x_hal_uart.h"

#if (configUSE_UART_PERIPHERAL == 1)

/*
================================================================================
            ##### Initialization/De-initialization functions #####
================================================================================
*/
/**
 * @brief  Initializes the UART mode according to the specified parameters in the UART_Init_T and create the associated handle.
 * @param  huart Pointer to a UART_Handle_T structure that contains the configuration information for the specified UART module.
 * @retval HAL status
 */
HAL_Status_T HAL_UART_Init(UART_Handle_T *huart)
{
    /* Check the UART handle */
    ASSERT_PARAM(huart);

    /* Check the parameters */
    if (huart->init.hw_flow_ctl != UART_HWCONTROL_NONE)
    {
        /* The hardware flow control is available only for UART 0/1/2/3*/
        ASSERT_PARAM(IS_UART_HWFLOW_INSTANCE(huart->instance));

        /* Check the parameters. */
        ASSERT_PARAM(IS_UART_HWFLOW_CONTROL(huart->init.hw_flow_ctl));
    }
    else
    {
        /* Check the UART0/1/2/3/4/5 Port.   */
        ASSERT_PARAM(IS_PERIPHERAL_UART_INSTANCE(huart->instance));
    }

    /* Check the parameters */
    /* The baud rate should be set according to the calculation of the system clock to determine whether it is supported. */
    /* Eg: The system clock frequency is 96M and the baud rate is set to 3000000bps/2000000bps/1500000bps/1444444bps/1000000bps/460800bps/307200bps */
    ASSERT_PARAM(IS_UART_BAUD_RATE(huart->init.baud_rate));
    ASSERT_PARAM(IS_UART_WORD_LENGTH(huart->init.word_length));
    ASSERT_PARAM(IS_UART_STOP_BITS(huart->init.stop_bits));
    ASSERT_PARAM(IS_UART_PARITY_ODD(huart->init.parity));

    if (huart->state == HAL_STATE_RESET)
    {
#if (configUSE_UART_REGISTER_CALLBACKS == 1)
        _uart_init_callbacks_default(huart);
        if (huart->MspInitCallback == NULL)
        {
            huart->MspInitCallback = HAL_UART_MspInit;
        }

        /* Init the low level hardware */
        huart->MspInitCallback(huart);
#else
        HAL_UART_MspInit(huart);
#endif /* configUSE_UART_REGISTER_CALLBACKS */

#if (configUSE_UART_NONBLOCKING_DMA == 1)
        if ((huart->hdmatx != NULL) || (huart->hdmarx != NULL))
        {
            if (huart->hdmatx != NULL)
            {
                RET_FLAG_FALSE(_uart_dma_tx_check(huart) == HAL_OK, HAL_ERROR);

                HAL_DMA_RegisterCallback(huart->hdmatx, HAL_DMA_XFER_CPLT_CB_ID, _uart_dma_tx_cplt_callback);
                HAL_DMA_RegisterCallback(huart->hdmatx, HAL_DMA_XFER_ERROR_CB_ID, _uart_dma_tx_err_callback);
                HAL_DMA_RegisterCallback(huart->hdmatx, HAL_DMA_XFER_ABORT_CB_ID, _uart_dma_tx_abort_callback);
            }

            if (huart->hdmarx != NULL)
            {
                RET_FLAG_FALSE(_uart_dma_rx_check(huart) == HAL_OK, HAL_ERROR);

                HAL_DMA_RegisterCallback(huart->hdmarx, HAL_DMA_XFER_CPLT_CB_ID, _uart_dma_rx_cplt_callback);
                HAL_DMA_RegisterCallback(huart->hdmarx, HAL_DMA_XFER_ERROR_CB_ID, _uart_dma_rx_err_callback);
                HAL_DMA_RegisterCallback(huart->hdmarx, HAL_DMA_XFER_ABORT_CB_ID, _uart_dma_rx_abort_callback);
            }
        }
#endif
    }
    else
    {
        huart->error_code = HAL_ERROR_REINIT;
        return HAL_ERROR;
    }

    /* Set the state to busy  */
    huart->state = HAL_STATE_BUSY;

    RET_FLAG_FALSE(_uart_set_config(huart) == HAL_OK, HAL_ERROR);

    /* Update the UART state */
    CLEAR_ERROR_CODE(huart);
    huart->state     = HAL_STATE_READY;
    huart->rx_state  = HAL_STATE_READY;
    huart->state_ext = EX_STATE_NONE;

    return HAL_OK;
}

/**
 * @brief  DeInitializes the UART peripheral.
 * @param  huart Pointer to a UART_Handle_T structure that contains the configuration information for the specified UART module.
 * @retval HAL status
 */
HAL_Status_T HAL_UART_DeInit(UART_Handle_T *huart)
{
    /* Check the UART handle */
    ASSERT_PARAM(huart);

    /* Check the UART instance */
    ASSERT_PARAM(IS_PERIPHERAL_UART_INSTANCE(huart->instance));

    /* Set the state to busy */
    huart->state = HAL_STATE_BUSY;

#if (configUSE_UART_REGISTER_CALLBACKS == 1)
    if (huart->MspDeInitCallback == NULL)
    {
        huart->MspDeInitCallback = HAL_UART_MspDeInit;
    }
    /* DeInit the low level hardware */
    huart->MspDeInitCallback(huart);
#else

    /* DeInit the low level hardware */
    HAL_UART_MspDeInit(huart);
#endif /* MspDeInitCallback */

#if (configUSE_UART_NONBLOCKING_DMA == 1)

    if (huart->hdmatx != NULL)
    {
        HAL_DMA_UnRegisterCallback(huart->hdmatx, HAL_DMA_ALL_CB_ID);
    }

    if (huart->hdmarx != NULL)
    {
        HAL_DMA_UnRegisterCallback(huart->hdmarx, HAL_DMA_ALL_CB_ID);
    }
#endif

    /* Update the UART state */
    CLEAR_ERROR_CODE(huart);
    huart->state     = HAL_STATE_RESET;
    huart->rx_state  = HAL_STATE_RESET;
    huart->state_ext = EX_STATE_NONE;

    /* Release the process protection */
    GIVE_MUTEX(huart);

    return HAL_OK;
}

/**
 * @brief  UART MSP Init.
 * @param  huart Pointer to a UART_Handle_T structure that contains the configuration information for the specified UART module.
 * @retval None
 */
__weak void HAL_UART_MspInit(UART_Handle_T *huart)
{
    /* Prevent unused argument(s) compilation warning */
    UNUSED(huart);
    /* NOTE: This function should not be modified, when the callback is needed, the HAL_UART_MspInit could be implemented in the user file */
}

/**
 * @brief  UART MSP DeInit.
 * @param  huart Pointer to a UART_Handle_T structure that contains the configuration information for the specified UART module.
 * @retval None
 */
__weak void HAL_UART_MspDeInit(UART_Handle_T *huart)
{
    /* Prevent unused argument(s) compilation warning */
    UNUSED(huart);
    /* NOTE: This function should not be modified, when the callback is needed, the HAL_UART_MspDeInit could be implemented in the user file */
}

#if (configUSE_UART_REGISTER_CALLBACKS == 1)
/**
 * @brief  Register a User UART Callback To be used instead of the weak predefined callback
 * @param  huart Pointer to a UART_Handle_T structure that contains the configuration information for the specified UART module.
 * @param  callback_id ID of the callback to be registered
 *         This parameter can be one of the following values:
 *           @arg @ref HAL_UART_TX_COMPLETE_CB_ID UART Tx Complete Callback ID
 *           @arg @ref HAL_UART_RX_COMPLETE_CB_ID UART Rx Complete Callback ID
 *           @arg @ref HAL_UART_ABORT_COMPLETE_CB_ID UART Abort Complete Callback ID
 *           @arg @ref HAL_UART_ABORT_TRANSMIT_COMPLETE_CB_ID UART Abort Transmit Complete Callback ID
 *           @arg @ref HAL_UART_ABORT_RECEIVE_COMPLETE_CB_ID UART Abort Receive Complete Callback ID
 *           @arg @ref HAL_UART_ERROR_CB_ID UART Error Callback ID
 *           @arg @ref HAL_UART_MSPINIT_CB_ID MspInit Callback ID
 *           @arg @ref HAL_UART_MSPDEINIT_CB_ID MspDeInit Callback ID
 * @param  callback pointer to the Callback function
 * @retval HAL status
 */
HAL_Status_T HAL_UART_RegisterCallback(UART_Handle_T *huart, HAL_UART_CallbackID_T callback_id, UART_Callback_T callback)
{
    /* Check the UART handle */
    ASSERT_PARAM(huart);

    /* Check the UART instance */
    ASSERT_PARAM(IS_PERIPHERAL_UART_INSTANCE(huart->instance));

    HAL_Status_T status = HAL_OK;
    CLEAR_ERROR_CODE(huart);

    /* Protect the process */
    TAKE_MUTEX(huart);

    switch (callback_id)
    {
        case HAL_UART_TX_COMPLETE_CB_ID:
        {
            huart->TxCpltCallback = callback;
        }
        break;

        case HAL_UART_RX_COMPLETE_CB_ID:
        {
            huart->RxCpltCallback = callback;
        }
        break;

        case HAL_UART_ABORT_COMPLETE_CB_ID:
        {
            huart->AbortCpltCallback = callback;
        }
        break;

        case HAL_UART_ABORT_TRANSMIT_COMPLETE_CB_ID:
        {
            huart->AbortTransmitCpltCallback = callback;
        }
        break;

        case HAL_UART_ABORT_RECEIVE_COMPLETE_CB_ID:
        {
            huart->AbortReceiveCpltCallback = callback;
        }
        break;

        case HAL_UART_ERROR_CB_ID:
        {
            huart->ErrorCallback = callback;
        }
        break;

        case HAL_UART_MSPINIT_CB_ID:
        {
            huart->MspInitCallback = callback;
        }
        break;

        case HAL_UART_MSPDEINIT_CB_ID:
        {
            huart->MspDeInitCallback = callback;
        }
        break;

        default:
        {
            /* Return error status */
            status = HAL_ERROR;
        }
        break;
    }

    /* Release the process protection */
    GIVE_MUTEX(huart);

    return status;
}

/**
 * @brief  Unregister an UART Callback, UART callback is redirected to the weak predefined callback
 * @param  huart Pointer to a UART_Handle_T structure that contains the configuration information for the specified UART module.
 * @param  callback_id ID of the callback to be unregistered
 *         This parameter can be one of the following values:
 *           @arg @ref HAL_UART_TX_COMPLETE_CB_ID UART Tx Complete Callback ID
 *           @arg @ref HAL_UART_RX_COMPLETE_CB_ID UART Rx Complete Callback ID
 *           @arg @ref HAL_UART_ABORT_COMPLETE_CB_ID UART Abort Complete Callback ID
 *           @arg @ref HAL_UART_ABORT_TRANSMIT_COMPLETE_CB_ID UART Abort Transmit Complete Callback ID
 *           @arg @ref HAL_UART_ABORT_RECEIVE_COMPLETE_CB_ID UART Abort Receive Complete Callback ID
 *           @arg @ref HAL_UART_ERROR_CB_ID UART Error Callback ID
 *           @arg @ref HAL_UART_MSPINIT_CB_ID MspInit Callback ID
 *           @arg @ref HAL_UART_MSPDEINIT_CB_ID MspDeInit Callback ID
 * @retval HAL status
 */
HAL_Status_T HAL_UART_UnRegisterCallback(UART_Handle_T *huart, HAL_UART_CallbackID_T callback_id)
{
    /* Check the UART handle */
    ASSERT_PARAM(huart);

    /* Check the UART instance */
    ASSERT_PARAM(IS_PERIPHERAL_UART_INSTANCE(huart->instance));

    HAL_Status_T status = HAL_OK;
    CLEAR_ERROR_CODE(huart);

    /* Protect the process */
    TAKE_MUTEX(huart);

    switch (callback_id)
    {
        case HAL_UART_TX_COMPLETE_CB_ID:
        {
            huart->TxCpltCallback = HAL_UART_TxCpltCallback;
        }
        break;

        case HAL_UART_RX_COMPLETE_CB_ID:
        {
            huart->RxCpltCallback = HAL_UART_RxCpltCallback;
        }
        break;

        case HAL_UART_ABORT_COMPLETE_CB_ID:
        {
            huart->AbortCpltCallback = HAL_UART_AbortCpltCallback;
        }
        break;

        case HAL_UART_ABORT_TRANSMIT_COMPLETE_CB_ID:
        {
            huart->AbortTransmitCpltCallback = HAL_UART_AbortTransmitCpltCallback;
        }
        break;

        case HAL_UART_ABORT_RECEIVE_COMPLETE_CB_ID:
        {
            huart->AbortReceiveCpltCallback = HAL_UART_AbortReceiveCpltCallback;
        }
        break;

        case HAL_UART_ERROR_CB_ID:
        {
            huart->ErrorCallback = HAL_UART_ErrorCallback;
        }
        break;

        case HAL_UART_MSPINIT_CB_ID:
        {
            huart->MspInitCallback = HAL_UART_MspInit;
        }
        break;

        case HAL_UART_MSPDEINIT_CB_ID:
        {
            huart->MspDeInitCallback = HAL_UART_MspDeInit;
        }
        break;

        default:
        {
            /* Return error status */
            status = HAL_ERROR;
        }
        break;
    }

    /* Release the process protection */
    GIVE_MUTEX(huart);

    return status;
}
#endif /* configUSE_UART_REGISTER_CALLBACKS */

/*
================================================================================
            ##### Peripheral State and Errors functions #####
================================================================================
*/
/**
 * @brief  Returns the UART state.
 * @param  huart Pointer to a UART_Handle_T structure that contains the configuration information for the specified UART module.
 * @retval HAL state
 */
HAL_State_T HAL_UART_GetState(UART_Handle_T *huart)
{
    return (HAL_State_T)(huart->state | huart->rx_state);
}

/**
 * @brief  Return the UART error code.
 * @param  huart Pointer to a UART_Handle_T structure that contains the configuration information for the specified UART module.
 * @retval UART error code
 */
uint32_t HAL_UART_GetError(UART_Handle_T *huart)
{
    /* Check the UART handle */
    ASSERT_PARAM(huart);
    /* Check the UART instance */
    ASSERT_PARAM(IS_PERIPHERAL_UART_INSTANCE(huart->instance));

    return huart->error_code;
}

/*
================================================================================
            ##### I/O Operation functions #####
================================================================================
*/
#if (configUSE_UART_BLOCKING == 1)
/**
 * @brief  Sends an amount of data in blocking mode.
 * @param  huart Pointer to a UART_Handle_T structure that contains the configuration information for the specified UART module.
 * @param  data Pointer to data buffer.
 * @param  size Amount of data elements to be sent.
 * @param  timeout Timeout duration.
 * @retval HAL status
 */
HAL_Status_T HAL_UART_Transmit(UART_Handle_T *huart, uint8_t *data, uint16_t size, uint32_t timeout)
{
    HAL_Status_T status = HAL_OK;

    /* Check the UART handle */
    ASSERT_PARAM(huart);
    /* Check the parameters */
    ASSERT_PARAM(data);
    ASSERT_PARAM(size);

    if (huart->state == HAL_STATE_READY)
    {
        /* Protect the process */
        TAKE_MUTEX(huart);

        /* Update the UART state */
        huart->state = HAL_STATE_BUSY_TX;
        CLEAR_ERROR_CODE(huart);

        /* Init tick_start for timeout management */
        uint32_t tick_start = HAL_GetTick();

        /* Set TX runtime info */
        huart->tx.buffer     = data;
        huart->tx.xfer_size  = size;
        huart->tx.xfer_count = size;
        huart->tx.abort      = 0;

        SN_UART0_Type *instance = huart->instance;

        /* Release the process protection */
        /* Ps: Protect only the data assignment phase. Rx and Tx support Duplex. */
        GIVE_MUTEX(huart);

        while (huart->tx.xfer_count > 0U)
        {
            /* Check that the TX FIFO is full */
            if (HAL_REG_READ(instance->II_b.TXFIFOFULL) == 0)
            {
                /* Send data */
                HAL_REG_WRITE(instance->TH, *(huart->tx.buffer));
                huart->tx.buffer++;
                huart->tx.xfer_count--;
            }

            /* Exit if it is abruptly terminated */
            if (huart->tx.abort == 1)
            {
                break;
            }

            /* Check timeout */
            BREAK_PROC_TIMEOUT(huart, tick_start, timeout);
        }

        /* Wait for TEMT */
        while (HAL_REG_READ(instance->LS_b.TEMT) == 0)
        {
            __NOP();
        }

        /* Update the UART state */
        huart->state = HAL_STATE_READY;
    }
    else if (huart->state == HAL_STATE_BUSY_TX)
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
 * @brief  Receives an amount of data in blocking mode.
 * @param  huart Pointer to a UART_Handle_T structure that contains the configuration information for the specified UART module.
 * @param  data Pointer to data buffer.
 * @param  size Amount of data elements to be received.
 * @param  timeout Timeout duration.
 * @retval HAL status
 */
HAL_Status_T HAL_UART_Receive(UART_Handle_T *huart, uint8_t *data, uint16_t size, uint32_t timeout)
{
    HAL_Status_T status = HAL_OK;

    /* Check the UART handle */
    ASSERT_PARAM(huart);
    /* Check the parameters */
    ASSERT_PARAM(data);
    ASSERT_PARAM(size);

    if (huart->rx_state == HAL_STATE_READY)
    {
        /* Protect the process */
        TAKE_MUTEX(huart);

        /* Update the RX state */
        huart->rx_state = HAL_STATE_BUSY_RX;
        CLEAR_ERROR_CODE(huart);

        /* Init tick_start for timeout management */
        uint32_t tick_start = HAL_GetTick();

        /* Set RX runtime info */
        huart->rx.buffer     = data;
        huart->rx.xfer_size  = size;
        huart->rx.xfer_count = size;
        huart->rx.abort      = 0;

        SN_UART0_Type *instance = huart->instance;
        /* Reset RX FIFO | Set RX FIFO Trigger Level (Because register are W, the all need to be reset each time)*/
        HAL_REG_WRITE(instance->FIFOCTRL, UART_FIFOCTRL_RXFIFORST | UART_FIFOCTRL_FIFOEN);

        /* Release the process protection */
        GIVE_MUTEX(huart);

        while (huart->rx.xfer_count > 0U)
        {
            /* Check RDR (receiver data ready) flag */
            if (HAL_REG_READ(instance->LS_b.RDR) == 1)
            {
                /* Receive data */
                *(huart->rx.buffer) = HAL_REG_READ(instance->RB_b.RB);
                huart->rx.buffer++;
                huart->rx.xfer_count--;
            }

            /* Check abort */
            if (huart->rx.abort == 1)
            {
                break;
            }

            /* Check timeout */
            BREAK_PROC_TIMEOUT(huart, tick_start, timeout);
        }

        /* Update the RX state */
        huart->rx_state = HAL_STATE_READY;
    }
    else if (huart->rx_state == HAL_STATE_BUSY_RX)
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
#endif /* configUSE_UART_BLOCKING */

#if (configUSE_UART_NONBLOCKING_IT == 1)
/**
 * @brief  Sends an amount of data in non-blocking mode.
 * @param  huart Pointer to a UART_Handle_T structure that contains the configuration information for the specified UART module.
 * @param  data Pointer to data buffer.
 * @param  size Amount of data elements to be sent
 * @retval HAL status
 */
HAL_Status_T HAL_UART_Transmit_IT(UART_Handle_T *huart, uint8_t *data, uint16_t size)
{
    HAL_Status_T status = HAL_OK;

    /* Check the UART handle */
    ASSERT_PARAM(huart);
    /* Check the parameters */
    ASSERT_PARAM(data);
    ASSERT_PARAM(size);

    if (huart->state == HAL_STATE_READY)
    {
        /* Protect the process */
        TAKE_MUTEX(huart);

        /* Update the UART state */
        huart->state = HAL_STATE_BUSY_TX;
        CLEAR_ERROR_CODE(huart);

        /* Set TX runtime info */
        huart->tx.buffer     = data;
        huart->tx.xfer_size  = size;
        huart->tx.xfer_count = size;
        huart->tx.abort      = 0;

        SN_UART0_Type *instance = (SN_UART0_Type *)huart->instance;

        /* Get Trigger level of RX FIFO */
        uint8_t RXTL_cnt = _uart_get_rxtl(huart);
        /* Reset TX FIFO (Because register are W, the all need to be reset each time)*/
        HAL_REG_WRITE(instance->FIFOCTRL, UART_FIFOCTRL_TXFIFORST | UART_FIFOCTRL_RXTL(RXTL_cnt) | UART_FIFOCTRL_FIFOEN);
        /* Enable THRE */
        HAL_REG_SBIT(instance->IE, UART_IE_THREIE);

        /* Release the process protection */
        GIVE_MUTEX(huart);
    }
    else if (huart->state == HAL_STATE_BUSY_TX)
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
 * @brief  Receives an amount of data in non-blocking mode.
 * @param  huart Pointer to a UART_Handle_T structure that contains the configuration information for the specified UART module.
 * @param  data Pointer to data buffer.
 * @param  size Amount of data elements to be received.
 * @retval HAL status
 */
HAL_Status_T HAL_UART_Receive_IT(UART_Handle_T *huart, uint8_t *data, uint16_t size)
{
    HAL_Status_T status = HAL_OK;

    /* Check the UART handle */
    ASSERT_PARAM(huart);
    /* Check the parameters */
    ASSERT_PARAM(data);
    ASSERT_PARAM(size);

    if (huart->rx_state == HAL_STATE_READY)
    {
        /* Protect the process */
        TAKE_MUTEX(huart);

        /* Update the UART state */
        huart->rx_state = HAL_STATE_BUSY_RX;
        CLEAR_ERROR_CODE(huart);

        /* Set RX runtime info */
        huart->rx.buffer     = data;
        huart->rx.xfer_size  = size;
        huart->rx.xfer_count = size;
        huart->rx.abort      = 0;

        SN_UART0_Type *instance = (SN_UART0_Type *)huart->instance;
        /* Get Trigger level of RX FIFO */
        uint8_t RXTL_cnt = _uart_get_rxtl(huart);
        /* Reset RX FIFO | Set RX FIFO Trigger Level(Because register are W, the all need to be reset each time) */
        HAL_REG_WRITE(instance->FIFOCTRL, UART_FIFOCTRL_RXFIFORST | UART_FIFOCTRL_RXTL(RXTL_cnt) | UART_FIFOCTRL_FIFOEN);
        /* Enable RDA */
        HAL_REG_SBIT(instance->IE, UART_IE_RDAIE);
        /* Enable RLS (Overrun error (OE), Parity error (PE), Framing error (FE) and Break interrupt (BI)) */
        HAL_REG_SBIT(instance->IE, UART_IE_RLSIE);

        /* Release the process protection */
        GIVE_MUTEX(huart);
    }
    else if (huart->rx_state == HAL_STATE_BUSY_RX)
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
#endif /* configUSE_UART_NONBLOCKING_IT */

#if (configUSE_UART_NONBLOCKING_DMA == 1)
/**
 * @brief  Sends an amount of data in DMA mode.
 * @param  huart  Pointer to a UART_Handle_T structure that contains
 *                the configuration information for the specified UART module.
 * @param  data Pointer to data buffer (u8 or u16 data elements).
 * @param  size  Amount of data elements (u8 or u16) to be sent
 * @retval HAL status
 */
HAL_Status_T HAL_UART_Transmit_DMA(UART_Handle_T *huart, uint8_t *data, uint16_t size)
{
    HAL_Status_T status = HAL_OK;

    /* Check the UART handle */
    ASSERT_PARAM(huart);
    ASSERT_PARAM(huart->hdmatx);
    /* Check the parameters */
    ASSERT_PARAM(data);
    ASSERT_PARAM(size);

    if (huart->state == HAL_STATE_READY)
    {
        /* Protect the process */
        TAKE_MUTEX(huart);

        /* Update the UART state */
        CLEAR_ERROR_CODE(huart);
        huart->state = HAL_STATE_BUSY_TX;

        /* Set TX runtime info */
        huart->tx.buffer    = data;
        huart->tx.xfer_size = size;

        SN_UART0_Type *instance = (SN_UART0_Type *)huart->instance;
        /* Reset TX FIFO (Because register are W, the all need to be reset each time)*/
        HAL_REG_WRITE(instance->FIFOCTRL, UART_FIFOCTRL_TXFIFORST | UART_FIFOCTRL_FIFOEN);

        do
        {
            /* Enable the UART transmit DMA stream */
            status = HAL_DMA_Start_IT(huart->hdmatx, (uint32_t)data, (uint32_t)&instance->TH, size);
            if (status != HAL_OK)
            {
                /* Update error state info */
                huart->state      = HAL_STATE_READY;
                huart->error_code = HAL_UART_ERROR_DMA;
                break;
            }

        } while (0);

        GIVE_MUTEX(huart);
    }
    else
    {
        return HAL_BUSY;
    }

    return status;
}

/**
 * @brief  Receives an amount of data in DMA mode.
 * @param  huart Pointer to a UART_Handle_T structure that contains
 *               the configuration information for the specified UART module.
 * @param  data Pointer to data buffer (u8 or u16 data elements).
 * @param  size  Amount of data elements (u8 or u16) to be received.
 * @retval HAL status
 */
HAL_Status_T HAL_UART_Receive_DMA(UART_Handle_T *huart, uint8_t *data, uint16_t size)
{
    HAL_Status_T status = HAL_OK;

    /* Check the UART handle */
    ASSERT_PARAM(huart);
    ASSERT_PARAM(huart->hdmarx);
    /* Check the parameters */
    ASSERT_PARAM(data);
    ASSERT_PARAM(size);

    if (huart->rx_state == HAL_STATE_READY)
    {
        TAKE_MUTEX(huart);

        /* Update the UART rx state */
        CLEAR_ERROR_CODE(huart);
        huart->rx_state = HAL_STATE_BUSY_RX;

        /* Set RX runtime info */
        huart->rx.buffer    = data;
        huart->rx.xfer_size = size;

        SN_UART0_Type *instance = (SN_UART0_Type *)huart->instance;
        /* Reset RX FIFO (Because register are W, the all need to be reset each time)*/
        HAL_REG_WRITE(instance->FIFOCTRL, UART_FIFOCTRL_RXFIFORST | UART_FIFOCTRL_FIFOEN);

        do
        {
            status = HAL_DMA_Start_IT(huart->hdmarx, (uint32_t)&instance->RB, (uint32_t)data, size);
            if (status != HAL_OK)
            {
                huart->state      = HAL_STATE_READY;
                huart->error_code = HAL_UART_ERROR_DMA;
                break;
            }

        } while (0);

        GIVE_MUTEX(huart);
    }
    else
    {
        return HAL_BUSY;
    }

    return status;
}

/**
 * @brief Pauses the DMA Transfer.
 * @param  huart  Pointer to a UART_Handle_T structure that contains
 *                the configuration information for the specified UART module.
 * @note Pause (Abort) Indicates the state transition.
 *       After this state occurs, you are advised to STOP or Resume
 * @retval HAL status
 */
HAL_Status_T HAL_UART_DMAPause(UART_Handle_T *huart)
{
    HAL_Status_T status = HAL_OK;

    /* Check the UART handle */
    ASSERT_PARAM(huart);

    TAKE_MUTEX(huart);

    if ((huart->hdmatx != NULL) && ((huart->state == HAL_STATE_BUSY_TX)))
    {
        HAL_FLAG_SET(huart->state_ext, EX_STATE_TX_ABORT);
        status = HAL_DMA_Abort_IT(huart->hdmatx);
        if (status != HAL_OK)
        {
            huart->state      = HAL_STATE_READY;
            huart->error_code = HAL_UART_ERROR_DMA;
        }
    }

    if ((huart->hdmarx != NULL) && (huart->rx_state == HAL_STATE_BUSY_RX))
    {
        HAL_FLAG_SET(huart->state_ext, EX_STATE_RX_ABORT);
        status = HAL_DMA_Abort_IT(huart->hdmarx);
        if (status != HAL_OK)
        {
            huart->rx_state   = HAL_STATE_READY;
            huart->error_code = HAL_UART_ERROR_DMA;
        }
    }

    GIVE_MUTEX(huart);

    return HAL_OK;
}

/**
 * @brief Resumes the DMA Transfer.
 * @param  huart  Pointer to a UART_Handle_T structure that contains
 *                the configuration information for the specified UART module.
 * @retval HAL status
 */
HAL_Status_T HAL_UART_DMAResume(UART_Handle_T *huart)
{
    HAL_Status_T status = HAL_OK;

    /* Check the UART handle */
    ASSERT_PARAM(huart);

    TAKE_MUTEX(huart);

    if ((huart->hdmatx != NULL) && (huart->state_ext & EX_STATE_TX_ABORT))
    {
        HAL_FLAG_CLR(huart->state_ext, EX_STATE_TX_ABORT);
        status = HAL_DMA_Resume_IT(huart->hdmatx);
        if (status != HAL_OK)
        {
            huart->state      = HAL_STATE_READY;
            huart->error_code = HAL_UART_ERROR_DMA;
        }
    }

    if ((huart->hdmarx != NULL) && (huart->state_ext & EX_STATE_RX_ABORT))
    {
        HAL_FLAG_CLR(huart->state_ext, EX_STATE_RX_ABORT);
        status = HAL_DMA_Resume_IT(huart->hdmarx);
        if (status != HAL_OK)
        {
            huart->rx_state   = HAL_STATE_READY;
            huart->error_code = HAL_UART_ERROR_DMA;
        }
    }

    GIVE_MUTEX(huart);

    return HAL_OK;
}
/**
 * @brief Stops the DMA Transfer.
 * @param  huart  Pointer to a UART_Handle_T structure that contains
 *                the configuration information for the specified UART module.
 * @retval HAL status
 */
HAL_Status_T HAL_UART_DMAStop(UART_Handle_T *huart)
{
    /* Check the UART handle */
    ASSERT_PARAM(huart);

    HAL_Status_T status = HAL_OK;
    TAKE_MUTEX(huart);

    if ((huart->hdmatx != NULL) && ((huart->state == HAL_STATE_BUSY_TX) || (huart->state_ext & EX_STATE_TX_ABORT)))
    {
        status = HAL_DMA_Abort_IT(huart->hdmatx);
        if (status != HAL_OK)
        {
            huart->state      = HAL_STATE_READY;
            huart->error_code = HAL_UART_ERROR_DMA;
        }
    }

    if ((huart->hdmarx != NULL) && ((huart->rx_state == HAL_STATE_BUSY_RX) || (huart->state_ext & EX_STATE_RX_ABORT)))
    {
        status = HAL_DMA_Abort_IT(huart->hdmarx);
        if (status != HAL_OK)
        {
            huart->rx_state   = HAL_STATE_READY;
            huart->error_code = HAL_UART_ERROR_DMA;
        }
    }

    huart->state_ext = EX_STATE_NONE;

    GIVE_MUTEX(huart);

    return HAL_OK;
}
#endif /* configUSE_UART_NONBLOCKING_DMA */

/*
================================================================================
            ##### Transfer Abort functions #####
================================================================================
*/
/**
 * @brief  Abort ongoing transfers and don't generate any event.
 * @param  huart Pointer to a UART_Handle_T structure that contains the configuration information for the specified UART module.
 * @retval HAL status
 */
ATTR_ISR HAL_Status_T HAL_UART_Abort(UART_Handle_T *huart)
{
    /* Check the UART handle */
    ASSERT_PARAM(huart);
    /* Check the state */
    RET_FLAG_FALSE(huart->state == HAL_STATE_BUSY_TX || huart->rx_state == HAL_STATE_BUSY_RX, HAL_ERROR);

    if (huart->state == HAL_STATE_BUSY_TX)
    {
        _uart_abort_tx(huart);
    }
    if (huart->rx_state == HAL_STATE_BUSY_RX)
    {
        _uart_abort_rx(huart);
    }

    return HAL_OK;
}

/**
 * @brief  Abort ongoing transmitting and don't generate any event.
 * @param  huart Pointer to a UART_Handle_T structure that contains the configuration information for the specified UART module.
 * @retval HAL status
 */
ATTR_ISR HAL_Status_T HAL_UART_AbortTransmit(UART_Handle_T *huart)
{
    /* Check the UART handle */
    ASSERT_PARAM(huart);
    /* Check the state */
    RET_FLAG_FALSE(huart->state == HAL_STATE_BUSY_TX, HAL_ERROR);

    _uart_abort_tx(huart);

    return HAL_OK;
}

/**
 * @brief  Abort ongoing receiving and don't generate any event.
 * @param  huart Pointer to a UART_Handle_T structure that contains the configuration information for the specified UART module.
 * @retval HAL status
 */
ATTR_ISR HAL_Status_T HAL_UART_AbortReceive(UART_Handle_T *huart)
{
    /* Check the UART handle */
    ASSERT_PARAM(huart);
    /* Check the state */
    RET_FLAG_FALSE(huart->rx_state == HAL_STATE_BUSY_RX, HAL_ERROR);

    _uart_abort_rx(huart);

    return HAL_OK;
}

/**
 * @brief  Abort ongoing transfers and generate a event.
 * @param  huart Pointer to a UART_Handle_T structure that contains the configuration information for the specified UART module.
 * @retval HAL status
 */
ATTR_ISR HAL_Status_T HAL_UART_Abort_IT(UART_Handle_T *huart)
{
    /* Check the UART handle */
    ASSERT_PARAM(huart);
    /* Check the state */
    RET_FLAG_FALSE(huart->state == HAL_STATE_BUSY_TX || huart->rx_state == HAL_STATE_BUSY_RX, HAL_ERROR);

    if (huart->state == HAL_STATE_BUSY_TX)
    {
        _uart_abort_tx(huart);
    }
    if (huart->rx_state == HAL_STATE_BUSY_RX)
    {
        _uart_abort_rx(huart);
    }

#if (configUSE_UART_REGISTER_CALLBACKS == 1)
    /* Call registered callback */
    if (huart->AbortCpltCallback != NULL)
    {
        huart->AbortCpltCallback(huart);
    }
#else
    /* Call legacy weak callback */
    HAL_UART_AbortCpltCallback(huart);
#endif /* configUSE_UART_REGISTER_CALLBACKS */

    return HAL_OK;
}

/**
 * @brief  Abort ongoing transmitting and generating a event.
 * @param  huart Pointer to a UART_Handle_T structure that contains the configuration information for the specified UART module.
 * @retval HAL status
 */
ATTR_ISR HAL_Status_T HAL_UART_AbortTransmit_IT(UART_Handle_T *huart)
{
    /* Check the UART handle */
    ASSERT_PARAM(huart);
    /* Check the state */
    RET_FLAG_FALSE(huart->state == HAL_STATE_BUSY_TX, HAL_ERROR);

    _uart_abort_tx(huart);

#if (configUSE_UART_REGISTER_CALLBACKS == 1)
    /* Call registered callback */
    if (huart->AbortTransmitCpltCallback != NULL)
    {
        huart->AbortTransmitCpltCallback(huart);
    }
#else
    /* Call legacy weak callback */
    HAL_UART_AbortTransmitCpltCallback(huart);
#endif /* configUSE_UART_REGISTER_CALLBACKS */

    return HAL_OK;
}

/**
 * @brief  Abort ongoing receiving and generating a event.
 * @param  huart Pointer to a UART_Handle_T structure that contains the configuration information for the specified UART module.
 * @retval HAL status
 */
ATTR_ISR HAL_Status_T HAL_UART_AbortReceive_IT(UART_Handle_T *huart)
{
    /* Check the UART handle */
    ASSERT_PARAM(huart);
    /* Check the state */
    RET_FLAG_FALSE(huart->rx_state == HAL_STATE_BUSY_RX, HAL_ERROR);

    _uart_abort_rx(huart);

#if (configUSE_UART_REGISTER_CALLBACKS == 1)
    /* Call registered callback */
    if (huart->AbortReceiveCpltCallback != NULL)
    {
        huart->AbortReceiveCpltCallback(huart);
    }
#else
    /* Call legacy weak callback */
    HAL_UART_AbortReceiveCpltCallback(huart);
#endif /* configUSE_UART_REGISTER_CALLBACKS */

    return HAL_OK;
}

/*
================================================================================
            ##### IRQHandler and Event Callback functions #####
================================================================================
*/
#if (configUSE_UART_NONBLOCKING_IT == 1)
/**
 * @brief  UART Interrupt Handle.
 * @param  huart Pointer to a UART_Handle_T structure that contains the configuration information for the specified UART module.
 * @retval None
 */
__weak ATTR_ISR void HAL_UART_IRQHandler(UART_Handle_T *huart)
{
    SN_UART0_Type *instance = (SN_UART0_Type *)huart->instance;

    static uint32_t interrupt_id = 0;
    /* Get interrupt identification */
    uint32_t ii = HAL_REG_READ(instance->II);

    /* INTSTATUS：At least one interrupt is pending */
    while (HAL_FLAG_LACKS(ii, UART_II_INTSTATUS_MASK))
    {
        /* Get interrupt id */
        interrupt_id = (ii & UART_II_INTID_MASK) >> UART_II_INTID_POS;
        switch (interrupt_id)
        {
            case UART_II_INTID_RLS:
            {
                _uart_rls_handle(huart);
            }
            break;

            case UART_II_INTID_CTI:
            case UART_II_INTID_RDA:
            {
                _uart_rda_handle(huart);
            }
            break;

            case UART_II_INTID_THRE:
            {
                _uart_thre_handle(huart);
            }
            break;

            case UART_II_INTID_MS:
            {
                _uart_ms_handle(huart);
            }
            break;

            default:
                break;
        }

        /* Polling the interrupt identification */
        ii = HAL_REG_READ(instance->II);
    }
}
#endif /* configUSE_UART_NONBLOCKING_IT */

/**
 * @brief  UART Tx Transfer Completed Callback.
 * @param  huart Pointer to a UART_Handle_T structure that contains the configuration information for the specified UART module.
 * @retval None
 */
__weak ATTR_ISR void HAL_UART_TxCpltCallback(UART_Handle_T *huart)
{
    /* Prevent unused argument(s) compilation warning */
    UNUSED(huart);
    /* NOTE: This function should not be modified, when the callback is needed, the HAL_UART_TxCpltCallback could be implemented in the user file */
}

/**
 * @brief  UART Rx Transfer Completed Callback.
 * @param  huart Pointer to a UART_Handle_T structure that contains the configuration information for the specified UART module.
 * @retval None
 */
__weak ATTR_ISR void HAL_UART_RxCpltCallback(UART_Handle_T *huart)
{
    /* Prevent unused argument(s) compilation warning */
    UNUSED(huart);
    /* NOTE: This function should not be modified, when the callback is needed, the HAL_UART_RxCpltCallback could be implemented in the user file */
}

/**
 * @brief  UART Abort Completed Callback.
 * @param  huart Pointer to a UART_Handle_T structure that contains the configuration information for the specified UART module.
 * @retval None
 */
__weak ATTR_ISR void HAL_UART_AbortCpltCallback(UART_Handle_T *huart)
{
    /* Prevent unused argument(s) compilation warning */
    UNUSED(huart);
    /* NOTE: This function should not be modified, when the callback is needed, the HAL_UART_AbortCpltCallback could be implemented in the user file */
}

/**
 * @brief  UART Abort Transmit Completed Callback.
 * @param  huart Pointer to a UART_Handle_T structure that contains the configuration information for the specified UART module.
 * @retval None
 */
__weak ATTR_ISR void HAL_UART_AbortTransmitCpltCallback(UART_Handle_T *huart)
{
    /* Prevent unused argument(s) compilation warning */
    UNUSED(huart);
    /* NOTE: This function should not be modified, when the callback is needed, the HAL_UART_AbortTransmitCpltCallback could be implemented in the user file */
}

/**
 * @brief  UART Abort Receive Completed Callback.
 * @param  huart Pointer to a UART_Handle_T structure that contains the configuration information for the specified UART module.
 * @retval None
 */
__weak ATTR_ISR void HAL_UART_AbortReceiveCpltCallback(UART_Handle_T *huart)
{
    /* Prevent unused argument(s) compilation warning */
    UNUSED(huart);
    /* NOTE: This function should not be modified, when the callback is needed, the HAL_UART_AbortReceiveCpltCallback could be implemented in the user file */
}

/**
 * @brief  UART Error Callback.
 * @param  huart Pointer to a UART_Handle_T structure that contains the configuration information for the specified UART module.
 * @retval None
 */
__weak ATTR_ISR void HAL_UART_ErrorCallback(UART_Handle_T *huart)
{
    /* Prevent unused argument(s) compilation warning */
    UNUSED(huart);
    /* NOTE: This function should not be modified, when the callback is needed, the HAL_UART_ErrorCallback could be implemented in the user file */
}

/* Private functions ---------------------------------------------------------*/
/**
 * @brief  Initialize the callbacks to their default values.
 * @param  huart Pointer to a UART_Handle_T structure that contains the configuration information for the specified UART module.
 * @retval None
 */
#if (configUSE_UART_REGISTER_CALLBACKS == 1)
static void _uart_init_callbacks_default(UART_Handle_T *huart)
{
    /* Init the UART Callback settings */
    huart->TxCpltCallback            = HAL_UART_TxCpltCallback;
    huart->RxCpltCallback            = HAL_UART_RxCpltCallback;
    huart->ErrorCallback             = HAL_UART_ErrorCallback;
    huart->AbortCpltCallback         = HAL_UART_AbortCpltCallback;
    huart->AbortTransmitCpltCallback = HAL_UART_AbortTransmitCpltCallback;
    huart->AbortReceiveCpltCallback  = HAL_UART_AbortReceiveCpltCallback;
}
#endif /* configUSE_UART_REGISTER_CALLBACKS */

#if (configUSE_UART_NONBLOCKING_IT == 1)
/**
 * @brief  Select the value that the RXTL register should be set to.
 * @param  huart Pointer to a UART_Handle_T structure that contains the configuration information for the specified UART module.
 * @retval RXTL value
 */
static uint8_t _uart_get_rxtl(UART_Handle_T *huart)
{
    /*if rx_size < 4, RXTL = 0(Trigger level of RX FIFO is 1 character) */
    if (huart->rx.xfer_size < 4)
        return 0;
    /*if 4 <= rx_size < 8, RXTL = 1(Trigger level of RX FIFO is 4 character) */
    else if ((huart->rx.xfer_size < 8) && (huart->rx.xfer_size >= 4))
        return 1;
    /*RXTL = 2(Trigger level of RX FIFO is 8 character) */
    else
        return 2;
}
#endif /* configUSE_UART_NONBLOCKING_IT */

/**
 * @brief  Configures the UART peripheral.
 * @param  huart Pointer to a UART_Handle_T structure that contains the configuration information for the specified UART module.
 * @retval HAL status
 */
static HAL_Status_T _uart_set_config(UART_Handle_T *huart)
{
    SN_UART0_Type *instance = (SN_UART0_Type *)huart->instance;
    uint32_t       lc_val   = 0;

    lc_val |= UART_LC_WLS(huart->init.word_length);
    lc_val |= UART_LC_SBS(huart->init.stop_bits);

    /* CTS or RTS only support UART0/1/2/3 */
    if ((instance == SN_UART0) || (instance == SN_UART1) || (instance == SN_UART2) || (instance == SN_UART3))
    {
        if (huart->init.hw_flow_ctl != UART_HWCONTROL_NONE)
        {
            if (huart->init.hw_flow_ctl == UART_HWCONTROL_RTS)
            {
                HAL_REG_SBIT(instance->IE, UART_IE_RTSEN);
            }
            else if (huart->init.hw_flow_ctl == UART_HWCONTROL_CTS)
            {
                HAL_REG_SBIT(instance->IE, UART_IE_CTSEN);
            }
            else if (huart->init.hw_flow_ctl == UART_HWCONTROL_RTS_CTS)
            {
                HAL_REG_SBIT(instance->IE, UART_IE_CTSEN);
                HAL_REG_SBIT(instance->IE, UART_IE_RTSEN);
            }
        }
        else
        {
            /* UART_HWCONTROL_NONE */
            HAL_REG_CBIT(instance->IE, UART_IE_CTSEN);
            HAL_REG_CBIT(instance->IE, UART_IE_RTSEN);
        }
    }

    if (huart->init.parity == UART_PARITY_NONE)
    {
        lc_val &= ~(UART_LC_PE);
    }
    else
    {
        lc_val |= UART_LC_PE;
        if (huart->init.parity == UART_PARITY_EVEN)
        {
            lc_val |= UART_LC_PS(1);
        }
        else if (huart->init.parity == UART_PARITY_ODD)
        {
            lc_val |= UART_LC_PS(0);
        }
    }

    /* Writes configuration values to LC registers */
    HAL_REG_WRITE(instance->LC, lc_val);

    RET_FLAG_FALSE(_uart_set_baudrate(huart, huart->init.baud_rate) == HAL_OK, HAL_ERROR);

    return HAL_OK;
}

/**
 * @brief Calculate the baudrate to setup DLM, DLL, PRE
 * @param huart Pointer to a UART_Handle_T structure that contains the configuration information for the specified UART module.
 * @param baud_rate The baudrate to setup
 * @retval HAL status
 */
static HAL_Status_T _uart_set_baudrate(UART_Handle_T *huart, uint32_t baud_rate)
{
    SN_UART0_Type *instance = (SN_UART0_Type *)huart->instance;

    uint32_t io_irda_uclk = 0;
    uint32_t uart_index   = 0;

    switch ((uint32_t)instance)
    {
        case SN_UART0_BASE:
        {
            uart_index = 0;
        }
        break;
        case SN_UART1_BASE:
        {
            uart_index = 1;
        }
        break;
        case SN_UART2_BASE:
        {
            uart_index = 2;
        }
        break;
        case SN_UART3_BASE:
        {
            uart_index = 3;
        }
        break;
        case SN_UART4_BASE:
        {
            uart_index = 4;
        }
        break;
        case SN_UART5_BASE:
        {
            uart_index = 5;
        }
        break;
    }

    uint32_t clk_sel = __HAL_RCC_GET_UARTn_CLKSEL(uart_index);
    if (clk_sel == RCC_UART_CLKSEL_PLLCLK)
    {
        io_irda_uclk = HAL_RCC_GetPLLCLKFreq();
    }
    else if (clk_sel == RCC_UART_CLKSEL_HCLK)
    {
        io_irda_uclk = HAL_RCC_GetHCLKFreq();
    }

    if (baud_rate > (io_irda_uclk >> 4))
    {
        return HAL_ERROR;
    }

    uint32_t temp = baud_rate * 16;

    /* 1. DL : Round the calculation */
    uint32_t DL_10 = ((io_irda_uclk * 10) / temp);
    uint32_t DL    = DL_10 / 10;
    if (DL_10 % 10 >= 5)
    {
        DL++;
    }

    /* Baud rate check (calculation error value:  Deviation < |3.125%|) */
    uint32_t rel_baud_rate = io_irda_uclk / (DL * 16);
    if (rel_baud_rate > baud_rate)
    {
        if (((rel_baud_rate - baud_rate) * 1000 / baud_rate) >= 31)
            return HAL_ERROR;
    }
    else if (rel_baud_rate < baud_rate)
    {
        if (((baud_rate - rel_baud_rate) * 1000 / baud_rate) >= 31)
            return HAL_ERROR;
    }

    /* check the inequation "56 * F_PCLK > F_B16XCLK" */
    uint32_t f_b16xclk = io_irda_uclk / (DL * 1); // PRE = 1
    uint32_t f_pclk    = 1;
    if (huart->instance == SN_UART0 || huart->instance == SN_UART5)
    {
        f_pclk = HAL_RCC_GetAPB1CLKFreq();
    }
    else
    {
        f_pclk = HAL_RCC_GetAPB0CLKFreq();
    }
    if (56 * f_pclk <= f_b16xclk)
    {
        return HAL_ERROR;
    }

    /* Enable divisor latch */
    HAL_REG_WRITE(instance->LC_b.DLAB, 1);
    /* PRE as the system clock pre frequency division; The default is 1. */
    HAL_REG_WRITE(instance->PRE, 1);
    HAL_REG_WRITE(instance->DLM, (DL >> 8) & 0xFFU);
    HAL_REG_WRITE(instance->DLL, DL & 0xFFU);
    /* Disable divisor latch */
    HAL_REG_WRITE(instance->LC_b.DLAB, 0);

    return HAL_OK;
}

#if (configUSE_UART_NONBLOCKING_IT == 1)
ATTR_ISR static void _uart_rls_handle(UART_Handle_T *huart)
{
    SN_UART0_Type *instance = (SN_UART0_Type *)huart->instance;

    /* Get line status */
    uint32_t ls = HAL_REG_READ(instance->LS);
    if (HAL_FLAG_HAS(ls, UART_IS_OE))
    {
        HAL_FLAG_SET(huart->error_code, HAL_UART_ERROR_OE);
    }
    else if (HAL_FLAG_HAS(ls, UART_IS_PE))
    {
        HAL_FLAG_SET(huart->error_code, HAL_UART_ERROR_PE);
    }
    else if (HAL_FLAG_HAS(ls, UART_IS_FE))
    {
        HAL_FLAG_SET(huart->error_code, HAL_UART_ERROR_FE);
    }
    else if (HAL_FLAG_HAS(ls, UART_IS_BI))
    {
        HAL_FLAG_SET(huart->error_code, HAL_UART_ERROR_BI);
    }

    /* Get the received data out from RX FIFO */
    uint8_t data = HAL_REG_READ(instance->RB_b.RB);
    UNUSED(data);

#if (configUSE_UART_REGISTER_CALLBACKS == 1)
    /* Call registered callback */
    if (huart->ErrorCallback != NULL)
    {
        huart->ErrorCallback(huart);
    }
#else
    /* Call legacy weak callback */
    HAL_UART_ErrorCallback(huart);
#endif /* configUSE_UART_REGISTER_CALLBACKS */
}

/**
 * @brief Read UARTn_RB register or UART FIFO drops below trigger level
 *
 * @param huart Pointer to a UART_Handle_T structure that contains the configuration information for the specified UART module.
 * @return ATTR_ISR
 */
ATTR_ISR static void _uart_rda_handle(UART_Handle_T *huart)
{
    SN_UART0_Type *instance = (SN_UART0_Type *)huart->instance;
    uint8_t        cnt      = 0;

    /* Check RX count */
    while (huart->rx.xfer_count > 0U)
    {
        /* Receive data */
        *(huart->rx.buffer++) = HAL_REG_READ(instance->RB_b.RB);
        huart->rx.xfer_count--;
        cnt++;

        /* Check RDR (receiver data ready) flag */
        if (HAL_REG_READ(instance->LS_b.RDR) == 0 || cnt == UART_FIFO_DEPTH_USAGE_RX)
        {
            break;
        }
    }

    /* Check whether all data has been received */
    if (huart->rx.xfer_count == 0U)
    {
        _uart_receive_complete(huart);
    }
}

/**
 * @brief Read UARTn_II register (if source of interrupt) or Write THR register
 *
 * @param huart Pointer to a UART_Handle_T structure that contains the configuration information for the specified UART module.
 * @return ATTR_ISR
 */
ATTR_ISR static void _uart_thre_handle(UART_Handle_T *huart)
{
    SN_UART0_Type *instance = (SN_UART0_Type *)huart->instance;
    uint8_t        cnt      = 0;

    /* Check TX count */
    while (huart->tx.xfer_count > 0U)
    {
        /* Send data */
        HAL_REG_WRITE(instance->TH, *(huart->tx.buffer++));
        huart->tx.xfer_count--;
        cnt++;

        /* Check TX FIFO full flag */
        if (cnt == UART_FIFO_DEPTH_USAGE_TX)
        {
            break;
        }
    }

    /* Check whether all data has been sent */
    if (huart->tx.xfer_count == 0U)
    {
        _uart_transmit_complete(huart);
    }
}

/**
 * @brief MSR Read
 *
 * @param huart Pointer to a UART_Handle_T structure that contains the configuration information for the specified UART module.
 * @return ATTR_ISR
 */
ATTR_ISR static void _uart_ms_handle(UART_Handle_T *huart)
{
    SN_UART0_Type *instance = (SN_UART0_Type *)huart->instance;

    if (HAL_REG_READ(instance->MS_b.DCTS))
    {
        // Low to High transition
    }
    else
    {
        // High to Low transition
    }
}

/**
 * @brief UART interrupts the transmission completion process.
 *
 * @param huart Pointer to a UART_Handle_T structure that contains the configuration information for the specified UART module.
 * @return ATTR_ISR
 */
ATTR_ISR static void _uart_transmit_complete(UART_Handle_T *huart)
{
    SN_UART0_Type *instance = (SN_UART0_Type *)huart->instance;

    /* Disable THRE */
    HAL_REG_CBIT(instance->IE, UART_IE_THREIE);

    /* Update the TX state */
    huart->state = HAL_STATE_READY;

#if (configUSE_UART_REGISTER_CALLBACKS == 1)
    /* Call registered callback */
    if (huart->TxCpltCallback != NULL)
    {
        huart->TxCpltCallback(huart);
    }
#else
    /* Call legacy weak callback */
    HAL_UART_TxCpltCallback(huart);
#endif /* configUSE_UART_REGISTER_CALLBACKS */
}

/**
 * @brief The UART interrupt receives completion processing.
 *
 * @param huart Pointer to a UART_Handle_T structure that contains the configuration information for the specified UART module.
 * @return ATTR_ISR
 */
ATTR_ISR static void _uart_receive_complete(UART_Handle_T *huart)
{
    SN_UART0_Type *instance = (SN_UART0_Type *)huart->instance;

    /* Disable RDA and RLS */
    HAL_REG_CBIT(instance->IE, UART_IE_RDAIE);
    HAL_REG_CBIT(instance->IE, UART_IE_RLSIE);

    /* Update the RX state */
    huart->rx_state = HAL_STATE_READY;

#if (configUSE_UART_REGISTER_CALLBACKS == 1)
    /* Call registered callback */
    if (huart->RxCpltCallback != NULL)
    {
        huart->RxCpltCallback(huart);
    }
#else
    /* Call legacy weak callback */
    HAL_UART_RxCpltCallback(huart);
#endif /* configUSE_UART_REGISTER_CALLBACKS */
}

#endif /* configUSE_UART_NONBLOCKING_IT */

/**
 * @brief UART terminates TX transmission
 *
 * @param huart Pointer to a UART_Handle_T structure that contains the configuration information for the specified UART module.
 * @return ATTR_ISR
 */
ATTR_ISR static void _uart_abort_tx(UART_Handle_T *huart)
{
    SN_UART0_Type *instance = (SN_UART0_Type *)huart->instance;

    /* Set TX abort flag */
    huart->tx.abort = 1;

    /* Check transmission mode by IE */
    if (HAL_REG_READ(instance->IE_b.THREIE) == HAL_DISABLE)
    {
        /* Wait for transmission to end (blocking mode) */
        while (huart->state == HAL_STATE_BUSY_TX)
        {
            __NOP();
        }
    }
    else
    {
        /* Disable TX interrupt (IT mode) */
        HAL_REG_CBIT(instance->IE, UART_IE_THREIE);
        /* Wait for TEMT */
        while (HAL_REG_READ(instance->LS_b.TEMT) == 0)
        {
            __NOP();
        }
        /* Update the UART state */
        huart->state = HAL_STATE_READY;
    }
}

/**
 * @brief UART terminates the RX transmission
 *
 * @param huart Pointer to a UART_Handle_T structure that contains the configuration information for the specified UART module.
 * @return ATTR_ISR
 */
ATTR_ISR static void _uart_abort_rx(UART_Handle_T *huart)
{
    SN_UART0_Type *instance = (SN_UART0_Type *)huart->instance;

    /* Set RX abort flag */
    huart->rx.abort = 1;

    /* Check receiving mode by IE */
    if (HAL_REG_READ(instance->IE_b.RDAIE) == HAL_DISABLE)
    {
        /* Wait for receiving to end (blocking mode) */
        while (huart->rx_state == HAL_STATE_BUSY_RX)
        {
            __NOP();
        }
    }
    else
    {
        /* Disable RX interrupt (IT mode) */
        HAL_REG_CBIT(instance->IE, UART_IE_RDAIE);
        HAL_REG_CBIT(instance->IE, UART_IE_RLSIE);

        /* Update the RX state */
        huart->rx_state = HAL_STATE_READY;
    }
}

#if (configUSE_UART_NONBLOCKING_DMA == 1)
/**
 * @brief UART checks whether the parameters of the UART and the parameters of the DMA are supported when using DMA transfers
 *
 * @param huart Pointer to a UART_Handle_T structure that contains the configuration information for the specified UART module.
 * @return HAL_Status_T
 */
static HAL_Status_T _uart_dma_tx_check(UART_Handle_T *huart)
{
    if ((huart->hdmatx->init.src_width != DMA_XFR_WIDTH_8BITS) || (huart->hdmatx->init.dst_width != DMA_XFR_WIDTH_8BITS))
    {
        return HAL_ERROR;
    }

    if (huart->hdmatx->init.src_burst != DMA_SRC_BURST_1)
    {
        return HAL_ERROR;
    }

    return HAL_OK;
}

/**
 * @brief UART checks whether the parameters of the UART and the parameters of the DMA are supported when using DMA transfers
 *
 * @param huart Pointer to a UART_Handle_T structure that contains the configuration information for the specified UART module.
 * @return HAL_Status_T
 */
static HAL_Status_T _uart_dma_rx_check(UART_Handle_T *huart)
{
    if ((huart->hdmarx->init.src_width != DMA_XFR_WIDTH_8BITS) || (huart->hdmarx->init.dst_width != DMA_XFR_WIDTH_8BITS))
    {
        return HAL_ERROR;
    }

    if (huart->hdmarx->init.src_burst != DMA_SRC_BURST_1)
    {
        return HAL_ERROR;
    }

    return HAL_OK;
}

/**
 * @brief  DMA UART transmit process complete callback.
 * @param  hdma  Pointer to a DMA_HandleTypeDef structure that contains
 *               the configuration information for the specified DMA module.
 * @retval None
 */
static void _uart_dma_tx_cplt_callback(DMA_Handle_T *hdma)
{
    UART_Handle_T *huart = (UART_Handle_T *)hdma->parent;

    /* Update the TX state */
    huart->state = HAL_STATE_READY;

    /* Call user TX complete callback */
#if (configUSE_UART_REGISTER_CALLBACKS == 1)
    /*Call registered Tx complete callback*/
    huart->TxCpltCallback(huart);
#else
    /*Call legacy weak Tx complete callback*/
    HAL_UART_TxCpltCallback(huart);
#endif /* configUSE_UART_REGISTER_CALLBACKS */
}

/**
 * @brief  DMA UART communication error callback.
 * @param  hdma  Pointer to a DMA_HandleTypeDef structure that contains
 *               the configuration information for the specified DMA module.
 * @retval None
 */
static void _uart_dma_tx_err_callback(DMA_Handle_T *hdma)
{
    UART_Handle_T *huart = (UART_Handle_T *)hdma->parent;

    /* Update the TX state */
    huart->state = HAL_STATE_READY;
    huart->error_code |= HAL_UART_ERROR_DMA;

#if (configUSE_UART_REGISTER_CALLBACKS == 1)
    /*Call registered error callback*/
    huart->ErrorCallback(huart);
#else
    /*Call legacy weak error callback*/
    HAL_UART_ErrorCallback(huart);
#endif /* configUSE_UART_REGISTER_CALLBACKS */
}

/**
 * @brief  DMA UART Tx communication abort callback, when initiated by user
 *         (To be called at end of DMA Tx Abort procedure following user abort request).
 * @param  hdma  Pointer to a DMA_HandleTypeDef structure that contains
 *               the configuration information for the specified DMA module.
 * @retval None
 */
static void _uart_dma_tx_abort_callback(DMA_Handle_T *hdma)
{
    UART_Handle_T *huart = (UART_Handle_T *)hdma->parent;

    /* Update the TX state */
    huart->state = HAL_STATE_READY;

    /* Call user Abort complete callback */
#if (configUSE_UART_REGISTER_CALLBACKS == 1)
    /* Call registered Abort complete callback */
    huart->AbortCpltCallback(huart);
#else
    /* Call legacy weak Abort complete callback */
    HAL_UART_AbortCpltCallback(huart);
#endif /* configUSE_UART_REGISTER_CALLBACKS */
}

/**
 * @brief  DMA UART receive process complete callback.
 * @param  hdma  Pointer to a DMA_HandleTypeDef structure that contains
 *               the configuration information for the specified DMA module.
 * @retval None
 */
static void _uart_dma_rx_cplt_callback(DMA_Handle_T *hdma)
{
    UART_Handle_T *huart = (UART_Handle_T *)hdma->parent;

    /* Update the RX state */
    huart->rx_state = HAL_STATE_READY;

    /* In other cases : use Rx Complete callback */
#if (configUSE_UART_REGISTER_CALLBACKS == 1)
    /*Call registered Rx complete callback*/
    huart->RxCpltCallback(huart);
#else
    /*Call legacy weak Rx complete callback*/
    HAL_UART_RxCpltCallback(huart);
#endif /* configUSE_UART_REGISTER_CALLBACKS */
}

/**
 * @brief  DMA UART communication error callback.
 * @param  hdma  Pointer to a DMA_HandleTypeDef structure that contains
 *               the configuration information for the specified DMA module.
 * @retval None
 */
static void _uart_dma_rx_err_callback(DMA_Handle_T *hdma)
{
    UART_Handle_T *huart = (UART_Handle_T *)hdma->parent;

    /* Update the RX state */
    huart->rx_state = HAL_STATE_READY;
    huart->error_code |= HAL_UART_ERROR_DMA;

#if (configUSE_UART_REGISTER_CALLBACKS == 1)
    /*Call registered error callback*/
    huart->ErrorCallback(huart);
#else
    /*Call legacy weak error callback*/
    HAL_UART_ErrorCallback(huart);
#endif /* configUSE_UART_REGISTER_CALLBACKS */
}

/**
 * @brief  DMA UART Rx communication abort callback, when initiated by user
 *         (To be called at end of DMA Rx Abort procedure following user abort request).
 * @param  hdma  Pointer to a DMA_HandleTypeDef structure that contains
 *               the configuration information for the specified DMA module.
 * @retval None
 */
static void _uart_dma_rx_abort_callback(DMA_Handle_T *hdma)
{
    UART_Handle_T *huart = (UART_Handle_T *)hdma->parent;

    /* Update the RX state */
    huart->rx_state = HAL_STATE_READY;

    /* Call user Abort complete callback */
#if (configUSE_UART_REGISTER_CALLBACKS == 1)
    /* Call registered Abort complete callback */
    huart->AbortCpltCallback(huart);
#else
    /* Call legacy weak Abort complete callback */
    HAL_UART_AbortCpltCallback(huart);
#endif /* configUSE_UART_REGISTER_CALLBACKS */
}
#endif /* configUSE_UART_NONBLOCKING_DMA */

#endif /* configUSE_UART_PERIPHERAL */
