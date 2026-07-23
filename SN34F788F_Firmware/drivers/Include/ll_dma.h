/***********************
 * SN34F78X SDK V2.1.4 *
 ***********************/

#ifndef _LL_DMA_H_
#define _LL_DMA_H_

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "sn34f78x_ll_def.h"
#include "ll_reg_msk.h"

/* Exported types ------------------------------------------------------------*/
/**
 * @brief LL DMA Init Structure Definition
 * \ingroup dma_struct_define
 */
typedef struct
{
    uint32_t channel;        /**< Specifies the channel used for the specified stream.
                                  This parameter can be a value of @ref dma_channel */
    uint32_t ch_xfer_mode;   /**< Specifies the operation mode of the DMA stream.
                                  This parameter can be a value of @ref dma_xfer_mode */
    uint32_t ch_priority;    /**< Specifies the software priority for the DMA stream.
                                  This parameter can be a value of @ref dma_priority */
    uint32_t ch_fifo_th;     /**< Specifies the FIFO threshold level.
                                  This parameter can be a value of @ref dma_fifo_threshold */
    uint32_t ch_sync;        /**< Specifies the synchronization logic enable for channel.
                                  When DMA PCLK(AHB bus) is different from PCLK(APBn bus), current field need be enable,
                                  others keep disable.
                                  This parameter can be a value of \b LL_ENABLE or \b LL_DISABLE */
    uint32_t src_burst;      /**< Specifies the source burst size selection
                                  This parameter can be a value of @ref dma_source_burst */
    uint32_t src_rs;         /**< specifies the source request selection.
                                  This parameter can be a value of @ref dma_dma0_rs and  @ref dma_dma1_rs */
    uint32_t src_width;      /**< Specifies the source transfer width
                                  This parameter can be a value of @ref dma_xfr_width */
    uint32_t src_addr_mode;  /**< specifies the source address control.
                                  This parameter can be a value of @ref dma_addr_mode */
    uint32_t src_ahb_master; /**< Specifies the source AHB master
                                  This parameter can be a value of @ref dma_ahb_master */
    uint32_t dst_rs;         /**< specifies the destination request selection.
                                  This parameter can be a value of @ref dma_dma0_rs for DMA0 and  @ref dma_dma1_rs for DMA1 */
    uint32_t dst_width;      /**< Specifies the destination transfer width
                                  This parameter can be a value of @ref dma_xfr_width */
    uint32_t dst_addr_mode;  /**< specifies the destination address control.
                                  This parameter can be a value of @ref dma_addr_mode */
    uint32_t dst_ahb_master; /**< Specifies the destination AHB master
                                  This parameter can be a value of @ref dma_ahb_master */
} LL_DMA_Init_T;

/* Exported constants --------------------------------------------------------*/
/**
 * \defgroup dma_channel DMA Channel
 * \ingroup dma_control
 * @{
 */
#define LL_DMA_CHANNEL_0 0x00000000U /**< DMA Channel 0 */
#define LL_DMA_CHANNEL_1 0x00000001U /**< DMA Channel 1 */
#define LL_DMA_CHANNEL_2 0x00000002U /**< DMA Channel 2 */
#define LL_DMA_CHANNEL_3 0x00000003U /**< DMA Channel 3 */
#define LL_DMA_CHANNEL_4 0x00000004U /**< DMA Channel 4 */
#define LL_DMA_CHANNEL_5 0x00000005U /**< DMA Channel 5 */
#define LL_DMA_CHANNEL_6 0x00000006U /**< DMA Channel 6 */
#define LL_DMA_CHANNEL_7 0x00000007U /**< DMA Channel 7 */
/**
 * @}
 */

/**
 * \defgroup dma_xfer_mode DMA Data Transfer Direction Mode
 * \ingroup dma_control
 * @{
 */
#define LL_DMA_MEMORY_TO_MEMORY 0x00000000U /**< DMA direction from memory to memory         */
#define LL_DMA_MEMORY_TO_PERIPH 0x00000001U /**< DMA direction from memory to peripheral     */
#define LL_DMA_PERIPH_TO_MEMORY 0x00000002U /**< DMA direction from peripheral to memory     */
#define LL_DMA_PERIPH_TO_PERIPH 0x00000003U /**< DMA direction from peripheral to peripheral */
/**
 * @}
 */

/**
 * \defgroup dma_priority DMA Channel Priority
 * \ingroup dma_control
 * @{
 */
#define LL_DMA_PRIORITY_LOW       0x00000000U /**< DMA priority level: low       */
#define LL_DMA_PRIORITY_MEDIUM    0x00000001U /**< DMA priority level: medium    */
#define LL_DMA_PRIORITY_HIGH      0x00000002U /**< DMA priority level: high      */
#define LL_DMA_PRIORITY_VERY_HIGH 0x00000003U /**< DMA priority level: very high */
/**
 * @}
 */

/**
 * \defgroup dma_fifo_threshold DMA FIFO Threshold Level
 * \ingroup  dma_control
 * @{
 */
#define LL_DMA_FIFO_TH_1 0x00000000U /**< DMA FIFO threshold 1*32 bits */
#define LL_DMA_FIFO_TH_2 0x00000001U /**< DMA FIFO threshold 2*32 bits */
#define LL_DMA_FIFO_TH_4 0x00000002U /**< DMA FIFO threshold 4*32 bits */
/**
 * @}
 */

/**
 * \defgroup dma_source_burst DMA Source Burst Size
 * \ingroup dma_control
 * @{
 */
#define LL_DMA_SRC_BURST_1   0x00000000U /**< DMA source burst size: 1   */
#define LL_DMA_SRC_BURST_4   0x00000001U /**< DMA source burst size: 4   */
#define LL_DMA_SRC_BURST_8   0x00000002U /**< DMA source burst size: 8   */
#define LL_DMA_SRC_BURST_16  0x00000003U /**< DMA source burst size: 16  */
#define LL_DMA_SRC_BURST_32  0x00000004U /**< DMA source burst size: 32  */
#define LL_DMA_SRC_BURST_64  0x00000005U /**< DMA source burst size: 64  */
#define LL_DMA_SRC_BURST_128 0x00000006U /**< DMA source burst size: 128 */
#define LL_DMA_SRC_BURST_256 0x00000007U /**< DMA source burst size: 256 */
/**
 * @}
 */

/**
 * \defgroup dma_dma0_rs DMA Request Selection of DMA0
 * \ingroup dma_control
 * @{
 */
#define LL_DMA_RS0_UART1_TX  0x00000000U /**< DMA request select UART1 TX  */
#define LL_DMA_RS0_UART1_RX  0x00000001U /**< DMA request select UART1 RX  */
#define LL_DMA_RS0_UART2_TX  0x00000002U /**< DMA request select UART2 TX  */
#define LL_DMA_RS0_UART2_RX  0x00000003U /**< DMA request select UART2 RX  */
#define LL_DMA_RS0_UART3_TX  0x00000004U /**< DMA request select UART3 TX  */
#define LL_DMA_RS0_UART3_RX  0x00000005U /**< DMA request select UART3 RX  */
#define LL_DMA_RS0_UART4_TX  0x00000006U /**< DMA request select UART4 TX  */
#define LL_DMA_RS0_UART4_RX  0x00000007U /**< DMA request select UART4 RX  */
#define LL_DMA_RS0_UART1_FIR 0x00000008U /**< DMA request select UART1 FIR */
#define LL_DMA_RS0_UART2_FIR 0x00000009U /**< DMA request select UART2 FIR */
#define LL_DMA_RS0_UART3_FIR 0x0000000AU /**< DMA request select UART3 FIR */
#define LL_DMA_RS0_UART4_FIR 0x0000000BU /**< DMA request select UART4 FIR */
#define LL_DMA_RS0_TPM6_MR0  0x00000010U /**< DMA request select TPM6 MR0  */
#define LL_DMA_RS0_TPM7_MR0  0x00000011U /**< DMA request select TPM7 MR0  */
/**
 * @}
 */

/**
 * \defgroup dma_dma1_rs DMA Request Selection of DMA1
 * \ingroup dma_control
 * @{
 */
#define LL_DMA_RS1_SSP0_TX   0x00000000U /**< DMA request select SSP0 TX   */
#define LL_DMA_RS1_SSP0_RX   0x00000001U /**< DMA request select SSP0 RX   */
#define LL_DMA_RS1_SSP1_TX   0x00000002U /**< DMA request select SSP1 TX   */
#define LL_DMA_RS1_SSP1_RX   0x00000003U /**< DMA request select SSP1 RX   */
#define LL_DMA_RS1_SSP2_TX   0x00000004U /**< DMA request select SSP2 TX   */
#define LL_DMA_RS1_SSP2_RX   0x00000005U /**< DMA request select SSP2 RX   */
#define LL_DMA_RS1_UART0_TX  0x00000006U /**< DMA request select UART0 TX  */
#define LL_DMA_RS1_UART0_RX  0x00000007U /**< DMA request select UART0 RX  */
#define LL_DMA_RS1_UART5_TX  0x00000008U /**< DMA request select UART5 TX  */
#define LL_DMA_RS1_UART5_RX  0x00000009U /**< DMA request select UART5 RX  */
#define LL_DMA_RS1_ADC0_0    0x0000000AU /**< DMA request select ADC0_0    */
#define LL_DMA_RS1_ADC0_1    0x0000000BU /**< DMA request select ADC0_1    */
#define LL_DMA_RS1_ADC0_2    0x0000000CU /**< DMA request select ADC0_2    */
#define LL_DMA_RS1_ADC0_3    0x0000000DU /**< DMA request select ADC0_3    */
#define LL_DMA_RS1_SDIO      0x0000000EU /**< DMA request select SDIO      */
#define LL_DMA_RS1_UART0_FIR 0x0000000FU /**< DMA request select UART0 FIR */
#define LL_DMA_RS1_UART5_FIR 0x00000010U /**< DMA request select UART5 FIR */
#define LL_DMA_RS1_LCM       0x00000011U /**< DMA request select LCM       */
#define LL_DMA_RS1_TPM0_CAP0 0x00000014U /**< DMA request select TPM0 CAP0 */
#define LL_DMA_RS1_TPM0_MR9  0x00000015U /**< DMA request select TPM0 MR9  */
#define LL_DMA_RS1_TPM0_MR0  0x00000016U /**< DMA request select TPM0 MR0  */
#define LL_DMA_RS1_TPM0_MR1  0x00000017U /**< DMA request select TPM0 MR1  */
#define LL_DMA_RS1_TPM0_MR2  0x00000018U /**< DMA request select TPM0 MR2  */
#define LL_DMA_RS1_TPM0_MR3  0x00000019U /**< DMA request select TPM0 MR3  */
#define LL_DMA_RS1_TPM1_CAP0 0x0000001AU /**< DMA request select TPM1 CAP0 */
#define LL_DMA_RS1_TPM1_MR9  0x0000001BU /**< DMA request select TPM1 MR9  */
#define LL_DMA_RS1_TPM1_MR0  0x0000001CU /**< DMA request select TPM1 MR0  */
#define LL_DMA_RS1_TPM1_MR1  0x0000001DU /**< DMA request select TPM1 MR1  */
#define LL_DMA_RS1_TPM1_MR2  0x0000001EU /**< DMA request select TPM1 MR2  */
#define LL_DMA_RS1_TPM1_MR3  0x0000001FU /**< DMA request select TPM1 MR3  */
#define LL_DMA_RS1_TPM2_CAP0 0x00000020U /**< DMA request select TPM2 CAP0 */
#define LL_DMA_RS1_TPM2_MR9  0x00000021U /**< DMA request select TPM2 MR9  */
#define LL_DMA_RS1_TPM2_MR0  0x00000022U /**< DMA request select TPM2 MR0  */
#define LL_DMA_RS1_TPM2_MR1  0x00000023U /**< DMA request select TPM2 MR1  */
#define LL_DMA_RS1_TPM2_MR2  0x00000024U /**< DMA request select TPM2 MR2  */
#define LL_DMA_RS1_TPM2_MR3  0x00000025U /**< DMA request select TPM2 MR3  */
#define LL_DMA_RS1_TPM3_MR9  0x00000027U /**< DMA request select TPM3 MR9  */
#define LL_DMA_RS1_TPM3_MR0  0x00000028U /**< DMA request select TPM3 MR0  */
#define LL_DMA_RS1_TPM3_MR1  0x00000029U /**< DMA request select TPM3 MR1  */
#define LL_DMA_RS1_TPM4_CAP0 0x0000002AU /**< DMA request select TPM4 CAP0 */
#define LL_DMA_RS1_TPM4_MR9  0x0000002BU /**< DMA request select TPM4 MR9  */
#define LL_DMA_RS1_TPM4_MR0  0x0000002CU /**< DMA request select TPM4 MR0  */
#define LL_DMA_RS1_TPM4_MR1  0x0000002DU /**< DMA request select TPM4 MR1  */
#define LL_DMA_RS1_TPM5_CAP0 0x0000002EU /**< DMA request select TPM5 CAP0 */
#define LL_DMA_RS1_TPM5_MR9  0x0000002FU /**< DMA request select TPM5 MR9  */
#define LL_DMA_RS1_TPM5_MR0  0x00000030U /**< DMA request select TPM5 MR0  */
#define LL_DMA_RS1_TPM5_MR1  0x00000031U /**< DMA request select TPM5 MR1  */
#define LL_DMA_RS1_TPM5_MR2  0x00000032U /**< DMA request select TPM5 MR2  */
#define LL_DMA_RS1_TPM5_MR3  0x00000033U /**< DMA request select TPM5 MR3  */
#define LL_DMA_RS1_TPM8_MR12 0x00000035U /**< DMA request select TPM8 MR12 */
/**
 * @}
 */

/**
 * \defgroup dma_xfr_width DMA Transfer Width for Source and Destination
 * \ingroup dma_control
 * @{
 */
#define LL_DMA_XFR_WIDTH_8BITS  0x00000000U /**< DMA transfer width 8 bits  */
#define LL_DMA_XFR_WIDTH_16BITS 0x00000001U /**< DMA transfer width 16 bits */
#define LL_DMA_XFR_WIDTH_32BITS 0x00000002U /**< DMA transfer width 32 bits */
/**
 * @}
 */

/**
 * \defgroup dma_addr_mode DMA Address Mode
 * \ingroup dma_control
 * @{
 */
#define LL_DMA_ADDR_MODE_INC    0x00000000U /**< DMA increment address        */
#define LL_DMA_ADDR_MODE_DEC    0x00000001U /**< DMA decrement address        */
#define LL_DMA_ADDR_MODE_FIXED  0x00000002U /**< DMA fixed address            */
#define LL_DMA_ADDR_MODE_CYCLIC 0x00000003U /**< DMA increment cyclic address */
/**
 * @}
 */

/**
 * \defgroup dma_ahb_master DMA AHB Master
 * \ingroup dma_control
 * @{
 */
#define LL_DMA_AHB_MASTER_MATRIX 0x00000000U /**< DMA AHB master 0 ; for AHB Matrix           */
#define LL_DMA_AHB_MASTER_PERIPH 0x00000001U /**< DMA AHB master 1 ; for APB1/APB2 peripheral */
/**
 * @}
 */

/* Private constants ---------------------------------------------------------*/
#undef SN_DMA1
#define SN_DMA1 ((SN_DMA0_Type *)SN_DMA1_BASE) /**< Rename DMA1 base address */

#define LL_DMA_CH_OFFSET                     (8U)                                                                                   /**< Channel offset */
#define LL_REG_CSR(__INSTANCE__, __CH__)     ((volatile uint32_t *)(&((__INSTANCE__)->C0_CSR) + (LL_DMA_CH_OFFSET * (__CH__))))     /**< CSR offset */
#define LL_REG_CFG(__INSTANCE__, __CH__)     ((volatile uint32_t *)(&((__INSTANCE__)->C0_CFG) + (LL_DMA_CH_OFFSET * (__CH__))))     /**< CFG offset */
#define LL_REG_SRCADDR(__INSTANCE__, __CH__) ((volatile uint32_t *)(&((__INSTANCE__)->C0_SRCADDR) + (LL_DMA_CH_OFFSET * (__CH__)))) /**< SRCADDR offset */
#define LL_REG_DSTADDR(__INSTANCE__, __CH__) ((volatile uint32_t *)(&((__INSTANCE__)->C0_DSTADDR) + (LL_DMA_CH_OFFSET * (__CH__)))) /**< DSTADDR offset */
#define LL_REG_SIZE(__INSTANCE__, __CH__)    ((volatile uint32_t *)(&((__INSTANCE__)->C0_SIZE) + (LL_DMA_CH_OFFSET * (__CH__))))    /**< Size offset */

/* Exported functions --------------------------------------------------------*/
/**
 * @brief  Enable the DMA controller
 * @param  dma DMA Instance
 * @retval none
 */
__STATIC_INLINE void LL_DMA_Enable_DMAControl(SN_DMA0_Type *dma)
{
    LL_REG_SBIT(dma->MCSR, DMA_MCSR_DMACEN);
}

/**
 * @brief  Disable the DMA controller
 * @param  dma DMA Instance
 * @retval none
 */
__STATIC_INLINE void LL_DMA_Disable_DMAControl(SN_DMA0_Type *dma)
{
    LL_REG_CBIT(dma->MCSR, DMA_MCSR_DMACEN);
}

/**
 * @brief  Return if DMA controller is enabled
 * @param  dma DMA Instance
 * @retval State of bit (1 or 0).
 */
__STATIC_INLINE uint32_t LL_DMA_IsEnabled_DMAControl(SN_DMA0_Type *dma)
{
    return (LL_REG_RBIT(dma->MCSR, DMA_MCSR_DMACEN) == (DMA_MCSR_DMACEN));
}

/**
 * @brief  Set AHB master 0 endian to little endian
 * @param  dma DMA Instance
 * @retval none
 */
__STATIC_INLINE void LL_DMA_SetAHBM0Endian_Little(SN_DMA0_Type *dma)
{
    LL_REG_CBIT(dma->MCSR, DMA_MCSR_M0ENDIAN);
}

/**
 * @brief  Set AHB master 0 endian to big endian
 * @param  dma DMA Instance
 * @retval none
 */
__STATIC_INLINE void LL_DMA_SetAHBM0Endian_Big(SN_DMA0_Type *dma)
{
    LL_REG_SBIT(dma->MCSR, DMA_MCSR_M0ENDIAN);
}

/**
 * @brief  Set AHB master 1 endian to little endian
 * @param  dma DMA Instance
 * @retval none
 */
__STATIC_INLINE void LL_DMA_SetAHBM1Endian_Little(SN_DMA0_Type *dma)
{
    LL_REG_CBIT(dma->MCSR, DMA_MCSR_M1ENDIAN);
}

/**
 * @brief  Set AHB master 1 endian to big endian
 * @param  dma DMA Instance
 * @retval none
 */
__STATIC_INLINE void LL_DMA_SetAHBM1Endian_Big(SN_DMA0_Type *dma)
{
    LL_REG_SBIT(dma->MCSR, DMA_MCSR_M1ENDIAN);
}

/**
 * @brief  Enable the DMA channel
 * @param  dma DMA Instance
 * @param  ch This parameter can be one of @ref dma_channel
 * @retval none
 */
__STATIC_INLINE void LL_DMA_Enable_CH(SN_DMA0_Type *dma, uint32_t ch)
{
    LL_REG_SBIT(*LL_REG_CSR(dma, ch), DMA_CSR_CH_EN);
}

/**
 * @brief  Disable the DMA channel
 * @param  dma DMA Instance
 * @param  ch This parameter can be one of @ref dma_channel
 * @retval none
 */
__STATIC_INLINE void LL_DMA_Disable_CH(SN_DMA0_Type *dma, uint32_t ch)
{
    LL_REG_CBIT(*LL_REG_CSR(dma, ch), DMA_CSR_CH_EN);
}

/**
 * @brief  Return if DMA channel is enabled.
 * @param  dma DMA Instance
 * @param  ch This parameter can be one of @ref dma_channel
 * @retval State of bit (1 or 0).
 */
__STATIC_INLINE uint32_t LL_DMA_IsEnabled_CH(SN_DMA0_Type *dma, uint32_t ch)
{
    return (LL_REG_RBIT(*LL_REG_CSR(dma, ch), DMA_CSR_CH_EN) == (DMA_CSR_CH_EN));
}

/**
 * @brief  Choose AHB master 0 for destination
 * @param  dma DMA Instance
 * @param  ch This parameter can be one of @ref dma_channel
 * @retval none
 */
__STATIC_INLINE void LL_DMA_SetDST_AHBMaster0(SN_DMA0_Type *dma, uint32_t ch)
{
    LL_REG_CBIT(*LL_REG_CSR(dma, ch), DMA_CSR_DST_SEL);
}

/**
 * @brief  Choose AHB master 1 for destination
 * @param  dma DMA Instance
 * @param  ch This parameter can be one of @ref dma_channel
 * @retval none
 */
__STATIC_INLINE void LL_DMA_SetDST_AHBMaster1(SN_DMA0_Type *dma, uint32_t ch)
{
    LL_REG_SBIT(*LL_REG_CSR(dma, ch), DMA_CSR_DST_SEL);
}

/**
 * @brief  Set AHB master for destination
 * @param  dma DMA Instance
 * @param  ch This parameter can be one of @ref dma_channel
 * @param  master This parameter can be one of @ref dma_ahb_master
 * @retval none
 */
__STATIC_INLINE void LL_DMA_SetDST_AHBMaster(SN_DMA0_Type *dma, uint32_t ch, uint32_t master)
{
    LL_REG_CBIT(*LL_REG_CSR(dma, ch), DMA_CSR_DST_SEL);
    LL_REG_SBIT(*LL_REG_CSR(dma, ch), (master << DMA_CSR_DST_SEL_Pos) & DMA_CSR_DST_SEL);
}

/**
 * @brief  Get AHB master for destination
 * @param  dma DMA Instance
 * @param  ch This parameter can be one of @ref dma_channel
 * @retval This parameter can be one of @ref dma_ahb_master
 */
__STATIC_INLINE uint32_t LL_DMA_GetDST_AHBMaster(SN_DMA0_Type *dma, uint32_t ch)
{
    return (LL_REG_RBIT(*LL_REG_CSR(dma, ch), DMA_CSR_DST_SEL) >> DMA_CSR_DST_SEL_Pos);
}

/**
 * @brief  Set AHB master for source
 * @param  dma DMA Instance
 * @param  ch This parameter can be one of @ref dma_channel
 * @param  master This parameter can be one of @ref dma_ahb_master
 * @retval none
 */
__STATIC_INLINE void LL_DMA_SetSRC_AHBMaster(SN_DMA0_Type *dma, uint32_t ch, uint32_t master)
{
    LL_REG_CBIT(*LL_REG_CSR(dma, ch), DMA_CSR_SRC_SEL);
    LL_REG_SBIT(*LL_REG_CSR(dma, ch), (master << DMA_CSR_SRC_SEL_Pos) & DMA_CSR_SRC_SEL);
}

/**
 * @brief  Get AHB master for source
 * @param  dma DMA Instance
 * @param  ch This parameter can be one of @ref dma_channel
 * @retval This parameter can be one of @ref dma_ahb_master
 */
__STATIC_INLINE uint32_t LL_DMA_GetSRC_AHBMaster(SN_DMA0_Type *dma, uint32_t ch)
{
    return (LL_REG_RBIT(*LL_REG_CSR(dma, ch), DMA_CSR_SRC_SEL) >> DMA_CSR_SRC_SEL_Pos);
}

/**
 * @brief  Set destination address mode
 * @param  dma DMA Instance
 * @param  ch This parameter can be one of @ref dma_channel
 * @param  addr_mode This parameter can be one of @ref dma_addr_mode
 * @retval none
 */
__STATIC_INLINE void LL_DMA_SetDST_AddressMode(SN_DMA0_Type *dma, uint32_t ch, uint32_t addr_mode)
{
    volatile uint32_t *csr = LL_REG_CSR(dma, ch);
    LL_REG_CBIT(*csr, DMA_CSR_DSTAD_CTL);
    LL_REG_SBIT(*csr, ((addr_mode << DMA_CSR_DSTAD_CTL_Pos) & DMA_CSR_DSTAD_CTL));
}

/**
 * @brief  Get destination address mode
 * @param  dma DMA Instance
 * @param  ch This parameter can be one of @ref dma_channel
 * @retval Returned value can be one of @ref dma_addr_mode
 */
__STATIC_INLINE uint32_t LL_DMA_GetDST_AddressMode(SN_DMA0_Type *dma, uint32_t ch)
{
    return (LL_REG_RBIT(*LL_REG_CSR(dma, ch), DMA_CSR_DSTAD_CTL) >> DMA_CSR_DSTAD_CTL_Pos);
}

/**
 * @brief  Set source address mode
 * @param  dma DMA Instance
 * @param  ch This parameter can be one of @ref dma_channel
 * @param  addr_mode This parameter can be one of @ref dma_addr_mode
 * @retval none
 */
__STATIC_INLINE void LL_DMA_SetSRC_AddressMode(SN_DMA0_Type *dma, uint32_t ch, uint32_t addr_mode)
{
    volatile uint32_t *csr = LL_REG_CSR(dma, ch);
    LL_REG_CBIT(*csr, DMA_CSR_SRCAD_CTL);
    LL_REG_SBIT(*csr, ((addr_mode << DMA_CSR_SRCAD_CTL_Pos) & DMA_CSR_SRCAD_CTL));
}

/**
 * @brief  Get source address mode
 * @param  dma DMA Instance
 * @param  ch This parameter can be one of @ref dma_channel
 * @retval Returned value can be one of @ref dma_addr_mode
 */
__STATIC_INLINE uint32_t LL_DMA_GetSRC_AddressMode(SN_DMA0_Type *dma, uint32_t ch)
{
    return (LL_REG_RBIT(*LL_REG_CSR(dma, ch), DMA_CSR_SRCAD_CTL) >> DMA_CSR_SRCAD_CTL_Pos);
}

/**
 * @brief  Set hardware handshake mode to normal mode
 * @param  dma DMA Instance
 * @param  ch This parameter can be one of @ref dma_channel
 * @retval none
 */
__STATIC_INLINE void LL_DMA_SetMode_Normal(SN_DMA0_Type *dma, uint32_t ch)
{
    LL_REG_CBIT(*LL_REG_CSR(dma, ch), DMA_CSR_MODE);
}

/**
 * @brief  Set hardware handshake mode to peripheral mode
 * @param  dma DMA Instance
 * @param  ch This parameter can be one of @ref dma_channel
 * @retval none
 */
__STATIC_INLINE void LL_DMA_SetMode_Peripheral(SN_DMA0_Type *dma, uint32_t ch)
{
    LL_REG_SBIT(*LL_REG_CSR(dma, ch), DMA_CSR_MODE);
}

/**
 * @brief  Set destination transfer width
 * @param  dma DMA Instance
 * @param  ch This parameter can be one of @ref dma_channel
 * @param  xfer_width This parameter can be one of @ref dma_xfr_width
 * @retval none
 */
__STATIC_INLINE void LL_DMA_SetDST_TransferWidth(SN_DMA0_Type *dma, uint32_t ch, uint32_t xfer_width)
{
    volatile uint32_t *csr = LL_REG_CSR(dma, ch);
    LL_REG_CBIT(*csr, DMA_CSR_DST_WIDTH);
    LL_REG_SBIT(*csr, ((xfer_width << DMA_CSR_DST_WIDTH_Pos) & DMA_CSR_DST_WIDTH));
}

/**
 * @brief  Get destination transfer width
 * @param  dma DMA Instance
 * @param  ch This parameter can be one of @ref dma_channel
 * @retval Returned value can be one of @ref dma_xfr_width
 */
__STATIC_INLINE uint32_t LL_DMA_GetDST_TransferWidth(SN_DMA0_Type *dma, uint32_t ch)
{
    return (LL_REG_RBIT(*LL_REG_CSR(dma, ch), DMA_CSR_DST_WIDTH) >> DMA_CSR_DST_WIDTH_Pos);
}

/**
 * @brief  Set source transfer width
 * @param  dma DMA Instance
 * @param  ch This parameter can be one of @ref dma_channel
 * @param  xfer_width This parameter can be one of @ref dma_xfr_width
 * @retval none
 */
__STATIC_INLINE void LL_DMA_SetSRC_TransferWidth(SN_DMA0_Type *dma, uint32_t ch, uint32_t xfer_width)
{
    volatile uint32_t *csr = LL_REG_CSR(dma, ch);
    LL_REG_CBIT(*csr, DMA_CSR_SRC_WIDTH);
    LL_REG_SBIT(*csr, ((xfer_width << DMA_CSR_SRC_WIDTH_Pos) & DMA_CSR_SRC_WIDTH));
}

/**
 * @brief  Get source transfer width
 * @param  dma DMA Instance
 * @param  ch This parameter can be one of @ref dma_channel
 * @retval Returned value can be one of @ref dma_xfr_width
 */
__STATIC_INLINE uint32_t LL_DMA_GetSRC_TransferWidth(SN_DMA0_Type *dma, uint32_t ch)
{
    return (LL_REG_RBIT(*LL_REG_CSR(dma, ch), DMA_CSR_SRC_WIDTH) >> DMA_CSR_SRC_WIDTH_Pos);
}

/**
 * @brief  Abort transaction
 * @param  dma DMA Instance
 * @param  ch This parameter can be one of @ref dma_channel
 * @retval none
 */
__STATIC_INLINE void LL_DMA_Abort(SN_DMA0_Type *dma, uint32_t ch)
{
    LL_REG_SBIT(*LL_REG_CSR(dma, ch), DMA_CSR_ABT);
}

/**
 * @brief  Set source burst size
 * @param  dma DMA Instance
 * @param  ch This parameter can be one of @ref dma_channel
 * @param  burst_size This parameter can be one of @ref dma_source_burst
 * @retval none
 */
__STATIC_INLINE void LL_DMA_SetSRC_BurstSize(SN_DMA0_Type *dma, uint32_t ch, uint32_t burst_size)
{
    volatile uint32_t *csr = LL_REG_CSR(dma, ch);
    LL_REG_CBIT(*csr, DMA_CSR_SRC_SIZE);
    LL_REG_SBIT(*csr, ((burst_size << DMA_CSR_SRC_SIZE_Pos) & DMA_CSR_SRC_SIZE));
}

/**
 * @brief  Get source burst size
 * @param  dma DMA Instance
 * @param  ch This parameter can be one of @ref dma_channel
 * @retval Returned value can be one of @ref dma_source_burst
 */
__STATIC_INLINE uint32_t LL_DMA_GetSRC_BurstSize(SN_DMA0_Type *dma, uint32_t ch)
{
    return (LL_REG_RBIT(*LL_REG_CSR(dma, ch), DMA_CSR_SRC_SIZE) >> DMA_CSR_SRC_SIZE_Pos);
}

/**
 * @brief  Set channel priority level
 * @param  dma DMA Instance
 * @param  ch This parameter can be one of @ref dma_channel
 * @param  priority This parameter can be one of @ref dma_priority
 * @retval none
 */
__STATIC_INLINE void LL_DMA_SetChannelPriority(SN_DMA0_Type *dma, uint32_t ch, uint32_t priority)
{
    volatile uint32_t *csr = LL_REG_CSR(dma, ch);
    LL_REG_CBIT(*csr, DMA_CSR_CHPRI);
    LL_REG_SBIT(*csr, ((priority << DMA_CSR_CHPRI_Pos) & DMA_CSR_CHPRI));
}

/**
 * @brief  Get channel priority level
 * @param  dma DMA Instance
 * @param  ch This parameter can be one of @ref dma_channel
 * @retval Returned value can be one of @ref dma_priority
 */
__STATIC_INLINE uint32_t LL_DMA_GetChannelPriority(SN_DMA0_Type *dma, uint32_t ch)
{
    return (LL_REG_RBIT(*LL_REG_CSR(dma, ch), DMA_CSR_CHPRI) >> DMA_CSR_CHPRI_Pos);
}

/**
 * @brief  Set FIFO threshold value
 * @param  dma DMA Instance
 * @param  ch This parameter can be one of @ref dma_channel
 * @param  fifo_th This parameter can be one of @ref dma_fifo_threshold
 * @retval none
 */
__STATIC_INLINE void LL_DMA_SetFIFOTH(SN_DMA0_Type *dma, uint32_t ch, uint32_t fifo_th)
{
    volatile uint32_t *csr = LL_REG_CSR(dma, ch);
    LL_REG_CBIT(*csr, DMA_CSR_DMA_FF_TH);
    LL_REG_SBIT(*csr, ((fifo_th << DMA_CSR_DMA_FF_TH_Pos) & DMA_CSR_DMA_FF_TH));
}

/**
 * @brief  Get FIFO threshold value
 * @param  dma DMA Instance
 * @param  ch This parameter can be one of @ref dma_channel
 * @retval Returned value can be one of @ref dma_fifo_threshold
 */
__STATIC_INLINE uint32_t LL_DMA_GetFIFOTH(SN_DMA0_Type *dma, uint32_t ch)
{
    return (LL_REG_RBIT(*LL_REG_CSR(dma, ch), DMA_CSR_DMA_FF_TH) >> DMA_CSR_DMA_FF_TH_Pos);
}

/**
 * @brief  Set terminal count status mask for current transaction
 * @note   When terminal count happens, TC status register will be set
 * @param  dma DMA Instance
 * @param  ch This parameter can be one of @ref dma_channel
 * @retval none
 */
__STATIC_INLINE void LL_DMA_SetMask_TC(SN_DMA0_Type *dma, uint32_t ch)
{
    LL_REG_CBIT(*LL_REG_CSR(dma, ch), DMA_CSR_DMA_TC_MSK);
}

/**
 * @brief  Clear terminal count status mask for current transaction
 * @note   When terminal count happens, TC status register will not be set
 * @param  dma DMA Instance
 * @param  ch This parameter can be one of @ref dma_channel
 * @retval none
 */
__STATIC_INLINE void LL_DMA_ClearMask_TC(SN_DMA0_Type *dma, uint32_t ch)
{
    LL_REG_SBIT(*LL_REG_CSR(dma, ch), DMA_CSR_DMA_TC_MSK);
}

/**
 * @brief  Set terminal count interrupt mask
 * @param  dma DMA Instance
 * @param  ch This parameter can be one of @ref dma_channel
 * @retval none
 */
__STATIC_INLINE void LL_DMA_SetMask_INT_TC(SN_DMA0_Type *dma, uint32_t ch)
{
    LL_REG_SBIT(*LL_REG_CFG(dma, ch), DMA_CFG_INT_TC_MSK);
}

/**
 * @brief  Clear terminal count interrupt mask
 * @param  dma DMA Instance
 * @param  ch This parameter can be one of @ref dma_channel
 * @retval none
 */
__STATIC_INLINE void LL_DMA_ClearMask_INT_TC(SN_DMA0_Type *dma, uint32_t ch)
{
    LL_REG_CBIT(*LL_REG_CFG(dma, ch), DMA_CFG_INT_TC_MSK);
}

/**
 * @brief  Set error interrupt mask
 * @param  dma DMA Instance
 * @param  ch This parameter can be one of @ref dma_channel
 * @retval none
 */
__STATIC_INLINE void LL_DMA_SetMask_INT_ERR(SN_DMA0_Type *dma, uint32_t ch)
{
    LL_REG_SBIT(*LL_REG_CFG(dma, ch), DMA_CFG_INT_ERR_MSK);
}

/**
 * @brief  Clear error interrupt mask
 * @param  dma DMA Instance
 * @param  ch This parameter can be one of @ref dma_channel
 * @retval none
 */
__STATIC_INLINE void LL_DMA_ClearMask_INT_ERR(SN_DMA0_Type *dma, uint32_t ch)
{
    LL_REG_CBIT(*LL_REG_CFG(dma, ch), DMA_CFG_INT_ERR_MSK);
}

/**
 * @brief  Set abort interrupt mask
 * @param  dma DMA Instance
 * @param  ch This parameter can be one of @ref dma_channel
 * @retval none
 */
__STATIC_INLINE void LL_DMA_SetMask_INT_ABT(SN_DMA0_Type *dma, uint32_t ch)
{
    LL_REG_SBIT(*LL_REG_CFG(dma, ch), DMA_CFG_INT_ABT_MSK);
}

/**
 * @brief  Clear abort interrupt mask
 * @param  dma DMA Instance
 * @param  ch This parameter can be one of @ref dma_channel
 * @retval none
 */
__STATIC_INLINE void LL_DMA_ClearMask_INT_ABT(SN_DMA0_Type *dma, uint32_t ch)
{
    LL_REG_CBIT(*LL_REG_CFG(dma, ch), DMA_CFG_INT_ABT_MSK);
}

/**
 * @brief  Enable source peripheral mode
 * @param  dma DMA Instance
 * @param  ch This parameter can be one of @ref dma_channel
 * @retval none
 */
__STATIC_INLINE void LL_DMA_Enable_SRCIPMode(SN_DMA0_Type *dma, uint32_t ch)
{
    LL_REG_SBIT(*LL_REG_CFG(dma, ch), DMA_CFG_SRC_HE);
}

/**
 * @brief  Disable source peripheral mode
 * @param  dma DMA Instance
 * @param  ch This parameter can be one of @ref dma_channel
 * @retval none
 */
__STATIC_INLINE void LL_DMA_Disable_SRCIPMode(SN_DMA0_Type *dma, uint32_t ch)
{
    LL_REG_CBIT(*LL_REG_CFG(dma, ch), DMA_CFG_SRC_HE);
}

/**
 * @brief  Return if source peripheral mode is enabled
 * @param  dma DMA Instance
 * @param  ch This parameter can be one of @ref dma_channel
 * @retval State of bit (1 or 0).
 */
__STATIC_INLINE uint32_t LL_DMA_IsEnabled_SRCIPMode(SN_DMA0_Type *dma, uint32_t ch)
{
    return (LL_REG_RBIT(*LL_REG_CFG(dma, ch), DMA_CFG_SRC_HE) == (DMA_CFG_SRC_HE));
}

/**
 * @brief  Return if channel is busy
 * @param  dma DMA Instance
 * @param  ch This parameter can be one of @ref dma_channel
 * @retval State of bit (1 or 0).
 */
__STATIC_INLINE uint32_t LL_DMA_IsBusy(SN_DMA0_Type *dma, uint32_t ch)
{
    return (LL_REG_RBIT(*LL_REG_CFG(dma, ch), DMA_CFG_BUSY) == (DMA_CFG_BUSY));
}

/**
 * @brief  Enable destination peripheral mode
 * @param  dma DMA Instance
 * @param  ch This parameter can be one of @ref dma_channel
 * @retval none
 */
__STATIC_INLINE void LL_DMA_Enable_DSTIPMode(SN_DMA0_Type *dma, uint32_t ch)
{
    LL_REG_SBIT(*LL_REG_CFG(dma, ch), DMA_CFG_DST_HE);
}

/**
 * @brief  Disable destination peripheral mode
 * @param  dma DMA Instance
 * @param  ch This parameter can be one of @ref dma_channel
 * @retval none
 */
__STATIC_INLINE void LL_DMA_Disable_DSTIPMode(SN_DMA0_Type *dma, uint32_t ch)
{
    LL_REG_CBIT(*LL_REG_CFG(dma, ch), DMA_CFG_DST_HE);
}

/**
 * @brief  Return if destination peripheral mode is enabled
 * @param  dma DMA Instance
 * @param  ch This parameter can be one of @ref dma_channel
 * @retval State of bit (1 or 0).
 */
__STATIC_INLINE uint32_t LL_DMA_IsEnabled_DSTIPMode(SN_DMA0_Type *dma, uint32_t ch)
{
    return (LL_REG_RBIT(*LL_REG_CFG(dma, ch), DMA_CFG_DST_HE) == (DMA_CFG_DST_HE));
}

/**
 * @brief  Set source request
 * @param  dma DMA Instance
 * @param  ch This parameter can be one of @ref dma_channel
 * @param  req This parameter can be one of @ref dma_dma0_rs or @ref dma_dma1_rs
 * @retval none
 */
__STATIC_INLINE void LL_DMA_SetSRC_Request(SN_DMA0_Type *dma, uint32_t ch, uint32_t req)
{
    volatile uint32_t *cfg = LL_REG_CFG(dma, ch);
    LL_REG_CBIT(*cfg, DMA_CFG_SRC_RS);
    LL_REG_SBIT(*cfg, ((req << DMA_CFG_SRC_RS_Pos) & DMA_CFG_SRC_RS));
}

/**
 * @brief  Get source request
 * @param  dma DMA Instance
 * @param  ch This parameter can be one of @ref dma_channel
 * @retval Returned value can be one of @ref dma_dma0_rs or @ref dma_dma1_rs
 */
__STATIC_INLINE uint32_t LL_DMA_GetSRC_Request(SN_DMA0_Type *dma, uint32_t ch)
{
    return (LL_REG_RBIT(*LL_REG_CFG(dma, ch), DMA_CFG_SRC_RS) >> DMA_CFG_SRC_RS_Pos);
}

/**
 * @brief  Set destination request
 * @param  dma DMA Instance
 * @param  ch This parameter can be one of @ref dma_channel
 * @param  req This parameter can be one of @ref dma_dma0_rs or @ref dma_dma1_rs
 * @retval none
 */
__STATIC_INLINE void LL_DMA_SetDST_Request(SN_DMA0_Type *dma, uint32_t ch, uint32_t req)
{
    volatile uint32_t *cfg = LL_REG_CFG(dma, ch);
    LL_REG_CBIT(*cfg, DMA_CFG_DST_RS);
    LL_REG_SBIT(*cfg, ((req << DMA_CFG_DST_RS_Pos) & DMA_CFG_DST_RS));
}

/**
 * @brief  Get destination request
 * @param  dma DMA Instance
 * @param  ch This parameter can be one of @ref dma_channel
 * @retval Returned value can be one of @ref dma_dma0_rs or @ref dma_dma1_rs
 */
__STATIC_INLINE uint32_t LL_DMA_GetDST_Request(SN_DMA0_Type *dma, uint32_t ch)
{
    return (LL_REG_RBIT(*LL_REG_CFG(dma, ch), DMA_CFG_DST_RS) >> DMA_CFG_DST_RS_Pos);
}

/**
 * @brief  Set source starting address
 * @param  dma DMA Instance
 * @param  ch This parameter can be one of @ref dma_channel
 * @param  addr Transfer starting address for source
 * @retval none
 */
__STATIC_INLINE void LL_DMA_SetSRC_Address(SN_DMA0_Type *dma, uint32_t ch, uint32_t addr)
{
    LL_REG_WRITE(*LL_REG_SRCADDR(dma, ch), addr);
}

/**
 * @brief  Get source starting address
 * @param  dma DMA Instance
 * @param  ch This parameter can be one of @ref dma_channel
 * @retval Transfer starting address for source
 */
__STATIC_INLINE uint32_t LL_DMA_GetSRC_Address(SN_DMA0_Type *dma, uint32_t ch)
{
    return LL_REG_READ(*LL_REG_SRCADDR(dma, ch));
}

/**
 * @brief  Set destination starting address
 * @param  dma DMA Instance
 * @param  ch This parameter can be one of @ref dma_channel
 * @param  addr Transfer starting address for destination
 * @retval none
 */
__STATIC_INLINE void LL_DMA_SetDST_Address(SN_DMA0_Type *dma, uint32_t ch, uint32_t addr)
{
    LL_REG_WRITE(*LL_REG_DSTADDR(dma, ch), addr);
}

/**
 * @brief  Get destination starting address
 * @param  dma DMA Instance
 * @param  ch This parameter can be one of @ref dma_channel
 * @retval Transfer starting address for destination
 */
__STATIC_INLINE uint32_t LL_DMA_GetDST_Address(SN_DMA0_Type *dma, uint32_t ch)
{
    return LL_REG_READ(*LL_REG_DSTADDR(dma, ch));
}

/**
 * @brief  Set transfer size
 * @param  dma DMA Instance
 * @param  ch This parameter can be one of @ref dma_channel
 * @param  size Transfer size
 * @retval none
 */
__STATIC_INLINE void LL_DMA_SetSize(SN_DMA0_Type *dma, uint32_t ch, uint32_t size)
{
    LL_REG_WRITE(*LL_REG_SIZE(dma, ch), size);
}

/**
 * @brief  Get transfer size
 * @param  dma DMA Instance
 * @param  ch This parameter can be one of @ref dma_channel
 * @retval Transfer size
 */
__STATIC_INLINE uint32_t LL_DMA_GetSize(SN_DMA0_Type *dma, uint32_t ch)
{
    return LL_REG_READ(*LL_REG_SIZE(dma, ch));
}

/**
 * @brief  Return if interrupt is active
 * @note   INT | INT_ERR | INT_TC
 * @param  dma DMA Instance
 * @param  ch This parameter can be one of @ref dma_channel
 * @retval State of bit (1 or 0).
 */
__STATIC_INLINE uint32_t LL_DMA_IsActiveFlag_INT(SN_DMA0_Type *dma, uint32_t ch)
{
    uint32_t mask = 0x01 << ch;
    return (LL_REG_RBIT(dma->INT, mask) == (mask));
}

/**
 * @brief  Return if terminal count interrupt is active
 * @param  dma DMA Instance
 * @param  ch This parameter can be one of @ref dma_channel
 * @retval State of bit (1 or 0).
 */
__STATIC_INLINE uint32_t LL_DMA_IsActiveFlag_INT_TC(SN_DMA0_Type *dma, uint32_t ch)
{
    uint32_t mask = 0x01 << ch;
    return (LL_REG_RBIT(dma->INT_TC, mask) == (mask));
}

/**
 * @brief  Clear interrupt of terminal count
 * @param  dma DMA Instance
 * @param  ch This parameter can be one of @ref dma_channel
 * @retval None
 */
__STATIC_INLINE void LL_DMA_ClearFlag_INT_TC(SN_DMA0_Type *dma, uint32_t ch)
{
    uint32_t mask = 0x01 << ch;
    LL_REG_SBIT(dma->INT_TC_CLR, mask);
}

/**
 * @brief  Return if error interrupt is active
 * @param  dma DMA Instance
 * @param  ch This parameter can be one of @ref dma_channel
 * @retval State of bit (1 or 0).
 */
__STATIC_INLINE uint32_t LL_DMA_IsActiveFlag_INT_ERR(SN_DMA0_Type *dma, uint32_t ch)
{
    uint32_t mask = 0x01 << ch;
    return (LL_REG_RBIT(dma->INT_ERR_ABT, mask) == (mask));
}

/**
 * @brief  Clear interrupt of error
 * @param  dma DMA Instance
 * @param  ch This parameter can be one of @ref dma_channel
 * @retval None
 */
__STATIC_INLINE void LL_DMA_ClearFlag_INT_ERR(SN_DMA0_Type *dma, uint32_t ch)
{
    uint32_t mask = 0x01 << ch;
    LL_REG_SBIT(dma->INT_ERR_ABT_CLR, mask);
}

/**
 * @brief  Return if abort interrupt is active
 * @param  dma DMA Instance
 * @param  ch This parameter can be one of @ref dma_channel
 * @retval State of bit (1 or 0).
 */
__STATIC_INLINE uint32_t LL_DMA_IsActiveFlag_INT_ABT(SN_DMA0_Type *dma, uint32_t ch)
{
    uint32_t mask = 0x01 << (ch + 16);
    return (LL_REG_RBIT(dma->INT_ERR_ABT, mask) == (mask));
}

/**
 * @brief  Clear interrupt of abort
 * @param  dma DMA Instance
 * @param  ch This parameter can be one of @ref dma_channel
 * @retval None
 */
__STATIC_INLINE void LL_DMA_ClearFlag_INT_ABT(SN_DMA0_Type *dma, uint32_t ch)
{
    uint32_t mask = 0x01 << (ch + 16);
    LL_REG_SBIT(dma->INT_ERR_ABT_CLR, mask);
}

/**
 * @brief  Return the status of terminal count
 * @param  dma DMA Instance
 * @param  ch This parameter can be one of @ref dma_channel
 * @retval State of bit (1 or 0).
 */
__STATIC_INLINE uint32_t LL_DMA_IsActiveFlag_TC(SN_DMA0_Type *dma, uint32_t ch)
{
    uint32_t mask = 0x01 << ch;
    return (LL_REG_RBIT(dma->TC, mask) == (mask));
}

/**
 * @brief  Return the status of error
 * @param  dma DMA Instance
 * @param  ch This parameter can be one of @ref dma_channel
 * @retval State of bit (1 or 0).
 */
__STATIC_INLINE uint32_t LL_DMA_IsActiveFlag_ERR(SN_DMA0_Type *dma, uint32_t ch)
{
    uint32_t mask = 0x01 << ch;
    return (LL_REG_RBIT(dma->ERR_ABT, mask) == (mask));
}

/**
 * @brief  Return the status of abort
 * @param  dma DMA Instance
 * @param  ch This parameter can be one of @ref dma_channel
 * @retval State of bit (1 or 0).
 */
__STATIC_INLINE uint32_t LL_DMA_IsActiveFlag_ABT(SN_DMA0_Type *dma, uint32_t ch)
{
    uint32_t mask = 0x01 << (ch + 16);
    return (LL_REG_RBIT(dma->ERR_ABT, mask) == (mask));
}

/**
 * @brief  Return the status of enablement
 * @param  dma DMA Instance
 * @param  ch This parameter can be one of @ref dma_channel
 * @retval State of bit (1 or 0).
 */
__STATIC_INLINE uint32_t LL_DMA_IsActiveFlag_EN(SN_DMA0_Type *dma, uint32_t ch)
{
    uint32_t mask = 0x01 << ch;
    return (LL_REG_RBIT(dma->CH_EN, mask) == (mask));
}

/**
 * @brief  Return the status of busy
 * @param  dma DMA Instance
 * @param  ch This parameter can be one of @ref dma_channel
 * @retval State of bit (1 or 0).
 */
__STATIC_INLINE uint32_t LL_DMA_IsActiveFlag_Busy(SN_DMA0_Type *dma, uint32_t ch)
{
    uint32_t mask = 0x01 << ch;
    return (LL_REG_RBIT(dma->CH_BUSY, mask) == (mask));
}

/**
 * @brief  Enable the synchronization logic
 * @param  dma DMA Instance
 * @param  ch This parameter can be one of @ref dma_channel
 * @retval none
 */
__STATIC_INLINE void LL_DMA_Enable_Sync(SN_DMA0_Type *dma, uint32_t ch)
{
    uint32_t mask = 0x01 << ch;
    LL_REG_SBIT(dma->SYNC, mask);
}

/**
 * @brief  Disable the synchronization logic
 * @param  dma DMA Instance
 * @param  ch This parameter can be one of @ref dma_channel
 * @retval none
 */
__STATIC_INLINE void LL_DMA_Disable_Sync(SN_DMA0_Type *dma, uint32_t ch)
{
    uint32_t mask = 0x01 << ch;
    LL_REG_CBIT(dma->SYNC, mask);
}

/**
 * @brief  Return if synchronization logic is enabled
 * @param  dma DMA Instance
 * @param  ch This parameter can be one of @ref dma_channel
 * @retval State of bit (1 or 0).
 */
__STATIC_INLINE uint32_t LL_DMA_IsEnabled_Sync(SN_DMA0_Type *dma, uint32_t ch)
{
    uint32_t mask = 0x01 << ch;
    return (LL_REG_RBIT(dma->SYNC, mask) == (mask));
}

#if defined(USE_FULL_LL_DRIVER)
LL_Status_T LL_DMA_Init(SN_DMA0_Type *dma, LL_DMA_Init_T *dma_init);
LL_Status_T LL_DMA_DeInit(SN_DMA0_Type *dma);
void        LL_DMA_StructInit(LL_DMA_Init_T *dma_init);
#endif /* USE_FULL_LL_DRIVER */

#ifdef __cplusplus
}
#endif

#endif /* _LL_DMA_H_ */
