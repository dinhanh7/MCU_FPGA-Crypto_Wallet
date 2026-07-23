/***********************
 * SN34F78X SDK V2.1.4 *
 ***********************/

#ifndef _HAL_TPM_H_
#define _HAL_TPM_H_

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "sn34f78x_hal_def.h"
/* Exported types ------------------------------------------------------------*/

/**
 * @brief TPM Init Structure Definition
 * \ingroup tpm_struct_define
 */
typedef struct
{
    uint32_t pre_scaler; /**< Specifies the Pre-Scaler value used to divide the HAL TPM clock.
                              This parameter can be a number between Min_Data = 0x00 and Max_Data = 0xFF.
                                    @note when \b PC(Pre-scale Counter) equal PreScaler, \b TC(TPMer Counter) will increment or reduce.   */
    uint32_t mode;       /**< Specifies the mode that decide when to increment or reduce \b TC or \b PC.
                               default valve is TPM_CTM_TIMER.
                               This parameter can be a value of @ref tpm_mode .
                                    @note in TPMer   mode, \b PC will increment or reduce at every rising PCLK edge.
                                    @note in counter mode, \b PC will increment or reduce at CAP pin rising(or falling, or both) edge.
                                    @note in counter mode, can not use capture function.
                                    @note in counter mode, the duration of the HIGH/LOW levels on the same CAP input
                                                               cannot be <b> shorter than 1 / (2 * PCLK). </b> */
    uint32_t cm;         /** Specifies the counting mode .
                             Default valve is TPM_CMMODE_UP_COUNTING.
                             This parameter can be a value of @ref tpm_cm_mode.*/

} TPM_Init_T;

/**
 * @brief TPM channel info Structure Definition
 * \ingroup tpm_struct_define
 */
typedef struct __TPM_Channel_T
{
    uint32_t state;     /**< TPM all channel state and every channel occupies two bits */
    uint32_t mode[2];   /**< TPM all channel mode and every channel occupies four bits.  */
    uint8_t  pwmn_mode; /**< TPM all channel pwmn mode and every channel occupies two bits.
                             If the instance not support PWMN, it aways is 0  */
} TPM_Channel_T;

/**
 * @brief TPM Handle Structure Definition
 * \ingroup tpm_struct_define
 */
typedef struct __TPM_Handle_T
{
    void         *instance;       /**< Register Base Address */
    TPM_Init_T    init;           /**< TPM Required Parameters */
    HAL_State_T   state;          /**< TPM State */
    uint32_t      active_channel; /**< TPM Active channel */
    TPM_Channel_T channel_info;   /**< TPM channel information */
    HAL_State_T   cap_state;      /**< TPM CAP state */
    HAL_MUTEX     mutex;          /**< TPM Process Protection */
    uint32_t      error_code;     /**< TPM Error Code */
    uint8_t       tpm_index;      /**< TPM index */
#if (configUSE_TPM_DMA == 1)
    DMA_Handle_T *hdma[7]; /**< TPM DMA Handle parameters
                                This array is accessed by a @ref tpm_dma_handle_index */
#endif                     /* configUSE_TPM_DMA */

#if (configUSE_TPM_REGISTER_CALLBACKS == 1)
    void (*MspInitCallback)(struct __TPM_Handle_T *htpm);           /**< TPM Msp Init Callback */
    void (*MspDeInitCallback)(struct __TPM_Handle_T *htpm);         /**< TPM Msp DeInit Callback */
    void (*OC_DelayElapsedCallback)(struct __TPM_Handle_T *htpm);   /**< Output Compare Delay Elapsed Callback */
    void (*PWM_PulseFinishedCallback)(struct __TPM_Handle_T *htpm); /**< PWM Pulse Finished Callback */
    void (*BreakCallback)(struct __TPM_Handle_T *htpm);             /**< Break Function Callback */
    void (*CaptureCallback)(struct __TPM_Handle_T *htpm);           /**< TPM Capture Callback */
    void (*ErrorCallback)(struct __TPM_Handle_T *htpm);             /**< TPM Error Callback */

#endif /* configUSE_TPM_REGISTER_CALLBACKS */
} TPM_Handle_T;

/**
 * @brief HAL TPM Callback pointer definition
 * \ingroup tpm_struct_define
 */
typedef void (*TPM_Callback_T)(TPM_Handle_T *htpm); /**< pointer to a TPM common callback functions */

/**
 * \brief   HAL TPM CAP Configuration Structure definition
 * \ingroup tpm_struct_define
 */
typedef struct __TPM_Capture_T
{
    uint32_t rising;  /**< Specifies capture TC on input signal rising edge.
                           This parameter can be a value of @ref HAL_DISABLE and @ref HAL_ENABLE  */
    uint32_t falling; /**< Specifies capture TC on input signal falling edge.
                           This parameter can be a value of @ref HAL_DISABLE and @ref HAL_ENABLE  */
} TPM_Capture_T;

/**
 * \brief   HAL TPM Output Compare and PWM Configuration Structure definition
 * \ingroup tpm_struct_define
 */
typedef struct __TPM_OC_T
{
    uint32_t mode;       /**< Specifies the TPM channle mode.
                              This parameter can be a value of @ref tpm_oc_mode */
    uint32_t period;     /**< Specifies the Period of current channel.
                              This parameter can be a number between Min_Data = 0x0000 and Max_Data = 0xFFFF. */
    uint32_t stop;       /**< Specifies the enable of current channel to stop counter when TC equal Match Register.
                              This parameter can be a value of @ref HAL_ENABLE of @ref HAL_DISABLE .  */
    uint32_t reset;      /**< Specifies the enable of current channel to reset counter when TC equal Match Register.
                              This parameter can be a value of @ref HAL_ENABLE of @ref HAL_DISABLE .  */
    uint32_t idle_state; /**< Specifies the TPM output compare pin state during idle state in EMC mode.
                              This parameter can be a value of @ref tpm_emc_idle_state
                              @note This parameter is valid only for EMC mode. */
} TPM_OC_T;

/**
 * \brief  HAL TPM Dead_band configuration Structure definition
 * \ingroup tpm_struct_define
 */
typedef struct __TPM_PWMN_T
{
    uint32_t mode; /**< Specifies PWMN mode.
                        This parameter can be a value of @ref tpm_pwmn_mode  */
    uint32_t db;   /**< Specifies Count of PWMN output dead-band period time.
                        This parameter can be a number between Min_Data = 0x0000 and Max_Data = 0x3FF.
                        PWMN output dead-band period time=db * CT16Bn_PCLK*(PR+1) cycle.
                        Recommend the dead-band period less than PWM high pulse width, or the PWM high pulse width disappears*/

} TPM_PWMN_T;

/**
 * \brief  HAL TPM Break function configuration Structure definition
 * \ingroup tpm_struct_define
 */
typedef struct __TPM_Break_T
{
    uint32_t break_mode; /**< Specifies the TPM trigger mode selection of PWM channels break function.
                                This parameter can be a value of @ref tpm_break_source  */
    uint32_t break_db;   /**< Specifies the Break pin (BRK) debounce time.
                              This parameter can be a value of @ref tpm_break_db  */
    uint32_t break_ie;   /**< Enable&disable generating an interrupt.
                              This parameter can be a value of @ref HAL_ENABLE of @ref HAL_DISABLE .  */
} TPM_Break_T;

/**
 * \brief  HAL TPM PWM load mode configuration Structure definition
 * \ingroup tpm_struct_define
 */
typedef struct __TPM_LoadMode_T
{
    uint8_t channel;   /**< Specifies the TPM set load mode channel.
                             This parameter can be a value of @ref tpm_channel.
                             This parameter must be a number between @ref TPM_CHANNEL_0 and @ref TPM_CHANNEL_3. */
    uint8_t load_mode; /**< Specifies the TPM load mode.
                             This parameter can be a value of @ref tpm_load_mode  */
} TPM_LoadMode_T;

/**
 * @brief HAL TPM Callback ID enumeration definition
 * \ingroup tpm_struct_define
 */
typedef enum
{
    HAL_TPM_MSPINIT_CB_ID            = 0x00U, /**< HAL TPM MspInit Callback ID                     */
    HAL_TPM_MSPDEINIT_CB_ID          = 0x01U, /**< HAL TPM MspDeInit Callback ID                   */
    HAL_TPM_OC_DELAY_ELAPSED_CB_ID   = 0x02U, /**< HAL TPM Output Compare Delay Elapsed Callback ID               */
    HAL_TPM_PWM_PULSE_FINISHED_CB_ID = 0x03U, /**< HAL TPM PWM Pulse Finished Callback ID           */
    HAL_TPM_BREAK_CB_ID              = 0x04U, /**< HAL TPM CAP0 Callback ID                              */
    HAL_TPM_CAP0_CB_ID               = 0x05U, /**< HAL TPM CAP0 Callback ID                              */
    HAL_TPM_ERROR_CB_ID              = 0x06U, /**< HAL TPM Error Callback ID                             */
    HAL_TPM_ALL_CB_ID                = 0x07U, /**< Unregister all callback      */
} HAL_TPM_CallbackID_T;

/**
 * \defgroup tpm_mode TPM Mode
 * \ingroup tpm_control
 * @{
 */
#define TPM_CTM_TIMER           0x00U /**< The mode is   Timer, TC will increment or reduce in PCLK rising edge.     */
#define TPM_CTM_COUNTER_RISING  0x01U /**< The mode is counter, TC will increment or reduce in CAP pin rising edge.  */
#define TPM_CTM_COUNTER_FALLING 0x02U /**< The mode is counter, TC will increment or reduce in CAP pin falling edge. */
#define TPM_CTM_COUNTER_BOTH    0x03U /**< The mode is counter, TC will increment or reduce in CAP pin both edge.    */
/**
 * @}
 */

/**
 * \defgroup tpm_cm_mode TPM Counting Mode
 * \ingroup tpm_control
 * @{
 */
#define TPM_COUNTING_UP          0x00U /**< Edge-aligned Up-counting mode.     */
#define TPM_COUNTING_DOWN        0x01U /**< Edge-aligned Down-counting mode.  */
#define TPM_COUNTING_CENTER_DOWN 0x02U /**< The match interrupt flag is set during the down-counting period.    */
#define TPM_COUNTING_CENTER_UP   0x04U /**< The match interrupt flag is set during the up-counting period. */
#define TPM_COUNTING_CENTER_BOTH 0x06U /**< The match interrupt flag is set during both up-counting and down-counting period.    */
/**
 * @}
 */

/**
 * \defgroup tpm_emc_idle_state TPM output compare pin state during idle state in EMC mode.
 * \ingroup tpm_control
 * @{
 */
#define TPM_IDLE_LOW  0x00U /**< Output low when TC != MR     */
#define TPM_IDLE_HIGH 0x01U /**< Output high when TC != MR     */
/**
 * @}
 */

/**
 * \defgroup tpm_channel TPM Channel
 * \ingroup tpm_control
 * @{
 */
#define TPM_CHANNEL_0       0x00U /**< Compare channel 0      */
#define TPM_CHANNEL_1       0x01U /**< Compare channel 1, SN_CT16B6&SN_CT16B7 not support the channel      */
#define TPM_CHANNEL_2       0x02U /**< Compare channel 2, SN_CT16B3&SN_CT16B4&SN_CT16B6&SN_CT16B7 not support the channel      */
#define TPM_CHANNEL_3       0x03U /**< Compare channel 3, SN_CT16B3&SN_CT16B4&SN_CT16B6&SN_CT16B7 not support the channel */
#define TPM_CHANNEL_4       0x04U /**< Compare channel 4, only SN_CT16B8 support the channel */
#define TPM_CHANNEL_5       0x05U /**< Compare channel 5, only SN_CT16B8 support the channel */
#define TPM_CHANNEL_6       0x06U /**< Compare channel 6, only SN_CT16B8 support the channel */
#define TPM_CHANNEL_7       0x07U /**< Compare channel 7, only SN_CT16B8 support the channel */
#define TPM_CHANNEL_8       0x08U /**< Compare channel 8, only  SN_CT16B8 support the channel */
#define TPM_CHANNEL_9       0x09U /**< Compare channel 9, SN_CT16B6&SN_CT16B7 not support the channel      */
#define TPM_CHANNEL_10      0x0AU /**< Compare channel 10, only SN_CT16B8 support the channel */
#define TPM_CHANNEL_11      0x0BU /**< Compare channel 11, only SN_CT16B8 support the channel */
#define TPM_CHANNEL_PERIOD  0x0CU /**< Compare channel period, only SN_CT16B8 support the channel */
#define TPM_CHANNEL_CLEARED 0x0FU /**< All active channels cleared */
/**
 * @}
 */

/**
 * \defgroup tpm_oc_mode TPM Output Compare and PWM Modes
 * \ingroup tpm_control
 * @{
 */
#define TPM_OCMODE_NONE        0x00U /**< Not config  */
#define TPM_OCMODE_TIMER       0x01U /**< Not output, only Timer     */
#define TPM_OCMODE_LOW         0x02U /**< EMC mode, output low when TC == MR   */
#define TPM_OCMODE_HIGH        0x03U /**< EMC mode, output high when TC == MR  */
#define TPM_OCMODE_TOGGLE      0x04U /**< EMC mode, toggle when TC == MR       */
#define TPM_OCMODE_PWM1        0x05U /**< PWM mode 1                 */
#define TPM_OCMODE_PWM2        0x06U /**< PWM mode 2                 */
#define TPM_OCMODE_FORCED_LOW  0x07U /**< Force ouput low level            */
#define TPM_OCMODE_FORCED_HIGH 0x08U /**< Force ouput high level           */
/**
 * @}
 */

/**
 * \defgroup tpm_pwmn_mode TPM PWMN Modes
 * \ingroup tpm_control
 * @{
 */
#define TPM_PWMN_GPIO   0x00U /**< PWNM pin is act as GPIO    */
#define TPM_PWMN_MODE_0 0x01U /**< PWMN pin outputs the inverse signal with dead-band of PWM, but same High signal during dead-band period  */
#define TPM_PWMN_MODE_1 0x02U /**< PWMN pin outputs the inverse signal with dead-band of PWM, but same Low signal during dead-band period  */
#define TPM_PWMN_MODE_2 0x03U /**< PWMN pin changes to non-inverse PWM output pin which outputs the same inverse PWM signal with dead-band period.  */
/**
 * @}
 */

/**
 * \defgroup tpm_break_source TPM trigger source selection of PWM channels break function
 * \ingroup tpm_control
 * @{
 */
#define TPM_BREAK_DISABLE  0x00U /**< Disable break */
#define TPM_BREAK_PIN_LOW  0x04U /**< TPM trigger when Break pin status is from high to low */
#define TPM_BREAK_PIN_HIGH 0x14U /**< TPM trigger when Break pin status is from low to high */
/**
 * @}
 */

/**
 * \defgroup tpm_break_db Break pin (BRK) debounce time
 * \ingroup tpm_control
 * @{
 */
#define TPM_BREAK_1_PCLK   0x00U /**< Debounce time=1*CT16B0_PCLK */
#define TPM_BREAK_2_PCLK   0x01U /**< Debounce time=2*CT16B0_PCLK */
#define TPM_BREAK_4_PCLK   0x02U /**< Debounce time=4*CT16B0_PCLK */
#define TPM_BREAK_8_PCLK   0x03U /**< Debounce time=8*CT16B0_PCLK */
#define TPM_BREAK_16_PCLK  0x04U /**< Debounce time=16*CT16B0_PCLK */
#define TPM_BREAK_32_PCLK  0x05U /**< Debounce time=32*CT16B0_PCLK */
#define TPM_BREAK_64_PCLK  0x06U /**< Debounce time=64*CT16B0_PCLK */
#define TPM_BREAK_128_PCLK 0x07U /**< Debounce time=128*CT16B0_PCLK */
/**
 * @}
 */

/** @defgroup tpm_dma_handle_index TPM DMA Handle Index
 * \ingroup tpm_control
 * @{
 */
#define TPM_DMA_ID_MR_0      0x00U /**< Index of the DMA handle used for MR1 DMA requests */
#define TPM_DMA_ID_MR_1      0x01U /**< Index of the DMA handle used for MR2 DMA requests */
#define TPM_DMA_ID_MR_2      0x02U /**< Index of the DMA handle used for MR3 DMA requests */
#define TPM_DMA_ID_MR_3      0x03U /**< Index of the DMA handle used for MR4 DMA requests */
#define TPM_DMA_ID_MR_9      0x04U /**< Index of the DMA handle used for MR9 DMA requests */
#define TPM_DMA_ID_MR_PERIOD 0x05U /**< Index of the DMA handle used for MR_PERIOD DMA requests */
#define TPM_DMA_ID_CAP       0x06U /**< Index of the DMA handle used for CAP0 DMA requests */
/**
 * @}
 */

/**
 * \defgroup tpm_load_mode TPM MRn load mode selection in center-aligned mode
 * \ingroup tpm_control
 * @{
 */
#define TPM_NO_EFFECT        0x00U /**< TPM MRn value is unchanged at any time */
#define TPM_LOAD_TC_EQ_0     0x01U /**< TPM load MRn value at TC=0 and MRn value is unchanged at TC=MR9 */
#define TPM_LOAD_TC_EQ_0_MR9 0x03U /**< TPM load MRn value at TC=0 and TC=MR9 */
/**
 * @}
 */

/* Exported functions --------------------------------------------------------*/

/* Initialization/de-initialization functions  ********************************/
HAL_Status_T HAL_TPM_Init(TPM_Handle_T *htpm);
HAL_Status_T HAL_TPM_DeInit(TPM_Handle_T *htpm);
void         HAL_TPM_MspInit(TPM_Handle_T *htpm);
void         HAL_TPM_MspDeInit(TPM_Handle_T *htpm);

/* Callbacks Register/UnRegister functions ************************************/
HAL_Status_T HAL_TPM_RegisterCallback(TPM_Handle_T *htpm, HAL_TPM_CallbackID_T callback_id, TPM_Callback_T callback);
HAL_Status_T HAL_TPM_UnRegisterCallback(TPM_Handle_T *htpm, HAL_TPM_CallbackID_T callback_id);

/* Base functions ********************************************************/
HAL_Status_T HAL_TPM_Base_Start(TPM_Handle_T *htpm);
HAL_Status_T HAL_TPM_Base_Stop(TPM_Handle_T *htpm);

HAL_Status_T HAL_TPM_Base_Start_IT(TPM_Handle_T *htpm);
HAL_Status_T HAL_TPM_Base_Stop_IT(TPM_Handle_T *htpm);

/* PWM functions ********************************************************/
HAL_Status_T HAL_TPM_PWM_ConfigChannel(TPM_Handle_T *htpm, TPM_OC_T *config, uint32_t channel);

HAL_Status_T HAL_TPM_PWM_Start(TPM_Handle_T *htpm, uint32_t channel);
HAL_Status_T HAL_TPM_PWM_Stop(TPM_Handle_T *htpm, uint32_t channel);

HAL_Status_T HAL_TPM_PWM_Start_IT(TPM_Handle_T *htpm, uint32_t channel);
HAL_Status_T HAL_TPM_PWM_Stop_IT(TPM_Handle_T *htpm, uint32_t channel);

HAL_Status_T HAL_TPM_PWM_Start_DMA(TPM_Handle_T *htpm, uint32_t channel, uint32_t *data, uint32_t length);
HAL_Status_T HAL_TPM_PWM_Stop_DMA(TPM_Handle_T *htpm, uint32_t channel);

HAL_Status_T HAL_TPM_PWM_ConfigPWMN(TPM_Handle_T *htpm, TPM_PWMN_T *config, uint32_t channel);
HAL_Status_T HAL_TPM_PWM_ConfigLoadMode(TPM_Handle_T *htpm, TPM_LoadMode_T *config);
HAL_Status_T HAL_TPM_PWM_ConfigMatchValue(TPM_Handle_T *htpm, uint32_t match_value, uint32_t channel);
/* Output Compare_ functions ********************************************************/
HAL_Status_T HAL_TPM_OC_ConfigChannel(TPM_Handle_T *htpm, TPM_OC_T *config, uint32_t channel);
HAL_Status_T HAL_TPM_OC_Start(TPM_Handle_T *htpm, uint32_t channel);
HAL_Status_T HAL_TPM_OC_Stop(TPM_Handle_T *htpm, uint32_t channel);

HAL_Status_T HAL_TPM_OC_Start_IT(TPM_Handle_T *htpm, uint32_t channel);
HAL_Status_T HAL_TPM_OC_Stop_IT(TPM_Handle_T *htpm, uint32_t channel);

HAL_Status_T HAL_TPM_OC_Start_DMA(TPM_Handle_T *htpm, uint32_t channel, uint32_t *data, uint32_t length);
HAL_Status_T HAL_TPM_OC_Stop_DMA(TPM_Handle_T *htpm, uint32_t channel);

/* Input Capture functions **********************************************/
HAL_Status_T HAL_TPM_CAP_Config(TPM_Handle_T *htpm, TPM_Capture_T *config);
HAL_Status_T HAL_TPM_CAP_Start(TPM_Handle_T *htpm);
HAL_Status_T HAL_TPM_CAP_Stop(TPM_Handle_T *htpm);

HAL_Status_T HAL_TPM_CAP_Start_IT(TPM_Handle_T *htpm);
HAL_Status_T HAL_TPM_CAP_Stop_IT(TPM_Handle_T *htpm);

HAL_Status_T HAL_TPM_CAP_Start_DMA(TPM_Handle_T *htpm, uint32_t *data, uint16_t length);
HAL_Status_T HAL_TPM_CAP_Stop_DMA(TPM_Handle_T *htpm);

uint32_t HAL_TPM_ReadCapturedValue(TPM_Handle_T *htpm);

/* Break functions  **************************************************/
HAL_Status_T HAL_TPM_ConfigBreak(TPM_Handle_T *htpm, TPM_Break_T *config);

/* Peripheral IRQ functions  **************************************************/
void HAL_TPM_IRQHandler(TPM_Handle_T *htpm);

/* Peripheral State and Errors functions **************************************/
HAL_State_T HAL_TPM_GetState(TPM_Handle_T *htpm);
HAL_State_T HAL_TPM_GetChannelState(TPM_Handle_T *htpm, uint32_t channel);
uint32_t    HAL_TPM_GetError(TPM_Handle_T *htpm);
uint32_t    HAL_TPM_GetActiveChannel(TPM_Handle_T *htpm);
/* Event Callback functions ***************************************************/
void HAL_TPM_CaptureCallback(TPM_Handle_T *htpm);
void HAL_TPM_OC_DelayElapsedCallback(TPM_Handle_T *htpm);
void HAL_TPM_PWM_PulseFinishedCallback(TPM_Handle_T *htpm);
void HAL_TPM_BreakCallback(TPM_Handle_T *htpm);
void HAL_TPM_ErrorCallback(TPM_Handle_T *htpm);

#ifdef __cplusplus
}
#endif

#endif /* _HAL_TPM_H_ */
