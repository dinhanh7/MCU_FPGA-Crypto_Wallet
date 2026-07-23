/***********************
 * SN34F78X SDK V2.1.4 *
 ***********************/

#include "bmp_wrapper.h"

//////////////////////////////////////////////////////////////////////////////////////////////

static int32_t _bmp_parser_file_header(BMP_FILE_RES_T *bmp)
{
    int32_t  ret_sonix        = HAL_OK;
    uint32_t file_header_size = BMP_FILE_HEADER_SIZE;
    memset(&bmp->file_header, 0, sizeof(BMP_FILE_HEADER_T));
    uint8_t *buffer = NULL;
    MEMORY_ALLOC(buffer, file_header_size, GFP_SRAM);
    ret_sonix                 = _file_seek_and_read(FILE_NOT_SEEK, &file_header_size, buffer, bmp->fp);
    bmp->file_header.type     = buffer[1] << 8 | buffer[0];
    bmp->file_header.size     = buffer[5] << 24 | buffer[4] << 16 | buffer[3] << 8 | buffer[2];
    bmp->file_header.off_bits = buffer[13] << 24 | buffer[12] << 16 | buffer[11] << 8 | buffer[10];
    MEMORY_FREE(buffer);

    if (HAL_FLAG_NEQ(bmp->file_header.type, BMP_TYPE))
    {
        MFILE_DEBUG_ERROR(HAL_FLAG_NEQ(bmp->file_header.type, BMP_TYPE), "The file is not BMP format");
        return HAL_ERROR;
    }

    MFILE_DEBUG_ERROR(ret_sonix != HAL_OK, "Get information header fail");
    CHECK_MIDDLEWARE_RET(ret_sonix);

    return HAL_OK;
}

static int32_t _bmp_parser_info_header(BMP_FILE_RES_T *bmp)
{
    int32_t  ret_sonix        = HAL_OK;
    uint32_t info_header_size = BMP_INFO_HEADER_SIZE;
    ret_sonix                 = _file_seek_and_read(BMP_FILE_HEADER_SIZE, &info_header_size, &bmp->info_header, bmp->fp);

    MFILE_DEBUG_ERROR(ret_sonix != HAL_OK, "Get information header fail");
    CHECK_MIDDLEWARE_RET(ret_sonix);

    bmp->palette_size = bmp->info_header.size - BMP_INFO_HEADER_SIZE;
    if (bmp->palette_size != 0)
    {
        MEMORY_ALLOC(bmp->palette_data, bmp->palette_size, GFP_SRAM);
        ret_sonix = _file_seek_and_read(BMP_FILE_HEADER_SIZE + BMP_INFO_HEADER_SIZE, &bmp->palette_size, bmp->palette_data, bmp->fp);
        CHECK_MIDDLEWARE_RET(ret_sonix);
    }

    return HAL_OK;
}

static int32_t _bmp_set_file_header(BMP_FILE_RES_T *bmp)
{
    memset(&bmp->file_header, 0, sizeof(BMP_FILE_HEADER_T));
    BMP_INFO_T *info          = &bmp->info;
    bmp->file_header.type     = BMP_TYPE;
    bmp->file_header.off_bits = BMP_FILE_HEADER_SIZE + BMP_INFO_HEADER_SIZE + info->palette_size;
    bmp->file_header.size     = info->bmp_width * info->bmp_height * (info->bit_count / 8) + bmp->file_header.off_bits;

    return HAL_OK;
}

static int32_t _bmp_set_info_header(BMP_FILE_RES_T *bmp)
{
    BMP_INFO_T *info                 = &bmp->info;
    bmp->info_header.size            = BMP_INFO_HEADER_SIZE + info->palette_size;
    bmp->info_header.height          = info->bmp_height;
    bmp->info_header.width           = info->bmp_width;
    bmp->info_header.planes          = 1;
    bmp->info_header.bit_count       = info->bit_count;
    bmp->info_header.compression     = info->compression;
    bmp->info_header.size_image      = info->bmp_width * info->bmp_height * (info->bit_count / 8);
    bmp->info_header.x_pels_permeter = info->x_pels_permeter;
    bmp->info_header.y_pels_permeter = info->y_pels_permeter;
    bmp->info_header.clr_used        = 0;
    bmp->info_header.clr_important   = 0;

    if ((bmp->info_header.bit_count == BIT_COUNT_8) || ((bmp->info_header.bit_count == BIT_COUNT_16)))
    {
        bmp->palette_size = bmp->info.palette_size;
        if (bmp->palette_size != 0)
        {
            MEMORY_ALLOC(bmp->palette_data, bmp->palette_size, GFP_SRAM);
            memcpy(bmp->palette_data, bmp->info.palette_data, bmp->palette_size);
        }
    }
    else
    {
        bmp->palette_size = 0;
        bmp->palette_data = NULL;
    }

    return HAL_OK;
}

static int32_t _bmp_open(BMP_FILE_RES_T *bmp)
{
    int32_t ret_sonix = HAL_OK;
    if (HAL_FLAG_HAS(bmp->mode, FA_READ))
    {
        ret_sonix = _bmp_parser_file_header(bmp);
        CHECK_MIDDLEWARE_RET(ret_sonix);

        ret_sonix = _bmp_parser_info_header(bmp);
        CHECK_MIDDLEWARE_RET(ret_sonix);
    }
    else if (HAL_FLAG_HAS(bmp->mode, FA_WRITE))
    {
        ret_sonix = _bmp_set_file_header(bmp);
        CHECK_MIDDLEWARE_RET(ret_sonix);

        ret_sonix = _bmp_set_info_header(bmp);
        CHECK_MIDDLEWARE_RET(ret_sonix);

        /* Write file header */
        uint8_t *buffer = NULL;
        MEMORY_ALLOC(buffer, BMP_FILE_HEADER_SIZE, GFP_SRAM);
        memset(buffer, 0, BMP_FILE_HEADER_SIZE);
        buffer[0]  = bmp->file_header.type & 0xFF;
        buffer[1]  = (bmp->file_header.type & 0xFF00) >> 8;
        buffer[2]  = bmp->file_header.size & 0xFF;
        buffer[3]  = (bmp->file_header.size & 0xFF00) >> 8;
        buffer[4]  = (bmp->file_header.size & 0xFF0000) >> 16;
        buffer[5]  = (bmp->file_header.size & 0xFF000000) >> 24;
        buffer[10] = bmp->file_header.off_bits & 0xFF;
        buffer[11] = (bmp->file_header.off_bits & 0xFF00) >> 8;
        buffer[12] = (bmp->file_header.off_bits & 0xFF0000) >> 16;
        buffer[13] = (bmp->file_header.off_bits & 0xFF0000) >> 24;
        ret_sonix  = _file_seek_and_write(FILE_NOT_SEEK, BMP_FILE_HEADER_SIZE, buffer, bmp->fp);
        MEMORY_FREE(buffer);
        CHECK_MIDDLEWARE_RET(ret_sonix);

        /* Write info header */
        ret_sonix = _file_seek_and_write(BMP_FILE_HEADER_SIZE, sizeof(BMP_INFO_HEADER_T), &bmp->info_header, bmp->fp);
        CHECK_MIDDLEWARE_RET(ret_sonix);

        if (bmp->info.bit_count == BIT_COUNT_8 || bmp->info.bit_count == BIT_COUNT_16)
        {
            if (bmp->info.palette_size != 0)
            {
                /* Write palette */
                ret_sonix = _file_seek_and_write(BMP_INFO_HEADER_SIZE + BMP_FILE_HEADER_SIZE, bmp->info.palette_size, bmp->info.palette_data, bmp->fp);
                CHECK_MIDDLEWARE_RET(ret_sonix);
            }
        }
    }

    ret_sonix = f_lseek(bmp->fp, bmp->file_header.off_bits);
    MFILE_DEBUG_ERROR(ret_sonix != HAL_OK, "Seek fail after get information header");
    CHECK_MIDDLEWARE_RET(ret_sonix);

    return HAL_OK;
}

static int32_t _bmp_close(BMP_FILE_RES_T *bmp)
{
    int32_t ret_sonix = HAL_OK;
    if (bmp->palette_data != NULL)
    {
        MEMORY_FREE(bmp->palette_data);
    }
    return ret_sonix;
}

static int32_t _bmp_read(BMP_FILE_RES_T *bmp, FILE_FRAME_T *frame, uint32_t is_blocking)
{
    uint64_t seek      = FILE_NOT_SEEK;
    int32_t  ret_sonix = _file_seek_and_read(seek, &frame->length, frame->buffer, bmp->fp);
    CHECK_MIDDLEWARE_RET(ret_sonix);
    return HAL_OK;
}

static int32_t _bmp_write(BMP_FILE_RES_T *bmp, const FILE_FRAME_T *frame, uint32_t is_blocking)
{
    uint64_t seek      = FILE_NOT_SEEK;
    int32_t  ret_sonix = _file_seek_and_write(seek, frame->length, frame->buffer, bmp->fp);
    CHECK_MIDDLEWARE_RET(ret_sonix);
    return HAL_OK;
}

static int32_t _bmp_seek(BMP_FILE_RES_T *bmp, const BMP_SEEK_T *seek)
{
    if (seek->pos >= (bmp->file_header.size - bmp->file_header.off_bits))
    {
        return HAL_ERROR;
    }

    FRESULT  result = FR_OK;
    uint32_t pos    = seek->pos + bmp->file_header.off_bits;
    result          = f_lseek(bmp->fp, pos);
    if (result != FR_OK)
    {
        return HAL_ERROR;
    }
    return HAL_OK;
}

static int32_t _bmp_wait(BMP_FILE_RES_T *wav)
{
    return HAL_OK;
}

static int32_t _bmp_get_status(BMP_FILE_RES_T *bmp, BMP_STATUS_T *status)
{
    status->bmp_pos         = f_tell(bmp->fp) - bmp->file_header.off_bits;
    status->bmp_size        = bmp->file_header.size;
    status->bmp_height      = bmp->info_header.height;
    status->bmp_width       = bmp->info_header.width;
    status->bit_count       = bmp->info_header.bit_count;
    status->compression     = bmp->info_header.compression;
    status->palette_data    = bmp->palette_data;
    status->palette_size    = bmp->palette_size;
    status->x_pels_permeter = bmp->info_header.x_pels_permeter;
    status->y_pels_permeter = bmp->info_header.y_pels_permeter;
    status->is_eof          = 0;
    if (status->bmp_size <= (status->bmp_pos + bmp->file_header.off_bits))
    {
        status->is_eof = 1;
    }
    return HAL_OK;
}

static int32_t BMP_Open(const void *bmp)
{
    CHECK_POINTER_NULL(bmp);
    CHECK_POINTER_NULL(((FIL_RES_T *)bmp)->res);
    return _bmp_open(((FIL_RES_T *)bmp)->res);
}

static int32_t BMP_Close(const void *bmp)
{
    CHECK_POINTER_NULL(bmp);
    if (((FIL_RES_T *)bmp)->res == NULL)
    {
        // close return ok to continue other close flow
        return HAL_OK;
    }
    return _bmp_close(((FIL_RES_T *)bmp)->res);
}

static int32_t BMP_Read(const void *bmp, void *frame, uint8_t is_blocking)
{
    CHECK_POINTER_NULL(bmp);
    CHECK_POINTER_NULL(((FIL_RES_T *)bmp)->res);
    CHECK_POINTER_NULL(frame);
    return _bmp_read(((FIL_RES_T *)bmp)->res, frame, is_blocking);
}

static int32_t BMP_Write(const void *bmp, const void *frame, uint8_t is_blocking)
{
    CHECK_POINTER_NULL(bmp);
    CHECK_POINTER_NULL(((FIL_RES_T *)bmp)->res);
    CHECK_POINTER_NULL(frame);
    return _bmp_write(((FIL_RES_T *)bmp)->res, frame, is_blocking);
}

static int32_t BMP_Seek(const void *bmp, const void *seek)
{
    CHECK_POINTER_NULL(bmp);
    CHECK_POINTER_NULL(((FIL_RES_T *)bmp)->res);
    CHECK_POINTER_NULL(seek);
    return _bmp_seek(((FIL_RES_T *)bmp)->res, seek);
}

static int32_t BMP_Wait(const void *bmp)
{
    CHECK_POINTER_NULL(bmp);
    CHECK_POINTER_NULL(((FIL_RES_T *)bmp)->res);
    return _bmp_wait(((FIL_RES_T *)bmp)->res);
}

static int32_t BMP_GetStatus(const void *bmp, void *status)
{
    CHECK_POINTER_NULL(bmp);
    CHECK_POINTER_NULL(((FIL_RES_T *)bmp)->res);
    CHECK_POINTER_NULL(status);
    return _bmp_get_status(((FIL_RES_T *)bmp)->res, status);
}

TYPE_WRAPPER BMP_Wrapper = {
    BMP_Open,
    BMP_Close,
    BMP_Read,
    BMP_Write,
    BMP_Wait,
    BMP_Seek,
    BMP_GetStatus,
};
