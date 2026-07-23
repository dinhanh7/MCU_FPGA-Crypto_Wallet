/***********************
 * SN34F78X SDK V2.1.4 *
 ***********************/

#ifndef _SN34F78X_HAL_FLASH_H_
#define _SN34F78X_HAL_FLASH_H_

/* Includes ------------------------------------------------------------------*/
#include "sn34f78x_hal.h"

/* private types -------------------------------------------------------------*/
/* private variables ---------------------------------------------------------*/
/* private constants ---------------------------------------------------------*/

/**
 * @brief  FLASH operation definition
 */
#define FLASH_OPERATION_ERASE              (0x01)
#define FLASH_OPERATION_PROGRAM            (0x02)
#define FLASH_OPERATION_ARB_WRITE          (0x03)
#define FLASH_OPERATION_MM_RDP             (0x04)
#define FLASH_OPERATION_WRP                (0x05)
#define FLASH_OPERATION_PROT_STATUS        (0x06)
#define FLASH_OPERATION_BOOT_FLAG_RW       (0x07)
#define FLASH_OPERATION_CUSTOMER_USE_WRITE (0x08)
#define FLASH_OPERATION_FREQ_RW            (0x09)
#define FLASH_OPERATION_CHECKSUM_CAL       (0x0A)
#define FLASH_OPERATION_MUX_SEL            (0x0B)

/**
 * @brief  FLASH status definition
 */
#define FLASH_STATUS_PASS                      (0x0000)
#define FLASH_STATUS_INVALID_ADDR              (0x0001)
#define FLASH_STATUS_INVALID_TYPE              (0x0002)
#define FLASH_STATUS_ILLEGAL_SM_OP             (0x0003)
#define FLASH_STATUS_ERASE_FAILED              (0x0004)
#define FLASH_STATUS_PROGRAM_FAILED            (0x0005)
#define FLASH_STATUS_RDP_CHANGE_FAILED         (0x0006)
#define FLASH_STATUS_WRP_EN_DIS_FAILED         (0x0007)
#define FLASH_STATUS_ARB_WRITE_FAILED          (0x0008)
#define FLASH_STATUS_INVALID_MAGIC_NUM         (0x0009)
#define FLASH_STATUS_BOOTFLAG_WRITE_FAILED     (0x000A)
#define FLASH_STATUS_CUSTOMER_USE_WRITE_FAILED (0x000B)
#define FLASH_STATUS_INVALID_OFFSET            (0x000C)
#define FLASH_STATUS_INVALID_LENGTH            (0x000D)
#define FLASH_STATUS_FAILURE                   (0xFFFF)

/**
 * @brief  FLASH Program Size definition
 */
#define FLASH_PROGRAM_SIZE_BYTE       (0x01)  // 1-byte
#define FLASH_PROGRAM_SIZE_HALFWORD   (0x02)  // 2-byte
#define FLASH_PROGRAM_SIZE_WORD       (0x04)  // 4-byte
#define FLASH_PROGRAM_SIZE_DOUBLEWORD (0x08)  // 8-byte
#define FLASH_PROGRAM_SIZE_PAGE       (0x200) // 512-byte

/**
 * @brief  FLASH Memory Type definition
 */
#define FLASH_TYPE_MM (0x00)
#define FLASH_TYPE_SM (0x01)

/**
 * @brief  FLASH Memory Address Mask definition
 */
#define FLASH_MM_ADDRESS_MASK (0x7FFFF)
#define FLASH_SM_ADDRESS_MASK (0x7FF)
#define FLASH_SM_PAGE_MASK    (0x3)

/**
 * @brief  FLASH Boot Flag Operation definition
 */
#define FLASH_CFG_READ  (0x00)
#define FLASH_CFG_WRITE (0x01)

/**
 * @brief  FLASH Boot Flag definition
 */
#define FLASH_FUNC_DISABLE (0x00)
#define FLASH_FUNC_ENABLE  (0X01)

/**
 * @brief  FLASH HW Base ADDRESS
 */
#define FLASH_SECTOR_SIZE  8
#define FLASH_PAGE_SIZE    512
#define FLASH_BUF_BASEADDR 0x0600000
#define FLASH_MSG_BASEADDR 0x0601000

/**
 * @brief  FLASH Magic Number
 */
#define FLASH_MAGIC_NUM 0xAB8EE986

/**
 * @brief  FLASH Map Address
 */
#define FLASH_BOOT_MAP_ADDR 0x0
#define FLASH_USER_MAP_ADDR 0x2000

/**
 * @brief  FLASH Total number
 */
#define FLASH_PAGE_TOTAL   1024U
#define FLASH_SECTOR_TOTAL 128U
/******************  Bit definition for Process Start Control register (0x40) *******************/
#define FLASH_PROC_START_Pos (0U)
#define FLASH_PROC_START_Msk (0x1U << FLASH_PROC_START_Pos) /*!< 0x00000001 */
#define FLASH_PROC_START     FLASH_PROC_START_Msk           /*!<Process start control bit      */

#define IS_FLASH_TYPEPROGRAM(VALUE) (((VALUE) == FLASH_TYPEPROGRAM_BYTE) ||       \
                                     ((VALUE) == FLASH_TYPEPROGRAM_HALFWORD) ||   \
                                     ((VALUE) == FLASH_TYPEPROGRAM_WORD) ||       \
                                     ((VALUE) == FLASH_TYPEPROGRAM_DOUBLEWORD) || \
                                     ((VALUE) == FLASH_TYPEPROGRAM_PAGE))

#define IS_FLASH_TYPEERASE(VALUE) (((VALUE) == FLASH_TYPEERASE_PAGES) ||      \
                                   ((VALUE) == FLASH_TYPEERASE_SUBSECTORS) || \
                                   ((VALUE) == FLASH_TYPEERASE_SECTORS) ||    \
                                   ((VALUE) == FLASH_TYPEERASE_MASSERASE))

#define IS_OPTIONCFG(VALUE) (((VALUE) <= (OPTIONCFG_WRP | OPTIONCFG_RDP | OPTIONCFG_BOOT | OPTIONCFG_CLKFREQ | OPTIONCFG_AUTOHOLD | OPTIONCFG_SMWRP)))

#define IS_FLASH_TYPE(TYPE) (((TYPE) == FLASH_TYPE_MM) || \
                             ((TYPE) == FLASH_TYPE_SM))

#define IS_FLASH_MM_ADDRESS(ADDRESS) ((((ADDRESS) >= FLASH_MM_BASE) && ((ADDRESS) <= FLASH_MM_END)))

/* Protect SM page 0-2 to program and erase */
#define IS_FLASH_ADDRESS(ADDRESS) (IS_FLASH_MM_ADDRESS(ADDRESS))

#define IS_FLASH_NBSECTORS(NBSECTORS) (((NBSECTORS) != 0) && ((NBSECTORS) <= FLASH_SECTOR_TOTAL))

#define IS_FLASH_NBPAGES(NBPAGES) ((((NBPAGES) != 0) && ((NBPAGES) <= FLASH_PAGE_TOTAL)) || (NBPAGES == FLASH_SM_PAGE3_NB))

/**
 * @brief  Enable the specified FLASH interrupt.
 * @param  __INTERRUPT__  FLASH interrupt
 *         This parameter can be any combination of the following values:
 *     @arg FLASH_IT_EOP: End of FLASH Operation Interrupt
 * @retval none
 */
#define __HAL_FLASH_ENABLE_IT(__INTERRUPT__) (SN_FLASH->IE |= (__INTERRUPT__))

/**
 * @brief  Disable the specified FLASH interrupt.
 * @param  __INTERRUPT__  FLASH interrupt
 *         This parameter can be any combination of the following values:
 *     @arg FLASH_IT_EOP: End of FLASH Operation Interrupt
 * @retval none
 */
#define __HAL_FLASH_DISABLE_IT(__INTERRUPT__) (SN_FLASH->IE &= ~(uint32_t)(__INTERRUPT__))

/**
 * @brief  Get the specified FLASH flag status.
 * @param  __FLAG__ specifies the FLASH flags to check.
 *          This parameter can be any combination of the following values:
 *            @arg FLASH_STATUS_PASS   : FLASH Success of Operation flag
 *            @arg FLASH_STATUS_INVALID_ADDR : Flash Invalid page address
 *            @arg FLASH_STATUS_INVALID_TYPE: Flash Invalid memory type
 *            @arg FLASH_STATUS_ILLEGAL_SM_OP: Flash Illegal SM operation
 *            @arg FLASH_STATUS_ERASE_FAILED: Flash Erase failed
 *            @arg FLASH_STATUS_PROGRAM_FAILED: Flash Program failed
 *            @arg FLASH_STATUS_RDP_CHANGE_FAILED : Flash Read protect change failed
 *            @arg FLASH_STATUS_WRP_EN_DIS_FAILED   : Flash Write protect enable/disable failed
 *            @arg FLASH_STATUS_ARB_WRITE_FAILED   : Flash Arbitary write failed
 *            @arg FLASH_STATUS_INVALID_MAGIC_NUM   : Flash Invalid magic number
 *            @arg FLASH_STATUS_BOOTFLAG_WRITE_FAILED   : Flash Write boot flag failed
 *            @arg FLASH_STATUS_CUSTOMER_USE_WRITE_FAILED   : Flash Write customer-used data failed
 *            @arg FLASH_STATUS_INVALID_OFFSET   : Flash Invalid offset
 *            @arg FLASH_STATUS_INVALID_LENGTH   : Flash Invalid length
 *            @arg FLASH_STATUS_FAILURE   : Flash Failed of Operation flag
 * @retval The new state of __FLAG__ (SET or RESET).
 */
#define __HAL_FLASH_GET_FLAG(__FLAG__) ((SN_FLASH->MSG3 & (__FLAG__)))

/**
 * @brief  Get the specified FLASH busy status.
 * @param  __STATUS__ specifies the FLASH status to check.
 *          This parameter can be any combination of the following values:
 *            @arg FLASH_PROC_STATUS_BUSY   : FLASH is BYSU or IDLE
 * @retval The new state of BUSY (BUSY or IDLE).
 */
#define __HAL_FLASH_GET_BUSY(__BUSY__) ((SN_FLASH->PROC_BUSY & (__BUSY__)))

/**
 * @brief  Get the specified FLASH IST status.
 * @param  __STATUS__ specifies the FLASH status to check.
 *          This parameter can be any combination of the following values:
 *            @arg FLASH_STATUS_DONE   : FLASH Process is done
 *            @arg FLASH_STATUS_FAIL   : FLASH Process is done but failed
 * @retval The new state of STATUS (Set or RESET).
 */
#define __HAL_FLASH_GET_STATUS(__STATUS__) ((SN_FLASH->IST & (__STATUS__)))

/**
 * @brief  Clear the specified FLASH IST status.
 * @param  __STATUS__ specifies the FLASH status to clear.
 *          This parameter can be any combination of the following values:
 *            @arg FLASH_STATUS_DONE   : FLASH Process is done
 *            @arg FLASH_STATUS_FAIL   : FLASH Process is done but failed
 * @retval none
 */
#define __HAL_FLASH_CLEAR_STATUS(__STATUS__) (SN_FLASH->IST = (__STATUS__))

/*******************Private function*******************************/
HAL_Status_T FLASH_WaitForLastOperation(uint32_t Timeout);
void         FLASH_MassErase(void);
void         FLASH_Erase_Sector(uint32_t Sector);
void         FLASH_Erase_Page(uint32_t Sector);

#endif /* _SN34F78X_HAL_FLASH_H_ */
