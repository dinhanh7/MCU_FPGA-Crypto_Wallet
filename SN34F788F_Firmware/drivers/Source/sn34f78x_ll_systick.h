/***********************
 * SN34F78X SDK V2.1.4 *
 ***********************/

#ifndef _SN34F78X_LL_SYSTICK_H_
#define _SN34F78X_LL_SYSTICK_H_

/* Includes ------------------------------------------------------------------*/
#include "sn34f78x_ll.h"

/* Private types -------------------------------------------------------------*/
extern uint32_t SystemCoreClock;
/* Private variables ---------------------------------------------------------*/
/* Private constants ---------------------------------------------------------*/
/* Private macros ------------------------------------------------------------*/
#define IS_LL_TICK_FREQ(FREQ) (((FREQ) == LL_TICK_FREQ_10HZ) ||  \
                               ((FREQ) == LL_TICK_FREQ_100HZ) || \
                               ((FREQ) == LL_TICK_FREQ_1KHZ))

/* Private functions prototypes ----------------------------------------------*/

#endif /* _SN34F78X_LL_SYSTICK_H_ */
