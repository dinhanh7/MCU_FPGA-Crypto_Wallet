/***********************
 * SN34F78X SDK V2.1.4 *
 ***********************/

#ifndef _SN34F78X_POWER_MANAGER_H_
#define _SN34F78X_POWER_MANAGER_H_

/* Includes ------------------------------------------------------------------*/
#include "sn34f78x_hal.h"

/* Private constants ---------------------------------------------------------*/
#define __ELS_FREQ (32768UL)
/* BackUp SRAM base address */
#define BKP_SRAM_BASE_ADDR (0x40051000)
/* BackUp SRAM MAX SIZE */
#define BKP_SRAM_SIZE_MAX (0x1000) // 4KB
/* Deviation multiple */
#define DEVIATION_MULTIPLE (20U) // 5%

/* Private macros ------------------------------------------------------------*/
#define PM_POWER_MODE_FALSE(low_power_mode, res) \
    if (low_power_mode > PM_DPD_MODE)            \
    {                                            \
        return res;                              \
    }

/* GPIO Mode */
#if (configUSE_POWERMANAGER_GPIO_WAKEUP == 1)
/* Define the driver / sinking pin mask */
#define GPIO0_STR_MASK 0x10FC07
#define GPIO1_STR_MASK 0xFFFF
#define GPIO2_STR_MASK 0xFFFF
#define GPIO3_STR_MASK 0x3FFF

#define GPIO_NORMAL_MODE   (0x1)
#define GPIO_NONE_CLK_MODE (0x0)

/* Check whether GPIO pin is correct */
#define CHECK_GPIO_PIN(INSTANCE_CNT, PIN)                                                                \
    ((((PIN & GPIO0_STR_MASK) != 0x00U) && ((PIN & ~GPIO0_STR_MASK) == 0x00U) && (INSTANCE_CNT == 0)) || \
     (((PIN & GPIO1_STR_MASK) != 0x00U) && ((PIN & ~GPIO1_STR_MASK) == 0x00U) && (INSTANCE_CNT == 1)) || \
     (((PIN & GPIO2_STR_MASK) != 0x00U) && ((PIN & ~GPIO2_STR_MASK) == 0x00U) && (INSTANCE_CNT == 2)) || \
     (((PIN & GPIO3_STR_MASK) != 0x00U) && ((PIN & ~GPIO3_STR_MASK) == 0x00U) && (INSTANCE_CNT == 3)))

#define __HAL_GPIO_CLK_ENABLE(port)         \
    do                                      \
    {                                       \
        if (port == SN_GPIO0)               \
        {                                   \
            __HAL_RCC_GPIO_P0_CLK_ENABLE(); \
        }                                   \
        else if (port == SN_GPIO1)          \
        {                                   \
            __HAL_RCC_GPIO_P1_CLK_ENABLE(); \
        }                                   \
        else if (port == SN_GPIO2)          \
        {                                   \
            __HAL_RCC_GPIO_P2_CLK_ENABLE(); \
        }                                   \
        else if (port == SN_GPIO3)          \
        {                                   \
            __HAL_RCC_GPIO_P3_CLK_ENABLE(); \
        }                                   \
    } while (0);

#define CHECK_GPIO_PIN_NONE(hpm) \
    ((hpm->init.gpio[0].pins == GPIO_PIN_NONE) && (hpm->init.gpio[1].pins == GPIO_PIN_NONE) && (hpm->init.gpio[2].pins == GPIO_PIN_NONE) && (hpm->init.gpio[3].pins == GPIO_PIN_NONE))
#else /* configUSE_POWERMANAGER_GPIO_WAKEUP */
#define CHECK_GPIO_PIN_NONE(hpm) 1
#endif /* configUSE_POWERMANAGER_GPIO_WAKEUP */

/* RTC Mode */
#if (configUSE_POWERMANAGER_RTC_WAKEUP == 1)
#define CHECK_RTC_HANDLE_NULL(hpm) (hpm->init.rtc.rtc_handle == NULL)
#else /* configUSE_POWERMANAGER_RTC_WAKEUP */
#define CHECK_RTC_HANDLE_NULL(hpm) 0
#endif /* configUSE_POWERMANAGER_RTC_WAKEUP */

#define IS_CLOCK_CONFIG_ENABLEMENT(__VALUE__) (((__VALUE__) == HAL_ENABLE) || ((__VALUE__) == HAL_DISABLE))
#define IS_CLOCK_CONFIG_PLL_SRC(__VALUE__)    (((__VALUE__) == PM_PLLSRC_NONE) || ((__VALUE__) == PM_PLLSRC_IHRC) || ((__VALUE__) == PM_PLLSRC_EHS))
#define IS_CLOCK_CONFIG_PLL_NS(__VALUE__)     (((__VALUE__) >= 6U) || ((__VALUE__) <= 80U))
#define IS_CLOCK_CONFIG_PLL_FS(__VALUE__)     ((__VALUE__) <= PM_PLLFS_DIV4)
#define IS_CLOCK_CONFIG_SYSCLK_SRC(__VALUE__) (((__VALUE__) == PM_SYSCLKSRC_IHRC) || ((__VALUE__) == PM_SYSCLKSRC_EHS) || ((__VALUE__) == PM_SYSCLKSRC_PLL))
#define IS_CLOCK_CONFIG_AHB_DIV(__VALUE__)    ((__VALUE__) <= PM_AHBCLK_DIV128)
#define IS_CLOCK_CONFIG_APB_DIV(__VALUE__)    ((__VALUE__) <= PM_APBCLK_DIV128)

/* Private types -------------------------------------------------------------*/
typedef struct
{
    uint32_t pll_clk;
    uint32_t ahb_clk;
    uint32_t apb0_clk;
    uint32_t apb1_clk;
} PM_Clock_Info_T;

typedef struct
{
    IRQn_Type    irq_no;
    uint32_t     wakeup_src;
    SN_GPIO_Type instance;
} GPIO_Wakeup_Info_T;

/* Private functions ---------------------------------------------------------*/
#if (configUSE_POWERMANAGER_GPIO_WAKEUP == 1U)
static HAL_Status_T _gpio_wakeup_init(uint8_t cnt);
static HAL_Status_T _set_gpio_mode(uint8_t mode);
#endif /* configUSE_POWERMANAGER_GPIO_WAKEUP */
#if (configUSE_POWERMANAGER_RTC_WAKEUP == 1U)
static HAL_Status_T _rtc_wakeup_irq(void);
static void         _rtc_set_relative_time(RTC_Time_T *alarm_time, uint8_t time_s);
#endif /* configUSE_POWERMANAGER_RTC_WAKEUP */
#if (configUSE_POWERMANAGER_CLOCK_CONFIG == 1U)
static HAL_Status_T _get_clock_i2c(void *instance, uint32_t *clock);
static HAL_Status_T _get_clock_uart(void *instance, uint32_t *clock);
static HAL_Status_T _get_clock_ssp(void *instance, uint32_t *clock);
static HAL_Status_T _get_clock_adc(void *instance, uint32_t *clock);
static HAL_Status_T _get_clock_tpm(void *instance, uint32_t *clock);
static HAL_Status_T _get_clock_sd(void *instance, uint32_t *clock);
static HAL_Status_T _set_clock_i2c(void *instance, uint32_t clock);
static HAL_Status_T _set_clock_uart(void *instance, uint32_t clock);
static HAL_Status_T _set_clock_ssp(void *instance, uint32_t clock);
static HAL_Status_T _set_clock_adc(void *instance, uint32_t clock);
static HAL_Status_T _set_clock_tpm(void *instance, uint32_t clock);
static HAL_Status_T _set_clock_sd(void *instance, uint32_t clock);
#endif /* configUSE_POWERMANAGER_CLOCK_CONFIG */

#endif /* _SN34F78X_POWER_MANAGER_H_ */
