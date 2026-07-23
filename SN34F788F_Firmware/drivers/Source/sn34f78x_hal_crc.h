/***********************
 * SN34F78X SDK V2.1.4 *
 ***********************/

#ifndef _SN34F78X_HAL_CRC_H_
#define _SN34F78X_HAL_CRC_H_

/* Includes ------------------------------------------------------------------*/
#include "sn34f78x_hal.h"

/* Private types -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Private constants ---------------------------------------------------------*/

/* private macros ------------------------------------------------------------*/
// check whether CRC instance is correct
#define IS_PERIPHERAL_CRC_INSTANCE(handler) (((handler)->instance) == SN_CRC)

#define IS_CRC_POLY(poly) (((poly) == CRC_POLY_CRC_16_CCITT) || ((poly) == CRC_POLY_CRC_16) || ((poly) == CRC_POLY_CRC_32))

#endif /* _SN34F78X_HAL_CRC_H_ */
