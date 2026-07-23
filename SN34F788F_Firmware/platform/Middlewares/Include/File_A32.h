/***********************
 * SN34F78X SDK V2.1.4 *
 ***********************/

#ifndef _FILE_A32_H_
#define _FILE_A32_H_

#include "sn34f78x_hal_def.h"
#define A32_ERR_MAX A32_ERR_END

//=============================================================================
//                  Structure Declaration
//=============================================================================
/**
 * \brief Negative values indicate errors of A32.
 * \ingroup filemanager_a32_para
 * \details The A32 has additional status error codes that are listed below. \n
 * range is [-0x100, 0].
 */
typedef enum _A32_RET_CODE
{
    A32_ERR_BEG = -0x100,
    A32_MSG_LACK_DATA, /**< a32 lack data */
    A32_MSG_LACK_BUFF, /**< a32 lack buff */
    A32_ERR_END,
} A32_RET_CODE;

/**
 * \brief A32_INIT_T
 * \ingroup filemanager_a32_struct
 * \details The structure define the audio32 file initialization info
 */
typedef struct _A32_INIT_T
{
    uint16_t format;          /**< a32 format */
    uint16_t num_of_channels; /**< a32 channels */
    uint32_t bit_rate;        /**< a32 bit rate */
    uint32_t sample_rate;     /**< a32 sample rate */
} A32_INIT_T;
typedef A32_INIT_T A32_INFO_T;

/**
 * \brief A32_STATUS_T
 * \ingroup filemanager_a32_struct
 */
typedef struct _A32_STATUS_T
{
    uint32_t frame_cnt;       /**< a32 frame count */
    uint32_t frame_idx;       /**< a32 frame index, not use it in M_FILE_STREAM */
    uint16_t format;          /**< a32 format */
    uint16_t num_of_channels; /**< a32 channels */
    uint32_t bit_rate;        /**< a32 bit rate */
    uint32_t sample_rate;     /**< a32 sample rate */
    uint8_t  is_eof;          /**< a32 file is end or not */
} A32_STATUS_T;

/**
 * \brief A32_SEEK_T
 * \ingroup filemanager_a32_struct
 */
typedef struct _A32_SEEK_T
{
    uint64_t index; /**< seek by index, unint: frame */
} A32_SEEK_T;

/////////////////////////////////////////////////////////////////////////////
/**
 * \brief struct for a32 parser handler
 * \ingroup filemanager_a32_struct
 */
typedef struct _A32_HANDLE_T
{
    uint8_t  is_eof;    /**< check this field for get eof status */
    uint8_t  last_buff; /**< tell a32 parser where is ending, 0:not ending, 1:at ending */
    uint32_t buff_addr; /**< tell a32 parser working area */
    uint32_t buff_size; /**< tell a32 parser working area size */
    uint32_t frame_len; /**< tell a32 parser needed frame size */
    void    *other_res; /**< reserved, init must be zero, a32 parser will handle this */
} A32_HANDLE_T;

/**
 * \brief struct for a32 parser frame
 * \ingroup filemanager_a32_struct
 */
typedef struct _A32_FRAME_T
{
    uint8_t     *data;      /**< pointer to cache for read data */
    uint32_t     data_len;  /**< length of read data */
    uint8_t     *cache;     /**< buffer as read cache */
    uint32_t     cache_len; /**< length of read cache */
    A32_STATUS_T status;    /**< a32 status */
} A32_FRAME_T;

#endif /* _FILE_A32_H_ */
