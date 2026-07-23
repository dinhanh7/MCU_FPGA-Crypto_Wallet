/***********************
 * SN34F78X SDK V2.1.4 *
 ***********************/

#ifndef _LL_RCC_H_
#define _LL_RCC_H_

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "ll_scu.h"
#include "ll_reg_msk.h"

/* Exported types ------------------------------------------------------------*/
/**
 * \brief PLL clock configuration
 * \ingroup rcc_struct_define
 */
typedef struct
{
    uint32_t PLLEn;     /*!< The PLL Configures LL_Enable or LL_Disable.
                             This parameter can be a value of @ref rcc_osc_clk_config  */
    uint32_t PLLSource; /*!< PLL entry clock source.
                             This parameter must be a value of @ref rcc_pll_clk_sel */
    uint32_t NS;        /*!< programmable loop divider. PLLCLK = FREF * NS /FS
                             This parameter can be a number between Min_Data = 6 and Max_Data = 80 */
    uint32_t FS;        /*!< PLL frequency range output divider.
                             This parameter must be a value of @ref rcc_pll_clk_div */
} LL_RCC_PLLConfig_t;

/**
 * \brief Oscillators configuration
 * \ingroup rcc_struct_define
 */
typedef struct
{
    uint32_t OscillatorType; /*!< The oscillators to be configured.
                                  This parameter can be a value of @ref rcc_osc_type */
    uint32_t IHRCEn;         /*!< The IHRC Configures Enable or Disable.
                                  This parameter can be a value of @ref rcc_osc_clk_config */
    uint32_t ELSEn;          /*!< The ELS Configures Enable or Disable.
                                  This parameter can be a value of @ref rcc_osc_clk_config */
    uint32_t EHSEn;          /*!< The EHS Configures Enable or Disable.
                                  This parameter can be a value of @ref rcc_osc_clk_config */
    LL_RCC_PLLConfig_t PLL;  /*!< PLL structure parameters */

} LL_RCC_OscConfig_t;

/**
 * \brief Clock configuration
 * \ingroup rcc_struct_define
 */
typedef struct
{
    uint32_t ClockType;      /*!< The clock to be configured.
                                  This parameter can be a value of @ref rcc_clk_type */
    uint32_t SYSCLKSource;   /*!< The clock source (SYSCLKS) used as system clock.
                                  This parameter can be a value of @ref rcc_system_clk_sel */
    uint32_t AHBCLKDivider;  /*!< The AHB clock (HCLK) divider. This clock is derived from the system clock (SYSCLK).
                                  This parameter can be a value of @ref rcc_hclk_div */
    uint32_t APB0CLKDivider; /*!< The APB0 clock (APB0CLK) divider. This clock is derived from the AHB clock (HCLK).
                                  This parameter can be a value of @ref rcc_apb0_apb1_clk_div */
    uint32_t APB1CLKDivider; /*!< The APB1 clock (APB1CLK) divider. This clock is derived from the AHB clock (HCLK).
                                  This parameter can be a value of @ref rcc_apb0_apb1_clk_div */
} LL_RCC_ClkConfig_t;

/* Exported constants --------------------------------------------------------*/
/**
 * \defgroup rcc_osc_type Oscillator Configuration Type
 * \ingroup rcc_control
 * \note
 * @{
 */
#define LL_RCC_OSC_TYPE_NONE 0x00000000U /*!< No Configure any Oscillator */
#define LL_RCC_OSC_TYPE_ELS  0x00000001U /*!< To Configure ELS */
#define LL_RCC_OSC_TYPE_EHS  0x00000002U /*!< To Configure EHS */
#define LL_RCC_OSC_TYPE_IHRC 0x00000004U /*!< To Configure IHRC */
/**
 * @}
 */

/**
 * \defgroup rcc_osc_clk_config OSC/Clock Config
 * \ingroup rcc_control
 * @{
 */
#define LL_RCC_OSCCLK_CFG_OFF LL_DISABLE /*!< Enable oscillator */
#define LL_RCC_OSCCLK_CFG_ON  LL_ENABLE  /*!< Disable oscillator */
/**
 * @}
 */

/**
 * \defgroup rcc_pll_clk_sel PLL Clock Source Selection
 * \ingroup rcc_control
 * \note
 * @{
 */
#define LL_RCC_PLL_SRC_IHRC SCU_PLLCTRL_PLLCLKSEL_IHRC /*!< Select IHRC as PLL clock source */
#define LL_RCC_PLL_SRC_EHS  SCU_PLLCTRL_PLLCLKSEL_EHS  /*!< Select EHS as PLL clock source */
/**
 * @}
 */

/**
 * \defgroup rcc_pll_clk_div PLL Clock Divider
 * \ingroup rcc_control
 * \note
 * @{
 */
#define LL_RCC_PLL_DIV32 0 /*!< PLL CLKOUT = PLLCLK / 32 */
#define LL_RCC_PLL_DIV16 1 /*!< PLL CLKOUT = PLLCLK / 16 */
#define LL_RCC_PLL_DIV8  2 /*!< PLL CLKOUT = PLLCLK / 8 */
#define LL_RCC_PLL_DIV4  3 /*!< PLL CLKOUT = PLLCLK / 4 */
/**
 * @}
 */

/**
 * \defgroup rcc_clk_type Clock Configuration Type
 * \ingroup rcc_control
 * \note
 * @{
 */
#define LL_RCC_CLK_TYPE_SYSCLK  0x00000001U /*!< To Configure system clock */
#define LL_RCC_CLK_TYPE_HCLK    0x00000002U /*!< To Configure AHB clock */
#define LL_RCC_CLK_TYPE_APB0CLK 0x00000004U /*!< To Configure APB0 clock */
#define LL_RCC_CLK_TYPE_APB1CLK 0x00000008U /*!< To Configure APB1 clock */
/**
 * @}
 */

/**
 * \defgroup rcc_system_clk_sel System Clock Source Selection
 * \ingroup rcc_control
 * @{
 */
#define LL_RCC_SYSCLK_SRC_IHRC SCU_PLLCTRL_SYSCLKSEL_IHRC /*!< Select IHRC as system clock source */
#define LL_RCC_SYSCLK_SRC_EHS  SCU_PLLCTRL_SYSCLKSEL_EHS  /*!< Select EHS as system clock source */
#define LL_RCC_SYSCLK_SRC_PLL  SCU_PLLCTRL_SYSCLKSEL_PLL  /*!< Select PLL as system clock source */
/**
 * @}
 */

/**
 * \defgroup rcc_hclk_div AHB clock(HCLK) prescaler
 * \ingroup rcc_control
 * \note
 * @{
 */
#define LL_RCC_SYSCLK_DIV1   0 /*!< HCLK = SYSCLK / 1 */
#define LL_RCC_SYSCLK_DIV2   1 /*!< HCLK = SYSCLK / 2 */
#define LL_RCC_SYSCLK_DIV4   2 /*!< HCLK = SYSCLK / 4 */
#define LL_RCC_SYSCLK_DIV8   3 /*!< HCLK = SYSCLK / 8 */
#define LL_RCC_SYSCLK_DIV16  4 /*!< HCLK = SYSCLK / 16 */
#define LL_RCC_SYSCLK_DIV32  5 /*!< HCLK = SYSCLK / 32 */
#define LL_RCC_SYSCLK_DIV64  6 /*!< HCLK = SYSCLK / 64 */
#define LL_RCC_SYSCLK_DIV128 7 /*!< HCLK = SYSCLK / 128 */
/**
 * @}
 */

/**
 * \defgroup rcc_apb0_apb1_clk_div APB0/APB1 clock prescaler
 * \ingroup rcc_control
 * \note
 * @{
 */
#define LL_RCC_HCLK_DIV1   0 /*!< APBCLK = HCLK / 1 */
#define LL_RCC_HCLK_DIV2   1 /*!< APBCLK = HCLK / 2 */
#define LL_RCC_HCLK_DIV4   2 /*!< APBCLK = HCLK / 4 */
#define LL_RCC_HCLK_DIV8   3 /*!< APBCLK = HCLK / 8 */
#define LL_RCC_HCLK_DIV16  4 /*!< APBCLK = HCLK / 16 */
#define LL_RCC_HCLK_DIV32  5 /*!< APBCLK = HCLK / 32 */
#define LL_RCC_HCLK_DIV64  6 /*!< APBCLK = HCLK / 64 */
#define LL_RCC_HCLK_DIV128 7 /*!< APBCLK = HCLK / 128 */
/**
 * @}
 */

/**
 * \defgroup rcc_peripheral_clk_sel Peripheral Clock Source Selection
 * \ingroup rcc_control
 * @{
 */
#define LL_RCC_PERIPHERAL_CLKSRC_HCLK 0x00000000U /*!< Peripheral Clock Source: HCLK */
#define LL_RCC_PERIPHERAL_CLKSRC_PLL  0x00000001U /*!< Peripheral Clock Source: PLL */
/**
 * @}
 */

/**
 * \defgroup rcc_ct16b5_clk_sel CT16B5 Clock Source Selection
 * \ingroup rcc_control
 * @{
 */
#define LL_RCC_CT16B5_CLKSRC_HCLK 0x00000000U /*!< CT16B5 Clock Source: HCLK */
#define LL_RCC_CT16B5_CLKSRC_PLL  0x00000001U /*!< CT16B5 Clock Source: PLL */
#define LL_RCC_CT16B5_CLKSRC_ELS  0x00000002U /*!< CT16B5 Clock Source: ELS */
/**
 * @}
 */

/**
 * \defgroup rcc_wwdt_clk_sel WWDT Clock Source Selection
 * \ingroup rcc_control
 * @{
 */
#define LL_RCC_WWDT_CLKSRC_APB0 0x00000000U /*!< WWDT Clock Source: APB0 */
#define LL_RCC_WWDT_CLKSRC_ILRC 0x00000001U /*!< WWDT Clock Source: ILRC */
/**
 * @}
 */

/**
 * \defgroup rcc_clkout_src Clock Output source
 * \ingroup rcc_control
 * @{
 */
#define LL_RCC_CLKOUT_SRC_DISABLE SCU_CLKSEL_CLKOUTSEL_DISABLE /*!< Clock Output source: Disable */
#define LL_RCC_CLKOUT_SRC_ILRC    SCU_CLKSEL_CLKOUTSEL_ILRC    /*!< Clock Output source: ILRC */
#define LL_RCC_CLKOUT_SRC_ELS     SCU_CLKSEL_CLKOUTSEL_ELS     /*!< Clock Output source: ELS */
#define LL_RCC_CLKOUT_SRC_HCLK    SCU_CLKSEL_CLKOUTSEL_HCLK    /*!< Clock Output source: HCLK */
#define LL_RCC_CLKOUT_SRC_IHRC    SCU_CLKSEL_CLKOUTSEL_IHRC    /*!< Clock Output source: IHRC */
#define LL_RCC_CLKOUT_SRC_EHS     SCU_CLKSEL_CLKOUTSEL_EHS     /*!< Clock Output source: EHS */
#define LL_RCC_CLKOUT_SRC_PLL     SCU_CLKSEL_CLKOUTSEL_PLL     /*!< Clock Output source: PLL */
/**
 * @}
 */

/**
 * \defgroup rcc_clkout_div Clock Output prescaler
 * \ingroup rcc_control
 * @{
 */
#define LL_RCC_CLKOUT_PRESCALE_DIV1   SCU_CLKPRE_CLKOUTPRE_Div_1   /*!< Clock Output prescaler: Div 1 */
#define LL_RCC_CLKOUT_PRESCALE_DIV2   SCU_CLKPRE_CLKOUTPRE_Div_2   /*!< Clock Output prescaler: Div 2 */
#define LL_RCC_CLKOUT_PRESCALE_DIV3   SCU_CLKPRE_CLKOUTPRE_Div_3   /*!< Clock Output prescaler: Div 3 */
#define LL_RCC_CLKOUT_PRESCALE_DIV4   SCU_CLKPRE_CLKOUTPRE_Div_4   /*!< Clock Output prescaler: Div 4 */
#define LL_RCC_CLKOUT_PRESCALE_DIV6   SCU_CLKPRE_CLKOUTPRE_Div_6   /*!< Clock Output prescaler: Div 6 */
#define LL_RCC_CLKOUT_PRESCALE_DIV8   SCU_CLKPRE_CLKOUTPRE_Div_8   /*!< Clock Output prescaler: Div 8 */
#define LL_RCC_CLKOUT_PRESCALE_DIV16  SCU_CLKPRE_CLKOUTPRE_Div_16  /*!< Clock Output prescaler: Div 16 */
#define LL_RCC_CLKOUT_PRESCALE_DIV32  SCU_CLKPRE_CLKOUTPRE_Div_32  /*!< Clock Output prescaler: Div 32 */
#define LL_RCC_CLKOUT_PRESCALE_DIV64  SCU_CLKPRE_CLKOUTPRE_Div_64  /*!< Clock Output prescaler: Div 64 */
#define LL_RCC_CLKOUT_PRESCALE_DIV128 SCU_CLKPRE_CLKOUTPRE_Div_128 /*!< Clock Output prescaler: Div 128 */
/**
 * @}
 */

/**
 * \defgroup rcc_lcm_prescaler LCM clock prescaler
 * \ingroup rcc_control
 * @{
 */
#define LL_RCC_LCM_PRESCALE_DIV1  SCU_PRECTRL_LCMPRE_Div_1  /*!< LCM prescaler: Div 1 */
#define LL_RCC_LCM_PRESCALE_DIV2  SCU_PRECTRL_LCMPRE_Div_2  /*!< LCM prescaler: Div 2 */
#define LL_RCC_LCM_PRESCALE_DIV4  SCU_PRECTRL_LCMPRE_Div_4  /*!< LCM prescaler: Div 4 */
#define LL_RCC_LCM_PRESCALE_DIV8  SCU_PRECTRL_LCMPRE_Div_8  /*!< LCM prescaler: Div 8 */
#define LL_RCC_LCM_PRESCALE_DIV16 SCU_PRECTRL_LCMPRE_Div_16 /*!< LCM prescaler: Div 16 */
/**
 * @}
 */

/**
 * \defgroup rcc_gpio_strength GPIO driving/sinking strength
 * \ingroup rcc_control
 * @{
 */
#define LL_RCC_GPIO_STRENGTH_17MA 0x00000000U /*!< GPIO driving/sinking strength: 17 mA */
#define LL_RCC_GPIO_STRENGTH_19MA 0x00000001U /*!< GPIO driving/sinking strength: 19 mA */
#define LL_RCC_GPIO_STRENGTH_21MA 0x00000002U /*!< GPIO driving/sinking strength: 21 mA */
#define LL_RCC_GPIO_STRENGTH_23MA 0x00000003U /*!< GPIO driving/sinking strength: 23 mA */
/**
 * @}
 */

/**
 * \defgroup rcc_gpio_n GPIO n Select
 * \ingroup rcc_control
 * @{
 */
#define LL_RCC_GPIO_0 0x00000000U /*!< GPIO port 0 */
#define LL_RCC_GPIO_1 0x00000001U /*!< GPIO port 1 */
#define LL_RCC_GPIO_2 0x00000002U /*!< GPIO port 2 */
#define LL_RCC_GPIO_3 0x00000003U /*!< GPIO port 3 */
/**
 * @}
 */

/**
 * \defgroup rcc_gpio_pin GPIO pin Select
 * \ingroup rcc_control
 * @{
 */
#define LL_RCC_GPIO_PIN_0  0x00000000 /*!< GPIO pin 0 */
#define LL_RCC_GPIO_PIN_1  0x00000001 /*!< GPIO pin 1 */
#define LL_RCC_GPIO_PIN_2  0x00000002 /*!< GPIO pin 2 */
#define LL_RCC_GPIO_PIN_3  0x00000003 /*!< GPIO pin 3 */
#define LL_RCC_GPIO_PIN_4  0x00000004 /*!< GPIO pin 4 */
#define LL_RCC_GPIO_PIN_5  0x00000005 /*!< GPIO pin 5 */
#define LL_RCC_GPIO_PIN_6  0x00000006 /*!< GPIO pin 6 */
#define LL_RCC_GPIO_PIN_7  0x00000007 /*!< GPIO pin 7 */
#define LL_RCC_GPIO_PIN_8  0x00000008 /*!< GPIO pin 8 */
#define LL_RCC_GPIO_PIN_9  0x00000009 /*!< GPIO pin 9 */
#define LL_RCC_GPIO_PIN_10 0x0000000A /*!< GPIO pin 10 */
#define LL_RCC_GPIO_PIN_11 0x0000000B /*!< GPIO pin 11 */
#define LL_RCC_GPIO_PIN_12 0x0000000C /*!< GPIO pin 12 */
#define LL_RCC_GPIO_PIN_13 0x0000000D /*!< GPIO pin 13 */
#define LL_RCC_GPIO_PIN_14 0x0000000E /*!< GPIO pin 14 */
#define LL_RCC_GPIO_PIN_15 0x0000000F /*!< GPIO pin 15 */
#define LL_RCC_GPIO_PIN_20 0x00000010 /*!< GPIO pin 20 */
/**
 * @}
 */

/**
 * \defgroup rcc_clkout_pin Clock Output pin
 * \ingroup rcc_control
 * \note
 * @{
 */
#define LL_RCC_CLKOUT_PIN_1 0x00000000U /*!< Clock out pin sets to P3.0 */
#define LL_RCC_CLKOUT_PIN_2 0x00000001U /*!< Clock out pin sets to P0.12 */
#define LL_RCC_CLKOUT_PIN_3 0x00000002U /*!< Clock out pin sets to P0.10 */
/**
 * @}
 */

/**
 * \defgroup rcc_sram_n_ctrl SRAMnCTRL Select
 * \ingroup rcc_control
 * @{
 */
#define LL_RCC_SRAM_1   0x00000000U /*!< SRAM 1 */
#define LL_RCC_SRAM_2   0x00000001U /*!< SRAM 2 */
#define LL_RCC_SRAM_3   0x00000002U /*!< SRAM 3 */
#define LL_RCC_SRAM_BKP 0x00000003U /*!< Backup SRAM */
/**
 * @}
 */

/**
 * \defgroup rcc_frequency_range Frequency range of EHS XTAL
 * \ingroup rcc_control
 * @{
 */
#define LL_RCC_LDO_FREQUENCY_RANGE_10_12 0x00000001U /*!< 10MHz ~ 12MHz */
#define LL_RCC_LDO_FREQUENCY_RANGE_12_16 0x00000002U /*!< 12MHz ~ 16MHz */
#define LL_RCC_LDO_FREQUENCY_RANGE_16_25 0x00000003U /*!< 16MHz ~ 25MHz */
/**
 * @}
 */

/**
 * \defgroup rcc_rcc_timeout RCC source ready timeout
 * \ingroup rcc_control
 * @{
 */
#define LL_RCC_IHRC_TIMEOUT         (100) /* ms (256*F_IHRC)    */ /*!< IHRC ready timeout(ms) */
#define LL_RCC_EHS_TIMEOUT          (400) /* ms (32768*F_EHS)   */ /*!< EHS ready timeout(ms) */
#define LL_RCC_ELS_TIMEOUT          (800) /* ms (32K*F_ELS)     */ /*!< ELS ready timeout(ms) */
#define LL_RCC_CLOCK_SWITCH_TIMEOUT (100) /* ms                 */ /*!< Clock switch complete timeout(ms) */
#define LL_RCC_PERRST_TIMEOUT       (100)                          /*!< Peripherals reset ready timeout(ms) */
/**
 * @}
 */

/**
 * \defgroup rcc_reset_interrupt_level LVD reset/interrupt leve
 * \ingroup rcc_control
 * @{
 */
#define LL_RCC_RESET_INTERRUPT_LEVEL_2_4V (ALWAYSON_LVDCTRL_LVDLVL_2_4_V) /*!< IHRC ready timeout(ms)  (256*F_IHRC) */
#define LL_RCC_RESET_INTERRUPT_LEVEL_2_6V (ALWAYSON_LVDCTRL_LVDLVL_2_6_V) /*!< EHS ready timeout(ms)   (32768*F_EHS) */
#define LL_RCC_RESET_INTERRUPT_LEVEL_2_8V (ALWAYSON_LVDCTRL_LVDLVL_2_8_V) /*!< ELS ready timeout(ms)   (32K*F_ELS)   */
#define LL_RCC_RESET_INTERRUPT_LEVEL_3_0V (ALWAYSON_LVDCTRL_LVDLVL_3_0_V) /*!< Clock switch complete timeout(ms)     */
/**
 * @}
 */

#define SCU_PnSTR_GPIO_OFFSET(gpio_n, gpio_pin)     ((gpio_n == LL_RCC_GPIO_0 && gpio_pin == LL_RCC_GPIO_PIN_20) ? 4 : gpio_n)         /*!< GPIO offset */
#define SCU_PnSTR_GPIO_PIN_OFFSET(gpio_n, gpio_pin) ((gpio_n == LL_RCC_GPIO_0 && gpio_pin == LL_RCC_GPIO_PIN_20) ? 8 : (gpio_pin * 2)) /*!< GPIO pin offset */

/* Exported functions --------------------------------------------------------*/
/**
 * @brief  Enable PLL
 * @retval None
 */
__STATIC_INLINE void LL_RCC_Enable_PLL(void)
{
    LL_REG_SBIT(SN_SCU->PLLCTRL, SCU_PLLCTRL_PLLEN);
}

/**
 * @brief  Disable PLL
 * @retval None
 */
__STATIC_INLINE void LL_RCC_Disable_PLL(void)
{
    LL_REG_CBIT(SN_SCU->PLLCTRL, SCU_PLLCTRL_PLLEN);
}

/**
 * @brief  Return if enabled PLL
 * @retval 1: Enabled;   0: Disabled
 */
__STATIC_INLINE uint32_t LL_RCC_IsEnabled_PLL(void)
{
    return ((LL_REG_READ(SN_SCU->PLLCTRL) & SCU_PLLCTRL_PLLEN) >> SCU_PLLCTRL_PLLEN_Pos);
}

/**
 * @brief  Return if PLL status is stable
 * @retval 1: stable;   0: not stable
 */
__STATIC_INLINE uint32_t LL_RCC_IsActiveFlag_PLLSTABLE(void)
{
    return ((LL_REG_READ(SN_SCU->PLLCTRL) & SCU_PLLCTRL_PLLSTABLE) >> SCU_PLLCTRL_PLLSTABLE_Pos);
}

/**
 * @brief  Set PLL clock source
 * @param  clk_sel This parameter can be one of @ref rcc_pll_clk_sel
 * @retval None
 */
__STATIC_INLINE void LL_RCC_SetPLLClockSource(uint32_t clk_sel)
{
    LL_REG_CBIT(SN_SCU->PLLCTRL, SCU_PLLCTRL_PLLCLKSEL);
    LL_REG_SBIT(SN_SCU->PLLCTRL, clk_sel & SCU_PLLCTRL_PLLCLKSEL);
}

/**
 * @brief  Get PLL clock source
 * @retval Returned value can be one of @ref rcc_pll_clk_sel
 */
__STATIC_INLINE uint32_t LL_RCC_GetPLLClockSource(void)
{
    return (LL_REG_RBIT(SN_SCU->PLLCTRL, SCU_PLLCTRL_PLLCLKSEL));
}

/**
 * @brief  Set System clock source
 * @param  clk_sel This parameter can be one of @ref rcc_system_clk_sel
 * @retval None
 */
__STATIC_INLINE void LL_RCC_SetSystemClockSource(uint32_t clk_sel)
{
    LL_REG_CBIT(SN_SCU->PLLCTRL, SCU_PLLCTRL_SYSCLKSEL);
    LL_REG_SBIT(SN_SCU->PLLCTRL, clk_sel & SCU_PLLCTRL_SYSCLKSEL);
}

/**
 * @brief  Get System clock source
 * @retval Returned value can be one of @ref rcc_system_clk_sel
 */
__STATIC_INLINE uint32_t LL_RCC_GetSystemClockSource(void)
{
    return (LL_REG_RBIT(SN_SCU->PLLCTRL, SCU_PLLCTRL_SYSCLKSEL));
}

/**
 * @brief  Set PLL frequency range output divider
 * @param  divider frequency output divider, \ref rcc_pll_clk_div
 * @retval None
 */
__STATIC_INLINE void LL_RCC_SetPLLFreqOutputDivider(uint8_t divider)
{
    LL_REG_CBIT(SN_SCU->PLLCTRL, SCU_PLLCTRL_FS);
    LL_REG_SBIT(SN_SCU->PLLCTRL, divider << SCU_PLLCTRL_FS_Pos);
}

/**
 * @brief  Set PLL frequency range output divider
 * @retval None
 */
__STATIC_INLINE uint32_t LL_RCC_GetPLLFreqOutputDivider(void)
{
    return ((LL_REG_READ(SN_SCU->PLLCTRL) & SCU_PLLCTRL_FS) >> SCU_PLLCTRL_FS_Pos);
}

/**
 * @brief  Set 7-bit programmable loop divider, PLLCLK = FREF * NS/FS
 * @param  divider 7-bit programmable loop divider, range from 6 to 80
 * @retval None
 */
__STATIC_INLINE void LL_RCC_SetLoopDivider(uint8_t divider)
{
    LL_REG_CBIT(SN_SCU->PLLCTRL, SCU_PLLCTRL_NS);
    LL_REG_SBIT(SN_SCU->PLLCTRL, divider << SCU_PLLCTRL_NS_Pos);
}

/**
 * @brief  Get 7-bit programmable loop divider
 * @retval divider
 */
__STATIC_INLINE uint32_t LL_RCC_GetLoopDivider(void)
{
    return ((LL_REG_READ(SN_SCU->PLLCTRL) & SCU_PLLCTRL_NS) >> SCU_PLLCTRL_NS_Pos);
}

/**
 * @brief  Enable DMA0 Clock
 * @retval None
 */
__STATIC_INLINE void LL_RCC_Enable_DMA0CLK(void)
{
    LL_REG_SBIT(SN_SCU->AHBCLKG, SCU_AHBCLKG_DMA0CLKEN);
}

/**
 * @brief  Disable DMA0 Clock
 * @retval None
 */
__STATIC_INLINE void LL_RCC_Disable_DMA0CLK(void)
{
    LL_REG_CBIT(SN_SCU->AHBCLKG, SCU_AHBCLKG_DMA0CLKEN);
}

/**
 * @brief  Return if enabled DMA0 Clock
 * @retval 1: Enabled;   0: Disabled
 */
__STATIC_INLINE uint32_t LL_RCC_IsEnabled_DMA0CLK(void)
{
    return ((LL_REG_READ(SN_SCU->AHBCLKG) & SCU_AHBCLKG_DMA0CLKEN) >> SCU_AHBCLKG_DMA0CLKEN_Pos);
}

/**
 * @brief  Enable DMA1 Clock
 * @retval None
 */
__STATIC_INLINE void LL_RCC_Enable_DMA1CLK(void)
{
    LL_REG_SBIT(SN_SCU->AHBCLKG, SCU_AHBCLKG_DMA1CLKEN);
}

/**
 * @brief  Disable DMA1 Clock
 * @retval None
 */
__STATIC_INLINE void LL_RCC_Disable_DMA1CLK(void)
{
    LL_REG_CBIT(SN_SCU->AHBCLKG, SCU_AHBCLKG_DMA1CLKEN);
}

/**
 * @brief  Return if enabled DMA1 Clock
 * @retval 1: Enabled;   0: Disabled
 */
__STATIC_INLINE uint32_t LL_RCC_IsEnabled_DMA1CLK(void)
{
    return ((LL_REG_READ(SN_SCU->AHBCLKG) & SCU_AHBCLKG_DMA1CLKEN) >> SCU_AHBCLKG_DMA1CLKEN_Pos);
}

/**
 * @brief  Enable SDIO Clock
 * @retval None
 */
__STATIC_INLINE void LL_RCC_Enable_SDIOCLK(void)
{
    LL_REG_SBIT(SN_SCU->AHBCLKG, SCU_AHBCLKG_SDIOCLKEN);
}

/**
 * @brief  Disable SDIO Clock
 * @retval None
 */
__STATIC_INLINE void LL_RCC_Disable_SDIOCLK(void)
{
    LL_REG_CBIT(SN_SCU->AHBCLKG, SCU_AHBCLKG_SDIOCLKEN);
}

/**
 * @brief  Return if enabled SDIO Clock
 * @retval 1: Enabled;   0: Disabled
 */
__STATIC_INLINE uint32_t LL_RCC_IsEnabled_SDIOCLK(void)
{
    return ((LL_REG_READ(SN_SCU->AHBCLKG) & SCU_AHBCLKG_SDIOCLKEN) >> SCU_AHBCLKG_SDIOCLKEN_Pos);
}

/**
 * @brief  Enable ETH Clock
 * @retval None
 */
__STATIC_INLINE void LL_RCC_Enable_ETHCLK(void)
{
    LL_REG_SBIT(SN_SCU->AHBCLKG, SCU_AHBCLKG_ETHCLKEN);
}

/**
 * @brief  Disable ETH Clock
 * @retval None
 */
__STATIC_INLINE void LL_RCC_Disable_ETHCLK(void)
{
    LL_REG_CBIT(SN_SCU->AHBCLKG, SCU_AHBCLKG_ETHCLKEN);
}

/**
 * @brief  Return if enabled ETH Clock
 * @retval 1: Enabled;   0: Disabled
 */
__STATIC_INLINE uint32_t LL_RCC_IsEnabled_ETHCLK(void)
{
    return ((LL_REG_READ(SN_SCU->AHBCLKG) & SCU_AHBCLKG_ETHCLKEN) >> SCU_AHBCLKG_ETHCLKEN_Pos);
}

/**
 * @brief  Enable Backup SRAM Clock
 * @retval None
 */
__STATIC_INLINE void LL_RCC_Enable_BKPSRAMCLK(void)
{
    LL_REG_SBIT(SN_SCU->AHBCLKG, SCU_AHBCLKG_BKPSRAMEN);
}

/**
 * @brief  Disable Backup SRAM Clock
 * @retval None
 */
__STATIC_INLINE void LL_RCC_Disable_BKPSRAMCLK(void)
{
    LL_REG_CBIT(SN_SCU->AHBCLKG, SCU_AHBCLKG_BKPSRAMEN);
}

/**
 * @brief  Return if enabled Backup SRAM Clock
 * @retval 1: Enabled;   0: Disabled
 */
__STATIC_INLINE uint32_t LL_RCC_IsEnabled_BKPSRAMCLK(void)
{
    return ((LL_REG_READ(SN_SCU->AHBCLKG) & SCU_AHBCLKG_BKPSRAMEN) >> SCU_AHBCLKG_BKPSRAMEN_Pos);
}

/**
 * @brief  Enable Ethernet MAC SRAM Clock
 * @retval None
 */
__STATIC_INLINE void LL_RCC_Enable_ETHSRAMCLK(void)
{
    LL_REG_SBIT(SN_SCU->AHBCLKG, SCU_AHBCLKG_ETHSRAMEN);
}

/**
 * @brief  Disable Ethernet MAC SRAM Clock
 * @retval None
 */
__STATIC_INLINE void LL_RCC_Disable_ETHSRAMCLK(void)
{
    LL_REG_CBIT(SN_SCU->AHBCLKG, SCU_AHBCLKG_ETHSRAMEN);
}

/**
 * @brief  Return if enabled Ethernet MAC SRAM Clock
 * @retval 1: Enabled;   0: Disabled
 */
__STATIC_INLINE uint32_t LL_RCC_IsEnabled_ETHSRAMCLK(void)
{
    return ((LL_REG_READ(SN_SCU->AHBCLKG) & SCU_AHBCLKG_ETHSRAMEN) >> SCU_AHBCLKG_ETHSRAMEN_Pos);
}

/**
 * @brief  Enable DMA0 Clock in sleep mode
 * @retval None
 */
__STATIC_INLINE void LL_RCC_Enable_DMA0CLK_Sleep(void)
{
    LL_REG_SBIT(SN_SCU->SLP_AHBCLKG, SCU_SLP_AHBCLKG_DMA0CLKEN);
}

/**
 * @brief  Disable DMA0 Clock in sleep mode
 * @retval None
 */
__STATIC_INLINE void LL_RCC_Disable_DMA0CLK_Sleep(void)
{
    LL_REG_CBIT(SN_SCU->SLP_AHBCLKG, SCU_SLP_AHBCLKG_DMA0CLKEN);
}

/**
 * @brief  Return if enabled DMA0 Clock in sleep mode
 * @retval 1: Enabled;   0: Disabled
 */
__STATIC_INLINE uint32_t LL_RCC_IsEnabled_DMA0CLK_Sleep(void)
{
    return ((LL_REG_READ(SN_SCU->SLP_AHBCLKG) & SCU_SLP_AHBCLKG_DMA0CLKEN) >> SCU_SLP_AHBCLKG_DMA0CLKEN_Pos);
}

/**
 * @brief  Enable DMA1 Clock in sleep mode
 * @retval None
 */
__STATIC_INLINE void LL_RCC_Enable_DMA1CLK_Sleep(void)
{
    LL_REG_SBIT(SN_SCU->SLP_AHBCLKG, SCU_SLP_AHBCLKG_DMA1CLKEN);
}

/**
 * @brief  Disable DMA1 Clock in sleep mode
 * @retval None
 */
__STATIC_INLINE void LL_RCC_Disable_DMA1CLK_Sleep(void)
{
    LL_REG_CBIT(SN_SCU->SLP_AHBCLKG, SCU_SLP_AHBCLKG_DMA1CLKEN);
}

/**
 * @brief  Return if enabled DMA1 Clock in sleep mode
 * @retval 1: Enabled;   0: Disabled
 */
__STATIC_INLINE uint32_t LL_RCC_IsEnabled_DMA1CLK_Sleep(void)
{
    return ((LL_REG_READ(SN_SCU->SLP_AHBCLKG) & SCU_SLP_AHBCLKG_DMA1CLKEN) >> SCU_SLP_AHBCLKG_DMA1CLKEN_Pos);
}

/**
 * @brief  Enable SRAM1 Clock in sleep mode
 * @retval None
 */
__STATIC_INLINE void LL_RCC_Enable_SRAM1CLK_Sleep(void)
{
    LL_REG_SBIT(SN_SCU->SLP_AHBCLKG, SCU_SLP_AHBCLKG_SRAM1EN);
}

/**
 * @brief  Disable SRAM1 Clock in sleep mode
 * @retval None
 */
__STATIC_INLINE void LL_RCC_Disable_SRAM1CLK_Sleep(void)
{
    LL_REG_CBIT(SN_SCU->SLP_AHBCLKG, SCU_SLP_AHBCLKG_SRAM1EN);
}

/**
 * @brief  Return if enabled SRAM1 Clock in sleep mode
 * @retval 1: Enabled;   0: Disabled
 */
__STATIC_INLINE uint32_t LL_RCC_IsEnabled_SRAM1CLK_Sleep(void)
{
    return ((LL_REG_READ(SN_SCU->SLP_AHBCLKG) & SCU_SLP_AHBCLKG_SRAM1EN) >> SCU_SLP_AHBCLKG_SRAM1EN_Pos);
}

/**
 * @brief  Enable SRAM2 Clock in sleep mode
 * @retval None
 */
__STATIC_INLINE void LL_RCC_Enable_SRAM2CLK_Sleep(void)
{
    LL_REG_SBIT(SN_SCU->SLP_AHBCLKG, SCU_SLP_AHBCLKG_SRAM2EN);
}

/**
 * @brief  Disable SRAM2 Clock in sleep mode
 * @retval None
 */
__STATIC_INLINE void LL_RCC_Disable_SRAM2CLK_Sleep(void)
{
    LL_REG_CBIT(SN_SCU->SLP_AHBCLKG, SCU_SLP_AHBCLKG_SRAM2EN);
}

/**
 * @brief  Return if enabled SRAM2 Clock in sleep mode
 * @retval 1: Enabled;   0: Disabled
 */
__STATIC_INLINE uint32_t LL_RCC_IsEnabled_SRAM2CLK_Sleep(void)
{
    return ((LL_REG_READ(SN_SCU->SLP_AHBCLKG) & SCU_SLP_AHBCLKG_SRAM2EN) >> SCU_SLP_AHBCLKG_SRAM2EN_Pos);
}

/**
 * @brief  Enable SRAM3 Clock in sleep mode
 * @retval None
 */
__STATIC_INLINE void LL_RCC_Enable_SRAM3CLK_Sleep(void)
{
    LL_REG_SBIT(SN_SCU->SLP_AHBCLKG, SCU_SLP_AHBCLKG_SRAM3EN);
}

/**
 * @brief  Disable SRAM3 Clock in sleep mode
 * @retval None
 */
__STATIC_INLINE void LL_RCC_Disable_SRAM3CLK_Sleep(void)
{
    LL_REG_CBIT(SN_SCU->SLP_AHBCLKG, SCU_SLP_AHBCLKG_SRAM3EN);
}

/**
 * @brief  Return if enabled SRAM3 Clock in sleep mode
 * @retval 1: Enabled;   0: Disabled
 */
__STATIC_INLINE uint32_t LL_RCC_IsEnabled_SRAM3CLK_Sleep(void)
{
    return ((LL_REG_READ(SN_SCU->SLP_AHBCLKG) & SCU_SLP_AHBCLKG_SRAM3EN) >> SCU_SLP_AHBCLKG_SRAM3EN_Pos);
}

/**
 * @brief  Enable FLASH Clock in sleep mode
 * @retval None
 */
__STATIC_INLINE void LL_RCC_Enable_FLASHCLK_Sleep(void)
{
    LL_REG_SBIT(SN_SCU->SLP_AHBCLKG, SCU_SLP_AHBCLKG_FLASHEN);
}

/**
 * @brief  Disable FLASH Clock in sleep mode
 * @retval None
 */
__STATIC_INLINE void LL_RCC_Disable_FLASHCLK_Sleep(void)
{
    LL_REG_CBIT(SN_SCU->SLP_AHBCLKG, SCU_SLP_AHBCLKG_FLASHEN);
}

/**
 * @brief  Return if enabled FLASH Clock in sleep mode
 * @retval 1: Enabled;   0: Disabled
 */
__STATIC_INLINE uint32_t LL_RCC_IsEnabled_FLASHCLK_Sleep(void)
{
    return ((LL_REG_READ(SN_SCU->SLP_AHBCLKG) & SCU_SLP_AHBCLKG_FLASHEN) >> SCU_SLP_AHBCLKG_FLASHEN_Pos);
}

/**
 * @brief  Enable SDIO Clock in sleep mode
 * @retval None
 */
__STATIC_INLINE void LL_RCC_Enable_SDIOCLK_Sleep(void)
{
    LL_REG_SBIT(SN_SCU->SLP_AHBCLKG, SCU_SLP_AHBCLKG_SDIOCLKEN);
}

/**
 * @brief  Disable SDIO Clock in sleep mode
 * @retval None
 */
__STATIC_INLINE void LL_RCC_Disable_SDIOCLK_Sleep(void)
{
    LL_REG_CBIT(SN_SCU->SLP_AHBCLKG, SCU_SLP_AHBCLKG_SDIOCLKEN);
}

/**
 * @brief  Return if enabled SDIO Clock in sleep mode
 * @retval 1: Enabled;   0: Disabled
 */
__STATIC_INLINE uint32_t LL_RCC_IsEnabled_SDIOCLK_Sleep(void)
{
    return ((LL_REG_READ(SN_SCU->SLP_AHBCLKG) & SCU_SLP_AHBCLKG_SDIOCLKEN) >> SCU_SLP_AHBCLKG_SDIOCLKEN_Pos);
}

/**
 * @brief  Enable ETH Clock in sleep mode
 * @retval None
 */
__STATIC_INLINE void LL_RCC_Enable_ETHCLK_Sleep(void)
{
    LL_REG_SBIT(SN_SCU->SLP_AHBCLKG, SCU_SLP_AHBCLKG_ETHCLKEN);
}

/**
 * @brief  Disable ETH Clock in sleep mode
 * @retval None
 */
__STATIC_INLINE void LL_RCC_Disable_ETHCLK_Sleep(void)
{
    LL_REG_CBIT(SN_SCU->SLP_AHBCLKG, SCU_SLP_AHBCLKG_ETHCLKEN);
}

/**
 * @brief  Return if enabled ETH Clock in sleep mode
 * @retval 1: Enabled;   0: Disabled
 */
__STATIC_INLINE uint32_t LL_RCC_IsEnabled_ETHCLK_Sleep(void)
{
    return ((LL_REG_READ(SN_SCU->SLP_AHBCLKG) & SCU_SLP_AHBCLKG_ETHCLKEN) >> SCU_SLP_AHBCLKG_ETHCLKEN_Pos);
}

/**
 * @brief  Enable Backup SRAM Clock in sleep mode
 * @retval None
 */
__STATIC_INLINE void LL_RCC_Enable_BKPSRAMCLK_Sleep(void)
{
    LL_REG_SBIT(SN_SCU->SLP_AHBCLKG, SCU_SLP_AHBCLKG_BKPSRAMEN);
}

/**
 * @brief  Disable Backup SRAM Clock in sleep mode
 * @retval None
 */
__STATIC_INLINE void LL_RCC_Disable_BKPSRAMCLK_Sleep(void)
{
    LL_REG_CBIT(SN_SCU->SLP_AHBCLKG, SCU_SLP_AHBCLKG_BKPSRAMEN);
}

/**
 * @brief  Return if enabled Backup SRAM Clock in sleep mode
 * @retval 1: Enabled;   0: Disabled
 */
__STATIC_INLINE uint32_t LL_RCC_IsEnabled_BKPSRAMCLK_Sleep(void)
{
    return ((LL_REG_READ(SN_SCU->SLP_AHBCLKG) & SCU_SLP_AHBCLKG_BKPSRAMEN) >> SCU_SLP_AHBCLKG_BKPSRAMEN_Pos);
}

/**
 * @brief  Enable AHB bus Matrix clock in sleep mode
 * @retval None
 */
__STATIC_INLINE void LL_RCC_Enable_AHBMCLK_Sleep(void)
{
    LL_REG_SBIT(SN_SCU->SLP_AHBCLKG, SCU_SLP_AHBCLKG_AHBMCLKEN);
}

/**
 * @brief  Disable AHB bus Matrix clock in sleep mode
 * @retval None
 */
__STATIC_INLINE void LL_RCC_Disable_AHBMCLK_Sleep(void)
{
    LL_REG_CBIT(SN_SCU->SLP_AHBCLKG, SCU_SLP_AHBCLKG_AHBMCLKEN);
}

/**
 * @brief  Return if enabled AHB bus Matrix clock in sleep mode
 * @retval 1: Enabled;   0: Disabled
 */
__STATIC_INLINE uint32_t LL_RCC_IsEnabled_AHBMCLK_Sleep(void)
{
    return ((LL_REG_READ(SN_SCU->SLP_AHBCLKG) & SCU_SLP_AHBCLKG_AHBMCLKEN) >> SCU_SLP_AHBCLKG_AHBMCLKEN_Pos);
}

/**
 * @brief  Enable AHB to APB0 bridge clock in sleep mode
 * @retval None
 */
__STATIC_INLINE void LL_RCC_Enable_AHB2APB0CLK_Sleep(void)
{
    LL_REG_SBIT(SN_SCU->SLP_AHBCLKG, SCU_SLP_AHBCLKG_H2P0HCLKEN);
}

/**
 * @brief  Disable AHB to APB0 bridge clock in sleep mode
 * @retval None
 */
__STATIC_INLINE void LL_RCC_Disable_AHB2APB0CLK_Sleep(void)
{
    LL_REG_CBIT(SN_SCU->SLP_AHBCLKG, SCU_SLP_AHBCLKG_H2P0HCLKEN);
}

/**
 * @brief  Return if enabled AHB to APB0 bridge clock in sleep mode
 * @retval 1: Enabled;   0: Disabled
 */
__STATIC_INLINE uint32_t LL_RCC_IsEnabled_AHB2APB0CLK_Sleep(void)
{
    return ((LL_REG_READ(SN_SCU->SLP_AHBCLKG) & SCU_SLP_AHBCLKG_H2P0HCLKEN) >> SCU_SLP_AHBCLKG_H2P0HCLKEN_Pos);
}

/**
 * @brief  Enable AHB to APB1 bridge clock in sleep mode
 * @retval None
 */
__STATIC_INLINE void LL_RCC_Enable_AHB2APB1CLK_Sleep(void)
{
    LL_REG_SBIT(SN_SCU->SLP_AHBCLKG, SCU_SLP_AHBCLKG_H2P1HCLKEN);
}

/**
 * @brief  Disable AHB to APB1 bridge clock in sleep mode
 * @retval None
 */
__STATIC_INLINE void LL_RCC_Disable_AHB2APB1CLK_Sleep(void)
{
    LL_REG_CBIT(SN_SCU->SLP_AHBCLKG, SCU_SLP_AHBCLKG_H2P1HCLKEN);
}

/**
 * @brief  Return if enabled AHB to APB1 bridge clock in sleep mode
 * @retval 1: Enabled;   0: Disabled
 */
__STATIC_INLINE uint32_t LL_RCC_IsEnabled_AHB2APB1CLK_Sleep(void)
{
    return ((LL_REG_READ(SN_SCU->SLP_AHBCLKG) & SCU_SLP_AHBCLKG_H2P1HCLKEN) >> SCU_SLP_AHBCLKG_H2P1HCLKEN_Pos);
}

/**
 * @brief  Enable Ethernet MAC SRAM clock in sleep mode
 * @retval None
 */
__STATIC_INLINE void LL_RCC_Enable_ETHSRAMCLK_Sleep(void)
{
    LL_REG_SBIT(SN_SCU->SLP_AHBCLKG, SCU_SLP_AHBCLKG_ETHSRAMEN);
}

/**
 * @brief  Disable Ethernet MAC SRAM clock in sleep mode
 * @retval None
 */
__STATIC_INLINE void LL_RCC_Disable_ETHSRAMCLK_Sleep(void)
{
    LL_REG_CBIT(SN_SCU->SLP_AHBCLKG, SCU_SLP_AHBCLKG_ETHSRAMEN);
}

/**
 * @brief  Return if enabled Ethernet MAC SRAM clock in sleep mode
 * @retval 1: Enabled;   0: Disabled
 */
__STATIC_INLINE uint32_t LL_RCC_IsEnabled_ETHSRAMCLK_Sleep(void)
{
    return ((LL_REG_READ(SN_SCU->SLP_AHBCLKG) & SCU_SLP_AHBCLKG_ETHSRAMEN) >> SCU_SLP_AHBCLKG_ETHSRAMEN_Pos);
}

/**
 * @brief  Enable WDT clock
 * @retval None
 */
__STATIC_INLINE void LL_RCC_Enable_WDTCLK(void)
{
    LL_REG_SBIT(SN_SCU->APB0CLKG, SCU_APB0CLKG_WDTCLKEN);
}

/**
 * @brief  Disable WDT clock
 * @retval None
 */
__STATIC_INLINE void LL_RCC_Disable_WDTCLK(void)
{
    LL_REG_CBIT(SN_SCU->APB0CLKG, SCU_APB0CLKG_WDTCLKEN);
}

/**
 * @brief  Return if enabled WDT clock
 * @retval 1: Enabled;   0: Disabled
 */
__STATIC_INLINE uint32_t LL_RCC_IsEnabled_WDTCLK(void)
{
    return ((LL_REG_READ(SN_SCU->APB0CLKG) & SCU_APB0CLKG_WDTCLKEN) >> SCU_APB0CLKG_WDTCLKEN_Pos);
}

/**
 * @brief  Enable WWDT clock
 * @retval None
 */
__STATIC_INLINE void LL_RCC_Enable_WWDTCLK(void)
{
    LL_REG_SBIT(SN_SCU->APB0CLKG, SCU_APB0CLKG_WWDTCLKEN);
}

/**
 * @brief  Disable WWDT clock
 * @retval None
 */
__STATIC_INLINE void LL_RCC_Disable_WWDTCLK(void)
{
    LL_REG_CBIT(SN_SCU->APB0CLKG, SCU_APB0CLKG_WWDTCLKEN);
}

/**
 * @brief  Return if enabled WWDT clock
 * @retval 1: Enabled;   0: Disabled
 */
__STATIC_INLINE uint32_t LL_RCC_IsEnabled_WWDTCLK(void)
{
    return ((LL_REG_READ(SN_SCU->APB0CLKG) & SCU_APB0CLKG_WWDTCLKEN) >> SCU_APB0CLKG_WWDTCLKEN_Pos);
}

/**
 * @brief  Enable CAN0 clock
 * @retval None
 */
__STATIC_INLINE void LL_RCC_Enable_CAN0CLK(void)
{
    LL_REG_SBIT(SN_SCU->APB0CLKG, SCU_APB0CLKG_CAN0CLKEN);
}

/**
 * @brief  Disable CAN0 clock
 * @retval None
 */
__STATIC_INLINE void LL_RCC_Disable_CAN0CLK(void)
{
    LL_REG_CBIT(SN_SCU->APB0CLKG, SCU_APB0CLKG_CAN0CLKEN);
}

/**
 * @brief  Return if enabled CAN0 clock
 * @retval 1: Enabled;   0: Disabled
 */
__STATIC_INLINE uint32_t LL_RCC_IsEnabled_CAN0CLK(void)
{
    return ((LL_REG_READ(SN_SCU->APB0CLKG) & SCU_APB0CLKG_CAN0CLKEN) >> SCU_APB0CLKG_CAN0CLKEN_Pos);
}

/**
 * @brief  Enable CAN1 clock
 * @retval None
 */
__STATIC_INLINE void LL_RCC_Enable_CAN1CLK(void)
{
    LL_REG_SBIT(SN_SCU->APB0CLKG, SCU_APB0CLKG_CAN1CLKEN);
}

/**
 * @brief  Disable CAN1 clock
 * @retval None
 */
__STATIC_INLINE void LL_RCC_Disable_CAN1CLK(void)
{
    LL_REG_CBIT(SN_SCU->APB0CLKG, SCU_APB0CLKG_CAN1CLKEN);
}

/**
 * @brief  Return if enabled CAN1 clock
 * @retval 1: Enabled;   0: Disabled
 */
__STATIC_INLINE uint32_t LL_RCC_IsEnabled_CAN1CLK(void)
{
    return ((LL_REG_READ(SN_SCU->APB0CLKG) & SCU_APB0CLKG_CAN1CLKEN) >> SCU_APB0CLKG_CAN1CLKEN_Pos);
}

/**
 * @brief  Enable UART1 clock
 * @retval None
 */
__STATIC_INLINE void LL_RCC_Enable_UART1CLK(void)
{
    LL_REG_SBIT(SN_SCU->APB0CLKG, SCU_APB0CLKG_UART1CLKEN);
}

/**
 * @brief  Disable UART1 clock
 * @retval None
 */
__STATIC_INLINE void LL_RCC_Disable_UART1CLK(void)
{
    LL_REG_CBIT(SN_SCU->APB0CLKG, SCU_APB0CLKG_UART1CLKEN);
}

/**
 * @brief  Return if enabled UART1 clock
 * @retval 1: Enabled;   0: Disabled
 */
__STATIC_INLINE uint32_t LL_RCC_IsEnabled_UART1CLK(void)
{
    return ((LL_REG_READ(SN_SCU->APB0CLKG) & SCU_APB0CLKG_UART1CLKEN) >> SCU_APB0CLKG_UART1CLKEN_Pos);
}

/**
 * @brief  Enable UART2 clock
 * @retval None
 */
__STATIC_INLINE void LL_RCC_Enable_UART2CLK(void)
{
    LL_REG_SBIT(SN_SCU->APB0CLKG, SCU_APB0CLKG_UART2CLKEN);
}

/**
 * @brief  Disable UART2 clock
 * @retval None
 */
__STATIC_INLINE void LL_RCC_Disable_UART2CLK(void)
{
    LL_REG_CBIT(SN_SCU->APB0CLKG, SCU_APB0CLKG_UART2CLKEN);
}

/**
 * @brief  Return if enabled UART2 clock
 * @retval 1: Enabled;   0: Disabled
 */
__STATIC_INLINE uint32_t LL_RCC_IsEnabled_UART2CLK(void)
{
    return ((LL_REG_READ(SN_SCU->APB0CLKG) & SCU_APB0CLKG_UART2CLKEN) >> SCU_APB0CLKG_UART2CLKEN_Pos);
}

/**
 * @brief  Enable UART3 clock
 * @retval None
 */
__STATIC_INLINE void LL_RCC_Enable_UART3CLK(void)
{
    LL_REG_SBIT(SN_SCU->APB0CLKG, SCU_APB0CLKG_UART3CLKEN);
}

/**
 * @brief  Disable UART3 clock
 * @retval None
 */
__STATIC_INLINE void LL_RCC_Disable_UART3CLK(void)
{
    LL_REG_CBIT(SN_SCU->APB0CLKG, SCU_APB0CLKG_UART3CLKEN);
}

/**
 * @brief  Return if enabled UART3 clock
 * @retval 1: Enabled;   0: Disabled
 */
__STATIC_INLINE uint32_t LL_RCC_IsEnabled_UART3CLK(void)
{
    return ((LL_REG_READ(SN_SCU->APB0CLKG) & SCU_APB0CLKG_UART3CLKEN) >> SCU_APB0CLKG_UART3CLKEN_Pos);
}

/**
 * @brief  Enable UART4 clock
 * @retval None
 */
__STATIC_INLINE void LL_RCC_Enable_UART4CLK(void)
{
    LL_REG_SBIT(SN_SCU->APB0CLKG, SCU_APB0CLKG_UART4CLKEN);
}

/**
 * @brief  Disable UART4 clock
 * @retval None
 */
__STATIC_INLINE void LL_RCC_Disable_UART4CLK(void)
{
    LL_REG_CBIT(SN_SCU->APB0CLKG, SCU_APB0CLKG_UART4CLKEN);
}

/**
 * @brief  Return if enabled UART4 clock
 * @retval 1: Enabled;   0: Disabled
 */
__STATIC_INLINE uint32_t LL_RCC_IsEnabled_UART4CLK(void)
{
    return ((LL_REG_READ(SN_SCU->APB0CLKG) & SCU_APB0CLKG_UART4CLKEN) >> SCU_APB0CLKG_UART4CLKEN_Pos);
}

/**
 * @brief  Enable CT16B6 clock
 * @retval None
 */
__STATIC_INLINE void LL_RCC_Enable_CT16B6CLK(void)
{
    LL_REG_SBIT(SN_SCU->APB0CLKG, SCU_APB0CLKG_CT16B6CLKEN);
}

/**
 * @brief  Disable CT16B6 clock
 * @retval None
 */
__STATIC_INLINE void LL_RCC_Disable_CT16B6CLK(void)
{
    LL_REG_CBIT(SN_SCU->APB0CLKG, SCU_APB0CLKG_CT16B6CLKEN);
}

/**
 * @brief  Return if enabled CT16B6 clock
 * @retval 1: Enabled;   0: Disabled
 */
__STATIC_INLINE uint32_t LL_RCC_IsEnabled_CT16B6CLK(void)
{
    return ((LL_REG_READ(SN_SCU->APB0CLKG) & SCU_APB0CLKG_CT16B6CLKEN) >> SCU_APB0CLKG_CT16B6CLKEN_Pos);
}

/**
 * @brief  Enable CT16B7 clock
 * @retval None
 */
__STATIC_INLINE void LL_RCC_Enable_CT16B7CLK(void)
{
    LL_REG_SBIT(SN_SCU->APB0CLKG, SCU_APB0CLKG_CT16B7CLKEN);
}

/**
 * @brief  Disable CT16B7 clock
 * @retval None
 */
__STATIC_INLINE void LL_RCC_Disable_CT16B7CLK(void)
{
    LL_REG_CBIT(SN_SCU->APB0CLKG, SCU_APB0CLKG_CT16B7CLKEN);
}

/**
 * @brief  Return if enabled CT16B7 clock
 * @retval 1: Enabled;   0: Disabled
 */
__STATIC_INLINE uint32_t LL_RCC_IsEnabled_CT16B7CLK(void)
{
    return ((LL_REG_READ(SN_SCU->APB0CLKG) & SCU_APB0CLKG_CT16B7CLKEN) >> SCU_APB0CLKG_CT16B7CLKEN_Pos);
}

/**
 * @brief  Enable Always-on domain registers clock
 * @retval None
 */
__STATIC_INLINE void LL_RCC_Enable_AlwaysOnRegCLK(void)
{
    LL_REG_SBIT(SN_SCU->APB0CLKG, SCU_APB0CLKG_ALWAYSONREGEN);
}

/**
 * @brief  Disable Always-on domain registers clock
 * @retval None
 */
__STATIC_INLINE void LL_RCC_Disable_AlwaysOnRegCLK(void)
{
    LL_REG_CBIT(SN_SCU->APB0CLKG, SCU_APB0CLKG_ALWAYSONREGEN);
}

/**
 * @brief  Return if enabled Always-on domain registers clock
 * @retval 1: Enabled;   0: Disabled
 */
__STATIC_INLINE uint32_t LL_RCC_IsEnabled_AlwaysOnRegCLK(void)
{
    return ((LL_REG_READ(SN_SCU->APB0CLKG) & SCU_APB0CLKG_ALWAYSONREGEN) >> SCU_APB0CLKG_ALWAYSONREGEN_Pos);
}

/**
 * @brief  Enable CAN0 SRAM clock
 * @retval None
 */
__STATIC_INLINE void LL_RCC_Enable_CAN0SRAMCLK(void)
{
    LL_REG_SBIT(SN_SCU->APB0CLKG, SCU_APB0CLKG_CAN0SRAMCLKEN);
}

/**
 * @brief  Disable CAN0 SRAM clock
 * @retval None
 */
__STATIC_INLINE void LL_RCC_Disable_CAN0SRAMCLK(void)
{
    LL_REG_CBIT(SN_SCU->APB0CLKG, SCU_APB0CLKG_CAN0SRAMCLKEN);
}

/**
 * @brief  Return if enabled CAN0 SRAM clock
 * @retval 1: Enabled;   0: Disabled
 */
__STATIC_INLINE uint32_t LL_RCC_IsEnabled_CAN0SRAMCLK(void)
{
    return ((LL_REG_READ(SN_SCU->APB0CLKG) & SCU_APB0CLKG_CAN0SRAMCLKEN) >> SCU_APB0CLKG_CAN0SRAMCLKEN_Pos);
}

/**
 * @brief  Enable CAN1 SRAM clock
 * @retval None
 */
__STATIC_INLINE void LL_RCC_Enable_CAN1SRAMCLK(void)
{
    LL_REG_SBIT(SN_SCU->APB0CLKG, SCU_APB0CLKG_CAN1SRAMCLKEN);
}

/**
 * @brief  Disable CAN1 SRAM clock
 * @retval None
 */
__STATIC_INLINE void LL_RCC_Disable_CAN1SRAMCLK(void)
{
    LL_REG_CBIT(SN_SCU->APB0CLKG, SCU_APB0CLKG_CAN1SRAMCLKEN);
}

/**
 * @brief  Return if enabled CAN1 SRAM clock
 * @retval 1: Enabled;   0: Disabled
 */
__STATIC_INLINE uint32_t LL_RCC_IsEnabled_CAN1SRAMCLK(void)
{
    return ((LL_REG_READ(SN_SCU->APB0CLKG) & SCU_APB0CLKG_CAN1SRAMCLKEN) >> SCU_APB0CLKG_CAN1SRAMCLKEN_Pos);
}

/**
 * @brief  Enable CLKOUT clock
 * @retval None
 */
__STATIC_INLINE void LL_RCC_Enable_CLKOUTCLK(void)
{
    LL_REG_SBIT(SN_SCU->APB0CLKG, SCU_APB0CLKG_CLKOUTEN);
}

/**
 * @brief  Disable CLKOUT clock
 * @retval None
 */
__STATIC_INLINE void LL_RCC_Disable_CLKOUTCLK(void)
{
    LL_REG_CBIT(SN_SCU->APB0CLKG, SCU_APB0CLKG_CLKOUTEN);
}

/**
 * @brief  Return if enabled CLKOUT clock
 * @retval 1: Enabled;   0: Disabled
 */
__STATIC_INLINE uint32_t LL_RCC_IsEnabled_CLKOUTCLK(void)
{
    return ((LL_REG_READ(SN_SCU->APB0CLKG) & SCU_APB0CLKG_CLKOUTEN) >> SCU_APB0CLKG_CLKOUTEN_Pos);
}

/**
 * @brief  Enable ADC0 clock
 * @retval None
 */
__STATIC_INLINE void LL_RCC_Enable_ADC0CLK(void)
{
    LL_REG_SBIT(SN_SCU->APB1CLKG, SCU_APB1CLKG_ADC0CLKEN);
}

/**
 * @brief  Disable ADC0 clock
 * @retval None
 */
__STATIC_INLINE void LL_RCC_Disable_ADC0CLK(void)
{
    LL_REG_CBIT(SN_SCU->APB1CLKG, SCU_APB1CLKG_ADC0CLKEN);
}

/**
 * @brief  Return if enabled ADC0 clock
 * @retval 1: Enabled;   0: Disabled
 */
__STATIC_INLINE uint32_t LL_RCC_IsEnabled_ADC0CLK(void)
{
    return ((LL_REG_READ(SN_SCU->APB1CLKG) & SCU_APB1CLKG_ADC0CLKEN) >> SCU_APB1CLKG_ADC0CLKEN_Pos);
}

/**
 * @brief  Enable SSP0 clock
 * @retval None
 */
__STATIC_INLINE void LL_RCC_Enable_SSP0CLK(void)
{
    LL_REG_SBIT(SN_SCU->APB1CLKG, SCU_APB1CLKG_SSP0CLKEN);
}

/**
 * @brief  Disable SSP0 clock
 * @retval None
 */
__STATIC_INLINE void LL_RCC_Disable_SSP0CLK(void)
{
    LL_REG_CBIT(SN_SCU->APB1CLKG, SCU_APB1CLKG_SSP0CLKEN);
}

/**
 * @brief  Return if enabled SSP0 clock
 * @retval 1: Enabled;   0: Disabled
 */
__STATIC_INLINE uint32_t LL_RCC_IsEnabled_SSP0CLK(void)
{
    return ((LL_REG_READ(SN_SCU->APB1CLKG) & SCU_APB1CLKG_SSP0CLKEN) >> SCU_APB1CLKG_SSP0CLKEN_Pos);
}

/**
 * @brief  Enable SSP1 clock
 * @retval None
 */
__STATIC_INLINE void LL_RCC_Enable_SSP1CLK(void)
{
    LL_REG_SBIT(SN_SCU->APB1CLKG, SCU_APB1CLKG_SSP1CLKEN);
}

/**
 * @brief  Disable SSP1 clock
 * @retval None
 */
__STATIC_INLINE void LL_RCC_Disable_SSP1CLK(void)
{
    LL_REG_CBIT(SN_SCU->APB1CLKG, SCU_APB1CLKG_SSP1CLKEN);
}

/**
 * @brief  Return if enabled SSP1 clock
 * @retval 1: Enabled;   0: Disabled
 */
__STATIC_INLINE uint32_t LL_RCC_IsEnabled_SSP1CLK(void)
{
    return ((LL_REG_READ(SN_SCU->APB1CLKG) & SCU_APB1CLKG_SSP1CLKEN) >> SCU_APB1CLKG_SSP1CLKEN_Pos);
}

/**
 * @brief  Enable SSP2 clock
 * @retval None
 */
__STATIC_INLINE void LL_RCC_Enable_SSP2CLK(void)
{
    LL_REG_SBIT(SN_SCU->APB1CLKG, SCU_APB1CLKG_SSP2CLKEN);
}

/**
 * @brief  Disable SSP2 clock
 * @retval None
 */
__STATIC_INLINE void LL_RCC_Disable_SSP2CLK(void)
{
    LL_REG_CBIT(SN_SCU->APB1CLKG, SCU_APB1CLKG_SSP2CLKEN);
}

/**
 * @brief  Return if enabled SSP2 clock
 * @retval 1: Enabled;   0: Disabled
 */
__STATIC_INLINE uint32_t LL_RCC_IsEnabled_SSP2CLK(void)
{
    return ((LL_REG_READ(SN_SCU->APB1CLKG) & SCU_APB1CLKG_SSP2CLKEN) >> SCU_APB1CLKG_SSP2CLKEN_Pos);
}

/**
 * @brief  Enable UART0 clock
 * @retval None
 */
__STATIC_INLINE void LL_RCC_Enable_UART0CLK(void)
{
    LL_REG_SBIT(SN_SCU->APB1CLKG, SCU_APB1CLKG_UART0CLKEN);
}

/**
 * @brief  Disable UART0 clock
 * @retval None
 */
__STATIC_INLINE void LL_RCC_Disable_UART0CLK(void)
{
    LL_REG_CBIT(SN_SCU->APB1CLKG, SCU_APB1CLKG_UART0CLKEN);
}

/**
 * @brief  Return if enabled UART0 clock
 * @retval 1: Enabled;   0: Disabled
 */
__STATIC_INLINE uint32_t LL_RCC_IsEnabled_UART0CLK(void)
{
    return ((LL_REG_READ(SN_SCU->APB1CLKG) & SCU_APB1CLKG_UART0CLKEN) >> SCU_APB1CLKG_UART0CLKEN_Pos);
}

/**
 * @brief  Enable UART5 clock
 * @retval None
 */
__STATIC_INLINE void LL_RCC_Enable_UART5CLK(void)
{
    LL_REG_SBIT(SN_SCU->APB1CLKG, SCU_APB1CLKG_UART5CLKEN);
}

/**
 * @brief  Disable UART5 clock
 * @retval None
 */
__STATIC_INLINE void LL_RCC_Disable_UART5CLK(void)
{
    LL_REG_CBIT(SN_SCU->APB1CLKG, SCU_APB1CLKG_UART5CLKEN);
}

/**
 * @brief  Return if enabled UART5 clock
 * @retval 1: Enabled;   0: Disabled
 */
__STATIC_INLINE uint32_t LL_RCC_IsEnabled_UART5CLK(void)
{
    return ((LL_REG_READ(SN_SCU->APB1CLKG) & SCU_APB1CLKG_UART5CLKEN) >> SCU_APB1CLKG_UART5CLKEN_Pos);
}

/**
 * @brief  Enable I2C0 clock
 * @retval None
 */
__STATIC_INLINE void LL_RCC_Enable_I2C0CLK(void)
{
    LL_REG_SBIT(SN_SCU->APB1CLKG, SCU_APB1CLKG_I2C0CLKEN);
}

/**
 * @brief  Disable I2C0 clock
 * @retval None
 */
__STATIC_INLINE void LL_RCC_Disable_I2C0CLK(void)
{
    LL_REG_CBIT(SN_SCU->APB1CLKG, SCU_APB1CLKG_I2C0CLKEN);
}

/**
 * @brief  Return if enabled I2C0 clock
 * @retval 1: Enabled;   0: Disabled
 */
__STATIC_INLINE uint32_t LL_RCC_IsEnabled_I2C0CLK(void)
{
    return ((LL_REG_READ(SN_SCU->APB1CLKG) & SCU_APB1CLKG_I2C0CLKEN) >> SCU_APB1CLKG_I2C0CLKEN_Pos);
}

/**
 * @brief  Enable I2C1 clock
 * @retval None
 */
__STATIC_INLINE void LL_RCC_Enable_I2C1CLK(void)
{
    LL_REG_SBIT(SN_SCU->APB1CLKG, SCU_APB1CLKG_I2C1CLKEN);
}

/**
 * @brief  Disable I2C1 clock
 * @retval None
 */
__STATIC_INLINE void LL_RCC_Disable_I2C1CLK(void)
{
    LL_REG_CBIT(SN_SCU->APB1CLKG, SCU_APB1CLKG_I2C1CLKEN);
}

/**
 * @brief  Return if enabled I2C1 clock
 * @retval 1: Enabled;   0: Disabled
 */
__STATIC_INLINE uint32_t LL_RCC_IsEnabled_I2C1CLK(void)
{
    return ((LL_REG_READ(SN_SCU->APB1CLKG) & SCU_APB1CLKG_I2C1CLKEN) >> SCU_APB1CLKG_I2C1CLKEN_Pos);
}

/**
 * @brief  Enable I2C2 clock
 * @retval None
 */
__STATIC_INLINE void LL_RCC_Enable_I2C2CLK(void)
{
    LL_REG_SBIT(SN_SCU->APB1CLKG, SCU_APB1CLKG_I2C2CLKEN);
}

/**
 * @brief  Disable I2C2 clock
 * @retval None
 */
__STATIC_INLINE void LL_RCC_Disable_I2C2CLK(void)
{
    LL_REG_CBIT(SN_SCU->APB1CLKG, SCU_APB1CLKG_I2C2CLKEN);
}

/**
 * @brief  Return if enabled I2C2 clock
 * @retval 1: Enabled;   0: Disabled
 */
__STATIC_INLINE uint32_t LL_RCC_IsEnabled_I2C2CLK(void)
{
    return ((LL_REG_READ(SN_SCU->APB1CLKG) & SCU_APB1CLKG_I2C2CLKEN) >> SCU_APB1CLKG_I2C2CLKEN_Pos);
}

/**
 * @brief  Enable CT16B0 clock
 * @retval None
 */
__STATIC_INLINE void LL_RCC_Enable_CT16B0CLK(void)
{
    LL_REG_SBIT(SN_SCU->APB1CLKG, SCU_APB1CLKG_CT16B0CLKEN);
}

/**
 * @brief  Disable CT16B0 clock
 * @retval None
 */
__STATIC_INLINE void LL_RCC_Disable_CT16B0CLK(void)
{
    LL_REG_CBIT(SN_SCU->APB1CLKG, SCU_APB1CLKG_CT16B0CLKEN);
}

/**
 * @brief  Return if enabled CT16B0 clock
 * @retval 1: Enabled;   0: Disabled
 */
__STATIC_INLINE uint32_t LL_RCC_IsEnabled_CT16B0CLK(void)
{
    return ((LL_REG_READ(SN_SCU->APB1CLKG) & SCU_APB1CLKG_CT16B0CLKEN) >> SCU_APB1CLKG_CT16B0CLKEN_Pos);
}

/**
 * @brief  Enable CT16B1 clock
 * @retval None
 */
__STATIC_INLINE void LL_RCC_Enable_CT16B1CLK(void)
{
    LL_REG_SBIT(SN_SCU->APB1CLKG, SCU_APB1CLKG_CT16B1CLKEN);
}

/**
 * @brief  Disable CT16B1 clock
 * @retval None
 */
__STATIC_INLINE void LL_RCC_Disable_CT16B1CLK(void)
{
    LL_REG_CBIT(SN_SCU->APB1CLKG, SCU_APB1CLKG_CT16B1CLKEN);
}

/**
 * @brief  Return if enabled CT16B1 clock
 * @retval 1: Enabled;   0: Disabled
 */
__STATIC_INLINE uint32_t LL_RCC_IsEnabled_CT16B1CLK(void)
{
    return ((LL_REG_READ(SN_SCU->APB1CLKG) & SCU_APB1CLKG_CT16B1CLKEN) >> SCU_APB1CLKG_CT16B1CLKEN_Pos);
}

/**
 * @brief  Enable CT16B2 clock
 * @retval None
 */
__STATIC_INLINE void LL_RCC_Enable_CT16B2CLK(void)
{
    LL_REG_SBIT(SN_SCU->APB1CLKG, SCU_APB1CLKG_CT16B2CLKEN);
}

/**
 * @brief  Disable CT16B2 clock
 * @retval None
 */
__STATIC_INLINE void LL_RCC_Disable_CT16B2CLK(void)
{
    LL_REG_CBIT(SN_SCU->APB1CLKG, SCU_APB1CLKG_CT16B2CLKEN);
}

/**
 * @brief  Return if enabled CT16B2 clock
 * @retval 1: Enabled;   0: Disabled
 */
__STATIC_INLINE uint32_t LL_RCC_IsEnabled_CT16B2CLK(void)
{
    return ((LL_REG_READ(SN_SCU->APB1CLKG) & SCU_APB1CLKG_CT16B2CLKEN) >> SCU_APB1CLKG_CT16B2CLKEN_Pos);
}

/**
 * @brief  Enable CT16B3 clock
 * @retval None
 */
__STATIC_INLINE void LL_RCC_Enable_CT16B3CLK(void)
{
    LL_REG_SBIT(SN_SCU->APB1CLKG, SCU_APB1CLKG_CT16B3CLKEN);
}

/**
 * @brief  Disable CT16B3 clock
 * @retval None
 */
__STATIC_INLINE void LL_RCC_Disable_CT16B3CLK(void)
{
    LL_REG_CBIT(SN_SCU->APB1CLKG, SCU_APB1CLKG_CT16B3CLKEN);
}

/**
 * @brief  Return if enabled CT16B3 clock
 * @retval 1: Enabled;   0: Disabled
 */
__STATIC_INLINE uint32_t LL_RCC_IsEnabled_CT16B3CLK(void)
{
    return ((LL_REG_READ(SN_SCU->APB1CLKG) & SCU_APB1CLKG_CT16B3CLKEN) >> SCU_APB1CLKG_CT16B3CLKEN_Pos);
}

/**
 * @brief  Enable CT16B4 clock
 * @retval None
 */
__STATIC_INLINE void LL_RCC_Enable_CT16B4CLK(void)
{
    LL_REG_SBIT(SN_SCU->APB1CLKG, SCU_APB1CLKG_CT16B4CLKEN);
}

/**
 * @brief  Disable CT16B4 clock
 * @retval None
 */
__STATIC_INLINE void LL_RCC_Disable_CT16B4CLK(void)
{
    LL_REG_CBIT(SN_SCU->APB1CLKG, SCU_APB1CLKG_CT16B4CLKEN);
}

/**
 * @brief  Return if enabled CT16B4 clock
 * @retval 1: Enabled;   0: Disabled
 */
__STATIC_INLINE uint32_t LL_RCC_IsEnabled_CT16B4CLK(void)
{
    return ((LL_REG_READ(SN_SCU->APB1CLKG) & SCU_APB1CLKG_CT16B4CLKEN) >> SCU_APB1CLKG_CT16B4CLKEN_Pos);
}

/**
 * @brief  Enable CT16B5 clock
 * @retval None
 */
__STATIC_INLINE void LL_RCC_Enable_CT16B5CLK(void)
{
    LL_REG_SBIT(SN_SCU->APB1CLKG, SCU_APB1CLKG_CT16B5CLKEN);
}

/**
 * @brief  Disable CT16B5 clock
 * @retval None
 */
__STATIC_INLINE void LL_RCC_Disable_CT16B5CLK(void)
{
    LL_REG_CBIT(SN_SCU->APB1CLKG, SCU_APB1CLKG_CT16B5CLKEN);
}

/**
 * @brief  Return if enabled CT16B5 clock
 * @retval 1: Enabled;   0: Disabled
 */
__STATIC_INLINE uint32_t LL_RCC_IsEnabled_CT16B5CLK(void)
{
    return ((LL_REG_READ(SN_SCU->APB1CLKG) & SCU_APB1CLKG_CT16B5CLKEN) >> SCU_APB1CLKG_CT16B5CLKEN_Pos);
}

/**
 * @brief  Enable CT16B8 clock
 * @retval None
 */
__STATIC_INLINE void LL_RCC_Enable_CT16B8CLK(void)
{
    LL_REG_SBIT(SN_SCU->APB1CLKG, SCU_APB1CLKG_CT16B8CLKEN);
}

/**
 * @brief  Disable CT16B8 clock
 * @retval None
 */
__STATIC_INLINE void LL_RCC_Disable_CT16B8CLK(void)
{
    LL_REG_CBIT(SN_SCU->APB1CLKG, SCU_APB1CLKG_CT16B8CLKEN);
}

/**
 * @brief  Return if enabled CT16B8 clock
 * @retval 1: Enabled;   0: Disabled
 */
__STATIC_INLINE uint32_t LL_RCC_IsEnabled_CT16B8CLK(void)
{
    return ((LL_REG_READ(SN_SCU->APB1CLKG) & SCU_APB1CLKG_CT16B8CLKEN) >> SCU_APB1CLKG_CT16B8CLKEN_Pos);
}

/**
 * @brief  Enable CRC clock
 * @retval None
 */
__STATIC_INLINE void LL_RCC_Enable_CRCCLK(void)
{
    LL_REG_SBIT(SN_SCU->APB1CLKG, SCU_APB1CLKG_CRCCLKEN);
}

/**
 * @brief  Disable CRC clock
 * @retval None
 */
__STATIC_INLINE void LL_RCC_Disable_CRCCLK(void)
{
    LL_REG_CBIT(SN_SCU->APB1CLKG, SCU_APB1CLKG_CRCCLKEN);
}

/**
 * @brief  Return if enabled CRC clock
 * @retval 1: Enabled;   0: Disabled
 */
__STATIC_INLINE uint32_t LL_RCC_IsEnabled_CRCCLK(void)
{
    return ((LL_REG_READ(SN_SCU->APB1CLKG) & SCU_APB1CLKG_CRCCLKEN) >> SCU_APB1CLKG_CRCCLKEN_Pos);
}

/**
 * @brief  Enable GPIO0 clock
 * @retval None
 */
__STATIC_INLINE void LL_RCC_Enable_GPIO0CLK(void)
{
    LL_REG_SBIT(SN_SCU->APB1CLKG, SCU_APB1CLKG_P0CLKEN);
}

/**
 * @brief  Disable GPIO0 clock
 * @retval None
 */
__STATIC_INLINE void LL_RCC_Disable_GPIO0CLK(void)
{
    LL_REG_CBIT(SN_SCU->APB1CLKG, SCU_APB1CLKG_P0CLKEN);
}

/**
 * @brief  Return if enabled GPIO0 clock
 * @retval 1: Enabled;   0: Disabled
 */
__STATIC_INLINE uint32_t LL_RCC_IsEnabled_GPIO0CLK(void)
{
    return ((LL_REG_READ(SN_SCU->APB1CLKG) & SCU_APB1CLKG_P0CLKEN) >> SCU_APB1CLKG_P0CLKEN_Pos);
}

/**
 * @brief  Enable GPIO1 clock
 * @retval None
 */
__STATIC_INLINE void LL_RCC_Enable_GPIO1CLK(void)
{
    LL_REG_SBIT(SN_SCU->APB1CLKG, SCU_APB1CLKG_P1CLKEN);
}

/**
 * @brief  Disable GPIO1 clock
 * @retval None
 */
__STATIC_INLINE void LL_RCC_Disable_GPIO1CLK(void)
{
    LL_REG_CBIT(SN_SCU->APB1CLKG, SCU_APB1CLKG_P1CLKEN);
}

/**
 * @brief  Return if enabled GPIO1 clock
 * @retval 1: Enabled;   0: Disabled
 */
__STATIC_INLINE uint32_t LL_RCC_IsEnabled_GPIO1CLK(void)
{
    return ((LL_REG_READ(SN_SCU->APB1CLKG) & SCU_APB1CLKG_P1CLKEN) >> SCU_APB1CLKG_P1CLKEN_Pos);
}

/**
 * @brief  Enable GPIO2 clock
 * @retval None
 */
__STATIC_INLINE void LL_RCC_Enable_GPIO2CLK(void)
{
    LL_REG_SBIT(SN_SCU->APB1CLKG, SCU_APB1CLKG_P2CLKEN);
}

/**
 * @brief  Disable GPIO2 clock
 * @retval None
 */
__STATIC_INLINE void LL_RCC_Disable_GPIO2CLK(void)
{
    LL_REG_CBIT(SN_SCU->APB1CLKG, SCU_APB1CLKG_P2CLKEN);
}

/**
 * @brief  Return if enabled GPIO2 clock
 * @retval 1: Enabled;   0: Disabled
 */
__STATIC_INLINE uint32_t LL_RCC_IsEnabled_GPIO2CLK(void)
{
    return ((LL_REG_READ(SN_SCU->APB1CLKG) & SCU_APB1CLKG_P2CLKEN) >> SCU_APB1CLKG_P2CLKEN_Pos);
}

/**
 * @brief  Enable GPIO3 clock
 * @retval None
 */
__STATIC_INLINE void LL_RCC_Enable_GPIO3CLK(void)
{
    LL_REG_SBIT(SN_SCU->APB1CLKG, SCU_APB1CLKG_P3CLKEN);
}

/**
 * @brief  Disable GPIO3 clock
 * @retval None
 */
__STATIC_INLINE void LL_RCC_Disable_GPIO3CLK(void)
{
    LL_REG_CBIT(SN_SCU->APB1CLKG, SCU_APB1CLKG_P3CLKEN);
}

/**
 * @brief  Return if enabled GPIO3 clock
 * @retval 1: Enabled;   0: Disabled
 */
__STATIC_INLINE uint32_t LL_RCC_IsEnabled_GPIO3CLK(void)
{
    return ((LL_REG_READ(SN_SCU->APB1CLKG) & SCU_APB1CLKG_P3CLKEN) >> SCU_APB1CLKG_P3CLKEN_Pos);
}

/**
 * @brief  Enable LCM clock
 * @retval None
 */
__STATIC_INLINE void LL_RCC_Enable_LCMCLK(void)
{
    LL_REG_SBIT(SN_SCU->APB1CLKG, SCU_APB1CLKG_LCMCLKEN);
}

/**
 * @brief  Disable LCM clock
 * @retval None
 */
__STATIC_INLINE void LL_RCC_Disable_LCMCLK(void)
{
    LL_REG_CBIT(SN_SCU->APB1CLKG, SCU_APB1CLKG_LCMCLKEN);
}

/**
 * @brief  Return if enabled LCM clock
 * @retval 1: Enabled;   0: Disabled
 */
__STATIC_INLINE uint32_t LL_RCC_IsEnabled_LCMCLK(void)
{
    return ((LL_REG_READ(SN_SCU->APB1CLKG) & SCU_APB1CLKG_LCMCLKEN) >> SCU_APB1CLKG_LCMCLKEN_Pos);
}

/**
 * @brief  Enable WDT clock in sleep mode
 * @retval None
 */
__STATIC_INLINE void LL_RCC_Enable_WDTCLK_Sleep(void)
{
    LL_REG_SBIT(SN_SCU->SLP_APB0CLKG, SCU_SLP_APB0CLKG_WDTCLKEN);
}

/**
 * @brief  Disable WDT clock in sleep mode
 * @retval None
 */
__STATIC_INLINE void LL_RCC_Disable_WDTCLK_Sleep(void)
{
    LL_REG_CBIT(SN_SCU->SLP_APB0CLKG, SCU_SLP_APB0CLKG_WDTCLKEN);
}

/**
 * @brief  Return if enabled WDT clock in sleep mode
 * @retval 1: Enabled;   0: Disabled
 */
__STATIC_INLINE uint32_t LL_RCC_IsEnabled_WDTCLK_Sleep(void)
{
    return ((LL_REG_READ(SN_SCU->SLP_APB0CLKG) & SCU_SLP_APB0CLKG_WDTCLKEN) >> SCU_SLP_APB0CLKG_WDTCLKEN_Pos);
}

/**
 * @brief  Enable WWDT clock in sleep mode
 * @retval None
 */
__STATIC_INLINE void LL_RCC_Enable_WWDTCLK_Sleep(void)
{
    LL_REG_SBIT(SN_SCU->SLP_APB0CLKG, SCU_SLP_APB0CLKG_WWDTCLKEN);
}

/**
 * @brief  Disable WWDT clock in sleep mode
 * @retval None
 */
__STATIC_INLINE void LL_RCC_Disable_WWDTCLK_Sleep(void)
{
    LL_REG_CBIT(SN_SCU->SLP_APB0CLKG, SCU_SLP_APB0CLKG_WWDTCLKEN);
}

/**
 * @brief  Return if enabled WWDT clock in sleep mode
 * @retval 1: Enabled;   0: Disabled
 */
__STATIC_INLINE uint32_t LL_RCC_IsEnabled_WWDTCLK_Sleep(void)
{
    return ((LL_REG_READ(SN_SCU->SLP_APB0CLKG) & SCU_SLP_APB0CLKG_WWDTCLKEN) >> SCU_SLP_APB0CLKG_WWDTCLKEN_Pos);
}

/**
 * @brief  Enable CAN0 clock in sleep mode
 * @retval None
 */
__STATIC_INLINE void LL_RCC_Enable_CAN0CLK_Sleep(void)
{
    LL_REG_SBIT(SN_SCU->SLP_APB0CLKG, SCU_SLP_APB0CLKG_CAN0CLKEN);
}

/**
 * @brief  Disable CAN0 clock in sleep mode
 * @retval None
 */
__STATIC_INLINE void LL_RCC_Disable_CAN0CLK_Sleep(void)
{
    LL_REG_CBIT(SN_SCU->SLP_APB0CLKG, SCU_SLP_APB0CLKG_CAN0CLKEN);
}

/**
 * @brief  Return if enabled CAN0 clock in sleep mode
 * @retval 1: Enabled;   0: Disabled
 */
__STATIC_INLINE uint32_t LL_RCC_IsEnabled_CAN0CLK_Sleep(void)
{
    return ((LL_REG_READ(SN_SCU->SLP_APB0CLKG) & SCU_SLP_APB0CLKG_CAN0CLKEN) >> SCU_SLP_APB0CLKG_CAN0CLKEN_Pos);
}

/**
 * @brief  Enable CAN1 clock in sleep mode
 * @retval None
 */
__STATIC_INLINE void LL_RCC_Enable_CAN1CLK_Sleep(void)
{
    LL_REG_SBIT(SN_SCU->SLP_APB0CLKG, SCU_SLP_APB0CLKG_CAN1CLKEN);
}

/**
 * @brief  Disable CAN1 clock in sleep mode
 * @retval None
 */
__STATIC_INLINE void LL_RCC_Disable_CAN1CLK_Sleep(void)
{
    LL_REG_CBIT(SN_SCU->SLP_APB0CLKG, SCU_SLP_APB0CLKG_CAN1CLKEN);
}

/**
 * @brief  Return if enabled CAN1 clock in sleep mode
 * @retval 1: Enabled;   0: Disabled
 */
__STATIC_INLINE uint32_t LL_RCC_IsEnabled_CAN1CLK_Sleep(void)
{
    return ((LL_REG_READ(SN_SCU->SLP_APB0CLKG) & SCU_SLP_APB0CLKG_CAN1CLKEN) >> SCU_SLP_APB0CLKG_CAN1CLKEN_Pos);
}

/**
 * @brief  Enable UART1 clock in sleep mode
 * @retval None
 */
__STATIC_INLINE void LL_RCC_Enable_UART1CLK_Sleep(void)
{
    LL_REG_SBIT(SN_SCU->SLP_APB0CLKG, SCU_SLP_APB0CLKG_UART1CLKEN);
}

/**
 * @brief  Disable UART1 clock in sleep mode
 * @retval None
 */
__STATIC_INLINE void LL_RCC_Disable_UART1CLK_Sleep(void)
{
    LL_REG_CBIT(SN_SCU->SLP_APB0CLKG, SCU_SLP_APB0CLKG_UART1CLKEN);
}

/**
 * @brief  Return if enabled UART1 clock in sleep mode
 * @retval 1: Enabled;   0: Disabled
 */
__STATIC_INLINE uint32_t LL_RCC_IsEnabled_UART1CLK_Sleep(void)
{
    return ((LL_REG_READ(SN_SCU->SLP_APB0CLKG) & SCU_SLP_APB0CLKG_UART1CLKEN) >> SCU_SLP_APB0CLKG_UART1CLKEN_Pos);
}

/**
 * @brief  Enable UART2 clock in sleep mode
 * @retval None
 */
__STATIC_INLINE void LL_RCC_Enable_UART2CLK_Sleep(void)
{
    LL_REG_SBIT(SN_SCU->SLP_APB0CLKG, SCU_SLP_APB0CLKG_UART2CLKEN);
}

/**
 * @brief  Disable UART2 clock in sleep mode
 * @retval None
 */
__STATIC_INLINE void LL_RCC_Disable_UART2CLK_Sleep(void)
{
    LL_REG_CBIT(SN_SCU->SLP_APB0CLKG, SCU_SLP_APB0CLKG_UART2CLKEN);
}

/**
 * @brief  Return if enabled UART2 clock in sleep mode
 * @retval 1: Enabled;   0: Disabled
 */
__STATIC_INLINE uint32_t LL_RCC_IsEnabled_UART2CLK_Sleep(void)
{
    return ((LL_REG_READ(SN_SCU->SLP_APB0CLKG) & SCU_SLP_APB0CLKG_UART2CLKEN) >> SCU_SLP_APB0CLKG_UART2CLKEN_Pos);
}

/**
 * @brief  Enable UART3 clock in sleep mode
 * @retval None
 */
__STATIC_INLINE void LL_RCC_Enable_UART3CLK_Sleep(void)
{
    LL_REG_SBIT(SN_SCU->SLP_APB0CLKG, SCU_SLP_APB0CLKG_UART3CLKEN);
}

/**
 * @brief  Disable UART3 clock in sleep mode
 * @retval None
 */
__STATIC_INLINE void LL_RCC_Disable_UART3CLK_Sleep(void)
{
    LL_REG_CBIT(SN_SCU->SLP_APB0CLKG, SCU_SLP_APB0CLKG_UART3CLKEN);
}

/**
 * @brief  Return if enabled UART3 clock in sleep mode
 * @retval 1: Enabled;   0: Disabled
 */
__STATIC_INLINE uint32_t LL_RCC_IsEnabled_UART3CLK_Sleep(void)
{
    return ((LL_REG_READ(SN_SCU->SLP_APB0CLKG) & SCU_SLP_APB0CLKG_UART3CLKEN) >> SCU_SLP_APB0CLKG_UART3CLKEN_Pos);
}

/**
 * @brief  Enable UART4 clock in sleep mode
 * @retval None
 */
__STATIC_INLINE void LL_RCC_Enable_UART4CLK_Sleep(void)
{
    LL_REG_SBIT(SN_SCU->SLP_APB0CLKG, SCU_SLP_APB0CLKG_UART4CLKEN);
}

/**
 * @brief  Disable UART4 clock in sleep mode
 * @retval None
 */
__STATIC_INLINE void LL_RCC_Disable_UART4CLK_Sleep(void)
{
    LL_REG_CBIT(SN_SCU->SLP_APB0CLKG, SCU_SLP_APB0CLKG_UART4CLKEN);
}

/**
 * @brief  Return if enabled UART4 clock in sleep mode
 * @retval 1: Enabled;   0: Disabled
 */
__STATIC_INLINE uint32_t LL_RCC_IsEnabled_UART4CLK_Sleep(void)
{
    return ((LL_REG_READ(SN_SCU->SLP_APB0CLKG) & SCU_SLP_APB0CLKG_UART4CLKEN) >> SCU_SLP_APB0CLKG_UART4CLKEN_Pos);
}

/**
 * @brief  Enable CT16B6 clock in sleep mode
 * @retval None
 */
__STATIC_INLINE void LL_RCC_Enable_CT16B6CLK_Sleep(void)
{
    LL_REG_SBIT(SN_SCU->SLP_APB0CLKG, SCU_SLP_APB0CLKG_CT16B6CLKEN);
}

/**
 * @brief  Disable CT16B6 clock in sleep mode
 * @retval None
 */
__STATIC_INLINE void LL_RCC_Disable_CT16B6CLK_Sleep(void)
{
    LL_REG_CBIT(SN_SCU->SLP_APB0CLKG, SCU_SLP_APB0CLKG_CT16B6CLKEN);
}

/**
 * @brief  Return if enabled CT16B6 clock in sleep mode
 * @retval 1: Enabled;   0: Disabled
 */
__STATIC_INLINE uint32_t LL_RCC_IsEnabled_CT16B6CLK_Sleep(void)
{
    return ((LL_REG_READ(SN_SCU->SLP_APB0CLKG) & SCU_SLP_APB0CLKG_CT16B6CLKEN) >> SCU_SLP_APB0CLKG_CT16B6CLKEN_Pos);
}

/**
 * @brief  Enable CT16B7 clock in sleep mode
 * @retval None
 */
__STATIC_INLINE void LL_RCC_Enable_CT16B7CLK_Sleep(void)
{
    LL_REG_SBIT(SN_SCU->SLP_APB0CLKG, SCU_SLP_APB0CLKG_CT16B7CLKEN);
}

/**
 * @brief  Disable CT16B7 clock in sleep mode
 * @retval None
 */
__STATIC_INLINE void LL_RCC_Disable_CT16B7CLK_Sleep(void)
{
    LL_REG_CBIT(SN_SCU->SLP_APB0CLKG, SCU_SLP_APB0CLKG_CT16B7CLKEN);
}

/**
 * @brief  Return if enabled CT16B7 clock in sleep mode
 * @retval 1: Enabled;   0: Disabled
 */
__STATIC_INLINE uint32_t LL_RCC_IsEnabled_CT16B7CLK_Sleep(void)
{
    return ((LL_REG_READ(SN_SCU->SLP_APB0CLKG) & SCU_SLP_APB0CLKG_CT16B7CLKEN) >> SCU_SLP_APB0CLKG_CT16B7CLKEN_Pos);
}

/**
 * @brief  Enable Always-on domain registers clock in sleep mode
 * @retval None
 */
__STATIC_INLINE void LL_RCC_Enable_AlwaysOnRegCLK_Sleep(void)
{
    LL_REG_SBIT(SN_SCU->SLP_APB0CLKG, SCU_SLP_APB0CLKG_ALWAYSONREGEN);
}

/**
 * @brief  Disable Always-on domain registers clock in sleep mode
 * @retval None
 */
__STATIC_INLINE void LL_RCC_Disable_AlwaysOnRegCLK_Sleep(void)
{
    LL_REG_CBIT(SN_SCU->SLP_APB0CLKG, SCU_SLP_APB0CLKG_ALWAYSONREGEN);
}

/**
 * @brief  Return if enabled Always-on domain registers clock in sleep mode
 * @retval 1: Enabled;   0: Disabled
 */
__STATIC_INLINE uint32_t LL_RCC_IsEnabled_AlwaysOnRegCLK_Sleep(void)
{
    return ((LL_REG_READ(SN_SCU->SLP_APB0CLKG) & SCU_SLP_APB0CLKG_ALWAYSONREGEN) >> SCU_SLP_APB0CLKG_ALWAYSONREGEN_Pos);
}

/**
 * @brief  Enable CAN0 SRAM clock in sleep mode
 * @retval None
 */
__STATIC_INLINE void LL_RCC_Enable_CAN0SRAMCLK_Sleep(void)
{
    LL_REG_SBIT(SN_SCU->SLP_APB0CLKG, SCU_SLP_APB0CLKG_CAN0SRAMCLKEN);
}

/**
 * @brief  Disable CAN0 SRAM clock in sleep mode
 * @retval None
 */
__STATIC_INLINE void LL_RCC_Disable_CAN0SRAMCLK_Sleep(void)
{
    LL_REG_CBIT(SN_SCU->SLP_APB0CLKG, SCU_SLP_APB0CLKG_CAN0SRAMCLKEN);
}

/**
 * @brief  Return if enabled CAN0 SRAM clock in sleep mode
 * @retval 1: Enabled;   0: Disabled
 */
__STATIC_INLINE uint32_t LL_RCC_IsEnabled_CAN0SRAMCLK_Sleep(void)
{
    return ((LL_REG_READ(SN_SCU->SLP_APB0CLKG) & SCU_SLP_APB0CLKG_CAN0SRAMCLKEN) >> SCU_SLP_APB0CLKG_CAN0SRAMCLKEN_Pos);
}

/**
 * @brief  Enable CAN1 SRAM clock in sleep mode
 * @retval None
 */
__STATIC_INLINE void LL_RCC_Enable_CAN1SRAMCLK_Sleep(void)
{
    LL_REG_SBIT(SN_SCU->SLP_APB0CLKG, SCU_SLP_APB0CLKG_CAN1SRAMCLKEN);
}

/**
 * @brief  Disable CAN1 SRAM clock in sleep mode
 * @retval None
 */
__STATIC_INLINE void LL_RCC_Disable_CAN1SRAMCLK_Sleep(void)
{
    LL_REG_CBIT(SN_SCU->SLP_APB0CLKG, SCU_SLP_APB0CLKG_CAN1SRAMCLKEN);
}

/**
 * @brief  Return if enabled CAN1 SRAM clock in sleep mode
 * @retval 1: Enabled;   0: Disabled
 */
__STATIC_INLINE uint32_t LL_RCC_IsEnabled_CAN1SRAMCLK_Sleep(void)
{
    return ((LL_REG_READ(SN_SCU->SLP_APB0CLKG) & SCU_SLP_APB0CLKG_CAN1SRAMCLKEN) >> SCU_SLP_APB0CLKG_CAN1SRAMCLKEN_Pos);
}

/**
 * @brief  Enable CLKOUT clock in sleep mode
 * @retval None
 */
__STATIC_INLINE void LL_RCC_Enable_CLKOUTCLK_Sleep(void)
{
    LL_REG_SBIT(SN_SCU->SLP_APB0CLKG, SCU_SLP_APB0CLKG_CLKOUTEN);
}

/**
 * @brief  Disable CLKOUT clock in sleep mode
 * @retval None
 */
__STATIC_INLINE void LL_RCC_Disable_CLKOUTCLK_Sleep(void)
{
    LL_REG_CBIT(SN_SCU->SLP_APB0CLKG, SCU_SLP_APB0CLKG_CLKOUTEN);
}

/**
 * @brief  Return if enabled CLKOUT clock in sleep mode
 * @retval 1: Enabled;   0: Disabled
 */
__STATIC_INLINE uint32_t LL_RCC_IsEnabled_CLKOUTCLK_Sleep(void)
{
    return ((LL_REG_READ(SN_SCU->SLP_APB0CLKG) & SCU_SLP_APB0CLKG_CLKOUTEN) >> SCU_SLP_APB0CLKG_CLKOUTEN_Pos);
}

/**
 * @brief  Enable ADC0 clock in sleep mode
 * @retval None
 */
__STATIC_INLINE void LL_RCC_Enable_ADC0CLK_Sleep(void)
{
    LL_REG_SBIT(SN_SCU->SLP_APB1CLKG, SCU_SLP_APB1CLKG_ADC0CLKEN);
}

/**
 * @brief  Disable ADC0 clock in sleep mode
 * @retval None
 */
__STATIC_INLINE void LL_RCC_Disable_ADC0CLK_Sleep(void)
{
    LL_REG_CBIT(SN_SCU->SLP_APB1CLKG, SCU_SLP_APB1CLKG_ADC0CLKEN);
}

/**
 * @brief  Return if enabled ADC0 clock in sleep mode
 * @retval 1: Enabled;   0: Disabled
 */
__STATIC_INLINE uint32_t LL_RCC_IsEnabled_ADC0CLK_Sleep(void)
{
    return ((LL_REG_READ(SN_SCU->SLP_APB1CLKG) & SCU_SLP_APB1CLKG_ADC0CLKEN) >> SCU_SLP_APB1CLKG_ADC0CLKEN_Pos);
}

/**
 * @brief  Enable SSP0 clock in sleep mode
 * @retval None
 */
__STATIC_INLINE void LL_RCC_Enable_SSP0CLK_Sleep(void)
{
    LL_REG_SBIT(SN_SCU->SLP_APB1CLKG, SCU_SLP_APB1CLKG_SSP0CLKEN);
}

/**
 * @brief  Disable SSP0 clock in sleep mode
 * @retval None
 */
__STATIC_INLINE void LL_RCC_Disable_SSP0CLK_Sleep(void)
{
    LL_REG_CBIT(SN_SCU->SLP_APB1CLKG, SCU_SLP_APB1CLKG_SSP0CLKEN);
}

/**
 * @brief  Return if enabled SSP0 clock in sleep mode
 * @retval 1: Enabled;   0: Disabled
 */
__STATIC_INLINE uint32_t LL_RCC_IsEnabled_SSP0CLK_Sleep(void)
{
    return ((LL_REG_READ(SN_SCU->SLP_APB1CLKG) & SCU_SLP_APB1CLKG_SSP0CLKEN) >> SCU_SLP_APB1CLKG_SSP0CLKEN_Pos);
}

/**
 * @brief  Enable SSP1 clock in sleep mode
 * @retval None
 */
__STATIC_INLINE void LL_RCC_Enable_SSP1CLK_Sleep(void)
{
    LL_REG_SBIT(SN_SCU->SLP_APB1CLKG, SCU_SLP_APB1CLKG_SSP1CLKEN);
}

/**
 * @brief  Disable SSP1 clock in sleep mode
 * @retval None
 */
__STATIC_INLINE void LL_RCC_Disable_SSP1CLK_Sleep(void)
{
    LL_REG_CBIT(SN_SCU->SLP_APB1CLKG, SCU_SLP_APB1CLKG_SSP1CLKEN);
}

/**
 * @brief  Return if enabled SSP1 clock in sleep mode
 * @retval 1: Enabled;   0: Disabled
 */
__STATIC_INLINE uint32_t LL_RCC_IsEnabled_SSP1CLK_Sleep(void)
{
    return ((LL_REG_READ(SN_SCU->SLP_APB1CLKG) & SCU_SLP_APB1CLKG_SSP1CLKEN) >> SCU_SLP_APB1CLKG_SSP1CLKEN_Pos);
}

/**
 * @brief  Enable SSP2 clock in sleep mode
 * @retval None
 */
__STATIC_INLINE void LL_RCC_Enable_SSP2CLK_Sleep(void)
{
    LL_REG_SBIT(SN_SCU->SLP_APB1CLKG, SCU_SLP_APB1CLKG_SSP2CLKEN);
}

/**
 * @brief  Disable SSP2 clock in sleep mode
 * @retval None
 */
__STATIC_INLINE void LL_RCC_Disable_SSP2CLK_Sleep(void)
{
    LL_REG_CBIT(SN_SCU->SLP_APB1CLKG, SCU_SLP_APB1CLKG_SSP2CLKEN);
}

/**
 * @brief  Return if enabled SSP2 clock in sleep mode
 * @retval 1: Enabled;   0: Disabled
 */
__STATIC_INLINE uint32_t LL_RCC_IsEnabled_SSP2CLK_Sleep(void)
{
    return ((LL_REG_READ(SN_SCU->SLP_APB1CLKG) & SCU_SLP_APB1CLKG_SSP2CLKEN) >> SCU_SLP_APB1CLKG_SSP2CLKEN_Pos);
}

/**
 * @brief  Enable UART0 clock in sleep mode
 * @retval None
 */
__STATIC_INLINE void LL_RCC_Enable_UART0CLK_Sleep(void)
{
    LL_REG_SBIT(SN_SCU->SLP_APB1CLKG, SCU_SLP_APB1CLKG_UART0CLKEN);
}

/**
 * @brief  Disable UART0 clock in sleep mode
 * @retval None
 */
__STATIC_INLINE void LL_RCC_Disable_UART0CLK_Sleep(void)
{
    LL_REG_CBIT(SN_SCU->SLP_APB1CLKG, SCU_SLP_APB1CLKG_UART0CLKEN);
}

/**
 * @brief  Return if enabled UART0 clock in sleep mode
 * @retval 1: Enabled;   0: Disabled
 */
__STATIC_INLINE uint32_t LL_RCC_IsEnabled_UART0CLK_Sleep(void)
{
    return ((LL_REG_READ(SN_SCU->SLP_APB1CLKG) & SCU_SLP_APB1CLKG_UART0CLKEN) >> SCU_SLP_APB1CLKG_UART0CLKEN_Pos);
}

/**
 * @brief  Enable UART5 clock in sleep mode
 * @retval None
 */
__STATIC_INLINE void LL_RCC_Enable_UART5CLK_Sleep(void)
{
    LL_REG_SBIT(SN_SCU->SLP_APB1CLKG, SCU_SLP_APB1CLKG_UART5CLKEN);
}

/**
 * @brief  Disable UART5 clock in sleep mode
 * @retval None
 */
__STATIC_INLINE void LL_RCC_Disable_UART5CLK_Sleep(void)
{
    LL_REG_CBIT(SN_SCU->SLP_APB1CLKG, SCU_SLP_APB1CLKG_UART5CLKEN);
}

/**
 * @brief  Return if enabled UART5 clock in sleep mode
 * @retval 1: Enabled;   0: Disabled
 */
__STATIC_INLINE uint32_t LL_RCC_IsEnabled_UART5CLK_Sleep(void)
{
    return ((LL_REG_READ(SN_SCU->SLP_APB1CLKG) & SCU_SLP_APB1CLKG_UART5CLKEN) >> SCU_SLP_APB1CLKG_UART5CLKEN_Pos);
}

/**
 * @brief  Enable I2C0 clock in sleep mode
 * @retval None
 */
__STATIC_INLINE void LL_RCC_Enable_I2C0CLK_Sleep(void)
{
    LL_REG_SBIT(SN_SCU->SLP_APB1CLKG, SCU_SLP_APB1CLKG_I2C0CLKEN);
}

/**
 * @brief  Disable I2C0 clock in sleep mode
 * @retval None
 */
__STATIC_INLINE void LL_RCC_Disable_I2C0CLK_Sleep(void)
{
    LL_REG_CBIT(SN_SCU->SLP_APB1CLKG, SCU_SLP_APB1CLKG_I2C0CLKEN);
}

/**
 * @brief  Return if enabled I2C0 clock in sleep mode
 * @retval 1: Enabled;   0: Disabled
 */
__STATIC_INLINE uint32_t LL_RCC_IsEnabled_I2C0CLK_Sleep(void)
{
    return ((LL_REG_READ(SN_SCU->SLP_APB1CLKG) & SCU_SLP_APB1CLKG_I2C0CLKEN) >> SCU_SLP_APB1CLKG_I2C0CLKEN_Pos);
}

/**
 * @brief  Enable I2C1 clock in sleep mode
 * @retval None
 */
__STATIC_INLINE void LL_RCC_Enable_I2C1CLK_Sleep(void)
{
    LL_REG_SBIT(SN_SCU->SLP_APB1CLKG, SCU_SLP_APB1CLKG_I2C1CLKEN);
}

/**
 * @brief  Disable I2C1 clock in sleep mode
 * @retval None
 */
__STATIC_INLINE void LL_RCC_Disable_I2C1CLK_Sleep(void)
{
    LL_REG_CBIT(SN_SCU->SLP_APB1CLKG, SCU_SLP_APB1CLKG_I2C1CLKEN);
}

/**
 * @brief  Return if enabled I2C1 clock in sleep mode
 * @retval 1: Enabled;   0: Disabled
 */
__STATIC_INLINE uint32_t LL_RCC_IsEnabled_I2C1CLK_Sleep(void)
{
    return ((LL_REG_READ(SN_SCU->SLP_APB1CLKG) & SCU_SLP_APB1CLKG_I2C1CLKEN) >> SCU_SLP_APB1CLKG_I2C1CLKEN_Pos);
}

/**
 * @brief  Enable I2C2 clock in sleep mode
 * @retval None
 */
__STATIC_INLINE void LL_RCC_Enable_I2C2CLK_Sleep(void)
{
    LL_REG_SBIT(SN_SCU->SLP_APB1CLKG, SCU_SLP_APB1CLKG_I2C2CLKEN);
}

/**
 * @brief  Disable I2C2 clock in sleep mode
 * @retval None
 */
__STATIC_INLINE void LL_RCC_Disable_I2C2CLK_Sleep(void)
{
    LL_REG_CBIT(SN_SCU->SLP_APB1CLKG, SCU_SLP_APB1CLKG_I2C2CLKEN);
}

/**
 * @brief  Return if enabled I2C2 clock in sleep mode
 * @retval 1: Enabled;   0: Disabled
 */
__STATIC_INLINE uint32_t LL_RCC_IsEnabled_I2C2CLK_Sleep(void)
{
    return ((LL_REG_READ(SN_SCU->SLP_APB1CLKG) & SCU_SLP_APB1CLKG_I2C2CLKEN) >> SCU_SLP_APB1CLKG_I2C2CLKEN_Pos);
}

/**
 * @brief  Enable CT16B0 clock in sleep mode
 * @retval None
 */
__STATIC_INLINE void LL_RCC_Enable_CT16B0CLK_Sleep(void)
{
    LL_REG_SBIT(SN_SCU->SLP_APB1CLKG, SCU_SLP_APB1CLKG_CT16B0CLKEN);
}

/**
 * @brief  Disable CT16B0 clock in sleep mode
 * @retval None
 */
__STATIC_INLINE void LL_RCC_Disable_CT16B0CLK_Sleep(void)
{
    LL_REG_CBIT(SN_SCU->SLP_APB1CLKG, SCU_SLP_APB1CLKG_CT16B0CLKEN);
}

/**
 * @brief  Return if enabled CT16B0 clock in sleep mode
 * @retval 1: Enabled;   0: Disabled
 */
__STATIC_INLINE uint32_t LL_RCC_IsEnabled_CT16B0CLK_Sleep(void)
{
    return ((LL_REG_READ(SN_SCU->SLP_APB1CLKG) & SCU_SLP_APB1CLKG_CT16B0CLKEN) >> SCU_SLP_APB1CLKG_CT16B0CLKEN_Pos);
}

/**
 * @brief  Enable CT16B1 clock in sleep mode
 * @retval None
 */
__STATIC_INLINE void LL_RCC_Enable_CT16B1CLK_Sleep(void)
{
    LL_REG_SBIT(SN_SCU->SLP_APB1CLKG, SCU_SLP_APB1CLKG_CT16B1CLKEN);
}

/**
 * @brief  Disable CT16B1 clock in sleep mode
 * @retval None
 */
__STATIC_INLINE void LL_RCC_Disable_CT16B1CLK_Sleep(void)
{
    LL_REG_CBIT(SN_SCU->SLP_APB1CLKG, SCU_SLP_APB1CLKG_CT16B1CLKEN);
}

/**
 * @brief  Return if enabled CT16B1 clock in sleep mode
 * @retval 1: Enabled;   0: Disabled
 */
__STATIC_INLINE uint32_t LL_RCC_IsEnabled_CT16B1CLK_Sleep(void)
{
    return ((LL_REG_READ(SN_SCU->SLP_APB1CLKG) & SCU_SLP_APB1CLKG_CT16B1CLKEN) >> SCU_SLP_APB1CLKG_CT16B1CLKEN_Pos);
}

/**
 * @brief  Enable CT16B2 clock in sleep mode
 * @retval None
 */
__STATIC_INLINE void LL_RCC_Enable_CT16B2CLK_Sleep(void)
{
    LL_REG_SBIT(SN_SCU->SLP_APB1CLKG, SCU_SLP_APB1CLKG_CT16B2CLKEN);
}

/**
 * @brief  Disable CT16B2 clock in sleep mode
 * @retval None
 */
__STATIC_INLINE void LL_RCC_Disable_CT16B2CLK_Sleep(void)
{
    LL_REG_CBIT(SN_SCU->SLP_APB1CLKG, SCU_SLP_APB1CLKG_CT16B2CLKEN);
}

/**
 * @brief  Return if enabled CT16B2 clock in sleep mode
 * @retval 1: Enabled;   0: Disabled
 */
__STATIC_INLINE uint32_t LL_RCC_IsEnabled_CT16B2CLK_Sleep(void)
{
    return ((LL_REG_READ(SN_SCU->SLP_APB1CLKG) & SCU_SLP_APB1CLKG_CT16B2CLKEN) >> SCU_SLP_APB1CLKG_CT16B2CLKEN_Pos);
}

/**
 * @brief  Enable CT16B3 clock in sleep mode
 * @retval None
 */
__STATIC_INLINE void LL_RCC_Enable_CT16B3CLK_Sleep(void)
{
    LL_REG_SBIT(SN_SCU->SLP_APB1CLKG, SCU_SLP_APB1CLKG_CT16B3CLKEN);
}

/**
 * @brief  Disable CT16B3 clock in sleep mode
 * @retval None
 */
__STATIC_INLINE void LL_RCC_Disable_CT16B3CLK_Sleep(void)
{
    LL_REG_CBIT(SN_SCU->SLP_APB1CLKG, SCU_SLP_APB1CLKG_CT16B3CLKEN);
}

/**
 * @brief  Return if enabled CT16B3 clock in sleep mode
 * @retval 1: Enabled;   0: Disabled
 */
__STATIC_INLINE uint32_t LL_RCC_IsEnabled_CT16B3CLK_Sleep(void)
{
    return ((LL_REG_READ(SN_SCU->SLP_APB1CLKG) & SCU_SLP_APB1CLKG_CT16B3CLKEN) >> SCU_SLP_APB1CLKG_CT16B3CLKEN_Pos);
}

/**
 * @brief  Enable CT16B4 clock in sleep mode
 * @retval None
 */
__STATIC_INLINE void LL_RCC_Enable_CT16B4CLK_Sleep(void)
{
    LL_REG_SBIT(SN_SCU->SLP_APB1CLKG, SCU_SLP_APB1CLKG_CT16B4CLKEN);
}

/**
 * @brief  Disable CT16B4 clock in sleep mode
 * @retval None
 */
__STATIC_INLINE void LL_RCC_Disable_CT16B4CLK_Sleep(void)
{
    LL_REG_CBIT(SN_SCU->SLP_APB1CLKG, SCU_SLP_APB1CLKG_CT16B4CLKEN);
}

/**
 * @brief  Return if enabled CT16B4 clock in sleep mode
 * @retval 1: Enabled;   0: Disabled
 */
__STATIC_INLINE uint32_t LL_RCC_IsEnabled_CT16B4CLK_Sleep(void)
{
    return ((LL_REG_READ(SN_SCU->SLP_APB1CLKG) & SCU_SLP_APB1CLKG_CT16B4CLKEN) >> SCU_SLP_APB1CLKG_CT16B4CLKEN_Pos);
}

/**
 * @brief  Enable CT16B5 clock in sleep mode
 * @retval None
 */
__STATIC_INLINE void LL_RCC_Enable_CT16B5CLK_Sleep(void)
{
    LL_REG_SBIT(SN_SCU->SLP_APB1CLKG, SCU_SLP_APB1CLKG_CT16B5CLKEN);
}

/**
 * @brief  Disable CT16B5 clock in sleep mode
 * @retval None
 */
__STATIC_INLINE void LL_RCC_Disable_CT16B5CLK_Sleep(void)
{
    LL_REG_CBIT(SN_SCU->SLP_APB1CLKG, SCU_SLP_APB1CLKG_CT16B5CLKEN);
}

/**
 * @brief  Return if enabled CT16B5 clock in sleep mode
 * @retval 1: Enabled;   0: Disabled
 */
__STATIC_INLINE uint32_t LL_RCC_IsEnabled_CT16B5CLK_Sleep(void)
{
    return ((LL_REG_READ(SN_SCU->SLP_APB1CLKG) & SCU_SLP_APB1CLKG_CT16B5CLKEN) >> SCU_SLP_APB1CLKG_CT16B5CLKEN_Pos);
}

/**
 * @brief  Enable CT16B8 clock in sleep mode
 * @retval None
 */
__STATIC_INLINE void LL_RCC_Enable_CT16B8CLK_Sleep(void)
{
    LL_REG_SBIT(SN_SCU->SLP_APB1CLKG, SCU_SLP_APB1CLKG_CT16B8CLKEN);
}

/**
 * @brief  Disable CT16B8 clock in sleep mode
 * @retval None
 */
__STATIC_INLINE void LL_RCC_Disable_CT16B8CLK_Sleep(void)
{
    LL_REG_CBIT(SN_SCU->SLP_APB1CLKG, SCU_SLP_APB1CLKG_CT16B8CLKEN);
}

/**
 * @brief  Return if enabled CT16B8 clock in sleep mode
 * @retval 1: Enabled;   0: Disabled
 */
__STATIC_INLINE uint32_t LL_RCC_IsEnabled_CT16B8CLK_Sleep(void)
{
    return ((LL_REG_READ(SN_SCU->SLP_APB1CLKG) & SCU_SLP_APB1CLKG_CT16B8CLKEN) >> SCU_SLP_APB1CLKG_CT16B8CLKEN_Pos);
}

/**
 * @brief  Enable CRC clock in sleep mode
 * @retval None
 */
__STATIC_INLINE void LL_RCC_Enable_CRCCLK_Sleep(void)
{
    LL_REG_SBIT(SN_SCU->SLP_APB1CLKG, SCU_SLP_APB1CLKG_CRCCLKEN);
}

/**
 * @brief  Disable CRC clock in sleep mode
 * @retval None
 */
__STATIC_INLINE void LL_RCC_Disable_CRCCLK_Sleep(void)
{
    LL_REG_CBIT(SN_SCU->SLP_APB1CLKG, SCU_SLP_APB1CLKG_CRCCLKEN);
}

/**
 * @brief  Return if enabled CRC clock in sleep mode
 * @retval 1: Enabled;   0: Disabled
 */
__STATIC_INLINE uint32_t LL_RCC_IsEnabled_CRCCLK_Sleep(void)
{
    return ((LL_REG_READ(SN_SCU->SLP_APB1CLKG) & SCU_SLP_APB1CLKG_CRCCLKEN) >> SCU_SLP_APB1CLKG_CRCCLKEN_Pos);
}

/**
 * @brief  Enable GPIO0 clock in sleep mode
 * @retval None
 */
__STATIC_INLINE void LL_RCC_Enable_GPIO0CLK_Sleep(void)
{
    LL_REG_SBIT(SN_SCU->SLP_APB1CLKG, SCU_SLP_APB1CLKG_P0CLKEN);
}

/**
 * @brief  Disable GPIO0 clock in sleep mode
 * @retval None
 */
__STATIC_INLINE void LL_RCC_Disable_GPIO0CLK_Sleep(void)
{
    LL_REG_CBIT(SN_SCU->SLP_APB1CLKG, SCU_SLP_APB1CLKG_P0CLKEN);
}

/**
 * @brief  Return if enabled GPIO0 clock in sleep mode
 * @retval 1: Enabled;   0: Disabled
 */
__STATIC_INLINE uint32_t LL_RCC_IsEnabled_GPIO0CLK_Sleep(void)
{
    return ((LL_REG_READ(SN_SCU->SLP_APB1CLKG) & SCU_SLP_APB1CLKG_P0CLKEN) >> SCU_SLP_APB1CLKG_P0CLKEN_Pos);
}

/**
 * @brief  Enable GPIO1 clock in sleep mode
 * @retval None
 */
__STATIC_INLINE void LL_RCC_Enable_GPIO1CLK_Sleep(void)
{
    LL_REG_SBIT(SN_SCU->SLP_APB1CLKG, SCU_SLP_APB1CLKG_P1CLKEN);
}

/**
 * @brief  Disable GPIO1 clock in sleep mode
 * @retval None
 */
__STATIC_INLINE void LL_RCC_Disable_GPIO1CLK_Sleep(void)
{
    LL_REG_CBIT(SN_SCU->SLP_APB1CLKG, SCU_SLP_APB1CLKG_P1CLKEN);
}

/**
 * @brief  Return if enabled GPIO1 clock in sleep mode
 * @retval 1: Enabled;   0: Disabled
 */
__STATIC_INLINE uint32_t LL_RCC_IsEnabled_GPIO1CLK_Sleep(void)
{
    return ((LL_REG_READ(SN_SCU->SLP_APB1CLKG) & SCU_SLP_APB1CLKG_P1CLKEN) >> SCU_SLP_APB1CLKG_P1CLKEN_Pos);
}

/**
 * @brief  Enable GPIO2 clock in sleep mode
 * @retval None
 */
__STATIC_INLINE void LL_RCC_Enable_GPIO2CLK_Sleep(void)
{
    LL_REG_SBIT(SN_SCU->SLP_APB1CLKG, SCU_SLP_APB1CLKG_P2CLKEN);
}

/**
 * @brief  Disable GPIO2 clock in sleep mode
 * @retval None
 */
__STATIC_INLINE void LL_RCC_Disable_GPIO2CLK_Sleep(void)
{
    LL_REG_CBIT(SN_SCU->SLP_APB1CLKG, SCU_SLP_APB1CLKG_P2CLKEN);
}

/**
 * @brief  Return if enabled GPIO2 clock in sleep mode
 * @retval 1: Enabled;   0: Disabled
 */
__STATIC_INLINE uint32_t LL_RCC_IsEnabled_GPIO2CLK_Sleep(void)
{
    return ((LL_REG_READ(SN_SCU->SLP_APB1CLKG) & SCU_SLP_APB1CLKG_P2CLKEN) >> SCU_SLP_APB1CLKG_P2CLKEN_Pos);
}

/**
 * @brief  Enable GPIO3 clock in sleep mode
 * @retval None
 */
__STATIC_INLINE void LL_RCC_Enable_GPIO3CLK_Sleep(void)
{
    LL_REG_SBIT(SN_SCU->SLP_APB1CLKG, SCU_SLP_APB1CLKG_P3CLKEN);
}

/**
 * @brief  Disable GPIO3 clock in sleep mode
 * @retval None
 */
__STATIC_INLINE void LL_RCC_Disable_GPIO3CLK_Sleep(void)
{
    LL_REG_CBIT(SN_SCU->SLP_APB1CLKG, SCU_SLP_APB1CLKG_P3CLKEN);
}

/**
 * @brief  Return if enabled GPIO3 clock in sleep mode
 * @retval 1: Enabled;   0: Disabled
 */
__STATIC_INLINE uint32_t LL_RCC_IsEnabled_GPIO3CLK_Sleep(void)
{
    return ((LL_REG_READ(SN_SCU->SLP_APB1CLKG) & SCU_SLP_APB1CLKG_P3CLKEN) >> SCU_SLP_APB1CLKG_P3CLKEN_Pos);
}

/**
 * @brief  Enable LCM clock in sleep mode
 * @retval None
 */
__STATIC_INLINE void LL_RCC_Enable_LCMCLK_Sleep(void)
{
    LL_REG_SBIT(SN_SCU->SLP_APB1CLKG, SCU_SLP_APB1CLKG_LCMCLKEN);
}

/**
 * @brief  Disable LCM clock in sleep mode
 * @retval None
 */
__STATIC_INLINE void LL_RCC_Disable_LCMCLK_Sleep(void)
{
    LL_REG_CBIT(SN_SCU->SLP_APB1CLKG, SCU_SLP_APB1CLKG_LCMCLKEN);
}

/**
 * @brief  Return if enabled LCM clock in sleep mode
 * @retval 1: Enabled;   0: Disabled
 */
__STATIC_INLINE uint32_t LL_RCC_IsEnabled_LCMCLK_Sleep(void)
{
    return ((LL_REG_READ(SN_SCU->SLP_APB1CLKG) & SCU_SLP_APB1CLKG_LCMCLKEN) >> SCU_SLP_APB1CLKG_LCMCLKEN_Pos);
}

/**
 * @brief  Set CT16B0 clock source
 * @param  clk_sel This parameter can be one of @ref rcc_peripheral_clk_sel
 * @retval None
 */
__STATIC_INLINE void LL_RCC_SetCT16B0ClockSource(uint32_t clk_sel)
{
    LL_REG_CBIT(SN_SCU->CLKSEL, SCU_CLKSEL_CT16B0CLKSEL);
    LL_REG_SBIT(SN_SCU->CLKSEL, (clk_sel << SCU_CLKSEL_CT16B0CLKSEL_Pos) & SCU_CLKSEL_CT16B0CLKSEL);
}

/**
 * @brief  Get CT16B0 clock source
 * @retval Returned value can be one of \ref rcc_peripheral_clk_sel
 */
__STATIC_INLINE uint32_t LL_RCC_GetCT16B0ClockSource(void)
{
    return (LL_REG_RBIT(SN_SCU->CLKSEL, SCU_CLKSEL_CT16B0CLKSEL) >> SCU_CLKSEL_CT16B0CLKSEL_Pos);
}

/**
 * @brief  Set CT16B1 clock source
 * @param  clk_sel This parameter can be one of @ref rcc_peripheral_clk_sel
 * @retval None
 */
__STATIC_INLINE void LL_RCC_SetCT16B1ClockSource(uint32_t clk_sel)
{
    LL_REG_CBIT(SN_SCU->CLKSEL, SCU_CLKSEL_CT16B1CLKSEL);
    LL_REG_SBIT(SN_SCU->CLKSEL, (clk_sel << SCU_CLKSEL_CT16B1CLKSEL_Pos) & SCU_CLKSEL_CT16B1CLKSEL);
}

/**
 * @brief  Get CT16B1 clock source
 * @retval Returned value can be one of \ref rcc_peripheral_clk_sel
 */
__STATIC_INLINE uint32_t LL_RCC_GetCT16B1ClockSource(void)
{
    return (LL_REG_RBIT(SN_SCU->CLKSEL, SCU_CLKSEL_CT16B1CLKSEL) >> SCU_CLKSEL_CT16B1CLKSEL_Pos);
}

/**
 * @brief  Set CT16B2 clock source
 * @param  clk_sel This parameter can be one of @ref rcc_peripheral_clk_sel
 * @retval None
 */
__STATIC_INLINE void LL_RCC_SetCT16B2ClockSource(uint32_t clk_sel)
{
    LL_REG_CBIT(SN_SCU->CLKSEL, SCU_CLKSEL_CT16B2CLKSEL);
    LL_REG_SBIT(SN_SCU->CLKSEL, (clk_sel << SCU_CLKSEL_CT16B2CLKSEL_Pos) & SCU_CLKSEL_CT16B2CLKSEL);
}

/**
 * @brief  Get CT16B2 clock source
 * @retval Returned value can be one of \ref rcc_peripheral_clk_sel
 */
__STATIC_INLINE uint32_t LL_RCC_GetCT16B2ClockSource(void)
{
    return (LL_REG_RBIT(SN_SCU->CLKSEL, SCU_CLKSEL_CT16B2CLKSEL) >> SCU_CLKSEL_CT16B2CLKSEL_Pos);
}

/**
 * @brief  Set CT16B3 clock source
 * @param  clk_sel This parameter can be one of @ref rcc_peripheral_clk_sel
 * @retval None
 */
__STATIC_INLINE void LL_RCC_SetCT16B3ClockSource(uint32_t clk_sel)
{
    LL_REG_CBIT(SN_SCU->CLKSEL, SCU_CLKSEL_CT16B3CLKSEL);
    LL_REG_SBIT(SN_SCU->CLKSEL, (clk_sel << SCU_CLKSEL_CT16B3CLKSEL_Pos) & SCU_CLKSEL_CT16B3CLKSEL);
}

/**
 * @brief  Get CT16B3 clock source
 * @retval Returned value can be one of \ref rcc_peripheral_clk_sel
 */
__STATIC_INLINE uint32_t LL_RCC_GetCT16B3ClockSource(void)
{
    return (LL_REG_RBIT(SN_SCU->CLKSEL, SCU_CLKSEL_CT16B3CLKSEL) >> SCU_CLKSEL_CT16B3CLKSEL_Pos);
}

/**
 * @brief  Set CT16B4 clock source
 * @param  clk_sel This parameter can be one of @ref rcc_peripheral_clk_sel
 * @retval None
 */
__STATIC_INLINE void LL_RCC_SetCT16B4ClockSource(uint32_t clk_sel)
{
    LL_REG_CBIT(SN_SCU->CLKSEL, SCU_CLKSEL_CT16B4CLKSEL);
    LL_REG_SBIT(SN_SCU->CLKSEL, (clk_sel << SCU_CLKSEL_CT16B4CLKSEL_Pos) & SCU_CLKSEL_CT16B4CLKSEL);
}

/**
 * @brief  Get CT16B4 clock source
 * @retval Returned value can be one of \ref rcc_peripheral_clk_sel
 */
__STATIC_INLINE uint32_t LL_RCC_GetCT16B4ClkSource(void)
{
    return (LL_REG_RBIT(SN_SCU->CLKSEL, SCU_CLKSEL_CT16B4CLKSEL) >> SCU_CLKSEL_CT16B4CLKSEL_Pos);
}

/**
 * @brief  Set CT16B5 clock source
 * @param  clk_sel This parameter can be one of @ref rcc_ct16b5_clk_sel
 * @retval None
 */
__STATIC_INLINE void LL_RCC_SetCT16B5ClockSource(uint32_t clk_sel)
{
    LL_REG_CBIT(SN_SCU->CLKSEL, SCU_CLKSEL_CT16B5CLKSEL);
    LL_REG_SBIT(SN_SCU->CLKSEL, (clk_sel << SCU_CLKSEL_CT16B5CLKSEL_Pos) & SCU_CLKSEL_CT16B5CLKSEL);
}

/**
 * @brief  Get CT16B5 clock source
 * @retval Returned value can be one of \ref rcc_ct16b5_clk_sel
 */
__STATIC_INLINE uint32_t LL_RCC_GetCT16B5ClockSource(void)
{
    return (LL_REG_RBIT(SN_SCU->CLKSEL, SCU_CLKSEL_CT16B5CLKSEL) >> SCU_CLKSEL_CT16B5CLKSEL_Pos);
}

/**
 * @brief  Set CT16B6 clock source
 * @param  clk_sel This parameter can be one of @ref rcc_peripheral_clk_sel
 * @retval None
 */
__STATIC_INLINE void LL_RCC_SetCT16B6ClockSource(uint32_t clk_sel)
{
    LL_REG_CBIT(SN_SCU->CLKSEL, SCU_CLKSEL_CT16B6CLKSEL);
    LL_REG_SBIT(SN_SCU->CLKSEL, (clk_sel << SCU_CLKSEL_CT16B6CLKSEL_Pos) & SCU_CLKSEL_CT16B6CLKSEL);
}

/**
 * @brief  Get CT16B6 clock source
 * @retval Returned value can be one of \ref rcc_peripheral_clk_sel
 */
__STATIC_INLINE uint32_t LL_RCC_GetCT16B6ClkSource(void)
{
    return (LL_REG_RBIT(SN_SCU->CLKSEL, SCU_CLKSEL_CT16B6CLKSEL) >> SCU_CLKSEL_CT16B6CLKSEL_Pos);
}

/**
 * @brief  Set CT16B7 clock source
 * @param  clk_sel This parameter can be one of @ref rcc_peripheral_clk_sel
 * @retval None
 */
__STATIC_INLINE void LL_RCC_SetCT16B7ClockSource(uint32_t clk_sel)
{
    LL_REG_CBIT(SN_SCU->CLKSEL, SCU_CLKSEL_CT16B7CLKSEL);
    LL_REG_SBIT(SN_SCU->CLKSEL, (clk_sel << SCU_CLKSEL_CT16B7CLKSEL_Pos) & SCU_CLKSEL_CT16B7CLKSEL);
}

/**
 * @brief  Get CT16B7 clock source
 * @retval Returned value can be one of \ref rcc_peripheral_clk_sel
 */
__STATIC_INLINE uint32_t LL_RCC_GetCT16B7ClkSource(void)
{
    return (LL_REG_RBIT(SN_SCU->CLKSEL, SCU_CLKSEL_CT16B7CLKSEL) >> SCU_CLKSEL_CT16B7CLKSEL_Pos);
}

/**
 * @brief  Set CT16B8 clock source
 * @param  clk_sel This parameter can be one of @ref rcc_peripheral_clk_sel
 * @retval None
 */
__STATIC_INLINE void LL_RCC_SetCT16B8ClockSource(uint32_t clk_sel)
{
    LL_REG_CBIT(SN_SCU->CLKSEL, SCU_CLKSEL_CT16B8CLKSEL);
    LL_REG_SBIT(SN_SCU->CLKSEL, (clk_sel << SCU_CLKSEL_CT16B8CLKSEL_Pos) & SCU_CLKSEL_CT16B8CLKSEL);
}

/**
 * @brief  Get CT16B8 clock source
 * @retval Returned value can be one of \ref rcc_peripheral_clk_sel
 */
__STATIC_INLINE uint32_t LL_RCC_GetCT16B8ClkSource(void)
{
    return (LL_REG_RBIT(SN_SCU->CLKSEL, SCU_CLKSEL_CT16B8CLKSEL) >> SCU_CLKSEL_CT16B8CLKSEL_Pos);
}

/**
 * @brief  Set UART0 clock source
 * @param  clk_sel This parameter can be one of @ref rcc_peripheral_clk_sel
 * @retval None
 */
__STATIC_INLINE void LL_RCC_SetUART0ClockSource(uint32_t clk_sel)
{
    LL_REG_CBIT(SN_SCU->CLKSEL, SCU_CLKSEL_UART0CLKSEL);
    LL_REG_SBIT(SN_SCU->CLKSEL, (clk_sel << SCU_CLKSEL_UART0CLKSEL_Pos) & SCU_CLKSEL_UART0CLKSEL);
}

/**
 * @brief  Get UART0 clock source
 * @retval Returned value can be one of \ref rcc_peripheral_clk_sel
 */
__STATIC_INLINE uint32_t LL_RCC_GetUART0ClockSource(void)
{
    return (LL_REG_RBIT(SN_SCU->CLKSEL, SCU_CLKSEL_UART0CLKSEL) >> SCU_CLKSEL_UART0CLKSEL_Pos);
}

/**
 * @brief  Set UART1 clock source
 * @param  clk_sel This parameter can be one of @ref rcc_peripheral_clk_sel
 * @retval None
 */
__STATIC_INLINE void LL_RCC_SetUART1ClockSource(uint32_t clk_sel)
{
    LL_REG_CBIT(SN_SCU->CLKSEL, SCU_CLKSEL_UART1CLKSEL);
    LL_REG_SBIT(SN_SCU->CLKSEL, (clk_sel << SCU_CLKSEL_UART1CLKSEL_Pos) & SCU_CLKSEL_UART1CLKSEL);
}

/**
 * @brief  Get UART1 clock source
 * @retval Returned value can be one of \ref rcc_peripheral_clk_sel
 */
__STATIC_INLINE uint32_t LL_RCC_GetUART1ClockSource(void)
{
    return (LL_REG_RBIT(SN_SCU->CLKSEL, SCU_CLKSEL_UART1CLKSEL) >> SCU_CLKSEL_UART1CLKSEL_Pos);
}

/**
 * @brief  Set UART2 clock source
 * @param  clk_sel This parameter can be one of @ref rcc_peripheral_clk_sel
 * @retval None
 */
__STATIC_INLINE void LL_RCC_SetUART2ClockSource(uint32_t clk_sel)
{
    LL_REG_CBIT(SN_SCU->CLKSEL, SCU_CLKSEL_UART2CLKSEL);
    LL_REG_SBIT(SN_SCU->CLKSEL, (clk_sel << SCU_CLKSEL_UART2CLKSEL_Pos) & SCU_CLKSEL_UART2CLKSEL);
}

/**
 * @brief  Get UART2 clock source
 * @retval Returned value can be one of \ref rcc_peripheral_clk_sel
 */
__STATIC_INLINE uint32_t LL_RCC_GetUART2ClockSource(void)
{
    return (LL_REG_RBIT(SN_SCU->CLKSEL, SCU_CLKSEL_UART2CLKSEL) >> SCU_CLKSEL_UART2CLKSEL_Pos);
}

/**
 * @brief  Set UART3 clock source
 * @param  clk_sel This parameter can be one of @ref rcc_peripheral_clk_sel
 * @retval None
 */
__STATIC_INLINE void LL_RCC_SetUART3ClockSource(uint32_t clk_sel)
{
    LL_REG_CBIT(SN_SCU->CLKSEL, SCU_CLKSEL_UART3CLKSEL);
    LL_REG_SBIT(SN_SCU->CLKSEL, (clk_sel << SCU_CLKSEL_UART3CLKSEL_Pos) & SCU_CLKSEL_UART3CLKSEL);
}

/**
 * @brief  Get UART3 clock source
 * @retval Returned value can be one of \ref rcc_peripheral_clk_sel
 */
__STATIC_INLINE uint32_t LL_RCC_GetUART3ClockSource(void)
{
    return (LL_REG_RBIT(SN_SCU->CLKSEL, SCU_CLKSEL_UART3CLKSEL) >> SCU_CLKSEL_UART3CLKSEL_Pos);
}

/**
 * @brief  Set UART4 clock source
 * @param  clk_sel This parameter can be one of @ref rcc_peripheral_clk_sel
 * @retval None
 */
__STATIC_INLINE void LL_RCC_SetUART4ClockSource(uint32_t clk_sel)
{
    LL_REG_CBIT(SN_SCU->CLKSEL, SCU_CLKSEL_UART4CLKSEL);
    LL_REG_SBIT(SN_SCU->CLKSEL, (clk_sel << SCU_CLKSEL_UART4CLKSEL_Pos) & SCU_CLKSEL_UART4CLKSEL);
}

/**
 * @brief  Get UART4 clock source
 * @retval Returned value can be one of \ref rcc_peripheral_clk_sel
 */
__STATIC_INLINE uint32_t LL_RCC_GetUART4ClockSource(void)
{
    return (LL_REG_RBIT(SN_SCU->CLKSEL, SCU_CLKSEL_UART4CLKSEL) >> SCU_CLKSEL_UART4CLKSEL_Pos);
}

/**
 * @brief  Set UART5 clock source
 * @param  clk_sel This parameter can be one of @ref rcc_peripheral_clk_sel
 * @retval None
 */
__STATIC_INLINE void LL_RCC_SetUART5ClockSource(uint32_t clk_sel)
{
    LL_REG_CBIT(SN_SCU->CLKSEL, SCU_CLKSEL_UART5CLKSEL);
    LL_REG_SBIT(SN_SCU->CLKSEL, (clk_sel << SCU_CLKSEL_UART5CLKSEL_Pos) & SCU_CLKSEL_UART5CLKSEL);
}

/**
 * @brief  Get UART5 clock source
 * @retval Returned value can be one of \ref rcc_peripheral_clk_sel
 */
__STATIC_INLINE uint32_t LL_RCC_GetUART5ClockSource(void)
{
    return (LL_REG_RBIT(SN_SCU->CLKSEL, SCU_CLKSEL_UART5CLKSEL) >> SCU_CLKSEL_UART5CLKSEL_Pos);
}

/**
 * @brief  Set SSP0 clock source
 * @param  clk_sel This parameter can be one of @ref rcc_peripheral_clk_sel
 * @retval None
 */
__STATIC_INLINE void LL_RCC_SetSSP0ClockSource(uint32_t clk_sel)
{
    LL_REG_CBIT(SN_SCU->CLKSEL, SCU_CLKSEL_SSP0CLKSEL);
    LL_REG_SBIT(SN_SCU->CLKSEL, (clk_sel << SCU_CLKSEL_SSP0CLKSEL_Pos) & SCU_CLKSEL_SSP0CLKSEL);
}

/**
 * @brief  Get SSP0 clock source
 * @retval Returned value can be one of \ref rcc_peripheral_clk_sel
 */
__STATIC_INLINE uint32_t LL_RCC_GetSSP0ClockSource(void)
{
    return (LL_REG_RBIT(SN_SCU->CLKSEL, SCU_CLKSEL_SSP0CLKSEL) >> SCU_CLKSEL_SSP0CLKSEL_Pos);
}

/**
 * @brief  Set SSP1 clock source
 * @param  clk_sel This parameter can be one of @ref rcc_peripheral_clk_sel
 * @retval None
 */
__STATIC_INLINE void LL_RCC_SetSSP1ClockSource(uint32_t clk_sel)
{
    LL_REG_CBIT(SN_SCU->CLKSEL, SCU_CLKSEL_SSP1CLKSEL);
    LL_REG_SBIT(SN_SCU->CLKSEL, (clk_sel << SCU_CLKSEL_SSP1CLKSEL_Pos) & SCU_CLKSEL_SSP1CLKSEL);
}

/**
 * @brief  Get SSP1 clock source
 * @retval Returned value can be one of \ref rcc_peripheral_clk_sel
 */
__STATIC_INLINE uint32_t LL_RCC_GetSSP1ClockSource(void)
{
    return (LL_REG_RBIT(SN_SCU->CLKSEL, SCU_CLKSEL_SSP1CLKSEL) >> SCU_CLKSEL_SSP1CLKSEL_Pos);
}

/**
 * @brief  Set SSP2 clock source
 * @param  clk_sel This parameter can be one of @ref rcc_peripheral_clk_sel
 * @retval None
 */
__STATIC_INLINE void LL_RCC_SetSSP2ClockSource(uint32_t clk_sel)
{
    LL_REG_CBIT(SN_SCU->CLKSEL, SCU_CLKSEL_SSP2CLKSEL);
    LL_REG_SBIT(SN_SCU->CLKSEL, (clk_sel << SCU_CLKSEL_SSP2CLKSEL_Pos) & SCU_CLKSEL_SSP2CLKSEL);
}

/**
 * @brief  Get SSP2 clock source
 * @retval Returned value can be one of \ref rcc_peripheral_clk_sel
 */
__STATIC_INLINE uint32_t LL_RCC_GetSSP2ClockSource(void)
{
    return (LL_REG_RBIT(SN_SCU->CLKSEL, SCU_CLKSEL_SSP2CLKSEL) >> SCU_CLKSEL_SSP2CLKSEL_Pos);
}

/**
 * @brief  Set SDIO clock source
 * @param  clk_sel This parameter can be one of @ref rcc_peripheral_clk_sel
 * @retval None
 */
__STATIC_INLINE void LL_RCC_SetSDIOClockSource(uint32_t clk_sel)
{
    LL_REG_CBIT(SN_SCU->CLKSEL, SCU_CLKSEL_SDIOCLKSEL);
    LL_REG_SBIT(SN_SCU->CLKSEL, (clk_sel << SCU_CLKSEL_SDIOCLKSEL_Pos) & SCU_CLKSEL_SDIOCLKSEL);
}

/**
 * @brief  Get SDIO clock source
 * @retval Returned value can be one of \ref rcc_peripheral_clk_sel
 */
__STATIC_INLINE uint32_t LL_RCC_GetSDIOClkSource(void)
{
    return (LL_REG_RBIT(SN_SCU->CLKSEL, SCU_CLKSEL_SDIOCLKSEL) >> SCU_CLKSEL_SDIOCLKSEL_Pos);
}

/**
 * @brief  Set WWDT clock source
 * @param  clk_sel This parameter can be one of @ref rcc_wwdt_clk_sel
 * @retval None
 */
__STATIC_INLINE void LL_RCC_SetWWDTClockSource(uint32_t clk_sel)
{
    LL_REG_CBIT(SN_SCU->CLKSEL, SCU_CLKSEL_WWDTCLKSEL);
    LL_REG_SBIT(SN_SCU->CLKSEL, (clk_sel << SCU_CLKSEL_WWDTCLKSEL_Pos) & SCU_CLKSEL_WWDTCLKSEL);
}

/**
 * @brief  Get WWDT clock source
 * @retval Returned value can be one of \ref rcc_wwdt_clk_sel
 */
__STATIC_INLINE uint32_t LL_RCC_GetWWDTClockSource(void)
{
    return (LL_REG_RBIT(SN_SCU->CLKSEL, SCU_CLKSEL_WWDTCLKSEL) >> SCU_CLKSEL_WWDTCLKSEL_Pos);
}

/**
 * @brief  Set LCM clock source
 * @param  clk_sel This parameter can be one of @ref rcc_wwdt_clk_sel
 * @retval None
 */
__STATIC_INLINE void LL_RCC_SetLCMClockSource(uint32_t clk_sel)
{
    LL_REG_CBIT(SN_SCU->CLKSEL, SCU_CLKSEL_LCMCLKSEL);
    LL_REG_SBIT(SN_SCU->CLKSEL, (clk_sel << SCU_CLKSEL_LCMCLKSEL_Pos) & SCU_CLKSEL_LCMCLKSEL);
}

/**
 * @brief  Get LCM clock source
 * @retval Returned value can be one of \ref rcc_peripheral_clk_sel
 */
__STATIC_INLINE uint32_t LL_RCC_GetLCMClockSource(void)
{
    return (LL_REG_RBIT(SN_SCU->CLKSEL, SCU_CLKSEL_LCMCLKSEL) >> SCU_CLKSEL_LCMCLKSEL_Pos);
}

/**
 * @brief  Set Clock output source
 * @param  clk_sel clock source, \ref rcc_clkout_src
 * @retval None
 */
__STATIC_INLINE void LL_RCC_SetCLKOUTSource(uint32_t clk_sel)
{
    LL_REG_CBIT(SN_SCU->CLKSEL, SCU_CLKSEL_CLKOUTSEL);
    LL_REG_SBIT(SN_SCU->CLKSEL, clk_sel);
}

/**
 * @brief  Get Clock output source
 * @retval returned value can be one of \ref rcc_clkout_src
 */
__STATIC_INLINE uint32_t LL_RCC_GetCLKOUTSource(void)
{
    return (LL_REG_RBIT(SN_SCU->CLKSEL, SCU_CLKSEL_CLKOUTSEL));
}

/**
 * @brief  Set HCLK prescaler
 * @param  prescaler prescaler, \ref rcc_hclk_div
 * @retval None
 *
 * @note   It is write-protected by WRPKEY
 */
__STATIC_INLINE void LL_RCC_SetHCLKPrescaler(uint8_t prescaler)
{
    LL_REG_CBIT(SN_SCU->CLKPRE, SCU_CLKPRE_WRPKEY);
    LL_REG_SBIT(SN_SCU->CLKPRE, 0x5AFA << SCU_CLKPRE_WRPKEY_Pos);
    LL_REG_CBIT(SN_SCU->CLKPRE, SCU_CLKPRE_AHBPRE);
    LL_REG_SBIT(SN_SCU->CLKPRE, prescaler << SCU_CLKPRE_AHBPRE_Pos);
}

/**
 * @brief  Get HCLK prescaler
 * @retval AHB Prescaler \ref rcc_hclk_div
 */
__STATIC_INLINE uint32_t LL_RCC_GetHCLKPrescaler(void)
{
    return ((LL_REG_READ(SN_SCU->CLKPRE) & SCU_CLKPRE_AHBPRE) >> SCU_CLKPRE_AHBPRE_Pos);
}

/**
 * @brief  Set APB0 prescaler
 * @param  prescaler prescaler, \ref rcc_apb0_apb1_clk_div
 * @retval None
 *
 * @note   It is write-protected by WRPKEY
 */
__STATIC_INLINE void LL_RCC_SetAPB0Prescaler(uint32_t prescaler)
{
    LL_REG_CBIT(SN_SCU->CLKPRE, SCU_CLKPRE_WRPKEY);
    LL_REG_SBIT(SN_SCU->CLKPRE, 0x5AFA << SCU_CLKPRE_WRPKEY_Pos);
    LL_REG_CBIT(SN_SCU->CLKPRE, SCU_CLKPRE_APB0PRE);
    LL_REG_SBIT(SN_SCU->CLKPRE, prescaler << SCU_CLKPRE_APB0PRE_Pos);
}

/**
 * @brief  Get APB0 prescaler
 * @retval APB0 prescale \ref rcc_apb0_apb1_clk_div
 */
__STATIC_INLINE uint32_t LL_RCC_GetAPB0Prescaler(void)
{
    return ((LL_REG_READ(SN_SCU->CLKPRE) & SCU_CLKPRE_APB0PRE) >> SCU_CLKPRE_APB0PRE_Pos);
}

/**
 * @brief  Set APB1 prescaler
 * @param  prescaler prescaler, \ref rcc_apb0_apb1_clk_div
 * @retval None
 *
 * @note   It is write-protected by WRPKEY
 */
__STATIC_INLINE void LL_RCC_SetAPB1Prescale(uint32_t prescaler)
{
    LL_REG_CBIT(SN_SCU->CLKPRE, SCU_CLKPRE_WRPKEY);
    LL_REG_SBIT(SN_SCU->CLKPRE, 0x5AFA << SCU_CLKPRE_WRPKEY_Pos);
    LL_REG_CBIT(SN_SCU->CLKPRE, SCU_CLKPRE_APB1PRE);
    LL_REG_SBIT(SN_SCU->CLKPRE, prescaler << SCU_CLKPRE_APB1PRE_Pos);
}

/**
 * @brief  Get APB1 prescaler
 * @retval APB1 prescaler \ref rcc_apb0_apb1_clk_div
 */
__STATIC_INLINE uint32_t LL_RCC_GetAPB1Prescaler(void)
{
    return (LL_REG_RBIT(SN_SCU->CLKPRE, SCU_CLKPRE_APB1PRE) >> SCU_CLKPRE_APB1PRE_Pos);
}

/**
 * @brief  Set CLKOUT prescaler
 * @param  prescaler prescaler, \ref rcc_clkout_div
 * @retval None
 * @note   It is write-protected by WRPKEY
 */
__STATIC_INLINE void LL_RCC_SetCLKOUTPrescaler(uint32_t prescaler)
{
    LL_REG_CBIT(SN_SCU->CLKPRE, SCU_CLKPRE_WRPKEY);
    LL_REG_SBIT(SN_SCU->CLKPRE, 0x5AFA << SCU_CLKPRE_WRPKEY_Pos);
    LL_REG_CBIT(SN_SCU->CLKPRE, SCU_CLKPRE_CLKOUTPRE);
    LL_REG_SBIT(SN_SCU->CLKPRE, prescaler);
}

/**
 * @brief  Get CLKOUT prescaler
 * @retval CLKOUT Prescaler \ref rcc_clkout_div
 */
__STATIC_INLINE uint32_t LL_RCC_GetCLKOUTPrescaler(void)
{
    return (LL_REG_RBIT(SN_SCU->CLKPRE, SCU_CLKPRE_CLKOUTPRE) >> SCU_CLKPRE_CLKOUTPRE_Pos);
}

/**
 * @brief  Enable Internal high-speed clock
 * @retval None
 */
__STATIC_INLINE void LL_RCC_Enable_IHRC(void)
{
    LL_REG_SBIT(SN_SCU->IHRCCTRL, SCU_IHRCCTRL_IHRCEN);
}

/**
 * @brief  Disable Internal high-speed clock
 * @retval None
 *
 * @note  This bit can NOT be cleared if the IHRC is selected as system clock
 *        or is selected to become the system clock
 */
__STATIC_INLINE void LL_RCC_Disable_IHRC(void)
{
    LL_REG_CBIT(SN_SCU->IHRCCTRL, SCU_IHRCCTRL_IHRCEN);
}

/**
 * @brief  Return if enabled Internal high-speed clock
 * @retval 1: Enabled;   0: Disabled
 */
__STATIC_INLINE uint32_t LL_RCC_IsEnabled_IHRC(void)
{
    return ((LL_REG_READ(SN_SCU->IHRCCTRL) & SCU_IHRCCTRL_IHRCEN) >> SCU_IHRCCTRL_IHRCEN_Pos);
}

/**
 * @brief  Return if Internal high-speed clock flag is ready
 * @retval 1: Ready;   0: Not Ready
 */
__STATIC_INLINE uint32_t LL_RCC_IsActiveFlag_IHRC_Ready(void)
{
    return ((LL_REG_READ(SN_SCU->IHRCCTRL) & SCU_IHRCCTRL_IHRCRDY) >> SCU_IHRCCTRL_IHRCRDY_Pos);
}

/**
 * @brief  Return if enabled PLL
 * @retval 1: Enabled;   0: Disabled
 */
__STATIC_INLINE uint32_t LL_RCC_IsActiveFlag_PLL_Enabled(void)
{
    return ((LL_REG_READ(SN_SCU->PLLSTS) & SCU_PLLSTS_PLLENSTS) >> SCU_PLLSTS_PLLENSTS_Pos);
}

/**
 * @brief  Get PLL clock status
 * @retval Returned value can be one of @ref rcc_pll_clk_sel
 */
__STATIC_INLINE uint32_t LL_RCC_GetPLLClockStatus(void)
{
    return (LL_REG_RBIT(SN_SCU->PLLSTS, SCU_PLLSTS_PLLCLKSTS));
}

/**
 * @brief  Get System clock status
 * @retval Returned value can be one of @ref rcc_system_clk_sel
 */
__STATIC_INLINE uint32_t LL_RCC_GetSYSClockStatus(void)
{
    return (LL_REG_RBIT(SN_SCU->PLLSTS, SCU_PLLSTS_SYSCLKSTS));
}

/**
 * @brief  Get PLL frequency range output divider status
 * @retval PLL div status \ref rcc_pll_clk_div
 *          0: PLL CKOUT= PLLCLK / 32
 *          1: PLL CKOUT= PLLCLK / 16
 *          2: PLL CKOUT= PLLCLK / 8
 *          3: PLL CKOUT= PLLCLK / 4
 */
__STATIC_INLINE uint32_t LL_RCC_GetPLLFreqDividerStatus(void)
{
    return ((LL_REG_READ(SN_SCU->PLLSTS) & SCU_PLLSTS_FSSTS) >> SCU_PLLSTS_FSSTS_Pos);
}

/**
 * @brief  Get 7-bit programmable loop divider status
 * @retval loop divider
 */
__STATIC_INLINE uint32_t LL_RCC_GetLoopDividerStatus(void)
{
    return ((LL_REG_READ(SN_SCU->PLLSTS) & SCU_PLLSTS_NSSTS) >> SCU_PLLSTS_NSSTS_Pos);
}

/**
 * @brief  Reset DMA0
 * @retval None
 */
__STATIC_INLINE void LL_RCC_Reset_DMA0(void)
{
    LL_REG_SBIT(SN_SCU->AHBRST, SCU_AHBRST_DMA0RST);
}

/**
 * @brief  Return if DMA0 Reset is done
 * @retval 1: Done;   0: Resetting
 */
__STATIC_INLINE uint32_t LL_RCC_IsActiveFlag_DMA0Reset_Done(void)
{
    return (((LL_REG_READ(SN_SCU->AHBRST) & SCU_AHBRST_DMA0RST) >> SCU_AHBRST_DMA0RST_Pos) == 0);
}

/**
 * @brief  Reset DMA1
 * @retval None
 */
__STATIC_INLINE void LL_RCC_Reset_DMA1(void)
{
    LL_REG_SBIT(SN_SCU->AHBRST, SCU_AHBRST_DMA1RST);
}

/**
 * @brief  Return if DMA1 Reset is done
 * @retval 1: Done;   0: Resetting
 */
__STATIC_INLINE uint32_t LL_RCC_IsActiveFlag_DMA1Reset_Done(void)
{
    return (((LL_REG_READ(SN_SCU->AHBRST) & SCU_AHBRST_DMA1RST) >> SCU_AHBRST_DMA1RST_Pos) == 0);
}

/**
 * @brief  Reset SDIO
 * @retval None
 */
__STATIC_INLINE void LL_RCC_Reset_SDIO(void)
{
    LL_REG_SBIT(SN_SCU->AHBRST, SCU_AHBRST_SDIORST);
}
/**
 * @brief  Return if SDIO Reset is done
 * @retval 1: Done;   0: Resetting
 */
__STATIC_INLINE uint32_t LL_RCC_IsActiveFlag_SDIOReset_Done(void)
{
    return (((LL_REG_READ(SN_SCU->AHBRST) & SCU_AHBRST_SDIORST) >> SCU_AHBRST_SDIORST_Pos) == 0);
}

/**
 * @brief  Reset ETH
 * @retval None
 */
__STATIC_INLINE void LL_RCC_Reset_ETH(void)
{
    LL_REG_SBIT(SN_SCU->AHBRST, SCU_AHBRST_ETHRST);
}

/**
 * @brief  Return if ETH Reset is done
 * @retval 1: Done;   0: Resetting
 */
__STATIC_INLINE uint32_t LL_RCC_IsActiveFlag_ETHReset_Done(void)
{
    return (((LL_REG_READ(SN_SCU->AHBRST) & SCU_AHBRST_ETHRST) >> SCU_AHBRST_ETHRST_Pos) == 0);
}

/**
 * @brief  reset WDT
 * @retval None
 */
__STATIC_INLINE void LL_RCC_Reset_WDT(void)
{
    LL_REG_SBIT(SN_SCU->APB0RST, SCU_APB0RST_WDTRST);
}

/**
 * @brief  Return if WDT reset is done
 * @retval 1: Done;   0: Resetting
 */
__STATIC_INLINE uint32_t LL_RCC_IsActiveFlag_WDTReset_Done(void)
{
    return (((LL_REG_READ(SN_SCU->APB0RST) & SCU_APB0RST_WDTRST) >> SCU_APB0RST_WDTRST_Pos) == 0);
}

/**
 * @brief  reset WWDT
 * @retval None
 */
__STATIC_INLINE void LL_RCC_Reset_WWDT(void)
{
    LL_REG_SBIT(SN_SCU->APB0RST, SCU_APB0RST_WWDTRST);
}

/**
 * @brief  Return if WWDT reset is done
 * @retval 1: Done;   0: Resetting
 */
__STATIC_INLINE uint32_t LL_RCC_IsActiveFlag_WWDTReset_Done(void)
{
    return (((LL_REG_READ(SN_SCU->APB0RST) & SCU_APB0RST_WWDTRST) >> SCU_APB0RST_WWDTRST_Pos) == 0);
}

/**
 * @brief  reset CAN0
 * @retval None
 */
__STATIC_INLINE void LL_RCC_Reset_CAN0(void)
{
    LL_REG_SBIT(SN_SCU->APB0RST, SCU_APB0RST_CAN0RST);
}

/**
 * @brief  Return if CAN0 reset is done
 * @retval 1: Done;   0: Resetting
 */
__STATIC_INLINE uint32_t LL_RCC_IsActiveFlag_CAN0Reset_Done(void)
{
    return (((LL_REG_READ(SN_SCU->APB0RST) & SCU_APB0RST_CAN0RST) >> SCU_APB0RST_CAN0RST_Pos) == 0);
}

/**
 * @brief  reset CAN1
 * @retval None
 */
__STATIC_INLINE void LL_RCC_Reset_CAN1(void)
{
    LL_REG_SBIT(SN_SCU->APB0RST, SCU_APB0RST_CAN1RST);
}

/**
 * @brief  Return if CAN1 reset is done
 * @retval 1: Done;   0: Resetting
 */
__STATIC_INLINE uint32_t LL_RCC_IsActiveFlag_CAN1Reset_Done(void)
{
    return (((LL_REG_READ(SN_SCU->APB0RST) & SCU_APB0RST_CAN1RST) >> SCU_APB0RST_CAN1RST_Pos) == 0);
}

/**
 * @brief  reset UART1
 * @retval None
 */
__STATIC_INLINE void LL_RCC_Reset_UART1(void)
{
    LL_REG_SBIT(SN_SCU->APB0RST, SCU_APB0RST_UART1RST);
}

/**
 * @brief  Return if UART1 reset is done
 * @retval 1: Done;   0: Resetting
 */
__STATIC_INLINE uint32_t LL_RCC_IsActiveFlag_UART1Reset_Done(void)
{
    return (((LL_REG_READ(SN_SCU->APB0RST) & SCU_APB0RST_UART1RST) >> SCU_APB0RST_UART1RST_Pos) == 0);
}

/**
 * @brief  reset UART2
 * @retval None
 */
__STATIC_INLINE void LL_RCC_Reset_UART2(void)
{
    LL_REG_SBIT(SN_SCU->APB0RST, SCU_APB0RST_UART2RST);
}

/**
 * @brief  Return if UART2 reset is done
 * @retval 1: Done;   0: Resetting
 */
__STATIC_INLINE uint32_t LL_RCC_IsActiveFlag_UART2Reset_Done(void)
{
    return (((LL_REG_READ(SN_SCU->APB0RST) & SCU_APB0RST_UART2RST) >> SCU_APB0RST_UART2RST_Pos) == 0);
}

/**
 * @brief  reset UART3
 * @retval None
 */
__STATIC_INLINE void LL_RCC_Reset_UART3(void)
{
    LL_REG_SBIT(SN_SCU->APB0RST, SCU_APB0RST_UART3RST);
}

/**
 * @brief  Return if UART3 reset is done
 * @retval 1: Done;   0: Resetting
 */
__STATIC_INLINE uint32_t LL_RCC_IsActiveFlag_UART3Reset_Done(void)
{
    return (((LL_REG_READ(SN_SCU->APB0RST) & SCU_APB0RST_UART3RST) >> SCU_APB0RST_UART3RST_Pos) == 0);
}

/**
 * @brief  reset UART4
 * @retval None
 */
__STATIC_INLINE void LL_RCC_Reset_UART4(void)
{
    LL_REG_SBIT(SN_SCU->APB0RST, SCU_APB0RST_UART4RST);
}

/**
 * @brief  Return if UART4 reset is done
 * @retval 1: Done;   0: Resetting
 */
__STATIC_INLINE uint32_t LL_RCC_IsActiveFlag_UART4Reset_Done(void)
{
    return (((LL_REG_READ(SN_SCU->APB0RST) & SCU_APB0RST_UART4RST) >> SCU_APB0RST_UART4RST_Pos) == 0);
}

/**
 * @brief  reset CT16B6
 * @retval None
 */
__STATIC_INLINE void LL_RCC_Reset_CT16B6(void)
{
    LL_REG_SBIT(SN_SCU->APB0RST, SCU_APB0RST_CT16B6RST);
}

/**
 * @brief  Return if CT16B6 reset is done
 * @retval 1: Done;   0: Resetting
 */
__STATIC_INLINE uint32_t LL_RCC_IsActiveFlag_CT16B6Reset_Done(void)
{
    return (((LL_REG_READ(SN_SCU->APB0RST) & SCU_APB0RST_CT16B6RST) >> SCU_APB0RST_CT16B6RST_Pos) == 0);
}

/**
 * @brief  reset CT16B7
 * @retval None
 */
__STATIC_INLINE void LL_RCC_Reset_CT16B7(void)
{
    LL_REG_SBIT(SN_SCU->APB0RST, SCU_APB0RST_CT16B7RST);
}

/**
 * @brief  Return if CT16B7 reset is done
 * @retval 1: Done;   0: Resetting
 */
__STATIC_INLINE uint32_t LL_RCC_IsActiveFlag_CT16B7Reset_Done(void)
{
    return (((LL_REG_READ(SN_SCU->APB0RST) & SCU_APB0RST_CT16B7RST) >> SCU_APB0RST_CT16B7RST_Pos) == 0);
}

/**
 * @brief  reset ADC0
 * @retval None
 */
__STATIC_INLINE void LL_RCC_Reset_ADC0(void)
{
    LL_REG_SBIT(SN_SCU->APB1RST, SCU_APB1RST_ADC0RST);
}

/**
 * @brief  Return if ADC0 reset is done
 * @retval 1: Done;   0: Resetting
 */
__STATIC_INLINE uint32_t LL_RCC_IsActiveFlag_ADC0Reset_Done(void)
{
    return (((LL_REG_READ(SN_SCU->APB1RST) & SCU_APB1RST_ADC0RST) >> SCU_APB1RST_ADC0RST_Pos) == 0);
}

/**
 * @brief  reset SSP0
 * @retval None
 */
__STATIC_INLINE void LL_RCC_Reset_SSP0(void)
{
    LL_REG_SBIT(SN_SCU->APB1RST, SCU_APB1RST_SSP0RST);
}

/**
 * @brief  Return if SSP0 reset is done
 * @retval 1: Done;   0: Resetting
 */
__STATIC_INLINE uint32_t LL_RCC_IsActiveFlag_SSP0Reset_Done(void)
{
    return (((LL_REG_READ(SN_SCU->APB1RST) & SCU_APB1RST_SSP0RST) >> SCU_APB1RST_SSP0RST_Pos) == 0);
}

/**
 * @brief  reset SSP1
 * @retval None
 */
__STATIC_INLINE void LL_RCC_Reset_SSP1(void)
{
    LL_REG_SBIT(SN_SCU->APB1RST, SCU_APB1RST_SSP1RST);
}

/**
 * @brief  Return if SSP1 reset is done
 * @retval 1: Done;   0: Resetting
 */
__STATIC_INLINE uint32_t LL_RCC_IsActiveFlag_SSP1Reset_Done(void)
{
    return (((LL_REG_READ(SN_SCU->APB1RST) & SCU_APB1RST_SSP1RST) >> SCU_APB1RST_SSP1RST_Pos) == 0);
}

/**
 * @brief  reset SSP2
 * @retval None
 */
__STATIC_INLINE void LL_RCC_Reset_SSP2(void)
{
    LL_REG_SBIT(SN_SCU->APB1RST, SCU_APB1RST_SSP2RST);
}

/**
 * @brief  Return if SSP2 reset is done
 * @retval 1: Done;   0: Resetting
 */
__STATIC_INLINE uint32_t LL_RCC_IsActiveFlag_SSP2Reset_Done(void)
{
    return (((LL_REG_READ(SN_SCU->APB1RST) & SCU_APB1RST_SSP2RST) >> SCU_APB1RST_SSP2RST_Pos) == 0);
}

/**
 * @brief  reset UART0
 * @retval None
 */
__STATIC_INLINE void LL_RCC_Reset_UART0(void)
{
    LL_REG_SBIT(SN_SCU->APB1RST, SCU_APB1RST_UART0RST);
}

/**
 * @brief  Return if UART0 reset is done
 * @retval 1: Done;   0: Resetting
 */
__STATIC_INLINE uint32_t LL_RCC_IsActiveFlag_UART0Reset_Done(void)
{
    return (((LL_REG_READ(SN_SCU->APB1RST) & SCU_APB1RST_UART0RST) >> SCU_APB1RST_UART0RST_Pos) == 0);
}

/**
 * @brief  reset UART5
 * @retval None
 */
__STATIC_INLINE void LL_RCC_Reset_UART5(void)
{
    LL_REG_SBIT(SN_SCU->APB1RST, SCU_APB1RST_UART5RST);
}

/**
 * @brief  Return if UART5 reset is done
 * @retval 1: Done;   0: Resetting
 */
__STATIC_INLINE uint32_t LL_RCC_IsActiveFlag_UART5Reset_Done(void)
{
    return (((LL_REG_READ(SN_SCU->APB1RST) & SCU_APB1RST_UART5RST) >> SCU_APB1RST_UART5RST_Pos) == 0);
}

/**
 * @brief  reset I2C0
 * @retval None
 */
__STATIC_INLINE void LL_RCC_Reset_I2C0(void)
{
    LL_REG_SBIT(SN_SCU->APB1RST, SCU_APB1RST_I2C0RST);
}

/**
 * @brief  Return if I2C0 reset is done
 * @retval 1: Done;   0: Resetting
 */
__STATIC_INLINE uint32_t LL_RCC_IsActiveFlag_I2C0Reset_Done(void)
{
    return (((LL_REG_READ(SN_SCU->APB1RST) & SCU_APB1RST_I2C0RST) >> SCU_APB1RST_I2C0RST_Pos) == 0);
}

/**
 * @brief  reset I2C1
 * @retval None
 */
__STATIC_INLINE void LL_RCC_Reset_I2C1(void)
{
    LL_REG_SBIT(SN_SCU->APB1RST, SCU_APB1RST_I2C1RST);
}

/**
 * @brief  Return if I2C1 reset is done
 * @retval 1: Done;   0: Resetting
 */
__STATIC_INLINE uint32_t LL_RCC_IsActiveFlag_I2C1Reset_Done(void)
{
    return (((LL_REG_READ(SN_SCU->APB1RST) & SCU_APB1RST_I2C1RST) >> SCU_APB1RST_I2C1RST_Pos) == 0);
}

/**
 * @brief  reset I2C2
 * @retval None
 */
__STATIC_INLINE void LL_RCC_Reset_I2C2(void)
{
    LL_REG_SBIT(SN_SCU->APB1RST, SCU_APB1RST_I2C2RST);
}

/**
 * @brief  Return if I2C2 reset is done
 * @retval 1: Done;   0: Resetting
 */
__STATIC_INLINE uint32_t LL_RCC_IsActiveFlag_I2C2Reset_Done(void)
{
    return (((LL_REG_READ(SN_SCU->APB1RST) & SCU_APB1RST_I2C2RST) >> SCU_APB1RST_I2C2RST_Pos) == 0);
}

/**
 * @brief  reset CT16B0
 * @retval None
 */
__STATIC_INLINE void LL_RCC_Reset_CT16B0(void)
{
    LL_REG_SBIT(SN_SCU->APB1RST, SCU_APB1RST_CT16B0RST);
}

/**
 * @brief  Return if CT16B0 reset is done
 * @retval 1: Done;   0: Resetting
 */
__STATIC_INLINE uint32_t LL_RCC_IsActiveFlag_CT16B0Reset_Done(void)
{
    return (((LL_REG_READ(SN_SCU->APB1RST) & SCU_APB1RST_CT16B0RST) >> SCU_APB1RST_CT16B0RST_Pos) == 0);
}

/**
 * @brief  reset CT16B1
 * @retval None
 */
__STATIC_INLINE void LL_RCC_Reset_CT16B1(void)
{
    LL_REG_SBIT(SN_SCU->APB1RST, SCU_APB1RST_CT16B1RST);
}

/**
 * @brief  Return if CT16B1 reset is done
 * @retval 1: Done;   0: Resetting
 */
__STATIC_INLINE uint32_t LL_RCC_IsActiveFlag_CT16B1Reset_Done(void)
{
    return (((LL_REG_READ(SN_SCU->APB1RST) & SCU_APB1RST_CT16B1RST) >> SCU_APB1RST_CT16B1RST_Pos) == 0);
}

/**
 * @brief  reset CT16B2
 * @retval None
 */
__STATIC_INLINE void LL_RCC_Reset_CT16B2(void)
{
    LL_REG_SBIT(SN_SCU->APB1RST, SCU_APB1RST_CT16B2RST);
}

/**
 * @brief  Return if CT16B2 reset is done
 * @retval 1: Done;   0: Resetting
 */
__STATIC_INLINE uint32_t LL_RCC_IsActiveFlag_CT16B2Reset_Done(void)
{
    return (((LL_REG_READ(SN_SCU->APB1RST) & SCU_APB1RST_CT16B2RST) >> SCU_APB1RST_CT16B2RST_Pos) == 0);
}

/**
 * @brief  reset CT16B3
 * @retval None
 */
__STATIC_INLINE void LL_RCC_Reset_CT16B3(void)
{
    LL_REG_SBIT(SN_SCU->APB1RST, SCU_APB1RST_CT16B3RST);
}

/**
 * @brief  Return if CT16B3 reset is done
 * @retval 1: Done;   0: Resetting
 */
__STATIC_INLINE uint32_t LL_RCC_IsActiveFlag_CT16B3Reset_Done(void)
{
    return (((LL_REG_READ(SN_SCU->APB1RST) & SCU_APB1RST_CT16B3RST) >> SCU_APB1RST_CT16B3RST_Pos) == 0);
}

/**
 * @brief  reset CT16B4
 * @retval None
 */
__STATIC_INLINE void LL_RCC_Reset_CT16B4(void)
{
    LL_REG_SBIT(SN_SCU->APB1RST, SCU_APB1RST_CT16B4RST);
}

/**
 * @brief  Return if CT16B4 reset is done
 * @retval 1: Done;   0: Resetting
 */
__STATIC_INLINE uint32_t LL_RCC_IsActiveFlag_CT16B4Reset_Done(void)
{
    return (((LL_REG_READ(SN_SCU->APB1RST) & SCU_APB1RST_CT16B4RST) >> SCU_APB1RST_CT16B4RST_Pos) == 0);
}

/**
 * @brief  reset CT16B5
 * @retval None
 */
__STATIC_INLINE void LL_RCC_Reset_CT16B5(void)
{
    LL_REG_SBIT(SN_SCU->APB1RST, SCU_APB1RST_CT16B5RST);
}

/**
 * @brief  Return if CT16B5 reset is done
 * @retval 1: Done;   0: Resetting
 */
__STATIC_INLINE uint32_t LL_RCC_IsActiveFlag_CT16B5Reset_Done(void)
{
    return (((LL_REG_READ(SN_SCU->APB1RST) & SCU_APB1RST_CT16B5RST) >> SCU_APB1RST_CT16B5RST_Pos) == 0);
}

/**
 * @brief  reset CT16B8
 * @retval None
 */
__STATIC_INLINE void LL_RCC_Reset_CT16B8(void)
{
    LL_REG_SBIT(SN_SCU->APB1RST, SCU_APB1RST_CT16B8RST);
}

/**
 * @brief  Return if CT16B8 reset is done
 * @retval 1: Done;   0: Resetting
 */
__STATIC_INLINE uint32_t LL_RCC_IsActiveFlag_CT16B8Reset_Done(void)
{
    return (((LL_REG_READ(SN_SCU->APB1RST) & SCU_APB1RST_CT16B8RST) >> SCU_APB1RST_CT16B8RST_Pos) == 0);
}

/**
 * @brief  reset CRC
 * @retval None
 */
__STATIC_INLINE void LL_RCC_Reset_CRC(void)
{
    LL_REG_SBIT(SN_SCU->APB1RST, SCU_APB1RST_CRCRST);
}

/**
 * @brief  Return if CRC reset is done
 * @retval 1: Done;   0: Resetting
 */
__STATIC_INLINE uint32_t LL_RCC_IsActiveFlag_CRCReset_Done(void)
{
    return (((LL_REG_READ(SN_SCU->APB1RST) & SCU_APB1RST_CRCRST) >> SCU_APB1RST_CRCRST_Pos) == 0);
}

/**
 * @brief  reset GPIO0
 * @retval None
 */
__STATIC_INLINE void LL_RCC_Reset_GPIO0(void)
{
    LL_REG_SBIT(SN_SCU->APB1RST, SCU_APB1RST_GPIO0RST);
}

/**
 * @brief  Return if GPIO0 reset is done
 * @retval 1: Done;   0: Resetting
 */
__STATIC_INLINE uint32_t LL_RCC_IsActiveFlag_GPIO0Reset_Done(void)
{
    return (((LL_REG_READ(SN_SCU->APB1RST) & SCU_APB1RST_GPIO0RST) >> SCU_APB1RST_GPIO0RST_Pos) == 0);
}

/**
 * @brief  reset GPIO1
 * @retval None
 */
__STATIC_INLINE void LL_RCC_Reset_GPIO1(void)
{
    LL_REG_SBIT(SN_SCU->APB1RST, SCU_APB1RST_GPIO1RST);
}

/**
 * @brief  Return if GPIO1 reset is done
 * @retval 1: Done;   0: Resetting
 */
__STATIC_INLINE uint32_t LL_RCC_IsActiveFlag_GPIO1Reset_Done(void)
{
    return (((LL_REG_READ(SN_SCU->APB1RST) & SCU_APB1RST_GPIO1RST) >> SCU_APB1RST_GPIO1RST_Pos) == 0);
}

/**
 * @brief  reset GPIO2
 * @retval None
 */
__STATIC_INLINE void LL_RCC_Reset_GPIO2(void)
{
    LL_REG_SBIT(SN_SCU->APB1RST, SCU_APB1RST_GPIO2RST);
}

/**
 * @brief  Return if GPIO2 reset is done
 * @retval 1: Done;   0: Resetting
 */
__STATIC_INLINE uint32_t LL_RCC_IsActiveFlag_GPIO2Reset_Done(void)
{
    return (((LL_REG_READ(SN_SCU->APB1RST) & SCU_APB1RST_GPIO2RST) >> SCU_APB1RST_GPIO2RST_Pos) == 0);
}

/**
 * @brief  reset GPIO3
 * @retval None
 */
__STATIC_INLINE void LL_RCC_Reset_GPIO3(void)
{
    LL_REG_SBIT(SN_SCU->APB1RST, SCU_APB1RST_GPIO3RST);
}

/**
 * @brief  Return if GPIO3 reset is done
 * @retval 1: Done;   0: Resetting
 */
__STATIC_INLINE uint32_t LL_RCC_IsActiveFlag_GPIO3Reset_Done(void)
{
    return (((LL_REG_READ(SN_SCU->APB1RST) & SCU_APB1RST_GPIO3RST) >> SCU_APB1RST_GPIO3RST_Pos) == 0);
}

/**
 * @brief  reset LCM
 * @retval None
 */
__STATIC_INLINE void LL_RCC_Reset_LCM(void)
{
    LL_REG_SBIT(SN_SCU->APB1RST, SCU_APB1RST_LCMRST);
}

/**
 * @brief  Return if LCM reset is done
 * @retval 1: Done;   0: Resetting
 */
__STATIC_INLINE uint32_t LL_RCC_IsActiveFlag_LCMReset_Done(void)
{
    return (((LL_REG_READ(SN_SCU->APB1RST) & SCU_APB1RST_LCMRST) >> SCU_APB1RST_LCMRST_Pos) == 0);
}

/**
 * @brief  Set LCM clock prescaler
 * @param  prescaler LCM clock prescaler, \ref rcc_lcm_prescaler
 * @retval None
 */
__STATIC_INLINE void LL_RCC_SetLCMPrescaler(uint8_t prescaler)
{
    LL_REG_CBIT(SN_SCU->PRECTRL, SCU_PRECTRL_LCMPRE);
    LL_REG_SBIT(SN_SCU->PRECTRL, prescaler);
}

/**
 * @brief  Get LCM prescaler
 * @retval CMP prescaler \ref rcc_lcm_prescaler
 *          0: LCMCLK / 1
 *          1: LCMCLK / 2
 *          2: LCMCLK / 4
 *          3: LCMCLK / 8
 *          4: LCMCLK / 16
 */
__STATIC_INLINE uint32_t LL_RCC_GetLCMPrescaler(void)
{
    return ((LL_REG_READ(SN_SCU->PRECTRL) & SCU_PRECTRL_LCMPRE) >> SCU_PRECTRL_LCMPRE_Pos);
}

/**
 * @brief  Set GPIO driving/sinking strength
 * @param  gpio_n which GPIO to select, \ref rcc_gpio_n
 * @param  gpio_pin which GPIO pin to select, \ref rcc_gpio_pin
 * @param  strength driving/sinking strength, \ref rcc_gpio_strength
 * @retval None
 */
__STATIC_INLINE void LL_RCC_SetGPIODrivingStrength(uint8_t gpio_n, uint8_t gpio_pin, uint8_t strength)
{
    LL_REG_CBIT(*(&SN_SCU->P0STR + SCU_PnSTR_GPIO_OFFSET(gpio_n, gpio_pin)), 0x03U << SCU_PnSTR_GPIO_PIN_OFFSET(gpio_n, gpio_pin));
    LL_REG_SBIT(*(&SN_SCU->P0STR + SCU_PnSTR_GPIO_OFFSET(gpio_n, gpio_pin)), strength << SCU_PnSTR_GPIO_PIN_OFFSET(gpio_n, gpio_pin));
}

/**
 * @brief Enable DMA0 Reset Mask
 * @retval None
 */
__STATIC_INLINE void LL_RCC_Enable_DMA0Reset_Mask(void)
{
    LL_REG_SBIT(SN_SCU->AHBRSTMSK, SCU_AHBRSTMSK_DMA0RSTMSK);
}

/**
 * @brief Disable DMA0 Reset Mask
 * @retval None
 */
__STATIC_INLINE void LL_RCC_Disable_DMA0Reset_Mask(void)
{
    LL_REG_CBIT(SN_SCU->AHBRSTMSK, SCU_AHBRSTMSK_DMA0RSTMSK);
}

/**
 * @brief  Return if enabled DMA0 Reset Mask
 * @retval 1: Enabled;   0: Disabled
 */
__STATIC_INLINE uint32_t LL_RCC_IsEnabled_DMA0Reset_Mask(void)
{
    return ((LL_REG_READ(SN_SCU->AHBRSTMSK) & SCU_AHBRSTMSK_DMA0RSTMSK) >> SCU_AHBRSTMSK_DMA0RSTMSK_Pos);
}

/**
 * @brief Enable DMA1 Reset Mask
 * @retval None
 */
__STATIC_INLINE void LL_RCC_Enable_DMA1Reset_Mask(void)
{
    LL_REG_SBIT(SN_SCU->AHBRSTMSK, SCU_AHBRSTMSK_DMA1RSTMSK);
}

/**
 * @brief Disable DMA1 Reset Mask
 * @retval None
 */
__STATIC_INLINE void LL_RCC_Disable_DMA1Reset_Mask(void)
{
    LL_REG_CBIT(SN_SCU->AHBRSTMSK, SCU_AHBRSTMSK_DMA1RSTMSK);
}

/**
 * @brief  Return if enabled DMA1 Reset Mask
 * @retval 1: Enabled;   0: Disabled
 */
__STATIC_INLINE uint32_t LL_RCC_IsEnabled_DMA1Reset_Mask(void)
{
    return ((LL_REG_READ(SN_SCU->AHBRSTMSK) & SCU_AHBRSTMSK_DMA1RSTMSK) >> SCU_AHBRSTMSK_DMA1RSTMSK_Pos);
}

/**
 * @brief Enable SDIO Reset Mask
 * @retval None
 */
__STATIC_INLINE void LL_RCC_Enable_SDIOReset_Mask(void)
{
    LL_REG_SBIT(SN_SCU->AHBRSTMSK, SCU_AHBRSTMSK_SDIORSTMSK);
}

/**
 * @brief Disable SDIO Reset Mask
 * @retval None
 */
__STATIC_INLINE void LL_RCC_Disable_SDIOReset_Mask(void)
{
    LL_REG_CBIT(SN_SCU->AHBRSTMSK, SCU_AHBRSTMSK_SDIORSTMSK);
}

/**
 * @brief  Return if enabled SDIO Reset Mask
 * @retval 1: Enabled;   0: Disabled
 */
__STATIC_INLINE uint32_t LL_RCC_IsEnabled_SDIOReset_Mask(void)
{
    return ((LL_REG_READ(SN_SCU->AHBRSTMSK) & SCU_AHBRSTMSK_SDIORSTMSK) >> SCU_AHBRSTMSK_SDIORSTMSK_Pos);
}

/**
 * @brief Enable ETH Reset Mask
 * @retval None
 */
__STATIC_INLINE void LL_RCC_Enable_ETHReset_Mask(void)
{
    LL_REG_SBIT(SN_SCU->AHBRSTMSK, SCU_AHBRSTMSK_ETHRSTMSK);
}

/**
 * @brief Disable ETH Reset Mask
 * @retval None
 */
__STATIC_INLINE void LL_RCC_Disable_ETHReset_Mask(void)
{
    LL_REG_CBIT(SN_SCU->AHBRSTMSK, SCU_AHBRSTMSK_ETHRSTMSK);
}

/**
 * @brief  Return if enabled ETH Reset Mask
 * @retval 1: Enabled;   0: Disabled
 */
__STATIC_INLINE uint32_t LL_RCC_IsEnabled_ETHReset_Mask(void)
{
    return ((LL_REG_READ(SN_SCU->AHBRSTMSK) & SCU_AHBRSTMSK_ETHRSTMSK) >> SCU_AHBRSTMSK_ETHRSTMSK_Pos);
}

/**
 * @brief Enable WDT Reset Mask
 * @retval None
 */
__STATIC_INLINE void LL_RCC_Enable_WDTReset_Mask(void)
{
    LL_REG_SBIT(SN_SCU->APB0RSTMSK, SCU_APB0RSTMSK_WDTRSTMSK);
}

/**
 * @brief Disable WDT Reset Mask
 * @retval None
 */
__STATIC_INLINE void LL_RCC_Disable_WDTReset_Mask(void)
{
    LL_REG_CBIT(SN_SCU->APB0RSTMSK, SCU_APB0RSTMSK_WDTRSTMSK);
}

/**
 * @brief  Return if enabled WDT Reset Mask
 * @retval 1: Enabled;   0: Disabled
 */
__STATIC_INLINE uint32_t LL_RCC_IsEnabled_WDTReset_Mask(void)
{
    return ((LL_REG_READ(SN_SCU->APB0RSTMSK) & SCU_APB0RSTMSK_WDTRSTMSK) >> SCU_APB0RSTMSK_WDTRSTMSK_Pos);
}

/**
 * @brief Enable WWDT Reset Mask
 * @retval None
 */
__STATIC_INLINE void LL_RCC_Enable_WWDTReset_Mask(void)
{
    LL_REG_SBIT(SN_SCU->APB0RSTMSK, SCU_APB0RSTMSK_WWDTRSTMSK);
}

/**
 * @brief Disable WWDT Reset Mask
 * @retval None
 */
__STATIC_INLINE void LL_RCC_Disable_WWDTReset_Mask(void)
{
    LL_REG_CBIT(SN_SCU->APB0RSTMSK, SCU_APB0RSTMSK_WWDTRSTMSK);
}

/**
 * @brief  Return if enabled WWDT Reset Mask
 * @retval 1: Enabled;   0: Disabled
 */
__STATIC_INLINE uint32_t LL_RCC_IsEnabled_WWDTReset_Mask(void)
{
    return ((LL_REG_READ(SN_SCU->APB0RSTMSK) & SCU_APB0RSTMSK_WWDTRSTMSK) >> SCU_APB0RSTMSK_WWDTRSTMSK_Pos);
}

/**
 * @brief Enable CAN0 Reset Mask
 * @retval None
 */
__STATIC_INLINE void LL_RCC_Enable_CAN0Reset_Mask(void)
{
    LL_REG_SBIT(SN_SCU->APB0RSTMSK, SCU_APB0RSTMSK_CAN0RSTMSK);
}

/**
 * @brief Disable CAN0 Reset Mask
 * @retval None
 */
__STATIC_INLINE void LL_RCC_Disable_CAN0Reset_Mask(void)
{
    LL_REG_CBIT(SN_SCU->APB0RSTMSK, SCU_APB0RSTMSK_CAN0RSTMSK);
}

/**
 * @brief  Return if enabled CAN0 Reset Mask
 * @retval 1: Enabled;   0: Disabled
 */
__STATIC_INLINE uint32_t LL_RCC_IsEnabled_CAN0Reset_Mask(void)
{
    return ((LL_REG_READ(SN_SCU->APB0RSTMSK) & SCU_APB0RSTMSK_CAN0RSTMSK) >> SCU_APB0RSTMSK_CAN0RSTMSK_Pos);
}

/**
 * @brief Enable CAN1 Reset Mask
 * @retval None
 */
__STATIC_INLINE void LL_RCC_Enable_CAN1Reset_Mask(void)
{
    LL_REG_SBIT(SN_SCU->APB0RSTMSK, SCU_APB0RSTMSK_CAN1RSTMSK);
}

/**
 * @brief Disable CAN1 Reset Mask
 * @retval None
 */
__STATIC_INLINE void LL_RCC_Disable_CAN1Reset_Mask(void)
{
    LL_REG_CBIT(SN_SCU->APB0RSTMSK, SCU_APB0RSTMSK_CAN1RSTMSK);
}

/**
 * @brief  Return if enabled CAN1 Reset Mask
 * @retval 1: Enabled;   0: Disabled
 */
__STATIC_INLINE uint32_t LL_RCC_IsEnabled_CAN1Reset_Mask(void)
{
    return ((LL_REG_READ(SN_SCU->APB0RSTMSK) & SCU_APB0RSTMSK_CAN1RSTMSK) >> SCU_APB0RSTMSK_CAN1RSTMSK_Pos);
}

/**
 * @brief Enable UART1 Reset Mask
 * @retval None
 */
__STATIC_INLINE void LL_RCC_Enable_UART1Reset_Mask(void)
{
    LL_REG_SBIT(SN_SCU->APB0RSTMSK, SCU_APB0RSTMSK_UART1RSTMSK);
}

/**
 * @brief Disable UART1 Reset Mask
 * @retval None
 */
__STATIC_INLINE void LL_RCC_Disable_UART1Reset_Mask(void)
{
    LL_REG_CBIT(SN_SCU->APB0RSTMSK, SCU_APB0RSTMSK_UART1RSTMSK);
}

/**
 * @brief  Return if enabled UART1 Reset Mask
 * @retval 1: Enabled;   0: Disabled
 */
__STATIC_INLINE uint32_t LL_RCC_IsEnabled_UART1Reset_Mask(void)
{
    return ((LL_REG_READ(SN_SCU->APB0RSTMSK) & SCU_APB0RSTMSK_UART1RSTMSK) >> SCU_APB0RSTMSK_UART1RSTMSK_Pos);
}

/**
 * @brief Enable UART2 Reset Mask
 * @retval None
 */
__STATIC_INLINE void LL_RCC_Enable_UART2Reset_Mask(void)
{
    LL_REG_SBIT(SN_SCU->APB0RSTMSK, SCU_APB0RSTMSK_UART2RSTMSK);
}

/**
 * @brief Disable UART2 Reset Mask
 * @retval None
 */
__STATIC_INLINE void LL_RCC_Disable_UART2Reset_Mask(void)
{
    LL_REG_CBIT(SN_SCU->APB0RSTMSK, SCU_APB0RSTMSK_UART2RSTMSK);
}

/**
 * @brief  Return if enabled UART2 Reset Mask
 * @retval 1: Enabled;   0: Disabled
 */
__STATIC_INLINE uint32_t LL_RCC_IsEnabled_UART2Reset_Mask(void)
{
    return ((LL_REG_READ(SN_SCU->APB0RSTMSK) & SCU_APB0RSTMSK_UART2RSTMSK) >> SCU_APB0RSTMSK_UART2RSTMSK_Pos);
}

/**
 * @brief Enable UART3 Reset Mask
 * @retval None
 */
__STATIC_INLINE void LL_RCC_Enable_UART3Reset_Mask(void)
{
    LL_REG_SBIT(SN_SCU->APB0RSTMSK, SCU_APB0RSTMSK_UART3RSTMSK);
}

/**
 * @brief Disable UART3 Reset Mask
 * @retval None
 */
__STATIC_INLINE void LL_RCC_Disable_UART3Reset_Mask(void)
{
    LL_REG_CBIT(SN_SCU->APB0RSTMSK, SCU_APB0RSTMSK_UART3RSTMSK);
}

/**
 * @brief  Return if enabled UART3 Reset Mask
 * @retval 1: Enabled;   0: Disabled
 */
__STATIC_INLINE uint32_t LL_RCC_IsEnabled_UART3Reset_Mask(void)
{
    return ((LL_REG_READ(SN_SCU->APB0RSTMSK) & SCU_APB0RSTMSK_UART3RSTMSK) >> SCU_APB0RSTMSK_UART3RSTMSK_Pos);
}

/**
 * @brief Enable UART4 Reset Mask
 * @retval None
 */
__STATIC_INLINE void LL_RCC_Enable_UART4Reset_Mask(void)
{
    LL_REG_SBIT(SN_SCU->APB0RSTMSK, SCU_APB0RSTMSK_UART4RSTMSK);
}

/**
 * @brief Disable UART4 Reset Mask
 * @retval None
 */
__STATIC_INLINE void LL_RCC_Disable_UART4Reset_Mask(void)
{
    LL_REG_CBIT(SN_SCU->APB0RSTMSK, SCU_APB0RSTMSK_UART4RSTMSK);
}

/**
 * @brief  Return if enabled UART4 Reset Mask
 * @retval 1: Enabled;   0: Disabled
 */
__STATIC_INLINE uint32_t LL_RCC_IsEnabled_UART4Reset_Mask(void)
{
    return ((LL_REG_READ(SN_SCU->APB0RSTMSK) & SCU_APB0RSTMSK_UART4RSTMSK) >> SCU_APB0RSTMSK_UART4RSTMSK_Pos);
}

/**
 * @brief Enable CT16B6 Reset Mask
 * @retval None
 */
__STATIC_INLINE void LL_RCC_Enable_CT16B6Reset_Mask(void)
{
    LL_REG_SBIT(SN_SCU->APB0RSTMSK, SCU_APB0RSTMSK_CT16B6RSTMSK);
}

/**
 * @brief Disable CT16B6 Reset Mask
 * @retval None
 */
__STATIC_INLINE void LL_RCC_Disable_CT16B6Reset_Mask(void)
{
    LL_REG_CBIT(SN_SCU->APB0RSTMSK, SCU_APB0RSTMSK_CT16B6RSTMSK);
}

/**
 * @brief  Return if enabled CT16B6 Reset Mask
 * @retval 1: Enabled;   0: Disabled
 */
__STATIC_INLINE uint32_t LL_RCC_IsEnabled_CT16B6Reset_Mask(void)
{
    return ((LL_REG_READ(SN_SCU->APB0RSTMSK) & SCU_APB0RSTMSK_CT16B6RSTMSK) >> SCU_APB0RSTMSK_CT16B6RSTMSK_Pos);
}

/**
 * @brief Enable CT16B7 Reset Mask
 * @retval None
 */
__STATIC_INLINE void LL_RCC_Enable_CT16B7Reset_Mask(void)
{
    LL_REG_SBIT(SN_SCU->APB0RSTMSK, SCU_APB0RSTMSK_CT16B7RSTMSK);
}

/**
 * @brief Disable CT16B7 Reset Mask
 * @retval None
 */
__STATIC_INLINE void LL_RCC_Disable_CT16B7Reset_Mask(void)
{
    LL_REG_CBIT(SN_SCU->APB0RSTMSK, SCU_APB0RSTMSK_CT16B7RSTMSK);
}

/**
 * @brief  Return if enabled CT16B7 Reset Mask
 * @retval 1: Enabled;   0: Disabled
 */
__STATIC_INLINE uint32_t LL_RCC_IsEnabled_CT16B7Reset_Mask(void)
{
    return ((LL_REG_READ(SN_SCU->APB0RSTMSK) & SCU_APB0RSTMSK_CT16B7RSTMSK) >> SCU_APB0RSTMSK_CT16B7RSTMSK_Pos);
}

/**
 * @brief Enable ADC0 Reset Mask
 * @retval None
 */
__STATIC_INLINE void LL_RCC_Enable_ADC0Reset_Mask(void)
{
    LL_REG_SBIT(SN_SCU->APB1RSTMSK, SCU_APB1RSTMSK_ADC0RSTMSK);
}

/**
 * @brief Disable ADC0 Reset Mask
 * @retval None
 */
__STATIC_INLINE void LL_RCC_Disable_ADC0Reset_Mask(void)
{
    LL_REG_CBIT(SN_SCU->APB1RSTMSK, SCU_APB1RSTMSK_ADC0RSTMSK);
}

/**
 * @brief  Return if enabled ADC0 Reset Mask
 * @retval 1: Enabled;   0: Disabled
 */
__STATIC_INLINE uint32_t LL_RCC_IsEnabled_ADC0Reset_Mask(void)
{
    return ((LL_REG_READ(SN_SCU->APB1RSTMSK) & SCU_APB1RSTMSK_ADC0RSTMSK) >> SCU_APB1RSTMSK_ADC0RSTMSK_Pos);
}

/**
 * @brief Enable SSP0 Reset Mask
 * @retval None
 */
__STATIC_INLINE void LL_RCC_Enable_SSP0Reset_Mask(void)
{
    LL_REG_SBIT(SN_SCU->APB1RSTMSK, SCU_APB1RSTMSK_SSP0RSTMSK);
}

/**
 * @brief Disable SSP0 Reset Mask
 * @retval None
 */
__STATIC_INLINE void LL_RCC_Disable_SSP0Reset_Mask(void)
{
    LL_REG_CBIT(SN_SCU->APB1RSTMSK, SCU_APB1RSTMSK_SSP0RSTMSK);
}

/**
 * @brief  Return if enabled SSP0 Reset Mask
 * @retval 1: Enabled;   0: Disabled
 */
__STATIC_INLINE uint32_t LL_RCC_IsEnabled_SSP0Reset_Mask(void)
{
    return ((LL_REG_READ(SN_SCU->APB1RSTMSK) & SCU_APB1RSTMSK_SSP0RSTMSK) >> SCU_APB1RSTMSK_SSP0RSTMSK_Pos);
}

/**
 * @brief Enable SSP1 Reset Mask
 * @retval None
 */
__STATIC_INLINE void LL_RCC_Enable_SSP1Reset_Mask(void)
{
    LL_REG_SBIT(SN_SCU->APB1RSTMSK, SCU_APB1RSTMSK_SSP1RSTMSK);
}

/**
 * @brief Disable SSP1 Reset Mask
 * @retval None
 */
__STATIC_INLINE void LL_RCC_Disable_SSP1Reset_Mask(void)
{
    LL_REG_CBIT(SN_SCU->APB1RSTMSK, SCU_APB1RSTMSK_SSP1RSTMSK);
}

/**
 * @brief  Return if enabled SSP1 Reset Mask
 * @retval 1: Enabled;   0: Disabled
 */
__STATIC_INLINE uint32_t LL_RCC_IsEnabled_SSP1Reset_Mask(void)
{
    return ((LL_REG_READ(SN_SCU->APB1RSTMSK) & SCU_APB1RSTMSK_SSP1RSTMSK) >> SCU_APB1RSTMSK_SSP1RSTMSK_Pos);
}

/**
 * @brief Enable SSP2 Reset Mask
 * @retval None
 */
__STATIC_INLINE void LL_RCC_Enable_SSP2Reset_Mask(void)
{
    LL_REG_SBIT(SN_SCU->APB1RSTMSK, SCU_APB1RSTMSK_SSP2RSTMSK);
}

/**
 * @brief Disable SSP2 Reset Mask
 * @retval None
 */
__STATIC_INLINE void LL_RCC_Disable_SSP2Reset_Mask(void)
{
    LL_REG_CBIT(SN_SCU->APB1RSTMSK, SCU_APB1RSTMSK_SSP2RSTMSK);
}

/**
 * @brief  Return if enabled SSP2 Reset Mask
 * @retval 1: Enabled;   0: Disabled
 */
__STATIC_INLINE uint32_t LL_RCC_IsEnabled_SSP2Reset_Mask(void)
{
    return ((LL_REG_READ(SN_SCU->APB1RSTMSK) & SCU_APB1RSTMSK_SSP2RSTMSK) >> SCU_APB1RSTMSK_SSP2RSTMSK_Pos);
}

/**
 * @brief Enable UART0 Reset Mask
 * @retval None
 */
__STATIC_INLINE void LL_RCC_Enable_UART0Reset_Mask(void)
{
    LL_REG_SBIT(SN_SCU->APB1RSTMSK, SCU_APB1RSTMSK_UART0RSTMSK);
}

/**
 * @brief Disable UART0 Reset Mask
 * @retval None
 */
__STATIC_INLINE void LL_RCC_Disable_UART0Reset_Mask(void)
{
    LL_REG_CBIT(SN_SCU->APB1RSTMSK, SCU_APB1RSTMSK_UART0RSTMSK);
}

/**
 * @brief  Return if enabled UART0 Reset Mask
 * @retval 1: Enabled;   0: Disabled
 */
__STATIC_INLINE uint32_t LL_RCC_IsEnabled_UART0Reset_Mask(void)
{
    return ((LL_REG_READ(SN_SCU->APB1RSTMSK) & SCU_APB1RSTMSK_UART0RSTMSK) >> SCU_APB1RSTMSK_UART0RSTMSK_Pos);
}

/**
 * @brief Enable UART5 Reset Mask
 * @retval None
 */
__STATIC_INLINE void LL_RCC_Enable_UART5Reset_Mask(void)
{
    LL_REG_SBIT(SN_SCU->APB1RSTMSK, SCU_APB1RSTMSK_UART5RSTMSK);
}

/**
 * @brief Disable UART5 Reset Mask
 * @retval None
 */
__STATIC_INLINE void LL_RCC_Disable_UART5Reset_Mask(void)
{
    LL_REG_CBIT(SN_SCU->APB1RSTMSK, SCU_APB1RSTMSK_UART5RSTMSK);
}

/**
 * @brief  Return if enabled UART5 Reset Mask
 * @retval 1: Enabled;   0: Disabled
 */
__STATIC_INLINE uint32_t LL_RCC_IsEnabled_UART5Reset_Mask(void)
{
    return ((LL_REG_READ(SN_SCU->APB1RSTMSK) & SCU_APB1RSTMSK_UART5RSTMSK) >> SCU_APB1RSTMSK_UART5RSTMSK_Pos);
}

/**
 * @brief Enable I2C0 Reset Mask
 * @retval None
 */
__STATIC_INLINE void LL_RCC_Enable_I2C0Reset_Mask(void)
{
    LL_REG_SBIT(SN_SCU->APB1RSTMSK, SCU_APB1RSTMSK_I2C0RSTMSK);
}

/**
 * @brief Disable I2C0 Reset Mask
 * @retval None
 */
__STATIC_INLINE void LL_RCC_Disable_I2C0Reset_Mask(void)
{
    LL_REG_CBIT(SN_SCU->APB1RSTMSK, SCU_APB1RSTMSK_I2C0RSTMSK);
}

/**
 * @brief  Return if enabled I2C0 Reset Mask
 * @retval 1: Enabled;   0: Disabled
 */
__STATIC_INLINE uint32_t LL_RCC_IsEnabled_I2C0Reset_Mask(void)
{
    return ((LL_REG_READ(SN_SCU->APB1RSTMSK) & SCU_APB1RSTMSK_I2C0RSTMSK) >> SCU_APB1RSTMSK_I2C0RSTMSK_Pos);
}

/**
 * @brief Enable I2C1 Reset Mask
 * @retval None
 */
__STATIC_INLINE void LL_RCC_Enable_I2C1Reset_Mask(void)
{
    LL_REG_SBIT(SN_SCU->APB1RSTMSK, SCU_APB1RSTMSK_I2C1RSTMSK);
}

/**
 * @brief Disable I2C1 Reset Mask
 * @retval None
 */
__STATIC_INLINE void LL_RCC_Disable_I2C1Reset_Mask(void)
{
    LL_REG_CBIT(SN_SCU->APB1RSTMSK, SCU_APB1RSTMSK_I2C1RSTMSK);
}

/**
 * @brief  Return if enabled I2C1 Reset Mask
 * @retval 1: Enabled;   0: Disabled
 */
__STATIC_INLINE uint32_t LL_RCC_IsEnabled_I2C1Reset_Mask(void)
{
    return ((LL_REG_READ(SN_SCU->APB1RSTMSK) & SCU_APB1RSTMSK_I2C1RSTMSK) >> SCU_APB1RSTMSK_I2C1RSTMSK_Pos);
}

/**
 * @brief Enable I2C2 Reset Mask
 * @retval None
 */
__STATIC_INLINE void LL_RCC_Enable_I2C2Reset_Mask(void)
{
    LL_REG_SBIT(SN_SCU->APB1RSTMSK, SCU_APB1RSTMSK_I2C2RSTMSK);
}

/**
 * @brief Disable I2C2 Reset Mask
 * @retval None
 */
__STATIC_INLINE void LL_RCC_Disable_I2C2Reset_Mask(void)
{
    LL_REG_CBIT(SN_SCU->APB1RSTMSK, SCU_APB1RSTMSK_I2C2RSTMSK);
}

/**
 * @brief  Return if enabled I2C2 Reset Mask
 * @retval 1: Enabled;   0: Disabled
 */
__STATIC_INLINE uint32_t LL_RCC_IsEnabled_I2C2Reset_Mask(void)
{
    return ((LL_REG_READ(SN_SCU->APB1RSTMSK) & SCU_APB1RSTMSK_I2C2RSTMSK) >> SCU_APB1RSTMSK_I2C2RSTMSK_Pos);
}

/**
 * @brief Enable CT16B0 Reset Mask
 * @retval None
 */
__STATIC_INLINE void LL_RCC_Enable_CT16B0Reset_Mask(void)
{
    LL_REG_SBIT(SN_SCU->APB1RSTMSK, SCU_APB1RSTMSK_CT16B0RSTMSK);
}

/**
 * @brief Disable CT16B0 Reset Mask
 * @retval None
 */
__STATIC_INLINE void LL_RCC_Disable_CT16B0Reset_Mask(void)
{
    LL_REG_CBIT(SN_SCU->APB1RSTMSK, SCU_APB1RSTMSK_CT16B0RSTMSK);
}

/**
 * @brief  Return if enabled CT16B0 Reset Mask
 * @retval 1: Enabled;   0: Disabled
 */
__STATIC_INLINE uint32_t LL_RCC_IsEnabled_CT16B0Reset_Mask(void)
{
    return ((LL_REG_READ(SN_SCU->APB1RSTMSK) & SCU_APB1RSTMSK_CT16B0RSTMSK) >> SCU_APB1RSTMSK_CT16B0RSTMSK_Pos);
}

/**
 * @brief Enable CT16B1 Reset Mask
 * @retval None
 */
__STATIC_INLINE void LL_RCC_Enable_CT16B1Reset_Mask(void)
{
    LL_REG_SBIT(SN_SCU->APB1RSTMSK, SCU_APB1RSTMSK_CT16B1RSTMSK);
}

/**
 * @brief Disable CT16B1 Reset Mask
 * @retval None
 */
__STATIC_INLINE void LL_RCC_Disable_CT16B1Reset_Mask(void)
{
    LL_REG_CBIT(SN_SCU->APB1RSTMSK, SCU_APB1RSTMSK_CT16B1RSTMSK);
}

/**
 * @brief  Return if enabled CT16B1 Reset Mask
 * @retval 1: Enabled;   0: Disabled
 */
__STATIC_INLINE uint32_t LL_RCC_IsEnabled_CT16B1Reset_Mask(void)
{
    return ((LL_REG_READ(SN_SCU->APB1RSTMSK) & SCU_APB1RSTMSK_CT16B1RSTMSK) >> SCU_APB1RSTMSK_CT16B1RSTMSK_Pos);
}

/**
 * @brief Enable CT16B2 Reset Mask
 * @retval None
 */
__STATIC_INLINE void LL_RCC_Enable_CT16B2Reset_Mask(void)
{
    LL_REG_SBIT(SN_SCU->APB1RSTMSK, SCU_APB1RSTMSK_CT16B2RSTMSK);
}

/**
 * @brief Disable CT16B2 Reset Mask
 * @retval None
 */
__STATIC_INLINE void LL_RCC_Disable_CT16B2Reset_Mask(void)
{
    LL_REG_CBIT(SN_SCU->APB1RSTMSK, SCU_APB1RSTMSK_CT16B2RSTMSK);
}

/**
 * @brief  Return if enabled CT16B2 Reset Mask
 * @retval 1: Enabled;   0: Disabled
 */
__STATIC_INLINE uint32_t LL_RCC_IsEnabled_CT16B2Reset_Mask(void)
{
    return ((LL_REG_READ(SN_SCU->APB1RSTMSK) & SCU_APB1RSTMSK_CT16B2RSTMSK) >> SCU_APB1RSTMSK_CT16B2RSTMSK_Pos);
}

/**
 * @brief Enable CT16B3 Reset Mask
 * @retval None
 */
__STATIC_INLINE void LL_RCC_Enable_CT16B3Reset_Mask(void)
{
    LL_REG_SBIT(SN_SCU->APB1RSTMSK, SCU_APB1RSTMSK_CT16B3RSTMSK);
}

/**
 * @brief Disable CT16B3 Reset Mask
 * @retval None
 */
__STATIC_INLINE void LL_RCC_Disable_CT16B3Reset_Mask(void)
{
    LL_REG_CBIT(SN_SCU->APB1RSTMSK, SCU_APB1RSTMSK_CT16B3RSTMSK);
}

/**
 * @brief  Return if enabled CT16B3 Reset Mask
 * @retval 1: Enabled;   0: Disabled
 */
__STATIC_INLINE uint32_t LL_RCC_IsEnabled_CT16B3Reset_Mask(void)
{
    return ((LL_REG_READ(SN_SCU->APB1RSTMSK) & SCU_APB1RSTMSK_CT16B3RSTMSK) >> SCU_APB1RSTMSK_CT16B3RSTMSK_Pos);
}

/**
 * @brief Enable CT16B4 Reset Mask
 * @retval None
 */
__STATIC_INLINE void LL_RCC_Enable_CT16B4Reset_Mask(void)
{
    LL_REG_SBIT(SN_SCU->APB1RSTMSK, SCU_APB1RSTMSK_CT16B4RSTMSK);
}

/**
 * @brief Disable CT16B4 Reset Mask
 * @retval None
 */
__STATIC_INLINE void LL_RCC_Disable_CT16B4Reset_Mask(void)
{
    LL_REG_CBIT(SN_SCU->APB1RSTMSK, SCU_APB1RSTMSK_CT16B4RSTMSK);
}

/**
 * @brief  Return if enabled CT16B4 Reset Mask
 * @retval 1: Enabled;   0: Disabled
 */
__STATIC_INLINE uint32_t LL_RCC_IsEnabled_CT16B4Reset_Mask(void)
{
    return ((LL_REG_READ(SN_SCU->APB1RSTMSK) & SCU_APB1RSTMSK_CT16B4RSTMSK) >> SCU_APB1RSTMSK_CT16B4RSTMSK_Pos);
}

/**
 * @brief Enable CT16B5 Reset Mask
 * @retval None
 */
__STATIC_INLINE void LL_RCC_Enable_CT16B5Reset_Mask(void)
{
    LL_REG_SBIT(SN_SCU->APB1RSTMSK, SCU_APB1RSTMSK_CT16B5RSTMSK);
}

/**
 * @brief Disable CT16B5 Reset Mask
 * @retval None
 */
__STATIC_INLINE void LL_RCC_Disable_CT16B5Reset_Mask(void)
{
    LL_REG_CBIT(SN_SCU->APB1RSTMSK, SCU_APB1RSTMSK_CT16B5RSTMSK);
}

/**
 * @brief  Return if enabled CT16B5 Reset Mask
 * @retval 1: Enabled;   0: Disabled
 */
__STATIC_INLINE uint32_t LL_RCC_IsEnabled_CT16B5Reset_Mask(void)
{
    return ((LL_REG_READ(SN_SCU->APB1RSTMSK) & SCU_APB1RSTMSK_CT16B5RSTMSK) >> SCU_APB1RSTMSK_CT16B5RSTMSK_Pos);
}

/**
 * @brief Enable CT16B8 Reset Mask
 * @retval None
 */
__STATIC_INLINE void LL_RCC_Enable_CT16B8Reset_Mask(void)
{
    LL_REG_SBIT(SN_SCU->APB1RSTMSK, SCU_APB1RSTMSK_CT16B8RSTMSK);
}

/**
 * @brief Disable CT16B8 Reset Mask
 * @retval None
 */
__STATIC_INLINE void LL_RCC_Disable_CT16B8Reset_Mask(void)
{
    LL_REG_CBIT(SN_SCU->APB1RSTMSK, SCU_APB1RSTMSK_CT16B8RSTMSK);
}

/**
 * @brief  Return if enabled CT16B8 Reset Mask
 * @retval 1: Enabled;   0: Disabled
 */
__STATIC_INLINE uint32_t LL_RCC_IsEnabled_CT16B8Reset_Mask(void)
{
    return ((LL_REG_READ(SN_SCU->APB1RSTMSK) & SCU_APB1RSTMSK_CT16B8RSTMSK) >> SCU_APB1RSTMSK_CT16B8RSTMSK_Pos);
}

/**
 * @brief Enable CRC Reset Mask
 * @retval None
 */
__STATIC_INLINE void LL_RCC_Enable_CRCReset_Mask(void)
{
    LL_REG_SBIT(SN_SCU->APB1RSTMSK, SCU_APB1RSTMSK_CRCRSTMSK);
}

/**
 * @brief Disable CRC Reset Mask
 * @retval None
 */
__STATIC_INLINE void LL_RCC_Disable_CRCReset_Mask(void)
{
    LL_REG_CBIT(SN_SCU->APB1RSTMSK, SCU_APB1RSTMSK_CRCRSTMSK);
}

/**
 * @brief  Return if enabled CRC Reset Mask
 * @retval 1: Enabled;   0: Disabled
 */
__STATIC_INLINE uint32_t LL_RCC_IsEnabled_CRCReset_Mask(void)
{
    return ((LL_REG_READ(SN_SCU->APB1RSTMSK) & SCU_APB1RSTMSK_CRCRSTMSK) >> SCU_APB1RSTMSK_CRCRSTMSK_Pos);
}

/**
 * @brief Enable GPIO0 Reset Mask
 * @retval None
 */
__STATIC_INLINE void LL_RCC_Enable_GPIO0Reset_Mask(void)
{
    LL_REG_SBIT(SN_SCU->APB1RSTMSK, SCU_APB1RSTMSK_GPIO0RSTMSK);
}

/**
 * @brief Disable GPIO0 Reset Mask
 * @retval None
 */
__STATIC_INLINE void LL_RCC_Disable_GPIO0Reset_Mask(void)
{
    LL_REG_CBIT(SN_SCU->APB1RSTMSK, SCU_APB1RSTMSK_GPIO0RSTMSK);
}

/**
 * @brief  Return if enabled GPIO0 Reset Mask
 * @retval 1: Enabled;   0: Disabled
 */
__STATIC_INLINE uint32_t LL_RCC_IsEnabled_GPIO0Reset_Mask(void)
{
    return ((LL_REG_READ(SN_SCU->APB1RSTMSK) & SCU_APB1RSTMSK_GPIO0RSTMSK) >> SCU_APB1RSTMSK_GPIO0RSTMSK_Pos);
}

/**
 * @brief Enable GPIO1 Reset Mask
 * @retval None
 */
__STATIC_INLINE void LL_RCC_Enable_GPIO1Reset_Mask(void)
{
    LL_REG_SBIT(SN_SCU->APB1RSTMSK, SCU_APB1RSTMSK_GPIO1RSTMSK);
}

/**
 * @brief Disable GPIO1 Reset Mask
 * @retval None
 */
__STATIC_INLINE void LL_RCC_Disable_GPIO1Reset_Mask(void)
{
    LL_REG_CBIT(SN_SCU->APB1RSTMSK, SCU_APB1RSTMSK_GPIO1RSTMSK);
}

/**
 * @brief  Return if enabled GPIO1 Reset Mask
 * @retval 1: Enabled;   0: Disabled
 */
__STATIC_INLINE uint32_t LL_RCC_IsEnabled_GPIO1Reset_Mask(void)
{
    return ((LL_REG_READ(SN_SCU->APB1RSTMSK) & SCU_APB1RSTMSK_GPIO1RSTMSK) >> SCU_APB1RSTMSK_GPIO1RSTMSK_Pos);
}

/**
 * @brief Enable GPIO2 Reset Mask
 * @retval None
 */
__STATIC_INLINE void LL_RCC_Enable_GPIO2Reset_Mask(void)
{
    LL_REG_SBIT(SN_SCU->APB1RSTMSK, SCU_APB1RSTMSK_GPIO2RSTMSK);
}

/**
 * @brief Disable GPIO2 Reset Mask
 * @retval None
 */
__STATIC_INLINE void LL_RCC_Disable_GPIO2Reset_Mask(void)
{
    LL_REG_CBIT(SN_SCU->APB1RSTMSK, SCU_APB1RSTMSK_GPIO2RSTMSK);
}

/**
 * @brief  Return if enabled GPIO2 Reset Mask
 * @retval 1: Enabled;   0: Disabled
 */
__STATIC_INLINE uint32_t LL_RCC_IsEnabled_GPIO2Reset_Mask(void)
{
    return ((LL_REG_READ(SN_SCU->APB1RSTMSK) & SCU_APB1RSTMSK_GPIO2RSTMSK) >> SCU_APB1RSTMSK_GPIO2RSTMSK_Pos);
}

/**
 * @brief Enable GPIO3 Reset Mask
 * @retval None
 */
__STATIC_INLINE void LL_RCC_Enable_GPIO3Reset_Mask(void)
{
    LL_REG_SBIT(SN_SCU->APB1RSTMSK, SCU_APB1RSTMSK_GPIO3RSTMSK);
}

/**
 * @brief Disable GPIO3 Reset Mask
 * @retval None
 */
__STATIC_INLINE void LL_RCC_Disable_GPIO3Reset_Mask(void)
{
    LL_REG_CBIT(SN_SCU->APB1RSTMSK, SCU_APB1RSTMSK_GPIO3RSTMSK);
}

/**
 * @brief  Return if enabled GPIO3 Reset Mask
 * @retval 1: Enabled;   0: Disabled
 */
__STATIC_INLINE uint32_t LL_RCC_IsEnabled_GPIO3Reset_Mask(void)
{
    return ((LL_REG_READ(SN_SCU->APB1RSTMSK) & SCU_APB1RSTMSK_GPIO3RSTMSK) >> SCU_APB1RSTMSK_GPIO3RSTMSK_Pos);
}

/**
 * @brief Enable LCM Reset Mask
 * @retval None
 */
__STATIC_INLINE void LL_RCC_Enable_LCMReset_Mask(void)
{
    LL_REG_SBIT(SN_SCU->APB1RSTMSK, SCU_APB1RSTMSK_LCMRSTMSK);
}

/**
 * @brief Disable LCM Reset Mask
 * @retval None
 */
__STATIC_INLINE void LL_RCC_Disable_LCMReset_Mask(void)
{
    LL_REG_CBIT(SN_SCU->APB1RSTMSK, SCU_APB1RSTMSK_LCMRSTMSK);
}

/**
 * @brief  Return if enabled LCM Reset Mask
 * @retval 1: Enabled;   0: Disabled
 */
__STATIC_INLINE uint32_t LL_RCC_IsEnabled_LCMReset_Mask(void)
{
    return ((LL_REG_READ(SN_SCU->APB1RSTMSK) & SCU_APB1RSTMSK_LCMRSTMSK) >> SCU_APB1RSTMSK_LCMRSTMSK_Pos);
}

/**
 * @brief Enable SRAM automatic nap function
 * @param sram_n which SRAM to select, \ref rcc_sram_n_ctrl
 * @retval None
 */
__STATIC_INLINE void LL_RCC_Enable_SRAMAutoNapFunction(uint8_t sram_n)
{
    LL_REG_SBIT(*(&SN_SCU->SRAM1CTRL + sram_n), SCU_SRAM1CTRL_AUTONAPEN);
}

/**
 * @brief Disbale SRAM automatic nap function
 * @param sram_n which SRAM to select, \ref rcc_sram_n_ctrl
 * @retval None
 */
__STATIC_INLINE void LL_RCC_Disable_SRAMAutoNapFunction(uint8_t sram_n)
{
    LL_REG_CBIT(*(&SN_SCU->SRAM1CTRL + sram_n), SCU_SRAM1CTRL_AUTONAPEN);
}

/**
 * @brief  Return if enabled SRAM automatic nap function
 * @param sram_n which SRAM to select, \ref rcc_sram_n_ctrl
 * @retval 1: Enabled;   0: Disabled
 */
__STATIC_INLINE uint32_t LL_RCC_IsEnabled_SRAMAutoNapFuntion(uint8_t sram_n)
{
    return ((LL_REG_READ(*(&SN_SCU->SRAM1CTRL + sram_n)) & SCU_SRAM1CTRL_AUTONAPEN) >> SCU_SRAM1CTRL_AUTONAPEN_Pos);
}

/**
 * @brief Enable SRAM automatic retention function
 * @param sram_n which SRAM to select, \ref rcc_sram_n_ctrl
 * @retval None
 */
__STATIC_INLINE void LL_RCC_Enable_SRAMAutoRetentionFunction(uint8_t sram_n)
{
    LL_REG_SBIT(*(&SN_SCU->SRAM1CTRL + sram_n), SCU_SRAM1CTRL_AUTORETEN);
}

/**
 * @brief Disbale SRAM automatic retention function
 * @param sram_n which SRAM to select, \ref rcc_sram_n_ctrl
 * @retval None
 */
__STATIC_INLINE void LL_RCC_Disable_SRAMAutoRetentionFunction(uint8_t sram_n)
{
    LL_REG_CBIT(*(&SN_SCU->SRAM1CTRL + sram_n), SCU_SRAM1CTRL_AUTORETEN);
}

/**
 * @brief  Return if enabled SRAM automatic retention function
 * @param sram_n which SRAM to select, \ref rcc_sram_n_ctrl
 * @retval 1: Enabled;   0: Disabled
 */
__STATIC_INLINE uint32_t LL_RCC_IsEnabled_SRAMAutoRetentionFuntion(uint8_t sram_n)
{
    return ((LL_REG_READ(*(&SN_SCU->SRAM1CTRL + sram_n)) & SCU_SRAM1CTRL_AUTORETEN) >> SCU_SRAM1CTRL_AUTORETEN_Pos);
}

/**
 * @brief Enable SRAM automatic Sleep function
 * @param sram_n which SRAM to select, \ref rcc_sram_n_ctrl
 * @retval None
 */
__STATIC_INLINE void LL_RCC_Enable_SRAMAutoSleepFunction(uint8_t sram_n)
{
    LL_REG_SBIT(*(&SN_SCU->SRAM1CTRL + sram_n), SCU_SRAM1CTRL_AUTOSLPEN);
}

/**
 * @brief Disbale SRAM automatic Sleep function
 * @param sram_n which SRAM to select, \ref rcc_sram_n_ctrl
 * @retval None
 */
__STATIC_INLINE void LL_RCC_Disable_SRAMAutoSleepFunction(uint8_t sram_n)
{
    LL_REG_CBIT(*(&SN_SCU->SRAM1CTRL + sram_n), SCU_SRAM1CTRL_AUTOSLPEN);
}

/**
 * @brief  Return if enabled SRAM automatic Sleep function
 * @param sram_n which SRAM to select, \ref rcc_sram_n_ctrl
 * @retval 1: Enabled;   0: Disabled
 */
__STATIC_INLINE uint32_t LL_RCC_IsEnabled_SRAMAutoSleepFuntion(uint8_t sram_n)
{
    return ((LL_REG_READ(*(&SN_SCU->SRAM1CTRL + sram_n)) & SCU_SRAM1CTRL_AUTOSLPEN) >> SCU_SRAM1CTRL_AUTOSLPEN_Pos);
}

/**
 * @brief Set Resuming delay time for SRAM nap mode returns to normal mode
 * @param sram_n which SRAM to select, \ref rcc_sram_n_ctrl
 * @param delay_time Resuming delay time, range from 0 to 15
 * @retval None
 */
__STATIC_INLINE void LL_RCC_SetSRAMNapResumingDelayTime(uint8_t sram_n, uint8_t delay_time)
{
    LL_REG_CBIT(*(&SN_SCU->SRAM1CTRL + sram_n), SCU_SRAM1CTRL_NAPRESDLY);
    LL_REG_SBIT(*(&SN_SCU->SRAM1CTRL + sram_n), delay_time << SCU_SRAM1CTRL_NAPRESDLY_Pos);
}

/**
 * @brief  Get Resuming delay time for SRAM nap mode returns to normal mode
 * @param sram_n which SRAM to select, \ref rcc_sram_n_ctrl
 * @retval nap delay time
 */
__STATIC_INLINE uint32_t LL_RCC_GetSRAMNapResumingDelayTime(uint8_t sram_n)
{
    return ((LL_REG_READ(*(&SN_SCU->SRAM1CTRL + sram_n)) & SCU_SRAM1CTRL_NAPRESDLY) >> SCU_SRAM1CTRL_NAPRESDLY_Pos);
}

/**
 * @brief Set Resuming delay time for SRAM retention mode returns to normal mode
 * @param sram_n which SRAM to select, \ref rcc_sram_n_ctrl
 * @param delay_time Resuming delay time, range from 0 to 15
 * @retval None
 */
__STATIC_INLINE void LL_RCC_SetSRAMRetentionResumingDelayTime(uint8_t sram_n, uint8_t delay_time)
{
    LL_REG_CBIT(*(&SN_SCU->SRAM1CTRL + sram_n), SCU_SRAM1CTRL_RETRESDLY);
    LL_REG_SBIT(*(&SN_SCU->SRAM1CTRL + sram_n), delay_time << SCU_SRAM1CTRL_RETRESDLY_Pos);
}

/**
 * @brief  Get Resuming delay time for SRAM retention mode returns to normal mode
 * @param sram_n which SRAM to select, \ref rcc_sram_n_ctrl
 * @retval retention delay time
 */
__STATIC_INLINE uint32_t LL_RCC_GetSRAMRetentionResumingDelayTime(uint8_t sram_n)
{
    return ((LL_REG_READ(*(&SN_SCU->SRAM1CTRL + sram_n)) & SCU_SRAM1CTRL_RETRESDLY) >> SCU_SRAM1CTRL_RETRESDLY_Pos);
}

/**
 * @brief Set Resuming delay time for SRAM Sleep mode returns to normal mode
 * @param sram_n which SRAM to select, \ref rcc_sram_n_ctrl
 * @param delay_time Resuming delay time, range from 0 to 15
 * @retval None
 */
__STATIC_INLINE void LL_RCC_SetSRAMSleepResumingDelayTime(uint8_t sram_n, uint8_t delay_time)
{
    LL_REG_CBIT(*(&SN_SCU->SRAM1CTRL + sram_n), SCU_SRAM1CTRL_SLPRESDLY);
    LL_REG_SBIT(*(&SN_SCU->SRAM1CTRL + sram_n), delay_time << SCU_SRAM1CTRL_SLPRESDLY_Pos);
}

/**
 * @brief  Get Resuming delay time for SRAM Sleep mode returns to normal mode
 * @param sram_n which SRAM to select, \ref rcc_sram_n_ctrl
 * @retval Sleep delay time
 */
__STATIC_INLINE uint32_t LL_RCC_GetSRAMSleepResumingDelayTime(uint8_t sram_n)
{
    return ((LL_REG_READ(*(&SN_SCU->SRAM1CTRL + sram_n)) & SCU_SRAM1CTRL_SLPRESDLY) >> SCU_SRAM1CTRL_SLPRESDLY_Pos);
}

/**
 * @brief Set Time-out counter to trigger SRAM enter the power-saving mode
 * @param sram_n which SRAM to select, \ref rcc_sram_n_ctrl
 * @param timeout timeout for counter, range from 0 to 4095
 * @retval None
 */
__STATIC_INLINE void LL_RCC_SetTimeoutTriggerSRAMEnterPowerSaving(uint8_t sram_n, uint16_t timeout)
{
    LL_REG_CBIT(*(&SN_SCU->SRAM1CTRL + sram_n), SCU_SRAM1CTRL_TIMEOUT);
    LL_REG_SBIT(*(&SN_SCU->SRAM1CTRL + sram_n), timeout << SCU_SRAM1CTRL_TIMEOUT_Pos);
}

/**
 * @brief  Enable  External high-speed oscillator
 * @retval None.
 */
__STATIC_INLINE void LL_RCC_Enable_EHS(void)
{
    LL_REG_SBIT(SN_ALWAYSON->OSCCTL, ALWAYSON_OSCCTL_EHSEN);
}

/**
 * @brief  Disable  External high-speed oscillator
 * @retval None.
 */
__STATIC_INLINE void LL_RCC_Disable_EHS(void)
{
    LL_REG_CBIT(SN_ALWAYSON->OSCCTL, ALWAYSON_OSCCTL_EHSEN);
}

/**
 * @brief  Determine if  External high-speed oscillator is enabled
 * @retval  External high-speed oscillator enable state.
 */
__STATIC_INLINE uint32_t LL_RCC_IsEnabled_EHS(void)
{
    return (LL_REG_RBIT(SN_ALWAYSON->OSCCTL, ALWAYSON_OSCCTL_EHSEN) == ALWAYSON_OSCCTL_EHSEN);
}

/**
 * @brief  Enable internal resistor. EHS with internal resistor
 * @retval None.
 */
__STATIC_INLINE void LL_RCC_Enable_EHS_XTAL(void)
{
    LL_REG_SBIT(SN_ALWAYSON->OSCCTL, ALWAYSON_OSCCTL_EHSINROFF);
}

/**
 * @brief  Disable internal resistor. EHS without internal resistor
 * @retval None.
 */
__STATIC_INLINE void LL_RCC_Disable_EHS_XTAL(void)
{
    LL_REG_CBIT(SN_ALWAYSON->OSCCTL, ALWAYSON_OSCCTL_EHSINROFF);
}

/**
 * @brief  Determine if  Internal resistor of EHS XTAL is enabled
 * @retval  Internal resistor of EHS XTAL enable state.
 */
__STATIC_INLINE uint32_t LL_RCC_IsEnabled_EHS_XTAL(void)
{
    return (LL_REG_RBIT(SN_ALWAYSON->OSCCTL, ALWAYSON_OSCCTL_EHSINROFF) == ALWAYSON_OSCCTL_EHSINROFF);
}

/**
 * @brief  Check EHS XTAL ready
 * @retval EHS XTAL ready flag.
 */
__STATIC_INLINE uint32_t LL_RCC_IsActiveFlag_EHS(void)
{
    return (LL_REG_RBIT(SN_ALWAYSON->OSCRDY, ALWAYSON_OSCRDY_EHSRDY) == ALWAYSON_OSCRDY_EHSRDY);
}

/**
 * @brief  Enable  External low-speed oscillator
 * @retval None.
 */
__STATIC_INLINE void LL_RCC_Enable_ELS(void)
{
    LL_REG_SBIT(SN_ALWAYSON->POR_MISC, ALWAYSON_POR_MISC_ELSEN);
}

/**
 * @brief  Disable  External low-speed oscillator
 * @retval None.
 */
__STATIC_INLINE void LL_RCC_Disable_ELS(void)
{
    LL_REG_CBIT(SN_ALWAYSON->POR_MISC, ALWAYSON_POR_MISC_ELSEN);
}

/**
 * @brief  Determine if  External low-speed oscillator is enabled
 * @retval  External low-speed oscillator enable state.
 */
__STATIC_INLINE uint32_t LL_RCC_IsEnabled_ELS(void)
{
    return (LL_REG_RBIT(SN_ALWAYSON->POR_MISC, ALWAYSON_POR_MISC_ELSEN) == ALWAYSON_POR_MISC_ELSEN);
}

/**
 * @brief  Enable  External low-speed oscillator clock filter control
 * @retval None.
 */
__STATIC_INLINE void LL_RCC_Enable_ELSFL(void)
{
    LL_REG_CBIT(SN_ALWAYSON->POR_MISC, ALWAYSON_POR_MISC_ELSFLOFF);
}

/**
 * @brief  Disable  External low-speed oscillator clock filter control
 * @retval None.
 */
__STATIC_INLINE void LL_RCC_Disable_ELSFL(void)
{
    LL_REG_SBIT(SN_ALWAYSON->POR_MISC, ALWAYSON_POR_MISC_ELSFLOFF);
}

/**
 * @brief  Determine if  External low-speed oscillator clock filter control is enabled
 * @retval  External low-speed oscillator clock filter control enable state.
 */
__STATIC_INLINE uint32_t LL_RCC_IsEnabled_ELSFL(void)
{
    return (LL_REG_RBIT(SN_ALWAYSON->POR_MISC, ALWAYSON_POR_MISC_ELSFLOFF) == ALWAYSON_POR_MISC_ELSFLOFF);
}

/**
 * @brief  Check ELS XTAL ready
 * @retval ELS XTAL ready flag.
 */
__STATIC_INLINE uint32_t LL_RCC_IsActiveFlag_ELS(void)
{
    return (LL_REG_RBIT(SN_ALWAYSON->POR_MISC, ALWAYSON_POR_MISC_ELSRDY) == ALWAYSON_POR_MISC_ELSRDY);
}

/**
 * @brief  Set Frequency range of EHS XTAL
 * @param  range Frequency range of EHS XTAL @ref rcc_frequency_range
 * @retval none.
 */
__STATIC_INLINE void LL_RCC_SetFrequencyRangeOfEHS(uint32_t range)
{
    LL_REG_CBIT(SN_ALWAYSON->OSCMISC, ALWAYSON_OSCMISC_EHSFREQ);
    LL_REG_SBIT(SN_ALWAYSON->OSCMISC, range << ALWAYSON_OSCMISC_EHSFREQ_Pos);
}

/**
 * @brief  Get Frequency range of EHS XTAL
 * @retval Frequency range of EHS XTAL @ref rcc_frequency_range
 */
__STATIC_INLINE uint32_t LL_RCC_GetFrequencyRangeOfEHS(void)
{
    return ((LL_REG_READ(SN_ALWAYSON->OSCMISC) & ALWAYSON_OSCMISC_EHSFREQ) >> ALWAYSON_OSCMISC_EHSFREQ_Pos);
}

/**
 * @brief  Return if LVD reset occurred
 * @retval State of bit (1 or 0).
 */
__STATIC_INLINE uint32_t LL_RCC_IsActiveFlag_LVDReset(void)
{
    return (LL_REG_RBIT(SN_ALWAYSON->RSTST, ALWAYSON_RSTST_LVDF) == ALWAYSON_RSTST_LVDF);
}

/**
 * @brief  Clear status of LVD reset
 * @retval None.
 */
__STATIC_INLINE void LL_RCC_ClearFlag_LVDReset(void)
{
    LL_REG_WRITE(SN_ALWAYSON->RSTST, ALWAYSON_RSTST_LVDF);
}

/**
 * @brief  Set LVD reset/interrupt level
 * @param  level reset/interrupt level, \ref rcc_reset_interrupt_level
 * @retval None.
 */
__STATIC_INLINE void LL_RCC_SetResetAndInterruptLevel(uint32_t level)
{
    LL_REG_CBIT(SN_ALWAYSON->LVDCTRL, ALWAYSON_LVDCTRL_LVDLVL);
    LL_REG_SBIT(SN_ALWAYSON->LVDCTRL, level);
}

/**
 * @brief  Get LVD reset/interrupt level
 * @retval reset/interrupt level, \ref rcc_reset_interrupt_level
 */
__STATIC_INLINE uint32_t LL_RCC_GetResetAndInterruptLevel(void)
{
    return ((LL_REG_READ(SN_ALWAYSON->LVDCTRL) & ALWAYSON_LVDCTRL_LVDLVL) >> ALWAYSON_LVDCTRL_LVDLVL_Pos);
}

/**
 * @brief  Enable LVD Reset mode
 * @retval None.
 */
__STATIC_INLINE void LL_RCC_Enable_LVDResetMode(void)
{
    LL_REG_SBIT(SN_ALWAYSON->LVDCTRL, ALWAYSON_LVDCTRL_LVDRSTEN);
}

/**
 * @brief  Enable LVD Flag mode
 * @retval None.
 */
__STATIC_INLINE void LL_RCC_Enable_LVDFlagMode(void)
{
    LL_REG_CBIT(SN_ALWAYSON->LVDCTRL, ALWAYSON_LVDCTRL_LVDRSTEN);
}

/**
 * @brief  Judge if Enabled LVD Reset mode
 * @retval 1: Enabled, 0: Disabled
 */
__STATIC_INLINE uint32_t LL_RCC_IsEnabled_LVDResetMode(void)
{
    return ((LL_REG_READ(SN_ALWAYSON->LVDCTRL) & ALWAYSON_LVDCTRL_LVDRSTEN) >> ALWAYSON_LVDCTRL_LVDRSTEN_Pos);
}

/**
 * @brief  Enable LVD function
 * @retval None.
 * @note   Always disable in Deep sleep/Deep power-down mode
 */
__STATIC_INLINE void LL_RCC_Enable_LVDFunction(void)
{
    LL_REG_SBIT(SN_ALWAYSON->LVDCTRL, ALWAYSON_LVDCTRL_LVDEN);
}

/**
 * @brief  Disable LVD function
 * @retval None.
 * @note   Always disable in Deep sleep/Deep power-down mode
 */
__STATIC_INLINE void LL_RCC_Disable_LVDFunction(void)
{
    LL_REG_CBIT(SN_ALWAYSON->LVDCTRL, ALWAYSON_LVDCTRL_LVDEN);
}

/**
 * @brief  Judge if Enabled LVD funtion
 * @retval 1: Enabled, 0: Disabled
 */
__STATIC_INLINE uint32_t LL_RCC_IsEnabled_LVDFunction(void)
{
    return ((LL_REG_READ(SN_ALWAYSON->LVDCTRL) & ALWAYSON_LVDCTRL_LVDEN) >> ALWAYSON_LVDCTRL_LVDEN_Pos);
}

#if defined(USE_FULL_LL_DRIVER)
LL_Status_T LL_RCC_OscConfig(LL_RCC_OscConfig_t *OscCfg);
LL_Status_T LL_RCC_ClockConfig(LL_RCC_ClkConfig_t *ClkCfg);
void        LL_RCC_GetOscConfig(LL_RCC_OscConfig_t *OscCfg);
void        LL_RCC_GetClockConfig(LL_RCC_ClkConfig_t *ClkCfg);
void        LL_RCC_ClockoutConfig(uint32_t Src, uint32_t Prescale, uint32_t Clockout_Pin);

uint32_t LL_RCC_GetPLLCLKFreq(void);
uint32_t LL_RCC_GetSysClockFreq(void);
uint32_t LL_RCC_GetHCLKFreq(void);
uint32_t LL_RCC_GetAPB0CLKFreq(void);
uint32_t LL_RCC_GetAPB1CLKFreq(void);

void LL_LVD_Callback(void);
void LL_LVD_IRQHandler(void);
#endif /* USE_FULL_LL_DRIVER */

#ifdef __cplusplus
}
#endif

#endif /* _LL_RCC_H_ */
