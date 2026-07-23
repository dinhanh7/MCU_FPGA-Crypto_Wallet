#ifndef __SONIX_APP_H__
#define __SONIX_APP_H__

#include "hal_systick.h"
#include "sn34f78x_hal.h"

extern SPI_Handle_T SPI0_Handle;

uint32_t init(void);
uint32_t uninit(void);
void     Error_Handler(void);
void     SystemClock_Config(void);
/* USER INC & DEF BEGIN */
#define TRANSFER_CNT 128
extern uint8_t master_out8[TRANSFER_CNT];
/* USER INC & DEF END */

#endif //__SONIX_APP_H__
