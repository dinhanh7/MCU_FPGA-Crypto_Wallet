/***********************
 * SN34F78X SDK V2.1.4 *
 ***********************/

#include "sn34f78x_ll_scu.h"

#if defined(USE_FULL_LL_DRIVER)

/* static functions --------------------------------------------------------*/

/* Exported functions --------------------------------------------------------*/
__weak ATTR_ISR void LL_PWR_Callback(uint32_t ris)
{
}

__weak ATTR_ISR void LL_RCC_Callback(uint32_t ris)
{
}

__weak ATTR_ISR void LL_RTC_Callback(uint32_t ris)
{
}

ATTR_ISR void LL_SCU_IRQHandler(void)
{
    uint32_t value = SN_SCU->RIS;
    SN_SCU->RIS    = value;
    if (value & (SCU_RIS_INT_DS_WAKEUP | SCU_RIS_INT_WAKEUP))
    {
        LL_PWR_Callback(value);
    }

    if (value & (SCU_RIS_INT_FCS | SCU_RIS_INT_REMAPCHG))
    {
        LL_RCC_Callback(value);
    }

    if (value & (SCU_RIS_INT_RTC_ALARM | SCU_RIS_INT_RTC_PER | SCU_RIS_INT_RTC_SEC))
    {
        LL_RTC_Callback(value);
    }
}

#endif /* USE_FULL_LL_DRIVER */
