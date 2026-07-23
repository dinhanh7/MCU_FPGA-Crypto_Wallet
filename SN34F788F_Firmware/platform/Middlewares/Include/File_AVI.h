/***********************
 * SN34F78X SDK V2.1.4 *
 ***********************/

#ifndef _FILE_AVI_H_
#define _FILE_AVI_H_

#include "sn34f78x_hal_def.h"
//=============================================================================
//                  Structure Declaration
//=============================================================================
/**
 * \brief AVI Frame types
 * \ingroup filemanager_avi_para
 */
typedef enum
{
    FrameAll   = 0x00U, /**< The frame type is not specified */
    FrameVideo = 0x01U, /**< The frame type is video */
    FrameAudio = 0x02U, /**< The frame type is audio */
} AVI_FRAME_TYPE;

/**
 * \brief AVI_INIT_T
 * \ingroup filemanager_avi_struct
 */
typedef struct _AVI_INIT_T
{
    uint8_t *head_data; /**< avi head data. */
    uint32_t head_size; /**< avi head size by BYTE. */
} AVI_INIT_T;
typedef AVI_INIT_T AVI_INFO_T;

/**
 * \brief AVI_STATUS_T
 * \ingroup filemanager_avi_struct
 */
typedef struct _AVI_STATUS_T
{
    uint32_t SecPerFrame;      /* Video frame interval time (in us) */
    uint32_t TotalFrame;       /* Total number of frames in the file */
    uint8_t *head_data;        /**< avi head data. */
    uint32_t head_size;        /**< avi head size by BYTE. */
    uint32_t Width;            /* Image width */
    uint32_t Height;           /* Image height */
    uint32_t SampleRate;       /* Audio sampling rate */
    uint16_t Channels;         /* The number of channels, generally 2, represents stereo sound */
    uint16_t AudioType;        /* Audio type :0X0001=PCM; 0X0050=MP2; 0X0055=MP3; 0X2000=AC3; */
    uint32_t VideoDecoderType; /* Video Decoder type :0X47504A4D = MJPG;*/
    uint32_t AudioDecoderType; /* Audio Decoder type :0X1 = PCM;*/
    uint32_t MaxFrameSize;     /* Max Frame Size */
    char    *VideoFLAG;        /* Video frame marker,VideoFLAG="00dc"/"01dc" */
    char    *AudioFLAG;        /* Audio frame marker,AudioFLAG="00wb"/"01wb" */
    uint8_t  is_a_eof;         /**< AVI audio is eof or not */
    uint8_t  is_v_eof;         /**< AVI video eof or not */
    uint8_t  is_eof;           /**< AVI eof or not */
} AVI_STATUS_T;

/**
 * \brief AVI_SEEK_T
 * \ingroup filemanager_avi_struct
 */
typedef struct _AVI_SEEK_T
{
    uint32_t       frame_index; /**< seek to position */
    AVI_FRAME_TYPE frame_type;  /**< audio or video */
} AVI_SEEK_T;

/**
 * \brief AVI_FRAME_INFO_T
 * \ingroup filemanager_avi_struct
 */
typedef struct _AVI_FRAME_INFO_T
{
    AVI_FRAME_TYPE frame_type; /**< audio or video */
    uint32_t       frame_flag; /**< Identifies whether the data block is a key frame (I-frame), Indicates whether the data block contains additional information such as padding bytes and whether it belongs to a subindex */
} AVI_FRAME_INFO_T;

#endif /* _FILE_AVI_H_ */
