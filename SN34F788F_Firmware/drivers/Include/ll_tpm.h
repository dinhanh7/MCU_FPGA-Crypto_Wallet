/***********************
 * SN34F78X SDK V2.1.4 *
 ***********************/

#ifndef _LL_TPM_H_
#define _LL_TPM_H_

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "sn34f78x_ll_def.h"
#include "ll_reg_msk.h"

/* Exported types ------------------------------------------------------------*/
/**
 * @brief TPM Init Structure Definition
 * \ingroup tpm_struct_define
 */
typedef struct
{
    uint32_t pre_scaler; /**< Specifies the Pre-Scaler value used to divide the HAL TPM clock.
                              This parameter can be a number between Min_Data = 0x00 and Max_Data = 0xFF.
                              @note when \b PC(Pre-scale Counter) equal PreScaler, \b TC(TPMer Counter) will increment or reduce. */
    uint32_t ctm;        /**< Specifies the mode that decide when to increment or reduce \b TC or \b PC.
                              default is every rising PCLK edge.
                              This parameter can be a value of @ref tpm0_ctm_mode (instance = SN_CT16B0)
                                                               @ref tpm125_ctm_mode. (instance = SN_CT16B1/2/5)
                                                               @ref tpm4_ctm_mode (instance = SN_CT16B4)
                              @note in TPMer   mode, \b PC will increment or reduce at every rising PCLK edge.
                              @note in counter mode, \b PC will increment or reduce at CAP pin rising(or falling, or both) edge.
                              @note in counter mode, can not use capture function.
                              @note in counter mode, the duration of the HIGH/LOW levels on the same CAP input
                              cannot be <b> shorter than 1 / (2 * PCLK). </b> */
    uint32_t cm;         /** Specifies the counting mode .
                             Default is Edge-aligned Up-counting mode.
                             This parameter can be a value of @ref tpm0_cm_mode.  (instance = SN_CT16B0)
                                                              @ref tpm125_cm_mode. (instance = SN_CT16B1/2/5) */
} LL_TPM_Init_T;

/**
 * \brief   LL TPM Output Compare and PWM Configuration Structure definition
 * \ingroup tpm_struct_define
 */
typedef struct __LL_TPM_OC_T
{
    uint32_t mode;       /**< Specifies the TPM channel mode.
                              This parameter can be a value of @ref tpm_oc_mode */
    uint32_t period;     /**< Specifies the Period of current channel.
                              This parameter can be a number between Min_Data = 0x0000 and Max_Data = 0xFFFF. */
    uint32_t stop;       /**< Specifies the enable of current channel to stop counter when TC equal Match Register.
                              This parameter can be a value of LL_ENABLE or LL_DISABLE .  */
    uint32_t reset;      /**< Specifies the enable of current channel to reset counter when TC equal Match Register.
                              This parameter can be a value of LL_ENABLE or LL_DISABLE .  */
    uint32_t idle_state; /**< Specifies the TPM output compare pin state during idle state in EMC mode.
                              This parameter can be a value of @ref tpm_emc_idle_state
                              @note This parameter is valid only for EMC mode. */
} LL_TPM_OC_T;

/**
 * \brief   TPM CAP Configuration Structure definition
 * \ingroup tpm_struct_define
 */
typedef struct __LL_TPM_Capture_T
{
    uint32_t rising;  /**< Specifies capture TC on input signal rising edge.
                           This parameter can be a value of @ref LL_ENABLE and @ref LL_DISABLE  */
    uint32_t falling; /**< Specifies capture TC on input signal falling edge.
                           This parameter can be a value of @ref LL_ENABLE and @ref LL_DISABLE  */
} LL_TPM_Capture_T;

/**
 * \brief  Dead_band configuration Structure definition
 * \ingroup tpm_struct_define
 */
typedef struct __LL_TPM_PWMN_T
{
    uint32_t mode; /**< Specifies PWMN mode.
                        This parameter can be a value of tpmx_pwmn_gpio_selection, x = 0,125,34,67,8  */
    uint32_t db;   /**< Specifies Count of PWMN output dead-band period time.
                        This parameter can be a number between Min_Data = 0x0000 and Max_Data = 0x3FF.
                        PWMN output dead-band period time=db * CT16Bn_PCLK*(PR+1) cycle.
                        Recommend the dead-band period less than PWM high pulse width, or the PWM high pulse width disappears*/

} LL_TPM_PWMN_T;

/**
 * \brief  LL TPM Break function configuration Structure definition
 * \ingroup tpm_struct_define
 */
typedef struct __LL_TPM_Break_T
{
    uint32_t break_mode; /**< Specifies the TPM trigger mode selection of PWM channels break function.
                                This parameter can be a value of @ref tpm_break_source  */
    uint32_t break_db;   /**< Specifies the Break pin (BRK) debounce time.
                              This parameter can be a value of @ref tpm_break_db  */
    uint32_t break_ie;   /**< Enable&disable generating an interrupt.
                              This parameter can be a value of @ref LL_ENABLE of @ref LL_DISABLE .  */
} LL_TPM_Break_T;

/**
 * \brief  LL TPM PWM load mode configuration Structure definition
 * \ingroup tpm_struct_define
 */
typedef struct __LL_TPM_LoadMode_T
{
    uint8_t channel;   /**< Specifies the TPM set load mode channel.
                             This parameter can be a value of @ref tpm_channel.
                             This parameter must be a number between @ref LL_TPM_CHANNEL_0 and @ref LL_TPM_CHANNEL_3. */
    uint8_t load_mode; /**< Specifies the TPM load mode.
                             This parameter can be a value of @ref tpm0_mr_loadmode or @ref tpm125_mr_loadmode  */
} LL_TPM_LoadMode_T;

/* Exported macro --------------------------------------------------------*/
/**
 * \defgroup tpm_emc_idle_state CT16Bn output compare pin state during idle state in EMC mode.
 * \ingroup tpm_control
 * @{
 */
#define LL_TPM_IDLE_LOW  0x00U /**< Output low when TC != MR     */
#define LL_TPM_IDLE_HIGH 0x01U /**< Output high when TC != MR     */
/**
 * @}
 */

/**
 * \defgroup tpm_oc_mode CT16Bn Output Compare and PWM Modes
 * \ingroup tpm_control
 * @{
 */
#define LL_TPM_OCMODE_NONE        0x00U /**< Not config  */
#define LL_TPM_OCMODE_TIMER       0x01U /**< Not output, only Timer     */
#define LL_TPM_OCMODE_LOW         0x02U /**< EMC mode, output low when TC == MR   */
#define LL_TPM_OCMODE_HIGH        0x03U /**< EMC mode, output high when TC == MR  */
#define LL_TPM_OCMODE_TOGGLE      0x04U /**< EMC mode, toggle when TC == MR       */
#define LL_TPM_OCMODE_PWM1        0x05U /**< PWM mode 1                 */
#define LL_TPM_OCMODE_PWM2        0x06U /**< PWM mode 2                 */
#define LL_TPM_OCMODE_FORCED_LOW  0x07U /**< Force output low level            */
#define LL_TPM_OCMODE_FORCED_HIGH 0x08U /**< Force output high level           */
/**
 * @}
 */

/**
 * \defgroup tpm_break_source CT16Bn trigger source selection of PWM channels break function
 * \ingroup tpm_control
 * @{
 */
#define LL_TPM_BREAK_DISABLE  0x00U /**< Disable break */
#define LL_TPM_BREAK_PIN_LOW  0x04U /**< TPM trigger when Break pin status is from high to low */
#define LL_TPM_BREAK_PIN_HIGH 0x14U /**< TPM trigger when Break pin status is from low to high */
/**
 * @}
 */
/**
 * \defgroup tpm_break_db CT16Bn Break pin (BRK) debounce time
 * \ingroup tpm_control
 * @{
 */
#define LL_TPM_BREAK_1_PCLK   0x00U /**< Debounce time=1*CT16B0_PCLK */
#define LL_TPM_BREAK_2_PCLK   0x01U /**< Debounce time=2*CT16B0_PCLK */
#define LL_TPM_BREAK_4_PCLK   0x02U /**< Debounce time=4*CT16B0_PCLK */
#define LL_TPM_BREAK_8_PCLK   0x03U /**< Debounce time=8*CT16B0_PCLK */
#define LL_TPM_BREAK_16_PCLK  0x04U /**< Debounce time=16*CT16B0_PCLK */
#define LL_TPM_BREAK_32_PCLK  0x05U /**< Debounce time=32*CT16B0_PCLK */
#define LL_TPM_BREAK_64_PCLK  0x06U /**< Debounce time=64*CT16B0_PCLK */
#define LL_TPM_BREAK_128_PCLK 0x07U /**< Debounce time=128*CT16B0_PCLK */
/**
 * @}
 */

/**
 * \defgroup tpm_channel CT16Bn Channel
 * \ingroup tpm_control
 * @{
 */
#define LL_TPM_CHANNEL_0       0x00U /**< Compare channel 0      */
#define LL_TPM_CHANNEL_1       0x01U /**< Compare channel 1, SN_CT16B6&SN_CT16B7 not support the channel      */
#define LL_TPM_CHANNEL_2       0x02U /**< Compare channel 2, SN_CT16B3&SN_CT16B4&SN_CT16B6&SN_CT16B7 not support the channel      */
#define LL_TPM_CHANNEL_3       0x03U /**< Compare channel 3, SN_CT16B3&SN_CT16B4&SN_CT16B6&SN_CT16B7 not support the channel */
#define LL_TPM_CHANNEL_4       0x04U /**< Compare channel 4, only SN_CT16B8 support the channel */
#define LL_TPM_CHANNEL_5       0x05U /**< Compare channel 5, only SN_CT16B8 support the channel */
#define LL_TPM_CHANNEL_6       0x06U /**< Compare channel 6, only SN_CT16B8 support the channel */
#define LL_TPM_CHANNEL_7       0x07U /**< Compare channel 7, only SN_CT16B8 support the channel */
#define LL_TPM_CHANNEL_8       0x08U /**< Compare channel 8, only  SN_CT16B8 support the channel */
#define LL_TPM_CHANNEL_9       0x09U /**< Compare channel 9, SN_CT16B6&SN_CT16B7 not support the channel      */
#define LL_TPM_CHANNEL_10      0x0AU /**< Compare channel 10, only SN_CT16B8 support the channel */
#define LL_TPM_CHANNEL_11      0x0BU /**< Compare channel 11, only SN_CT16B8 support the channel */
#define LL_TPM_CHANNEL_PERIOD  0x0CU /**< Compare channel period, only SN_CT16B8 support the channel */
#define LL_TPM_CHANNEL_CLEARED 0x0FU /**< All active channels cleared */
/**
 * @}
 */

#if defined(USE_FULL_LL_DRIVER)
LL_Status_T LL_TPM_Init(void *tpm, LL_TPM_Init_T *tpm_init);
LL_Status_T LL_TPM_DeInit(void *tpm);
void        LL_TPM_StructInit(void *tpm, LL_TPM_Init_T *tpm_init);

// Configuration API
LL_Status_T LL_TPM_OC_ConfigChannel(void *tpm, LL_TPM_Init_T *tpm_init, LL_TPM_OC_T *config, uint8_t channel);
LL_Status_T LL_TPM_PWM_ConfigChannel(void *tpm, LL_TPM_Init_T *tpm_init, LL_TPM_OC_T *config, uint32_t channel);
LL_Status_T LL_TPM_PWM_ConfigPWMN(void *htpm, LL_TPM_PWMN_T *config, uint32_t channel);
LL_Status_T LL_TPM_CAP_Config(void *tpm, LL_TPM_Init_T *tpm_init, LL_TPM_Capture_T *config);
LL_Status_T LL_TPM_ConfigBreak(void *tpm, LL_TPM_Break_T *config);
LL_Status_T LL_TPM_ConfigLoadMode(void *tpm, LL_TPM_LoadMode_T *config);
#endif /* USE_FULL_LL_DRIVER */

#ifdef __cplusplus
}
#endif

#endif /* _LL_tpm_H_ */
