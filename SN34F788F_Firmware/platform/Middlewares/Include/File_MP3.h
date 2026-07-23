/***********************
 * SN34F78X SDK V2.1.4 *
 ***********************/

#ifndef _FILE_MP3_H_
#define _FILE_MP3_H_

#include "sn34f78x_hal_def.h"
#define MP3_ERR_MAX MP3_ERR_END
//=============================================================================
//                  Structure Declaration
//=============================================================================
/**
 * \brief Negative values indicate errors of MP3.
 * \ingroup filemanager_mp3_para
 * \details The MP3 has additional status error codes that are listed below. \n
 * range is [-0x100, 0].
 */
typedef enum _MP3_RET_CODE
{
    MP3_ERR_BEG = -0x100,
    MP3_MSG_LACK_DATA, /**< mp3 lack data */
    MP3_MSG_LACK_BUFF, /**< mp3 lack buffer */
    MP3_ERR_END,
} MP3_RET_CODE;

/**
 * \brief enum for mp3 channel
 * \ingroup filemanager_mp3_para
 */
typedef enum _MP3_CHANNEL_T
{
    MP3_CHANNEL_MONO   = 1, /**< mono   channel */
    MP3_CHANNEL_STEREO = 2, /**< stereo channel */
    MP3_CHANNEL_MAX,
} MP3_CHANNEL_T;

/**
 * \brief enum for mp3 layer version
 * \ingroup filemanager_mp3_para
 */
typedef enum _MP3_LAYER_T
{
    MP3_LAYER_1, /**< LAYER 1 */
    MP3_LAYER_2, /**< LAYER 2 */
    MP3_LAYER_3, /**< LAYER 3 */
    MP3_LAYER_MAX,
} MP3_LAYER_T;

/**
 * \brief enum for mp3 mpeg version
 * \ingroup filemanager_mp3_para
 */
typedef enum _MP3_MPEG_T
{
    MP3_MPEG_V4, /**< MPEG2.5 */
    MP3_MPEG_V3, /**< reserve */
    MP3_MPEG_V2, /**< MPEG2 */
    MP3_MPEG_V1, /**< MPEG1 */
    MP3_MPEG_MAX,
} MP3_MPEG_T;

/**
 * \brief enum for mp3 sample rate
 * \ingroup filemanager_mp3_para
 */
typedef enum _MP3_SAMPLE_RATE_BASE_T
{
    MP3_SAMPLE_RATE_11025, /**< base 11025 HZ/s, if MP3_MPEG_V4 rate*=1, if MP3_MPEG_V2 rate*=2, if MP3_MPEG_V1 rate*=4 */
    MP3_SAMPLE_RATE_12000, /**< base 12000 HZ/s, if MP3_MPEG_V4 rate*=1, if MP3_MPEG_V2 rate*=2, if MP3_MPEG_V1 rate*=4 */
    MP3_SAMPLE_RATE_8000,  /**< base 8000  HZ/s, if MP3_MPEG_V4 rate*=1, if MP3_MPEG_V2 rate*=2, if MP3_MPEG_V1 rate*=4 */
} MP3_SAMPLE_RATE_BASE_T;

/**
 * \brief struct for mp3 initialize information
 * \ingroup filemanager_mp3_struct
 */
typedef struct _MP3_INIT_T
{
    MP3_MPEG_T             ver;         /**< MPEG Audio version ID, see /ref MP3_MPEG_T */
    MP3_LAYER_T            layer;       /**< Layer description, see /ref MP3_LAYER_T */
    uint16_t               bit_rate;    /**< Bitrate unint: kb/s=1000bit/s */
    MP3_SAMPLE_RATE_BASE_T sample_rate; /**< Sampling rate frequency, see \ref MP3_SAMPLE_RATE_BASE_T */
    MP3_CHANNEL_T          channel;     /**< Channel Mode, see \ref MP3_CHANNEL_T */
} MP3_INIT_T;
typedef MP3_INIT_T MP3_INFO_T;

/**
 * \brief struct for mp3 status
 * \ingroup filemanager_mp3_struct
 */
typedef struct _MP3_STATUS_T
{
    MP3_LAYER_T   layer;         /**< Layer description, see /ref MP3_LAYER_T */
    MP3_MPEG_T    ver;           /**< MPEG Audio version ID, see /ref MP3_MPEG_T*/
    uint16_t      bit_rate;      /**< Bitrate unint: kb/s=1000bit/s */
    uint16_t      sample_rate;   /**< Sampling rate frequency, unint:hz/s */
    uint16_t      sample_point;  /**< Sampling point number */
    MP3_CHANNEL_T channel;       /**< Channel Mode, see \ref MP3_CHANNEL_T */
    uint8_t       is_eof;        /**< MP3 is eof or not */
    uint8_t       ratio_pos;     /**< MP3 current pos by percent (unint:1%), not use it in M_FILE_STREAM */
    uint32_t      frame_num_vbr; /**< The total number of frames in the VBR mode */
    uint32_t      id3_size;      /**< ID3  information size, It includes (id3.1 + id3.2), unint: byte*/
} MP3_STATUS_T;

/**
 * \brief struct for mp3 seek information
 * \ingroup filemanager_mp3_struct
 */
typedef struct _MP3_SEEK_T
{
    uint8_t seek_ratio; /**< seek file by ratio [0, 100], as same as [seek_range_beg, seek_range_end] */
} MP3_SEEK_T;

//=============================================================================
//                  other usage for get MP3 frame data
//=============================================================================
/**
 * \brief struct for mp3 parser handler
 * \ingroup filemanager_mp3_struct
 */
typedef struct _MP3_HANDLE_T
{
    uint8_t  is_eof;    /**< check this field for get eof status */
    uint8_t  last_buff; /**< tell mp3 parser where is ending, 0:not ending, 1:at ending */
    uint32_t buff_addr; /**< tell mp3 parser working area */
    uint32_t buff_size; /**< tell mp3 parser working area size */
    uint32_t frame_len; /**< tell mp3 parser needed frame size */
    void    *other_res; /**< reserved, init must be zero, mp3 parser will handle this */
} MP3_HANDLE_T;

/**
 * \brief struct for mp3 parser frame
 * \ingroup filemanager_mp3_struct
 */
typedef struct _MP3_FRAME_T
{
    uint8_t     *data;      /**< pointer to cache for read data */
    uint32_t     data_len;  /**< length of read data */
    uint8_t     *cache;     /**< buffer as read cache */
    uint32_t     cache_len; /**< length of read cache */
    MP3_STATUS_T status;    /**< mp3 status */
} MP3_FRAME_T;

/**
 * \brief interface for mp3 read frame from a buff
 * \ingroup filemanager_mp3_struct
 */
typedef struct _MP3_PARSER
{
    int32_t (*init)(MP3_HANDLE_T *fh_mp3);                          /**< initialize the parser  */
    int32_t (*ReadFrame)(MP3_HANDLE_T *fh_mp3, MP3_FRAME_T *frame); /**< read data from the file by frame */
    int32_t (*uninit)(MP3_HANDLE_T *fh_mp3);                        /**< de-initialize the parser  */
} const MP3_PARSER;

extern MP3_PARSER MP3_Parser;

#endif /* _FILE_MP3_H_ */
