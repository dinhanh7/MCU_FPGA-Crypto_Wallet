/***********************
 * SN34F78X SDK V2.1.4 *
 ***********************/

#include "sn34f78x_hal_wwdg.h"

#if (configUSE_WWDG_PERIPHERAL == 1)

/* Exported functions --------------------------------------------------------*/
/*
================================================================================
            ##### Initialization/De-initialization functions #####
================================================================================
*/
/**
 * @brief  Initializes the WWDG mode according to the specified parameters in the WWDG_Init_T and create the associated handle.
 * @param  hwwdg Pointer to a WWDG_Handle_T structure that contains the configuration information for the specified WWDG module.
 * @retval HAL status
 */
HAL_Status_T HAL_WWDG_Init(WWDG_Handle_T *hwwdg)
{
    /* check the WWDG handle */
    ASSERT_PARAM(hwwdg);

    /* check the WWDG instance */
    ASSERT_PARAM(IS_WWDG_INSTANCE(hwwdg));

    /* check whether is already initialized, return error */
    if (HAL_FLAG_EQU(hwwdg->state, HAL_STATE_RESET))
    {
        /* check the parameters */
        RET_FLAG_FALSE(_wwdg_check_config(hwwdg) == HAL_OK, HAL_ERROR);

#if (configUSE_WWDG_REGISTER_CALLBACKS == 1)
        /* Init default callback */
        if (hwwdg->MspInitCallback == NULL)
        {
            hwwdg->MspInitCallback = HAL_WWDG_MspInit;
        }
        if (hwwdg->TimeoutCallback == NULL)
        {
            hwwdg->TimeoutCallback = HAL_WWDG_TimeoutCallback;
        }
        if (hwwdg->EarlyWakeupCallback == NULL)
        {
            hwwdg->EarlyWakeupCallback = HAL_WWDG_EarlyWakeupCallback;
        }
        /* Init the low level hardware : GPIO, CLOCK, NVIC */
        hwwdg->MspInitCallback(hwwdg);
#else
        HAL_WWDG_MspInit(hwwdg);
#endif /* configUSE_WWDG_REGISTER_CALLBACKS */

        /* Delay to wait for the clock to be ready */
        uint32_t sys_clk = HAL_RCC_GetHCLKFreq();
        for (uint32_t cnt = 0; cnt < sys_clk / 100000; cnt++)
        {
            __NOP();
        }

        /* config WWDG */
        RET_FLAG_FALSE(_wwdg_set_config(hwwdg) == HAL_OK, HAL_ERROR);

        /* update the state */
        CLEAR_ERROR_CODE(hwwdg);
        hwwdg->state = HAL_STATE_BUSY;

        return HAL_OK;
    }
    else
    {
        hwwdg->error_code = HAL_ERROR_REINIT;
        return HAL_ERROR;
    }
}

/**
 * @brief  DeInitializes the WWDG peripheral.
 * @param  hwwdg Pointer to a WWDG_Handle_T structure that contains the configuration information for the specified WWDG module.
 * @retval HAL status
 */
HAL_Status_T HAL_WWDG_DeInit(WWDG_Handle_T *hwwdg)
{
    /* check the WWDG handle */
    ASSERT_PARAM(hwwdg);
    /* check the WWDG instance */
    ASSERT_PARAM(IS_WWDG_INSTANCE(hwwdg));

    /* WWDG is not running */
    if (hwwdg->state != HAL_STATE_BUSY)
    {
        return HAL_OK;
    }

#if (configUSE_WWDG_REGISTER_CALLBACKS == 1)
    if (hwwdg->MspDeInitCallback == NULL)
    {
        hwwdg->MspDeInitCallback = HAL_WWDG_MspDeInit;
    }

    hwwdg->MspDeInitCallback(hwwdg);
#else
    HAL_WWDG_MspDeInit(hwwdg);
#endif

    /* update the state */
    CLEAR_ERROR_CODE(hwwdg);
    hwwdg->state = HAL_STATE_RESET;

    /* release the process protection */
    GIVE_MUTEX(hwwdg);

    return HAL_OK;
}

/**
 * @brief  WWDG Msp Init callback
 * @param  hwwdg WWDG handle
 * @retval None
 */
__weak void HAL_WWDG_MspInit(WWDG_Handle_T *hwwdg)
{
    /* check the WWDG handle */
    ASSERT_PARAM(hwwdg);
    /* check the WWDG instance */
    ASSERT_PARAM(IS_WWDG_INSTANCE(hwwdg));
    /* Prevent unused argument(s) compilation warning */
    UNUSED(hwwdg);

    /* NOTE : This function should not be modified, when the callback is needed,
              the HAL_MspInitCallback could be implemented in the user file
     */
}

/**
 * @brief  WWDG Msp DeInit callback
 * @param  hwwdg WWDG handle
 * @retval None
 */
__weak void HAL_WWDG_MspDeInit(WWDG_Handle_T *hwwdg)
{
    /* Prevent unused argument(s) compilation warning */
    UNUSED(hwwdg);
    /* NOTE: This function should not be modified, when the callback is needed, the HAL_MspDeInitCallback could be implemented in the user file */
}

/*
================================================================================
            ##### Callbacks Register/UnRegister functions #####
================================================================================
*/
#if (configUSE_WWDG_REGISTER_CALLBACKS == 1)
/**
 * @brief  Register a User WWDG callback to be used instead of the weak predefined callback.
 * @param  hwwdg Pointer to a WWDG_Handle_T structure that contains the configuration information for the specified WWDG module.
 * @param  callback_id ID of the callback to be registered.
 *         This parameter can be one of the following values:
 *           @arg @ref HAL_WWDG_TIMEOUT_CB_ID WWDG Timeout Callback ID
 * @param  callback pointer to the Callback function
 * @retval HAL status
 */
HAL_Status_T HAL_WWDG_RegisterCallback(WWDG_Handle_T *hwwdg, HAL_WWDG_CallbackID_T callback_id, WWDG_Callback_T callback)
{
    HAL_Status_T status = HAL_OK;
    CLEAR_ERROR_CODE(hwwdg);

    /* check the WWDG handle */
    ASSERT_PARAM(hwwdg);
    /* check the WWDG instance */
    ASSERT_PARAM(IS_WWDG_INSTANCE(hwwdg));

    /* check the parameters */
    if (callback == NULL)
    {
        /* update the error code */
        HAL_FLAG_SET(hwwdg->error_code, HAL_WWDG_ERROR_INVALID_CALLBACK);
        /* return error status */
        return HAL_ERROR;
    }

    /* protect the process */
    TAKE_MUTEX(hwwdg);

    switch (callback_id)
    {
        case HAL_WWDG_MSPINIT_CB_ID:
        {
            hwwdg->MspInitCallback = callback;
        }
        break;
        case HAL_WWDG_MSPDEINIT_CB_ID:
        {
            hwwdg->MspDeInitCallback = callback;
        }
        break;
        case HAL_WWDG_EWI_CB_ID:
        {
            hwwdg->EarlyWakeupCallback = callback;
        }
        break;
        case HAL_WWDG_TIMEOUT_CB_ID:
        {
            hwwdg->TimeoutCallback = callback;
        }
        break;

        default:
        {
            /* update the error code */
            HAL_FLAG_SET(hwwdg->error_code, HAL_WWDG_ERROR_INVALID_CALLBACK);
            /* return error status */
            status = HAL_ERROR;
        }
        break;
    }

    /* release the process protection */
    GIVE_MUTEX(hwwdg);

    return status;
}

/**
 * @brief  Unregister an WWDG callback, WWDG callback is redirected to the weak predefined callback.
 * @param  hwwdg Pointer to a WWDG_Handle_T structure that contains the configuration information for the specified WWDG module.
 * @param  callback_id ID of the callback to be unregistered.
 *         This parameter can be one of the following values:
 *           @arg @ref HAL_WWDG_TIMEOUT_CB_ID WWDG Timeout Callback ID
 * @retval HAL status
 */
HAL_Status_T HAL_WWDG_UnRegisterCallback(WWDG_Handle_T *hwwdg, HAL_WWDG_CallbackID_T callback_id)
{
    HAL_Status_T status = HAL_OK;
    CLEAR_ERROR_CODE(hwwdg);

    /* check the WWDG handle */
    ASSERT_PARAM(hwwdg);
    /* check the WWDG instance */
    ASSERT_PARAM(IS_WWDG_INSTANCE(hwwdg));

    /* protect the process */
    TAKE_MUTEX(hwwdg);

    switch (callback_id)
    {
        case HAL_WWDG_MSPINIT_CB_ID:
        {
            hwwdg->MspInitCallback = HAL_WWDG_MspInit;
        }
        break;
        case HAL_WWDG_MSPDEINIT_CB_ID:
        {
            hwwdg->MspDeInitCallback = HAL_WWDG_MspDeInit;
        }
        break;
        case HAL_WWDG_EWI_CB_ID:
        {
            hwwdg->EarlyWakeupCallback = HAL_WWDG_EarlyWakeupCallback;
        }
        break;
        case HAL_WWDG_TIMEOUT_CB_ID:
        {
            hwwdg->TimeoutCallback = HAL_WWDG_TimeoutCallback;
        }
        break;

        default:
        {
            /* update the error code */
            HAL_FLAG_SET(hwwdg->error_code, HAL_WWDG_ERROR_INVALID_CALLBACK);
            /* return error status */
            status = HAL_ERROR;
        }
        break;
    }

    /* release the process protection */
    GIVE_MUTEX(hwwdg);

    return status;
}
#endif /* configUSE_WWDG_REGISTER_CALLBACKS */

/*
================================================================================
            ##### Peripheral State and Errors functions #####
================================================================================
*/
/**
 * @brief  Returns the WWDG state.
 * @param  hwwdg Pointer to a WWDG_Handle_T structure that contains the configuration information for the specified WWDG module.
 * @retval HAL state
 */
HAL_State_T HAL_WWDG_GetState(WWDG_Handle_T *hwwdg)
{
    /* check the WWDG handle */
    ASSERT_PARAM(hwwdg);
    /* check the WWDG instance */
    ASSERT_PARAM(IS_WWDG_INSTANCE(hwwdg));

    return hwwdg->state;
}

/**
 * @brief  Return the WWDG error code
 * @param  hwwdg Pointer to a WWDG_Handle_T structure that contains the configuration information for the specified WWDG module.
 * @retval WWDG error code
 */
uint32_t HAL_WWDG_GetError(WWDG_Handle_T *hwwdg)
{
    /* check the WWDG handle */
    ASSERT_PARAM(hwwdg);
    /* check the WWDG instance */
    ASSERT_PARAM(IS_WWDG_INSTANCE(hwwdg));

    return hwwdg->error_code;
}

/*
================================================================================
            ##### Peripheral Control functions #####
================================================================================
*/
/**
 * @brief  Feed watchdog.
 * @param  hwwdg Pointer to a WWDG_Handle_T structure that contains the configuration information for the specified WWDG module.
 * @retval HAL status
 */
HAL_Status_T HAL_WWDG_Refresh(WWDG_Handle_T *hwwdg)
{
    HAL_Status_T status = HAL_OK;
    CLEAR_ERROR_CODE(hwwdg);

    /* check the WWDG handle */
    ASSERT_PARAM(hwwdg);
    /* check the WWDG instance */
    ASSERT_PARAM(IS_WWDG_INSTANCE(hwwdg));

    /* protect the process */
    TAKE_MUTEX(hwwdg);

    /* if WWDG is already running */
    if (hwwdg->state == HAL_STATE_BUSY)
    {
        /* wait for RELOAD_LOCK unlock*/
        WWDG_WAIT_RELOADLOCK_COMPLETE(hwwdg);
        /* feed watchdog */
        HAL_REG_WRITE(hwwdg->instance->KEY, WWDG_FEED_KEY);
    }
    else /* WWDG is not running */
    {
        /* update the error code */
        HAL_FLAG_SET(hwwdg->error_code, HAL_WWDG_ERROR_NOT_RUNNING);
        /* return error status */
        status = HAL_ERROR;
    }

    /* release the process protection */
    GIVE_MUTEX(hwwdg);

    return status;
}

/*
================================================================================
            ##### I/O Operation functions #####
================================================================================
*/
/**
 * @brief  This function handles WWDG interrupt request.
 * @param  hwwdg Pointer to a WWDG_Handle_T structure that contains the configuration information for the specified WWDG module.
 * @retval None
 */
ATTR_ISR void HAL_WWDG_IRQHandle(WWDG_Handle_T *hwwdg)
{
    /* check the WWDG handle */
    ASSERT_PARAM(hwwdg);
    /* check the WWDG instance */
    ASSERT_PARAM(IS_WWDG_INSTANCE(hwwdg));

#if (configUSE_WWDG_REGISTER_CALLBACKS == 1)
    /*When early wakeup interruption occurs*/
    if (IS_WWDG_EARLYWAKEUP(hwwdg))
    {
        /* call early wakeup callback */
        if (hwwdg->EarlyWakeupCallback != NULL)
        {
            hwwdg->EarlyWakeupCallback(hwwdg);
        }
        /* clear WWDG underflow status */
        HAL_REG_WRITE(hwwdg->instance->RIS, WWDG_RIS_UDF_MASK);
    }
    /*When overflow interruption occurs*/
    if (IS_WWDG_OVERFLOW(hwwdg))
    {
        /* call timeout callback */
        if (hwwdg->TimeoutCallback != NULL)
        {
            hwwdg->TimeoutCallback(hwwdg);
        }
        /* clear WWDG overflow status */
        HAL_REG_WRITE(hwwdg->instance->RIS, WWDG_RIS_OVF_MASK);
    }
#else
    /*When early wakeup interruption occurs*/
    if (IS_WWDG_EARLYWAKEUP(hwwdg))
    {
        /* call legacy weak early wakeup callback */
        HAL_WWDG_EarlyWakeupCallback(hwwdg);
        /* clear WWDG underflow status */
        HAL_REG_WRITE(hwwdg->instance->RIS, WWDG_RIS_UDF_MASK);
    }
    /*When overflow interruption occurs*/
    if (IS_WWDG_OVERFLOW(hwwdg))
    {
        /* call legacy weak timeout callback */
        HAL_WWDG_TimeoutCallback(hwwdg);
        /* clear WWDG overflow status */
        HAL_REG_WRITE(hwwdg->instance->RIS, WWDG_RIS_OVF_MASK);
    }
#endif /* configUSE_WWDG_REGISTER_CALLBACKS */
}

/*
================================================================================
            ##### Event Callback functions #####
================================================================================
*/

/**
 * @brief  WWDG timeout callback.
 * @param  hwwdg Pointer to a WWDG_Handle_T structure that contains the configuration information for the specified WWDG module.
 * @retval None
 */
__weak ATTR_ISR void HAL_WWDG_TimeoutCallback(WWDG_Handle_T *hwwdg)
{
    /* prevent unused argument(s) compilation warning */
    UNUSED(hwwdg);
    /* NOTE: This function should not be modified, when the callback is needed, the HAL_WWDG_TimeoutCallback could be implemented in the user file */
}

/**
 * @brief  WWDG early wakeup callback.
 * @param  hwwdg Pointer to a WWDG_Handle_T structure that contains the configuration information for the specified WWDG module.
 * @retval None
 */
__weak void HAL_WWDG_EarlyWakeupCallback(WWDG_Handle_T *hwwdg)
{
    /* prevent unused argument(s) compilation warning */
    UNUSED(hwwdg);
    /* NOTE: This function should not be modified, when the callback is needed, the HAL_WWDG_TimeoutCallback could be implemented in the user file */
}

/* Private functions ---------------------------------------------------------*/
/**
 * @brief  Check the WWDG configuration.
 * @param  hwwdg Pointer to a WWDG_Handle_T structure that contains the configuration information for the specified WWDG module.
 * @retval HAL status
 */
static HAL_Status_T _wwdg_check_config(WWDG_Handle_T *hwwdg)
{
    /* check the WWDG handle */
    ASSERT_PARAM(hwwdg);
    /* check the WWDG instance */
    ASSERT_PARAM(IS_WWDG_INSTANCE(hwwdg));

    /* check WWDG mode */
    RET_FLAG_FALSE(IS_WWDG_MODE(hwwdg), HAL_ERROR);
    /* check WWDG prescaler */
    RET_FLAG_FALSE(IS_WWDG_PRESCALER(hwwdg), HAL_ERROR);
    /* check WWDG window constant if underflow is enable */
    if (HAS_WWDG_MODE_EARLYWAKEUP(hwwdg))
    {
        RET_FLAG_FALSE(IS_WWDG_WIN_CONSTANT(hwwdg), HAL_ERROR);
    }
    /* check WWDG timer constant */
    RET_FLAG_FALSE(IS_WWDG_TIMER_CONSTANT(hwwdg), HAL_ERROR);

    return HAL_OK;
}

/**
 * @brief  Configures the WWDG peripheral.
 * @param  hwwdg Pointer to a WWDG_Handle_T structure that contains the configuration information for the specified WWDG module.
 * @retval HAL_Status_T
 */
static HAL_Status_T _wwdg_set_config(WWDG_Handle_T *hwwdg)
{
    /* enable access to the WWDT_PRESCALER and WWDT_RELOAD */
    HAL_REG_WRITE(hwwdg->instance->KEY, WWDG_REPROG_KEY);

    /* set WWDG prescaler*/
    HAL_REG_WRITE(hwwdg->instance->PRESCALER, hwwdg->init.prescaler);

    /* wait for settings prescaler to complete */
    WWDG_WAIT_PRELOCK_COMPLETE(hwwdg);

    /* set WWDG timer constant */
    HAL_REG_WRITE(hwwdg->instance->RELOAD, hwwdg->init.counter);

    /* wait for settings reload to complete */
    WWDG_WAIT_RELOADLOCK_COMPLETE(hwwdg);

    /* set early wakeup mode */
    if (HAS_WWDG_MODE_EARLYWAKEUP(hwwdg))
    {
        /* check whether win constant is less than timer constant */
        RET_FLAG_FALSE(IS_WWDG_WIN_CONSTANT_LESS_TIMER_CONSTANT(hwwdg), HAL_ERROR);
        /* set window constant if underflow is enable */
        HAL_REG_WRITE(hwwdg->instance->UDFV, hwwdg->init.window);
        /* enable underflow IRQ */
        HAL_REG_WRITE(hwwdg->instance->CTRL_b.UDFIE, 1);
        /* enable underflow */
        HAL_REG_WRITE(hwwdg->instance->CTRL_b.UDFEN, 1);
    }
    /* set overflow mode */
    if (HAS_WWDG_MODE_OVERFLOW(hwwdg))
    {
        /* enable IRQ*/
        HAL_REG_WRITE(hwwdg->instance->CTRL_b.IE, 1);
    }
    /* set reset mode */
    if (HAS_WWDG_MODE_RESET(hwwdg))
    {
        /* enable reset*/
        HAL_REG_WRITE(hwwdg->instance->CTRL_b.RSTEN, 1);
    }

    /* disable access to the WWDT_PRESCALER and WWDT_RELOAD */
    HAL_REG_WRITE(hwwdg->instance->KEY, WWDG_DISREPROG_KEY);
    /* wait for settings reload to complete */
    WWDG_WAIT_RELOADLOCK_COMPLETE(hwwdg);
    /* start WWDG */
    HAL_REG_WRITE(hwwdg->instance->KEY, WWDG_START_KEY);

    return HAL_OK;
}

#endif /* configUSE_WWDG_PERIPHERAL */
