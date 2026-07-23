/***********************
 * SN34F78X SDK V2.1.4 *
 ***********************/

#ifndef _MIDDLEWARE_POWERMANAGER_H_
#define _MIDDLEWARE_POWERMANAGER_H_

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "sn34f78x_hal.h"

/* Exported types ------------------------------------------------------------*/
/**
 * @brief PowerManager Low Power Mode Structure Definition
 * \ingroup powermanager_struct_define
 */
typedef enum
{
    PM_SLEEP_MODE,     /**< Sleep mode                  */
    PM_DEEPSLEEP_MODE, /**< DeepSleep(STOP) mode        */
    PM_DPD_MODE,       /**< DeepPowerDown(STANDBY) mode */
} PM_LowPowerMode;

/**
 * @brief PowerManager GPIO IRQ Enumeration Definition
 * \ingroup powermanager_struct_define
 * \note GPIO interrupt trigger of edge
 */
typedef enum
{
    PM_GPIO_IRQ_RISING_EDGE,  /**< GPIO Wakeup IRQ: rising edge  */
    PM_GPIO_IRQ_FALLING_EDGE, /**< GPIO Wakeup IRQ: falling edge */
    PM_GPIO_IRQ_BOTH_EDGE,    /**< GPIO Wakeup IRQ: both edge    */
} PM_GPIO_IRQ;

/**
 * @brief PowerManager System Clock Parameter Structure Definition
 * \ingroup powermanager_struct_define
 */
typedef struct
{
    uint32_t ihrc_en;     /* Specifies IHRC enablement. This parameter can be a value of @ref HAL_ENABLE or @ref HAL_DISABLE. */
    uint32_t ehs_en;      /* Specifies EHS enablement. This parameter can be a value of @ref HAL_ENABLE or @ref HAL_DISABLE. */
    uint32_t els_en;      /* Specifies ELS enablement. This parameter can be a value of @ref HAL_ENABLE or @ref HAL_DISABLE. */
    uint32_t pll_src;     /* Specifies PLL clock source. This parameter can be a value of @ref pm_pll_src */
    uint32_t pll_ns;      /* Specifies PLL clock loop divider. This parameter can be a value between 6 and 80 */
    uint32_t pll_fs;      /* Specifies PLL clock frequency range output divider. This parameter can be a value of @ref pm_pll_fs */
    uint32_t sys_clk_src; /* Specifies System clock source. This parameter can be a value of @ref pm_sysclk_src */
    uint32_t ahb_div;     /* Specifies AHB clock divider. This parameter can be a value of @ref pm_ahbclk_div */
    uint32_t apb0_div;    /* Specifies APB0 clock divider. This parameter can be a value of @ref pm_apbclk_div */
    uint32_t apb1_div;    /* Specifies APB1 clock divider. This parameter can be a value of @ref pm_apbclk_div */
} PM_Clock_Config_T;

/**
 * @brief PowerManager Peripheral for Clock Adjusting Enumeration Definition
 * \ingroup powermanager_struct_define
 */
typedef enum
{
    PM_IP_I2C,  /**< I2C */
    PM_IP_UART, /**< UART */
    PM_IP_SSP,  /**< SSP */
    PM_IP_ADC,  /**< ADC */
    PM_IP_TPM,  /**< TPM */
    PM_IP_SD,   /**< SD */
} PM_CLKCFG_IP;

/**
 * @brief PowerManager BackUp Register Definition
 * \ingroup powermanager_struct_define
 */
typedef enum
{
    PM_BKUP_REG_0,  /**< RTC_BKP_DR0  */
    PM_BKUP_REG_1,  /**< RTC_BKP_DR1  */
    PM_BKUP_REG_2,  /**< RTC_BKP_DR2  */
    PM_BKUP_REG_3,  /**< RTC_BKP_DR3  */
    PM_BKUP_REG_4,  /**< RTC_BKP_DR4  */
    PM_BKUP_REG_5,  /**< RTC_BKP_DR5  */
    PM_BKUP_REG_6,  /**< RTC_BKP_DR6  */
    PM_BKUP_REG_7,  /**< RTC_BKP_DR7  */
    PM_BKUP_REG_8,  /**< RTC_BKP_DR8  */
    PM_BKUP_REG_9,  /**< RTC_BKP_DR9  */
    PM_BKUP_REG_10, /**< RTC_BKP_DR10 */
    PM_BKUP_REG_11, /**< RTC_BKP_DR11 */
    PM_BKUP_REG_12, /**< RTC_BKP_DR12 */
    PM_BKUP_REG_13, /**< RTC_BKP_DR13 */
    PM_BKUP_REG_14, /**< RTC_BKP_DR14 */
    PM_BKUP_REG_15, /**< RTC_BKP_DR15 */
    PM_BKUP_REG_16, /**< RTC_BKP_DR16 */
    PM_BKUP_REG_17, /**< RTC_BKP_DR17 */
    PM_BKUP_REG_18, /**< RTC_BKP_DR18 */
    PM_BKUP_REG_19, /**< RTC_BKP_DR19 */
} PM_BKUP_REG;

/**
 * @brief PowerManager GPIO Configuration Structure Definition
 * \ingroup powermanager_struct_define
 */
typedef struct
{
    uint32_t    pins; /**< GPIO pin mask */
    PM_GPIO_IRQ irq;  /**< GPIO interrupt trigger type */
} PM_GPIO_Config_T;

#if configUSE_POWERMANAGER_RTC_WAKEUP
/**
 * @brief PowerManager RTC Configuration Structure Definition
 * \ingroup powermanager_struct_define
 */
typedef struct
{
    RTC_Time_T    alarm_absolute; /**< Set absolute alarm time */
    uint32_t      alarm_relative; /**< Set relative alarm time (s) with "bin" format*/
    RTC_Handle_T *rtc_handle;     /**< RTC handle */
} PM_RTC_Config_T;
#endif

/**
 * @brief PowerManager Init Structure Definition
 * \ingroup powermanager_struct_define
 */
typedef struct
{
    PM_LowPowerMode low_power_mode; /**< Specials low power mode */
#if configUSE_POWERMANAGER_GPIO_WAKEUP
    PM_GPIO_Config_T gpio[4]; /**< Wakeup options for GPIO */
#endif
#if configUSE_POWERMANAGER_RTC_WAKEUP
    PM_RTC_Config_T rtc; /**< Wakeup options for RTC */
#endif
} PowerManager_Init_T;

/**
 * @brief PowerManager Handle Structure Definition
 * \ingroup powermanager_struct_define
 */
typedef struct
{
    PowerManager_Init_T init;  /**< PowerManager Required Parameters */
    __IO HAL_State_T    state; /**< PowerManager state */
} PowerManager_Handle_T;

/* Exported constants --------------------------------------------------------*/
/**
 * \defgroup pm_pll_src PLL Clock Source
 * \ingroup powermanager_control
 * @{
 */
#define PM_PLLSRC_NONE 0x000000FFU /**< Disable PLL */
#define PM_PLLSRC_IHRC 0x00000000U /**< PLL source: IHRC */
#define PM_PLLSRC_EHS  0x00000001U /**< PLL source: EHS */
/**
 * @}
 */

/**
 * \defgroup pm_pll_fs PLL Frequency Range Output Divider
 * \ingroup powermanager_control
 * @{
 */
#define PM_PLLFS_DIV32 0x00000000U /**< PLL CLKOUT = PLLCLK / 32 */
#define PM_PLLFS_DIV16 0x00000001U /**< PLL CLKOUT = PLLCLK / 16 */
#define PM_PLLFS_DIV8  0x00000002U /**< PLL CLKOUT = PLLCLK / 8 */
#define PM_PLLFS_DIV4  0x00000003U /**< PLL CLKOUT = PLLCLK / 4 */
/**
 * @}
 */

/**
 * \defgroup pm_sysclk_src System Clock Source
 * \ingroup powermanager_control
 * @{
 */
#define PM_SYSCLKSRC_IHRC 0x00000000U /**< SYSCLK source: IHRC */
#define PM_SYSCLKSRC_EHS  0x00000002U /**< SYSCLK source: EHS */
#define PM_SYSCLKSRC_PLL  0x00000004U /**< SYSCLK source: PLL */
/**
 * @}
 */

/**
 * \defgroup pm_ahbclk_div AHB Clock (HCLK) Divider
 * \ingroup powermanager_control
 * @{
 */
#define PM_AHBCLK_DIV1   0x00000000U /**< HCLK = SYSCLK / 1 */
#define PM_AHBCLK_DIV2   0x00000001U /**< HCLK = SYSCLK / 2 */
#define PM_AHBCLK_DIV4   0x00000002U /**< HCLK = SYSCLK / 4 */
#define PM_AHBCLK_DIV8   0x00000003U /**< HCLK = SYSCLK / 8 */
#define PM_AHBCLK_DIV16  0x00000004U /**< HCLK = SYSCLK / 16 */
#define PM_AHBCLK_DIV32  0x00000005U /**< HCLK = SYSCLK / 32 */
#define PM_AHBCLK_DIV64  0x00000006U /**< HCLK = SYSCLK / 64 */
#define PM_AHBCLK_DIV128 0x00000007U /**< HCLK = SYSCLK / 128 */
/**
 * @}
 */

/**
 * \defgroup pm_apbclk_div APB Clock (APB0 / APB1) Divider
 * \ingroup powermanager_control
 * @{
 */
#define PM_APBCLK_DIV1   0x00000000U /**< APBCLK = HCLK / 1 */
#define PM_APBCLK_DIV2   0x00000001U /**< APBCLK = HCLK / 2 */
#define PM_APBCLK_DIV4   0x00000002U /**< APBCLK = HCLK / 4 */
#define PM_APBCLK_DIV8   0x00000003U /**< APBCLK = HCLK / 8 */
#define PM_APBCLK_DIV16  0x00000004U /**< APBCLK = HCLK / 16 */
#define PM_APBCLK_DIV32  0x00000005U /**< APBCLK = HCLK / 32 */
#define PM_APBCLK_DIV64  0x00000006U /**< APBCLK = HCLK / 64 */
#define PM_APBCLK_DIV128 0x00000007U /**< APBCLK = HCLK / 128 */
/**
 * @}
 */

/* Exported macros -----------------------------------------------------------*/
#define CLK_MHz(n) ((uint32_t)(n * 1000 * 1000))
#define CLK_KHz(n) ((uint32_t)(n * 1000))

/* Exported functions --------------------------------------------------------*/
/* Initialization and de-initialization functions *****************************/
HAL_Status_T HAL_PM_Init(PowerManager_Handle_T *hpm);
HAL_Status_T HAL_PM_DeInit(PowerManager_Handle_T *hpm);

/* Control functions **********************************************************/
HAL_Status_T HAL_PM_EnterLowPowerMode(PowerManager_Handle_T *hpm);
HAL_Status_T HAL_PM_ConfigSystemClock(PM_Clock_Config_T *clk_cfg);
HAL_Status_T HAL_PM_GetPeripheralClock(PM_CLKCFG_IP ip, void *instance, uint32_t *clock);
HAL_Status_T HAL_PM_SetPeripheralClock(PM_CLKCFG_IP ip, void *instance, uint32_t clock);

/* I/O Operation functions ****************************************************/
HAL_Status_T HAL_PM_BKUPSRAM_Write(uint8_t *data, uint32_t size);
HAL_Status_T HAL_PM_BKUPSRAM_Read(uint8_t *data, uint32_t size);
HAL_Status_T HAL_PM_BKUPREG_Write(PM_BKUP_REG backup, uint32_t data);
uint32_t     HAL_PM_BKUPREG_Read(PM_BKUP_REG backup);

#ifdef __cplusplus
}
#endif

#endif // _MIDDLEWARE_POWERMANAGER_H_
