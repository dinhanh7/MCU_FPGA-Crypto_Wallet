/***********************
 * SN34F78X SDK V2.1.4 *
 ***********************/

#ifndef _FILE_JPG_H_
#define _FILE_JPG_H_

#include "sn34f78x_hal_def.h"
//=============================================================================
//                  Structure Declaration
//=============================================================================

/**
 * \brief JPG Scan types
 * \ingroup filemanager_jpg_para
 */
typedef enum _JPG_SAMP_FORMAT
{
    JPG_GRAYSCALE = 0,
    JPG_YH1V1,
    JPG_YH2V1,
    JPG_YH1V2,
    JPG_YH2V2
} JPG_SAMP_FORMAT;

/**
 * \brief JPG_INIT_T
 * \ingroup filemanager_jpg_struct
 * \details The structure define the jpg file initialization info. It will take effect when write mode
 */
typedef struct _JPG_INIT_T
{
    uint8_t *head_data; /**< jpg palette data. */
    uint32_t head_size; /**< jpg palette size by BYTE. */
} JPG_INIT_T;
typedef JPG_INIT_T JPG_INFO_T;

/**
 * \brief JPG_STATUS_T
 * \ingroup filemanager_jpg_struct
 */
typedef struct _JPG_STATUS_T
{
    uint32_t        pos;            /**< jpg pointer position */
    uint32_t        size;           /**< jpg size */
    uint16_t        width;          /**< jpg width */
    uint16_t        height;         /**< jpg height */
    uint32_t        x_pels_perinch; /**< Horizontal resolution, pixels per inch(dpi) */
    uint32_t        y_pels_perinch; /**< Vertical resolution, pixels per inch(dpi) */
    uint8_t        *head_data;      /**< jpg head data */
    uint32_t        head_size;      /**< jpg head size by BYTE */
    JPG_SAMP_FORMAT samp_format;    /**< jpg sample format */
    uint8_t         is_eof;         /**< jpg is end or not */
} JPG_STATUS_T;

/**
 * \brief JPG_SEEK_T
 * \ingroup filemanager_jpg_struct
 */
typedef struct _JPG_SEEK_T
{
    uint32_t pos; /**< seek to position */
} JPG_SEEK_T;

#endif /* _FILE_JPG_H_ */
