/***********************
 * SN34F78X SDK V2.1.4 *
 ***********************/

#ifndef _GIF_WRAPPER_H_
#define _GIF_WRAPPER_H_

#include "file_wrapper.h"
#include "File_GIF.h"
//=============================================================================
//                  Macro Definition
//=============================================================================
#define GIF_TYPE_GIF89A (0x0000613938464947)
#define GIF_TYPE_GIF87A (0x0000613738464947)

#define GIF_FILE_HEADER_SIZE                    (0x6)
#define GIF_FILE_LOGICAL_SCREEN_DESCRIPTOR_SIZE (0x7)

//=============================================================================
//                  Structure Declaration
//=============================================================================
/**
 * \brief struct for gif header information
 * \ingroup filemanager_struct_define_gr
 */
typedef struct _GIF_INFO_LSD_T
{
    uint16_t width;                   // GIF width, in pixels
    uint16_t height;                  // GIF height, in pixels
    uint8_t  global_color_table_flag; // GIF global color table flag
    uint8_t  color_resolution;        // GIF color resolution, (color_resolution + 1) determines the color depth of the image
    uint8_t  sort_flag;               // GIF sort flag, If it is 1, it means the global color list classification permutation
    uint8_t  pixel;                   // GIF pixel, (pixel + 1) determines the number of bits per pixel
    uint8_t  background_color;        // GIF background color
    uint8_t  pixel_aspect_ratio;      // GIF pixel aspect ratio
} GIF_INFO_LSD_T;

typedef struct _GIF_FILE_RES_T
{
    FIL           *fp;                        // GIF fatfs file handler
    uint8_t        mode;                      // GIF at mode which is read or write
    uint64_t       head_info;                 // GIF head info
    GIF_INFO_T     info;                      // GIF info
    GIF_INFO_LSD_T logical_screen_descriptor; // GIF Logical Screen Descriptor
    uint8_t       *head_data;                 // GIF palette data
    uint32_t       head_size;                 // GIF palette size by BYTE
    uint32_t       seek;                      // GIF info seek
    uint8_t        is_eof;                    // GIF is eof or not
} GIF_FILE_RES_T;

extern TYPE_WRAPPER GIF_Wrapper;

#endif /* _GIF_WRAPPER_H_ */
