/***********************
 * SN34F78X SDK V2.1.4 *
 ***********************/

#ifndef _HAL_ADC_H_
#define _HAL_ADC_H_

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "sn34f78x_hal_def.h"

/* Exported types ------------------------------------------------------------*/
/**
 * @brief ADC Init Structure Definition
 * \ingroup adc_struct_define
 */
typedef struct
{
    uint32_t conv_mode;   /**< Specifies the ADC conversion mode.
                               This parameter can be a value of @ref adc_conv_mode.  */
    uint32_t scan_ch_num; /**< Specifies the ADC scan conversion channel numbers in each scan.
                               This parameter must be a number between Min_Data = 0x01 and Max_Data = 0x12.
                               If \b conv_mode is ADC_CVMODE_SINGLE_STEP, it should be set 0x01. */
    uint32_t autopwdn;    /**< Specifies ADC Auto Power down Mode. default valve is HAL_DISABLE.
                               This parameter only invalid when \b conv_mode is @ref ADC_CONVMODE_SINGLE_STEP or @ref ADC_CONVMODE_SINGLE_SCAN.
                               If it is @ref HAL_ENABLE, ADC automatically enters the power-down mode when ADC scan conversion finishes
                               This parameter can be a value of @ref HAL_DISABLE and @ref HAL_ENABLE. */
    uint32_t pre_clock;   /**< Specifies the ADC prescaler clock.
                               This parameter must be a number between Min_Data = 0x01 and Max_Data = 0xFF.*/
} ADC_Init_T;

/**
 * @brief ADC Handle Structure Definition
 * \ingroup adc_struct_define
 */
typedef struct __ADC_Handle_T
{
    SN_ADC0_Type *instance;       /**< Register base address */
    ADC_Init_T    init;           /**< ADC required parameters */
    HAL_State_T   state;          /**< ADC state */
    uint8_t       state_ext;      /**< ADC extern state */
    HAL_MUTEX     mutex;          /**< ADC process protection */
    uint32_t      error_code;     /**< ADC error code */
    uint8_t       active_channel; /**< ADC channel conversion complete or channel trigger threshold. It refer to @ref adc_channel */
#if (configUSE_ADC_NONBLOCKING_DMA == 1)
    DMA_Handle_T *hdma[4];           /**< ADC DMA Handle parameters. This array is accessed by a @ref adc_dma_channel */
    uint32_t      dma_channel_state; /**< ADC DMA channel state */
#endif

#if (configUSE_ADC_REGISTER_CALLBACKS == 1)
    void (*MspInitCallback)(struct __ADC_Handle_T *hadc);          /**< ADC Msp Init Callback */
    void (*MspDeInitCallback)(struct __ADC_Handle_T *hadc);        /**< ADC Msp DeInit Callback */
    void (*ConvCpltCallback)(struct __ADC_Handle_T *hadc);         /**< ADC conversion complete callback */
    void (*StopCallback)(struct __ADC_Handle_T *hadc);             /**< ADC conversion stop callback */
    void (*LevelOutOfWindowCallback)(struct __ADC_Handle_T *hadc); /**< ADC analog watchdog callback */
    void (*ErrorCallback)(struct __ADC_Handle_T *hadc);            /**< ADC Error Callback */
#endif
} ADC_Handle_T;

/**
 * @brief ADC Timing Structure Definition
 * \ingroup adc_struct_define
 */
typedef struct __ADC_Timing_T
{
    uint32_t power_en_time; /**< Specifies the ADC power enable cycle number of MCLK, c_pweren = (power_en_time+1) * MCLK
                                 This parameter must be a number between Min_Data = 0x00 and Max_Data = 0xFFFF. */
    uint32_t ch_delay_time; /**< Specifies the delay time cycle number of MCLK for each channel sampling, c_chdly = (ch_delay_time+1) * Tmclk
                                 This parameter must be a number between Min_Data = 0x00 and Max_Data = 0xFF. */
    uint32_t ch_sel_time;   /**< Specifies the analog input channel change discharge cycle number of MCLK, c_chsel = (ch_sel_time+1) * Tmclk
                                 This parameter must be a number between Min_Data = 0x00 and Max_Data = 0x7. */
    uint32_t hold_time;     /**< Specifies the analog input channel hold cycle number of MCLK, c_hold = hold_time * Tmclk
                                 This parameter must be a number between Min_Data = 0x00 and Max_Data = 0x6. */
    uint32_t sampl_time;    /**< Specifies the analog input channel sample clock high level cycle number of MCLK, c_sampl = (sampl_time+1) * Tmclk
                                 This parameter must be a number between Min_Data = 0x00 and Max_Data = 0xF. */
    uint32_t settl_time;    /**< Specifies the analog input channel settling cycle number of MCLK, c_settl = (settl_time+1) * Tmclk
                                 This parameter must be a number between Min_Data = 0x00 and Max_Data = 0xFF. */
    uint32_t disch_time;    /**< Specifies the sensing discharge cycle number of MCLK, c_disch = (disch_time+1) * Tmclk
                                 This parameter must be a number between Min_Data = 0x00 and Max_Data = 0xFFFF. */
} ADC_Timing_T;

/**
 * @brief ADC Channel Structure Definition
 * \ingroup adc_struct_define
 */
typedef struct __ADC_Channel_T
{
    uint32_t channel; /**< Specifies the channel to configure into ADC regular group.
                           This parameter can be a value of @ref adc_channel */
    uint32_t rank;    /**< Specifies the rank in the regular group sequencer.
                           This parameter must be a number between Min_Data = 0x01 and Max_Data = 0x12 .
                           If \b conv_mode is ADC_MODE_SINGLE_STEP, it should be set 0x01 */
} ADC_Channel_T;

/**
 * @brief ADC Window WatchDog Structure Definition
 * \ingroup adc_struct_define
 */
typedef struct __ADC_AnalogWDG_T
{
    uint8_t enable;            /**< Specifies which enable/disable detection of current channel
                                    This parameter can be a value of @ref HAL_DISABLE and @ref HAL_ENABLE  */
    uint8_t channel;           /**< Specifies which channel will be detected.
                                    This parameter can be a value of @ref adc_channel */
    uint8_t watchdog_mode;     /**< Specifies the threshold watchdog mode.
                                    This parameter can be the ORed value of @ref adc_watchdog_mode.
                                    If want to simultaneously detect LTR and HTR in same channel, watchdog_mode = ADC_DETECT_LTR | ADC_DETECT_HTR */
    uint16_t lower_threshold;  /**< Specifies the Lower threshold value.
                                    This parameter only invalid when watchdog_mode has @ref ADC_DETECT_LTR.
                                    This parameter must be a number between Min_Data = 0x001 and Max_Data = 0xFFE . */
    uint16_t higher_threshold; /**< Specifies the Lower threshold value.
                                    This parameter only invalid when watchdog_mode has @ref ADC_DETECT_HTR.
                                    This parameter must be a number between Min_Data = 0x01 and Max_Data = 0xFFE . */
} ADC_AnalogWDG_T;

/**
 * @brief ADC DMA channel Structure Definition
 * \ingroup adc_struct_define
 */
typedef struct __ADC_DMA_Channel_T
{
    uint8_t dma_channel;    /**< Specifies the DMA channel.
                                 This parameter can be a value of @ref adc_dma_channel */
    uint8_t injected_mode;  /**< Specifies the DMA injected data mode.
                                 This parameter can be a value of @ref adc_dma_inject_mode. */
    uint8_t injected_ch;    /**< Specifies the channel for DMA injected data .
                                 This parameter only invalid when injected_mode is @ref ADC_DMA_INJECT_SINGLE_CH.
                                 This parameter can be a value of @ref adc_channel. */
    uint8_t fifo_threshold; /**< Specifies the DMA threshold configuration.
                                 This parameter can be a value of @ref adc_dma_fifo_threshold. */

} ADC_DMA_Channel_T;

/**
 * @brief HAL ADC Callback function definition
 * \ingroup adc_struct_define
 */
typedef void (*ADC_Callback_T)(ADC_Handle_T *hadc); /**< pointer to a ADC common callback functions  */

/**
 * @brief HAL ADC Callback ID enumeration definition
 * \ingroup adc_struct_define
 */
typedef enum
{
    HAL_ADC_MSPINIT_CB_ID   = 0x00U, /**< HAL ADC MspInit Callback ID */
    HAL_ADC_MSPDEINIT_CB_ID = 0x01U, /**< HAL ADC MspDeInit Callback ID */
    HAL_ADC_CONV_CPLT_CB_ID = 0x02U, /**< HAL ADC conversion complete Callback ID */
    HAL_ADC_STOP_CB_ID      = 0x03U, /**< HAL ADC conversion stop Callback ID */
    HAL_ADC_DETECT_CB_ID    = 0x04U, /**< HAL ADC analog detect Callback ID */
    HAL_ADC_ERROR_CB_ID     = 0x05U, /**< HAL ADC error Callback ID */
} HAL_ADC_CallbackID_T;

/**
 * \defgroup adc_conv_mode ADC conversion Mode
 * \ingroup adc_control
 * @{
 */
#define ADC_CONVMODE_SINGLE_STEP 0x00U /**< Single step conversion mode.  */
#define ADC_CONVMODE_SINGLE_SCAN 0x01U /**< Single scan conversion mode. */
#define ADC_CONVMODE_LOOP_SCAN   0x02U /**< Continuous scan conversion mode. */
/**
 * @}
 */

/**
 * \defgroup adc_ref_option ADC reference option select
 * \ingroup adc_control
 * @{
 */
#define ADC_REF_EXTERNAL      0x00U /**< ADC in external reference. */
#define ADC_REF_VDD_GND       0x01U /**< ADC in VDD/GND reference mode. */
#define ADC_REF_INTERNAL_2_5V 0x02U /**< ADC in internal 2.5v reference mode. */
#define ADC_REF_INTERNAL_2_0V 0x06U /**< ADC in internal 2.0v reference mode. */
#define ADC_REF_INTERNAL_1_5V 0x0AU /**< ADC in internal 1.5v reference mode. */
/**
 * @}
 */

/**
 * \defgroup adc_channel ADC conversion channel
 * \ingroup adc_control
 * @{
 */
#define ADC_CHANNEL_0   0x00U /**<  ADC channel 0 */
#define ADC_CHANNEL_1   0x01U /**<  ADC channel 1 */
#define ADC_CHANNEL_2   0x02U /**<  ADC channel 2 */
#define ADC_CHANNEL_3   0x03U /**<  ADC channel 3 */
#define ADC_CHANNEL_4   0x04U /**<  ADC channel 4 */
#define ADC_CHANNEL_5   0x05U /**<  ADC channel 5 */
#define ADC_CHANNEL_6   0x06U /**<  ADC channel 6 */
#define ADC_CHANNEL_7   0x07U /**<  ADC channel 7 */
#define ADC_CHANNEL_8   0x08U /**<  ADC channel 8 */
#define ADC_CHANNEL_9   0x09U /**<  ADC channel 9 */
#define ADC_CHANNEL_10  0x0AU /**<  ADC channel 10 */
#define ADC_CHANNEL_11  0x0BU /**<  ADC channel 11 */
#define ADC_CHANNEL_12  0x0CU /**<  ADC channel 12 */
#define ADC_CHANNEL_13  0x0DU /**<  ADC channel 13 */
#define ADC_CHANNEL_14  0x0EU /**<  ADC channel 14 */
#define ADC_CHANNEL_15  0x0FU /**<  ADC channel 15 */
#define ADC_CHANNEL_VDD 0x10U /**<  ADC channel VDD */
#define ADC_CHANNEL_VSS 0x11U /**<  ADC channel VSS */
/**
 * @}
 */

/**
 * \defgroup adc_dma_channel ADC DMA channel
 * \ingroup adc_control
 * @{
 */
#define ADC_DMA_CHANNEL_0 0x00U /**<  ADC dma channel 0 */
#define ADC_DMA_CHANNEL_1 0x01U /**<  ADC dma channel 1 */
#define ADC_DMA_CHANNEL_2 0x02U /**<  ADC dma channel 2 */
#define ADC_DMA_CHANNEL_3 0x03U /**<  ADC dma channel 3 */
/**
 * @}
 */

/**
 * \defgroup adc_dma_inject_mode ADC DMA injected data mode
 * \ingroup adc_control
 * @{
 */
#define ADC_DMA_INJECT_SINGLE_CH 0x00U /**< Injected data by select channel /b injected_ch */
#define ADC_DMA_INJECT_ALL_CONV  0x01U /**< Injected data for all conversion */
/**
 * @}
 */

/**
 * \defgroup adc_dma_fifo_threshold ADC DMA threshold configuration
 * \ingroup adc_control
 * @{
 */
#define ADC_DMA_FIFO_4  0x00U /**< Issue DMA request after 4 data in FIFO */
#define ADC_DMA_FIFO_8  0x01U /**< Issue DMA request after 8 data in FIFO */
#define ADC_DMA_FIFO_16 0x02U /**< Issue DMA request after 16 data in FIFO */
#define ADC_DMA_FIFO_32 0x03U /**< Issue DMA request after 32 data in FIFO  */
/**
 * @}
 */

/**
 * \defgroup adc_watchdog_mode ADC Threshold WatchDog Mode
 * \ingroup adc_control
 * @{
 */
#define ADC_DETECT_LTR 0x01U /**< Enable the LTR watchdog mode under the threshold */
#define ADC_DETECT_HTR 0x02U /**< Enable the HTR watchdog mode over the threshold */

/**
 * @}
 */

/**
 * \defgroup adc_wakeup_cycles ADC wakeup cycles of ADC conversion
 * \ingroup adc_control
 * @{
 */
#define ADC_WKUP_3_CYCLES 0x03U /**< Waiting 3 ADC conversion cycle. */
#define ADC_WKUP_4_CYCLES 0x04U /**< Waiting 4 ADC conversion cycle. */
#define ADC_WKUP_5_CYCLES 0x05U /**< Waiting 5 ADC conversion cycle. */
#define ADC_WKUP_6_CYCLES 0x06U /**< Waiting 6 ADC conversion cycle. */
#define ADC_WKUP_7_CYCLES 0x07U /**< Waiting 7 ADC conversion cycle. */
/**
 * @}
 */

/* Exported functions --------------------------------------------------------*/
/* Initialization/de-initialization functions  ********************************/
HAL_Status_T HAL_ADC_Init(ADC_Handle_T *hadc);
HAL_Status_T HAL_ADC_DeInit(ADC_Handle_T *hadc);
void         HAL_ADC_MspInit(ADC_Handle_T *hadc);
void         HAL_ADC_MspDeInit(ADC_Handle_T *hadc);

/* Callbacks Register/UnRegister functions ************************************/
HAL_Status_T HAL_ADC_RegisterCallback(ADC_Handle_T *hadc, HAL_ADC_CallbackID_T callback_id, ADC_Callback_T callback);
HAL_Status_T HAL_ADC_UnRegisterCallback(ADC_Handle_T *hadc, HAL_ADC_CallbackID_T callback_id);

/* I/O operation functions ******************************************************/
HAL_Status_T HAL_ADC_Start_IT(ADC_Handle_T *hadc);
HAL_Status_T HAL_ADC_Stop_IT(ADC_Handle_T *hadc);

HAL_Status_T HAL_ADC_Start_DMA(ADC_Handle_T *hadc, uint32_t dma_channel, uint32_t *data, uint32_t length);
HAL_Status_T HAL_ADC_Stop_DMA(ADC_Handle_T *hadc, uint32_t dma_channel);

uint32_t HAL_ADC_GetValue(ADC_Handle_T *hadc, uint32_t ch);

/* Peripheral Control functions *************************************************/
HAL_Status_T HAL_ADC_ConfigTiming(ADC_Handle_T *hadc, ADC_Timing_T *timing);
HAL_Status_T HAL_ADC_ConfigChannel(ADC_Handle_T *hadc, ADC_Channel_T *ch);
HAL_Status_T HAL_ADC_ConfigAnalogWDG(ADC_Handle_T *hadc, ADC_AnalogWDG_T *analog_wdg);
HAL_Status_T HAL_ADC_SelectRefOption(ADC_Handle_T *hadc, uint32_t ref_option);
HAL_Status_T HAL_ADC_ConfigWakeup(ADC_Handle_T *hadc, uint32_t wakeup_time);
HAL_Status_T HAL_ADC_ConfigDMA_Channel(ADC_Handle_T *hadc, ADC_DMA_Channel_T *dma_channel);
HAL_Status_T HAL_ADC_Wakeup(ADC_Handle_T *hadc);
HAL_Status_T HAL_ADC_PowerDown(ADC_Handle_T *hadc);

/* Peripheral State functions ***************************************************/
HAL_State_T HAL_ADC_GetState(ADC_Handle_T *hadc);
uint32_t    HAL_ADC_GetError(ADC_Handle_T *hadc);
uint32_t    HAL_ADC_GetActiveChannel(ADC_Handle_T *hadc);

/* IRQ Handler functions ******************************************************/
void HAL_ADC_IRQHandler(ADC_Handle_T *hadc);

/* Event Callback functions ***************************************************/
void HAL_ADC_ConvCpltCallback(ADC_Handle_T *hadc);
void HAL_ADC_StopCallback(ADC_Handle_T *hadc);
void HAL_ADC_LevelOutOfWindowCallback(ADC_Handle_T *hadc);
void HAL_ADC_ErrorCallback(ADC_Handle_T *hadc);

#ifdef __cplusplus
}
#endif
#endif /* _HAL_ADC_H_ */
