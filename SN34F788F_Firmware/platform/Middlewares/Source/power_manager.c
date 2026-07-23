/***********************
 * SN34F78X SDK V2.1.4 *
 ***********************/

#include "power_manager.h"
#include "hal_rcc.h"
#include "ll_reg_msk.h"

#if (configUSE_POWERMANAGER_MIDDLEWARE == 1U)

static PowerManager_Handle_T *_pm_handle = NULL;

#if (configUSE_POWERMANAGER_GPIO_WAKEUP == 1U)
static const GPIO_Wakeup_Info_T _gpio_wakeup_info_arr[] = {
    {P0_IRQn, PWR_WKPSRC_P0, SN_GPIO0},
    {P1_IRQn, PWR_WKPSRC_P1, SN_GPIO1},
    {P2_IRQn, PWR_WKPSRC_P2, SN_GPIO2},
    {P3_IRQn, PWR_WKPSRC_P3, SN_GPIO3},
};

static const uint8_t _gpio_irq[][2] = {
    {GPIO_PULL_DOWN, GPIO_MODE_IT_RISING},
    {GPIO_PULL_UP, GPIO_MODE_IT_FALLING},
    {GPIO_PULL_UP, GPIO_MODE_IT_BOTH},
};
#endif /* configUSE_POWERMANAGER_GPIO_WAKEUP */

/* Exported functions --------------------------------------------------------*/
/**
 * @brief  Initializes the PowerManager according to the specified parameters in the PowerManager_Init_T structure and initialize the associated handle.
 * @param  hpm Pointer to a PowerManager_Handle_T structure
 * @retval HAL status
 */

HAL_Status_T HAL_PM_Init(PowerManager_Handle_T *hpm)
{
    ASSERT_PARAM(hpm);
    PM_POWER_MODE_FALSE(hpm->init.low_power_mode, HAL_ERROR);

    uint32_t wkp_src_mask = 0;

    if (_pm_handle != NULL)
    {
        RET_FLAG_TRUE(_pm_handle != hpm, HAL_ERROR);
        RET_FLAG_TRUE(_pm_handle == hpm, HAL_OK);
    }
    _pm_handle = hpm;

    /* A check wake-up mode must exist */
    if (CHECK_RTC_HANDLE_NULL(hpm) && CHECK_GPIO_PIN_NONE(hpm))
    {
        return HAL_ERROR;
    }

#if (configUSE_POWERMANAGER_GPIO_WAKEUP == 1U)
    for (uint8_t cnt = 0; cnt < (sizeof(_gpio_wakeup_info_arr) / sizeof(_gpio_wakeup_info_arr)[0]); cnt++)
    {
        if (hpm->init.gpio[cnt].pins != GPIO_PIN_NONE)
        {
            /* Set the parameters required by the GPIO */
            if (_gpio_wakeup_init(cnt) != HAL_OK)
                return HAL_ERROR;
            wkp_src_mask |= _gpio_wakeup_info_arr[cnt].wakeup_src;
        }
    }
#endif /* configUSE_POWERMANAGER_GPIO_WAKEUP */

    /* Set the parameters required by the RTC */
#if (configUSE_POWERMANAGER_RTC_WAKEUP == 1U)
    /* If the user has not set rtc_handle, set one again */
    if (hpm->init.rtc.rtc_handle != NULL)
    {
        wkp_src_mask |= PWR_WKPSRC_RTC;
    }
#endif /* configUSE_POWERMANAGER_RTC_WAKEUP */

    /* When entering DPD and DeepSleep, you need to set the wake-up source */
    if ((hpm->init.low_power_mode == PM_DEEPSLEEP_MODE) || (hpm->init.low_power_mode == PM_DPD_MODE))
    {
        if (HAL_PWR_SetWakeUpSource(wkp_src_mask) != HAL_OK)
        {
            return HAL_ERROR;
        }
    }

    hpm->state = HAL_STATE_READY;

    return HAL_OK;
}

/**
 * @brief  DeInitialize the PowerManager.
 * @param  hpm Pointer to a PowerManager_Handle_T structure
 * @retval HAL status
 */
HAL_Status_T HAL_PM_DeInit(PowerManager_Handle_T *hpm)
{
    ASSERT_PARAM(hpm);

    /* If you are in DPD mode, release the relevant registers */
    HAL_PWR_DeInit();

    if (hpm->state == HAL_STATE_READY)
    {
#if (configUSE_POWERMANAGER_GPIO_WAKEUP == 1U)
        for (uint8_t cnt = 0; cnt < (sizeof(_gpio_wakeup_info_arr) / sizeof(_gpio_wakeup_info_arr)[0]); cnt++)
        {
            if (hpm->init.gpio[cnt].pins != GPIO_PIN_NONE)
            {
                HAL_GPIO_DeInit(_gpio_wakeup_info_arr[cnt].instance, hpm->init.gpio[cnt].pins);
            }
        }
#endif /* configUSE_POWERMANAGER_GPIO_WAKEUP */
        _pm_handle = NULL;
    }

    hpm->state = HAL_STATE_RESET;

    return HAL_OK;
}

/**
 * @brief  Enter the low power mode which specified in the PowerManager_Init_T structure.
 * @param  hpm Pointer to a PowerManager_Handle_T structure
 * @retval HAL status
 */
HAL_Status_T HAL_PM_EnterLowPowerMode(PowerManager_Handle_T *hpm)
{
    ASSERT_PARAM(hpm);

    if (hpm->state != HAL_STATE_READY)
    {
        return HAL_ERROR;
    }

    uint32_t mode = hpm->init.low_power_mode;

/* Configure RTC interrupts */
#if (configUSE_POWERMANAGER_RTC_WAKEUP == 1U)
    if (hpm->init.rtc.rtc_handle != NULL)
    {
        /* Configure alarm interrupt */
        if (_rtc_wakeup_irq() != HAL_OK)
        {
            return HAL_ERROR;
        }
    }
#endif /* configUSE_POWERMANAGER_RTC_WAKEUP */

    switch (mode)
    {
        case PM_SLEEP_MODE:
        {
            /* suspend systick */
            HAL_SuspendTick();
            HAL_PWR_EnterSleepMode();
            /* continue running after wakeup and resume systick */
            HAL_ResumeTick();
        }
        break;

        case PM_DEEPSLEEP_MODE:
        {
#if (configUSE_POWERMANAGER_GPIO_WAKEUP == 1U)
            /*At Deep Sleep Mode, GPIO only Non Clock Mode  */
            if (HAL_OK != _set_gpio_mode(GPIO_NONE_CLK_MODE))
            {
                return HAL_ERROR;
            }
#endif /* configUSE_POWERMANAGER_GPIO_WAKEUP */
            HAL_PWR_EnterDeepSleepMode();

#if (configUSE_POWERMANAGER_GPIO_WAKEUP == 1U)
            /*Exit Deep Sleep Mode, GPIO resumes  Non Clock Mode  */
            if (HAL_OK != _set_gpio_mode(GPIO_NORMAL_MODE))
            {
                return HAL_ERROR;
            }
#endif /* configUSE_POWERMANAGER_GPIO_WAKEUP */
        }
        break;

        case PM_DPD_MODE:
        {
#if (configUSE_POWERMANAGER_GPIO_WAKEUP == 1U)
            /*At DPD Mode, GPIO only Non Clock Mode  */
            if (HAL_OK != _set_gpio_mode(GPIO_NONE_CLK_MODE))
            {
                return HAL_ERROR;
            }
#endif /* configUSE_POWERMANAGER_GPIO_WAKEUP */
            HAL_PWR_EnterDeepPowerDownMode();
        }
        break;
    }

    return HAL_OK;
}

#if (configUSE_POWERMANAGER_CLOCK_CONFIG == 1U)
/**
 * @brief  Configure the oscillations and clocks of the system.
 * @param  clk_cfg Pointer to a PM_Clock_Config_T structure
 * @retval HAL status
 */
HAL_Status_T HAL_PM_ConfigSystemClock(PM_Clock_Config_T *clk_cfg)
{
    ASSERT_PARAM(clk_cfg);
    ASSERT_PARAM(HAL_EHS_FREQ_MHZ <= 25);
    ASSERT_PARAM(IS_CLOCK_CONFIG_ENABLEMENT(clk_cfg->ihrc_en));
    ASSERT_PARAM(IS_CLOCK_CONFIG_ENABLEMENT(clk_cfg->ehs_en));
    ASSERT_PARAM(IS_CLOCK_CONFIG_ENABLEMENT(clk_cfg->els_en));
    ASSERT_PARAM(IS_CLOCK_CONFIG_PLL_SRC(clk_cfg->pll_src));
    ASSERT_PARAM(IS_CLOCK_CONFIG_PLL_NS(clk_cfg->pll_ns));
    ASSERT_PARAM(IS_CLOCK_CONFIG_PLL_FS(clk_cfg->pll_fs));
    ASSERT_PARAM(IS_CLOCK_CONFIG_SYSCLK_SRC(clk_cfg->sys_clk_src));
    ASSERT_PARAM(IS_CLOCK_CONFIG_AHB_DIV(clk_cfg->ahb_div));
    ASSERT_PARAM(IS_CLOCK_CONFIG_APB_DIV(clk_cfg->apb0_div));
    ASSERT_PARAM(IS_CLOCK_CONFIG_APB_DIV(clk_cfg->apb1_div));

    /* Check the parameters */
    RET_FLAG_TRUE(((clk_cfg->sys_clk_src == PM_SYSCLKSRC_IHRC || clk_cfg->pll_src == PM_PLLSRC_IHRC) && (clk_cfg->ihrc_en == HAL_DISABLE)), HAL_ERROR);
    RET_FLAG_TRUE(((clk_cfg->sys_clk_src == PM_SYSCLKSRC_EHS || clk_cfg->pll_src == PM_PLLSRC_EHS) && (clk_cfg->ehs_en == HAL_DISABLE)), HAL_ERROR);
    RET_FLAG_TRUE(((clk_cfg->sys_clk_src == PM_SYSCLKSRC_PLL) && (clk_cfg->pll_src == PM_PLLSRC_NONE)), HAL_ERROR);

    HAL_Status_T status   = HAL_OK;
    bool         ihrc_rdy = false, ehs_rdy = false, pll_chg = false;

    /* Get current SYSCLK source and PLL infomation  */
    uint32_t pll_ctrl            = HAL_REG_READ(SN_SCU->PLLCTRL);
    uint8_t  current_sys_clk_src = (pll_ctrl & SCU_PLLCTRL_SYSCLKSEL) >> SCU_PLLCTRL_SYSCLKSEL_Pos;
    uint8_t  current_pll_clk_src = (pll_ctrl & SCU_PLLCTRL_PLLCLKSEL) >> SCU_PLLCTRL_PLLCLKSEL_Pos;
    uint8_t  current_pll_ns      = (pll_ctrl & SCU_PLLCTRL_NS) >> SCU_PLLCTRL_NS_Pos;
    uint8_t  current_pll_fs      = (pll_ctrl & SCU_PLLCTRL_FS) >> SCU_PLLCTRL_FS_Pos;
    bool     current_pll_en      = (pll_ctrl & SCU_PLLCTRL_PLLEN) >> SCU_PLLCTRL_PLLEN_Pos;

    RCC_OscConfig_t oscillator_config = {0};
    oscillator_config.PLL.PLLEn       = clk_cfg->pll_src == PM_PLLSRC_NONE ? HAL_DISABLE : HAL_ENABLE;
    oscillator_config.PLL.PLLSource   = clk_cfg->pll_src == PM_PLLSRC_NONE ? current_pll_clk_src : clk_cfg->pll_src;
    oscillator_config.PLL.NS          = clk_cfg->pll_src == PM_PLLSRC_NONE ? current_pll_ns : clk_cfg->pll_ns;
    oscillator_config.PLL.FS          = clk_cfg->pll_src == PM_PLLSRC_NONE ? current_pll_fs : clk_cfg->pll_fs;

    RCC_ClkConfig_t clock_config = {0};
    clock_config.SYSCLKSource    = clk_cfg->sys_clk_src;
    clock_config.AHBCLKDivider   = clk_cfg->ahb_div;
    clock_config.APB0CLKDivider  = clk_cfg->apb0_div;
    clock_config.APB1CLKDivider  = clk_cfg->apb1_div;

    /* Check whether the PLL is changed or not */
    if (clk_cfg->pll_src != PM_PLLSRC_NONE)
    {
        if (current_pll_clk_src != clk_cfg->pll_src ||
            current_pll_ns != clk_cfg->pll_ns ||
            current_pll_fs != clk_cfg->pll_fs)
        {
            pll_chg = true;
        }
    }

    /* Flash clock frequency */
    if (current_sys_clk_src == PM_SYSCLKSRC_PLL)
    {
        if (pll_chg)
        {
            /* If source of SYSCLK is configured as PLL and want to change any PLL setting(such as NS, FS...) as this time,
               Flash clock frequency should be set large enough in the whole sequence of SYSCLK frequency updating */
            FLASH_ConfigProgramInitTypeDef flash_config = {
                .OptionType = OPTIONCFG_CLKFREQ,
                .CLKFreq    = 192,
            };
            if (HAL_FLASHEx_ConfigProgram(&flash_config) != HAL_OK)
            {
                return HAL_ERROR;
            }
        }
    }

    /* Enable the oscillators or keep the oscillators running */
    if (clk_cfg->sys_clk_src == PM_SYSCLKSRC_IHRC || clk_cfg->pll_src == PM_PLLSRC_IHRC)
    {
        if (HAL_REG_READ(SN_SCU->IHRCCTRL_b.IHRCEN) == 0)
        {
            oscillator_config.OscillatorType |= RCC_OSC_TYPE_IHRC;
            oscillator_config.IHRCEn = HAL_ENABLE;
            ihrc_rdy                 = true;
        }
    }
    else if (clk_cfg->ihrc_en == HAL_DISABLE)
    {
        if (current_pll_clk_src != PM_PLLSRC_IHRC && current_sys_clk_src != PM_SYSCLKSRC_IHRC)
        {
            if (HAL_REG_READ(SN_SCU->IHRCCTRL_b.IHRCEN) == 1)
            {
                oscillator_config.OscillatorType |= RCC_OSC_TYPE_IHRC;
                oscillator_config.IHRCEn = HAL_DISABLE;
                ihrc_rdy                 = true;
            }
        }
    }

    if ((clk_cfg->sys_clk_src == PM_SYSCLKSRC_EHS) || (clk_cfg->pll_src == PM_PLLSRC_EHS))
    {
        if (HAL_REG_READ(SN_ALWAYSON->OSCCTL_b.EHSEN) == 0)
        {
            oscillator_config.OscillatorType |= RCC_OSC_TYPE_EHS;
            oscillator_config.EHSEn = HAL_ENABLE;
            ehs_rdy                 = true;
        }
    }
    else if (clk_cfg->ehs_en == HAL_DISABLE)
    {
        if (current_pll_clk_src != PM_PLLSRC_EHS && current_sys_clk_src != PM_SYSCLKSRC_EHS)
        {
            if (HAL_REG_READ(SN_ALWAYSON->OSCCTL_b.EHSEN) == 1)
            {
                oscillator_config.OscillatorType |= RCC_OSC_TYPE_EHS;
                oscillator_config.EHSEn = HAL_DISABLE;
                ehs_rdy                 = true;
            }
        }
    }

    if (clk_cfg->els_en == HAL_ENABLE)
    {
        if (HAL_REG_READ(SN_ALWAYSON->POR_MISC_b.ELSEN) == 0)
        {
            oscillator_config.OscillatorType |= RCC_OSC_TYPE_ELS;
            oscillator_config.ELSEn = HAL_ENABLE;
        }
    }
    else if (clk_cfg->els_en == HAL_DISABLE)
    {
        if (HAL_REG_READ(SN_ALWAYSON->POR_MISC_b.ELSEN) == 1)
        {
            oscillator_config.OscillatorType |= RCC_OSC_TYPE_ELS;
            oscillator_config.ELSEn = HAL_DISABLE;
        }
    }

    if (clk_cfg->pll_src == PM_PLLSRC_NONE)
    {
        if (current_sys_clk_src == PM_SYSCLKSRC_PLL)
        {
            oscillator_config.PLL.PLLEn = HAL_ENABLE;
        }
    }

    if (oscillator_config.OscillatorType != 0 ||
        current_pll_en != oscillator_config.PLL.PLLEn ||
        pll_chg == true)
    {
        status = (HAL_Status_T)HAL_RCC_OscConfig(&oscillator_config);
        RET_FLAG_FALSE(status == HAL_OK, status);

        if (clk_cfg->pll_src == PM_PLLSRC_NONE)
        {
            /* Disable PLL later */
            oscillator_config.PLL.PLLEn = HAL_DISABLE;
        }
    }

    /* Configure the clocks */
    if (current_sys_clk_src != clk_cfg->sys_clk_src)
    {
        clock_config.ClockType |= RCC_CLK_TYPE_SYSCLK;
    }
    clock_config.ClockType |= RCC_CLK_TYPE_HCLK | RCC_CLK_TYPE_APB0CLK | RCC_CLK_TYPE_APB1CLK;

    status = (HAL_Status_T)HAL_RCC_ClockConfig(&clock_config);
    RET_FLAG_FALSE(status == HAL_OK, status);

    /* Disable the oscillators */
    oscillator_config.OscillatorType = 0;

    if (!ihrc_rdy)
    {
        if (clk_cfg->ihrc_en == HAL_DISABLE)
        {
            if (HAL_REG_READ(SN_SCU->IHRCCTRL_b.IHRCEN) == 1)
            {
                oscillator_config.OscillatorType |= RCC_OSC_TYPE_IHRC;
                oscillator_config.IHRCEn = HAL_DISABLE;
                ihrc_rdy                 = true;
            }
        }
    }

    if (!ehs_rdy)
    {
        if (clk_cfg->ehs_en == HAL_DISABLE)
        {
            if (HAL_REG_READ(SN_ALWAYSON->OSCCTL_b.EHSEN) == 1)
            {
                oscillator_config.OscillatorType |= RCC_OSC_TYPE_EHS;
                oscillator_config.EHSEn = HAL_DISABLE;
                ehs_rdy                 = true;
            }
        }
    }

    if (oscillator_config.OscillatorType != 0 ||
        HAL_REG_READ(SN_SCU->PLLCTRL_b.PLLEN) != oscillator_config.PLL.PLLEn)
    {
        status = (HAL_Status_T)HAL_RCC_OscConfig(&oscillator_config);
        RET_FLAG_FALSE(status == HAL_OK, status);
    }

    return HAL_OK;
}

/**
 * @brief  Get the clock frequency of peripheral.
 * @param  ip Selected peripheral
 * @param  instance Register base address of peripheral
 * @param  clock Point to a to variable for saving the clock frequency
 * @retval HAL status
 */
HAL_Status_T HAL_PM_GetPeripheralClock(PM_CLKCFG_IP ip, void *instance, uint32_t *clock)
{
    ASSERT_PARAM(instance);
    ASSERT_PARAM(clock);

    switch (ip)
    {
        case PM_IP_I2C:
        {
            return _get_clock_i2c(instance, clock);
        }

        case PM_IP_UART:
        {
            return _get_clock_uart(instance, clock);
        }

        case PM_IP_SSP:
        {
            return _get_clock_ssp(instance, clock);
        }

        case PM_IP_ADC:
        {
            return _get_clock_adc(instance, clock);
        }

        case PM_IP_TPM:
        {
            return _get_clock_tpm(instance, clock);
        }

        case PM_IP_SD:
        {
            return _get_clock_sd(instance, clock);
        }

        default:
            return HAL_ERROR;
    }
}

/**
 * @brief  Set the clock frequency of peripheral.
 * @param  ip Selected peripheral
 * @param  instance Register base address of peripheral
 * @param  clock The clock frequency to be set
 * @retval HAL status
 */
HAL_Status_T HAL_PM_SetPeripheralClock(PM_CLKCFG_IP ip, void *instance, uint32_t clock)
{
    ASSERT_PARAM(instance);
    ASSERT_PARAM(clock);

    switch (ip)
    {
        case PM_IP_I2C:
        {
            return _set_clock_i2c(instance, clock);
        }

        case PM_IP_UART:
        {
            return _set_clock_uart(instance, clock);
        }

        case PM_IP_SSP:
        {
            return _set_clock_ssp(instance, clock);
        }

        case PM_IP_ADC:
        {
            return _set_clock_adc(instance, clock);
        }

        case PM_IP_TPM:
        {
            return _set_clock_tpm(instance, clock);
        }

        case PM_IP_SD:
        {
            return _set_clock_sd(instance, clock);
        }

        default:
            return HAL_ERROR;
    }
}
#endif /* configUSE_POWERMANAGER_CLOCK_CONFIG */

/**
 * @brief  Write data to backup SRAM.
 * @param  data Pointer to data buffer
 * @param  size Amount of data elements to be written
 * @retval HAL status
 */
HAL_Status_T HAL_PM_BKUPSRAM_Write(uint8_t *data, uint32_t size)
{
    __IO uint8_t *SRAM_ADDR = (uint8_t *)BKP_SRAM_BASE_ADDR;

    ASSERT_PARAM(data);
    ASSERT_PARAM(size);

    if (size > BKP_SRAM_SIZE_MAX)
    {
        return HAL_ERROR;
    }

    memcpy((void *)(SRAM_ADDR), (void *)data, size);

    return HAL_OK;
}

/**
 * @brief  Read data from backup SRAM.
 * @param  data Pointer to data buffer
 * @param  size Amount of data elements to be read
 * @retval HAL status
 */
HAL_Status_T HAL_PM_BKUPSRAM_Read(uint8_t *data, uint32_t size)
{
    __IO uint8_t *SRAM_ADDR = (uint8_t *)BKP_SRAM_BASE_ADDR;

    ASSERT_PARAM(data);
    ASSERT_PARAM(size);

    if (size > BKP_SRAM_SIZE_MAX)
    {
        return HAL_ERROR;
    }

    memcpy((void *)data, (void *)(SRAM_ADDR), size);

    return HAL_OK;
}

/**
 * @brief  Write data to backup register.
 * @param  data Value to be written
 * @retval HAL status
 */
HAL_Status_T HAL_PM_BKUPREG_Write(PM_BKUP_REG backup, uint32_t data)
{
    uint32_t tmp = 0U;

    tmp = (uint32_t)&(SN_ALWAYSON->BKPREG0);
    tmp += (backup * 4U);

    /* Write the specified register */
    *(__IO uint32_t *)tmp = (uint32_t)data;

    return HAL_OK;
}

/**
 * @brief  Read data from backup SRAM.
 * @retval Value read from backup register
 */
uint32_t HAL_PM_BKUPREG_Read(PM_BKUP_REG backup)
{
    uint32_t tmp = 0U;

    tmp = (uint32_t)&(SN_ALWAYSON->BKPREG0);
    tmp += (backup * 4U);

    /* Read the specified register */
    return (*(__IO uint32_t *)tmp);
}

/* Private functions ---------------------------------------------------------*/
#if (configUSE_POWERMANAGER_GPIO_WAKEUP == 1U)
static HAL_Status_T _gpio_wakeup_init(uint8_t cnt)
{
    GPIO_Init_T      wakeup_pin = {0};
    PM_GPIO_Config_T gpio_conf  = _pm_handle->init.gpio[cnt];

    /* Determine whether the gpio pin is within the range that can be set */
    if (!CHECK_GPIO_PIN(cnt, gpio_conf.pins))
    {
        return HAL_ERROR;
    }

    /* Configure the gpio to wake up */
    wakeup_pin.pull = _gpio_irq[gpio_conf.irq][0];
    wakeup_pin.mode = _gpio_irq[gpio_conf.irq][1];
    wakeup_pin.pin  = gpio_conf.pins;

    /* Configure the wakeup gpio */
    if (HAL_GPIO_Init(_gpio_wakeup_info_arr[cnt].instance, &wakeup_pin) != HAL_OK)
    {
        return HAL_ERROR;
    }

    /* Default Enable clock for GPIOx  */
    __HAL_GPIO_CLK_ENABLE(_gpio_wakeup_info_arr[cnt].instance);
    NVIC_ClearPendingIRQ(_gpio_wakeup_info_arr[cnt].irq_no);
    NVIC_EnableIRQ(_gpio_wakeup_info_arr[cnt].irq_no);

    return HAL_OK;
}

static HAL_Status_T _set_gpio_mode(uint8_t mode)
{
    for (uint8_t cnt = 0; cnt < (sizeof(_gpio_wakeup_info_arr) / sizeof(_gpio_wakeup_info_arr)[0]); cnt++)
    {
        if (_pm_handle->init.gpio[cnt].pins != GPIO_PIN_NONE)
        {
            if (0x1 == mode) // normal mode
            {
                if (HAL_GPIO_ChangeClockMode(_gpio_wakeup_info_arr[cnt].instance, GPIO_CLK_NORMAL) != HAL_OK)
                {
                    return HAL_ERROR;
                }
            }
            else if (0x0 == mode) // none clk mode
            {
                if (HAL_GPIO_ChangeClockMode(_gpio_wakeup_info_arr[cnt].instance, GPIO_CLK_NON) != HAL_OK)
                {
                    return HAL_ERROR;
                }
            }
            else
            {
                return HAL_ERROR;
            }
        }
    }

    return HAL_OK;
}
#endif /* configUSE_POWERMANAGER_GPIO_WAKEUP */

#if (configUSE_POWERMANAGER_RTC_WAKEUP == 1U)
static HAL_Status_T _rtc_wakeup_irq(void)
{
    RTC_Alarm_T salarm = {0};

    salarm.alarm_date_weekday_sel = RTC_ALARMSEL_DATE;
    salarm.alarm_date_weekday     = HAL_REG_READ(SN_SCU->RTC_TIME2_b.DATE);

    if (_pm_handle->init.rtc.alarm_relative != 0) // relative alarm time
    {
        /* The alarm clock time is consistent with the actual time */
        salarm.alarm_time.hours   = HAL_REG_READ(SN_SCU->RTC_TIME1_b.HOUR);
        salarm.alarm_time.minutes = HAL_REG_READ(SN_SCU->RTC_TIME1_b.MIN);
        salarm.alarm_time.seconds = HAL_REG_READ(SN_SCU->RTC_TIME1_b.SEC);

        /* Set an alarm for a count down time */
        _rtc_set_relative_time(&salarm.alarm_time, _pm_handle->init.rtc.alarm_relative);
    }
    else // absolute alarm time
    {
        /* Set an alarm for an absolute time */
        salarm.alarm_time.hours   = _pm_handle->init.rtc.alarm_absolute.hours;
        salarm.alarm_time.minutes = _pm_handle->init.rtc.alarm_absolute.minutes;
        salarm.alarm_time.seconds = _pm_handle->init.rtc.alarm_absolute.seconds;
    }

    /* Set interrupt wake up,  */
    if (HAL_RTC_SetAlarm_IT(_pm_handle->init.rtc.rtc_handle, &salarm, RTC_FORMAT_BIN) != HAL_OK)
    {
        return HAL_ERROR;
    }
    return HAL_OK;
}

static void _rtc_set_relative_time(RTC_Time_T *alarm_time, uint8_t time_s)
{
    uint8_t sec  = alarm_time->seconds;
    uint8_t min  = alarm_time->minutes;
    uint8_t hour = alarm_time->hours;

    hour = (hour + ((min + ((sec + time_s) / 60)) / 60)) % 24;
    min  = (min + ((sec + time_s) / 60)) % 60;
    sec  = (sec + time_s) % 60;

    alarm_time->seconds = sec;
    alarm_time->minutes = min;
    alarm_time->hours   = hour;
}
#endif /* configUSE_POWERMANAGER_RTC_WAKEUP */

#if (configUSE_POWERMANAGER_CLOCK_CONFIG == 1U)
static HAL_Status_T _get_clock_i2c(void *instance, uint32_t *clock)
{
    uint32_t count = 0, count_h = 0, divisor = 0, divisor_h = 0, scl_clk = 0;

    switch ((uint32_t)instance)
    {
        case SN_I2C0_BASE:
        {
            count   = HAL_REG_READ(SN_I2C0->CLKDIV_b.COUNT);
            count_h = HAL_REG_READ(SN_I2C0->CLKDIV_b.COUNTH);
        }
        break;

        case SN_I2C1_BASE:
        {
            count   = HAL_REG_READ(SN_I2C1->CLKDIV_b.COUNT);
            count_h = HAL_REG_READ(SN_I2C1->CLKDIV_b.COUNTH);
        }
        break;

        case SN_I2C2_BASE:
        {
            count   = HAL_REG_READ(SN_I2C2->CLKDIV_b.COUNT);
            count_h = HAL_REG_READ(SN_I2C2->CLKDIV_b.COUNTH);
        }
        break;

        default:
            return HAL_ERROR;
    }

    uint32_t apb1_clk = HAL_RCC_GetAPB1CLKFreq();
    divisor           = count * 2 + 6;
    divisor_h         = count_h * 2 + 6;
    scl_clk           = apb1_clk / divisor;
    if (scl_clk != 100000UL &&
        scl_clk != 400000UL &&
        scl_clk != 1000000UL)
    {
        scl_clk = apb1_clk / divisor_h;
        if (scl_clk != 30000000UL)
        {
            return HAL_ERROR;
        }
    }

    *clock = scl_clk;
    return HAL_OK;
}

static HAL_Status_T _get_clock_uart(void *instance, uint32_t *clock)
{
    uint32_t uart_index = 0, io_irda_uclk = 0, DL = 0, baud_rate = 0;

    switch ((uint32_t)instance)
    {
        case SN_UART0_BASE:
        {
            uart_index = 0;
            HAL_REG_WRITE(SN_UART0->LC_b.DLAB, 1);
            DL = (HAL_REG_READ(SN_UART0->DLM) << 8) | (HAL_REG_READ(SN_UART0->DLL));
            HAL_REG_WRITE(SN_UART0->LC_b.DLAB, 0);
        }
        break;

        case SN_UART1_BASE:
        {
            uart_index = 1;
            HAL_REG_WRITE(SN_UART1->LC_b.DLAB, 1);
            DL = (HAL_REG_READ(SN_UART1->DLM) << 8) | (HAL_REG_READ(SN_UART1->DLL));
            HAL_REG_WRITE(SN_UART1->LC_b.DLAB, 0);
        }
        break;

        case SN_UART2_BASE:
        {
            uart_index = 2;
            HAL_REG_WRITE(SN_UART2->LC_b.DLAB, 1);
            DL = (HAL_REG_READ(SN_UART2->DLM) << 8) | (HAL_REG_READ(SN_UART2->DLL));
            HAL_REG_WRITE(SN_UART2->LC_b.DLAB, 0);
        }
        break;

        case SN_UART3_BASE:
        {
            uart_index = 3;
            HAL_REG_WRITE(SN_UART3->LC_b.DLAB, 1);
            DL = (HAL_REG_READ(SN_UART3->DLM) << 8) | (HAL_REG_READ(SN_UART3->DLL));
            HAL_REG_WRITE(SN_UART3->LC_b.DLAB, 0);
        }
        break;

        case SN_UART4_BASE:
        {
            uart_index = 4;
            HAL_REG_WRITE(SN_UART4->LC_b.DLAB, 1);
            DL = (HAL_REG_READ(SN_UART4->DLM) << 8) | (HAL_REG_READ(SN_UART4->DLL));
            HAL_REG_WRITE(SN_UART4->LC_b.DLAB, 0);
        }
        break;

        case SN_UART5_BASE:
        {
            uart_index = 5;
            HAL_REG_WRITE(SN_UART5->LC_b.DLAB, 1);
            DL = (HAL_REG_READ(SN_UART5->DLM) << 8) | (HAL_REG_READ(SN_UART5->DLL));
            HAL_REG_WRITE(SN_UART5->LC_b.DLAB, 0);
        }
        break;

        default:
            return HAL_ERROR;
    }

    io_irda_uclk = __HAL_RCC_GET_UARTn_CLKSEL(uart_index) == 0 ? HAL_RCC_GetHCLKFreq() : HAL_RCC_GetPLLCLKFreq();
    baud_rate    = io_irda_uclk / DL / 16;

    *clock = baud_rate;
    return HAL_OK;
}

static HAL_Status_T _get_clock_ssp(void *instance, uint32_t *clock)
{
    uint32_t ssp_clk = 0, data_size = 0, sclk_div = 0, sclk = 0, operation_mode = 0;

    switch ((uint32_t)instance)
    {
        case SN_SPI0_BASE:
        {
            ssp_clk        = (__HAL_RCC_GET_SSP0_CLKSEL() == 0 ? HAL_RCC_GetHCLKFreq() : HAL_RCC_GetPLLCLKFreq());
            sclk_div       = HAL_REG_READ(SN_SPI0->CTRL1_b.SCLKDIV) + 1;
            data_size      = HAL_REG_READ(SN_SPI0->CTRL1_b.SDL) + 1;
            operation_mode = HAL_REG_READ(SN_SPI0->CTRL0_b.OPM);
        }
        break;

        case SN_SPI1_BASE:
        {
            ssp_clk        = (__HAL_RCC_GET_SSP1_CLKSEL() == 0 ? HAL_RCC_GetHCLKFreq() : HAL_RCC_GetPLLCLKFreq());
            sclk_div       = HAL_REG_READ(SN_SPI1->CTRL1_b.SCLKDIV) + 1;
            data_size      = HAL_REG_READ(SN_SPI1->CTRL1_b.SDL) + 1;
            operation_mode = HAL_REG_READ(SN_SPI1->CTRL0_b.OPM);
        }
        break;

        case SN_SPI2_BASE:
        {
            ssp_clk        = (__HAL_RCC_GET_SSP2_CLKSEL() == 0 ? HAL_RCC_GetHCLKFreq() : HAL_RCC_GetPLLCLKFreq());
            sclk_div       = HAL_REG_READ(SN_SPI2->CTRL1_b.SCLKDIV) + 1;
            data_size      = HAL_REG_READ(SN_SPI2->CTRL1_b.SDL) + 1;
            operation_mode = HAL_REG_READ(SN_SPI2->CTRL0_b.OPM);
        }
        break;

        default:
            return HAL_ERROR;
    }

    /* Check Master/Slave */
    if (operation_mode < 2) // Slave
    {
        return HAL_ERROR;
    }

    uint32_t apb1_clk = HAL_RCC_GetAPB1CLKFreq();
    sclk              = ssp_clk / (sclk_div * 2);
    if (7 * sclk >= 2 * apb1_clk * data_size)
    {
        return HAL_ERROR;
    }

    *clock = sclk;
    return HAL_OK;
}

static HAL_Status_T _get_clock_adc(void *instance, uint32_t *clock)
{
    uint32_t mclk_div = 0, mclk = 0;

    switch ((uint32_t)instance)
    {
        case SN_ADC0_BASE:
        {
            mclk_div = HAL_REG_READ(SN_ADC0->PRE) + 1;
        }
        break;

        default:
            return HAL_ERROR;
    }

    uint32_t apb1_clk = HAL_RCC_GetAPB1CLKFreq();
    mclk              = apb1_clk / mclk_div;

    *clock = mclk;
    return HAL_OK;
}

static HAL_Status_T _get_clock_tpm(void *instance, uint32_t *clock)
{
    uint32_t tpm_pclk = 0, tpm_clk_sel = 0, tpm_pc = 0, tpm_tc = 0;

    switch ((uint32_t)instance)
    {
        case SN_CT16B0_BASE:
        {
            tpm_clk_sel = __HAL_RCC_GET_CT16B0_CLKSEL();
            tpm_pc      = HAL_REG_READ(SN_CT16B0->PRE) + 1;
        }
        break;

        case SN_CT16B1_BASE:
        {
            tpm_clk_sel = __HAL_RCC_GET_CT16B1_CLKSEL();
            tpm_pc      = HAL_REG_READ(SN_CT16B1->PRE) + 1;
        }
        break;

        case SN_CT16B2_BASE:
        {
            tpm_clk_sel = __HAL_RCC_GET_CT16B2_CLKSEL();
            tpm_pc      = HAL_REG_READ(SN_CT16B2->PRE) + 1;
        }
        break;

        case SN_CT16B3_BASE:
        {
            tpm_clk_sel = __HAL_RCC_GET_CT16B3_CLKSEL();
            tpm_pc      = HAL_REG_READ(SN_CT16B3->PRE) + 1;
        }
        break;

        case SN_CT16B4_BASE:
        {
            tpm_clk_sel = __HAL_RCC_GET_CT16B4_CLKSEL();
            tpm_pc      = HAL_REG_READ(SN_CT16B4->PRE) + 1;
        }
        break;

        case SN_CT16B5_BASE:
        {
            tpm_clk_sel = __HAL_RCC_GET_CT16B5_CLKSEL();
            tpm_pc      = HAL_REG_READ(SN_CT16B5->PRE) + 1;
        }
        break;

        case SN_CT16B6_BASE:
        {
            tpm_clk_sel = __HAL_RCC_GET_CT16B6_CLKSEL();
            tpm_pc      = HAL_REG_READ(SN_CT16B6->PRE) + 1;
        }
        break;

        case SN_CT16B7_BASE:
        {
            tpm_clk_sel = __HAL_RCC_GET_CT16B7_CLKSEL();
            tpm_pc      = HAL_REG_READ(SN_CT16B7->PRE) + 1;
        }
        break;

        case SN_CT16B8_BASE:
        {
            tpm_clk_sel = __HAL_RCC_GET_CT16B8_CLKSEL();
            tpm_pc      = HAL_REG_READ(SN_CT16B8->PRE) + 1;
        }
        break;

        default:
            return HAL_ERROR;
    }

    if (tpm_clk_sel == RCC_CT16B_CLKSEL_HCLK)
    {
        tpm_pclk = HAL_RCC_GetHCLKFreq();
    }
    else if (tpm_clk_sel == RCC_CT16B_CLKSEL_PLLCLK)
    {
        tpm_pclk = HAL_RCC_GetPLLCLKFreq();
    }
    else if (tpm_clk_sel == RCC_CT16B5_CLKSEL_ELSCLK)
    {
        tpm_pclk = __ELS_FREQ;
    }
    tpm_tc = tpm_pclk / tpm_pc;

    *clock = tpm_tc;
    return HAL_OK;
}

static HAL_Status_T _get_clock_sd(void *instance, uint32_t *clock)
{
    uint32_t sd_clk = 0, div = 0, sdio_div = 0, sdio_clk = 0;

    switch ((uint32_t)instance)
    {
        case SN_SDIO_BASE:
        {
            sd_clk   = __HAL_RCC_GET_SDIO_CLKSEL() == 0 ? HAL_RCC_GetHCLKFreq() : HAL_RCC_GetPLLCLKFreq();
            div      = HAL_REG_READ(SN_SDIO->CLKCTRL_b.LCLKSET);
            sdio_div = div == 0 ? 1 : div * 2;
        }
        break;

        default:
            return HAL_ERROR;
    }

    sdio_clk = (sd_clk / 2) / sdio_div;
    if (sdio_clk != 400000 &&
        sdio_clk != 24000000 &&
        sdio_clk != 48000000)
    {
        return HAL_ERROR;
    }

    *clock = sdio_clk;
    return HAL_OK;
}

static HAL_Status_T _set_clock_i2c(void *instance, uint32_t scl_clk)
{
    if (scl_clk != 100000 &&
        scl_clk != 400000 &&
        scl_clk != 1000000 &&
        scl_clk != 3000000)
    {
        return HAL_ERROR;
    }

    uint32_t count = 0, count_h = 0, divisor = 0, divisor_h = 0;

    switch ((uint32_t)instance)
    {
        case SN_I2C0_BASE:
        {
            RET_FLAG_FALSE(__HAL_RCC_I2C0_IS_CLK_ENABLE(), HAL_ERROR);
        }
        break;

        case SN_I2C1_BASE:
        {
            RET_FLAG_FALSE(__HAL_RCC_I2C1_IS_CLK_ENABLE(), HAL_ERROR);
        }
        break;

        case SN_I2C2_BASE:
        {
            RET_FLAG_FALSE(__HAL_RCC_I2C2_IS_CLK_ENABLE(), HAL_ERROR);
        }
        break;

        default:
            return HAL_ERROR;
    }

    uint32_t apb1_clk = HAL_RCC_GetAPB1CLKFreq();
    if (scl_clk < 30000000UL) // FS mode
    {
        divisor = apb1_clk / scl_clk;
        /* COUNT >= 4 + GSR(2) + TSR(1) */
        for (count = 7; count <= 1048575; count++) // COUNT: 20 bits
        {
            if (count * 2 + 6 == divisor) // GSR_max + 4 = 6
            {
                switch ((uint32_t)instance)
                {
                    case SN_I2C0_BASE:
                    {
                        HAL_REG_WRITE(SN_I2C0->CLKDIV_b.COUNT, count);
                    }
                    break;

                    case SN_I2C1_BASE:
                    {
                        HAL_REG_WRITE(SN_I2C1->CLKDIV_b.COUNT, count);
                    }
                    break;

                    case SN_I2C2_BASE:
                    {
                        HAL_REG_WRITE(SN_I2C2->CLKDIV_b.COUNT, count);
                    }
                    break;
                }

                return HAL_OK;
            }
        }
    }
    else // HS mode
    {
        divisor_h = apb1_clk / scl_clk;
        /* COUNTH >= 4 + GSR(2) + TSR(1) */
        for (count_h = 7; count_h <= 255; count_h++) // COUNTH: 8 bits
        {
            if (count_h * 2 + 6 == divisor_h) // GSR_max + 4 = 6
            {
                switch ((uint32_t)instance)
                {
                    case SN_I2C0_BASE:
                    {
                        HAL_REG_WRITE(SN_I2C0->CLKDIV_b.COUNTH, count_h);
                    }
                    break;

                    case SN_I2C1_BASE:
                    {
                        HAL_REG_WRITE(SN_I2C1->CLKDIV_b.COUNTH, count_h);
                    }
                    break;

                    case SN_I2C2_BASE:
                    {
                        HAL_REG_WRITE(SN_I2C2->CLKDIV_b.COUNTH, count_h);
                    }
                    break;
                }

                return HAL_OK;
            }
        }
    }

    return HAL_ERROR;
}

static HAL_Status_T _set_clock_uart(void *instance, uint32_t baud_rate)
{
    uint32_t uart_index = 0, io_irda_uclk = 0, DL = 0;

    switch ((uint32_t)instance)
    {
        case SN_UART0_BASE:
        {
            RET_FLAG_FALSE(__HAL_RCC_UART0_IS_CLK_ENABLE(), HAL_ERROR);
            uart_index = 0;
        }
        break;

        case SN_UART1_BASE:
        {
            RET_FLAG_FALSE(__HAL_RCC_UART1_IS_CLK_ENABLE(), HAL_ERROR);
            uart_index = 1;
        }
        break;

        case SN_UART2_BASE:
        {
            RET_FLAG_FALSE(__HAL_RCC_UART2_IS_CLK_ENABLE(), HAL_ERROR);
            uart_index = 2;
        }
        break;

        case SN_UART3_BASE:
        {
            RET_FLAG_FALSE(__HAL_RCC_UART3_IS_CLK_ENABLE(), HAL_ERROR);
            uart_index = 3;
        }
        break;

        case SN_UART4_BASE:
        {
            RET_FLAG_FALSE(__HAL_RCC_UART4_IS_CLK_ENABLE(), HAL_ERROR);
            uart_index = 4;
        }
        break;

        case SN_UART5_BASE:
        {
            RET_FLAG_FALSE(__HAL_RCC_UART5_IS_CLK_ENABLE(), HAL_ERROR);
            uart_index = 5;
        }
        break;

        default:
            return HAL_ERROR;
    }

    io_irda_uclk = __HAL_RCC_GET_UARTn_CLKSEL(uart_index) == 0 ? HAL_RCC_GetHCLKFreq() : HAL_RCC_GetPLLCLKFreq();

    /* DL : Round the calculation */
    uint32_t DL_10 = ((io_irda_uclk * 10) / (baud_rate * 16));
    if (DL_10 < 10)
    {
        return HAL_ERROR;
    }
    DL = DL_10 / 10;
    if (DL_10 % 10 >= 5)
    {
        DL++;
    }

    /* Baud rate checking (Deviation < |3.125%|) */
    uint32_t real_baud_rate = io_irda_uclk / (DL * 16);
    if (real_baud_rate > baud_rate)
    {
        if (((real_baud_rate - baud_rate) * 1000 / baud_rate) >= 31)
        {
            return HAL_ERROR;
        }
    }
    else if (real_baud_rate < baud_rate)
    {
        if (((baud_rate - real_baud_rate) * 1000 / baud_rate) >= 31)
        {
            return HAL_ERROR;
        }
    }

    /* Check the inequation (56 * F_PCLK > F_B16XCLK) */
    uint32_t f_b16xclk = io_irda_uclk / (DL * 1); // PRE = 1
    uint32_t f_pclk    = 1;
    if (instance == SN_UART0 || instance == SN_UART5)
    {
        f_pclk = HAL_RCC_GetAPB1CLKFreq();
    }
    else
    {
        f_pclk = HAL_RCC_GetAPB0CLKFreq();
    }
    if (56 * f_pclk <= f_b16xclk)
    {
        return HAL_ERROR;
    }

    switch ((uint32_t)instance)
    {
        case SN_UART0_BASE:
        {
            HAL_REG_WRITE(SN_UART0->LC_b.DLAB, 1);
            HAL_REG_WRITE(SN_UART0->PRE, 1);
            HAL_REG_WRITE(SN_UART0->DLM, (DL >> 8) & 0xFFU);
            HAL_REG_WRITE(SN_UART0->DLL, DL & 0xFFU);
            HAL_REG_WRITE(SN_UART0->LC_b.DLAB, 0);
        }
        break;

        case SN_UART1_BASE:
        {
            HAL_REG_WRITE(SN_UART1->LC_b.DLAB, 1);
            HAL_REG_WRITE(SN_UART1->PRE, 1);
            HAL_REG_WRITE(SN_UART1->DLM, (DL >> 8) & 0xFFU);
            HAL_REG_WRITE(SN_UART1->DLL, DL & 0xFFU);
            HAL_REG_WRITE(SN_UART1->LC_b.DLAB, 0);
        }
        break;

        case SN_UART2_BASE:
        {
            HAL_REG_WRITE(SN_UART2->LC_b.DLAB, 1);
            HAL_REG_WRITE(SN_UART2->PRE, 1);
            HAL_REG_WRITE(SN_UART2->DLM, (DL >> 8) & 0xFFU);
            HAL_REG_WRITE(SN_UART2->DLL, DL & 0xFFU);
            HAL_REG_WRITE(SN_UART2->LC_b.DLAB, 0);
        }
        break;

        case SN_UART3_BASE:
        {
            HAL_REG_WRITE(SN_UART3->LC_b.DLAB, 1);
            HAL_REG_WRITE(SN_UART3->PRE, 1);
            HAL_REG_WRITE(SN_UART3->DLM, (DL >> 8) & 0xFFU);
            HAL_REG_WRITE(SN_UART3->DLL, DL & 0xFFU);
            HAL_REG_WRITE(SN_UART3->LC_b.DLAB, 0);
        }
        break;

        case SN_UART4_BASE:
        {
            HAL_REG_WRITE(SN_UART4->LC_b.DLAB, 1);
            HAL_REG_WRITE(SN_UART4->PRE, 1);
            HAL_REG_WRITE(SN_UART4->DLM, (DL >> 8) & 0xFFU);
            HAL_REG_WRITE(SN_UART4->DLL, DL & 0xFFU);
            HAL_REG_WRITE(SN_UART4->LC_b.DLAB, 0);
        }
        break;

        case SN_UART5_BASE:
        {
            HAL_REG_WRITE(SN_UART5->LC_b.DLAB, 1);
            HAL_REG_WRITE(SN_UART5->PRE, 1);
            HAL_REG_WRITE(SN_UART5->DLM, (DL >> 8) & 0xFFU);
            HAL_REG_WRITE(SN_UART5->DLL, DL & 0xFFU);
            HAL_REG_WRITE(SN_UART5->LC_b.DLAB, 0);
        }
        break;
    }

    return HAL_OK;
}

static HAL_Status_T _set_clock_ssp(void *instance, uint32_t sclk)
{
    uint32_t ssp_clk = 0, data_size = 0, sclk_div = 0, frame_format = 0, operation_mode = 0;

    switch ((uint32_t)instance)
    {
        case SN_SPI0_BASE:
        {
            RET_FLAG_FALSE(__HAL_RCC_SSP0_IS_CLK_ENABLE(), HAL_ERROR);
            ssp_clk        = (__HAL_RCC_GET_SSP0_CLKSEL() == 0 ? HAL_RCC_GetHCLKFreq() : HAL_RCC_GetPLLCLKFreq());
            data_size      = HAL_REG_READ(SN_SPI0->CTRL1_b.SDL) + 1;
            frame_format   = HAL_REG_READ(SN_SPI0->CTRL0_b.FFMT);
            operation_mode = HAL_REG_READ(SN_SPI0->CTRL0_b.OPM);
        }
        break;

        case SN_SPI1_BASE:
        {
            RET_FLAG_FALSE(__HAL_RCC_SSP1_IS_CLK_ENABLE(), HAL_ERROR);
            ssp_clk        = (__HAL_RCC_GET_SSP1_CLKSEL() == 0 ? HAL_RCC_GetHCLKFreq() : HAL_RCC_GetPLLCLKFreq());
            data_size      = HAL_REG_READ(SN_SPI1->CTRL1_b.SDL) + 1;
            frame_format   = HAL_REG_READ(SN_SPI1->CTRL0_b.FFMT);
            operation_mode = HAL_REG_READ(SN_SPI1->CTRL0_b.OPM);
        }
        break;

        case SN_SPI2_BASE:
        {
            RET_FLAG_FALSE(__HAL_RCC_SSP2_IS_CLK_ENABLE(), HAL_ERROR);
            ssp_clk        = (__HAL_RCC_GET_SSP2_CLKSEL() == 0 ? HAL_RCC_GetHCLKFreq() : HAL_RCC_GetPLLCLKFreq());
            data_size      = HAL_REG_READ(SN_SPI2->CTRL1_b.SDL) + 1;
            frame_format   = HAL_REG_READ(SN_SPI2->CTRL0_b.FFMT);
            operation_mode = HAL_REG_READ(SN_SPI2->CTRL0_b.OPM);
        }
        break;

        default:
            return HAL_ERROR;
    }

    /* Check Master/Slave */
    if (operation_mode < 2) // Slave
    {
        return HAL_ERROR;
    }

    /* Check the deviation */
    if ((((ssp_clk / 2) % sclk) * DEVIATION_MULTIPLE) > sclk)
    {
        return HAL_ERROR;
    }

    sclk_div = ssp_clk / sclk / 2;
    if (frame_format == 1) // SPI
    {
        if (sclk_div < 2)
        {
            return HAL_ERROR;
        }
    }
    else if (frame_format == 3) // I2S
    {
        if (sclk_div < 3)
        {
            return HAL_ERROR;
        }
    }

    uint32_t apb1_clk = HAL_RCC_GetAPB1CLKFreq();
    if (7 * sclk >= 2 * apb1_clk * data_size)
    {
        return HAL_ERROR;
    }

    switch ((uint32_t)instance)
    {
        case SN_SPI0_BASE:
        {
            HAL_REG_WRITE(SN_SPI0->CTRL1_b.SCLKDIV, sclk_div - 1);
        }
        break;

        case SN_SPI1_BASE:
        {
            HAL_REG_WRITE(SN_SPI1->CTRL1_b.SCLKDIV, sclk_div - 1);
        }
        break;

        case SN_SPI2_BASE:
        {
            HAL_REG_WRITE(SN_SPI2->CTRL1_b.SCLKDIV, sclk_div - 1);
        }
        break;
    }

    return HAL_OK;
}

static HAL_Status_T _set_clock_adc(void *instance, uint32_t mclk)
{
    uint32_t mclk_div = 0;

    switch ((uint32_t)instance)
    {
        case SN_ADC0_BASE:
        {
            RET_FLAG_FALSE(__HAL_RCC_ADC0_IS_CLK_ENABLE(), HAL_ERROR);
        }
        break;

        default:
            return HAL_ERROR;
    }

    uint32_t apb1_clk = HAL_RCC_GetAPB1CLKFreq();
    /* Check the deviation */
    if (((apb1_clk % mclk) * DEVIATION_MULTIPLE) > mclk)
    {
        return HAL_ERROR;
    }

    mclk_div = apb1_clk / mclk;
    if (mclk_div < 2U || mclk_div > 256U)
    {
        return HAL_ERROR;
    }

    switch ((uint32_t)instance)
    {
        case SN_ADC0_BASE:
        {
            HAL_REG_WRITE(SN_ADC0->PRE, mclk_div - 1);
        }
        break;
    }

    return HAL_OK;
}

static HAL_Status_T _set_clock_tpm(void *instance, uint32_t tpm_tc)
{
    uint32_t tpm_pclk = 0, tpm_clk_sel = 0, tpm_pc = 0;

    switch ((uint32_t)instance)
    {
        case SN_CT16B0_BASE:
        {
            RET_FLAG_FALSE(__HAL_RCC_CT16B0_IS_CLK_ENABLE(), HAL_ERROR);
            tpm_clk_sel = __HAL_RCC_GET_CT16B0_CLKSEL();
        }
        break;

        case SN_CT16B1_BASE:
        {
            RET_FLAG_FALSE(__HAL_RCC_CT16B1_IS_CLK_ENABLE(), HAL_ERROR);
            tpm_clk_sel = __HAL_RCC_GET_CT16B1_CLKSEL();
        }
        break;

        case SN_CT16B2_BASE:
        {
            RET_FLAG_FALSE(__HAL_RCC_CT16B2_IS_CLK_ENABLE(), HAL_ERROR);
            tpm_clk_sel = __HAL_RCC_GET_CT16B2_CLKSEL();
        }
        break;

        case SN_CT16B3_BASE:
        {
            RET_FLAG_FALSE(__HAL_RCC_CT16B3_IS_CLK_ENABLE(), HAL_ERROR);
            tpm_clk_sel = __HAL_RCC_GET_CT16B3_CLKSEL();
        }
        break;

        case SN_CT16B4_BASE:
        {
            RET_FLAG_FALSE(__HAL_RCC_CT16B4_IS_CLK_ENABLE(), HAL_ERROR);
            tpm_clk_sel = __HAL_RCC_GET_CT16B4_CLKSEL();
        }
        break;

        case SN_CT16B5_BASE:
        {
            RET_FLAG_FALSE(__HAL_RCC_CT16B5_IS_CLK_ENABLE(), HAL_ERROR);
            tpm_clk_sel = __HAL_RCC_GET_CT16B5_CLKSEL();
        }
        break;

        case SN_CT16B6_BASE:
        {
            RET_FLAG_FALSE(__HAL_RCC_CT16B6_IS_CLK_ENABLE(), HAL_ERROR);
            tpm_clk_sel = __HAL_RCC_GET_CT16B6_CLKSEL();
        }
        break;

        case SN_CT16B7_BASE:
        {
            RET_FLAG_FALSE(__HAL_RCC_CT16B7_IS_CLK_ENABLE(), HAL_ERROR);
            tpm_clk_sel = __HAL_RCC_GET_CT16B7_CLKSEL();
        }
        break;

        case SN_CT16B8_BASE:
        {
            RET_FLAG_FALSE(__HAL_RCC_CT16B8_IS_CLK_ENABLE(), HAL_ERROR);
            tpm_clk_sel = __HAL_RCC_GET_CT16B8_CLKSEL();
        }
        break;

        default:
            return HAL_ERROR;
    }

    if (tpm_clk_sel == RCC_CT16B_CLKSEL_HCLK)
    {
        tpm_pclk = HAL_RCC_GetHCLKFreq();
    }
    else if (tpm_clk_sel == RCC_CT16B_CLKSEL_PLLCLK)
    {
        tpm_pclk = HAL_RCC_GetPLLCLKFreq();
    }
    else if (tpm_clk_sel == RCC_CT16B5_CLKSEL_ELSCLK)
    {
        tpm_pclk = __ELS_FREQ;
    }

    /* Check the deviation */
    if (((tpm_pclk % tpm_tc) * DEVIATION_MULTIPLE) > tpm_tc)
    {
        return HAL_ERROR;
    }

    tpm_pc = tpm_pclk / tpm_tc;
    if (tpm_pc < 1U || tpm_pc > 256U)
    {
        return HAL_ERROR;
    }

    switch ((uint32_t)instance)
    {
        case SN_CT16B0_BASE:
        {
            HAL_REG_WRITE(SN_CT16B0->PRE, tpm_pc - 1);
        }
        break;

        case SN_CT16B1_BASE:
        {
            HAL_REG_WRITE(SN_CT16B1->PRE, tpm_pc - 1);
        }
        break;

        case SN_CT16B2_BASE:
        {
            HAL_REG_WRITE(SN_CT16B2->PRE, tpm_pc - 1);
        }
        break;

        case SN_CT16B3_BASE:
        {
            HAL_REG_WRITE(SN_CT16B3->PRE, tpm_pc - 1);
        }
        break;

        case SN_CT16B4_BASE:
        {
            HAL_REG_WRITE(SN_CT16B4->PRE, tpm_pc - 1);
        }
        break;

        case SN_CT16B5_BASE:
        {
            HAL_REG_WRITE(SN_CT16B5->PRE, tpm_pc - 1);
        }
        break;

        case SN_CT16B6_BASE:
        {
            HAL_REG_WRITE(SN_CT16B6->PRE, tpm_pc - 1);
        }
        break;

        case SN_CT16B7_BASE:
        {
            HAL_REG_WRITE(SN_CT16B7->PRE, tpm_pc - 1);
        }
        break;

        case SN_CT16B8_BASE:
        {
            HAL_REG_WRITE(SN_CT16B8->PRE, tpm_pc - 1);
        }
        break;
    }

    return HAL_OK;
}

static HAL_Status_T _set_clock_sd(void *instance, uint32_t sdio_clk)
{
    if (sdio_clk != 400000 &&
        sdio_clk != 24000000 &&
        sdio_clk != 48000000)
    {
        return HAL_ERROR;
    }

    uint32_t sd_clk = 0, sdio_div = 0;

    switch ((uint32_t)instance)
    {
        case SN_SDIO_BASE:
        {
            RET_FLAG_FALSE(__HAL_RCC_SDIO_IS_CLK_ENABLE(), HAL_ERROR);
            sd_clk = __HAL_RCC_GET_SDIO_CLKSEL() == 0 ? HAL_RCC_GetHCLKFreq() : HAL_RCC_GetPLLCLKFreq();
        }
        break;

        default:
            return HAL_ERROR;
    }

    /* Check the deviation */
    if ((((sd_clk / 2) % sdio_clk) * DEVIATION_MULTIPLE) > sdio_clk)
    {
        return HAL_ERROR;
    }

    sdio_div = (sd_clk / 2) / sdio_clk;
    if (sdio_div == 0 || (sdio_div & 0x01) == 0x01)
    {
        if (sdio_div != 1U)
        {
            return HAL_ERROR;
        }
    }

    switch ((uint32_t)instance)
    {
        case SN_SDIO_BASE:
        {
            HAL_REG_WRITE(SN_SDIO->CLKCTRL_b.LCLKSET, sdio_div / 2);
        }
        break;
    }

    return HAL_OK;
}
#endif /* configUSE_POWERMANAGER_CLOCK_CONFIG */

#endif /* configUSE_POWERMANAGER_MIDDLEWARE */
