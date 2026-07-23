/**
 * @file sn34f78x_hal_conf.h
 * @author PD
 * @brief enum all peripheries and middlewares driver function.
 * The QS tool will copy current file to generated project
 * The APP project which created by manual should copy current to project.
 *
 * By enable/disable reference IP symbol to use or unused IP driver.
 * @version 1.0
 * @date 2023-10-25
 *
 */
//-------- <<< Use Configuration Wizard in Context Menu >>> --------------------

#ifndef _SN34F78X_HAL_CONF_H_
#define _SN34F78X_HAL_CONF_H_

#ifdef __cplusplus
extern "C" {
#endif

//<o0> Internal resistor of EHS X'TAL control
//                  <0=> Enable
//                  <1=> Disable
//<o1> EHS Source Frequency (MHz)   <10-25>
//      <i> EHS frequency range = 10 ~ 25
//
#define HAL_EHS_INR_DS   0  /**< Internal resistor of EHS X'TAL control */
#define HAL_EHS_FREQ_MHZ 12 /**< EHS Source Frequency (MHz) */

#define __EHS_FREQ    (HAL_EHS_FREQ_MHZ * 1000000UL) /**< EHS frequency(HZ)  */
#define __IHRC_FREQ   (12000000UL)                   /**< IHRC frequency */
#define HAL_ILRC_FREQ (32000UL)                      /**< ILRC frequency */

/**
 * \defgroup hal_common_rte_config_SYSTICK SysTick Driver Config
 * \ingroup hal_common_rte_config
 * @{
 */
#define SYS_TICK_INT_PRIORITY 0x0FU /**< tick interrupt priority */
#include "hal_systick.h"
/**
 * @}
 */

/**
 * \defgroup hal_common_rte_config_scu System Control Unit(SCU) Config
 * \ingroup hal_common_rte_config
 * @{
 */
#include "hal_scu.h"
/**
 * @}
 */

/**
 * \defgroup hal_common_rte_config_gpio GPIO Driver Config
 * \ingroup hal_common_rte_config
 * @{
 */
#include "hal_gpio.h"
/**
 * @}
 */

// <e> DMA (Direct Memory Access)
/**
 * \defgroup hal_common_rte_config_dma DMA Driver Config
 * \ingroup hal_common_rte_config
 * @{
 */
#define configUSE_DMA_PERIPHERAL 1 /**< 0: disable dma driver; 1: enable dma driver; default is 0 */
#if (configUSE_DMA_PERIPHERAL == 1)
#include "hal_dma.h"
#endif
/**
 * @}
 */
// </e> DMA (Direct Memory Access)

// <e> CAN (Controller Area Network)
/**
 * \defgroup hal_common_rte_config_can CAN Driver Config
 * \ingroup hal_common_rte_config
 * @{
 */
#define configUSE_CAN_PERIPHERAL 0 /**< 0: disable can driver; 1: enable can driver; default is 0 */
#if (configUSE_CAN_PERIPHERAL == 1)
// <e> Enable Callback
#define configUSE_CAN_REGISTER_CALLBACKS 0 /**< 0: disable CALLBACK API; 1: enable CALLBACK API; default is 0 */
// </e> Enable Callback
#include "hal_can.h"
#endif
/**
 * @}
 */
// </e> CAN (Controller Area Network)

// <e> CRC (Cyclic Redundancy Check)
/**
 * \defgroup hal_common_rte_config_crc CRC Driver Config
 * \ingroup hal_common_rte_config
 * @{
 */
#define configUSE_CRC_PERIPHERAL 0 /**< 0: disable crc driver; 1: enable crc driver; default is 0 */
#if (configUSE_CRC_PERIPHERAL == 1)
// <e> Enable Callback
#define configUSE_CRC_REGISTER_CALLBACKS 0 /**< 0: disable CALLBACK API; 1: enable CALLBACK API; default is 0 */
// </e> Enable Callback
#include "hal_crc.h"
#endif
/**
 * @}
 */
// </e> CRC (Cyclic Redundancy Check)

// <e> PWR (Power Management)
/**
 * \defgroup hal_common_rte_config_pwr PWR Driver Config
 * \ingroup hal_common_rte_config
 * @{
 */
#define configUSE_PWR_PERIPHERAL 0 /**< 0: disable pwr driver; 1: enable pwr driver; default is 0 */
#if (configUSE_PWR_PERIPHERAL == 1)
#include "hal_pwr.h"
#endif
/**
 * @}
 */
// </e> PWR (Power Management)

// <e> RCC (Reset and Clock Control)
/**
 * \defgroup hal_common_rte_config_rcc RCC Driver Config
 * \ingroup hal_common_rte_config
 * @{
 */
#define configUSE_RCC_PERIPHERAL 1 /**< 0: disable RCC driver; 1: enable RCC driver; default is 1 */
#if (configUSE_RCC_PERIPHERAL == 1)
#include "hal_rcc.h"
#endif
/**
 * @}
 */
// </e> RCC (Reset and Clock Control)

// <e> IWDG (WatchDog Timer)
/**
 * \defgroup hal_common_rte_config_iwdg IWDG Driver Config
 * \ingroup hal_common_rte_config
 * @{
 */
#define configUSE_IWDG_PERIPHERAL 0 /**< 0: disable iwdg driver; 1: enable iwdg driver; default is 0 */
#if (configUSE_IWDG_PERIPHERAL == 1)
// <e> Enable Callback
#define configUSE_IWDG_REGISTER_CALLBACKS 0 /**< 0: disable CALLBACK API; 1: enable CALLBACK API; default is 0 */
// </e> Enable Callback
#include "hal_iwdg.h"
#endif
/**
 * @}
 */
// </e> IWDG (Window WatchDog Timer)

// <e> WWDG (WatchDog Timer)
/**
 * \defgroup hal_common_rte_config_wwdg WWDG Driver Config
 * \ingroup hal_common_rte_config
 * @{
 */
#define configUSE_WWDG_PERIPHERAL 0 /**< 0: disable iwdg driver; 1: enable wwdg driver; default is 0 */
#if (configUSE_WWDG_PERIPHERAL == 1)
// <e> Enable Callback
#define configUSE_WWDG_REGISTER_CALLBACKS 0 /**< 0: disable CALLBACK API; 1: enable CALLBACK API; default is 0 */
// </e> Enable Callback
#include "hal_wwdg.h"
#endif
/**
 * @}
 */
// </e> WWDG (Window WatchDog Timer)

// <e> ADC (Analog to Digital Converter)
/**
 * \defgroup hal_common_rte_config_adc ADC Driver Config
 * \ingroup hal_common_rte_config
 * @{
 */
#define configUSE_ADC_PERIPHERAL 0 /**< 0: disable adc driver; 1: enable adc driver; default is 0 */
#if (configUSE_ADC_PERIPHERAL == 1)
// <e> Enable Callback
#define configUSE_ADC_REGISTER_CALLBACKS 0 /**< 0: disable CALLBACK API; 1: enable CALLBACK API; default is 0 */
// </e> Enable Callback
// <e> Enable IT API
#define configUSE_ADC_NONBLOCKING_IT 0
// </e> Enable IT API
// <e> Enable DMA API
#define configUSE_ADC_NONBLOCKING_DMA 0
// </e> Enable DMA API
#include "hal_adc.h"
#endif
/**
 * @}
 */
// </e> ADC (Analog to Digital Converter)

// <e> TPM (16-BIT Timer with Capture)
/**
 * \defgroup hal_common_rte_config_tpm TPM Driver Config
 * \ingroup hal_common_rte_config
 * @{
 */
#define configUSE_TPM_PERIPHERAL 0 /**< 0: disable tpm driver; 1: enable tpm driver; default is 0 */
#if (configUSE_TPM_PERIPHERAL == 1)
// <e> Enable Callback
#define configUSE_TPM_REGISTER_CALLBACKS 0 /**< 0: disable CALLBACK API; 1: enable CALLBACK API; default is 0 */
// </e> Enable Callback
// <e> Enable PMW Function
#define configUSE_TPM_PWM 0 /**< 0: disable PWM API; 1: enable PWM API; default is 0 */
// </e> Enable PMW Function
// <e> Enable Output Compare Function
#define configUSE_TPM_OC 0 /**< 0: disable EMC API; 1: enable OC API; default is 0 */
// </e> Enable Output Compare Function
// <e> Enable Capture Function
#define configUSE_TPM_CAP 0 /**< 0: disable CAP API; 1: enable CAP API; default is 0 */
// </e> Enable Capture Function
// <e> Enable DMA Function
#define configUSE_TPM_DMA 0 /**< 0: disable DMA API:; 1: enable DMA API; default is 0 */
// </e> Enable DMA Function
#include "hal_tpm.h"
#endif
/**
 * @}
 */
// </e> TPM (16-BIT Timer with Capture)

// <e> SPI (Serial Peripheral Interface)
/**
 * \defgroup hal_common_rte_config_spi SPI Driver Config
 * \ingroup hal_common_rte_config
 * @{
 */
#define configUSE_SPI_PERIPHERAL 1 /**< 0: disable spi driver; 1: enable spi driver; default is 0 */
#if (configUSE_SPI_PERIPHERAL == 1)
// <e> Enable Callback
#define configUSE_SPI_REGISTER_CALLBACKS 1 /**< 0: disable CALLBACK API; 1: enable CALLBACK API; default is 0 */
// </e> Enable Callback
// <e> Enable Blocking API
#define configUSE_SPI_BLOCKING 1 /**< 0: disable blocking API; 1: enable blocking API; default is 0 */
// </e> Enable Blocking API
// <e> Enable IT API
#define configUSE_SPI_NONBLOCKING_IT 1 /**< 0: disable IT API; 1: enable IT API; default is 0 */
// </e> Enable IT API
// <e> Enable DMA API
#define configUSE_SPI_NONBLOCKING_DMA 1 /**< 0: disable DMA API; 1: enable DMA API; default is 0 */
// </e> Enable DMA API
#include "hal_spi.h"
#endif
/**
 * @}
 */
// </e> SPI (Serial Peripheral Interface)

// <e> UART (Universal Asynchronous Receiver and Transmitter)
/**
 * \defgroup hal_common_rte_config_uart UART Driver Config
 * \ingroup hal_common_rte_config
 * @{
 */
#define configUSE_UART_PERIPHERAL 1 /**< 0: disable uart driver; 1: enable uart driver; default is 0 */
#if (configUSE_UART_PERIPHERAL == 1)
// <e> Enable Callback
#define configUSE_UART_REGISTER_CALLBACKS 0 /**< 0: disable CALLBACK API; 1: enable CALLBACK API; default is 0 */
// </e> Enable Callback
// <e> Enable Blocking API
#define configUSE_UART_BLOCKING 1 /**< 0: disable blocking API; 1: enable blocking API; default is 0 */
// </e> Enable Blocking API
// <e> Enable IT API
#define configUSE_UART_NONBLOCKING_IT 0 /**< 0: disable IT API; 1: enable IT API; default is 0 */
// </e> Enable IT API
// <e> Enable DMA API
#define configUSE_UART_NONBLOCKING_DMA 0 /**< 0: disable IT API; 1: enable IT API; default is 0 */
// </e> Enable DMA API
#include "hal_uart.h"
#endif
/**
 * @}
 */
// </e> UART (Universal Asynchronous Receiver and Transmitter)

// <e> FMC (Flash Memory Controller)
/**
 * \defgroup hal_common_rte_config_flash FMC Driver Config
 * \ingroup hal_common_rte_config
 * @{
 */
#define configUSE_FLASH_PERIPHERAL 1 /**< 0: Disable Flash driver; 1: Enable Flash driver; default is 1 */
#if (configUSE_FLASH_PERIPHERAL == 1)
// <e> Enable Callback
#define configUSE_FLASH_REGISTER_CALLBACKS 0 /**< 0: disable CALLBACK API; 1: enable CALLBACK API; default is 0 */
// </e> Enable Callback
// <e> Enable Blocking API
#define configUSE_FLASH_BLOCKING 0 /**< 0: disable blocking API; 1: enable blocking API; default is 0 */
// </e> Enable Blocking API
// <e> Enable IT API
#define configUSE_FLASH_NONBLOCKING_IT 0 /**< 0: disable IT API; 1: enable IT API; default is 0 */
// </e> Enable IT API
#include "hal_flash.h"
#endif
/**
 * @}
 */
// </e> FMC (Flash Memory Controller)

// <e> SD (Secure Digital)
/**
 * \defgroup hal_common_rte_config_sd SD Driver Config
 * \ingroup hal_common_rte_config
 * @{
 */
#define configUSE_SD_PERIPHERAL 0 /**< 0: disable sdio driver; 1: enable sdio driver; default is 0 */
#if (configUSE_SD_PERIPHERAL == 1)
// <e> Enable Callback
#define configUSE_SD_REGISTER_CALLBACKS 1 /**< 0: disable CALLBACK API; 1: enable CALLBACK API; default is 0 */
// </e> Enable Callback
// <e> Enable Blocking API
#define configUSE_SD_BLOCKING 0 /**< 0: disable blocking API; 1: enable blocking API; default is 0 */
// </e> Enable Blocking API
// <e> Enable IT API
#define configUSE_SD_NONBLOCKING_IT 0 /**< 0: disable IT API; 1: enable IT API; default is 0 */
// </e> Enable IT API
// <e> Enable DMA API
#define configUSE_SD_NONBLOCKING_DMA 0 /**< 0: disable DMA API; 1: enable DMA API; default is 0 */
// </e> Enable DMA API
#include "hal_sd.h"
#endif
/**
 * @}
 */
// </e>  SD (Secure Digital)

// <e> RTC (Real Time Clock)
/**
 * \defgroup hal_common_rte_config_rtc RTC Driver Config
 * \ingroup hal_common_rte_config
 * @{
 */
#define configUSE_RTC_PERIPHERAL 0 /**< 0: disable rtc driver; 1: enable rtc driver; default is 0 */
#if (configUSE_RTC_PERIPHERAL == 1)
// <e> Enable Callback
#define configUSE_RTC_REGISTER_CALLBACKS 0 /**< 0: disable CALLBACK API; 1: enable CALLBACK API; default is 0 */
// </e> Enable Callback
#include "hal_rtc.h"
#endif
/**
 * @}
 */
// </e> RTC (Real Time Clock)

// <e> I2C (Inter-Integrated Circuit)
/**
 * \defgroup hal_common_rte_config_i2c I2C Driver Config
 * \ingroup hal_common_rte_config
 * @{
 */
#define configUSE_I2C_PERIPHERAL 0 /**< 0: disable i2c driver; 1: enable i2c driver; default is 0 */
#if (configUSE_I2C_PERIPHERAL == 1)
// <e> Enable Callback
#define configUSE_I2C_REGISTER_CALLBACKS 0 /**< 0: disable CALLBACK API; 1: enable CALLBACK API; default is 0 */
// </e> Enable Callback
// <e> Enable Blocking API
#define configUSE_I2C_BLOCKING 0 /**< 0: disable blocking API; 1: enable blocking API; default is 0 */
// </e> Enable Blocking API
// <e> Enable IT API
#define configUSE_I2C_NONBLOCKING_IT 0 /**< 0: disable IT API; 1: enable IT API; default is 0 */
// </e> Enable IT API
// <e> Enable BURST API
#define configUSE_I2C_NONBLOCKING_BURST 0 /**< 0: disable BURST API; 1: enable BURST API; default is 0 */
// </e> Enable BURST API
#include "hal_i2c.h"
#endif
/**
 * @}
 */
// </e> I2C (Inter-Integrated Circuit)

// <e> I2S (Inter IC Sound)
/**
 * \defgroup hal_common_rte_config_i2s I2S Driver Config
 * \ingroup hal_common_rte_config
 * @{
 */
#define configUSE_I2S_PERIPHERAL 0 /**< 0: disable i2s driver; 1: enable i2s driver; default is 0 */
#if (configUSE_I2S_PERIPHERAL == 1)
// <e> Enable Callback
#define configUSE_I2S_REGISTER_CALLBACKS 0 /**< 0: disable CALLBACK API; 1: enable CALLBACK API; default is 0 */
// </e> Enable Callback
// <e> Enable Blocking API
#define configUSE_I2S_BLOCKING 0 /**< 0: disable blocking API; 1: enable blocking API; default is 0 */
// </e> Enable Blocking API
// <e> Enable IT API
#define configUSE_I2S_NONBLOCKING_IT 0 /**< 0: disable IT API; 1: enable IT API; default is 0 */
// </e> Enable IT API
// <e> Enable DMA API
#define configUSE_I2S_NONBLOCKING_DMA 0 /**< 0: disable DMA API; 1: enable DMA API; default is 0 */
// </e> Enable DMA API
#include "hal_i2s.h"
#endif
/**
 * @}
 */
// </e> I2S (Inter IC Sound)

// <e> LCM
/**
 * \defgroup hal_common_rte_config_lcm LCM Driver Config
 * \ingroup hal_common_rte_config
 * @{
 */
#define configUSE_LCM_PERIPHERAL 0 /**< 0: disable lcm driver; 1: enable lcm driver; default is 0 */
#if (configUSE_LCM_PERIPHERAL == 1)
// <e> Enable Callback
#define configUSE_LCM_REGISTER_CALLBACKS 0 /**< 0: disable CALLBACK API; 1: enable CALLBACK API; default is 0 */
// </e> Enable Callback
// <e> Enable Blocking API
#define configUSE_LCM_BLOCKING 0 /**< 0: disable blocking API; 1: enable blocking API; default is 0 */
// </e> Enable Blocking API
// <e> Enable DMA API
#define configUSE_LCM_NONBLOCKING_DMA 0 /**< 0: disable DMA API; 1: enable DMA API; default is 0 */
// </e> Enable DMA API
#include "hal_lcm.h"
#endif
/**
 * @}
 */
// </e> LCM

// <e> ETH (Ethernet)
/**
 * \defgroup hal_common_rte_config_eth ETH Driver Config
 * \ingroup hal_common_rte_config
 * @{
 */
#define configUSE_ETH_PERIPHERAL 0 /**< 0: disable RCC driver; 1: enable RCC driver; default is 1 */
#if (configUSE_ETH_PERIPHERAL == 1)
// <e> Enable Callback
#define configUSE_ETH_REGISTER_CALLBACKS 0 /**< 0: disable CALLBACK API; 1: enable CALLBACK API; default is 0 */
// </e> Enable Callback
#include "hal_eth.h"
#endif
/**
 * @}
 */
// </e> ETH (Ethernet)

// <e> Debugger
/**
 * \defgroup hal_common_rte_config_debugger DEBUGGER RTE Config
 * \brief define the middleware debugger config switch symbol
 * \ingroup hal_common_rte_config
 * @{
 */
#define configUSE_DEBUGGER_MIDDLEWARE 0 /**< 0: disable debugger; 1: enable debugger; default is 0 */
#if (configUSE_DEBUGGER_MIDDLEWARE == 1)
//<o> Debug Option <0=>Uart Print Mode <1=>Uart ATCMD Mode <2=>SWO Print Mode
#define configUSE_DEBUGGER_OPTION 0 /**< 0: Uart TX under NONOS;  1: Uart ATCMD Mode; 2: under SWO mode; */
//<o> Debug Level <0=>Emergency  <1=> Alert <2=> Critical <3=>Error <4=>Warnings <5=>Notice <6=>Informational <7=>Debugging
#define configUSE_DEBUGGER_LEVEL 0 /**< debug level (0-7) */
//<i> Emergency:: used for emergency messages, usually those that precede a crash.
//<i> Alert:: a situation requiring immediate action
//<i> Critical :: critical conditions, often related to serious hardware or software failures
//<i> Error  :: used to report error conditions; device drivers often use KERN_ERR to report hardware difficulties
//<i> Warnings :: warnings about problematic situations that do not, in themselves, create serious problems with the system
//<i> Notice :: situations that are normal, but still worthy of note. A number of security-related conditions are reported at this level
//<i> Informational :: informational messages. Many drivers print information about the hardware they find at startup time at this level
//<i> Debugging:: used for debugging messages

#include "Middleware_debugger.h"
#endif
/**
 * @}
 */
// </e> Debugger

// <e> Assert Function
/**
 * \defgroup hal_common_rte_config_assert Assert Config
 * \ingroup hal_common_rte_config
 */
#define configUSE_ASSERT 0 /**< 0: disable assert function; 1: enable assert function; default is 0 */
// #if (configUSE_ASSERT == 1 && configUSE_DEBUGGER_MIDDLEWARE == 1)
#if (configUSE_ASSERT == 1)
#include "hal_assert.h"
/**
 * @brief  The ASSERT_PARAM macro is used for function's parameters check under project debugging mode
 *         After all functions of current project are verified, the configUSE_ASSERT could be reset to 0 for releasing mode
 * @param  expr If expr is false, it calls SNX_Assert_False function which reports the name of the source
 *         file and the source line number of the call that failed.
 *         If expr is true, it returns no value.
 * @note   the SNX_Assert_False function is a weak function, and can be replaced by user self.
 */
#define ASSERT_PARAM(expr) ((expr) ? (void)0U : AssertParaFalse((uint8_t *)__FILE__, __LINE__))
#else
/**
 * @brief unused The ASSERT_PARAM macro
 */
#define ASSERT_PARAM(expr) ((void)0U) /**< Check parameters of function for debugging */
#endif
// </e> Assert Function

#ifdef __cplusplus
}
#endif

#endif //_SN34F7_HAL_CONFIG_H_

//-------- <<< end of configuration section >>> --------------------
