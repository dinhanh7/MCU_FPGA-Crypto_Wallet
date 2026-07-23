/***********************
 * SN34F78X SDK V2.1.4 *
 ***********************/

#ifndef _HAL_RCC_H_
#define _HAL_RCC_H_

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "SN34F780.h"
#include "SN34F700_Def.h"

/** \defgroup rcc_ahb_clk_ctrl AHB Peripheral Clock Enable Disable
 * \brief  Enable or disable the AHB Peripheral clock.
 * \ingroup rcc_api_define
 * \note
 * \{
 */
#define __HAL_RCC_DMA0_CLK_ENABLE()          (SN_SCU->AHBCLKG_b.DMA0CLKEN = ENABLE)  /*!< Enable clock for DAM0 */
#define __HAL_RCC_DMA1_CLK_ENABLE()          (SN_SCU->AHBCLKG_b.DMA1CLKEN = ENABLE)  /*!< Enable clock for DAM1 */
#define __HAL_RCC_SDIO_CLK_ENABLE()          (SN_SCU->AHBCLKG_b.SDIOCLKEN = ENABLE)  /*!< Enable clock for SDIO */
#define __HAL_RCC_LCD_CLK_ENABLE()           (SN_SCU->AHBCLKG_b.LCDCLKEN = ENABLE)   /*!< Enable clock for LCD */
#define __HAL_RCC_ETHERNET_CLK_ENABLE()      (SN_SCU->AHBCLKG_b.ETHCLKEN = ENABLE)   /*!< Enable clock for Ethernet MAC */
#define __HAL_RCC_USB_HS_CLK_ENABLE()        (SN_SCU->AHBCLKG_b.USBHSCLKEN = ENABLE) /*!< Enable clock for USB HS controller */
#define __HAL_RCC_BACKUP_SRAM_CLK_ENABLE()   (SN_SCU->AHBCLKG_b.BKPSRAMEN = ENABLE)  /*!< Enable clock for Badkup SRAM */
#define __HAL_RCC_ETHERNET_SRAM_CLK_ENABLE() (SN_SCU->AHBCLKG_b.ETHSRAMEN = ENABLE)  /*!< Enable clock for Ethernet MAC SRAM */

#define __HAL_RCC_DMA0_CLK_DISABLE()          (SN_SCU->AHBCLKG_b.DMA0CLKEN = DISABLE)  /*!< Disable clock for DAM0 */
#define __HAL_RCC_DMA1_CLK_DISABLE()          (SN_SCU->AHBCLKG_b.DMA1CLKEN = DISABLE)  /*!< Disable clock for DAM1 */
#define __HAL_RCC_SDIO_CLK_DISABLE()          (SN_SCU->AHBCLKG_b.SDIOCLKEN = DISABLE)  /*!< Disable clock for SDIO */
#define __HAL_RCC_LCD_CLK_DISABLE()           (SN_SCU->AHBCLKG_b.LCDCLKEN = DISABLE)   /*!< Disable clock for LCD */
#define __HAL_RCC_ETHERNET_CLK_DISABLE()      (SN_SCU->AHBCLKG_b.ETHCLKEN = DISABLE)   /*!< Disable clock for Ethernet MAC */
#define __HAL_RCC_USB_HS_CLK_DISABLE()        (SN_SCU->AHBCLKG_b.USBHSCLKEN = DISABLE) /*!< Disable clock for USB HS controller */
#define __HAL_RCC_BACKUP_SRAM_CLK_DISABLE()   (SN_SCU->AHBCLKG_b.BKPSRAMEN = DISABLE)  /*!< Disable clock for Badkup SRAM */
#define __HAL_RCC_ETHERNET_SRAM_CLK_DISABLE() (SN_SCU->AHBCLKG_b.ETHSRAMEN = DISABLE)  /*!< Disable clock for Ethernet MAC SRAM */
/**
 * @}
 */

/** \defgroup rcc_ahb_clk_chk AHB Peripheral Clock checking
 * \brief  Checking the AHB Peripheral clock.
 * \ingroup rcc_api_define
 * \note
 * \{
 */
#define __HAL_RCC_DMA0_IS_CLK_ENABLE()          (SN_SCU->AHBCLKG_b.DMA0CLKEN == ENABLE)  /*!< Check DAM0 clock enable */
#define __HAL_RCC_DMA1_IS_CLK_ENABLE()          (SN_SCU->AHBCLKG_b.DMA1CLKEN == ENABLE)  /*!< Check DAM1 clock enable */
#define __HAL_RCC_SDIO_IS_CLK_ENABLE()          (SN_SCU->AHBCLKG_b.SDIOCLKEN == ENABLE)  /*!< Check SDIO clock enable */
#define __HAL_RCC_LCD_IS_CLK_ENABLE()           (SN_SCU->AHBCLKG_b.LCDCLKEN == ENABLE)   /*!< Check LCD clock enable */
#define __HAL_RCC_ETHERNET_IS_CLK_ENABLE()      (SN_SCU->AHBCLKG_b.ETHCLKEN == ENABLE)   /*!< Check Ethernet MAC clock enable */
#define __HAL_RCC_USB_HS_IS_CLK_ENABLE()        (SN_SCU->AHBCLKG_b.USBHSCLKEN == ENABLE) /*!< Check USB HS controller clock enable */
#define __HAL_RCC_BACKUP_SRAM_IS_CLK_ENABLE()   (SN_SCU->AHBCLKG_b.BKPSRAMEN == ENABLE)  /*!< Check Badkup SRAM clock enable */
#define __HAL_RCC_ETHERNET_SRAM_IS_CLK_ENABLE() (SN_SCU->AHBCLKG_b.ETHSRAMEN == ENABLE)  /*!< Check Ethernet MAC SRAM clock enable */

#define __HAL_RCC_DMA0_IS_CLK_DISABLE()          (SN_SCU->AHBCLKG_b.DMA0CLKEN == DISABLE)  /*!< Check DAM0 clock disable */
#define __HAL_RCC_DMA1_IS_CLK_DISABLE()          (SN_SCU->AHBCLKG_b.DMA1CLKEN == DISABLE)  /*!< Check DAM1 clock disable */
#define __HAL_RCC_SDIO_IS_CLK_DISABLE()          (SN_SCU->AHBCLKG_b.SDIOCLKEN == DISABLE)  /*!< Check SDIO clock disable */
#define __HAL_RCC_LCD_IS_CLK_DISABLE()           (SN_SCU->AHBCLKG_b.LCDCLKEN == DISABLE)   /*!< Check LCD clock disable */
#define __HAL_RCC_ETHERNET_IS_CLK_DISABLE()      (SN_SCU->AHBCLKG_b.ETHCLKEN == DISABLE)   /*!< Check Ethernet MAC clock disable */
#define __HAL_RCC_USB_HS_IS_CLK_DISABLE()        (SN_SCU->AHBCLKG_b.USBHSCLKEN == DISABLE) /*!< Check USB HS controller clock disable */
#define __HAL_RCC_BACKUP_SRAM_IS_CLK_DISABLE()   (SN_SCU->AHBCLKG_b.BKPSRAMEN == DISABLE)  /*!< Check Badkup SRAM clock disable */
#define __HAL_RCC_ETHERNET_SRAM_IS_CLK_DISABLE() (SN_SCU->AHBCLKG_b.ETHSRAMEN == DISABLE)  /*!< Check Ethernet MAC SRAM clock disable */
/**
 * @}
 */

/** \defgroup rcc_apb0_clk_ctrl APB0 Peripheral Clock Enable Disable
 * \brief  Enable or disable the APB0 Peripheral clock.
 * \ingroup rcc_api_define
 * \note
 * \{
 */
#define __HAL_RCC_WDT_CLK_ENABLE()       (SN_SCU->APB0CLKG_b.WDTCLKEN = ENABLE)       /*!< Enable clock for WDT */
#define __HAL_RCC_WWDT_CLK_ENABLE()      (SN_SCU->APB0CLKG_b.WWDTCLKEN = ENABLE)      /*!< Enable clock for WWDT */
#define __HAL_RCC_CAN0_CLK_ENABLE()      (SN_SCU->APB0CLKG_b.CAN0CLKEN = ENABLE)      /*!< Enable clock for CAN0 */
#define __HAL_RCC_CAN1_CLK_ENABLE()      (SN_SCU->APB0CLKG_b.CAN1CLKEN = ENABLE)      /*!< Enable clock for CAN1 */
#define __HAL_RCC_UART1_CLK_ENABLE()     (SN_SCU->APB0CLKG_b.UART1CLKEN = ENABLE)     /*!< Enable clock for UART1 */
#define __HAL_RCC_UART2_CLK_ENABLE()     (SN_SCU->APB0CLKG_b.UART2CLKEN = ENABLE)     /*!< Enable clock for UART2 */
#define __HAL_RCC_UART3_CLK_ENABLE()     (SN_SCU->APB0CLKG_b.UART3CLKEN = ENABLE)     /*!< Enable clock for UART3 */
#define __HAL_RCC_UART4_CLK_ENABLE()     (SN_SCU->APB0CLKG_b.UART4CLKEN = ENABLE)     /*!< Enable clock for UART4 */
#define __HAL_RCC_CT16B6_CLK_ENABLE()    (SN_SCU->APB0CLKG_b.CT16B6CLKEN = ENABLE)    /*!< Enable clock for CT16B6 */
#define __HAL_RCC_CT16B7_CLK_ENABLE()    (SN_SCU->APB0CLKG_b.CT16B7CLKEN = ENABLE)    /*!< Enable clock for CT16B7 */
#define __HAL_RCC_ALWAYSON_CLK_ENABLE()  (SN_SCU->APB0CLKG_b.ALWAYSONREGEN = ENABLE)  /*!< Enable clock for Always-on domain */
#define __HAL_RCC_USBPHY_CLK_ENABLE()    (SN_SCU->APB0CLKG_b.USBPHYCLKEN = ENABLE)    /*!< Enable clock for USB HS PHY PCLK */
#define __HAL_RCC_USBPHY12M_CLK_ENABLE() (SN_SCU->APB0CLKG_b.USBPHY12MCLKEN = ENABLE) /*!< Enable clock for USB HS PHY 12M clock */
#define __HAL_RCC_CAN0_SRAM_CLK_ENABLE() (SN_SCU->APB0CLKG_b.CAN0SRAMCLKEN = ENABLE)  /*!< Enable clock for WDT CAN0 SRAM */
#define __HAL_RCC_CAN1_SRAM_CLK_ENABLE() (SN_SCU->APB0CLKG_b.CAN1SRAMCLKEN = ENABLE)  /*!< Enable clock for WDT CAN1 SRAM */
#define __HAL_RCC_CLKOUT_CLK_ENABLE()    (SN_SCU->APB0CLKG_b.CLKOUTEN = ENABLE)       /*!< Enable clock for WDT CLKOUT */

#define __HAL_RCC_WDT_CLK_DISABLE()       (SN_SCU->APB0CLKG_b.WDTCLKEN = DISABLE)       /*!< Disable clock for WDT */
#define __HAL_RCC_WWDT_CLK_DISABLE()      (SN_SCU->APB0CLKG_b.WWDTCLKEN = DISABLE)      /*!< Disable clock for WWDT */
#define __HAL_RCC_CAN0_CLK_DISABLE()      (SN_SCU->APB0CLKG_b.CAN0CLKEN = DISABLE)      /*!< Disable clock for CAN0 */
#define __HAL_RCC_CAN1_CLK_DISABLE()      (SN_SCU->APB0CLKG_b.CAN1CLKEN = DISABLE)      /*!< Disable clock for CAN1 */
#define __HAL_RCC_UART1_CLK_DISABLE()     (SN_SCU->APB0CLKG_b.UART1CLKEN = DISABLE)     /*!< Disable clock for UART1 */
#define __HAL_RCC_UART2_CLK_DISABLE()     (SN_SCU->APB0CLKG_b.UART2CLKEN = DISABLE)     /*!< Disable clock for UART2 */
#define __HAL_RCC_UART3_CLK_DISABLE()     (SN_SCU->APB0CLKG_b.UART3CLKEN = DISABLE)     /*!< Disable clock for UART3 */
#define __HAL_RCC_UART4_CLK_DISABLE()     (SN_SCU->APB0CLKG_b.UART4CLKEN = DISABLE)     /*!< Disable clock for UART4 */
#define __HAL_RCC_CT16B6_CLK_DISABLE()    (SN_SCU->APB0CLKG_b.CT16B6CLKEN = DISABLE)    /*!< Disable clock for CT16B6 */
#define __HAL_RCC_CT16B7_CLK_DISABLE()    (SN_SCU->APB0CLKG_b.CT16B7CLKEN = DISABLE)    /*!< Disable clock for CT16B7 */
#define __HAL_RCC_ALWAYSON_CLK_DISABLE()  (SN_SCU->APB0CLKG_b.ALWAYSONREGEN = DISABLE)  /*!< Disable clock for Always-on domain */
#define __HAL_RCC_USBPHY_CLK_DISABLE()    (SN_SCU->APB0CLKG_b.USBPHYCLKEN = DISABLE)    /*!< Disable clock for USB HS PHY PCLK */
#define __HAL_RCC_USBPHY12M_CLK_DISABLE() (SN_SCU->APB0CLKG_b.USBPHY12MCLKEN = DISABLE) /*!< Disable clock for USB HS PHY 12M clock */
#define __HAL_RCC_CAN0_SRAM_CLK_DISABLE() (SN_SCU->APB0CLKG_b.CAN0SRAMCLKEN = DISABLE)  /*!< Disable clock for WDT CAN0 SRAM */
#define __HAL_RCC_CAN1_SRAM_CLK_DISABLE() (SN_SCU->APB0CLKG_b.CAN1SRAMCLKEN = DISABLE)  /*!< Disable clock for WDT CAN1 SRAM */
#define __HAL_RCC_CLKOUT_CLK_DISABLE()    (SN_SCU->APB0CLKG_b.CLKOUTEN = DISABLE)       /*!< Disable clock for WDT CLKOUT */
/**
 * @}
 */

/** \defgroup rcc_apb0_clk_chk APB0 Peripheral Clock checking
 * \brief  Checking the APB0 Peripheral clock.
 * \ingroup rcc_api_define
 * \note
 * \{
 */
#define __HAL_RCC_WDT_IS_CLK_ENABLE()       (SN_SCU->APB0CLKG_b.WDTCLKEN == ENABLE)       /*!< Check WDT clock enable */
#define __HAL_RCC_WWDT_IS_CLK_ENABLE()      (SN_SCU->APB0CLKG_b.WWDTCLKEN == ENABLE)      /*!< Check WWDT clock enable */
#define __HAL_RCC_CAN0_IS_CLK_ENABLE()      (SN_SCU->APB0CLKG_b.CAN0CLKEN == ENABLE)      /*!< Check CAN0 clock enable */
#define __HAL_RCC_CAN1_IS_CLK_ENABLE()      (SN_SCU->APB0CLKG_b.CAN1CLKEN == ENABLE)      /*!< Check CAN1 clock enable */
#define __HAL_RCC_UART1_IS_CLK_ENABLE()     (SN_SCU->APB0CLKG_b.UART1CLKEN == ENABLE)     /*!< Check UART1 clock enable */
#define __HAL_RCC_UART2_IS_CLK_ENABLE()     (SN_SCU->APB0CLKG_b.UART2CLKEN == ENABLE)     /*!< Check UART2 clock enable */
#define __HAL_RCC_UART3_IS_CLK_ENABLE()     (SN_SCU->APB0CLKG_b.UART3CLKEN == ENABLE)     /*!< Check UART3 clock enable */
#define __HAL_RCC_UART4_IS_CLK_ENABLE()     (SN_SCU->APB0CLKG_b.UART4CLKEN == ENABLE)     /*!< Check UART4 clock enable */
#define __HAL_RCC_CT16B6_IS_CLK_ENABLE()    (SN_SCU->APB0CLKG_b.CT16B6CLKEN == ENABLE)    /*!< Check CT16B6 clock enable */
#define __HAL_RCC_CT16B7_IS_CLK_ENABLE()    (SN_SCU->APB0CLKG_b.CT16B7CLKEN == ENABLE)    /*!< Check CT16B7 clock enable */
#define __HAL_RCC_ALWAYSON_IS_CLK_ENABLE()  (SN_SCU->APB0CLKG_b.ALWAYSONREGEN == ENABLE)  /*!< Check Always-on domain registers clock enable */
#define __HAL_RCC_USBPHY_IS_CLK_ENABLE()    (SN_SCU->APB0CLKG_b.USBPHYCLKEN == ENABLE)    /*!< Check USB HS PHY PCLK clock enable */
#define __HAL_RCC_USBPHY12M_IS_CLK_ENABLE() (SN_SCU->APB0CLKG_b.USBPHY12MCLKEN == ENABLE) /*!< Check USB HS PHY 12M clock enable */
#define __HAL_RCC_CAN0_SRAM_IS_CLK_ENABLE() (SN_SCU->APB0CLKG_b.CAN0SRAMCLKEN == ENABLE)  /*!< Check WDT CAN0 SRAM clock enable */
#define __HAL_RCC_CAN1_SRAM_IS_CLK_ENABLE() (SN_SCU->APB0CLKG_b.CAN1SRAMCLKEN == ENABLE)  /*!< Check WDT CAN1 SRAM clock enable */
#define __HAL_RCC_CLKOUT_IS_CLK_ENABLE()    (SN_SCU->APB0CLKG_b.CLKOUTEN == ENABLE)       /*!< Check WDT CLKOUT clock enable */

#define __HAL_RCC_WDT_IS_CLK_DISABLE()       (SN_SCU->APB0CLKG_b.WDTCLKEN == DISABLE)       /*!< Check WDT clock disable */
#define __HAL_RCC_WWDT_IS_CLK_DISABLE()      (SN_SCU->APB0CLKG_b.WWDTCLKEN == DISABLE)      /*!< Check WWDT clock disable */
#define __HAL_RCC_CAN0_IS_CLK_DISABLE()      (SN_SCU->APB0CLKG_b.CAN0CLKEN == DISABLE)      /*!< Check CAN0 clock disable */
#define __HAL_RCC_CAN1_IS_CLK_DISABLE()      (SN_SCU->APB0CLKG_b.CAN1CLKEN == DISABLE)      /*!< Check CAN1 clock disable */
#define __HAL_RCC_UART1_IS_CLK_DISABLE()     (SN_SCU->APB0CLKG_b.UART1CLKEN == DISABLE)     /*!< Check UART1 clock disable */
#define __HAL_RCC_UART2_IS_CLK_DISABLE()     (SN_SCU->APB0CLKG_b.UART2CLKEN == DISABLE)     /*!< Check UART2 clock disable */
#define __HAL_RCC_UART3_IS_CLK_DISABLE()     (SN_SCU->APB0CLKG_b.UART3CLKEN == DISABLE)     /*!< Check UART3 clock disable */
#define __HAL_RCC_UART4_IS_CLK_DISABLE()     (SN_SCU->APB0CLKG_b.UART4CLKEN == DISABLE)     /*!< Check UART4 clock disable */
#define __HAL_RCC_CT16B6_IS_CLK_DISABLE()    (SN_SCU->APB0CLKG_b.CT16B6CLKEN == DISABLE)    /*!< Check CT16B6 clock disable */
#define __HAL_RCC_CT16B7_IS_CLK_DISABLE()    (SN_SCU->APB0CLKG_b.CT16B7CLKEN == DISABLE)    /*!< Check CT16B7 clock disable */
#define __HAL_RCC_ALWAYSON_IS_CLK_DISABLE()  (SN_SCU->APB0CLKG_b.ALWAYSONREGEN == DISABLE)  /*!< Check Always-on domain clock disable */
#define __HAL_RCC_USBPHY_IS_CLK_DISABLE()    (SN_SCU->APB0CLKG_b.USBPHYCLKEN == DISABLE)    /*!< Check USB HS PHY PCLK clock disable */
#define __HAL_RCC_USBPHY12M_IS_CLK_DISABLE() (SN_SCU->APB0CLKG_b.USBPHY12MCLKEN == DISABLE) /*!< Check USB HS PHY 12M clock disable */
#define __HAL_RCC_CAN0_SRAM_IS_CLK_DISABLE() (SN_SCU->APB0CLKG_b.CAN0SRAMCLKEN == DISABLE)  /*!< Check WDT CAN0 SRAM clock disable */
#define __HAL_RCC_CAN1_SRAM_IS_CLK_DISABLE() (SN_SCU->APB0CLKG_b.CAN1SRAMCLKEN == DISABLE)  /*!< Check WDT CAN1 SRAM clock disable */
#define __HAL_RCC_CLKOUT_IS_CLK_DISABLE()    (SN_SCU->APB0CLKG_b.CLKOUTEN == DISABLE)       /*!< Check WDT CLKOUT clock disable */
/**
 * @}
 */

/** \defgroup rcc_apb1_clk_ctrl APB1 Peripheral Clock Enable Disable
 * \brief  Enable or disable the APB1 Peripheral clock.
 * \ingroup rcc_api_define
 * \note
 * \{
 */
#define __HAL_RCC_ADC0_CLK_ENABLE()    (SN_SCU->APB1CLKG_b.ADC0CLKEN = ENABLE)   /*!< Enable clock for ADC0 */
#define __HAL_RCC_SSP0_CLK_ENABLE()    (SN_SCU->APB1CLKG_b.SSP0CLKEN = ENABLE)   /*!< Enable clock for SSP0 */
#define __HAL_RCC_SSP1_CLK_ENABLE()    (SN_SCU->APB1CLKG_b.SSP1CLKEN = ENABLE)   /*!< Enable clock for SSP1 */
#define __HAL_RCC_SSP2_CLK_ENABLE()    (SN_SCU->APB1CLKG_b.SSP2CLKEN = ENABLE)   /*!< Enable clock for SSP2 */
#define __HAL_RCC_UART0_CLK_ENABLE()   (SN_SCU->APB1CLKG_b.UART0CLKEN = ENABLE)  /*!< Enable clock for UART0 */
#define __HAL_RCC_UART5_CLK_ENABLE()   (SN_SCU->APB1CLKG_b.UART5CLKEN = ENABLE)  /*!< Enable clock for UART5 */
#define __HAL_RCC_I2C0_CLK_ENABLE()    (SN_SCU->APB1CLKG_b.I2C0CLKEN = ENABLE)   /*!< Enable clock for I2C0 */
#define __HAL_RCC_I2C1_CLK_ENABLE()    (SN_SCU->APB1CLKG_b.I2C1CLKEN = ENABLE)   /*!< Enable clock for I2C1 */
#define __HAL_RCC_I2C2_CLK_ENABLE()    (SN_SCU->APB1CLKG_b.I2C2CLKEN = ENABLE)   /*!< Enable clock for I2C2 */
#define __HAL_RCC_CT16B0_CLK_ENABLE()  (SN_SCU->APB1CLKG_b.CT16B0CLKEN = ENABLE) /*!< Enable clock for CT16B0 */
#define __HAL_RCC_CT16B1_CLK_ENABLE()  (SN_SCU->APB1CLKG_b.CT16B1CLKEN = ENABLE) /*!< Enable clock for CT16B1 */
#define __HAL_RCC_CT16B2_CLK_ENABLE()  (SN_SCU->APB1CLKG_b.CT16B2CLKEN = ENABLE) /*!< Enable clock for CT16B2 */
#define __HAL_RCC_CT16B3_CLK_ENABLE()  (SN_SCU->APB1CLKG_b.CT16B3CLKEN = ENABLE) /*!< Enable clock for CT16B3 */
#define __HAL_RCC_CT16B4_CLK_ENABLE()  (SN_SCU->APB1CLKG_b.CT16B4CLKEN = ENABLE) /*!< Enable clock for CT16B4 */
#define __HAL_RCC_CT16B5_CLK_ENABLE()  (SN_SCU->APB1CLKG_b.CT16B5CLKEN = ENABLE) /*!< Enable clock for CT16B5 */
#define __HAL_RCC_CT16B8_CLK_ENABLE()  (SN_SCU->APB1CLKG_b.CT16B8CLKEN = ENABLE) /*!< Enable clock for CT16B8 */
#define __HAL_RCC_CRC_CLK_ENABLE()     (SN_SCU->APB1CLKG_b.CRCCLKEN = ENABLE)    /*!< Enable clock for CRC */
#define __HAL_RCC_GPIO_P0_CLK_ENABLE() (SN_SCU->APB1CLKG_b.P0CLKEN = ENABLE)     /*!< Enable clock for GPIO0 */
#define __HAL_RCC_GPIO_P1_CLK_ENABLE() (SN_SCU->APB1CLKG_b.P1CLKEN = ENABLE)     /*!< Enable clock for GPIO1 */
#define __HAL_RCC_GPIO_P2_CLK_ENABLE() (SN_SCU->APB1CLKG_b.P2CLKEN = ENABLE)     /*!< Enable clock for GPIO2 */
#define __HAL_RCC_GPIO_P3_CLK_ENABLE() (SN_SCU->APB1CLKG_b.P3CLKEN = ENABLE)     /*!< Enable clock for GPIO3 */
#define __HAL_RCC_LCM_CLK_ENABLE()     (SN_SCU->APB1CLKG_b.LCMCLKEN = ENABLE)    /*!< Enable clock for LCK */

#define __HAL_RCC_ADC0_CLK_DISABLE()    (SN_SCU->APB1CLKG_b.ADC0CLKEN = DISABLE)   /*!< Disable clock for ADC0 */
#define __HAL_RCC_SSP0_CLK_DISABLE()    (SN_SCU->APB1CLKG_b.SSP0CLKEN = DISABLE)   /*!< Disable clock for SSP0 */
#define __HAL_RCC_SSP1_CLK_DISABLE()    (SN_SCU->APB1CLKG_b.SSP1CLKEN = DISABLE)   /*!< Disable clock for SSP1 */
#define __HAL_RCC_SSP2_CLK_DISABLE()    (SN_SCU->APB1CLKG_b.SSP2CLKEN = DISABLE)   /*!< Disable clock for SSP2 */
#define __HAL_RCC_UART0_CLK_DISABLE()   (SN_SCU->APB1CLKG_b.UART0CLKEN = DISABLE)  /*!< Disable clock for UART0 */
#define __HAL_RCC_UART5_CLK_DISABLE()   (SN_SCU->APB1CLKG_b.UART5CLKEN = DISABLE)  /*!< Disable clock for UART5 */
#define __HAL_RCC_I2C0_CLK_DISABLE()    (SN_SCU->APB1CLKG_b.I2C0CLKEN = DISABLE)   /*!< Disable clock for I2C0 */
#define __HAL_RCC_I2C1_CLK_DISABLE()    (SN_SCU->APB1CLKG_b.I2C1CLKEN = DISABLE)   /*!< Disable clock for I2C1 */
#define __HAL_RCC_I2C2_CLK_DISABLE()    (SN_SCU->APB1CLKG_b.I2C2CLKEN = DISABLE)   /*!< Disable clock for I2C2 */
#define __HAL_RCC_CT16B0_CLK_DISABLE()  (SN_SCU->APB1CLKG_b.CT16B0CLKEN = DISABLE) /*!< Disable clock for CT16B0 */
#define __HAL_RCC_CT16B1_CLK_DISABLE()  (SN_SCU->APB1CLKG_b.CT16B1CLKEN = DISABLE) /*!< Disable clock for CT16B1 */
#define __HAL_RCC_CT16B2_CLK_DISABLE()  (SN_SCU->APB1CLKG_b.CT16B2CLKEN = DISABLE) /*!< Disable clock for CT16B2 */
#define __HAL_RCC_CT16B3_CLK_DISABLE()  (SN_SCU->APB1CLKG_b.CT16B3CLKEN = DISABLE) /*!< Disable clock for CT16B3 */
#define __HAL_RCC_CT16B4_CLK_DISABLE()  (SN_SCU->APB1CLKG_b.CT16B4CLKEN = DISABLE) /*!< Disable clock for CT16B4 */
#define __HAL_RCC_CT16B5_CLK_DISABLE()  (SN_SCU->APB1CLKG_b.CT16B5CLKEN = DISABLE) /*!< Disable clock for CT16B5 */
#define __HAL_RCC_CT16B8_CLK_DISABLE()  (SN_SCU->APB1CLKG_b.CT16B8CLKEN = DISABLE) /*!< Disable clock for CT16B8 */
#define __HAL_RCC_CRC_CLK_DISABLE()     (SN_SCU->APB1CLKG_b.CRCCLKEN = DISABLE)    /*!< Disable clock for CRC */
#define __HAL_RCC_GPIO_P0_CLK_DISABLE() (SN_SCU->APB1CLKG_b.P0CLKEN = DISABLE)     /*!< Disable clock for GPIO0 */
#define __HAL_RCC_GPIO_P1_CLK_DISABLE() (SN_SCU->APB1CLKG_b.P1CLKEN = DISABLE)     /*!< Disable clock for GPIO1 */
#define __HAL_RCC_GPIO_P2_CLK_DISABLE() (SN_SCU->APB1CLKG_b.P2CLKEN = DISABLE)     /*!< Disable clock for GPIO2 */
#define __HAL_RCC_GPIO_P3_CLK_DISABLE() (SN_SCU->APB1CLKG_b.P3CLKEN = DISABLE)     /*!< Disable clock for GPIO3 */
#define __HAL_RCC_LCM_CLK_DISABLE()     (SN_SCU->APB1CLKG_b.LCMCLKEN = DISABLE)    /*!< Disable clock for LCK */
/**
 * @}
 */

/** \defgroup rcc_apb1_clk_chk APB1 Peripheral Clock checking
 * \brief  Checking the APB1 Peripheral clock.
 * \ingroup rcc_api_define
 * \note
 * \{
 */
#define __HAL_RCC_ADC0_IS_CLK_ENABLE()    (SN_SCU->APB1CLKG_b.ADC0CLKEN == ENABLE)   /*!< Check ADC0 clock enable */
#define __HAL_RCC_SSP0_IS_CLK_ENABLE()    (SN_SCU->APB1CLKG_b.SSP0CLKEN == ENABLE)   /*!< Check SSP0 clock enable */
#define __HAL_RCC_SSP1_IS_CLK_ENABLE()    (SN_SCU->APB1CLKG_b.SSP1CLKEN == ENABLE)   /*!< Check SSP1 clock enable */
#define __HAL_RCC_SSP2_IS_CLK_ENABLE()    (SN_SCU->APB1CLKG_b.SSP2CLKEN == ENABLE)   /*!< Check SSP2 clock enable */
#define __HAL_RCC_UART0_IS_CLK_ENABLE()   (SN_SCU->APB1CLKG_b.UART0CLKEN == ENABLE)  /*!< Check UART0 clock enable */
#define __HAL_RCC_UART5_IS_CLK_ENABLE()   (SN_SCU->APB1CLKG_b.UART5CLKEN == ENABLE)  /*!< Check UART5 clock enable */
#define __HAL_RCC_I2C0_IS_CLK_ENABLE()    (SN_SCU->APB1CLKG_b.I2C0CLKEN == ENABLE)   /*!< Check I2C0 clock enable */
#define __HAL_RCC_I2C1_IS_CLK_ENABLE()    (SN_SCU->APB1CLKG_b.I2C1CLKEN == ENABLE)   /*!< Check I2C1 clock enable */
#define __HAL_RCC_I2C2_IS_CLK_ENABLE()    (SN_SCU->APB1CLKG_b.I2C2CLKEN == ENABLE)   /*!< Check I2C2 clock enable */
#define __HAL_RCC_CT16B0_IS_CLK_ENABLE()  (SN_SCU->APB1CLKG_b.CT16B0CLKEN == ENABLE) /*!< Check CT16B0 clock enable */
#define __HAL_RCC_CT16B1_IS_CLK_ENABLE()  (SN_SCU->APB1CLKG_b.CT16B1CLKEN == ENABLE) /*!< Check CT16B1 clock enable */
#define __HAL_RCC_CT16B2_IS_CLK_ENABLE()  (SN_SCU->APB1CLKG_b.CT16B2CLKEN == ENABLE) /*!< Check CT16B2 clock enable */
#define __HAL_RCC_CT16B3_IS_CLK_ENABLE()  (SN_SCU->APB1CLKG_b.CT16B3CLKEN == ENABLE) /*!< Check CT16B3 clock enable */
#define __HAL_RCC_CT16B4_IS_CLK_ENABLE()  (SN_SCU->APB1CLKG_b.CT16B4CLKEN == ENABLE) /*!< Check CT16B4 clock enable */
#define __HAL_RCC_CT16B5_IS_CLK_ENABLE()  (SN_SCU->APB1CLKG_b.CT16B5CLKEN == ENABLE) /*!< Check CT16B5 clock enable */
#define __HAL_RCC_CT16B8_IS_CLK_ENABLE()  (SN_SCU->APB1CLKG_b.CT16B8CLKEN == ENABLE) /*!< Check CT16B8 clock enable */
#define __HAL_RCC_CRC_IS_CLK_ENABLE()     (SN_SCU->APB1CLKG_b.CRCCLKEN == ENABLE)    /*!< Check CRC clock enable */
#define __HAL_RCC_GPIO_P0_IS_CLK_ENABLE() (SN_SCU->APB1CLKG_b.P0CLKEN == ENABLE)     /*!< Check GPIO0 clock enable */
#define __HAL_RCC_GPIO_P1_IS_CLK_ENABLE() (SN_SCU->APB1CLKG_b.P1CLKEN == ENABLE)     /*!< Check GPIO1 clock enable */
#define __HAL_RCC_GPIO_P2_IS_CLK_ENABLE() (SN_SCU->APB1CLKG_b.P2CLKEN == ENABLE)     /*!< Check GPIO2 clock enable */
#define __HAL_RCC_GPIO_P3_IS_CLK_ENABLE() (SN_SCU->APB1CLKG_b.P3CLKEN == ENABLE)     /*!< Check GPIO3 clock enable */
#define __HAL_RCC_LCM_IS_CLK_ENABLE()     (SN_SCU->APB1CLKG_b.LCMCLKEN == ENABLE)    /*!< Check LCM clock enable */

#define __HAL_RCC_ADC0_IS_CLK_DISABLE()    (SN_SCU->APB1CLKG_b.ADC0CLKEN == DISABLE)   /*!< Check ADC0 clock disable */
#define __HAL_RCC_SSP0_IS_CLK_DISABLE()    (SN_SCU->APB1CLKG_b.SSP0CLKEN == DISABLE)   /*!< Check SSP0 clock disable */
#define __HAL_RCC_SSP1_IS_CLK_DISABLE()    (SN_SCU->APB1CLKG_b.SSP1CLKEN == DISABLE)   /*!< Check SSP1 clock disable */
#define __HAL_RCC_SSP2_IS_CLK_DISABLE()    (SN_SCU->APB1CLKG_b.SSP2CLKEN == DISABLE)   /*!< Check SSP2 clock disable */
#define __HAL_RCC_UART0_IS_CLK_DISABLE()   (SN_SCU->APB1CLKG_b.UART0CLKEN == DISABLE)  /*!< Check UART0 clock disable */
#define __HAL_RCC_UART5_IS_CLK_DISABLE()   (SN_SCU->APB1CLKG_b.UART5CLKEN == DISABLE)  /*!< Check UART5 clock disable */
#define __HAL_RCC_I2C0_IS_CLK_DISABLE()    (SN_SCU->APB1CLKG_b.I2C0CLKEN == DISABLE)   /*!< Check I2C0 clock disable */
#define __HAL_RCC_I2C1_IS_CLK_DISABLE()    (SN_SCU->APB1CLKG_b.I2C1CLKEN == DISABLE)   /*!< Check I2C1 clock disable */
#define __HAL_RCC_I2C2_IS_CLK_DISABLE()    (SN_SCU->APB1CLKG_b.I2C2CLKEN == DISABLE)   /*!< Check I2C2 clock disable */
#define __HAL_RCC_CT16B0_IS_CLK_DISABLE()  (SN_SCU->APB1CLKG_b.CT16B0CLKEN == DISABLE) /*!< Check CT16B0 clock disable */
#define __HAL_RCC_CT16B1_IS_CLK_DISABLE()  (SN_SCU->APB1CLKG_b.CT16B1CLKEN == DISABLE) /*!< Check CT16B1 clock disable */
#define __HAL_RCC_CT16B2_IS_CLK_DISABLE()  (SN_SCU->APB1CLKG_b.CT16B2CLKEN == DISABLE) /*!< Check CT16B2 clock disable */
#define __HAL_RCC_CT16B3_IS_CLK_DISABLE()  (SN_SCU->APB1CLKG_b.CT16B3CLKEN == DISABLE) /*!< Check CT16B3 clock disable */
#define __HAL_RCC_CT16B4_IS_CLK_DISABLE()  (SN_SCU->APB1CLKG_b.CT16B4CLKEN == DISABLE) /*!< Check CT16B4 clock disable */
#define __HAL_RCC_CT16B5_IS_CLK_DISABLE()  (SN_SCU->APB1CLKG_b.CT16B5CLKEN == DISABLE) /*!< Check CT16B5 clock disable */
#define __HAL_RCC_CT16B8_IS_CLK_DISABLE()  (SN_SCU->APB1CLKG_b.CT16B8CLKEN == DISABLE) /*!< Check CT16B8 clock disable */
#define __HAL_RCC_CRC_IS_CLK_DISABLE()     (SN_SCU->APB1CLKG_b.CRCCLKEN == DISABLE)    /*!< Check CRC clock disable */
#define __HAL_RCC_GPIO_P0_IS_CLK_DISABLE() (SN_SCU->APB1CLKG_b.P0CLKEN == DISABLE)     /*!< Check GPIO0 clock disable */
#define __HAL_RCC_GPIO_P1_IS_CLK_DISABLE() (SN_SCU->APB1CLKG_b.P1CLKEN == DISABLE)     /*!< Check GPIO1 clock disable */
#define __HAL_RCC_GPIO_P2_IS_CLK_DISABLE() (SN_SCU->APB1CLKG_b.P2CLKEN == DISABLE)     /*!< Check GPIO2 clock disable */
#define __HAL_RCC_GPIO_P3_IS_CLK_DISABLE() (SN_SCU->APB1CLKG_b.P3CLKEN == DISABLE)     /*!< Check GPIO3 clock disable */
#define __HAL_RCC_LCM_IS_CLK_DISABLE()     (SN_SCU->APB1CLKG_b.LCMCLKEN == DISABLE)    /*!< Check LCM clock disable */

/**
 * @}
 */

/** \defgroup rcc_sleep_ahb_clk_ctrl AHB Peripheral Clock in sleep mode Enable Disable
 * \brief  Enable or disable the AHB Peripheral clock in sleep mode.
 * \ingroup rcc_api_define
 * \note
 * \{
 */
#define __HAL_RCC_DMA0_CLK_SLEEP_ENABLE()          (SN_SCU->SLP_AHBCLKG_b.DMA0CLKEN = ENABLE)  /*!< Enable clock for DMA0 in sleep mode */
#define __HAL_RCC_DMA1_CLK_SLEEP_ENABLE()          (SN_SCU->SLP_AHBCLKG_b.DMA1CLKEN = ENABLE)  /*!< Enable clock for DMA1 in sleep mode */
#define __HAL_RCC_SRAM1_CLK_SLEEP_ENABLE()         (SN_SCU->SLP_AHBCLKG_b.SRAM1EN = ENABLE)    /*!< Enable clock for SRAM1 in sleep mode */
#define __HAL_RCC_SRAM2_CLK_SLEEP_ENABLE()         (SN_SCU->SLP_AHBCLKG_b.SRAM2EN = ENABLE)    /*!< Enable clock for SRAM2 in sleep mode */
#define __HAL_RCC_SRAM3_CLK_SLEEP_ENABLE()         (SN_SCU->SLP_AHBCLKG_b.SRAM3EN = ENABLE)    /*!< Enable clock for SRAM3 in sleep mode */
#define __HAL_RCC_FLASH_CLK_SLEEP_ENABLE()         (SN_SCU->SLP_AHBCLKG_b.FLASHEN = ENABLE)    /*!< Enable clock for FLASH in sleep mode */
#define __HAL_RCC_SDIO_CLK_SLEEP_ENABLE()          (SN_SCU->SLP_AHBCLKG_b.SDIOCLKEN = ENABLE)  /*!< Enable clock for SDIO in sleep mode */
#define __HAL_RCC_LCD_CLK_SLEEP_ENABLE()           (SN_SCU->SLP_AHBCLKG_b.LCDCLKEN = ENABLE)   /*!< Enable clock for LCD in sleep mode */
#define __HAL_RCC_ETHERNET_CLK_SLEEP_ENABLE()      (SN_SCU->SLP_AHBCLKG_b.ETHCLKEN = ENABLE)   /*!< Enable clock for Ethernet MAC in sleep mode */
#define __HAL_RCC_USB_HS_CLK_SLEEP_ENABLE()        (SN_SCU->SLP_AHBCLKG_b.USBHSCLKEN = ENABLE) /*!< Enable clock for USB_HS controller in sleep mode */
#define __HAL_RCC_BACKUP_SRAM_CLK_SLEEP_ENABLE()   (SN_SCU->SLP_AHBCLKG_b.BKPSRAMEN = ENABLE)  /*!< Enable clock for Backup SRAL in sleep mode */
#define __HAL_RCC_AHBM_CLK_SLEEP_ENABLE()          (SN_SCU->SLP_AHBCLKG_b.AHBMCLKEN = ENABLE)  /*!< Enable clock for AHBM bus matrix in sleep mode */
#define __HAL_RCC_H2P0_CLK_SLEEP_ENABLE()          (SN_SCU->SLP_AHBCLKG_b.H2P0HCLKEN = ENABLE) /*!< Enable clock for AHB to APB0 bridge in sleep mode */
#define __HAL_RCC_H2P1_SRAM_CLK_SLEEP_ENABLE()     (SN_SCU->SLP_AHBCLKG_b.H2P1HCLKEN = ENABLE) /*!< Enable clock for AHB to APB1 bridge and AHB to APB2 bridge in sleep mode */
#define __HAL_RCC_ETHERNET_SRAM_CLK_SLEEP_ENABLE() (SN_SCU->SLP_AHBCLKG_b.ETHSRAMEN = ENABLE)  /*!< Enable clock for Ethernet MAC SREAM in sleep mode */

#define __HAL_RCC_DMA0_CLK_SLEEP_DISABLE()          (SN_SCU->SLP_AHBCLKG_b.DMA0CLKEN = DISABLE)  /*!< Disable clock for DMA0 in sleep mode */
#define __HAL_RCC_DMA1_CLK_SLEEP_DISABLE()          (SN_SCU->SLP_AHBCLKG_b.DMA1CLKEN = DISABLE)  /*!< Disable clock for DMA1 in sleep mode */
#define __HAL_RCC_SRAM1_CLK_SLEEP_DISABLE()         (SN_SCU->SLP_AHBCLKG_b.SRAM1EN = DISABLE)    /*!< Disable clock for SRAM1 in sleep mode */
#define __HAL_RCC_SRAM2_CLK_SLEEP_DISABLE()         (SN_SCU->SLP_AHBCLKG_b.SRAM2EN = DISABLE)    /*!< Disable clock for SRAM2 in sleep mode */
#define __HAL_RCC_SRAM3_CLK_SLEEP_DISABLE()         (SN_SCU->SLP_AHBCLKG_b.SRAM3EN = DISABLE)    /*!< Disable clock for SRAM3 in sleep mode */
#define __HAL_RCC_FLASH_CLK_SLEEP_DISABLE()         (SN_SCU->SLP_AHBCLKG_b.FLASHEN = DISABLE)    /*!< Disable clock for FLASH in sleep mode */
#define __HAL_RCC_SDIO_CLK_SLEEP_DISABLE()          (SN_SCU->SLP_AHBCLKG_b.SDIOCLKEN = DISABLE)  /*!< Disable clock for SDIO in sleep mode */
#define __HAL_RCC_LCD_CLK_SLEEP_DISABLE()           (SN_SCU->SLP_AHBCLKG_b.LCDCLKEN = DISABLE)   /*!< Disable clock for LCD in sleep mode */
#define __HAL_RCC_ETHERNET_CLK_SLEEP_DISABLE()      (SN_SCU->SLP_AHBCLKG_b.ETHCLKEN = DISABLE)   /*!< Disable clock for Ethernet MAC in sleep mode */
#define __HAL_RCC_USB_HS_CLK_SLEEP_DISABLE()        (SN_SCU->SLP_AHBCLKG_b.USBHSCLKEN = DISABLE) /*!< Disable clock for USB_HS controller in sleep mode */
#define __HAL_RCC_BACKUP_SRAM_CLK_SLEEP_DISABLE()   (SN_SCU->SLP_AHBCLKG_b.BKPSRAMEN = DISABLE)  /*!< Disable clock for Backup SRAL in sleep mode */
#define __HAL_RCC_AHBM_CLK_SLEEP_DISABLE()          (SN_SCU->SLP_AHBCLKG_b.AHBMCLKEN = DISABLE)  /*!< Disable clock for AHBM bus matrix in sleep mode */
#define __HAL_RCC_H2P0_CLK_SLEEP_DISABLE()          (SN_SCU->SLP_AHBCLKG_b.H2P0HCLKEN = DISABLE) /*!< Disable clock for AHB to APB0 bridge in sleep mode */
#define __HAL_RCC_H2P1_SRAM_CLK_SLEEP_DISABLE()     (SN_SCU->SLP_AHBCLKG_b.H2P1HCLKEN = DISABLE) /*!< Disable clock for AHB to APB1 bridge and AHB to APB2 bridge in sleep mode */
#define __HAL_RCC_ETHERNET_SRAM_CLK_SLEEP_DISABLE() (SN_SCU->SLP_AHBCLKG_b.ETHSRAMEN = DISABLE)  /*!< Disable clock for Ethernet MAC SREAM in sleep mode */
/**
 * @}
 */

/** \defgroup rcc_sleep_apb0_clk_ctrl APB0 Peripheral Clock in sleep mode Enable Disable
 * \brief  Enable or disable the APB0 Peripheral clock in sleep mode.
 * \ingroup rcc_api_define
 * \note
 * \{
 */
#define __HAL_RCC_WDT_CLK_SLEEP_ENABLE()       (SN_SCU->SLP_APB0CLKG_b.WDTCLKEN = ENABLE)       /*!< Enable clock for WDT in sleep mode*/
#define __HAL_RCC_WWDT_CLK_SLEEP_ENABLE()      (SN_SCU->SLP_APB0CLKG_b.WWDTCLKEN = ENABLE)      /*!< Enable clock for WWDT in sleep mode*/
#define __HAL_RCC_CAN0_CLK_SLEEP_ENABLE()      (SN_SCU->SLP_APB0CLKG_b.CAN0CLKEN = ENABLE)      /*!< Enable clock for CAN0 in sleep mode*/
#define __HAL_RCC_CAN1_CLK_SLEEP_ENABLE()      (SN_SCU->SLP_APB0CLKG_b.CAN1CLKEN = ENABLE)      /*!< Enable clock for CAN1 in sleep mode*/
#define __HAL_RCC_UART1_CLK_SLEEP_ENABLE()     (SN_SCU->SLP_APB0CLKG_b.UART1CLKEN = ENABLE)     /*!< Enable clock for UART1 in sleep mode*/
#define __HAL_RCC_UART2_CLK_SLEEP_ENABLE()     (SN_SCU->SLP_APB0CLKG_b.UART2CLKEN = ENABLE)     /*!< Enable clock for UART2 in sleep mode*/
#define __HAL_RCC_UART3_CLK_SLEEP_ENABLE()     (SN_SCU->SLP_APB0CLKG_b.UART3CLKEN = ENABLE)     /*!< Enable clock for UART3 in sleep mode*/
#define __HAL_RCC_UART4_CLK_SLEEP_ENABLE()     (SN_SCU->SLP_APB0CLKG_b.UART4CLKEN = ENABLE)     /*!< Enable clock for UART4 in sleep mode*/
#define __HAL_RCC_CT16B6_CLK_SLEEP_ENABLE()    (SN_SCU->SLP_APB0CLKG_b.CT16B6CLKEN = ENABLE)    /*!< Enable clock for CT16B6 in sleep mode*/
#define __HAL_RCC_CT16B7_CLK_SLEEP_ENABLE()    (SN_SCU->SLP_APB0CLKG_b.CT16B7CLKEN = ENABLE)    /*!< Enable clock for CT16B7 in sleep mode*/
#define __HAL_RCC_ALWAYSON_CLK_SLEEP_ENABLE()  (SN_SCU->SLP_APB0CLKG_b.ALWAYSONREGEN = ENABLE)  /*!< Enable clock for Always-On domain registers in sleep mode*/
#define __HAL_RCC_USBPHY_CLK_SLEEP_ENABLE()    (SN_SCU->SLP_APB0CLKG_b.USBPHYCLKEN = ENABLE)    /*!< Enable clock for USB HS PHY PCLK in sleep mode*/
#define __HAL_RCC_USBPHY12M_CLK_SLEEP_ENABLE() (SN_SCU->SLP_APB0CLKG_b.USBPHY12MCLKEN = ENABLE) /*!< Enable clock for USB HS PHY 12M clock in sleep mode*/
#define __HAL_RCC_CAN0_SRAM_CLK_SLEEP_ENABLE() (SN_SCU->SLP_APB0CLKG_b.CAN0SRAMCLKEN = ENABLE)  /*!< Enable clock for CAN0 SRAM in sleep mode*/
#define __HAL_RCC_CAN1_SRAM_CLK_SLEEP_ENABLE() (SN_SCU->SLP_APB0CLKG_b.CAN1SRAMCLKEN = ENABLE)  /*!< Enable clock for CAN1 SRAM in sleep mode*/
#define __HAL_RCC_CLKOUT_CLK_SLEEP_ENABLE()    (SN_SCU->SLP_APB0CLKG_b.CLKOUTEN = ENABLE)       /*!< Enable clock for CLKOUT in sleep mode*/

#define __HAL_RCC_WDT_CLK_SLEEP_DISABLE()       (SN_SCU->SLP_APB0CLKG_b.WDTCLKEN = DISABLE)       /*!< Disable clock for WDT in sleep mode*/
#define __HAL_RCC_WWDT_CLK_SLEEP_DISABLE()      (SN_SCU->SLP_APB0CLKG_b.WWDTCLKEN = DISABLE)      /*!< Disable clock for WWDT in sleep mode*/
#define __HAL_RCC_CAN0_CLK_SLEEP_DISABLE()      (SN_SCU->SLP_APB0CLKG_b.CAN0CLKEN = DISABLE)      /*!< Disable clock for CAN0 in sleep mode*/
#define __HAL_RCC_CAN1_CLK_SLEEP_DISABLE()      (SN_SCU->SLP_APB0CLKG_b.CAN1CLKEN = DISABLE)      /*!< Disable clock for CAN1 in sleep mode*/
#define __HAL_RCC_UART1_CLK_SLEEP_DISABLE()     (SN_SCU->SLP_APB0CLKG_b.UART1CLKEN = DISABLE)     /*!< Disable clock for UART1 in sleep mode*/
#define __HAL_RCC_UART2_CLK_SLEEP_DISABLE()     (SN_SCU->SLP_APB0CLKG_b.UART2CLKEN = DISABLE)     /*!< Disable clock for UART2 in sleep mode*/
#define __HAL_RCC_UART3_CLK_SLEEP_DISABLE()     (SN_SCU->SLP_APB0CLKG_b.UART3CLKEN = DISABLE)     /*!< Disable clock for UART3 in sleep mode*/
#define __HAL_RCC_UART4_CLK_SLEEP_DISABLE()     (SN_SCU->SLP_APB0CLKG_b.UART4CLKEN = DISABLE)     /*!< Disable clock for UART4 in sleep mode*/
#define __HAL_RCC_CT16B6_CLK_SLEEP_DISABLE()    (SN_SCU->SLP_APB0CLKG_b.CT16B6CLKEN = DISABLE)    /*!< Disable clock for CT16B6 in sleep mode*/
#define __HAL_RCC_CT16B7_CLK_SLEEP_DISABLE()    (SN_SCU->SLP_APB0CLKG_b.CT16B7CLKEN = DISABLE)    /*!< Disable clock for CT16B7 in sleep mode*/
#define __HAL_RCC_ALWAYSON_CLK_SLEEP_DISABLE()  (SN_SCU->SLP_APB0CLKG_b.ALWAYSONREGEN = DISABLE)  /*!< Disable clock for Always-On domain registers in sleep mode*/
#define __HAL_RCC_USBPHY_CLK_SLEEP_DISABLE()    (SN_SCU->SLP_APB0CLKG_b.USBPHYCLKEN = DISABLE)    /*!< Disable clock for USB HS PHY PCLK in sleep mode*/
#define __HAL_RCC_USBPHY12M_CLK_SLEEP_DISABLE() (SN_SCU->SLP_APB0CLKG_b.USBPHY12MCLKEN = DISABLE) /*!< Disable clock for USB HS PHY 12M clock in sleep mode*/
#define __HAL_RCC_CAN0_SRAM_CLK_SLEEP_DISABLE() (SN_SCU->SLP_APB0CLKG_b.CAN0SRAMCLKEN = DISABLE)  /*!< Disable clock for CAN0 SRAM in sleep mode*/
#define __HAL_RCC_CAN1_SRAM_CLK_SLEEP_DISABLE() (SN_SCU->SLP_APB0CLKG_b.CAN1SRAMCLKEN = DISABLE)  /*!< Disable clock for CAN1 SRAM in sleep mode*/
#define __HAL_RCC_CLKOUT_CLK_SLEEP_DISABLE()    (SN_SCU->SLP_APB0CLKG_b.CLKOUTEN = DISABLE)       /*!< Disable clock for CLKOUT in sleep mode*/
/**
 * @}
 */

/** \defgroup rcc_sleep_apb1_clk_ctrl APB1 Peripheral Clock in sleep mode Enable Disable
 * \brief  Enable or disable the APB1 Peripheral clock in sleep mode.
 * \ingroup rcc_api_define
 * \note
 * \{
 */
#define __HAL_RCC_ADC0_CLK_SLEEP_ENABLE()    (SN_SCU->SLP_APB1CLKG_b.ADC0CLKEN = ENABLE)   /*!< Enable clock for ADC0 in sleep mode */
#define __HAL_RCC_SSP0_CLK_SLEEP_ENABLE()    (SN_SCU->SLP_APB1CLKG_b.SSP0CLKEN = ENABLE)   /*!< Enable clock for SSP0 in sleep mode */
#define __HAL_RCC_SSP1_CLK_SLEEP_ENABLE()    (SN_SCU->SLP_APB1CLKG_b.SSP1CLKEN = ENABLE)   /*!< Enable clock for SSP1 in sleep mode */
#define __HAL_RCC_SSP2_CLK_SLEEP_ENABLE()    (SN_SCU->SLP_APB1CLKG_b.SSP2CLKEN = ENABLE)   /*!< Enable clock for SSP2 in sleep mode */
#define __HAL_RCC_UART0_CLK_SLEEP_ENABLE()   (SN_SCU->SLP_APB1CLKG_b.UART0CLKEN = ENABLE)  /*!< Enable clock for UART0 in sleep mode */
#define __HAL_RCC_UART5_CLK_SLEEP_ENABLE()   (SN_SCU->SLP_APB1CLKG_b.UART5CLKEN = ENABLE)  /*!< Enable clock for UART5 in sleep mode */
#define __HAL_RCC_I2C0_CLK_SLEEP_ENABLE()    (SN_SCU->SLP_APB1CLKG_b.I2C0CLKEN = ENABLE)   /*!< Enable clock for I2C0 in sleep mode */
#define __HAL_RCC_I2C1_CLK_SLEEP_ENABLE()    (SN_SCU->SLP_APB1CLKG_b.I2C1CLKEN = ENABLE)   /*!< Enable clock for I2C1 in sleep mode */
#define __HAL_RCC_I2C2_CLK_SLEEP_ENABLE()    (SN_SCU->SLP_APB1CLKG_b.I2C2CLKEN = ENABLE)   /*!< Enable clock for I2C2 in sleep mode */
#define __HAL_RCC_CT16B0_CLK_SLEEP_ENABLE()  (SN_SCU->SLP_APB1CLKG_b.CT16B0CLKEN = ENABLE) /*!< Enable clock for CT16B0 in sleep mode */
#define __HAL_RCC_CT16B1_CLK_SLEEP_ENABLE()  (SN_SCU->SLP_APB1CLKG_b.CT16B1CLKEN = ENABLE) /*!< Enable clock for CT16B1 in sleep mode */
#define __HAL_RCC_CT16B2_CLK_SLEEP_ENABLE()  (SN_SCU->SLP_APB1CLKG_b.CT16B2CLKEN = ENABLE) /*!< Enable clock for CT16B2 in sleep mode */
#define __HAL_RCC_CT16B3_CLK_SLEEP_ENABLE()  (SN_SCU->SLP_APB1CLKG_b.CT16B3CLKEN = ENABLE) /*!< Enable clock for CT16B3 in sleep mode */
#define __HAL_RCC_CT16B4_CLK_SLEEP_ENABLE()  (SN_SCU->SLP_APB1CLKG_b.CT16B4CLKEN = ENABLE) /*!< Enable clock for CT16B4 in sleep mode */
#define __HAL_RCC_CT16B5_CLK_SLEEP_ENABLE()  (SN_SCU->SLP_APB1CLKG_b.CT16B5CLKEN = ENABLE) /*!< Enable clock for CT16B5 in sleep mode */
#define __HAL_RCC_CT16B8_CLK_SLEEP_ENABLE()  (SN_SCU->SLP_APB1CLKG_b.CT16B8CLKEN = ENABLE) /*!< Enable clock for CT16B8 in sleep mode */
#define __HAL_RCC_CRC_CLK_SLEEP_ENABLE()     (SN_SCU->SLP_APB1CLKG_b.CRCCLKEN = ENABLE)    /*!< Enable clock for CRC in sleep mode */
#define __HAL_RCC_GPIO_P0_CLK_SLEEP_ENABLE() (SN_SCU->SLP_APB1CLKG_b.P0CLKEN = ENABLE)     /*!< Enable clock for GPIO0 in sleep mode */
#define __HAL_RCC_GPIO_P1_CLK_SLEEP_ENABLE() (SN_SCU->SLP_APB1CLKG_b.P1CLKEN = ENABLE)     /*!< Enable clock for GPIO1 in sleep mode */
#define __HAL_RCC_GPIO_P2_CLK_SLEEP_ENABLE() (SN_SCU->SLP_APB1CLKG_b.P2CLKEN = ENABLE)     /*!< Enable clock for GPIO2 in sleep mode */
#define __HAL_RCC_GPIO_P3_CLK_SLEEP_ENABLE() (SN_SCU->SLP_APB1CLKG_b.P3CLKEN = ENABLE)     /*!< Enable clock for GPIO3 in sleep mode */
#define __HAL_RCC_LCM_CLK_SLEEP_ENABLE()     (SN_SCU->SLP_APB1CLKG_b.LCMCLKEN = ENABLE)    /*!< Enable clock for LCM in sleep mode */

#define __HAL_RCC_ADC0_CLK_SLEEP_DISABLE()    (SN_SCU->SLP_APB1CLKG_b.ADC0CLKEN = DISABLE)   /*!< Disable clock for ADC0 in sleep mode */
#define __HAL_RCC_SSP0_CLK_SLEEP_DISABLE()    (SN_SCU->SLP_APB1CLKG_b.SSP0CLKEN = DISABLE)   /*!< Disable clock for SSP0 in sleep mode */
#define __HAL_RCC_SSP1_CLK_SLEEP_DISABLE()    (SN_SCU->SLP_APB1CLKG_b.SSP1CLKEN = DISABLE)   /*!< Disable clock for SSP1 in sleep mode */
#define __HAL_RCC_SSP2_CLK_SLEEP_DISABLE()    (SN_SCU->SLP_APB1CLKG_b.SSP2CLKEN = DISABLE)   /*!< Disable clock for SSP2 in sleep mode */
#define __HAL_RCC_UART0_CLK_SLEEP_DISABLE()   (SN_SCU->SLP_APB1CLKG_b.UART0CLKEN = DISABLE)  /*!< Disable clock for UART0 in sleep mode */
#define __HAL_RCC_UART5_CLK_SLEEP_DISABLE()   (SN_SCU->SLP_APB1CLKG_b.UART5CLKEN = DISABLE)  /*!< Disable clock for UART5 in sleep mode */
#define __HAL_RCC_I2C0_CLK_SLEEP_DISABLE()    (SN_SCU->SLP_APB1CLKG_b.I2C0CLKEN = DISABLE)   /*!< Disable clock for I2C0 in sleep mode */
#define __HAL_RCC_I2C1_CLK_SLEEP_DISABLE()    (SN_SCU->SLP_APB1CLKG_b.I2C1CLKEN = DISABLE)   /*!< Disable clock for I2C1 in sleep mode */
#define __HAL_RCC_I2C2_CLK_SLEEP_DISABLE()    (SN_SCU->SLP_APB1CLKG_b.I2C2CLKEN = DISABLE)   /*!< Disable clock for I2C2 in sleep mode */
#define __HAL_RCC_CT16B0_CLK_SLEEP_DISABLE()  (SN_SCU->SLP_APB1CLKG_b.CT16B0CLKEN = DISABLE) /*!< Disable clock for CT16B0 in sleep mode */
#define __HAL_RCC_CT16B1_CLK_SLEEP_DISABLE()  (SN_SCU->SLP_APB1CLKG_b.CT16B1CLKEN = DISABLE) /*!< Disable clock for CT16B1 in sleep mode */
#define __HAL_RCC_CT16B2_CLK_SLEEP_DISABLE()  (SN_SCU->SLP_APB1CLKG_b.CT16B2CLKEN = DISABLE) /*!< Disable clock for CT16B2 in sleep mode */
#define __HAL_RCC_CT16B3_CLK_SLEEP_DISABLE()  (SN_SCU->SLP_APB1CLKG_b.CT16B3CLKEN = DISABLE) /*!< Disable clock for CT16B3 in sleep mode */
#define __HAL_RCC_CT16B4_CLK_SLEEP_DISABLE()  (SN_SCU->SLP_APB1CLKG_b.CT16B4CLKEN = DISABLE) /*!< Disable clock for CT16B4 in sleep mode */
#define __HAL_RCC_CT16B5_CLK_SLEEP_DISABLE()  (SN_SCU->SLP_APB1CLKG_b.CT16B5CLKEN = DISABLE) /*!< Disable clock for CT16B5 in sleep mode */
#define __HAL_RCC_CT16B8_CLK_SLEEP_DISABLE()  (SN_SCU->SLP_APB1CLKG_b.CT16B8CLKEN = DISABLE) /*!< Disable clock for CT16B8 in sleep mode */
#define __HAL_RCC_CRC_CLK_SLEEP_DISABLE()     (SN_SCU->SLP_APB1CLKG_b.CRCCLKEN = DISABLE)    /*!< Disable clock for CRC in sleep mode */
#define __HAL_RCC_GPIO_P0_CLK_SLEEP_DISABLE() (SN_SCU->SLP_APB1CLKG_b.P0CLKEN = DISABLE)     /*!< Disable clock for GPIO0 in sleep mode */
#define __HAL_RCC_GPIO_P1_CLK_SLEEP_DISABLE() (SN_SCU->SLP_APB1CLKG_b.P1CLKEN = DISABLE)     /*!< Disable clock for GPIO1 in sleep mode */
#define __HAL_RCC_GPIO_P2_CLK_SLEEP_DISABLE() (SN_SCU->SLP_APB1CLKG_b.P2CLKEN = DISABLE)     /*!< Disable clock for GPIO2 in sleep mode */
#define __HAL_RCC_GPIO_P3_CLK_SLEEP_DISABLE() (SN_SCU->SLP_APB1CLKG_b.P3CLKEN = DISABLE)     /*!< Disable clock for GPIO3 in sleep mode */
#define __HAL_RCC_LCM_CLK_SLEEP_DISABLE()     (SN_SCU->SLP_APB1CLKG_b.LCMCLKEN = DISABLE)    /*!< Disable clock for LCM in sleep mode */
/**
 * @}
 */

#define RCC_CT16B_CLKSEL_HCLK    0 /*!< CT16Bn clock source switches to HCLK */
#define RCC_CT16B_CLKSEL_PLLCLK  1 /*!< CT16Bn clock source switches to PLL clock */
#define RCC_CT16B5_CLKSEL_ELSCLK 2 /*!< CT16Bn clock source switches to ELS clock */

/** \defgroup rcc_clk_sel APB1 Peripheral clock selection Get Set
 * \brief  Get or set Peripheral clock source selection.
 * \ingroup rcc_api_define
 * \note
 * \{
 */
#define __HAL_RCC_SET_CT16B0_CLKSEL(x) (SN_SCU->CLKSEL_b.CT16B0CLKSEL = x) /*!< Set CT16B0 clock source as RCC_CT16B_CLKSEL_xxx */
#define __HAL_RCC_SET_CT16B1_CLKSEL(x) (SN_SCU->CLKSEL_b.CT16B1CLKSEL = x) /*!< Set CT16B1 clock source as RCC_CT16B_CLKSEL_xxx */
#define __HAL_RCC_SET_CT16B2_CLKSEL(x) (SN_SCU->CLKSEL_b.CT16B2CLKSEL = x) /*!< Set CT16B2 clock source as RCC_CT16B_CLKSEL_xxx */
#define __HAL_RCC_SET_CT16B3_CLKSEL(x) (SN_SCU->CLKSEL_b.CT16B3CLKSEL = x) /*!< Set CT16B3 clock source as RCC_CT16B_CLKSEL_xxx */
#define __HAL_RCC_SET_CT16B4_CLKSEL(x) (SN_SCU->CLKSEL_b.CT16B4CLKSEL = x) /*!< Set CT16B4 clock source as RCC_CT16B_CLKSEL_xxx */
#define __HAL_RCC_SET_CT16B5_CLKSEL(x) (SN_SCU->CLKSEL_b.CT16B5CLKSEL = x) /*!< Set CT16B5 clock source as RCC_CT16B_CLKSEL_xxx */
#define __HAL_RCC_SET_CT16B6_CLKSEL(x) (SN_SCU->CLKSEL_b.CT16B6CLKSEL = x) /*!< Set CT16B6 clock source as RCC_CT16B_CLKSEL_xxx */
#define __HAL_RCC_SET_CT16B7_CLKSEL(x) (SN_SCU->CLKSEL_b.CT16B7CLKSEL = x) /*!< Set CT16B7 clock source as RCC_CT16B_CLKSEL_xxx */
#define __HAL_RCC_SET_CT16B8_CLKSEL(x) (SN_SCU->CLKSEL_b.CT16B8CLKSEL = x) /*!< Set CT16B8 clock source as RCC_CT16B_CLKSEL_xxx */

#define __HAL_RCC_GET_CT16B0_CLKSEL()  (SN_SCU->CLKSEL_b.CT16B0CLKSEL) /*!< Get CT16B0 clock source */
#define __HAL_RCC_GET_CT16B1_CLKSEL()  (SN_SCU->CLKSEL_b.CT16B1CLKSEL) /*!< Get CT16B1 clock source */
#define __HAL_RCC_GET_CT16B2_CLKSEL()  (SN_SCU->CLKSEL_b.CT16B2CLKSEL) /*!< Get CT16B2 clock source */
#define __HAL_RCC_GET_CT16B3_CLKSEL()  (SN_SCU->CLKSEL_b.CT16B3CLKSEL) /*!< Get CT16B3 clock source */
#define __HAL_RCC_GET_CT16B4_CLKSEL()  (SN_SCU->CLKSEL_b.CT16B4CLKSEL) /*!< Get CT16B4 clock source */
#define __HAL_RCC_GET_CT16B5_CLKSEL()  (SN_SCU->CLKSEL_b.CT16B5CLKSEL) /*!< Get CT16B5 clock source */
#define __HAL_RCC_GET_CT16B6_CLKSEL()  (SN_SCU->CLKSEL_b.CT16B6CLKSEL) /*!< Get CT16B6 clock source */
#define __HAL_RCC_GET_CT16B7_CLKSEL()  (SN_SCU->CLKSEL_b.CT16B7CLKSEL) /*!< Get CT16B7 clock source */
#define __HAL_RCC_GET_CT16B8_CLKSEL()  (SN_SCU->CLKSEL_b.CT16B8CLKSEL) /*!< Get CT16B8 clock source */
#define __HAL_RCC_GET_CT16Bn_CLKSEL(i) ((SN_SCU->CLKSEL >> (i)) & 1)   /*!< Get CT16Bn clock source */

#define RCC_UART_CLKSEL_HCLK   0 /*!< UARTn clock source switches to HCLK */
#define RCC_UART_CLKSEL_PLLCLK 1 /*!< UARTn clock source switches to PLL clock */

#define __HAL_RCC_SET_UART0_CLKSEL(x) (SN_SCU->CLKSEL_b.UART0CLKSEL = x) /*!< Set UART0 clock source as RCC_UART_CLKSEL_xxx */
#define __HAL_RCC_SET_UART1_CLKSEL(x) (SN_SCU->CLKSEL_b.UART1CLKSEL = x) /*!< Set UART1 clock source as RCC_UART_CLKSEL_xxx */
#define __HAL_RCC_SET_UART2_CLKSEL(x) (SN_SCU->CLKSEL_b.UART2CLKSEL = x) /*!< Set UART2 clock source as RCC_UART_CLKSEL_xxx */
#define __HAL_RCC_SET_UART3_CLKSEL(x) (SN_SCU->CLKSEL_b.UART3CLKSEL = x) /*!< Set UART3 clock source as RCC_UART_CLKSEL_xxx */
#define __HAL_RCC_SET_UART4_CLKSEL(x) (SN_SCU->CLKSEL_b.UART4CLKSEL = x) /*!< Set UART4 clock source as RCC_UART_CLKSEL_xxx */
#define __HAL_RCC_SET_UART5_CLKSEL(x) (SN_SCU->CLKSEL_b.UART5CLKSEL = x) /*!< Set UART5 clock source as RCC_UART_CLKSEL_xxx */

#define __HAL_RCC_GET_UART0_CLKSEL()  (SN_SCU->CLKSEL_b.UART0CLKSEL)       /*!< Get UART0 clock source */
#define __HAL_RCC_GET_UART1_CLKSEL()  (SN_SCU->CLKSEL_b.UART1CLKSEL)       /*!< Get UART1 clock source */
#define __HAL_RCC_GET_UART2_CLKSEL()  (SN_SCU->CLKSEL_b.UART2CLKSEL)       /*!< Get UART2 clock source */
#define __HAL_RCC_GET_UART3_CLKSEL()  (SN_SCU->CLKSEL_b.UART3CLKSEL)       /*!< Get UART3 clock source */
#define __HAL_RCC_GET_UART4_CLKSEL()  (SN_SCU->CLKSEL_b.UART4CLKSEL)       /*!< Get UART4 clock source */
#define __HAL_RCC_GET_UART5_CLKSEL()  (SN_SCU->CLKSEL_b.UART5CLKSEL)       /*!< Get UART5 clock source */
#define __HAL_RCC_GET_UARTn_CLKSEL(i) ((SN_SCU->CLKSEL >> ((i) + 10)) & 1) /*!< Get UARTn clock source */

#define RCC_SPI_CLKSEL_HCLK   0 /*!< SPIn clock source switches to HCLK */
#define RCC_SPI_CLKSEL_PLLCLK 1 /*!< SPIn clock source switches to PLL clock */

#define __HAL_RCC_SET_SSP0_CLKSEL(x) (SN_SCU->CLKSEL_b.SSP0CLKSEL = x) /*!< Set SPI0 clock source as RCC_SPI_CLKSEL_xxx */
#define __HAL_RCC_SET_SSP1_CLKSEL(x) (SN_SCU->CLKSEL_b.SSP1CLKSEL = x) /*!< Set SPI1 clock source as RCC_SPI_CLKSEL_xxx */
#define __HAL_RCC_SET_SSP2_CLKSEL(x) (SN_SCU->CLKSEL_b.SSP2CLKSEL = x) /*!< Set SPI2 clock source as RCC_SPI_CLKSEL_xxx */

#define __HAL_RCC_GET_SSP0_CLKSEL()  (SN_SCU->CLKSEL_b.SSP0CLKSEL)        /*!< Get SPI0 clock source */
#define __HAL_RCC_GET_SSP1_CLKSEL()  (SN_SCU->CLKSEL_b.SSP1CLKSEL)        /*!< Get SPI1 clock source */
#define __HAL_RCC_GET_SSP2_CLKSEL()  (SN_SCU->CLKSEL_b.SSP2CLKSEL)        /*!< Get SPI2 clock source */
#define __HAL_RCC_GET_SSPn_CLKSEL(i) ((SN_SCU->CLKSEL >> ((i) + 16)) & 1) /*!< Get SPIn clock source */

#define RCC_SDIO_CLKSEL_HCLK         0                                 /*!< SDIO clock source switches to HCLK */
#define RCC_SDIO_CLKSEL_PLLCLK       1                                 /*!< SDIO clock source switches to PLL clock */
#define __HAL_RCC_SET_SDIO_CLKSEL(x) (SN_SCU->CLKSEL_b.SDIOCLKSEL = x) /*!< Set SDIO clock source as RCC_SDIO_CLKSEL_xxx */
#define __HAL_RCC_GET_SDIO_CLKSEL()  (SN_SCU->CLKSEL_b.SDIOCLKSEL)     /*!< Get SDIO clock source */

#define RCC_LCD_CLKSEL_HCLK         0                                /*!< LCD clock source switches to HCLK */
#define RCC_LCD_CLKSEL_PLLCLK       1                                /*!< LCD clock source switches to PLL clock */
#define __HAL_RCC_SET_LCD_CLKSEL(x) (SN_SCU->CLKSEL_b.LCDCLKSEL = x) /*!< Set LCD clock source as RCC_LCD_CLKSEL_xxx */
#define __HAL_RCC_GET_LCD_CLKSEL()  (SN_SCU->CLKSEL_b.LCDCLKSEL)     /*!< Get LCD clock source */

#define RCC_WWDT_CLKSEL_APB0CLK      0                                 /*!< WWD clock source switches to APB0 clock */
#define RCC_WWDT_CLKSEL_ILRC         1                                 /*!< WWD clock source switches to ILRC */
#define __HAL_RCC_SET_WWDT_CLKSEL(x) (SN_SCU->CLKSEL_b.WWDTCLKSEL = x) /*!< Set WWDT clock source as RCC_WWDT_CLKSEL_xxx */
#define __HAL_RCC_GET_WWDT_CLKSEL()  (SN_SCU->CLKSEL_b.WWDTCLKSEL)     /*!< Get WWDT clock source */

#define RCC_LCM_CLKSEL_HCLK         0                                /*!< LCM clock source switches to HCLK */
#define RCC_LCM_CLKSEL_PLLCLK       1                                /*!< LCM clock source switches to PLL clock */
#define __HAL_RCC_SET_LCM_CLKSEL(x) (SN_SCU->CLKSEL_b.LCMCLKSEL = x) /*!< Set LCM clock source as RCC_LCD_CLKSEL_xxx */
#define __HAL_RCC_GET_LCM_CLKSEL()  (SN_SCU->CLKSEL_b.LCMCLKSEL)     /*!< Get LCM clock source */
/**
 * @}
 */

/** \defgroup rcc_ahb_peri_rst AHB Peripheral Reset
 * \brief  To trigger the AHB Peripheral reset.
 * \ingroup rcc_api_define
 * \note
 * \{
 */
#define __HAL_RCC_DMA0_RESET()     (SN_SCU->AHBRST = 0x00000001) /*!< DMA0 rest control */
#define __HAL_RCC_DMA1_RESET()     (SN_SCU->AHBRST = 0x00000002) /*!< DMA1 rest control */
#define __HAL_RCC_SDIO_RESET()     (SN_SCU->AHBRST = 0x00000040) /*!< SDIO rest control */
#define __HAL_RCC_LCD_RESET()      (SN_SCU->AHBRST = 0x00000080) /*!< LCD rest control */
#define __HAL_RCC_ETHERNET_RESET() (SN_SCU->AHBRST = 0x00000100) /*!< Ethernet MAC rest control */
#define __HAL_RCC_USBHS_RESET()    (SN_SCU->AHBRST = 0x00000200) /*!< USB HS rest control */
/**
 * @}
 */

/** \defgroup rcc_apb0_peri_rst APB0 Peripheral Reset
 * \brief  To trigger the APB0 Peripheral reset.
 * \ingroup rcc_api_define
 * \note
 * \{
 */
#define __HAL_RCC_WDT_RESET()    (SN_SCU->APB0RST = 0x00000001) /*!< WDT rest control */
#define __HAL_RCC_WWDT_RESET()   (SN_SCU->APB0RST = 0x00000002) /*!< WWDT rest control */
#define __HAL_RCC_CAN0_RESET()   (SN_SCU->APB0RST = 0x00000004) /*!< CAN0 rest control */
#define __HAL_RCC_CAN1_RESET()   (SN_SCU->APB0RST = 0x00000008) /*!< CAN1 rest control */
#define __HAL_RCC_UART1_RESET()  (SN_SCU->APB0RST = 0x00000010) /*!< UART1 rest control */
#define __HAL_RCC_UART2_RESET()  (SN_SCU->APB0RST = 0x00000020) /*!< UART2 rest control */
#define __HAL_RCC_UART3_RESET()  (SN_SCU->APB0RST = 0x00000040) /*!< UART3 rest control */
#define __HAL_RCC_UART4_RESET()  (SN_SCU->APB0RST = 0x00000080) /*!< UART4 rest control */
#define __HAL_RCC_CT16B6_RESET() (SN_SCU->APB0RST = 0x00000100) /*!< CT16B6 rest control */
#define __HAL_RCC_CT16B7_RESET() (SN_SCU->APB0RST = 0x00000200) /*!< CT16B7 rest control */

/**
 * @}
 */

/** \defgroup rcc_apb1_peri_rst APB1 Peripheral Reset
 * \brief  To trigger the APB1 Peripheral reset.
 * \ingroup rcc_api_define
 * \note
 * \{
 */
#define __HAL_RCC_ADC0_RESET()   (SN_SCU->APB1RST = 0x00000001) /*!< ADC0 rest control */
#define __HAL_RCC_SSP0_RESET()   (SN_SCU->APB1RST = 0x00000002) /*!< SSP0 rest control */
#define __HAL_RCC_SSP1_RESET()   (SN_SCU->APB1RST = 0x00000004) /*!< SSP1 rest control */
#define __HAL_RCC_SSP2_RESET()   (SN_SCU->APB1RST = 0x00000008) /*!< SSP2 rest control */
#define __HAL_RCC_UART0_RESET()  (SN_SCU->APB1RST = 0x00000010) /*!< UART0 rest control */
#define __HAL_RCC_UART5_RESET()  (SN_SCU->APB1RST = 0x00000020) /*!< UART5 rest control */
#define __HAL_RCC_I2C0_RESET()   (SN_SCU->APB1RST = 0x00000040) /*!< I2C0 rest control */
#define __HAL_RCC_I2C1_RESET()   (SN_SCU->APB1RST = 0x00000080) /*!< I2C1 rest control */
#define __HAL_RCC_I2C2_RESET()   (SN_SCU->APB1RST = 0x00000100) /*!< I2C2 rest control */
#define __HAL_RCC_CT16B0_RESET() (SN_SCU->APB1RST = 0x00000200) /*!< CT16B0 rest control */
#define __HAL_RCC_CT16B1_RESET() (SN_SCU->APB1RST = 0x00000400) /*!< CT16B1 rest control */
#define __HAL_RCC_CT16B2_RESET() (SN_SCU->APB1RST = 0x00000800) /*!< CT16B2 rest control */
#define __HAL_RCC_CT16B3_RESET() (SN_SCU->APB1RST = 0x00001000) /*!< CT16B3 rest control */
#define __HAL_RCC_CT16B4_RESET() (SN_SCU->APB1RST = 0x00002000) /*!< CT16B4 rest control */
#define __HAL_RCC_CT16B5_RESET() (SN_SCU->APB1RST = 0x00004000) /*!< CT16B5 rest control */
#define __HAL_RCC_CT16B8_RESET() (SN_SCU->APB1RST = 0x00008000) /*!< CT16B8 rest control */
#define __HAL_RCC_CRC_RESET()    (SN_SCU->APB1RST = 0x00010000) /*!< CRC rest control */
#define __HAL_RCC_GPIO0_RESET()  (SN_SCU->APB1RST = 0x00020000) /*!< GPIO0 rest control */
#define __HAL_RCC_GPIO1_RESET()  (SN_SCU->APB1RST = 0x00040000) /*!< GPIO1 rest control */
#define __HAL_RCC_GPIO2_RESET()  (SN_SCU->APB1RST = 0x00080000) /*!< GPIO2 rest control */
#define __HAL_RCC_GPIO3_RESET()  (SN_SCU->APB1RST = 0x00100000) /*!< GPIO3 rest control */
#define __HAL_RCC_LCM_RESET()    (SN_SCU->APB1RST = 0x00200000) /*!< LCM rest control */

/**
 * @}
 */

/** \defgroup rcc_ahb_peri_rst_msk AHB Peripheral Reset Mask Unmask
 * \brief  Mask or Unmask the AHB peripheral reset.
 * \ingroup rcc_api_define
 * \note
 * \{
 */
#define __HAL_RCC_DMA0_RESET_MASK()     (SN_SCU->AHBRSTMSK_b.DMA0RSTMSK = ENABLE)  /*!< Mask PERRST for DMA0 register */
#define __HAL_RCC_DMA1_RESET_MASK()     (SN_SCU->AHBRSTMSK_b.DMA1RSTMSK = ENABLE)  /*!< Mask PERRST for DMA1 register */
#define __HAL_RCC_SDIO_RESET_MASK()     (SN_SCU->AHBRSTMSK_b.SDIORSTMSK = ENABLE)  /*!< Mask PERRST for SDIO register */
#define __HAL_RCC_LCD_RESET_MASK()      (SN_SCU->AHBRSTMSK_b.LCDRSTMSK = ENABLE)   /*!< Mask PERRST for LCD register */
#define __HAL_RCC_ETHERNET_RESET_MASK() (SN_SCU->AHBRSTMSK_b.ETHRSTMSK = ENABLE)   /*!< Mask PERRST for Ethernet MAC register */
#define __HAL_RCC_USBHS_RESET_MASK()    (SN_SCU->AHBRSTMSK_b.USBHSRSTMSK = ENABLE) /*!< Mask PERRST for USB HS controller and PHY register */

#define __HAL_RCC_DMA0_RESET_UNMASK()     (SN_SCU->AHBRSTMSK_b.DMA0RSTMSK = DISABLE)  /*!< No mask PERRST for DMA0 register */
#define __HAL_RCC_DMA1_RESET_UNMASK()     (SN_SCU->AHBRSTMSK_b.DMA1RSTMSK = DISABLE)  /*!< No mask PERRST for DMA1 register */
#define __HAL_RCC_SDIO_RESET_UNMASK()     (SN_SCU->AHBRSTMSK_b.SDIORSTMSK = DISABLE)  /*!< No mask PERRST for SDIO register */
#define __HAL_RCC_LCD_RESET_UNMASK()      (SN_SCU->AHBRSTMSK_b.LCDRSTMSK = DISABLE)   /*!< No mask PERRST for LCD register */
#define __HAL_RCC_ETHERNET_RESET_UNMASK() (SN_SCU->AHBRSTMSK_b.ETHRSTMSK = DISABLE)   /*!< No mask PERRST for Ethernet MAC register */
#define __HAL_RCC_USBHS_RESET_UNMASK()    (SN_SCU->AHBRSTMSK_b.USBHSRSTMSK = DISABLE) /*!< No mask PERRST for USB HS controller and PHY register */
/**
 * @}
 */

/** \defgroup rcc_apb0_peri_rst_msk APB0 Peripheral Reset Mask Unmask
 * \brief  Mask or Unmask the APB0 peripheral reset.
 * \ingroup rcc_api_define
 * \note
 * \{
 */
#define __HAL_RCC_WDT_RESET_MASK()    (SN_SCU->APB0RSTMSK_b.WDTRSTMSK = ENABLE)    /*!< Mask PERRST for WDT register */
#define __HAL_RCC_WWDT_RESET_MASK()   (SN_SCU->APB0RSTMSK_b.WWDTRSTMSK = ENABLE)   /*!< Mask PERRST for WWDT register */
#define __HAL_RCC_CAN0_RESET_MASK()   (SN_SCU->APB0RSTMSK_b.CAN0RSTMSK = ENABLE)   /*!< Mask PERRST for CAN0 register */
#define __HAL_RCC_CAN1_RESET_MASK()   (SN_SCU->APB0RSTMSK_b.CAN1RSTMSK = ENABLE)   /*!< Mask PERRST for CAN1 register */
#define __HAL_RCC_UART1_RESET_MASK()  (SN_SCU->APB0RSTMSK_b.UART1RSTMSK = ENABLE)  /*!< Mask PERRST for UART1 register */
#define __HAL_RCC_UART2_RESET_MASK()  (SN_SCU->APB0RSTMSK_b.UART2RSTMSK = ENABLE)  /*!< Mask PERRST for UART2 register */
#define __HAL_RCC_UART3_RESET_MASK()  (SN_SCU->APB0RSTMSK_b.UART3RSTMSK = ENABLE)  /*!< Mask PERRST for UART3 register */
#define __HAL_RCC_UART4_RESET_MASK()  (SN_SCU->APB0RSTMSK_b.UART4RSTMSK = ENABLE)  /*!< Mask PERRST for UART4 register */
#define __HAL_RCC_CT16B6_RESET_MASK() (SN_SCU->APB0RSTMSK_b.CT16B6RSTMSK = ENABLE) /*!< Mask PERRST for CT16B6 register */
#define __HAL_RCC_CT16B7_RESET_MASK() (SN_SCU->APB0RSTMSK_b.CT16B7RSTMSK = ENABLE) /*!< Mask PERRST for CT16B7 register */

#define __HAL_RCC_WDT_RESET_UNMASK()    (SN_SCU->APB0RSTMSK_b.WDTRSTMSK = DISABLE)    /*!< No mask PERRST for WDT register */
#define __HAL_RCC_WWDT_RESET_UNMASK()   (SN_SCU->APB0RSTMSK_b.WWDTRSTMSK = DISABLE)   /*!< No mask PERRST for WWDT register */
#define __HAL_RCC_CAN0_RESET_UNMASK()   (SN_SCU->APB0RSTMSK_b.CAN0RSTMSK = DISABLE)   /*!< No mask PERRST for CAN0 register */
#define __HAL_RCC_CAN1_RESET_UNMASK()   (SN_SCU->APB0RSTMSK_b.CAN1RSTMSK = DISABLE)   /*!< No mask PERRST for CAN1 register */
#define __HAL_RCC_UART1_RESET_UNMASK()  (SN_SCU->APB0RSTMSK_b.UART1RSTMSK = DISABLE)  /*!< No mask PERRST for UART1 register */
#define __HAL_RCC_UART2_RESET_UNMASK()  (SN_SCU->APB0RSTMSK_b.UART2RSTMSK = DISABLE)  /*!< No mask PERRST for UART2 register */
#define __HAL_RCC_UART3_RESET_UNMASK()  (SN_SCU->APB0RSTMSK_b.UART3RSTMSK = DISABLE)  /*!< No mask PERRST for UART3 register */
#define __HAL_RCC_UART4_RESET_UNMASK()  (SN_SCU->APB0RSTMSK_b.UART4RSTMSK = DISABLE)  /*!< No mask PERRST for UART4 register */
#define __HAL_RCC_CT16B6_RESET_UNMASK() (SN_SCU->APB0RSTMSK_b.CT16B6RSTMSK = DISABLE) /*!< No mask PERRST for CT16B6 register */
#define __HAL_RCC_CT16B7_RESET_UNMASK() (SN_SCU->APB0RSTMSK_b.CT16B7RSTMSK = DISABLE) /*!< No mask PERRST for CT16B7 register */
/**
 * @}
 */

/** \defgroup rcc_apb1_peri_rst_msk APB1 Peripheral Reset Mask Unmask
 * \brief  Mask or Unmask the APB1 peripheral reset.
 * \ingroup rcc_api_define
 * \note
 * \{
 */
#define __HAL_RCC_ADC0_RESET_MASK()   (SN_SCU->APB1RSTMSK_b.ADC0RSTMSK = ENABLE)   /*!< Mask PERRST for ADC0 register */
#define __HAL_RCC_SSP0_RESET_MASK()   (SN_SCU->APB1RSTMSK_b.SSP0RSTMSK = ENABLE)   /*!< Mask PERRST for SSP0 register */
#define __HAL_RCC_SSP1_RESET_MASK()   (SN_SCU->APB1RSTMSK_b.SSP1RSTMSK = ENABLE)   /*!< Mask PERRST for SSP1 register */
#define __HAL_RCC_SSP2_RESET_MASK()   (SN_SCU->APB1RSTMSK_b.SSP2RSTMSK = ENABLE)   /*!< Mask PERRST for SSP2 register */
#define __HAL_RCC_UART0_RESET_MASK()  (SN_SCU->APB1RSTMSK_b.UART0RSTMSK = ENABLE)  /*!< Mask PERRST for UART0 register */
#define __HAL_RCC_UART5_RESET_MASK()  (SN_SCU->APB1RSTMSK_b.UART5RSTMSK = ENABLE)  /*!< Mask PERRST for UART5 register */
#define __HAL_RCC_I2C0_RESET_MASK()   (SN_SCU->APB1RSTMSK_b.I2C0RSTMSK = ENABLE)   /*!< Mask PERRST for I2C0 register */
#define __HAL_RCC_I2C1_RESET_MASK()   (SN_SCU->APB1RSTMSK_b.I2C1RSTMSK = ENABLE)   /*!< Mask PERRST for I2C1 register */
#define __HAL_RCC_I2C2_RESET_MASK()   (SN_SCU->APB1RSTMSK_b.I2C2RSTMSK = ENABLE)   /*!< Mask PERRST for I2C2 register */
#define __HAL_RCC_CT16B0_RESET_MASK() (SN_SCU->APB1RSTMSK_b.CT16B0RSTMSK = ENABLE) /*!< Mask PERRST for CT16B0 register */
#define __HAL_RCC_CT16B1_RESET_MASK() (SN_SCU->APB1RSTMSK_b.CT16B1RSTMSK = ENABLE) /*!< Mask PERRST for CT16B1 register */
#define __HAL_RCC_CT16B2_RESET_MASK() (SN_SCU->APB1RSTMSK_b.CT16B2RSTMSK = ENABLE) /*!< Mask PERRST for CT16B2 register */
#define __HAL_RCC_CT16B3_RESET_MASK() (SN_SCU->APB1RSTMSK_b.CT16B3RSTMSK = ENABLE) /*!< Mask PERRST for CT16B3 register */
#define __HAL_RCC_CT16B4_RESET_MASK() (SN_SCU->APB1RSTMSK_b.CT16B4RSTMSK = ENABLE) /*!< Mask PERRST for CT16B4 register */
#define __HAL_RCC_CT16B5_RESET_MASK() (SN_SCU->APB1RSTMSK_b.CT16B5RSTMSK = ENABLE) /*!< Mask PERRST for CT16B5 register */
#define __HAL_RCC_CT16B8_RESET_MASK() (SN_SCU->APB1RSTMSK_b.CT16B8RSTMSK = ENABLE) /*!< Mask PERRST for CT16B8 register */
#define __HAL_RCC_CRC_RESET_MASK()    (SN_SCU->APB1RSTMSK_b.CRCRSTMSK = ENABLE)    /*!< Mask PERRST for CRC register */
#define __HAL_RCC_GPIO0_RESET_MASK()  (SN_SCU->APB1RSTMSK_b.GPIO0RSTMSK = ENABLE)  /*!< Mask PERRST for GPIO0 register */
#define __HAL_RCC_GPIO1_RESET_MASK()  (SN_SCU->APB1RSTMSK_b.GPIO1RSTMSK = ENABLE)  /*!< Mask PERRST for GPIO1 register */
#define __HAL_RCC_GPIO2_RESET_MASK()  (SN_SCU->APB1RSTMSK_b.GPIO2RSTMSK = ENABLE)  /*!< Mask PERRST for GPIO2 register */
#define __HAL_RCC_GPIO3_RESET_MASK()  (SN_SCU->APB1RSTMSK_b.GPIO3RSTMSK = ENABLE)  /*!< Mask PERRST for GPIO3 register */
#define __HAL_RCC_LCM_RESET_MASK()    (SN_SCU->APB1RSTMSK_b.LCMRSTMSK = ENABLE)    /*!< Mask PERRST for LCM register */

#define __HAL_RCC_ADC0_RESET_UNMASK()   (SN_SCU->APB1RSTMSK_b.ADC0RSTMSK = DISABLE)   /*!< No mask PERRST for ADC0 register */
#define __HAL_RCC_SSP0_RESET_UNMASK()   (SN_SCU->APB1RSTMSK_b.SSP0RSTMSK = DISABLE)   /*!< No mask PERRST for SSP0 register */
#define __HAL_RCC_SSP1_RESET_UNMASK()   (SN_SCU->APB1RSTMSK_b.SSP1RSTMSK = DISABLE)   /*!< No mask PERRST for SSP1 register */
#define __HAL_RCC_SSP2_RESET_UNMASK()   (SN_SCU->APB1RSTMSK_b.SSP2RSTMSK = DISABLE)   /*!< No mask PERRST for SSP2 register */
#define __HAL_RCC_UART0_RESET_UNMASK()  (SN_SCU->APB1RSTMSK_b.UART0RSTMSK = DISABLE)  /*!< No mask PERRST for UART0 register */
#define __HAL_RCC_UART5_RESET_UNMASK()  (SN_SCU->APB1RSTMSK_b.UART5RSTMSK = DISABLE)  /*!< No mask PERRST for UART5 register */
#define __HAL_RCC_I2C0_RESET_UNMASK()   (SN_SCU->APB1RSTMSK_b.I2C0RSTMSK = DISABLE)   /*!< No mask PERRST for I2C0 register */
#define __HAL_RCC_I2C1_RESET_UNMASK()   (SN_SCU->APB1RSTMSK_b.I2C1RSTMSK = DISABLE)   /*!< No mask PERRST for I2C1 register */
#define __HAL_RCC_I2C2_RESET_UNMASK()   (SN_SCU->APB1RSTMSK_b.I2C2RSTMSK = DISABLE)   /*!< No mask PERRST for I2C2 register */
#define __HAL_RCC_CT16B0_RESET_UNMASK() (SN_SCU->APB1RSTMSK_b.CT16B0RSTMSK = DISABLE) /*!< No mask PERRST for CT16B0 register */
#define __HAL_RCC_CT16B1_RESET_UNMASK() (SN_SCU->APB1RSTMSK_b.CT16B1RSTMSK = DISABLE) /*!< No mask PERRST for CT16B1 register */
#define __HAL_RCC_CT16B2_RESET_UNMASK() (SN_SCU->APB1RSTMSK_b.CT16B2RSTMSK = DISABLE) /*!< No mask PERRST for CT16B2 register */
#define __HAL_RCC_CT16B3_RESET_UNMASK() (SN_SCU->APB1RSTMSK_b.CT16B3RSTMSK = DISABLE) /*!< No mask PERRST for CT16B3 register */
#define __HAL_RCC_CT16B4_RESET_UNMASK() (SN_SCU->APB1RSTMSK_b.CT16B4RSTMSK = DISABLE) /*!< No mask PERRST for CT16B4 register */
#define __HAL_RCC_CT16B5_RESET_UNMASK() (SN_SCU->APB1RSTMSK_b.CT16B5RSTMSK = DISABLE) /*!< No mask PERRST for CT16B5 register */
#define __HAL_RCC_CT16B8_RESET_UNMASK() (SN_SCU->APB1RSTMSK_b.CT16B8RSTMSK = DISABLE) /*!< No mask PERRST for CT16B8 register */
#define __HAL_RCC_CRC_RESET_UNMASK()    (SN_SCU->APB1RSTMSK_b.CRCRSTMSK = DISABLE)    /*!< No mask PERRST for CRC register */
#define __HAL_RCC_GPIO0_RESET_UNMASK()  (SN_SCU->APB1RSTMSK_b.GPIO0RSTMSK = DISABLE)  /*!< No mask PERRST for GPIO0 register */
#define __HAL_RCC_GPIO1_RESET_UNMASK()  (SN_SCU->APB1RSTMSK_b.GPIO1RSTMSK = DISABLE)  /*!< No mask PERRST for GPIO1 register */
#define __HAL_RCC_GPIO2_RESET_UNMASK()  (SN_SCU->APB1RSTMSK_b.GPIO2RSTMSK = DISABLE)  /*!< No mask PERRST for GPIO2 register */
#define __HAL_RCC_GPIO3_RESET_UNMASK()  (SN_SCU->APB1RSTMSK_b.GPIO3RSTMSK = DISABLE)  /*!< No mask PERRST for GPIO3 register */
#define __HAL_RCC_LCM_RESET_UNMASK()    (SN_SCU->APB1RSTMSK_b.LCMRSTMSK = DISABLE)    /*!< No mask PERRST for LCM register */
/**
 * @}
 */

/** \defgroup rcc_remap_code Code remap control
 * \brief Code remap control.
 * \ingroup rcc_api_define
 * \note
 * \{
 */
#define RCC_REMAP_BOOT_CODE             0                                   /*!< Remapping to boot code */
#define RCC_REMAP_USER_CODE             1                                   /*!< Remapping to user code */
#define __HAL_RCC_SET_REMAP_BOOT_CODE() (SN_SCU->PWRMODE_b.REMAP = DISABLE) /*!< Set Remapping to boot code */
#define __HAL_RCC_SET_REMAP_USER_CODE() (SN_SCU->PWRMODE_b.REMAP = ENABLE)  /*!< Set remapping to user code */
#define __HAL_RCC_GET_REMAP()           (SN_SCU->PWRMODE_b.REMAP)           /*!< Get remapping value, the value is any of RCC_REMAP_xxx_CODE */
/**
 * @}
 */

#define __HAL_RCC_REBOOT() (SN_SCU->PWRMODE_b.REBOOT = ENABLE) /*!< Reboot control */

#define RCC_IHRC_TIMEOUT         100 /* ms (256*F_IHRC)    */ /*!< IHRC ready timeout(ms) */
#define RCC_EHS_TIMEOUT          400 /* ms (32768*F_EHS)   */ /*!< EHS ready timeout(ms) */
#define RCC_ELS_TIMEOUT          800 /* ms (32K*F_ELS)     */ /*!< ELS ready timeout(ms) */
#define RCC_CLOCK_SWITCH_TIMEOUT 100 /* ms                 */ /*!< Clock switch complete timeout(ms) */
#define RCC_PERRST_TIMEOUT       100                          /*!< Peripherals reset ready timeout(ms) */

// #define __HAL_RCC_IHRC_ENABLE()                         (SN_SCU->IHRCCTRL_b.IHRCEN = DISABLE)

/** \defgroup RCC_OSC_Type Oscillator Configuration Type
 * \ingroup rcc_control
 * \note
 * \{
 */
#define RCC_OSC_TYPE_NONE 0x00000000U /*!< No Configure any Oscillator */
#define RCC_OSC_TYPE_ELS  0x00000001U /*!< To Configure ELS */
#define RCC_OSC_TYPE_EHS  0x00000002U /*!< To Configure EHS */
#define RCC_OSC_TYPE_IHRC 0x00000004U /*!< To Configure IHRC */
// #define RCC_OSC_TYPE_ILRC                   0x00000008U
/**
 * @}
 */

/** \defgroup RCC_OSC_CLK_Config OSC/Clock Config
 * \ingroup rcc_control
 * \note
 * \{
 */
#define RCC_OSCCLK_CFG_OFF DISABLE /*!< Enable oscillator */
#define RCC_OSCCLK_CFG_ON  ENABLE  /*!< Disable oscillator */
/**
 * @}
 */

/** \defgroup RCC_PLL_Clock_Source PLL Clock Source
 * \ingroup rcc_control
 * \note
 * \{
 */
#define RCC_PLL_SRC_IHRC 0 /*!< To select IHRC as PLL clock source */
#define RCC_PLL_SRC_EHS  1 /*!< To select EHS as PLL clock source */
/**
 * @}
 */

/** \defgroup RCC_PLL_Clock_Divider PLL Clock Divider
 * \ingroup rcc_control
 * \note
 * \{
 */
#define RCC_PLL_DIV32 0 /*!< PLL CLKOUT = PLLCLK / 32 */
#define RCC_PLL_DIV16 1 /*!< PLL CLKOUT = PLLCLK / 16 */
#define RCC_PLL_DIV8  2 /*!< PLL CLKOUT = PLLCLK / 8 */
#define RCC_PLL_DIV4  3 /*!< PLL CLKOUT = PLLCLK / 4 */
/**
 * @}
 */

/** \defgroup RCC_CLK_Type Clock Configuration Type
 * \ingroup rcc_control
 * \note
 * \{
 */
#define RCC_CLK_TYPE_SYSCLK  0x00000001U /*!< To Configure system clock */
#define RCC_CLK_TYPE_HCLK    0x00000002U /*!< To Configure AHB clock */
#define RCC_CLK_TYPE_APB0CLK 0x00000004U /*!< To Configure APB0 clock */
#define RCC_CLK_TYPE_APB1CLK 0x00000008U /*!< To Configure APB1 clock */
/**
 * @}
 */

/** \defgroup RCC_SYSCLK_Clock_Source System Clock Source
 * \ingroup rcc_control
 * \note
 * \{
 */
#define RCC_SYSCLK_SRC_IHRC 0 /*!< To select IHRC as system clock source */
#define RCC_SYSCLK_SRC_EHS  2 /*!< To select EHS as system clock source */
#define RCC_SYSCLK_SRC_PLL  4 /*!< To select PLL as system clock source */
/**
 * @}
 */

/** \defgroup RCC_HCLK_Divider AHB clock(HCLK) prescaler
 * \ingroup rcc_control
 * \note
 * \{
 */
#define RCC_SYSCLK_DIV1   0 /*!< HCLK = SYSCLK / 1 */
#define RCC_SYSCLK_DIV2   1 /*!< HCLK = SYSCLK / 2 */
#define RCC_SYSCLK_DIV4   2 /*!< HCLK = SYSCLK / 4 */
#define RCC_SYSCLK_DIV8   3 /*!< HCLK = SYSCLK / 8 */
#define RCC_SYSCLK_DIV16  4 /*!< HCLK = SYSCLK / 16 */
#define RCC_SYSCLK_DIV32  5 /*!< HCLK = SYSCLK / 32 */
#define RCC_SYSCLK_DIV64  6 /*!< HCLK = SYSCLK / 64 */
#define RCC_SYSCLK_DIV128 7 /*!< HCLK = SYSCLK / 128 */
/**
 * @}
 */

/** \defgroup RCC_APB0_APB1_CLK_Divider  APB0/APB1 clock prescaler
 * \ingroup rcc_control
 * \note
 * \{
 */
#define RCC_HCLK_DIV1   0 /*!< APBCLK = HCLK / 1 */
#define RCC_HCLK_DIV2   1 /*!< APBCLK = HCLK / 2 */
#define RCC_HCLK_DIV4   2 /*!< APBCLK = HCLK / 4 */
#define RCC_HCLK_DIV8   3 /*!< APBCLK = HCLK / 8 */
#define RCC_HCLK_DIV16  4 /*!< APBCLK = HCLK / 16 */
#define RCC_HCLK_DIV32  5 /*!< APBCLK = HCLK / 32 */
#define RCC_HCLK_DIV64  6 /*!< APBCLK = HCLK / 64 */
#define RCC_HCLK_DIV128 7 /*!< APBCLK = HCLK / 128 */
/**
 * @}
 */

/** \defgroup RCC_CLKOUT_Source Clock Output source
 * \ingroup rcc_control
 * \note
 * \{
 */
#define RCC_CLKOUT_SRC_DISABLE 0 /*!< Disable clock out source */
#define RCC_CLKOUT_SRC_ILRC    1 /*!< To select ILRC as clock out source */
#define RCC_CLKOUT_SRC_ELS     2 /*!< To select ELS as clock out source */
#define RCC_CLKOUT_SRC_HCLK    4 /*!< To select HCLK as clock out source */
#define RCC_CLKOUT_SRC_IHRC    5 /*!< To select IHRC as clock out source */
#define RCC_CLKOUT_SRC_EHS     6 /*!< To select EHS as clock out source */
#define RCC_CLKOUT_SRC_PLL     7 /*!< To select PLL as clock out source */
/**
 * @}
 */

/** \defgroup RCC_Clkout_Divider Clock Output prescaler
 * \ingroup rcc_control
 * \note
 * \{
 */
#define RCC_CLKOUT_PRE_DIV_1   0 /*!< CLKOUT = SRC / 1 */
#define RCC_CLKOUT_PRE_DIV_2   1 /*!< CLKOUT = SRC / 2 */
#define RCC_CLKOUT_PRE_DIV_3   2 /*!< CLKOUT = SRC / 3 */
#define RCC_CLKOUT_PRE_DIV_4   3 /*!< CLKOUT = SRC / 4 */
#define RCC_CLKOUT_PRE_DIV_6   4 /*!< CLKOUT = SRC / 6 */
#define RCC_CLKOUT_PRE_DIV_8   5 /*!< CLKOUT = SRC / 8 */
#define RCC_CLKOUT_PRE_DIV_16  6 /*!< CLKOUT = SRC / 16 */
#define RCC_CLKOUT_PRE_DIV_32  7 /*!< CLKOUT = SRC / 32 */
#define RCC_CLKOUT_PRE_DIV_64  8 /*!< CLKOUT = SRC / 64 */
#define RCC_CLKOUT_PRE_DIV_128 9 /*!< CLKOUT = SRC / 128 */
/**
 * @}
 */

/** \defgroup RCC_Clkout_Pin Clock Output pin
 * \ingroup rcc_control
 * \note
 * \{
 */
#define RCC_CLKOUT_PIN_1 0 /*!< Clock out pin sets to P3.0  */
#define RCC_CLKOUT_PIN_2 1 /*!< Clock out pin sets to P0.12  */
#define RCC_CLKOUT_PIN_3 2 /*!< Clock out pin sets to P0.10  */
                           /**
                            * @}
                            */

/**
 * \brief PLL clock configuration
 * \ingroup rcc_struct_define
 */
typedef struct
{
    uint32_t PLLEn;     /*!< The PLL Configures Enable or Disable.
                            This parameter can be a value of @ref RCC_OSC_CLK_Config  */
    uint32_t PLLSource; /*!< PLL entry clock source.
                            This parameter must be a value of @ref RCC_PLL_Clock_Source */
    uint32_t NS;        /*!< programmable loop divider. PLLCLK = FREF * NS /FS
                            This parameter can be a number between Min_Data = 6 and Max_Data = 80 */
    uint32_t FS;        /*!< PLL frequency range output divider.
                            This parameter must be a value of @ref RCC_PLL_Clock_Divider */
} RCC_PLLConfig_t;

/**
 * \brief Oscillators configuration
 * \ingroup rcc_struct_define
 */
typedef struct
{
    uint32_t OscillatorType; /*!< The oscillators to be configured.
                               This parameter can be a value of @ref RCC_OSC_Type */
    uint32_t IHRCEn;         /*!< The IHRC Configures Enable or Disable.
                               This parameter can be a value of @ref RCC_OSC_CLK_Config */
    uint32_t ELSEn;          /*!< The ELS Configures Enable or Disable.
                               This parameter can be a value of @ref RCC_OSC_CLK_Config */
    uint32_t EHSEn;          /*!< The EHS Configures Enable or Disable.
                               This parameter can be a value of @ref RCC_OSC_CLK_Config */
    RCC_PLLConfig_t PLL;     /*!< PLL structure parameters */

} RCC_OscConfig_t;

/**
 * \brief Clock configuration
 * \ingroup rcc_struct_define
 */
typedef struct
{
    uint32_t ClockType;      /*!< The clock to be configured.
                                  This parameter can be a value of @ref RCC_CLK_Type */
    uint32_t SYSCLKSource;   /*!< The clock source (SYSCLKS) used as system clock.
                                This parameter can be a value of @ref RCC_SYSCLK_Clock_Source */
    uint32_t AHBCLKDivider;  /*!< The AHB clock (HCLK) divider. This clock is derived from the system clock (SYSCLK).
                                This parameter can be a value of @ref RCC_HCLK_Divider */
    uint32_t APB0CLKDivider; /*!< The APB0 clock (APB0CLK) divider. This clock is derived from the AHB clock (HCLK).
                                This parameter can be a value of @ref RCC_APB0_APB1_CLK_Divider */
    uint32_t APB1CLKDivider; /*!< The APB1 clock (APB1CLK) divider. This clock is derived from the AHB clock (HCLK).
                                This parameter can be a value of @ref RCC_APB0_APB1_CLK_Divider */
} RCC_ClkConfig_t;

/**
 * \brief LVD reset/interupt level
 * \ingroup rcc_struct_define
 */
typedef enum
{
    RVD_RESET_INT_LVL_2P4V = 0, /*!< LVD reset/interrupt  Level is 2.40 V */
    RVD_RESET_INT_LVL_2P6V = 1, /*!< LVD reset/interrupt  Level is 2.60 V */
    RVD_RESET_INT_LVL_2P8V = 2, /*!< LVD reset/interrupt  Level is 2.80 V */
    RVD_RESET_INT_LVL_3P0V = 3, /*!< LVD reset/interrupt  Level is 3.00 V */
} LVD_RstInt_Level_t;

uint32_t HAL_RCC_OscConfig(RCC_OscConfig_t *OscCfg);
uint32_t HAL_RCC_ClockConfig(RCC_ClkConfig_t *ClkCfg);
void     HAL_RCC_GetOscConfig(RCC_OscConfig_t *OscCfg);
void     HAL_RCC_GetClockConfig(RCC_ClkConfig_t *ClkCfg);
uint32_t HAL_RCC_GetPLLCLKFreq(void);
uint32_t HAL_RCC_GetSysClockFreq(void);
uint32_t HAL_RCC_GetHCLKFreq(void);
uint32_t HAL_RCC_GetAPB0CLKFreq(void);
uint32_t HAL_RCC_GetAPB1CLKFreq(void);
void     HAL_RCC_ClockoutConfig(uint32_t Src, uint32_t Prescale, uint32_t Clockout_Pin);
uint32_t HAL_RCC_PeripheralReset(uint16_t Delay, uint16_t Active);
void     HAL_RCC_PeripheralResetMaskClear(void);
void     HAL_LVD_Callback(void);
void     HAL_LVD_IRQHandler(void);
void     LVD_Control(uint8_t LvdEn, uint8_t RstEn, LVD_RstInt_Level_t Lvl);
#ifdef __cplusplus
}
#endif

#endif /* _HAL_RCC_H_ */
