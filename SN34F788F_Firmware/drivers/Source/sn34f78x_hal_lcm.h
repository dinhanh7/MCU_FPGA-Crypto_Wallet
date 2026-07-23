/***********************
 * SN34F78X SDK V2.1.4 *
 ***********************/

#ifndef _SN34F78X_HAL_LCM_H_
#define _SN34F78X_HAL_LCM_H_

/* Includes ------------------------------------------------------------------*/
#include "sn34f78x_hal.h"

/* Private types -------------------------------------------------------------*/

/* Private constants ---------------------------------------------------------*/
// function for LCM clock enable
#define LCM_APB1_CLKEN (1 << 21UL)

// function for LCM peripheral reset
#define LCM_APB1_RST (1 << 21UL)

// function for LCM enable
#define LCM_ENABLE_LCMEN (1 << 0UL)

// function for LCM DMA enable
#define LCM_ENABLE_DMAEN (1 << 1UL)

// function for LCM DMA enable
#define LCM_ENABLE_ALLIE (0x1FFUL)
#define LCM_CLEAR_ALLIS  (0x1FFUL)

// status for LCM interrupt during fifo mode
#define LCM_IS_IFMASK  (0x1FF)
#define LCM_IS_DMAERR  (0x1 << 8UL)
#define LCM_IS_UDRUN   (0x1 << 6UL)
#define LCM_IS_DATCNTE (0x1 << 1UL)

// clock speed
#define LCM_SRC_CLK_MAX 148000000UL
#define LCM_CLK_PRE     1UL

/* Private macros ------------------------------------------------------------*/
// check whether LCM instance is correct
#define IS_LCM_INSTANCE(HANDLE) (((HANDLE)->instance) == SN_LCM)

// check whether LCM panel type is correct
#define IS_LCM_PANEL(HANDLE) ((HANDLE->init.panel == LCM_PANEL_MONO) ||   \
                              (HANDLE->init.panel == LCM_PANEL_16BITS) || \
                              (HANDLE->init.panel == LCM_PANEL_18BITS))

// check whether LCM bus width is valid
#define IS_LCM_BUS_WIDTH(HANDLE) ((HANDLE->init.bus_width == LCM_BUS_WIDTH_8BITS) ||  \
                                  (HANDLE->init.bus_width == LCM_BUS_WIDTH_9BITS) ||  \
                                  (HANDLE->init.bus_width == LCM_BUS_WIDTH_16BITS) || \
                                  (HANDLE->init.bus_width == LCM_BUS_WIDTH_18BITS))
// check whether LCM transfer mode is valid
#define IS_LCM_XFER_MODE(HANDLE) ((HANDLE->init.xfer_mode == LCM_XFER_ONE_16BITS) || \
                                  (HANDLE->init.xfer_mode == LCM_XFER_18BITS_MSB) || \
                                  (HANDLE->init.xfer_mode == LCM_XFER_18BITS_LSB))

// check whether LCM callback id correct
#define IS_LCM_CALLBACK_ID(CBID) ((CBID) <= HAL_LCM_ALL_CB_ID)

// check LCM is idle
#define LCM_WAIT_RDY(HANDLE) while ((HANDLE)->instance->RDY != 0x1)
// check LCM is idle
#define LCM_WAIT_FIFO_CLR(HANDLE) while ((HANDLE)->instance->DMAFIFOCTRL_b.FIFOCLR == 0x1)

// by register CMD to send user cmd
#define LCM_SEND_CMD(HANDLE, cmd) HAL_REG_WRITE((HANDLE)->instance->CMD, cmd)
// after cmd has beed sent, by register DAT to send user tx data
#define LCM_WRITE_DATA(HANDLE, data) HAL_REG_WRITE((HANDLE)->instance->DAT, data)
// after RS set to 1, by register CMD line to read data
#define LCM_READ_DATA(HANDLE) HAL_REG_READ((HANDLE)->instance->CMD)

/* Private functions prototypes ----------------------------------------------*/
#if (configUSE_LCM_REGISTER_CALLBACKS == 1)
static void _reset_callback(LCM_Handle_T *hlcm);
#endif /* configUSE_LCM_REGISTER_CALLBACKS */

static ATTR_ISR void _lcm_dma_xfer_cplt_callback(DMA_Handle_T *hdma);
static ATTR_ISR void _lcm_dma_xfer_abort_callback(DMA_Handle_T *hdma);
static ATTR_ISR void _lcm_dma_xfer_error_callback(DMA_Handle_T *hdma);
static HAL_Status_T  _lcm_dma_write_kernal(LCM_Handle_T *hlcm, uint32_t src_addr, uint32_t dst_addr, uint16_t data_len);

#endif /* _SN34F78X_HAL_LCM_H_ */
