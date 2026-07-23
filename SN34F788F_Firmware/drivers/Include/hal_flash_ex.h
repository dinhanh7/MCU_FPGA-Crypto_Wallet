/***********************
 * SN34F78X SDK V2.1.4 *
 ***********************/

#ifndef _HAL_FLASH_EX_H_
#define _HAL_FLASH_EX_H_

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "sn34f78x_hal_def.h"

/* Exported types ------------------------------------------------------------*/
/**
 * @brief Flash Erase Structure Definition
 * \ingroup flash_struct_define
 * @{
 */
typedef struct
{
    uint32_t TypeErase; /*!< Mass erase or sector Erase.
                             This parameter can be a value of FLASHEx_Type_Erase */
    uint32_t Sector;    /*!< Initial FLASH sector to erase when Mass erase is disabled
                             This parameter must be a value of FLASHEx_Sectors */
    uint32_t NbSectors; /*!< Number of sectors to be erased.
                             This parameter must be a value between 1 and (max number of sectors - value of Initial sector) */
    uint32_t Page;      /*!< Initial FLASH page to erase when Mass erase is disabled
                             This parameter must be a value of FLASHEx_Sectors */
    uint32_t NbPages;   /*!< Number of pages to be erased.
                             This parameter must be a value between 1 and (max number of sectors - value of Initial sector) */
} FLASH_EraseInitTypeDef;
/**
 * @}
 */

/**
 * @brief Flash ProgramOption Structure Definition
 * \ingroup flash_struct_define
 * @{
 */
typedef struct
{
    uint32_t OptionType;    /*!< Option byte to be configured.
                                 This parameter can be a value of FLASHEx_Option_Type */
    uint32_t WRPState;      /*!< Write protection activation or deactivation.
                                 This parameter can be a value of FLASHEx_WRP_State */
    uint32_t SMWRPState;    /*!< Write protection activation or deactivation.
                                 This parameter can be a value of FLASHEx_WRP_State */
    uint8_t  WRPALLZone[8]; /*!< Write protection Status */
    uint32_t WRPZone;       /*!< MM Write protection Zone config */
    uint8_t  SMWRP;         /*!< SM Write protection page */
    uint32_t RDPLevel;      /*!< Set the read protection level.
                                 This parameter can be a value of FLASHEx_Option_Bytes_Read_Protection */
    uint8_t  BOOTFlag;      /*!< BOOT Config */
    uint32_t CLKFreq;       /*!< Wait State Config */
    uint8_t  AUTOHold;      /*!< Auto Hold Config */
} FLASH_ConfigProgramInitTypeDef;
/**
 * @}
 */

/* Exported constants --------------------------------------------------------*/
/**
 * \defgroup flash_memory_map Flash Memory Map Definition
 * \ingroup flash_control
 * @{
 */
#define FLASH_MM_BASE       0x00000000U                      /*!< FLASH(up to 1 MB) base address in the alias region          */
#define FLASH_MM_END        0x0007FFFFU                      /*!< FLASH end address                                           */
#define FLASH_SM_PAGE0_BASE 0x00400000U                      /*!< Base address of : (up to 528 Bytes) embedded FLASH OTP Area */
#define FLASH_SM_PAGE1_BASE 0x00400200U                      /*!< Base address of : (up to 528 Bytes) embedded FLASH OTP Area */
#define FLASH_SM_PAGE1_END  0x004003FFU                      /*!< Base address of : (up to 528 Bytes) embedded FLASH OTP Area */
#define FLASH_SM_PAGE2_BASE 0x00400428U                      /*!< Base address of : (up to 528 Bytes) embedded FLASH OTP Area */
#define FLASH_SM_PAGE2_END  0x004005FFU                      /*!< Base address of : (up to 528 Bytes) embedded FLASH OTP Area */
#define FLASH_SM_PAGE3_BASE 0x00400600U                      /*!< End address of : (up to 528 Bytes) embedded FLASH OTP Area  */
#define FLASH_SM_PAGE3_END  0x004007FFU                      /*!< End address of : (up to 528 Bytes) embedded FLASH OTP Area  */
#define FLASH_SM_PAGE3_NB   ((FLASH_SM_PAGE3_BASE >> 9) + 1) /*!< SM Page 3 page number	                                      */
/**
 * @}
 */

/**
 * \defgroup flash_erase_type Flash Erase Type Definition
 * @ingroup flash_control
 * @{
 */
#define FLASH_TYPEERASE_PAGES      0x00000000U /*!< Pages erase only            */
#define FLASH_TYPEERASE_SUBSECTORS 0x00000001U /*!< SubSectors erase only       */
#define FLASH_TYPEERASE_SECTORS    0x00000002U /*!< Sectors erase only          */
#define FLASH_TYPEERASE_MASSERASE  0x00000003U /*!< Flash Mass erase activation */
/**
 * @}
 */

/**
 * \defgroup flash_wrp_state Flash WRP State Definition
 * @ingroup flash_control
 * @{
 */
#define WRPSTATE_DISABLE 0x00000000U /*!< Disable the write protection of the desired bank 1 sectors */
#define WRPSTATE_ENABLE  0x00000001U /*!< Enable the write protection of the desired bank 1 sectors  */
/**
 * @}
 */

/**
 * \defgroup flash_option Flash Option Definition
 * \ingroup flash_control
 * @{
 */
#define OPTIONCFG_WRP      0x00000001U /*!< MM WRP option byte configuration   */
#define OPTIONCFG_SMWRP    0x00000002U /*!< SM WRP option byte configuration   */
#define OPTIONCFG_BOOT     0x00000004U /*!< BOOT option byte configuration     */
#define OPTIONCFG_CLKFREQ  0x00000008U /*!< CLKFREQ option byte configuration  */
#define OPTIONCFG_AUTOHOLD 0x00000010U /*!< AUTOHOLD option byte configuration */
#define OPTIONCFG_RDP      0x00000020U /*!< RDP option byte configuration      */
/**
 * @}
 */

/**
 * \defgroup flash_read_protection Flash Read Protection Definition
 * \ingroup flash_control
 * @{
 */
#define RDP_LEVEL_0 ((uint8_t)0x00) /*!< Read protection Level 0 */
#define RDP_LEVEL_1 ((uint8_t)0x5A) /*!< Read protection Level 1 */
#define RDP_LEVEL_2 ((uint8_t)0xA5) /*!< Warning: When enabling read protection level 2 it s no more possible to go back to level 1 or 0 */
/**
 * @}
 */

/**
 * \defgroup flash_boot_flag Flash Boot Flag Definition
 * \ingroup flash_control
 * @{
 */
#define FLASH_BOOT_FROM_ROM  (0x00) /*!< BOOT from boot rom */
#define FLASH_BOOT_FROM_USER (0x5A) /*!< BOOT from user rom */
/**
 * @}
 */

/**
 * @brief Flash Write Protect Option Zone Definition
 * \ingroup flash_control
 * @{
 */
typedef enum
{
    HAL_FLASH_MM_WRPZone_0,           /*!< Write Protect Zone 0 */
    HAL_FLASH_MM_WRPZone_1,           /*!< Write Protect Zone 1 */
    HAL_FLASH_MM_WRPZone_2,           /*!< Write Protect Zone 2 */
    HAL_FLASH_MM_WRPZone_3,           /*!< Write Protect Zone 3 */
    HAL_FLASH_MM_WRPZone_4,           /*!< Write Protect Zone 4 */
    HAL_FLASH_MM_WRPZone_5,           /*!< Write Protect Zone 5 */
    HAL_FLASH_MM_WRPZone_6,           /*!< Write Protect Zone 6 */
    HAL_FLASH_MM_WRPZone_7,           /*!< Write Protect Zone 7 */
    HAL_FLASH_MM_WRPZone_8,           /*!< Write Protect Zone 8 */
    HAL_FLASH_MM_WRPZone_9,           /*!< Write Protect Zone 9 */
    HAL_FLASH_MM_WRPZone_10,          /*!< Write Protect Zone 10 */
    HAL_FLASH_MM_WRPZone_11,          /*!< Write Protect Zone 11 */
    HAL_FLASH_MM_WRPZone_12,          /*!< Write Protect Zone 12 */
    HAL_FLASH_MM_WRPZone_13,          /*!< Write Protect Zone 13 */
    HAL_FLASH_MM_WRPZone_14,          /*!< Write Protect Zone 14 */
    HAL_FLASH_MM_WRPZone_15,          /*!< Write Protect Zone 15 */
    HAL_FLASH_MM_WRPZone_16,          /*!< Write Protect Zone 16 */
    HAL_FLASH_MM_WRPZone_17,          /*!< Write Protect Zone 17 */
    HAL_FLASH_MM_WRPZone_18,          /*!< Write Protect Zone 18 */
    HAL_FLASH_MM_WRPZone_19,          /*!< Write Protect Zone 19 */
    HAL_FLASH_MM_WRPZone_20,          /*!< Write Protect Zone 20 */
    HAL_FLASH_MM_WRPZone_21,          /*!< Write Protect Zone 21 */
    HAL_FLASH_MM_WRPZone_22,          /*!< Write Protect Zone 22 */
    HAL_FLASH_MM_WRPZone_23,          /*!< Write Protect Zone 23 */
    HAL_FLASH_MM_WRPZone_24,          /*!< Write Protect Zone 24 */
    HAL_FLASH_MM_WRPZone_25,          /*!< Write Protect Zone 25 */
    HAL_FLASH_MM_WRPZone_26,          /*!< Write Protect Zone 26 */
    HAL_FLASH_MM_WRPZone_27,          /*!< Write Protect Zone 27 */
    HAL_FLASH_MM_WRPZone_28,          /*!< Write Protect Zone 28 */
    HAL_FLASH_MM_WRPZone_29,          /*!< Write Protect Zone 29 */
    HAL_FLASH_MM_WRPZone_30,          /*!< Write Protect Zone 30 */
    HAL_FLASH_MM_WRPZone_31,          /*!< Write Protect Zone 31 */
    HAL_FLASH_MM_WRPZone_32,          /*!< Write Protect Zone 32 */
    HAL_FLASH_MM_WRPZone_33,          /*!< Write Protect Zone 33 */
    HAL_FLASH_MM_WRPZone_34,          /*!< Write Protect Zone 34 */
    HAL_FLASH_MM_WRPZone_35,          /*!< Write Protect Zone 35 */
    HAL_FLASH_MM_WRPZone_36,          /*!< Write Protect Zone 36 */
    HAL_FLASH_MM_WRPZone_37,          /*!< Write Protect Zone 37 */
    HAL_FLASH_MM_WRPZone_38,          /*!< Write Protect Zone 38 */
    HAL_FLASH_MM_WRPZone_39,          /*!< Write Protect Zone 39 */
    HAL_FLASH_MM_WRPZone_40,          /*!< Write Protect Zone 40 */
    HAL_FLASH_MM_WRPZone_41,          /*!< Write Protect Zone 41 */
    HAL_FLASH_MM_WRPZone_42,          /*!< Write Protect Zone 42 */
    HAL_FLASH_MM_WRPZone_43,          /*!< Write Protect Zone 43 */
    HAL_FLASH_MM_WRPZone_44,          /*!< Write Protect Zone 44 */
    HAL_FLASH_MM_WRPZone_45,          /*!< Write Protect Zone 45 */
    HAL_FLASH_MM_WRPZone_46,          /*!< Write Protect Zone 46 */
    HAL_FLASH_MM_WRPZone_47,          /*!< Write Protect Zone 47 */
    HAL_FLASH_MM_WRPZone_48,          /*!< Write Protect Zone 48 */
    HAL_FLASH_MM_WRPZone_49,          /*!< Write Protect Zone 49 */
    HAL_FLASH_MM_WRPZone_50,          /*!< Write Protect Zone 50 */
    HAL_FLASH_MM_WRPZone_51,          /*!< Write Protect Zone 51 */
    HAL_FLASH_MM_WRPZone_52,          /*!< Write Protect Zone 52 */
    HAL_FLASH_MM_WRPZone_53,          /*!< Write Protect Zone 53 */
    HAL_FLASH_MM_WRPZone_54,          /*!< Write Protect Zone 54 */
    HAL_FLASH_MM_WRPZone_55,          /*!< Write Protect Zone 55 */
    HAL_FLASH_MM_WRPZone_56,          /*!< Write Protect Zone 56 */
    HAL_FLASH_MM_WRPZone_57,          /*!< Write Protect Zone 57 */
    HAL_FLASH_MM_WRPZone_58,          /*!< Write Protect Zone 58 */
    HAL_FLASH_MM_WRPZone_59,          /*!< Write Protect Zone 59 */
    HAL_FLASH_MM_WRPZone_60,          /*!< Write Protect Zone 60 */
    HAL_FLASH_MM_WRPZone_61,          /*!< Write Protect Zone 61 */
    HAL_FLASH_MM_WRPZone_62,          /*!< Write Protect Zone 62 */
    HAL_FLASH_MM_WRPZone_63,          /*!< Write Protect Zone 63 */
    HAL_FLASH_MM_WRPZone_MAX,         /*!< Write Protect Zone Max */
    HAL_FLASH_MM_WRPZone_ALL = 0xFFFF /*!< Write Protect Zone All */
} HAL_FLASH_MM_WRPZoneDef;            /*!< RDP option byte configuration */
/**
 * @}
 */

/**
 * @brief Flash SM Write Protect Option Zone Definition
 * \ingroup flash_struct_define
 * @{
 */
typedef enum
{
    HAL_FLASH_SM_WRPPage_0,         /*!< Write Protect Page 0 */
    HAL_FLASH_SM_WRPPage_1,         /*!< Write Protect Page 1 */
    HAL_FLASH_SM_WRPPage_2,         /*!< Write Protect Page 2 */
    HAL_FLASH_SM_WRPPage_3,         /*!< Write Protect Page 3 */
    HAL_FLASH_SM_WRPPage_MAX,       /*!< Write Protect Page Max */
    HAL_FLASH_SM_WRPPage_ALL = 0xFF /*!< Write Protect Page All */
} HAL_FLASH_SM_WRPPageDef;          /*!< Write Protect Page Define */
/**
 * @}
 */

/* Exported functions --------------------------------------------------------*/
HAL_Status_T HAL_FLASHEx_Erase(FLASH_EraseInitTypeDef *pEraseInit, uint32_t *SectorError);
HAL_Status_T HAL_FLASHEx_Erase_IT(FLASH_EraseInitTypeDef *pEraseInit);
HAL_Status_T HAL_FLASHEx_ConfigProgram(FLASH_ConfigProgramInitTypeDef *pCfgInit);
HAL_Status_T HAL_FLASHEx_Erase_UserROM(void);
void         HAL_FLASHEx_GetConfig(FLASH_ConfigProgramInitTypeDef *pCfgInit);
HAL_Status_T HAL_FLASHEx_ChecksumCalculate(uint32_t Address, uint32_t Size, uint16_t *CheckSum);

#ifdef __cplusplus
}
#endif

#endif /* _HAL_FLASH_EX_H_ */
