/***********************
 * SN34F78X SDK V2.1.4 *
 ***********************/

#ifndef _A32_WRAPPER_H_
#define _A32_WRAPPER_H_

#include "file_wrapper.h"
#include "File_A32.h"

//=============================================================================
//                  Macro Definition
//=============================================================================
#define A32_TAG (0x5541)

//=============================================================================
//                  Structure Declaration
//=============================================================================
#pragma pack(push)
#pragma pack(1)
typedef struct _A32_FILE_HEADER_T
{
    uint16_t A32Tag;       // "AU"
    uint16_t sample_rate;  // sample rate
    uint16_t bit_rate_tag; // bit rate tag
    uint16_t channel;      // channel
    uint32_t frame_cnt;    // frame count
    uint32_t file_len;     // file length, unint:word
    uint16_t MF;           // MF
    uint16_t SF;           // SF
    uint16_t MBF;          // MBF
    uint16_t PCS;          // PCS
    uint16_t sRec;         // sRec
    uint16_t header_len;   // header length, unint:word
    uint16_t audio_type;   // audio type
    uint16_t stop_code;    // stop code
    uint16_t s_header;     // s_header
} A32_FILE_HEADER_T;
#pragma pack()
#pragma pack(pop)

///////////////////////////////////////////////////////////////////////////////
typedef struct _A32_FILE_RES_T
{
    FIL              *fp;         /* A32 file handler */
    DEV_FIL_T        *sfp;        /* A32 stream file handler */
    A32_INFO_T        info;       /* A32 info just for write */
    FILE_PARSER_T     parser;     /* A32 parser info */
    A32_FILE_HEADER_T header;     /* A32 header info */
    uint8_t           mode;       /* A32 at mode which is read or write */
    uint64_t          frame_siz;  /* A32 frame size */
    uint64_t          frame_cnt;  /* A32 all frame count */
    uint64_t          frame_idx;  /* A32 frame index currently */
    uint32_t          header_len; /* A32 header length */
    uint32_t          bit_rate;   /* A32 bit rate */
    uint32_t          file_pos;   /* A32 file pos */
    uint32_t          file_siz;   /* A32 file size */
    uint32_t          a32_status; /* A32 status machin */
    FILE_SRC_T        src;        /* A32 source from */
    uint8_t           is_eof;     /* A32 is eof or not */
} A32_FILE_RES_T;

extern TYPE_WRAPPER  A32_Wrapper;
extern PARSE_WRAPPER A32_Stream;

#endif /* _A32_WRAPPER_H_ */
