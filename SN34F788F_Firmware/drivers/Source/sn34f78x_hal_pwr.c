/***********************
 * SN34F78X SDK V2.1.4 *
 ***********************/

#include "sn34f78x_hal_pwr.h"

#if (configUSE_PWR_PERIPHERAL == 1)

/* Exported functions --------------------------------------------------------*/
/*
================================================================================
            ##### Initialization/De-initialization functions #####
================================================================================
*/
/**
 * @brief  DeInitialize the PWR peripheral.
 * @retval None
 */
void HAL_PWR_DeInit(void)
{
    /* Reset PWR mode */
    HAL_REG_CLEAR(SN_SCU->PWRMODE);
}

/**
 * @brief  Enter Sleep mode.
 * @note   System clock is stopped and execution of instructions is suspended.
 *         Peripheral/Analog functions can continue operation. Processor state and registers,
 *         peripheral registers, and internal SRAM values are maintained and the logic levels of the pins remain static.
 * @note   Any interruption will cause wakeup.
 * @retval None
 */
void HAL_PWR_EnterSleepMode(void)
{
    /* Enable Sleep mode wakeup event interrupt */
    HAL_REG_WRITE(SN_SCU->IE_b.WAKEUP_EINT, HAL_ENABLE);
    /* Enable wakeup for NVIC */
    HAL_REG_WRITE(SN_SCU->SLP_WAKUPEN_b.SLP_WAKUP_INTEN, HAL_ENABLE);
    /* Set the SLEEPDEEP of the kernel register to 1 */
    HAL_REG_SBIT(SCB->SCR, SCB_SCR_SLEEPDEEP_Msk);
    /* Set Sleep mode */
    HAL_REG_SBIT(SN_SCU->PWRMODE, PWR_ENTER_SLEEPMODE);
    /* Request Wait For Interrupt */
    __WFI();

    /* Disable Sleep wakeup for NVIC */
    if (HAL_REG_READ(SN_SCU->SLP_WAKUPEN_b.SLP_WAKUP_INTEN) == HAL_ENABLE)
    {
        HAL_REG_WRITE(SN_SCU->SLP_WAKUPEN_b.SLP_WAKUP_INTEN, HAL_DISABLE);
    }
    /* Disable Sleep mode wakeup event event */
    if (HAL_REG_READ(SN_SCU->IE_b.WAKEUP_EINT) == HAL_ENABLE)
    {
        HAL_REG_WRITE(SN_SCU->IE_b.WAKEUP_EINT, HAL_DISABLE);
    }
    /* If a wake-up event occurs at the same time as entering sleep mode,
    the WFI instruction may be ignored and the CPU will execute the next program.
    When this occurs, the SLEEP bit cannot be cleared automatically. In this case,
    the PWRMODECLR bit can be used to clear it */
    HAL_REG_WRITE(SN_SCU->PWRMODEMISC_b.PWRMODECLR, 0);

    /* After wakeup, the CPU will continue to execute from here */
    /* Auto clear sleep bit after wakeup */
}

/**
 * @brief  Enter Deep Sleep mode.
 * @note   In Deep Sleep mode, all I/O pins keep the same state as in Running mode.
 *         Exit Deep Sleep mode by issuing an interrupt or a wakeup event.
 * @retval None
 */
void HAL_PWR_EnterDeepSleepMode(void)
{
    /* Clean boot-up status(write 1 as clean) (SPEC) */
    HAL_REG_WRITE(SN_SCU->BTUP_STS, PWR_BTUP_MASK);
    /* Enable Deep Sleep mode wakeup event interrupt */
    HAL_REG_WRITE(SN_SCU->IE_b.DS_WAKEUP_EINT, HAL_ENABLE);
    /* Set Deep Sleep mode */
    HAL_REG_SBIT(SN_SCU->PWRMODE, PWR_ENTER_DEEPSLEEPMODE);
    /* Request Wait For Interrupt */
    __WFI();

    /* Disable Deep Sleep mode wakeup event */
    if (HAL_REG_READ(SN_SCU->IE_b.DS_WAKEUP_EINT) == HAL_ENABLE)
    {
        HAL_REG_WRITE(SN_SCU->IE_b.DS_WAKEUP_EINT, HAL_DISABLE);
    }
    /* If a wake-up event occurs at the same time as entering sleep mode,
    the WFI instruction may be ignored and the CPU will execute the next program.
    When this occurs, the SLEEP bit cannot be cleared automatically. In this case,
    the PWRMODECLR bit can be used to clear it */
    HAL_REG_WRITE(SN_SCU->PWRMODEMISC_b.PWRMODECLR, 0);

    /* After wakeup, the CPU will continue to execute from here */
    /* Auto clear Deep Sleep bit after wakeup */
}

/**
 * @brief  Enter Deep Power Down mode.
 * @note   Power (Turn off the on-chip voltage regulator) and clocks are shut off to the entire chip
 *         with the exception of the WAKEUP pin. The processor state and registers,
 *         peripheral registers, and internal SRAM values are not retained.
 * @retval None
 */
void HAL_PWR_EnterDeepPowerDownMode(void)
{
    /* Clean boot-up status(write 1 as clean) (SPEC) */
    HAL_REG_WRITE(SN_SCU->BTUP_STS, PWR_BTUP_MASK);
    /* Enter Deep Power Down mode */
    HAL_REG_SBIT(SN_SCU->PWRMODE, PWR_ENTER_DPDMODE);

    /* Request Wait For Interrupt */
    __WFI();

    /* If a wake-up event occurs at the same time as entering sleep mode,
    the WFI instruction may be ignored and the CPU will execute the next program.
    When this occurs, the SLEEP bit cannot be cleared automatically. In this case,
    the PWRMODECLR bit can be used to clear it */
    HAL_REG_WRITE(SN_SCU->PWRMODEMISC_b.PWRMODECLR, 0);

    /* After wakeup, the system will reboot */
}

/**
 * @brief  Config wakeup source for Deep Sleep or Deep Power Down mode
 * @param  wakeup_source Wakeup Source, value of @ref PWR_wakeup_source
 * @return HAL_Status_T
 */
HAL_Status_T HAL_PWR_SetWakeUpSource(uint32_t wakeup_source)
{
    /* Parameter check (required) */
    if (!HAL_FLAG_LACKS(wakeup_source, ~PWR_BTUP_MASK))
    {
        return HAL_ERROR;
    }
    HAL_REG_CBIT(SN_SCU->BTUP_CTRL, PWR_BTUP_MASK);
    HAL_REG_SBIT(SN_SCU->BTUP_CTRL, wakeup_source);

    return HAL_OK;
}

/**
 * @brief Clear wakeup flag for Deep Sleep or Deep Power Down mode
 * @param wakeup_source wakeup source
 * @return HAL_Status_T
 */
HAL_Status_T HAL_PWR_ClearWakeUpFlag(uint32_t wakeup_source)
{
    /* Parameter check (required) */
    if (!HAL_FLAG_LACKS(wakeup_source, ~PWR_BTUP_MASK))
    {
        return HAL_ERROR;
    }
    /* Write 1 to clear the status */
    HAL_REG_WRITE(SN_SCU->BTUP_STS, wakeup_source);

    return HAL_OK;
}

/**
 * @brief  Get wakeup info for Deep Sleep or Deep Power Down mode
 * @return uint32_t
 */
uint32_t HAL_PWR_GetWakeUpInfo(void)
{
    return (HAL_REG_READ(SN_SCU->BTUP_STS) & PWR_BTUP_MASK);
}

/**
 * @brief  Check whether wakeup from Deep Power Down mode.
 * @retval uint8_t, 0 for not and 1 for yes
 */
uint8_t HAL_PWR_CheckDeepPowerDownFlag(void)
{
    return IS_PWR_WAKEUP_FROM_DPD;
}

/**
 * @brief  Clear Deep Power Down wakeup flag and GPIO hold on flag.
 * @retval None
 */
void HAL_PWR_ClearDeepPowerDownFlag(void)
{
    if (IS_PWR_WAKEUP_FROM_DPD)
    {
        /* Write 1 to clear the status */
        HAL_REG_WRITE(SN_SCU->BTUP_STS_b.DPDWKF, 1);
        /* Write 1 to clear the status */
        HAL_REG_WRITE(SN_SCU->BTUP_STS_b.GPIO_HOLD, 1);
    }
}

/**
 * @brief  Set SRAM Enable or Disable.
 * @param  value of HAL_ENABLE or HAL_DISABLE
 * @retval None
 */
void HAL_PWR_SetSRAMPwrSave(uint8_t is_pwr_save)
{
    SN_SCU->SRAM1CTRL_b.AUTOSLPEN   = is_pwr_save;
    SN_SCU->SRAM2CTRL_b.AUTOSLPEN   = is_pwr_save;
    SN_SCU->SRAM3CTRL_b.AUTOSLPEN   = is_pwr_save;
    SN_SCU->BKPSRAMCTRL_b.AUTOSLPEN = is_pwr_save;
}

/**
 * @brief  Check whether enter eFlash standby in sleep mode
 * @param  value of HAL_ENABLE or HAL_DISABLE
 * @retval None
 */
void HAL_PWR_SetEflash_Standby(uint8_t is_en)
{
    if (is_en)
    {
        __HAL_SCU_EFLASH_STANDBY_ON();
    }
    else
    {
        __HAL_SCU_EFLASH_STANDBY_OFF();
    }
}

/*
================================================================================
            ##### Event Callback functions functions #####
================================================================================
*/
/**
 * @brief  This function handles the PWR wakeup interrupt request.
 * @note   This API should be called under the WKP_IRQHandler().
 * @param[in] ris Interrupt status.
 * @retval None
 */
void HAL_PWR_WKP_IRQHandler(uint32_t ris)
{
    if (IS_PWR_WAKEUP_FROM_SLEEP(ris)) // Wakeup from Sleep mode
    {
        /* Disable Sleep wakeup for NVIC */
        HAL_REG_WRITE(SN_SCU->SLP_WAKUPEN_b.SLP_WAKUP_INTEN, HAL_DISABLE);
        /* Disable Sleep mode wakeup event event */
        HAL_REG_WRITE(SN_SCU->IE_b.WAKEUP_EINT, HAL_DISABLE);
        /* Clean Sleep mode NVIC interrupt status */
        if (IS_PWR_WAKEUP_FROM_SLEEP_INT)
        {
            HAL_REG_WRITE(SN_SCU->SLP_WAKUPST, PWR_WAKUPST);
        }
        /* Sleep mode callback*/
        HAL_PWR_SleepWkpCallback();
    }
    else if (IS_PWR_WAKEUP_FROM_DEEPSLEEP(ris)) // Wakeup from Deep Sleep mode*/
    {
        /* Disable Deep Sleep mode wakeup event */
        HAL_REG_WRITE(SN_SCU->IE_b.DS_WAKEUP_EINT, HAL_DISABLE);
        /* Deep Sleep mode callback*/
        HAL_PWR_DeepSleepWkpCallback();
    }
}

/**
 * @brief  PWR Sleep wakeup interrupt callback
 * @retval None
 */
__weak void HAL_PWR_SleepWkpCallback(void)
{
    /* NOTE: This function Should not be modified, when the callback is needed, the HAL_PWR_SleepWkpCallback could be implemented in the user file */
}

/**
 * @brief  PWR Deep Sleep wakeup interrupt callback
 * @retval None
 */
__weak void HAL_PWR_DeepSleepWkpCallback(void)
{
    /* NOTE: This function Should not be modified, when the callback is needed, the HAL_PWR_DeepSleepWkpCallback could be implemented in the user file */
}

/* Private functions ---------------------------------------------------------*/

#endif
