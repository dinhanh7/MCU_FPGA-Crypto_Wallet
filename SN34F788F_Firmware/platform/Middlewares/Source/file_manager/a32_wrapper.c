/***********************
 * SN34F78X SDK V2.1.4 *
 ***********************/

#include "a32_wrapper.h"

#define A32_PARSE_INIT       (0x1 << 0)
#define A32_PARSE_LACK_DATA  (0x1 << 1)
#define A32_PARSE_READ_FRAME (0x1 << 2)
#define A32_PARSE_FIND_FIRST (0x1 << 3)

#define A32_USE_FATFS  (1)
#define A32_USE_STREAM (0)

#if A32_USE_STREAM
#include "Storage_wrapper.h"
#endif

////////////////////////////////////////////////////////////////////////////////////////
static int32_t _a32_f_tell(A32_FILE_RES_T *a32)
{
    if (a32->src == FILE_SRC_FROM_FATFS)
    {
#if A32_USE_FATFS
        return f_tell(a32->fp) - a32->parser.buff_size + a32->parser.buff_pos;
#else
        return 0;
#endif
    }
    else
    {
        return a32->file_pos - a32->parser.buff_size + a32->parser.buff_pos;
    }
}

static int32_t _a32_f_size(A32_FILE_RES_T *a32)
{
    if (a32->src == FILE_SRC_FROM_FATFS)
    {
#if A32_USE_FATFS
        return f_size(a32->fp);
#else
        return 0;
#endif
    }
    else
    {
        return a32->file_siz;
    }
}

#if A32_USE_FATFS || A32_USE_STREAM
static int32_t _a32_fill_buffer(A32_FILE_RES_T *a32, uint8_t *data_addr, uint32_t data_size)
{
    if ((a32->file_pos == 0) || ((a32->a32_status & A32_PARSE_LACK_DATA) == A32_PARSE_LACK_DATA))
    {
        a32->parser.data_addr = data_addr;
        a32->parser.data_size = data_size;
        a32->file_pos += data_size;
        if (a32->parser.buff_addr == NULL)
        {
            a32->parser.buff_cnt++;
            a32->parser.buff_pos   = 0;
            a32->parser.buff_size  = a32->parser.data_size;
            a32->parser.buff_total = a32->parser.data_size;
            MEMORY_ALLOC(a32->parser.buff_addr, a32->parser.buff_size, GFP_SRAM);
            memcpy(a32->parser.buff_addr, a32->parser.data_addr, a32->parser.buff_size);
        }
    }
    return HAL_OK;
}
#endif

static int32_t _a32_fill_parser(A32_FILE_RES_T *a32)
{
    int32_t ret_sonix = HAL_OK;
    if (a32->src == FILE_SRC_FROM_FATFS)
    {
#if A32_USE_FATFS
        if (a32->parser.data_addr == NULL)
        {
            a32->parser.data_size = FILE_PARSE_BUFF_SIZE;
            MEMORY_ALLOC(a32->parser.data_addr, a32->parser.data_size, GFP_SRAM);
        }
        ret_sonix = _file_seek_and_read(FILE_NOT_SEEK, &a32->parser.data_size, a32->parser.data_addr, a32->fp);
        CHECK_MIDDLEWARE_RET(ret_sonix);
        if (a32->parser.data_size == 0)
        {
            a32->is_eof = FILE_IS_EOF;
        }
        ret_sonix = _a32_fill_buffer(a32, a32->parser.data_addr, a32->parser.data_size);
        CHECK_MIDDLEWARE_RET(ret_sonix);
#endif
    }
    else
    {
        return A32_MSG_LACK_DATA;
    }
    return ret_sonix;
}

static int32_t _a32_get_nbyte(uint8_t *need_data, uint32_t need_size, A32_FILE_RES_T *a32)
{
    int32_t ret_sonix = HAL_OK;
    if (a32->parser.buff_addr == NULL)
    {
        return HAL_ERROR;
    }
    if ((a32->parser.buff_pos + need_size) > a32->parser.buff_size)
    {
        return HAL_ERROR;
    }

    if (need_data)
    {
        memcpy(need_data, a32->parser.buff_addr + a32->parser.buff_pos, need_size);
    }
    a32->parser.buff_pos += need_size;
    int64_t size = _a32_f_size(a32) - _a32_f_tell(a32);
    if (size <= 0)
    {
        a32->is_eof = FILE_IS_EOF;
    }
    return ret_sonix;
}

static int32_t _a32_leak_data(A32_FILE_RES_T *a32, uint32_t need_size)
{
    int32_t ret_sonix = HAL_OK;
    if ((a32->a32_status & A32_PARSE_LACK_DATA) == A32_PARSE_LACK_DATA)
    {
        a32->parser.buff_cnt++;
        a32->parser.buff_pos = 0;

        uint32_t buff_size = a32->parser.tail_used + a32->parser.data_size;
        if (buff_size > a32->parser.buff_total)
        {
            MEMORY_FREE(a32->parser.buff_addr);
            MEMORY_ALLOC(a32->parser.buff_addr, buff_size, GFP_KERNEL);
            a32->parser.buff_total = buff_size;
        }
        memcpy(a32->parser.buff_addr, a32->parser.tail_addr, a32->parser.tail_used);
        memcpy(a32->parser.buff_addr + a32->parser.tail_used, a32->parser.data_addr, a32->parser.data_size);

        a32->parser.buff_size = buff_size;
        a32->a32_status &= ~A32_PARSE_LACK_DATA;
        ret_sonix = HAL_OK;
    }
    if (a32->parser.buff_pos + need_size > a32->parser.buff_size)
    {
        uint32_t tail_size = a32->parser.buff_size - a32->parser.buff_pos;
        if (tail_size != 0)
        {
            if (tail_size > a32->parser.tail_size)
            {
                MEMORY_FREE(a32->parser.tail_addr);
                MEMORY_ALLOC(a32->parser.tail_addr, tail_size, GFP_KERNEL);
                a32->parser.tail_size = tail_size;
            }
            memcpy(a32->parser.tail_addr, a32->parser.buff_addr + a32->parser.buff_pos, tail_size);
        }

        a32->parser.tail_used = tail_size;
        a32->a32_status |= A32_PARSE_LACK_DATA;
        ret_sonix = A32_MSG_LACK_DATA;
    }
    return ret_sonix;
}

static int32_t _a32_seek_and_write(uint64_t seek, uint32_t write_size, void *write_buff, A32_FILE_RES_T *a32)
{
    if (a32->src == FILE_SRC_FROM_FATFS)
    {
#if A32_USE_FATFS
        return _file_seek_and_write(seek, write_size, write_buff, a32->fp);
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
static int32_t _a32_parse_header(A32_FILE_RES_T *a32)
{
    int32_t ret_sonix = HAL_ERROR;
    ret_sonix         = _a32_leak_data(a32, sizeof(A32_FILE_HEADER_T));
    CHECK_MIDDLEWARE_RET(ret_sonix);

    ret_sonix = _a32_get_nbyte((uint8_t *)&a32->header, sizeof(A32_FILE_HEADER_T), a32);
    CHECK_MIDDLEWARE_RET(ret_sonix);
    if (a32->header.A32Tag != A32_TAG)
    {
        return HAL_ERROR;
    }

    a32->header_len = sizeof(A32_FILE_HEADER_T);
    a32->bit_rate   = a32->header.bit_rate_tag * 10 * a32->header.sample_rate / 16000;
    a32->frame_siz  = (a32->bit_rate * 320 / a32->header.sample_rate) / 8;
    a32->frame_cnt  = (_a32_f_size(a32) - a32->header_len) / a32->frame_siz;
    return HAL_OK;
}

static int32_t _a32_header_set(A32_FILE_RES_T *a32)
{
    a32->header.A32Tag       = A32_TAG;
    a32->header.sample_rate  = a32->info.sample_rate;
    a32->header.bit_rate_tag = a32->info.bit_rate * 16000 / (10 * a32->info.sample_rate);
    a32->header.channel      = a32->info.num_of_channels;
    a32->header.frame_cnt    = 0;
    a32->header.file_len     = 0;
    a32->header.MF           = 0;
    a32->header.SF           = 0;
    a32->header.MBF          = 0;
    a32->header.PCS          = 0xFFFF;
    a32->header.sRec         = 0;
    a32->header.header_len   = sizeof(A32_FILE_HEADER_T) / 2;
    a32->header.audio_type   = a32->info.format;
    a32->header.stop_code    = 0xFFFF;
    a32->header.s_header     = 0xFFFF;

    a32->bit_rate  = a32->header.bit_rate_tag * 10 * a32->header.sample_rate / 16000;
    a32->frame_siz = (a32->bit_rate * 320 / a32->header.sample_rate) / 8;
    return HAL_OK;
}

static int32_t _a32_free_parser(A32_FILE_RES_T *a32)
{
    if (a32->src == FILE_SRC_FROM_FATFS)
    {
#if A32_USE_FATFS
        MEMORY_FREE(a32->parser.data_addr);
#endif
    }
    MEMORY_FREE(a32->parser.buff_addr);
    MEMORY_FREE(a32->parser.tail_addr);
    memset(&a32->parser, 0, sizeof(a32->parser));

    return HAL_OK;
}

//////////////////////////////////////////////////////////////////////////////////////////////
static int32_t _a32_open(A32_FILE_RES_T *a32)
{
    int32_t ret_sonix = HAL_ERROR;
    if ((a32->mode & FA_READ) == FA_READ)
    {
        // ret_sonix = _a32_parse_header(a32);
        // CHECK_MIDDLEWARE_RET(ret_sonix);

        ret_sonix = _a32_fill_parser(a32);
        while (a32->is_eof == FILE_IS_NOT_EOF)
        {
            ret_sonix = _a32_parse_header(a32);
            if (ret_sonix == A32_MSG_LACK_DATA)
            {
                ret_sonix = _a32_fill_parser(a32);
                CHECK_MIDDLEWARE_RET(ret_sonix);
                continue;
            }
            CHECK_MIDDLEWARE_RET(ret_sonix);
            break;
        }
    }
    if ((a32->mode & FA_WRITE) == FA_WRITE)
    {
        ret_sonix = _a32_header_set(a32);
        CHECK_MIDDLEWARE_RET(ret_sonix);
        ret_sonix = _a32_seek_and_write(FILE_NOT_SEEK, sizeof(A32_FILE_HEADER_T), &a32->header, a32);
        CHECK_MIDDLEWARE_RET(ret_sonix);
    }
    return HAL_OK;
}

static int32_t _a32_close(A32_FILE_RES_T *a32)
{
    _a32_free_parser(a32);
    if ((a32->mode & FA_WRITE) == FA_WRITE)
    {
        int32_t ret_sonix     = HAL_ERROR;
        a32->header.frame_cnt = a32->frame_cnt;
        a32->header.file_len >>= 1;
        ret_sonix = _a32_seek_and_write(0, sizeof(A32_FILE_HEADER_T), &a32->header, a32);
        CHECK_MIDDLEWARE_RET(ret_sonix);
    }
    return HAL_OK;
}

static int32_t _a32_read(A32_FILE_RES_T *a32, FILE_FRAME_T *frame, uint32_t is_blocking)
{
    if (frame->length < a32->frame_siz)
    {
        frame->length = a32->frame_siz;
        return A32_MSG_LACK_BUFF;
    }
    if (a32->frame_idx >= a32->frame_cnt)
    {
        return HAL_OK;
    }

    int32_t ret_sonix = HAL_OK;
    while (a32->is_eof == FILE_IS_NOT_EOF)
    {
        ret_sonix = _a32_leak_data(a32, a32->frame_siz);
        if (ret_sonix == A32_MSG_LACK_DATA)
        {
            ret_sonix = _a32_fill_parser(a32);
            CHECK_MIDDLEWARE_RET(ret_sonix);
        }
        else
        {
            CHECK_MIDDLEWARE_RET(ret_sonix);
            ret_sonix = _a32_get_nbyte(frame->buffer, a32->frame_siz, a32);
            CHECK_MIDDLEWARE_RET(ret_sonix);
            a32->frame_idx++;
            frame->length = a32->frame_siz;
            break;
        }
    }

    return HAL_OK;
}

static int32_t _a32_write(A32_FILE_RES_T *a32, const FILE_FRAME_T *frame, uint32_t is_blocking)
{
    if (frame->length != a32->frame_siz)
    {
        return HAL_ERROR;
    }

    int32_t ret_sonix = _a32_seek_and_write(FILE_NOT_SEEK, frame->length, frame->buffer, a32);
    CHECK_MIDDLEWARE_RET(ret_sonix);
    a32->header.file_len += frame->length;
    a32->frame_idx++;
    a32->frame_cnt = a32->frame_idx;
    return HAL_OK;
}

static int32_t _a32_seek(A32_FILE_RES_T *a32, const A32_SEEK_T *seek)
{
#if A32_USE_FATFS
    if (seek->index >= a32->frame_cnt)
    {
        return HAL_ERROR;
    }

    _a32_free_parser(a32);
    a32->is_eof    = FILE_IS_NOT_EOF;
    a32->frame_idx = seek->index;
    if (a32->src == FILE_SRC_FROM_FATFS)
    {
        uint64_t seek      = a32->header_len + a32->frame_siz * a32->frame_idx;
        int32_t  ret_sonix = _file_seek(a32->fp, seek);
        CHECK_MIDDLEWARE_RET(ret_sonix);
    }
    return HAL_OK;
#else
    return HAL_ERROR;
#endif
}

static int32_t _a32_wait(A32_FILE_RES_T *a32)
{
    return HAL_OK;
}

static int32_t _a32_get_status(A32_FILE_RES_T *a32, A32_STATUS_T *status)
{
    status->format          = a32->header.audio_type;
    status->num_of_channels = a32->header.channel;
    status->sample_rate     = a32->header.sample_rate;
    status->bit_rate        = a32->bit_rate;
    status->frame_cnt       = a32->frame_cnt;
    status->frame_idx       = a32->frame_idx;
    status->is_eof          = 0;
    if (status->frame_cnt == status->frame_idx)
    {
        status->is_eof = 1;
    }
    return HAL_OK;
}

///////////////////////////////////////////////////////////////////////////////////////
static int32_t A32_Open(const void *a32)
{
    CHECK_POINTER_NULL(a32);
    CHECK_POINTER_NULL(((FIL_RES_T *)a32)->res);
    return _a32_open(((FIL_RES_T *)a32)->res);
};
static int32_t A32_Close(const void *a32)
{
    CHECK_POINTER_NULL(a32);
    // CHECK_POINTER_NULL(((FIL_RES_T *)a32)->res);
    if (((FIL_RES_T *)a32)->res == NULL)
    { // close return ok to continue other close flow
        return HAL_OK;
    }
    return _a32_close(((FIL_RES_T *)a32)->res);
};
static int32_t A32_Read(const void *a32, void *frame, uint8_t is_blocking)
{
    CHECK_POINTER_NULL(a32);
    CHECK_POINTER_NULL(((FIL_RES_T *)a32)->res);
    CHECK_POINTER_NULL(frame);
    return _a32_read(((FIL_RES_T *)a32)->res, frame, is_blocking);
};
static int32_t A32_Write(const void *a32, const void *frame, uint8_t is_blocking)
{
    CHECK_POINTER_NULL(a32);
    CHECK_POINTER_NULL(((FIL_RES_T *)a32)->res);
    CHECK_POINTER_NULL(frame);
    return _a32_write(((FIL_RES_T *)a32)->res, frame, is_blocking);
};
static int32_t A32_Seek(const void *a32, const void *seek)
{
    CHECK_POINTER_NULL(a32);
    CHECK_POINTER_NULL(((FIL_RES_T *)a32)->res);
    CHECK_POINTER_NULL(seek);
    return _a32_seek(((FIL_RES_T *)a32)->res, seek);
};
static int32_t A32_Wait(const void *a32)
{
    CHECK_POINTER_NULL(a32);
    CHECK_POINTER_NULL(((FIL_RES_T *)a32)->res);
    return _a32_wait(((FIL_RES_T *)a32)->res);
};
static int32_t A32_GetStatus(const void *a32, void *status)
{
    CHECK_POINTER_NULL(a32);
    CHECK_POINTER_NULL(((FIL_RES_T *)a32)->res);
    CHECK_POINTER_NULL(status);
    return _a32_get_status(((FIL_RES_T *)a32)->res, status);
};

TYPE_WRAPPER A32_Wrapper = {
    A32_Open,
    A32_Close,
    A32_Read,
    A32_Write,
    A32_Wait,
    A32_Seek,
    A32_GetStatus,
};

/////////////////////////////////////////////////////////////////////////////////
static int32_t A32_DoInit(void *sh)
{
    int32_t ret_sonix = HAL_ERROR;
#if A32_USE_STREAM
    CHECK_POINTER_NULL(sh);
    A32_HANDLE_T *buf_fh = sh;

    A32_FILE_RES_T *a32_res = buf_fh->other_res;
    MEMORY_ALLOC(a32_res, sizeof(A32_FILE_RES_T), GFP_KERNEL);
    if ((a32_res->a32_status & A32_PARSE_INIT) == A32_PARSE_INIT)
    {
        return HAL_OK;
    }

    buf_fh->is_eof    = FILE_IS_NOT_EOF;
    buf_fh->last_buff = 0;
    buf_fh->buff_addr = 0;
    buf_fh->buff_size = 0;
    buf_fh->other_res = a32_res;

    a32_res->mode     = FA_READ;
    a32_res->src      = FILE_SRC_FROM_BUFF;
    a32_res->file_siz = 0x7FFFFFFF;
    a32_res->a32_status |= A32_PARSE_INIT;
    ret_sonix = HAL_OK;
#endif
    return ret_sonix;
}
static int32_t A32_DeInit(void *sh)
{
    int32_t ret_sonix = HAL_ERROR;
#if A32_USE_STREAM
    CHECK_POINTER_NULL(sh);
    A32_HANDLE_T   *buf_fh  = sh;
    A32_FILE_RES_T *a32_res = buf_fh->other_res;
    CHECK_POINTER_NULL(a32_res);

    if (a32_res->sfp->init == 1)
    {
        a32_res->sfp->init = 0;
    }
    _a32_close(a32_res);
    buf_fh->is_eof    = FILE_IS_NOT_EOF;
    buf_fh->last_buff = 0;
    buf_fh->buff_addr = 0;
    buf_fh->buff_size = 0;
    MEMORY_FREE(a32_res->sfp);
    MEMORY_FREE(buf_fh->other_res);
    ret_sonix = HAL_OK;
#endif
    return ret_sonix;
}
static int32_t A32_ReadDev(void *sh, void *info)
{
    int32_t ret_sonix = HAL_ERROR;
#if A32_USE_STREAM
    CHECK_POINTER_NULL(sh);
    CHECK_POINTER_NULL(info);
    A32_HANDLE_T   *buf_fh  = sh;
    A32_FILE_RES_T *a32_res = buf_fh->other_res;
    CHECK_POINTER_NULL(a32_res);
    if (a32_res->sfp->beg_addr == 0 ||
        a32_res->sfp->end_addr == 0 ||
        a32_res->sfp->dev == (uint8_t)DEV_NULL)
    {
        return HAL_ERROR;
    }

    if (a32_res->sfp->init == 0)
    {
        a32_res->sfp->init = 1;
    }

    FILE_FRAME_T *frame_tmp = info;
    uint32_t      size      = a32_res->sfp->end_addr - a32_res->sfp->cur_addr;
    if (size > frame_tmp->length)
    {
        size = frame_tmp->length;
    }
    else
    {
        a32_res->is_eof = FILE_IS_EOF;
    }
    ret_sonix = StgWrapper.ReadByte(a32_res->sfp->dev, frame_tmp->buffer, a32_res->sfp->cur_addr, frame_tmp->length);
    CHECK_MIDDLEWARE_RET(ret_sonix);
    a32_res->sfp->cur_addr += size;
    frame_tmp->length = size;
#endif
    return ret_sonix;
}
static int32_t A32_GetFrame(void *sh, void *frame)
{
    int32_t ret_sonix = HAL_ERROR;
#if A32_USE_STREAM
    CHECK_POINTER_NULL(sh);
    CHECK_POINTER_NULL(frame);
    A32_HANDLE_T   *buf_fh  = sh;
    A32_FILE_RES_T *a32_res = buf_fh->other_res;
    CHECK_POINTER_NULL(buf_fh);
    CHECK_POINTER_NULL(a32_res);
    if (buf_fh->buff_addr == NULL || buf_fh->buff_size == 0)
    {
        return A32_MSG_LACK_DATA;
    }

    // check init & fill parser buffer
    if ((a32_res->is_eof == FILE_IS_EOF) || (a32_res->a32_status & A32_PARSE_INIT) != A32_PARSE_INIT)
    {
        return HAL_ERROR;
    }

    ret_sonix = _a32_fill_buffer(a32_res, (uint8_t *)buf_fh->buff_addr, buf_fh->buff_size);
    CHECK_MIDDLEWARE_RET(ret_sonix);
    if (buf_fh->last_buff == 1)
    {
        a32_res->file_siz = a32_res->file_pos;
    }

    // parser header
    if ((a32_res->a32_status & A32_PARSE_FIND_FIRST) != A32_PARSE_FIND_FIRST)
    {
        ret_sonix = _a32_parse_header(a32_res);
        CHECK_MIDDLEWARE_RET(ret_sonix);
        a32_res->a32_status |= A32_PARSE_FIND_FIRST;
    }

    // read a valid frame
    A32_FRAME_T *frame_out = frame;
    FILE_FRAME_T frame_tmp = {0};
    frame_tmp.buffer       = frame_out->cache;
    frame_tmp.length       = frame_out->cache_len;
    ret_sonix              = _a32_read(a32_res, &frame_tmp, FILE_IS_BLOCKING);
    buf_fh->frame_len      = frame_tmp.length;
    CHECK_MIDDLEWARE_RET(ret_sonix);
    frame_out->data     = frame_tmp.buffer;
    frame_out->data_len = frame_tmp.length;
    ret_sonix           = _a32_get_status(a32_res, &frame_out->status);
    CHECK_MIDDLEWARE_RET(ret_sonix);

    buf_fh->is_eof = a32_res->is_eof;
#endif
    return ret_sonix;
}
PARSE_WRAPPER A32_Stream = {
    A32_DoInit,
    A32_DeInit,
    A32_ReadDev,
    A32_GetFrame,
};
