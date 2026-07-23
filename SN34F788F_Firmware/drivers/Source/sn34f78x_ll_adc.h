/***********************
 * SN34F78X SDK V2.1.4 *
 ***********************/

#ifndef _SN34F78X_LL_ADC_H_
#define _SN34F78X_LL_ADC_H_

/* Includes ------------------------------------------------------------------*/
#include "sn34f78x_ll.h"

/* Private types -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Private constants ---------------------------------------------------------*/
/* Private macros ------------------------------------------------------------*/

// check whether ADC instance is correct
#define IS_LL_ADC_INSTANCE(__INSTANCE__) ((__INSTANCE__) == SN_ADC0)
// check whether ADC converter mode is correct
#define IS_LL_ADC_CONV_MODE(conv_mode) ((conv_mode == LL_ADC_CONVMODE_SINGLE_STEP) || (conv_mode == LL_ADC_CONVMODE_SINGLE_SCAN) || (conv_mode == LL_ADC_CONVMODE_LOOP_SCAN))
// check whether ADC scan conversion channel numbers is correct
#define IS_LL_ADC_SCAN_NUM(scan_ch_num) (scan_ch_num <= 0x12U)
// check whether ADC AUTOPWDN is correct
#define IS_LL_ADC_AUTOPWDN(autopwdn) ((autopwdn == LL_DISABLE) || (autopwdn == LL_ENABLE))
// check whether ADC prescaler clock is correct
#define IS_LL_ADC_MCLK(pre_clock) ((pre_clock > 0) && (pre_clock <= 256))
// check whether ADC AUTOPWDN converter mode is correct
#define CHECK_AUTOPWDN_IN_SINGLE_MODE(conv_mode, autopwdn) ((conv_mode != LL_ADC_CONVMODE_LOOP_SCAN) || (autopwdn == LL_DISABLE))
// check whether ADC is continue scan
#define IS_LL_ADC_LOOP_SCAN(conv_mode) (conv_mode == LL_ADC_CONVMODE_LOOP_SCAN)

/* Private functions prototypes ----------------------------------------------*/

#endif /* _SN34F78X_LL_ADC_H_ */
