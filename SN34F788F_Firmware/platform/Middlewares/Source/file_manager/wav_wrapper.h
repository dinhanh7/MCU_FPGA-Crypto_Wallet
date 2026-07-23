/***********************
 * SN34F78X SDK V2.1.4 *
 ***********************/

#ifndef _WAV_WRAPPER_H_
#define _WAV_WRAPPER_H_

#include "file_wrapper.h"
#include "File_WAV.h"

//=============================================================================
//                  Macro Definition
//=============================================================================
#define CHUNK_ID_RIFF (0x46464952) // "RIFF"
#define CHUNK_ID_WAVE (0x45564157) // "WAVE"
#define CHUNK_ID_FMT_ (0x20746D66) // "fmt "
#define CHUNK_ID_DATA (0x61746164) // "data"
#define CHUNK_ID_LIST (0x5453494C) // "LIST"
#define CHUNK_ID_FACT (0x74636166) // "fact"

#define WAV_CHUNK_TAG_SIZE (8)

//=============================================================================
//                  Structure Declaration
//=============================================================================
typedef struct _WAV_FORMAT_PCM_T
{
    uint16_t AudioFormat;   // format type. 0x1->PCM, 0x3->IEEE float, 0x6->8bit A law, 0x7->8bit U law, 0x11->IMA ADPCM
    uint16_t NumOfChannels; // 0x1 for mono, 0x2 for stereo
    uint32_t SampleRate;    // sampling rate (blocks per second)
    uint32_t ByteRate;      // PCM:SampleRate * NumOfChannels * BitsPerSample / 8
    uint16_t BlockAlign;    // PCM:NumOfChannels * BitsPerSample / 8  // IMA:packet bytes
    uint16_t BitsPerSample; // bits per sample, 8- 8bits, 16- 16 bits, etc
} WAV_FORMAT_PCM_T;

typedef struct _WAV_FORMAT_FMT_T
{
    uint32_t         ChunkFMTTag;     // "fmt "
    uint32_t         ChunkFMTLen;     // length of format data
    WAV_FORMAT_PCM_T pcm;             // wav type
    uint16_t         ExtraFmtSize;    // PCM dont care, ima_adpcm = 2, ms_adpcm = 32
    uint16_t         SamplesPerBlock; // for ima pcm
} WAV_FORMAT_FMT_T;

typedef struct _WAV_FORMAT_RIFF_T
{
    uint32_t ChunkRIFFTag; // "RIFF"
    uint32_t ChunkRIFFLen; // overall size of file in bytes
    uint32_t Format;       // "WAVE"
} WAV_FORMAT_RIFF_T;

typedef struct _WAV_FORMAT_DATA_T
{
    uint32_t ChunkDataTag; // "data"
    uint32_t ChunkDataLen; // NumSamples * NumOfChannels * BitsPerSample / 8 - size of the data chunk
} WAV_FORMAT_DATA_T;

typedef struct _WAV_FORMAT_FACT_T
{
    uint32_t ChunkFactTag; // "fact"
    uint32_t ChunkFactLen; // 4 byte
    uint32_t Fact;         // Fact data
} WAV_FORMAT_FACT_T;

typedef struct _WAV_FILE_HEADER_T
{
    WAV_FORMAT_RIFF_T riff; // wav riff header
    WAV_FORMAT_FMT_T  fmt;  // wav format info
    WAV_FORMAT_FACT_T fact; // wav fact info
    WAV_FORMAT_DATA_T data; // wav data info
} WAV_FILE_HEADER_T;

///////////////////////////////////////////////////////////////////////////////
typedef struct _WAV_FILE_RES_T
{
    FIL              *fp;         /* WAV fatfs file handler */
    DEV_FIL_T        *sfp;        /* WAV stream file handler */
    WAV_INFO_T        info;       /* WAV info just for write */
    FILE_PARSER_T     parser;     /* WAV parser info */
    WAV_FILE_HEADER_T header;     /* WAV header info */
    uint8_t           mode;       /* WAV at mode which is read or write */
    uint64_t          sample_cnt; /* WAV all sample count */
    uint64_t          sample_idx; /* WAV sample index currently */
    uint32_t          header_len; /* WAV header length */
    uint32_t          extra_len;  /* WAV extra info length */
    uint32_t          file_pos;   /* WAV file pos */
    uint32_t          file_siz;   /* WAV file size */
    uint32_t          wav_status; /* WAV status machin */
    FILE_SRC_T        src;        /* WAV source from */
    uint8_t           is_eof;     /* WAV is eof or not */
} WAV_FILE_RES_T;

extern TYPE_WRAPPER  WAV_Wrapper;
extern PARSE_WRAPPER WAV_Stream;

#endif /* _WAV_WRAPPER_H_ */
