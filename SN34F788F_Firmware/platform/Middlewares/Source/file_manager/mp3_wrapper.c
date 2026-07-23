/***********************
 * SN34F78X SDK V2.1.4 *
 ***********************/

#include "mp3_wrapper.h"

#define MP3_SAMPLE_RATE_MAX   (0x3)
#define MP3_BIT_RATE_MAX      (0x10)
#define MP3_SAMPLE_TO_BYTE(x) (x / 8)
#define MP3_KB_TO_BIT(x)      (x * 1000)
#define MP3_FRAME_LEN_MAX     (0x800)
#define MP3_ID3V2_TAG_LEN     (0x3)
#define MP3_MIN_BUFF_SIZE     (0x400)

#define MP3_PARSE_INIT        (0x1 << 0)
#define MP3_PARSE_XING        (0x1 << 1)
#define MP3_PARSE_IS_EOF      (0x1 << 2)
#define MP3_PARSE_LACK_DATA   (0x1 << 3)
#define MP3_PARSE_ID3V2_SIZE  (0x1 << 4)
#define MP3_PARSE_ID3V2_DATA  (0x1 << 5)
#define MP3_PARSE_READ_FRAME  (0x1 << 6)
#define MP3_PARSE_FIND_FIRST  (0x1 << 7)
#define MP3_PARSE_FIRST_FRAME (0x1 << 8)
#define MP3_PARSE_VALID_FRAME (0x1 << 9)

#define MP3_USE_FATFS  (1)
#define MP3_USE_STREAM (0)

#if MP3_USE_STREAM
#include "Storage_wrapper.h"
#endif

////////////////////////////////////////////////////////////////////////////////////////

// hz/s
#define MP3_SAMPLE_RATE1 11025, 0, 22050, 44100
#define MP3_SAMPLE_RATE2 12000, 0, 24000, 48000
#define MP3_SAMPLE_RATE3 8000, 0, 16000, 32000
static uint16_t MP3_sample_rate[MP3_SAMPLE_RATE_MAX][MP3_MPEG_MAX] = {
    MP3_SAMPLE_RATE1,
    MP3_SAMPLE_RATE2,
    MP3_SAMPLE_RATE3,
};

// point/s
#define MP3_SAMPLE_LAYER1 0x180, 0, 0x180, 0x180
#define MP3_SAMPLE_LAYER2 0x480, 0, 0x480, 0x480
#define MP3_SAMPLE_LAYER3 0x240, 0, 0x240, 0x480
static uint16_t MP3_sample_point[MP3_LAYER_MAX][MP3_MPEG_MAX] = {
    MP3_SAMPLE_LAYER1,
    MP3_SAMPLE_LAYER2,
    MP3_SAMPLE_LAYER3,
};

// kb/s=1000bit/s
#define MP3_BITRATE_V1L1 0x00, 0x20, 0x40, 0x60, 0x80, 0xA0, 0xC0, 0xE0, 0x100, 0x120, 0x140, 0x160, 0x180, 0x1A0, 0x1C0, 0xFFFF
#define MP3_BITRATE_V1L2 0x00, 0x20, 0x30, 0x38, 0x40, 0x50, 0x60, 0x70, 0x080, 0x0A0, 0x0C0, 0x0E0, 0x100, 0x140, 0x180, 0xFFFF
#define MP3_BITRATE_V1L3 0x00, 0x20, 0x28, 0x30, 0x38, 0x40, 0x50, 0x60, 0x070, 0x080, 0x0A0, 0x0C0, 0x0E0, 0x100, 0x140, 0xFFFF
#define MP3_BITRATE_V2L1 0x00, 0x20, 0x30, 0x38, 0x40, 0x50, 0x60, 0x70, 0x080, 0x090, 0x0A0, 0x0B0, 0x0C0, 0x0E0, 0x100, 0xFFFF
#define MP3_BITRATE_V2L2 0x00, 0x08, 0x10, 0x18, 0x20, 0x28, 0x30, 0x38, 0x040, 0x050, 0x060, 0x070, 0x080, 0x090, 0x0A0, 0xFFFF
#define MP3_BITRATE_V2L3 0x00, 0x08, 0x10, 0x18, 0x20, 0x28, 0x30, 0x38, 0x040, 0x050, 0x060, 0x070, 0x080, 0x090, 0x0A0, 0xFFFF
static uint16_t MP3_bit_rate[2][MP3_LAYER_MAX][MP3_BIT_RATE_MAX] = {
    MP3_BITRATE_V2L1,
    MP3_BITRATE_V2L2,
    MP3_BITRATE_V2L3,
    MP3_BITRATE_V1L1,
    MP3_BITRATE_V1L2,
    MP3_BITRATE_V1L3,
};

// byte
#define MP3_SIDE_INFORMATION_SIZE_MPEGV1 0x11, 0x20
#define MP3_SIDE_INFORMATION_SIZE_MPEGV2 0x09, 0x11
#define MP3_SIDE_INFORMATION_SIZE_MPEGV3 0x00, 0x00
#define MP3_SIDE_INFORMATION_SIZE_MPEGV4 0x09, 0x11
static uint8_t MP3_side_information_size[MP3_MPEG_MAX][MP3_CHANNEL_MAX - 1] = {
    MP3_SIDE_INFORMATION_SIZE_MPEGV4,
    MP3_SIDE_INFORMATION_SIZE_MPEGV3,
    MP3_SIDE_INFORMATION_SIZE_MPEGV2,
    MP3_SIDE_INFORMATION_SIZE_MPEGV1,
};

////////////////////////////////////////////////////////////////////////////////////////
static int32_t _mp3_f_tell(MP3_FILE_RES_T *mp3)
{
    if (mp3->src == FILE_SRC_FROM_FATFS)
    {
#if MP3_USE_FATFS
        return f_tell(mp3->fp) - mp3->parser.buff_size + mp3->parser.buff_pos;
#else
        return 0;
#endif
    }
    else
    {
        return mp3->file_pos - mp3->parser.buff_size + mp3->parser.buff_pos;
    }
}

static int32_t _mp3_f_size(MP3_FILE_RES_T *mp3)
{
    if (mp3->src == FILE_SRC_FROM_FATFS)
    {
#if MP3_USE_FATFS
        return f_size(mp3->fp);
#else
        return 0;
#endif
    }
    else
    {
        return mp3->file_siz;
    }
}

#if MP3_USE_FATFS || MP3_USE_STREAM
static int32_t _mp3_fill_buffer(MP3_FILE_RES_T *mp3, uint8_t *data_addr, uint32_t data_size)
{
    if ((mp3->file_pos == 0) || ((mp3->mp3_status & MP3_PARSE_LACK_DATA) == MP3_PARSE_LACK_DATA))
    {
        mp3->parser.data_addr = data_addr;
        mp3->parser.data_size = data_size;
        mp3->file_pos += data_size;
        if (mp3->parser.buff_addr == NULL)
        {
            mp3->parser.buff_cnt++;
            mp3->parser.buff_pos   = 0;
            mp3->parser.buff_size  = mp3->parser.data_size;
            mp3->parser.buff_total = mp3->parser.data_size;
            MEMORY_ALLOC(mp3->parser.buff_addr, mp3->parser.buff_size, GFP_SRAM);
            memcpy(mp3->parser.buff_addr, mp3->parser.data_addr, mp3->parser.buff_size);
        }
    }
    return HAL_OK;
}
#endif

/* Read the data of FILE_PARSE_BUFF_SIZE(1 KB) and store it in mp3->parser.buff_addr */
static int32_t _mp3_fill_parser(MP3_FILE_RES_T *mp3)
{
    int32_t ret_sonix = HAL_ERROR;
    if (mp3->src == FILE_SRC_FROM_FATFS)
    {
#if MP3_USE_FATFS
        if (mp3->parser.data_addr == NULL)
        {
            mp3->parser.data_size = FILE_PARSE_BUFF_SIZE;
            MEMORY_ALLOC(mp3->parser.data_addr, mp3->parser.data_size, GFP_SRAM);
        }
        ret_sonix = _file_seek_and_read(FILE_NOT_SEEK, &mp3->parser.data_size, mp3->parser.data_addr, mp3->fp);
        CHECK_MIDDLEWARE_RET(ret_sonix);
        if (mp3->parser.data_size == 0)
        {
            mp3->is_eof = FILE_IS_EOF;
        }
        ret_sonix = _mp3_fill_buffer(mp3, mp3->parser.data_addr, mp3->parser.data_size);
        CHECK_MIDDLEWARE_RET(ret_sonix);
#endif
    }
    else
    {
        return MP3_MSG_LACK_DATA;
    }
    return ret_sonix;
}

static int32_t _mp3_get_nbyte(uint8_t *need_data, uint32_t need_size, MP3_FILE_RES_T *mp3)
{
    int32_t ret_sonix = HAL_OK;
    if (mp3->parser.buff_addr == NULL)
    {
        return HAL_ERROR;
    }
    if ((mp3->parser.buff_pos + need_size) > mp3->parser.buff_size)
    {
        return HAL_ERROR;
    }

    if (need_data)
    {
        memcpy(need_data, mp3->parser.buff_addr + mp3->parser.buff_pos, need_size);
    }
    mp3->parser.buff_pos += need_size;
    int64_t size = _mp3_f_size(mp3) - _mp3_f_tell(mp3);
    if (size <= 0)
    {
        mp3->is_eof = FILE_IS_EOF;
    }
    return ret_sonix;
}

static int32_t _mp3_leak_data(MP3_FILE_RES_T *mp3, uint32_t need_size)
{
    int32_t ret_sonix = HAL_OK;
    if ((mp3->mp3_status & MP3_PARSE_LACK_DATA) == MP3_PARSE_LACK_DATA)
    {
        mp3->parser.buff_cnt++;
        mp3->parser.buff_pos = 0;

        uint32_t buff_size = mp3->parser.tail_used + mp3->parser.data_size;
        if (buff_size > mp3->parser.buff_total)
        {
            MEMORY_FREE(mp3->parser.buff_addr);
            MEMORY_ALLOC(mp3->parser.buff_addr, buff_size, GFP_KERNEL);
            mp3->parser.buff_total = buff_size;
        }
        memcpy(mp3->parser.buff_addr, mp3->parser.tail_addr, mp3->parser.tail_used);
        memcpy(mp3->parser.buff_addr + mp3->parser.tail_used, mp3->parser.data_addr, mp3->parser.data_size);

        mp3->parser.buff_size = buff_size;
        mp3->mp3_status &= ~MP3_PARSE_LACK_DATA;
        ret_sonix = HAL_OK;
    }
    if (mp3->parser.buff_pos + need_size > mp3->parser.buff_size)
    {
        uint32_t tail_size = mp3->parser.buff_size - mp3->parser.buff_pos;
        if (tail_size != 0)
        {
            if (tail_size > mp3->parser.tail_size)
            {
                MEMORY_FREE(mp3->parser.tail_addr);
                MEMORY_ALLOC(mp3->parser.tail_addr, tail_size, GFP_KERNEL);
                mp3->parser.tail_size = tail_size;
            }
            memcpy(mp3->parser.tail_addr, mp3->parser.buff_addr + mp3->parser.buff_pos, tail_size);
        }

        mp3->parser.tail_used = tail_size;
        mp3->mp3_status |= MP3_PARSE_LACK_DATA;
        ret_sonix = MP3_MSG_LACK_DATA;
    }
    return ret_sonix;
}

static int32_t _mp3_seek_and_write(uint64_t seek, uint32_t write_size, void *write_buff, MP3_FILE_RES_T *mp3)
{
    if (mp3->src == FILE_SRC_FROM_FATFS)
    {
#if MP3_USE_FATFS
        return _file_seek_and_write(seek, write_size, write_buff, mp3->fp);
#else
        return HAL_ERROR;
#endif
    }
    else
    {
        return HAL_ERROR;
    }
}

static int32_t _mp3_free_parser(MP3_FILE_RES_T *mp3)
{
    if (mp3->src == FILE_SRC_FROM_FATFS)
    {
#if MP3_USE_FATFS
        MEMORY_FREE(mp3->parser.data_addr);
#endif
    }
    MEMORY_FREE(mp3->parser.buff_addr);
    MEMORY_FREE(mp3->parser.tail_addr);
    memset(&mp3->parser, 0, sizeof(mp3->parser));

    return HAL_OK;
}
////////////////////////////////////////////////////////////////////////////////////////
static int32_t _mp3_check_frame(MP3_FILE_RES_T *mp3)
{
    int32_t ret_sonix = HAL_OK;
    ret_sonix         = _mp3_leak_data(mp3, sizeof(MP3_FRAME_HEADER_T));
    CHECK_MIDDLEWARE_RET(ret_sonix);

    MP3_FRAME_INFO_T *header   = &mp3->frame_curr->header;
    uint8_t           sample   = 0;
    uint8_t           layer    = 0;
    uint8_t           ver      = 0;
    uint8_t           bit      = 0;
    uint8_t           padding  = 0;
    uint8_t           head_len = 0;
    uint8_t           rvt_size = 0;

    ret_sonix = _mp3_get_nbyte((uint8_t *)&header->sync1, 1, mp3);
    CHECK_MIDDLEWARE_RET(ret_sonix);
    // rvt_size += 1;
    if (header->sync1 != MP3_FRAME_SYNC1)
    {
        goto __EXIT_MP3_CHECK_HEADER;
    }

    ret_sonix = _mp3_get_nbyte((uint8_t *)&header->sync2, 1, mp3);
    CHECK_MIDDLEWARE_RET(ret_sonix);
    rvt_size += 1;
    if (header->sync2.sync != MP3_FRAME_SYNC2)
    {
        goto __EXIT_MP3_CHECK_HEADER;
    }
    layer = 3 - header->sync2.layer;
    if (layer == MP3_LAYER_MAX)
    {
        goto __EXIT_MP3_CHECK_HEADER;
    }
    ver = header->sync2.version;
    if (ver == MP3_MPEG_V3)
    {
        goto __EXIT_MP3_CHECK_HEADER;
    }

    ret_sonix = _mp3_get_nbyte((uint8_t *)&header->header, sizeof(MP3_FRAME_H_T), mp3);
    CHECK_MIDDLEWARE_RET(ret_sonix);
    rvt_size += sizeof(MP3_FRAME_H_T);

    bit = header->header.bit_rate_index;
    if (bit == 0x0 || bit == 0xF)
    {
        goto __EXIT_MP3_CHECK_HEADER;
    }
    sample = header->header.sample_rate_index;
    if (sample == 0x3)
    {
        goto __EXIT_MP3_CHECK_HEADER;
    }

    layer    = 3 - header->sync2.layer;
    ver      = header->sync2.version;
    padding  = header->header.padding * ((layer == MP3_LAYER_1) ? 4 : 1);
    head_len = sizeof(MP3_FRAME_INFO_T) + (1 - header->sync2.protection) * sizeof(uint16_t);
    if (header->sync2.protection == 0)
    {
        ret_sonix = _mp3_get_nbyte((uint8_t *)&mp3->frame_curr->CRC, sizeof(uint16_t), mp3);
        CHECK_MIDDLEWARE_RET(ret_sonix);
        rvt_size += sizeof(uint16_t);
    }
    mp3->data_len = MP3_SAMPLE_TO_BYTE(MP3_sample_point[layer][ver]) * MP3_KB_TO_BIT(MP3_bit_rate[ver & 0x1][layer][bit]) / MP3_sample_rate[sample][ver] + padding - head_len;
    if (mp3->data_len >= MP3_FRAME_LEN_MAX)
    {
        goto __EXIT_MP3_CHECK_HEADER;
    }
    mp3->head_len = head_len;
    return HAL_OK;

__EXIT_MP3_CHECK_HEADER:
    mp3->parser.buff_pos -= rvt_size;
    CHECK_MIDDLEWARE_RET(ret_sonix);
    return HAL_ERROR;
}

static int32_t _mp3_valid_frame(MP3_FILE_RES_T *mp3)
{
    if ((mp3->mp3_status & MP3_PARSE_VALID_FRAME) == MP3_PARSE_VALID_FRAME)
    {
        return HAL_OK;
    }
    if (mp3->frame_curr == NULL)
    {
        MEMORY_ALLOC(mp3->frame_curr, sizeof(MP3_FRAME_HEADER_T), GFP_KERNEL);
    }

    int32_t ret_sonix = HAL_OK;
    while (mp3->is_eof == FILE_IS_NOT_EOF)
    {
        ret_sonix = _mp3_check_frame(mp3);
        if (ret_sonix == MP3_MSG_LACK_DATA)
        {
            return MP3_MSG_LACK_DATA;
        }
        else if (ret_sonix == HAL_OK)
        {
            break;
        }
    }

    mp3->mp3_status |= MP3_PARSE_VALID_FRAME;
    return HAL_OK;
}

static int32_t _mp3_test_frame(MP3_FILE_RES_T *mp3)
{
    if (mp3->frame_keep == NULL)
    {
        MEMORY_ALLOC(mp3->frame_keep, sizeof(MP3_FRAME_HEADER_T), GFP_KERNEL);
        memcpy(mp3->frame_keep, mp3->frame_curr, sizeof(MP3_FRAME_HEADER_T));
        return HAL_OK;
    }

    if (mp3->frame_keep->header.header.sample_rate_index != mp3->frame_curr->header.header.sample_rate_index)
    {
        return HAL_ERROR;
    }
    if (mp3->frame_keep->header.header.channel != mp3->frame_curr->header.header.channel)
    {
        return HAL_ERROR;
    }
    if (mp3->frame_keep->header.sync2.layer != mp3->frame_curr->header.sync2.layer)
    {
        return HAL_ERROR;
    }
    if (mp3->frame_keep->header.sync2.version != mp3->frame_curr->header.sync2.version)
    {
        return HAL_ERROR;
    }
    return HAL_OK;
}

static int32_t _mp3_read_frame(MP3_FILE_RES_T *mp3, uint32_t *buff_addr)
{
    if ((mp3->mp3_status & MP3_PARSE_READ_FRAME) == MP3_PARSE_READ_FRAME)
    {
        return HAL_OK;
    }
    int32_t ret_sonix = HAL_OK;
    ret_sonix         = _mp3_leak_data(mp3, mp3->data_len);
    CHECK_MIDDLEWARE_RET(ret_sonix);

    if (buff_addr)
    {
        uint8_t *buffer = (uint8_t *)*buff_addr;
        memcpy(buffer, (uint8_t *)mp3->frame_curr, mp3->head_len);
        ret_sonix = _mp3_get_nbyte(buffer + mp3->head_len, mp3->data_len, mp3);
        CHECK_MIDDLEWARE_RET(ret_sonix);
    }
    else
    {
        ret_sonix = _mp3_get_nbyte(NULL, mp3->data_len, mp3);
        CHECK_MIDDLEWARE_RET(ret_sonix);
    }
    int64_t size = _mp3_f_size(mp3) - _mp3_f_tell(mp3) + 1;
    if (size < mp3->head_len + mp3->data_len)
    {
        mp3->is_eof = FILE_IS_EOF;
    }

    mp3->mp3_status |= MP3_PARSE_READ_FRAME;
    return HAL_OK;
}

static int32_t _mp3_begin_frame(MP3_FILE_RES_T *mp3)
{
    if ((mp3->mp3_status & MP3_PARSE_FIRST_FRAME) == MP3_PARSE_FIRST_FRAME)
    {
        return HAL_OK;
    }

    int32_t ret_sonix = HAL_OK;
    ret_sonix         = _mp3_test_frame(mp3);
    if (ret_sonix != HAL_OK)
    {
        memcpy(mp3->frame_keep, mp3->frame_curr, sizeof(MP3_FRAME_HEADER_T));
    }
    mp3->mp3_status |= MP3_PARSE_FIRST_FRAME;
    return HAL_OK;
}

static int32_t _mp3_save_frame(MP3_FILE_RES_T *mp3)
{
    int32_t ret_sonix = HAL_OK;
    ret_sonix         = _mp3_valid_frame(mp3);
    CHECK_MIDDLEWARE_RET(ret_sonix);
    ret_sonix = _mp3_begin_frame(mp3);
    CHECK_MIDDLEWARE_RET(ret_sonix);
    uint32_t index = mp3->check_cnt;
    if (mp3->head_len + mp3->data_len > MP3_FRAME_LEN_MAX)
    {
        return HAL_ERROR;
    }
    MEMORY_ALLOC(mp3->frame_save[index].data.buffer, MP3_FRAME_LEN_MAX, GFP_KERNEL);
    mp3->frame_save[index].data.length = mp3->head_len + mp3->data_len;
    ret_sonix                          = _mp3_read_frame(mp3, (uint32_t *)&mp3->frame_save[index].data.buffer);
    CHECK_MIDDLEWARE_RET(ret_sonix);
    mp3->frame_save[index].head_len = mp3->head_len;
    mp3->check_cnt++;
    mp3->mp3_status &= ~MP3_PARSE_VALID_FRAME;
    mp3->mp3_status &= ~MP3_PARSE_FIRST_FRAME;
    mp3->mp3_status &= ~MP3_PARSE_READ_FRAME;
    return ret_sonix;
}

static int32_t _mp3_parse_xing(MP3_FILE_RES_T *mp3)
{
    uint8_t header_buf[5] = {0};
    uint8_t side_information_size;

    if ((mp3->mp3_status & MP3_PARSE_XING) == MP3_PARSE_XING)
    {
        return HAL_OK;
    }

    // if (first frame = xing / info / vbri) // establish seek tab
    // 	discard first frame;
    // else
    // 	save first frame;
    int32_t ret_sonix = HAL_OK;
    ret_sonix         = _mp3_save_frame(mp3);
    CHECK_MIDDLEWARE_RET(ret_sonix);

    MP3_FRAME_INFO_T *header  = &mp3->frame_curr->header;
    uint8_t           ver     = (MP3_MPEG_T)header->sync2.version;
    uint8_t           channel = (header->header.channel == 3) ? MP3_CHANNEL_MONO : MP3_CHANNEL_STEREO;

    /* Get the side information size */
    side_information_size = MP3_side_information_size[ver][channel - 1];

    /* Determine whether the type is VBR or CBR */
    memcpy(header_buf, mp3->frame_save[0].data.buffer + 4 + side_information_size, 4);
    if ((0 == (memcmp(header_buf, "Xing", 4))) || (0 == (memcmp(header_buf, "Info", 4))))
    {
        // VBR - xing
        memcpy(&(mp3->header.xing.xing), mp3->frame_save[0].data.buffer + XING_XING_OFFSET_ADDR + side_information_size, 4);
        memcpy(&mp3->header.xing.flag, mp3->frame_save[0].data.buffer + XING_FLAG_OFFSET_ADDR + side_information_size, 4);
        memcpy(&mp3->header.xing.frame_num, mp3->frame_save[0].data.buffer + XING_FRAME_NUM_OFFSET_ADDR + side_information_size, 4);
        memcpy(&mp3->header.xing.bytes_num, mp3->frame_save[0].data.buffer + XING_BYTES_NUM_OFFSET_ADDR + side_information_size, 4);
    }
    else if ((0 == (memcmp(header_buf, "VBRI", 4))) || (0 == (memcmp(header_buf, "vbri", 4))))
    {
        // VBR - VBRI
        memcpy(&mp3->header.vbri.vbri, mp3->frame_save[0].data.buffer + VBRI_VBRI_OFFSET_ADDR + side_information_size, 4);
        memcpy(&mp3->header.vbri.bytes_num, mp3->frame_save[0].data.buffer + VBRI_BYTES_NUM_OFFSET_ADDR + side_information_size, 2);
        memcpy(&mp3->header.vbri.frame_num, mp3->frame_save[0].data.buffer + VBRI_FRAME_NUM_OFFSET_ADDR + side_information_size, 2);
    }
    else
    {
        // CBR
    }

    mp3->mp3_status |= MP3_PARSE_XING;
    return ret_sonix;
}

static int32_t _mp3_parse_id3v2_size(MP3_FILE_RES_T *mp3)
{
    if ((mp3->mp3_status & MP3_PARSE_ID3V2_SIZE) == MP3_PARSE_ID3V2_SIZE)
    {
        return HAL_OK;
    }
    int32_t ret_sonix = HAL_ERROR;
    ret_sonix         = _mp3_leak_data(mp3, MP3_ID3V2_TAG_LEN + sizeof(MP3_ID3_INFO_T));
    CHECK_MIDDLEWARE_RET(ret_sonix);

    uint32_t id = 0;
    ret_sonix   = _mp3_get_nbyte((uint8_t *)&id, MP3_ID3V2_TAG_LEN, mp3);
    CHECK_MIDDLEWARE_RET(ret_sonix);
    if (id == MP3_ID3V2_TAG)
    {
        MEMORY_ALLOC(mp3->header.id3, sizeof(MP3_ID3_INFO_T), GFP_KERNEL);
        ret_sonix = _mp3_get_nbyte(&mp3->header.id3->version, 1, mp3);
        CHECK_MIDDLEWARE_RET(ret_sonix);
        ret_sonix = _mp3_get_nbyte(&mp3->header.id3->revision, 1, mp3);
        CHECK_MIDDLEWARE_RET(ret_sonix);
        ret_sonix = _mp3_get_nbyte(&mp3->header.id3->flag, 1, mp3);
        CHECK_MIDDLEWARE_RET(ret_sonix);
        ret_sonix = _mp3_get_nbyte((uint8_t *)&mp3->header.id3->size, 4, mp3);
        CHECK_MIDDLEWARE_RET(ret_sonix);
    }
    else
    {
        mp3->parser.buff_pos -= MP3_ID3V2_TAG_LEN;
    }
    mp3->mp3_status |= MP3_PARSE_ID3V2_SIZE;
    return ret_sonix;
}

static int32_t _mp3_parse_id3v2_data(MP3_FILE_RES_T *mp3)
{
    int32_t ret_sonix = HAL_OK;
    if ((mp3->mp3_status & MP3_PARSE_ID3V2_DATA) == MP3_PARSE_ID3V2_DATA)
    {
        return HAL_OK;
    }
    uint32_t id3_size = 0;
    if (mp3->header.id3)
    {
        id3_size = MP3_ID3_INFO_SIZE(mp3->header.id3->size) - 10;
    }

    if (mp3->skip_bytes == 0)
    {
        mp3->skip_bytes = id3_size;
    }
    while (mp3->skip_bytes)
    {
        uint32_t size = mp3->parser.buff_size - mp3->parser.buff_pos;
        if (size >= mp3->skip_bytes || size == 0)
        {
            size = mp3->skip_bytes;
        }
        ret_sonix = _mp3_leak_data(mp3, size);
        CHECK_MIDDLEWARE_RET(ret_sonix);
        ret_sonix = _mp3_get_nbyte(NULL, size, mp3);
        CHECK_MIDDLEWARE_RET(ret_sonix);
        mp3->skip_bytes -= size;
    }

    mp3->mp3_status |= MP3_PARSE_ID3V2_DATA;
    return ret_sonix;
}

static int32_t _mp3_parse_header(MP3_FILE_RES_T *mp3)
{
    int32_t ret_sonix = HAL_OK;

    // ID3V2 info
    ret_sonix = _mp3_parse_id3v2_size(mp3);
    CHECK_MIDDLEWARE_RET(ret_sonix);
    ret_sonix = _mp3_parse_id3v2_data(mp3);
    CHECK_MIDDLEWARE_RET(ret_sonix);

    // save 3 valid frame at beginning
    ret_sonix = _mp3_parse_xing(mp3);
    CHECK_MIDDLEWARE_RET(ret_sonix);
    while (mp3->check_cnt < MP3_VALID_FRAME_CNT)
    {
        ret_sonix = _mp3_save_frame(mp3);
        CHECK_MIDDLEWARE_RET(ret_sonix);
    }

    return ret_sonix;
}

static int32_t _mp3_set_header(MP3_FILE_RES_T *mp3)
{
    return HAL_OK;
}

static int32_t _mp3_read_kernal(MP3_FILE_RES_T *mp3, FILE_FRAME_T *frame)
{
    int32_t ret_sonix = HAL_OK;

    // find valid frame
    ret_sonix = _mp3_valid_frame(mp3);
    CHECK_MIDDLEWARE_RET(ret_sonix);
    // check frame format
    ret_sonix = _mp3_test_frame(mp3);
    if (ret_sonix == HAL_OK)
    {
        // read frame data
        if (frame->length < mp3->head_len + mp3->data_len)
        {
            return MP3_MSG_LACK_BUFF;
        }
        ret_sonix = _mp3_read_frame(mp3, (uint32_t *)&frame->buffer);
        CHECK_MIDDLEWARE_RET(ret_sonix);
        frame->length = mp3->head_len + mp3->data_len;
        mp3->mp3_status &= ~MP3_PARSE_VALID_FRAME;
        mp3->mp3_status &= ~MP3_PARSE_READ_FRAME;
    }
    else
    {
        // discard frame data
        ret_sonix = _mp3_read_frame(mp3, NULL);
        CHECK_MIDDLEWARE_RET(ret_sonix);
        mp3->mp3_status &= ~MP3_PARSE_VALID_FRAME;
        mp3->mp3_status &= ~MP3_PARSE_READ_FRAME;
        return MP3_MSG_INVALID_FRAME;
    }

    return HAL_OK;
}

////////////////////////////////////////////////////////////////////////////////////////
static int32_t _mp3_open(MP3_FILE_RES_T *mp3)
{
    int32_t ret_sonix = HAL_ERROR;
    if ((mp3->mode & FA_READ) == FA_READ)
    {
        if (mp3->src == FILE_SRC_FROM_FATFS)
        {
#if MP3_USE_FATFS
            ret_sonix = _file_seek(mp3->fp, 0);
            CHECK_MIDDLEWARE_RET(ret_sonix);
#endif
        }

        ret_sonix = _mp3_fill_parser(mp3);
        while (mp3->is_eof == FILE_IS_NOT_EOF)
        {
            ret_sonix = _mp3_parse_header(mp3);
            if (ret_sonix == MP3_MSG_LACK_DATA)
            {
                ret_sonix = _mp3_fill_parser(mp3);
                CHECK_MIDDLEWARE_RET(ret_sonix);
                continue;
            }
            CHECK_MIDDLEWARE_RET(ret_sonix);
            break;
        }
    }
    if ((mp3->mode & FA_WRITE) == FA_WRITE)
    {
        ret_sonix = _mp3_set_header(mp3);
        CHECK_MIDDLEWARE_RET(ret_sonix);
        ret_sonix = _mp3_seek_and_write(0, sizeof(MP3_FILE_HEADER_T), &mp3->header, mp3);
        CHECK_MIDDLEWARE_RET(ret_sonix);
    }
    return HAL_OK;
}

static int32_t _mp3_close(MP3_FILE_RES_T *mp3)
{
    for (uint8_t index = 0; index < MP3_VALID_FRAME_CNT; index++)
    {
        MEMORY_FREE(mp3->frame_save[index].data.buffer);
    }
    _mp3_free_parser(mp3);
    MEMORY_FREE(mp3->header.id3);
    MEMORY_FREE(mp3->frame_keep);
    MEMORY_FREE(mp3->frame_curr);
    return HAL_OK;
}

static int32_t _mp3_read(MP3_FILE_RES_T *mp3, FILE_FRAME_T *frame, uint32_t is_blocking)
{
    if (mp3->check_cnt != 0)
    {
        uint8_t index = MP3_VALID_FRAME_CNT - mp3->check_cnt;
        if (frame->length < mp3->frame_save[index].data.length)
        {
            mp3->data_len = mp3->frame_save[index].data.length;
            mp3->head_len = 0;
            return MP3_MSG_LACK_BUFF;
        }
        frame->length = mp3->frame_save[index].data.length;
        memcpy(frame->buffer, mp3->frame_save[index].data.buffer, frame->length);
        memset(mp3->frame_curr, 0, sizeof(MP3_FRAME_HEADER_T));
        memcpy(mp3->frame_curr, (uint8_t *)mp3->frame_save[index].data.buffer, mp3->frame_save[index].head_len);
        MEMORY_FREE(mp3->frame_save[index].data.buffer);
        mp3->frame_save[index].data.length = 0;
        mp3->frame_idx++;
        mp3->check_cnt--;
        return HAL_OK;
    }

    int32_t ret_sonix = HAL_OK;
    while (mp3->is_eof == FILE_IS_NOT_EOF)
    {
        ret_sonix = _mp3_read_kernal(mp3, frame);
        if (ret_sonix == MP3_MSG_LACK_DATA)
        {
            ret_sonix = _mp3_fill_parser(mp3);
            CHECK_MIDDLEWARE_RET(ret_sonix);
            if (mp3->is_eof == FILE_IS_EOF)
            {
                frame->length = 0;
            }
            continue;
        }
        else if (ret_sonix == MP3_MSG_INVALID_FRAME)
        {
            continue;
        }
        CHECK_MIDDLEWARE_RET(ret_sonix);
        mp3->frame_idx++;
        break;
    }

    // update info
    return HAL_OK;
}

static int32_t _mp3_write(MP3_FILE_RES_T *mp3, const FILE_FRAME_T *frame, uint32_t is_blocking)
{
    uint64_t seek = FILE_NOT_SEEK;
    if (mp3->frame_idx == 0)
    {
        seek = 0;
    }
    int32_t ret_sonix = _mp3_seek_and_write(seek, frame->length, frame->buffer, mp3);
    CHECK_MIDDLEWARE_RET(ret_sonix);
    mp3->frame_idx++;
    return HAL_OK;
}

static int32_t _mp3_seek(MP3_FILE_RES_T *mp3, const MP3_SEEK_T *seek)
{
#if MP3_USE_FATFS
    if (seek->seek_ratio > 100)
    {
        return HAL_ERROR;
    }
    if (seek->seek_ratio != 0)
    {
        mp3->check_cnt = 0;
    }

    _mp3_free_parser(mp3);
    mp3->file_pos  = seek->seek_ratio * _mp3_f_size(mp3) / 100;
    mp3->frame_idx = mp3->file_pos / (mp3->head_len + mp3->data_len);
    mp3->is_eof    = mp3->file_pos < _mp3_f_size(mp3) ? FILE_IS_NOT_EOF : FILE_IS_EOF;
    return _file_seek(mp3->fp, mp3->file_pos);
#else
    return HAL_ERROR;
#endif
}

static int32_t _mp3_wait(MP3_FILE_RES_T *mp3)
{
    return HAL_ERROR;
}

static int32_t _mp3_get_status(MP3_FILE_RES_T *mp3, MP3_STATUS_T *status)
{
    MP3_FRAME_INFO_T *header   = &mp3->frame_curr->header;
    uint8_t           layer    = 3 - header->sync2.layer;
    uint8_t           ver      = header->sync2.version;
    uint8_t           sample   = header->header.sample_rate_index;
    uint8_t           bit      = header->header.bit_rate_index;
    uint8_t          *id3_size = mp3->header.id3->size;

    status->layer        = (MP3_LAYER_T)layer;
    status->ver          = (MP3_MPEG_T)ver;
    status->bit_rate     = MP3_bit_rate[ver & 0x1][layer][bit];
    status->channel      = (header->header.channel == 3) ? MP3_CHANNEL_MONO : MP3_CHANNEL_STEREO;
    status->sample_rate  = MP3_sample_rate[sample][ver];
    status->is_eof       = mp3->is_eof;
    status->sample_point = MP3_sample_point[layer][ver];
    status->ratio_pos    = mp3->file_pos * 100 / _mp3_f_size(mp3);
    status->id3_size     = (id3_size[0] & 0x7F) * 0x200000 + (id3_size[1] & 0x7F) * 0x400 + (id3_size[2] & 0x7F) * 0x80 + (id3_size[3] & 0x7F) + 128;

    if (0 != mp3->header.xing.xing) // vbr - xing
    {
        status->frame_num_vbr = L2B32(mp3->header.xing.frame_num);
    }
    else if (0 != mp3->header.vbri.vbri) // vbr - vbri
    {
        status->frame_num_vbr = L2B32(mp3->header.vbri.frame_num);
    }

    return HAL_OK;
}

///////////////////////////////////////////////////////////////////////////////////////
static int32_t MP3_Open(const void *mp3)
{
    CHECK_POINTER_NULL(mp3);
    CHECK_POINTER_NULL(((FIL_RES_T *)mp3)->res);
    return _mp3_open(((FIL_RES_T *)mp3)->res);
};
static int32_t MP3_Close(const void *mp3)
{
    CHECK_POINTER_NULL(mp3);
    // CHECK_POINTER_NULL(((FIL_RES_T *)mp3)->res);
    if (((FIL_RES_T *)mp3)->res == NULL)
    { // close return ok to continue other close flow
        return HAL_OK;
    }
    return _mp3_close(((FIL_RES_T *)mp3)->res);
};
static int32_t MP3_Read(const void *mp3, void *frame, uint8_t is_blocking)
{
    CHECK_POINTER_NULL(mp3);
    CHECK_POINTER_NULL(((FIL_RES_T *)mp3)->res);
    CHECK_POINTER_NULL(frame);
    return _mp3_read(((FIL_RES_T *)mp3)->res, frame, is_blocking);
};
static int32_t MP3_Write(const void *mp3, const void *frame, uint8_t is_blocking)
{
    CHECK_POINTER_NULL(mp3);
    CHECK_POINTER_NULL(((FIL_RES_T *)mp3)->res);
    CHECK_POINTER_NULL(frame);
    return _mp3_write(((FIL_RES_T *)mp3)->res, frame, is_blocking);
};
static int32_t MP3_Seek(const void *mp3, const void *seek)
{
    CHECK_POINTER_NULL(mp3);
    CHECK_POINTER_NULL(((FIL_RES_T *)mp3)->res);
    CHECK_POINTER_NULL(seek);
    return _mp3_seek(((FIL_RES_T *)mp3)->res, seek);
};
static int32_t MP3_Wait(const void *mp3)
{
    CHECK_POINTER_NULL(mp3);
    CHECK_POINTER_NULL(((FIL_RES_T *)mp3)->res);
    return _mp3_wait(((FIL_RES_T *)mp3)->res);
};
static int32_t MP3_GetStatus(const void *mp3, void *status)
{
    CHECK_POINTER_NULL(mp3);
    CHECK_POINTER_NULL(((FIL_RES_T *)mp3)->res);
    CHECK_POINTER_NULL(status);
    return _mp3_get_status(((FIL_RES_T *)mp3)->res, status);
};
TYPE_WRAPPER MP3_Wrapper = {
    MP3_Open,
    MP3_Close,
    MP3_Read,
    MP3_Write,
    MP3_Wait,
    MP3_Seek,
    MP3_GetStatus,
};

////////////////////////////////////////////////////////////////////////////////////
static int32_t MP3_Parser_Init(MP3_HANDLE_T *buf_fh)
{
    CHECK_POINTER_NULL(buf_fh);

    MP3_FILE_RES_T *mp3_res = buf_fh->other_res;
    MEMORY_ALLOC(mp3_res, sizeof(MP3_FILE_RES_T), GFP_KERNEL);
    if ((mp3_res->mp3_status & MP3_PARSE_INIT) == MP3_PARSE_INIT)
    {
        return HAL_OK;
    }

    buf_fh->is_eof    = FILE_IS_NOT_EOF;
    buf_fh->last_buff = 0;
    buf_fh->buff_addr = 0;
    buf_fh->buff_size = 0;
    buf_fh->other_res = mp3_res;

    mp3_res->mode     = FA_READ;
    mp3_res->src      = FILE_SRC_FROM_BUFF;
    mp3_res->file_siz = 0x7FFFFFFF;
    mp3_res->mp3_status |= MP3_PARSE_INIT;
    return HAL_OK;
}
static int32_t MP3_Parser_Read(MP3_HANDLE_T *buf_fh, MP3_FRAME_T *frame_out)
{
    CHECK_POINTER_NULL(buf_fh);
    CHECK_POINTER_NULL(buf_fh->other_res);
    if (buf_fh->buff_addr == NULL || buf_fh->buff_size == 0)
    {
        return MP3_MSG_LACK_DATA;
    }

    // check init & fill parser buffer
    int32_t         ret_sonix = HAL_OK;
    MP3_FILE_RES_T *mp3_res   = buf_fh->other_res;
    if ((mp3_res->is_eof == FILE_IS_EOF) || (mp3_res->mp3_status & MP3_PARSE_INIT) != MP3_PARSE_INIT)
    {
        return HAL_ERROR;
    }

    ret_sonix = _mp3_fill_buffer(mp3_res, (uint8_t *)buf_fh->buff_addr, buf_fh->buff_size);
    CHECK_MIDDLEWARE_RET(ret_sonix);
    if (buf_fh->last_buff == 1)
    {
        mp3_res->file_siz = mp3_res->file_pos;
    }

    // find first frame & save 3 valid frame
    if ((mp3_res->mp3_status & MP3_PARSE_FIND_FIRST) != MP3_PARSE_FIND_FIRST)
    {
        ret_sonix = _mp3_parse_header(mp3_res);
        CHECK_MIDDLEWARE_RET(ret_sonix);
        mp3_res->mp3_status |= MP3_PARSE_FIND_FIRST;
    }

    // read a valid frame
    FILE_FRAME_T frame_tmp = {0};
    frame_tmp.buffer       = frame_out->cache;
    frame_tmp.length       = frame_out->cache_len;
    ret_sonix              = _mp3_read(mp3_res, &frame_tmp, FILE_IS_BLOCKING);
    buf_fh->frame_len      = mp3_res->data_len + mp3_res->head_len;
    if (buf_fh->last_buff == 1 && ret_sonix == MP3_MSG_LACK_DATA)
    {
        buf_fh->is_eof = FILE_IS_EOF;
        ret_sonix      = HAL_ERROR;
    }
    CHECK_MIDDLEWARE_RET(ret_sonix);
    frame_out->data     = frame_tmp.buffer;
    frame_out->data_len = frame_tmp.length;
    ret_sonix           = _mp3_get_status(mp3_res, &frame_out->status);
    CHECK_MIDDLEWARE_RET(ret_sonix);

    buf_fh->is_eof = mp3_res->is_eof;
    return HAL_OK;
}
static int32_t MP3_Parser_Uninit(MP3_HANDLE_T *buf_fh)
{
    MP3_FILE_RES_T *mp3_res = buf_fh->other_res;
    if (mp3_res)
    {
        _mp3_close(mp3_res);
        buf_fh->is_eof    = FILE_IS_NOT_EOF;
        buf_fh->last_buff = 0;
        buf_fh->buff_addr = 0;
        buf_fh->buff_size = 0;
        MEMORY_FREE(buf_fh->other_res);
    }
    return HAL_OK;
}
MP3_PARSER MP3_Parser = {
    MP3_Parser_Init,
    MP3_Parser_Read,
    MP3_Parser_Uninit,
};

////////////////////////////////////////////////////////////////////////////////////
static int32_t MP3_DoInit(void *sh)
{
    int32_t ret_sonix = HAL_ERROR;
#if MP3_USE_STREAM
    CHECK_POINTER_NULL(sh);
    ret_sonix = MP3_Parser_Init(sh);
    CHECK_MIDDLEWARE_RET(ret_sonix);
#endif
    return ret_sonix;
}
static int32_t MP3_DeInit(void *sh)
{
    int32_t ret_sonix = HAL_ERROR;
#if MP3_USE_STREAM
    CHECK_POINTER_NULL(sh);

    MP3_FILE_RES_T *mp3_res = ((MP3_HANDLE_T *)sh)->other_res;
    if (mp3_res->sfp->init == 1)
    {
        mp3_res->sfp->init = 0;
    }
    MEMORY_FREE(mp3_res->sfp);
    ret_sonix = MP3_Parser_Uninit(sh);
    CHECK_MIDDLEWARE_RET(ret_sonix);
#endif
    return ret_sonix;
}
static int32_t MP3_ReadDev(void *sh, void *info)
{
    int32_t ret_sonix = HAL_ERROR;
#if MP3_USE_STREAM
    CHECK_POINTER_NULL(sh);
    CHECK_POINTER_NULL(info);
    MP3_FILE_RES_T *mp3_res = ((MP3_HANDLE_T *)sh)->other_res;
    if (mp3_res->sfp->beg_addr == 0 ||
        mp3_res->sfp->end_addr == 0 ||
        mp3_res->sfp->dev == (uint8_t)DEV_NULL)
    {
        return HAL_ERROR;
    }

    if (mp3_res->sfp->init == 0)
    {
        mp3_res->sfp->init = 1;
    }

    FILE_FRAME_T *frame_tmp = info;
    uint32_t      size      = mp3_res->sfp->end_addr - mp3_res->sfp->cur_addr;
    if (size > frame_tmp->length)
    {
        size = frame_tmp->length;
    }
    else
    {
        mp3_res->is_eof = FILE_IS_EOF;
    }
    ret_sonix = StgWrapper.ReadByte(mp3_res->sfp->dev, frame_tmp->buffer, mp3_res->sfp->cur_addr, frame_tmp->length);
    CHECK_MIDDLEWARE_RET(ret_sonix);
    mp3_res->sfp->cur_addr += size;
    frame_tmp->length = size;
#endif
    return ret_sonix;
}
static int32_t MP3_GetFrame(void *sh, void *frame)
{
    int32_t ret_sonix = HAL_ERROR;
#if MP3_USE_STREAM
    CHECK_POINTER_NULL(sh);
    CHECK_POINTER_NULL(frame);
    ret_sonix = MP3_Parser_Read(sh, frame);
    CHECK_MIDDLEWARE_RET(ret_sonix);
#endif
    return ret_sonix;
}
PARSE_WRAPPER MP3_Stream = {
    MP3_DoInit,
    MP3_DeInit,
    MP3_ReadDev,
    MP3_GetFrame,
};
