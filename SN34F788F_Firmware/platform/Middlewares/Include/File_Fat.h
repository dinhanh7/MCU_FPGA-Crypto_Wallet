/***********************
 * SN34F78X SDK V2.1.4 *
 ***********************/

#ifndef _FILE_FAT_H_
#define _FILE_FAT_H_

#include "sn34f78x_hal_def.h"
#include "diskio.h"

//=============================================================================
//                  Macro Definition
//=============================================================================
#define FATFS_LABEL_LEN    (12)     /**< Partition Label length */
#define FILE_OBJ_SIZE      (0x80)   /**< Max number of Open File&Dir */
#define FILE_NAME_LEN      (0x80)   /**< Max length of file full path */
#define FILE_PHY_DISK_FLAG (1 << 4) /**< flag of phy disk */

#define FILE_ERROR_OVER_SIZE (-100) /**< file size cannot reach 4 GiB at FAT volume */

/**
 * \brief enum dir format
 * \ingroup filemanager_common_para
 * \details The structure define the FILEMANAGER suppot file types information
 */
typedef enum _DIR_ATTR_T
{
    DIR_ATTR_FIL, /**< dir attribute is file */
    DIR_ATTR_DIR, /**< dir attribute is dir */
} DIR_ATTR_T;

/**
 * \brief enum file format
 * \ingroup filemanager_common_para
 * \details The structure define the FILEMANAGER suppot file types information
 */
typedef enum _FILE_FORMAT_T
{
    FILE_TYPE_BIN,  /**< file format is bin */
    FILE_TYPE_TXT,  /**< file format is txt */
    FILE_TYPE_AVI,  /**< file format is avi */
    FILE_TYPE_WAV,  /**< file format is wav/ima */
    FILE_TYPE_JPEG, /**< file format is jpeg */
    FILE_TYPE_MP3,  /**< file format is mp3 */
    FILE_TYPE_A32,  /**< file format is audio32 */
    FILE_TYPE_BMP,  /**< file format is bmp */
    FILE_TYPE_JPG,  /**< file format is jpg */
    FILE_TYPE_GIF,  /**< file format is gif */
} FILE_FORMAT_T;

/**
 * \brief enum open file mode
 * \ingroup filemanager_common_para
 * \details The structure define the FILEMANAGER support file open mode information
 */
typedef enum _FILE_OPEN_MODE_T
{
    FILE_OPEN_R  = FA_READ,                     /**< Read only */
    FILE_OPEN_W  = FA_WRITE | FA_CREATE_ALWAYS, /**< Write & Overwrite */
    FILE_OPEN_A  = FA_WRITE | FA_OPEN_APPEND,   /**< Write & Append */
    FILE_OPEN_WX = FA_WRITE | FA_CREATE_NEW,    /**< Write & Create */
} FILE_OPEN_MODE_T;

/**
 * \brief file physical drive
 * \ingroup filemanager_common_para
 * \details The structure define the FILEMANAGER suppot device type information
 */
typedef enum _FATFS_PHY_DEV_T
{
    FATFS_PHY_DEV_SD1   = FILE_PHY_DISK_FLAG | DEV_MMC, /**< Map SD1   to physical drive */
    FATFS_PHY_DEV_FLASH = FILE_PHY_DISK_FLAG | DEV_FMC, /**< Map Flash to physical drive */
    FATFS_PHY_DEV_SD0   = FILE_PHY_DISK_FLAG | DEV_SD0, /**< Map SD0   to physical drive */
    FATFS_PHY_DEV_USB   = FILE_PHY_DISK_FLAG | DEV_USB, /**< Map SD0   to physical drive */
} FATFS_PHY_DEV_T;

/**
 * \brief file logical drive
 * \ingroup filemanager_common_para
 */
typedef enum _FATFS_LOGIC_DEV_T
{
    FATFS_LOGIC_DEV0, /**< Map logical drive 0 */
    FATFS_LOGIC_DEV1, /**< Map logical drive 1 */
    FATFS_LOGIC_DEV2, /**< Map logical drive 2 */
    FATFS_LOGIC_DEV3, /**< Map logical drive 3 */
    FATFS_LOGIC_DEV4, /**< Map logical drive 4 */
    FATFS_LOGIC_DEV5, /**< Map logical drive 5 */
    FATFS_LOGIC_DEV6, /**< Map logical drive 6 */
    FATFS_LOGIC_DEV7, /**< Map logical drive 7 */
    FATFS_LOGIC_DEV8, /**< Map logical drive 8 */
    FATFS_LOGIC_DEV9, /**< Map logical drive 9 */
    FATFS_LOGIC_DEVM, /**< Map logical drive max */
} FATFS_LOGIC_DEV_T;

/**
 * \brief file logical partition
 * \ingroup filemanager_common_para
 */
typedef enum _FATFS_LOGIC_PART_T
{
    FATFS_LOGIC_PART1, /**< Map logical part 1 on physical drive */
    FATFS_LOGIC_PART2, /**< Map logical part 2 on physical drive */
    FATFS_LOGIC_PART3, /**< Map logical part 3 on physical drive */
    FATFS_LOGIC_PART4, /**< Map logical part 4 on physical drive */
    FATFS_LOGIC_PARTM, /**< Map logical part max on physical drive */
} FATFS_LOGIC_PART_T;
#if FF_MULTI_PARTITION
typedef FATFS_LOGIC_DEV_T FATFS_DEV_T;
#else
typedef FATFS_PHY_DEV_T FATFS_DEV_T;
#endif

/**
 * \brief enum open file mode
 * \ingroup filemanager_common_para
 */
typedef enum _FATFS_PART_METHOD_T
{
    FATFS_PART_METHOD_NULL = 0,
    FATFS_PART_METHOD_SECTOR, /**< Partition by sector; unit : SD is 512Byte, Flash is 4KB */
    FATFS_PART_METHOD_RATE,   /**< Partition by rate, unint:1% */
    FATFS_PART_METHOD_REST,   /**< Partition by rest, use all the rest space */
} FATFS_PART_METHOD_T;

/**
 * \brief struct of disk partition
 * \ingroup filemanager_common_struct
 */
typedef struct _FATFS_PART_T
{
    uint32_t            size;                   /**< Partition size, see \ref FATFS_PART_METHOD_T */
    FATFS_PART_METHOD_T method;                 /**< Partition size method, see \ref FATFS_PART_METHOD_T */
    TCHAR               label[FATFS_LABEL_LEN]; /**< Assign partition a label */
} FATFS_PART_T;

/**
 * \brief struct of disk info
 * \ingroup filemanager_common_struct
 */
typedef struct _FATFS_DISK_T
{
    FATFS_PHY_DEV_T dev;                     /**< Physical drive number, see \ref FATFS_PHY_DEV_T */
    FATFS_PART_T    part[FATFS_LOGIC_PARTM]; /**< logical drive info, see \ref FATFS_PART_T */
} FATFS_DISK_T;

/**
 * \brief struct of partition status
 * \ingroup filemanager_common_struct
 */
typedef struct _FATFS_PART_STATUS_T
{
    FATFS_PHY_DEV_T    dev;                    /**< Physical drive number, see \ref FATFS_PHY_DEV_T */
    FATFS_LOGIC_PART_T part;                   /**< Physical drive Partition index */
    uint32_t           sector;                 /**< Physical drive Sector size (unint:byte) */
    FATFS_LOGIC_DEV_T  logic;                  /**< Logical drive number, access drive by this number */
    uint32_t           size;                   /**< Logical drive size (unint:sector) */
    uint32_t           free;                   /**< Logical drive free size (unint:sector) */
    TCHAR              label[FATFS_LABEL_LEN]; /**< Logical drive label */
    uint8_t            multi;
} FATFS_PART_STATUS_T;

/**
 * \brief struct of disk status
 * \ingroup filemanager_common_struct
 */
typedef struct _FATFS_DISK_STATUS_T
{
    FATFS_PART_STATUS_T part[DEV_MAX][FATFS_LOGIC_PARTM]; /**< Partition each info in all Physical drive */
} FATFS_DISK_STATUS_T;

#endif /* _FILE_FAT_H_ */
