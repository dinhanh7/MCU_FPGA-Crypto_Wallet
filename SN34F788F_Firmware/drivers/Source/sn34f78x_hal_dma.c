/***********************
 * SN34F78X SDK V2.1.4 *
 ***********************/

#include "sn34f78x_hal_dma.h"

#if (configUSE_DMA_PERIPHERAL == 1)

/* Exported functions --------------------------------------------------------*/
/*
================================================================================
            ##### Initialization/De-initialization functions #####
================================================================================
*/
/**
 * @brief  Initializes the DMA mode according to the specified parameters in the DMA_Init_T and create the associated handle.
 * @param  hdma Pointer to a DMA_Handle_T structure that contains the configuration information for the specified DMA module.
 * @retval HAL status
 */
HAL_Status_T HAL_DMA_Init(DMA_Handle_T *hdma)
{
    /* check the DMA handle */
    ASSERT_PARAM(hdma);

    /* check the DMA instance */
    ASSERT_PARAM(IS_PERIPHERAL_DMA_INSTANCE(hdma));

    /* check the DMA channel */
    ASSERT_PARAM(IS_DMA_CHANNEL(hdma));

    /* check the DMA transfer mode */
    ASSERT_PARAM(IS_DMA_XFER_MODE(hdma));

    /* check the DMA priority mode */
    ASSERT_PARAM(IS_DMA_PRIORITY(hdma));

    /* check the DMA priority mode */
    ASSERT_PARAM(IS_DMA_SRC_BURST(hdma));

    /* check the DMA transfer data width */
    ASSERT_PARAM(IS_DMA_XFER_WIDTH(hdma));

    /* check the DMA address mode config */
    ASSERT_PARAM(IS_DMA_ADDR_MODE(hdma));

    /* check the DMA address mode config */
    ASSERT_PARAM(IS_DMA_AHB_MASTER(hdma));

    /* check the DMA fifo threshold config */
    ASSERT_PARAM(IS_DMA_FIFO_TH(hdma));

    /* check current channel has already run */
    ASSERT_PARAM(!IS_ACTIVE_CH(hdma));

    /* check whether current channel is already initialized */
    if (HAL_FLAG_EQU(hdma->state, HAL_STATE_RESET))
    {
        ASSERT_PARAM(!IS_DMA_LIMITATION(hdma));

        volatile uint32_t *reg_csr   = REG_CSR(hdma);
        volatile uint32_t *reg_cfg   = REG_CFG(hdma);
        uint8_t            xfer_mode = CALC_XFER_MODE(hdma);

        uint32_t csr_val = ((HAL_DISABLE << 0) |
                            (hdma->init.dst_ahb_master << 1) | (hdma->init.src_ahb_master << 2) |
                            (hdma->init.dst_addr_mode << 3) | (hdma->init.src_addr_mode << 5) |
                            (xfer_mode << 7) |
                            (hdma->init.dst_width << 8) | (hdma->init.src_width << 11) |
                            (hdma->init.src_burst << 16) |
                            (hdma->init.ch_priority << 22) |
                            (hdma->init.ch_fifo_th << 24));

        uint8_t  src_mode = CALC_SRC_MODE(hdma);
        uint8_t  dst_mode = CALC_DST_MODE(hdma);
        uint32_t cfg_val  = (src_mode << 7) |
                           (dst_mode << 13) |
                           (hdma->init.src_rs << 16) |
                           (hdma->init.dst_rs << 24);

        uint32_t mcsr_val = (HAL_ENABLE << 0);

        uint32_t sync_val = (hdma->init.ch_sync << hdma->init.channel);
        uint32_t sync_msk = (1 << hdma->init.channel);

        // 0: set default callback function
        hdma->XferCpltCallback  = HAL_DMA_XferCpltCallback;
        hdma->XferErrorCallback = HAL_DMA_XferErrorCallback;
        hdma->XferAbortCallback = HAL_DMA_XferAbortCallback;

        // user should initialize the fields below in HAL_DMA_MspInit
        // 1: enable reference clock
        // 2: the MspInitCallback could be register before initialization
        if (hdma->MspInitCallback == NULL)
        {
            hdma->MspInitCallback = HAL_DMA_MspInit;
        }
        hdma->MspInitCallback(hdma);

        HAL_REG_WRITE(*reg_csr, csr_val);
        HAL_REG_WRITE(*reg_cfg, cfg_val);
        HAL_REG_SBIT(hdma->instance->MCSR, mcsr_val);

        // config channel sync setting
        HAL_REG_CBIT(hdma->instance->SYNC, sync_msk);
        HAL_REG_SBIT(hdma->instance->SYNC, sync_val);

        // set the xfer reference callback to default

        /* update state to ready */
        CLEAR_ERROR_CODE(hdma);
        SET_DMA_ACTIVE_CH(hdma);
        hdma->state = HAL_STATE_READY;

        /* init mutex */
        GIVE_MUTEX(hdma);
    }
    else
    {
        /* update the state */
        hdma->error_code = HAL_ERROR_REINIT;
        return HAL_ERROR;
    }

    return HAL_OK;
}

/**
 * @brief  DeInitializes the DMA peripheral.
 * @param  hdma Pointer to a DMA_Handle_T structure that contains the configuration information for the specified DMA module.
 * @retval HAL status
 */
HAL_Status_T HAL_DMA_DeInit(DMA_Handle_T *hdma)
{
    /* check the DMA handle */
    ASSERT_PARAM(hdma);
    /* check the DMA instance */
    ASSERT_PARAM(IS_PERIPHERAL_DMA_INSTANCE(hdma));

    /* DMA is not ready */
    RET_STATE_NOT_READY(hdma);

    TAKE_MUTEX(hdma);

    /* release current channel */
    volatile uint32_t *reg_csr = REG_CSR(hdma);
    HAL_REG_CBIT(*reg_csr, HAL_ENABLE);

    /* disable DMA */
    HAL_REG_WRITE(hdma->instance->MCSR_b.DMACEN, HAL_DISABLE);

    // user user should deinitialize the fields below in HAL_DMA_MspDeInit
    // 1: disable reference clock
    if (hdma->MspDeInitCallback == NULL)
    {
        hdma->MspDeInitCallback = HAL_DMA_MspDeInit;
    }
    hdma->MspDeInitCallback(hdma);

    // 2: set default callback function
    HAL_DMA_UnRegisterCallback(hdma, HAL_DMA_ALL_CB_ID);

    /* update the state */
    CLEAR_ERROR_CODE(hdma);
    CLR_DMA_ACTIVE_CH(hdma);
    hdma->state     = HAL_STATE_RESET;
    hdma->xfer_size = 0;
    /* release the process protection */
    GIVE_MUTEX(hdma);

    return HAL_OK;
}

/*
================================================================================
            ##### Callbacks Register/UnRegister functions #####
================================================================================
*/

/**
 * @brief  Register a User DMA callback to be used instead of the weak predefined callback.
 * @param  hdma Pointer to a DMA_Handle_T structure that contains the configuration information for the specified DMA module.
 * @param  callback_id ID of the callback to be registered.
 *         This parameter can be one of the following values:
 *           @arg @ref HAL_DMA_TIMEOUT_CB_ID DMA Timeout Callback ID
 * @param  callback pointer to the Callback function
 * @retval HAL status
 */
HAL_Status_T HAL_DMA_RegisterCallback(DMA_Handle_T *hdma, HAL_DMA_CallbackID_T callback_id, DMA_Callback_T callback)
{
    /* check the DMA handle */
    ASSERT_PARAM(hdma);
    /* check the DMA instance */
    ASSERT_PARAM(IS_PERIPHERAL_DMA_INSTANCE(hdma));
    /* check the DMA callback id */
    ASSERT_PARAM(IS_DMA_CALLBACK_ID(callback_id));

    /* protect the process */
    TAKE_MUTEX(hdma);

    switch (callback_id)
    {
        case HAL_DMA_MSP_INIT_CB_ID:
        {
            hdma->MspInitCallback = callback;
        }
        break;
        case HAL_DMA_MSP_DEINIT_CB_ID:
        {
            hdma->MspDeInitCallback = callback;
        }
        break;
        case HAL_DMA_XFER_CPLT_CB_ID:
        {
            hdma->XferCpltCallback = callback;
        }
        break;
        case HAL_DMA_XFER_ERROR_CB_ID:
        {
            hdma->XferErrorCallback = callback;
        }
        break;
        case HAL_DMA_XFER_ABORT_CB_ID:
        {
            hdma->XferAbortCallback = callback;
        }
        break;
        default:
            break;
    }

    /* release the process protection */
    GIVE_MUTEX(hdma);

    return HAL_OK;
}

/**
 * @brief  Unregister an DMA callback, DMA callback is redirected to the weak predefined callback.
 * @param  hdma Pointer to a DMA_Handle_T structure that contains the configuration information for the specified DMA module.
 * @param  callback_id ID of the callback to be unregistered.
 *         This parameter can be one of the following values:
 *           @arg @ref HAL_DMA_TIMEOUT_CB_ID DMA Timeout Callback ID
 * @retval HAL status
 */
HAL_Status_T HAL_DMA_UnRegisterCallback(DMA_Handle_T *hdma, HAL_DMA_CallbackID_T callback_id)
{
    /* check the DMA handle */
    ASSERT_PARAM(hdma);
    /* check the DMA instance */
    ASSERT_PARAM(IS_PERIPHERAL_DMA_INSTANCE(hdma));
    /* check the DMA callback id */
    ASSERT_PARAM(IS_DMA_CALLBACK_ID(callback_id));

    /* protect the process */
    TAKE_MUTEX(hdma);
    switch (callback_id)
    {
        case HAL_DMA_MSP_INIT_CB_ID:
        {
            hdma->MspInitCallback = HAL_DMA_MspInit;
        }
        break;
        case HAL_DMA_MSP_DEINIT_CB_ID:
        {
            hdma->MspDeInitCallback = HAL_DMA_MspDeInit;
        }
        break;
        case HAL_DMA_XFER_CPLT_CB_ID:
        {
            hdma->XferCpltCallback = HAL_DMA_XferCpltCallback;
        }
        break;
        case HAL_DMA_XFER_ERROR_CB_ID:
        {
            hdma->XferErrorCallback = HAL_DMA_XferErrorCallback;
        }
        break;
        case HAL_DMA_XFER_ABORT_CB_ID:
        {
            hdma->XferAbortCallback = HAL_DMA_XferAbortCallback;
        }
        break;
        case HAL_DMA_ALL_CB_ID:
        {
            hdma->MspInitCallback   = HAL_DMA_MspInit;
            hdma->MspDeInitCallback = HAL_DMA_MspDeInit;
            hdma->XferCpltCallback  = HAL_DMA_XferCpltCallback;
            hdma->XferErrorCallback = HAL_DMA_XferErrorCallback;
            hdma->XferAbortCallback = HAL_DMA_XferAbortCallback;
        }
        break;
    }

    /* release the process protection */
    GIVE_MUTEX(hdma);

    return HAL_OK;
}

/**
 * @brief config the endian for current DMA
 *
 * @param hdma hdma Pointer to a DMA_Handle_T structure that contains the configuration information for the specified DMA module.
 * @param endian the detail endian information \ref dma_ahb_endian
 * @return HAL_Status_T
 */
HAL_Status_T HAL_DMA_ConfigEndian(DMA_Handle_T *hdma, HAL_Endian_T *endian)
{
    /* check the DMA handle */
    ASSERT_PARAM(hdma);
    /* check the DMA instance */
    ASSERT_PARAM(IS_PERIPHERAL_DMA_INSTANCE(hdma));
    /* check the DMA AHB endian */
    ASSERT_PARAM(IS_DMA_AHB_ENDIAN(endian));

    uint32_t mcsr_val      = ((endian->m0_endian << 1) | (endian->m1_endian << 2));
    uint32_t mcsr_val_mask = ((1 << 1) | (1 << 2));
    HAL_REG_CBIT(hdma->instance->MCSR, mcsr_val_mask);
    HAL_REG_SBIT(hdma->instance->MCSR, mcsr_val);
    return HAL_OK;
}

/**
 * @brief get the enable status of all channels
 *
 * @param hdma Pointer to a DMA_Handle_T structure that contains the configuration information for the specified DMA module.
 * @param ch_status the returned status memory
 * @return HAL_Status_T
 */
HAL_Status_T HAL_DMA_GetChannelStatus(DMA_Handle_T *hdma, uint32_t *ch_status)
{
    /* check the DMA handle */
    ASSERT_PARAM(hdma);
    /* check the DMA instance */
    ASSERT_PARAM(IS_PERIPHERAL_DMA_INSTANCE(hdma));
    /* check the store status should not be NULL */
    ASSERT_PARAM(ch_status);

    *ch_status = HAL_REG_READ(hdma->instance->CH_EN);
    return HAL_OK;
}

/*
================================================================================
            ##### Peripheral State and Errors functions #####
================================================================================
*/
/**
 * @brief  Returns the DMA state.
 * @param  hdma Pointer to a DMA_Handle_T structure that contains the configuration information for the specified DMA module.
 * @retval HAL state
 */
HAL_State_T HAL_DMA_GetState(DMA_Handle_T *hdma)
{
    /* check the DMA handle */
    ASSERT_PARAM(hdma);
    /* check the DMA instance */
    ASSERT_PARAM(IS_PERIPHERAL_DMA_INSTANCE(hdma));

    return hdma->state;
}

/**
 * @brief  Return the DMA error code
 * @param  hdma Pointer to a DMA_Handle_T structure that contains the configuration information for the specified DMA module.
 * @retval DMA error code
 */
uint32_t HAL_DMA_GetError(DMA_Handle_T *hdma)
{
    /* check the DMA handle */
    ASSERT_PARAM(hdma);
    /* check the DMA instance */
    ASSERT_PARAM(IS_PERIPHERAL_DMA_INSTANCE(hdma));

    return hdma->error_code;
}

uint32_t HAL_DMA_GetSize(DMA_Handle_T *hdma)
{
    volatile uint32_t *reg_size = REG_SIZE(hdma);

    return HAL_REG_READ(*reg_size);
}

/*
================================================================================
            ##### Peripheral Control functions #####
================================================================================
*/

/**
 * @brief start DMA to transfer data under blocking mode
 *
 * @param hdma Pointer to a DMA_Handle_T structure that contains the configuration information for the specified DMA module.
 * @param src_addr the source address for DMA to acquire data
 * @param dst_addr the destination address for DMA to transfer
 * @param data_len the data length of transferring data
 * @return HAL_Status_T
 */
HAL_Status_T HAL_DMA_Start(DMA_Handle_T *hdma, uint32_t src_addr, uint32_t dst_addr, uint32_t data_len)
{
    HAL_Status_T status = HAL_OK;

    /* check the DMA handle */
    ASSERT_PARAM(hdma);
    /* check the DMA instance */
    ASSERT_PARAM(IS_PERIPHERAL_DMA_INSTANCE(hdma));
    ASSERT_PARAM(data_len);
    ASSERT_PARAM(IS_DMA_ACCESS_MEMORY(src_addr));
    ASSERT_PARAM(IS_DMA_ACCESS_MEMORY(dst_addr));

    /* Check the src_addr in the case of big end and m2m or m2p aligned address is 4 bytes */
    status = _dma_check_transfer_option(hdma, src_addr);
    RET_FLAG_FALSE(status == HAL_OK, status);

    RET_STATE_NOT_READY(hdma);

    /* protect the process */
    TAKE_MUTEX(hdma);
    /* update the state */
    hdma->state = HAL_STATE_BUSY;

    hdma->xfer_size                 = data_len;
    volatile uint32_t *reg_cfg      = REG_CFG(hdma);
    volatile uint32_t *reg_csr      = REG_CSR(hdma);
    volatile uint32_t *reg_size     = REG_SIZE(hdma);
    volatile uint32_t *reg_src_addr = REG_SRCADDR(hdma);
    volatile uint32_t *reg_dst_addr = REG_DSTADDR(hdma);

    HAL_REG_WRITE(*reg_size, data_len);
    HAL_REG_WRITE(*reg_src_addr, src_addr);
    HAL_REG_WRITE(*reg_dst_addr, dst_addr);

    /* disable interrupt flag */
    HAL_REG_SBIT(*reg_cfg, DMA_INT_MASK);
    /* enable current channel to do transferring */
    HAL_REG_SBIT(*reg_csr, HAL_ENABLE);
    /* wait for TC */
    DMA_WAIT_CHANNEL_TC(hdma);
    /* clear TC interrupt */
    DMA_CLR_INT_TC(hdma);
    /* disable current channel */
    HAL_REG_CBIT(*reg_csr, HAL_ENABLE);
    /* enable interrupt flag as default */
    HAL_REG_CBIT(*reg_cfg, DMA_INT_MASK);

    /* release the process protection */
    GIVE_MUTEX(hdma);
    hdma->state = HAL_STATE_READY;
    return HAL_OK;
}

/**
 * @brief DMA pause data transferring under blocking mode
 *
 * @param hdma Pointer to a DMA_Handle_T structure that contains the configuration information for the specified DMA module.
 * @return HAL_Status_T
 */
HAL_Status_T HAL_DMA_Abort(DMA_Handle_T *hdma)
{
    /* check the DMA handle */
    ASSERT_PARAM(hdma);
    /* check the DMA instance */
    ASSERT_PARAM(IS_PERIPHERAL_DMA_INSTANCE(hdma));

    /* protect the process */
    TAKE_MUTEX(hdma);
    CLEAR_ERROR_CODE(hdma);
    HAL_Status_T status = HAL_OK;

    volatile uint32_t *reg_csr = REG_CSR(hdma);
    HAL_REG_SBIT(*reg_csr, DMA_CH_ABORT);

    DMA_WAIT_CHANNEL_ABT(hdma);
    hdma->state = HAL_STATE_READY;

    /* release the process protection */
    GIVE_MUTEX(hdma);
    return status;
}

/**
 * @brief DMA resume from abort state, and continue to do data transferring under blocking mode
 *
 * @param hdma Pointer to a DMA_Handle_T structure that contains the configuration information for the specified DMA module.
 * @return HAL_Status_T
 */
HAL_Status_T HAL_DMA_Resume(DMA_Handle_T *hdma)
{
    /* check the DMA handle */
    ASSERT_PARAM(hdma);
    /* check the DMA instance */
    ASSERT_PARAM(IS_PERIPHERAL_DMA_INSTANCE(hdma));

    /* protect the process */
    TAKE_MUTEX(hdma);
    HAL_Status_T status = HAL_OK;
    CLEAR_ERROR_CODE(hdma);
    do
    {
        if (hdma->state != HAL_STATE_READY)
        {
            status           = HAL_ERROR;
            hdma->error_code = HAL_DMA_ERROR_NO_XFER;
            break;
        }

        volatile uint32_t *reg_csr = REG_CSR(hdma);

        /* enable current channel to do transferring */
        HAL_REG_SBIT(*reg_csr, HAL_ENABLE);
        hdma->state = HAL_STATE_BUSY;

        DMA_WAIT_CHANNEL_TC(hdma);

        hdma->state = HAL_STATE_READY;
        HAL_REG_CBIT(*reg_csr, HAL_ENABLE);
    } while (0);

    /* release the process protection */
    GIVE_MUTEX(hdma);

    return status;
}

/**
 * @brief start DMA to transfer data under non-blocking mode
 *
 * @param hdma Pointer to a DMA_Handle_T structure that contains the configuration information for the specified DMA module.
 * @param src_addr the source address for DMA to acquire data
 * @param dst_addr the destination address for DMA to transfer
 * @param data_len the data length of transferring data
 * @return HAL_Status_T
 */
HAL_Status_T HAL_DMA_Start_IT(DMA_Handle_T *hdma, uint32_t src_addr, uint32_t dst_addr, uint32_t data_len)
{
    HAL_Status_T status = HAL_OK;

    /* check the DMA handle */
    ASSERT_PARAM(hdma);
    /* check the DMA instance */
    ASSERT_PARAM(IS_PERIPHERAL_DMA_INSTANCE(hdma));

    /* Check the src_addr in the case of big end and m2m or m2p aligned address is 4 bytes */
    status = _dma_check_transfer_option(hdma, src_addr);
    RET_FLAG_FALSE(status == HAL_OK, status);

    RET_STATE_NOT_READY(hdma);

    /* protect the process */
    TAKE_MUTEX(hdma);
    /* update the state */
    hdma->state = HAL_STATE_BUSY;
    CLEAR_ERROR_CODE(hdma);

    hdma->xfer_size                 = data_len;
    volatile uint32_t *reg_cfg      = REG_CFG(hdma);
    volatile uint32_t *reg_csr      = REG_CSR(hdma);
    volatile uint32_t *reg_size     = REG_SIZE(hdma);
    volatile uint32_t *reg_src_addr = REG_SRCADDR(hdma);
    volatile uint32_t *reg_dst_addr = REG_DSTADDR(hdma);

    HAL_REG_WRITE(*reg_size, data_len);
    HAL_REG_WRITE(*reg_src_addr, src_addr);
    HAL_REG_WRITE(*reg_dst_addr, dst_addr);

    /* enable interrupt flag */
    HAL_REG_CBIT(*reg_cfg, DMA_INT_MASK);
    /* enable current channel to do transferring */
    HAL_REG_SBIT(*reg_csr, HAL_ENABLE);

    /* release the process protection */
    GIVE_MUTEX(hdma);

    return HAL_OK;
}

/**
 * @brief DMA pause data transferring under non-blocking mode
 *
 * @param hdma Pointer to a DMA_Handle_T structure that contains the configuration information for the specified DMA module.
 * @return HAL_Status_T
 */
HAL_Status_T HAL_DMA_Abort_IT(DMA_Handle_T *hdma)
{
    /* check the DMA handle */
    ASSERT_PARAM(hdma);
    /* check the DMA instance */
    ASSERT_PARAM(IS_PERIPHERAL_DMA_INSTANCE(hdma));

    /* protect the process */
    TAKE_MUTEX(hdma);
    CLEAR_ERROR_CODE(hdma);

    volatile uint32_t *reg_csr = REG_CSR(hdma);
    /* startup abort */
    HAL_REG_SBIT(*reg_csr, DMA_CH_ABORT);
    hdma->state = HAL_STATE_READY;

    /* release the process protection */
    GIVE_MUTEX(hdma);
    return HAL_OK;
}

/**
 * @brief resume the transaction which is abort before
 *
 * @param hdma Pointer to a DMA_Handle_T structure that contains the configuration information for the specified DMA module.
 * @return HAL_Status_T
 */
HAL_Status_T HAL_DMA_Resume_IT(DMA_Handle_T *hdma)
{
    /* check the DMA handle */
    ASSERT_PARAM(hdma);
    /* check the DMA instance */
    ASSERT_PARAM(IS_PERIPHERAL_DMA_INSTANCE(hdma));

    /* protect the process */
    TAKE_MUTEX(hdma);
    HAL_Status_T status = HAL_OK;
    CLEAR_ERROR_CODE(hdma);
    do
    {
        if (hdma->state != HAL_STATE_READY)
        {
            status           = HAL_ERROR;
            hdma->error_code = HAL_DMA_ERROR_NO_XFER;
            break;
        }

        volatile uint32_t *reg_csr = REG_CSR(hdma);

        /* enable current channel to do transferring */
        HAL_REG_SBIT(*reg_csr, HAL_ENABLE);
        hdma->state = HAL_STATE_BUSY;

    } while (0);

    /* release the process protection */
    GIVE_MUTEX(hdma);

    return status;
}

HAL_Status_T HAL_DMA_PollForTransfer(DMA_Handle_T *hdma, uint32_t cplt_lev, uint32_t timeout)
{
    /* check the DMA handle */
    ASSERT_PARAM(hdma);
    /* check the DMA instance */
    ASSERT_PARAM(IS_PERIPHERAL_DMA_INSTANCE(hdma));
    /* check transfer complete level */
    ASSERT_PARAM(IS_DMA_XFER_CPLT(cplt_lev));

    if (hdma->xfer_size == 0)
    {
        return HAL_ERROR;
    }
    CLEAR_ERROR_CODE(hdma);
    volatile uint32_t *reg_size   = REG_SIZE(hdma);
    uint16_t           tick_start = HAL_GetTick(); /* Init tick_start for timeout management */
    if (cplt_lev == DMA_XFER_CPLT_FULL)
    {
        while (1)
        {
            BREAK_PROC_TIMEOUT(hdma, tick_start, timeout);
            if (HAL_REG_READ(*reg_size) == 0)
            {
                hdma->state = HAL_STATE_READY;
                break;
            }
        }
    }
    else
    {
        uint32_t half_size = hdma->xfer_size / 2;

        while (1)
        {
            BREAK_PROC_TIMEOUT(hdma, tick_start, timeout);
            if (HAL_REG_READ(*reg_size) <= half_size)
            {
                break;
            }
        }
    }
    RET_FLAG_TRUE((hdma->error_code == HAL_ERROR_TIMEOUT), HAL_ERROR);
    return HAL_OK;
}
/*
================================================================================
            ##### Event Callback functions #####
================================================================================
*/

/**
 * @brief  DMA Msp Init callback
 * @param  hdma DMA handle
 * @retval None
 */
__weak void HAL_DMA_MspInit(DMA_Handle_T *hdma)
{
    UNUSED(hdma);
    /* NOTE: This function should not be modified, when the callback is needed, the HAL_MspInitCallback could be implemented in the user file */
}

/**
 * @brief  DMA Msp Init callback
 * @param  hdma DMA handle
 * @retval None
 */
__weak void HAL_DMA_MspDeInit(DMA_Handle_T *hdma)
{
    UNUSED(hdma);
    /* NOTE: This function should not be modified, when the callback is needed, the HAL_MspInitCallback could be implemented in the user file */
}

/**
 * @brief  DMA transfer done callback function. this function will be called during external DMA IRQ routine.
 * @param  hdma Pointer to a DMA_Handle_T structure that contains the configuration information for the specified DMA module.
 * @retval None
 */
__weak ATTR_ISR void HAL_DMA_XferCpltCallback(DMA_Handle_T *hdma)
{
    UNUSED(hdma);
    /* NOTE: This function should not be modified, when the callback is needed, the HAL_MspInitCallback could be implemented in the user file */
}

/**
 * @brief  DMA transfer error callback function. this function will be called during external DMA IRQ routine.
 * @param  hdma Pointer to a DMA_Handle_T structure that contains the configuration information for the specified DMA module.
 * @retval None
 */
__weak ATTR_ISR void HAL_DMA_XferErrorCallback(DMA_Handle_T *hdma)
{
    UNUSED(hdma);
    /* NOTE: This function should not be modified, when the callback is needed, the HAL_MspInitCallback could be implemented in the user file */
}

/**
 * @brief  DMA transfer abort success callback function. this function will be called during external DMA IRQ routine.
 * @param  hdma Pointer to a DMA_Handle_T structure that contains the configuration information for the specified DMA module.
 * @retval None
 */
__weak ATTR_ISR void HAL_DMA_XferAbortCallback(DMA_Handle_T *hdma)
{
    UNUSED(hdma);
    /* NOTE: This function should not be modified, when the callback is needed, the HAL_MspInitCallback could be implemented in the user file */
}

/*
================================================================================
            ##### I/O Operation functions #####
================================================================================
*/
/**
 * @brief  This function handles DMA interrupt request.
 * @param  hdma Pointer to a DMA_Handle_T structure that contains the configuration information for the specified DMA module.
 * @retval None
 */
__weak ATTR_ISR void HAL_DMA_IRQHandler(DMA_Handle_T *hdma)
{
    // Check INT_TC[n] or INT_ERR_ABT[n] is set
    hdma->state = HAL_STATE_READY;
    if (IS_DMA_INT_TC(hdma) != 0)
    {
        uint32_t channel = hdma->init.channel;
        HAL_REG_SBIT(hdma->instance->INT_TC_CLR, 1 << channel);
        CLEAR_ERROR_CODE(hdma);
        hdma->XferCpltCallback(hdma);
    }
    else if (IS_DMA_INT_ABT(hdma) != 0)
    {
        uint32_t channel = hdma->init.channel;
        CLEAR_ERROR_CODE(hdma);
        HAL_REG_SBIT(hdma->instance->INT_ERR_ABT_CLR, 1 << (channel + 16));
        hdma->XferAbortCallback(hdma);
    }
    else if (IS_DMA_INT_ERR(hdma) != 0)
    {
        uint32_t channel = hdma->init.channel;
        hdma->error_code = HAL_DMA_ERROR_TE;
        HAL_REG_SBIT(hdma->instance->INT_ERR_ABT_CLR, 1 << channel);
        hdma->XferErrorCallback(hdma);
    }
}

static HAL_Status_T _dma_check_transfer_option(DMA_Handle_T *hdma, uint32_t src_addr)
{
    uint32_t endian;

    /* Check that src width is the same as dst width in circular mode  */
    do
    {
        volatile uint32_t *reg_csr   = REG_CSR(hdma);
        uint32_t           src_width = ((*reg_csr) >> 11) & 0x3;
        uint32_t           dst_width = ((*reg_csr) >> 8) & 0x3;
        uint32_t           src_mode  = ((*reg_csr) >> 5) & 0x3;
        uint32_t           dst_mode  = ((*reg_csr) >> 3) & 0x3;

        if ((DMA_ADDR_MODE_CYCLIC == src_mode) || (DMA_ADDR_MODE_CYCLIC == dst_mode))
        {
            if (src_width == dst_width)
            {
                break;
            }
            else
            {
                return HAL_ERROR;
            }
        }
    } while (0);

    /* Addresses are 4 bytes aligned when dma transfer mode is M2M or M2P and dma transfer mode is big endian or decrement mode*/
    do
    {
        /* Check the dma transfer mode*/
        if ((hdma->init.ch_xfer_mode == DMA_MEMORY_TO_MEMORY) || (hdma->init.ch_xfer_mode == DMA_MEMORY_TO_PERIPH))
        {
            /* Check the dma addr byte-order */
            uint32_t dma_ahb_master = hdma->init.src_ahb_master;
            if (DMA_AHB_MASTER_MATRIX == dma_ahb_master)
            {
                endian = HAL_REG_READ(hdma->instance->MCSR_b.M0ENDIAN) & 0x1;
            }
            else
            {
                endian = HAL_REG_READ(hdma->instance->MCSR_b.M1ENDIAN) & 0x1;
            }

            /* Check the dma addr for the big end or decrement*/
            if ((endian == DMA_AHB_BIG_ENDIAN) || (DMA_ADDR_MODE_DEC == hdma->init.src_addr_mode))
            {
                /* Check whether addr is 4 byte alignment */
                if (0 != (src_addr & 0x3))
                {
                    return HAL_ERROR;
                }
                else
                {
                    return HAL_OK;
                }
            }
        }

    } while (0);

    /* When dma is configured otherwise, the src address memory is aligned with the src_width bytes */
    uint32_t width_mask;
    switch (hdma->init.src_width)
    {
        case DMA_XFR_WIDTH_8BITS:
            width_mask = 0x0;
            break;
        case DMA_XFR_WIDTH_16BITS:
            width_mask = 0x1;
            break;
        case DMA_XFR_WIDTH_32BITS:
            width_mask = 0x3;
            break;
    }
    if (0 != (src_addr & width_mask))
    {
        return HAL_ERROR;
    }
    else
    {
        return HAL_OK;
    }
}

#endif /* configUSE_DMA_PERIPHERAL */
