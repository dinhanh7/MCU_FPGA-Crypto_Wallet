/***********************
 * SN34F78X SDK V2.1.4 *
 ***********************/

#ifndef _SN34F78X_LL_DMA_H_
#define _SN34F78X_LL_DMA_H_

/* Includes ------------------------------------------------------------------*/
#include "sn34f78x_ll.h"

/* Private types -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Private constants ---------------------------------------------------------*/
/* Private macros ------------------------------------------------------------*/
#define IS_LL_DMA_INSTANCE(__VALUE__) ((__VALUE__) == SN_DMA0 || \
                                       (__VALUE__) == SN_DMA1)
#define IS_LL_DMA_CHANNEL(__VALUE__)    (__VALUE__ <= LL_DMA_CHANNEL_7)
#define IS_LL_DMA_XFER_MODE(__VALUE__)  (__VALUE__ <= LL_DMA_PERIPH_TO_PERIPH)
#define IS_LL_DMA_PRIORITY(__VALUE__)   (__VALUE__ <= LL_DMA_PRIORITY_VERY_HIGH)
#define IS_LL_DMA_FIFO_TH(__VALUE__)    (__VALUE__ <= LL_DMA_FIFO_TH_4)
#define IS_LL_DMA_SYNC(__VALUE__)       ((__VALUE__ == LL_ENABLE) || (__VALUE__ == LL_DISABLE))
#define IS_LL_DMA_BURST(__VALUE__)      (__VALUE__ <= LL_DMA_SRC_BURST_256)
#define IS_LL_DMA0_RS(__VALUE__)        (__VALUE__ <= LL_DMA_RS0_TPM7_MR0)
#define IS_LL_DMA1_RS(__VALUE__)        (__VALUE__ <= LL_DMA_RS1_TPM8_MR12)
#define IS_LL_DMA_WIDTH(__VALUE__)      (__VALUE__ <= LL_DMA_XFR_WIDTH_32BITS)
#define IS_LL_DMA_ADDR_MODE(__VALUE__)  (__VALUE__ <= LL_DMA_ADDR_MODE_CYCLIC)
#define IS_LL_DMA_AHB_MASTER(__VALUE__) (__VALUE__ <= LL_DMA_AHB_MASTER_PERIPH)

/* Private functions prototypes ----------------------------------------------*/

#endif /* _SN34F78X_LL_DMA_H_ */
