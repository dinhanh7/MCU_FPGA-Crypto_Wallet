/***********************
 * SN34F78X SDK V2.1.4 *
 ***********************/

#include "bin_wrapper.h"

//=============================================================================
//                  Private Function Definition
//=============================================================================
static int32_t _bin_open(BIN_RES_T *bin)
{
    return HAL_OK;
}

static int32_t _bin_close(BIN_RES_T *bin)
{
    return HAL_OK;
}

static int32_t _bin_read(BIN_RES_T *bin, FILE_FRAME_T *frame, uint8_t is_blocking)
{
    uint64_t seek      = FILE_NOT_SEEK;
    int32_t  ret_sonix = _file_seek_and_read(seek, &frame->length, frame->buffer, bin->fp);
    if (ret_sonix != HAL_OK)
    {
        return ret_sonix;
    }
    return HAL_OK;
}

static int32_t _bin_write(BIN_RES_T *bin, const FILE_FRAME_T *frame, uint8_t is_blocking)
{
    uint64_t seek      = FILE_NOT_SEEK;
    int32_t  ret_sonix = _file_seek_and_write(seek, frame->length, frame->buffer, bin->fp);
    if (ret_sonix != HAL_OK)
    {
        return ret_sonix;
    }
    return HAL_OK;
}

static int32_t _bin_seek(BIN_RES_T *bin, const BIN_SEEK_T *seek)
{
    if (seek->pos >= f_size(bin->fp))
    {
        return HAL_ERROR;
    }

    FRESULT result = FR_OK;
    result         = f_lseek(bin->fp, seek->pos);
    if (result != FR_OK)
    {
        return HAL_ERROR;
    }
    return HAL_OK;
}

static int32_t _bin_get_status(BIN_RES_T *bin, BIN_STATUS_T *status)
{
    status->bin_pos  = f_tell(bin->fp);
    status->bin_size = f_size(bin->fp);
    status->is_eof   = 0;
    if (f_size(bin->fp) <= f_tell(bin->fp))
    {
        status->is_eof = 1;
    }
    return HAL_OK;
}

static int32_t _bin_wait(BIN_RES_T *bin)
{
    return HAL_OK;
}

///////////////////////////////////////////////////////////////////////////////////////
static int32_t BIN_Open(const void *bin)
{
    CHECK_POINTER_NULL(bin);
    CHECK_POINTER_NULL(((FIL_RES_T *)bin)->res);
    return _bin_open(((FIL_RES_T *)bin)->res);
};
static int32_t BIN_Close(const void *bin)
{
    CHECK_POINTER_NULL(bin);
    // CHECK_POINTER_NULL(((FIL_RES_T *)bin)->res);
    if (((FIL_RES_T *)bin)->res == NULL)
    { // close return ok to continue other close flow
        return HAL_OK;
    }
    return _bin_close(((FIL_RES_T *)bin)->res);
};
static int32_t BIN_Read(const void *bin, void *frame, uint8_t is_blocking)
{
    CHECK_POINTER_NULL(bin);
    CHECK_POINTER_NULL(((FIL_RES_T *)bin)->res);
    CHECK_POINTER_NULL(frame);
    return _bin_read(((FIL_RES_T *)bin)->res, frame, is_blocking);
};
static int32_t BIN_Write(const void *bin, const void *frame, uint8_t is_blocking)
{
    CHECK_POINTER_NULL(bin);
    CHECK_POINTER_NULL(((FIL_RES_T *)bin)->res);
    CHECK_POINTER_NULL(frame);
    return _bin_write(((FIL_RES_T *)bin)->res, frame, is_blocking);
};
static int32_t BIN_Seek(const void *bin, const void *seek)
{
    CHECK_POINTER_NULL(bin);
    CHECK_POINTER_NULL(((FIL_RES_T *)bin)->res);
    CHECK_POINTER_NULL(seek);
    return _bin_seek(((FIL_RES_T *)bin)->res, seek);
};
static int32_t BIN_Wait(const void *bin)
{
    CHECK_POINTER_NULL(bin);
    CHECK_POINTER_NULL(((FIL_RES_T *)bin)->res);
    return _bin_wait(((FIL_RES_T *)bin)->res);
};
static int32_t BIN_GetStatus(const void *bin, void *status)
{
    CHECK_POINTER_NULL(bin);
    CHECK_POINTER_NULL(((FIL_RES_T *)bin)->res);
    CHECK_POINTER_NULL(status);
    return _bin_get_status(((FIL_RES_T *)bin)->res, status);
};

TYPE_WRAPPER BIN_Wrapper = {
    BIN_Open,
    BIN_Close,
    BIN_Read,
    BIN_Write,
    BIN_Wait,
    BIN_Seek,
    BIN_GetStatus,
};
