/***********************
 * SN34F78X SDK V2.1.4 *
 ***********************/

#include "avi_wrapper.h"

char *const AVI_VIDS_FLAG_TBL[2] = {"00dc", "01dc"}; /* Video encoding flag string,00dc/01dc */
char *const AVI_AUDS_FLAG_TBL[2] = {"00wb", "01wb"}; /* Audio encoding flag string,00wb/01wb */

static IDX1_HEADER idx1_header_info         = {0};
static LIST_HEADER movi_header_info         = {0};
static uint32_t    data_offset              = 0;
static FIL         idx1_file                = {0};
static TCHAR       path_name[FILE_NAME_LEN] = {0};
//////////////////////////////////////////////////////////////////////////////////////////////
/**
 * @brief Searches the given buffer for the specified ID
 *
 * Searches the given buffer for the specified ID and returns the position of the ID in the buffer if found or -1 otherwise.
 *
 * @param buf Buffer pointer
 * @param size Buffer size
 * @param id The ID string to search for
 *
 * @return If the ID is found, its position in the buffer is returned; Otherwise, -1 is returned
 */
static int16_t avi_srarch_id(uint8_t *buf, uint32_t size, char *id)
{
    uint32_t i;

    for (i = 0; i < size; i++)
    {
        if ((buf[i] == id[0]) &&
            (buf[i + 1] == id[1]) &&
            (buf[i + 2] == id[2]) &&
            (buf[i + 3] == id[3]))
        {
            return i;
        }
    }

    return -1;
}

/**
 * @brief Find and count the number of occurrences of the specified ID in the buffer
 *
 * Finds the number of occurrences of the specified ID in a given buffer. Each time an ID is found,
 * t is checked whether the frame immediately following is at least 16 bytes in size, and if so,
 * the counter is incremented by one.
 *
 * @param buf Buffer pointer
 * @param size uffer size
 * @param id The ID string to search for
 *
 * @return The number of valid ids found
 */
static uint32_t avi_record_id_number(uint8_t *buf, uint32_t size, char *id)
{
    uint32_t i;
    uint32_t ret = 0;

    for (i = 0; i < size; i++)
    {
        if ((buf[i] == id[0]) &&
            (buf[i + 1] == id[1]) &&
            (buf[i + 2] == id[2]) &&
            (buf[i + 3] == id[3]))
        {
            ret++;
        }
    }

    return ret;
}

/**
 * @brief Read data from files or Pointers according to the schema
 *
 * Reads data of a specified size from a file or pointer to a buffer according to a given pattern (file operation or pointer operation).
 *
 * @param seek From the start of a file or pointer
 * @param read_size A pointer to the size of the read data
 * @param read_buff A pointer to the buffer where the read data is stored
 * @param ptr A pointer to a file or data
 * @param mode Operation mode (of type GET_DATA_MODE), which can be FileOperations or PointerOperations
 *
 * @return The result of the operation is 0 on success and nonzero on failure
 */
static int32_t _data_seek_and_read(uint64_t seek, uint32_t *read_size, void *read_buff, void *ptr, GET_DATA_MODE mode)
{
    int32_t ret = 0;

    if (mode == FileOperations) // file operations mode
    {
        ret = _file_seek_and_read(seek, read_size, read_buff, ptr);
    }
    else if (mode == PointerOperations) // pointer operations mode
    {
        if (seek == FILE_NOT_SEEK)
            seek = 0;

        memcpy(read_buff, (uint8_t *)ptr + seek, *read_size);
    }

    return ret;
}

static int32_t _avi_removal(void *header, uint32_t *offset)
{
    JUNK_HEADER *junk_header = header;
    LIST_HEADER *info_header = header;

    /* If it is a junk field, it is ignored */
    if (junk_header->BlockID == 0x4B4E554A)
    {
        (*offset) += junk_header->BlockSize + sizeof(JUNK_HEADER);

        /* If it is a junk block, 1 is returned */
        return 1;
    }

    /* If it is a info field, it is ignored */
    if (info_header->ListType == 0x4F464E49)
    {
        (*offset) += info_header->BlockSize + sizeof(LIST_HEADER) - 4;

        /* If it is a info block, 2 is returned */
        return 2;
    }

    return 0;
}

/**
 * @brief Parse AVI file headers
 *
 * This function is used to parse various header information of AVI files, including AVI file header, list header, video stream header, audio stream header, and so on.
 *
 * @param avi AVI file resource pointer
 * @param ptr Data pointer to AVI file data
 * @param mode Data read mode
 *
 * @return The return value indicates whether the operation was successful, returning HAL_OK on success and HAL_ERROR on failure
 */
static int32_t _avi_parser_header(AVI_FILE_RES_T *avi, void *ptr, GET_DATA_MODE mode)
{
    int32_t     ret_sonix       = HAL_OK;
    int32_t     ret_junk        = 0;
    LIST_HEADER list_header     = {0};
    STRH_HEADER strh_header     = {0};
    uint32_t    avi_header_len  = 0;
    uint32_t    list_header_len = 0;
    uint32_t    avih_header_len = 0;
    uint32_t    strh_header_len = 0;
    uint32_t    strf_bmp_len    = 0;
    uint32_t    strf_wav_len    = 0;

    /* Get AVI Header */
    avi->head_size = 0;
    avi_header_len = sizeof(AVI_HEADER);
    ret_sonix      = _data_seek_and_read(FILE_NOT_SEEK, &avi_header_len, &avi->head.avi_header, ptr, mode);
    MFILE_DEBUG_ERROR(ret_sonix != HAL_OK, "Get avi header fail");
    CHECK_MIDDLEWARE_RET(ret_sonix);

    /* Check AVI ID */
    if ((avi->head.avi_header.RiffID != AVI_RIFF_ID) || (avi->head.avi_header.AviID != AVI_AVI_ID))
    {
        MFILE_DEBUG_ERROR(avi->head.avi_header.RiffID != AVI_RIFF_ID, "avi RIFF ID error");
        MFILE_DEBUG_ERROR(avi->head.avi_header.AviID != AVI_AVI_ID, "avi AVI ID error");
        return HAL_ERROR;
    }
    avi->head_size += avi_header_len;

    do
    {
        /* Get AVI list Header */
        list_header_len = sizeof(LIST_HEADER);
        ret_sonix       = _data_seek_and_read(avi->head_size, &list_header_len, &list_header, ptr, mode);
        MFILE_DEBUG_ERROR(ret_sonix != HAL_OK, "Get avi list header fail");
        CHECK_MIDDLEWARE_RET(ret_sonix);

        /* Skip junk Quick */
        ret_junk = _avi_removal((void *)(&list_header), &avi->head_size);

    } while (ret_junk != 0);

    /* Check LIST ID */
    if ((list_header.ListID != AVI_LIST_ID) || (list_header.ListType != AVI_HDRL_ID))
    {
        MFILE_DEBUG_ERROR(list_header.ListID != AVI_LIST_ID, "avi LIST ID error");
        MFILE_DEBUG_ERROR(list_header.ListType != AVI_HDRL_ID, "avi HDRL ID error");
        return HAL_ERROR;
    }
    avi->head_size += list_header_len;

    do
    {
        /* Get AVI avih Header */
        avih_header_len = sizeof(AVIH_HEADER);
        ret_sonix       = _data_seek_and_read(avi->head_size, &avih_header_len, &avi->head.avih_header, ptr, mode);
        MFILE_DEBUG_ERROR(ret_sonix != HAL_OK, "Get avi avih header fail");
        CHECK_MIDDLEWARE_RET(ret_sonix);

        /* Skip junk Quick */
        ret_junk = _avi_removal((void *)(&avi->head.avih_header), &avi->head_size);

    } while (ret_junk != 0);

    /* Check avih ID */
    if (avi->head.avih_header.BlockID != AVI_AVIH_ID)
    {
        MFILE_DEBUG_ERROR(avi->head.avih_header.BlockID != AVI_AVIH_ID, "avi AVIH ID error");
        return HAL_ERROR;
    }
    avi->status.SecPerFrame = avi->head.avih_header.SecPerFrame; /* Get the frame interval time */
    avi->status.TotalFrame  = avi->head.avih_header.TotalFrame;  /* The total number of frames is obtained */
    avi->head_size += avi->head.avih_header.BlockSize + 8;

    do
    {
        /* Get AVI list Header */
        list_header_len = sizeof(LIST_HEADER);
        ret_sonix       = _data_seek_and_read(avi->head_size, &list_header_len, &list_header, ptr, mode);
        MFILE_DEBUG_ERROR(ret_sonix != HAL_OK, "Get avi list header fail");
        CHECK_MIDDLEWARE_RET(ret_sonix);

        /* Skip junk Quick */
        ret_junk = _avi_removal((void *)(&list_header), &avi->head_size);

    } while (ret_junk != 0);

    /* Check LIST ID */
    if ((list_header.ListID != AVI_LIST_ID) || (list_header.ListType != AVI_STRL_ID))
    {
        MFILE_DEBUG_ERROR(list_header.ListID != AVI_LIST_ID, "avi LIST ID error");
        MFILE_DEBUG_ERROR(list_header.ListType != AVI_STRL_ID, "avi STRL ID error");
        return HAL_ERROR;
    }

    do
    {
        /* Get AVI strh Header */
        strh_header_len = sizeof(STRH_HEADER);
        ret_sonix       = _data_seek_and_read(avi->head_size + list_header_len, &strh_header_len, &strh_header, ptr, mode);
        MFILE_DEBUG_ERROR(ret_sonix != HAL_OK, "Get avi strh header fail");
        CHECK_MIDDLEWARE_RET(ret_sonix);

        /* Skip junk Quick */
        ret_junk = _avi_removal((void *)(&strh_header), &avi->head_size);

    } while (ret_junk != 0);

    if (strh_header.BlockID != AVI_STRH_ID)
    {
        MFILE_DEBUG_ERROR(strh_header.BlockID != AVI_STRH_ID, "avi strh header error");
        return HAL_ERROR;
    }

    if (strh_header.StreamType == AVI_VIDS_STREAM) /* The video frame is in the front */
    {
        memcpy(&avi->head.strh_video_header, &strh_header, strh_header_len);

        avi->status.VideoFLAG = AVI_VIDS_FLAG_TBL[0]; /* Video stream tagging  "00dc" */
        avi->status.AudioFLAG = AVI_AUDS_FLAG_TBL[1]; /* Audio stream tagging  "01wb" */

        /* Get Video Decoder */
        avi->status.VideoDecoderType = avi->head.strh_video_header.Handler;

        do
        {
            /* Get AVI strf */
            strf_bmp_len = sizeof(STRF_BMPHEADER);
            ret_sonix    = _data_seek_and_read(avi->head_size + list_header_len + strh_header.BlockSize + 8, &strf_bmp_len, &avi->head.strf_bmp_header, ptr, mode);
            MFILE_DEBUG_ERROR(ret_sonix != HAL_OK, "Get avi strf bmp fail");
            CHECK_MIDDLEWARE_RET(ret_sonix);

            /* Skip junk Quick */
            ret_junk = _avi_removal((void *)(&avi->head.strf_bmp_header), &avi->head_size);

        } while (ret_junk != 0);

        if (avi->head.strf_bmp_header.BlockID != AVI_STRF_ID)
        {
            MFILE_DEBUG_ERROR(avi->head.strf_bmp_header.BlockID != AVI_STRF_ID, "avi STRF ID error");
            return HAL_ERROR;
        }

        avi->status.Width  = avi->head.strf_bmp_header.bmiHeader.Width;
        avi->status.Height = avi->head.strf_bmp_header.bmiHeader.Height;
        avi->head_size += list_header.BlockSize + 8;

        do
        {
            /* Get AVI list Header */
            list_header_len = sizeof(LIST_HEADER);
            ret_sonix       = _data_seek_and_read(avi->head_size, &list_header_len, &list_header, ptr, mode);
            MFILE_DEBUG_ERROR(ret_sonix != HAL_OK, "Get avi list header fail");
            CHECK_MIDDLEWARE_RET(ret_sonix);

            /* Skip junk Quick */
            ret_junk = _avi_removal((void *)(&list_header), &avi->head_size);

        } while (ret_junk != 0);

        /* Is a video file with no audio frames */
        if (list_header.ListID != AVI_LIST_ID)
        {
            avi->status.SampleRate = 0; /* Audio sampling rate */
            avi->status.Channels   = 0; /* Number of audio channels */
            avi->status.AudioType  = 0; /* Audio format */
        }
        else
        {
            if (list_header.ListType != AVI_STRL_ID)
            {
                MFILE_DEBUG_ERROR(list_header.ListType != AVI_STRL_ID, "avi STRL ID error");
                return HAL_ERROR;
            }

            do
            {
                strh_header_len = sizeof(STRH_HEADER);
                ret_sonix       = _data_seek_and_read(avi->head_size + list_header_len, &strh_header_len, &avi->head.strh_audio_header, ptr, mode);
                MFILE_DEBUG_ERROR(ret_sonix != HAL_OK, "Get avi strh audio header fail");
                CHECK_MIDDLEWARE_RET(ret_sonix);

                /* Skip junk Quick */
                ret_junk = _avi_removal((void *)(&avi->head.strh_audio_header), &avi->head_size);

            } while (ret_junk != 0);

            if ((avi->head.strh_audio_header.BlockID != AVI_STRH_ID) || (avi->head.strh_audio_header.StreamType != AVI_AUDS_STREAM))
            {
                MFILE_DEBUG_ERROR(avi->head.strh_audio_header.BlockID != AVI_STRH_ID, "avi STRH ID error");
                MFILE_DEBUG_ERROR(avi->head.strh_audio_header.StreamType != AVI_AUDS_STREAM, "avi audio format error");
                return HAL_ERROR;
            }

            /* Get Audio Decoder */
            avi->status.AudioDecoderType = avi->head.strh_audio_header.Handler;

            do
            {
                /* Get AVI strf */
                strf_wav_len = sizeof(STRF_WAVHEADER);
                ret_sonix    = _data_seek_and_read(avi->head_size + list_header_len + avi->head.strh_audio_header.BlockSize + 8, &strf_wav_len, &avi->head.strf_wav_header, ptr, mode);
                MFILE_DEBUG_ERROR(ret_sonix != HAL_OK, "Get avi strf wav fail");
                CHECK_MIDDLEWARE_RET(ret_sonix);

                /* Skip junk Quick */
                ret_junk = _avi_removal((void *)(&avi->head.strf_wav_header), &avi->head_size);

            } while (ret_junk != 0);

            if (avi->head.strf_bmp_header.BlockID != AVI_STRF_ID)
            {
                MFILE_DEBUG_ERROR(avi->head.strf_bmp_header.BlockID != AVI_STRF_ID, "avi STRF ID error");
                return HAL_ERROR;
            }

            avi->status.SampleRate = avi->head.strf_wav_header.SampleRate; /* Audio sampling rate */
            avi->status.Channels   = avi->head.strf_wav_header.Channels;   /* Number of audio channels */
            avi->status.AudioType  = avi->head.strf_wav_header.FormatTag;  /* Audio format */
            avi->head_size += list_header.BlockSize + 8;
        }
    }
    else if (strh_header.StreamType == AVI_AUDS_STREAM) /* The audio frame is in the front */
    {
        avi->status.VideoFLAG = AVI_VIDS_FLAG_TBL[1]; /* Video stream tagging  "01dc" */
        avi->status.AudioFLAG = AVI_AUDS_FLAG_TBL[0]; /* Audio stream tagging  "00wb" */

        memcpy(&avi->head.strh_audio_header, &strh_header, strh_header_len);

        /* Get Audio Decoder */
        avi->status.AudioDecoderType = avi->head.strh_audio_header.Handler;

        do
        {
            /* Get AVI strf */
            strf_wav_len = sizeof(STRF_WAVHEADER);
            ret_sonix    = _data_seek_and_read(avi->head_size + list_header_len + strh_header.BlockSize + 8, &strf_wav_len, &avi->head.strf_wav_header, ptr, mode);
            MFILE_DEBUG_ERROR(ret_sonix != HAL_OK, "Get avi strf wav fail");
            CHECK_MIDDLEWARE_RET(ret_sonix);

            /* Skip junk Quick */
            ret_junk = _avi_removal((void *)(&avi->head.strf_wav_header), &avi->head_size);

        } while (ret_junk != 0);

        if (avi->head.strf_wav_header.BlockID != AVI_STRF_ID)
        {
            MFILE_DEBUG_ERROR(avi->head.strf_wav_header.BlockID != AVI_STRF_ID, "avi STRF ID error");
            return HAL_ERROR;
        }

        avi->status.SampleRate = avi->head.strf_wav_header.SampleRate; /* Audio sampling rate */
        avi->status.Channels   = avi->head.strf_wav_header.Channels;   /* Number of audio channels */
        avi->status.AudioType  = avi->head.strf_wav_header.FormatTag;  /* Audio format */
        avi->head_size += list_header.BlockSize + 8;

        do
        {
            /* Get AVI list Header */
            list_header_len = sizeof(LIST_HEADER);
            ret_sonix       = _data_seek_and_read(avi->head_size, &list_header_len, &list_header, ptr, mode);
            MFILE_DEBUG_ERROR(ret_sonix != HAL_OK, "Get avi list header fail");
            CHECK_MIDDLEWARE_RET(ret_sonix);

            /* Skip junk Quick */
            ret_junk = _avi_removal((void *)(&list_header), &avi->head_size);

        } while (ret_junk != 0);

        /* no video frame */
        if ((list_header.ListType != AVI_STRL_ID) || (list_header.ListID != AVI_LIST_ID))
        {
            avi->status.Width  = 0;
            avi->status.Height = 0;
        }
        else
        {
            do
            {
                strh_header_len = sizeof(STRH_HEADER);
                ret_sonix       = _data_seek_and_read(avi->head_size + list_header_len, &strh_header_len, &avi->head.strh_video_header, ptr, mode);
                MFILE_DEBUG_ERROR(ret_sonix != HAL_OK, "Get avi strh video header fail");
                CHECK_MIDDLEWARE_RET(ret_sonix);

                /* Skip junk Quick */
                ret_junk = _avi_removal((void *)(&avi->head.strh_video_header), &avi->head_size);

            } while (ret_junk != 0);

            if ((avi->head.strh_video_header.BlockID != AVI_STRH_ID) || (avi->head.strh_video_header.StreamType != AVI_VIDS_STREAM))
            {
                MFILE_DEBUG_ERROR(avi->head.strh_video_header.BlockID != AVI_STRH_ID, "avi STRH ID error");
                MFILE_DEBUG_ERROR(avi->head.strh_video_header.StreamType != AVI_AUDS_STREAM, "avi video format error");
                return HAL_ERROR;
            }

            /* Get Audio Decoder */
            avi->status.VideoDecoderType = avi->head.strh_video_header.Handler;

            do
            {
                /* Get AVI strf */
                strf_bmp_len = sizeof(STRF_BMPHEADER);
                ret_sonix    = _data_seek_and_read(avi->head_size + list_header_len + avi->head.strh_video_header.BlockSize + 8, &strf_bmp_len, &avi->head.strf_bmp_header, ptr, mode);
                MFILE_DEBUG_ERROR(ret_sonix != HAL_OK, "Get avi strf bmp fail");
                CHECK_MIDDLEWARE_RET(ret_sonix);

                /* Skip junk Quick */
                ret_junk = _avi_removal((void *)(&avi->head.strf_bmp_header), &avi->head_size);

            } while (ret_junk != 0);

            if (avi->head.strf_bmp_header.BlockID != AVI_STRF_ID)
            {
                MFILE_DEBUG_ERROR(avi->head.strf_bmp_header.BlockID != AVI_STRF_ID, "avi STRF ID error");
                return HAL_ERROR;
            }

            if (avi->head.strf_bmp_header.bmiHeader.Compression != AVI_FORMAT_MJPG)
            {
                MFILE_DEBUG_ERROR(avi->head.strf_bmp_header.bmiHeader.Compression != AVI_FORMAT_MJPG, "avi video format error");
                return HAL_ERROR;
            }

            avi->status.Width  = avi->head.strf_bmp_header.bmiHeader.Width;
            avi->status.Height = avi->head.strf_bmp_header.bmiHeader.Height;
            avi->head_size += list_header.BlockSize + 8;
        }
    }

    return ret_sonix;
}

/**
 * @brief Get the header data from the AVI file
 *
 * Parse and obtain header data from AVI file resources, including file header, movi list, and idx1 index information.
 *
 * @param avi AVI file resource pointer
 *
 * @return Returns the result of the operation, HAL_OK on success and HAL_ERROR on failure
 */
static int32_t _avi_get_header_data(AVI_FILE_RES_T *avi)
{
    int32_t ret_sonix   = HAL_OK;
    int16_t movi_offset = 0;

    /* parser header */
    ret_sonix = _avi_parser_header(avi, avi->fp, FileOperations);
    MFILE_DEBUG_ERROR(ret_sonix != HAL_OK, "");
    CHECK_MIDDLEWARE_RET(ret_sonix);

    /* movi */
    uint32_t movi_pos      = avi->head_size;
    uint8_t *avi_fifo_ptr  = NULL;
    uint32_t avi_fifo_size = AVI_FIFO_SIZE;

    MEMORY_ALLOC(avi_fifo_ptr, avi_fifo_size, GFP_KERNEL);

    do
    {
        memset(avi_fifo_ptr, 0, avi_fifo_size);

        ret_sonix = _data_seek_and_read(movi_pos, &avi_fifo_size, avi_fifo_ptr, avi->fp, FileOperations);
        MFILE_DEBUG_ERROR(ret_sonix != HAL_OK, "");
        if (ret_sonix != HAL_OK)
        {
            MEMORY_FREE(avi_fifo_ptr);
            return HAL_ERROR;
        }

        movi_offset = avi_srarch_id(avi_fifo_ptr, avi_fifo_size, "movi");
        if (movi_offset != -1)
        {
            break;
        }
        movi_pos += avi_fifo_size;

    } while (movi_pos < f_size(avi->fp));

    if (movi_pos >= f_size(avi->fp))
    {
        MFILE_DEBUG_ERROR(movi_offset == -1, "find movi list fail");
        MEMORY_FREE(avi_fifo_ptr);
        return HAL_ERROR;
    }

    MEMORY_FREE(avi_fifo_ptr);
    movi_offset -= 8;

    LIST_HEADER movi_header;
    uint32_t    movi_header_len = sizeof(LIST_HEADER);

    movi_pos += movi_offset;
    ret_sonix = _data_seek_and_read(movi_pos, &movi_header_len, &movi_header, avi->fp, FileOperations);
    MFILE_DEBUG_ERROR(ret_sonix != HAL_OK, "");
    CHECK_MIDDLEWARE_RET(ret_sonix);
    avi->head_size = movi_pos;

    /* idx1 */
    uint32_t current_resolved_pos = 0;
    current_resolved_pos          = movi_pos;
    uint32_t    data_start_addr   = 0;
    IDX1_HEADER idx1_header       = {0};

    do
    {
        if (avi_fifo_ptr == NULL)
        {
            MEMORY_ALLOC(avi_fifo_ptr, avi_fifo_size, GFP_KERNEL);
        }

        ret_sonix = _data_seek_and_read(current_resolved_pos + movi_header.BlockSize + 8, &avi_fifo_size, avi_fifo_ptr, avi->fp, FileOperations);
        MFILE_DEBUG_ERROR(ret_sonix != HAL_OK, "");
        if (ret_sonix != HAL_OK)
        {
            MEMORY_FREE(avi_fifo_ptr);
            return HAL_ERROR;
        }

        int16_t idx1_offset = avi_srarch_id(avi_fifo_ptr, avi_fifo_size, "idx1");
        if (idx1_offset != -1)
        {
            data_start_addr = current_resolved_pos + movi_header.BlockSize + 8 + idx1_offset;
            break;
        }
        current_resolved_pos += avi_fifo_size;

    } while ((current_resolved_pos + movi_header.BlockSize + 8) < f_size(avi->fp));
    MEMORY_FREE(avi_fifo_ptr);

    if ((current_resolved_pos + movi_header.BlockSize + 8) >= f_size(avi->fp))
    {
        MFILE_DEBUG_ERROR(0x1, "Files without idx1 are not supported");
        return HAL_ERROR;
    }

    /* idx1 header */
    uint32_t idx1_header_len = sizeof(IDX1_HEADER);

    ret_sonix = _data_seek_and_read(data_start_addr, &idx1_header_len, &idx1_header, avi->fp, FileOperations);
    MFILE_DEBUG_ERROR(ret_sonix != HAL_OK, "");
    CHECK_MIDDLEWARE_RET(ret_sonix);

    /* Cache idx1 information */
    uint8_t *idx1_info = NULL;
    uint32_t idx1_size = idx1_header.BlockSize;

    MEMORY_ALLOC(idx1_info, idx1_size, GFP_SRAM);
    ret_sonix = _data_seek_and_read(data_start_addr + idx1_header_len, &idx1_size, idx1_info, avi->fp, FileOperations);
    MFILE_DEBUG_ERROR(ret_sonix != HAL_OK, "");
    if (ret_sonix != HAL_OK)
    {
        MEMORY_FREE(idx1_info);
        return HAL_ERROR;
    }

    avi->idx1_info_offset          = data_start_addr + idx1_header_len;
    avi->current_video_frame       = data_start_addr + idx1_header_len;
    avi->current_audio_frame       = data_start_addr + idx1_header_len;
    avi->current_frame             = data_start_addr + idx1_header_len;
    avi->current_video_frame_index = 0;
    avi->current_audio_frame_index = 0;
    avi->current_frame_index       = 0;
    avi->audio_total_frame         = avi_record_id_number(idx1_info, idx1_size, avi->status.AudioFLAG);
    avi->video_total_frame         = avi_record_id_number(idx1_info, idx1_size, avi->status.VideoFLAG);
    avi->total_frame               = avi->audio_total_frame + avi->video_total_frame;
    avi->status.TotalFrame         = avi->total_frame;

    /* Get Max Frame Size */
    uint32_t max_frame_size = 0;
    for (int cnt = 0; cnt < avi->total_frame; cnt++)
    {
        memcpy(&max_frame_size, idx1_info + 12 + cnt * sizeof(IDX1_INFO), sizeof(uint32_t));
        if (max_frame_size > avi->status.MaxFrameSize)
        {
            avi->status.MaxFrameSize = max_frame_size;
        }
    }

    /* Confirm whether dwBlockOffset is based on a file start offset or a movi offset */
    uint32_t data_offset = 0;
    memcpy(&data_offset, idx1_info + 8, sizeof(data_offset));
    if (data_offset < movi_pos) // dwBlockOffset is based on movi offset
    {
        avi->idx1_dwBlockOffset_ex = movi_pos + 8;
    }
    else // dwBlockOffset is based on a file start offset
    {
        avi->idx1_dwBlockOffset_ex = 0;
    }
    MEMORY_FREE(idx1_info);

    MEMORY_ALLOC(avi->head_data, avi->head_size, GFP_SRAM);
    memset(avi->head_data, 0, avi->head_size);
    ret_sonix             = _data_seek_and_read(0, &avi->head_size, avi->head_data, avi->fp, FileOperations);
    avi->status.head_data = avi->head_data;
    avi->status.head_size = avi->head_size;

    return ret_sonix;
}

/**
 * @brief Set AVI file header data
 *
 * This function is used to set the header data of AVI files, including parsing the header data, writing the header data,
 * setting the movi header information, and allocating memory for idx1.
 *
 * @param avi A pointer to the AVI file resource structure
 *
 * @return Returns the result of the operation, HAL_OK on success and the corresponding error code on failure
 */
static int32_t _avi_set_header_data(AVI_FILE_RES_T *avi)
{
    int32_t ret_sonix = HAL_OK;

    /* parser header */
    ret_sonix = _avi_parser_header(avi, avi->info.head_data, PointerOperations);
    MFILE_DEBUG_ERROR(ret_sonix != HAL_OK, "");
    CHECK_MIDDLEWARE_RET(ret_sonix);

    /* Write header data */
    ret_sonix = _file_seek_and_write(FILE_NOT_SEEK, avi->info.head_size, avi->info.head_data, avi->fp);
    MFILE_DEBUG_ERROR(ret_sonix != HAL_OK, "Set avi header fail");
    CHECK_MIDDLEWARE_RET(ret_sonix);

    /* movi */
    movi_header_info.ListID   = 0x5453494c; // LIST
    movi_header_info.ListType = 0x69766f6d; // movi

    ret_sonix = _file_seek_and_write(avi->info.head_size, sizeof(LIST_HEADER), &movi_header_info, avi->fp);
    MFILE_DEBUG_ERROR(ret_sonix != HAL_OK, "Set avi movi header fail");
    CHECK_MIDDLEWARE_RET(ret_sonix);

    /* write data pos */
    data_offset = avi->info.head_size + sizeof(LIST_HEADER);

    /* idx1 */
    sprintf(path_name, "%d:%s", avi->dev, "idx1.txt");
    ret_sonix = _file_create(&idx1_file, path_name);
    MFILE_DEBUG_ERROR(ret_sonix != HAL_OK, "Failed to create idx1 temporary storage file");
    CHECK_MIDDLEWARE_RET(ret_sonix);

    return HAL_OK;
}

static int32_t _avi_open(AVI_FILE_RES_T *avi)
{
    int32_t ret_sonix = HAL_OK;
    if (HAL_FLAG_HAS(avi->mode, FA_READ))
    {
        ret_sonix = _avi_get_header_data(avi);
        CHECK_MIDDLEWARE_RET(ret_sonix);
    }
    else if (HAL_FLAG_HAS(avi->mode, FA_WRITE))
    {
        ret_sonix = _avi_set_header_data(avi);
        CHECK_MIDDLEWARE_RET(ret_sonix);
    }

    //    ret_sonix = f_lseek(avi->fp, avi->head_size);
    //    MFILE_DEBUG_ERROR(ret_sonix != HAL_OK, "Seek fail after get information header");
    //    CHECK_MIDDLEWARE_RET(ret_sonix);

    return HAL_OK;
}

static int32_t _avi_close(AVI_FILE_RES_T *avi)
{
    int32_t  ret_sonix = HAL_OK;
    uint8_t *idx1_fifo = NULL;

    if (HAL_FLAG_HAS(avi->mode, FA_WRITE))
    {
        idx1_header_info.BlockID = 0x31786469; //'idx1'
        movi_header_info.BlockSize += 4;       // + 'movi'

        /* update movi size */
        ret_sonix = _file_seek_and_write(avi->info.head_size, sizeof(LIST_HEADER), &movi_header_info, avi->fp);
        MFILE_DEBUG_ERROR(ret_sonix != HAL_OK, "Set avi movi header fail");
        CHECK_MIDDLEWARE_RET(ret_sonix);

        /* write idx1 header */
        ret_sonix = _file_seek_and_write(data_offset, sizeof(IDX1_HEADER), &idx1_header_info, avi->fp);
        MFILE_DEBUG_ERROR(ret_sonix != HAL_OK, "");
        CHECK_MIDDLEWARE_RET(ret_sonix);
        data_offset += sizeof(IDX1_HEADER);

        /* write idx1 */
        uint32_t idx1_info_size   = idx1_header_info.BlockSize;
        uint32_t idx1_info_offset = 0;
        uint32_t idx1_fifo_size   = 1024 * 1;

        MEMORY_ALLOC(idx1_fifo, idx1_fifo_size, GFP_SRAM);

        while (idx1_info_size != 0)
        {
            if (idx1_info_size < idx1_fifo_size)
            {
                idx1_fifo_size = idx1_info_size;
            }

            ret_sonix = _file_seek_and_read(idx1_info_offset, &idx1_fifo_size, idx1_fifo, &idx1_file);
            MFILE_DEBUG_ERROR(ret_sonix != HAL_OK, "");
            if (ret_sonix != HAL_OK)
            {
                break;
            }

            ret_sonix = _file_seek_and_write(data_offset + idx1_info_offset, idx1_fifo_size, idx1_fifo, avi->fp);
            MFILE_DEBUG_ERROR(ret_sonix != HAL_OK, "");
            if (ret_sonix != HAL_OK)
            {
                break;
            }

            idx1_info_offset += idx1_fifo_size;
            idx1_info_size -= idx1_fifo_size;
        }
        MEMORY_FREE(idx1_fifo);

        _file_close(&idx1_file, path_name);

        return ret_sonix;
    }

    if (avi->head_data != NULL)
    {
        MEMORY_FREE(avi->head_data);
    }

    return ret_sonix;
}

static int32_t _avi_read(AVI_FILE_RES_T *avi, FILE_FRAME_T *frame, uint32_t is_blocking)
{
    int32_t   ret_sonix      = HAL_OK;
    uint32_t  data_offset    = 0;
    uint32_t  data_size      = 0;
    IDX1_INFO idx1_info      = {0};
    uint32_t  idx1_info_size = sizeof(IDX1_INFO);

    /* Gets the current frame type to be read, if the frame type is FrameAll, the current frame type is not qualified */
    AVI_FRAME_INFO_T *frame_info = (AVI_FRAME_INFO_T *)(frame->frame_info);
    if (frame_info->frame_type > FrameAudio)
    {
        return HAL_ERROR;
    }

    /* Specify the frame type as audio */
    if (frame_info->frame_type == FrameAudio) // audio
    {
        if (avi->status.is_a_eof != FILE_IS_NOT_EOF)
        {
            return HAL_OK;
        }

        /* Query the offset of the audio frame in idx1 */
        do
        {
            ret_sonix = _data_seek_and_read(avi->current_audio_frame, &idx1_info_size, &idx1_info, avi->fp, FileOperations);
            MFILE_DEBUG_ERROR(ret_sonix != HAL_OK, "");
            CHECK_MIDDLEWARE_RET(ret_sonix);

            avi->current_audio_frame += idx1_info_size;

            if (idx1_info.ChunkId == *avi->status.AudioFLAG)
            {
                avi->current_audio_frame_index++;
                break;
            }
        } while (idx1_info.ChunkId != *avi->status.AudioFLAG);

        /* Query the offset of the audio frame data in the entire file */
        data_offset            = avi->idx1_dwBlockOffset_ex + idx1_info.Offset + 8;
        frame_info->frame_flag = idx1_info.Flags;
        data_size              = idx1_info.Size;

        /* Reading audio data */
        ret_sonix = _data_seek_and_read(data_offset, &data_size, frame->buffer, avi->fp, FileOperations);
        MFILE_DEBUG_ERROR(ret_sonix != HAL_OK, "");
        CHECK_MIDDLEWARE_RET(ret_sonix);

        frame->length = data_size;

        /* Update the end status of the audio file */
        if (avi->current_audio_frame_index == avi->audio_total_frame)
        {
            avi->status.is_a_eof = FILE_IS_EOF;
        }
    }
    /* Specify the frame type as video */
    else if (frame_info->frame_type == FrameVideo) // video
    {
        if (avi->status.is_v_eof != FILE_IS_NOT_EOF)
        {
            return HAL_OK;
        }

        /* Query the offset of the video frame in idx1 */
        do
        {
            ret_sonix = _data_seek_and_read(avi->current_video_frame, &idx1_info_size, &idx1_info, avi->fp, FileOperations);
            MFILE_DEBUG_ERROR(ret_sonix != HAL_OK, "");
            CHECK_MIDDLEWARE_RET(ret_sonix);

            avi->current_video_frame += idx1_info_size;

            if (idx1_info.ChunkId == *avi->status.VideoFLAG)
            {
                avi->current_video_frame_index++;
                break;
            }
        } while (idx1_info.ChunkId != *avi->status.VideoFLAG);

        /* Query the offset of the video frame data in the entire file */
        data_offset            = avi->idx1_dwBlockOffset_ex + idx1_info.Offset + 8;
        frame_info->frame_flag = idx1_info.Flags;
        data_size              = idx1_info.Size;

        /* Reading video data */
        ret_sonix = _data_seek_and_read(data_offset, &data_size, frame->buffer, avi->fp, FileOperations);
        MFILE_DEBUG_ERROR(ret_sonix != HAL_OK, "");
        CHECK_MIDDLEWARE_RET(ret_sonix);

        frame->length = data_size;

        /* Update the end status of the video file */
        if (avi->current_video_frame_index == avi->video_total_frame)
        {
            avi->status.is_v_eof = FILE_IS_EOF;
        }
    }
    /* Handle frames with an unspecified frame type */
    else if (frame_info->frame_type == FrameAll) // No frame type is specified
    {
        uint32_t frame_type      = 0;
        uint32_t frame_type_size = sizeof(uint32_t);

        if (avi->status.is_eof != FILE_IS_NOT_EOF)
        {
            return HAL_OK;
        }

        ret_sonix = _data_seek_and_read(avi->current_frame, &idx1_info_size, &idx1_info, avi->fp, FileOperations);
        MFILE_DEBUG_ERROR(ret_sonix != HAL_OK, "");
        CHECK_MIDDLEWARE_RET(ret_sonix);

        avi->current_frame += idx1_info_size;
        avi->current_frame_index++;

        /* Query the offset of the video frame data in the entire file */
        data_offset            = avi->idx1_dwBlockOffset_ex + idx1_info.Offset;
        frame_info->frame_flag = idx1_info.Flags;
        data_size              = idx1_info.Size;

        /* Reading data frame type*/
        ret_sonix = _data_seek_and_read(data_offset, &frame_type_size, &frame_type, avi->fp, FileOperations);
        MFILE_DEBUG_ERROR(ret_sonix != HAL_OK, "");
        CHECK_MIDDLEWARE_RET(ret_sonix);

        /* Determining the Frame Type */
        if (memcmp(&frame_type, avi->status.VideoFLAG, 4) == 0)
        {
            frame_info->frame_type = FrameVideo;
        }
        else
        {
            frame_info->frame_type = FrameAudio;
        }

        /* Reading data */
        ret_sonix = _data_seek_and_read(data_offset + 8, &data_size, frame->buffer, avi->fp, FileOperations);
        MFILE_DEBUG_ERROR(ret_sonix != HAL_OK, "");
        CHECK_MIDDLEWARE_RET(ret_sonix);

        frame->length = data_size;

        /* Update the end status of the file */
        if (avi->current_frame_index == avi->total_frame)
        {
            avi->status.is_eof = FILE_IS_EOF;
        }
    }

    return HAL_OK;
}

static int32_t _avi_write(AVI_FILE_RES_T *avi, const FILE_FRAME_T *frame, uint32_t is_blocking)
{
    int32_t          ret_sonix     = 0;
    AVI_FRAME_INFO_T frame_info    = *(AVI_FRAME_INFO_T *)(frame->frame_info);
    IDX1_INFO        idx1_data     = {0};
    uint8_t          zero_vale[]   = {0};
    uint16_t         pad_data_size = 0;

    /* If the file has reached the end, HAL_OK is returned */
    if (avi->status.is_eof != FILE_IS_NOT_EOF)
    {
        return HAL_OK;
    }

    /* movi */
    FRAME_HEADER data_header = {0};

    /* Set the frame ID based on the frame type */
    if (frame_info.frame_type == FrameAudio)
    {
        data_header.FrameID = *(uint32_t *)(avi->status.AudioFLAG);
    }
    else if (frame_info.frame_type == FrameVideo)
    {
        data_header.FrameID = *(uint32_t *)(avi->status.VideoFLAG);
    }
    data_header.FrameSize = frame->length;

    /* idx1 */
    idx1_data.ChunkId = data_header.FrameID;
    idx1_data.Flags   = frame_info.frame_flag;
    idx1_data.Offset  = data_offset - avi->info.head_size - 8;
    idx1_data.Size    = frame->length;

    ret_sonix = _file_seek_and_write(idx1_header_info.BlockSize, sizeof(IDX1_INFO), &idx1_data, &idx1_file);
    MFILE_DEBUG_ERROR(ret_sonix != HAL_OK, "");
    CHECK_MIDDLEWARE_RET(ret_sonix);

    idx1_header_info.BlockSize += sizeof(IDX1_INFO);
    avi->current_frame_index++;

    /* write frame header info */
    ret_sonix = _file_seek_and_write(data_offset, sizeof(FRAME_HEADER), &data_header, avi->fp);
    MFILE_DEBUG_ERROR(ret_sonix != HAL_OK, "");
    CHECK_MIDDLEWARE_RET(ret_sonix);
    data_offset += sizeof(FRAME_HEADER);

    /* write frame data */
    ret_sonix = _file_seek_and_write(data_offset, frame->length, frame->buffer, avi->fp);
    MFILE_DEBUG_ERROR(ret_sonix != HAL_OK, "");
    CHECK_MIDDLEWARE_RET(ret_sonix);
    data_offset += frame->length;

    /* If the frame size is odd, 0 is padded */
    if (frame->length % 2)
    {
        pad_data_size = 1;
        ret_sonix     = _file_seek_and_write(data_offset, 1, zero_vale, avi->fp);
        MFILE_DEBUG_ERROR(ret_sonix != HAL_OK, "");
        CHECK_MIDDLEWARE_RET(ret_sonix);
        data_offset += pad_data_size;
    }

    f_sync(avi->fp);

    /* update movi header info */
    movi_header_info.BlockSize += frame->length + sizeof(FRAME_HEADER) + pad_data_size;

    return HAL_OK;
}

static int32_t _avi_seek(AVI_FILE_RES_T *avi, const AVI_SEEK_T *seek)
{
    int32_t   ret_sonix      = HAL_OK;
    IDX1_INFO idx1_info      = {0};
    uint32_t  idx1_info_size = sizeof(IDX1_INFO);

    if (HAL_FLAG_HAS(avi->mode, FA_READ))
    {
        if (seek->frame_type == FrameAudio) // audio
        {
            if (seek->frame_index >= avi->audio_total_frame)
                return HAL_ERROR;
        }
        else if (seek->frame_type == FrameVideo) // video
        {
            if (seek->frame_index >= avi->video_total_frame)
                return HAL_ERROR;
        }
        else if (seek->frame_type == FrameAll) // video
        {
            if (seek->frame_index >= avi->total_frame)
                return HAL_ERROR;
        }

        if (seek->frame_type == FrameAudio) // audio
        {
            /* If the number of seek is greater than the previous frame cnt, we simply continue searching for the keyword */
            if (seek->frame_index > avi->current_audio_frame_index)
            {
                uint32_t index        = 0;
                uint32_t frame_offset = 0;

                while (avi->current_audio_frame_index + index != seek->frame_index)
                {
                    ret_sonix = _data_seek_and_read(avi->current_audio_frame + frame_offset, &idx1_info_size, &idx1_info, avi->fp, FileOperations);
                    MFILE_DEBUG_ERROR(ret_sonix != HAL_OK, "");
                    CHECK_MIDDLEWARE_RET(ret_sonix);

                    frame_offset += idx1_info_size;

                    if (idx1_info.ChunkId == *avi->status.AudioFLAG)
                    {
                        index++;
                    }
                }

                avi->current_audio_frame_index = seek->frame_index;
                avi->current_audio_frame += frame_offset;
            }
            else /* If the number of seek is smaller than the previous frame cnt, the keyword is searched from the beginning  */
            {
                uint32_t index        = 0;
                uint32_t frame_offset = 0;

                while (index == seek->frame_index)
                {
                    ret_sonix = _data_seek_and_read(avi->idx1_info_offset + frame_offset, &idx1_info_size, &idx1_info, avi->fp, FileOperations);
                    MFILE_DEBUG_ERROR(ret_sonix != HAL_OK, "");
                    CHECK_MIDDLEWARE_RET(ret_sonix);

                    frame_offset += idx1_info_size;

                    if (idx1_info.ChunkId == *avi->status.AudioFLAG)
                    {
                        index++;
                    }
                }

                avi->current_audio_frame_index = seek->frame_index;
                avi->current_audio_frame       = avi->idx1_info_offset + frame_offset;
            }
            avi->status.is_a_eof = FILE_IS_NOT_EOF;
        }
        else if (seek->frame_type == FrameVideo) // video
        {
            /* If the number of seek is greater than the previous frame cnt, we simply continue searching for the keyword */
            if (seek->frame_index > avi->current_video_frame_index)
            {
                uint32_t index        = 0;
                uint32_t frame_offset = 0;

                while (avi->current_video_frame_index + index == seek->frame_index)
                {
                    ret_sonix = _data_seek_and_read(avi->current_video_frame + frame_offset, &idx1_info_size, &idx1_info, avi->fp, FileOperations);
                    MFILE_DEBUG_ERROR(ret_sonix != HAL_OK, "");
                    CHECK_MIDDLEWARE_RET(ret_sonix);

                    frame_offset += idx1_info_size;

                    if (idx1_info.ChunkId == *avi->status.VideoFLAG)
                    {
                        index++;
                    }
                }

                avi->current_video_frame_index = seek->frame_index;
                avi->current_video_frame += frame_offset;
            }
            else /* If the number of seek is smaller than the previous frame cnt, the keyword is searched from the beginning  */
            {
                uint32_t index        = 0;
                uint32_t frame_offset = 0;

                while (index == seek->frame_index)
                {
                    ret_sonix = _data_seek_and_read(avi->idx1_info_offset + frame_offset, &idx1_info_size, &idx1_info, avi->fp, FileOperations);
                    MFILE_DEBUG_ERROR(ret_sonix != HAL_OK, "");
                    CHECK_MIDDLEWARE_RET(ret_sonix);

                    frame_offset += idx1_info_size;

                    if (idx1_info.ChunkId == *avi->status.VideoFLAG)
                    {
                        index++;
                    }
                }

                avi->current_video_frame_index = seek->frame_index;
                avi->current_video_frame       = avi->idx1_info_offset + frame_offset;
            }
            avi->status.is_v_eof = FILE_IS_NOT_EOF;
        }
        else if (seek->frame_type == FrameAll) // No frame type is specified
        {
            avi->current_frame_index = seek->frame_index;
            avi->current_frame       = avi->idx1_info_offset + idx1_info_size * seek->frame_index;

            avi->status.is_eof = FILE_IS_NOT_EOF;
        }
    }

    return HAL_OK;
}

static int32_t _avi_wait(AVI_FILE_RES_T *wav)
{
    return HAL_OK;
}

static int32_t _avi_get_status(AVI_FILE_RES_T *avi, AVI_STATUS_T *status)
{
    memcpy(status, &avi->status, sizeof(AVI_STATUS_T));

    return HAL_OK;
}

static int32_t AVI_Open(const void *avi)
{
    CHECK_POINTER_NULL(avi);
    CHECK_POINTER_NULL(((FIL_RES_T *)avi)->res);
    return _avi_open(((FIL_RES_T *)avi)->res);
}

static int32_t AVI_Close(const void *avi)
{
    CHECK_POINTER_NULL(avi);
    if (((FIL_RES_T *)avi)->res == NULL)
    {
        // close return ok to continue other close flow
        return HAL_OK;
    }
    return _avi_close(((FIL_RES_T *)avi)->res);
}

static int32_t AVI_Read(const void *avi, void *frame, uint8_t is_blocking)
{
    CHECK_POINTER_NULL(avi);
    CHECK_POINTER_NULL(((FIL_RES_T *)avi)->res);
    CHECK_POINTER_NULL(frame);
    return _avi_read(((FIL_RES_T *)avi)->res, frame, is_blocking);
}

static int32_t AVI_Write(const void *avi, const void *frame, uint8_t is_blocking)
{
    CHECK_POINTER_NULL(avi);
    CHECK_POINTER_NULL(((FIL_RES_T *)avi)->res);
    CHECK_POINTER_NULL(frame);
    return _avi_write(((FIL_RES_T *)avi)->res, frame, is_blocking);
}

static int32_t AVI_Seek(const void *avi, const void *seek)
{
    CHECK_POINTER_NULL(avi);
    CHECK_POINTER_NULL(((FIL_RES_T *)avi)->res);
    CHECK_POINTER_NULL(seek);
    return _avi_seek(((FIL_RES_T *)avi)->res, seek);
}

static int32_t AVI_Wait(const void *avi)
{
    CHECK_POINTER_NULL(avi);
    CHECK_POINTER_NULL(((FIL_RES_T *)avi)->res);
    return _avi_wait(((FIL_RES_T *)avi)->res);
}

static int32_t AVI_GetStatus(const void *avi, void *status)
{
    CHECK_POINTER_NULL(avi);
    CHECK_POINTER_NULL(((FIL_RES_T *)avi)->res);
    CHECK_POINTER_NULL(status);
    return _avi_get_status(((FIL_RES_T *)avi)->res, status);
}

TYPE_WRAPPER AVI_Wrapper = {
    AVI_Open,
    AVI_Close,
    AVI_Read,
    AVI_Write,
    AVI_Wait,
    AVI_Seek,
    AVI_GetStatus,
};
