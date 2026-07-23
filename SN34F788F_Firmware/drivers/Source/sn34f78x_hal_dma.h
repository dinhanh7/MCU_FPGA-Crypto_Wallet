/***********************
 * SN34F78X SDK V2.1.4 *
 ***********************/

#ifndef _SN34F78X_HAL_DMA_H_
#define _SN34F78X_HAL_DMA_H_

/* Includes ------------------------------------------------------------------*/
#include "sn34f78x_hal.h"

/* Private types -------------------------------------------------------------*/

/* Private constants ---------------------------------------------------------*/
// function for DMA memory APB scope
#define DMA_MEM_APB_START (0x40000000U)
#define DMA_MEM_APB_END   (0x4007FFFFU)
// function for DMA memory DMA scope
#define DMA_MEM_SRAM_START (0x20000000U)
#define DMA_MEM_SRAM_END   (0x20027FFFU)
// function for DMA memory ROM scope
#define DMA_MEM_ROM_START (0x00200000U)
#define DMA_MEM_ROM_END   (0x0027FFFFU)
// channel offset
#define DMA_CH_OFFSET (8U)

#define DMA_CH_ABORT (1 << 15)

#define DMA_INT_MASK (0x7U)

/* Private macros ------------------------------------------------------------*/
// check is DMA0 instance
#define IS_DMA0_INSTANCE(HANDLE) ((HANDLE->instance) == SN_DMA0)

// check is DMA1 instance
#define IS_DMA1_INSTANCE(HANDLE) ((HANDLE->instance) == SN_DMA1)

// check whether DMA instance is correct
#define IS_PERIPHERAL_DMA_INSTANCE(HANDLE) (((HANDLE->instance) == SN_DMA0) || ((HANDLE->instance) == SN_DMA1))

// check whether DMA panel type is correct
#define IS_DMA_AHB_ENDIAN(PVAR) (((PVAR->m0_endian == DMA_AHB_LITTLE_ENDIAN) || (PVAR->m0_endian == DMA_AHB_BIG_ENDIAN)) && \
                                 ((PVAR->m1_endian == DMA_AHB_LITTLE_ENDIAN) || (PVAR->m1_endian == DMA_AHB_BIG_ENDIAN)))

// check whether DMA bus width is valid
#define IS_DMA_CHANNEL(HANDLE) ((HANDLE->init.channel == DMA_CHANNEL_0) || (HANDLE->init.channel == DMA_CHANNEL_1) || (HANDLE->init.channel == DMA_CHANNEL_2) || \
                                (HANDLE->init.channel == DMA_CHANNEL_3) || (HANDLE->init.channel == DMA_CHANNEL_4) || (HANDLE->init.channel == DMA_CHANNEL_5) || \
                                (HANDLE->init.channel == DMA_CHANNEL_6) || (HANDLE->init.channel == DMA_CHANNEL_7))

// check whether DMA panel type is correct
#define IS_DMA_XFER_MODE(HANDLE) ((HANDLE->init.ch_xfer_mode == DMA_MEMORY_TO_MEMORY) || (HANDLE->init.ch_xfer_mode == DMA_MEMORY_TO_PERIPH) || \
                                  (HANDLE->init.ch_xfer_mode == DMA_PERIPH_TO_MEMORY) || (HANDLE->init.ch_xfer_mode == DMA_PERIPH_TO_PERIPH))

// check whether DMA panel type is correct
#define IS_DMA_PRIORITY(HANDLE) ((HANDLE->init.ch_priority == DMA_PRIORITY_LOW) || (HANDLE->init.ch_priority == DMA_PRIORITY_MEDIUM) || \
                                 (HANDLE->init.ch_priority == DMA_PRIORITY_HIGH) || (HANDLE->init.ch_priority == DMA_PRIORITY_VERY_HIGH))

// check whether DMA transfer mode is valid, only when src and dst are all
#define IS_DMA_FIFO_TH(HANDLE) ((HANDLE->init.ch_fifo_th == DMA_FIFO_TH_1) || (HANDLE->init.ch_fifo_th == DMA_FIFO_TH_2) || (HANDLE->init.ch_fifo_th == DMA_FIFO_TH_4))

// check whether DMA bus width is valid
#define IS_DMA_SRC_BURST(HANDLE) ((HANDLE->init.src_burst == DMA_SRC_BURST_1) || (HANDLE->init.src_burst == DMA_SRC_BURST_4) ||   \
                                  (HANDLE->init.src_burst == DMA_SRC_BURST_8) || (HANDLE->init.src_burst == DMA_SRC_BURST_16) ||  \
                                  (HANDLE->init.src_burst == DMA_SRC_BURST_32) || (HANDLE->init.src_burst == DMA_SRC_BURST_64) || \
                                  (HANDLE->init.src_burst == DMA_SRC_BURST_128) || (HANDLE->init.src_burst == DMA_SRC_BURST_256))

// check DMA0 request selection is valid
#define IS_DMA0_REQUEST_SOURCE(HANDLE) ((HANDLE->instance == SN_DMA0) &&                                                                                       \
                                        (((HANDLE->init.src_rs >= 0 && HANDLE->init.src_rs < 12) || (HANDLE->init.src_rs > 15 && HANDLE->init.src_rs < 18)) || \
                                         ((HANDLE->init.dst_rs >= 0 && HANDLE->init.dst_rs < 12) || (HANDLE->init.dst_rs > 15 && HANDLE->init.dst_rs < 18))))

// check DMA1 request selection is valid
#define IS_DMA1_REQUEST_SOURCE(HANDLE) ((HANDLE->instance == SN_DMA1) &&                                                                                        \
                                        (((HANDLE->init.src_rs >= 0 && HANDLE->init.src_rs < 54) &&                                                             \
                                          (HANDLE->init.src_rs != 18 && HANDLE->init.src_rs < 19 && HANDLE->init.src_rs != 38 && HANDLE->init.src_rs != 52)) || \
                                         ((HANDLE->init.dst_rs >= 0 && HANDLE->init.dst_rs < 54) &&                                                             \
                                          (HANDLE->init.dst_rs != 18 && HANDLE->init.dst_rs < 19 && HANDLE->init.dst_rs != 38 && HANDLE->init.dst_rs != 52))))

// check whether DMA panel type is correct
#define IS_DMA_XFER_WIDTH(HANDLE) (((HANDLE->init.src_width == DMA_XFR_WIDTH_8BITS) || (HANDLE->init.src_width == DMA_XFR_WIDTH_16BITS) || (HANDLE->init.src_width == DMA_XFR_WIDTH_32BITS)) && \
                                   ((HANDLE->init.dst_width == DMA_XFR_WIDTH_8BITS) || (HANDLE->init.dst_width == DMA_XFR_WIDTH_16BITS) || (HANDLE->init.dst_width == DMA_XFR_WIDTH_32BITS)))

// check whether DMA panel type is correct
#define IS_DMA_ADDR_MODE(HANDLE) (((HANDLE->init.src_addr_mode == DMA_ADDR_MODE_INC) || (HANDLE->init.src_addr_mode == DMA_ADDR_MODE_DEC) ||       \
                                   (HANDLE->init.src_addr_mode == DMA_ADDR_MODE_FIXED) || (HANDLE->init.src_addr_mode == DMA_ADDR_MODE_CYCLIC)) && \
                                  ((HANDLE->init.dst_addr_mode == DMA_ADDR_MODE_INC) || (HANDLE->init.dst_addr_mode == DMA_ADDR_MODE_DEC) ||       \
                                   (HANDLE->init.dst_addr_mode == DMA_ADDR_MODE_FIXED) || (HANDLE->init.dst_addr_mode == DMA_ADDR_MODE_CYCLIC)))

// check whether DMA panel type is correct
#define IS_DMA_AHB_MASTER(HANDLE) (((HANDLE->init.src_ahb_master == DMA_AHB_MASTER_MATRIX) || (HANDLE->init.src_ahb_master == DMA_AHB_MASTER_PERIPH)) && \
                                   ((HANDLE->init.dst_ahb_master == DMA_AHB_MASTER_MATRIX) || (HANDLE->init.dst_ahb_master == DMA_AHB_MASTER_PERIPH)))

// check whether DMA transfer mode is valid
#define IS_DMA_XFER_CPLT(VAR) ((VAR == DMA_XFER_CPLT_FULL) || (VAR == DMA_XFER_CPLT_HALF))

// check whether DMA transfer mode is valid
#define IS_DMA_ACCESS_MEMORY(VAR) ((VAR >= DMA_MEM_APB_START && VAR <= DMA_MEM_APB_END) || \
                                   (VAR >= DMA_MEM_ROM_START && VAR <= DMA_MEM_ROM_END) || \
                                   (VAR >= DMA_MEM_SRAM_START && VAR <= DMA_MEM_SRAM_END))

// check current channel is busy before
#define IS_DMA_CHANNEL_BUSY(HANDLE) (((1 << (HANDLE)->init.channel) & ((HANDLE)->instance->CH_BUSY)) != 0)

// check current channel transfer complete
#define IS_DMA_CHANNEL_TC(HANDLE) (((1 << (HANDLE)->init.channel) & ((HANDLE)->instance->TC)) != 0)

// check current channel is busy before
#define IS_DMA_CHANNEL_ABT(HANDLE) (((1 << ((HANDLE)->init.channel + 16)) & ((HANDLE)->instance->ERR_ABT)) != 0)

// check abort interrupt happen in current channel
#define IS_DMA_INT_ABT(HANDLE) (((1 << ((HANDLE)->init.channel + 16)) & ((HANDLE)->instance->INT_ERR_ABT)) != 0)

// check error interrupt happen in current channel
#define IS_DMA_INT_ERR(HANDLE) (((1 << ((HANDLE)->init.channel)) & ((HANDLE)->instance->INT_ERR_ABT)) != 0)

// check transfer complete interrupt happen in current channel
#define IS_DMA_INT_TC(HANDLE) (((1 << ((HANDLE)->init.channel)) & ((HANDLE)->instance->INT_TC)) != 0)

// clear TC interrupt
#define DMA_CLR_INT_TC(HANDLE) HAL_REG_SBIT((HANDLE)->instance->INT_TC_CLR, (1 << ((HANDLE)->init.channel)))

// check transfer limitation
#define WIDTH_BYTE(VAR) (1 << VAR)
#define BURST_BYTE(VAR) ((VAR == 0) ? 1 : 1 << (VAR + 1))
// (Burst size * SRC_WIDTH) must be equal to or larger than DST_WIDTH;
#define DMA_LIMIT_INVALID_SRC_WIDTH(HANDLE) ((WIDTH_BYTE(HANDLE->init.src_width) * BURST_BYTE(HANDLE->init.src_burst)) < WIDTH_BYTE(HANDLE->init.dst_width))
// do not under decrement source address mode when the pack/unpack function works;
#define DMA_LIMIT_INVALID_DEC_ADDR(HANDLE) ((HANDLE->init.src_width != HANDLE->init.dst_width) && (HANDLE->init.src_addr_mode == DMA_ADDR_MODE_DEC))
// do not set SRC_SIZE = 0 (burst size = 1) when the pack function works
#define DMA_LIMIT_INVALID_SRC_CFG(HANDLE) ((HANDLE->init.src_width < HANDLE->init.dst_width) && (HANDLE->init.src_width == DMA_XFR_WIDTH_8BITS) && (HANDLE->init.src_burst == DMA_SRC_BURST_1))
// the total DMA source and destination limitation check
#define IS_DMA_LIMITATION(HANDLE) (DMA_LIMIT_INVALID_SRC_WIDTH(HANDLE) || DMA_LIMIT_INVALID_DEC_ADDR(HANDLE) || DMA_LIMIT_INVALID_SRC_CFG(HANDLE))

// check the valid of callback id
#define IS_DMA_CALLBACK_ID(VAR) (VAR <= HAL_DMA_ALL_CB_ID)

#define REG_CSR(HANDLE)     ((volatile uint32_t *)(&((HANDLE)->instance->C0_CSR) + ((HANDLE)->init.channel * DMA_CH_OFFSET)))
#define REG_CFG(HANDLE)     ((volatile uint32_t *)(&((HANDLE)->instance->C0_CFG) + ((HANDLE)->init.channel * DMA_CH_OFFSET)))
#define REG_SIZE(HANDLE)    ((volatile uint32_t *)(&((HANDLE)->instance->C0_SIZE) + ((HANDLE)->init.channel * DMA_CH_OFFSET)))
#define REG_SRCADDR(HANDLE) ((volatile uint32_t *)(&((HANDLE)->instance->C0_SRCADDR) + ((HANDLE)->init.channel * DMA_CH_OFFSET)))
#define REG_DSTADDR(HANDLE) ((volatile uint32_t *)(&((HANDLE)->instance->C0_DSTADDR) + ((HANDLE)->init.channel * DMA_CH_OFFSET)))

#define CALC_XFER_MODE(HANDLE) (HANDLE->init.ch_xfer_mode != DMA_MEMORY_TO_MEMORY)
#define CALC_SRC_MODE(HANDLE)  ((HANDLE->init.ch_xfer_mode & 0x2) == 0x2)
#define CALC_DST_MODE(HANDLE)  ((HANDLE->init.ch_xfer_mode & 0x1) == 0x1)

// check abort interrupt happen in current channel
#define IS_DMA_ENABLE_ABT(HANDLE) ((*REG_CFG(HANDLE) & (1 << 2)) == 0)
// check transfer complete interrupt happen in current channel
#define IS_DMA_ENABLE_TC(HANDLE) ((*REG_CFG(HANDLE) & (1 << 0)) == 0)

// wait current channel idle
#define DMA_WAIT_CHANNEL_IDLE(HANDLE) while (!IS_DMA_CHANNEL_BUSY(HANDLE))

// wait current channel transfer done
#define DMA_WAIT_CHANNEL_TC(HANDLE)                                                 \
    do                                                                              \
    {                                                                               \
        if (IS_DMA_ENABLE_TC(HANDLE))                                               \
        {                                                                           \
            while (!IS_DMA_CHANNEL_TC(HANDLE) && (HANDLE)->state == HAL_STATE_BUSY) \
                ;                                                                   \
        }                                                                           \
        else                                                                        \
        {                                                                           \
            while (!IS_DMA_CHANNEL_TC(HANDLE))                                      \
                ;                                                                   \
        }                                                                           \
    } while (0)

// wait current channel abort finish for blocking abort interface
#define DMA_WAIT_CHANNEL_ABT(HANDLE)                                                 \
    do                                                                               \
    {                                                                                \
        if (IS_DMA_ENABLE_ABT(HANDLE))                                               \
        {                                                                            \
            while (!IS_DMA_CHANNEL_ABT(HANDLE) && (HANDLE)->state == HAL_STATE_BUSY) \
                ;                                                                    \
        }                                                                            \
        else                                                                         \
        {                                                                            \
            while (!IS_DMA_CHANNEL_ABT(HANDLE))                                      \
                ;                                                                    \
        }                                                                            \
    } while (0)

// record the active channel
#define SET_DMA_ACTIVE_CH(HANDLE)                                      \
    do                                                                 \
    {                                                                  \
        if (IS_DMA0_INSTANCE(HANDLE))                                  \
        {                                                              \
            HAL_FLAG_SET(active_ch[0], (1 << (HANDLE)->init.channel)); \
        }                                                              \
        else                                                           \
        {                                                              \
            HAL_FLAG_SET(active_ch[1], (1 << (HANDLE)->init.channel)); \
        }                                                              \
    } while (0)

// clear the active channel
#define CLR_DMA_ACTIVE_CH(HANDLE)                                      \
    do                                                                 \
    {                                                                  \
        if (IS_DMA0_INSTANCE(HANDLE))                                  \
        {                                                              \
            HAL_FLAG_CLR(active_ch[0], (1 << (HANDLE)->init.channel)); \
        }                                                              \
        else                                                           \
        {                                                              \
            HAL_FLAG_CLR(active_ch[1], (1 << (HANDLE)->init.channel)); \
        }                                                              \
    } while (0)

#define IS_ACTIVE_CH(HANDLE) ((IS_DMA0_INSTANCE(HANDLE) && (active_ch[0] & (1 << (HANDLE)->init.channel))) || \
                              (IS_DMA1_INSTANCE(HANDLE) && (active_ch[1] & (1 << (HANDLE)->init.channel))))

/* Private variable prototypes  ----------------------------------------------*/
static uint8_t active_ch[2] = {0};
/* Private functions prototypes ----------------------------------------------*/
static HAL_Status_T _dma_check_xfer_width(DMA_Handle_T *hdma);
static HAL_Status_T _dma_check_xfer_cond(DMA_Handle_T *hdma);
static HAL_Status_T _dma_check_transfer_option(DMA_Handle_T *hdma, uint32_t src_addr);

#endif /* _SN34F78X_HAL_DMA_H_ */
