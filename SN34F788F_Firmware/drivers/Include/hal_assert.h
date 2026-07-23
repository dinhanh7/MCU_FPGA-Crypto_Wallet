/***********************
 * SN34F78X SDK V2.1.4 *
 ***********************/

#ifndef _HAL_ASSERT_H_
#define _HAL_ASSERT_H_

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "sn34f78x_hal_def.h"

/* Exported functions --------------------------------------------------------*/

/* Assert Parameter functions  ********************************/
void AssertParaFalse(uint8_t *file, uint32_t line);

#ifdef __cplusplus
}
#endif

#endif /* _HAL_ASSERT_H_ */
