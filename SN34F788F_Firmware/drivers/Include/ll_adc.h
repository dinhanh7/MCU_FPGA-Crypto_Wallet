/***********************
 * SN34F78X SDK V2.1.4 *
 ***********************/

#ifndef _LL_ADC_H_
#define _LL_ADC_H_

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "sn34f78x_ll_def.h"
#include "ll_reg_msk.h"

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
    uint32_t autopwdn;    /**< Specifies ADC Auto Power down Mode. default valve is LL_DISABLE.
                               This parameter only invalid when \b conv_mode is @ref LL_ADC_CONVMODE_SINGLE_STEP or @ref LL_ADC_CONVMODE_SINGLE_SCAN.
                               If it is @ref LL_ENABLE, ADC automatically enters the power-down mode when ADC scan conversion finishes
                               This parameter can be a value of @ref LL_DISABLE and @ref LL_ENABLE. */
    uint32_t pre_clock;   /**< Specifies the ADC prescaler number.
                               This parameter must be a number between Min_Data = 0x01 and Max_Data = 0xFF.*/
} LL_ADC_Init_T;

/* Exported constants --------------------------------------------------------*/

/**
 * \defgroup adc_channel ADC conversion channel
 * \ingroup adc_control
 * @{
 */
#define LL_ADC_CHANNEL_0   0x00U /**<  ADC channel 0 */
#define LL_ADC_CHANNEL_1   0x01U /**<  ADC channel 1 */
#define LL_ADC_CHANNEL_2   0x02U /**<  ADC channel 2 */
#define LL_ADC_CHANNEL_3   0x03U /**<  ADC channel 3 */
#define LL_ADC_CHANNEL_4   0x04U /**<  ADC channel 4 */
#define LL_ADC_CHANNEL_5   0x05U /**<  ADC channel 5 */
#define LL_ADC_CHANNEL_6   0x06U /**<  ADC channel 6 */
#define LL_ADC_CHANNEL_7   0x07U /**<  ADC channel 7 */
#define LL_ADC_CHANNEL_8   0x08U /**<  ADC channel 8 */
#define LL_ADC_CHANNEL_9   0x09U /**<  ADC channel 9 */
#define LL_ADC_CHANNEL_10  0x0AU /**<  ADC channel 10 */
#define LL_ADC_CHANNEL_11  0x0BU /**<  ADC channel 11 */
#define LL_ADC_CHANNEL_12  0x0CU /**<  ADC channel 12 */
#define LL_ADC_CHANNEL_13  0x0DU /**<  ADC channel 13 */
#define LL_ADC_CHANNEL_14  0x0EU /**<  ADC channel 14 */
#define LL_ADC_CHANNEL_15  0x0FU /**<  ADC channel 15 */
#define LL_ADC_CHANNEL_VDD 0x10U /**<  ADC channel VDD */
#define LL_ADC_CHANNEL_VSS 0x11U /**<  ADC channel VSS */
/**
 * @}
 */

/**
 * \defgroup adc_dma_channel ADC DMA channel
 * \ingroup adc_control
 * @{
 */
#define LL_ADC_DMA_CHANNEL_0 0x00U /**<  ADC dma channel 0 */
#define LL_ADC_DMA_CHANNEL_1 0x01U /**<  ADC dma channel 1 */
#define LL_ADC_DMA_CHANNEL_2 0x02U /**<  ADC dma channel 2 */
#define LL_ADC_DMA_CHANNEL_3 0x03U /**<  ADC dma channel 3 */
/**
 * @}
 */

/** @defgroup adc_sequence_rank  ADC group regular - Sequencer ranks
 * \ingroup adc_control
 * @{
 */
#define LL_ADC_REG_RANK_1  0x00U /*!< ADC group regular sequencer rank 1 */
#define LL_ADC_REG_RANK_2  0x01U /*!< ADC group regular sequencer rank 2 */
#define LL_ADC_REG_RANK_3  0x02U /*!< ADC group regular sequencer rank 3 */
#define LL_ADC_REG_RANK_4  0x03U /*!< ADC group regular sequencer rank 4 */
#define LL_ADC_REG_RANK_5  0x04U /*!< ADC group regular sequencer rank 5 */
#define LL_ADC_REG_RANK_6  0x05U /*!< ADC group regular sequencer rank 6 */
#define LL_ADC_REG_RANK_7  0x06U /*!< ADC group regular sequencer rank 7 */
#define LL_ADC_REG_RANK_8  0x07U /*!< ADC group regular sequencer rank 8 */
#define LL_ADC_REG_RANK_9  0x08U /*!< ADC group regular sequencer rank 9 */
#define LL_ADC_REG_RANK_10 0x09U /*!< ADC group regular sequencer rank 10 */
#define LL_ADC_REG_RANK_11 0x0AU /*!< ADC group regular sequencer rank 11 */
#define LL_ADC_REG_RANK_12 0x0BU /*!< ADC group regular sequencer rank 12 */
#define LL_ADC_REG_RANK_13 0x0CU /*!< ADC group regular sequencer rank 13 */
#define LL_ADC_REG_RANK_14 0x0DU /*!< ADC group regular sequencer rank 14 */
#define LL_ADC_REG_RANK_15 0x0EU /*!< ADC group regular sequencer rank 15 */
#define LL_ADC_REG_RANK_16 0x0FU /*!< ADC group regular sequencer rank 16 */
#define LL_ADC_REG_RANK_17 0x10U /*!< ADC group regular sequencer rank 17 */
#define LL_ADC_REG_RANK_18 0x11U /*!< ADC group regular sequencer rank 18 */
/**
 * @}
 */

/**
 * \defgroup adc_conv_mode ADC conversion Mode
 * \ingroup adc_control
 * @{
 */
#define LL_ADC_CONVMODE_SINGLE_STEP 0x00000000U                   /*!< Single step conversion mode */
#define LL_ADC_CONVMODE_SINGLE_SCAN ADC_CTRL_CONVMODE_SINGLE_SCAN /*!< Single scan conversion mode */
#define LL_ADC_CONVMODE_LOOP_SCAN   ADC_CTRL_CONVMODE_LOOP_SCAN   /*!< Continuous scan conversion mode */
/**
 * @}
 */

/**
 * \defgroup adc_ldo_pd_ctrl_mode Power-down control signal mode for  internal reference LDO
 * \ingroup adc_control
 * @{
 */
#define LL_ADC_LDO_NORMAL_OP_MODE 0x00000000U      /*!< Normal operating mode */
#define LL_ADC_LDO_PD_MODE        ADC_MISC_XPD_LDO /*!< Power-down mode */
/**
 * @}
 */

/**
 * \defgroup adc_core_pd_ctrl_mode Power-down control signal mode for ADC core
 * \ingroup adc_control
 * @{
 */
#define LL_ADC_CORE_NORMAL_OP_MODE 0x00000000U      /*!< Normal operating mode */
#define LL_ADC_CORE_PD_MODE        ADC_MISC_XPD_ADC /*!< Power-down mode */
/**
 * @}
 */

/**
 * \defgroup adc_ldo_pd_signal_source Internal Reference LDO power-down control signal select
 * \ingroup adc_control
 * @{
 */
#define LL_ADC_LDO_PD_SIGNAL_FROM_ADCEN   0x00000000U         /*!< From ADCEN and AUTOPWDN bits */
#define LL_ADC_LDO_PD_SIGNAL_FROM_XPD_LDO ADC_MISC_XPD_LDOSEL /*!< From XPD_LDO bit */
/**
 * @}
 */

/**
 * \defgroup adc_core_pd_signal_source Whole ADC power-down control signal select
 * \ingroup adc_control
 * @{
 */
#define LL_ADC_CORE_PD_SIGNAL_FROM_ADCEN   0x00000000U         /*!< From ADCEN and AUTOPWDN bits */
#define LL_ADC_CORE_PD_SIGNAL_FROM_XPD_ADC ADC_MISC_XPD_ADCSEL /*!< From XPD_ADC bit */
/**
 * @}
 */

/**
 * \defgroup adc_ldo_ref_voltage LDO Internal reference voltage
 * \ingroup adc_control
 * @{
 */
#define LL_ADC_LDO_REFVOL_2_5V 0x00000000U                  /*!< Internal reference voltage= 2.5V */
#define LL_ADC_LDO_REFVOL_2_0V ADC_MISC_XSELLDO_REFVOL_2_0V /*!< Internal reference voltage= 2.0V */
#define LL_ADC_LDO_REFVOL_1_5V ADC_MISC_XSELLDO_REFVOL_1_5V /*!< Internal reference voltage= 1.5V */
/**
 * @}
 */

/**
 * \defgroup adc_adcref_voltage ADC Internal reference voltage
 * \ingroup adc_control
 * @{
 */
#define LL_ADC_ADCREF_EXT   0x00000000U            /*!< ADC in external reference mode (VSS~AVREFH) */
#define LL_ADC_ADCREF_VDD   ADC_MISC_XREFSEL_VDD   /*!< ADC in VDD/GND reference mode (VSS~VDD) */
#define LL_ADC_ADCREF_INTER ADC_MISC_XREFSEL_INTER /*!< ADC in internal reference mode (VSS~IREF) */
/**
 * @}
 */

/**
 * \defgroup adc_dma_threshold ADC DMA Threshold
 * \ingroup adc_control
 * @{
 */
#define LL_ADC_DMA_THD_4    0x00000000U               /*!< Issue DMA request after 4 data in FIFO */
#define LL_ADC_DMA_THD_8    ADC_DMAxCTRL_DMA_THD_8    /*!< Issue DMA request after 8 data in FIFO */
#define LL_ADC_DMA_THD_16   ADC_DMAxCTRL_DMA_THD_16   /*!< Issue DMA request after 16 data in FIFO */
#define LL_ADC_DMA_THD_FULL ADC_DMAxCTRL_DMA_THD_FULL /*!< Issue DMA request after Full in FIFO */
/**
 * @}
 */

/**
 * \defgroup adc_wakeup_time ADC Wakeup Time
 * \ingroup adc_control
 * @{
 */
#define LL_ADC_WKUPTIME_DISABLE 0x00000000U          /*!< Disable */
#define LL_ADC_WKUPTIME_3       ADC_TPARM_WKUPTIME_3 /*!< Waiting 3 ADC conversion cycle */
#define LL_ADC_WKUPTIME_4       ADC_TPARM_WKUPTIME_4 /*!< Waiting 4 ADC conversion cycle */
#define LL_ADC_WKUPTIME_5       ADC_TPARM_WKUPTIME_5 /*!< Waiting 5 ADC conversion cycle */
#define LL_ADC_WKUPTIME_6       ADC_TPARM_WKUPTIME_6 /*!< Waiting 6 ADC conversion cycle */
#define LL_ADC_WKUPTIME_7       ADC_TPARM_WKUPTIME_7 /*!< Waiting 7 ADC conversion cycle */
/**
 * @}
 */

/* Exported macros --------------------------------------------------------*/
/**
 * \defgroup adc_other_time_define ADC other time definition
 * \ingroup adc_control
 * @{
 */
#define ADC_WARM_UP_DELAY     (3 * 16U) /*!< ADC warm-up time:  3 * ADC conversion cycle */
#define ADC_CALIBRATION_TIMES 0x0AU     /*!< ADC calibration times */
/**
 * @}
 */
/* Exported functions --------------------------------------------------------*/
/**
 * @brief  Read ADC channel Data reg
 * @param  adc ADC Base Address
 * @param  channel which channel data reg to be read
 * @retval Value is 0(not active) or 1 (active)
 */
__STATIC_INLINE uint32_t LL_ADC_ReadChannelData(SN_ADC0_Type *adc, uint32_t channel)
{
    return (LL_REG_READ(*(&adc->DATA0 + channel)) & ADC_DATA_DATA);
}

/**
 * @brief  Enable ADC
 * @param  adc ADC Base Address
 * @retval None
 */
__STATIC_INLINE void LL_ADC_Enable(SN_ADC0_Type *adc)
{
    LL_REG_SBIT(adc->CTRL, ADC_CTRL_ADCEN);
}

/**
 * @brief  Disable ADC
 * @param  adc ADC Base Address
 * @retval None
 */
__STATIC_INLINE void LL_ADC_Disable(SN_ADC0_Type *adc)
{
    LL_REG_CBIT(adc->CTRL, ADC_CTRL_ADCEN);
}

/**
 * @brief  Get the selected ADC instance enable state.
 * @param  adc ADC instance
 * @retval 0: ADC is disabled, 1: ADC is enabled.
 */
__STATIC_INLINE uint32_t LL_ADC_IsEnabled(SN_ADC0_Type *adc)
{
    return ((LL_REG_READ(adc->CTRL) & ADC_CTRL_ADCEN) == ADC_CTRL_ADCEN);
}

/**
 * @brief  Start ADC group regular conversion.
 * @param  adc ADC Base Address
 * @retval None
 */
__STATIC_INLINE void LL_ADC_StartConversionSW(SN_ADC0_Type *adc)
{
    LL_REG_SBIT(adc->CTRL, ADC_CTRL_SWSTART);
}

/**
 * @brief  Stop ADC group regular conversion.
 * @param  adc ADC Base Address
 * @retval None
 */
__STATIC_INLINE void LL_ADC_StopConversionSW(SN_ADC0_Type *adc)
{
    LL_REG_CBIT(adc->CTRL, ADC_CTRL_SWSTART);
}

/**
 * @brief  If ADC Start conversion Enabled
 * @param  adc ADC Base Address
 * @retval Value is 0(not active) or 1 (active)
 */
__STATIC_INLINE uint32_t LL_ADC_IsEnabled_SWStart(SN_ADC0_Type *adc)
{
    return (LL_REG_RBIT(adc->CTRL, ADC_CTRL_SWSTART) == ADC_CTRL_SWSTART);
}

/**
 * @brief  Set converter mode
 * @param  adc ADC Base Address
 * @param  conv_mode This parameter can refer \ref adc_conv_mode
 * @retval None
 */
__STATIC_INLINE void LL_ADC_SetConverterMode(SN_ADC0_Type *adc, uint32_t conv_mode)
{
    LL_REG_MBIT(adc->CTRL, ADC_CTRL_CONVMODE, conv_mode);
}

/**
 * @brief  Get converter mode
 * @param  adc ADC Base Address
 * @retval convert mode, \ref adc_conv_mode
 */
__STATIC_INLINE uint32_t LL_ADC_GetConverterMode(SN_ADC0_Type *adc)
{
    return (LL_REG_READ(adc->CTRL) & ADC_CTRL_CONVMODE);
}

/**
 * @brief  ADC automatically enters the power-down mode when scan conversion finished
 * @param  adc ADC Base Address
 * @retval None
 */
__STATIC_INLINE void LL_ADC_Enable_AutoPowerDown(SN_ADC0_Type *adc)
{
    LL_REG_SBIT(adc->CTRL, ADC_CTRL_AUTOPWDN);
}

/**
 * @brief  ADC still keeps powering on after finishing ADC conversion
 * @param  adc ADC Base Address
 * @retval None
 */
__STATIC_INLINE void LL_ADC_Disable_AutoPowerDown(SN_ADC0_Type *adc)
{
    LL_REG_CBIT(adc->CTRL, ADC_CTRL_AUTOPWDN);
}

/**
 * @brief  Judge if open Auto Power Down mode
 * @param  adc ADC Base Address
 * @retval 1 : enabled;   0 : disable
 */
__STATIC_INLINE uint32_t LL_ADC_IsEnabled_AutoPowerDown(SN_ADC0_Type *adc)
{
    return ((LL_REG_READ(adc->CTRL) & ADC_CTRL_AUTOPWDN) == ADC_CTRL_AUTOPWDN);
}

/**
 * @brief  Set ADC scan conversion channel numbers
 * @param  adc ADC Base Address
 * @param  scan_ch_num The number of the scan channels ranges from 1 to 18
 * @retval None
 */
__STATIC_INLINE void LL_ADC_SetScanChannelNumber(SN_ADC0_Type *adc, uint32_t scan_ch_num)
{
    LL_REG_MBIT(adc->CTRL, ADC_CTRL_SCANNUM, (scan_ch_num - 1) << ADC_CTRL_SCANNUM_Pos);
}

/**
 * @brief  Get ADC scan conversion channel numbers
 * @param  adc ADC Base Address
 * @retval channel numbers
 */
__STATIC_INLINE uint32_t LL_ADC_GetScanChannelNumber(SN_ADC0_Type *adc)
{
    return (((LL_REG_READ(adc->CTRL) & ADC_CTRL_SCANNUM) >> ADC_CTRL_SCANNUM_Pos) + 1);
}

/**
 * @brief  Set Power-down control signal for internal reference LDO
 * @param  adc ADC Base Address
 * @param  mode ADC mode , \ref adc_ldo_pd_ctrl_mode
 * @retval None
 */
__STATIC_INLINE void LL_ADC_SetLDOPDControlMode(SN_ADC0_Type *adc, uint32_t mode)
{
    LL_REG_MBIT(adc->MISC, ADC_MISC_XPD_LDO, mode);
}

/**
 * @brief  Read operating mode for internal reference LDO
 * @param  adc ADC Base Address
 * @retval PD operating mode, \ref adc_ldo_pd_ctrl_mode
 */
__STATIC_INLINE uint32_t LL_ADC_GetLDOPDControlMode(SN_ADC0_Type *adc)
{
    return (LL_REG_READ(adc->MISC) & ADC_MISC_XPD_LDO);
}

/**
 * @brief  Set Power-down control signal mode for ADC core
 * @param  adc ADC Base Address
 * @param  mode ADC mode , \ref adc_core_pd_ctrl_mode
 * @retval None
 */
__STATIC_INLINE void LL_ADC_SetADCCorePDControlMode(SN_ADC0_Type *adc, uint32_t mode)
{
    LL_REG_MBIT(adc->MISC, ADC_MISC_XPD_ADC, mode);
}

/**
 * @brief  Read operating mode for internal ADC core
 * @param  adc ADC Base Address
 * @retval PD operating mode \ref adc_core_pd_ctrl_mode
 */
__STATIC_INLINE uint32_t LL_ADC_GetADCCorePDControlMode(SN_ADC0_Type *adc)
{
    return (LL_REG_READ(adc->MISC) & ADC_MISC_XPD_ADC);
}

/**
 * @brief  Set Internal Reference LDO power-down control signal source
 * @param  adc ADC Base Address
 * @param  source Control source , \ref adc_ldo_pd_signal_source
 * @retval None
 */
__STATIC_INLINE void LL_ADC_SetLDOPDSignalSource(SN_ADC0_Type *adc, uint32_t source)
{
    LL_REG_MBIT(adc->MISC, ADC_MISC_XPD_LDOSEL, source);
}

/**
 * @brief  Read LDO Power down signal source
 * @param  adc ADC Base Address
 * @retval Signal source, \ref adc_ldo_pd_signal_source
 */
__STATIC_INLINE uint32_t LL_ADC_GetLDOPDSignalSource(SN_ADC0_Type *adc)
{
    return (LL_REG_READ(adc->MISC) & ADC_MISC_XPD_LDOSEL);
}

/**
 * @brief  Set whole ADC power-down control signal select source
 * @param  adc ADC Base Address
 * @param  source Control source , \ref adc_core_pd_signal_source
 * @retval None
 */
__STATIC_INLINE void LL_ADC_SetADCCorePDSignalSource(SN_ADC0_Type *adc, uint32_t source)
{
    LL_REG_MBIT(adc->MISC, ADC_MISC_XPD_ADCSEL, source);
}

/**
 * @brief  Read ADC core Power down signal source
 * @param  adc ADC Base Address
 * @retval Signal source, \ref adc_core_pd_signal_source
 */
__STATIC_INLINE uint32_t LL_ADC_GetADCCorePDSignalSource(SN_ADC0_Type *adc)
{
    return (LL_REG_READ(adc->MISC) & ADC_MISC_XPD_ADCSEL);
}

/**
 * @brief  Set LDO Internal reference voltage
 * @param  adc ADC Base Address
 * @param  option This param refers to \ref adc_ldo_ref_voltage
 * @retval None
 */
__STATIC_INLINE void LL_ADC_SetLDOReferVoltage(SN_ADC0_Type *adc, uint32_t option)
{
    LL_REG_MBIT(adc->MISC, ADC_MISC_XSEL_LDO, option);
}

/**
 * @brief  Read LDO Internal reference voltage
 * @param  adc ADC Base Address
 * @retval 0: 2.5V;  1: 2.0V;  2: 1.5V
 */
__STATIC_INLINE uint32_t LL_ADC_GetLDOReferVoltage(SN_ADC0_Type *adc)
{
    return ((LL_REG_READ(adc->MISC) & ADC_MISC_XSEL_LDO) >> ADC_MISC_XSEL_LDO_Pos);
}

/**
 * @brief  Set ADC Internal reference voltage
 * @param  adc ADC Base Address
 * @param  option This param refers to \ref adc_adcref_voltage
 * @retval None
 */
__STATIC_INLINE void LL_ADC_SetADCReferVoltage(SN_ADC0_Type *adc, uint32_t option)
{
    LL_REG_MBIT(adc->MISC, ADC_MISC_XREFSEL, option);
}

/**
 * @brief  Read ADC Internal reference voltage
 * @param  adc ADC Base Address
 * @retval 0: in external reference mode;  1:  in VDD/GND reference mode;  2: in internal reference mode
 */
__STATIC_INLINE uint32_t LL_ADC_GetADCReferVoltage(SN_ADC0_Type *adc)
{
    return ((LL_REG_READ(adc->MISC) & ADC_MISC_XREFSEL) >> ADC_MISC_XREFSEL_Pos);
}

/**
 * @brief  Set Test Pin
 * @param  adc ADC Base Address
 * @param  test_pin test pin number
 * @retval None
 */
__STATIC_INLINE void LL_ADC_SetTestPin(SN_ADC0_Type *adc, uint32_t test_pin)
{
    LL_REG_MBIT(adc->MISC, ADC_MISC_XTEST, test_pin << ADC_MISC_XTEST_Pos);
}

/**
 * @brief  Get Test Pin number
 * @param  adc ADC Base Address
 * @retval test pin number
 */
__STATIC_INLINE uint32_t LL_ADC_GetTestPin(SN_ADC0_Type *adc)
{
    return ((LL_REG_READ(adc->MISC) & ADC_MISC_XTEST) >> ADC_MISC_XTEST_Pos);
}

/**
 * @brief  Set Offset calibration code
 * @param  adc ADC Base Address
 * @param  code_option Calibration code option
 * @retval None
 */
__STATIC_INLINE void LL_ADC_SetCalibrationCode(SN_ADC0_Type *adc, uint32_t code_option)
{
    LL_REG_MBIT(adc->MISC, ADC_MISC_XBVOS, code_option << ADC_MISC_XBVOS_Pos);
}

/**
 * @brief  Get Offset calibration code
 * @param  adc ADC Base Address
 * @retval Offset calibration code
 */
__STATIC_INLINE uint32_t LL_ADC_GetCalibrationCode(SN_ADC0_Type *adc)
{
    return ((LL_REG_READ(adc->MISC) & ADC_MISC_XBVOS) >> ADC_MISC_XBVOS_Pos);
}

/**
 * @brief  Enable ADC calibration mode
 * @param  adc ADC Base Address
 * @retval None
 */
__STATIC_INLINE void LL_ADC_Enable_CalibrationMode(SN_ADC0_Type *adc)
{
    LL_REG_SBIT(adc->MISC, ADC_MISC_XCAL);
}

/**
 * @brief  Disable ADC calibration mode
 * @param  adc ADC Base Address
 * @retval None
 */
__STATIC_INLINE void LL_ADC_Disable_CalibrationMode(SN_ADC0_Type *adc)
{
    LL_REG_CBIT(adc->MISC, ADC_MISC_XCAL);
}

/**
 * @brief  Judge If in calibration mode
 * @param  adc ADC Base Address
 * @retval 0 : normal conversion mode;  1 : calibration mode
 */
__STATIC_INLINE uint32_t LL_ADC_IsActiveFlag_Calibration(SN_ADC0_Type *adc)
{
    return ((LL_REG_READ(adc->MISC) & ADC_MISC_XCAL) == ADC_MISC_XCAL);
}

/**
 * @brief  Enable ADC conversion stop interrupt
 * @param  adc ADC Base Address
 * @retval None
 */
__STATIC_INLINE void LL_ADC_EnableIT_Stop(SN_ADC0_Type *adc)
{
    LL_REG_SBIT(adc->IE, ADC_IE_ADC_STOPIE);
}

/**
 * @brief  Disable ADC conversion stop interrupt
 * @param  adc ADC Base Address
 * @retval None
 */
__STATIC_INLINE void LL_ADC_DisableIT_Stop(SN_ADC0_Type *adc)
{
    LL_REG_CBIT(adc->IE, ADC_IE_ADC_STOPIE);
}

/**
 * @brief  Judge ADC conversion stop interrupt
 * @param  adc ADC Base Address
 * @retval 0 : Disable;  1 : Enable
 */
__STATIC_INLINE uint32_t LL_ADC_IsEnabled_StopIT(SN_ADC0_Type *adc)
{
    return ((LL_REG_READ(adc->IE) & ADC_IE_ADC_STOPIE) == ADC_IE_ADC_STOPIE);
}

/**
 * @brief  Enable ADC conversion done interrupt
 * @param  adc ADC Base Address
 * @retval None
 */
__STATIC_INLINE void LL_ADC_EnableIT_Done(SN_ADC0_Type *adc)
{
    LL_REG_SBIT(adc->IE, ADC_IE_ADC_DONEIE);
}

/**
 * @brief  Disable ADC conversion done interrupt
 * @param  adc ADC Base Address
 * @retval None
 */
__STATIC_INLINE void LL_ADC_DisableIT_Done(SN_ADC0_Type *adc)
{
    LL_REG_CBIT(adc->IE, ADC_IE_ADC_DONEIE);
}

/**
 * @brief  Judge ADC conversion done interrupt
 * @param  adc ADC Base Address
 * @retval 0 : Disable;  1 : Enable
 */
__STATIC_INLINE uint32_t LL_ADC_IsEnabled_DoneIT(SN_ADC0_Type *adc)
{
    return ((LL_REG_READ(adc->IE) & ADC_IE_ADC_DONEIE) == ADC_IE_ADC_DONEIE);
}

/**
 * @brief  Read RIS register
 * @param  adc ADC Base Address
 * @retval RIS register value
 */
__STATIC_INLINE uint32_t LL_ADC_GetRawInterruptStatus(SN_ADC0_Type *adc)
{
    return (LL_REG_READ(adc->RIS));
}

/**
 * @brief  Clear ADC RIS register
 * @param  adc ADC Base Address
 * @retval None
 */
__STATIC_INLINE void LL_ADC_ClearRawInterruptStatus(SN_ADC0_Type *adc, uint32_t flags)
{
    LL_REG_WRITE(adc->RIS, flags);
}

/**
 * @brief  Clear ADC converter done interrupt flag
 * @param  adc ADC Base Address
 * @retval None
 */
__STATIC_INLINE void LL_ADC_ClearFlag_DONE(SN_ADC0_Type *adc)
{
    LL_REG_WRITE(adc->RIS, ADC_RIS_ADC_DONEIF);
}

/**
 * @brief  Judge ADC converter done interrupt flag
 * @param  adc ADC Base Address
 * @retval 0 : not active;  1 : active
 */
__STATIC_INLINE uint32_t LL_ADC_IsActiveFlag_DONE(SN_ADC0_Type *adc)
{
    return ((LL_REG_READ(adc->RIS) & ADC_RIS_ADC_DONEIF) >> ADC_RIS_ADC_DONEIF_Pos);
}

/**
 * @brief  Clear ADC converter stop interrupt flag
 * @param  adc ADC Base Address
 * @retval None
 */
__STATIC_INLINE void LL_ADC_ClearFlag_STOP(SN_ADC0_Type *adc)
{
    LL_REG_WRITE(adc->RIS, ADC_RIS_ADC_STOPIF);
}

/**
 * @brief  Judge ADC converter stop interrupt flag
 * @param  adc ADC Base Address
 * @retval 0 : not active;  1 : active
 */
__STATIC_INLINE uint32_t LL_ADC_IsActiveFlag_STOP(SN_ADC0_Type *adc)
{
    return ((LL_REG_READ(adc->RIS) & ADC_RIS_ADC_STOPIF) >> ADC_RIS_ADC_STOPIF_Pos);
}

/**
 * @brief  Clear ADC DMA Channel 0 interrupt flag
 * @param  adc ADC Base Address
 * @retval None
 */
__STATIC_INLINE void LL_ADC_ClearFlag_DMACH0(SN_ADC0_Type *adc)
{
    LL_REG_WRITE(adc->RIS, ADC_RIS_DMACH0IF);
}

/**
 * @brief  Judge ADC DMA Channel 0 interrupt flag
 * @param  adc ADC Base Address
 * @retval 0 : not active;  1 : active
 */
__STATIC_INLINE uint32_t LL_ADC_IsActiveFlag_DMACH0(SN_ADC0_Type *adc)
{
    return ((LL_REG_READ(adc->RIS) & ADC_RIS_DMACH0IF) >> ADC_RIS_DMACH0IF_Pos);
}

/**
 * @brief  Clear ADC DMA Channel 1 interrupt flag
 * @param  adc ADC Base Address
 * @retval None
 */
__STATIC_INLINE void LL_ADC_ClearFlag_DMACH1(SN_ADC0_Type *adc)
{
    LL_REG_WRITE(adc->RIS, ADC_RIS_DMACH1IF);
}

/**
 * @brief  Judge ADC DMA Channel 1 interrupt flag
 * @param  adc ADC Base Address
 * @retval 0 : not active;  1 : active
 */
__STATIC_INLINE uint32_t LL_ADC_IsActiveFlag_DMACH1(SN_ADC0_Type *adc)
{
    return ((LL_REG_READ(adc->RIS) & ADC_RIS_DMACH1IF) >> ADC_RIS_DMACH1IF_Pos);
}

/**
 * @brief  Clear ADC DMA Channel 2 interrupt flag
 * @param  adc ADC Base Address
 * @retval None
 */
__STATIC_INLINE void LL_ADC_ClearFlag_DMACH2(SN_ADC0_Type *adc)
{
    LL_REG_WRITE(adc->RIS, ADC_RIS_DMACH2IF);
}

/**
 * @brief  Judge ADC DMA Channel 2 interrupt flag
 * @param  adc ADC Base Address
 * @retval 0 : not active;  1 : active
 */
__STATIC_INLINE uint32_t LL_ADC_IsActiveFlag_DMACH2(SN_ADC0_Type *adc)
{
    return ((LL_REG_READ(adc->RIS) & ADC_RIS_DMACH2IF) >> ADC_RIS_DMACH2IF_Pos);
}

/**
 * @brief  Clear ADC DMA Channel 3 interrupt flag
 * @param  adc ADC Base Address
 * @retval None
 */
__STATIC_INLINE void LL_ADC_ClearFlag_DMACH3(SN_ADC0_Type *adc)
{
    LL_REG_WRITE(adc->RIS, ADC_RIS_DMACH3IF);
}

/**
 * @brief  Judge ADC DMA Channel 3 interrupt flag
 * @param  adc ADC Base Address
 * @retval 0 : not active;  1 : active
 */
__STATIC_INLINE uint32_t LL_ADC_IsActiveFlag_DMACH3(SN_ADC0_Type *adc)
{
    return ((LL_REG_READ(adc->RIS) & ADC_RIS_DMACH3IF) >> ADC_RIS_DMACH3IF_Pos);
}

/**
 * @brief  Set ADC power enable cycle number of MCLK
 * @param  adc ADC Base Address
 * @param  number This param ranges from 1 to 65535
 * @retval None
 */
__STATIC_INLINE void LL_ADC_SetPowerEnableCycleNumber(SN_ADC0_Type *adc, uint32_t number)
{
    LL_REG_MBIT(adc->TPARM, ADC_TPARM_PWRENTIME, (number == 0 ? 0 : number - 1) << ADC_TPARM_PWRENTIME_Pos);
}

/**
 * @brief  Get ADC power enable cycle number of MCLK
 * @param  adc ADC Base Address
 * @retval power enable cycle number of MCLK
 */
__STATIC_INLINE uint32_t LL_ADC_GetPowerEnableCycleNumber(SN_ADC0_Type *adc)
{
    return (((LL_REG_READ(adc->TPARM) & ADC_TPARM_PWRENTIME) >> ADC_TPARM_PWRENTIME_Pos) + 1);
}

/**
 * @brief  Set ADC wakeup time
 * @param  adc ADC Base Address
 * @param  time This param refer to \ref adc_wakeup_time
 * @retval None
 */
__STATIC_INLINE void LL_ADC_SetWakeupTime(SN_ADC0_Type *adc, uint32_t time)
{
    LL_REG_MBIT(adc->TPARM, ADC_TPARM_WKUPTIME, time);
}

/**
 * @brief  Get ADC wakeup time
 * @param  adc ADC Base Address
 * @retval wakeup conversion cycle
 */
__STATIC_INLINE uint32_t LL_ADC_GetWakeupTime(SN_ADC0_Type *adc)
{
    return ((LL_REG_READ(adc->TPARM) & ADC_TPARM_WKUPTIME) >> ADC_TPARM_WKUPTIME_Pos);
}

/**
 * @brief  Set Analog input channel settling cycle number of MCLK
 * @param  adc ADC Base Address
 * @param  time This param ranges from 1 to 256
 * @note   time is 1T if input is 0/1, it's minimus time
 * @retval None
 */
__STATIC_INLINE void LL_ADC_SetSettleTime(SN_ADC0_Type *adc, uint32_t time)
{
    LL_REG_MBIT(adc->SMPR, ADC_SMPR_SETTLTIME, (time == 0 ? 0 : time - 1) << ADC_SMPR_SETTLTIME_Pos);
}

/**
 * @brief  Get ADC settling cycle number of MCLK
 * @param  adc ADC Base Address
 * @retval settling cycle number of MCLK
 */
__STATIC_INLINE uint32_t LL_ADC_GetSettleTime(SN_ADC0_Type *adc)
{
    return (((LL_REG_READ(adc->SMPR) & ADC_SMPR_SETTLTIME) >> ADC_SMPR_SETTLTIME_Pos) + 1);
}

/**
 * @brief  Set Analog input channel sample clock high level cycle number of MCLK
 * @param  adc ADC Base Address
 * @param  time This param ranges from 1 to 16
 * @note   time is 1T if input is 0/1, it's minimus time
 * @retval None
 */
__STATIC_INLINE void LL_ADC_SetSampleTime(SN_ADC0_Type *adc, uint32_t time)
{
    LL_REG_MBIT(adc->SMPR, ADC_SMPR_SAMPLTIME, (time == 0 ? 0 : time - 1) << ADC_SMPR_SAMPLTIME_Pos);
}

/**
 * @brief  Get ADC sample clock high level cycle number of MCLK
 * @param  adc ADC Base Address
 * @retval sample clock high level cycle number of MCLK
 */
__STATIC_INLINE uint32_t LL_ADC_GetSampleTime(SN_ADC0_Type *adc)
{
    return (((LL_REG_READ(adc->SMPR) & ADC_SMPR_SAMPLTIME) >> ADC_SMPR_SAMPLTIME_Pos) + 1);
}

/**
 * @brief  Set Analog input channel hold cycle number of MCLK
 * @param  adc ADC Base Address
 * @param  time This param ranges from 0 to 15
 * @retval None
 */
__STATIC_INLINE void LL_ADC_SetHoldTime(SN_ADC0_Type *adc, uint32_t time)
{
    LL_REG_MBIT(adc->SMPR, ADC_SMPR_HOLDTIME, time << ADC_SMPR_HOLDTIME_Pos);
}

/**
 * @brief  Get ADC hold cycle number of MCLK
 * @param  adc ADC Base Address
 * @retval hold cycle number of MCLK
 */
__STATIC_INLINE uint32_t LL_ADC_GetHoldTime(SN_ADC0_Type *adc)
{
    return ((LL_REG_READ(adc->SMPR) & ADC_SMPR_HOLDTIME) >> ADC_SMPR_HOLDTIME_Pos);
}

/**
 * @brief  Set Analog input channel change discharge cycle number of MCLK
 * @param  adc ADC Base Address
 * @param  time This param ranges from 1 to 8
 * @note   time is 1T if input is 0/1, it's minimus time
 * @retval None
 */
__STATIC_INLINE void LL_ADC_SetCHSELTime(SN_ADC0_Type *adc, uint32_t time)
{
    LL_REG_MBIT(adc->SMPR, ADC_SMPR_CHSELTIME, (time == 0 ? 0 : time - 1) << ADC_SMPR_CHSELTIME_Pos);
}

/**
 * @brief  Get ADC change discharge cycle number of MCLK
 * @param  adc ADC Base Address
 * @retval change discharge cycle number of MCLK
 */
__STATIC_INLINE uint32_t LL_ADC_GetCHSELTime(SN_ADC0_Type *adc)
{
    return (((LL_REG_READ(adc->SMPR) & ADC_SMPR_CHSELTIME) >> ADC_SMPR_CHSELTIME_Pos) + 1);
}

/**
 * @brief  Set Delay time cycle number of MCLK for each channel sampling
 * @param  adc ADC Base Address
 * @param  time This param ranges from 1 to 256
 * @note   time is 1T if input is 0/1, it's minimus time
 * @retval None
 */
__STATIC_INLINE void LL_ADC_SetCHDLYTime(SN_ADC0_Type *adc, uint32_t time)
{
    LL_REG_MBIT(adc->SMPR, ADC_SMPR_CHDLYTIME, (time == 0 ? 0 : time - 1) << ADC_SMPR_CHDLYTIME_Pos);
}

/**
 * @brief  Get Delay time cycle number of MCLK for each channel sampling
 * @param  adc ADC Base Address
 * @retval Delay time cycle number of MCLK
 */
__STATIC_INLINE uint32_t LL_ADC_GetCHDLYTime(SN_ADC0_Type *adc)
{
    return (((LL_REG_READ(adc->SMPR) & ADC_SMPR_CHDLYTIME) >> ADC_SMPR_CHDLYTIME_Pos) + 1);
}

/**
 * @brief  Set Sensing Discharge cycle number of MCLK
 * @param  adc ADC Base Address
 * @param  time This param ranges from 1 to 65536
 * @note   time is 1T if input is 0/1, it's minimus time
 * @retval None
 */
__STATIC_INLINE void LL_ADC_SetDischargeTime(SN_ADC0_Type *adc, uint32_t time)
{
    LL_REG_MBIT(adc->DISCHTIME, ADC_DISCHTIME_DISCHTIME, (time == 0 ? 0 : time - 1) << ADC_DISCHTIME_DISCHTIME_Pos);
}

/**
 * @brief  Get Sensing Discharge cycle number of MCLK
 * @param  adc ADC Base Address
 * @retval Sensing Discharge cycle number of MCLK
 */
__STATIC_INLINE uint32_t LL_ADC_GetDischargeTime(SN_ADC0_Type *adc)
{
    return (((LL_REG_READ(adc->DISCHTIME) & ADC_DISCHTIME_DISCHTIME) >> ADC_DISCHTIME_DISCHTIME_Pos) + 1);
}

/**
 * @brief  Configure ADC scan prescaler clock.
 * @param  adc ADC Base Address
 * @param  clock_pre ADC MCLK prescaler.
 * @retval None
 */
__STATIC_INLINE void LL_ADC_SetClockPrescaler(SN_ADC0_Type *adc, uint32_t clock_pre)
{
    LL_REG_MBIT(adc->PRE, ADC_PRE_MCLKDIV_Msk, (clock_pre == 0 ? 0 : clock_pre - 1));
}

/**
 * @brief  Get ADC MCLK prescaler.
 * @param  adc ADC Base Address
 * @retval prescaler division
 */
__STATIC_INLINE uint32_t LL_ADC_GetClockPrescaler(SN_ADC0_Type *adc)
{
    uint32_t clock_pre = ((LL_REG_READ(adc->PRE) & ADC_PRE_MCLKDIV) >> ADC_PRE_MCLKDIV_Pos);

    return (clock_pre == 0 ? 0 : clock_pre + 1);
}

/**
 * @brief  Set ADC channel x in rank n.
 * @param  adc ADC Base Address
 * @param  rank This parameter can be one of \ref adc_sequence_rank
 * @param  channel This parameter can be one of \ref adc_channel
 * @retval None
 */
__STATIC_INLINE void LL_ADC_SetSequencerRanks(SN_ADC0_Type *adc, uint32_t rank, uint32_t channel)
{
    volatile uint8_t *sqc = (uint8_t *)&(adc->SQR0) + rank;
    LL_REG_MBIT(*sqc, ADC_SQRx_SQC_Msk, channel);
}

/**
 * @brief  Get ADC group regular sequence: channel on the selected scan sequence rank.
 * @param  adc ADC Base Address
 * @param  rank This parameter can be one of \ref adc_channel
 * @retval channel
 */
__STATIC_INLINE uint32_t LL_ADC_GetSequencerRanks(SN_ADC0_Type *adc, uint32_t rank)
{
    volatile uint8_t *sqc = (uint8_t *)&(adc->SQR0) + rank;
    return (uint32_t)(*sqc & ADC_SQRx_SQC_Msk);
}

/**
 * @brief  Enable ADC EOC interrupt
 * @param  adc ADC Base Address
 * @param  channel This param assigns which channel (0-17) to enable EOC interrupt
 * @retval None
 */
__STATIC_INLINE void LL_ADC_EnableIT_EOC(SN_ADC0_Type *adc, uint32_t channel)
{
    LL_REG_SBIT(adc->EOCIE, 1 << channel);
}

/**
 * @brief  Disable ADC EOC interrupt
 * @param  adc ADC Base Address
 * @param  channel This param assigns which channel (0-17) to disable EOC interrupt
 * @retval None
 */
__STATIC_INLINE void LL_ADC_DisableIT_EOC(SN_ADC0_Type *adc, uint32_t channel)
{
    LL_REG_CBIT(adc->EOCIE, 1 << channel);
}

/**
 * @brief  Judge ADC EOC interrupt if enabled
 * @param  adc ADC Base Address
 * @param  channel This param assigns which channel (0-17) to judge EOC interrupt
 * @retval 1: Enabled   0: Disabled
 */
__STATIC_INLINE uint32_t LL_ADC_IsEnableIT_EOC(SN_ADC0_Type *adc, uint32_t channel)
{
    return (((LL_REG_READ(adc->EOCIE) & 0x1U << channel) >> channel) == 1);
}

/**
 * @brief  Clear ADC All EOC interrupt
 * @param  adc ADC Base Address
 * @retval None
 */
__STATIC_INLINE void LL_ADC_DisableIT_AllEOC(SN_ADC0_Type *adc)
{
    LL_REG_WRITE(adc->EOCIE, 0);
}

/**
 * @brief  Enable ADC Threshold Interrupt
 * @param  adc ADC Base Address
 * @param  threhold_detection Threshold detection x interrupt, this param ranges from 0 to 31
 * @retval None
 */
__STATIC_INLINE void LL_ADC_EnableIT_THDET0_31(SN_ADC0_Type *adc, uint32_t threhold_detection)
{
    LL_REG_SBIT(adc->THRDIE0, 1 << threhold_detection);
}

/**
 * @brief  Enable ADC Threshold Interrupt
 * @param  adc ADC Base Address
 * @param  threhold_detection Threshold detection x interrupt, this param ranges from 32 to 35
 * @retval None
 */
__STATIC_INLINE void LL_ADC_EnableIT_THDET32_35(SN_ADC0_Type *adc, uint32_t threhold_detection)
{
    LL_REG_SBIT(adc->THRDIE1, 1 << (threhold_detection - 32));
}

/**
 * @brief  Disable ADC Threshold Interrupt
 * @param  adc ADC Base Address
 * @param  threhold_detection Threshold detection x interrupt, this param ranges from 0 to 31
 * @retval None
 */
__STATIC_INLINE void LL_ADC_DisableIT_THDET0_31(SN_ADC0_Type *adc, uint32_t threhold_detection)
{
    LL_REG_CBIT(adc->THRDIE0, 1 << threhold_detection);
}

/**
 * @brief  Disable ADC Threshold Interrupt
 * @param  adc ADC Base Address
 * @param  threhold_detection Threshold detection x interrupt, this param ranges from 32 to 35
 * @retval None
 */
__STATIC_INLINE void LL_ADC_DisableIT_THDET32_35(SN_ADC0_Type *adc, uint32_t threhold_detection)
{
    LL_REG_CBIT(adc->THRDIE1, 1 << (threhold_detection - 32));
}

/**
 * @brief  Judge ADC Threshold (0-31) Interrupt if enabled
 * @param  adc ADC Base Address
 * @param  threhold_detection Threshold detection x interrupt, this param ranges from 0 to 31
 * @retval 1: Enabled   0: Disabled
 */
__STATIC_INLINE uint32_t LL_ADC_IsEnableIT_THDET0_31(SN_ADC0_Type *adc, uint32_t threhold_detection)
{
    return (((LL_REG_READ(adc->THRDIE0) & 0x1U << threhold_detection) >> threhold_detection) == 1);
}

/**
 * @brief  Judge ADC Threshold (32-35) Interrupt if enabled
 * @param  adc ADC Base Address
 * @param  threhold_detection Threshold detection x interrupt, this param ranges from 0 to 31
 * @retval 1: Enabled   0: Disabled
 */
__STATIC_INLINE uint32_t LL_ADC_IsEnableIT_THDET32_35(SN_ADC0_Type *adc, uint32_t threhold_detection)
{
    return (((LL_REG_READ(adc->THRDIE1) & 0x1U << (threhold_detection - 32)) >> (threhold_detection - 32)) == 1);
}

/**
 * @brief  Get ADC EOCIS register
 * @param  adc ADC Base Address
 * @retval EOCIS register value
 */
__STATIC_INLINE uint32_t LL_ADC_GetEOCInterruptStatus(SN_ADC0_Type *adc)
{
    return LL_REG_READ(adc->EOCIS);
}

/**
 * @brief  Clear ADC EOCIS register
 * @param  adc ADC Base Address
 * @retval None
 */
__STATIC_INLINE void LL_ADC_ClearEOCInterruptStatus(SN_ADC0_Type *adc, uint32_t flags)
{
    LL_REG_WRITE(adc->EOCIS, flags);
}

/**
 * @brief  Judge ADC channel x EOC interrupt flag
 * @param  adc ADC Base Address
 * @param  channel Which channel to read, \ref adc_channel
 * @retval 0 : not active;  1 : active
 */
__STATIC_INLINE uint32_t LL_ADC_IsActiveFlag_EOC(SN_ADC0_Type *adc, uint32_t channel)
{
    return (((LL_REG_READ(adc->EOCIS) >> channel) & 0x1) == 1);
}

/**
 * @brief  Clear ADC channel x EOC interrupt flag
 * @param  adc ADC Base Address
 * @param  channel Which channel to read, \ref adc_channel
 * @retval None
 */
__STATIC_INLINE void LL_ADC_ClearFlag_EOC(SN_ADC0_Type *adc, uint32_t channel)
{
    LL_REG_WRITE(adc->EOCIS, 1 << channel);
}

/**
 * @brief  Clear ADC all channel EOC interrupt flag
 * @param  adc ADC Base Address
 * @retval None
 */
__STATIC_INLINE void LL_ADC_ClearFlag_AllEOC(SN_ADC0_Type *adc)
{
    LL_REG_WRITE(adc->EOCIS, 0x3FFFF);
}

/**
 * @brief  Get ADC THRDIS0 register
 * @param  adc ADC Base Address
 * @retval THRDIS0 register value
 */
__STATIC_INLINE uint32_t LL_ADC_GetTHRD0InterruptStatus(SN_ADC0_Type *adc)
{
    return LL_REG_READ(adc->THRDIS0);
}

/**
 * @brief  Clear ADC THRDIS0 register
 * @param  adc ADC Base Address
 * @retval None
 */
__STATIC_INLINE void LL_ADC_ClearTHRD0InterruptStatus(SN_ADC0_Type *adc, uint32_t flags)
{
    LL_REG_WRITE(adc->THRDIS0, flags);
}

/**
 * @brief  Get ADC THRDIS1 register
 * @param  adc ADC Base Address
 * @retval THRDIS1 register value
 */
__STATIC_INLINE uint32_t LL_ADC_GetTHRD1InterruptStatus(SN_ADC0_Type *adc)
{
    return LL_REG_READ(adc->THRDIS1);
}

/**
 * @brief  Clear ADC THRDIS1 register
 * @param  adc ADC Base Address
 * @retval None
 */
__STATIC_INLINE void LL_ADC_ClearTHRD1InterruptStatus(SN_ADC0_Type *adc, uint32_t flags)
{
    LL_REG_WRITE(adc->THRDIS1, flags);
}

/**
 * @brief  Judge ADC Threshold detection x interrupt flag
 * @param  adc ADC Base Address
 * @param  threhold_detection Threshold detection x interrupt, this param ranges from 0 to 31
 * @retval Value is 0(not active) or 1 (active)
 */
__STATIC_INLINE uint32_t LL_ADC_IsActiveFlag_THDET0_31(SN_ADC0_Type *adc, uint32_t threhold_detection)
{
    return (((LL_REG_READ(adc->THRDIS0) >> threhold_detection) & 0x1) == 1);
}

/**
 * @brief  Judge ADC Threshold detection x interrupt flag
 * @param  adc ADC Base Address
 * @param  threhold_detection Threshold detection x interrupt, this param ranges from 32 to 35
 * @retval Value is 0(not active) or 1 (active)
 */
__STATIC_INLINE uint32_t LL_ADC_IsActiveFlag_THDET32_35(SN_ADC0_Type *adc, uint32_t threhold_detection)
{
    return (((LL_REG_READ(adc->THRDIS1) >> (threhold_detection - 32)) & 0x1) == 1);
}

/**
 * @brief  Clear ADC Threshold detection x interrupt flag
 * @param  adc ADC Base Address
 * @param  threhold_detection Threshold detection x interrupt, this param ranges from 0 to 31
 * @retval None
 */
__STATIC_INLINE void LL_ADC_ClearFlag_THDET0_31(SN_ADC0_Type *adc, uint32_t threhold_detection)
{
    LL_REG_WRITE(adc->THRDIS0, 1 << threhold_detection);
}

/**
 * @brief  Clear ADC Threshold detection x interrupt flag
 * @param  adc ADC Base Address
 * @param  threhold_detection Threshold detection x interrupt, this param ranges from 32 to 35
 * @retval None
 */
__STATIC_INLINE void LL_ADC_ClearFlag_THDET32_35(SN_ADC0_Type *adc, uint32_t threhold_detection)
{
    LL_REG_WRITE(adc->THRDIS1, 1 << (threhold_detection - 32));
}

/**
 * @brief  Read ADC data port register even conversion data(DMA_FIFO1)
 * @param  adc ADC Base Address
 * @param  dma Which dma data port to read, ranges from 0 to 3
 * @retval 0 : not active;  1 : active
 */
__STATIC_INLINE uint32_t LL_ADC_ReadDataPortEvenData(SN_ADC0_Type *adc, uint32_t dma)
{
    volatile uint32_t *sqc = (uint32_t *)&(adc->DMA0DAT) + 4 * dma;
    return ((*sqc & ADC_DMADAT_DMAFIFO1) >> ADC_DMADAT_DMAFIFO1_Pos);
}

/**
 * @brief  Read ADC data port register odd conversion data(DMA_FIFO0)
 * @param  adc ADC Base Address
 * @param  dma Which dma data port to read, ranges from 0 to 3
 * @retval 0 : not active;  1 : active
 */
__STATIC_INLINE uint32_t LL_ADC_ReadDataPortOddData(SN_ADC0_Type *adc, uint32_t dma)
{
    volatile uint32_t *sqc = (uint32_t *)&(adc->DMA0DAT) + 4 * dma;
    return ((*sqc & ADC_DMADAT_DMAFIFO0) >> ADC_DMADAT_DMAFIFO0_Pos);
}

/**
 * @brief  DMA Injected data from all conversion channels
 * @param  adc ADC Base Address
 * @param  dma which dma to operate
 * @retval None
 */
__STATIC_INLINE void LL_ADC_SetDMAxDataFromALL(SN_ADC0_Type *adc, uint32_t dma)
{
    LL_REG_SBIT(*((&adc->DMA0CTRL) + 4 * dma), ADC_DMAxCTRL_DMA_INJECT_ALL);
}

/**
 * @brief  DMAx Injected data from selected channel
 * @param  adc ADC Base Address
 * @param  dma which dma to operate
 * @retval None
 */
__STATIC_INLINE void LL_ADC_SetDMADataFromSelected(SN_ADC0_Type *adc, uint32_t dma)
{
    LL_REG_CBIT(*((&adc->DMA0CTRL) + 4 * dma), ADC_DMAxCTRL_DMA_INJECT);
}

/**
 * @brief  Get DMAx injected data source
 * @param  adc ADC Base Address
 * @param  dma which dma to operate
 * @retval 1: from all conversion;   0: from selected channel;  Other: Reserved
 */
__STATIC_INLINE uint32_t LL_ADC_GetDMADataSource(SN_ADC0_Type *adc, uint32_t dma)
{
    return ((*((&adc->DMA0CTRL) + 4 * dma) & ADC_DMAxCTRL_DMA_INJECT) >> ADC_DMAxCTRL_DMA_INJECT_Pos);
}

/**
 * @brief  Enable ADC DMAx
 * @param  adc ADC Base Address
 * @param  dma which dma to operate
 * @retval None
 */
__STATIC_INLINE void LL_ADC_Enable_DMA(SN_ADC0_Type *adc, uint32_t dma)
{
    LL_REG_SBIT(*((&adc->DMA0CTRL) + 4 * dma), ADC_DMAxCTRL_DMA_EN);
}

/**
 * @brief  Disable ADC DMAx
 * @param  adc ADC Base Address
 * @param  dma which dma to operate
 * @retval None
 */
__STATIC_INLINE void LL_ADC_Disable_DMA(SN_ADC0_Type *adc, uint32_t dma)
{
    LL_REG_CBIT(*((&adc->DMA0CTRL) + 4 * dma), ADC_DMAxCTRL_DMA_EN);
}

/**
 * @brief  Judge if Enabled ADC DMAx
 * @param  adc ADC Base Address
 * @param  dma which dma to operate
 * @retval 1: Enabled;   0: Disabled
 */
__STATIC_INLINE uint32_t LL_ADC_IsEnabled_DMA(SN_ADC0_Type *adc, uint32_t dma)
{
    return ((*((&adc->DMA0CTRL) + 4 * dma) & ADC_DMAxCTRL_DMA_EN) == ADC_DMAxCTRL_DMA_EN);
}

/**
 * @brief  Set DMAx trigger threshold
 * @param  adc ADC Base Address
 * @param  dma which dma to operate
 * @param  threshold This param refer to \ref adc_dma_threshold
 * @retval None
 */
__STATIC_INLINE void LL_ADC_SetDMAThreshold(SN_ADC0_Type *adc, uint32_t dma, uint32_t threshold)
{
    LL_REG_MBIT(*((&adc->DMA0CTRL) + 4 * dma), ADC_DMAxCTRL_DMA_THD, threshold);
}

/**
 * @brief  Get DMAx trigger threshold
 * @param  adc ADC Base Address
 * @param  dma which dma to operate, ranges from 0 to 3
 * @retval None
 */
__STATIC_INLINE uint32_t LL_ADC_GetDMAThreshold(SN_ADC0_Type *adc, uint32_t dma)
{
    return ((*((&adc->DMA0CTRL) + 4 * dma) & ADC_DMAxCTRL_DMA_THD) >> ADC_DMAxCTRL_DMA_THD_Pos);
}

/**
 * @brief  Set DMAx injected channel
 * @param  adc ADC Base Address
 * @param  dma which dma to operate, ranges from 0 to 3
 * @param  channel This param ranges from 0 to 17, or \ref adc_channel
 * @retval None
 */
__STATIC_INLINE void LL_ADC_SetDMAChannel(SN_ADC0_Type *adc, uint32_t dma, uint32_t channel)
{
    LL_REG_MBIT(*((&adc->DMA0CTRL) + 4 * dma), ADC_DMAxCTRL_DMA_CH, channel & ADC_DMAxCTRL_DMA_CH);
}

/**
 * @brief  Get DMAx injected channel
 * @param  adc ADC Base Address
 * @param  dma which dma to operate, ranges from 0 to 3
 * @retval None
 */
__STATIC_INLINE uint32_t LL_ADC_GetDMAChannel(SN_ADC0_Type *adc, uint32_t dma)
{
    return ((*((&adc->DMA0CTRL) + 4 * dma) & ADC_DMAxCTRL_DMA_CH) >> ADC_DMAxCTRL_DMA_CH_Pos);
}

/**
 * @brief  Enable ADC DMA underrun interrupt enable
 * @param  adc ADC Base Address
 * @param  dma Which dma to operate, ranges from 0 to 3
 * @retval None
 */
__STATIC_INLINE void LL_ADC_EnableIT_Underrun(SN_ADC0_Type *adc, uint32_t dma)
{
    LL_REG_SBIT(*(&adc->DMA0IE + 4 * dma), ADC_DMAIE_DMA_UDRIE);
}

/**
 * @brief  Disable ADC DMA underrun interrupt
 * @param  adc ADC Base Address
 * @param  dma Which dma to operate, ranges from 0 to 3
 * @retval None
 */
__STATIC_INLINE void LL_ADC_DisableIT_Underrun(SN_ADC0_Type *adc, uint32_t dma)
{
    LL_REG_CBIT(*(&adc->DMA0IE + 4 * dma), ADC_DMAIE_DMA_UDRIE);
}

/**
 * @brief  Judge ADC underrun interrupt if enabled
 * @param  adc ADC Base Address
 * @param  dma Which dma to operate, ranges from 0 to 3
 * @retval 1: Enabled   0: Disabled
 */
__STATIC_INLINE uint32_t LL_ADC_IsEnableIT_Underrun(SN_ADC0_Type *adc, uint32_t dma)
{
    return ((*(&adc->DMA0IE + 4 * dma) & ADC_DMAIE_DMA_UDRIE) == ADC_DMAIE_DMA_UDRIE);
}

/**
 * @brief  Enable ADC DMA overrun interrupt enable
 * @param  adc ADC Base Address
 * @param  dma Which dma data port to read, ranges from 0 to 3
 * @retval None
 */
__STATIC_INLINE void LL_ADC_EnableIT_Overrun(SN_ADC0_Type *adc, uint32_t dma)
{
    LL_REG_SBIT(*(&adc->DMA0IE + 4 * dma), ADC_DMAIE_DMA_OVRIE);
}

/**
 * @brief  Disable ADC DMA overrun interrupt
 * @param  adc ADC Base Address
 * @param  dma Which dma to operate, ranges from 0 to 3
 * @retval None
 */
__STATIC_INLINE void LL_ADC_DisableIT_Overrun(SN_ADC0_Type *adc, uint32_t dma)
{
    LL_REG_CBIT(*(&adc->DMA0IE + 4 * dma), ADC_DMAIE_DMA_OVRIE);
}

/**
 * @brief  Judge ADC overrun interrupt if enabled
 * @param  adc ADC Base Address
 * @param  dma Which dma to operate, ranges from 0 to 3
 * @retval 1: Enabled   0: Disabled
 */
__STATIC_INLINE uint32_t LL_ADC_IsEnableIT_Overrun(SN_ADC0_Type *adc, uint32_t dma)
{
    return ((*(&adc->DMA0IE + 4 * dma) & ADC_DMAIE_DMA_OVRIE) == ADC_DMAIE_DMA_OVRIE);
}

/**
 * @brief  Get ADC DMAnIS (n = 0,1,2,3) register value
 * @param  adc ADC Base Address
 * @param  dma DMA index
 * @retval DMAnIS register value
 */
__STATIC_INLINE uint32_t LL_ADC_GetDMAnISRegValue(SN_ADC0_Type *adc, uint32_t dma)
{
    return LL_REG_READ(*(&adc->DMA0IS + 4 * dma));
}

/**
 * @brief  Jugde ADC DMA underrun interrupt flag if active
 * @param  adc ADC Base Address
 * @param  dma Which dma data port to read, ranges from 0 to 3
 * @retval 0 : not active;  1 : active
 */
__STATIC_INLINE uint32_t LL_ADC_IsActiveFlag_Underrun(SN_ADC0_Type *adc, uint32_t dma)
{
    return ((LL_REG_READ(*(&adc->DMA0IS + 4 * dma)) & ADC_DMAIS_DMA_UDRIF) == ADC_DMAIS_DMA_UDRIF);
}

/**
 * @brief  Jugde ADC DMA overrun interrupt flag if active
 * @param  adc ADC Base Address
 * @param  dma Which dma data port to read, ranges from 0 to 3
 * @retval 0 : not active;  1 : active
 */
__STATIC_INLINE uint32_t LL_ADC_IsActiveFlag_Overrun(SN_ADC0_Type *adc, uint32_t dma)
{
    return ((LL_REG_READ(*(&adc->DMA0IS + 4 * dma)) & ADC_DMAIS_DMA_OVRIF) == ADC_DMAIS_DMA_OVRIF);
}

/**
 * @brief  Clear ADC DMA underrun interrupt flag
 * @param  adc ADC Base Address
 * @param  dma Which dma data port to read, ranges from 0 to 3
 * @retval None
 */
__STATIC_INLINE void LL_ADC_ClearFlag_Underrun(SN_ADC0_Type *adc, uint32_t dma)
{
    LL_REG_WRITE((*(&adc->DMA0IS + 4 * dma)), ADC_DMAIS_DMA_UDRIF);
}

/**
 * @brief  Clear ADC DMA overrun interrupt flag
 * @param  adc ADC Base Address
 * @param  dma Which dma data port to read, ranges from 0 to 3
 * @retval None
 */
__STATIC_INLINE void LL_ADC_ClearFlag_Overrun(SN_ADC0_Type *adc, uint32_t dma)
{
    LL_REG_WRITE((*(&adc->DMA0IS + 4 * dma)), ADC_DMAIS_DMA_OVRIF);
}

/**
 * @brief  Jugde ADC DMA FIFO if empty
 * @param  adc ADC Base Address
 * @param  dma Which dma data port to read, ranges from 0 to 3
 * @retval 0 : not active;  1 : active
 */
__STATIC_INLINE uint32_t LL_ADC_IsActiveFlag_FIFOEmpty(SN_ADC0_Type *adc, uint32_t dma)
{
    return ((LL_REG_READ(*(&adc->DMA0IS + 4 * dma)) & ADC_DMAIS_RPTR_EMPTY) == ADC_DMAIS_RPTR_EMPTY);
}

/**
 * @brief  Jugde ADC DMA FIFO if Full according to DMA_THD
 * @param  adc ADC Base Address
 * @param  dma Which dma data port to read, ranges from 0 to 3
 * @retval 0 : not active;  1 : active
 */
__STATIC_INLINE uint32_t LL_ADC_IsActiveFlag_FIFOFull_THD(SN_ADC0_Type *adc, uint32_t dma)
{
    return ((LL_REG_READ(*(&adc->DMA0IS + 4 * dma)) & ADC_DMAIS_FIFO_HFULL) == ADC_DMAIS_FIFO_HFULL);
}

/**
 * @brief  Jugde ADC DMA FIFO if Full
 * @param  adc ADC Base Address
 * @param  dma Which dma data port to read, ranges from 0 to 3
 * @retval 0 : not active;  1 : active
 */
__STATIC_INLINE uint32_t LL_ADC_IsActiveFlag_FIFOFull(SN_ADC0_Type *adc, uint32_t dma)
{
    return ((LL_REG_READ(*(&adc->DMA0IS + 4 * dma)) & ADC_DMAIS_WPTR_FULL) == ADC_DMAIS_WPTR_FULL);
}

/**
 * @brief  Get ADC DMA FIFO Count
 * @param  adc ADC Base Address
 * @param  dma Which dma data port to read, ranges from 0 to 3
 * @retval Value between 0 and 31
 */
__STATIC_INLINE uint32_t LL_ADC_GetFIFOCount(SN_ADC0_Type *adc, uint32_t dma)
{
    return ((LL_REG_READ(*(&adc->DMA0IS + 4 * dma)) & ADC_DMAIS_FIFO_DAT_CNT) >> ADC_DMAIS_FIFO_DAT_CNT_Pos);
}

/**
 * @brief  Set ADC Threshold detect x value
 * @param  adc ADC Base Address
 * @param  threhold_detection Which one to set, ranges from 0 to 35
 * @param  threshold_val Threshold detect value, ranges from 0 to 4095
 * @retval None
 */
__STATIC_INLINE void LL_ADC_SetTHDECValue(SN_ADC0_Type *adc, uint32_t threhold_detection, uint32_t threshold_val)
{
    LL_REG_SBIT((*(&adc->THRHOLD0 + threhold_detection)), threshold_val << ADC_THRHOLD_THRHD_VAL_Pos);
}

/**
 * @brief  Get ADC Threshold detect x value
 * @param  adc ADC Base Address
 * @param  threhold_detection Which one to read, ranges from 0 to 35
 * @retval Value ranges from 0 to 4095
 */
__STATIC_INLINE uint32_t LL_ADC_GetTHDECValue(SN_ADC0_Type *adc, uint32_t threhold_detection)
{
    return ((LL_REG_READ(*(&adc->THRHOLD0 + threhold_detection)) & ADC_THRHOLD_THRHD_VAL) >> ADC_THRHOLD_THRHD_VAL_Pos);
}

/**
 * @brief  Set Threshold detect for ADC channels
 * @param  adc ADC Base Address
 * @param  threhold_detection Which threshold dectection to set, ranges from 0 to 35
 * @param  channel Which channel to set, ranges from 0 to 17
 * @retval None
 */
__STATIC_INLINE void LL_ADC_SetTHDECToChannel(SN_ADC0_Type *adc, uint32_t threhold_detection, uint32_t channel)
{
    LL_REG_SBIT(*(&adc->THRHOLD0 + threhold_detection), channel << ADC_THRHOLD_THRHD_CH_Pos);
}

/**
 * @brief  Get Threshold detect belonging to which channel
 * @param  adc ADC Base Address
 * @param  threhold_detection Which threshold dectection to read, ranges from 0 to 35
 * @retval Value means threhold_detection n for channel x
 */
__STATIC_INLINE uint32_t LL_ADC_GetChannelByTHDEC(SN_ADC0_Type *adc, uint32_t threhold_detection)
{
    return ((LL_REG_READ(*(&adc->THRHOLD0 + threhold_detection)) & ADC_THRHOLD_THRHD_CH) >> ADC_THRHOLD_THRHD_CH_Pos);
}

/**
 * @brief  Set Over the threshold mode for HTR
 * @param  adc ADC Base Address
 * @param  threhold_detection Which threshold dectection to set, ranges from 0 to 35
 * @retval None
 */
__STATIC_INLINE void LL_ADC_SetTHDECForHTR(SN_ADC0_Type *adc, uint32_t threhold_detection)
{
    LL_REG_SBIT(*(&adc->THRHOLD0 + threhold_detection), ADC_THRHOLD_THRHD_MODE);
}

/**
 * @brief  Set Over the threshold mode for LTR
 * @param  adc ADC Base Address
 * @param  threhold_detection Which threshold dectection to set, ranges from 0 to 35
 * @retval None
 */
__STATIC_INLINE void LL_ADC_SetTHDECForLTR(SN_ADC0_Type *adc, uint32_t threhold_detection)
{
    LL_REG_CBIT(*(&adc->THRHOLD0 + threhold_detection), ADC_THRHOLD_THRHD_MODE);
}

/**
 * @brief  Read Threshold detect mode
 * @param  adc ADC Base Address
 * @param  threhold_detection Which threshold dectection to read, ranges from 0 to 35
 * @retval Value is 0 (LTR) or 1(HTR)
 */
__STATIC_INLINE uint32_t LL_ADC_GetTHDECMode(SN_ADC0_Type *adc, uint32_t threhold_detection)
{
    return ((LL_REG_READ(*(&adc->THRHOLD0 + threhold_detection)) & ADC_THRHOLD_THRHD_MODE) >> ADC_THRHOLD_THRHD_MODE_Pos);
}

#if defined(USE_FULL_LL_DRIVER)
LL_Status_T LL_ADC_Init(SN_ADC0_Type *adc, LL_ADC_Init_T *adc_init);
LL_Status_T LL_ADC_DeInit(SN_ADC0_Type *adc);
void        LL_ADC_StructInit(LL_ADC_Init_T *adc_init);
void        LL_ADC_Calibration(SN_ADC0_Type *adc, LL_ADC_Init_T *adc_init);
uint32_t    LL_ADC_GetMCLK(void);
#endif /* USE_FULL_LL_DRIVER */

#ifdef __cplusplus
}
#endif

#endif /* _LL_ADC_H_ */
