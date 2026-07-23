/***********************
 * SN34F78X SDK V2.1.4 *
 ***********************/

#ifndef _LL_SYSTICK_H_
#define _LL_SYSTICK_H_

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "sn34f78x_ll_def.h"
#include "ll_reg_msk.h"

/* Exported types ------------------------------------------------------------*/
/**
 * \brief SysTick Frequency
 * \ingroup systick_struct_define
 */
typedef enum _LL_TickFreq_T
{
    LL_TICK_FREQ_10HZ    = 100U,              /**< SYSTICK frequency 10HZ  */
    LL_TICK_FREQ_100HZ   = 10U,               /**< SYSTICK frequency 100HZ */
    LL_TICK_FREQ_1KHZ    = 1U,                /**< SYSTICK frequency 1KHZ  */
    LL_TICK_FREQ_DEFAULT = LL_TICK_FREQ_1KHZ, /**< SYSTICK default        */
} LL_TickFreq_T;

/* Private types -------------------------------------------------------------*/

/* Exported functions --------------------------------------------------------*/

#if defined(USE_FULL_LL_DRIVER)
LL_Status_T LL_InitTick(uint32_t tick_priority);

/* I/O Operation functions ****************************************************/
void LL_IncTick(void);
void LL_Delay(uint32_t delay);

uint32_t LL_GetTick(void);
uint32_t LL_GetTickPrio(void);

LL_Status_T   LL_SetTickFreq(LL_TickFreq_T freq);
LL_TickFreq_T LL_GetTickFreq(void);

void LL_SuspendTick(void);
void LL_ResumeTick(void);
#endif /* USE_FULL_LL_DRIVER */

#ifdef __cplusplus
}
#endif

#endif
