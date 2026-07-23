/***********************
 * SN34F78X SDK V2.1.4 *
 ***********************/

#include "sn34f78x_hal_systick.h"

__IO uint32_t  uwTick     = 0;
uint32_t       uwTickPrio = (1UL << __NVIC_PRIO_BITS); /* Invalid PRIO */
HAL_TickFreq_T uwTickFreq = HAL_TICK_FREQ_DEFAULT;     /* 1KHz */

/* Exported functions --------------------------------------------------------*/
/*
================================================================================
            ##### Initialization Tick functions #####
================================================================================
*/

/**
 * @brief This function configures the source of the time base.
 *        The time source is configured to have 1ms time base with a dedicated
 *        tick interrupt priority.
 * @note This function is called automatically at the beginning of program after
 *       reset by HAL_Init() or at any time when clock is reconfigured  by HAL_RCC_ClockConfig().
 * @note In the default implementation, SysTick timer is the source of time base.
 *       It is used to generate interrupts at regular time intervals.
 *       Care must be taken if HAL_Delay() is called from a peripheral ISR process,
 *       The SysTick interrupt must have higher priority (numerically lower)
 *       than the peripheral interrupt. Otherwise the caller ISR process will be blocked.
 *       The function is declared as __weak to be overwritten  in case of other
 *       implementation  in user file.
 * @param tick_priority Tick interrupt priority.
 * @retval HAL status
 */
__weak HAL_Status_T HAL_InitTick(uint32_t tick_priority)
{
    /* Configure the SysTick to have interrupt in 1ms time basis */
    SystemCoreClockUpdate();
    if (SysTick_Config(SystemCoreClock / (1000U / uwTickFreq)) > 0U)
    {
        return HAL_ERROR;
    }

    /* Configure the SysTick IRQ priority */
    if (tick_priority < (1UL << __NVIC_PRIO_BITS))
    {
        uint32_t priority_group = NVIC_GetPriorityGrouping();
        NVIC_SetPriority(SysTick_IRQn, NVIC_EncodePriority(priority_group, tick_priority, 0U));
        uwTickPrio = tick_priority;
    }
    else
    {
        return HAL_ERROR;
    }

    /* Return function status */
    return HAL_OK;
}

/*
================================================================================
            ##### I/O Operation functions #####
================================================================================
*/

/**
 * @brief This function is called to increment a global variable "uwTick"
 *        used as application time base.
 * @note In the default implementation, this variable is incremented each 1ms
 *       in SysTick ISR.
 * @note This function is declared as __weak to be overwritten in case of other
 *       implementations in user file.
 * @retval None
 */
__weak void HAL_IncTick(void)
{
    uwTick += uwTickFreq;
}

/**
 * @brief This function provides minimum delay (in milliseconds) based
 *        on variable incremented.
 * @note In the default implementation, SysTick timer is the source of time base.
 *       It is used to generate interrupts at regular time intervals where uwTick
 *       is incremented.
 * @note This function is declared as __weak to be overwritten in case of other
 *       implementations in user file.
 * @param delay specifies the delay time length, in milliseconds.
 * @retval None
 */
__weak void HAL_Delay(uint32_t delay)
{
    uint32_t tick_start = HAL_GetTick();
    uint32_t wait       = delay;

    while ((HAL_GetTick() - tick_start) < wait)
    {
        __NOP();
    }
}

/**
 * @brief Provides a tick value in millisecond.
 * @note This function is declared as __weak to be overwritten in case of other
 *       implementations in user file.
 * @retval tick value
 */
__weak uint32_t HAL_GetTick(void)
{
    return uwTick;
}

/**
 * @brief This function returns a tick priority.
 * @retval tick priority
 */
uint32_t HAL_GetTickPrio(void)
{
    return uwTickPrio;
}

/**
 * @brief Set new tick frequency.
 * @param freq Tick frequency.
 * @retval Status
 */
HAL_Status_T HAL_SetTickFreq(HAL_TickFreq_T freq)
{
    HAL_Status_T   status = HAL_OK;
    HAL_TickFreq_T prev_tick_freq;

    ASSERT_PARAM(IS_TICK_FREQ(freq));

    if (uwTickFreq != freq)
    {
        /* Back up uwTickFreq frequency */
        prev_tick_freq = uwTickFreq;

        /* Update uwTickFreq global variable used by HAL_InitTick() */
        uwTickFreq = freq;

        /* Apply the new tick Freq  */
        status = HAL_InitTick(uwTickPrio);

        if (status != HAL_OK)
        {
            /* Restore previous tick frequency */
            uwTickFreq = prev_tick_freq;
        }
    }

    return status;
}

/**
 * @brief Return tick frequency.
 * @retval tick period in Hz
 */
HAL_TickFreq_T HAL_GetTickFreq(void)
{
    return uwTickFreq;
}

/**
 * @brief Suspend Tick increment.
 * @note In the default implementation , SysTick timer is the source of time base. It is
 *       used to generate interrupts at regular time intervals. Once HAL_SuspendTick()
 *       is called, the SysTick interrupt will be disabled and so Tick increment
 *       is suspended.
 * @note This function is declared as __weak to be overwritten in case of other
 *       implementations in user file.
 * @retval None
 */
__weak void HAL_SuspendTick(void)
{
    /* Disable SysTick Interrupt */
    SysTick->CTRL &= ~SysTick_CTRL_TICKINT_Msk;
}

/**
 * @brief Resume Tick increment.
 * @note In the default implementation , SysTick timer is the source of time base. It is
 *       used to generate interrupts at regular time intervals. Once HAL_ResumeTick()
 *       is called, the SysTick interrupt will be enabled and so Tick increment
 *       is resumed.
 * @note This function is declared as __weak to be overwritten in case of other
 *       implementations in user file.
 * @retval None
 */
__weak void HAL_ResumeTick(void)
{
    /* Enable SysTick Interrupt */
    SysTick->CTRL |= SysTick_CTRL_TICKINT_Msk;
}
