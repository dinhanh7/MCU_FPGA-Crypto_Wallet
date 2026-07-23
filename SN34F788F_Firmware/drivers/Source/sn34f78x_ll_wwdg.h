/***********************
 * SN34F78X SDK V2.1.4 *
 ***********************/

#ifndef _SN34F78X_LL_WWDG_H_
#define _SN34F78X_LL_WWDG_H_

/* Includes ------------------------------------------------------------------*/
#include "sn34f78x_ll.h"

/* Private types -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Private constants ---------------------------------------------------------*/
/* Private macros ------------------------------------------------------------*/
#define IS_LL_WWDG_LOAD(__VALUE__)                                          (((__VALUE__ & LL_WWDG_TIMER_MAX) == __VALUE__) && (__VALUE__ != (LL_WWDG_TIMER_MIN - 1)))
#define IS_LL_WWDG_PRESCALER(__VALUE__)                                     ((__VALUE__ & LL_WWDG_PRESCALER_7) == __VALUE__)
#define IS_LL_WWDG_WIN_CONSTANT_LESS_TIMER_CONSTANT(__VALUE1__, __VALUE2__) (__VALUE1__ < __VALUE2__)

/* Private functions prototypes ----------------------------------------------*/

#endif /* _SN34F78X_LL_WWDG_H_ */
