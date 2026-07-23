/***********************
 * SN34F78X SDK V2.1.4 *
 ***********************/

#ifndef _SN34F78X_HAL_SYSTICK_H_
#define _SN34F78X_HAL_SYSTICK_H_

/* Includes ------------------------------------------------------------------*/
#include "sn34f78x_hal.h"

/* Private types -------------------------------------------------------------*/
extern uint32_t SystemCoreClock;
/* Private constants ---------------------------------------------------------*/

/* Private macros ------------------------------------------------------------*/
#define IS_TICK_FREQ(FREQ) (((FREQ) == HAL_TICK_FREQ_10HZ) ||  \
                            ((FREQ) == HAL_TICK_FREQ_100HZ) || \
                            ((FREQ) == HAL_TICK_FREQ_1KHZ))
/* Private functions prototypes ----------------------------------------------*/

#endif /* _SN34F78X_HAL_SYSTICK_H_ */
