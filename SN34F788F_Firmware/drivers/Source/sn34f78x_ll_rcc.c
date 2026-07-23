/***********************
 * SN34F78X SDK V2.1.4 *
 ***********************/

#include "sn34f78x_ll_rcc.h"

#if defined(USE_FULL_LL_DRIVER)

/**
 * @brief Initializes the RCC Oscillators according to the specified parameters in the LL_RCC_OscConfig_t
 * @param OscCfg pointer to a LL_RCC_OscConfig_t structure that contains the configuration information for the oscillators
 * @retval LL_Status_T
 */
LL_Status_T LL_RCC_OscConfig(LL_RCC_OscConfig_t *OscCfg)
{
    uint32_t tickstart;

    /* Check Null pointer */
    if (OscCfg == NULL)
    {
        return LL_ERROR;
    }

    RET_FLAG_FALSE(IS_LL_RCC_OSC_TYPE(OscCfg->OscillatorType), LL_ERROR);
    /*------------------------------- IHRC Configuration ------------------------*/
    if (OscCfg->OscillatorType & LL_RCC_OSC_TYPE_IHRC)
    {
        RET_FLAG_FALSE(IS_LL_RCC_OSCCLK_CFG(OscCfg->IHRCEn), LL_ERROR);
        if (OscCfg->IHRCEn == LL_RCC_OSCCLK_CFG_OFF)
        {
            /* IHRC clock is not allowed disable while being used as source clock for PLL or SYSCLK */
            if ((LL_RCC_IsActiveFlag_PLL_Enabled() == LL_ENABLE && LL_RCC_GetPLLClockStatus() == LL_RCC_PLL_SRC_IHRC) || (LL_RCC_GetSYSClockStatus() == LL_RCC_SYSCLK_SRC_IHRC))
            {
                return LL_ERROR;
            }
            else
            {
                LL_RCC_Disable_IHRC();
            }
        }
        else
        {
            LL_RCC_Enable_IHRC();
            tickstart = LL_GetTick();
            /* Wait until IHRC is ready */
            while (LL_RCC_IsActiveFlag_IHRC_Ready() != LL_ENABLE)
            {
                if ((LL_GetTick() - tickstart) > LL_RCC_IHRC_TIMEOUT)
                    return LL_TIMEOUT;
            }
        }
    }

    /*------------------------------- EHS Configuration ------------------------*/
    if (OscCfg->OscillatorType & LL_RCC_OSC_TYPE_EHS)
    {
        RET_FLAG_FALSE(IS_LL_RCC_OSCCLK_CFG(OscCfg->EHSEn), LL_ERROR);
        if (OscCfg->EHSEn == LL_RCC_OSCCLK_CFG_OFF)
        {
            /* EHS clock is not allowed disable while being used as source clock for PLL or SYSCLK */
            if ((LL_RCC_IsActiveFlag_PLL_Enabled() && LL_RCC_GetPLLClockStatus() == LL_RCC_PLL_SRC_EHS) || LL_RCC_GetSYSClockStatus() == LL_RCC_SYSCLK_SRC_EHS)
            {
                return LL_ERROR;
            }
            else
            {
                LL_RCC_Disable_EHS();
            }
        }
        else
        {
            /* External high-speed Xtal input/output pins */
            LL_GPIO_SetAFIO_P312(LL_GPIO_P312_XIN);
            LL_GPIO_SetAFIO_P313(LL_GPIO_P313_XOUT);

            LL_RCC_Disable_EHS_XTAL();

            if (LL_EHS_FREQ_MHZ <= 12)
                LL_RCC_SetFrequencyRangeOfEHS(LL_RCC_LDO_FREQUENCY_RANGE_12_16); // L2
            else if (LL_EHS_FREQ_MHZ <= 16)
                LL_RCC_SetFrequencyRangeOfEHS(LL_RCC_LDO_FREQUENCY_RANGE_10_12); // L3
            else if (LL_EHS_FREQ_MHZ <= 25)
                LL_RCC_SetFrequencyRangeOfEHS(LL_RCC_LDO_FREQUENCY_RANGE_16_25); // L4
            else
                return LL_ERROR;

            LL_RCC_Enable_EHS();

            tickstart = LL_GetTick();
            /* Wait until EHS is ready */
            while (!LL_RCC_IsActiveFlag_EHS())
            {
                if ((LL_GetTick() - tickstart) > LL_RCC_EHS_TIMEOUT)
                    return LL_TIMEOUT;
            }
        }
    }

    /*------------------------------- ELS Configuration ------------------------*/
    if (OscCfg->OscillatorType & LL_RCC_OSC_TYPE_ELS)
    {
        RET_FLAG_FALSE(IS_LL_RCC_OSCCLK_CFG(OscCfg->ELSEn), LL_ERROR);
        if (OscCfg->ELSEn == LL_RCC_OSCCLK_CFG_OFF)
        {
            LL_RCC_Enable_ELSFL();
            LL_RCC_Disable_ELS();
        }
        else
        {
            /* External low-speed Xtal input/output pins */
            LL_GPIO_SetAFIO_P310(LL_GPIO_P310_LXIN);
            LL_GPIO_SetAFIO_P311(LL_GPIO_P311_LXOUT);

            LL_RCC_Disable_ELSFL(); // Enable clock filter
            LL_RCC_Enable_ELS();

            tickstart = LL_GetTick();
            /* Wait until ELS is ready */
            while (LL_RCC_IsActiveFlag_ELS() != LL_ENABLE)
            {
                if ((LL_GetTick() - tickstart) > LL_RCC_ELS_TIMEOUT)
                    return LL_TIMEOUT;
            }
        }
    }

    /*------------------------------- PLL Configuration ------------------------*/
    RET_FLAG_FALSE(IS_LL_RCC_OSCCLK_CFG(OscCfg->PLL.PLLEn), LL_ERROR);
    if (OscCfg->PLL.PLLEn == LL_RCC_OSCCLK_CFG_OFF)
    {
        if (LL_RCC_GetSYSClockStatus() == LL_RCC_SYSCLK_SRC_PLL)
        {
            return LL_ERROR;
        }
        else
        {
            LL_RCC_Disable_PLL();
            _rcc_fcs_command();
        }
    }
    else
    {
        RET_FLAG_FALSE(IS_LL_RCC_PLL_SRC(OscCfg->PLL.PLLSource), LL_ERROR);
        RET_FLAG_FALSE(IS_LL_RCC_PLL_FS(OscCfg->PLL.FS), LL_ERROR);
        RET_FLAG_FALSE(IS_LL_RCC_PLL_NS(OscCfg->PLL.NS), LL_ERROR);

        LL_RCC_SetPLLClockSource(OscCfg->PLL.PLLSource);
        LL_RCC_SetPLLFreqOutputDivider(OscCfg->PLL.FS);
        LL_RCC_SetLoopDivider(OscCfg->PLL.NS);
        LL_RCC_Enable_PLL();

        SN_SCU->PWRMODE_b.FCS_PLLRSTOFF = LL_DISABLE; // reset PLL for locking
        _rcc_fcs_command();

        tickstart = LL_GetTick();
        while (LL_RCC_IsActiveFlag_PLLSTABLE() != LL_ENABLE)
        {
            if ((LL_GetTick() - tickstart) > LL_RCC_CLOCK_SWITCH_TIMEOUT)
                return LL_TIMEOUT;
        }
    }
    return LL_OK;
}

/**
 * @brief Initializes or updates the CPU, AHB and APB busses clocks according to the specified parameters in the LL_RCC_ClkConfig_t
 * @param ClkCfg pointer to a LL_RCC_ClkConfig_t structure that contains the configuration information for the specified clock
 * @retval LL_Status_T
 */
LL_Status_T LL_RCC_ClockConfig(LL_RCC_ClkConfig_t *ClkCfg)
{
    uint32_t                      hclk_freq_MHz;
    uint32_t                      tickstart;
    LL_FLASH_Config_ProgramInit_T CfgInit = {
        .option_type = LL_OPTION_CLKFREQ,
    };
    uint8_t ChgWaitStateAfterFCS = 0;

    /* Check Null pointer */
    if (ClkCfg == NULL)
        return LL_ERROR;

    RET_FLAG_FALSE(IS_LL_RCC_CLK_TYPE(ClkCfg->ClockType), LL_ERROR);

    SN_SCU->PWRMODE_b.FCS_PLLRSTOFF = LL_ENABLE; // Keep PLL active in FCS

    /* To update flash wait states when CPU clock is changed */
    if (ClkCfg->ClockType & (LL_RCC_CLK_TYPE_SYSCLK | LL_RCC_CLK_TYPE_HCLK))
    {
        hclk_freq_MHz = _rcc_get_new_hclk_freq(ClkCfg) / 1000000;

        LL_FLASH_GetConfig(&CfgInit);
        if (hclk_freq_MHz > CfgInit.clk_freq)
        {
            /* Increasing flash wait state time before frequency changes */
            CfgInit.option_type = LL_OPTION_CLKFREQ;
            CfgInit.clk_freq    = hclk_freq_MHz;
            if (LL_FLASH_ConfigProgram(&CfgInit) != LL_OK)
                return LL_ERROR;
            LL_FLASH_GetConfig(&CfgInit);
            if (CfgInit.clk_freq != hclk_freq_MHz)
                return LL_ERROR;
        }
        else
        {
            ChgWaitStateAfterFCS = 1;
        }
    }

    if (ClkCfg->ClockType & LL_RCC_CLK_TYPE_SYSCLK)
    {
        RET_FLAG_FALSE(IS_LL_RCC_SYSCLK_SRC(ClkCfg->SYSCLKSource), LL_ERROR);

        /* check source clock ready */
        if ((ClkCfg->SYSCLKSource == LL_RCC_SYSCLK_SRC_IHRC && LL_RCC_IsActiveFlag_IHRC_Ready() == LL_DISABLE) ||
            (ClkCfg->SYSCLKSource == LL_RCC_SYSCLK_SRC_EHS && LL_RCC_IsActiveFlag_ELS() == LL_DISABLE) ||
            (ClkCfg->SYSCLKSource == LL_RCC_SYSCLK_SRC_PLL && LL_RCC_IsActiveFlag_PLLSTABLE() == LL_DISABLE))
        {
            return LL_ERROR;
        }

        LL_RCC_SetSystemClockSource(ClkCfg->SYSCLKSource);
    }

    RET_FLAG_FALSE(IS_LL_RCC_HCLK_DIV(ClkCfg->AHBCLKDivider), LL_ERROR);
    RET_FLAG_FALSE(IS_LL_RCC_APB0APB1_CLK_DIV(ClkCfg->APB0CLKDivider), LL_ERROR);
    RET_FLAG_FALSE(IS_LL_RCC_APB0APB1_CLK_DIV(ClkCfg->APB1CLKDivider), LL_ERROR);

    if (ClkCfg->ClockType & LL_RCC_CLK_TYPE_HCLK)
    {
        LL_RCC_SetHCLKPrescaler(ClkCfg->AHBCLKDivider);
    }
    if (ClkCfg->ClockType & LL_RCC_CLK_TYPE_APB0CLK)
    {
        LL_RCC_SetAPB0Prescaler(ClkCfg->APB0CLKDivider);
    }
    if (ClkCfg->ClockType & LL_RCC_CLK_TYPE_APB1CLK)
    {
        LL_RCC_SetAPB0Prescaler(ClkCfg->APB1CLKDivider);
    }

    _rcc_fcs_command();
    if (ClkCfg->ClockType & LL_RCC_CLK_TYPE_SYSCLK)
    {
        tickstart = LL_GetTick();
        while (LL_RCC_GetSYSClockStatus() != ClkCfg->SYSCLKSource)
        {
            if ((LL_GetTick() - tickstart) > LL_RCC_CLOCK_SWITCH_TIMEOUT)
                return LL_TIMEOUT;
        }
    }

    if (ChgWaitStateAfterFCS)
    {
        /* Decreasing flash wait state time before frequency changes */
        CfgInit.option_type = LL_OPTION_CLKFREQ;
        CfgInit.clk_freq    = hclk_freq_MHz;
        if (LL_FLASH_ConfigProgram(&CfgInit) != LL_OK)
            return LL_ERROR;
        LL_FLASH_GetConfig(&CfgInit);
        if (CfgInit.clk_freq != hclk_freq_MHz)
            return LL_ERROR;
    }

    /* Update SystemCoreClock global variable */
    SystemCoreClock = LL_RCC_GetSysClockFreq() >> LL_RCC_GetHCLKPrescaler();

    /* Configure the source of time base considering new system clocks settings */
    LL_InitTick(LL_GetTickPrio());
    return LL_OK;
}

/**
 * @brief  Clock output configuration
 * @param  Src  specifies clk-out source. This parameter can be one of @ref rcc_clkout_src
 * @param  Prescale  specifies the clk-out prescaler. This parameter can be one of @ref rcc_clkout_div
 * @param  Clockout_Pin specifies the clk-out pin. This parameter can be one of @ref rcc_clkout_pin
 * @retval None
 */
void LL_RCC_ClockoutConfig(uint32_t Src, uint32_t Prescale, uint32_t Clockout_Pin)
{
    LL_RCC_Disable_CLKOUTCLK();
    switch (Clockout_Pin)
    {
        case LL_RCC_CLKOUT_PIN_1:
            LL_GPIO_SetAFIO_P30(LL_GPIO_P30_CLKOUT1);
            break;
        case LL_RCC_CLKOUT_PIN_2:
            LL_GPIO_SetAFIO_P012(LL_GPIO_P012_CLKOUT2);
            break;
        case LL_RCC_CLKOUT_PIN_3:
            LL_GPIO_SetAFIO_P010(LL_GPIO_P010_CLKOUT3);
            break;
    }
    LL_RCC_SetCLKOUTSource(Src);
    LL_RCC_SetCLKOUTPrescaler(Prescale);
    LL_RCC_Enable_CLKOUTCLK();
}

/**
 * @brief  Get Oscillators configuration
 * @param  OscCfg pointer to a LL_RCC_OscConfig_t structure that contains the configuration information for the oscillators
 * @retval None
 */
void LL_RCC_GetOscConfig(LL_RCC_OscConfig_t *OscCfg)
{
    OscCfg->OscillatorType = LL_RCC_OSC_TYPE_EHS | LL_RCC_OSC_TYPE_ELS | LL_RCC_OSC_TYPE_IHRC;

    OscCfg->EHSEn         = SN_ALWAYSON->OSCCTL_b.EHSEN;
    OscCfg->ELSEn         = LL_RCC_IsEnabled_ELS();
    OscCfg->IHRCEn        = LL_RCC_IsEnabled_IHRC();
    OscCfg->PLL.PLLEn     = LL_RCC_IsActiveFlag_PLL_Enabled();
    OscCfg->PLL.PLLSource = LL_RCC_GetPLLClockStatus();
    OscCfg->PLL.NS        = LL_RCC_GetLoopDividerStatus();
    OscCfg->PLL.FS        = LL_RCC_GetPLLFreqDividerStatus();
}

/**
 * @brief  Get Oscillators configuration
 * @param  ClkCfg pointer to a LL_RCC_OscConfig_t structure that contains the configuration information for the specified clock
 * @retval None
 */
void LL_RCC_GetClockConfig(LL_RCC_ClkConfig_t *ClkCfg)
{
    ClkCfg->ClockType      = LL_RCC_CLK_TYPE_SYSCLK | LL_RCC_CLK_TYPE_HCLK | LL_RCC_CLK_TYPE_APB0CLK | LL_RCC_CLK_TYPE_APB1CLK;
    ClkCfg->SYSCLKSource   = LL_RCC_GetSYSClockStatus();
    ClkCfg->AHBCLKDivider  = LL_RCC_GetHCLKPrescaler();
    ClkCfg->APB0CLKDivider = LL_RCC_GetAPB0Prescaler();
    ClkCfg->APB1CLKDivider = LL_RCC_GetAPB1Prescaler();
}

/**
 * @brief   Get the PLL clock frequency
 * @retval  PLL clock frequency
 */
uint32_t LL_RCC_GetPLLCLKFreq(void)
{
    uint32_t div      = 0xffffffff;
    uint32_t PLL_freq = 0;

    if (LL_RCC_GetPLLClockStatus() == LL_RCC_PLL_SRC_IHRC)
        PLL_freq = _IHRC_FREQ;
    else
        PLL_freq = _EHS_FREQ;

    PLL_freq *= LL_RCC_GetLoopDividerStatus();

    switch (LL_RCC_GetPLLFreqDividerStatus())
    {
        case LL_RCC_PLL_DIV32:
            div = 32;
            break;
        case LL_RCC_PLL_DIV16:
            div = 16;
            break;
        case LL_RCC_PLL_DIV8:
            div = 8;
            break;
        case LL_RCC_PLL_DIV4:
            div = 4;
            break;
        default:
            break;
    }
    PLL_freq /= div;
    return PLL_freq;
}

/**
 * @brief Get the SYSCLK frequency
 * @retval SYSCLK frequency
 */
uint32_t LL_RCC_GetSysClockFreq(void)
{
    uint32_t sysclk_freq = 0;
    switch (LL_RCC_GetSYSClockStatus())
    {
        case LL_RCC_SYSCLK_SRC_IHRC:
            sysclk_freq = _IHRC_FREQ;
            break;
        case LL_RCC_SYSCLK_SRC_EHS:
            sysclk_freq = _EHS_FREQ;
            break;
        case LL_RCC_SYSCLK_SRC_PLL:
            sysclk_freq = LL_RCC_GetPLLCLKFreq();
            break;
        default:
            break;
    }
    return sysclk_freq;
}

/**
 * @brief   Get the HCLK clock frequency
 * @retval  HCLK clock frequency
 */
uint32_t LL_RCC_GetHCLKFreq(void)
{
    return LL_RCC_GetSysClockFreq() >> LL_RCC_GetHCLKPrescaler();
}

/**
 * @brief   Get the APB0 clock frequency
 * @retval  APB0 clock frequency
 */
uint32_t LL_RCC_GetAPB0CLKFreq(void)
{
    return LL_RCC_GetHCLKFreq() >> LL_RCC_GetAPB0Prescaler();
}

/**
 * @brief   Get the APB1 clock frequency
 * @retval  APB1 clock frequency
 */
uint32_t LL_RCC_GetAPB1CLKFreq(void)
{
    return LL_RCC_GetHCLKFreq() >> LL_RCC_GetAPB1Prescaler();
}

__weak ATTR_ISR void LL_LVD_Callback(void)
{
}

void LL_LVD_IRQHandler(void)
{
    if (LL_RCC_IsActiveFlag_LVDReset() == 1U)
    {
        LL_RCC_ClearFlag_LVDReset();
    }
    LL_LVD_Callback();
}

static void _rcc_fcs_command(void)
{
    uint32_t i;
    uint32_t wIntEnBackup[8], wSystickCTRLBackup, wSCUIEBackup;

    /* Disable all peripheral interrupt to avoid wake-up system. */
    for (i = 0; i < 8; i++) // backup NVIC interrupt enable value
    {
        wIntEnBackup[i] = NVIC->ISER[i];
        NVIC->ICER[i]   = 0xFFFFFFFF; // Clear all interrupt enable
    }
    wSystickCTRLBackup = SysTick->CTRL;
    SysTick->CTRL &= 0xFFFFFFFD; // Clear systick interrupt enable
    wSCUIEBackup = SN_SCU->IE;
    SN_SCU->IE   = 0; // Clear all SCU interrupt enable

    SN_SCU->RIS = SCU_RIS_INT_FCS;
    LL_SCU_EnableIT_FCS();

    NVIC_EnableIRQ(SCU_IRQn);
    LL_SCU_EnableEFCSTB();
    SN_SCU->PWRMODE_b.FCS = LL_ENABLE;
    __WFI();

    // Entering Idle mode for FCS sequence...
    LL_SCU_DisableEFCSTB();

    // Restore all interrupt setting.
    for (i = 0; i < 8; i++)
    {
        NVIC->ISER[i] = wIntEnBackup[i];
    }
    SysTick->CTRL = wSystickCTRLBackup;
    SN_SCU->IE    = wSCUIEBackup;
}

static uint32_t _rcc_get_new_hclk_freq(LL_RCC_ClkConfig_t *ClkCfg)
{
    uint32_t clk_src, sysclk_freq, ahb_pre;

    if (ClkCfg->ClockType & LL_RCC_CLK_TYPE_SYSCLK)
        clk_src = ClkCfg->SYSCLKSource;
    else
        clk_src = LL_RCC_GetSYSClockStatus();

    switch (clk_src)
    {
        case LL_RCC_SYSCLK_SRC_IHRC:
            sysclk_freq = _IHRC_FREQ;
            break;
        case LL_RCC_SYSCLK_SRC_EHS:
            sysclk_freq = _EHS_FREQ;
            break;
        case LL_RCC_SYSCLK_SRC_PLL:
            sysclk_freq = LL_RCC_GetPLLCLKFreq();
            break;
        default:
            sysclk_freq = 0xffff;
            break;
    }

    if (ClkCfg->ClockType & LL_RCC_CLK_TYPE_HCLK)
        ahb_pre = ClkCfg->AHBCLKDivider;
    else
        ahb_pre = LL_RCC_GetHCLKPrescaler();

    return sysclk_freq >> ahb_pre;
}

#endif /* USE_FULL_LL_DRIVER */
