/***********************
 * SN34F78X SDK V2.1.4 *
 ***********************/

#ifndef _HAL_DMA_H_
#define _HAL_DMA_H_

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "sn34f78x_hal_def.h"

/* Exported types ------------------------------------------------------------*/
// the DMA1 has different request content, but has the same prototype with DMA0
// so redefine the SN_DMA1 prototype to DMA0.
#undef SN_DMA1
#define SN_DMA1 ((SN_DMA0_Type *)SN_DMA1_BASE) /**< match type */

/**
 * @brief DMA Channel Configuration Structure Definition
 * \ingroup dma_struct_define
 */
typedef struct
{
    uint32_t channel;        /**< Specifies the channel used for the specified stream.
                                  This parameter can be a value of @ref dma_channel                                         */
    uint32_t ch_xfer_mode;   /**< Specifies the operation mode of the DMA stream.
                                  This parameter can be a value of @ref dma_xfer_mode                                       */
    uint32_t ch_priority;    /**< Specifies the software priority for the DMA stream.
                                  This parameter can be a value of @ref dma_ch_priority                                     */
    uint32_t ch_fifo_th;     /**< Specifies the FIFO threshold level.
                                  This parameter can be a value of @ref dma_ch_fifo_threshold                               */
    uint32_t ch_sync;        /**< Specifies the synchronization logic enable for channel.
                                  When DMA PCLK(AHB bus) is different from PCLK(APBn bus), current field need be enable,
                                  others keep disable.
                                  This parameter can be a value of \b HAL_ENABLE or \b HAL_DISABLE                          */
    uint32_t src_burst;      /**< Specifies the source burst size selection
                                  This parameter can be a value of @ref dma_ch_source_burst                                 */
    uint32_t src_rs;         /**< specifies the source request selection.
                                  This parameter can be a value of @ref dma_dma0_rs and  @ref dma_dma1_rs                   */
    uint32_t src_width;      /**< Specifies the source transfer width
                                  This parameter can be a value of @ref dma_ch_xfr_width                                    */
    uint32_t src_addr_mode;  /**< specifies the source address control.
                                  This parameter can be a value of @ref dma_ch_addr_mode                                    */
    uint32_t src_ahb_master; /**< Specifies the source AHB master
                                  This parameter can be a value of @ref dma_ch_ahb_master                                   */
    uint32_t dst_rs;         /**< specifies the destination request selection.
                                  This parameter can be a value of @ref dma_dma0_rs for DMA0 and  @ref dma_dma1_rs for DMA1 */
    uint32_t dst_width;      /**< Specifies the destination transfer width
                                  This parameter can be a value of @ref dma_ch_xfr_width                                    */
    uint32_t dst_addr_mode;  /**< specifies the destination address control.
                                  This parameter can be a value of @ref dma_ch_addr_mode                                    */
    uint32_t dst_ahb_master; /**< Specifies the destination AHB master
                                  This parameter can be a value of @ref dma_ch_ahb_master                                   */
} DMA_Init_T;

/**
 * @brief DMA Master Endian Definition
 * \ingroup dma_struct_define
 *
 */
typedef struct _master_endian
{
    uint32_t m0_endian; /**< Specifies the dma AHB Master 0 endian configuration.
                               This parameter can be a value of @ref dma_ahb_endian  */
    uint32_t m1_endian; /**< Specifies the dma AHB Master 1 endian configuration.
                                This parameter can be a value of @ref dma_ahb_endian */
} HAL_Endian_T;

/**
 * @brief DMA Handle Structure definition
 * \ingroup dma_struct_define
 */
typedef struct __DMA_Handle_T
{
    SN_DMA0_Type     *instance;                             /**< Register base address             */
    DMA_Init_T        init;                                 /**< DMA required parameters           */
    HAL_MUTEX         mutex;                                /**< DMA process protection            */
    __IOM HAL_State_T state;                                /**< DMA state                         */
    __IOM uint32_t    error_code;                           /**< DMA error code                    */
    uint32_t          xfer_size;                            /**< the total size of one transaction */
    void             *parent;                               /**< DMA parent object state           */
    void (*XferCpltCallback)(struct __DMA_Handle_T *hdma);  /**< DMA transfer done callback        */
    void (*XferErrorCallback)(struct __DMA_Handle_T *hdma); /**< DMA transfer error callback       */
    void (*XferAbortCallback)(struct __DMA_Handle_T *hdma); /**< DMA transfer Abort callback       */
    void (*MspInitCallback)(struct __DMA_Handle_T *hspi);   /**< DMA Msp Init callback             */
    void (*MspDeInitCallback)(struct __DMA_Handle_T *hspi); /**< DMA Msp DeInit callback           */
} DMA_Handle_T;

/**
 * @brief Define DMA Error Code.
 * \ingroup dma_error_code
 */
typedef enum
{
    HAL_DMA_ERROR_TE      = (HAL_ERROR_SPECIFY + 1U), /**< transfer error                       */
    HAL_DMA_ERROR_DME     = (HAL_ERROR_SPECIFY + 2U), /**< direct mode error                    */
    HAL_DMA_ERROR_NO_XFER = (HAL_ERROR_SPECIFY + 3U), /**< abort requested with no xfer ongoing */
} HAL_DMA_Error_T;

/**
 * @brief HAL DMA Callback ID enumeration definition
 * \ingroup dma_struct_define
 */
typedef enum
{
    HAL_DMA_XFER_CPLT_CB_ID,  /**< DMA full transfer            */
    HAL_DMA_XFER_ERROR_CB_ID, /**< DMA error                    */
    HAL_DMA_XFER_ABORT_CB_ID, /**< DMA abort                    */
    HAL_DMA_MSP_INIT_CB_ID,   /**< DMA Msp Init callback ID     */
    HAL_DMA_MSP_DEINIT_CB_ID, /**< DMA Msp DeInit callback ID   */
    HAL_DMA_ALL_CB_ID,        /**< Unregister all callback      */
} HAL_DMA_CallbackID_T;

/**
 * @brief HAL DMA Callback pointer definition
 * \ingroup dma_struct_define
 */
typedef void (*DMA_Callback_T)(DMA_Handle_T *hdma); /**< pointer to a peripheral handler which this handler must have DMA_Handle_T* object */

/* Exported constants --------------------------------------------------------*/
/**
 * \defgroup dma_channel DMA Channel
 * \ingroup dma_control
 * @{
 */
#define DMA_CHANNEL_0 0x00000000U /**< DMA Channel 0 */
#define DMA_CHANNEL_1 0x00000001U /**< DMA Channel 1 */
#define DMA_CHANNEL_2 0x00000002U /**< DMA Channel 2 */
#define DMA_CHANNEL_3 0x00000003U /**< DMA Channel 3 */
#define DMA_CHANNEL_4 0x00000004U /**< DMA Channel 4 */
#define DMA_CHANNEL_5 0x00000005U /**< DMA Channel 5 */
#define DMA_CHANNEL_6 0x00000006U /**< DMA Channel 6 */
#define DMA_CHANNEL_7 0x00000007U /**< DMA Channel 7 */
/**
 * @}
 */

/**
 * \defgroup dma_xfer_mode DMA Data Transfer Direction Mode
 * \ingroup dma_control
 * @{
 */
#define DMA_MEMORY_TO_MEMORY 0x00000000U /**< DMA direction from memory to memory         */
#define DMA_MEMORY_TO_PERIPH 0x00000001U /**< DMA direction from memory to peripheral     */
#define DMA_PERIPH_TO_MEMORY 0x00000002U /**< DMA direction from peripheral to memory     */
#define DMA_PERIPH_TO_PERIPH 0x00000003U /**< DMA direction from peripheral to peripheral */
/**
 * @}
 */

/**
 * \defgroup dma_dma0_rs DMA Request Selection of DMA0
 * \ingroup dma_control
 * @{
 */
#define DMA_RS0_UART1_TX  0x00000000U /**< DMA request select UART1 tx  */
#define DMA_RS0_UART1_RX  0x00000001U /**< DMA request select UART1 rx  */
#define DMA_RS0_UART2_TX  0x00000002U /**< DMA request select UART2 tx  */
#define DMA_RS0_UART2_RX  0x00000003U /**< DMA request select UART2 rx  */
#define DMA_RS0_UART3_TX  0x00000004U /**< DMA request select UART3 tx  */
#define DMA_RS0_UART3_RX  0x00000005U /**< DMA request select UART3 rx  */
#define DMA_RS0_UART4_TX  0x00000006U /**< DMA request select UART4 tx  */
#define DMA_RS0_UART4_RX  0x00000007U /**< DMA request select UART4 rx  */
#define DMA_RS0_UART1_FIR 0x00000008U /**< DMA request select UART1 fir */
#define DMA_RS0_UART2_FIR 0x00000009U /**< DMA request select UART2 fir */
#define DMA_RS0_UART3_FIR 0x0000000AU /**< DMA request select UART3 fir */
#define DMA_RS0_UART4_FIR 0x0000000BU /**< DMA request select UART4 fir */
#define DMA_RS0_TPM6_MR0  0x00000010U /**< DMA request select TPM6 MR0  */
#define DMA_RS0_TPM7_MR0  0x00000011U /**< DMA request select TPM7 MR0  */
/**
 * @}
 */

/**
 * \defgroup dma_dma1_rs DMA Request Selection of DMA1
 * \ingroup dma_control
 * @{
 */
#define DMA_RS1_SSP0_TX   0x00000000U /**< DMA request select SSP0 tx   */
#define DMA_RS1_SSP0_RX   0x00000001U /**< DMA request select SSP0 rx   */
#define DMA_RS1_SSP1_TX   0x00000002U /**< DMA request select SSP1 tx   */
#define DMA_RS1_SSP1_RX   0x00000003U /**< DMA request select SSP1 rx   */
#define DMA_RS1_SSP2_TX   0x00000004U /**< DMA request select SSP2 tx   */
#define DMA_RS1_SSP2_RX   0x00000005U /**< DMA request select SSP2 rx   */
#define DMA_RS1_UART0_TX  0x00000006U /**< DMA request select UART0 tx  */
#define DMA_RS1_UART0_RX  0x00000007U /**< DMA request select UART0 rx  */
#define DMA_RS1_UART5_TX  0x00000008U /**< DMA request select UART5 tx  */
#define DMA_RS1_UART5_RX  0x00000009U /**< DMA request select UART5 rx  */
#define DMA_RS1_ADC0_0    0x0000000AU /**< DMA request select ADC0_0    */
#define DMA_RS1_ADC0_1    0x0000000BU /**< DMA request select ADC0_1    */
#define DMA_RS1_ADC0_2    0x0000000CU /**< DMA request select ADC0_2    */
#define DMA_RS1_ADC0_3    0x0000000DU /**< DMA request select ADC0_3    */
#define DMA_RS1_SDIO      0x0000000EU /**< DMA request select SDIO      */
#define DMA_RS1_UART0_FIR 0x0000000FU /**< DMA request select UART0 fir */
#define DMA_RS1_UART5_FIR 0x00000010U /**< DMA request select UART5 fir */
#define DMA_RS1_LCM       0x00000011U /**< DMA request select LCM       */
#define DMA_RS1_TPM0_CAP0 0x00000014U /**< DMA request select TPM0 CAP0 */
#define DMA_RS1_TPM0_MR9  0x00000015U /**< DMA request select TPM0 MR9  */
#define DMA_RS1_TPM0_MR0  0x00000016U /**< DMA request select TPM0 MR0  */
#define DMA_RS1_TPM0_MR1  0x00000017U /**< DMA request select TPM0 MR1  */
#define DMA_RS1_TPM0_MR2  0x00000018U /**< DMA request select TPM0 MR2  */
#define DMA_RS1_TPM0_MR3  0x00000019U /**< DMA request select TPM0 MR3  */
#define DMA_RS1_TPM1_CAP0 0x0000001AU /**< DMA request select TPM1 CAP0 */
#define DMA_RS1_TPM1_MR9  0x0000001BU /**< DMA request select TPM1 MR9  */
#define DMA_RS1_TPM1_MR0  0x0000001CU /**< DMA request select TPM1 MR0  */
#define DMA_RS1_TPM1_MR1  0x0000001DU /**< DMA request select TPM1 MR1  */
#define DMA_RS1_TPM1_MR2  0x0000001EU /**< DMA request select TPM1 MR2  */
#define DMA_RS1_TPM1_MR3  0x0000001FU /**< DMA request select TPM1 MR3  */
#define DMA_RS1_TPM2_CAP0 0x00000020U /**< DMA request select TPM2 CAP0 */
#define DMA_RS1_TPM2_MR9  0x00000021U /**< DMA request select TPM2 MR9  */
#define DMA_RS1_TPM2_MR0  0x00000022U /**< DMA request select TPM2 MR0  */
#define DMA_RS1_TPM2_MR1  0x00000023U /**< DMA request select TPM2 MR1  */
#define DMA_RS1_TPM2_MR2  0x00000024U /**< DMA request select TPM2 MR2  */
#define DMA_RS1_TPM2_MR3  0x00000025U /**< DMA request select TPM2 MR3  */
#define DMA_RS1_TPM3_MR9  0x00000027U /**< DMA request select TPM3 MR9  */
#define DMA_RS1_TPM3_MR0  0x00000028U /**< DMA request select TPM3 MR0  */
#define DMA_RS1_TPM3_MR1  0x00000029U /**< DMA request select TPM3 MR1  */
#define DMA_RS1_TPM4_CAP0 0x0000002AU /**< DMA request select TPM4 CAP0 */
#define DMA_RS1_TPM4_MR9  0x0000002BU /**< DMA request select TPM4 MR9  */
#define DMA_RS1_TPM4_MR0  0x0000002CU /**< DMA request select TPM4 MR0  */
#define DMA_RS1_TPM4_MR1  0x0000002DU /**< DMA request select TPM4 MR1  */
#define DMA_RS1_TPM5_CAP0 0x0000002EU /**< DMA request select TPM5 CAP0 */
#define DMA_RS1_TPM5_MR9  0x0000002FU /**< DMA request select TPM5 MR9  */
#define DMA_RS1_TPM5_MR0  0x00000030U /**< DMA request select TPM5 MR0  */
#define DMA_RS1_TPM5_MR1  0x00000031U /**< DMA request select TPM5 MR1  */
#define DMA_RS1_TPM5_MR2  0x00000032U /**< DMA request select TPM5 MR2  */
#define DMA_RS1_TPM5_MR3  0x00000033U /**< DMA request select TPM5 MR3  */
#define DMA_RS1_TPM8_MR12 0x00000035U /**< DMA request select TPM8 MR12 */
/**
 * @}
 */

/**
 * \defgroup dma_ch_priority DMA Channel Priority
 * \ingroup dma_control
 * @{
 */
#define DMA_PRIORITY_LOW       0x00000000U /**< DMA priority level: low       */
#define DMA_PRIORITY_MEDIUM    0x00000001U /**< DMA priority level: medium    */
#define DMA_PRIORITY_HIGH      0x00000002U /**< DMA priority level: high      */
#define DMA_PRIORITY_VERY_HIGH 0x00000003U /**< DMA priority level: very high */
/**
 * @}
 */

/**
 * \defgroup dma_ch_source_burst DMA Channel Source Burst Size
 * \ingroup dma_control
 * @{
 */
#define DMA_SRC_BURST_1   0x00000000U /**< DMA source burst size: 1   */
#define DMA_SRC_BURST_4   0x00000001U /**< DMA source burst size: 4   */
#define DMA_SRC_BURST_8   0x00000002U /**< DMA source burst size: 8   */
#define DMA_SRC_BURST_16  0x00000003U /**< DMA source burst size: 16  */
#define DMA_SRC_BURST_32  0x00000004U /**< DMA source burst size: 32  */
#define DMA_SRC_BURST_64  0x00000005U /**< DMA source burst size: 64  */
#define DMA_SRC_BURST_128 0x00000006U /**< DMA source burst size: 128 */
#define DMA_SRC_BURST_256 0x00000007U /**< DMA source burst size: 256 */
/**
 * @}
 */

/**
 * \defgroup dma_ch_xfr_width DMA Channel Transfer Width for Source and Destination
 * \ingroup dma_control
 * @{
 */
#define DMA_XFR_WIDTH_8BITS  0x00000000U /**< DMA transfer width 8 bits  */
#define DMA_XFR_WIDTH_16BITS 0x00000001U /**< DMA transfer width 16 bits */
#define DMA_XFR_WIDTH_32BITS 0x00000002U /**< DMA transfer width 32 bits */
/**
 * @}
 */

/**
 * \defgroup dma_ch_addr_mode DMA Channel Address Mode
 * \ingroup dma_control
 * @{
 */
#define DMA_ADDR_MODE_INC    0x00000000U /**< DMA increment address        */
#define DMA_ADDR_MODE_DEC    0x00000001U /**< DMA decrement address        */
#define DMA_ADDR_MODE_FIXED  0x00000002U /**< DMA fixed address            */
#define DMA_ADDR_MODE_CYCLIC 0x00000003U /**< DMA increment cyclic address */
/**
 * @}
 */

/**
 * \defgroup dma_ch_fifo_threshold DMA FIFO Threshold Level
 * \ingroup  dma_control
 * @{
 */
#define DMA_FIFO_TH_1 0x00000000U /**< DMA FIFO threshold 1*32 bits */
#define DMA_FIFO_TH_2 0x00000001U /**< DMA FIFO threshold 2*32 bits */
#define DMA_FIFO_TH_4 0x00000002U /**< DMA FIFO threshold 4*32 bits */
/**
 * @}
 */

/**
 * \defgroup dma_ch_ahb_master DMA Channel AHB Master
 * \ingroup dma_control
 * @{
 */
#define DMA_AHB_MASTER_MATRIX 0x00000000U /**< DMA AHB master 0 ; for AHB Matrix            */
#define DMA_AHB_MASTER_PERIPH 0x00000001U /**< DMA AHB master 1 ; for APB1/APB2 peripheral  */
/**
 * @}
 */

/** \defgroup dma_ahb_endian DMA Channel AHB Endian Configuration
 * \ingroup dma_control
 * @{
 */
#define DMA_AHB_LITTLE_ENDIAN 0x00U /**< DMA AHB little endian */
#define DMA_AHB_BIG_ENDIAN    0x01U /**< DMA AHB big endian    */
/**
 * @}
 */

/**
 * \defgroup dma_xfer_cplt_lev DMA Transfer Completely Level
 * \ingroup dma_control
 * @{
 */
#define DMA_XFER_CPLT_FULL 0x00U /**< DMA xfer cplt full transfer */
#define DMA_XFER_CPLT_HALF 0x01U /**< DMA xfer cplt half transfer */
/**
 * @}
 */

/* Exported functions --------------------------------------------------------*/
/* Initialization/de-initialization functions *********************************/
HAL_Status_T HAL_DMA_Init(DMA_Handle_T *hdma);
HAL_Status_T HAL_DMA_DeInit(DMA_Handle_T *hdma);
void         HAL_DMA_MspInit(DMA_Handle_T *hdma);
void         HAL_DMA_MspDeInit(DMA_Handle_T *hdma);

/* Callbacks register/unRegister functions ************************************/
HAL_Status_T HAL_DMA_RegisterCallback(DMA_Handle_T *hdma, HAL_DMA_CallbackID_T callback_id, DMA_Callback_T callback);
HAL_Status_T HAL_DMA_UnRegisterCallback(DMA_Handle_T *hdma, HAL_DMA_CallbackID_T callback_id);

/* Configuration functions ****************************************************/
HAL_Status_T HAL_DMA_ConfigEndian(DMA_Handle_T *hdma, HAL_Endian_T *endian);
HAL_Status_T HAL_DMA_GetChannelStatus(DMA_Handle_T *hdma, uint32_t *ch_status);

/* I/O Operation functions ****************************************************/
HAL_Status_T HAL_DMA_Start(DMA_Handle_T *hdma, uint32_t src_addr, uint32_t dst_addr, uint32_t data_len);
HAL_Status_T HAL_DMA_Abort(DMA_Handle_T *hdma);
HAL_Status_T HAL_DMA_Resume(DMA_Handle_T *hdma);

HAL_Status_T HAL_DMA_Start_IT(DMA_Handle_T *hdma, uint32_t src_addr, uint32_t dst_addr, uint32_t data_len);
HAL_Status_T HAL_DMA_Abort_IT(DMA_Handle_T *hdma);
HAL_Status_T HAL_DMA_Resume_IT(DMA_Handle_T *hdma);

HAL_Status_T HAL_DMA_PollForTransfer(DMA_Handle_T *hdma, uint32_t cplt_lev, uint32_t timeout);

/* Peripheral state and errors functions **************************************/
HAL_State_T HAL_DMA_GetState(DMA_Handle_T *hdma);
uint32_t    HAL_DMA_GetError(DMA_Handle_T *hdma);
uint32_t    HAL_DMA_GetSize(DMA_Handle_T *hdma);

/* IRQ Handler functions ******************************************************/
void HAL_DMA_IRQHandler(DMA_Handle_T *hdma);

/* Event Callback functions ***************************************************/
void HAL_DMA_XferCpltCallback(DMA_Handle_T *hdma);
void HAL_DMA_XferErrorCallback(DMA_Handle_T *hdma);
void HAL_DMA_XferAbortCallback(DMA_Handle_T *hdma);
#ifdef __cplusplus
}
#endif

#endif /* _HAL_DMA_H_ */
