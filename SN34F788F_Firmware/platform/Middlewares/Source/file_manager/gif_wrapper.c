/***********************
 * SN34F78X SDK V2.1.4 *
 ***********************/

#include "gif_wrapper.h"

//////////////////////////////////////////////////////////////////////////////////////////////

/**
 * @brief Parse the GIF file header
 *
 * Parse the information header from the given GIF file resource and
 * update the file information structure
 *
 * @param gif:GIF file resource pointer
 *
 * @return Returns the result of the operation with HAL OK for success
 */
static int32_t _gif_parser_info_header(GIF_FILE_RES_T *gif)
{
    int32_t ret_sonix = HAL_OK;

    gif->head_size            = 0;
    uint32_t segment_head_len = GIF_FILE_HEADER_SIZE;
    uint64_t segment_head     = 0;
    ret_sonix                 = _file_seek_and_read(FILE_NOT_SEEK, &segment_head_len, &segment_head, gif->fp);
    MFILE_DEBUG_ERROR(ret_sonix != HAL_OK, "Get gif format fail");
    CHECK_MIDDLEWARE_RET(ret_sonix);

    /* Determine whether the headers are GTF87a and GTF89a */
    if (HAL_FLAG_NEQ(segment_head, GIF_TYPE_GIF89A) && HAL_FLAG_NEQ(segment_head, GIF_TYPE_GIF87A))
    {
        MFILE_DEBUG_ERROR(0x1, "The file is not GIF format");
        return HAL_ERROR;
    }
    gif->head_info = segment_head;

    /* Header Size */
    gif->head_size += segment_head_len;

    return ret_sonix;
}

/**
 * @brief Parse the logical screen descriptor of the GIF file
 *
 * The logical screen descriptor is parsed from the given GIF file resource and
 * stored into the specified GIF_FILE_RES_T structure。
 *
 * @param gif:GIF file resource pointer
 *
 * @return Returns the result of the operation with HAL OK for success
 */
static int32_t _gif_parser_info_logical_screen_descriptor(GIF_FILE_RES_T *gif)
{
    int32_t ret_sonix = HAL_OK;

    uint32_t segment_head_info_len                                      = GIF_FILE_LOGICAL_SCREEN_DESCRIPTOR_SIZE;
    uint8_t  segment_head_info[GIF_FILE_LOGICAL_SCREEN_DESCRIPTOR_SIZE] = {0};
    ret_sonix                                                           = _file_seek_and_read(GIF_FILE_HEADER_SIZE, &segment_head_info_len, segment_head_info, gif->fp);
    MFILE_DEBUG_ERROR(ret_sonix != HAL_OK, "Get gif format info fail");
    CHECK_MIDDLEWARE_RET(ret_sonix);

    gif->logical_screen_descriptor.width                   = ((segment_head_info[0] & 0xFF) << 8) | ((segment_head_info[1] & 0xFF00) >> 8);
    gif->logical_screen_descriptor.height                  = ((segment_head_info[2] & 0xFF) << 8) | ((segment_head_info[3] & 0xFF00) >> 8);
    gif->logical_screen_descriptor.global_color_table_flag = (segment_head_info[4] & 0x80) >> 7;
    gif->logical_screen_descriptor.color_resolution        = (segment_head_info[4] & 0x70) >> 4;
    gif->logical_screen_descriptor.sort_flag               = (segment_head_info[4] & 0x8) >> 3;
    gif->logical_screen_descriptor.pixel                   = 2 << (segment_head_info[4] & 0x7);
    gif->logical_screen_descriptor.background_color        = segment_head_info[5];
    gif->logical_screen_descriptor.pixel_aspect_ratio      = segment_head_info[6];

    /* Logical Screen Descriptor Size */
    gif->head_size += segment_head_info_len;

    /* Global Color Table Size*/
    gif->head_size += (1 << gif->logical_screen_descriptor.pixel_aspect_ratio) * 3;

    /* The characters preceding the image descriptor are stored */
    MEMORY_ALLOC(gif->head_data, gif->head_size, GFP_SRAM);
    memset(gif->head_data, 0, gif->head_size);
    ret_sonix = _file_seek_and_read(0, &gif->head_size, gif->head_data, gif->fp);
    MFILE_DEBUG_ERROR(ret_sonix != HAL_OK, "Get gif Logical Screen Descriptor fail");
    CHECK_MIDDLEWARE_RET(ret_sonix);

    return ret_sonix;
}

/**
 * @brief Set the GIF information header and logical screen descriptor
 *
 * Based on the given GIF file resource, set the GIF information header
 * and logical screen descriptor.
 *
 * @param gif:GIF file resource pointer
 *
 * @return Returns the result of the operation with HAL OK for success
 */
static int32_t _gif_set_info_header_and_logical_screen_descriptor(GIF_FILE_RES_T *gif)
{
    int32_t  ret_sonix = HAL_OK;
    uint64_t headers   = 0;

    gif->head_size = gif->info.head_size;
    MEMORY_ALLOC(gif->head_data, gif->head_size, GFP_SRAM);
    memset(gif->head_data, 0, gif->head_size);
    memset(&gif->logical_screen_descriptor, 0, sizeof(GIF_INFO_LSD_T));
    memcpy(gif->head_data, gif->info.head_data, gif->info.head_size);

    /* Determine whether the headers are GTF87a and GTF89a */
    memcpy(&headers, gif->info.head_data, GIF_FILE_HEADER_SIZE);
    if (HAL_FLAG_NEQ(headers, GIF_TYPE_GIF89A) && HAL_FLAG_NEQ(headers, GIF_TYPE_GIF87A))
    {
        MFILE_DEBUG_ERROR(0x1, "The file is not GIF format");
        return HAL_ERROR;
    }

    /* Write header data */
    ret_sonix = _file_seek_and_write(FILE_NOT_SEEK, gif->head_size, gif->head_data, gif->fp);
    MFILE_DEBUG_ERROR(ret_sonix != HAL_OK, "Set gif header fail");
    CHECK_MIDDLEWARE_RET(ret_sonix);

    return ret_sonix;
}

/**
 * @brief Open the GIF file
 *
 * According to the given GIF file resource pointer, open the GIF file and
 * initialize the relevant parameters
 *
 * @param gif:GIF file resource pointer
 *
 * @return Returns the result of the operation with HAL OK for success
 */
static int32_t _gif_open(GIF_FILE_RES_T *gif)
{
    int32_t ret_sonix = HAL_OK;
    if (HAL_FLAG_HAS(gif->mode, FA_READ))
    {
        ret_sonix = _gif_parser_info_header(gif);
        CHECK_MIDDLEWARE_RET(ret_sonix);

        ret_sonix = _gif_parser_info_logical_screen_descriptor(gif);
        CHECK_MIDDLEWARE_RET(ret_sonix);
    }
    else if (HAL_FLAG_HAS(gif->mode, FA_WRITE))
    {
        ret_sonix = _gif_set_info_header_and_logical_screen_descriptor(gif);
        CHECK_MIDDLEWARE_RET(ret_sonix);
    }

    ret_sonix = f_lseek(gif->fp, gif->head_size);
    MFILE_DEBUG_ERROR(ret_sonix != HAL_OK, "Seek fail after getting information header");
    CHECK_MIDDLEWARE_RET(ret_sonix);

    return HAL_OK;
}

/**
 * @brief Close GIF files
 *
 * Free the resources associated with the GIF file and close the GIF file.
 *
 * @param gif:GIF file resource pointer
 *
 * @return Returns the result of the operation with HAL OK for success
 */
static int32_t _gif_close(GIF_FILE_RES_T *gif)
{
    int32_t ret_sonix = HAL_OK;
    if (gif->head_data != NULL)
    {
        MEMORY_FREE(gif->head_data);
    }

    return ret_sonix;
}

/**
 * @brief Reading GIF files
 *
 * Read image data of the specified size from a given
 * GIF file and store it in the specified frame structure
 *
 * @param gif :GIF file resource pointer
 * @param frame :Frame structure pointer
 * @param is_blocking :Whether to block until the read is complete
 *
 * @return Returns the result of the operation with HAL OK for success
 */
static int32_t _gif_read(GIF_FILE_RES_T *gif, FILE_FRAME_T *frame, uint32_t is_blocking)
{
    uint64_t seek      = FILE_NOT_SEEK;
    int32_t  ret_sonix = _file_seek_and_read(seek, &frame->length, frame->buffer, gif->fp);
    CHECK_MIDDLEWARE_RET(ret_sonix);
    return HAL_OK;
}

/**
 * @brief Write the frame data to a GIF file
 *
 * The given frame data is written to a GIF file and the result of the operation is returned
 *
 * @param gif :GIF file resource pointer
 * @param frame :Frame structure pointer
 * @param is_blocking :Whether to block until the write is complete
 *
 * @return Returns the result of the operation with HAL OK for success
 */
static int32_t _gif_write(GIF_FILE_RES_T *gif, const FILE_FRAME_T *frame, uint32_t is_blocking)
{
    uint64_t seek      = FILE_NOT_SEEK;
    int32_t  ret_sonix = _file_seek_and_write(seek, frame->length, frame->buffer, gif->fp);
    CHECK_MIDDLEWARE_RET(ret_sonix);
    return HAL_OK;
}

/**
 * @brief GIF file location function
 *
 * Position the GIF file pointer at the specified location, given the GIF_SEEK_T structure.
 *
 * @param gif :GIF file resource pointer
 * @param seek :GIF file location structure pointer
 *
 * @return Returns the result of the operation with HAL OK for success
 */
static int32_t _gif_seek(GIF_FILE_RES_T *gif, const GIF_SEEK_T *seek)
{
    if (HAL_FLAG_HAS(gif->mode, FA_READ))
    {
        if (seek->pos >= (f_size(gif->fp) - gif->head_size))
        {
            return HAL_ERROR;
        }
    }

    FRESULT  result = FR_OK;
    uint32_t pos    = seek->pos + gif->head_size;
    result          = f_lseek(gif->fp, pos);
    if (result != FR_OK)
    {
        return HAL_ERROR;
    }
    return HAL_OK;
}

/**
 * @brief GIF wait functions
 *
 * This function is used to wait for the GIF resource to be processed.
 *
 * @param gif :GIF file resource pointer
 *
 * @return Returns the result of the operation with HAL OK for success
 */
static int32_t _gif_wait(GIF_FILE_RES_T *gif)
{
    return HAL_OK;
}

/**
 * @brief Get the GIF file status
 *
 * According to the given GIF file resource pointer, get the current status
 * information of the GIF file
 *
 * @param gif :GIF file resource pointer
 * @param status :GIF file status structure pointer
 *
 * @return Returns the result of the operation with HAL OK for success
 */
static int32_t _gif_get_status(GIF_FILE_RES_T *gif, GIF_STATUS_T *status)
{
    status->pos                = f_tell(gif->fp) - gif->head_size;
    status->size               = f_size(gif->fp);
    status->width              = gif->logical_screen_descriptor.width;
    status->height             = gif->logical_screen_descriptor.height;
    status->color_resolution   = gif->logical_screen_descriptor.color_resolution;
    status->pixel              = gif->logical_screen_descriptor.pixel;
    status->pixel_aspect_ratio = gif->logical_screen_descriptor.pixel_aspect_ratio;
    status->head_data          = gif->head_data;
    status->head_size          = gif->head_size;
    status->is_eof             = 0;
    if (status->size <= (status->pos + gif->head_size))
    {
        status->is_eof = 1;
    }
    return HAL_OK;
}

/**
 * @brief Open the GIF file
 *
 * Open the GIF file with the given GIF file pointer
 *
 * @param gif :GIF file resource pointer
 *
 * @return Returns the result of the operation with HAL OK for success
 */
static int32_t GIF_Open(const void *gif)
{
    CHECK_POINTER_NULL(gif);
    CHECK_POINTER_NULL(((FIL_RES_T *)gif)->res);
    return _gif_open(((FIL_RES_T *)gif)->res);
}

/**
 * @brief Close GIF files
 *
 * Closes the specified GIF file and releases the associated resource.
 *
 * @param gif :GIF file resource pointer
 *
 * @return Returns the result of the operation with HAL OK for success
 */
static int32_t GIF_Close(const void *gif)
{
    CHECK_POINTER_NULL(gif);
    if (((FIL_RES_T *)gif)->res == NULL)
    {
        // close return ok to continue other close flow
        return HAL_OK;
    }
    return _gif_close(((FIL_RES_T *)gif)->res);
}

/**
 * @brief Read GIF image data
 *
 * Read a length of data from a given GIF image and store it
 * in a specified frame buffer.
 *
 * @param gif :GIF file resource pointer
 * @param frame :Frame buffer pointer
 * @param is_blocking :Whether the function is blocking or not
 *
 * @return Returns the result of the operation with HAL OK for success
 */
static int32_t GIF_Read(const void *gif, void *frame, uint8_t is_blocking)
{
    CHECK_POINTER_NULL(gif);
    CHECK_POINTER_NULL(((FIL_RES_T *)gif)->res);
    CHECK_POINTER_NULL(frame);
    return _gif_read(((FIL_RES_T *)gif)->res, frame, is_blocking);
}

/**
 * @brief GIF Writing data
 *
 * Writes the specified GIF image data to a GIF file
 *
 * @param gif :GIF file resource pointer
 * @param frame :Frame buffer pointer
 * @param is_blocking :Whether the function is blocking or not
 *
 * @return Returns the result of the operation with HAL OK for success
 */
static int32_t GIF_Write(const void *gif, const void *frame, uint8_t is_blocking)
{
    CHECK_POINTER_NULL(gif);
    CHECK_POINTER_NULL(((FIL_RES_T *)gif)->res);
    CHECK_POINTER_NULL(frame);
    return _gif_write(((FIL_RES_T *)gif)->res, frame, is_blocking);
}

/**
 * @brief GIF Positioning Functions
 *
 * Based on the given GIF file and resource Pointers, the GIF file is positioned
 * to the specified location
 *
 * @param gif :GIF file resource pointer
 * @param seek :Positioning pointer
 *
 * @return Returns the result of the operation with HAL OK for success
 */
static int32_t GIF_Seek(const void *gif, const void *seek)
{
    CHECK_POINTER_NULL(gif);
    CHECK_POINTER_NULL(((FIL_RES_T *)gif)->res);
    CHECK_POINTER_NULL(seek);
    return _gif_seek(((FIL_RES_T *)gif)->res, seek);
}

/**
 * @brief GIF Waiting Functions
 *
 * This function is used to wait for the GIF resource to be processed.。
 *
 * @param gif :GIF file resource pointer
 *
 * @return Returns the result of the operation with HAL OK for success
 */
static int32_t GIF_Wait(const void *gif)
{
    CHECK_POINTER_NULL(gif);
    CHECK_POINTER_NULL(((FIL_RES_T *)gif)->res);
    return _gif_wait(((FIL_RES_T *)gif)->res);
}

/**
 * @brief Gets the status of the GIF
 *
 * Based on the given GIF pointer, get the state of the GIF and fill the state information
 * into the given state pointer.
 *
 * @param gif :GIF file resource pointer
 * @param status :State pointer
 *
 * @return Returns the result of the operation with HAL OK for success
 */
static int32_t GIF_GetStatus(const void *gif, void *status)
{
    CHECK_POINTER_NULL(gif);
    CHECK_POINTER_NULL(((FIL_RES_T *)gif)->res);
    CHECK_POINTER_NULL(status);
    return _gif_get_status(((FIL_RES_T *)gif)->res, status);
}

TYPE_WRAPPER GIF_Wrapper = {
    GIF_Open,
    GIF_Close,
    GIF_Read,
    GIF_Write,
    GIF_Wait,
    GIF_Seek,
    GIF_GetStatus,
};
