/***********************
 * SN34F78X SDK V2.1.4 *
 ***********************/

#ifndef _HAL_LCM_H_
#define _HAL_LCM_H_

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "sn34f78x_hal_def.h"

/* Exported types ------------------------------------------------------------*/

/**
 * @brief LCM init structure definition
 * \ingroup lcm_struct_define
 */
typedef struct
{
    uint8_t tpwhw;  /**< Tpwh width for write cycle=(TPWHW+1)*LC_PCLK clock cycle.
                        The programmable cycles is 1 up to 16 cycles  @ref lcm_timing. */
    uint32_t tpwl;  /**< Tpwl width=(TPWL+1)*LC_PCLK clock cycle
                        The programmable cycles is 1 up to 16 cycles  @ref lcm_timing. */
    uint32_t tah;   /**< Tah width=(TAH+1)*LC_PCLK clock cycle
                        The programmable cycles is 1 up to 16 cycles  @ref lcm_timing. */
    uint32_t tas;   /**< Tas width=(TAS+1)*LC_PCLK clock cycle.
                        The programmable cycles is 1 up to 16 cycles  @ref lcm_timing. */
    uint32_t tpwhr; /**< Tpwh width for read cycle=(TPWHR+1)*LC_PCLK clock cycle.
                        The programmable cycles is 1 up to 16 cycles  @ref lcm_timing. */

} LCM_Timing_T;

/**
 * @brief LCM init structure definition
 * \ingroup lcm_struct_define
 */
typedef struct
{
    uint32_t panel;     /**< Specifies panel type.
                             This parameter can be a value of @ref lcm_panel            */
    uint32_t bus_width; /**< Specifies the xfr data bits width on the bus.
                            This parameter can be a value of @ref lcm_bus_width         */
    uint32_t xfer_mode; /**< Specifies data transfer mode.
                           This parameter can be a value in range of @ref lcm_xfr_mode */
    uint32_t clk_pre;   /**< Specifies clock pre-scaler.
                             This parameter can be a value in range of @ref lcm_clk_pre */
} LCM_Init_T;

/**
 * @brief HAL_LCM_ERROR_T Define LCM Error Code
 * \ingroup hal_common_running_state
 */
typedef enum
{
    HAL_LCM_ERROR_DMAERR  = (HAL_ERROR_SPECIFY + 0x01U), /**< DAM transfer error during tx processing */
    HAL_LCM_ERROR_UDRUN   = (HAL_ERROR_SPECIFY + 0x02U), /**< DMA transfer under flow                 */
    HAL_LCM_ERROR_DATCNTE = (HAL_ERROR_SPECIFY + 0x03U), /**< DMA transfer data count is empty        */
} HAL_LCM_Error_T;

/**
 * @brief LCM handle structure definition
 * \ingroup lcm_struct_define
 */
typedef struct __LCM_Handle_T
{
    SN_LCM_Type      *instance;   /**< Register base address   */
    LCM_Init_T        init;       /**< LCM required parameters */
    HAL_MUTEX         mutex;      /**< LCM process protection  */
    __IOM HAL_State_T state;      /**< LCM state               */
    __IOM uint32_t    error_code; /**< LCM error code          */

#if (configUSE_LCM_NONBLOCKING_DMA == 1)
    DMA_Handle_T *hdma; /**< LCM DMA write handle parameters */
#endif

#if (configUSE_LCM_REGISTER_CALLBACKS == 1)
    void (*XferErrorCallback)(struct __LCM_Handle_T *hlcm); /**< LCM dma transfer error callback */
    void (*XferCpltCallback)(struct __LCM_Handle_T *hlcm);  /**< LCM transfer done callback      */
    void (*XferAbortCallback)(struct __LCM_Handle_T *hlcm); /**< LCM transfer done callback      */
    void (*MspInitCallback)(struct __LCM_Handle_T *hlcm);   /**< LCM MspInit callback            */
    void (*MspDeInitCallback)(struct __LCM_Handle_T *hlcm); /**< LCM MspDeInit callback          */
#endif
} LCM_Handle_T;

/**
 * @brief HAL LCM Callback ID enumeration definition
 * \ingroup lcm_struct_define
 */
typedef enum
{
    HAL_LCM_XFER_ERROR_CB_ID, /**< LCM DMA transfer data error callback id      */
    HAL_LCM_XFER_CPLT_CB_ID,  /**< LCM DMA transfer data completely callback id */
    HAL_LCM_XFER_ABORT_CB_ID, /**< LCM DMA transfer abort success callback id   */
    HAL_LCM_MSPINIT_CB_ID,    /**< LCM Msp DeInit callback ID                   */
    HAL_LCM_MSPDEINIT_CB_ID,  /**< LCM Msp DeInit callback ID                   */
    HAL_LCM_ALL_CB_ID         /**< Unregister all callback      */
} HAL_LCM_CallbackID_T;

/**
 * @brief HAL LCM Callback pointer definition
 * \ingroup lcm_struct_define
 */
typedef void (*LCM_Callback_T)(LCM_Handle_T *hlcm); /**< pointer to a LCM common callback functions */

/* Exported constants --------------------------------------------------------*/
/**
 * \defgroup lcm_panel LCM Pannel
 * \ingroup lcm_control
 * @{
 */
#define LCM_PANEL_MONO   0X00U /**< LCM mono panel  */
#define LCM_PANEL_16BITS 0x01U /**< LCM 16bits panel  */
#define LCM_PANEL_18BITS 0x02U /**< LCM 18bits panel */

/**
 * @}
 */

/**
 * \defgroup lcm_bus_width LCM Bus Width
 * \ingroup lcm_control
 * @{
 */
#define LCM_BUS_WIDTH_8BITS  0X00U /**< LCM bus 8bits interface  */
#define LCM_BUS_WIDTH_9BITS  0x01U /**< LCM bus 9bits interface  */
#define LCM_BUS_WIDTH_16BITS 0x02U /**< LCM bus 16bits interface */
#define LCM_BUS_WIDTH_18BITS 0x03U /**< LCM bus 18bits interface */
/**
 * @}
 */

/**
 * \defgroup lcm_xfr_mode LCM Transfer Mode
 * \ingroup lcm_control
 * @{
 */
#define LCM_XFER_ONE_16BITS 0X00U /**< LCM one transfer/pixel (65536colors)                                                               */
#define LCM_XFER_18BITS_MSB 0x01U /**< LCM Two transfers/pixel (262144colors), first transfer=16bits (MSB), other transfers = 2bits (LSB) */
#define LCM_XFER_18BITS_LSB 0x02U /**< LCM Two transfers/pixel(262144colors), first transfer=2bits (MSB), other transfers = 16bits (LSB)  */
/**
 * @}
 */

/**
 * \defgroup lcm_timing LCM Timing Constant Range (Unit: cycles)
 * \ingroup lcm_control
 * @{
 */
#define LCM_TIMING_MIN 1U /**< LCM timing programmable min-cycles is 1  */
#define LCM_TIMING_MAX 16 /**< LCM timing programmable max-cycles is 16 */
/**
 * @}
 */

/**
 * \defgroup lcm_clk_pre LCM Clock Prescaler
 * \ingroup lcm_control
 * @{
 */
#define LCM_CLK_DIV_1  0U /**< LCM LCMCLK/1  */
#define LCM_CLK_DIV_2  1U /**< LCM LCMCLK/2  */
#define LCM_CLK_DIV_4  2U /**< LCM LCMCLK/4  */
#define LCM_CLK_DIV_8  3U /**< LCM LCMCLK/8  */
#define LCM_CLK_DIV_16 4U /**< LCM LCMCLK/16 */
/**
 * @}
 */

/* Exported functions --------------------------------------------------------*/
/* Initialization/de-initialization functions *********************************/
HAL_Status_T HAL_LCM_Init(LCM_Handle_T *hlcm);
HAL_Status_T HAL_LCM_DeInit(LCM_Handle_T *hlcm);
void         HAL_LCM_MspInit(LCM_Handle_T *hlcm);
void         HAL_LCM_MspDeInit(LCM_Handle_T *hlcm);

/* Callbacks register/unRegister functions ************************************/
HAL_Status_T HAL_LCM_RegisterCallback(LCM_Handle_T *hlcm, HAL_LCM_CallbackID_T callback_id, LCM_Callback_T callback);
HAL_Status_T HAL_LCM_UnRegisterCallback(LCM_Handle_T *hlcm, HAL_LCM_CallbackID_T callback_id);

/* I/O Operation functions ****************************************************/
HAL_Status_T HAL_LCM_WriteCmd(LCM_Handle_T *hlcm, uint32_t cmd);
HAL_Status_T HAL_LCM_WriteData(LCM_Handle_T *hlcm, uint32_t data);

HAL_Status_T HAL_LCM_ReadReg(LCM_Handle_T *hlcm, uint32_t reg, uint32_t *data);
HAL_Status_T HAL_LCM_WriteReg(LCM_Handle_T *hlcm, uint32_t reg, uint32_t data);

HAL_Status_T HAL_LCM_WriteData_DMA(LCM_Handle_T *hlcm, uint32_t src_addr, uint16_t data_len);
HAL_Status_T HAL_LCM_WriteCmd_DMA(LCM_Handle_T *hlcm, uint32_t src_addr, uint16_t data_len);
HAL_Status_T HAL_LCM_DMAPause(LCM_Handle_T *hlcm);
HAL_Status_T HAL_LCM_DMAResume(LCM_Handle_T *hlcm);
HAL_Status_T HAL_LCM_DMAStop(LCM_Handle_T *hlcm);

/* Peripheral state and errors functions **************************************/
HAL_State_T HAL_LCM_GetState(LCM_Handle_T *hlcm);
uint32_t    HAL_LCM_GetError(LCM_Handle_T *hlcm);

/* IRQ Handler functions ******************************************************/
void HAL_LCM_IRQHandler(LCM_Handle_T *hlcm);

/* Event Callback functions ***************************************************/
void HAL_LCM_XferCpltCallback(LCM_Handle_T *hlcm);
void HAL_LCM_XferErrorCallback(LCM_Handle_T *hlcm);
void HAL_LCM_XferAbortCallback(LCM_Handle_T *hlcm);

#ifdef __cplusplus
}
#endif

#endif /* _HAL_LCM_H_ */
