/***********************
 * SN34F78X SDK V2.1.4 *
 ***********************/

#include "sn34f78x_hal_i2c.h"

/* Private constants ---------------------------------------------------------*/
#if (configUSE_I2C_PERIPHERAL == 1)
#if IT_OR_BURST
const static I2C_IRQ_Handler IRQ_Handler[3][4] = {
    [HAL_I2C_MODE_NONE] = {
        NULL, NULL, NULL, NULL},

    [HAL_I2C_MODE_MASTER] = {
#if configUSE_I2C_NONBLOCKING_IT
        _i2c_master_tx_it_handler,
        _i2c_master_rx_it_handler,
#else  /* configUSE_I2C_NONBLOCKING_IT */
        NULL,
        NULL,
#endif /* configUSE_I2C_NONBLOCKING_IT */

#if configUSE_I2C_NONBLOCKING_BURST
        _i2c_master_tx_burst_handler,
        _i2c_master_rx_burst_handler,
#else  /* configUSE_I2C_NONBLOCKING_BURST */
        NULL,
        NULL,
#endif /* configUSE_I2C_NONBLOCKING_BURST */
    },

    [HAL_I2C_MODE_SLAVE] = {
#if configUSE_I2C_NONBLOCKING_IT
        _i2c_slave_tx_it_handler,
        _i2c_slave_rx_it_handler,
#else  /* configUSE_I2C_NONBLOCKING_IT */
        NULL,
        NULL,
#endif /* configUSE_I2C_NONBLOCKING_IT */
        NULL,
        NULL,
    }};
#endif

/* Exported functions --------------------------------------------------------*/
/*
================================================================================
            ##### Initialization/De-initialization functions #####
================================================================================
*/
/**
 * @brief  Initializes the I2C according to the specified parameters in the I2C_InitTypeDef and initialize the associated handle.
 * @param  hi2c Pointer to a I2C_Handle_T structure that contains the configuration information for the specified I2C.
 * @retval HAL status
 */
HAL_Status_T HAL_I2C_Init(I2C_Handle_T *hi2c)
{
    /* Check the I2C handle */
    ASSERT_PARAM(hi2c);
    /* Check the I2C instance */
    ASSERT_PARAM(IS_PERIPHERAL_I2C_INSTANCE(hi2c));

    /* Check the parameters */
    ASSERT_PARAM(IS_PERIPHERAL_I2C_CLOCK_SPEED(hi2c->init.clock_speed));
    ASSERT_PARAM(IS_PERIPHERAL_I2C_ADDRESS(hi2c->init.own_address));
    ASSERT_PARAM(IS_PERIPHERAL_I2C_ADDRESSING_MODE(hi2c->init.addressing_mode));
    ASSERT_PARAM(IS_PERIPHERAL_I2C_GENERAL_CALL_EN(hi2c->init.gc_en));
    RET_FLAG_TRUE(((hi2c->init.addressing_mode == I2C_ADDRESSINGMODE_10BIT) && hi2c->init.gc_en), HAL_ERROR);

    if (hi2c->state == HAL_STATE_RESET)
    {
        /* Release the process protection */
        GIVE_MUTEX(hi2c);

#if (configUSE_I2C_REGISTER_CALLBACKS == 1)
        _i2c_init_callbacks_to_default(hi2c);
        if (hi2c->MspInitCallback == NULL)
        {
            hi2c->MspInitCallback = HAL_I2C_MspInit;
        }
        /* Init the low level hardware */
        hi2c->MspInitCallback(hi2c);
#else
        /* Init the low level hardware */
        HAL_I2C_MspInit(hi2c);
#endif /* configUSE_I2C_REGISTER_CALLBACKS */

        /* Set the state to busy */
        hi2c->state = HAL_STATE_BUSY;

        /* Config I2C */
        hi2c->own_address = hi2c->init.own_address;
        HAL_REG_SBIT(hi2c->instance->CTRL, I2C_CTRL_MASK_RESET);
        HAL_REG_WRITE(hi2c->instance->ADDR_b.ADDR10EN, hi2c->init.addressing_mode);
        HAL_REG_WRITE(hi2c->instance->CTRL_b.GCEN, hi2c->init.gc_en);
        RET_FLAG_TRUE(_i2c_set_clock_speed(hi2c) != HAL_OK, HAL_ERROR);

        /* Update the I2C state */
        hi2c->state       = HAL_STATE_READY;
        hi2c->mode        = HAL_I2C_MODE_NONE;
        hi2c->xfer_option = I2C_NO_OPTION_FRAME;
        CLEAR_ERROR_CODE(hi2c);

        return HAL_OK;
    }
    else
    {
        hi2c->error_code = HAL_ERROR_REINIT;
        return HAL_ERROR;
    }
}

/**
 * @brief  DeInitialize the I2C peripheral.
 * @param  hi2c Pointer to a I2C_Handle_T structure that contains the configuration information for the specified I2C.
 * @retval HAL status
 */
HAL_Status_T HAL_I2C_DeInit(I2C_Handle_T *hi2c)
{
    /* Check the I2C handle */
    ASSERT_PARAM(hi2c);
    /* Check the I2C instance */
    ASSERT_PARAM(IS_PERIPHERAL_I2C_INSTANCE(hi2c));

    /* Set the state to busy */
    hi2c->state = HAL_STATE_BUSY;

    /* Reset the peripheral */
    HAL_REG_SBIT(hi2c->instance->CTRL, I2C_CTRL_MASK_RESET);

#if (configUSE_I2C_REGISTER_CALLBACKS == 1)
    if (hi2c->MspDeInitCallback == NULL)
    {
        hi2c->MspDeInitCallback = HAL_I2C_MspDeInit;
    }
    /* DeInit the low level hardware */
    hi2c->MspDeInitCallback(hi2c);
#else
    /* DeInit the low level hardware */
    HAL_I2C_MspDeInit(hi2c);
#endif /* configUSE_I2C_REGISTER_CALLBACKS */

    /* Update the I2C state */
    hi2c->state       = HAL_STATE_RESET;
    hi2c->mode        = HAL_I2C_MODE_NONE;
    hi2c->xfer_option = I2C_NO_OPTION_FRAME;
    CLEAR_ERROR_CODE(hi2c);

    /* Release the process protection */
    GIVE_MUTEX(hi2c);

    return HAL_OK;
}

/**
 * @brief  I2C MSP Init.
 * @param  hi2c Pointer to a I2C_Handle_T structure that contains the configuration information for the specified I2C.
 * @retval None
 */
__weak void HAL_I2C_MspInit(I2C_Handle_T *hi2c)
{
    /* Prevent unused argument(s) compilation warning */
    UNUSED(hi2c);
    /* NOTE: This function should not be modified, when the callback is needed, the HAL_I2C_MspInit could be implemented in the user file */
}

/**
 * @brief  I2C MSP DeInit.
 * @param  hi2c Pointer to a I2C_Handle_T structure that contains the configuration information for the specified I2C.
 * @retval None
 */
__weak void HAL_I2C_MspDeInit(I2C_Handle_T *hi2c)
{
    /* Prevent unused argument(s) compilation warning */
    UNUSED(hi2c);
    /* NOTE: This function should not be modified, when the callback is needed, the HAL_I2C_MspDeInit could be implemented in the user file */
}

/*
================================================================================
            ##### Callbacks Register/UnRegister functions #####
================================================================================
*/
#if (configUSE_I2C_REGISTER_CALLBACKS == 1)
/**
 * @brief  Register a User I2C Callback To be used instead of the weak predefined callback
 * @param  hi2c Pointer to a I2C_Handle_T structure that contains the configuration information for the specified I2C module.
 * @param  callback_id ID of the callback to be registered
 *         This parameter can be one of the following values:
 *           @arg @ref HAL_I2C_MASTER_TX_COMPLETE_CB_ID I2C Master Tx Complete Callback ID
 *           @arg @ref HAL_I2C_MASTER_RX_COMPLETE_CB_ID I2C Master Rx Complete Callback ID
 *           @arg @ref HAL_I2C_SLAVE_TX_COMPLETE_CB_ID I2C Slave Tx Complete Callback ID
 *           @arg @ref HAL_I2C_SLAVE_RX_COMPLETE_CB_ID I2C Slave Rx Complete Callback ID
 *           @arg @ref HAL_I2C_ABORT_CB_ID I2C Abort Callback ID
 *           @arg @ref HAL_I2C_ERROR_CB_ID I2C Error Callback ID
 *           @arg @ref HAL_I2C_MSPINIT_CB_ID MspInit Callback ID
 *           @arg @ref HAL_I2C_MSPDEINIT_CB_ID MspDeInit Callback ID
 * @param  callback pointer to the Callback function
 * @retval HAL status
 */
HAL_Status_T HAL_I2C_RegisterCallback(I2C_Handle_T *hi2c, HAL_I2C_CallbackID_T callback_id, I2C_Callback_T callback)
{
    HAL_Status_T status = HAL_OK;

    /* Check the I2C handle */
    ASSERT_PARAM(hi2c);
    /* Check the I2C instance */
    ASSERT_PARAM(IS_PERIPHERAL_I2C_INSTANCE(hi2c));
    /* Check the parameters */
    ASSERT_PARAM(callback);

    /* Protect the process */
    TAKE_MUTEX(hi2c);

    switch (callback_id)
    {
        case HAL_I2C_MSPINIT_CB_ID:
        {
            hi2c->MspInitCallback = callback;
        }
        break;

        case HAL_I2C_MSPDEINIT_CB_ID:
        {
            hi2c->MspDeInitCallback = callback;
        }
        break;

        case HAL_I2C_MASTER_TX_COMPLETE_CB_ID:
        {
            hi2c->MasterTxCpltCallback = callback;
        }
        break;

        case HAL_I2C_MASTER_RX_COMPLETE_CB_ID:
        {
            hi2c->MasterRxCpltCallback = callback;
        }
        break;

        case HAL_I2C_SLAVE_TX_COMPLETE_CB_ID:
        {
            hi2c->SlaveTxCpltCallback = callback;
        }
        break;

        case HAL_I2C_SLAVE_RX_COMPLETE_CB_ID:
        {
            hi2c->SlaveRxCpltCallback = callback;
        }
        break;

        case HAL_I2C_ABORT_CB_ID:
        {
            hi2c->AbortCpltCallback = callback;
        }
        break;

        case HAL_I2C_ERROR_CB_ID:
        {
            hi2c->ErrorCallback = callback;
        }
        break;

        default:
        {
            /* Update the error code */
            hi2c->error_code = HAL_I2C_ERROR_INVALID_CALLBACK;
            /* Return error status */
            status = HAL_ERROR;
        }
        break;
    }

    /* Release the process protection */
    GIVE_MUTEX(hi2c);

    return status;
}

/**
 * @brief  Unregister an I2C Callback, I2C callback is redirected to the weak predefined callback
 * @param  hi2c Pointer to a I2C_Handle_T structure that contains the configuration information for the specified I2C module.
 * @param  callback_id ID of the callback to be unregistered
 *         This parameter can be one of the following values:
 *           @arg @ref HAL_I2C_MASTER_TX_COMPLETE_CB_ID I2C Master Tx Complete Callback ID
 *           @arg @ref HAL_I2C_MASTER_RX_COMPLETE_CB_ID I2C Master Rx Complete Callback ID
 *           @arg @ref HAL_I2C_SLAVE_TX_COMPLETE_CB_ID I2C Slave Tx Complete Callback ID
 *           @arg @ref HAL_I2C_SLAVE_RX_COMPLETE_CB_ID I2C Slave Rx Complete Callback ID
 *           @arg @ref HAL_I2C_ABORT_CB_ID I2C Abort Callback ID
 *           @arg @ref HAL_I2C_ERROR_CB_ID I2C Error Callback ID
 *           @arg @ref HAL_I2C_MSPINIT_CB_ID MspInit Callback ID
 *           @arg @ref HAL_I2C_MSPDEINIT_CB_ID MspDeInit Callback ID
 * @retval HAL status
 */
HAL_Status_T HAL_I2C_UnRegisterCallback(I2C_Handle_T *hi2c, HAL_I2C_CallbackID_T callback_id)
{
    HAL_Status_T status = HAL_OK;

    /* Check the I2C handle */
    ASSERT_PARAM(hi2c);
    /* Check the I2C instance */
    ASSERT_PARAM(IS_PERIPHERAL_I2C_INSTANCE(hi2c));

    /* Protect the process */
    TAKE_MUTEX(hi2c);

    switch (callback_id)
    {
        case HAL_I2C_MSPINIT_CB_ID:
        {
            hi2c->MspInitCallback = HAL_I2C_MspInit;
        }
        break;

        case HAL_I2C_MSPDEINIT_CB_ID:
        {
            hi2c->MspDeInitCallback = HAL_I2C_MspDeInit;
        }
        break;

        case HAL_I2C_MASTER_TX_COMPLETE_CB_ID:
        {
            hi2c->MasterTxCpltCallback = HAL_I2C_MasterTxCpltCallback;
        }
        break;

        case HAL_I2C_MASTER_RX_COMPLETE_CB_ID:
        {
            hi2c->MasterRxCpltCallback = HAL_I2C_MasterRxCpltCallback;
        }
        break;

        case HAL_I2C_SLAVE_TX_COMPLETE_CB_ID:
        {
            hi2c->SlaveTxCpltCallback = HAL_I2C_SlaveTxCpltCallback;
        }
        break;

        case HAL_I2C_SLAVE_RX_COMPLETE_CB_ID:
        {
            hi2c->SlaveRxCpltCallback = HAL_I2C_SlaveRxCpltCallback;
        }
        break;

        case HAL_I2C_ABORT_CB_ID:
        {
            hi2c->AbortCpltCallback = HAL_I2C_AbortCpltCallback;
        }
        break;

        case HAL_I2C_ERROR_CB_ID:
        {
            hi2c->ErrorCallback = HAL_I2C_ErrorCallback;
        }
        break;

        default:
        {
            /* Update the error code */
            hi2c->error_code = HAL_I2C_ERROR_INVALID_CALLBACK;
            /* Return error status */
            status = HAL_ERROR;
        }
        break;
    }

    /* Release the process protection */
    GIVE_MUTEX(hi2c);

    return status;
}
#endif /* configUSE_I2C_REGISTER_CALLBACKS */

/*
================================================================================
            ##### Peripheral State, Mode and Error functions #####
================================================================================
*/
/**
 * @brief  Returns the I2C state.
 * @param  hi2c Pointer to a I2C_Handle_T structure that contains the configuration information for the specified I2C.
 * @retval HAL state
 */
HAL_State_T HAL_I2C_GetState(I2C_Handle_T *hi2c)
{
    /* Check the I2C handle */
    ASSERT_PARAM(hi2c);
    /* Check the I2C instance */
    ASSERT_PARAM(IS_PERIPHERAL_I2C_INSTANCE(hi2c));

    return hi2c->state;
}

/**
 * @brief  Returns the I2C mode.
 * @param  hi2c Pointer to a I2C_Handle_T structure that contains the configuration information for the specified I2C.
 * @retval I2C mode
 */
I2C_Mode_T HAL_I2C_GetMode(I2C_Handle_T *hi2c)
{
    /* Check the I2C handle */
    ASSERT_PARAM(hi2c);
    /* Check the I2C instance */
    ASSERT_PARAM(IS_PERIPHERAL_I2C_INSTANCE(hi2c));

    return hi2c->mode;
}

/**
 * @brief  Return the I2C error code.
 * @param  hi2c Pointer to a I2C_Handle_T structure that contains the configuration information for the specified I2C.
 * @retval I2C error code
 */
uint32_t HAL_I2C_GetError(I2C_Handle_T *hi2c)
{
    /* Check the I2C handle */
    ASSERT_PARAM(hi2c);
    /* Check the I2C instance */
    ASSERT_PARAM(IS_PERIPHERAL_I2C_INSTANCE(hi2c));

    return hi2c->error_code;
}

/*
================================================================================
            ##### I/O Operation functions #####
================================================================================
*/
#if (configUSE_I2C_BLOCKING == 1)
/**
 * @brief  Transmits in master mode an amount of data in blocking mode.
 * @param  hi2c Pointer to a I2C_Handle_T structure that contains the configuration information for the specified I2C.
 * @param  dev_address Target device address
 * @param  data Pointer to data buffer
 * @param  size Amount of data elements to be sent
 * @param  timeout Timeout duration
 * @retval HAL status
 */
HAL_Status_T HAL_I2C_Master_Transmit(I2C_Handle_T *hi2c, uint16_t dev_address, uint8_t *data, uint16_t size, uint32_t timeout)
{
    HAL_Status_T status = HAL_OK;

    /* Check the I2C handle */
    ASSERT_PARAM(hi2c);
    /* Check the I2C instance */
    ASSERT_PARAM(IS_PERIPHERAL_I2C_INSTANCE(hi2c));
    /* Check the parameters */
    ASSERT_PARAM(data);
    ASSERT_PARAM(size);
    ASSERT_PARAM(timeout);

    if (hi2c->state == HAL_STATE_READY)
    {
        /* Protect the process */
        TAKE_MUTEX(hi2c);

        /* Update the I2C state */
        hi2c->state = HAL_STATE_BUSY;
        hi2c->mode  = HAL_I2C_MODE_MASTER;
        CLEAR_ERROR_CODE(hi2c);

        /* Init tick_start for timeout management */
        uint32_t tick_start = HAL_GetTick();

        /* Set runtime info */
        hi2c->buffer_ptr  = data;
        hi2c->xfer_size   = size;
        hi2c->xfer_count  = size;
        hi2c->direction   = I2C_DIRECTION_TX;
        hi2c->dev_address = dev_address;

        SN_I2C0_Type *reg = hi2c->instance;
        /* Clear status */
        I2C_CLEAR_ALL_STATUS(reg);
        /* Enable the peripheral */
        /* Set master mode */
        uint32_t ctrl_mask = I2C_CTRL_MASK_I2CEN | I2C_CTRL_MASK_MSTEN;

        /* Check clock speed */
        I2C_STATUS_T error_status = I2C_STATUS_NONE;
        if (hi2c->init.clock_speed < I2C_CLOCKSPEED_HIGH_SPEED) // FS mode
        {
            /* Send slave address */
            uint32_t addr = 0;
            if (hi2c->init.addressing_mode == I2C_ADDRESSINGMODE_7BIT) // 7-bit addressing mode
            {
                addr = I2C_7BITS_ADDR(hi2c->dev_address);
            }
            else // 10-bit addressing mode
            {
                addr = I2C_10BITS_ADDR_HIGH(hi2c->dev_address);
            }
            HAL_REG_WRITE(reg->DATA, addr);
            error_status = I2C_STATUS_NACK;
        }
        else // HS mode
        {
            /* Send HS code */
            HAL_REG_WRITE(reg->DATA, I2C_HS_CTRL_CODE);
        }

        /* Set start condition */
        ctrl_mask |= I2C_CTRL_MASK_START;
        HAL_REG_SBIT(reg->CTRL, ctrl_mask);
        /* Wait for transfer-done flag */
        status = _i2c_wait_for_flag_until_timeout_error_first(hi2c, I2C_STATUS_TRANSFER_DONE, error_status, tick_start, timeout, false);
        CHECK_STATUS_IN_TRANSFER(status);
        /* Clear Start & NACK flag */
        HAL_REG_WRITE(reg->STATUS, I2C_STATUS_START | I2C_STATUS_NACK);

        /* Check clock speed */
        if (hi2c->init.clock_speed >= I2C_CLOCKSPEED_HIGH_SPEED) // HS mode
        {
            /* Enable HS mode */
            HAL_REG_SBIT(reg->CTRL, I2C_CTRL_MASK_HSMODE);
            /* Send slave address */
            uint32_t addr = 0;
            if (hi2c->init.addressing_mode == I2C_ADDRESSINGMODE_7BIT) // 7-bit addressing mode
            {
                addr = I2C_7BITS_ADDR(hi2c->dev_address);
            }
            else // 10-bit addressing mode
            {
                addr = I2C_10BITS_ADDR_HIGH(hi2c->dev_address);
            }
            HAL_REG_WRITE(reg->DATA, addr);
            /* Set start condition */
            HAL_REG_SBIT(reg->CTRL, I2C_CTRL_MASK_START);
            /* Wait for transfer-done flag */
            status = _i2c_wait_for_flag_until_timeout_error_first(hi2c, I2C_STATUS_TRANSFER_DONE, I2C_STATUS_NACK, tick_start, timeout, false);
            CHECK_STATUS_IN_TRANSFER(status);
            /* Clear Start flag */
            HAL_REG_WRITE(reg->STATUS, I2C_STATUS_START);
        }

        /* 10-bit low address */
        if (HAL_REG_READ(reg->ADDR_b.ADDR10EN) == 1)
        {
            /* Send 10-bit low address */
            HAL_REG_WRITE(reg->DATA, I2C_10BITS_ADDR_LOW(hi2c->dev_address));
            /* Enable transfer byte */
            HAL_REG_SBIT(reg->CTRL, I2C_CTRL_MASK_TBEN);
            /* Wait for transfer-done flag */
            status = _i2c_wait_for_flag_until_timeout_error_first(hi2c, I2C_STATUS_TRANSFER_DONE, I2C_STATUS_NACK, tick_start, timeout, false);
            CHECK_STATUS_IN_TRANSFER(status);
        }

        /* Transmit data */
        while (hi2c->xfer_count > 0U)
        {
            /* Transmit byte */
            HAL_REG_WRITE(reg->DATA, *(hi2c->buffer_ptr));
            hi2c->buffer_ptr++;
            hi2c->xfer_count--;
            /* Enable transfer byte */
            HAL_REG_SBIT(reg->CTRL, I2C_CTRL_MASK_TBEN);

            if (hi2c->xfer_count > 0U)
            {
                /* Wait for transfer-done flag */
                status = _i2c_wait_for_flag_until_timeout_error_first(hi2c, I2C_STATUS_TRANSFER_DONE, I2C_STATUS_NACK, tick_start, timeout, false);
                if (status != HAL_OK)
                {
                    /* Set stop condition */
                    HAL_REG_SBIT(reg->CTRL, I2C_CTRL_MASK_STOP);
                }
                CHECK_STATUS_IN_LOOP(status);
            }
            else
            {
                /* Set stop condition */
                HAL_REG_SBIT(reg->CTRL, I2C_CTRL_MASK_STOP);
            }
        }

        /* Wait for stop flag */
        HAL_Status_T stop_status = _i2c_wait_for_flag_until_timeout_error_first(hi2c, I2C_STATUS_STOP, I2C_STATUS_NONE, tick_start, timeout, false);
        /* Clear TD flag */
        HAL_REG_WRITE(reg->STATUS, I2C_STATUS_TRANSFER_DONE);
        if (status == HAL_OK)
        {
            status = stop_status;
        }

        /* Disable the peripheral */
        PERIPHERAL_I2C_DISABLE(hi2c);
        /* Update the I2C state */
        hi2c->state = HAL_STATE_READY;
        hi2c->mode  = HAL_I2C_MODE_NONE;
        /* Check transfer count */
        if (hi2c->xfer_count != 0U)
        {
            if (status == HAL_OK)
            {
                status = HAL_ERROR;
            }
        }
        /* Release the process protection */
        GIVE_MUTEX(hi2c);
    }
    else if (hi2c->state == HAL_STATE_BUSY)
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
 * @brief  Receives in master mode an amount of data in blocking mode.
 * @param  hi2c Pointer to a I2C_Handle_T structure that contains the configuration information for the specified I2C.
 * @param  dev_address Target device address
 * @param  data Pointer to data buffer
 * @param  size Amount of data to be sent
 * @param  timeout Timeout duration
 * @retval HAL status
 */
HAL_Status_T HAL_I2C_Master_Receive(I2C_Handle_T *hi2c, uint16_t dev_address, uint8_t *data, uint16_t size, uint32_t timeout)
{
    HAL_Status_T status = HAL_OK;

    /* Check the I2C handle */
    ASSERT_PARAM(hi2c);
    /* Check the I2C instance */
    ASSERT_PARAM(IS_PERIPHERAL_I2C_INSTANCE(hi2c));
    /* Check the parameters */
    ASSERT_PARAM(data);
    ASSERT_PARAM(size);
    ASSERT_PARAM(timeout);

    if (hi2c->state == HAL_STATE_READY)
    {
        /* Protect the process */
        TAKE_MUTEX(hi2c);

        /* Update the I2C state */
        hi2c->state = HAL_STATE_BUSY;
        hi2c->mode  = HAL_I2C_MODE_MASTER;
        CLEAR_ERROR_CODE(hi2c);

        /* Init tick_start for timeout management */
        uint32_t tick_start = HAL_GetTick();

        /* Set runtime info */
        hi2c->buffer_ptr  = data;
        hi2c->xfer_size   = size;
        hi2c->xfer_count  = size;
        hi2c->direction   = I2C_DIRECTION_RX;
        hi2c->dev_address = dev_address;

        SN_I2C0_Type *reg = hi2c->instance;
        /* Clear status */
        I2C_CLEAR_ALL_STATUS(reg);
        /* Enable the peripheral */
        /* Set master mode */
        uint32_t ctrl_mask = I2C_CTRL_MASK_I2CEN | I2C_CTRL_MASK_MSTEN;

        /* Check clock speed */
        I2C_STATUS_T error_status = I2C_STATUS_NONE;
        if (hi2c->init.clock_speed < I2C_CLOCKSPEED_HIGH_SPEED) // FS mode
        {
            /* Send slave address */
            uint32_t addr = 0;
            if (hi2c->init.addressing_mode == I2C_ADDRESSINGMODE_7BIT) // 7-bit addressing mode
            {
                addr = I2C_7BITS_ADDR(hi2c->dev_address) | I2C_READ_FLAG;
            }
            else // 10-bit addressing mode
            {
                addr = I2C_10BITS_ADDR_HIGH(hi2c->dev_address);
            }
            HAL_REG_WRITE(reg->DATA, addr);
            error_status = I2C_STATUS_NACK;
        }
        else // HS mode
        {
            /* Send HS code */
            HAL_REG_WRITE(reg->DATA, I2C_HS_CTRL_CODE);
        }

        /* Set start condition */
        ctrl_mask |= I2C_CTRL_MASK_START;
        HAL_REG_SBIT(reg->CTRL, ctrl_mask);
        /* Wait for transfer-done flag */
        status = _i2c_wait_for_flag_until_timeout_error_first(hi2c, I2C_STATUS_TRANSFER_DONE, error_status, tick_start, timeout, false);
        CHECK_STATUS_IN_TRANSFER(status);
        /* Clear Start & NACK flag */
        HAL_REG_WRITE(reg->STATUS, I2C_STATUS_START | I2C_STATUS_NACK);

        /* Check clock speed */
        if (hi2c->init.clock_speed >= I2C_CLOCKSPEED_HIGH_SPEED) // HS mode
        {
            /* Enable HS mode */
            HAL_REG_SBIT(reg->CTRL, I2C_CTRL_MASK_HSMODE);
            /* Send slave address */
            uint32_t addr = 0;
            if (hi2c->init.addressing_mode == I2C_ADDRESSINGMODE_7BIT) // 7-bit addressing mode
            {
                addr = I2C_7BITS_ADDR(hi2c->dev_address) | I2C_READ_FLAG;
            }
            else // 10-bit addressing mode
            {
                addr = I2C_10BITS_ADDR_HIGH(hi2c->dev_address);
            }
            HAL_REG_WRITE(reg->DATA, addr);
            /* Set start condition */
            HAL_REG_SBIT(reg->CTRL, I2C_CTRL_MASK_START);
            /* Wait for transfer-done flag */
            status = _i2c_wait_for_flag_until_timeout_error_first(hi2c, I2C_STATUS_TRANSFER_DONE, I2C_STATUS_NACK, tick_start, timeout, false);
            CHECK_STATUS_IN_TRANSFER(status);
            /* Clear Start flag */
            HAL_REG_WRITE(reg->STATUS, I2C_STATUS_START);
        }

        /* 10-bit low address */
        if (HAL_REG_READ(reg->ADDR_b.ADDR10EN) == 1)
        {
            /* Send 10-bit low address */
            HAL_REG_WRITE(reg->DATA, I2C_10BITS_ADDR_LOW(hi2c->dev_address));
            /* Enable transfer byte */
            HAL_REG_SBIT(reg->CTRL, I2C_CTRL_MASK_TBEN);
            /* Wait for transfer-done flag */
            status = _i2c_wait_for_flag_until_timeout_error_first(hi2c, I2C_STATUS_TRANSFER_DONE, I2C_STATUS_NACK, tick_start, timeout, false);
            CHECK_STATUS_IN_TRANSFER(status);
            /* Send 10-bit high address with reading flag */
            HAL_REG_WRITE(reg->DATA, I2C_10BITS_ADDR_HIGH(hi2c->dev_address) | I2C_READ_FLAG);
            /* Set 2nd start condition */
            HAL_REG_SBIT(reg->CTRL, I2C_CTRL_MASK_START);
            /* Wait for transfer-done flag */
            status = _i2c_wait_for_flag_until_timeout_error_first(hi2c, I2C_STATUS_TRANSFER_DONE, I2C_STATUS_NACK, tick_start, timeout, false);
            CHECK_STATUS_IN_TRANSFER(status);
            /* Clear Start flag */
            HAL_REG_WRITE(reg->STATUS, I2C_STATUS_START);
        }

        if (hi2c->xfer_count == 1U) // Only one transaction of data
        {
            /* Send NACK */
            HAL_REG_SBIT(reg->CTRL, I2C_CTRL_MASK_ACKNACK);
        }
        /* Enable transfer byte */
        HAL_REG_SBIT(reg->CTRL, I2C_CTRL_MASK_TBEN);

        /* Receive data */
        while (hi2c->xfer_count > 0U)
        {
            /* Wait for transfer-done flag */
            status = _i2c_wait_for_flag_until_timeout_error_first(hi2c, I2C_STATUS_TRANSFER_DONE, I2C_STATUS_NONE, tick_start, timeout, false);
            CHECK_STATUS_IN_LOOP(status);

            /* Receive byte */
            *(hi2c->buffer_ptr) = HAL_REG_READ(reg->DATA);
            hi2c->buffer_ptr++;
            hi2c->xfer_count--;
            /* Enable transfer byte */
            HAL_REG_SBIT(reg->CTRL, I2C_CTRL_MASK_TBEN);

            if (hi2c->xfer_count == 1U) // The next one is the last transaction of data
            {
                /* Send NACK */
                HAL_REG_SBIT(reg->CTRL, I2C_CTRL_MASK_ACKNACK);
            }
        }

        /* Set stop condition */
        HAL_REG_SBIT(reg->CTRL, I2C_CTRL_MASK_STOP);
        /* Wait for stop flag */
        HAL_Status_T stop_status = _i2c_wait_for_flag_until_timeout_error_first(hi2c, I2C_STATUS_STOP, I2C_STATUS_NONE, tick_start, timeout, false);
        if (status == HAL_OK)
        {
            status = stop_status;
        }

        /* Reset NACK */
        HAL_REG_CBIT(reg->CTRL, I2C_CTRL_MASK_ACKNACK);
        /* Disable the peripheral */
        PERIPHERAL_I2C_DISABLE(hi2c);
        /* Update the I2C state */
        hi2c->state = HAL_STATE_READY;
        hi2c->mode  = HAL_I2C_MODE_NONE;
        /* Check transfer count */
        if (hi2c->xfer_count != 0U)
        {
            if (status == HAL_OK)
            {
                status = HAL_ERROR;
            }
        }
        /* Release the process protection */
        GIVE_MUTEX(hi2c);
    }
    else if (hi2c->state == HAL_STATE_BUSY)
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
 * @brief  Transmits in slave mode an amount of data in blocking mode.
 * @param  hi2c Pointer to a I2C_Handle_T structure that contains the configuration information for the specified I2C.
 * @param  data Pointer to data buffer
 * @param  size Amount of data to be sent
 * @param  timeout Timeout duration
 * @retval HAL status
 */
HAL_Status_T HAL_I2C_Slave_Transmit(I2C_Handle_T *hi2c, uint8_t *data, uint16_t size, uint32_t timeout)
{
    HAL_Status_T status = HAL_OK;

    /* Check the I2C handle */
    ASSERT_PARAM(hi2c);
    /* Check the I2C instance */
    ASSERT_PARAM(IS_PERIPHERAL_I2C_INSTANCE(hi2c));
    /* Check the parameters */
    ASSERT_PARAM(data);
    ASSERT_PARAM(size);
    ASSERT_PARAM(timeout);

    if (hi2c->state == HAL_STATE_READY)
    {
        /* Protect the process */
        TAKE_MUTEX(hi2c);

        /* Update the I2C state */
        hi2c->state = HAL_STATE_BUSY;
        hi2c->mode  = HAL_I2C_MODE_SLAVE;
        CLEAR_ERROR_CODE(hi2c);

        /* Init tick_start for timeout management */
        uint32_t tick_start = HAL_GetTick();

        /* Set runtime info */
        hi2c->buffer_ptr = data;
        hi2c->xfer_size  = size;
        hi2c->xfer_count = size;
        hi2c->direction  = I2C_DIRECTION_TX;

        SN_I2C0_Type *reg = hi2c->instance;
        /* Clear status */
        I2C_CLEAR_ALL_STATUS(reg);
        /* Set slave mode */
        HAL_REG_CBIT(reg->CTRL, I2C_CTRL_MASK_MSTEN);
        /* Set slave address */
        HAL_REG_WRITE(reg->ADDR_b.ADDR, hi2c->own_address);
        /* Enable the peripheral */
        PERIPHERAL_I2C_ENABLE(hi2c);

        /* Wait for address-hit flag */
        status = _i2c_wait_for_flag_until_timeout_error_first(hi2c, I2C_STATUS_HIT, I2C_STATUS_STOP, tick_start, timeout, false);
        CHECK_STATUS_IN_TRANSFER(status);
        /* Clear TD flag */
        HAL_REG_WRITE(reg->STATUS, I2C_STATUS_TRANSFER_DONE);

        /* 10-bit addressing mode */
        if (HAL_REG_READ(reg->ADDR_b.ADDR10EN) == 1)
        {
            /* Enable transfer byte */
            HAL_REG_SBIT(reg->CTRL, I2C_CTRL_MASK_TBEN);
            /* Wait for 2nd address-hit flag */
            status = _i2c_wait_for_flag_until_timeout_error_first(hi2c, I2C_STATUS_HIT, I2C_STATUS_STOP, tick_start, timeout, false);
            CHECK_STATUS_IN_TRANSFER(status);
            /* Clear TD flag */
            HAL_REG_WRITE(reg->STATUS, I2C_STATUS_TRANSFER_DONE);
        }

        /* Transmit data */
        while (hi2c->xfer_count > 0U)
        {
            /* Transmit byte */
            HAL_REG_WRITE(reg->DATA, *(hi2c->buffer_ptr));
            hi2c->buffer_ptr++;
            hi2c->xfer_count--;
            /* Enable transfer byte */
            HAL_REG_SBIT(reg->CTRL, I2C_CTRL_MASK_TBEN);

            if (hi2c->xfer_count > 0U)
            {
                /* Wait for transfer-done flag */
                status = _i2c_wait_for_flag_until_timeout_error_first(hi2c, I2C_STATUS_TRANSFER_DONE, I2C_STATUS_NACK, tick_start, timeout, false);
                CHECK_STATUS_IN_LOOP(status);
            }
        }

        /* Wait for stop flag */
        HAL_Status_T stop_status = _i2c_wait_for_flag_until_timeout_error_first(hi2c, I2C_STATUS_STOP, I2C_STATUS_NONE, tick_start, timeout, true);
        /* Clear TD flag */
        HAL_REG_WRITE(reg->STATUS, I2C_STATUS_TRANSFER_DONE);
        if (status == HAL_OK)
        {
            status = stop_status;
        }

        /* Disable the peripheral */
        PERIPHERAL_I2C_DISABLE(hi2c);
        /* Update the I2C state */
        hi2c->state = HAL_STATE_READY;
        hi2c->mode  = HAL_I2C_MODE_NONE;
        /* Check transfer count */
        if (hi2c->xfer_count != 0U)
        {
            if (status == HAL_OK)
            {
                status = HAL_ERROR;
            }
        }
        /* Release the process protection */
        GIVE_MUTEX(hi2c);
    }
    else if (hi2c->state == HAL_STATE_BUSY)
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
 * @brief  Receive in slave mode an amount of data in blocking mode
 * @param  hi2c Pointer to a I2C_Handle_T structure that contains the configuration information for the specified I2C.
 * @param  data Pointer to data buffer
 * @param  size Amount of data to be sent
 * @param  timeout Timeout duration
 * @retval HAL status
 */
HAL_Status_T HAL_I2C_Slave_Receive(I2C_Handle_T *hi2c, uint8_t *data, uint16_t size, uint32_t timeout)
{
    HAL_Status_T status = HAL_OK;

    /* Check the I2C handle */
    ASSERT_PARAM(hi2c);
    /* Check the I2C instance */
    ASSERT_PARAM(IS_PERIPHERAL_I2C_INSTANCE(hi2c));
    /* Check the parameters */
    ASSERT_PARAM(data);
    ASSERT_PARAM(size);
    ASSERT_PARAM(timeout);

    if (hi2c->state == HAL_STATE_READY)
    {
        /* Protect the process */
        TAKE_MUTEX(hi2c);

        /* Update the I2C state */
        hi2c->state = HAL_STATE_BUSY;
        hi2c->mode  = HAL_I2C_MODE_SLAVE;
        CLEAR_ERROR_CODE(hi2c);

        /* Init tick_start for timeout management */
        uint32_t tick_start = HAL_GetTick();

        /* Set runtime info */
        hi2c->buffer_ptr = data;
        hi2c->xfer_size  = size;
        hi2c->xfer_count = size;
        hi2c->direction  = I2C_DIRECTION_RX;

        SN_I2C0_Type *reg = hi2c->instance;
        /* Clear status */
        I2C_CLEAR_ALL_STATUS(reg);
        /* Set slave mode */
        HAL_REG_CBIT(reg->CTRL, I2C_CTRL_MASK_MSTEN);
        /* Set slave address */
        HAL_REG_WRITE(reg->ADDR_b.ADDR, hi2c->own_address);
        /* Enable the peripheral */
        PERIPHERAL_I2C_ENABLE(hi2c);

        /* Wait for address-hit flag */
        status = _i2c_wait_for_flag_until_timeout_error_first(hi2c, I2C_STATUS_HIT, I2C_STATUS_STOP, tick_start, timeout, false);
        CHECK_STATUS_IN_TRANSFER(status);
        /* Clear TD flag */
        HAL_REG_WRITE(reg->STATUS, I2C_STATUS_TRANSFER_DONE);

        if (hi2c->xfer_count == 1U) // Only one transaction of data
        {
            /* Send NACK */
            HAL_REG_SBIT(reg->CTRL, I2C_CTRL_MASK_ACKNACK);
        }
        /* Enable transfer byte */
        HAL_REG_SBIT(reg->CTRL, I2C_CTRL_MASK_TBEN);

        /* Receive data */
        while (hi2c->xfer_count > 0U)
        {
            /* Wait for transfer-done flag */
            status = _i2c_wait_for_flag_until_timeout_target_first(hi2c, I2C_STATUS_TRANSFER_DONE, I2C_STATUS_STOP, tick_start, timeout);
            CHECK_STATUS_IN_LOOP(status);

            /* Receive byte */
            *(hi2c->buffer_ptr) = HAL_REG_READ(reg->DATA);
            hi2c->buffer_ptr++;
            hi2c->xfer_count--;
            /* Enable transfer byte */
            HAL_REG_SBIT(reg->CTRL, I2C_CTRL_MASK_TBEN);

            if (hi2c->xfer_count == 1U) // The next one is the last transaction of data
            {
                /* Send NACK */
                HAL_REG_SBIT(reg->CTRL, I2C_CTRL_MASK_ACKNACK);
            }
        }

        /* All data has been transmitted */
        if (status == HAL_OK)
        {
            /* Wait for stop flag */
            status = _i2c_wait_for_flag_until_timeout_error_first(hi2c, I2C_STATUS_STOP, I2C_STATUS_NONE, tick_start, timeout, true);
        }

        /* Reset NACK */
        HAL_REG_CBIT(reg->CTRL, I2C_CTRL_MASK_ACKNACK);
        /* Disable the peripheral */
        PERIPHERAL_I2C_DISABLE(hi2c);
        /* Update the I2C state */
        hi2c->state = HAL_STATE_READY;
        hi2c->mode  = HAL_I2C_MODE_NONE;
        /* Check transfer count */
        if (hi2c->xfer_count != 0U)
        {
            if (status == HAL_OK)
            {
                status = HAL_ERROR;
            }
        }
        /* Release the process protection */
        GIVE_MUTEX(hi2c);
    }
    else if (hi2c->state == HAL_STATE_BUSY)
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
#endif /* configUSE_I2C_BLOCKING */

#if (configUSE_I2C_NONBLOCKING_IT == 1)
/**
 * @brief  Transmit in master mode an amount of data in non-blocking mode with Interrupt
 * @param  hi2c Pointer to a I2C_Handle_T structure that contains the configuration information for the specified I2C.
 * @param  dev_address Target device address
 * @param  data Pointer to data buffer
 * @param  size Amount of data to be sent
 * @retval HAL status
 */
HAL_Status_T HAL_I2C_Master_Transmit_IT(I2C_Handle_T *hi2c, uint16_t dev_address, uint8_t *data, uint16_t size)
{
    hi2c->direction   = I2C_DIRECTION_TX;
    hi2c->xfer_option = I2C_NO_OPTION_FRAME;
    return _i2c_master_transfer_it(hi2c, I2C_DIRECTION_TX, dev_address, data, size, I2C_NO_OPTION_FRAME);
}

/**
 * @brief  Receive in master mode an amount of data in non-blocking mode with Interrupt
 * @param  hi2c Pointer to a I2C_Handle_T structure that contains the configuration information for the specified I2C.
 * @param  dev_address Target device address
 * @param  data Pointer to data buffer
 * @param  size Amount of data to be sent
 * @retval HAL status
 */
HAL_Status_T HAL_I2C_Master_Receive_IT(I2C_Handle_T *hi2c, uint16_t dev_address, uint8_t *data, uint16_t size)
{
    hi2c->direction   = I2C_DIRECTION_RX;
    hi2c->xfer_option = I2C_NO_OPTION_FRAME;
    return _i2c_master_transfer_it(hi2c, I2C_DIRECTION_RX, dev_address, data, size, I2C_NO_OPTION_FRAME);
}

/**
 * @brief  Transmit in slave mode an amount of data in non-blocking mode with Interrupt
 * @param  hi2c Pointer to a I2C_Handle_T structure that contains the configuration information for the specified I2C.
 * @param  data Pointer to data buffer
 * @param  size Amount of data to be sent
 * @retval HAL status
 */
HAL_Status_T HAL_I2C_Slave_Transmit_IT(I2C_Handle_T *hi2c, uint8_t *data, uint16_t size)
{
    hi2c->direction   = I2C_DIRECTION_TX;
    hi2c->xfer_option = I2C_NO_OPTION_FRAME;
    return _i2c_slave_transfer_it(hi2c, I2C_DIRECTION_TX, data, size, I2C_NO_OPTION_FRAME);
}

/**
 * @brief  Receive in slave mode an amount of data in non-blocking mode with Interrupt
 * @param  hi2c Pointer to a I2C_Handle_T structure that contains the configuration information for the specified I2C.
 * @param  data Pointer to data buffer
 * @param  size Amount of data to be sent
 * @retval HAL status
 */
HAL_Status_T HAL_I2C_Slave_Receive_IT(I2C_Handle_T *hi2c, uint8_t *data, uint16_t size)
{
    hi2c->direction   = I2C_DIRECTION_RX;
    hi2c->xfer_option = I2C_NO_OPTION_FRAME;
    return _i2c_slave_transfer_it(hi2c, I2C_DIRECTION_RX, data, size, I2C_NO_OPTION_FRAME);
}

/**
 * @brief  Sequential transmit in master I2C mode an amount of data in non-blocking mode with Interrupt.
 * @note   This interface allow to manage repeated start condition when a direction change during transfer
 * @param  hi2c Pointer to a I2C_Handle_T structure that contains the configuration information for the specified I2C.
 * @param  dev_address Target device address
 * @param  data Pointer to data buffer
 * @param  size Amount of data to be sent
 * @param  xfer_option Option of Transfer, value of @ref i2c_xfer_option
 * @retval HAL status
 */
HAL_Status_T HAL_I2C_Master_Seq_Transmit_IT(I2C_Handle_T *hi2c, uint16_t dev_address, uint8_t *data, uint16_t size, uint32_t xfer_option)
{
    ASSERT_PARAM(IS_I2C_TRANSFER_OPTION(xfer_option));
    return _i2c_master_transfer_it(hi2c, I2C_DIRECTION_TX, dev_address, data, size, xfer_option);
}

/**
 * @brief  Sequential receive in master I2C mode an amount of data in non-blocking mode with Interrupt
 * @note   This interface allow to manage repeated start condition when a direction change during transfer
 * @param  hi2c Pointer to a I2C_Handle_T structure that contains the configuration information for the specified I2C.
 * @param  dev_address Target device address
 * @param  data Pointer to data buffer
 * @param  size Amount of data to be sent
 * @param  xfer_option Option of Transfer, value of @ref i2c_xfer_option
 * @retval HAL status
 */
HAL_Status_T HAL_I2C_Master_Seq_Receive_IT(I2C_Handle_T *hi2c, uint16_t dev_address, uint8_t *data, uint16_t size, uint32_t xfer_option)
{
    ASSERT_PARAM(IS_I2C_TRANSFER_OPTION(xfer_option));
    return _i2c_master_transfer_it(hi2c, I2C_DIRECTION_RX, dev_address, data, size, xfer_option);
}

/**
 * @brief  Sequential transmit in slave mode an amount of data in non-blocking mode with Interrupt
 * @note   This interface allow to manage repeated start condition when a direction change during transfer
 * @param  hi2c Pointer to a I2C_Handle_T structure that contains the configuration information for the specified I2C.
 * @param  data Pointer to data buffer
 * @param  size Amount of data to be sent
 * @param  xfer_option Option of Transfer, value of @ref i2c_xfer_option
 * @retval HAL status
 */
HAL_Status_T HAL_I2C_Slave_Seq_Transmit_IT(I2C_Handle_T *hi2c, uint8_t *data, uint16_t size, uint32_t xfer_option)
{
    ASSERT_PARAM(IS_I2C_TRANSFER_OPTION(xfer_option));
    return _i2c_slave_transfer_it(hi2c, I2C_DIRECTION_TX, data, size, xfer_option);
}

/**
 * @brief  Sequential receive in slave mode an amount of data in non-blocking mode with Interrupt
 * @note   This interface allow to manage repeated start condition when a direction change during transfer
 * @param  hi2c Pointer to a I2C_Handle_T structure that contains the configuration information for the specified I2C.
 * @param  data Pointer to data buffer
 * @param  size Amount of data to be sent
 * @param  xfer_option Option of Transfer, value of @ref i2c_xfer_option
 * @retval HAL status
 */
HAL_Status_T HAL_I2C_Slave_Seq_Receive_IT(I2C_Handle_T *hi2c, uint8_t *data, uint16_t size, uint32_t xfer_option)
{
    ASSERT_PARAM(IS_I2C_TRANSFER_OPTION(xfer_option));
    return _i2c_slave_transfer_it(hi2c, I2C_DIRECTION_RX, data, size, xfer_option);
}

/**
 * @brief  Transmit and receive in slave mode an amount of data in non-blocking mode with Interrupt
 * @param  hi2c Pointer to a I2C_Handle_T structure that contains the configuration information for the specified I2C.
 * @param  tx_data Pointer to TX data buffer
 * @param  tx_size Amount of data to be sent
 * @param  rx_data Pointer to RX data buffer
 * @param  rx_size Amount of data to be received
 * @retval HAL status
 */
HAL_Status_T HAL_I2CEx_Slave_Transfer_IT(I2C_Handle_T *hi2c, uint8_t *tx_data, uint16_t tx_size, uint8_t *rx_data, uint16_t rx_size)
{
    HAL_Status_T status = HAL_OK;

    /* Check the I2C handle */
    ASSERT_PARAM(hi2c);
    /* Check the I2C instance */
    ASSERT_PARAM(IS_PERIPHERAL_I2C_INSTANCE(hi2c));
    /* Check the parameters */
    ASSERT_PARAM(hi2c->xfer_option == I2C_NO_OPTION_FRAME);
    ASSERT_PARAM(tx_data || rx_data);
    if (tx_data != NULL)
    {
        ASSERT_PARAM(tx_size);
    }
    else
    {
        tx_size = 0;
    }
    if (rx_data != NULL)
    {
        ASSERT_PARAM(rx_size);
    }
    else
    {
        rx_size = 0;
    }

    if (hi2c->state == HAL_STATE_READY)
    {
        /* Protect the process */
        TAKE_MUTEX(hi2c);

        /* Update the I2C state */
        hi2c->state = HAL_STATE_BUSY;
        hi2c->mode  = HAL_I2C_MODE_SLAVE;
        CLEAR_ERROR_CODE(hi2c);

        /* Set runtime info */
        hi2c->ext_xfer.en            = HAL_ENABLE;
        hi2c->ext_xfer.tx_buffer_ptr = tx_data;
        hi2c->ext_xfer.tx_xfer_size  = tx_size;
        hi2c->ext_xfer.tx_xfer_count = tx_size;
        hi2c->ext_xfer.tx_xfer_num   = 0;
        hi2c->ext_xfer.rx_buffer_ptr = rx_data;
        hi2c->ext_xfer.rx_xfer_size  = rx_size;
        hi2c->ext_xfer.rx_xfer_count = rx_size;
        hi2c->ext_xfer.rx_xfer_num   = 0;
        hi2c->direction              = I2C_DIRECTION_NONE;
        hi2c->xfer_state             = I2C_XFER_STATE_START;

        SN_I2C0_Type *reg = hi2c->instance;
        /* Clear I2C status */
        I2C_CLEAR_ALL_STATUS(reg);
        /* Set slave mode */
        HAL_REG_CBIT(reg->CTRL, I2C_CTRL_MASK_MSTEN);
        /* Set slave address */
        HAL_REG_WRITE(reg->ADDR_b.ADDR, hi2c->own_address);
        /* Enable the peripheral */
        /* Enable interrupt: START & TD & STOP & NACK & SAM */
        /* Enable transfer byte */
        uint32_t ctrl_mask = I2C_CTRL_MASK_I2CEN | I2C_CTRL_MASK_STARTIEN | I2C_CTRL_MASK_TDIEN | I2C_CTRL_MASK_STOPIEN | I2C_CTRL_MASK_NACKIEN | I2C_CTRL_MASK_SAMIEN | I2C_CTRL_MASK_TBEN;

        /* Check addressing mode */
        if (hi2c->init.addressing_mode == I2C_ADDRESSINGMODE_7BIT) // 7-bit addressing mode
        {
            hi2c->xfer_state = I2C_XFER_STATE_7BITS_ADDR;
        }
        else // 10-bit addressing mode
        {
            hi2c->xfer_state = I2C_XFER_STATE_10BITS_ADDR_H;
        }

        /* Set Control Register */
        HAL_REG_SBIT(reg->CTRL, ctrl_mask);
        /* Release the process protection */
        GIVE_MUTEX(hi2c);
    }
    else if (hi2c->state == HAL_STATE_BUSY)
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
 * @brief  Update data buffer for transmitting during extended slave transfer.
 * @param  hi2c Pointer to a I2C_Handle_T structure that contains the configuration information for the specified I2C.
 * @param  tx_data Pointer to TX data buffer
 * @param  tx_size Amount of data to be sent
 * @retval HAL status
 */
HAL_Status_T HAL_I2CEx_Slave_SetTXBuffer(I2C_Handle_T *hi2c, uint8_t *tx_data, uint16_t tx_size)
{
    /* Check the I2C handle */
    ASSERT_PARAM(hi2c);
    /* Check the I2C instance */
    ASSERT_PARAM(IS_PERIPHERAL_I2C_INSTANCE(hi2c));
    /* Check the parameters */
    ASSERT_PARAM(tx_data);
    ASSERT_PARAM(tx_size);

    if (hi2c->ext_xfer.en == HAL_DISABLE)
    {
        return HAL_ERROR;
    }
    if (hi2c->state != HAL_STATE_BUSY)
    {
        return HAL_ERROR;
    }

    hi2c->ext_xfer.tx_buffer_ptr = tx_data;
    hi2c->ext_xfer.tx_xfer_size  = tx_size;
    hi2c->ext_xfer.tx_xfer_count = tx_size;
    hi2c->ext_xfer.tx_xfer_num   = 0;

    return HAL_OK;
}

/**
 * @brief  Update data buffer for receiving during extended slave transfer.
 * @param  hi2c Pointer to a I2C_Handle_T structure that contains the configuration information for the specified I2C.
 * @param  rx_data Pointer to RX data buffer
 * @param  rx_size Amount of data to be received
 * @retval HAL status
 */
HAL_Status_T HAL_I2CEx_Slave_SetRXBuffer(I2C_Handle_T *hi2c, uint8_t *rx_data, uint16_t rx_size)
{
    /* Check the I2C handle */
    ASSERT_PARAM(hi2c);
    /* Check the I2C instance */
    ASSERT_PARAM(IS_PERIPHERAL_I2C_INSTANCE(hi2c));
    /* Check the parameters */
    ASSERT_PARAM(rx_data);
    ASSERT_PARAM(rx_size);

    if (hi2c->ext_xfer.en == HAL_DISABLE)
    {
        return HAL_ERROR;
    }
    if (hi2c->state != HAL_STATE_BUSY)
    {
        return HAL_ERROR;
    }

    hi2c->ext_xfer.rx_buffer_ptr = rx_data;
    hi2c->ext_xfer.rx_xfer_size  = rx_size;
    hi2c->ext_xfer.rx_xfer_count = rx_size;
    hi2c->ext_xfer.rx_xfer_num   = 0;

    return HAL_OK;
}

/**
 * @brief  Abort a master I2C IT process communication with Interrupt.
 * @param  hi2c Pointer to a I2C_HandleTypeDef structure that contains the configuration information for the specified I2C.
 * @retval HAL status
 */
HAL_Status_T HAL_I2C_Master_Abort_IT(I2C_Handle_T *hi2c)
{
    /* Check the I2C handle */
    ASSERT_PARAM(hi2c);
    /* Check the I2C instance */
    ASSERT_PARAM(IS_PERIPHERAL_I2C_INSTANCE(hi2c));
    /* Check transfer type */
    ASSERT_PARAM(HAL_REG_READ(hi2c->instance->ADDR_b.M2BIDX_EN) == HAL_DISABLE);
    /* Check the mode and states */
    RET_FLAG_FALSE(hi2c->mode == HAL_I2C_MODE_MASTER, HAL_ERROR);
    RET_FLAG_FALSE(hi2c->state == HAL_STATE_BUSY, HAL_ERROR);
    RET_FLAG_FALSE(hi2c->direction <= I2C_DIRECTION_RX, HAL_ERROR);

    /* Protect the process */
    TAKE_MUTEX(hi2c);
    /* Set abort flag */
    hi2c->abort = 1;
    /* Release the process protection */
    GIVE_MUTEX(hi2c);

    return HAL_OK;
}
#endif /* configUSE_I2C_NONBLOCKING_IT */

#if (configUSE_I2C_NONBLOCKING_BURST == 1)
/**
 * @brief  Transmit in master mode an amount of data burst mode (non-blocking)
 * @param  hi2c Pointer to a I2C_Handle_T structure that contains the configuration information for the specified I2C.
 * @param  dev_address Target device address
 * @param  data Pointer to data buffer
 * @param  size Amount of data to be sent
 * @param  burst_options Pointer to a I2C_Burst_T structure that contains the options for burst mode
 * @retval HAL status
 */
HAL_Status_T HAL_I2C_Master_Transmit_BURST(I2C_Handle_T *hi2c, uint16_t dev_address, uint8_t *data, uint8_t size, I2C_Burst_T *burst_options)
{
    return _i2c_master_transfer_burst(hi2c, I2C_DIRECTION_TX_BURST, dev_address, data, size, burst_options);
}

/**
 * @brief  Receive in master mode an amount of data burst mode (non-blocking)
 * @param  hi2c Pointer to a I2C_Handle_T structure that contains the configuration information for the specified I2C.
 * @param  dev_address Target device address
 * @param  burst_options Pointer to a I2C_Burst_T structure that contains the options for burst mode
 * @param  data Pointer to data buffer
 * @param  size Amount of data to be sent
 * @retval HAL status
 */
HAL_Status_T HAL_I2C_Master_Receive_BURST(I2C_Handle_T *hi2c, uint16_t dev_address, uint8_t *data, uint8_t size, I2C_Burst_T *burst_options)
{
    return _i2c_master_transfer_burst(hi2c, I2C_DIRECTION_RX_BURST, dev_address, data, size, burst_options);
}
#endif /* configUSE_I2C_NONBLOCKING_BURST */

/*
================================================================================
            ##### IRQHandler and Event Callback functions #####
================================================================================
*/
#if IT_OR_BURST
/**
 * @brief  This function handles I2C interrupt request.
 * @param  hi2c Pointer to a I2C_Handle_T structure that contains the configuration information for the specified I2C.
 * @retval None
 */
void HAL_I2C_IRQHandler(I2C_Handle_T *hi2c)
{
    /* Get status */
    SN_I2C0_Type *reg        = hi2c->instance;
    uint32_t      raw_status = HAL_REG_READ(reg->STATUS);
    if (hi2c->ext_xfer.en == HAL_DISABLE)
    {
        /* Check AL & HS */
        if (hi2c->mode == HAL_I2C_MODE_MASTER)
        {
            if (HAL_FLAG_HAS(raw_status, I2C_STATUS_ARBITRATION_LOST))
            {
                /* Clear flag */
                HAL_REG_WRITE(reg->STATUS, I2C_STATUS_ARBITRATION_LOST);
                /* Check NACK & Stop */
                if (HAL_FLAG_LACKS(raw_status, I2C_STATUS_NACK) && HAL_FLAG_LACKS(raw_status, I2C_STATUS_STOP))
                {
                    /* Call arbitration lost handler */
                    _i2c_master_al_handler(hi2c);
                    return;
                }
            }
        }
        else if (hi2c->mode == HAL_I2C_MODE_SLAVE)
        {
            if (HAL_FLAG_HAS(raw_status, I2C_STATUS_HS_MODE)) // HS mode detected
            {
                /* Clear flag */
                HAL_REG_WRITE(reg->STATUS, I2C_STATUS_HS_MODE);
                /* Clear flag */
                HAL_REG_WRITE(reg->STATUS, I2C_STATUS_TRANSFER_DONE);

                return;
            }
        }
        else
        {
            /* Unexpected */
            return;
        }
        /* Call handler  */
        IRQ_Handler[hi2c->mode][hi2c->direction](hi2c, raw_status);
    }
    else if (hi2c->ext_xfer.en == HAL_ENABLE)
    {
        if (HAL_FLAG_HAS(raw_status, I2C_STATUS_HS_MODE)) // HS mode detected
        {
            /* Clear flag */
            HAL_REG_WRITE(reg->STATUS, I2C_STATUS_HS_MODE);

            return;
        }

        if (hi2c->init.addressing_mode == I2C_ADDRESSINGMODE_7BIT)
        {
            if (HAL_FLAG_HAS(raw_status, I2C_STATUS_START))
            {
                /* Clear flag */
                HAL_REG_WRITE(reg->STATUS, I2C_STATUS_START);
                /* Clear NACK */
                HAL_REG_CBIT(reg->CTRL, I2C_CTRL_MASK_ACKNACK);

                if (hi2c->xfer_state == I2C_XFER_STATE_DATA || hi2c->xfer_state == I2C_XFER_STATE_STOP)
                {
                    hi2c->xfer_state = I2C_XFER_STATE_7BITS_ADDR;

                    _i2c_slave_cplt_callback(hi2c);
                }
            }

            if (IS_I2C_MASTER_TX_SLAVE_RX(raw_status))
            {
                _i2c_ext_slave_rx_it_handler(hi2c, raw_status);
            }
            else if (IS_I2C_MASTER_RX_SLAVE_TX(raw_status))
            {
                _i2c_ext_slave_tx_it_handler(hi2c, raw_status);
            }
        }
        else if (hi2c->init.addressing_mode == I2C_ADDRESSINGMODE_10BIT)
        {
            if (HAL_FLAG_HAS(raw_status, I2C_STATUS_START))
            {
                /* Clear flag */
                HAL_REG_WRITE(reg->STATUS, I2C_STATUS_START);
                /* Clear NACK */
                HAL_REG_CBIT(reg->CTRL, I2C_CTRL_MASK_ACKNACK);

                switch (hi2c->xfer_state)
                {
                    case I2C_XFER_STATE_10BITS_ADDR_L: // Start after Hit
                    {
                        if (hi2c->direction == I2C_DIRECTION_TX)
                        {
                            hi2c->xfer_state = I2C_XFER_STATE_10BITS_RESTART; // Wait for Restart
                            /* Revert DATA */
                            if (hi2c->ext_xfer.tx_buffer_ptr != NULL)
                            {
                                if (hi2c->ext_xfer.tx_xfer_num > 0U)
                                {
                                    hi2c->ext_xfer.tx_buffer_ptr--;
                                    hi2c->ext_xfer.tx_xfer_count++;
                                    hi2c->ext_xfer.tx_xfer_num--;
                                }
                            }
                        }
                        else if (hi2c->direction == I2C_DIRECTION_RX)
                        {
                            hi2c->xfer_state = I2C_XFER_STATE_10BITS_ADDR_H; // Wait for Hit
                        }
                        hi2c->direction = I2C_DIRECTION_NONE;
                    }
                    break;

                    case I2C_XFER_STATE_DATA:
                    case I2C_XFER_STATE_STOP:
                    {
                        _i2c_slave_cplt_callback(hi2c);
                    }
                    case I2C_XFER_STATE_COMPLETED:
                    {
                        hi2c->xfer_state = I2C_XFER_STATE_10BITS_ADDR_H;
                    }
                    break;

                    default:
                        break;
                }

                return;
            }

            if (hi2c->xfer_state == I2C_XFER_STATE_10BITS_ADDR_H)
            {
                if (HAL_FLAG_HAS(raw_status, I2C_STATUS_TRANSFER_DONE))
                {
                    /* Clear flag */
                    HAL_REG_WRITE(reg->STATUS, I2C_STATUS_TRANSFER_DONE);
                }
                if (HAL_FLAG_HAS(raw_status, I2C_STATUS_HIT)) // First Hit
                {
                    /* Clear flag */
                    HAL_REG_WRITE(reg->STATUS, I2C_STATUS_HIT);

                    hi2c->xfer_state = I2C_XFER_STATE_10BITS_ADDR_L;

                    if (IS_I2C_MASTER_RX_SLAVE_TX(raw_status))
                    {
                        hi2c->direction = I2C_DIRECTION_TX;
                        /* Preset DATA */
                        if (hi2c->ext_xfer.tx_buffer_ptr != NULL)
                        {
                            if (hi2c->ext_xfer.tx_xfer_count >= 1U)
                            {
                                /* Transmit byte */
                                HAL_REG_WRITE(reg->DATA, *(hi2c->ext_xfer.tx_buffer_ptr));
                                hi2c->ext_xfer.tx_buffer_ptr++;
                                hi2c->ext_xfer.tx_xfer_count--;
                                hi2c->ext_xfer.tx_xfer_num++;
                            }
                        }
                    }
                    else if (IS_I2C_MASTER_TX_SLAVE_RX(raw_status))
                    {
                        hi2c->direction = I2C_DIRECTION_RX;
                        /* Preset NACK */
                        if (hi2c->ext_xfer.rx_buffer_ptr != NULL)
                        {
                            if (hi2c->ext_xfer.rx_xfer_count <= 1U)
                            {
                                /* Send NACK */
                                HAL_REG_SBIT(hi2c->instance->CTRL, I2C_CTRL_MASK_ACKNACK);
                            }
                        }
                        else
                        {
                            /* Send NACK */
                            HAL_REG_SBIT(hi2c->instance->CTRL, I2C_CTRL_MASK_ACKNACK);
                        }
                    }
                    /* Enable transfer byte */
                    HAL_REG_SBIT(reg->CTRL, I2C_CTRL_MASK_TBEN);
                }
            }
            else if (hi2c->xfer_state == I2C_XFER_STATE_10BITS_ADDR_L)
            {
                if (HAL_FLAG_HAS(raw_status, I2C_STATUS_TRANSFER_DONE)) // TD after Hit
                {
                    hi2c->xfer_state = I2C_XFER_STATE_DATA;

                    if (IS_I2C_MASTER_RX_SLAVE_TX(raw_status))
                    {
                        _i2c_ext_slave_tx_it_handler(hi2c, raw_status);
                    }
                    else if (IS_I2C_MASTER_TX_SLAVE_RX(raw_status))
                    {
                        _i2c_ext_slave_rx_it_handler(hi2c, raw_status);
                    }
                }
            }
            else if (hi2c->xfer_state == I2C_XFER_STATE_10BITS_RESTART)
            {
                if (HAL_FLAG_HAS(raw_status, I2C_STATUS_TRANSFER_DONE))
                {
                    /* Clear flag */
                    HAL_REG_WRITE(reg->STATUS, I2C_STATUS_TRANSFER_DONE);
                }
                if (HAL_FLAG_HAS(raw_status, I2C_STATUS_HIT)) // Second Hit
                {
                    /* Set transfer state for TX handler */
                    hi2c->xfer_state = I2C_XFER_STATE_10BITS_ADDR_L;

                    _i2c_ext_slave_tx_it_handler(hi2c, raw_status);
                }
            }
            else if (hi2c->xfer_state >= I2C_XFER_STATE_DATA)
            {
                if (IS_I2C_MASTER_RX_SLAVE_TX(raw_status))
                {
                    _i2c_ext_slave_tx_it_handler(hi2c, raw_status);
                }
                else if (IS_I2C_MASTER_TX_SLAVE_RX(raw_status))
                {
                    _i2c_ext_slave_rx_it_handler(hi2c, raw_status);
                }
            }
        }
    }
}
#endif /* IT_OR_BURST */

/**
 * @brief  I2C Master Tx Transfer Completed Callback.
 * @param  hi2c Pointer to a I2C_Handle_T structure that contains the configuration information for the specified I2C.
 * @retval None
 */
__weak ATTR_ISR void HAL_I2C_MasterTxCpltCallback(I2C_Handle_T *hi2c)
{
    /* Prevent unused argument(s) compilation warning */
    UNUSED(hi2c);
    /* NOTE: This function should not be modified, when the callback is needed, the HAL_I2C_MasterTxCpltCallback could be implemented in the user file */
}

/**
 * @brief  I2C Master Rx Transfer Completed Callback.
 * @param  hi2c Pointer to a I2C_Handle_T structure that contains the configuration information for the specified I2C.
 * @retval None
 */
__weak ATTR_ISR void HAL_I2C_MasterRxCpltCallback(I2C_Handle_T *hi2c)
{
    /* Prevent unused argument(s) compilation warning */
    UNUSED(hi2c);
    /* NOTE: This function should not be modified, when the callback is needed, the HAL_I2C_MasterRxCpltCallback could be implemented in the user file */
}

/**
 * @brief  I2C Slave Tx Transfer Completed Callback.
 * @param  hi2c Pointer to a I2C_Handle_T structure that contains the configuration information for the specified I2C.
 * @retval None
 */
__weak ATTR_ISR void HAL_I2C_SlaveTxCpltCallback(I2C_Handle_T *hi2c)
{
    /* Prevent unused argument(s) compilation warning */
    UNUSED(hi2c);
    /* NOTE: This function should not be modified, when the callback is needed, the HAL_I2C_SlaveTxCpltCallback could be implemented in the user file */
}

/**
 * @brief  I2C Slave Rx Transfer Completed Callback.
 * @param  hi2c Pointer to a I2C_Handle_T structure that contains the configuration information for the specified I2C.
 * @retval None
 */
__weak ATTR_ISR void HAL_I2C_SlaveRxCpltCallback(I2C_Handle_T *hi2c)
{
    /* Prevent unused argument(s) compilation warning */
    UNUSED(hi2c);
    /* NOTE: This function should not be modified, when the callback is needed, the HAL_I2C_SlaveRxCpltCallback could be implemented in the user file */
}

/**
 * @brief  I2C Abort Completed Callback.
 * @param  hi2c Pointer to a I2C_Handle_T structure that contains the configuration information for the specified I2C .
 * @retval None
 */
__weak ATTR_ISR void HAL_I2C_AbortCpltCallback(I2C_Handle_T *hi2c)
{
    /* Prevent unused argument(s) compilation warning */
    UNUSED(hi2c);
    /* NOTE: This function should not be modified, when the callback is needed, the HAL_I2C_AbortCpltCallback could be implemented in the user file */
}

/**
 * @brief  I2C Error Callback.
 * @param  hi2c Pointer to a I2C_Handle_T structure that contains the configuration information for the specified I2C.
 * @retval None
 */
__weak ATTR_ISR void HAL_I2C_ErrorCallback(I2C_Handle_T *hi2c)
{
    /* Prevent unused argument(s) compilation warning */
    UNUSED(hi2c);
    /* NOTE: This function should not be modified, when the callback is needed, the HAL_I2C_ErrorCallback could be implemented in the user file */
}

/* Private functions ---------------------------------------------------------*/
static HAL_Status_T _i2c_set_clock_speed(I2C_Handle_T *hi2c)
{
    uint32_t clock_list[] = {100000UL, 400000UL, 1000000UL, 3000000UL};
    uint32_t apb1_clk     = HAL_RCC_GetAPB1CLKFreq();
    uint32_t divisor      = apb1_clk / clock_list[hi2c->init.clock_speed];
    uint32_t count        = (divisor - 6) / 2; // SCL = PCLK / ( 2 * COUNT + GSR + 4)

    if (hi2c->init.clock_speed < I2C_CLOCKSPEED_HIGH_SPEED) // FS mode
    {
        /* COUNT >= 4 + GSR(2) + TSR(1) */
        if (count < 7 || count > 1048575)
        {
            hi2c->error_code = HAL_I2C_ERROR_CLK;
            return HAL_ERROR;
        }
        HAL_REG_WRITE(hi2c->instance->CLKDIV_b.COUNT, count);
    }
    else // HS mode
    {
        /* COUNT >= 4 + GSR(2) + TSR(1) */
        if (count < 7 || count > 255)
        {
            hi2c->error_code = HAL_I2C_ERROR_CLK;
            return HAL_ERROR;
        }
        HAL_REG_WRITE(hi2c->instance->CLKDIV_b.COUNTH, count);

        /* Set COUNT with 100K */
        divisor = apb1_clk / 100000UL;
        count   = (divisor - 6) / 2;
        HAL_REG_WRITE(hi2c->instance->CLKDIV_b.COUNT, count);
    }

    HAL_REG_WRITE(hi2c->instance->TGS_b.TSR, 1);
    HAL_REG_WRITE(hi2c->instance->TGS_b.GSR, 2);
    return HAL_OK;
}

#if (configUSE_I2C_REGISTER_CALLBACKS == 1)
/**
 * @brief  Initialize the callbacks to their default values.
 * @param  hi2c Pointer to a I2C_Handle_T structure that contains the configuration information for the specified I2C.
 * @retval None
 */
static void _i2c_init_callbacks_to_default(I2C_Handle_T *hi2c)
{
    hi2c->MasterTxCpltCallback = HAL_I2C_MasterTxCpltCallback;
    hi2c->MasterRxCpltCallback = HAL_I2C_MasterRxCpltCallback;
    hi2c->SlaveTxCpltCallback  = HAL_I2C_SlaveTxCpltCallback;
    hi2c->SlaveRxCpltCallback  = HAL_I2C_SlaveRxCpltCallback;
    hi2c->AbortCpltCallback    = HAL_I2C_AbortCpltCallback;
    hi2c->ErrorCallback        = HAL_I2C_ErrorCallback;
}
#endif /* configUSE_I2C_REGISTER_CALLBACKS */

#if (configUSE_I2C_BLOCKING == 1)
static HAL_Status_T _i2c_wait_for_flag_until_timeout_target_first(I2C_Handle_T *hi2c, I2C_STATUS_T target_status, I2C_STATUS_T error_status, uint32_t tick_start, uint32_t timeout)
{
    SN_I2C0_Type *reg    = hi2c->instance;
    uint32_t      status = HAL_REG_READ(reg->STATUS);
    while (true)
    {
        /* Check target status */
        if ((status & target_status) == target_status)
        {
            /* Clear flag */
            HAL_REG_WRITE(reg->STATUS, target_status);
            return HAL_OK;
        }
        /* Check error status */
        if (error_status != I2C_STATUS_NONE)
        {
            if ((status & error_status) == error_status)
            {
                /* Clear flag */
                HAL_REG_WRITE(reg->STATUS, error_status);
                hi2c->error_code = HAL_I2C_ERROR_UNEXPECTED_STATUS;
                return HAL_ERROR;
            }
        }
        /* Check timeout */
        BREAK_PROC_TIMEOUT(hi2c, tick_start, timeout);
        /* Update status */
        status = HAL_REG_READ(reg->STATUS);
    }

    return HAL_TIMEOUT;
}

static HAL_Status_T _i2c_wait_for_flag_until_timeout_error_first(I2C_Handle_T *hi2c, I2C_STATUS_T target_status, I2C_STATUS_T error_status, uint32_t tick_start, uint32_t timeout, bool tb_en)
{
    SN_I2C0_Type *reg    = hi2c->instance;
    uint32_t      status = HAL_REG_READ(reg->STATUS);
    while (true)
    {
        /* Check error status */
        if (error_status != I2C_STATUS_NONE)
        {
            if ((status & error_status) == error_status)
            {
                /* Clear flag */
                HAL_REG_WRITE(reg->STATUS, error_status);
                hi2c->error_code = HAL_I2C_ERROR_UNEXPECTED_STATUS;
                return HAL_ERROR;
            }
        }
        /* Check target status */
        if ((status & target_status) == target_status)
        {
            /* Clear flag */
            HAL_REG_WRITE(reg->STATUS, target_status);
            return HAL_OK;
        }
        if (tb_en)
        {
            /* Enable transfer byte */
            HAL_REG_SBIT(reg->CTRL, I2C_CTRL_MASK_TBEN);
        }
        /* Check timeout */
        BREAK_PROC_TIMEOUT(hi2c, tick_start, timeout);
        /* Update status */
        status = HAL_REG_READ(reg->STATUS);
    }

    return HAL_TIMEOUT;
}
#endif /* configUSE_I2C_BLOCKING */

#if (configUSE_I2C_NONBLOCKING_IT == 1)
static HAL_Status_T _i2c_master_transfer_it(I2C_Handle_T *hi2c, I2C_DIRECTION_T direction, uint16_t dev_address, uint8_t *data, uint16_t size, uint32_t xfer_option)
{
    HAL_Status_T status = HAL_OK;

    /* Check the I2C handle */
    ASSERT_PARAM(hi2c);
    /* Check the I2C instance */
    ASSERT_PARAM(IS_PERIPHERAL_I2C_INSTANCE(hi2c));
    /* Check the parameters */
    ASSERT_PARAM(data);
    ASSERT_PARAM(size);

    if (hi2c->state == HAL_STATE_READY)
    {
        /* Check transfer option */
        status = _i2c_check_transfer_option(hi2c, direction, xfer_option);
        RET_FLAG_FALSE(status == HAL_OK, status);

        /* Protect the process */
        TAKE_MUTEX(hi2c);

        /* Update the I2C state */
        hi2c->state = HAL_STATE_BUSY;
        hi2c->mode  = HAL_I2C_MODE_MASTER;
        CLEAR_ERROR_CODE(hi2c);

        uint32_t prev_xfer_option = hi2c->xfer_option;
        uint32_t prev_direction   = hi2c->direction;

        /* If ACK was sent in the previous round and a restart will be sent in this round, which is not supported */
        if ((prev_direction == I2C_DIRECTION_RX) && (prev_xfer_option != I2C_NO_OPTION_FRAME))
        {
            /* Previous transfer: RX without stop */
            if (HAL_FLAG_LACKS(prev_xfer_option, I2C_OPTION_NACK_FLAG) && HAL_FLAG_HAS(xfer_option, I2C_OPTION_START_FLAG))
            {
                /* Previous transfer: ACK, current transfer: restart */
                return HAL_ERROR;
            }
        }

        /* Set runtime info */
        hi2c->buffer_ptr  = data;
        hi2c->xfer_size   = size;
        hi2c->xfer_count  = size;
        hi2c->direction   = direction;
        hi2c->dev_address = dev_address;
        hi2c->xfer_option = xfer_option;
        hi2c->xfer_state  = I2C_XFER_STATE_START;

        SN_I2C0_Type *reg = hi2c->instance;
        /* Clear I2C status */
        I2C_CLEAR_ALL_STATUS(reg);
        /* Check speed */
        uint32_t i2c_status = HAL_REG_READ(reg->STATUS);
        if (HAL_FLAG_LACKS(i2c_status, I2C_STATUS_BUSY))
        {
            if (hi2c->init.clock_speed >= I2C_CLOCKSPEED_HIGH_SPEED)
            {
                hi2c->xfer_state = I2C_XFER_STATE_HS_CTRL;
            }
        }
        CHECK_STATUS_IN_TRANSFER(status);

        /* Enable the peripheral */
        /* Set master mode */
        /* Enable interrupt: TD & NACK & STOP & AL*/
        uint32_t ctrl_mask = I2C_CTRL_MASK_I2CEN | I2C_CTRL_MASK_MSTEN | I2C_CTRL_MASK_TDIEN | I2C_CTRL_MASK_NACKIEN | I2C_CTRL_MASK_STOPIEN | I2C_CTRL_MASK_ALIEN;

        if (hi2c->xfer_state == I2C_XFER_STATE_HS_CTRL) // Send HS code
        {
            /* Send HS code */
            HAL_REG_WRITE(reg->DATA, I2C_HS_CTRL_CODE);
            /* Enable interrupt: Start */
            /* Set start condition */
            ctrl_mask |= I2C_CTRL_MASK_STARTIEN | I2C_CTRL_MASK_START;
        }
        else
        {
            if (hi2c->xfer_option == I2C_NO_OPTION_FRAME || HAL_FLAG_HAS(hi2c->xfer_option, I2C_OPTION_START_FLAG)) // With start/restart condition
            {
                /* Send slave address */
                uint32_t addr = 0;
                if (hi2c->init.addressing_mode == I2C_ADDRESSINGMODE_7BIT) // 7-bit addressing mode
                {
                    hi2c->xfer_state = I2C_XFER_STATE_7BITS_ADDR;
                    addr             = I2C_7BITS_ADDR(hi2c->dev_address);
                    if (hi2c->direction == I2C_DIRECTION_RX)
                    {
                        addr |= I2C_READ_FLAG;
                    }
                }
                else // 10-bit addressing mode
                {
                    hi2c->xfer_state = I2C_XFER_STATE_10BITS_ADDR_H;
                    addr             = I2C_10BITS_ADDR_HIGH(hi2c->dev_address);
                    /* Check 10-bit Restart */
                    if (HAL_FLAG_HAS(i2c_status, I2C_STATUS_BUSY))
                    {
                        /* Change direction, then only send high address(restart) */
                        if (prev_direction == I2C_DIRECTION_TX && hi2c->direction == I2C_DIRECTION_RX)
                        {
                            addr |= I2C_READ_FLAG;
                            hi2c->xfer_state = I2C_XFER_STATE_10BITS_RESTART;
                        }
                    }
                }
                HAL_REG_WRITE(reg->DATA, addr);
                /* Enable interrupt: Start */
                /* Set start condition */
                ctrl_mask |= I2C_CTRL_MASK_STARTIEN | I2C_CTRL_MASK_START;
            }
            else // No start condition
            {
                /* Update transfer state */
                hi2c->xfer_state = I2C_XFER_STATE_DATA;
                /* Transfer data */
                if (hi2c->direction == I2C_DIRECTION_TX)
                {
                    /* Transmit byte */
                    HAL_REG_WRITE(reg->DATA, *(hi2c->buffer_ptr));
                    hi2c->buffer_ptr++;
                    hi2c->xfer_count--;

                    if (hi2c->xfer_count == 0U)
                    {
                        /* Set stop condition */
                        _i2c_master_stop_condition(hi2c);
                    }
                }
                else if (hi2c->direction == I2C_DIRECTION_RX)
                {
                    if (hi2c->xfer_count == 1)
                    {
                        _i2c_nack_condition(hi2c);
                    }
                }
                /* Enable transfer byte */
                ctrl_mask |= I2C_CTRL_MASK_TBEN;
            }
        }
        /* Set Control Register */
        HAL_REG_SBIT(reg->CTRL, ctrl_mask);
        /* Release the process protection */
        GIVE_MUTEX(hi2c);
    }
    else if (hi2c->state == HAL_STATE_BUSY)
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

static HAL_Status_T _i2c_slave_transfer_it(I2C_Handle_T *hi2c, I2C_DIRECTION_T direction, uint8_t *data, uint16_t size, uint32_t xfer_option)
{
    HAL_Status_T status = HAL_OK;

    /* Check the I2C handle */
    ASSERT_PARAM(hi2c);
    /* Check the I2C instance */
    ASSERT_PARAM(IS_PERIPHERAL_I2C_INSTANCE(hi2c));
    /* Check the parameters */
    ASSERT_PARAM(data);
    ASSERT_PARAM(size);

    if (hi2c->state == HAL_STATE_READY)
    {
        /* Check transfer option */
        status = _i2c_check_transfer_option(hi2c, direction, xfer_option);
        RET_FLAG_FALSE(status == HAL_OK, status);

        /* Protect the process */
        TAKE_MUTEX(hi2c);

        /* Update the I2C state */
        hi2c->state = HAL_STATE_BUSY;
        hi2c->mode  = HAL_I2C_MODE_SLAVE;
        CLEAR_ERROR_CODE(hi2c);

        uint32_t prev_xfer_option = hi2c->xfer_option;
        uint32_t prev_direction   = hi2c->direction;

        /* Set runtime info */
        hi2c->buffer_ptr  = data;
        hi2c->xfer_size   = size;
        hi2c->xfer_count  = size;
        hi2c->direction   = direction;
        hi2c->xfer_option = xfer_option;
        hi2c->xfer_state  = I2C_XFER_STATE_START;

        SN_I2C0_Type *reg = hi2c->instance;
        /* Clear I2C status */
        I2C_CLEAR_ALL_STATUS(reg);
        /* Set slave mode */
        HAL_REG_CBIT(reg->CTRL, I2C_CTRL_MASK_MSTEN);
        /* Set slave address */
        HAL_REG_WRITE(reg->ADDR_b.ADDR, hi2c->own_address);
        /* Enable the peripheral */
        /* Enable interrupt: TD & STOP | NACK */
        uint32_t ctrl_mask = I2C_CTRL_MASK_I2CEN | I2C_CTRL_MASK_TDIEN | I2C_CTRL_MASK_STOPIEN;
        if (hi2c->direction == I2C_DIRECTION_TX)
        {
            ctrl_mask |= I2C_CTRL_MASK_NACKIEN;
        }

        if (hi2c->xfer_option == I2C_NO_OPTION_FRAME || HAL_FLAG_HAS(hi2c->xfer_option, I2C_OPTION_START_FLAG)) // With start/restart condition
        {
            /* Check addressing mode */
            if (hi2c->init.addressing_mode == I2C_ADDRESSINGMODE_7BIT) // 7-bit addressing mode
            {
                hi2c->xfer_state = I2C_XFER_STATE_7BITS_ADDR;
            }
            else // 10-bit addressing mode
            {
                hi2c->xfer_state = I2C_XFER_STATE_10BITS_ADDR_H;
                /* Check 10-bit Restart */
                uint32_t i2c_status = HAL_REG_READ(reg->STATUS);
                if (HAL_FLAG_HAS(i2c_status, I2C_STATUS_BUSY))
                {
                    /* Change direction, then only send high address(restart) */
                    if (prev_direction == I2C_DIRECTION_RX && hi2c->direction == I2C_DIRECTION_TX)
                    {
                        hi2c->xfer_state = I2C_XFER_STATE_10BITS_ADDR_L;
                    }
                }
            }
            /* Enable interrupt: SAM */
            ctrl_mask |= I2C_CTRL_MASK_SAMIEN;
        }
        else // No start condition
        {
            hi2c->xfer_state = I2C_XFER_STATE_DATA;
            /* Transfer data */
            if (hi2c->direction == I2C_DIRECTION_TX)
            {
                /* Transmit byte */
                HAL_REG_WRITE(reg->DATA, *(hi2c->buffer_ptr));
                hi2c->buffer_ptr++;
                hi2c->xfer_count--;
            }
            else if (hi2c->direction == I2C_DIRECTION_RX)
            {
                if (hi2c->xfer_count == 1)
                {
                    _i2c_nack_condition(hi2c);
                }
            }
        }
        /* Enable transfer byte */
        ctrl_mask |= I2C_CTRL_MASK_TBEN;
        /* Set Control Register */
        HAL_REG_SBIT(reg->CTRL, ctrl_mask);
        /* Release the process protection */
        GIVE_MUTEX(hi2c);
    }
    else if (hi2c->state == HAL_STATE_BUSY)
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

static HAL_Status_T _i2c_check_transfer_option(I2C_Handle_T *hi2c, uint32_t direction, uint32_t xfer_option)
{
    HAL_Status_T status = HAL_OK;
    if (xfer_option != I2C_NO_OPTION_FRAME)
    {
        if (hi2c->xfer_option == I2C_NO_OPTION_FRAME)
        {
            /* New transfer */
            if (HAL_FLAG_LACKS(xfer_option, I2C_OPTION_START_FLAG))
            {
                /* No start/restart condition, unexpected */
                status = HAL_ERROR;
            }
        }
        else
        {
            /* Not new transfer */
            if (direction != hi2c->direction)
            {
                /* Direction change */
                if (HAL_FLAG_LACKS(xfer_option, I2C_OPTION_START_FLAG))
                {
                    /* No start/restart condition, unexpected */
                    status = HAL_ERROR;
                }
            }
            else
            {
                /* No direction change */
                if (HAL_FLAG_LACKS(xfer_option, I2C_OPTION_START_FLAG))
                {
                    /* No start/restart condition */
                    if (HAL_FLAG_HAS(hi2c->xfer_option, I2C_OPTION_NACK_FLAG))
                    {
                        /* No ACK in previous transfer */
                        status = HAL_ERROR;
                    }
                }
            }
        }
    }

    return status;
}

static void _i2c_master_tx_it_handler(I2C_Handle_T *hi2c, uint32_t raw_status)
{
    SN_I2C0_Type *reg = hi2c->instance;
    /* Check I2C Status */
    if (HAL_FLAG_HAS(raw_status, I2C_STATUS_START))
    {
        /* Clear flag */
        HAL_REG_WRITE(reg->STATUS, I2C_STATUS_START);

        switch (hi2c->xfer_state)
        {
            case I2C_XFER_STATE_7BITS_ADDR:
            case I2C_XFER_STATE_10BITS_RESTART:
            {
                hi2c->xfer_state = I2C_XFER_STATE_DATA;
            }
            case I2C_XFER_STATE_10BITS_ADDR_H:
                break;

            default:
                break;
        }
    }
    if (HAL_FLAG_HAS(raw_status, I2C_STATUS_NACK))
    {
        /* Clear flag */
        HAL_REG_WRITE(reg->STATUS, I2C_STATUS_NACK);

        if (hi2c->xfer_state == I2C_XFER_STATE_HS_CTRL)
        {
            uint32_t addr = 0;
            if (hi2c->init.addressing_mode == I2C_ADDRESSINGMODE_7BIT) // 7-bit addressing mode
            {
                addr             = I2C_7BITS_ADDR(hi2c->dev_address);
                hi2c->xfer_state = I2C_XFER_STATE_7BITS_ADDR;
            }
            else // 10-bit addressing mode
            {
                addr             = I2C_10BITS_ADDR_HIGH(hi2c->dev_address);
                hi2c->xfer_state = I2C_XFER_STATE_10BITS_ADDR_H;
            }

            /* Send slave address */
            HAL_REG_WRITE(reg->DATA, addr);
            /* Enable HS mode */
            /* Set start condition */
            HAL_REG_SBIT(reg->CTRL, I2C_CTRL_MASK_HSMODE | I2C_CTRL_MASK_START);
            /* Clear TD flag */
            HAL_REG_WRITE(reg->STATUS, I2C_STATUS_TRANSFER_DONE);
            return;
        }
        else if (hi2c->xfer_state < I2C_XFER_STATE_DATA)
        {
            /* NACK after address */
            hi2c->xfer_state = I2C_XFER_STATE_STOP;
            HAL_REG_SBIT(reg->CTRL, I2C_CTRL_MASK_STOP);
        }
        else if (hi2c->xfer_state == I2C_XFER_STATE_DATA)
        {
            /* Set stop condition */
            _i2c_master_stop_condition(hi2c);
        }
    }
    if (HAL_FLAG_HAS(raw_status, I2C_STATUS_TRANSFER_DONE))
    {
        /* Clear flag */
        HAL_REG_WRITE(reg->STATUS, I2C_STATUS_TRANSFER_DONE);

        switch (hi2c->xfer_state)
        {
            case I2C_XFER_STATE_10BITS_ADDR_H:
            {
                hi2c->xfer_state = I2C_XFER_STATE_10BITS_ADDR_L;
                /* Send 10-bit low address */
                HAL_REG_WRITE(reg->DATA, I2C_10BITS_ADDR_LOW(hi2c->dev_address));
                /* Enable transfer byte */
                HAL_REG_SBIT(reg->CTRL, I2C_CTRL_MASK_TBEN);
            }
            break;

            case I2C_XFER_STATE_10BITS_ADDR_L:
            {
                hi2c->xfer_state = I2C_XFER_STATE_DATA;
            }
            case I2C_XFER_STATE_DATA:
            {
                if (hi2c->abort == 1)
                {
                    hi2c->xfer_state = I2C_XFER_STATE_STOP;
                    HAL_REG_WRITE(reg->DATA, 0);
                    HAL_REG_SBIT(reg->CTRL, I2C_CTRL_MASK_STOP);
                    break;
                }

                if (hi2c->xfer_count > 0U)
                {
                    /* Transmit byte */
                    HAL_REG_WRITE(reg->DATA, *(hi2c->buffer_ptr));
                    hi2c->buffer_ptr++;
                    hi2c->xfer_count--;
                    /* Enable transfer byte */
                    HAL_REG_SBIT(reg->CTRL, I2C_CTRL_MASK_TBEN);

                    if (hi2c->xfer_count == 0U)
                    {
                        /* Set stop condition */
                        _i2c_master_stop_condition(hi2c);
                    }
                }
            }
            break;

            case I2C_XFER_STATE_STOP:
            {
                _i2c_update_stop_state(hi2c);
            }
            break;

            default:
                break;
        }
    }
    if (HAL_FLAG_HAS(raw_status, I2C_STATUS_STOP))
    {
        /* Clear flag */
        HAL_REG_WRITE(reg->STATUS, I2C_STATUS_STOP);
        /* Disable HS Mode */
        /* Disable the peripheral */
        HAL_REG_CBIT(reg->CTRL, I2C_CTRL_MASK_HSMODE | I2C_CTRL_MASK_I2CEN);
        /* With stop condition */
        hi2c->xfer_state  = I2C_XFER_STATE_COMPLETED;
        hi2c->xfer_option = I2C_NO_OPTION_FRAME;
    }

    /* Call completed handler */
    if (hi2c->xfer_state == I2C_XFER_STATE_COMPLETED)
    {
        _i2c_master_completion_handler(hi2c);
    }
}

static void _i2c_master_rx_it_handler(I2C_Handle_T *hi2c, uint32_t raw_status)
{
    SN_I2C0_Type *reg = hi2c->instance;
    /* Check I2C Status */
    if (HAL_FLAG_HAS(raw_status, I2C_STATUS_START))
    {
        /* Clear flag */
        HAL_REG_WRITE(reg->STATUS, I2C_STATUS_START);

        switch (hi2c->xfer_state)
        {
            case I2C_XFER_STATE_10BITS_RESTART:
            {
                hi2c->xfer_state = I2C_XFER_STATE_10BITS_READ;
            }
            case I2C_XFER_STATE_7BITS_ADDR:
            case I2C_XFER_STATE_10BITS_ADDR_H:
                break;

            default:
                break;
        }
    }
    if (HAL_FLAG_HAS(raw_status, I2C_STATUS_NACK))
    {
        /* Clear flag */
        HAL_REG_WRITE(reg->STATUS, I2C_STATUS_NACK);

        if (hi2c->xfer_state == I2C_XFER_STATE_HS_CTRL)
        {
            uint32_t addr = 0;
            if (hi2c->init.addressing_mode == I2C_ADDRESSINGMODE_7BIT) // 7-bit addressing mode
            {
                addr             = I2C_7BITS_ADDR(hi2c->dev_address) | I2C_READ_FLAG;
                hi2c->xfer_state = I2C_XFER_STATE_7BITS_ADDR;
            }
            else // 10-bit addressing mode
            {
                addr             = I2C_10BITS_ADDR_HIGH(hi2c->dev_address);
                hi2c->xfer_state = I2C_XFER_STATE_10BITS_ADDR_H;
            }

            /* Send slave address */
            HAL_REG_WRITE(reg->DATA, addr);
            /* Enable HS mode */
            /* Set start condition */
            HAL_REG_SBIT(reg->CTRL, I2C_CTRL_MASK_HSMODE | I2C_CTRL_MASK_START);
            /* Clear TD flag */
            HAL_REG_WRITE(reg->STATUS, I2C_STATUS_TRANSFER_DONE);
            return;
        }
        else if (hi2c->xfer_state < I2C_XFER_STATE_DATA)
        {
            /* NACK after address */
            hi2c->xfer_state = I2C_XFER_STATE_STOP;
            HAL_REG_SBIT(reg->CTRL, I2C_CTRL_MASK_STOP);
        }
    }
    if (HAL_FLAG_HAS(raw_status, I2C_STATUS_TRANSFER_DONE))
    {
        /* Clear flag */
        HAL_REG_WRITE(reg->STATUS, I2C_STATUS_TRANSFER_DONE);

        switch (hi2c->xfer_state)
        {
            case I2C_XFER_STATE_10BITS_ADDR_H:
            {
                hi2c->xfer_state = I2C_XFER_STATE_10BITS_ADDR_L;
                /* Send 10-bit low address */
                HAL_REG_WRITE(reg->DATA, I2C_10BITS_ADDR_LOW(hi2c->dev_address));
                /* Enable transfer byte */
                HAL_REG_SBIT(reg->CTRL, I2C_CTRL_MASK_TBEN);
            }
            break;

            case I2C_XFER_STATE_10BITS_ADDR_L:
            {
                hi2c->xfer_state = I2C_XFER_STATE_10BITS_RESTART;
                /* Send 10-bit high address with reading flag */
                HAL_REG_WRITE(reg->DATA, I2C_10BITS_ADDR_HIGH(hi2c->dev_address) | I2C_READ_FLAG);
                /* Set 2nd start condition */
                HAL_REG_SBIT(reg->CTRL, I2C_CTRL_MASK_START);
            }
            break;

            case I2C_XFER_STATE_7BITS_ADDR:
            case I2C_XFER_STATE_10BITS_READ:
            {
                hi2c->xfer_state = I2C_XFER_STATE_DATA;
                /* Enable transfer byte */
                HAL_REG_SBIT(reg->CTRL, I2C_CTRL_MASK_TBEN);

                if (hi2c->xfer_count == 1U) // The next one is the last transaction of data
                {
                    /* Send NACK */
                    _i2c_nack_condition(hi2c);
                }
            }
            break;

            case I2C_XFER_STATE_DATA:
            {
                if (hi2c->abort == 1)
                {
                    hi2c->xfer_state = I2C_XFER_STATE_STOP;
                    HAL_REG_SBIT(reg->CTRL, I2C_CTRL_MASK_ACKNACK | I2C_CTRL_MASK_STOP);
                    break;
                }

                if (hi2c->xfer_count > 0U)
                {
                    /* Receive byte */
                    *(hi2c->buffer_ptr) = HAL_REG_READ(reg->DATA);
                    hi2c->buffer_ptr++;
                    hi2c->xfer_count--;

                    if (hi2c->xfer_count > 0U)
                    {
                        /* Enable transfer byte */
                        HAL_REG_SBIT(reg->CTRL, I2C_CTRL_MASK_TBEN);
                        if (hi2c->xfer_count == 1U) // The next one is the last transaction of data
                        {
                            /* Send NACK */
                            _i2c_nack_condition(hi2c);
                        }
                    }
                    else
                    {
                        /* Set stop condition */
                        _i2c_master_stop_condition(hi2c);
                        _i2c_update_stop_state(hi2c);
                    }
                }
            }
            break;

            default:
                break;
        }
    }
    if (HAL_FLAG_HAS(raw_status, I2C_STATUS_STOP))
    {
        /* Clear flag */
        HAL_REG_WRITE(reg->STATUS, I2C_STATUS_STOP);
        /* Disable HS Mode */
        /* Disable the peripheral */
        HAL_REG_CBIT(reg->CTRL, I2C_CTRL_MASK_HSMODE | I2C_CTRL_MASK_I2CEN);
        /* With stop condition */
        hi2c->xfer_state  = I2C_XFER_STATE_COMPLETED;
        hi2c->xfer_option = I2C_NO_OPTION_FRAME;
    }

    /* Call completed handler */
    if (hi2c->xfer_state == I2C_XFER_STATE_COMPLETED)
    {
        _i2c_master_completion_handler(hi2c);
    }
}

static void _i2c_slave_tx_it_handler(I2C_Handle_T *hi2c, uint32_t raw_status)
{
    SN_I2C0_Type *reg = hi2c->instance;
    /* Check I2C Status */
    if (HAL_FLAG_HAS(raw_status, I2C_STATUS_HIT))
    {
        /* Clear flag */
        HAL_REG_WRITE(reg->STATUS, I2C_STATUS_START | I2C_STATUS_HIT | I2C_STATUS_TRANSFER_DONE);

        switch (hi2c->xfer_state)
        {
            case I2C_XFER_STATE_7BITS_ADDR:
            case I2C_XFER_STATE_10BITS_ADDR_L:
            {
                hi2c->xfer_state = I2C_XFER_STATE_DATA;
                /* Transmit byte */
                HAL_REG_WRITE(reg->DATA, *(hi2c->buffer_ptr));
                hi2c->buffer_ptr++;
                hi2c->xfer_count--;
            }
            break;

            case I2C_XFER_STATE_10BITS_ADDR_H:
            {
                hi2c->xfer_state = I2C_XFER_STATE_10BITS_ADDR_L;
            }
            break;

            case I2C_XFER_STATE_STOP: // Restart condition after NACK
            {
                if (hi2c->init.addressing_mode == I2C_ADDRESSINGMODE_7BIT)
                {
                    hi2c->xfer_state = I2C_XFER_STATE_DATA;
                    if (hi2c->xfer_count > 0U)
                    {
                        /* Transmit byte */
                        HAL_REG_WRITE(reg->DATA, *(hi2c->buffer_ptr));
                        hi2c->buffer_ptr++;
                        hi2c->xfer_count--;
                    }
                }
                else if (hi2c->init.addressing_mode == I2C_ADDRESSINGMODE_10BIT)
                {
                    hi2c->xfer_state = I2C_XFER_STATE_10BITS_ADDR_L;
                }
            }
            break;

            default:
                break;
        }
        /* Enable transfer byte */
        HAL_REG_SBIT(reg->CTRL, I2C_CTRL_MASK_TBEN);
        /* Wait for next interrupt */
        return;
    }
    if (HAL_FLAG_HAS(raw_status, I2C_STATUS_NACK))
    {
        /* Clear flag */
        HAL_REG_WRITE(reg->STATUS, I2C_STATUS_NACK);

        hi2c->xfer_state = I2C_XFER_STATE_STOP;
    }
    if (HAL_FLAG_HAS(raw_status, I2C_STATUS_TRANSFER_DONE))
    {
        /* Clear flag */
        HAL_REG_WRITE(reg->STATUS, I2C_STATUS_TRANSFER_DONE);

        if (HAL_FLAG_HAS(raw_status, I2C_STATUS_START)) // TD before Hit
        {
            HAL_REG_WRITE(reg->STATUS, I2C_STATUS_START);
            return;
        }

        switch (hi2c->xfer_state)
        {
            case I2C_XFER_STATE_10BITS_ADDR_L:
            {
                hi2c->xfer_state = I2C_XFER_STATE_DATA;
            }
            case I2C_XFER_STATE_DATA:
            {
                if (hi2c->xfer_count > 0U)
                {
                    /* Transmit byte */
                    HAL_REG_WRITE(reg->DATA, *(hi2c->buffer_ptr));
                    hi2c->buffer_ptr++;
                    hi2c->xfer_count--;
                    /* Enable transfer byte */
                    HAL_REG_SBIT(reg->CTRL, I2C_CTRL_MASK_TBEN);
                }
                else
                {
                    hi2c->xfer_state = I2C_XFER_STATE_STOP;

                    if (hi2c->xfer_option == I2C_NO_OPTION_FRAME) // Avoid occupying the bus when MRX size > STX size
                    {
                        /* Enable transfer byte */
                        HAL_REG_SBIT(reg->CTRL, I2C_CTRL_MASK_TBEN);
                    }

                    _i2c_update_stop_state(hi2c);
                }
            }
            break;

            case I2C_XFER_STATE_STOP:
            {
                if (hi2c->xfer_option == I2C_NO_OPTION_FRAME) // Avoid occupying the bus when MRX size > STX size
                {
                    /* Enable transfer byte */
                    HAL_REG_SBIT(reg->CTRL, I2C_CTRL_MASK_TBEN);
                }

                _i2c_update_stop_state(hi2c);
            }
            break;

            case I2C_XFER_STATE_COMPLETED:
            case I2C_XFER_STATE_10BITS_ADDR_H:
                break;

            default:
                break;
        }
    }
    if (HAL_FLAG_HAS(raw_status, I2C_STATUS_STOP))
    {
        /* Clear flag */
        HAL_REG_WRITE(reg->STATUS, I2C_STATUS_STOP);

        /* If the stop interrupt is triggered, slave can only be disabled when the the address confirmation has completed */
        if (hi2c->xfer_state < I2C_XFER_STATE_DATA)
        {
            return;
        }

        /* With stop condition */
        hi2c->xfer_state  = I2C_XFER_STATE_COMPLETED;
        hi2c->xfer_option = I2C_NO_OPTION_FRAME;
        /* Disable the peripheral */
        PERIPHERAL_I2C_DISABLE(hi2c);
    }

    /* Call completed handler */
    if (hi2c->xfer_state == I2C_XFER_STATE_COMPLETED)
    {
        _i2c_slave_completion_handler(hi2c);
    }
}

static void _i2c_slave_rx_it_handler(I2C_Handle_T *hi2c, uint32_t raw_status)
{
    SN_I2C0_Type *reg = hi2c->instance;
    /* Check I2C Status */
    if (HAL_FLAG_HAS(raw_status, I2C_STATUS_HIT))
    {
        /* Clear flag */
        HAL_REG_WRITE(reg->STATUS, I2C_STATUS_START | I2C_STATUS_HIT | I2C_STATUS_TRANSFER_DONE);

        hi2c->xfer_state = I2C_XFER_STATE_DATA;
        if (hi2c->xfer_count == 1U) // Only one transaction of data
        {
            /* Send NACK */
            _i2c_nack_condition(hi2c);
        }
        /* Enable transfer byte */
        HAL_REG_SBIT(reg->CTRL, I2C_CTRL_MASK_TBEN);
        /* Wait for next interrupt */
        return;
    }
    if (HAL_FLAG_HAS(raw_status, I2C_STATUS_TRANSFER_DONE))
    {
        /* Clear flag */
        HAL_REG_WRITE(reg->STATUS, I2C_STATUS_TRANSFER_DONE);

        if (HAL_FLAG_HAS(raw_status, I2C_STATUS_START)) // TD before Hit
        {
            HAL_REG_WRITE(reg->STATUS, I2C_STATUS_START);
            return;
        }

        switch (hi2c->xfer_state)
        {
            case I2C_XFER_STATE_DATA:
            {
                if (hi2c->xfer_count > 0U)
                {
                    /* Receive byte */
                    *(hi2c->buffer_ptr) = HAL_REG_READ(reg->DATA);
                    hi2c->buffer_ptr++;
                    hi2c->xfer_count--;

                    if (hi2c->xfer_count > 0U)
                    {
                        /* Enable transfer byte */
                        HAL_REG_SBIT(reg->CTRL, I2C_CTRL_MASK_TBEN);
                        if (hi2c->xfer_count == 1U) // The next one is the last transaction of data
                        {
                            /* Send NACK */
                            _i2c_nack_condition(hi2c);
                        }
                    }
                    else
                    {
                        hi2c->xfer_state = I2C_XFER_STATE_STOP;
                        _i2c_update_stop_state(hi2c);
                    }
                }
            }
            break;

            case I2C_XFER_STATE_STOP:
            case I2C_XFER_STATE_COMPLETED:
            {
                /* Send NACK */
                _i2c_nack_condition(hi2c);
            }

            break;

            default:
                break;
        }
    }
    if (HAL_FLAG_HAS(raw_status, I2C_STATUS_STOP))
    {
        /* Clear flag */
        HAL_REG_WRITE(reg->STATUS, I2C_STATUS_STOP);

        /* If the stop interrupt is triggered, slave can only be disabled when the the address confirmation has completed */
        if (hi2c->xfer_state < I2C_XFER_STATE_DATA)
        {
            return;
        }

        /* With stop condition */
        hi2c->xfer_state  = I2C_XFER_STATE_COMPLETED;
        hi2c->xfer_option = I2C_NO_OPTION_FRAME;
        /* Disable the peripheral */
        PERIPHERAL_I2C_DISABLE(hi2c);
    }

    /* Call completed handler */
    if (hi2c->xfer_state == I2C_XFER_STATE_COMPLETED)
    {
        _i2c_slave_completion_handler(hi2c);
    }
}

static void _i2c_nack_condition(I2C_Handle_T *hi2c)
{
    /* Generate nack condition
         No option frame
         Only frame (sequential usage)
         Last frame (sequential usage) */
    if (hi2c->xfer_option == I2C_NO_OPTION_FRAME || HAL_FLAG_HAS(hi2c->xfer_option, I2C_OPTION_NACK_FLAG))
    {
        HAL_REG_SBIT(hi2c->instance->CTRL, I2C_CTRL_MASK_ACKNACK);
    }
}

static void _i2c_master_stop_condition(I2C_Handle_T *hi2c)
{
    hi2c->xfer_state = I2C_XFER_STATE_STOP;
    /* Generate stop condition
         No option frame
         Only frame (sequential usage)
         Last frame with stop (sequential usage) */
    if (hi2c->xfer_option == I2C_NO_OPTION_FRAME || HAL_FLAG_HAS(hi2c->xfer_option, I2C_OPTION_STOP_FLAG))
    {
        HAL_REG_SBIT(hi2c->instance->CTRL, I2C_CTRL_MASK_STOP);
    }
}

static void _i2c_update_stop_state(I2C_Handle_T *hi2c)
{
    /* Check transfer option */
    if (hi2c->xfer_option != I2C_NO_OPTION_FRAME)
    {
        if (HAL_FLAG_LACKS(hi2c->xfer_option, I2C_OPTION_STOP_FLAG))
        {
            /* No stop condition (sequential usage) */
            hi2c->xfer_state = I2C_XFER_STATE_COMPLETED;
        }
    }
}

static void _i2c_master_completion_handler(I2C_Handle_T *hi2c)
{
    _i2c_it_transfer_end(hi2c);

    if (hi2c->abort == 1) // Transfer aborted
    {
        hi2c->abort = 0;
#if (configUSE_I2C_REGISTER_CALLBACKS == 1)
        /* Call registered callback */
        hi2c->AbortCpltCallback(hi2c);
#else
        /* Call legacy weak callback */
        HAL_I2C_AbortCpltCallback(hi2c);
#endif /* configUSE_I2C_REGISTER_CALLBACKS */
    }
    else // Transfer completed
    {
        if (hi2c->xfer_count == 0U)
        {
#if (configUSE_I2C_REGISTER_CALLBACKS == 1)
            /* Call registered callback */
            if (hi2c->direction == I2C_DIRECTION_TX)
            {
                hi2c->MasterTxCpltCallback(hi2c);
            }
            else if (hi2c->direction == I2C_DIRECTION_RX)
            {
                hi2c->MasterRxCpltCallback(hi2c);
            }
#else
            /* Call legacy weak callback */
            if (hi2c->direction == I2C_DIRECTION_TX)
            {
                HAL_I2C_MasterTxCpltCallback(hi2c);
            }
            else if (hi2c->direction == I2C_DIRECTION_RX)
            {
                HAL_I2C_MasterRxCpltCallback(hi2c);
            }
#endif /* configUSE_I2C_REGISTER_CALLBACKS */
        }
        else
        {
            hi2c->error_code = HAL_I2C_ERROR_AF;
#if (configUSE_I2C_REGISTER_CALLBACKS == 1)
            /* Call registered callback */
            hi2c->ErrorCallback(hi2c);
#else
            /* Call legacy weak callback */
            HAL_I2C_ErrorCallback(hi2c);
#endif /* configUSE_I2C_REGISTER_CALLBACKS */
        }
    }
}

static void _i2c_slave_completion_handler(I2C_Handle_T *hi2c)
{
    _i2c_it_transfer_end(hi2c);

    if (hi2c->xfer_count == 0U)
    {
        _i2c_slave_cplt_callback(hi2c);
    }
    else
    {
        hi2c->error_code = HAL_I2C_ERROR_AF;
#if (configUSE_I2C_REGISTER_CALLBACKS == 1)
        /* Call registered callback */
        hi2c->ErrorCallback(hi2c);
#else
        /* Call legacy weak callback */
        HAL_I2C_ErrorCallback(hi2c);
#endif /* configUSE_I2C_REGISTER_CALLBACKS */
    }
}
#endif /* configUSE_I2C_NONBLOCKING_IT */

#if IT_OR_BURST
static void _i2c_master_al_handler(I2C_Handle_T *hi2c)
{
    _i2c_it_transfer_end(hi2c);

    hi2c->error_code = HAL_I2C_ERROR_ARLO;
#if (configUSE_I2C_REGISTER_CALLBACKS == 1)
    /* Call registered callback */
    hi2c->ErrorCallback(hi2c);
#else
    /* Call legacy weak callback */
    HAL_I2C_ErrorCallback(hi2c);
#endif /* configUSE_I2C_REGISTER_CALLBACKS */
}

static void _i2c_it_transfer_end(I2C_Handle_T *hi2c)
{
    /* Disable all interrupts */
    /* Reset NACK */
    uint32_t ctrl_mask = I2C_CTRL_MASK_STARTIEN | I2C_CTRL_MASK_SAMIEN | I2C_CTRL_MASK_TDIEN | I2C_CTRL_MASK_NACKIEN | I2C_CTRL_MASK_STOPIEN | I2C_CTRL_MASK_ALIEN | I2C_CTRL_MASK_ACKNACK;
    HAL_REG_CBIT(hi2c->instance->CTRL, ctrl_mask);

    /* Update the I2C state */
    hi2c->state       = HAL_STATE_READY;
    hi2c->mode        = HAL_I2C_MODE_NONE;
    hi2c->ext_xfer.en = HAL_DISABLE;
}

static void _i2c_slave_cplt_callback(I2C_Handle_T *hi2c)
{
#if (configUSE_I2C_REGISTER_CALLBACKS == 1)
    /* Call registered callback */
    if (hi2c->direction == I2C_DIRECTION_TX)
    {
        hi2c->SlaveTxCpltCallback(hi2c);
        hi2c->ext_xfer.tx_xfer_num = 0;
    }
    else if (hi2c->direction == I2C_DIRECTION_RX)
    {
        hi2c->SlaveRxCpltCallback(hi2c);
        hi2c->ext_xfer.rx_xfer_num = 0;
    }
#else
    /* Call legacy weak callback */
    if (hi2c->direction == I2C_DIRECTION_TX)
    {
        HAL_I2C_SlaveTxCpltCallback(hi2c);
        hi2c->ext_xfer.tx_xfer_num = 0;
    }
    else if (hi2c->direction == I2C_DIRECTION_RX)
    {
        HAL_I2C_SlaveRxCpltCallback(hi2c);
        hi2c->ext_xfer.rx_xfer_num = 0;
    }
#endif /* configUSE_I2C_REGISTER_CALLBACKS */
}

static void _i2c_ext_slave_tx_it_handler(I2C_Handle_T *hi2c, uint32_t raw_status)
{
    SN_I2C0_Type *reg  = hi2c->instance;
    I2CEx_XFER_T *xfer = &hi2c->ext_xfer;
    /* Check I2C Status */
    if (HAL_FLAG_HAS(raw_status, I2C_STATUS_HIT))
    {
        /* Clear flag */
        HAL_REG_WRITE(reg->STATUS, I2C_STATUS_HIT | I2C_STATUS_TRANSFER_DONE);
        /* Set direction */
        hi2c->direction = I2C_DIRECTION_TX;

        switch (hi2c->xfer_state)
        {
            case I2C_XFER_STATE_7BITS_ADDR:
            case I2C_XFER_STATE_10BITS_ADDR_L:
            {
                hi2c->xfer_state = I2C_XFER_STATE_DATA;
                if (xfer->tx_buffer_ptr != NULL && xfer->tx_xfer_count > 0U)
                {
                    /* Transmit byte */
                    HAL_REG_WRITE(reg->DATA, *(xfer->tx_buffer_ptr));
                    xfer->tx_buffer_ptr++;
                    xfer->tx_xfer_count--;
                    xfer->tx_xfer_num++;
                }
            }
            break;

            case I2C_XFER_STATE_10BITS_ADDR_H:
            {
                hi2c->xfer_state = I2C_XFER_STATE_10BITS_ADDR_L;
            }
            break;

            case I2C_XFER_STATE_STOP: // Restart condition after NACK
            case I2C_XFER_STATE_COMPLETED:
            {
                if (hi2c->init.addressing_mode == I2C_ADDRESSINGMODE_7BIT)
                {
                    hi2c->xfer_state = I2C_XFER_STATE_DATA;
                    if (xfer->tx_buffer_ptr != NULL && xfer->tx_xfer_count > 0U)
                    {
                        /* Transmit byte */
                        HAL_REG_WRITE(reg->DATA, *(xfer->tx_buffer_ptr));
                        xfer->tx_buffer_ptr++;
                        xfer->tx_xfer_count--;
                        xfer->tx_xfer_num++;
                    }
                }
                else if (hi2c->init.addressing_mode == I2C_ADDRESSINGMODE_10BIT)
                {
                    hi2c->xfer_state = I2C_XFER_STATE_10BITS_ADDR_L;
                }
            }
            break;

            default:
                break;
        }
        /* Enable transfer byte */
        HAL_REG_SBIT(reg->CTRL, I2C_CTRL_MASK_TBEN);
        /* Wait for next interrupt */
        return;
    }
    if (HAL_FLAG_HAS(raw_status, I2C_STATUS_NACK))
    {
        /* Clear flag */
        HAL_REG_WRITE(reg->STATUS, I2C_STATUS_NACK);

        hi2c->xfer_state = I2C_XFER_STATE_STOP;
    }
    if (HAL_FLAG_HAS(raw_status, I2C_STATUS_TRANSFER_DONE))
    {
        /* Clear flag */
        HAL_REG_WRITE(reg->STATUS, I2C_STATUS_TRANSFER_DONE);

        switch (hi2c->xfer_state)
        {
            case I2C_XFER_STATE_10BITS_ADDR_L:
            {
                hi2c->xfer_state = I2C_XFER_STATE_DATA;
            }
            case I2C_XFER_STATE_DATA:
            {
                if (xfer->tx_buffer_ptr != NULL)
                {
                    if (xfer->tx_xfer_count > 0U)
                    {
                        /* Transmit byte */
                        HAL_REG_WRITE(reg->DATA, *(xfer->tx_buffer_ptr));
                        xfer->tx_buffer_ptr++;
                        xfer->tx_xfer_count--;
                        xfer->tx_xfer_num++;
                    }
                    else
                    {
                        hi2c->xfer_state = I2C_XFER_STATE_STOP;
                    }
                }
                else
                {
                    hi2c->xfer_state = I2C_XFER_STATE_STOP;
                }
                /* Enable transfer byte */
                HAL_REG_SBIT(reg->CTRL, I2C_CTRL_MASK_TBEN);
            }
            break;

            case I2C_XFER_STATE_STOP:
            {
                /* Enable transfer byte */
                HAL_REG_SBIT(reg->CTRL, I2C_CTRL_MASK_TBEN);
            }
            break;

            case I2C_XFER_STATE_10BITS_ADDR_H:
                break;

            default:
                break;
        }
    }
    if (HAL_FLAG_HAS(raw_status, I2C_STATUS_STOP))
    {
        /* Clear flag */
        HAL_REG_WRITE(reg->STATUS, I2C_STATUS_STOP);

        _i2c_ext_slave_stop_handler(hi2c);
    }
}

static void _i2c_ext_slave_rx_it_handler(I2C_Handle_T *hi2c, uint32_t raw_status)
{
    SN_I2C0_Type *reg  = hi2c->instance;
    I2CEx_XFER_T *xfer = &hi2c->ext_xfer;
    /* Check I2C Status */
    if (HAL_FLAG_HAS(raw_status, I2C_STATUS_HIT))
    {
        /* Clear flag */
        HAL_REG_WRITE(reg->STATUS, I2C_STATUS_HIT | I2C_STATUS_TRANSFER_DONE);
        /* Set direction */
        hi2c->direction = I2C_DIRECTION_RX;

        hi2c->xfer_state = I2C_XFER_STATE_DATA;

        if (xfer->rx_xfer_count <= 1U)
        {
            /* Send NACK */
            HAL_REG_SBIT(reg->CTRL, I2C_CTRL_MASK_ACKNACK);
        }
        /* Enable transfer byte */
        HAL_REG_SBIT(reg->CTRL, I2C_CTRL_MASK_TBEN);
        /* Wait for next interrupt */
        return;
    }
    if (HAL_FLAG_HAS(raw_status, I2C_STATUS_TRANSFER_DONE))
    {
        /* Clear flag */
        HAL_REG_WRITE(reg->STATUS, I2C_STATUS_TRANSFER_DONE);

        if (HAL_FLAG_HAS(raw_status, I2C_STATUS_START)) // TD before Hit
        {
            return;
        }

        switch (hi2c->xfer_state)
        {
            case I2C_XFER_STATE_DATA:
            {
                if (xfer->rx_buffer_ptr != NULL)
                {
                    if (xfer->rx_xfer_count > 0U)
                    {
                        /* Receive byte */
                        *(xfer->rx_buffer_ptr) = HAL_REG_READ(reg->DATA);
                        xfer->rx_buffer_ptr++;
                        xfer->rx_xfer_count--;
                        xfer->rx_xfer_num++;

                        if (xfer->rx_xfer_count > 0U)
                        {
                            /* Enable transfer byte */
                            HAL_REG_SBIT(reg->CTRL, I2C_CTRL_MASK_TBEN);
                            if (xfer->rx_xfer_count == 1U) // The next one is the last transaction of data
                            {
                                /* Send NACK */
                                HAL_REG_SBIT(reg->CTRL, I2C_CTRL_MASK_ACKNACK);
                            }
                        }
                        else
                        {
                            hi2c->xfer_state = I2C_XFER_STATE_STOP;
                        }
                    }
                    else
                    {
                        /* Send NACK */
                        HAL_REG_SBIT(reg->CTRL, I2C_CTRL_MASK_ACKNACK);
                    }
                }
                else
                {
                    hi2c->xfer_state = I2C_XFER_STATE_STOP;
                    /* Send NACK */
                    HAL_REG_SBIT(reg->CTRL, I2C_CTRL_MASK_ACKNACK);
                }
            }
            break;

            case I2C_XFER_STATE_STOP:
            {
                /* Send NACK */
                HAL_REG_SBIT(reg->CTRL, I2C_CTRL_MASK_ACKNACK);
            }
            break;

            default:
                break;
        }
    }
    if (HAL_FLAG_HAS(raw_status, I2C_STATUS_NACK))
    {
        /* Clear flag */
        HAL_REG_WRITE(reg->STATUS, I2C_STATUS_NACK);
    }
    if (HAL_FLAG_HAS(raw_status, I2C_STATUS_STOP))
    {
        /* Clear flag */
        HAL_REG_WRITE(reg->STATUS, I2C_STATUS_STOP);

        _i2c_ext_slave_stop_handler(hi2c);
    }
}

static void _i2c_ext_slave_stop_handler(I2C_Handle_T *hi2c)
{
    I2CEx_XFER_T *xfer = &hi2c->ext_xfer;

    /* If the stop interrupt is triggered, slave can only be disabled when the the address confirmation has completed */
    if (hi2c->xfer_state < I2C_XFER_STATE_DATA)
    {
        return;
    }

    _i2c_slave_cplt_callback(hi2c);
    /* Clear direction */
    hi2c->direction  = I2C_DIRECTION_NONE;
    hi2c->xfer_state = I2C_XFER_STATE_COMPLETED;

    if (xfer->tx_xfer_count == 0U && xfer->rx_xfer_count == 0U)
    {
        /* Disable the peripheral */
        PERIPHERAL_I2C_DISABLE(hi2c);
        _i2c_it_transfer_end(hi2c);
    }
}
#endif /* IT_OR_BURST */

#if (configUSE_I2C_NONBLOCKING_BURST == 1)
static HAL_Status_T _i2c_master_transfer_burst(I2C_Handle_T *hi2c, I2C_DIRECTION_T direction, uint16_t dev_address, uint8_t *data, uint8_t size, I2C_Burst_T *burst_options)
{
    HAL_Status_T status = HAL_OK;

    /* Check the I2C handle */
    ASSERT_PARAM(hi2c);
    /* Check the I2C instance */
    ASSERT_PARAM(IS_PERIPHERAL_I2C_INSTANCE(hi2c));
    /* Check the parameters */
    ASSERT_PARAM(data);
    ASSERT_PARAM(burst_options);
    ASSERT_PARAM(IS_PERIPHERAL_I2C_BURST_INDEX2_EN(burst_options->index2_en));
    uint32_t max_size = 1U << HAL_REG_READ(hi2c->instance->BSTM_b.BUFSZ);
    RET_FLAG_TRUE(size > max_size, HAL_ERROR);
    RET_FLAG_FALSE(hi2c->xfer_option == I2C_NO_OPTION_FRAME, HAL_ERROR);

    if (hi2c->state == HAL_STATE_READY)
    {
        /* Protect the process */
        TAKE_MUTEX(hi2c);

        /* Update the I2C state */
        hi2c->state = HAL_STATE_BUSY;
        hi2c->mode  = HAL_I2C_MODE_MASTER;
        CLEAR_ERROR_CODE(hi2c);

        /* Set runtime info */
        hi2c->buffer_ptr  = data;
        hi2c->xfer_size   = size;
        hi2c->xfer_count  = size;
        hi2c->direction   = direction;
        hi2c->dev_address = dev_address;

        SN_I2C0_Type *reg = hi2c->instance;
        /* Clear status */
        I2C_CLEAR_ALL_STATUS(reg);
        /* Enable the peripheral */
        /* Set master mode */
        /* Enable interrupt: TD  & STOP & AL*/
        uint32_t ctrl_mask = I2C_CTRL_MASK_I2CEN | I2C_CTRL_MASK_MSTEN | I2C_CTRL_MASK_TDIEN | I2C_CTRL_MASK_STOPIEN | I2C_CTRL_MASK_ALIEN;
        HAL_REG_SBIT(reg->CTRL, ctrl_mask);
        /* Set slave address */
        HAL_REG_WRITE(reg->ADDR_b.ADDR, hi2c->dev_address);

        /* Set index */
        HAL_REG_WRITE(reg->ADDR_b.MEM_IDX, burst_options->index1);
        HAL_REG_WRITE(reg->ADDR_b.MEM_IDX2, burst_options->index2);
        HAL_REG_WRITE(reg->ADDR_b.M2BIDX_EN, burst_options->index2_en);

        /* Set data count */
        HAL_REG_WRITE(reg->BSTM_b.TDC, size);

        if (hi2c->direction == I2C_DIRECTION_TX_BURST) // Load data
        {
            for (uint8_t i = 0; i < size; i++)
            {
                HAL_REG_WRITE(reg->DATA, *(hi2c->buffer_ptr));
                hi2c->buffer_ptr++;
            }
        }
        else if (hi2c->direction == I2C_DIRECTION_RX_BURST) // Set threshold
        {
            HAL_REG_WRITE(reg->BSTM_b.BSTTHOD, size);
        }

        /* Enable burst mode */
        HAL_REG_WRITE(reg->CTRL_b.BURSTEN, I2C_BURST_MODE(hi2c->direction));

        /* Release the process protection */
        GIVE_MUTEX(hi2c);
    }
    else if (hi2c->state == HAL_STATE_BUSY)
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

static void _i2c_master_tx_burst_handler(I2C_Handle_T *hi2c, uint32_t raw_status)
{
    if (HAL_FLAG_HAS(raw_status, I2C_STATUS_TRANSFER_DONE))
    {
        SN_I2C0_Type *reg = hi2c->instance;
        /* Clear flag */
        HAL_REG_WRITE(reg->STATUS, I2C_STATUS_START);

        hi2c->xfer_count -= HAL_REG_READ(reg->BSTM_b.TDC);
    }

    if (HAL_FLAG_HAS(raw_status, I2C_STATUS_STOP))
    {
        /* Disable the peripheral */
        PERIPHERAL_I2C_DISABLE(hi2c);
        _i2c_it_transfer_end(hi2c);

        if (hi2c->xfer_count == 0U)
        {
#if (configUSE_I2C_REGISTER_CALLBACKS == 1)
            /* Call registered callback */
            hi2c->MasterTxCpltCallback(hi2c);
#else
            HAL_I2C_MasterTxCpltCallback(hi2c);
#endif /* configUSE_I2C_REGISTER_CALLBACKS */
        }
        else
        {
            hi2c->error_code = HAL_I2C_ERROR_AF;
#if (configUSE_I2C_REGISTER_CALLBACKS == 1)
            /* Call registered callback */
            hi2c->ErrorCallback(hi2c);
#else
            /* Call legacy weak callback */
            HAL_I2C_ErrorCallback(hi2c);
#endif /* configUSE_I2C_REGISTER_CALLBACKS */
        }
    }
}

static void _i2c_master_rx_burst_handler(I2C_Handle_T *hi2c, uint32_t raw_status)
{
    if (HAL_FLAG_HAS(raw_status, I2C_STATUS_TRANSFER_DONE))
    {
        SN_I2C0_Type *reg = hi2c->instance;
        /* Clear flag */
        HAL_REG_WRITE(reg->STATUS, I2C_STATUS_START);
        /* Receive data */
        uint32_t size = HAL_REG_READ(reg->BSTM_b.BSTTHOD);
        for (uint8_t i = 0; i < size; i++)
        {
            *(hi2c->buffer_ptr) = HAL_REG_READ(reg->DATA);
            hi2c->buffer_ptr++;
            hi2c->xfer_count--;
        }
    }

    if (HAL_FLAG_HAS(raw_status, I2C_STATUS_STOP))
    {
        /* Disable the peripheral */
        PERIPHERAL_I2C_DISABLE(hi2c);
        _i2c_it_transfer_end(hi2c);

        if (hi2c->xfer_count == 0U)
        {
#if (configUSE_I2C_REGISTER_CALLBACKS == 1)
            /* Call registered callback */
            hi2c->MasterRxCpltCallback(hi2c);
#else
            HAL_I2C_MasterRxCpltCallback(hi2c);
#endif /* configUSE_I2C_REGISTER_CALLBACKS */
        }
        else
        {
            hi2c->error_code = HAL_I2C_ERROR_AF;
#if (configUSE_I2C_REGISTER_CALLBACKS == 1)
            /* Call registered callback */
            hi2c->ErrorCallback(hi2c);
#else
            /* Call legacy weak callback */
            HAL_I2C_ErrorCallback(hi2c);
#endif /* configUSE_I2C_REGISTER_CALLBACKS */
        }
    }
}
#endif /* configUSE_I2C_NONBLOCKING_BURST */

#endif /* configUSE_I2C_PERIPHERAL */
