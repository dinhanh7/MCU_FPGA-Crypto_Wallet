/***********************
 * SN34F78X SDK V2.1.4 *
 ***********************/

#ifndef _SN34F78X_LL_FLASH_H_
#define _SN34F78X_LL_FLASH_H_

/* Includes ------------------------------------------------------------------*/
#include "sn34f78x_ll.h"

/* Private types -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/**
 * @brief  FLASH Procedure structure definition
 */
typedef enum
{
    LL_FLASH_PROC_NONE = 0U,
    LL_FLASH_PROC_PAGEERASE,
    LL_FLASH_PROC_SECTERASE,
    LL_FLASH_PROC_MASSERASE,
    LL_FLASH_PROC_PROGRAM
} LL_FLASH_Procedure_T;

/**
 * @brief  FLASH handle Structure definition
 */
typedef struct
{
    __IO LL_FLASH_Procedure_T ProcedureOnGoing; /*Internal variable to indicate which procedure is ongoing or not in IT context */
    __IO uint32_t             NbSectorsToErase; /*Internal variable to save the remaining sectors to erase in IT context */
    __IO uint32_t             Sector;           /*Internal variable to define the current sector which is erasing */
    __IO uint32_t             NbPagesToErase;   /*Internal variable to save the remaining sectors to erase in IT context */
    __IO uint32_t             Page;             /*Internal variable to define the current sector which is erasing */
    __IO uint32_t             Address;          /*Internal variable to save address selected for program */
    __IO uint32_t             MapAddress;       /*Internal variable to save remap address for program */
    __IO uint32_t             ErrorCode;        /* FLASH error code */

} LL_FLASH_Process_T;
/* Private constants ---------------------------------------------------------*/
/* Private macros ------------------------------------------------------------*/

#define LL_FLASH_PAGE_TOTAL   1024U
#define LL_FLASH_SECTOR_TOTAL 128U

/**
 * @brief  FLASH Memory Address Mask definition
 */
#define LL_FLASH_MM_ADDRESS_MASK (0x7FFFF)
#define LL_FLASH_SM_ADDRESS_MASK (0x7FF)
#define LL_FLASH_SM_PAGE_MASK    (0x3)

#define IS_LL_FLASH_TYPE_ERASE(VALUE) (((VALUE) == LL_FLASH_TYPE_ERASE_PAGES) ||       \
                                       ((VALUE) == LL_FLASH_TYPE_ERASE_SUB_SECTORS) || \
                                       ((VALUE) == LL_FLASH_TYPE_ERASE_SECTORS) ||     \
                                       ((VALUE) == LL_FLASH_TYPE_ERASE_MASS))

#define IS_LL_FLASH_TYPE_PROGRAM(VALUE) (((VALUE) == LL_FLASH_PROGRAM_TYPE_BYTE) ||       \
                                         ((VALUE) == LL_FLASH_PROGRAM_TYPE_HALFWORD) ||   \
                                         ((VALUE) == LL_FLASH_PROGRAM_TYPE_WORD) ||       \
                                         ((VALUE) == LL_FLASH_PROGRAM_TYPE_DOUBLEWORD) || \
                                         ((VALUE) == LL_FLASH_PROGRAM_TYPE_PAGE))

#define IS_LL_OPTION_CFG(VALUE) (((VALUE) <= (LL_OPTION_WRP | LL_OPTION_RDP | LL_OPTION_BOOT | \
                                              LL_OPTION_CLKFREQ | LL_OPTION_AUTOHOLD | LL_OPTION_SMWRP)))

#define IS_LL_WRPSTATE(VALUE) (((VALUE) == LL_WRPSTATE_DISABLE) || \
                               ((VALUE) == LL_WRPSTATE_ENABLE))

#define IS_LL_FLASH_TYPE(TYPE) (((TYPE) == LL_FLASH_TYPE_MM) || \
                                ((TYPE) == LL_FLASH_TYPE_SM))

#define IS_LL_RDP_LEVEL(LEVEL) (((LEVEL) == LL_RDP_LEVEL_0) || \
                                ((LEVEL) == LL_RDP_LEVEL_1) || \
                                ((LEVEL) == LL_RDP_LEVEL_2))

#define IS_LL_FLASH_SECTOR_NUM(SECTOR_NUM) (((SECTOR_NUM) != 0) && ((SECTOR_NUM) <= LL_FLASH_SECTOR_TOTAL))

#define IS_LL_FLASH_SM_ADDRESS(ADDRESS) ((((ADDRESS) >= LL_FLASH_SM_PAGE0_BASE) && ((ADDRESS) <= LL_FLASH_SM_PAGE1_END)) || \
                                         (((ADDRESS) >= LL_FLASH_SM_PAGE2_BASE) && ((ADDRESS) <= LL_FLASH_SM_PAGE3_END)))

#define IS_LL_FLASH_MM_ADDRESS(ADDRESS)       ((((ADDRESS) >= LL_FLASH_MM_BASE) && ((ADDRESS) <= LL_FLASH_MM_END)))
#define IS_LL_FLASH_SM_PAGE2_ADDRESS(ADDRESS) (((ADDRESS) >= LL_FLASH_SM_PAGE2_BASE) && ((ADDRESS) <= LL_FLASH_SM_PAGE2_END))
#define IS_LL_FLASH_SM_PAGE3_ADDRESS(ADDRESS) (((ADDRESS) >= LL_FLASH_SM_PAGE3_BASE) && ((ADDRESS) <= LL_FLASH_SM_PAGE3_END))
#define IS_LL_FLASH_ADDRESS(ADDRESS)          ((IS_LL_FLASH_MM_ADDRESS(ADDRESS)) || (IS_LL_FLASH_SM_PAGE3_ADDRESS(ADDRESS)))

/**
 * @brief  FLASH Map Address
 */
#define LL_FLASH_BOOT_MAP_ADDR 0x0
#define LL_FLASH_USER_MAP_ADDR 0x2000

/**
 * @brief  FLASH timeout value
 */
#define LL_FLASH_TIMEOUT_VALUE 50000U /* 50 s */

/**
 * @brief  FLASH Magic Number
 */
#define LL_FLASH_MAGIC_NUM 0xAB8EE986

/**
 * @brief  FLASH HW Base ADDRESS
 */
#define LL_FLASH_SECTOR_SIZE  8
#define LL_FLASH_PAGE_SIZE    512
#define LL_FLASH_BUF_BASEADDR 0x0600000
#define LL_FLASH_MSG_BASEADDR 0x0601000

/**
 * @brief  FLASH Boot Flag definition
 */
#define LL_FLASH_FUNC_DISABLE (0x00)
#define LL_FLASH_FUNC_ENABLE  (0X01)

/** @brief FLASH_Write_protect_Option_Zone_Definition FLASH Write protect Option Zone Definition
 * @ingroup flash_control
 * @{
 */
typedef enum
{
    LL_FLASH_MM_WRPZone_0,
    LL_FLASH_MM_WRPZone_1,
    LL_FLASH_MM_WRPZone_2,
    LL_FLASH_MM_WRPZone_3,
    LL_FLASH_MM_WRPZone_4,
    LL_FLASH_MM_WRPZone_5,
    LL_FLASH_MM_WRPZone_6,
    LL_FLASH_MM_WRPZone_7,
    LL_FLASH_MM_WRPZone_8,
    LL_FLASH_MM_WRPZone_9,
    LL_FLASH_MM_WRPZone_10,
    LL_FLASH_MM_WRPZone_11,
    LL_FLASH_MM_WRPZone_12,
    LL_FLASH_MM_WRPZone_13,
    LL_FLASH_MM_WRPZone_14,
    LL_FLASH_MM_WRPZone_15,
    LL_FLASH_MM_WRPZone_16,
    LL_FLASH_MM_WRPZone_17,
    LL_FLASH_MM_WRPZone_18,
    LL_FLASH_MM_WRPZone_19,
    LL_FLASH_MM_WRPZone_20,
    LL_FLASH_MM_WRPZone_21,
    LL_FLASH_MM_WRPZone_22,
    LL_FLASH_MM_WRPZone_23,
    LL_FLASH_MM_WRPZone_24,
    LL_FLASH_MM_WRPZone_25,
    LL_FLASH_MM_WRPZone_26,
    LL_FLASH_MM_WRPZone_27,
    LL_FLASH_MM_WRPZone_28,
    LL_FLASH_MM_WRPZone_29,
    LL_FLASH_MM_WRPZone_30,
    LL_FLASH_MM_WRPZone_31,
    LL_FLASH_MM_WRPZone_32,
    LL_FLASH_MM_WRPZone_33,
    LL_FLASH_MM_WRPZone_34,
    LL_FLASH_MM_WRPZone_35,
    LL_FLASH_MM_WRPZone_36,
    LL_FLASH_MM_WRPZone_37,
    LL_FLASH_MM_WRPZone_38,
    LL_FLASH_MM_WRPZone_39,
    LL_FLASH_MM_WRPZone_40,
    LL_FLASH_MM_WRPZone_41,
    LL_FLASH_MM_WRPZone_42,
    LL_FLASH_MM_WRPZone_43,
    LL_FLASH_MM_WRPZone_44,
    LL_FLASH_MM_WRPZone_45,
    LL_FLASH_MM_WRPZone_46,
    LL_FLASH_MM_WRPZone_47,
    LL_FLASH_MM_WRPZone_48,
    LL_FLASH_MM_WRPZone_49,
    LL_FLASH_MM_WRPZone_50,
    LL_FLASH_MM_WRPZone_51,
    LL_FLASH_MM_WRPZone_52,
    LL_FLASH_MM_WRPZone_53,
    LL_FLASH_MM_WRPZone_54,
    LL_FLASH_MM_WRPZone_55,
    LL_FLASH_MM_WRPZone_56,
    LL_FLASH_MM_WRPZone_57,
    LL_FLASH_MM_WRPZone_58,
    LL_FLASH_MM_WRPZone_59,
    LL_FLASH_MM_WRPZone_60,
    LL_FLASH_MM_WRPZone_61,
    LL_FLASH_MM_WRPZone_62,
    LL_FLASH_MM_WRPZone_63,
    LL_FLASH_MM_WRPZone_MAX,
    LL_FLASH_MM_WRPZone_ALL = 0xFFFF
} LL_FLASH_MM_WRPZoneDef; /*!< RDP option byte configuration */
/**
 * @}
 */

/** @brief FLASH_SM_Write_protect_Option_Zone_Definition FLASH SM Write protect Option Zone Definition
 * @{
 */
typedef enum
{
    LL_FLASH_SM_WRPPage_0,
    LL_FLASH_SM_WRPPage_1,
    LL_FLASH_SM_WRPPage_2,
    LL_FLASH_SM_WRPPage_3,
    LL_FLASH_SM_WRPPage_MAX,
    LL_FLASH_SM_WRPPage_ALL = 0xFF
} LL_FLASH_SM_WRPPageDef;
/**
 * @}
 */

/* Private functions prototypes ----------------------------------------------*/
LL_Status_T _wait_for_last_operation(uint32_t timeout);

LL_Status_T _flash_protect_get_status(uint8_t *wrp_page, uint8_t *sm_wrp, uint32_t *rdp_level);
uint16_t    _flash_get_mm_wrp(uint8_t *wrp_page);
uint16_t    _flash_get_sm_wrp(uint8_t *sm_wrp);
uint16_t    _flash_get_rdp(uint32_t *rdp_level);
uint16_t    _flash_get_boot_flag(uint8_t *boot_flag);
uint16_t    _flash_get_clk_freq(uint32_t *clk_freq);
uint16_t    _flash_get_auto_hold(uint8_t *auto_hold);

void _flash_erase_sector(uint32_t sector);
void _flash_erase_page(uint32_t page);
void _flash_erase_mass(void);

LL_Status_T _flash_enable_wrp(uint32_t wrp_type, uint8_t *wrp_page);
LL_Status_T _flash_disable_wrp(uint32_t wrp_type, uint8_t *wrp_page);

LL_Status_T _flash_boot_flag_config(uint8_t boot_flag);
LL_Status_T _flash_auto_hold_config(uint8_t auto_hold);
LL_Status_T _flash_clk_freq_config(uint32_t clk_freq);
LL_Status_T _flash_rdp_level_config(uint8_t level);

#endif /* _SN34F78X_LL_FLASH_H_ */
