/***********************
 * SN34F78X SDK V2.1.4 *
 ***********************/

#include "sn34f78x_hal_rcc.h"

#if (configUSE_RCC_PERIPHERAL == 1)
/**
 * @brief Initializes the RCC Oscillators according to the specified parameters
 *        in the RCC_OscConfig_t
 * @param OscCfg pointer to a RCC_OscConfig_t structure that contains the configuration
 *        information for the RCC Oscillators.
 * @return HAL_Status_T
 */
uint32_t HAL_RCC_OscConfig(RCC_OscConfig_t *OscCfg)
{
    uint32_t tickstart;

    /* Check Null pointer */
    ASSERT_PARAM(OscCfg);
    ASSERT_PARAM(IS_RCC_OSC_TYPE(OscCfg->OscillatorType));

    /*------------------------------- IHRC Configuration ------------------------*/
    if (OscCfg->OscillatorType & RCC_OSC_TYPE_IHRC)
    {
        ASSERT_PARAM(IS_RCC_OSCCLK_CFG(OscCfg->IHRCEn));
        if (OscCfg->IHRCEn == RCC_OSCCLK_CFG_OFF)
        {
            /* IHRC clock is not allowed disable while being used as source clock for PLL or SYSCLK */
            if ((SN_SCU->PLLSTS_b.PLLENSTS == ENABLE && SN_SCU->PLLSTS_b.PLLCLKSTS == RCC_PLL_SRC_IHRC) ||
                (SN_SCU->PLLSTS_b.SYSCLKSTS == RCC_SYSCLK_SRC_IHRC))
            {
                return HAL_ERROR;
            }
            else
            {
                SN_SCU->IHRCCTRL_b.IHRCEN = DISABLE;
            }
        }
        else
        {
            SN_SCU->IHRCCTRL_b.IHRCEN = ENABLE;

            tickstart = HAL_GetTick();
            /* Wait until IHRC is ready */
            while (SN_SCU->IHRCCTRL_b.IHRCRDY != ENABLE)
            {
                if ((HAL_GetTick() - tickstart) > RCC_IHRC_TIMEOUT)
                    return HAL_TIMEOUT;
            }
        }
    }

    /*------------------------------- EHS Configuration ------------------------*/
    if (OscCfg->OscillatorType & RCC_OSC_TYPE_EHS)
    {
        ASSERT_PARAM(IS_RCC_OSCCLK_CFG(OscCfg->EHSEn));
        if (OscCfg->EHSEn == RCC_OSCCLK_CFG_OFF)
        {
            /* EHS clock is not allowed disable while being used as source clock for PLL or SYSCLK */
            if ((SN_SCU->PLLSTS_b.PLLENSTS && SN_SCU->PLLSTS_b.PLLCLKSTS == RCC_PLL_SRC_EHS) ||
                SN_SCU->PLLSTS_b.SYSCLKSTS == RCC_SYSCLK_SRC_EHS)
            {
                return HAL_ERROR;
            }
            else
            {
                SN_ALWAYSON->OSCCTL_b.EHSEN = DISABLE;
            }
        }
        else
        {
            /* External high-speed Xtal input/output pins */
            HAL_GPIO_SetAFIO(SN_GPIO3, GPIO_PIN_12, GPIO_P312_XIN);
            HAL_GPIO_SetAFIO(SN_GPIO3, GPIO_PIN_13, GPIO_P313_XOUT);

            SN_ALWAYSON->OSCCTL_b.EHSINROFF = HAL_EHS_INR_DS;

            if (HAL_EHS_FREQ_MHZ <= 12)
                SN_ALWAYSON->OSCMISC_b.EHSFREQ = 2; // L2
            else if (HAL_EHS_FREQ_MHZ <= 16)
                SN_ALWAYSON->OSCMISC_b.EHSFREQ = 1; // L3
            else if (HAL_EHS_FREQ_MHZ <= 25)
                SN_ALWAYSON->OSCMISC_b.EHSFREQ = 3; // L4
            else
                return HAL_ERROR;

            SN_ALWAYSON->OSCCTL_b.EHSEN = ENABLE;

            tickstart = HAL_GetTick();
            /* Wait until EHS is ready */
            while (SN_ALWAYSON->OSCRDY_b.EHSRDY != ENABLE)
            {
                if ((HAL_GetTick() - tickstart) > RCC_EHS_TIMEOUT)
                    return HAL_TIMEOUT;
            }
        }
    }

    /*------------------------------- ELS Configuration ------------------------*/
    if (OscCfg->OscillatorType & RCC_OSC_TYPE_ELS)
    {
        ASSERT_PARAM(IS_RCC_OSCCLK_CFG(OscCfg->ELSEn));
        if (OscCfg->ELSEn == RCC_OSCCLK_CFG_OFF)
        {
            SN_ALWAYSON->POR_MISC_b.ELSFLOFF = ENABLE;
            SN_ALWAYSON->POR_MISC_b.ELSEN    = DISABLE;
        }
        else
        {
            /* External low-speed Xtal input/output pins */
            HAL_GPIO_SetAFIO(SN_GPIO3, GPIO_PIN_10, GPIO_P310_LXIN);
            HAL_GPIO_SetAFIO(SN_GPIO3, GPIO_PIN_11, GPIO_P311_LXOUT);

            SN_ALWAYSON->POR_MISC_b.ELSFLOFF = DISABLE; // Enable clock filter
            SN_ALWAYSON->POR_MISC_b.ELSEN    = ENABLE;

            tickstart = HAL_GetTick();
            /* Wait until ELS is ready */
            while (SN_ALWAYSON->POR_MISC_b.ELSRDY != ENABLE)
            {
                if ((HAL_GetTick() - tickstart) > RCC_ELS_TIMEOUT)
                    return HAL_TIMEOUT;
            }
        }
    }

    /*------------------------------- PLL Configuration ------------------------*/
    ASSERT_PARAM(IS_RCC_OSCCLK_CFG(OscCfg->PLL.PLLEn));
    if (OscCfg->PLL.PLLEn == RCC_OSCCLK_CFG_OFF)
    {
        if (SN_SCU->PLLSTS_b.SYSCLKSTS == RCC_SYSCLK_SRC_PLL)
        {
            return HAL_ERROR;
        }
        else
        {
            SN_SCU->PLLCTRL_b.PLLEN = DISABLE;
            RCC_FCS_Command();
        }
    }
    else
    {
        ASSERT_PARAM(IS_RCC_PLL_SRC(OscCfg->PLL.PLLSource));
        ASSERT_PARAM(IS_RCC_PLL_FS(OscCfg->PLL.FS));
        ASSERT_PARAM(IS_RCC_PLL_NS(OscCfg->PLL.NS));
        SN_SCU->PLLCTRL_b.PLLCLKSEL     = OscCfg->PLL.PLLSource;
        SN_SCU->PLLCTRL_b.FS            = OscCfg->PLL.FS;
        SN_SCU->PLLCTRL_b.NS            = OscCfg->PLL.NS;
        SN_SCU->PLLCTRL_b.PLLEN         = ENABLE;
        SN_SCU->PWRMODE_b.FCS_PLLRSTOFF = DISABLE; // reset PLL for locking
        RCC_FCS_Command();

        tickstart = HAL_GetTick();
        while (SN_SCU->PLLCTRL_b.PLLSTABLE != ENABLE)
        {
            if ((HAL_GetTick() - tickstart) > RCC_CLOCK_SWITCH_TIMEOUT)
                return HAL_TIMEOUT;
        }
    }
    return HAL_OK;
}

/**
 * @brief Initializes or updates the CPU, AHB and APB busses clocks according to the specified
 *        parameters in the RCC_ClkConfig_t
 * @param ClkCfg pointer to a RCC_ClkConfig_t structure that contains the configuration
 *         information for the RCC peripheral.
 * @return HAL_Status_T
 */
uint32_t HAL_RCC_ClockConfig(RCC_ClkConfig_t *ClkCfg)
{
    uint32_t                       w_Temp, hclk_freq_MHz;
    uint32_t                       tickstart;
    FLASH_ConfigProgramInitTypeDef CfgInit = {
        .OptionType = OPTIONCFG_CLKFREQ,
    };
    uint8_t ChgWaitStateAfterFCS = 0;

    /* Check Null pointer */
    ASSERT_PARAM(ClkCfg);
    ASSERT_PARAM(IS_RCC_CLK_TYPE(ClkCfg->ClockType));

    SN_SCU->PWRMODE_b.FCS_PLLRSTOFF = ENABLE; // Keep PLL active in FCS

    /* To update flash wait states when CPU clock is changed */
    if (ClkCfg->ClockType & (RCC_CLK_TYPE_SYSCLK | RCC_CLK_TYPE_HCLK))
    {
        hclk_freq_MHz = HAL_GetNewHCLKFreq(ClkCfg) / 1000000;
        HAL_FLASHEx_GetConfig(&CfgInit);
        if (hclk_freq_MHz > CfgInit.CLKFreq)
        {
            /* Increasing flash wait state time before frequency changes */
            CfgInit.OptionType = OPTIONCFG_CLKFREQ;
            CfgInit.CLKFreq    = hclk_freq_MHz;
            if (HAL_FLASHEx_ConfigProgram(&CfgInit) != HAL_OK)
                return HAL_ERROR;
            HAL_FLASHEx_GetConfig(&CfgInit);
            if (CfgInit.CLKFreq != hclk_freq_MHz)
                return HAL_ERROR;
        }
        else
        {
            ChgWaitStateAfterFCS = 1;
        }
    }

    if (ClkCfg->ClockType & RCC_CLK_TYPE_SYSCLK)
    {
        ASSERT_PARAM(IS_RCC_SYSCLK_SRC(ClkCfg->SYSCLKSource));

        /* check source clock ready */
        if ((ClkCfg->SYSCLKSource == RCC_SYSCLK_SRC_IHRC && SN_SCU->IHRCCTRL_b.IHRCRDY == DISABLE) ||
            (ClkCfg->SYSCLKSource == RCC_SYSCLK_SRC_EHS && SN_ALWAYSON->OSCRDY_b.EHSRDY == DISABLE) ||
            (ClkCfg->SYSCLKSource == RCC_SYSCLK_SRC_PLL && SN_SCU->PLLCTRL_b.PLLSTABLE == DISABLE))
        {
            return HAL_ERROR;
        }

        SN_SCU->PLLCTRL_b.SYSCLKSEL = ClkCfg->SYSCLKSource;
    }

    ASSERT_PARAM(IS_RCC_HCLK_DIV(ClkCfg->AHBCLKDivider));
    ASSERT_PARAM(IS_RCC_APB0_APB1CLK_DIV(ClkCfg->APB0CLKDivider));
    ASSERT_PARAM(IS_RCC_APB0_APB1CLK_DIV(ClkCfg->APB1CLKDivider));

    w_Temp = SN_SCU->CLKPRE;
    w_Temp |= RCC_CLKPRE_WRPKEY;
    if (ClkCfg->ClockType & RCC_CLK_TYPE_HCLK)
    {
        w_Temp &= ~RCC_CLKPRE_AHB_Msk;
        w_Temp |= (ClkCfg->AHBCLKDivider << RCC_CLKPRE_AHB_Pos);
    }
    if (ClkCfg->ClockType & RCC_CLK_TYPE_APB0CLK)
    {
        w_Temp &= ~RCC_CLKPRE_APB0_Msk;
        w_Temp |= (ClkCfg->APB0CLKDivider << RCC_CLKPRE_APB0_Pos);
    }
    if (ClkCfg->ClockType & RCC_CLK_TYPE_APB1CLK)
    {
        w_Temp &= ~RCC_CLKPRE_APB1_Msk;
        w_Temp |= (ClkCfg->APB1CLKDivider << RCC_CLKPRE_APB1_Pos);
    }
    SN_SCU->CLKPRE = w_Temp;

    RCC_FCS_Command();
    if (ClkCfg->ClockType & RCC_CLK_TYPE_SYSCLK)
    {
        tickstart = HAL_GetTick();
        while ((SN_SCU->PLLSTS_b.SYSCLKSTS) != ClkCfg->SYSCLKSource)
        {
            if ((HAL_GetTick() - tickstart) > RCC_CLOCK_SWITCH_TIMEOUT)
                return HAL_TIMEOUT;
        }
    }

    if (ChgWaitStateAfterFCS)
    {
        /* Decreasing flash wait state time before frequency changes */
        CfgInit.OptionType = OPTIONCFG_CLKFREQ;
        CfgInit.CLKFreq    = hclk_freq_MHz;
        if (HAL_FLASHEx_ConfigProgram(&CfgInit) != HAL_OK)
            return HAL_ERROR;
        HAL_FLASHEx_GetConfig(&CfgInit);
        if (CfgInit.CLKFreq != hclk_freq_MHz)
            return HAL_ERROR;
    }

    /* Update SystemCoreClock global variable */
    SystemCoreClock = HAL_RCC_GetSysClockFreq() >> SN_SCU->CLKPRE_b.AHBPRE;

    /* Configure the source of time base considering new system clocks settings */
    HAL_InitTick(HAL_GetTickPrio());
    return HAL_OK;
}

void HAL_RCC_GetOscConfig(RCC_OscConfig_t *OscCfg)
{
    OscCfg->OscillatorType = RCC_OSC_TYPE_EHS | RCC_OSC_TYPE_ELS | RCC_OSC_TYPE_IHRC;

    OscCfg->EHSEn         = SN_ALWAYSON->OSCCTL_b.EHSEN;
    OscCfg->ELSEn         = SN_ALWAYSON->POR_MISC_b.ELSEN;
    OscCfg->IHRCEn        = SN_SCU->IHRCCTRL_b.IHRCEN;
    OscCfg->PLL.PLLEn     = SN_SCU->PLLSTS_b.PLLENSTS;
    OscCfg->PLL.PLLSource = SN_SCU->PLLSTS_b.PLLCLKSTS;
    OscCfg->PLL.NS        = SN_SCU->PLLSTS_b.NSSTS;
    OscCfg->PLL.FS        = SN_SCU->PLLSTS_b.FSSTS;
}
void HAL_RCC_GetClockConfig(RCC_ClkConfig_t *ClkCfg)
{
    ClkCfg->ClockType      = RCC_CLK_TYPE_SYSCLK | RCC_CLK_TYPE_HCLK | RCC_CLK_TYPE_APB0CLK | RCC_CLK_TYPE_APB1CLK;
    ClkCfg->SYSCLKSource   = SN_SCU->PLLSTS_b.SYSCLKSTS;
    ClkCfg->AHBCLKDivider  = SN_SCU->CLKPRE_b.AHBPRE;
    ClkCfg->APB0CLKDivider = SN_SCU->CLKPRE_b.APB0PRE;
    ClkCfg->APB1CLKDivider = SN_SCU->CLKPRE_b.APB1PRE;
}

uint32_t HAL_RCC_GetPLLCLKFreq(void)
{
    uint32_t div      = 0xffffffff;
    uint32_t PLL_freq = 0;

    if (SN_SCU->PLLSTS_b.PLLCLKSTS == RCC_PLL_SRC_IHRC)
        PLL_freq = __IHRC_FREQ;
    else
        PLL_freq = __EHS_FREQ;

    PLL_freq *= SN_SCU->PLLSTS_b.NSSTS;

    switch (SN_SCU->PLLSTS_b.FSSTS)
    {
        case RCC_PLL_DIV32:
            div = 32;
            break;
        case RCC_PLL_DIV16:
            div = 16;
            break;
        case RCC_PLL_DIV8:
            div = 8;
            break;
        case RCC_PLL_DIV4:
            div = 4;
            break;
        default:
            break;
    }
    PLL_freq /= div;
    return PLL_freq;
}

/**
 * @brief Returns the SYSCLK frequency
 * @return SYSCLK frequency
 */
uint32_t HAL_RCC_GetSysClockFreq(void)
{
    uint32_t sysclk_freq = 0;
    switch (SN_SCU->PLLSTS_b.SYSCLKSTS)
    {
        case RCC_SYSCLK_SRC_IHRC:
            sysclk_freq = __IHRC_FREQ;
            break;
        case RCC_SYSCLK_SRC_EHS:
            sysclk_freq = __EHS_FREQ;
            break;
        case RCC_SYSCLK_SRC_PLL:
            sysclk_freq = HAL_RCC_GetPLLCLKFreq();
            break;
        default:
            break;
    }
    return sysclk_freq;
}

uint32_t HAL_RCC_GetHCLKFreq(void)
{
    return HAL_RCC_GetSysClockFreq() >> SN_SCU->CLKPRE_b.AHBPRE;
}
uint32_t HAL_RCC_GetAPB0CLKFreq(void)
{
    return HAL_RCC_GetHCLKFreq() >> SN_SCU->CLKPRE_b.APB0PRE;
}
uint32_t HAL_RCC_GetAPB1CLKFreq(void)
{
    return HAL_RCC_GetHCLKFreq() >> SN_SCU->CLKPRE_b.APB1PRE;
}

/**
 * @brief  Clock output configuraton
 * @param  Src  specifies clkout source.
 *             This parameter can be one of @ref RCC_CLKOUT_Source
 * @param  Prescale   specifies the clkout prescaler.
 *             This parameter can be one of @ref RCC_Clkout_Divider
 * @param  Clockout_Pin specifies the clkout pin
                This parameter can be one of @ref RCC_Clkout_Pin
 * @retval None
 */
void HAL_RCC_ClockoutConfig(uint32_t Src, uint32_t Prescale, uint32_t Clockout_Pin)
{
    uint32_t w_Temp;
    SN_SCU->APB0CLKG_b.CLKOUTEN = DISABLE;
    switch (Clockout_Pin)
    {
        case RCC_CLKOUT_PIN_1:
            HAL_GPIO_SetAFIO(SN_GPIO3, GPIO_PIN_0, GPIO_P30_CLKOUT1);
            break;
        case RCC_CLKOUT_PIN_2:
            HAL_GPIO_SetAFIO(SN_GPIO0, GPIO_PIN_12, GPIO_P012_CLKOUT2);
            break;
        case RCC_CLKOUT_PIN_3:
            HAL_GPIO_SetAFIO(SN_GPIO0, GPIO_PIN_10, GPIO_P010_CLKOUT3);
            break;
    }
    w_Temp = SN_SCU->CLKPRE & ~RCC_CLKPRE_CLKOUT_Msk;
    w_Temp |= (RCC_CLKPRE_WRPKEY | (Prescale << RCC_CLKPRE_CLKOUT_Pos));
    SN_SCU->CLKPRE              = w_Temp;
    SN_SCU->CLKSEL_b.CLKOUTSEL  = (uint32_t)Src;
    SN_SCU->APB0CLKG_b.CLKOUTEN = ENABLE;
}

/**
 * @brief  Pripheral reset
 * @param  db_Delay  Reset delay cycle count.
 * @param  db_Active reset active cycle count.
 * @return HAL_Status_T
 */
uint32_t HAL_RCC_PeripheralReset(uint16_t Delay, uint16_t Active)
{
    uint32_t tickstart;
    // PERRST reset delay cycles
    SN_SCU->PERRSTCTL_b.PERRSTDC = Delay;

    // PERRST reset active cycle
    SN_SCU->PERRSTCTL_b.PERRSTAC = Active;

    tickstart = HAL_GetTick();
    // PRERST Command
    SN_SCU->PWRMODE_b.PERRST = ENABLE;
    /* Wait until peripheral reset is completed */
    while (SN_SCU->PWRMODE_b.PERRST == ENABLE)
    {
        if ((HAL_GetTick() - tickstart) > RCC_PERRST_TIMEOUT)
            return HAL_TIMEOUT;
    }
    return HAL_OK;
}

/**
 * @brief  Clears Pripheral reset mask
 * @retval None
 */
void HAL_RCC_PeripheralResetMaskClear(void)
{
    SN_SCU->AHBRSTMSK  = 0;
    SN_SCU->APB0RSTMSK = 0;
    SN_SCU->APB1RSTMSK = 0;
}

__weak ATTR_ISR void HAL_LVD_Callback(void)
{
}

void HAL_LVD_IRQHandler(void)
{
    uint32_t value     = SN_ALWAYSON->RSTST;
    SN_ALWAYSON->RSTST = value;
    HAL_LVD_Callback();
}

void LVD_Control(uint8_t LvdEn, uint8_t RstEn, LVD_RstInt_Level_t Lvl)
{
    SN_ALWAYSON->RSTST           = 0x04;    // Clear LVDF Status
    SN_ALWAYSON->LVDCTRL_b.LVDEN = DISABLE; // Disable LVD

    if (LvdEn == ENABLE)
    {
        SN_ALWAYSON->LVDCTRL_b.LVDLVL   = Lvl;
        SN_ALWAYSON->LVDCTRL_b.LVDRSTEN = RstEn;
        NVIC_ClearPendingIRQ(LVD_IRQn);
        NVIC_EnableIRQ(LVD_IRQn);
        SN_ALWAYSON->LVDCTRL_b.LVDEN = ENABLE;
    }
    else
    {
        NVIC_DisableIRQ(LVD_IRQn);
    }
}

void RCC_FCS_Command(void)
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

    __HAL_SCU_RIS_FCS_CLR();
    __HAL_SCU_SET_IE_FCS(ENABLE);
    NVIC_EnableIRQ(SCU_IRQn);
    __HAL_SCU_EFLASH_STANDBY_OFF();
    SN_SCU->PWRMODE_b.FCS = ENABLE;
    __WFI();

    // Entering Idle mode for FCS sequence...

    __HAL_SCU_EFLASH_STANDBY_ON();

    // Restore all interrupt setting.
    for (i = 0; i < 8; i++)
    {
        NVIC->ISER[i] = wIntEnBackup[i];
    }
    SysTick->CTRL = wSystickCTRLBackup;
    SN_SCU->IE    = wSCUIEBackup;
}
uint32_t HAL_GetNewHCLKFreq(RCC_ClkConfig_t *ClkCfg)
{
    uint32_t clk_src, sysclk_freq, ahb_pre;

    if (ClkCfg->ClockType & RCC_CLK_TYPE_SYSCLK)
        clk_src = ClkCfg->SYSCLKSource;
    else
        clk_src = SN_SCU->PLLSTS_b.SYSCLKSTS;

    switch (clk_src)
    {
        case RCC_SYSCLK_SRC_IHRC:
            sysclk_freq = __IHRC_FREQ;
            break;
        case RCC_SYSCLK_SRC_EHS:
            sysclk_freq = __EHS_FREQ;
            break;
        case RCC_SYSCLK_SRC_PLL:
            sysclk_freq = HAL_RCC_GetPLLCLKFreq();
            break;
        default:
            sysclk_freq = 0xffff;
            break;
    }

    if (ClkCfg->ClockType & RCC_CLK_TYPE_HCLK)
        ahb_pre = ClkCfg->AHBCLKDivider;
    else
        ahb_pre = SN_SCU->CLKPRE_b.AHBPRE;

    return sysclk_freq >> ahb_pre;
}

#endif /* configUSE_RCC_PERIPHERAL */
