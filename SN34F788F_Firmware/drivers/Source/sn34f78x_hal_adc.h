/***********************
 * SN34F78X SDK V2.1.4 *
 ***********************/

#ifndef _SN34F78X_HAL_ADC_H_
#define _SN34F78X_HAL_ADC_H_

/* Includes ------------------------------------------------------------------*/
#include "sn34f78x_hal.h"

/* private types -------------------------------------------------------------*/
/* private variables ---------------------------------------------------------*/
/* private constants ---------------------------------------------------------*/

/* Private macros ------------------------------------------------------------*/
// check whether ADC instance is correct
#define IS_ADC_INSTANCE(HANDLE) ((HANDLE)->instance == SN_ADC0)

// check whether ADC converter mode is correct
#define IS_ADC_CONV_MODE(HANDLE) (((HANDLE)->init.conv_mode == ADC_CONVMODE_SINGLE_STEP) || ((HANDLE)->init.conv_mode == ADC_CONVMODE_SINGLE_SCAN) || \
                                  ((HANDLE)->init.conv_mode == ADC_CONVMODE_LOOP_SCAN))

// check whether ADC scan conversion channel numbers is correct
#define IS_ADC_SCAN_NUM(HANDLE) ((HANDLE)->init.scan_ch_num <= 0x12U)

// check whether ADC AUTOPWDN is correct
#define IS_ADC_AUTOPWDN(HANDLE) (((HANDLE)->init.autopwdn == HAL_DISABLE) || ((HANDLE)->init.autopwdn == HAL_ENABLE))

// check whether ADC is continue scan
#define IS_ADC_LOOP_SCAN(HANDLE) ((HANDLE)->init.conv_mode == ADC_CONVMODE_LOOP_SCAN)

// check whether ADC AUTOPWDN converter mode is correct
#define CHECK_AUTOPWDN_IN_SINGLE_MODE(HANDLE) (((HANDLE)->init.conv_mode != ADC_CONVMODE_LOOP_SCAN) || ((HANDLE)->init.autopwdn == HAL_DISABLE))

// check whether ADC prescaler clock is correct
#define IS_ADC_MCLK(HANDLE) (((HANDLE)->init.pre_clock > 0x00) && ((HANDLE)->init.pre_clock <= 0xFF))

// check whether ADC power_on time is correct
#define IS_ADC_PWRON_TIME(TIMING) ((TIMING)->power_en_time <= 0xFFFF)

// check whether ADC channel delay time is correct
#define IS_ADC_CH_DELAY_TIME(TIMING) ((TIMING)->ch_delay_time <= 0xFF)

// check whether ADC channel sel time is correct
#define IS_ADC_CH_SEL_TIME(TIMING) ((TIMING)->ch_sel_time <= 0x7)

// check whether ADC hold time is correct
#define IS_ADC_HOLD_TIME(TIMING) ((TIMING)->hold_time <= 0x6)

// check whether ADC sample time is correct
#define IS_ADC_SAMPL_TIME(TIMING) ((TIMING)->sampl_time <= 0xF)

// check whether ADC settling time is correct
#define IS_ADC_SETTL_TIME(TIMING) ((TIMING)->settl_time <= 0xFF)

// check whether ADC sensing discharge time is correct
#define IS_ADC_DISCH_TIME(TIMING) ((TIMING)->disch_time <= 0xFFFF)

// check whether ADC channel config is correct
#define IS_CHANNEL_CONFIG(CHANNEL) (((CHANNEL)->channel <= ADC_CHANNEL_VSS) && ((CHANNEL)->rank <= 0x12U))

// check whether ADC analog watchdog config is correct
#define IS_ANALOG_WDG(ANALOG) ((((ANALOG)->enable == HAL_DISABLE) || ((ANALOG)->enable == HAL_ENABLE)) &&                                                                                            \
                               ((ANALOG)->channel <= ADC_CHANNEL_VSS) &&                                                                                                                             \
                               (((ANALOG)->watchdog_mode == ADC_DETECT_LTR) || (((ANALOG)->watchdog_mode == ADC_DETECT_HTR)) || (((ANALOG)->watchdog_mode == (ADC_DETECT_HTR | ADC_DETECT_LTR)))) && \
                               (((ANALOG)->lower_threshold > 0x000) && ((ANALOG)->lower_threshold < 0xFFF)) &&                                                                                       \
                               (((ANALOG)->higher_threshold > 0x000) && ((ANALOG)->higher_threshold < 0xFFF)))

// check whether ADC reference option select is correct
#define IS_REF_OPTION(OPTION) ((OPTION == ADC_REF_EXTERNAL) || (OPTION == ADC_REF_VDD_GND) ||            \
                               (OPTION == ADC_REF_INTERNAL_2_5V) || (OPTION == ADC_REF_INTERNAL_2_0V) || \
                               (OPTION == ADC_REF_INTERNAL_1_5V))

// check whether ADC wakeup time is correct
#define IS_WAKEUP_TIME(TIME) ((TIME >= ADC_WKUP_3_CYCLES) && (TIME <= ADC_WKUP_7_CYCLES))

// check whether ADC DMA channel config is correct
#define IS_DMA_CHANNEL(DMA_CHANNEL) (((DMA_CHANNEL)->dma_channel <= ADC_DMA_CHANNEL_3) &&                                                                         \
                                     (((DMA_CHANNEL)->injected_mode == ADC_DMA_INJECT_SINGLE_CH) || ((DMA_CHANNEL)->injected_mode == ADC_DMA_INJECT_ALL_CONV)) && \
                                     ((DMA_CHANNEL)->injected_ch <= ADC_CHANNEL_VSS) &&                                                                           \
                                     ((DMA_CHANNEL)->fifo_threshold <= ADC_DMA_FIFO_32))

// check ADC SCANNUM when single step mode
#define CHECK_SCANNUM(HANDLE) (((HANDLE)->init.conv_mode != ADC_CONVMODE_SINGLE_STEP) || ((HANDLE)->init.scan_ch_num == 1) || ((HANDLE)->init.scan_ch_num == 0))

// check ADC channel rank when single step mode
#define CHECK_CHANNEL_RANK(HANDLE, CHANNEL) (((HANDLE)->init.conv_mode != ADC_CONVMODE_SINGLE_STEP) || ((CHANNEL)->rank == 1))

// ADC channel state
#define ADC_CHANNEL_STATE_RESET 0x00U
#define ADC_CHANNEL_STATE_READY 0x01U
#define ADC_CHANNEL_STATE_BUSY  0x02U

// ADC DMA state
#define ADC_DMA_READY 0x00U
#define ADC_DMA_BUSY  0x01U

// ADC power state
#define ADC_EXT_STATE_WAKE_UP         0x00U
#define ADC_EXT_STATE_CORE_POWER_DOWN 0x01U
#define ADC_EXT_STATE_LDO_POWER_DOWN  0x02U
#define ADC_EXT_STATE_AUTO_POWER_DOWN 0x04U

// ADC calibration times
#define ADC_CALIBRATION_TIMES 0x0AU

// ADC DMA FIFO ADDR
#define ADC_DMA_CH0_OFFSET_ADDR 0x200
#define ADC_DMA_CH1_OFFSET_ADDR 0x210
#define ADC_DMA_CH2_OFFSET_ADDR 0x220
#define ADC_DMA_CH3_OFFSET_ADDR 0x230

static uint32_t _adc_dma_ch_offset_addr[4] = {
    ADC_DMA_CH0_OFFSET_ADDR, // DMA CH0
    ADC_DMA_CH1_OFFSET_ADDR, // DMA CH1
    ADC_DMA_CH2_OFFSET_ADDR, // DMA CH2
    ADC_DMA_CH3_OFFSET_ADDR, // DMA CH3
};

/* Private function ----------------------------------------------------------*/
static void    _reset_base(ADC_Handle_T *hadc);
static void    _enable_adc(ADC_Handle_T *hadc);
static void    _start_adc(ADC_Handle_T *hadc, uint8_t ie);
static void    _stop_adc(ADC_Handle_T *hadc);
static uint8_t _get_channel_form_sqc(ADC_Handle_T *hadc, uint8_t sequence);
static void    _set_channel_sequence(ADC_Handle_T *hadc, ADC_Channel_T *ch);
static void    _offset_calibration(ADC_Handle_T *hadc);
static uint8_t _check_ref_option(ADC_Handle_T *hadc);
#if (configUSE_ADC_REGISTER_CALLBACKS == 1)
static void _reset_callback(ADC_Handle_T *hadc);
#endif
/* Sangle channel conversion */
#if (configUSE_ADC_NONBLOCKING_DMA == 1)
static void _stop_adc_dma(ADC_Handle_T *hadc);
static void _adc_dma_xfer_done_callback(DMA_Handle_T *hdma);
static void _adc_dma_xfer_abort_callback(DMA_Handle_T *hdma);
static void _adc_dma_xfer_error_callback(DMA_Handle_T *hdma);
#endif

#endif /* _SN34F78X_HAL_ADC_H_ */
