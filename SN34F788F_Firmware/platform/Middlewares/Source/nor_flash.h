/***********************
 * SN34F78X SDK V2.1.4 *
 ***********************/

#ifndef _SN34F78X_NORFLASH_H_
#define _SN34F78X_NORFLASH_H_

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "sn34f78x_hal.h"

HAL_Status_T NORFlash_Get_NewTimeout(uint32_t *tick_start, uint32_t old_timeout, uint32_t *new_timeout);

#ifdef __cplusplus
}
#endif

#endif /* _SN34F78X_NORFLASH_H_ */
