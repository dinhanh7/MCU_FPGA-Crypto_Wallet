/***********************
 * SN34F78X SDK V2.1.4 *
 ***********************/

#ifndef _LL_FLASH_H_
#define _LL_FLASH_H_

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "sn34f78x_ll_def.h"
#include "ll_reg_msk.h"

/* Exported types ------------------------------------------------------------*/
/**
 * \brief FLASH Erase structure definition
 * \ingroup flash_struct_define
 */

typedef struct
{
    uint32_t type_erase; /*!< Mass erase or sector Erase.
                             This parameter can be a value of FLASHEx_Type_Erase */
    uint32_t sector;     /*!< Initial FLASH sector to erase when Mass erase is disabled
                              sector = Start_addr / sector_size (sector_size is custom) */
    uint32_t sector_num; /*!< Number of sectors to be erased.
                             This parameter must be a value between 1 and (max number of sectors - value of Initial sector) */
    uint32_t page;       /*!< Initial FLASH page to erase when Mass erase is disabled
                              page = Start_addr / page_size (page_size is custom) */
    uint32_t page_num;   /*!< Number of pages to be erased.
                             This parameter must be a value between 1 and (max number of sectors - value of Initial sector) */
} LL_FLASH_Config_EraseInit_T;

/**
 * @brief  FLASH Option Config structure definition
 * \ingroup flash_struct_define
 */
typedef struct
{
    uint32_t option_type;     /*!< Option byte to be configured.
                                This parameter can be a value of FLASHEx_Option_Type */
    uint32_t wrp_state;       /*!< Write protection activation or deactivation.
                                  This parameter can be a value of FLASHEx_WRP_State */
    uint32_t sm_wrp_state;    /*!< Write protection activation or deactivation.
                                 This parameter can be a value of FLASHEx_WRP_State */
    uint8_t  wrp_all_zone[8]; /*!< Write protection Status */
    uint32_t wrp_zone;        /*!< MM Write protection Zone config */
    uint8_t  sm_wrp;          /*!< SM Write protection page */
    uint32_t rdp_level;       /*!< Set the read protection level.
                                  This parameter can be a value of flash_option_bytes_read_protection */
    uint8_t  boot_flag;       /*!< BOOT Config , \ref flash_boot_flag_definition */
    uint32_t clk_freq;        /*!< Wait State Config */
    uint8_t  auto_hold;       /*!< Auto Hold Config */
} LL_FLASH_Config_ProgramInit_T;

/**
 * \defgroup flash_type_erase FLASH Type Erase
 * \ingroup flash_control
 * @{
 */
#define LL_FLASH_TYPE_ERASE_PAGES       0x00000000U /*!< Pages erase only            */
#define LL_FLASH_TYPE_ERASE_SUB_SECTORS 0x00000001U /*!< SubSectors erase only       */
#define LL_FLASH_TYPE_ERASE_SECTORS     0x00000002U /*!< Sectors erase only          */
#define LL_FLASH_TYPE_ERASE_MASS        0x00000003U /*!< Flash Mass erase activation */
/**
 * @}
 */

/**
 * \defgroup flash_option_type FLASH Option Type
 * \ingroup flash_control
 * @{
 */
#define LL_OPTION_WRP      0x00000001U /*!< MM WRP option byte configuration   */
#define LL_OPTION_SMWRP    0x00000002U /*!< SM WRP option byte configuration   */
#define LL_OPTION_BOOT     0x00000004U /*!< BOOT option byte configuration     */
#define LL_OPTION_CLKFREQ  0x00000008U /*!< CLKFREQ option byte configuration  */
#define LL_OPTION_AUTOHOLD 0x00000010U /*!< AUTOHOLD option byte configuration */
#define LL_OPTION_RDP      0x00000020U /*!< RDP option byte configuration      */
/**
 * @}
 */

/**
 * \defgroup flash_wrp_state FLASH WRP State
 * \ingroup flash_control
 * @{
 */
#define LL_WRPSTATE_DISABLE 0x00000000U /*!< Disable the write protection of the desired bank 1 sectors */
#define LL_WRPSTATE_ENABLE  0x00000001U /*!< Enable the write protection of the desired bank 1 sectors  */
/**
 * @}
 */

/** @defgroup flash_option_bytes_read_protection FLASH Option Bytes Read Protection
 * @ingroup flash_control
 * @{
 */
#define LL_RDP_LEVEL_0 ((uint8_t)0x00) /*!< Read protection Level 0 */
#define LL_RDP_LEVEL_1 ((uint8_t)0x5A) /*!< Read protection Level 1 */
#define LL_RDP_LEVEL_2 ((uint8_t)0xA5) /*!< Warning: When enabling read protection level 2 it s no more possible to go back to level 1 or 0 */
/**
 * @}
 */

/**
 * \defgroup flash_operation Flash operation
 * \ingroup flash_control
 * @{
 */
#define LL_FLASH_OPERATION_ERASE              (FLASH_MSG0_OPERATION_Erase)          /*!< Erase operation */
#define LL_FLASH_OPERATION_PROGRAM            (FLASH_MSG0_OPERATION_Program)        /*!< Program operation */
#define LL_FLASH_OPERATION_ARB_WRITE          (FLASH_MSG0_OPERATION_W_Arbitrary)    /*!< Arbitrary write operation */
#define LL_FLASH_OPERATION_MM_RDP             (FLASH_MSG0_OPERATION_R_Protect)      /*!< Read protect operation */
#define LL_FLASH_OPERATION_WRP                (FLASH_MSG0_OPERATION_W_Protect)      /*!< Write protect operation */
#define LL_FLASH_OPERATION_PROT_STATUS        (FLASH_MSG0_OPERATION_R_Protect_Stat) /*!< Read protect status operation */
#define LL_FLASH_OPERATION_BOOT_FLAG_RW       (FLASH_MSG0_OPERATION_RW_Boot_Flag)   /*!< Read/Write boot flag operation */
#define LL_FLASH_OPERATION_CUSTOMER_USE_WRITE (FLASH_MSG0_OPERATION_W_Custom)       /*!< customer use write operation */
#define LL_FLASH_OPERATION_FREQ_RW            (FLASH_MSG0_OPERATION_RW_Freq)        /*!< Read/Write FLASH ROM frequency operation */
#define LL_FLASH_OPERATION_CHECKSUM_CAL       (FLASH_MSG0_OPERATION_Checksum)       /*!< Calculate MM checksum operation */
#define LL_FLASH_OPERATION_MUX_SEL            (FLASH_MSG0_OPERATION_MUX_SEL)        /*!< Mux Select operation */
/**
 * @}
 */

/**
 * \defgroup flash_memory_type Flash memory type
 * \ingroup flash_control
 * @{
 */
#define LL_FLASH_TYPE_MM (FLASH_MSG0_TYPE_MM) /*!< Main memory/Read boot flag/Read FLASH ROM frequency */
#define LL_FLASH_TYPE_SM (FLASH_MSG0_TYPE_SM) /*!< Supervisory memory/Write boot flag/Write FLASH ROM frequency*/
/**
 * @}
 */

/**
 * \defgroup flash_memory_range Flash memory range
 * \ingroup flash_control
 * @{
 */
#define LL_FLASH_MEM_RANGE_PAGE       (FLASH_MSG0_RANGE_Page)      /*!< Page (MM and SM) */
#define LL_FLASH_MEM_RANGE_SUB_SECTOR (FLASH_MSG0_RANGE_Subsector) /*!< Subsector (MM only) */
#define LL_FLASH_MEM_RANGE_SECTOR     (FLASH_MSG0_RANGE_Sector)    /*!< Sector (MM only) */
#define LL_FLASH_MEM_RANGE_MASS       (FLASH_MSG0_RANGE_Bulk_All)  /*!< Bulk all (MM only) */
/**
 * @}
 */

/**
 * \defgroup flash_status_code Flash status code
 * \ingroup flash_control
 * @{
 */
#define FLASH_STATUS_PASS                      (0x0000)                                    /*!< Pass */
#define FLASH_STATUS_INVALID_ADDR              (FLASH_MSG3_STATUS_CODE_Invalid_Addr)       /*!< Invalid address */
#define FLASH_STATUS_INVALID_TYPE              (FLASH_MSG3_STATUS_CODE_Invalid_Type)       /*!< Invalid Type */
#define FLASH_STATUS_ILLEGAL_SM_OP             (FLASH_MSG3_STATUS_CODE_Erase_Op)           /*!< Illegal erase operation */
#define FLASH_STATUS_ERASE_FAILED              (FLASH_MSG3_STATUS_CODE_Erase_Failed)       /*!< Erase fail */
#define FLASH_STATUS_PROGRAM_FAILED            (FLASH_MSG3_STATUS_CODE_Program_Failed)     /*!< Program fail */
#define FLASH_STATUS_RDP_CHANGE_FAILED         (FLASH_MSG3_STATUS_CODE_RDP_Change_Failed)  /*!< Read protect change fail */
#define FLASH_STATUS_WRP_EN_DIS_FAILED         (FLASH_MSG3_STATUS_CODE_WRP_En_Dis_Failed)  /*!< Write protect enable/disable fail */
#define FLASH_STATUS_ARB_WRITE_FAILED          (FLASH_MSG3_STATUS_CODE_W_Arbitrary_Failed) /*!< Arbitrary write fail */
#define FLASH_STATUS_INVALID_MAGIC_NUM         (FLASH_MSG3_STATUS_CODE_Invalid_Magic_Num)  /*!< Invalid magic number */
#define FLASH_STATUS_BOOTFLAG_WRITE_FAILED     (FLASH_MSG3_STATUS_CODE_W_Boot_Flag_Failed) /*!< Write boot flag fail */
#define FLASH_STATUS_CUSTOMER_USE_WRITE_FAILED (FLASH_MSG3_STATUS_CODE_W_Custom_Failed)    /*!< Write custom fail */
#define FLASH_STATUS_INVALID_OFFSET            (FLASH_MSG3_STATUS_CODE_Invalid_Offset)     /*!< Invalid offset */
#define FLASH_STATUS_INVALID_LENGTH            (FLASH_MSG3_STATUS_CODE_Invalid_Length)     /*!< Illegal length */
#define FLASH_STATUS_FAILURE                   (FLASH_MSG3_STATUS_CODE_Failure)            /*!< Failure */
/**
 * @}
 */

/**
 * \defgroup flash_memory_map FLASH memory map definition
 * \ingroup flash_control
 * @{
 */
#define LL_FLASH_MM_BASE       0x00000000U                         /*!< FLASH(up to 1 MB) base address in the alias region          */
#define LL_FLASH_MM_END        0x0007FFFFU                         /*!< FLASH end address                                           */
#define LL_FLASH_SM_PAGE0_BASE 0x00400000U                         /*!< Base address of : (up to 528 Bytes) embedded FLASH OTP Area */
#define LL_FLASH_SM_PAGE1_BASE 0x00400200U                         /*!< Base address of : (up to 528 Bytes) embedded FLASH OTP Area */
#define LL_FLASH_SM_PAGE1_END  0x004003FFU                         /*!< Base address of : (up to 528 Bytes) embedded FLASH OTP Area */
#define LL_FLASH_SM_PAGE2_BASE 0x00400428U                         /*!< Base address of : (up to 528 Bytes) embedded FLASH OTP Area */
#define LL_FLASH_SM_PAGE2_END  0x004005FFU                         /*!< Base address of : (up to 528 Bytes) embedded FLASH OTP Area */
#define LL_FLASH_SM_PAGE3_BASE 0x00400600U                         /*!< End address of : (up to 528 Bytes) embedded FLASH OTP Area  */
#define LL_FLASH_SM_PAGE3_END  0x004007FFU                         /*!< End address of : (up to 528 Bytes) embedded FLASH OTP Area  */
#define LL_FLASH_SM_PAGE3_NB   ((LL_FLASH_SM_PAGE3_BASE >> 9) + 1) /*!< SM Page 3 page number	                                      */
/**
 * @}
 */

/** @defgroup flash_boot_flag_definition FLASH Boot Flag definition
 * @ingroup flash_control
 * @{
 */
#define LL_FLASH_BOOT_FROM_ROM  (0x00) /*!< BOOT from boot rom */
#define LL_FLASH_BOOT_FROM_USER (0x5A) /*!< BOOT from user rom */
/**
 * @}
 */

/**
 * \defgroup flash_error_code_define FLASH error type
 * \ingroup flash_control
 * @{
 */
#define LL_FLASH_ERROR_NONE                      0x00000000U /*!< FLASH Success of Operation flag           */
#define LL_FLASH_ERROR_INVALID_ADDR              0x00000001U /*!< Flash Invalid page address                */
#define LL_FLASH_ERROR_INVALID_TYPE              0x00000002U /*!< Flash Invalid memory type                 */
#define LL_FLASH_ERROR_ILLEGAL_SM_OP             0x00000003U /*!< Flash Illegal SM operation                */
#define LL_FLASH_ERROR_ERASE_FAILED              0x00000004U /*!< Flash Erase failed                        */
#define LL_FLASH_ERROR_PROGRAM_FAILED            0x00000005U /*!< Flash Program failed                      */
#define LL_FLASH_ERROR_RDP_CHANGE_FAILED         0x00000006U /*!< Flash Read protect change failed          */
#define LL_FLASH_ERROR_WRP_EN_DIS_FAILED         0x00000007U /*!< Flash Write protect enable/disable failed */
#define LL_FLASH_ERROR_ARB_WRITE_FAILED          0x00000008U /*!< Flash Arbitrary write failed              */
#define LL_FLASH_ERROR_INVALID_MAGIC_NUM         0x00000009U /*!< Flash Invalid magic number                */
#define LL_FLASH_ERROR_BOOTFLAG_WRITE_FAILED     0x0000000AU /*!< Flash Write boot flag failed              */
#define LL_FLASH_ERROR_CUSTOMER_USE_WRITE_FAILED 0x0000000BU /*!< Flash Write customer-used data failed     */
#define LL_FLASH_ERROR_INVALID_OFFSET            0x0000000CU /*!< Flash Invalid offset                      */
#define LL_FLASH_ERROR_INVALID_LENGTH            0x0000000DU /*!< Flash Invalid length                      */
#define LL_FLASH_ERROR_FAILURE                   0x0000FFFFU /*!< Flash Failed of Operation flag            */
/**
 * @}
 */

/* Exported functions --------------------------------------------------------*/

/****************************** FLASH MSG0 register content **************************************/
/**
 * @brief  FMC operation
 * @param  op operation \ref flash_operation
 * @retval None
 */
__STATIC_INLINE void LL_FLASH_SetOperation(uint32_t op)
{
    LL_REG_CBIT(SN_FLASH->MSG0, FLASH_MSG0_OPERATION);
    LL_REG_SBIT(SN_FLASH->MSG0, op);
}

/**
 * @brief  Get FMC operation
 * @retval Operation type
 *         1: Erase operation
 *         2: Program operation
 *         3: Arbitrary write operation
 *         4: Read protect operation
 *         5: Write protect operation
 *         6: Read protect status operation
 *         7: Read/Write boot flag operation
 *         8: customer use write operation
 *         9: Read/Write FLASH ROM frequency operation
 *         10: Calculate MM checksum operation
 *         11: Mux Select operation
 *         other : Reserved
 */
__STATIC_INLINE uint32_t LL_FLASH_GetOperation(void)
{
    return ((LL_REG_READ(SN_FLASH->MSG0) & FLASH_MSG0_OPERATION) >> FLASH_MSG0_OPERATION_Pos);
}

/**
 * @brief  Set Memory type
 * @param  type memory type \ref flash_memory_type
 * @retval None
 */
__STATIC_INLINE void LL_FLASH_SetMemoryType(uint32_t type)
{
    LL_REG_CBIT(SN_FLASH->MSG0, FLASH_MSG0_TYPE);
    LL_REG_SBIT(SN_FLASH->MSG0, type);
}

/**
 * @brief  Get FMC memory type
 * @retval Memory type
 *         0: Main memory/Read boot flag/Read FLASH ROM frequency
 *         1: Supervisory memory/Write boot flag/Write FLASH ROM frequency
 *         other : Reserved
 */
__STATIC_INLINE uint32_t LL_FLASH_GetMemoryType(void)
{
    return ((LL_REG_READ(SN_FLASH->MSG0) & FLASH_MSG0_TYPE) >> FLASH_MSG0_TYPE_Pos);
}

/**
 * @brief  Set Memory range (used only for Erase/Program operation)
 * @param  range memory range \ref flash_memory_range
 * @retval None
 */
__STATIC_INLINE void LL_FLASH_SetMemoryRange(uint32_t range)
{
    LL_REG_CBIT(SN_FLASH->MSG0, FLASH_MSG0_RANGE);
    LL_REG_SBIT(SN_FLASH->MSG0, range);
}

/**
 * @brief  Get FMC memory range
 * @retval Memory range
 *         0: Page (MM and SM)
 *         1: Subsector (MM only)
 *         2: Sector (MM only)
 *         3: Bulk all (MM only)
 */
__STATIC_INLINE uint32_t LL_FLASH_GetMemoryRange(void)
{
    return ((LL_REG_READ(SN_FLASH->MSG0) & FLASH_MSG0_RANGE) >> FLASH_MSG0_RANGE_Pos);
}

/****************************** End FLASH MSG0 register content **************************************/

/****************************** FLASH MSG1 register content **************************************/
/**
 * @brief  Set FMC Message 1
 * @param  message1 message 1, range from 0 to 0xFFFFFFFF
 * @retval None
 */
__STATIC_INLINE void LL_FLASH_SetMessage1(uint32_t message1)
{
    LL_REG_CBIT(SN_FLASH->MSG1, FLASH_MSG1_MSG1);
    LL_REG_SBIT(SN_FLASH->MSG1, message1);
}

/**
 * @brief  Get FMC Message 1
 * @retval Message 1
 */
__STATIC_INLINE uint32_t LL_FLASH_GetMessage1(void)
{
    return ((LL_REG_READ(SN_FLASH->MSG1) & FLASH_MSG1_MSG1) >> FLASH_MSG1_MSG1_Pos);
}

/****************************** End FLASH MSG1 register content **************************************/

/****************************** FLASH MSG2 register content **************************************/
/**
 * @brief  Set FMC Message 2
 * @param  message2 message 2, range from 0 to 0xFFFFFFFF
 * @retval None
 */
__STATIC_INLINE void LL_FLASH_SetMessage2(uint32_t message2)
{
    LL_REG_CBIT(SN_FLASH->MSG2, FLASH_MSG2_MSG2);
    LL_REG_SBIT(SN_FLASH->MSG2, message2);
}

/**
 * @brief  Get FMC Message 2
 * @retval Message 2
 */
__STATIC_INLINE uint32_t LL_FLASH_GetMessage2(void)
{
    return ((LL_REG_READ(SN_FLASH->MSG2) & FLASH_MSG2_MSG2) >> FLASH_MSG2_MSG2_Pos);
}

/****************************** End FLASH MSG2 register content **************************************/

/****************************** FLASH MSG3 register content **************************************/
/**
 * @brief  Set Status code
 * @param  status status code range \ref flash_status_code
 * @retval None
 */
__STATIC_INLINE void LL_FLASH_SetStatusCode(uint16_t status)
{
    LL_REG_CBIT(SN_FLASH->MSG3, FLASH_MSG3_STATUS_CODE);
    LL_REG_SBIT(SN_FLASH->MSG3, status);
}

/**
 * @brief  Get Status code
 * @retval Memory range  \n
 *         0: Pass       \n
 *         1: Invalid page address   \n
 *         2: Invalid memory type   \n
 *         3: Erase operation   \n
 *         4: Erase failed   \n
 *         5: Program failed  \n
 *         6: Read protect change failed  \n
 *         7: Write protect enable/disable failed  \n
 *         8: Arbitrary write failed  \n
 *         9: Invalid magic number  \n
 *         10: Write boot flag failed  \n
 *         11: Write customer-used data failed  \n
 *         12: Invalid offset  \n
 *         13: Invalid length  \n
 *         0xFFFF：Failure  \n
 */
__STATIC_INLINE uint32_t LL_FLASH_GetStatusCode(void)
{
    return ((LL_REG_READ(SN_FLASH->MSG3) & FLASH_MSG3_STATUS_CODE) >> FLASH_MSG3_STATUS_CODE_Pos);
}

/**
 * @brief  Set Status code 2
 * @param  status status code 2, range from 0 to 0xFFFF
 * @retval None
 */
__STATIC_INLINE void LL_FLASH_SetStatusCode2(uint16_t status)
{
    LL_REG_CBIT(SN_FLASH->MSG3, FLASH_MSG3_STATUS_CODE2);
    LL_REG_SBIT(SN_FLASH->MSG3, status << FLASH_MSG3_STATUS_CODE2_Pos);
}

/**
 * @brief  Get Status code 2
 * @retval status code 2
 */
__STATIC_INLINE uint32_t LL_FLASH_GetStatusCode2(void)
{
    return ((LL_REG_READ(SN_FLASH->MSG3) & FLASH_MSG3_STATUS_CODE2) >> FLASH_MSG3_STATUS_CODE2_Pos);
}

/****************************** End FLASH MSG3 register content **************************************/

/****************************** FLASH PROC_START register content **************************************/
/**
 * @brief  Start process
 * @retval None
 */
__STATIC_INLINE void LL_FLASH_StartProcess(void)
{
    LL_REG_SBIT(SN_FLASH->PROC_START, FLASH_PROC_START_PROC_START);
}

/**
 * @brief  Idle or finish process
 * @retval None
 */
__STATIC_INLINE void LL_FLASH_IdleProcess(void)
{
    LL_REG_CBIT(SN_FLASH->PROC_START, FLASH_PROC_START_PROC_START);
}

/**
 * @brief  Check if start process
 * @retval 0 : Idle or finish process
 *         1 : start process
 */
__STATIC_INLINE uint32_t LL_FLASH_IsEnabled_StartProcess(void)
{
    return ((LL_REG_READ(SN_FLASH->PROC_START) & FLASH_PROC_START_PROC_START) >> FLASH_PROC_START_PROC_START_Pos);
}

/****************************** End FLASH PROC_START register content **************************************/

/****************************** FLASH PROC_BUSY register content **************************************/
/**
 * @brief  Judge if process is busy
 * @retval 0 : Process is idle
 *         1 : Process is busy
 */
__STATIC_INLINE uint32_t LL_FLASH_IsActiveFlag_Busy(void)
{
    return ((LL_REG_READ(SN_FLASH->PROC_BUSY) & FLASH_PROC_BUSY_PROC_BUSY) == FLASH_PROC_BUSY_PROC_BUSY);
}

/****************************** End FLASH PROC_BUSY register content **************************************/

/****************************** FLASH IE register content **************************************/
/**
 * @brief  Enable interrupt
 * @retval None
 */
__STATIC_INLINE void LL_FLASH_EnableIT_IE(void)
{
    LL_REG_SBIT(SN_FLASH->IE, FLASH_IE_IE);
}

/**
 * @brief  Disable interrupt
 * @retval None
 */
__STATIC_INLINE void LL_FLASH_DisableIT_IE(void)
{
    LL_REG_CBIT(SN_FLASH->IE, FLASH_IE_IE);
}

/**
 * @brief  Judge if Enabled IT
 * @retval 0 : Disabled
 *         1 : Enabled
 */
__STATIC_INLINE uint32_t LL_FLASH_IsEnabledIT_IE(void)
{
    return ((LL_REG_READ(SN_FLASH->IE) & FLASH_IE_IE) == FLASH_IE_IE);
}

/****************************** End FLASH IE register content **************************************/

/****************************** FLASH IST register content **************************************/
/**
 * @brief  Clear DONE flag
 * @retval None
 */
__STATIC_INLINE void LL_FLASH_ClearFlag_DONE(void)
{
    LL_REG_SBIT(SN_FLASH->IST, FLASH_IST_DONE);
}

/**
 * @brief  Judge if triggered DONE flag
 * @retval 0 : Not Triggered
 *         1 : Triggered
 */
__STATIC_INLINE uint32_t LL_FLASH_IsActiveFlag_DONE(void)
{
    return ((LL_REG_READ(SN_FLASH->IST) & FLASH_IST_DONE) == FLASH_IST_DONE);
}

/**
 * @brief  Clear FAIL flag
 * @retval None
 */
__STATIC_INLINE void LL_FLASH_ClearFlag_FAIL(void)
{
    LL_REG_SBIT(SN_FLASH->IST, FLASH_IST_FAIL);
}

/**
 * @brief  Judge if triggered FAIL flag
 * @retval 0 : Not Triggered
 *         1 : Triggered
 */
__STATIC_INLINE uint32_t LL_FLASH_IsActiveFlag_FAIL(void)
{
    return ((LL_REG_READ(SN_FLASH->IST) & FLASH_IST_FAIL) == FLASH_IST_FAIL);
}

/****************************** End FLASH IST register content **************************************/

/****************************** FLASH AUTO_HOLD register content **************************************/
/**
 * @brief  Enable AHB interface auto_hold
 * @retval None
 */
__STATIC_INLINE void LL_FLASH_Enable_AutoHold(void)
{
    LL_REG_SBIT(SN_FLASH->AUTO_HOLD, FLASH_AUTO_HOLD_AUTO_HOLD);
}

/**
 * @brief  Disable AHB interface auto_hold
 * @retval None
 */
__STATIC_INLINE void LL_FLASH_Disable_AutoHold(void)
{
    LL_REG_CBIT(SN_FLASH->AUTO_HOLD, FLASH_AUTO_HOLD_AUTO_HOLD);
}

/**
 * @brief  Judge if Enabled AHB interface auto_hold
 * @retval 0 : Disabled
 *         1 : Enabled
 */
__STATIC_INLINE uint32_t LL_FLASH_IsEnabled_AutoHold(void)
{
    return ((LL_REG_READ(SN_FLASH->AUTO_HOLD) & FLASH_AUTO_HOLD_AUTO_HOLD) == FLASH_AUTO_HOLD_AUTO_HOLD);
}

/****************************** End FLASH AUTO_HOLD register content **************************************/

#if defined(USE_FULL_LL_DRIVER)
void        LL_InitFlash(void);
LL_Status_T LL_FLASH_ConfigProgram(LL_FLASH_Config_ProgramInit_T *cfg_init);
void        LL_FLASH_GetConfig(LL_FLASH_Config_ProgramInit_T *pro_init);
#endif /* USE_FULL_LL_DRIVER */

#ifdef __cplusplus
}
#endif

#endif /* _LL_FLASH_H_ */
