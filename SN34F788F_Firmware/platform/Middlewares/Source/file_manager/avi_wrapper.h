/***********************
 * SN34F78X SDK V2.1.4 *
 ***********************/

#ifndef _AVI_WRAPPER_H_
#define _AVI_WRAPPER_H_

#include "file_wrapper.h"
#include "File_AVI.h"

//=============================================================================
//                  Macro Definition
//=============================================================================
#define AVI_RIFF_ID 0X46464952
#define AVI_AVI_ID  0X20495641
#define AVI_LIST_ID 0X5453494C
#define AVI_HDRL_ID 0X6C726468 /* Information block flag */
#define AVI_MOVI_ID 0X69766F6D /* Data block flag */
#define AVI_STRL_ID 0X6C727473 /* strl flag */

#define AVI_AVIH_ID 0X68697661 /* avih∈AVI_HDRL_ID */
#define AVI_STRH_ID 0X68727473 /* strh∈AVI_STRL_ID */
#define AVI_STRF_ID 0X66727473 /* strf∈AVI_STRL_ID */
#define AVI_STRD_ID 0X64727473 /* strd∈AVI_STRL_ID (Optional) */

#define AVI_VIDS_STREAM 0X73646976 /* Video streaming */
#define AVI_AUDS_STREAM 0X73647561 /* Audio streaming */

#define AVI_VIDS_FLAG 0X6463 /* Video streaming flag */
#define AVI_AUDS_FLAG 0X7762 /* Audio streaming flag */

#define AVI_FORMAT_MJPG 0X47504A4D

#define AVI_FIFO_SIZE (1024)

#define MAKEWORD(ptr)  (uint16_t)(((uint16_t)*((uint8_t *)(ptr)) << 8) | (uint16_t)*(uint8_t *)((ptr) + 1))
#define MAKEDWORD(ptr) (uint32_t)(((uint16_t)*(uint8_t *)(ptr) | (((uint16_t)*(uint8_t *)(ptr + 1)) << 8) | \
                                   (((uint16_t)*(uint8_t *)(ptr + 2)) << 16) | (((uint16_t)*(uint8_t *)(ptr + 3)) << 24)))

//=============================================================================
//                  Structure Declaration
//=============================================================================
typedef enum
{
    FileOperations    = 0x00U, /**< The frame type is not specified */
    PointerOperations = 0x01U, /**< The frame type is video */
} GET_DATA_MODE;

/* AVI Block information */
typedef struct
{
    uint32_t RiffID;   /* RiffID=='RIFF'==0X61766968 */
    uint32_t FileSize; /* AVI file size (excluding the initial 8 bytes, RIFFID and FileSize) */
    uint32_t AviID;    /* AviID=='AVI '==0X41564920 */
} AVI_HEADER;

/* AVI Block information */
typedef struct
{
    uint32_t FrameID;   /* frame ID,FrameID=='RIFF'==0X61766968 */
    uint32_t FrameSize; /* frame size */
} FRAME_HEADER;

/* LIST Block information */
typedef struct
{
    uint32_t ListID;    /* ListID=='LIST'==0X4c495354 */
    uint32_t BlockSize; /* Block size (not including the initial 8 bytes, nor are ListID and BlockSize counted) */
    uint32_t ListType;  /* LIST sub-block type :hdrl(information block)/movi(data block)/idxl(index block, optional) */
} LIST_HEADER;

/* avih Sub-block information */
typedef struct
{
    uint32_t BlockID;            /* Block flag:avih==0X61766968 */
    uint32_t BlockSize;          /* Block size (excluding the initial 8 bytes, i.e., BlockID and BlockSize are not counted) */
    uint32_t SecPerFrame;        /* Video frame interval time (in us) */
    uint32_t MaxByteSec;         /* Maximum data transfer rate in bytes/s */
    uint32_t PaddingGranularity; /* Granularity of data padding */
    uint32_t Flags;              /* Global markers of AVI files, such as whether they contain index blocks, and so on */
    uint32_t TotalFrame;         /* Total number of frames in the file */
    uint32_t InitFrames;         /* Specifies the initial number of的 frames for interactive formats (0 for non-interactive formats) */
    uint32_t Streams;            /* Number of streams to include, usually two */
    uint32_t RefBufSize;         /* The recommended cache size for reading this file (the largest block that should fit) is probably 1M bytes by default!! */
    uint32_t Width;              /* Image width */
    uint32_t Height;             /* Image height */
    uint32_t Reserved[4];        /* */
} AVIH_HEADER;

/* strh Flow head sub-block information(strh∈strl) */
typedef struct
{
    uint32_t BlockID;    /* Block flag:strh==0X73747268 */
    uint32_t BlockSize;  /* Block size (excluding the initial 8 bytes, i.e., BlockID and BlockSize are not counted) */
    uint32_t StreamType; /* Stream type, vids(0X73646976): video; auds(0X73647561): Audio */
    uint32_t Handler;    /* Specifies the stream handler, which in the case of audio and video is the decoder, such as MJPG/H264 */
    uint32_t Flags;      /* Flag: Is this stream output allowed? Does the color palette change? */
    uint16_t Priority;   /* Stream priority (when there are multiple streams of the same type, the highest priority is the default stream) */
    uint16_t Language;   /* The language code for audio */
    uint32_t InitFrames; /* Specify an initial number of frames for the interactive format */
    uint32_t Scale;      /* Amount of data, the size of the video per frame or the sample size of the audio */
    uint32_t Rate;       /* Scale/Rate = Samples per second*/
    uint32_t Start;      /* The position, in Scale, at which the stream begins to play */
    uint32_t Length;     /* The amount of data in a stream, in Scale */
    uint32_t RefBufSize; /* Recommended buffer size */
    uint32_t Quality;    /* Decompress the quality parameter, larger values indicate better quality */
    uint32_t SampleSize; /* Sample size of the audio */
    struct               /* The rectangle occupied by the video frame */
    {
        short Left;
        short Top;
        short Right;
        short Bottom;
    } Frame;
} STRH_HEADER;

/* BMP structs */
typedef struct
{
    uint32_t BmpSize;      /* bmp structure size, inclusive (BmpSize included) */
    long     Width;        /* Image width */
    long     Height;       /* Image height */
    uint16_t Planes;       /* Number of planes, must be 1 */
    uint16_t BitCount;     /* Number of pixels,0X0018 for 24 */
    uint32_t Compression;  /* Compression type e.g. MJPG/H264 etc */
    uint32_t SizeImage;    /* Image size */
    long     XpixPerMeter; /* Horizontal resolution */
    long     YpixPerMeter; /* Vertical resolution */
    uint32_t ClrUsed;      /* The actual number of colors in the palette is used, not in the compressed format */
    uint32_t ClrImportant; /* Important colors */
} BMP_HEADER;

/* Color chart */
typedef struct
{
    uint8_t rgbBlue;     /* The brightness of blue (0-255) */
    uint8_t rgbGreen;    /* The brightness of green (0-255) */
    uint8_t rgbRed;      /* The brightness of red (0-255) */
    uint8_t rgbReserved; /* zero */
} AVIRGBQUAD;

/* For strh, if it is a video stream,strf(stream format) makes the STRH_BMPHEADER block */
typedef struct
{
    uint32_t   BlockID;     /* Block flag,strf==0X73747266 */
    uint32_t   BlockSize;   /* Block size (excluding the initial 8 bytes, i.e., BlockID and local BlockSize are not counted) */
    BMP_HEADER bmiHeader;   /* Bitmap information header */
    AVIRGBQUAD bmColors[1]; /* Color chart */
} STRF_BMPHEADER;

/* For strh, if it is an audio stream,strf(stream format) makes the STRH_WAVHEADER block */
typedef struct
{
    uint32_t BlockID;    /* Block flag,strf==0X73747266 */
    uint32_t BlockSize;  /* Block size (excluding the initial 8 bytes, i.e., BlockID and local BlockSize are not counted) */
    uint16_t FormatTag;  /* Format flag:0X0001=PCM,0X0055=MP3 */
    uint16_t Channels;   /* The number of channels, generally 2, represents stereo sound */
    uint32_t SampleRate; /* Audio sampling rate */
    uint32_t BaudRate;   /* Baud rate */
    uint16_t BlockAlign; /* Data block alignment flags */
    uint16_t Size;       /* Size of the structure */
} STRF_WAVHEADER;

/* idx1 header*/
typedef struct
{
    uint32_t BlockID;   /* Block flag,idx1 */
    uint32_t BlockSize; /* Block size (excluding the initial 8 bytes, i.e., BlockID and local BlockSize are not counted) */
} IDX1_HEADER;

/* idx1 info*/
typedef struct
{
    uint32_t ChunkId; /* The four-character code for this data block */
    uint32_t Flags;   /* Indicates whether the block is a keyframe, a 'rec' list, and so on */
    uint32_t Offset;  /* The offset of this block in the file */
    uint32_t Size;    /* The size of this data block */
} IDX1_INFO;

/* junk header*/
typedef struct
{
    uint32_t BlockID;   /* Block flag,junk */
    uint32_t BlockSize; /* Block size (excluding the initial 8 bytes, i.e., BlockID and local BlockSize are not counted) */
} JUNK_HEADER;

typedef struct _AVI_RES_T
{
    FIL    *fp;
    uint8_t mode;
    uint8_t is_eof;
} AVI_RES_T;

/**
 * \brief struct for avi header information
 * \ingroup filemanager_struct_define_gr
 */
typedef struct _AVI_INFO_HEADER_T
{
    AVI_HEADER     avi_header;
    AVIH_HEADER    avih_header;
    STRH_HEADER    strh_audio_header;
    STRF_WAVHEADER strf_wav_header;
    STRH_HEADER    strh_video_header;
    AVIRGBQUAD     strf_rgb_quad;
    STRF_BMPHEADER strf_bmp_header;
} AVI_INFO_HEADER_T;

/**
 * \brief struct for avi list information
 * \ingroup filemanager_struct_define_gr
 */
typedef struct _AVI_INFO_LIST_T
{
    uint32_t chunk_offset; // The offset of a data block in a file
    uint32_t chunk_size;   // data block size
    uint32_t chunk_id;     // data block ID
} AVI_INFO_CHUNK_T;

typedef struct _AVI_FILE_RES_T
{
    FIL              *fp;                        // AVI fatfs file handler
    uint8_t           dev;                       // Physical/Logical drive index
    uint8_t           mode;                      // AVI at mode which is read or write
    AVI_STATUS_T      status;                    // AVI status
    AVI_INFO_T        info;                      // AVI info
    AVI_INFO_HEADER_T head;                      // AVI head
    uint8_t          *head_data;                 // AVI head data
    uint32_t          head_size;                 // AVI head size by BYTE
    uint32_t          idx1_info_offset;          // idx1 info offset
    uint32_t          current_video_frame;       // video frame offset
    uint32_t          current_video_frame_index; // Current video frame index
    uint32_t          video_total_frame;         // Total number of video frames
    uint32_t          current_audio_frame;       // audio frame offset
    uint32_t          current_audio_frame_index; // Current audio frame index
    uint32_t          audio_total_frame;         // Total number of audio frames
    uint32_t          current_frame;             // frame offset
    uint32_t          current_frame_index;       // Current frame index
    uint32_t          total_frame;               // Total number of frames
    uint32_t          idx1_size;                 // Size of idx1
    uint32_t          idx1_dwBlockOffset_ex;     // 0 if dwBlockOffset represents an offset based on the beginning of the file. If offset from movi, is the offset of movi
} AVI_FILE_RES_T;

extern TYPE_WRAPPER AVI_Wrapper;

#endif /* _AVI_WRAPPER_H_ */
