/***********************
 * SN34F78X SDK V2.1.4 *
 ***********************/

#include "sn34f78x_hal_scu.h"

__weak ATTR_ISR void HAL_PWR_Callback(uint32_t ris)
{
}

__weak ATTR_ISR void HAL_RCC_Callback(uint32_t ris)
{
}

__weak ATTR_ISR void HAL_RTC_Callback(uint32_t ris)
{
}

ATTR_ISR void HAL_SCU_IRQHandler(void)
{
    uint32_t value = SN_SCU->RIS;
    SN_SCU->RIS    = value;
    if (value & (SCU_RIS_DS_WAKEUP | SCU_RIS_SLP_WAKEUP))
    {
        HAL_PWR_Callback(value);
    }

    if (value & (SCU_RIS_FCS | SCU_RIS_REMAPCHG))
    {
        HAL_RCC_Callback(value);
    }

    if (value & (SCU_RIS_RTC_ALARM | SCU_RIS_RTC_PER | SCU_RIS_RTC_SEC))
    {
        HAL_RTC_Callback(value);
    }
}
