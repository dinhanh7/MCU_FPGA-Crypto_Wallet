/***********************
 * SN34F78X SDK V2.1.4 *
 ***********************/

#include "wav_wrapper.h"

#define WAV_PARSE_INIT       (0x1 << 0)
#define WAV_PARSE_LACK_DATA  (0x1 << 1)
#define WAV_PARSE_READ_FRAME (0x1 << 2)
#define WAV_PARSE_FIND_FIRST (0x1 << 3)

#define WAV_USE_FATFS  (1)
#define WAV_USE_STREAM (0)

#if WAV_USE_STREAM
#include "Storage_wrapper.h"
#endif

////////////////////////////////////////////////////////////////////////////////////////
static int32_t _wav_f_tell(WAV_FILE_RES_T *wav)
{
    if (wav->src == FILE_SRC_FROM_FATFS)
    {
#if WAV_USE_FATFS
        return f_tell(wav->fp) - wav->parser.buff_size + wav->parser.buff_pos;
#else
        return 0;
#endif
    }
    else
    {
        return wav->file_pos - wav->parser.buff_size + wav->parser.buff_pos;
    }
}

static int32_t _wav_f_size(WAV_FILE_RES_T *wav)
{
    if (wav->src == FILE_SRC_FROM_FATFS)
    {
#if WAV_USE_FATFS
        return f_size(wav->fp);
#else
        return 0;
#endif
    }
    else
    {
        return wav->file_siz;
    }
}

#if WAV_USE_FATFS || WAV_USE_STREAM
static int32_t _wav_fill_buffer(WAV_FILE_RES_T *wav, uint8_t *data_addr, uint32_t data_size)
{
    if ((wav->file_pos == 0) || ((wav->wav_status & WAV_PARSE_LACK_DATA) == WAV_PARSE_LACK_DATA))
    {
        wav->parser.data_addr = data_addr;
        wav->parser.data_size = data_size;
        wav->file_pos += data_size;
        if (wav->parser.buff_addr == NULL)
        {
            wav->parser.buff_cnt++;
            wav->parser.buff_pos   = 0;
            wav->parser.buff_size  = wav->parser.data_size;
            wav->parser.buff_total = wav->parser.data_size;
            MEMORY_ALLOC(wav->parser.buff_addr, wav->parser.buff_size, GFP_SRAM);
            memcpy(wav->parser.buff_addr, wav->parser.data_addr, wav->parser.buff_size);
        }
    }
    return HAL_OK;
}
#endif

static int32_t _wav_fill_parser(WAV_FILE_RES_T *wav)
{
    int32_t ret_sonix = HAL_OK;
    if (wav->src == FILE_SRC_FROM_FATFS)
    {
#if WAV_USE_FATFS
        if (wav->parser.data_addr == NULL)
        {
            wav->parser.data_size = FILE_PARSE_BUFF_SIZE;
            MEMORY_ALLOC(wav->parser.data_addr, wav->parser.data_size, GFP_SRAM);
        }
        ret_sonix = _file_seek_and_read(FILE_NOT_SEEK, &wav->parser.data_size, wav->parser.data_addr, wav->fp);
        MFILE_DEBUG_ERROR(ret_sonix != HAL_OK, "_file_seek_and_read %d", ret_sonix);
        CHECK_MIDDLEWARE_RET(ret_sonix);
        if (wav->parser.data_size == 0)
        {
            wav->is_eof = FILE_IS_EOF;
        }
        ret_sonix = _wav_fill_buffer(wav, wav->parser.data_addr, wav->parser.data_size);
        MFILE_DEBUG_ERROR(ret_sonix != HAL_OK, "_wav_fill_buffer %d", ret_sonix);
        CHECK_MIDDLEWARE_RET(ret_sonix);
#endif
    }
    else
    {
        return WAV_MSG_LACK_DATA;
    }
    return ret_sonix;
}

static int32_t _wav_get_nbyte(uint8_t *need_data, uint32_t need_size, WAV_FILE_RES_T *wav)
{
    int32_t ret_sonix = HAL_OK;
    if (wav->parser.buff_addr == NULL)
    {
        return HAL_ERROR;
    }
    if ((wav->parser.buff_pos + need_size) > wav->parser.buff_size)
    {
        return HAL_ERROR;
    }

    if (need_data)
    {
        memcpy(need_data, wav->parser.buff_addr + wav->parser.buff_pos, need_size);
    }
    wav->parser.buff_pos += need_size;
    int64_t size = _wav_f_size(wav) - _wav_f_tell(wav);
    if (size <= 0)
    {
        wav->is_eof = FILE_IS_EOF;
    }
    return ret_sonix;
}

static int32_t _wav_leak_data(WAV_FILE_RES_T *wav, uint32_t need_size)
{
    int32_t ret_sonix = HAL_OK;
    if ((wav->wav_status & WAV_PARSE_LACK_DATA) == WAV_PARSE_LACK_DATA)
    {
        wav->parser.buff_cnt++;
        wav->parser.buff_pos = 0;

        uint32_t buff_size = wav->parser.tail_used + wav->parser.data_size;
        if (buff_size > wav->parser.buff_total)
        {
            MEMORY_FREE(wav->parser.buff_addr);
            MEMORY_ALLOC(wav->parser.buff_addr, buff_size, GFP_KERNEL);
            wav->parser.buff_total = buff_size;
        }
        memcpy(wav->parser.buff_addr, wav->parser.tail_addr, wav->parser.tail_used);
        memcpy(wav->parser.buff_addr + wav->parser.tail_used, wav->parser.data_addr, wav->parser.data_size);

        wav->parser.buff_size = buff_size;
        wav->wav_status &= ~WAV_PARSE_LACK_DATA;
        ret_sonix = HAL_OK;
    }
    if (wav->parser.buff_pos + need_size > wav->parser.buff_size)
    {
        uint32_t tail_size = wav->parser.buff_size - wav->parser.buff_pos;
        if (tail_size != 0)
        {
            if (tail_size > wav->parser.tail_size)
            {
                MEMORY_FREE(wav->parser.tail_addr);
                MEMORY_ALLOC(wav->parser.tail_addr, tail_size, GFP_KERNEL);
                wav->parser.tail_size = tail_size;
            }
            memcpy(wav->parser.tail_addr, wav->parser.buff_addr + wav->parser.buff_pos, tail_size);
        }

        wav->parser.tail_used = tail_size;
        wav->wav_status |= WAV_PARSE_LACK_DATA;
        ret_sonix = WAV_MSG_LACK_DATA;
    }
    return ret_sonix;
}

static int32_t _wav_seek_and_write(uint64_t seek, uint32_t write_size, void *write_buff, WAV_FILE_RES_T *wav)
{
    if (wav->src == FILE_SRC_FROM_FATFS)
    {
#if WAV_USE_FATFS
        return _file_seek_and_write(seek, write_size, write_buff, wav->fp);
#else
        return HAL_ERROR;
#endif
    }
    else
    {
        return HAL_ERROR;
    }
}

//////////////////////////////////////////////////////////////////////////////////////////////
static int32_t _wav_parse_header(WAV_FILE_RES_T *wav)
{
    int32_t ret_sonix = HAL_ERROR;
    ret_sonix         = _wav_leak_data(wav, 0x100);
    CHECK_MIDDLEWARE_RET(ret_sonix);

    ret_sonix = _wav_get_nbyte((uint8_t *)&wav->header.riff, sizeof(WAV_FORMAT_RIFF_T), wav);
    CHECK_MIDDLEWARE_RET(ret_sonix);
    if (wav->header.riff.ChunkRIFFTag != CHUNK_ID_RIFF)
    {
        return HAL_ERROR;
    }
    if (wav->header.riff.Format != CHUNK_ID_WAVE)
    {
        return HAL_ERROR;
    }

    ret_sonix = _wav_get_nbyte((uint8_t *)&wav->header.fmt.ChunkFMTTag, sizeof(uint32_t), wav);
    CHECK_MIDDLEWARE_RET(ret_sonix);
    if (wav->header.fmt.ChunkFMTTag != CHUNK_ID_FMT_)
    {
        return HAL_ERROR;
    }

    uint32_t format_used = 0;
    ret_sonix            = _wav_get_nbyte((uint8_t *)&wav->header.fmt.ChunkFMTLen, sizeof(uint32_t), wav);
    CHECK_MIDDLEWARE_RET(ret_sonix);
    format_used += sizeof(WAV_FORMAT_PCM_T);
    ret_sonix = _wav_get_nbyte((uint8_t *)&wav->header.fmt.pcm, sizeof(WAV_FORMAT_PCM_T), wav);
    CHECK_MIDDLEWARE_RET(ret_sonix);
    if (wav->header.fmt.pcm.AudioFormat == WAV_IMA)
    {
        // extra format para
        format_used += sizeof(uint16_t);
        ret_sonix = _wav_get_nbyte((uint8_t *)&wav->header.fmt.ExtraFmtSize, sizeof(uint16_t), wav);
        CHECK_MIDDLEWARE_RET(ret_sonix);
        format_used += sizeof(uint16_t);
        ret_sonix = _wav_get_nbyte((uint8_t *)&wav->header.fmt.SamplesPerBlock, sizeof(uint16_t), wav);
        CHECK_MIDDLEWARE_RET(ret_sonix);
    }
    if (wav->header.fmt.ChunkFMTLen < format_used)
    {
        return HAL_ERROR;
    }
    // skip dummy
    ret_sonix = _wav_get_nbyte(NULL, wav->header.fmt.ChunkFMTLen - format_used, wav);
    CHECK_MIDDLEWARE_RET(ret_sonix);
    if (wav->header.fmt.pcm.NumOfChannels == 0 || wav->header.fmt.pcm.BlockAlign == 0 || wav->header.fmt.pcm.BitsPerSample == 0)
    {
        return HAL_ERROR;
    }

    ret_sonix = _wav_get_nbyte((uint8_t *)&wav->header.data.ChunkDataTag, sizeof(uint32_t), wav);
    CHECK_MIDDLEWARE_RET(ret_sonix);
    if (wav->header.data.ChunkDataTag == CHUNK_ID_LIST) // extra info of wav
    {
        uint32_t tmp_len = 0;
        ret_sonix        = _wav_get_nbyte((uint8_t *)&tmp_len, sizeof(uint32_t), wav);
        CHECK_MIDDLEWARE_RET(ret_sonix);
        ret_sonix = _wav_get_nbyte(NULL, tmp_len, wav);
        CHECK_MIDDLEWARE_RET(ret_sonix);
        ret_sonix = _wav_get_nbyte((uint8_t *)&wav->header.data.ChunkDataTag, sizeof(uint32_t), wav);
        CHECK_MIDDLEWARE_RET(ret_sonix);
    }
    if (wav->header.data.ChunkDataTag == CHUNK_ID_FACT) // ima fact info
    {
        wav->header.fact.ChunkFactTag = wav->header.data.ChunkDataTag;
        ret_sonix                     = _wav_get_nbyte((uint8_t *)&wav->header.fact.ChunkFactLen, sizeof(uint32_t), wav);
        CHECK_MIDDLEWARE_RET(ret_sonix);
        ret_sonix = _wav_get_nbyte((uint8_t *)&wav->header.fact.Fact, wav->header.fact.ChunkFactLen, wav);
        CHECK_MIDDLEWARE_RET(ret_sonix);
        ret_sonix = _wav_get_nbyte((uint8_t *)&wav->header.data.ChunkDataTag, sizeof(uint32_t), wav);
        CHECK_MIDDLEWARE_RET(ret_sonix);
    }
    if (wav->header.data.ChunkDataTag != CHUNK_ID_DATA)
    {
        return HAL_ERROR;
    }

    ret_sonix = _wav_get_nbyte((uint8_t *)&wav->header.data.ChunkDataLen, sizeof(uint32_t), wav);
    CHECK_MIDDLEWARE_RET(ret_sonix);
    wav->header_len = wav->parser.buff_pos + (wav->parser.buff_cnt - 1) * wav->parser.buff_size;

    wav->sample_cnt = wav->header.data.ChunkDataLen / wav->header.fmt.pcm.BlockAlign;
    if (wav->header.data.ChunkDataLen % wav->header.fmt.pcm.BlockAlign)
    {
        wav->sample_cnt++;
    }
    return HAL_OK;
}

static int32_t _wav_header_set(WAV_FILE_RES_T *wav)
{
    WAV_INFO_T *info                  = &wav->info;
    wav->header.riff.ChunkRIFFTag     = CHUNK_ID_RIFF;
    wav->header.riff.ChunkRIFFLen     = 0; // need write back
    wav->header.riff.Format           = CHUNK_ID_WAVE;
    wav->header.fmt.ChunkFMTTag       = CHUNK_ID_FMT_;
    wav->header.fmt.ChunkFMTLen       = sizeof(WAV_FORMAT_FMT_T) - WAV_CHUNK_TAG_SIZE;
    wav->header.fmt.pcm.AudioFormat   = info->audio_format;
    wav->header.fmt.pcm.NumOfChannels = info->num_of_channels;
    wav->header.fmt.pcm.SampleRate    = info->sample_rate;
    wav->header.fmt.pcm.BitsPerSample = info->bits_per_sample;
    wav->header.fmt.ExtraFmtSize      = 0;
    if (info->audio_format == WAV_IMA)
    {
        wav->header.fmt.ExtraFmtSize    = 2;
        wav->header.fmt.pcm.BlockAlign  = info->block_align;
        wav->header.fmt.SamplesPerBlock = 4 * (info->block_align / 2 - 2) + 1;
        wav->header.fmt.pcm.ByteRate    = (uint32_t)((double)info->sample_rate * info->block_align / wav->header.fmt.SamplesPerBlock + 0.5);
        wav->header.fact.ChunkFactTag   = CHUNK_ID_FACT;
        wav->header.fact.ChunkFactLen   = sizeof(WAV_FORMAT_FACT_T) - WAV_CHUNK_TAG_SIZE;
        wav->header.fact.Fact           = 0;
    }
    else
    {
        wav->header.fmt.pcm.BlockAlign = info->num_of_channels * info->bits_per_sample / 8;
        wav->header.fmt.pcm.ByteRate   = info->sample_rate * wav->header.fmt.pcm.BlockAlign;
    }
    wav->header.data.ChunkDataTag = CHUNK_ID_DATA;
    wav->header.data.ChunkDataLen = 0; // need write back
    return HAL_OK;
}

static int32_t _wav_free_parser(WAV_FILE_RES_T *wav)
{
    if (wav->src == FILE_SRC_FROM_FATFS)
    {
#if WAV_USE_FATFS
        MEMORY_FREE(wav->parser.data_addr);
#endif
    }
    MEMORY_FREE(wav->parser.buff_addr);
    MEMORY_FREE(wav->parser.tail_addr);
    memset(&wav->parser, 0, sizeof(wav->parser));

    return HAL_OK;
}

//////////////////////////////////////////////////////////////////////////////////////////////
static int32_t _wav_open(WAV_FILE_RES_T *wav)
{
    int32_t ret_sonix = HAL_ERROR;
    if ((wav->mode & FA_READ) == FA_READ)
    {
        ret_sonix = _wav_fill_parser(wav);
        while (wav->is_eof == FILE_IS_NOT_EOF)
        {
            ret_sonix = _wav_parse_header(wav);
            if (ret_sonix == WAV_MSG_LACK_DATA)
            {
                ret_sonix = _wav_fill_parser(wav);
                CHECK_MIDDLEWARE_RET(ret_sonix);
                continue;
            }
            CHECK_MIDDLEWARE_RET(ret_sonix);
            break;
        }
    }
    if ((wav->mode & FA_WRITE) == FA_WRITE)
    {
        ret_sonix = _wav_header_set(wav);
        CHECK_MIDDLEWARE_RET(ret_sonix);
        ret_sonix = _wav_seek_and_write(0, sizeof(WAV_FORMAT_RIFF_T), &wav->header.riff, wav);
        CHECK_MIDDLEWARE_RET(ret_sonix);
        ret_sonix = _wav_seek_and_write(FILE_NOT_SEEK, sizeof(WAV_FORMAT_FMT_T), &wav->header.fmt, wav);
        CHECK_MIDDLEWARE_RET(ret_sonix);
        if (wav->header.fmt.pcm.AudioFormat == WAV_IMA)
        {
            ret_sonix = _wav_seek_and_write(FILE_NOT_SEEK, sizeof(WAV_FORMAT_FACT_T), &wav->header.fact, wav);
            CHECK_MIDDLEWARE_RET(ret_sonix);
        }
        ret_sonix = _wav_seek_and_write(FILE_NOT_SEEK, sizeof(WAV_FORMAT_DATA_T), &wav->header.data, wav);
        CHECK_MIDDLEWARE_RET(ret_sonix);
    }
    return HAL_OK;
}

static int32_t _wav_close(WAV_FILE_RES_T *wav)
{
    _wav_free_parser(wav);
    if ((wav->mode & FA_WRITE) == FA_WRITE)
    {
        int32_t ret_sonix             = HAL_ERROR;
        wav->header.riff.ChunkRIFFLen = _wav_f_size(wav) - WAV_CHUNK_TAG_SIZE;
        ret_sonix                     = _wav_seek_and_write(0, sizeof(WAV_FORMAT_RIFF_T), &wav->header.riff, wav);
        CHECK_MIDDLEWARE_RET(ret_sonix);
        ret_sonix = _wav_seek_and_write(FILE_NOT_SEEK, sizeof(WAV_FORMAT_FMT_T), &wav->header.fmt, wav);
        CHECK_MIDDLEWARE_RET(ret_sonix);
        if (wav->header.fmt.pcm.AudioFormat == WAV_IMA)
        {
            wav->header.fact.Fact = wav->sample_cnt * wav->header.fmt.SamplesPerBlock;
            ret_sonix             = _wav_seek_and_write(FILE_NOT_SEEK, sizeof(WAV_FORMAT_FACT_T), &wav->header.fact, wav);
            CHECK_MIDDLEWARE_RET(ret_sonix);
        }
        ret_sonix = _wav_seek_and_write(FILE_NOT_SEEK, sizeof(WAV_FORMAT_DATA_T), &wav->header.data, wav);
        CHECK_MIDDLEWARE_RET(ret_sonix);
    }
    return HAL_OK;
}

static int32_t _wav_read(WAV_FILE_RES_T *wav, FILE_FRAME_T *frame, uint32_t is_blocking)
{
    if (frame->length < wav->header.fmt.pcm.BlockAlign)
    {
        frame->length = wav->header.fmt.pcm.BlockAlign;
        return WAV_MSG_LACK_BUFF;
    }
    if (wav->sample_idx >= wav->sample_cnt)
    {
        return HAL_ERROR;
    }

    uint32_t sample_cnt = frame->length / wav->header.fmt.pcm.BlockAlign;
    if (wav->sample_idx + sample_cnt > wav->sample_cnt)
    {
        sample_cnt = wav->sample_cnt - wav->sample_idx;
    }
    uint32_t size = sample_cnt * wav->header.fmt.pcm.BlockAlign;

    int32_t ret_sonix = HAL_OK;
    while (wav->is_eof == FILE_IS_NOT_EOF)
    {
        ret_sonix = _wav_leak_data(wav, size);
        if (ret_sonix == WAV_MSG_LACK_DATA)
        {
            ret_sonix = _wav_fill_parser(wav);
            MFILE_DEBUG_ERROR(ret_sonix != HAL_OK, "_wav_fill_parser %d", ret_sonix);
            CHECK_MIDDLEWARE_RET(ret_sonix);
        }
        else
        {
            MFILE_DEBUG_ERROR(ret_sonix != HAL_OK, "_wav_leak_data %d", ret_sonix);
            CHECK_MIDDLEWARE_RET(ret_sonix);
            ret_sonix = _wav_get_nbyte(frame->buffer, size, wav);
            MFILE_DEBUG_ERROR(ret_sonix != HAL_OK, "_wav_get_nbyte %d", ret_sonix);
            CHECK_MIDDLEWARE_RET(ret_sonix);
            wav->sample_idx += sample_cnt;
            frame->length = size;
            if (wav->sample_idx == wav->sample_cnt)
            {
                wav->is_eof = FILE_IS_EOF;
            }
            break;
        }
    }

    return HAL_OK;
}

static int32_t _wav_write(WAV_FILE_RES_T *wav, const FILE_FRAME_T *frame, uint32_t is_blocking)
{
    int32_t ret_sonix = _wav_seek_and_write(FILE_NOT_SEEK, frame->length, frame->buffer, wav);
    CHECK_MIDDLEWARE_RET(ret_sonix);
    wav->header.data.ChunkDataLen += frame->length;
    wav->sample_idx += frame->length / wav->header.fmt.pcm.BlockAlign;
    wav->sample_cnt = wav->sample_idx;
    return HAL_OK;
}

static int32_t _wav_seek(WAV_FILE_RES_T *wav, const WAV_SEEK_T *seek)
{
#if WAV_USE_FATFS
    if (seek->index >= wav->sample_cnt)
    {
        return HAL_ERROR;
    }

    _wav_free_parser(wav);
    wav->is_eof     = FILE_IS_NOT_EOF;
    wav->sample_idx = seek->index;
    if (wav->src == FILE_SRC_FROM_FATFS)
    {
        uint64_t seek      = wav->header_len + wav->header.fmt.pcm.BlockAlign * wav->sample_idx;
        int32_t  ret_sonix = _file_seek(wav->fp, seek);
        CHECK_MIDDLEWARE_RET(ret_sonix);
    }
    return HAL_OK;
#else
    return HAL_ERROR;
#endif
}

static int32_t _wav_wait(WAV_FILE_RES_T *wav)
{
    return HAL_OK;
}

static int32_t _wav_get_status(WAV_FILE_RES_T *wav, WAV_STATUS_T *status)
{
    status->audio_frame_cnt = wav->sample_cnt;
    status->audio_frame_idx = wav->sample_idx;
    status->num_of_channels = wav->header.fmt.pcm.NumOfChannels;
    status->bits_per_sample = wav->header.fmt.pcm.BitsPerSample;
    status->audio_format    = wav->header.fmt.pcm.AudioFormat;
    status->sample_rate     = wav->header.fmt.pcm.SampleRate;
    status->block_align     = wav->header.fmt.pcm.BlockAlign;
    status->is_eof          = wav->is_eof;

    return HAL_OK;
}

///////////////////////////////////////////////////////////////////////////////////////
static int32_t WAV_Open(const void *wav)
{
    CHECK_POINTER_NULL(wav);
    CHECK_POINTER_NULL(((FIL_RES_T *)wav)->res);
    return _wav_open(((FIL_RES_T *)wav)->res);
};
static int32_t WAV_Close(const void *wav)
{
    CHECK_POINTER_NULL(wav);
    // CHECK_POINTER_NULL(((FIL_RES_T *)wav)->res);
    if (((FIL_RES_T *)wav)->res == NULL)
    { // close return ok to continue other close flow
        return HAL_OK;
    }
    return _wav_close(((FIL_RES_T *)wav)->res);
};
static int32_t WAV_Read(const void *wav, void *frame, uint8_t is_blocking)
{
    CHECK_POINTER_NULL(wav);
    CHECK_POINTER_NULL(((FIL_RES_T *)wav)->res);
    CHECK_POINTER_NULL(frame);
    return _wav_read(((FIL_RES_T *)wav)->res, frame, is_blocking);
};
static int32_t WAV_Write(const void *wav, const void *frame, uint8_t is_blocking)
{
    CHECK_POINTER_NULL(wav);
    CHECK_POINTER_NULL(((FIL_RES_T *)wav)->res);
    CHECK_POINTER_NULL(frame);
    return _wav_write(((FIL_RES_T *)wav)->res, frame, is_blocking);
};
static int32_t WAV_Seek(const void *wav, const void *seek)
{
    CHECK_POINTER_NULL(wav);
    CHECK_POINTER_NULL(((FIL_RES_T *)wav)->res);
    CHECK_POINTER_NULL(seek);
    return _wav_seek(((FIL_RES_T *)wav)->res, seek);
};
static int32_t WAV_Wait(const void *wav)
{
    CHECK_POINTER_NULL(wav);
    CHECK_POINTER_NULL(((FIL_RES_T *)wav)->res);
    return _wav_wait(((FIL_RES_T *)wav)->res);
};
static int32_t WAV_GetStatus(const void *wav, void *status)
{
    CHECK_POINTER_NULL(wav);
    CHECK_POINTER_NULL(((FIL_RES_T *)wav)->res);
    CHECK_POINTER_NULL(status);
    return _wav_get_status(((FIL_RES_T *)wav)->res, status);
};

TYPE_WRAPPER WAV_Wrapper = {
    WAV_Open,
    WAV_Close,
    WAV_Read,
    WAV_Write,
    WAV_Wait,
    WAV_Seek,
    WAV_GetStatus,
};

/////////////////////////////////////////////////////////////////////////////////
static int32_t WAV_DoInit(void *sh)
{
    int32_t ret_sonix = HAL_ERROR;
#if WAV_USE_STREAM
    CHECK_POINTER_NULL(sh);
    WAV_HANDLE_T *buf_fh = sh;

    WAV_FILE_RES_T *wav_res = buf_fh->other_res;
    MEMORY_ALLOC(wav_res, sizeof(WAV_FILE_RES_T), GFP_KERNEL);
    if ((wav_res->wav_status & WAV_PARSE_INIT) == WAV_PARSE_INIT)
    {
        return HAL_OK;
    }

    buf_fh->is_eof    = FILE_IS_NOT_EOF;
    buf_fh->last_buff = 0;
    buf_fh->buff_addr = 0;
    buf_fh->buff_size = 0;
    buf_fh->other_res = wav_res;

    wav_res->mode     = FA_READ;
    wav_res->src      = FILE_SRC_FROM_BUFF;
    wav_res->file_siz = 0x7FFFFFFF;
    wav_res->wav_status |= WAV_PARSE_INIT;
    ret_sonix = HAL_OK;
#endif
    return ret_sonix;
}
static int32_t WAV_DeInit(void *sh)
{
    int32_t ret_sonix = HAL_ERROR;
#if WAV_USE_STREAM
    CHECK_POINTER_NULL(sh);
    WAV_HANDLE_T   *buf_fh  = sh;
    WAV_FILE_RES_T *wav_res = buf_fh->other_res;
    CHECK_POINTER_NULL(wav_res);

    if (wav_res->sfp->init == 1)
    {
        wav_res->sfp->init = 0;
    }
    _wav_close(wav_res);
    buf_fh->is_eof    = FILE_IS_NOT_EOF;
    buf_fh->last_buff = 0;
    buf_fh->buff_addr = 0;
    buf_fh->buff_size = 0;
    MEMORY_FREE(wav_res->sfp);
    MEMORY_FREE(buf_fh->other_res);
    ret_sonix = HAL_OK;
#endif
    return ret_sonix;
}
static int32_t WAV_ReadDev(void *sh, void *info)
{
    int32_t ret_sonix = HAL_ERROR;
#if WAV_USE_STREAM
    CHECK_POINTER_NULL(sh);
    CHECK_POINTER_NULL(info);
    WAV_HANDLE_T   *buf_fh  = sh;
    WAV_FILE_RES_T *wav_res = buf_fh->other_res;
    CHECK_POINTER_NULL(wav_res);
    if (wav_res->sfp->beg_addr == 0 ||
        wav_res->sfp->end_addr == 0 ||
        wav_res->sfp->dev == (uint8_t)DEV_NULL)
    {
        return HAL_ERROR;
    }

    if (wav_res->sfp->init == 0)
    {
        wav_res->sfp->init = 1;
    }

    FILE_FRAME_T *frame_tmp = info;
    uint32_t      size      = wav_res->sfp->end_addr - wav_res->sfp->cur_addr;
    if (size > frame_tmp->length)
    {
        size = frame_tmp->length;
    }
    else
    {
        wav_res->is_eof = FILE_IS_EOF;
    }
    ret_sonix = StgWrapper.ReadByte(wav_res->sfp->dev, frame_tmp->buffer, wav_res->sfp->cur_addr, frame_tmp->length);
    CHECK_MIDDLEWARE_RET(ret_sonix);
    wav_res->sfp->cur_addr += size;
    frame_tmp->length = size;
#endif
    return ret_sonix;
}
static int32_t WAV_GetFrame(void *sh, void *frame)
{
    int32_t ret_sonix = HAL_ERROR;
#if WAV_USE_STREAM
    CHECK_POINTER_NULL(sh);
    CHECK_POINTER_NULL(frame);
    WAV_HANDLE_T   *buf_fh  = sh;
    WAV_FILE_RES_T *wav_res = buf_fh->other_res;
    CHECK_POINTER_NULL(buf_fh);
    CHECK_POINTER_NULL(wav_res);
    if (buf_fh->buff_addr == NULL || buf_fh->buff_size == 0)
    {
        return WAV_MSG_LACK_DATA;
    }

    // check init & fill parser buffer
    if ((wav_res->is_eof == FILE_IS_EOF) || (wav_res->wav_status & WAV_PARSE_INIT) != WAV_PARSE_INIT)
    {
        return HAL_ERROR;
    }

    ret_sonix = _wav_fill_buffer(wav_res, (uint8_t *)buf_fh->buff_addr, buf_fh->buff_size);
    CHECK_MIDDLEWARE_RET(ret_sonix);
    if (buf_fh->last_buff == 1)
    {
        wav_res->file_siz = wav_res->file_pos;
    }

    // parser header
    if ((wav_res->wav_status & WAV_PARSE_FIND_FIRST) != WAV_PARSE_FIND_FIRST)
    {
        ret_sonix = _wav_parse_header(wav_res);
        CHECK_MIDDLEWARE_RET(ret_sonix);
        wav_res->wav_status |= WAV_PARSE_FIND_FIRST;
    }

    // read a valid frame
    WAV_FRAME_T *frame_out = frame;
    FILE_FRAME_T frame_tmp = {0};
    frame_tmp.buffer       = frame_out->cache;
    frame_tmp.length       = frame_out->cache_len;
    ret_sonix              = _wav_read(wav_res, &frame_tmp, FILE_IS_BLOCKING);
    buf_fh->frame_len      = frame_tmp.length;
    CHECK_MIDDLEWARE_RET(ret_sonix);
    frame_out->data     = frame_tmp.buffer;
    frame_out->data_len = frame_tmp.length;
    ret_sonix           = _wav_get_status(wav_res, &frame_out->status);
    CHECK_MIDDLEWARE_RET(ret_sonix);

    buf_fh->is_eof = wav_res->is_eof;
#endif
    return ret_sonix;
}
PARSE_WRAPPER WAV_Stream = {
    WAV_DoInit,
    WAV_DeInit,
    WAV_ReadDev,
    WAV_GetFrame,
};
