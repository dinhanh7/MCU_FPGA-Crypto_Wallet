/**
 * @file sn34f7_ll_crc.h
 * @author PD
 * @brief Define the private structure, option for SN34F7 LL CRC.
 * @version 1.0
 * @date 2024-03-21
 *
 * @copyright Copyright (c) 2024
 *  *
 */
#ifndef _SN34F78X_LL_CRC_H_
#define _SN34F78X_LL_CRC_H_

/* Includes ------------------------------------------------------------------*/
#include "sn34f78x_ll.h"

/* Private types -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Private constants ---------------------------------------------------------*/
/* Private macros ------------------------------------------------------------*/
#define IS_LL_CRC_INSTANCE(__INSTANCE__) ((__INSTANCE__) == SN_CRC)

#define IS_LL_CRC_POLY(__VALUE__) (((__VALUE__) == LL_CRC_POLY_CRC_16_CCITT) || \
                                   ((__VALUE__) == LL_CRC_POLY_CRC_16) ||       \
                                   ((__VALUE__) == LL_CRC_POLY_CRC_32))

/* Private functions prototypes ----------------------------------------------*/

#endif /* _SN34F78X_LL_CRC_H_ */
