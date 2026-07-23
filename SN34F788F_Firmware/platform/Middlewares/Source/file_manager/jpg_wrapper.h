/***********************
 * SN34F78X SDK V2.1.4 *
 ***********************/

#ifndef _JPG_WRAPPER_H_
#define _JPG_WRAPPER_H_

#include "file_wrapper.h"
#include "File_JPG.h"

//=============================================================================
//                  Macro Definition
//=============================================================================
#define JPG_TYPE 0xFFD8
typedef enum
{
    JPG_SOF0 = 0xFFC0,
    JPG_SOF1 = 0xFFC1,
    JPG_SOF2 = 0xFFC2,
    JPG_SOF3 = 0xFFC3,

    JPG_SOF5 = 0xFFC5,
    JPG_SOF6 = 0xFFC6,
    JPG_SOF7 = 0xFFC7,

    JPG_JPG   = 0xFFC8,
    JPG_SOF9  = 0xFFC9,
    JPG_SOF10 = 0xFFCA,
    JPG_SOF11 = 0xFFCB,

    JPG_SOF13 = 0xFFCD,
    JPG_SOF14 = 0xFFCE,
    JPG_SOF15 = 0xFFCF,

    JPG_DHT = 0xFFC4,

    JPG_DAC = 0xFFCC,

    JPG_RST0 = 0xFFD0,
    JPG_RST1 = 0xFFD1,
    JPG_RST2 = 0xFFD2,
    JPG_RST3 = 0xFFD3,
    JPG_RST4 = 0xFFD4,
    JPG_RST5 = 0xFFD5,
    JPG_RST6 = 0xFFD6,
    JPG_RST7 = 0xFFD7,

    JPG_SOI = 0xFFD8,
    JPG_EOI = 0xFFD9,
    JPG_SOS = 0xFFDA,
    JPG_DQT = 0xFFDB,
    JPG_DNL = 0xFFDC,
    JPG_DRI = 0xFFDD,
    JPG_DHP = 0xFFDE,
    JPG_EXP = 0xFFDF,

    JPG_APP0  = 0xFFE0,
    JPG_APP15 = 0xFFEF,

    JPG_JPG0  = 0xFFF0,
    JPG_JPG13 = 0xFFFD,
    JPG_COM   = 0xFFFE,

    JPG_TEM = 0xFF01,
} JPG_MARKER;
//=============================================================================
//                  Structure Declaration
//=============================================================================

typedef struct _JPG_RES_T
{
    FIL    *fp;
    uint8_t mode;
    uint8_t is_eof;
} JPG_RES_T;

/**
 * \brief struct for jpg header information
 * \ingroup filemanager_struct_define_gr
 */
typedef struct _JPG_INFO_HEADER_T
{
    uint16_t        width;          // JPG width, in pixels
    uint16_t        height;         // JPG height, in pixels
    uint32_t        x_pels_perinch; // JPG horizontal resolution, pixels per inch(dpi)
    uint32_t        y_pels_perinch; // JPG vertical resolution, pixels per inch(dpi)
    JPG_SAMP_FORMAT samp_format;    // JPG sampling format
} JPG_INFO_HEADER_T;

typedef struct _JPG_FILE_RES_T
{
    FIL              *fp;        // JPG fatfs file handler
    uint8_t           mode;      // JPG at mode which is read or write
    JPG_INFO_T        info;      // JPG info
    JPG_INFO_HEADER_T head;      // JPG head
    uint8_t          *head_data; // JPG palette data
    uint32_t          head_size; // JPG palette size by BYTE
    uint32_t          seek;      // JPG info seek
    uint8_t           is_eof;    // JPG is eof or not
} JPG_FILE_RES_T;

extern TYPE_WRAPPER JPG_Wrapper;

#endif /* _JPG_WRAPPER_H_ */
