/***********************
 * SN34F78X SDK V2.1.4 *
 ***********************/

#ifndef _HAL_SCU_H_
#define _HAL_SCU_H_

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "sn34f78x_hal_def.h"

/** \defgroup scu_chip_info Chip infomation
 * \brief  Chip ID and version.
 * \ingroup scu_api_define
 * \note
 * \{
 */
#define __HAL_SCU_CHIP_ID    (SN_SCU->CHIPID)  /*!< Chip ID */
#define __HAL_SCU_IP_VERSION (SN_SCU->VERSION) /*!< Chip version */
/**
 * @}
 */

/** \defgroup scu_ris SCU interrupt status
 * \brief  SCU RIS(raw interrupt status) bit definition.
 * \ingroup scu_control
 * \note
 * \{
 */
#define SCU_RIS_DS_WAKEUP_Pos (2U)                             /*!< Deep sleep mode wakeup interrupt status bit offset */
#define SCU_RIS_DS_WAKEUP_Msk (0x1UL << SCU_RIS_DS_WAKEUP_Pos) /*!< Deep sleep mode wakeup interrupt status mask: 0x00000004 */
#define SCU_RIS_DS_WAKEUP     SCU_RIS_DS_WAKEUP_Msk            /*!< Deep sleep mode wakeup interrupt status */

#define SCU_RIS_SLP_WAKEUP_Pos (3U)                              /*!< Sleep mode wakeup interrupt status bit offset */
#define SCU_RIS_SLP_WAKEUP_Msk (0x1UL << SCU_RIS_SLP_WAKEUP_Pos) /*!< Sleep mode wakeup interrupt status mask: 0x00000008 */
#define SCU_RIS_SLP_WAKEUP     SCU_RIS_SLP_WAKEUP_Msk            /*!< Sleep mode wakeup interrupt status */

#define SCU_RIS_FCS_Pos (6U)                       /*!< FCS command finish interrupt status bit offset */
#define SCU_RIS_FCS_Msk (0x1UL << SCU_RIS_FCS_Pos) /*!< FCS command finish interrupt status mask: 0x00000040 */
#define SCU_RIS_FCS     SCU_RIS_FCS_Msk            /*!< FCS command finish interrupt status */

#define SCU_RIS_REMAPCHG_Pos (11U)                           /*!< Remap is changed status after reboot command bit offset */
#define SCU_RIS_REMAPCHG_Msk (0x1UL << SCU_RIS_REMAPCHG_Pos) /*!< Remap is changed status after reboot command make: 0x00000800 */
#define SCU_RIS_REMAPCHG     SCU_RIS_REMAPCHG_Msk            /*!< Remap is changed status after reboot command  */

#define SCU_RIS_RTC_ALARM_Pos (16U)                            /*!< RTC alarm interrupt status bit offset */
#define SCU_RIS_RTC_ALARM_Msk (0x1UL << SCU_RIS_RTC_ALARM_Pos) /*!< RTC alarm interrupt status mask: 0x00010000 */
#define SCU_RIS_RTC_ALARM     SCU_RIS_RTC_ALARM_Msk            /*!< RTC alarm interrupt status */

#define SCU_RIS_RTC_PER_Pos (17U)                          /*!< RTC periodic interrupt status bit offset */
#define SCU_RIS_RTC_PER_Msk (0x1UL << SCU_RIS_RTC_PER_Pos) /*!< RTC periodic interrupt status mask: 0x00020000 */
#define SCU_RIS_RTC_PER     SCU_RIS_RTC_PER_Msk            /*!< RTC periodic interrupt status */

#define SCU_RIS_RTC_SEC_Pos (18U)                          /*!< RTC second out interrupt status bit offset */
#define SCU_RIS_RTC_SEC_Msk (0x1UL << SCU_RIS_RTC_SEC_Pos) /*!< RTC second out interrupt status mask: 0x00040000 */
#define SCU_RIS_RTC_SEC     SCU_RIS_RTC_SEC_Msk            /*!< RTC second out interrupt status */
/**
 * @}
 */

/** \defgroup scu_ie_ctl Interrupt Controls
 * \brief  SCU interrupt controls.
 * \ingroup scu_api_define
 * \note
 * \{
 */
#define __HAL_SCU_SET_IE_DS_WAKEUP(x)  (SN_SCU->IE_b.DS_WAKEUP_EINT = !!(x)) /*!< Enable deep sleep mode wakeup interrupt event*/
#define __HAL_SCU_SET_IE_SLP_WAKEUP(x) (SN_SCU->IE_b.WAKEUP_EINT = !!(x))    /*!< Enable sleep mode wakeup interrupt status interrupt event */
#define __HAL_SCU_SET_IE_FCS(x)        (SN_SCU->IE_b.FCS_EINT = !!(x))       /*!< Enable fCS command finish interrupt status interrupt event */
#define __HAL_SCU_SET_IE_REMAP_CHG(x)  (SN_SCU->IE_b.REMAPCHG_EINT = !!(x))  /*!< Enable remap is changed status after reboot command interrupt event */
#define __HAL_SCU_SET_IE_RTC_ALART(x)  (SN_SCU->IE_b.RTC_ALARM_EINT = !!(x)) /*!< Enable RTC alarm interrupt status interrupt event */
#define __HAL_SCU_SET_IE_RTC_PER(x)    (SN_SCU->IE_b.RTC_PER_EINT = !!(x))   /*!< Enable RTC periodic interrupt status interrupt event */
#define __HAL_SCU_SET_IE_RTC_EINT(x)   (SN_SCU->IE_b.RTC_SEC_EINT = !!(x))   /*!< Enable RTC second out interrupt status interrupt event */

#define __HAL_SCU_GET_IE_DS_WAKEUP(x)  (SN_SCU->IE_b.DS_WAKEUP_EINT) /*!< Disable deep sleep mode wakeup interrupt event*/
#define __HAL_SCU_GET_IE_SLP_WAKEUP(x) (SN_SCU->IE_b.WAKEUP_EINT)    /*!< Disable sleep mode wakeup interrupt status interrupt event */
#define __HAL_SCU_GET_IE_FCS(x)        (SN_SCU->IE_b.FCS_EINT)       /*!< Disable fCS command finish interrupt status interrupt event */
#define __HAL_SCU_GET_IE_REMAP_CHG(x)  (SN_SCU->IE_b.REMAPCHG_EINT)  /*!< Disable remap is changed status after reboot command interrupt event */
#define __HAL_SCU_GET_IE_RTC_ALART(x)  (SN_SCU->IE_b.RTC_ALARM_EINT) /*!< Disable RTC alarm interrupt status interrupt event */
#define __HAL_SCU_GET_IE_RTC_PER(x)    (SN_SCU->IE_b.RTC_PER_EINT)   /*!< Disable RTC periodic interrupt status interrupt event */
#define __HAL_SCU_GET_IE_RTC_EINT(x)   (SN_SCU->IE_b.RTC_SEC_EINT)   /*!< Disable RTC second out interrupt status interrupt event */

#define __HAL_SCU_RIS_DS_WAKEUP_CLR()    (SN_SCU->RIS = SCU_RIS_DS_WAKEUP)  /*!< Clear deep sleep mode wakeup interrupt event*/
#define __HAL_SCU_RIS_SLP_WAKEUP_CLR()   (SN_SCU->RIS = SCU_RIS_SLP_WAKEUP) /*!< Clear sleep mode wakeup interrupt status interrupt event */
#define __HAL_SCU_RIS_FCS_CLR()          (SN_SCU->RIS = SCU_RIS_FCS)        /*!< Clear fCS command finish interrupt status interrupt event */
#define __HAL_SCU_RIS_IE_REMAP_CHG_CLR() (SN_SCU->RIS = SCU_RIS_REMAPCHG)   /*!< Clear remap is changed status after reboot command interrupt event */
#define __HAL_SCU_RIS_RTC_ALART_CLR()    (SN_SCU->RIS = SCU_RIS_RTC_ALARM)  /*!< Clear RTC alarm interrupt status interrupt event */
#define __HAL_SCU_RIS_RTC_PER_CLR()      (SN_SCU->RIS = SCU_RIS_RTC_PER)    /*!< Clear RTC periodic interrupt status interrupt event */
#define __HAL_SCU_RIS_RTC_EINT_CLR()     (SN_SCU->RIS = SCU_RIS_RTC_SEC)    /*!< Clear RTC second out interrupt status interrupt event */
/**
 * @}
 */

/** \defgroup scu_efc_stb_ctl eflash stb control
 * \ingroup scu_api_define
 * \note
 * \{
 */
#define __HAL_SCU_EFLASH_STANDBY_ON()  (SN_SCU->PWRMODE_b.EFC_STB_OFF = DISABLE) /*!< stand by */
#define __HAL_SCU_EFLASH_STANDBY_OFF() (SN_SCU->PWRMODE_b.EFC_STB_OFF = ENABLE)  /*!< free run */
/**
 * @}
 */

void HAL_PWR_Callback(uint32_t ris);
void HAL_RCC_Callback(uint32_t ris);
void HAL_RTC_Callback(uint32_t ris);
void HAL_SCU_IRQHandler(void);

#ifdef __cplusplus
}
#endif

#endif /* _HAL_SCU_H_ */
