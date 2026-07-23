/***********************
 * SN34F78X SDK V2.1.4 *
 ***********************/

#ifndef _HAL_PWR_H_
#define _HAL_PWR_H_

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "sn34f78x_hal_def.h"
/* Exported types ------------------------------------------------------------*/

/* Exported constants --------------------------------------------------------*/
/**
 * \defgroup pwr_wakeup_source PWR Wakeup Source
 * \ingroup pwr_control
 * @{
 */
#define PWR_WKPSRC_RTC  (1 << 17) /**< wakeup by RTC alarm in Deep Sleep or DPD mode */
#define PWR_WKPSRC_P0   (1 << 18) /**< wakeup by GPIO0 in Deep Sleep or DPD mode */
#define PWR_WKPSRC_P1   (1 << 19) /**< wakeup by GPIO1 in Deep Sleep or DPD mode */
#define PWR_WKPSRC_P2   (1 << 20) /**< wakeup by GPIO2 in Deep Sleep or DPD mode */
#define PWR_WKPSRC_P3   (1 << 21) /**< wakeup by GPIO3 in Deep Sleep or DPD mode */
#define PWR_WKPSRC_USBD (1 << 22) /**< wakeup by USB Device suspend in Deep Sleep or DPD mode */
#define PWR_WKPSRC_ETH  (1 << 23) /**< wakeup by Ethernet Wake-on-LAN frame in Deep Sleep or DPD mode */
#define PWR_WKPSRC_USBH (1 << 24) /**< wakeup by USB Host suspend in Deep Sleep or DPD mode */
/**
 * @}
 */

/* Exported functions --------------------------------------------------------*/
/* Initialization/de-initialization functions  ********************************/
void HAL_PWR_DeInit(void);
/* Peripheral control functions  **********************************************/
/* Low Power modes entry */
void HAL_PWR_EnterSleepMode(void);
void HAL_PWR_EnterDeepSleepMode(void);
void HAL_PWR_EnterDeepPowerDownMode(void);
/* Config wakeup source/Clear wakeup flag/Get wakeup info */
HAL_Status_T HAL_PWR_SetWakeUpSource(uint32_t wakeup_source);
HAL_Status_T HAL_PWR_ClearWakeUpFlag(uint32_t wakeup_source);
uint32_t     HAL_PWR_GetWakeUpInfo(void);
/* Check Deep Power Down flag */
uint8_t HAL_PWR_CheckDeepPowerDownFlag(void);
/* Clear Deep Power Down flag */
void HAL_PWR_ClearDeepPowerDownFlag(void);
/* set sram enable or disable */
void HAL_PWR_SetSRAMPwrSave(uint8_t is_pwr_save);
/* check whether enter eFlash standby mode */
void HAL_PWR_SetEflash_Standby(uint8_t is_en);
/* IRQHandler and Event Callback functions ************************************/
void HAL_PWR_WKP_IRQHandler(uint32_t ris);
void HAL_PWR_SleepWkpCallback(void);
void HAL_PWR_DeepSleepWkpCallback(void);

#ifdef __cplusplus
}
#endif

#endif /* _HAL_PWR_H_ */
