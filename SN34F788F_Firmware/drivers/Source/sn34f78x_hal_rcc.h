/***********************
 * SN34F78X SDK V2.1.4 *
 ***********************/

#ifndef _SN34F78X_HAL_RCC_H_
#define _SN34F78X_HAL_RCC_H_

/* Includes ------------------------------------------------------------------*/
#include "sn34f78x_hal.h"

/** @defgroup RCC_Reset_Control Reset Control
 * @brief  Reset control.
 * @note
 * @{
 */

#define __HAL_RCC_FCS_ENABLE() (SN_SCU->PWRMODE_b.FCS = ENABLE)

#define __HAL_RCC_PERIPHERAL_RESET()  (SN_SCU->PWRMODE_b.PERRST = ENABLE)
#define IS_RCC_PERIPHERAL_RESET_RDY() (SN_SCU->PWRMODE_b.PERRST == DISABLE)

#define __HAL_RCC_PLL_RESET_IN_FCS()     (SN_SCU->PWRMODE_b.FCS_PLLRSTOFF = DISABLE)
#define __HAL_RCC_PLL_RESET_OFF_IN_FCS() (SN_SCU->PWRMODE_b.FCS_PLLRSTOFF = ENABLE)

/* clock prescaler */
#define RCC_CLKPRE_WRPKEY_Pos (16U)
#define RCC_CLKPRE_WRPKEY_Msk (0xffffUL << RCC_CLKPRE_WRPKEY_Pos) /*!< 0xFFFF0000 */
#define RCC_CLKPRE_WRPKEY     0x5AFA0000

#define RCC_CLKPRE_LCD_Pos (13U)
#define RCC_CLKPRE_LCD_Msk (0x3UL << RCC_CLKPRE_LCD_Pos) /*!< 0x00006000 */

#define RCC_CLKPRE_CLKOUT_Pos (9U)
#define RCC_CLKPRE_CLKOUT_Msk (0xFUL << RCC_CLKPRE_CLKOUT_Pos) /*!< 0x00001E00 */

#define RCC_CLKPRE_APB1_Pos (6U)
#define RCC_CLKPRE_APB1_Msk (0x7UL << RCC_CLKPRE_APB1_Pos) /*!< 0x000001C0 */

#define RCC_CLKPRE_APB0_Pos (3U)
#define RCC_CLKPRE_APB0_Msk (0x7UL << RCC_CLKPRE_APB0_Pos) /*!< 0x00000038 */

#define RCC_CLKPRE_AHB_Pos (0U)
#define RCC_CLKPRE_AHB_Msk (0x7UL << RCC_CLKPRE_AHB_Pos) /*!< 0x00000007 */
/**
 * @}
 */

#define SCU_RIS_DS_WAKEUP_Pos (2U)
#define SCU_RIS_DS_WAKEUP_Msk (0x1UL << SCU_RIS_DS_WAKEUP_Pos) /*!< 0x00000002 */
#define SCU_RIS_DS_WAKEUP     SCU_RIS_DS_WAKEUP_Msk

#define IS_RCC_OSC_TYPE(x)   ((x) <= (RCC_OSC_TYPE_ELS | RCC_OSC_TYPE_EHS | RCC_OSC_TYPE_IHRC))
#define IS_RCC_OSCCLK_CFG(x) (((x) == RCC_OSCCLK_CFG_OFF) || ((x) == RCC_OSCCLK_CFG_ON))
#define IS_RCC_PLL_SRC(x)    (((x) == RCC_PLL_SRC_IHRC) || ((x) == RCC_PLL_SRC_EHS))
#define IS_RCC_PLL_NS(x)     (((x) >= 6) && ((x) <= 80))
#define IS_RCC_PLL_FS(x)     ((x) <= RCC_PLL_DIV4)

#define IS_RCC_CLK_TYPE(x)         ((x) <= (RCC_CLK_TYPE_SYSCLK | RCC_CLK_TYPE_HCLK | RCC_CLK_TYPE_APB0CLK | RCC_CLK_TYPE_APB1CLK))
#define IS_RCC_SYSCLK_SRC(x)       (((x) == RCC_SYSCLK_SRC_IHRC) || ((x) == RCC_SYSCLK_SRC_EHS) || ((x) == RCC_SYSCLK_SRC_PLL))
#define IS_RCC_HCLK_DIV(x)         ((x) <= RCC_SYSCLK_DIV64)
#define IS_RCC_APB0_APB1CLK_DIV(x) ((x) <= RCC_HCLK_DIV128)
#define IS_RCC_CLKOUT_SRC(x)       (((x) >= RCC_CLKOUT_SRC_DIV_ILRC) && ((x) <= RCC_CLKOUT_SRC_DIV_PLL) && (x) != 3)
#define IS_RCC_CLKOUT_DIV(x)       (((x) >= RCC_CLKOUT_PRE_DIV_1) && ((x) <= RCC_CLKOUT_PRE_DIV_128))
#define IS_RCC_CLKOUT_PIN(x)       (((x) >= RCC_CLKOUT_PIN_1) && ((x) <= RCC_CLKOUT_PIN_3))

void     RCC_FCS_Command(void);
uint32_t HAL_GetNewHCLKFreq(RCC_ClkConfig_t *ClkCfg);

#endif /* _SN34F78X_HAL_RCC_H_ */
