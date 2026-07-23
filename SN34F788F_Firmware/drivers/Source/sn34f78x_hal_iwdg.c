/***********************
 * SN34F78X SDK V2.1.4 *
 ***********************/

#include "sn34f78x_hal_iwdg.h"

#if (configUSE_IWDG_PERIPHERAL == 1)

/* Exported functions --------------------------------------------------------*/
/*
================================================================================
            ##### Initialization/De-initialization functions #####
================================================================================
*/
/**
 * @brief  Initializes the IWDG mode according to the specified parameters in the IWDG_Init_T and create the associated handle.
 * @param  hiwdg Pointer to a IWDG_Handle_T structure that contains the configuration information for the specified IWDG module.
 * @retval HAL status
 */
HAL_Status_T HAL_IWDG_Init(IWDG_Handle_T *hiwdg)
{
    /* check the IWDG handle */
    ASSERT_PARAM(hiwdg);
    /* check the IWDG instance */
    ASSERT_PARAM(IS_IWDG_INSTANCE(hiwdg));

    /* check whether is already initialized */
    if (HAL_FLAG_EQU(hiwdg->state, HAL_STATE_RESET))
    {
        /* check the parameters */
        RET_FLAG_FALSE(_iwdg_check_config(hiwdg) == HAL_OK, HAL_ERROR);

#if (configUSE_IWDG_REGISTER_CALLBACKS == 1)
        if (hiwdg->MspInitCallback == NULL)
        {
            hiwdg->MspInitCallback = HAL_IWDG_MspInit;
        }
        if (hiwdg->TimeoutCallback == NULL)
        {
            hiwdg->TimeoutCallback = HAL_IWDG_TimeoutCallback;
        }
        hiwdg->MspInitCallback(hiwdg);
#else
        HAL_IWDG_MspInit(hiwdg);
#endif /* configUSE_IWDG_REGISTER_CALLBACKS */

        /* Delay to wait for the clock to be ready */
        uint32_t sys_clk = HAL_RCC_GetHCLKFreq();
        for (uint32_t cnt = 0; cnt < sys_clk / 100000; cnt++)
        {
            __NOP();
        }

        /* config IWDG */
        RET_FLAG_FALSE(_iwdg_set_config(hiwdg) == HAL_OK, HAL_ERROR);
        /* update the state */
        CLEAR_ERROR_CODE(hiwdg);
        hiwdg->state = HAL_STATE_BUSY;

        return HAL_OK;
    }
    else
    {
        hiwdg->error_code = HAL_ERROR_REINIT;
        return HAL_ERROR;
    }
}

/**
 * @brief  DeInitializes the IWDG peripheral.
 * @param  hiwdg Pointer to a IWDG_Handle_T structure that contains the configuration information for the specified IWDG module.
 * @retval HAL status
 */
HAL_Status_T HAL_IWDG_DeInit(IWDG_Handle_T *hiwdg)
{
    /* check the IWDG handle */
    ASSERT_PARAM(hiwdg);
    /* check the IWDG instance */
    ASSERT_PARAM(IS_IWDG_INSTANCE(hiwdg));

    /* IWDG is not running */
    if (hiwdg->state != HAL_STATE_BUSY)
    {
        return HAL_OK;
    }

    /* disable IWDG */
    HAL_REG_CBIT(hiwdg->instance->CTRL, IWDG_EN_MASK);

#if (configUSE_IWDG_REGISTER_CALLBACKS == 1)
    if (hiwdg->MspDeInitCallback == NULL)
    {
        hiwdg->MspDeInitCallback = HAL_IWDG_MspDeInit;
    }

    hiwdg->MspDeInitCallback(hiwdg);
#else
    HAL_IWDG_MspDeInit(hiwdg);
#endif /* configUSE_IWDG_REGISTER_CALLBACKS */

    /* update the state */
    CLEAR_ERROR_CODE(hiwdg);
    hiwdg->state = HAL_STATE_RESET;

    /* release the process protection */
    GIVE_MUTEX(hiwdg);

    return HAL_OK;
}

/**
 * @brief  IWDG Msp Init callback
 * @param  hiwdg IWDG handle
 * @retval None
 */
__weak void HAL_IWDG_MspInit(IWDG_Handle_T *hiwdg)
{
    /* Prevent unused argument(s) compilation warning */
    UNUSED(hiwdg);
    /* NOTE: This function should not be modified, when the callback is needed, the HAL_MspInitCallback could be implemented in the user file */
}

/**
 * @brief  IWDG Msp DeInit callback
 * @param  hiwdg IWDG handle
 * @retval None
 */
__weak void HAL_IWDG_MspDeInit(IWDG_Handle_T *hiwdg)
{
    /* Prevent unused argument(s) compilation warning */
    UNUSED(hiwdg);
    /* NOTE: This function should not be modified, when the callback is needed,the HAL_MspDeInitCallback could be implemented in the user file */
}
/*
================================================================================
            ##### Callbacks Register/UnRegister functions #####
================================================================================
*/
#if (configUSE_IWDG_REGISTER_CALLBACKS == 1)
/**
 * @brief  Register a User IWDG callback to be used instead of the weak predefined callback.
 * @param  hiwdg Pointer to a IWDG_Handle_T structure that contains the configuration information for the specified IWDG module.
 * @param  callback_id ID of the callback to be registered.
 *         This parameter can be one of the following values:
 *           @arg @ref HAL_IWDG_TIMEOUT_CB_ID IWDG Timeout Callback ID
 * @param  callback pointer to the Callback function
 * @retval HAL status
 */
HAL_Status_T HAL_IWDG_RegisterCallback(IWDG_Handle_T *hiwdg, HAL_IWDG_CallbackID_T callback_id, IWDG_Callback_T callback)
{
    HAL_Status_T status = HAL_OK;
    CLEAR_ERROR_CODE(hiwdg);

    /* check the IWDG handle */
    ASSERT_PARAM(hiwdg);
    /* check the IWDG instance */
    ASSERT_PARAM(IS_IWDG_INSTANCE(hiwdg));

    /* check the parameters */
    if (callback == NULL)
    {
        /* update the error code */
        HAL_FLAG_SET(hiwdg->error_code, HAL_IWDG_ERROR_INVALID_CALLBACK);
        /* return error status */
        return HAL_ERROR;
    }

    /* protect the process */
    TAKE_MUTEX(hiwdg);

    switch (callback_id)
    {
        case HAL_IWDG_MSPINIT_CB_ID:
        {
            hiwdg->MspInitCallback = callback;
        }
        break;
        case HAL_IWDG_MSPDEINIT_CB_ID:
        {
            hiwdg->MspDeInitCallback = callback;
        }
        break;
        case HAL_IWDG_TIMEOUT_CB_ID:
        {
            hiwdg->TimeoutCallback = callback;
        }
        break;

        default:
        {
            /* update the error code */
            HAL_FLAG_SET(hiwdg->error_code, HAL_IWDG_ERROR_INVALID_CALLBACK);
            /* return error status */
            status = HAL_ERROR;
        }
        break;
    }

    /* release the process protection */
    GIVE_MUTEX(hiwdg);

    return status;
}

/**
 * @brief  Unregister an IWDG callback, IWDG callback is redirected to the weak predefined callback.
 * @param  hiwdg Pointer to a IWDG_Handle_T structure that contains the configuration information for the specified IWDG module.
 * @param  callback_id ID of the callback to be unregistered.
 *         This parameter can be one of the following values:
 *           @arg @ref HAL_IWDG_TIMEOUT_CB_ID IWDG Timeout Callback ID
 * @retval HAL status
 */
HAL_Status_T HAL_IWDG_UnRegisterCallback(IWDG_Handle_T *hiwdg, HAL_IWDG_CallbackID_T callback_id)
{
    HAL_Status_T status = HAL_OK;
    CLEAR_ERROR_CODE(hiwdg);

    /* check the IWDG handle */
    ASSERT_PARAM(hiwdg);
    /* check the IWDG instance */
    ASSERT_PARAM(IS_IWDG_INSTANCE(hiwdg));

    /* protect the process */
    TAKE_MUTEX(hiwdg);

    switch (callback_id)
    {
        case HAL_IWDG_MSPINIT_CB_ID:
        {
            hiwdg->MspInitCallback = HAL_IWDG_MspInit;
        }
        break;
        case HAL_IWDG_MSPDEINIT_CB_ID:
        {
            hiwdg->MspDeInitCallback = HAL_IWDG_MspDeInit;
        }
        break;
        case HAL_IWDG_TIMEOUT_CB_ID:
        {
            hiwdg->TimeoutCallback = HAL_IWDG_TimeoutCallback;
        }
        break;

        default:
        {
            /* update the error code */
            HAL_FLAG_SET(hiwdg->error_code, HAL_IWDG_ERROR_INVALID_CALLBACK);
            /* return error status */
            status = HAL_ERROR;
        }
        break;
    }

    /* release the process protection */
    GIVE_MUTEX(hiwdg);

    return status;
}
#endif /* configUSE_IWDG_REGISTER_CALLBACKS */

/*
================================================================================
            ##### Peripheral State and Errors functions #####
================================================================================
*/
/**
 * @brief  Returns the IWDG state.
 * @param  hiwdg Pointer to a IWDG_Handle_T structure that contains the configuration information for the specified IWDG module.
 * @retval HAL state
 */
HAL_State_T HAL_IWDG_GetState(IWDG_Handle_T *hiwdg)
{
    /* check the IWDG handle */
    ASSERT_PARAM(hiwdg);
    /* check the IWDG instance */
    ASSERT_PARAM(IS_IWDG_INSTANCE(hiwdg));

    return hiwdg->state;
}

/**
 * @brief  Return the IWDG error code
 * @param  hiwdg Pointer to a IWDG_Handle_T structure that contains the configuration information for the specified IWDG module.
 * @retval IWDG error code
 */
uint32_t HAL_IWDG_GetError(IWDG_Handle_T *hiwdg)
{
    /* check the IWDG handle */
    ASSERT_PARAM(hiwdg);
    /* check the IWDG instance */
    ASSERT_PARAM(IS_IWDG_INSTANCE(hiwdg));

    return hiwdg->error_code;
}

/*
================================================================================
            ##### Peripheral Control functions #####
================================================================================
*/
/**
 * @brief  Feed watchdog.
 * @param  hiwdg Pointer to a IWDG_Handle_T structure that contains the configuration information for the specified IWDG module.
 * @retval HAL status
 */
HAL_Status_T HAL_IWDG_Refresh(IWDG_Handle_T *hiwdg)
{
    HAL_Status_T status = HAL_OK;
    CLEAR_ERROR_CODE(hiwdg);

    /* check the IWDG handle */
    ASSERT_PARAM(hiwdg);
    /* check the IWDG instance */
    ASSERT_PARAM(IS_IWDG_INSTANCE(hiwdg));

    /* protect the process */
    TAKE_MUTEX(hiwdg);

    /* if IWDG is already running */
    if (hiwdg->state == HAL_STATE_BUSY)
    {
        /* feed watchdog */
        HAL_REG_WRITE(hiwdg->instance->RESTART, IWDG_FEED_KEY);
    }
    else /* IWDG is not running */
    {
        /* update the error code */
        HAL_FLAG_SET(hiwdg->error_code, HAL_IWDG_ERROR_NOT_RUNNING);
        /* return error status */
        status = HAL_ERROR;
    }

    /* release the process protection */
    GIVE_MUTEX(hiwdg);

    return status;
}

/*
================================================================================
            ##### I/O Operation functions #####
================================================================================
*/
/**
 * @brief  This function handles IWDG interrupt request.
 * @param  hiwdg Pointer to a IWDG_Handle_T structure that contains the configuration information for the specified IWDG module.
 * @retval None
 */
ATTR_ISR void HAL_IWDG_IRQHandle(IWDG_Handle_T *hiwdg)
{
    /* check the IWDG handle */
    ASSERT_PARAM(hiwdg);
    /* check the IWDG instance */
    ASSERT_PARAM(IS_IWDG_INSTANCE(hiwdg));

    if (hiwdg->state != HAL_STATE_BUSY) /* IWDG is not running */
    {
        return;
    }

    /* clear IWDG status */
    HAL_REG_WRITE(hiwdg->instance->CLEAR, 1);

#if (configUSE_IWDG_REGISTER_CALLBACKS == 1)
    /* call registered callback */
    if (hiwdg->TimeoutCallback != NULL)
    {
        hiwdg->TimeoutCallback(hiwdg);
    }
#else
    /* Call legacy weak timeout callback */
    HAL_IWDG_TimeoutCallback(hiwdg);
#endif /* configUSE_IWDG_REGISTER_CALLBACKS */
}

/*
================================================================================
            ##### Event Callback functions #####
================================================================================
*/

/**
 * @brief  IWDG timeout callback.
 * @param  hiwdg Pointer to a IWDG_Handle_T structure that contains the configuration information for the specified IWDG module.
 * @retval None
 */
__weak ATTR_ISR void HAL_IWDG_TimeoutCallback(IWDG_Handle_T *hiwdg)
{
    /* prevent unused argument(s) compilation warning */
    UNUSED(hiwdg);
    /* NOTE: This function should not be modified, when the callback is needed,the HAL_MspDeInitCallback could be implemented in the user file */
}

/* Private functions ---------------------------------------------------------*/
/**
 * @brief  Check the IWDG configuration.
 * @param  hiwdg Pointer to a IWDG_Handle_T structure that contains the configuration information for the specified IWDG module.
 * @retval HAL status
 */
static HAL_Status_T _iwdg_check_config(IWDG_Handle_T *hiwdg)
{
    /* check the IWDG handle */
    ASSERT_PARAM(hiwdg);
    /* check the IWDG instance */
    ASSERT_PARAM(IS_IWDG_INSTANCE(hiwdg));
    /* check IWDG mode */
    RET_FLAG_FALSE(IS_IWDG_MODE(hiwdg), HAL_ERROR);
    /* check IWDG timer constant */
    RET_FLAG_FALSE(IS_IWDG_TIMER_CONSTANT(hiwdg), HAL_ERROR);

    return HAL_OK;
}

/**
 * @brief  Configures the IWDG peripheral.
 * @param  hiwdg Pointer to a IWDG_Handle_T structure that contains the configuration information for the specified IWDG module.
 * @retval HAL_Status_T
 */
static HAL_Status_T _iwdg_set_config(IWDG_Handle_T *hiwdg)
{
    /* set IWDG mode */
    if (IS_IWDG_MODE_RESET(hiwdg))
    {
        HAL_REG_WRITE(hiwdg->instance->CTRL_b.IE, 0);
        HAL_REG_WRITE(hiwdg->instance->CTRL_b.RSTEN, 1);
    }
    else
    {
        HAL_REG_WRITE(hiwdg->instance->CTRL_b.RSTEN, 0);
        HAL_REG_WRITE(hiwdg->instance->CTRL_b.IE, 1);
    }

    /* set IWDG timer constant */
    HAL_REG_WRITE(hiwdg->instance->LOAD, hiwdg->init.timer_constant);

    /* trigger IWDG to reload timer constant */
    HAL_REG_WRITE(hiwdg->instance->RESTART, IWDG_FEED_KEY);

    /* enable IWDG */
    HAL_REG_WRITE(hiwdg->instance->CTRL_b.WDTEN, IWDG_EN_MASK);

    return HAL_OK;
}

#endif /* configUSE_IWDG_PERIPHERAL */
