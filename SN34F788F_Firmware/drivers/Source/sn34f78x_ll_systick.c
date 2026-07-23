/***********************
 * SN34F78X SDK V2.1.4 *
 ***********************/

#include "sn34f78x_ll_systick.h"

#if defined(USE_FULL_LL_DRIVER)
__IO uint32_t tick      = 0;
uint32_t      tick_prio = (1UL << __NVIC_PRIO_BITS); /* Invalid PRIO */
LL_TickFreq_T tick_freq = LL_TICK_FREQ_DEFAULT;      /* 1KHz */

/* Exported functions --------------------------------------------------------*/
/**
 * @brief This function configures the source of the time base.
 *        The time source is configured to have 1ms time base with a dedicated
 *        tick interrupt priority.
 * @note This function is called automatically at the beginning of program after
 *       reset by LL_Init() or at any time when clock is reconfigured  by LL_RCC_ClockConfig().
 * @note In the default implementation, SysTick timer is the source of time base.
 *       It is used to generate interrupts at regular time intervals.
 *       Care must be taken if LL_Delay() is called from a peripheral ISR process,
 *       The SysTick interrupt must have higher priority (numerically lower)
 *       than the peripheral interrupt. Otherwise the caller ISR process will be blocked.
 *       The function is declared as __weak to be overwritten  in case of other
 *       implementation  in user file.
 * @param tick_priority Tick interrupt priority.
 * @retval LL status
 */
__weak LL_Status_T LL_InitTick(uint32_t tick_priority)
{
    /* Configure the SysTick to have interrupt in 1ms time basis */
    SystemCoreClockUpdate();
    if (SysTick_Config(SystemCoreClock / (1000U / tick_freq)) > 0U)
    {
        return LL_ERROR;
    }

    /* Configure the SysTick IRQ priority */
    if (tick_priority < (1UL << __NVIC_PRIO_BITS))
    {
        uint32_t priority_group = NVIC_GetPriorityGrouping();
        NVIC_SetPriority(SysTick_IRQn, NVIC_EncodePriority(priority_group, tick_priority, 0U));
        tick_prio = tick_priority;
    }
    else
    {
        return LL_ERROR;
    }

    /* Return function status */
    return LL_OK;
}

/**
 * @brief This function is called to increment a global variable "tick"
 *        used as application time base.
 * @note In the default implementation, this variable is incremented each 1ms
 *       in SysTick ISR.
 * @note This function is declared as __weak to be overwritten in case of other
 *       implementations in user file.
 * @retval None
 */
__weak void LL_IncTick(void)
{
    tick += tick_freq;
}

/**
 * @brief This function provides minimum delay (in milliseconds) based
 *        on variable incremented.
 * @note In the default implementation, SysTick timer is the source of time base.
 *       It is used to generate interrupts at regular time intervals where tick
 *       is incremented.
 * @note This function is declared as __weak to be overwritten in case of other
 *       implementations in user file.
 * @param delay specifies the delay time length, in milliseconds.
 * @retval None
 */
__weak void LL_Delay(uint32_t delay)
{
    uint32_t tick_start = LL_GetTick();
    uint32_t wait       = delay;

    while ((LL_GetTick() - tick_start) < wait)
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
__weak uint32_t LL_GetTick(void)
{
    return tick;
}

/**
 * @brief This function returns a tick priority.
 * @retval tick priority
 */
uint32_t LL_GetTickPrio(void)
{
    return tick_prio;
}

/**
 * @brief Set new tick frequency.
 * @param freq Tick frequency.
 * @retval Status
 */
LL_Status_T LL_SetTickFreq(LL_TickFreq_T freq)
{
    LL_Status_T   status = LL_OK;
    LL_TickFreq_T prev_tick_freq;

    RET_FLAG_FALSE(IS_LL_TICK_FREQ(freq), LL_ERROR);

    if (tick_freq != freq)
    {
        /* Back up tick_freq frequency */
        prev_tick_freq = tick_freq;

        /* Update tick_freq global variable used by LL_InitTick() */
        tick_freq = freq;

        /* Apply the new tick Freq  */
        status = LL_InitTick(tick_prio);

        if (status != LL_OK)
        {
            /* Restore previous tick frequency */
            tick_freq = prev_tick_freq;
        }
    }

    return status;
}

/**
 * @brief Return tick frequency.
 * @retval tick period in Hz
 */
LL_TickFreq_T LL_GetTickFreq(void)
{
    return tick_freq;
}

/**
 * @brief Suspend Tick increment.
 * @note In the default implementation , SysTick timer is the source of time base. It is
 *       used to generate interrupts at regular time intervals. Once LL_SuspendTick()
 *       is called, the SysTick interrupt will be disabled and so Tick increment
 *       is suspended.
 * @note This function is declared as __weak to be overwritten in case of other
 *       implementations in user file.
 * @retval None
 */
__weak void LL_SuspendTick(void)
{
    /* Disable SysTick Interrupt */
    SysTick->CTRL &= ~SysTick_CTRL_TICKINT_Msk;
}

/**
 * @brief Resume Tick increment.
 * @note In the default implementation , SysTick timer is the source of time base. It is
 *       used to generate interrupts at regular time intervals. Once LL_ResumeTick()
 *       is called, the SysTick interrupt will be enabled and so Tick increment
 *       is resumed.
 * @note This function is declared as __weak to be overwritten in case of other
 *       implementations in user file.
 * @retval None
 */
__weak void LL_ResumeTick(void)
{
    /* Enable SysTick Interrupt */
    SysTick->CTRL |= SysTick_CTRL_TICKINT_Msk;
}

#endif /* USE_FULL_LL_DRIVER */
