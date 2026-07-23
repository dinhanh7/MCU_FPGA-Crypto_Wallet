/***********************
 * SN34F78X SDK V2.1.4 *
 ***********************/

#include "jpg_wrapper.h"

//////////////////////////////////////////////////////////////////////////////////////////////

static int32_t _jpg_parser_header(JPG_FILE_RES_T *jpg)
{
    int32_t ret_sonix = HAL_OK;

    jpg->head_size            = 0;
    uint32_t segment_head_len = 2;
    uint32_t segment_head     = 0;
    ret_sonix                 = _file_seek_and_read(FILE_NOT_SEEK, &segment_head_len, &segment_head, jpg->fp);
    MFILE_DEBUG_ERROR(ret_sonix != HAL_OK, "Get jpg format fail");
    CHECK_MIDDLEWARE_RET(ret_sonix);

    segment_head = ((segment_head & 0xFF) << 8) | (segment_head >> 8);
    if (HAL_FLAG_NEQ(segment_head, JPG_TYPE))
    {
        MFILE_DEBUG_ERROR(HAL_FLAG_NEQ(jpg_format, BMP_TYPE), "The file is not JPG format");
        return HAL_ERROR;
    }
    segment_head_len = 4;
    jpg->head_size += 2;

    /* Collect SOF information only once */
    uint8_t collected_SOF = 0;

    while (1)
    {
        ret_sonix = _file_seek_and_read(jpg->head_size, &segment_head_len, &segment_head, jpg->fp);
        MFILE_DEBUG_ERROR(ret_sonix != HAL_OK, "Get jpg information fail");
        CHECK_MIDDLEWARE_RET(ret_sonix);

        uint32_t segment_format = segment_head & 0xFFFF;

        segment_format = ((segment_format & 0xFF) << 8) | (segment_format >> 8);

        uint32_t segment_size = segment_head >> 16;

        segment_size = (((segment_size & 0xFF) << 8) | (segment_size >> 8)) - 2;

        jpg->head_size += 4;

        if (HAL_FLAG_EQU(segment_format & 0xFFF0, JPG_SOF0) && (HAL_FLAG_NEQ(segment_format, JPG_DHT) && HAL_FLAG_NEQ(segment_format, JPG_DAC)))
        {
            if (collected_SOF == 0)
            {
                uint8_t *jpg_sofn_buf = NULL;
                MEMORY_ALLOC(jpg_sofn_buf, segment_size, GFP_SRAM);
                ret_sonix = _file_seek_and_read(jpg->head_size, &segment_size, jpg_sofn_buf, jpg->fp);
                MFILE_DEBUG_ERROR(ret_sonix != HAL_OK, "Get jpg information fail");
                CHECK_MIDDLEWARE_RET(ret_sonix);
                jpg->head.height      = jpg_sofn_buf[1] << 8 | jpg_sofn_buf[2];
                jpg->head.width       = jpg_sofn_buf[3] << 8 | jpg_sofn_buf[4];
                uint8_t  compsinframe = jpg_sofn_buf[5];
                uint16_t ver_samp     = jpg_sofn_buf[7] & 0xF;
                uint16_t hor_samp     = jpg_sofn_buf[7] >> 4;
                MEMORY_FREE(jpg_sofn_buf);
                if (compsinframe == 1)
                {
                    jpg->head.samp_format = JPG_GRAYSCALE;
                }
                else if (compsinframe == 3)
                {
                    if (hor_samp == 1 && ver_samp == 1)
                    {
                        jpg->head.samp_format = JPG_YH1V1;
                    }
                    else if (hor_samp == 2 && ver_samp == 1)
                    {
                        jpg->head.samp_format = JPG_YH2V1;
                    }
                    else if (hor_samp == 1 && ver_samp == 2)
                    {
                        jpg->head.samp_format = JPG_YH1V2;
                    }
                    else if (hor_samp == 2 && ver_samp == 2)
                    {
                        jpg->head.samp_format = JPG_YH2V2;
                    }
                    else
                    {
                        return HAL_ERROR;
                    }
                }
                else
                {
                    return HAL_ERROR;
                }
                collected_SOF = 1;
            }
        }
        else if (HAL_FLAG_EQU(segment_format, JPG_APP0))
        {
            uint8_t *jpg_app0_buf = NULL;
            MEMORY_ALLOC(jpg_app0_buf, segment_size, GFP_SRAM);
            ret_sonix = _file_seek_and_read(jpg->head_size, &segment_size, jpg_app0_buf, jpg->fp);
            MFILE_DEBUG_ERROR(ret_sonix != HAL_OK, "Get jpg information fail");
            CHECK_MIDDLEWARE_RET(ret_sonix);
            uint8_t unit = jpg_app0_buf[7];

            jpg->head.x_pels_perinch = (jpg_app0_buf[8] << 8) | jpg_app0_buf[9];
            jpg->head.y_pels_perinch = (jpg_app0_buf[10] << 8) | jpg_app0_buf[11];
            if (unit == 2)
            {
                jpg->head.x_pels_perinch = jpg->head.x_pels_perinch * 0.3937;
                jpg->head.y_pels_perinch = jpg->head.y_pels_perinch * 0.3937;
            }

            MEMORY_FREE(jpg_app0_buf);
        }

        jpg->head_size += segment_size;

        if (HAL_FLAG_EQU(segment_format, JPG_SOS))
        {
            break;
        }
    }
    MEMORY_ALLOC(jpg->head_data, jpg->head_size, GFP_SRAM);
    memset(jpg->head_data, 0, jpg->head_size);
    ret_sonix = _file_seek_and_read(0, &jpg->head_size, jpg->head_data, jpg->fp);
    return HAL_OK;
}

static int32_t _jpg_set_header_data(JPG_FILE_RES_T *jpg)
{
    int32_t ret_sonix = HAL_OK;
    jpg->head_size    = jpg->info.head_size;
    MEMORY_ALLOC(jpg->head_data, jpg->head_size, GFP_SRAM);
    memset(jpg->head_data, 0, jpg->head_size);
    memset(&jpg->head, 0, sizeof(JPG_INFO_HEADER_T));
    memcpy(jpg->head_data, jpg->info.head_data, jpg->info.head_size);

    if (HAL_FLAG_NEQ((jpg->head_data[0] << 8 | jpg->head_data[1]), JPG_SOI))
    {
        MFILE_DEBUG_ERROR(HAL_FLAG_NEQ(jpg_format, BMP_TYPE), "The file is not JPG format");
        return HAL_ERROR;
    }

    /* Write header data */
    ret_sonix = _file_seek_and_write(FILE_NOT_SEEK, jpg->head_size, jpg->head_data, jpg->fp);
    CHECK_MIDDLEWARE_RET(ret_sonix);

    /* Collect SOF information only once */
    uint8_t collected_SOF = 0;
    for (uint32_t i = 2; i < jpg->head_size;)
    {
        uint32_t segment_format = jpg->head_data[i] << 8 | jpg->head_data[i + 1];
        uint32_t segment_size   = jpg->head_data[i + 2] << 8 | jpg->head_data[i + 3];
        switch (segment_format)
        {
            case JPG_SOF0:
            case JPG_SOF1:
            case JPG_SOF2:
            case JPG_SOF3:
            case JPG_SOF5:
            case JPG_SOF6:
            case JPG_SOF7:
            case JPG_JPG:
            case JPG_SOF9:
            case JPG_SOF10:
            case JPG_SOF11:
            case JPG_SOF13:
            case JPG_SOF14:
            case JPG_SOF15:
            {
                i += 4;
                if (collected_SOF == 0)
                {
                    jpg->head.height      = jpg->head_data[i + 1] << 8 | jpg->head_data[i + 2];
                    jpg->head.width       = jpg->head_data[i + 3] << 8 | jpg->head_data[i + 4];
                    uint8_t  compsinframe = jpg->head_data[i + 5];
                    uint16_t ver_samp     = jpg->head_data[i + 7] & 0xF;
                    uint16_t hor_samp     = jpg->head_data[i + 7] >> 4;
                    if (compsinframe == 1)
                    {
                        jpg->head.samp_format = JPG_GRAYSCALE;
                    }
                    else if (compsinframe == 3)
                    {
                        if (hor_samp == 1 && ver_samp == 1)
                        {
                            jpg->head.samp_format = JPG_YH1V1;
                        }
                        else if (hor_samp == 2 && ver_samp == 1)
                        {
                            jpg->head.samp_format = JPG_YH2V1;
                        }
                        else if (hor_samp == 1 && ver_samp == 2)
                        {
                            jpg->head.samp_format = JPG_YH1V2;
                        }
                        else if (hor_samp == 2 && ver_samp == 2)
                        {
                            jpg->head.samp_format = JPG_YH2V2;
                        }
                        else
                        {
                            return HAL_ERROR;
                        }
                    }
                    else
                    {
                        return HAL_ERROR;
                    }
                }

                collected_SOF = 1;

                i += segment_size - 2;
                break;
            }
            case JPG_APP0:
            case JPG_APP15:
            {
                i += 4;
                uint8_t unit = jpg->head_data[i + 7];

                jpg->head.x_pels_perinch = (jpg->head_data[i + 8] << 8) | jpg->head_data[i + 9];
                jpg->head.y_pels_perinch = (jpg->head_data[i + 10] << 8) | jpg->head_data[i + 11];
                if (unit == 2)
                {
                    jpg->head.x_pels_perinch = jpg->head.x_pels_perinch * 0.3937;
                    jpg->head.y_pels_perinch = jpg->head.y_pels_perinch * 0.3937;
                }
                i += segment_size - 2;
                break;
            }
            case JPG_DHT:
            case JPG_DAC:
            case JPG_RST0:
            case JPG_RST1:
            case JPG_RST2:
            case JPG_RST3:
            case JPG_RST4:
            case JPG_RST5:
            case JPG_RST6:
            case JPG_RST7:
            case JPG_SOI:
            case JPG_EOI:
            case JPG_SOS:
            case JPG_DQT:
            case JPG_DNL:
            case JPG_DRI:
            case JPG_DHP:
            case JPG_EXP:
            case JPG_JPG0:
            case JPG_JPG13:
            case JPG_COM:
            case JPG_TEM:
            {
                i += 4;
                i += segment_size - 2;
                break;
            }
            default:
                i += 2;
        }
    }

    return ret_sonix;
}

static int32_t _jpg_open(JPG_FILE_RES_T *jpg)
{
    int32_t ret_sonix = HAL_OK;
    if (HAL_FLAG_HAS(jpg->mode, FA_READ))
    {
        ret_sonix = _jpg_parser_header(jpg);
        CHECK_MIDDLEWARE_RET(ret_sonix);
    }
    else if (HAL_FLAG_HAS(jpg->mode, FA_WRITE))
    {
        ret_sonix = _jpg_set_header_data(jpg);
        CHECK_MIDDLEWARE_RET(ret_sonix);
    }

    ret_sonix = f_lseek(jpg->fp, jpg->head_size);
    MFILE_DEBUG_ERROR(ret_sonix != HAL_OK, "Seek fail after get information header");
    CHECK_MIDDLEWARE_RET(ret_sonix);

    return HAL_OK;
}

static int32_t _jpg_close(JPG_FILE_RES_T *jpg)
{
    int32_t ret_sonix = HAL_OK;
    if (jpg->head_data != NULL)
    {
        MEMORY_FREE(jpg->head_data);
    }
    if (HAL_FLAG_HAS(jpg->mode, FA_WRITE))
    {
        /* Write header data */
        uint32_t size    = f_size(jpg->fp);
        uint16_t jpg_eoi = JPG_EOI;
        jpg_eoi          = ((jpg_eoi & 0xFF) << 8) | (jpg_eoi >> 8);
        ret_sonix        = _file_seek_and_write(size, 2, &jpg_eoi, jpg->fp);
        CHECK_MIDDLEWARE_RET(ret_sonix);
    }

    return ret_sonix;
}

static int32_t _jpg_read(JPG_FILE_RES_T *jpg, FILE_FRAME_T *frame, uint32_t is_blocking)
{
    uint64_t seek      = FILE_NOT_SEEK;
    int32_t  ret_sonix = _file_seek_and_read(seek, &frame->length, frame->buffer, jpg->fp);
    CHECK_MIDDLEWARE_RET(ret_sonix);
    return HAL_OK;
}

static int32_t _jpg_write(JPG_FILE_RES_T *jpg, const FILE_FRAME_T *frame, uint32_t is_blocking)
{
    uint64_t seek      = FILE_NOT_SEEK;
    int32_t  ret_sonix = _file_seek_and_write(seek, frame->length, frame->buffer, jpg->fp);
    CHECK_MIDDLEWARE_RET(ret_sonix);
    return HAL_OK;
}

static int32_t _jpg_seek(JPG_FILE_RES_T *jpg, const JPG_SEEK_T *seek)
{
    if (HAL_FLAG_HAS(jpg->mode, FA_READ))
    {
        if (seek->pos >= (f_size(jpg->fp) - jpg->head_size))
        {
            return HAL_ERROR;
        }
    }

    FRESULT  result = FR_OK;
    uint32_t pos    = seek->pos + jpg->head_size;
    result          = f_lseek(jpg->fp, pos);
    if (result != FR_OK)
    {
        return HAL_ERROR;
    }
    return HAL_OK;
}

static int32_t _jpg_wait(JPG_FILE_RES_T *wav)
{
    return HAL_OK;
}

static int32_t _jpg_get_status(JPG_FILE_RES_T *jpg, JPG_STATUS_T *status)
{
    status->pos            = f_tell(jpg->fp) - jpg->head_size;
    status->size           = f_size(jpg->fp);
    status->width          = jpg->head.width;
    status->height         = jpg->head.height;
    status->head_data      = jpg->head_data;
    status->head_size      = jpg->head_size;
    status->x_pels_perinch = jpg->head.x_pels_perinch;
    status->y_pels_perinch = jpg->head.y_pels_perinch;
    status->is_eof         = 0;
    status->samp_format    = jpg->head.samp_format;
    if (status->size <= (status->pos + jpg->head_size))
    {
        status->is_eof = 1;
    }
    return HAL_OK;
}

static int32_t JPG_Open(const void *jpg)
{
    CHECK_POINTER_NULL(jpg);
    CHECK_POINTER_NULL(((FIL_RES_T *)jpg)->res);
    return _jpg_open(((FIL_RES_T *)jpg)->res);
}

static int32_t JPG_Close(const void *jpg)
{
    CHECK_POINTER_NULL(jpg);
    if (((FIL_RES_T *)jpg)->res == NULL)
    {
        // close return ok to continue other close flow
        return HAL_OK;
    }
    return _jpg_close(((FIL_RES_T *)jpg)->res);
}

static int32_t JPG_Read(const void *jpg, void *frame, uint8_t is_blocking)
{
    CHECK_POINTER_NULL(jpg);
    CHECK_POINTER_NULL(((FIL_RES_T *)jpg)->res);
    CHECK_POINTER_NULL(frame);
    return _jpg_read(((FIL_RES_T *)jpg)->res, frame, is_blocking);
}

static int32_t JPG_Write(const void *jpg, const void *frame, uint8_t is_blocking)
{
    CHECK_POINTER_NULL(jpg);
    CHECK_POINTER_NULL(((FIL_RES_T *)jpg)->res);
    CHECK_POINTER_NULL(frame);
    return _jpg_write(((FIL_RES_T *)jpg)->res, frame, is_blocking);
}

static int32_t JPG_Seek(const void *jpg, const void *seek)
{
    CHECK_POINTER_NULL(jpg);
    CHECK_POINTER_NULL(((FIL_RES_T *)jpg)->res);
    CHECK_POINTER_NULL(seek);
    return _jpg_seek(((FIL_RES_T *)jpg)->res, seek);
}

static int32_t JPG_Wait(const void *jpg)
{
    CHECK_POINTER_NULL(jpg);
    CHECK_POINTER_NULL(((FIL_RES_T *)jpg)->res);
    return _jpg_wait(((FIL_RES_T *)jpg)->res);
}

static int32_t JPG_GetStatus(const void *jpg, void *status)
{
    CHECK_POINTER_NULL(jpg);
    CHECK_POINTER_NULL(((FIL_RES_T *)jpg)->res);
    CHECK_POINTER_NULL(status);
    return _jpg_get_status(((FIL_RES_T *)jpg)->res, status);
}

TYPE_WRAPPER JPG_Wrapper = {
    JPG_Open,
    JPG_Close,
    JPG_Read,
    JPG_Write,
    JPG_Wait,
    JPG_Seek,
    JPG_GetStatus,
};
