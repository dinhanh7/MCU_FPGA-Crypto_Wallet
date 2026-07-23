/***********************
 * SN34F78X SDK V2.1.4 *
 ***********************/

#ifndef _FILE_BMP_H_
#define _FILE_BMP_H_

#include "sn34f78x_hal_def.h"
//=============================================================================
//                  Structure Declaration
//=============================================================================
/**
 * \brief BMP compression type.
 * \ingroup filemanager_bmp_para
 */
typedef enum _BMP_COMPRESSION_TYPE
{
    BI_RGB       = 0, /**< No compression */
    BI_BITFIELDS = 3, /**< Bit fields. Used for 16/32 bit bitmaps */
} BMP_COMPRESSION_TYPE;

/**
 * \brief BMP Bit Count.
 * \ingroup filemanager_bmp_para
 */
typedef enum _BMP_BIT_COUNT
{
    BIT_COUNT_8  = 8,  /**< 8 bit count */
    BIT_COUNT_16 = 16, /**< 16 bit count */
    BIT_COUNT_24 = 24, /**< 24 bit count */
    BIT_COUNT_32 = 32, /**< 32 bit count */
} BMP_BIT_COUNT;

/**
 * \brief BMP_INIT_T
 * \ingroup filemanager_bmp_struct
 * \details The structure define the bmp file initialization info. It will take effect when write mode
 */
typedef struct _BMP_INIT_T
{
    uint32_t             bmp_width;       /**< bmp width */
    uint32_t             bmp_height;      /**< bmp height */
    BMP_BIT_COUNT        bit_count;       /**< bmp bit count */
    uint32_t             x_pels_permeter; /**< Horizontal resolution of bitmap, pixels per meter */
    uint32_t             y_pels_permeter; /**< Vertical resolution, pixels per meter */
    uint8_t             *palette_data;    /**< bmp palette data. If bit_count is /ref BIT_COUNT_24 or /ref BIT_COUNT_32, it will not take effect */
    uint32_t             palette_size;    /**< bmp palette size by BYTE. If bit_count is /ref BIT_COUNT_24 or /ref BIT_COUNT_32, it will not take effect */
    BMP_COMPRESSION_TYPE compression;     /**< bmp compression type */
} BMP_INIT_T;
typedef BMP_INIT_T BMP_INFO_T;

/**
 * \brief BMP_STATUS_T
 * \ingroup filemanager_bmp_struct
 */
typedef struct _BMP_STATUS_T
{
    uint32_t             bmp_pos;         /**< bmp pointer position */
    uint32_t             bmp_size;        /**< bmp total size */
    uint32_t             bmp_width;       /**< bmp width */
    uint32_t             bmp_height;      /**< bmp height */
    BMP_BIT_COUNT        bit_count;       /**< bmp bit count, number of bits occupied by pixels */
    BMP_COMPRESSION_TYPE compression;     /**< bmp compression type */
    uint32_t             x_pels_permeter; /**< horizontal resolution of bitmap, pixels per meter */
    uint32_t             y_pels_permeter; /**< vertical resolution, pixels per meter */
    uint8_t             *palette_data;    /**< bmp palette data */
    uint32_t             palette_size;    /**< bmp palette size by BYTE */
    uint8_t              is_eof;          /**< bmp is end or not */
} BMP_STATUS_T;

/**
 * \brief BMP_SEEK_T
 * \ingroup filemanager_bmp_struct
 */
typedef struct _BMP_SEEK_T
{
    uint32_t pos; /**< seek to position */
} BMP_SEEK_T;

#endif /* _FILE_BMP_H_ */
