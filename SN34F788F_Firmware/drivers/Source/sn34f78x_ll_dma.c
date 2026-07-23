/***********************
 * SN34F78X SDK V2.1.4 *
 ***********************/

#include "sn34f78x_ll_dma.h"

#if defined(USE_FULL_LL_DRIVER)

/* Exported functions --------------------------------------------------------*/
LL_Status_T LL_DMA_Init(SN_DMA0_Type *dma, LL_DMA_Init_T *dma_init)
{
    /* Check the parameters */
    RET_FLAG_FALSE(IS_LL_DMA_INSTANCE(dma), LL_ERROR);
    RET_FLAG_FALSE(IS_LL_DMA_CHANNEL(dma_init->channel), LL_ERROR);
    RET_FLAG_FALSE(IS_LL_DMA_XFER_MODE(dma_init->ch_xfer_mode), LL_ERROR);
    RET_FLAG_FALSE(IS_LL_DMA_PRIORITY(dma_init->ch_priority), LL_ERROR);
    RET_FLAG_FALSE(IS_LL_DMA_FIFO_TH(dma_init->ch_fifo_th), LL_ERROR);
    RET_FLAG_FALSE(IS_LL_DMA_SYNC(dma_init->ch_sync), LL_ERROR);
    RET_FLAG_FALSE(IS_LL_DMA_BURST(dma_init->src_burst), LL_ERROR);
    RET_FLAG_FALSE(IS_LL_DMA_WIDTH(dma_init->src_width), LL_ERROR);
    RET_FLAG_FALSE(IS_LL_DMA_ADDR_MODE(dma_init->src_addr_mode), LL_ERROR);
    RET_FLAG_FALSE(IS_LL_DMA_AHB_MASTER(dma_init->src_ahb_master), LL_ERROR);
    RET_FLAG_FALSE(IS_LL_DMA_WIDTH(dma_init->dst_width), LL_ERROR);
    RET_FLAG_FALSE(IS_LL_DMA_ADDR_MODE(dma_init->dst_addr_mode), LL_ERROR);
    RET_FLAG_FALSE(IS_LL_DMA_AHB_MASTER(dma_init->dst_ahb_master), LL_ERROR);
    if (dma == SN_DMA0)
    {
        RET_FLAG_FALSE(IS_LL_DMA0_RS(dma_init->src_rs), LL_ERROR);
    }
    else
    {
        RET_FLAG_FALSE(IS_LL_DMA1_RS(dma_init->src_rs), LL_ERROR);
    }

    if (dma_init->ch_xfer_mode == LL_DMA_MEMORY_TO_MEMORY)
    {
        LL_DMA_SetMode_Normal(dma, dma_init->channel);
    }
    else
    {
        LL_DMA_SetMode_Peripheral(dma, dma_init->channel);
        if (dma_init->ch_xfer_mode == LL_DMA_MEMORY_TO_PERIPH)
        {
            LL_DMA_Disable_SRCIPMode(dma, dma_init->channel);
            LL_DMA_Enable_DSTIPMode(dma, dma_init->channel);
        }
        else if (dma_init->ch_xfer_mode == LL_DMA_PERIPH_TO_MEMORY)
        {
            LL_DMA_Enable_SRCIPMode(dma, dma_init->channel);
            LL_DMA_Disable_DSTIPMode(dma, dma_init->channel);
        }
        else if (dma_init->ch_xfer_mode == LL_DMA_PERIPH_TO_PERIPH)
        {
            LL_DMA_Enable_SRCIPMode(dma, dma_init->channel);
            LL_DMA_Enable_DSTIPMode(dma, dma_init->channel);
        }
    }
    LL_DMA_SetChannelPriority(dma, dma_init->channel, dma_init->ch_priority);
    LL_DMA_SetFIFOTH(dma, dma_init->channel, dma_init->ch_fifo_th);
    if (dma_init->ch_sync == LL_ENABLE)
    {
        LL_DMA_Enable_Sync(dma, dma_init->channel);
    }
    else
    {
        LL_DMA_Disable_Sync(dma, dma_init->channel);
    }
    LL_DMA_SetSRC_BurstSize(dma, dma_init->channel, dma_init->src_burst);
    LL_DMA_SetSRC_Request(dma, dma_init->channel, dma_init->src_rs);
    LL_DMA_SetSRC_TransferWidth(dma, dma_init->channel, dma_init->src_width);
    LL_DMA_SetSRC_AddressMode(dma, dma_init->channel, dma_init->src_addr_mode);
    LL_DMA_SetSRC_AHBMaster(dma, dma_init->channel, dma_init->src_ahb_master);
    LL_DMA_SetDST_Request(dma, dma_init->channel, dma_init->dst_rs);
    LL_DMA_SetDST_TransferWidth(dma, dma_init->channel, dma_init->dst_width);
    LL_DMA_SetDST_AddressMode(dma, dma_init->channel, dma_init->dst_addr_mode);
    LL_DMA_SetDST_AHBMaster(dma, dma_init->channel, dma_init->dst_ahb_master);
    LL_DMA_Enable_DMAControl(dma);

    return LL_OK;
}

LL_Status_T LL_DMA_DeInit(SN_DMA0_Type *dma)
{
    LL_DMA_Disable_DMAControl(dma);

    return LL_OK;
}

void LL_DMA_StructInit(LL_DMA_Init_T *dma_init)
{
    if (dma_init != NULL)
    {
        dma_init->channel        = LL_DMA_CHANNEL_0;
        dma_init->ch_xfer_mode   = LL_DMA_MEMORY_TO_MEMORY;
        dma_init->ch_priority    = LL_DMA_PRIORITY_LOW;
        dma_init->ch_fifo_th     = LL_DMA_FIFO_TH_1;
        dma_init->ch_sync        = LL_DISABLE;
        dma_init->src_burst      = LL_DMA_SRC_BURST_1;
        dma_init->src_rs         = 0;
        dma_init->src_width      = LL_DMA_XFR_WIDTH_8BITS;
        dma_init->src_addr_mode  = LL_DMA_ADDR_MODE_INC;
        dma_init->src_ahb_master = LL_DMA_AHB_MASTER_MATRIX;
        dma_init->dst_rs         = 0;
        dma_init->dst_width      = LL_DMA_XFR_WIDTH_8BITS;
        dma_init->dst_addr_mode  = LL_DMA_ADDR_MODE_INC;
        dma_init->dst_ahb_master = LL_DMA_AHB_MASTER_MATRIX;
    }
}

#endif /* USE_FULL_LL_DRIVER */
