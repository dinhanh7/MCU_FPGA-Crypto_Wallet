/***********************
 * SN34F78X SDK V2.1.4 *
 ***********************/

#ifndef _BMP_WRAPPER_H_
#define _BMP_WRAPPER_H_

#include "file_wrapper.h"
#include "File_BMP.h"

//=============================================================================
//                  Macro Definition
//=============================================================================
#define BMP_FILE_HEADER_SIZE 0x0E
#define BMP_INFO_HEADER_SIZE 0x28

#define BMP_TYPE 0x4D42 // For "BM"

//=============================================================================
//                  Structure Declaration
//=============================================================================

typedef struct _BMP_RES_T
{
    FIL    *fp;
    uint8_t mode;
    uint8_t is_eof;
} BMP_RES_T;

/**
 * \brief struct for bmp file header
 * \ingroup filemanager_struct_define_gr
 */
typedef struct _BMP_FILE_HEADER_T
{
    uint16_t type;      // The type of bitmap file must be BM
    uint32_t size;      // The size of a bitmap file, in bytes
    uint16_t reserved1; // Reseved, must set 0
    uint16_t reserved2; // Reseved, must set 0
    uint32_t off_bits;  // The address offset of the position of the bitmap data, i.e. the starting position, relative to the bitmap
} BMP_FILE_HEADER_T;

/**
 * \brief struct for bmp information header
 * \ingroup filemanager_struct_define_gr
 */
typedef struct _BMP_INFO_HEADER_T
{
    uint32_t             size;      // Bitmap header size
    int32_t              width;     // Bitmap width, in pixels
    int32_t              height;    // Bitmap height, in pixels
    uint16_t             planes;    // Save the number of color planes used. Its value is always 1
    BMP_BIT_COUNT        bit_count; // Save the number of bits per pixel, which is the color depth of the image. Its value is 8,16,24,32
    uint8_t              reserved0;
    BMP_COMPRESSION_TYPE compression; // efine the compression algorithm used. The allowed values are 0, 1, 2, 3, 4, 5
    uint8_t              reserved1;
    uint8_t              reserved2;
    uint8_t              reserved3;
    uint32_t             size_image;      // The size of the bitmap (which includes empty bytes added to fill in multiples of 4 rows), in bytes/
    uint32_t             x_pels_permeter; // Horizontal resolution of bitmap, pixels per meter
    uint32_t             y_pels_permeter; // Vertical resolution, pixels per meter
    uint32_t             clr_used;        // The number of colors in the actual color table used by the bitmap. If set to 0, it means using all palette items
    uint32_t             clr_important;   // Important number of colors during bitmap display. If it is 0, it means both are important
} BMP_INFO_HEADER_T;

typedef struct _BMP_FILE_RES_T
{
    FIL              *fp;           // BMP fatfs file handler
    uint8_t           mode;         // BMP at mode which is read or write
    BMP_INFO_T        info;         // BMP info
    BMP_FILE_HEADER_T file_header;  // BMP file header data
    BMP_INFO_HEADER_T info_header;  // BMP info header data
    uint8_t          *palette_data; // BMP palette data
    uint32_t          palette_size; // BMP palette size by BYTE
    uint32_t          seek;         // BMP info seek
    uint8_t           is_eof;       // BMP is eof or not
} BMP_FILE_RES_T;

extern TYPE_WRAPPER BMP_Wrapper;

#endif /* _BMP_WRAPPER_H_ */
