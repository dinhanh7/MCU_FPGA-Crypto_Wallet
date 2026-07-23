/***********************
 * SN34F78X SDK V2.1.4 *
 ***********************/

#ifndef _HAL_SYSTICK_H_
#define _HAL_SYSTICK_H_

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "sn34f78x_hal_def.h"

/* Exported types ------------------------------------------------------------*/

/**
 * \brief SysTick Frequency
 * \ingroup systick_struct_define
 */
typedef enum _HAL_TickFreq_T
{
    HAL_TICK_FREQ_10HZ    = 100U,               /**< SYSTICK frequency 10HZ  */
    HAL_TICK_FREQ_100HZ   = 10U,                /**< SYSTICK frequency 100HZ */
    HAL_TICK_FREQ_1KHZ    = 1U,                 /**< SYSTICK frequency 1KHZ  */
    HAL_TICK_FREQ_DEFAULT = HAL_TICK_FREQ_1KHZ, /**< SYSTICK default        */
} HAL_TickFreq_T;

/* Exported constants --------------------------------------------------------*/

/* Exported functions --------------------------------------------------------*/
/* Initialization/de-initialization functions *********************************/
HAL_Status_T HAL_InitTick(uint32_t tick_priority);

/* I/O Operation functions ****************************************************/
void HAL_IncTick(void);
void HAL_Delay(uint32_t delay);

uint32_t HAL_GetTick(void);
uint32_t HAL_GetTickPrio(void);

HAL_Status_T   HAL_SetTickFreq(HAL_TickFreq_T freq);
HAL_TickFreq_T HAL_GetTickFreq(void);

void HAL_SuspendTick(void);
void HAL_ResumeTick(void);

#ifdef __cplusplus
}
#endif

#endif /* _HAL_SYSTICK_H_ */
