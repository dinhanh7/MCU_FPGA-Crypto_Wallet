/***********************
 * SN34F78X SDK V2.1.4 *
 ***********************/

#ifndef _SN34F78X_LL_RCC_H_
#define _SN34F78X_LL_RCC_H_

/* Includes ------------------------------------------------------------------*/
#include "sn34f78x_ll.h"

/* Private types -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/

/* Private constants ---------------------------------------------------------*/
/* Private macros ------------------------------------------------------------*/
#define IS_LL_RCC_OSC_TYPE(x)   ((x) <= (LL_RCC_OSC_TYPE_ELS | LL_RCC_OSC_TYPE_EHS | LL_RCC_OSC_TYPE_IHRC))
#define IS_LL_RCC_OSCCLK_CFG(x) (((x) == LL_RCC_OSCCLK_CFG_OFF) || ((x) == LL_RCC_OSCCLK_CFG_ON))
#define IS_LL_RCC_PLL_NS(x)     (((x) >= 6) && ((x) <= 80))
#define IS_LL_RCC_PLL_FS(x)     ((x) <= LL_RCC_PLL_DIV4)
#define IS_LL_RCC_CLK_TYPE(x)   ((x) <= (LL_RCC_CLK_TYPE_SYSCLK | LL_RCC_CLK_TYPE_HCLK | LL_RCC_CLK_TYPE_APB0CLK | LL_RCC_CLK_TYPE_APB1CLK))

#define IS_LL_RCC_PLL_SRC(x)    (((x) == LL_RCC_PLL_SRC_IHRC) || ((x) == LL_RCC_PLL_SRC_EHS))
#define IS_LL_RCC_SYSCLK_SRC(x) (((x) == LL_RCC_SYSCLK_SRC_IHRC) || ((x) == LL_RCC_SYSCLK_SRC_EHS) || ((x) == LL_RCC_SYSCLK_SRC_PLL))

#define IS_LL_RCC_HCLK_DIV(x)         ((x) <= LL_RCC_SYSCLK_DIV128)
#define IS_LL_RCC_APB0APB1_CLK_DIV(x) ((x) <= LL_RCC_HCLK_DIV128)

/* Private functions prototypes ----------------------------------------------*/
static void     _rcc_fcs_command(void);
static uint32_t _rcc_get_new_hclk_freq(LL_RCC_ClkConfig_t *ClkCfg);

#endif /* _SN34F78X_LL_RCC_H_ */
