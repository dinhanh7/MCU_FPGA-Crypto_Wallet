/***********************
 * SN34F78X SDK V2.1.4 *
 ***********************/

#include "sn34f78x_hal_crc.h"

#if (configUSE_CRC_PERIPHERAL == 1)

/* Exported functions --------------------------------------------------------*/
/*
================================================================================
            ##### Initialization/De-initialization functions #####
================================================================================
*/
/**
 * @brief  Initializes the CRC mode according to the specified parameters in the CRC_Init_T and create the associated handle.
 * @param  hcrc Pointer to a CRC_Handle_T structure that contains the configuration information for the specified CRC module.
 * @retval HAL status
 */
HAL_Status_T HAL_CRC_Init(CRC_Handle_T *hcrc)
{
    /* Check the valid of CRC handle */
    ASSERT_PARAM(hcrc);
    /* Check the CRC instance */
    ASSERT_PARAM(IS_PERIPHERAL_CRC_INSTANCE(hcrc));
    /* Check the parameters */
    ASSERT_PARAM(IS_CRC_POLY(hcrc->init.polynomial));

    if (HAL_FLAG_EQU(hcrc->state, HAL_STATE_RESET))
    {
        /* init the mutex */
        GIVE_MUTEX(hcrc);

        HAL_CRC_MspInit(hcrc);

        /* set the polynomial  */
        HAL_REG_WRITE(hcrc->instance->CTRL_b.CRC, hcrc->init.polynomial);
        /* Change CRC peripheral state */
        hcrc->state = HAL_STATE_READY;
        CLEAR_ERROR_CODE(hcrc);

        return HAL_OK;
    }
    else
    {
        /* update the state */
        hcrc->error_code = HAL_ERROR_REINIT;

        return HAL_ERROR;
    }
}

/**
 * @brief  DeInitialize the CRC peripheral.
 * @param  hcrc Pointer to a CRC_Handle_T structure that contains the configuration information for the specified CRC module.
 * @retval HAL status
 */
HAL_Status_T HAL_CRC_DeInit(CRC_Handle_T *hcrc)
{
    /* Check the CRC handle allocation */
    ASSERT_PARAM(hcrc);
    /* Check the CRC instance */
    ASSERT_PARAM(IS_PERIPHERAL_CRC_INSTANCE(hcrc));

    /* Change CRC peripheral state */
    hcrc->state = HAL_STATE_BUSY;

    /* reset crc  */
    HAL_REG_WRITE(hcrc->instance->CTRL_b.RESET, 1);

    /* Clear crc data */
    HAL_REG_CLEAR(hcrc->instance->DATA);

    /* DeInit the low level hardware */
    HAL_CRC_MspDeInit(hcrc);

    /* Change CRC peripheral state */
    hcrc->state = HAL_STATE_RESET;
    CLEAR_ERROR_CODE(hcrc);

    /* free the mutex */
    GIVE_MUTEX(hcrc);

    /* Return function status */
    return HAL_OK;
}

/**
 * @brief  CRC MSP Init.
 * @param  hcrc Pointer to a CRC_Handle_T structure that contains the configuration information for the specified CRC.
 * @retval None
 */
__weak void HAL_CRC_MspInit(CRC_Handle_T *hcrc)
{
    /* Prevent unused argument(s) compilation warning */
    UNUSED(hcrc);
    /* NOTE: This function should not be modified, when the callback is needed, the HAL_CRC_MspInit could be implemented in the user file */
}

/**
 * @brief  CRC MSP DeInit.
 * @param  hcrc Pointer to a CRC_Handle_T structure that contains the configuration information for the specified CRC.
 * @retval None
 */
__weak void HAL_CRC_MspDeInit(CRC_Handle_T *hcrc)
{
    /* Prevent unused argument(s) compilation warning */
    UNUSED(hcrc);
    /* NOTE: This function should not be modified, when the callback is needed, the HAL_CRC_MspDeInit could be implemented in the user file */
}

/*
================================================================================
                   ##### Peripheral Control functions #####
================================================================================
*/
/**
 * @brief replace the initialized polynomial value with specified polynomial for calculation
 *        and the polynomial value in handler will be kept(not modified).
 * @param hcrc Pointer to a CRC_Handle_T structure that contains the configuration information for the specified CRC module.
 * @param polynomial new CRC polynomial value for update
 * @return HAL_Status_T
 */
HAL_Status_T HAL_CRC_UpdatePoly(CRC_Handle_T *hcrc, uint32_t polynomial)
{
    /* Check the CRC handle allocation */
    ASSERT_PARAM(hcrc);
    /* Check the CRC instance */
    ASSERT_PARAM(IS_PERIPHERAL_CRC_INSTANCE(hcrc));
    /* Check the parameters */
    ASSERT_PARAM(IS_CRC_POLY(polynomial));

    /* Update polynomial */
    HAL_REG_WRITE(hcrc->instance->CTRL_b.CRC, polynomial);

    return HAL_OK;
}

/*
================================================================================
                   ##### IO Operation functions #####
================================================================================
*/
/**
 * @brief Calculate CRC data
 * @param hcrc Pointer to a CRC_Handle_T structure that contains the configuration information for the specified CRC module.
 * @return calculation result
 */
uint32_t HAL_CRC_Calculate(CRC_Handle_T *hcrc, uint8_t *buf, uint32_t buf_size)
{
    /* Check the CRC handle */
    ASSERT_PARAM(hcrc);
    /* Check the CRC instance */
    ASSERT_PARAM(IS_PERIPHERAL_CRC_INSTANCE(hcrc));
    /* Check the parameters */
    ASSERT_PARAM(buf);
    ASSERT_PARAM(buf_size);

    /* protect the process */
    TAKE_MUTEX(hcrc);
    /* change CRC peripheral state */
    HAL_FLAG_SET(hcrc->state, HAL_STATE_BUSY);
    /* reset crc  */
    HAL_REG_WRITE(hcrc->instance->CTRL_b.RESET, 1);

    for (uint32_t i = 0; i < buf_size; i++)
    {
        HAL_REG_WRITE(hcrc->instance->DATA, buf[i]);
        while (hcrc->instance->CTRL_b.BUSY != 0)
        {
            __NOP();
        }
    }

    uint32_t result = HAL_REG_READ(hcrc->instance->DATA);
    /* change CRC peripheral state */
    HAL_FLAG_WRITE(hcrc->state, HAL_STATE_READY);
    /* free the mutex */
    GIVE_MUTEX(hcrc);

    return result;
}

/*
================================================================================
                   ##### Peripheral State and Errors functions #####
================================================================================
*/
/**
 * @brief  Return the CRC state.
 * @param  hcrc Pointer to a CRC_Handle_T structure that contains the configuration information for the specified CRC module.
 * @retval HAL state
 */
HAL_State_T HAL_CRC_GetState(CRC_Handle_T *hcrc)
{
    /* Check the CRC handle */
    ASSERT_PARAM(hcrc);
    /* Check the CRC instance */
    ASSERT_PARAM(IS_PERIPHERAL_CRC_INSTANCE(hcrc));

    /* Return CRC handle state */
    return hcrc->state;
}

/**
 * @brief Return the CRC error code
 * @param hcrc Pointer to a CRC_Handle_T structure that contains the configuration information for the specified CRC module.
 * @return int32_t CRC error code
 */

int32_t HAL_CRC_GetError(CRC_Handle_T *hcrc)
{
    /* Check the CRC handle */
    ASSERT_PARAM(hcrc);
    /* Check the CRC instance */
    ASSERT_PARAM(IS_PERIPHERAL_CRC_INSTANCE(hcrc));

    return hcrc->error_code;
}
#endif /* configUSE_CRC_PERIPHERAL */
