/***********************
 * SN34F78X SDK V2.1.4 *
 ***********************/

#include "sn34f78x_hal_lcm.h"

#if (configUSE_LCM_PERIPHERAL == 1)

/* Exported functions --------------------------------------------------------*/
/*
================================================================================
            ##### Initialization/De-initialization functions #####
================================================================================
*/
/**
 * @brief  Initializes the LCM mode according to the specified parameters in the LCM_Init_T and create the associated handle.
 * @param  hlcm Pointer to a LCM_Handle_T structure that contains the configuration information for the specified LCM module.
 * @retval HAL status
 */
HAL_Status_T HAL_LCM_Init(LCM_Handle_T *hlcm)
{
    /* check the LCM handle */
    ASSERT_PARAM(hlcm);

    /* check the LCM instance */
    ASSERT_PARAM(IS_LCM_INSTANCE(hlcm));

    /* check the LCM panel */
    ASSERT_PARAM(IS_LCM_PANEL(hlcm));

    /* check the LCM bus width */
    ASSERT_PARAM(IS_LCM_BUS_WIDTH(hlcm));

    /* check the LCM transfer mode */
    ASSERT_PARAM(IS_LCM_XFER_MODE(hlcm));

    /* check whether is already initialized */
    if (HAL_FLAG_EQU(hlcm->state, HAL_STATE_RESET))
    {
/* the LCM clock and reset should be setup in MspDeInitCallback under user project */
#if (configUSE_LCM_REGISTER_CALLBACKS == 1)
        if (hlcm->MspInitCallback == NULL)
        {
            hlcm->MspInitCallback = HAL_LCM_MspInit;
        }
        if (hlcm->XferErrorCallback == NULL)
        {
            hlcm->XferErrorCallback = HAL_LCM_XferErrorCallback;
        }
        if (hlcm->XferCpltCallback == NULL)
        {
            hlcm->XferCpltCallback = HAL_LCM_XferCpltCallback;
        }
        if (hlcm->XferAbortCallback == NULL)
        {
            hlcm->XferAbortCallback = HAL_LCM_XferAbortCallback;
        }
        hlcm->MspInitCallback(hlcm);
#else
        HAL_LCM_MspInit(hlcm);
#endif /* configUSE_LCM_REGISTER_CALLBACKS */

#if (configUSE_LCM_NONBLOCKING_DMA == 1U)
        /* register DAM LCM CH callback function */
        if (hlcm->hdma)
        {
            hlcm->hdma->parent = (void *)hlcm;
            HAL_DMA_RegisterCallback(hlcm->hdma, HAL_DMA_XFER_CPLT_CB_ID, _lcm_dma_xfer_cplt_callback);
            HAL_DMA_RegisterCallback(hlcm->hdma, HAL_DMA_XFER_ABORT_CB_ID, _lcm_dma_xfer_abort_callback);
            HAL_DMA_RegisterCallback(hlcm->hdma, HAL_DMA_XFER_ERROR_CB_ID, _lcm_dma_xfer_error_callback);
        }
#endif
        /* init lcm panel */
        HAL_REG_WRITE(hlcm->instance->MODE_b.PANEL, hlcm->init.panel);
        /* init lcm bus width */
        HAL_REG_WRITE(hlcm->instance->MODE_b.BUSWIDTH, hlcm->init.bus_width);
        /* init lcm transfer mode */
        HAL_REG_WRITE(hlcm->instance->MODE_b.TRANSMODE, hlcm->init.xfer_mode);

        /* dam default setting */
        HAL_REG_WRITE(hlcm->instance->MODE_b.DMAFORMAT, 0x30);
        HAL_REG_WRITE(hlcm->instance->DMACFG_b.REQSRC, 0x4);
        HAL_REG_WRITE(hlcm->instance->DMACFG_b.WATERMARK, 0x4);

        /* default timing setting */
        HAL_REG_WRITE(hlcm->instance->TMCTRL_b.TPWHW, 0x3);
        HAL_REG_WRITE(hlcm->instance->TMCTRL_b.TPWL, 0x1);
        HAL_REG_WRITE(hlcm->instance->TMCTRL_b.TAH, 0x1);
        HAL_REG_WRITE(hlcm->instance->TMCTRL_b.TAS, 0x1);
        HAL_REG_WRITE(hlcm->instance->TMCTRL_b.TPWHR, 0x3);

        /* default under CPU mode */
        HAL_REG_WRITE(hlcm->instance->ENABLE_b.DMAEN, HAL_DISABLE);

        /* check clock */
        uint32_t clk     = 0;
        uint32_t clk_sel = __HAL_RCC_GET_LCM_CLKSEL();
        if (clk_sel == RCC_LCM_CLKSEL_HCLK)
        {
            clk = HAL_RCC_GetHCLKFreq();
        }
        else if (clk_sel == RCC_LCM_CLKSEL_PLLCLK)
        {
            clk = HAL_RCC_GetPLLCLKFreq();
        }
        if (clk > LCM_SRC_CLK_MAX)
        {
            /* Set LCM clock prescaler when source clock frequency > 148M */
            HAL_REG_WRITE(SN_SCU->PRECTRL_b.LCMPRE, LCM_CLK_PRE);
        }

        /* enable LCM */
        HAL_REG_WRITE(hlcm->instance->ENABLE_b.LCMEN, HAL_ENABLE);

        /* update state to ready */
        CLEAR_ERROR_CODE(hlcm);
        hlcm->state = HAL_STATE_READY;

        /* init mutex */
        GIVE_MUTEX(hlcm);
    }
    else
    {
        /* update the state */
        hlcm->error_code = HAL_ERROR_REINIT;
        return HAL_ERROR;
    }

    return HAL_OK;
}

/**
 * @brief  DeInitializes the LCM peripheral.
 * @param  hlcm Pointer to a LCM_Handle_T structure that contains the configuration information for the specified LCM module.
 * @retval HAL status
 */
HAL_Status_T HAL_LCM_DeInit(LCM_Handle_T *hlcm)
{
    /* check the LCM handle */
    ASSERT_PARAM(hlcm);
    /* check the LCM instance */
    ASSERT_PARAM(IS_LCM_INSTANCE(hlcm));

    /* LCM is not running */
    CLEAR_ERROR_CODE(hlcm);

    TAKE_MUTEX(hlcm);

    /* default under CPU mode */
    HAL_REG_WRITE(hlcm->instance->ENABLE_b.DMAEN, HAL_DISABLE);

    /* disable LCM */
    HAL_REG_WRITE(hlcm->instance->ENABLE_b.LCMEN, HAL_DISABLE);

/* the LCM clock and reset should be setup in MspDeInitCallback under user project */
#if (configUSE_LCM_REGISTER_CALLBACKS == 1)
    if (hlcm->MspDeInitCallback == NULL)
    {
        hlcm->MspDeInitCallback = HAL_LCM_MspDeInit;
    }

    hlcm->MspDeInitCallback(hlcm);
#else
    HAL_LCM_MspDeInit(hlcm);
#endif /* configUSE_LCM_REGISTER_CALLBACKS */

#if (configUSE_LCM_NONBLOCKING_DMA == 1U)
    if (hlcm->hdma)
    {
        hlcm->hdma->parent = NULL;
        HAL_DMA_UnRegisterCallback(hlcm->hdma, HAL_DMA_ALL_CB_ID);
    }
#endif

#if (configUSE_LCM_REGISTER_CALLBACKS == 1)
    /* reset LCM all callback */
    _reset_callback(hlcm);
#endif

    /* update the state */
    CLEAR_ERROR_CODE(hlcm);
    hlcm->state = HAL_STATE_RESET;

    /* release the process protection */
    GIVE_MUTEX(hlcm);

    return HAL_OK;
}

/*
================================================================================
            ##### Callbacks Register/UnRegister functions #####
================================================================================
*/
#if (configUSE_LCM_REGISTER_CALLBACKS == 1)
/**
 * @brief  Register a User LCM callback to be used instead of the weak predefined callback.
 * @param  hlcm Pointer to a LCM_Handle_T structure that contains the configuration information for the specified LCM module.
 * @param  callback_id ID of the callback to be registered.
 *         This parameter can be one of the following values:
 *           @arg @ref HAL_LCM_TIMEOUT_CB_ID LCM Timeout Callback ID
 * @param  callback pointer to the Callback function
 * @retval HAL status
 */
HAL_Status_T HAL_LCM_RegisterCallback(LCM_Handle_T *hlcm, HAL_LCM_CallbackID_T callback_id, LCM_Callback_T callback)
{
    /* check the LCM handle */
    ASSERT_PARAM(hlcm);
    /* check the callback function */
    ASSERT_PARAM(callback);
    /* check the LCM instance */
    ASSERT_PARAM(IS_LCM_INSTANCE(hlcm));
    /* check the callback id */
    ASSERT_PARAM(IS_LCM_CALLBACK_ID(callback_id));

    HAL_Status_T status = HAL_OK;

    /* protect the process */
    TAKE_MUTEX(hlcm);

    switch (callback_id)
    {
        case HAL_LCM_MSPINIT_CB_ID:
        {
            hlcm->MspInitCallback = callback;
        }
        break;
        case HAL_LCM_MSPDEINIT_CB_ID:
        {
            hlcm->MspDeInitCallback = callback;
        }
        break;
        case HAL_LCM_XFER_CPLT_CB_ID:
        {
            hlcm->XferCpltCallback = callback;
        }
        break;
        case HAL_LCM_XFER_ERROR_CB_ID:
        {
            hlcm->XferErrorCallback = callback;
        }
        break;
        case HAL_LCM_XFER_ABORT_CB_ID:
        {
            hlcm->XferAbortCallback = callback;
        }
        break;
        default:
        {
            status = HAL_ERROR;
        }
        break;
    }

    /* release the process protection */
    GIVE_MUTEX(hlcm);
    return status;
}

/**
 * @brief  Unregister an LCM callback, LCM callback is redirected to the weak predefined callback.
 * @param  hlcm Pointer to a LCM_Handle_T structure that contains the configuration information for the specified LCM module.
 * @param  callback_id ID of the callback to be unregistered.
 *         This parameter can be one of the following values:
 *           @arg @ref HAL_LCM_TIMEOUT_CB_ID LCM Timeout Callback ID
 * @retval HAL status
 */
HAL_Status_T HAL_LCM_UnRegisterCallback(LCM_Handle_T *hlcm, HAL_LCM_CallbackID_T callback_id)
{
    /* check the LCM handle */
    ASSERT_PARAM(hlcm);
    /* check the LCM instance */
    ASSERT_PARAM(IS_LCM_INSTANCE(hlcm));
    /* check the callback id */
    ASSERT_PARAM(IS_LCM_CALLBACK_ID(callback_id));

    HAL_Status_T status = HAL_OK;

    /* protect the process */
    TAKE_MUTEX(hlcm);

    switch (callback_id)
    {
        case HAL_LCM_MSPINIT_CB_ID:
        {
            hlcm->MspInitCallback = HAL_LCM_MspInit;
        }
        break;
        case HAL_LCM_MSPDEINIT_CB_ID:
        {
            hlcm->MspDeInitCallback = HAL_LCM_MspDeInit;
        }
        break;
        case HAL_LCM_XFER_CPLT_CB_ID:
        {
            hlcm->XferCpltCallback = HAL_LCM_XferCpltCallback;
        }
        break;
        case HAL_LCM_XFER_ERROR_CB_ID:
        {
            hlcm->XferErrorCallback = HAL_LCM_XferErrorCallback;
        }
        break;
        case HAL_LCM_XFER_ABORT_CB_ID:
        {
            hlcm->XferAbortCallback = HAL_LCM_XferAbortCallback;
        }
        break;
        case HAL_LCM_ALL_CB_ID:
        {
            hlcm->MspInitCallback   = HAL_LCM_MspInit;
            hlcm->MspDeInitCallback = HAL_LCM_MspDeInit;
            hlcm->XferCpltCallback  = HAL_LCM_XferCpltCallback;
            hlcm->XferErrorCallback = HAL_LCM_XferErrorCallback;
            hlcm->XferAbortCallback = HAL_LCM_XferAbortCallback;
        }
        break;
        default:
        {
            status = HAL_ERROR;
        }
        break;
    }

    /* release the process protection */
    GIVE_MUTEX(hlcm);

    return status;
}
#endif /* configUSE_LCM_REGISTER_CALLBACKS */

/*
================================================================================
            ##### Peripheral State and Errors functions #####
================================================================================
*/
/**
 * @brief  Returns the LCM state.
 * @param  hlcm Pointer to a LCM_Handle_T structure that contains the configuration information for the specified LCM module.
 * @retval HAL state
 */
HAL_State_T HAL_LCM_GetState(LCM_Handle_T *hlcm)
{
    /* check the LCM handle */
    ASSERT_PARAM(hlcm);
    /* check the LCM instance */
    ASSERT_PARAM(IS_LCM_INSTANCE(hlcm));

    return hlcm->state;
}

/**
 * @brief  Return the LCM error code
 * @param  hlcm Pointer to a LCM_Handle_T structure that contains the configuration information for the specified LCM module.
 * @retval LCM error code
 */
uint32_t HAL_LCM_GetError(LCM_Handle_T *hlcm)
{
    /* check the LCM handle */
    ASSERT_PARAM(hlcm);
    /* check the LCM instance */
    ASSERT_PARAM(IS_LCM_INSTANCE(hlcm));

    return hlcm->error_code;
}

/*
================================================================================
            ##### Peripheral Control functions #####
================================================================================
*/

/**
 * @brief LCM write cmd under cpu mode
 *
 * @param hlcm Pointer to a LCM_Handle_T structure that contains the configuration information for the specified LCM module.
 * @param cmd Command that need LCM to send out
 * @return HAL_Status_T
 */
HAL_Status_T HAL_LCM_WriteCmd(LCM_Handle_T *hlcm, uint32_t cmd)
{
    /* check the LCM handle */
    ASSERT_PARAM(hlcm);
    /* check the LCM instance */
    ASSERT_PARAM(IS_LCM_INSTANCE(hlcm));

    RET_STATE_NOT_READY(hlcm);

    /* protect the process */
    TAKE_MUTEX(hlcm);

    LCM_WAIT_RDY(hlcm);
    LCM_SEND_CMD(hlcm, cmd);

    /* release the process protection */
    GIVE_MUTEX(hlcm);
    return HAL_OK;
}
/**
 * @brief LCM write data under cpu mode
 *
 * @param hlcm
 * @param data
 * @return HAL_Status_T
 */
HAL_Status_T HAL_LCM_WriteData(LCM_Handle_T *hlcm, uint32_t data)
{
    /* check the LCM handle */
    ASSERT_PARAM(hlcm);
    /* check the LCM instance */
    ASSERT_PARAM(IS_LCM_INSTANCE(hlcm));

    RET_STATE_NOT_READY(hlcm);

    /* protect the process */
    TAKE_MUTEX(hlcm);

    /* third write data */
    LCM_WAIT_RDY(hlcm);
    LCM_WRITE_DATA(hlcm, data);

    /* release the process protection */
    GIVE_MUTEX(hlcm);

    return HAL_OK;
}

/**
 * @brief LCM read register data under cpu mode
 *
 * @param hlcm Pointer to a LCM_Handle_T structure that contains the configuration information for the specified LCM module.
 * @param reg specify which register will be read from
 * @param data the register value will be stored there
 * @return HAL_Status_T
 */

HAL_Status_T HAL_LCM_ReadReg(LCM_Handle_T *hlcm, uint32_t reg, uint32_t *data)
{
    /* check the LCM handle */
    ASSERT_PARAM(hlcm);
    /* check the LCM instance */
    ASSERT_PARAM(IS_LCM_INSTANCE(hlcm));
    /* check the register address */
    ASSERT_PARAM(reg);

    RET_STATE_NOT_READY(hlcm);

    /* protect the process */
    TAKE_MUTEX(hlcm);

    /* first send register address */
    LCM_WAIT_RDY(hlcm);
    LCM_SEND_CMD(hlcm, reg);

    /* second change to read data mode */
    LCM_WAIT_RDY(hlcm);
    HAL_REG_WRITE(hlcm->instance->RS, 1);

    /* third read data */
    LCM_WAIT_RDY(hlcm);
    *data = LCM_READ_DATA(hlcm);

    /* release the process protection */
    GIVE_MUTEX(hlcm);

    return HAL_OK;
}

/**
 * @brief LCM write register data under cpu mode
 *
 * @param hlcm Pointer to a LCM_Handle_T structure that contains the configuration information for the specified LCM module.
 * @param reg specify which register will be written to.
 * @param data specify which data will be update to current register.
 * @return HAL_Status_T
 */
HAL_Status_T HAL_LCM_WriteReg(LCM_Handle_T *hlcm, uint32_t reg, uint32_t data)
{
    /* check the LCM handle */
    ASSERT_PARAM(hlcm);
    /* check the LCM instance */
    ASSERT_PARAM(IS_LCM_INSTANCE(hlcm));
    /* check the register address */
    ASSERT_PARAM(reg);

    RET_STATE_NOT_READY(hlcm);

    /* protect the process */
    TAKE_MUTEX(hlcm);

    /* first send cmd */
    LCM_WAIT_RDY(hlcm);
    LCM_SEND_CMD(hlcm, reg);

    /* third write data */
    LCM_WAIT_RDY(hlcm);
    LCM_WRITE_DATA(hlcm, data);

    /* release the process protection */
    GIVE_MUTEX(hlcm);

    return HAL_OK;
}

#if (configUSE_LCM_NONBLOCKING_DMA == 1U)

/**
 * @brief by external dma to transfer data on LCM bus
 *
 * @param hlcm Pointer to a LCM_Handle_T structure that contains the configuration information for the specified LCM module.
 * @param src_addr the source address for LCM dam to read from
 * @param data_len the data length for current transfer
 * @return HAL_Status_T
 */
HAL_Status_T HAL_LCM_WriteData_DMA(LCM_Handle_T *hlcm, uint32_t src_addr, uint16_t data_len)
{
    /* check the LCM handle could not be NULL */
    ASSERT_PARAM(hlcm);
    /* check the valid of LCM instance */
    ASSERT_PARAM(IS_LCM_INSTANCE(hlcm));
    /* check the source address could not be NULL */
    ASSERT_PARAM(src_addr);
    /* check the data length could not be 0 */
    ASSERT_PARAM(data_len);

    RET_STATE_NOT_READY(hlcm);

    HAL_FLAG_WRITE(hlcm->state, HAL_STATE_BUSY);

    uint32_t dst_addr = (uint32_t)(&(hlcm->instance->DMADAT));
    return _lcm_dma_write_kernal(hlcm, src_addr, dst_addr, data_len);
}

/**
 * @brief by external dam to transfer command on LCM bus
 *
 * @param hlcm Pointer to a LCM_Handle_T structure that contains the configuration information for the specified LCM module.
 * @param src_addr the source address for LCM dam to read from
 * @param cmd_len the cmd length for current transfer
 * @return HAL_Status_T
 */
HAL_Status_T HAL_LCM_WriteCmd_DMA(LCM_Handle_T *hlcm, uint32_t src_addr, uint16_t cmd_len)
{
    /* check the LCM handle */
    ASSERT_PARAM(hlcm);
    /* check the LCM instance */
    ASSERT_PARAM(IS_LCM_INSTANCE(hlcm));
    /* check the source address could not be NULL */
    ASSERT_PARAM(src_addr);
    /* check the command length could not be 0 */
    ASSERT_PARAM(cmd_len);

    RET_STATE_NOT_READY(hlcm);

    uint32_t dst_addr = (uint32_t)(&(hlcm->instance->DMACMD));
    return _lcm_dma_write_kernal(hlcm, src_addr, dst_addr, cmd_len);
}

/**
 * @brief pause current transfer
 *
 * @param hlcm Pointer to a LCM_Handle_T structure that contains the configuration information for the specified LCM module.
 * @return HAL_Status_T
 */
HAL_Status_T HAL_LCM_DMAPause(LCM_Handle_T *hlcm)
{
    /* check the LCM handle */
    ASSERT_PARAM(hlcm);
    /* check the LCM instance */
    ASSERT_PARAM(IS_LCM_INSTANCE(hlcm));

    RET_FLAG_FALSE(HAL_FLAG_LACKS(hlcm->state, HAL_STATE_BUSY), HAL_ERROR);
    /* abort LCM DMA */
    HAL_REG_WRITE(hlcm->instance->ENABLE_b.DMAEN, HAL_DISABLE);

    return HAL_DMA_Abort_IT(hlcm->hdma);
}

/**
 * @brief resume current transfer
 *
 * @param hlcm
 * @return HAL_Status_T
 */
HAL_Status_T HAL_LCM_DMAResume(LCM_Handle_T *hlcm)
{
    /* check the LCM handle */
    ASSERT_PARAM(hlcm);
    /* check the LCM instance */
    ASSERT_PARAM(IS_LCM_INSTANCE(hlcm));

    RET_STATE_NOT_READY(hlcm);

    /* resume LCM DMA */
    HAL_REG_WRITE(hlcm->instance->ENABLE_b.DMAEN, HAL_ENABLE);
    /* check the source address could not be NULL */
    return HAL_DMA_Resume_IT(hlcm->hdma);
}

/**
 * @brief stop current transfer. if need to transfer again, the \ref HAL_LCM_WriteData_DMA or \ref HAL_LCM_WriteCmd_DMA
 *          should be called again.
 * @param hlcm Pointer to a LCM_Handle_T structure that contains the configuration information for the specified LCM module.
 * @return HAL_Status_T
 */
HAL_Status_T HAL_LCM_DMAStop(LCM_Handle_T *hlcm)
{
    /* check the LCM handle */
    ASSERT_PARAM(hlcm);
    /* check the LCM instance */
    ASSERT_PARAM(IS_LCM_INSTANCE(hlcm));
    /* check the source address could not be NULL */

    RET_FLAG_FALSE(HAL_FLAG_LACKS(hlcm->state, HAL_STATE_BUSY), HAL_ERROR);

    HAL_DMA_Abort_IT(hlcm->hdma);

    HAL_REG_WRITE(hlcm->instance->DMAFIFOCTRL_b.FIFOCLR, 1);
    LCM_WAIT_FIFO_CLR(hlcm);

    return HAL_OK;
}

/**
 * @brief the kernel implement for calling extern dma to do transferring.
 *
 * @param hlcm Pointer to a LCM_Handle_T structure that contains the configuration information for the specified LCM module.
 * @param src_addr the source address for DMA to access
 * @param dst_addr the destination address for DMA to access; there the dst_address is DMADAT or DMACMD
 * @param data_len the data length that the dma will transfer
 * @return HAL_Status_T
 */
static HAL_Status_T _lcm_dma_write_kernal(LCM_Handle_T *hlcm, uint32_t src_addr, uint32_t dst_addr, uint16_t data_len)
{
    // problem3: LCM whether support DMA READ Interface?

    /* check the LCM handle */
    ASSERT_PARAM(hlcm);
    /* check the valid of LCM instance */
    ASSERT_PARAM(IS_LCM_INSTANCE(hlcm));
    /* check the valid of source address */
    ASSERT_PARAM(src_addr);
    /* check the valid of data_len */
    ASSERT_PARAM(data_len);

    /* before dma tx starting, the dam tx channel should be config under use APP*/
    ASSERT_PARAM(hlcm->hdma);

    CLEAR_ERROR_CODE(hlcm);

    /* protect the process */
    TAKE_MUTEX(hlcm);

    /* the work mode: DMA mode */
    HAL_REG_WRITE(hlcm->instance->ENABLE_b.DMAEN, HAL_ENABLE);

    // /* before the dma running, first clear all the IS */
    // HAL_REG_WRITE(hlcm->instance->DMAIS, LCM_CLEAR_ALLIS);

    // /* enbale interrupt */
    // HAL_REG_WRITE(hlcm->instance->DMAIE, LCM_ENABLE_ALLIE);

    /* clear tx fifo */
    HAL_REG_WRITE(hlcm->instance->DMAFIFOCTRL_b.FIFOCLR, 1);
    LCM_WAIT_FIFO_CLR(hlcm);

    /* start tx process, the end flag should be checked under DMA callback routine*/

    HAL_Status_T status = HAL_DMA_Start_IT(hlcm->hdma, src_addr, dst_addr, data_len);
    if (status != HAL_OK)
    {
        hlcm->error_code = HAL_LCM_ERROR_DMAERR;
    }

    /* release mutex signal */
    GIVE_MUTEX(hlcm);

    return status;
}

#endif

/*
================================================================================
            ##### Event Callback functions #####
================================================================================
*/

/**
 * @brief  LCM Msp Init callback
 * @param  hlcm LCM handle
 * @retval None
 */
__weak void HAL_LCM_MspInit(LCM_Handle_T *hlcm)
{
    /* check the LCM handle */
    ASSERT_PARAM(hlcm);
    /* check the WWDG instance */
    ASSERT_PARAM(IS_LCM_INSTANCE(hlcm));
    /* Prevent unused argument(s) compilation warning */
    UNUSED(hlcm);

    /* NOTE : This function should not be modified, when the callback is needed, the HAL_MspInitCallback could be implemented in the user file */
}

/**
 * @brief  LCM Msp DeInit callback
 * @param  hlcm LCM handle
 * @retval None
 */
__weak void HAL_LCM_MspDeInit(LCM_Handle_T *hlcm)
{
    /* check the LCM handle */
    ASSERT_PARAM(hlcm);
    /* Prevent unused argument(s) compilation warning */
    UNUSED(hlcm);

    /* NOTE : This function should not be modified, when the callback is needed, the HAL_MspDeInitCallback could be implemented in the user file */
}

/**
 * @brief  LCM transfer done callback function. this function will be called during external DMA IRQ routine.
 * @param  hlcm Pointer to a LCM_Handle_T structure that contains the configuration information for the specified LCM module.
 * @retval None
 */
__weak ATTR_ISR void HAL_LCM_XferCpltCallback(LCM_Handle_T *hlcm)
{
    /* check the LCM handle */
    ASSERT_PARAM(hlcm);
    /* check the LCM instance */
    ASSERT_PARAM(IS_LCM_INSTANCE(hlcm));
    /* prevent unused argument(s) compilation warning */
    UNUSED(hlcm);
    /* NOTE: This function should not be modified, when the callback is needed, the HAL_LCM_XferCpltCallback could be implemented in the user file */
}

/**
 * @brief  LCM error callback during transfer proc.
 * @param  hlcm Pointer to a LCM_Handle_T structure that contains the configuration information for the specified LCM module.
 * @retval None
 */
__weak ATTR_ISR void HAL_LCM_XferErrorCallback(LCM_Handle_T *hlcm)
{
    /* check the LCM handle */
    ASSERT_PARAM(hlcm);
    /* check the LCM instance */
    ASSERT_PARAM(IS_LCM_INSTANCE(hlcm));
    /* prevent unused argument(s) compilation warning */
    UNUSED(hlcm);
    /* NOTE: This function should not be modified, when the callback is needed, the HAL_LCM_XferErrorCallback could be implemented in the user file */
}

/**
 * @brief  LCM abort success callback during transfer proc.
 * @param  hlcm Pointer to a LCM_Handle_T structure that contains the configuration information for the specified LCM module.
 * @retval None
 */
__weak ATTR_ISR void HAL_LCM_XferAbortCallback(LCM_Handle_T *hlcm)
{
    /* check the LCM handle */
    ASSERT_PARAM(hlcm);
    /* check the LCM instance */
    ASSERT_PARAM(IS_LCM_INSTANCE(hlcm));
    /* prevent unused argument(s) compilation warning */
    UNUSED(hlcm);
    /* NOTE: This function should not be modified, when the callback is needed, the HAL_LCM_XferAbortCallback could be implemented in the user file */
}

/*
================================================================================
            ##### I/O Operation functions #####
================================================================================
*/
/**
 * @brief  This function handles LCM interrupt request.
 * @param  hlcm Pointer to a LCM_Handle_T structure that contains the configuration information for the specified LCM module.
 * @retval None
 */
ATTR_ISR void HAL_LCM_IRQHandler(LCM_Handle_T *hlcm)
{
    /* read LCM status */
    uint32_t dmais = HAL_REG_READ(hlcm->instance->DMAIS) & LCM_IS_IFMASK;

    /* clear LCM status */
    HAL_REG_WRITE(hlcm->instance->DMAIS, dmais);

    CLEAR_ERROR_CODE(hlcm);
    hlcm->state = HAL_STATE_READY;
    if (HAL_FLAG_HAS(dmais, LCM_IS_DMAERR))
    {
        hlcm->error_code = HAL_LCM_ERROR_DMAERR;
    }
    else if (HAL_FLAG_HAS(dmais, LCM_IS_UDRUN))
    {
        hlcm->error_code = HAL_LCM_ERROR_UDRUN;
    }
    else if (HAL_FLAG_HAS(dmais, LCM_IS_DATCNTE))
    {
        hlcm->error_code = HAL_LCM_ERROR_DATCNTE;
    }

#if (configUSE_LCM_REGISTER_CALLBACKS == 1)
    /* call registered callback */
    hlcm->XferErrorCallback(hlcm);
#else
    /* Call legacy weak timeout callback */
    HAL_LCM_XferErrorCallback(hlcm);
#endif /* configUSE_LCM_REGISTER_CALLBACKS */
}

#if (configUSE_LCM_REGISTER_CALLBACKS == 1)
/**
 * @brief  Reset LCM all callback
 * @param  hlcm Pointer to a LCM_Handle_T structure that contains the configuration information for the specified LCM module.
 * @retval None
 */
static void _reset_callback(LCM_Handle_T *hlcm)
{
    hlcm->MspInitCallback   = HAL_LCM_MspInit;
    hlcm->MspDeInitCallback = HAL_LCM_MspDeInit;
    hlcm->XferCpltCallback  = HAL_LCM_XferCpltCallback;
    hlcm->XferErrorCallback = HAL_LCM_XferErrorCallback;
    hlcm->XferAbortCallback = HAL_LCM_XferAbortCallback;
}
#endif /* configUSE_LCM_REGISTER_CALLBACKS */

#if (configUSE_LCM_NONBLOCKING_DMA == 1U)
/**
 * @brief  LCM transfer done callback function. this function will be called during external DMA IRQ routine.
 * @param  hdam Pointer to a DMA_Handle_T structure that contains the configuration information for the specified DMA module.
 * @retval None
 */
static ATTR_ISR void _lcm_dma_xfer_cplt_callback(DMA_Handle_T *hdam)
{
    /* check the LCM handle */
    ASSERT_PARAM(hdam);

    LCM_Handle_T *hlcm = (LCM_Handle_T *)hdam->parent;
    hlcm->state        = HAL_STATE_READY;
    CLEAR_ERROR_CODE(hlcm);

    /* default under CPU mode */
    HAL_REG_WRITE(hlcm->instance->ENABLE_b.DMAEN, HAL_DISABLE);

#if (configUSE_LCM_REGISTER_CALLBACKS == 1)
    /* call registered callback */
    hlcm->XferCpltCallback(hlcm);
#else
    /* Call legacy weak timeout callback */
    HAL_LCM_XferCpltCallback(hlcm);
#endif /* configUSE_LCM_REGISTER_CALLBACKS */
}

/**
 * @brief  LCM transfer pause callback function. this function will be called during external DMA IRQ routine.
 * @param  hdam Pointer to a DMA_Handle_T structure that contains the configuration information for the specified DMA module.
 * @retval None
 */
static ATTR_ISR void _lcm_dma_xfer_abort_callback(DMA_Handle_T *hdam)
{
    /* check the LCM handle */
    ASSERT_PARAM(hdam);

    LCM_Handle_T *hlcm = (LCM_Handle_T *)hdam->parent;
    hlcm->state        = HAL_STATE_READY;
    CLEAR_ERROR_CODE(hlcm);

    /* default under CPU mode */
    HAL_REG_WRITE(hlcm->instance->ENABLE_b.DMAEN, HAL_DISABLE);

#if (configUSE_LCM_REGISTER_CALLBACKS == 1)
    /* call registered callback */
    hlcm->XferAbortCallback(hlcm);
#else
    /* Call legacy weak timeout callback */
    HAL_LCM_XferAbortCallback(hlcm);
#endif /* configUSE_LCM_REGISTER_CALLBACKS */
}

/**
 * @brief  LCM transfer error callback function. this function will be called during external DMA IRQ routine.
 * @param  hdam Pointer to a DMA_Handle_T structure that contains the configuration information for the specified DMA module.
 * @retval None
 */
static ATTR_ISR void _lcm_dma_xfer_error_callback(DMA_Handle_T *hdam)
{
    /* check the LCM handle */
    ASSERT_PARAM(hdam);

    LCM_Handle_T *hlcm = (LCM_Handle_T *)hdam->parent;
    hlcm->state        = HAL_STATE_READY;
    hlcm->error_code   = HAL_DMA_GetError(hdam);

    /* default under CPU mode */
    HAL_REG_WRITE(hlcm->instance->ENABLE_b.DMAEN, HAL_DISABLE);

#if (configUSE_LCM_REGISTER_CALLBACKS == 1)
    /* call registered callback */
    hlcm->XferErrorCallback(hlcm);
#else
    /* Call legacy weak timeout callback */
    HAL_LCM_XferErrorCallback(hlcm);
#endif /* configUSE_LCM_REGISTER_CALLBACKS */
}

#endif

#endif /* configUSE_LCM_PERIPHERAL */
