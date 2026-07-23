/***********************
 * SN34F78X SDK V2.1.4 *
 ***********************/

#ifndef _HAL_GPIO_EX_H_
#define _HAL_GPIO_EX_H_

#ifdef __cplusplus
extern "C" {
#endif

/**
 * ————————————————————————————————————————————————————————————————————————————————————————————————————————————————————————————————————————————————————————————————————————————————————
 * peripheral  |   pin name            |   PA0     |   PA1     |   PA2     |   PA3     |   PA4     |   PA5     |   PA6     |   PA7     |   PA8     |   PA9     |   PA10    |   PA11   |
 * ————————————————————————————————————————————————————————————————————————————————————————————————————————————————————————————————————————————————————————————————————————————————————
 *             |   URXD0 / IRDA_RXDL0  |   P0.11   |   P2.0    |   P3.2    |   P1.0    |
 *   UART0     |   UTXD0 / IRDA_TXD0   |   P0.10   |   P2.1    |   P3.1    |   P1.8    |
 *             |   UCTS0               |   P3.3    |
 *             |   URTS0               |   P3.4    |
 * —————————————————————————————————————————————————————————————————————————————————————————————————
 *             |   URXD1 / IRDA_RXDL1  |   P1.8    |   P2.3    |   ----    |   P3.4    |   P2.8    |
 *   UART1     |   UTXD1 / IRDA_TXD1   |   P1.9    |   P2.2    |   ----    |   P3.6    |   P3.0    |
 *             |   UCTS1               |   P2.0    |
 *             |   URTS1               |   P2.1    |
 * —————————————————————————————————————————————————————————————————————————————————————————————————
 *             |   URXD2 / IRDA_RXDL2  |   P0.2    |   P1.3    |   P2.15   |   P1.6    |   P0.1    |
 *   UART2     |   UTXD2 / IRDA_TXD2   |   P0.1    |   P1.4    |   P2.14   |   P1.7    |   ----    |
 *             |   UCTS2               |   P1.13   |   P2.6    |   ----
 *             |   URTS2               |   P1.11   |   P1.14   |   P2.9
 * —————————————————————————————————————————————————————————————————————————————————————————————————
 *             |   URXD3 / IRDA_RXDL3  |   P3.13   |   P1.6    |   P2.1    |   P1.9    |
 *   UART3     |   UTXD3 / IRDA_TXD3   |   P3.12   |   P1.7    |   P2.0    |   ----    |   P2.1    |
 *             |   UCTS3               |   P0.11   |
 *             |   URTS3               |   P1.8    |
 * —————————————————————————————————————————————————————————————————————————————————————
 *             |   URXD4 /IRDA_RXDL4   |   P1.1    |   P1.2    |   P1.4    |   P1.11   |
 *   UART4     |   UTXD4 /IRDA_TXD4    |   P0.10   |   P1.0    |   P1.5    |   P1.10   |
 * —————————————————————————————————————————————————————————————————————————————————————
 *             |   URXD5 / IRDA_RXDL5  |   P0.14   |   P2.5    |   P2.11   |   P3.4    |
 *   UART5     |   UTXD5 / IRDA_TXD5   |   P0.15   |   P2.4    |   P2.10   |   P3.3    |
 * —————————————————————————————————————————————————————————————————————————————————————
 *             |   SCK0                |   P0.0    |   P2.5    |   P1.0    |   P3.0    |
 *   SPI0      |   SEL0                |   P0.1    |   P2.4    |   P1.8    |   P1.7    |
 *             |   MISO0               |   P0.2    |   P2.6    |   P1.1    |   P3.1    |
 *             |   MOSI0               |   ----    |   P2.7    |   P1.2    |   P3.2    |
 * —————————————————————————————————————————————————————————————————————————————————————————————————————————————————————————
 *             |   SCK1                |   P1.13   |   P1.8    |   P0.1    |   P0.12   |   P0.14    |  P2.9    |   P3.1    |
 *   SPI1      |   SEL1                |   P1.12   |   P1.9    |   P1.5    |   P0.13   |   P3.3     |  ----    |   ----    |
 *             |   MISO1               |   P1.14   |   P1.10   |   P2.12   |   ----    |   P3.4     |  ----    |   ----    |
 *             |   MOSI1               |   P1.15   |   P1.11   |   P2.13   |   ----    |   P2.11    |  P3.2    |   ----    |
 * —————————————————————————————————————————————————————————————————————————————————————————————————————————————
 *             |   SCK2                |   P1.0    |   P1.7    |   P1.12   |   ----    |   ----     |  ----    |
 *   SPI2      |   SEL2                |   P2.4    |   P1.8    |   ----    |   ----    |   ----     |  ----    |
 *             |   MISO2               |   P1.1    |   P1.6    |   P3.5    |   ----    |   ----     |  ----    |
 *             |   MOSI2               |   P1.2    |   P1.10   |   P3.6    |   P2.8    |   P2.11    |  P0.0    |
 * —————————————————————————————————————————————————————————————————————————————————————————————————————————————————————————
 *             |   SCL0                |   ----    |   P1.4    |   P0.10   |   P1.1    |   P0.15    |  P3.1    |   P3.13   |
 *   I2C0      |   SDA0                |   ----    |   P1.5    |   P0.11   |   P1.2    |   P0.14    |  P3.2    |   P3.12   |
 * ————————————————————————————————————————————————————————————————————————————————————————————————————————————————————————————————————————————————————————————————————————————————————-
 *             |   SCL1                |   P1.10   |   P1.13   |   P0.1    |   P1.8    |   P1.9     |  P2.0    |   P3.3    |   P3.13   |   ----    |   ----    |   ----    |   ----    |
 *   I2C1      |   SDA1                |   P1.11   |   P1.14   |   P0.2    |   P1.9    |   ----     |  P1.0    |   P1.5    |   P1.10   |   P2.1    |   P3.4    |   P3.12   |   ----    |
 * —————————————————————————————————————————————————————————————————————————————————————————————————————————————————————————
 *             |   SCL2                |   P3.0    |   P1.15   |   P2.10   |   P1.13   |
 *   I2C2      |   SDA2                |   P0.12   |   P1.14   |   P2.11   |   P1.1    |
 * —————————————————————————————————————————————————————————————————————————————————————
 *             |   BCLK0               |   P3.1    |   P2.5    |   P1.0    |
 *   I2S0      |   WS0                 |   P3.2    |   P2.4    |   P1.8    |
 *             |   DOUT0               |   P3.3    |   P2.7    |   P1.2    |
 *             |   DIN0                |   P3.4    |   P2.8    |   P1.3    |
 * —————————————————————————————————————————————————————————————————————————————————————————————————————————————————————————
 *             |   BCLK1               |   P0.1    |   P1.6    |   P1.13   |   ----    |   P3.1    |   P2.9    |   P0.14   |
 *   I2S1      |   WS1                 |   P1.9    |   P1.5    |   P1.12   |   ----    |   P3.3    |   ----    |   ----    |
 *             |   DOUT1               |   P2.13   |   P1.2    |   P1.15   |   ----    |   P3.2    |   P2.11   |   ----    |
 *             |   DIN1                |   P1.10   |   P1.1    |   ----    |   ----    |   ----    |   ----    |   ----    |
 * —————————————————————————————————————————————————————————————————————————————————————————————————————————————————————————
 *             |   BCLK2               |   P1.0    |   P1.7    |   P1.12   |   ----    |   ----    |   ----    |
 *   I2S2      |   WS2                 |   P2.4    |   P1.8    |   ----    |   ----    |  ----     |   ----    |
 *             |   DOUT2               |   P0.0    |   P1.10   |   P2.8    |   P1.2    |   P2.11   |   P3.6    |
 *             |   DIN2                |   N/A     |
 * —————————————————————————————————————————————————————————————————————————————————————————————————————————————
 *             |   CAN_STBY0           |   P2.9    |   P1.2    |   P1.14   |   P1.9    |
 *   CAN0      |   CAN_RX0             |   P0.0    |   P1.0    |   P1.12   |   P3.3    |
 *             |   CAN_TX0             |   P0.1    |   P1.1    |   P1.13   |   P3.4    |
 * —————————————————————————————————————————————————————————————————————————————————————————————————————————————
 *             |   CAN_STBY1           |   P2.8    |   P1.3    |   P2.0    |   P3.0    |  ----     |  ----     |
 *   CAN1      |   CAN_RX1             |   P0.2    |   P1.4    |   P2.1    |   P3.1    |  P1.2     |  P1.12    |
 *             |   CAN_TX1             |   ----    |   P1.5    |   P2.2    |   P3.2    |  P0.10    |  P1.13    |
 * —————————————————————————————————————————————————————————————————————————————————————————————————————————————
 *             |   PWM0                |   P0.2    |   P3.0    |   P2.6    |   P1.12   |  P0.0     |  P0.15    |
 *   CT16B0    |   PWM0N               |   ----    |   P1.13   |   P2.7    |   P0.12   |  P2.5     |  ----     |
 *             |   PWM1                |   P2.9    |   P3.1    |   P0.1    |   P2.5    |  P0.14    |  P2.12    |
 *             |   PWM1N               |   P1.14   |   P0.0    |   P2.8    |   P2.4    |  ----     |  ----     |
 *             |   PWM2                |   P2.7    |   P3.2    |   P0.0    |   P2.8    |  P0.13    |  ----     |
 *             |   PWM2N               |   P1.15   |   P2.6    |   P2.9    |   P0.1    |  ----     |  ----     |
 *             |   PWM3                |   P0.1    |   P3.3    |   ----    |   P2.14   |  P0.12    |  ----     |
 *             |   PWM3N               |   P0.0    |   ----    |   P0.2    |   P2.15   |  ----     |  ----     |
 *             |   BRK                 |   ----    |   P0.10   |   P1.12   |   P2.6    |  ----     |  ----     |
 *             |   CAP0                |   P3.9    |
 * —————————————————————————————————————————————————————————————————————————————————————————————————————————————
 *             |   PWM0                |   ----    |   P1.1    |   P1.12   |   P2.0    |
 *   CT16B1    |   PWM1                |   ----    |   P1.2    |   P1.13   |   P2.4    |
 *             |   PWM2                |   ----    |   P0.10   |   P1.14   |   P2.7    |
 *             |   PWM3                |   ----    |   P0.11   |   P1.15   |   P2.9    |
 *             |   CAP0                |   P3.8    |
 * —————————————————————————————————————————————————————————————————————————————————————————————————————————————
 *             |   PWM0                |   P2.0    |   P2.5    |   P1.8    |   P0.0    |   P0.10   |   P1.4    |
 *   CT16B2    |   PWM1                |   P2.1    |   P1.0    |   P1.7    |   ----    |   P0.11   |   P1.5    |
 *             |   PWM2                |   P2.2    |   P0.1    |   P1.6    |   ----    |   P1.4    |   ----    |
 *             |   PWM3                |   P2.3    |   P0.2    |   P1.10   |   ----    |   P0.0    |   P1.5    |
 *             |   CAP0                |   P0.11   |
 * —————————————————————————————————————————————————————————————————————————————————————————————————————————————
 *             |   PWM0                |   P1.14   |   P2.2    |   P2.6    |   P1.4    |   P2.14   |
 *   CT16B3    |   PWM0N               |   P1.15   |   P2.1    |   P0.10   |   P1.3    |   ----    |
 *             |   PWM1                |   P2.3    |   P1.15   |   P1.12   |   P1.5    |   P2.15   |
 *             |   PWM1N               |   P2.4    |   P0.11   |   P1.13   |   ----    |   ----    |
 * —————————————————————————————————————————————————————————————————————————————————————————————————————————————
 *             |   PWM0                |   P2.7    |   P1.5    |   P0.10   |   ----    |   P1.14   |   ----    |
 *   CT16B4    |   PWM0N               |   P0.11   |   P2.6    |   P1.2    |   ----    |   ----    |   ----    |
 *             |   PWM1                |   P2.6    |   P1.4    |   P2.5    |   P1.12   |   P0.11   |   P1.15   |
 *             |   PWM1N               |   P0.10   |   P1.3    |   P2.4    |   P1.13   |   ----    |   ----    |
 *             |   CAP0                |   P0.13   |
 * —————————————————————————————————————————————————————————————————————————————————————————————————————————————————————————
 *             |   PWM0                |   P2.6    |   P0.15   |   P1.1    |   ----    |   P1.9    |   P1.12   |   P2.0    |
 *   CT16B5    |   PWM1                |   P2.7    |   P0.14   |   P1.2    |   ----    |   ----    |   P1.7    |   P2.1    |
 *             |   PWM2                |   P2.8    |   P0.13   |   P1.0    |   ----    |   P1.6    |   P2.2    |   ----    |
 *             |   PWM3                |   P2.9    |   P0.12   |   ----    |   ----    |   P0.2    |   P2.3    |   ----    |
 *             |   CAP0                |   P1.15   |
 * —————————————————————————————————————————————————————————————————————————————————————————————————
 *             |   PWM0                |   P0.0    |   ----    |   P1.6    |   ----    |   ----    |
 *   CT16B8    |   PWM1                |   P0.1    |   ----    |   P1.10   |   ----    |   ----    |
 *             |   PWM2                |   P0.2    |   ----    |   P1.11   |   ----    |   P2.6    |
 *             |   PWM3                |   ----    |   ----    |   P1.0    |   ----    |   P1.14   |
 *             |   PWM4                |   ----    |   ----    |   P1.1    |   ----    |   P1.15   |
 *             |   PWM5                |   ----    |   ----    |   P0.15   |   ----    |   P0.10   |
 *             |   PWM6                |   ----    |   ----    |   P0.14   |   ----    |   P0.11   |
 *             |   PWM7                |   ----    |   ----    |   P0.13   |   ----    |   P1.3    |
 *             |   PWM8                |   ----    |   ----    |   P0.12   |   ----    |   P1.4    |
 *             |   PWM9                |   ----    |   ----    |   P1.9    |   ----    |   P1.5    |
 *             |   PWM10               |   ----    |   ----    |   P1.8    |   ----    |   P3.8    |
 *             |   PWM11               |   ----    |   ----    |   P1.7    |   ----    |   P3.9    |
 * —————————————————————————————————————————————————————————————————————————————————————————————————
 *   CLKOUT    |   CLKOUT              |   P3.0    |   P0.12   |   P0.10   |
 * —————————————————————————————————————————————————————————————————————————————————————
 *             |   AD0                 |   P0.0    |   ----    |   ----    |   ----    |
 *   LCM       |   AD1                 |   P0.1    |   ----    |   ----    |   ----    |
 *             |   AD2                 |   P0.2    |   ----    |   ----    |   ----    |
 *             |   AD3                 |   ----    |   P1.8    |   ----    |   ----    |
 *             |   AD4                 |   P1.12   |   ----    |   ----    |   ----    |
 *             |   AD5                 |   P1.13   |   ----    |   ----    |   ----    |
 *             |   AD6                 |   P1.14   |   ----    |   ----    |   ----    |
 *             |   AD7                 |   P1.15   |   ----    |   ----    |   ----    |
 *             |   AD8                 |   ----    |   P3.8    |   ----    |   ----    |
 *             |   AD9                 |   ----    |   P3.9    |   ----    |   ----    |
 *             |   AD10                |   P2.4    |   ----    |   ----    |   ----    |
 *             |   AD11                |   P2.5    |   ----    |   ----    |   ----    |
 *             |   AD12                |   P2.6    |   ----    |   ----    |   ----    |
 *             |   AD13                |   P2.7    |   ----    |   ----    |   ----    |
 *             |   AD14                |   P2.8    |   ----    |   ----    |   ----    |
 *             |   AD15                |   P2.9    |   ----    |   ----    |   ----    |
 *             |   AD16                |   P2.10   |   ----    |   ----    |   ----    |
 *             |   AD17                |   P2.11   |   ----    |   ----    |   ----    |
 *             |   A0                  |   P3.0    |   P1.11   |   ----    |   ----    |
 *             |   CS                  |   P0.10   |   P1.1    |   P2.2    |   ----    |
 *             |   WE                  |   ----    |   P3.10   |   ----    |   ----    |
 *             |   OE                  |   ----    |   P3.11   |   ----    |   ----    |
 * —————————————————————————————————————————————————————————————————————————————————————
 *             |   D0                  |   P0.13   |   P3.1    |   P0.0    |
 *   SDIO      |   D1                  |   P0.12   |   P3.2    |   P0.1    |
 *             |   D2                  |   P1.7    |   P3.3    |   P0.2    |
 *             |   D3                  |   P1.6    |   P3.4    |   P2.5    |
 *             |   D4                  |   P1.4    |   ----    |   P2.6    |
 *             |   D5                  |   P1.5    |   ----    |   P2.7    |
 *             |   D6                  |   P0.15   |   ----    |   P2.8    |
 *             |   D7                  |   P0.14   |   ----    |   P2.9    |
 *             |   CK                  |   P1.10   |   P2.0    |   P1.12   |
 *             |   CMD                 |   P1.11   |   P2.1    |   P1.13   |
 * —————————————————————————————————————————————————————————————————————————
 *             |   MII_TX_EN           |   P0.2    |
 *   ETHERNET  |   MII_TX_CLK          |   P2.13   |
 *             |   MII_TXD0            |   P1.12   |
 *             |   MII_TXD1            |   P1.13   |
 *             |   MII_TXD2            |   P2.12   |
 *             |   MII_TXD3            |   P1.4    |
 *             |   MII_RX_ER           |   P0.1    |
 *             |   MII_RX_CLK          |   P2.1    |
 *             |   MII_RX_DV           |   P2.7    |
 *             |   MII_RXD0            |   P2.14   |
 *             |   MII_RXD1            |   P2.15   |
 *             |   MII_RXD2            |   P2.8    |
 *             |   MII_RXD3            |   P2.9    |
 *             |   MII_COL             |   P2.3    |
 *             |   MII_CRS             |   P2.0    |
 *             |   RMII_TX_EN          |   P0.2    |
 *             |   RMII_TXD0           |   P1.12   |
 *             |   RMII_TXD1           |   P1.13   |
 *             |   RMII_RX_ER          |   P3.3    |
 *             |   RMII_RXD0           |   P2.14   |
 *             |   RMII_RXD1           |   P2.15   |
 *             |   RMII_REF_CLK        |   P2.1    |
 *             |   RMII_CRS_DV         |   P2.7    |
 *             |   MDIO                |   P2.2    |
 *             |   MDC                 |   P2.11   |
 *             |   TXER                |   P3.4    |
 *             |   PHY_PDN             |   P3.9    |
 *             |   PHY_LINKSTS         |   P3.8    |
 * —————————————————————————————————————————————————
 *             |   AIN0                |   P2.0    |
 *   ADC       |   AIN1                |   P2.1    |
 *             |   AIN2                |   P2.2    |
 *             |   AIN3                |   P2.3    |
 *             |   AIN4                |   P2.4    |
 *             |   AIN5                |   P2.5    |
 *             |   AIN6                |   P2.6    |
 *             |   AIN7                |   P2.7    |
 *             |   AIN8                |   P2.8    |
 *             |   AIN9                |   P2.9    |
 *             |   AIN10               |   P2.10   |
 *             |   AIN11               |   P2.11   |
 *             |   AIN12               |   P2.12   |
 *             |   AIN13               |   P2.13   |
 *             |   AIN14               |   P2.14   |
 *             |   AIN15               |   P2.15   |
 * —————————————————————————————————————————————————
 */

/* Includes ------------------------------------------------------------------*/
#include "sn34f78x_hal_def.h"

/* Exported constants --------------------------------------------------------*/
/**
 * \defgroup gpio_p00_afio P0.0 of P0_AFIO0
 * \ingroup gpio_afio
 * @{
 */
#define GPIO_P00_GPIO       (0)
#define GPIO_P00_SPI_SCK0   (1)
#define GPIO_P00_SPI_MO2    (2)
#define GPIO_P00_I2S_DOUT2  (3)
#define GPIO_P00_TPM0_PWM2  (6)
#define GPIO_P00_TPM0_PWM3N (7)
#define GPIO_P00_TPM0_PWM1N (8)
#define GPIO_P00_TPM2_PWM3  (9)
#define GPIO_P00_TPM0_PWM0  (10)
#define GPIO_P00_TPM2_PWM0  (11)
#define GPIO_P00_TPM8_PWM0  (12)
#define GPIO_P00_LCM_AD0    (13)
#define GPIO_P00_CAN_RX0    (14)
#define GPIO_P00_SPI_SI2    (16)
#define GPIO_P00_SDIO_D0    (20)
/**
 * @}
 */

/**
 * \defgroup gpio_p01_afio P0.1 of P0_AFIO0
 * \ingroup gpio_afio
 * @{
 */
#define GPIO_P01_GPIO          (0)
#define GPIO_P01_UART_UTXD2    (1)
#define GPIO_P01_UART_URXD2    (2)
#define GPIO_P01_SPI_SEL0      (3)
#define GPIO_P01_SPI_SCK1      (4)
#define GPIO_P01_I2C_SCL1      (5)
#define GPIO_P01_I2S_BCLK1     (6)
#define GPIO_P01_TPM0_PWM1     (8)
#define GPIO_P01_TPM0_PWM2N    (9)
#define GPIO_P01_TPM0_PWM3     (10)
#define GPIO_P01_TPM2_PWM2     (11)
#define GPIO_P01_TPM8_PWM1     (12)
#define GPIO_P01_LCM_AD1       (13)
#define GPIO_P01_CAN_TX0       (14)
#define GPIO_P01_ETH_MII_RX_ER (15)
#define GPIO_P01_IRDA_TXD2     (16)
#define GPIO_P01_IRDA_RXDL2    (18)
#define GPIO_P01_SDIO_D1       (20)
/**
 * @}
 */

/**
 * \defgroup gpio_p02_afio P0.2 of P0_AFIO0
 * \ingroup gpio_afio
 * @{
 */
#define GPIO_P02_GPIO           (0)
#define GPIO_P02_UART_URXD2     (1)
#define GPIO_P02_SPI_MI0        (2)
#define GPIO_P02_I2C_SDA1       (3)
#define GPIO_P02_CAN_RX1        (4)
#define GPIO_P02_TPM0_PWM0      (8)
#define GPIO_P02_TPM5_PWM3      (9)
#define GPIO_P02_TPM0_PWM3N     (10)
#define GPIO_P02_TPM2_PWM3      (11)
#define GPIO_P02_TPM8_PWM2      (12)
#define GPIO_P02_LCM_AD2        (13)
#define GPIO_P02_ETH_RMII_TX_EN (14)
#define GPIO_P02_ETH_MII_TX_EN  (15)
#define GPIO_P02_SPI_SO0        (16)
#define GPIO_P02_IRDA_RXDL2     (18)
#define GPIO_P02_SDIO_D2        (20)
/**
 * @}
 */

/**
 * \defgroup gpio_p010_afio P0.10 of P0_AFIO1
 * \ingroup gpio_afio
 * @{
 */
#define GPIO_P010_GPIO       (0)
#define GPIO_P010_UART_UTXD0 (1)
#define GPIO_P010_UART_UTXD4 (2)
#define GPIO_P010_I2C_SCL0   (3)
#define GPIO_P010_CAN_TX1    (4)
#define GPIO_P010_TPM0_BRK   (6)
#define GPIO_P010_TPM1_PWM2  (7)
#define GPIO_P010_TPM2_PWM0  (8)
#define GPIO_P010_TPM3_PWM0N (9)
#define GPIO_P010_TPM4_PWM0  (10)
#define GPIO_P010_TPM4_PWM1N (11)
#define GPIO_P010_TPM8_PWM5  (12)
#define GPIO_P010_LCM_CS     (14)
#define GPIO_P010_CLKOUT3    (15)
#define GPIO_P010_IRDA_TXD0  (16)
#define GPIO_P010_IRDA_TXD4  (17)
/**
 * @}
 */

/**
 * \defgroup gpio_p011_afio P0.11 of P0_AFIO1
 * \ingroup gpio_afio
 * @{
 */
#define GPIO_P011_GPIO       (0)
#define GPIO_P011_UART_URXD0 (1)
#define GPIO_P011_UART_UCTS3 (2)
#define GPIO_P011_I2C_SDA0   (3)
#define GPIO_P011_TPM2_CAP0  (6)
#define GPIO_P011_TPM1_PWM3  (7)
#define GPIO_P011_TPM2_PWM1  (8)
#define GPIO_P011_TPM3_PWM1N (9)
#define GPIO_P011_TPM4_PWM1  (10)
#define GPIO_P011_TPM4_PWM0N (11)
#define GPIO_P011_TPM8_PWM6  (12)
#define GPIO_P011_IRDA_RXDL0 (18)
/**
 * @}
 */

/**
 * \defgroup gpio_p012_afio P0.12 of P0_AFIO2
 * \ingroup gpio_afio
 * @{
 */
#define GPIO_P012_GPIO       (0)
#define GPIO_P012_SPI_SCK1   (1)
#define GPIO_P012_I2C_SDA2   (3)
#define GPIO_P012_TPM0_PWM3  (7)
#define GPIO_P012_TPM0_PWM0N (8)
#define GPIO_P012_TPM5_PWM3  (10)
#define GPIO_P012_TPM8_PWM8  (12)
#define GPIO_P012_SDIO_D1    (14)
#define GPIO_P012_CLKOUT2    (15)
/**
 * @}
 */

/**
 * \defgroup gpio_p013_afio P0.13 of P0_AFIO2
 * \ingroup gpio_afio
 * @{
 */
#define GPIO_P013_GPIO      (0)
#define GPIO_P013_SPI_SEL1  (1)
#define GPIO_P013_TPM4_CAP0 (6)
#define GPIO_P013_TPM0_PWM2 (7)
#define GPIO_P013_TPM5_PWM2 (10)
#define GPIO_P013_TPM8_PWM7 (12)
#define GPIO_P013_SDIO_D0   (14)
/**
 * @}
 */

/**
 * \defgroup gpio_p014_afio P0.14 of P0_AFIO2
 * \ingroup gpio_afio
 * @{
 */
#define GPIO_P014_GPIO       (0)
#define GPIO_P014_UART_URXD5 (1)
#define GPIO_P014_SPI_SCK1   (2)
#define GPIO_P014_I2C_SDA0   (3)
#define GPIO_P014_I2S_BCLK1  (5)
#define GPIO_P014_TPM0_PWM1  (7)
#define GPIO_P014_TPM5_PWM1  (10)
#define GPIO_P014_TPM8_PWM6  (12)
#define GPIO_P014_SDIO_D7    (14)
#define GPIO_P014_IRDA_RXDL5 (18)
/**
 * @}
 */

/**
 * \defgroup gpio_p015_afio P0.15 of P0_AFIO2
 * \ingroup gpio_afio
 * @{
 */
#define GPIO_P015_GPIO       (0)
#define GPIO_P015_UART_UTXD5 (1)
#define GPIO_P015_I2C_SCL0   (3)
#define GPIO_P015_TPM0_PWM0  (7)
#define GPIO_P015_TPM5_PWM0  (10)
#define GPIO_P015_TPM8_PWM5  (12)
#define GPIO_P015_SDIO_D6    (14)
#define GPIO_P015_IRDA_TXD5  (16)
/**
 * @}
 */

/**
 * \defgroup gpio_p10_afio P1.0 of P1_AFIO0
 * \ingroup gpio_afio
 * @{
 */
#define GPIO_P10_GPIO       (0)
#define GPIO_P10_UART_UTXD4 (1)
#define GPIO_P10_UART_URXD0 (2)
#define GPIO_P10_SPI_SCK0   (3)
#define GPIO_P10_SPI_SCK2   (4)
#define GPIO_P10_I2C_SDA1   (5)
#define GPIO_P10_I2S_BCLK0  (6)
#define GPIO_P10_I2S_BCLK2  (7)
#define GPIO_P10_CAN_RX0    (8)
#define GPIO_P10_TPM2_PWM1  (9)
#define GPIO_P10_TPM5_PWM2  (10)
#define GPIO_P10_TPM8_PWM3  (12)
#define GPIO_P10_SWO        (15)
#define GPIO_P10_IRDA_TXD4  (16)
#define GPIO_P10_IRDA_RXDL0 (18)
/**
 * @}
 */

/**
 * \defgroup gpio_p11_afio P1.1 of P1_AFIO0
 * \ingroup gpio_afio
 * @{
 */
#define GPIO_P11_GPIO       (0)
#define GPIO_P11_UART_URXD4 (1)
#define GPIO_P11_SPI_MI0    (2)
#define GPIO_P11_SPI_MI2    (3)
#define GPIO_P11_I2C_SCL0   (4)
#define GPIO_P11_I2C_SDA2   (5)
#define GPIO_P11_I2S_DIN1   (7)
#define GPIO_P11_CAN_TX0    (8)
#define GPIO_P11_TPM1_PWM0  (9)
#define GPIO_P11_TPM5_PWM0  (10)
#define GPIO_P11_TPM8_PWM4  (12)
#define GPIO_P11_LCM_CS     (14)
#define GPIO_P11_SPI_SO0    (16)
#define GPIO_P11_SPI_SO2    (17)
#define GPIO_P11_IRDA_RXDL4 (18)
/**
 * @}
 */

/**
 * \defgroup gpio_p12_afio P1.2 of P1_AFIO0
 * \ingroup gpio_afio
 * @{
 */
#define GPIO_P12_GPIO       (0)
#define GPIO_P12_UART_URXD4 (1)
#define GPIO_P12_SPI_MO0    (2)
#define GPIO_P12_SPI_MO2    (3)
#define GPIO_P12_I2C_SDA0   (4)
#define GPIO_P12_I2S_DOUT0  (5)
#define GPIO_P12_I2S_DOUT1  (6)
#define GPIO_P12_I2S_DOUT2  (7)
#define GPIO_P12_CAN_STBY0  (8)
#define GPIO_P12_CAN_RX1    (9)
#define GPIO_P12_TPM1_PWM1  (10)
#define GPIO_P12_TPM4_PWM0N (11)
#define GPIO_P12_TPM5_PWM1  (12)
#define GPIO_P12_SPI_SI0    (16)
#define GPIO_P12_SPI_SI2    (17)
#define GPIO_P12_IRDA_RXDL4 (18)
/**
 * @}
 */

/**
 * \defgroup gpio_p13_afio P1.3 of P1_AFIO0
 * \ingroup gpio_afio
 * @{
 */
#define GPIO_P13_GPIO       (0)
#define GPIO_P13_UART_URXD2 (1)
#define GPIO_P13_I2S_DIN0   (2)
#define GPIO_P13_CAN_STBY1  (8)
#define GPIO_P13_TPM3_PWM0N (10)
#define GPIO_P13_TPM4_PWM1N (11)
#define GPIO_P13_TPM8_PWM7  (12)
#define GPIO_P13_IRDA_RXDL2 (18)
/**
 * @}
 */

/**
 * \defgroup gpio_p14_afio P1.4 of P1_AFIO0
 * \ingroup gpio_afio
 * @{
 */
#define GPIO_P14_GPIO         (0)
#define GPIO_P14_UART_UTXD2   (1)
#define GPIO_P14_UART_URXD4   (2)
#define GPIO_P14_I2C_SCL0     (3)
#define GPIO_P14_CAN_RX1      (7)
#define GPIO_P14_TPM2_PWM0    (8)
#define GPIO_P14_TPM2_PWM2    (9)
#define GPIO_P14_TPM3_PWM0    (10)
#define GPIO_P14_TPM4_PWM1    (11)
#define GPIO_P14_TPM8_PWM8    (12)
#define GPIO_P14_SDIO_D4      (14)
#define GPIO_P14_ETH_MII_TXD3 (15)
#define GPIO_P14_IRDA_TXD2    (16)
#define GPIO_P14_IRDA_RXDL4   (18)
/**
 * @}
 */

/**
 * \defgroup gpio_p15_afio P1.5 of P1_AFIO0
 * \ingroup gpio_afio
 * @{
 */
#define GPIO_P15_GPIO       (0)
#define GPIO_P15_UART_UTXD4 (1)
#define GPIO_P15_SPI_SEL1   (2)
#define GPIO_P15_I2C_SDA0   (3)
#define GPIO_P15_I2C_SDA1   (4)
#define GPIO_P15_I2S_WS1    (5)
#define GPIO_P15_CAN_TX1    (7)
#define GPIO_P15_TPM2_PWM1  (8)
#define GPIO_P15_TPM2_PWM3  (9)
#define GPIO_P15_TPM3_PWM1  (10)
#define GPIO_P15_TPM4_PWM0  (11)
#define GPIO_P15_TPM8_PWM9  (12)
#define GPIO_P15_SDIO_D5    (14)
#define GPIO_P15_IRDA_TXD4  (16)
/**
 * @}
 */

/**
 * \defgroup gpio_p16_afio P1.6 of P1_AFIO1
 * \ingroup gpio_afio
 * @{
 */
#define GPIO_P16_GPIO       (0)
#define GPIO_P16_UART_URXD2 (1)
#define GPIO_P16_UART_URXD3 (2)
#define GPIO_P16_SPI_MI2    (3)
#define GPIO_P16_I2S_BCLK1  (4)
#define GPIO_P16_TPM2_PWM2  (8)
#define GPIO_P16_TPM5_PWM2  (9)
#define GPIO_P16_TPM8_PWM0  (12)
#define GPIO_P16_SDIO_D3    (14)
#define GPIO_P16_SPI_SO2    (16)
#define GPIO_P16_IRDA_RXDL2 (18)
#define GPIO_P16_IRDA_RXDL3 (20)
/**
 * @}
 */

/**
 * \defgroup gpio_p17_afio P1.7 of P1_AFIO1
 * \ingroup gpio_afio
 * @{
 */
#define GPIO_P17_GPIO       (0)
#define GPIO_P17_UART_UTXD2 (1)
#define GPIO_P17_UART_UTXD3 (2)
#define GPIO_P17_SPI_SEL0   (3)
#define GPIO_P17_SPI_SCK2   (4)
#define GPIO_P17_I2S_BCLK2  (6)
#define GPIO_P17_TPM2_PWM1  (8)
#define GPIO_P17_TPM5_PWM1  (9)
#define GPIO_P17_TPM8_PWM11 (12)
#define GPIO_P17_SDIO_D2    (14)
#define GPIO_P17_IRDA_TXD2  (16)
#define GPIO_P17_IRDA_TXD3  (17)
/**
 * @}
 */

/**
 * \defgroup gpio_p18_afio P1.8 of P1_AFIO1
 * \ingroup gpio_afio
 * @{
 */
#define GPIO_P18_GPIO       (0)
#define GPIO_P18_UART_URXD1 (1)
#define GPIO_P18_UART_URTS3 (2)
#define GPIO_P18_UART_UTXD0 (3)
#define GPIO_P18_SPI_SEL0   (4)
#define GPIO_P18_SPI_SCK1   (5)
#define GPIO_P18_SPI_SEL2   (6)
#define GPIO_P18_I2C_SCL1   (7)
#define GPIO_P18_I2S_WS0    (8)
#define GPIO_P18_I2S_WS2    (9)
#define GPIO_P18_TPM2_PWM0  (10)
#define GPIO_P18_TPM8_PWM10 (12)
#define GPIO_P18_LCM_AD3    (13)
#define GPIO_P18_IRDA_TXD0  (16)
#define GPIO_P18_IRDA_RXDL1 (18)
/**
 * @}
 */

/**
 * \defgroup gpio_p19_afio P1.9 of P1_AFIO1
 * \ingroup gpio_afio
 * @{
 */
#define GPIO_P19_GPIO       (0)
#define GPIO_P19_UART_UTXD1 (1)
#define GPIO_P19_UART_URXD3 (2)
#define GPIO_P19_SPI_SEL1   (3)
#define GPIO_P19_I2C_SDA1   (4)
#define GPIO_P19_I2C_SCL1   (5)
#define GPIO_P19_I2S_WS1    (6)
#define GPIO_P19_CAN_STBY0  (7)
#define GPIO_P19_TPM5_PWM0  (9)
#define GPIO_P19_TPM8_PWM9  (12)
#define GPIO_P19_IRDA_TXD1  (16)
#define GPIO_P19_IRDA_RXDL3 (18)
/**
 * @}
 */

/**
 * \defgroup gpio_p110_afio P1.10 of P1_AFIO1
 * \ingroup gpio_afio
 * @{
 */
#define GPIO_P110_GPIO       (0)
#define GPIO_P110_UART_UTXD4 (1)
#define GPIO_P110_SPI_MI1    (2)
#define GPIO_P110_SPI_MO2    (3)
#define GPIO_P110_I2C_SCL1   (4)
#define GPIO_P110_I2C_SDA1   (5)
#define GPIO_P110_I2S_DIN1   (6)
#define GPIO_P110_I2S_DOUT2  (7)
#define GPIO_P110_TPM2_PWM3  (9)
#define GPIO_P110_TPM8_PWM1  (12)
#define GPIO_P110_SDIO_CK    (14)
#define GPIO_P110_SPI_SO1    (16)
#define GPIO_P110_SPI_SI2    (17)
#define GPIO_P110_IRDA_TXD4  (18)
/**
 * @}
 */

/**
 * \defgroup gpio_p111_afio P1.11 of P1_AFIO1
 * \ingroup gpio_afio
 * @{
 */
#define GPIO_P111_GPIO       (0)
#define GPIO_P111_UART_URXD4 (1)
#define GPIO_P111_UART_URTS2 (2)
#define GPIO_P111_SPI_MO1    (3)
#define GPIO_P111_I2C_SDA1   (4)
#define GPIO_P111_TPM8_PWM2  (11)
#define GPIO_P111_LCM_A0     (12)
#define GPIO_P111_SDIO_CMD   (14)
#define GPIO_P111_SPI_SI1    (16)
#define GPIO_P111_IRDA_RXDL4 (18)
/**
 * @}
 */

/**
 * \defgroup gpio_p112_afio P1.12 of P1_AFIO2
 * \ingroup gpio_afio
 * @{
 */
#define GPIO_P112_GPIO          (0)
#define GPIO_P112_SPI_SEL1      (1)
#define GPIO_P112_SPI_SCK2      (2)
#define GPIO_P112_I2S_WS1       (3)
#define GPIO_P112_I2S_BCLK2     (4)
#define GPIO_P112_CAN_RX0       (5)
#define GPIO_P112_CAN_RX1       (6)
#define GPIO_P112_TPM0_BRK      (7)
#define GPIO_P112_TPM0_PWM0     (8)
#define GPIO_P112_TPM1_PWM0     (9)
#define GPIO_P112_TPM3_PWM1     (10)
#define GPIO_P112_TPM4_PWM1     (11)
#define GPIO_P112_TPM5_PWM0     (12)
#define GPIO_P112_LCM_AD4       (13)
#define GPIO_P112_ETH_RMII_TXD0 (14)
#define GPIO_P112_ETH_MII_TXD0  (15)
#define GPIO_P112_SDIO_CK       (20)
/**
 * @}
 */

/**
 * \defgroup gpio_p113_afio P1.13 of P1_AFIO2
 * \ingroup gpio_afio
 * @{
 */
#define GPIO_P113_GPIO          (0)
#define GPIO_P113_UART_UCTS2    (1)
#define GPIO_P113_SPI_SCK1      (2)
#define GPIO_P113_I2C_SCL1      (3)
#define GPIO_P113_I2C_SCL2      (4)
#define GPIO_P113_I2S_BCLK1     (5)
#define GPIO_P113_CAN_TX0       (6)
#define GPIO_P113_CAN_TX1       (7)
#define GPIO_P113_TPM0_PWM0N    (8)
#define GPIO_P113_TPM1_PWM1     (9)
#define GPIO_P113_TPM3_PWM1N    (10)
#define GPIO_P113_TPM4_PWM1N    (11)
#define GPIO_P113_LCM_AD5       (13)
#define GPIO_P113_ETH_RMII_TXD1 (14)
#define GPIO_P113_ETH_MII_TXD1  (15)
#define GPIO_P113_SDIO_CMD      (20)
/**
 * @}
 */

/**
 * \defgroup gpio_p114_afio P1.14 of P1_AFIO2
 * \ingroup gpio_afio
 * @{
 */
#define GPIO_P114_GPIO       (0)
#define GPIO_P114_UART_URTS2 (1)
#define GPIO_P114_SPI_MI1    (2)
#define GPIO_P114_I2C_SDA1   (3)
#define GPIO_P114_I2C_SDA2   (4)
#define GPIO_P114_CAN_STBY0  (6)
#define GPIO_P114_TPM0_PWM1N (8)
#define GPIO_P114_TPM1_PWM2  (9)
#define GPIO_P114_TPM3_PWM0  (10)
#define GPIO_P114_TPM4_PWM0  (11)
#define GPIO_P114_TPM8_PWM3  (12)
#define GPIO_P114_LCM_AD6    (14)
#define GPIO_P114_SPI_SO1    (16)
/**
 * @}
 */

/**
 * \defgroup gpio_p115_afio P1.15 of P1_AFIO2
 * \ingroup gpio_afio
 * @{
 */
#define GPIO_P115_GPIO       (0)
#define GPIO_P115_SPI_MO1    (2)
#define GPIO_P115_I2S_DOUT1  (3)
#define GPIO_P115_I2C_SCL2   (4)
#define GPIO_P115_TPM5_CAP0  (6)
#define GPIO_P115_TPM3_PWM1  (7)
#define GPIO_P115_TPM0_PWM2N (8)
#define GPIO_P115_TPM1_PWM3  (9)
#define GPIO_P115_TPM3_PWM0N (10)
#define GPIO_P115_TPM4_PWM1  (11)
#define GPIO_P115_TPM8_PWM4  (12)
#define GPIO_P115_LCM_AD7    (14)
#define GPIO_P115_SPI_SI1    (16)
/**
 * @}
 */

/**
 * \defgroup gpio_p20_afio P2.0 of P2_AFIO0
 * \ingroup gpio_afio
 * @{
 */
#define GPIO_P20_GPIO        (0)
#define GPIO_P20_UART_UTXD3  (1)
#define GPIO_P20_UART_URXD0  (2)
#define GPIO_P20_UART_UCTS1  (3)
#define GPIO_P20_I2C_SCL1    (5)
#define GPIO_P20_CAN_STBY1   (6)
#define GPIO_P20_TPM2_PWM0   (7)
#define GPIO_P20_TPM1_PWM0   (9)
#define GPIO_P20_TPM5_PWM0   (11)
#define GPIO_P20_ADC0_AIN0   (12)
#define GPIO_P20_SDIO_CK     (13)
#define GPIO_P20_ETH_MII_CRS (14)
#define GPIO_P20_IRDA_TXD3   (16)
#define GPIO_P20_IRDA_RXDL0  (18)
/**
 * @}
 */

/**
 * \defgroup gpio_p21_afio P2.1 of P2_AFIO0
 * \ingroup gpio_afio
 * @{
 */
#define GPIO_P21_GPIO             (0)
#define GPIO_P21_UART_URXD3       (1)
#define GPIO_P21_UART_UTXD0       (2)
#define GPIO_P21_UART_UTXD3       (3)
#define GPIO_P21_UART_URTS1       (4)
#define GPIO_P21_I2C_SDA1         (5)
#define GPIO_P21_CAN_RX1          (6)
#define GPIO_P21_TPM2_PWM1        (7)
#define GPIO_P21_TPM3_PWM0N       (8)
#define GPIO_P21_TPM5_PWM1        (11)
#define GPIO_P21_ADC0_AIN1        (12)
#define GPIO_P21_SDIO_CMD         (13)
#define GPIO_P21_ETH_RMII_REF_CLK (14)
#define GPIO_P21_ETH_MII_RX_CLK   (15)
#define GPIO_P21_IRDA_TXD0        (16)
#define GPIO_P21_IRDA_TXD3        (17)
#define GPIO_P21_IRDA_RXDL3       (18)
/**
 * @}
 */

/**
 * \defgroup gpio_p22_afio P2.2 of P2_AFIO0
 * \ingroup gpio_afio
 * @{
 */
#define GPIO_P22_GPIO       (0)
#define GPIO_P22_UART_UTXD1 (1)
#define GPIO_P22_CAN_TX1    (6)
#define GPIO_P22_TPM2_PWM2  (7)
#define GPIO_P22_TPM3_PWM0  (8)
#define GPIO_P22_TPM5_PWM2  (11)
#define GPIO_P22_ADC0_AIN2  (12)
#define GPIO_P22_LCM_CS     (14)
#define GPIO_P22_ETH_MDIO   (15)
#define GPIO_P22_IRDA_TXD1  (16)
/**
 * @}
 */

/**
 * \defgroup gpio_p23_afio P2.3 of P2_AFIO0
 * \ingroup gpio_afio
 * @{
 */
#define GPIO_P23_GPIO        (0)
#define GPIO_P23_UART_URXD1  (1)
#define GPIO_P23_TPM2_PWM3   (7)
#define GPIO_P23_TPM3_PWM1   (8)
#define GPIO_P23_TPM5_PWM3   (11)
#define GPIO_P23_ADC0_AIN3   (12)
#define GPIO_P23_ETH_MII_COL (15)
#define GPIO_P23_IRDA_RXDL1  (18)
/**
 * @}
 */

/**
 * \defgroup gpio_p24_afio P2.4 of P2_AFIO0
 * \ingroup gpio_afio
 * @{
 */
#define GPIO_P24_GPIO       (0)
#define GPIO_P24_UART_UTXD5 (1)
#define GPIO_P24_SPI_SEL0   (2)
#define GPIO_P24_SPI_SEL2   (3)
#define GPIO_P24_I2S_WS0    (4)
#define GPIO_P24_I2S_WS2    (5)
#define GPIO_P24_TPM0_PWM1N (6)
#define GPIO_P24_TPM1_PWM1  (7)
#define GPIO_P24_TPM3_PWM1N (8)
#define GPIO_P24_TPM4_PWM1N (9)
#define GPIO_P24_ADC0_AIN4  (12)
#define GPIO_P24_LCM_AD10   (14)
#define GPIO_P24_IRDA_TXD5  (16)
/**
 * @}
 */

/**
 * \defgroup gpio_p25_afio P2.5 of P2_AFIO0
 * \ingroup gpio_afio
 * @{
 */
#define GPIO_P25_GPIO       (0)
#define GPIO_P25_UART_URXD5 (1)
#define GPIO_P25_SPI_SCK0   (2)
#define GPIO_P25_I2S_BCLK0  (3)
#define GPIO_P25_TPM0_PWM1  (6)
#define GPIO_P25_TPM0_PWM0N (7)
#define GPIO_P25_TPM2_PWM0  (8)
#define GPIO_P25_TPM4_PWM1  (9)
#define GPIO_P25_ADC0_AIN5  (12)
#define GPIO_P25_LCM_AD11   (13)
#define GPIO_P25_IRDA_RXDL5 (18)
#define GPIO_P25_SDIO_D3    (20)
/**
 * @}
 */

/**
 * \defgroup gpio_p26_afio P2.6 of P2_AFIO1
 * \ingroup gpio_afio
 * @{
 */
#define GPIO_P26_GPIO       (0)
#define GPIO_P26_UART_UCTS2 (1)
#define GPIO_P26_SPI_MI0    (2)
#define GPIO_P26_TPM8_PWM2  (5)
#define GPIO_P26_TPM0_PWM0  (6)
#define GPIO_P26_TPM0_PWM2N (7)
#define GPIO_P26_TPM3_PWM0  (8)
#define GPIO_P26_TPM4_PWM0N (9)
#define GPIO_P26_TPM4_PWM1  (10)
#define GPIO_P26_TPM5_PWM0  (11)
#define GPIO_P26_ADC0_AIN6  (12)
#define GPIO_P26_LCM_AD12   (13)
#define GPIO_P26_TPM0_BRK   (15)
#define GPIO_P26_SPI_SO0    (16)
#define GPIO_P26_SDIO_D4    (20)
/**
 * @}
 */

/**
 * \defgroup gpio_p27_afio P2.7 of P2_AFIO1
 * \ingroup gpio_afio
 * @{
 */
#define GPIO_P27_GPIO            (0)
#define GPIO_P27_SPI_MO0         (1)
#define GPIO_P27_I2S_DOUT0       (2)
#define GPIO_P27_TPM0_PWM0N      (6)
#define GPIO_P27_TPM0_PWM2       (7)
#define GPIO_P27_TPM1_PWM2       (8)
#define GPIO_P27_TPM4_PWM0       (9)
#define GPIO_P27_TPM5_PWM1       (11)
#define GPIO_P27_ADC0_AIN7       (12)
#define GPIO_P27_LCM_AD13        (13)
#define GPIO_P27_ETH_MII_RX_DV   (14)
#define GPIO_P27_ETH_RMII_CRS_DV (15)
#define GPIO_P27_SPI_SI0         (16)
#define GPIO_P27_SDIO_D5         (20)
/**
 * @}
 */

/**
 * \defgroup gpio_p28_afio P2.8 of P2_AFIO1
 * \ingroup gpio_afio
 * @{
 */
#define GPIO_P28_GPIO         (0)
#define GPIO_P28_UART_URXD1   (1)
#define GPIO_P28_SPI_MO2      (2)
#define GPIO_P28_I2S_DIN0     (3)
#define GPIO_P28_I2S_DOUT2    (5)
#define GPIO_P28_TPM0_PWM1N   (7)
#define GPIO_P28_TPM0_PWM2    (8)
#define GPIO_P28_TPM5_PWM2    (11)
#define GPIO_P28_ADC0_AIN8    (12)
#define GPIO_P28_LCM_AD14     (13)
#define GPIO_P28_CAN_STBY1    (14)
#define GPIO_P28_ETH_MII_RXD2 (15)
#define GPIO_P28_SPI_SI2      (16)
#define GPIO_P28_IRDA_RXDL1   (18)
#define GPIO_P28_SDIO_D6      (20)
/**
 * @}
 */

/**
 * \defgroup gpio_p29_afio P2.9 of P2_AFIO1
 * \ingroup gpio_afio
 * @{
 */
#define GPIO_P29_GPIO         (0)
#define GPIO_P29_UART_URTS2   (1)
#define GPIO_P29_SPI_SCK1     (2)
#define GPIO_P29_I2S_BCLK1    (3)
#define GPIO_P29_TPM1_PWM3    (6)
#define GPIO_P29_TPM0_PWM1    (7)
#define GPIO_P29_TPM0_PWM2N   (8)
#define GPIO_P29_TPM5_PWM3    (11)
#define GPIO_P29_ADC0_AIN9    (12)
#define GPIO_P29_LCM_AD15     (13)
#define GPIO_P29_CAN_STBY0    (14)
#define GPIO_P29_ETH_MII_RXD3 (15)
#define GPIO_P29_SDIO_D7      (20)
/**
 * @}
 */

/**
 * \defgroup gpio_p210_afio P2.10 of P2_AFIO1
 * \ingroup gpio_afio
 * @{
 */
#define GPIO_P210_GPIO       (0)
#define GPIO_P210_UART_UTXD5 (1)
#define GPIO_P210_I2C_SCL2   (2)
#define GPIO_P210_ADC0_AIN10 (12)
#define GPIO_P210_LCM_AD16   (13)
#define GPIO_P210_IRDA_TXD5  (16)
/**
 * @}
 */

/**
 * \defgroup gpio_p211_afio P2.11 of P2_AFIO1
 * \ingroup gpio_afio
 * @{
 */
#define GPIO_P211_GPIO       (0)
#define GPIO_P211_UART_URXD5 (1)
#define GPIO_P211_SPI_MO2    (2)
#define GPIO_P211_SPI_MO1    (3)
#define GPIO_P211_I2C_SDA2   (4)
#define GPIO_P211_I2S_DOUT2  (5)
#define GPIO_P211_I2S_DOUT1  (6)
#define GPIO_P211_ADC0_AIN11 (12)
#define GPIO_P211_LCM_AD17   (13)
#define GPIO_P211_ETH_MDC    (15)
#define GPIO_P211_SPI_SI2    (16)
#define GPIO_P211_SPI_SI1    (17)
#define GPIO_P211_IRDA_RXDL5 (18)
/**
 * @}
 */

/**
 * \defgroup gpio_p212_afio P2.12 of P2_AFIO2
 * \ingroup gpio_afio
 * @{
 */
#define GPIO_P212_GPIO         (0)
#define GPIO_P212_SPI_MI1      (1)
#define GPIO_P212_TPM0_PWM1    (8)
#define GPIO_P212_ADC0_AIN12   (12)
#define GPIO_P212_ETH_MII_TXD2 (15)
#define GPIO_P212_SPI_SO1      (16)
/**
 * @}
 */

/**
 * \defgroup gpio_p213_afio P2.13 of P2_AFIO2
 * \ingroup gpio_afio
 * @{
 */
#define GPIO_P213_GPIO           (0)
#define GPIO_P213_SPI_MO1        (1)
#define GPIO_P213_I2S_DOUT1      (2)
#define GPIO_P213_ADC0_AIN13     (12)
#define GPIO_P213_ETH_MII_TX_CLK (15)
#define GPIO_P213_SPI_SI1        (16)
/**
 * @}
 */

/**
 * \defgroup gpio_p214_afio P2.14 of P2_AFIO2
 * \ingroup gpio_afio
 * @{
 */
#define GPIO_P214_GPIO          (0)
#define GPIO_P214_UART_UTXD2    (1)
#define GPIO_P214_TPM0_PWM3     (8)
#define GPIO_P214_TPM3_PWM0     (9)
#define GPIO_P214_ADC0_AIN14    (12)
#define GPIO_P214_ETH_RMII_RXD0 (14)
#define GPIO_P214_ETH_MII_RXD0  (15)
#define GPIO_P214_IRDA_TXD2     (16)
/**
 * @}
 */

/**
 * \defgroup gpio_p215_afio P2.15 of P2_AFIO2
 * \ingroup gpio_afio
 * @{
 */
#define GPIO_P215_GPIO          (0)
#define GPIO_P215_UART_URXD2    (1)
#define GPIO_P215_TPM0_PWM3N    (8)
#define GPIO_P215_TPM3_PWM1     (9)
#define GPIO_P215_ADC0_AIN15    (12)
#define GPIO_P215_ETH_RMII_RXD1 (14)
#define GPIO_P215_ETH_MII_RXD1  (15)
#define GPIO_P215_IRDA_RXDL2    (18)
/**
 * @}
 */

/**
 * \defgroup gpio_p30_afio P3.0 of P3_AFIO0
 * \ingroup gpio_afio
 * @{
 */
#define GPIO_P30_GPIO       (0)
#define GPIO_P30_UART_UTXD1 (1)
#define GPIO_P30_SPI_SCK0   (2)
#define GPIO_P30_I2C_SCL2   (3)
#define GPIO_P30_CAN_STBY1  (7)
#define GPIO_P30_TPM0_PWM0  (8)
#define GPIO_P30_LCM_A0     (13)
#define GPIO_P30_CLKOUT1    (15)
#define GPIO_P30_IRDA_TXD1  (16)
/**
 * @}
 */

/**
 * \defgroup gpio_p31_afio P3.1 of P3_AFIO0
 * \ingroup gpio_afio
 * @{
 */
#define GPIO_P31_GPIO       (0)
#define GPIO_P31_UART_UTXD0 (1)
#define GPIO_P31_SPI_MI0    (2)
#define GPIO_P31_SPI_SCK1   (3)
#define GPIO_P31_I2C_SCL0   (4)
#define GPIO_P31_I2S_BCLK0  (5)
#define GPIO_P31_I2S_BCLK1  (6)
#define GPIO_P31_CAN_RX1    (7)
#define GPIO_P31_TPM0_PWM1  (8)
#define GPIO_P31_SDIO_D0    (13)
#define GPIO_P31_SPI_SO0    (16)
#define GPIO_P31_IRDA_TXD0  (17)
/**
 * @}
 */

/**
 * \defgroup gpio_p32_afio P3.2 of P3_AFIO0
 * \ingroup gpio_afio
 * @{
 */
#define GPIO_P32_GPIO       (0)
#define GPIO_P32_UART_URXD0 (1)
#define GPIO_P32_SPI_MO0    (2)
#define GPIO_P32_SPI_MO1    (3)
#define GPIO_P32_I2C_SDA0   (4)
#define GPIO_P32_I2S_WS0    (5)
#define GPIO_P32_I2S_DOUT1  (6)
#define GPIO_P32_CAN_TX1    (7)
#define GPIO_P32_TPM0_PWM2  (8)
#define GPIO_P32_SDIO_D1    (13)
#define GPIO_P32_SPI_SI0    (16)
#define GPIO_P32_SPI_SI1    (17)
#define GPIO_P32_IRDA_RXDL0 (18)
/**
 * @}
 */

/**
 * \defgroup gpio_p33_afio P3.3 of P3_AFIO0
 * \ingroup gpio_afio
 * @{
 */
#define GPIO_P33_GPIO           (0)
#define GPIO_P33_UART_UCTS0     (1)
#define GPIO_P33_UART_UTXD5     (2)
#define GPIO_P33_SPI_SEL1       (3)
#define GPIO_P33_I2C_SCL1       (4)
#define GPIO_P33_I2S_DOUT0      (5)
#define GPIO_P33_I2S_WS1        (6)
#define GPIO_P33_CAN_RX0        (7)
#define GPIO_P33_TPM0_PWM3      (8)
#define GPIO_P33_SDIO_D2        (13)
#define GPIO_P33_ETH_RMII_RX_ER (15)
#define GPIO_P33_IRDA_TXD5      (16)
/**
 * @}
 */

/**
 * \defgroup gpio_p34_afio P3.4 of P3_AFIO0
 * \ingroup gpio_afio
 * @{
 */
#define GPIO_P34_GPIO       (0)
#define GPIO_P34_UART_URXD1 (1)
#define GPIO_P34_UART_URXD5 (2)
#define GPIO_P34_UART_URTS0 (3)
#define GPIO_P34_SPI_MI1    (4)
#define GPIO_P34_I2C_SDA1   (5)
#define GPIO_P34_I2S_DIN0   (6)
#define GPIO_P34_CAN_TX0    (7)
#define GPIO_P34_SDIO_D3    (13)
#define GPIO_P34_ETH_TXER   (15)
#define GPIO_P34_SPI_SO1    (16)
#define GPIO_P34_IRDA_RXDL1 (18)
#define GPIO_P34_IRDA_RXDL5 (20)
/**
 * @}
 */

/**
 * \defgroup gpio_p35_afio P3.5 of P3_AFIO0
 * \ingroup gpio_afio
 * @{
 */
#define GPIO_P35_SWDIO   (0)
#define GPIO_P35_GPIO    (1)
#define GPIO_P35_SPI_MI2 (2)
#define GPIO_P35_SPI_SO2 (16)
/**
 * @}
 */

/**
 * \defgroup gpio_p36_afio P3.6 of P3_AFIO1
 * \ingroup gpio_afio
 * @{
 */
#define GPIO_P36_SWCLK      (0)
#define GPIO_P36_GPIO       (1)
#define GPIO_P36_UART_UTXD1 (2)
#define GPIO_P36_SPI_MO2    (3)
#define GPIO_P36_I2S_DOUT2  (4)
#define GPIO_P36_SPI_SI2    (16)
#define GPIO_P36_IRDA_TXD1  (17)
/**
 * @}
 */

/**
 * \defgroup gpio_p37_afio P3.7 of P3_AFIO1
 * \ingroup gpio_afio
 * @{
 */
#define GPIO_P37_GPIO           (0)
#define GPIO_P37_EXTERNAL_RESET (1)
/**
 * @}
 */

/**
 * \defgroup gpio_p38_afio P3.8 of P3_AFIO1
 * \ingroup gpio_afio
 * @{
 */
#define GPIO_P38_GPIO            (0)
#define GPIO_P38_TPM1_CAP0       (6)
#define GPIO_P38_TPM8_PWM10      (12)
#define GPIO_P38_LCM_AD8         (14)
#define GPIO_P38_ETH_PHY_LINKSTS (15)
/**
 * @}
 */

/**
 * \defgroup gpio_p39_afio P3.9 of P3_AFIO1
 * \ingroup gpio_afio
 * @{
 */
#define GPIO_P39_GPIO        (0)
#define GPIO_P39_TPM0_CAP0   (6)
#define GPIO_P39_TPM8_PWM11  (12)
#define GPIO_P39_LCM_AD9     (14)
#define GPIO_P39_ETH_PHY_PDN (15)
/**
 * @}
 */

/**
 * \defgroup gpio_p310_afio P3.10 of P3_AFIO1
 * \ingroup gpio_afio
 * @{
 */
#define GPIO_P310_GPIO   (0)
#define GPIO_P310_LXIN   (1)
#define GPIO_P310_LCM_WE (13)
/**
 * @}
 */

/**
 * \defgroup gpio_p311_afio P3.11 of P3_AFIO1
 * \ingroup gpio_afio
 * @{
 */
#define GPIO_P311_GPIO   (0)
#define GPIO_P311_LXOUT  (1)
#define GPIO_P311_LCM_OE (13)
/**
 * @}
 */

/**
 * \defgroup gpio_p312_afio P3.12 of P3_AFIO2
 * \ingroup gpio_afio
 * @{
 */
#define GPIO_P312_GPIO       (0)
#define GPIO_P312_XIN        (1)
#define GPIO_P312_UART_UTXD3 (2)
#define GPIO_P312_I2C_SDA0   (3)
#define GPIO_P312_I2C_SDA1   (4)
#define GPIO_P312_IRDA_TXD3  (16)
/**
 * @}
 */

/**
 * \defgroup gpio_p313_afio P3.13 of P3_AFIO2
 * \ingroup gpio_afio
 * @{
 */
#define GPIO_P313_GPIO       (0)
#define GPIO_P313_XOUT       (1)
#define GPIO_P313_UART_URXD3 (2)
#define GPIO_P313_I2C_SCL0   (3)
#define GPIO_P313_I2C_SCL1   (4)
#define GPIO_P313_IRDA_RXDL3 (18)
/**
 * @}
 */

#ifdef __cplusplus
}
#endif

#endif /* _HAL_GPIO_EX_H_ */
