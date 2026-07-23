/***********************
 * SN34F78X SDK V2.1.4 *
 ***********************/

#ifndef _FILE_GIF_H_
#define _FILE_GIF_H_

#include "sn34f78x_hal_def.h"
//=============================================================================
//                  Structure Declaration
//=============================================================================
/**
 * \brief GIF_INIT_T
 * \ingroup filemanager_gif_struct
 * \details The structure define the gif file initialization info. It will take effect when write mode
 */
typedef struct _GIF_INIT_T
{
    uint8_t *head_data; /**< gif palette data. */
    uint32_t head_size; /**< gif palette size by BYTE. */
} GIF_INIT_T;
typedef GIF_INIT_T GIF_INFO_T;

/**
 * \brief GIF_STATUS_T
 * \ingroup filemanager_gif_struct
 */
typedef struct _GIF_STATUS_T
{
    uint32_t pos;                /**< GIF pointer position */
    uint32_t size;               /**< GIF size */
    uint16_t width;              /**< GIF width */
    uint16_t height;             /**< GIF height */
    uint8_t  color_resolution;   /**< GIF color resolution, (color_resolution + 1) determines the color depth of the image */
    uint8_t  pixel;              /**< GIF pixel, (pixel + 1) determines the number of bits per pixel */
    uint8_t  pixel_aspect_ratio; /**< GIF pixel aspect ratio */
    uint8_t *head_data;          /**< GIF head data */
    uint32_t head_size;          /**< GIF head size by BYTE */
    uint8_t  is_eof;             /**< GIF is end or not */
} GIF_STATUS_T;

/**
 * \brief GIF_SEEK_T
 * \ingroup filemanager_gif_struct
 */
typedef struct _GIF_SEEK_T
{
    uint32_t pos; /**< seek to position */
} GIF_SEEK_T;

#endif /* _FILE_GIF_H_ */
