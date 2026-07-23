/***********************
 * SN34F78X SDK V2.1.4 *
 ***********************/

#ifndef _MP3_WRAPPER_H_
#define _MP3_WRAPPER_H_

#include "file_wrapper.h"
#include "File_MP3.h"

//=============================================================================
//                  Macro Definition
//=============================================================================
#define MP3_ID3V2_TAG        (0x334449)
#define MP3_FRAME_SYNC1      (0xFF)
#define MP3_FRAME_SYNC2      (0x7)
#define MP3_ID3_SIZE(x)      ((x & 0x7F) << 7)
#define MP3_ID3_INFO_SIZE(x) (MP3_ID3_SIZE(MP3_ID3_SIZE(MP3_ID3_SIZE(x[0]) | x[1]) | x[2]) | x[3])

#define MP3_MSG_INVALID_FRAME (MP3_ERR_MAX + 1)

/* xing offset addr */
#define XING_XING_OFFSET_ADDR      (4)
#define XING_FLAG_OFFSET_ADDR      (8)
#define XING_FRAME_NUM_OFFSET_ADDR (12)
#define XING_BYTES_NUM_OFFSET_ADDR (16)
/* vbri offset addr */
#define VBRI_VBRI_OFFSET_ADDR      (4)
#define VBRI_BYTES_NUM_OFFSET_ADDR (14)
#define VBRI_FRAME_NUM_OFFSET_ADDR (16)

/* Conversion between little endian and big endian  */
#define L2B32(Little) (((Little & 0xff) << 24) | (((Little) & 0xff00) << 8) | (((Little) & 0xff0000) >> 8) | ((Little >> 24) & 0xff))

//=============================================================================
//                  Structure Declaration
//=============================================================================
typedef struct _MP3_SYNC2_T
{
    uint8_t protection : 1; // Protection bit
    uint8_t layer : 2;      // Layer description
    uint8_t version : 2;    // MPEG Audio version ID
    uint8_t sync : 3;       // Frame sync MSB (0x7)
} MP3_SYNC2_T;

typedef struct _MP3_FRAME_H_T
{
    uint8_t private : 1;           // Private bit
    uint8_t padding : 1;           // Padding bit
    uint8_t sample_rate_index : 2; // Sample rate index
    uint8_t bit_rate_index : 4;    // Bit rate index
    uint8_t emphasis : 2;          // Emphasis
    uint8_t original : 1;          // Original bit
    uint8_t copyright : 1;         // Copyright bit
    uint8_t extension : 2;         // channel mode extension, (Only used in Joint stereo)
    uint8_t channel : 2;           // channel mode
} MP3_FRAME_H_T;

typedef struct _MP3_FRAME_INFO_T
{
    uint8_t       sync1;  // sync 1: Frame sync LSB(0xFF)
    MP3_SYNC2_T   sync2;  // sync 2
    MP3_FRAME_H_T header; // header
} MP3_FRAME_INFO_T;

typedef struct _MP3_FRAME_HEADER_T
{
    MP3_FRAME_INFO_T header;
    uint16_t         CRC;
} MP3_FRAME_HEADER_T;

typedef struct _MP3_ID3_INFO_T
{
    uint8_t version;  //
    uint8_t revision; //
    uint8_t flag;     //
    uint8_t size[4];  // ID3 header size
} MP3_ID3_INFO_T;

typedef struct _MP3_XING_INFO_T
{
    uint32_t xing;      // :: xing or info marker
    uint32_t flag;      // :: xing flag (1|2|4|8)
    uint32_t frame_num; // 1: xing frame number(unit:frame)
    uint32_t bytes_num; // 2: xing file total length(unit:byte)
    uint8_t  toc[100];  // 4: xing toc
    uint32_t quality;   // 8: xing quality [0,100]
} MP3_XING_INFO_T;

typedef struct _MP3_VBRI_INFO_T
{
    uint32_t vbri;            // VBR header ID in 4 ASCII chars, always 'VBRI', not NULL-terminated
    uint16_t id;              // Version ID as Big-Endian WORD
    uint16_t delay;           // Delay as Big-Endian float
    uint16_t quality;         // Quality indicator
    uint32_t bytes_num;       // Number of Bytes as Big-Endian DWORD
    uint32_t frame_num;       // Number of Frames as Big-Endian DWORD
    uint16_t toc_entries_num; // Number of entries within TOC table as Big-Endian WORD
    uint16_t toc_Scalefactor; // Scale factor of TOC table entries as Big-Endian DWORD
    uint16_t per_table_size;  // Size per table entry in bytes (max 4) as Big-Endian WORD
    uint16_t per_table_frame; // Frames per table entry as Big-Endian WORD
    uint8_t  toc_table;       // TOC entries for seeking as Big-Endian integral. From size per table entry and number of entries, you can calculate the length of this field
} MP3_VBRI_INFO_T;

typedef struct _MP3_FILE_HEADER_T
{
    MP3_ID3_INFO_T *id3;
    MP3_XING_INFO_T xing;
    MP3_VBRI_INFO_T vbri;
} MP3_FILE_HEADER_T;

#define MP3_VALID_FRAME_CNT (3)
typedef struct _MP3_SAVE_FRAME_T
{
    uint32_t     head_len; /* MP3 saved frame head length */
    FILE_FRAME_T data;     /* MP3 saved frame data */
} MP3_SAVE_FRAME_T;

///////////////////////////////////////////////////////////////////////////////
typedef struct _MP3_FILE_RES_T
{
    FIL                *fp;                              /* MP3 file handler for fatfs */
    DEV_FIL_T          *sfp;                             /* MP3 file handler for storage */
    FILE_PARSER_T       parser;                          /* MP3 parser info */
    MP3_SAVE_FRAME_T    frame_save[MP3_VALID_FRAME_CNT]; /* MP3 saved frame */
    FILE_SRC_T          src;                             /* MP3 src */
    MP3_INFO_T          info;                            /* MP3 info just for write */
    MP3_FILE_HEADER_T   header;                          /* MP3 header info */
    MP3_FRAME_HEADER_T *frame_keep;                      /* MP3 keepped frame */
    MP3_FRAME_HEADER_T *frame_curr;                      /* MP3 current frame */
    uint8_t             mode;                            /* MP3 at mode which is read or write */
    uint8_t             check_cnt;                       /* MP3 frame header check count */
    uint8_t             is_eof;                          /* MP3 is eof or not */
    uint64_t            frame_cnt;                       /* MP3 all frame count */
    uint64_t            frame_idx;                       /* MP3 frame index currently */
    uint32_t            file_pos;                        /* MP3 file current position */
    uint32_t            file_siz;                        /* MP3 file total size */
    uint32_t            head_len;                        /* MP3 current frame head length */
    uint32_t            data_len;                        /* MP3 current frame data length */
    uint32_t            mp3_status;                      /* MP3 status */
    uint32_t            skip_bytes;                      /* MP3 skip bytes status */
} MP3_FILE_RES_T;

extern TYPE_WRAPPER  MP3_Wrapper;
extern PARSE_WRAPPER MP3_Stream;

#endif /* _MP3_WRAPPER_H_ */
