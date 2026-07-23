/***********************
 * SN34F78X SDK V2.1.4 *
 ***********************/

#include "sn34f78x_hal.h"

// add board information
#include "sn34f78x_board.c"

// add io re-target
#include "retarget_io.c"

/* Exported functions --------------------------------------------------------*/
/*
================================================================================
            ##### Initialization/De-initialization functions #####
================================================================================
*/
/**
 * @brief  This function is used to initialize the HAL Library;
 * @note   SysTick is used as time base for the HAL_Delay() function, the application
 *         need to ensure that the SysTick time base is always set to 1 millisecond
 *         to have correct HAL operation.
 * @retval HAL status
 */
HAL_Status_T HAL_Init(void)
{
    /* Use sys_tick as time base source and configure 1ms tick (default clock after Reset is HSI) */
    HAL_InitTick(SYS_TICK_INT_PRIORITY);

    /* Set auto hold and flash operation init */
    HAL_InitFlash();

    /* Init the low level hardware */
    HAL_MspInit();

    /* Return function status */
    return HAL_OK;
}
/**
 * @brief  This function de-Initializes common part of the HAL and stops the sys_tick.
 *         This function is optional.
 * @retval HAL status
 */
HAL_Status_T HAL_DeInit(void)
{
    /* Reset of all peripherals */

    /* De-Init the low level hardware */
    HAL_MspDeInit();

    /* Return function status */
    return HAL_OK;
}

/*
================================================================================
            ##### Event Callback functions #####
================================================================================
*/

/**
 * @brief  Initialize the MSP.
 * @retval None
 */
__weak void HAL_MspInit(void)
{
    /* NOTE : This function should not be modified, when the callback is needed, the HAL_MspInit could be implemented in the user file */
}

/**
 * @brief  DeInitializes the MSP.
 * @retval None
 */
__weak void HAL_MspDeInit(void)
{
    /* NOTE : This function should not be modified, when the callback is needed,the HAL_MspDeInit could be implemented in the user file */
}
