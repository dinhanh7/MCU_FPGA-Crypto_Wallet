/***********************
 * SN34F78X SDK V2.1.4 *
 ***********************/

#ifndef _FILE_WAV_H_
#define _FILE_WAV_H_

#include "sn34f78x_hal_def.h"
#define WAV_ERR_MAX WAV_ERR_END

//=============================================================================
//                  Structure Declaration
//=============================================================================
/**
 * \brief Negative values indicate errors of WAV.
 * \ingroup filemanager_wav_para
 * \details The WAV has additional status error codes that are listed below. \n
 * range is [-0x100, 0].
 */
typedef enum _WAV_RET_CODE
{
    WAV_ERR_BEG = -0x100,
    WAV_MSG_LACK_DATA, /**< wav lack data */
    WAV_MSG_LACK_BUFF, /**< wav lack buff */
    WAV_ERR_END,
} WAV_RET_CODE;

/**
 * \brief WAV_FORMAT_T
 * \ingroup filemanager_wav_para
 * \details The enum define the wave format
 */
typedef enum _WAV_FORMAT_T
{
    WAV_PCM = 0x01, /**< wav format, 0x01->PCM */
    WAV_IMA = 0x11, /**< wav format, 0x11->IMA */
} WAV_FORMAT_T;

/**
 * \brief WAV_INIT_T
 * \ingroup filemanager_wav_struct
 * \details The structure define the wave file initialization info
 */
typedef struct _WAV_INIT_T
{
    uint16_t audio_format;    /**< wav format, see \ref WAV_FORMAT_T  */
    uint16_t num_of_channels; /**< wav channels, 0x01- mono, 0x02- stereo */
    uint32_t bits_per_sample; /**< wav bits per sample, 8- 8bits, 16- 16 bits */
    uint32_t sample_rate;     /**< wav sample rate, unint:HZ */
    uint16_t block_align;     /**< wav block align, just for ima pcm */
} WAV_INIT_T;
typedef WAV_INIT_T WAV_INFO_T;

/**
 * \brief WAV_STATUS_T
 * \ingroup filemanager_wav_struct
 */
typedef struct _WAV_STATUS_T
{
    uint32_t audio_frame_cnt; /**< wav frame count */
    uint32_t audio_frame_idx; /**< wav frame index, not use it in M_FILE_STREAM */
    uint16_t audio_format;    /**< wav format */
    uint16_t num_of_channels; /**< wav channels */
    uint32_t bits_per_sample; /**< wav bits per sample */
    uint32_t sample_rate;     /**< wav sample rate */
    uint16_t block_align;     /**< wav block align, just for ima pcm */
    uint8_t  is_eof;          /**< wav file is end or not */
} WAV_STATUS_T;

/**
 * \brief WAV_SEEK_T
 * \ingroup filemanager_wav_struct
 */
typedef struct _WAV_SEEK_T
{
    uint64_t index; /**< seek by frame index */
} WAV_SEEK_T;

/////////////////////////////////////////////////////////////////////////////
/**
 * \brief struct for wav parser handler
 * \ingroup filemanager_wav_struct
 */
typedef struct _WAV_HANDLE_T
{
    uint8_t  is_eof;    /**< check this field for get eof status */
    uint8_t  last_buff; /**< tell wav parser where is ending, 0:not ending, 1:at ending */
    uint32_t buff_addr; /**< tell wav parser working area */
    uint32_t buff_size; /**< tell wav parser working area size */
    uint32_t frame_len; /**< tell wav parser needed frame size */
    void    *other_res; /**< reserved, init must be zero, wav parser will handle this */
} WAV_HANDLE_T;

/**
 * \brief struct for wav parser frame
 * \ingroup filemanager_wav_struct
 */
typedef struct _WAV_FRAME_T
{
    uint8_t     *data;      /**< pointer to cache for read data */
    uint32_t     data_len;  /**< length of read data */
    uint8_t     *cache;     /**< buffer as read cache */
    uint32_t     cache_len; /**< length of read cache */
    WAV_STATUS_T status;    /**< wav status */
} WAV_FRAME_T;

#endif /* _FILE_WAV_H_ */
