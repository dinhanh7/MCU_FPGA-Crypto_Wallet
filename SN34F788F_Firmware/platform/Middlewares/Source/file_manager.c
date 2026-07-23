/***********************
 * SN34F78X SDK V2.1.4 *
 ***********************/

#include "file_manager.h"

#if (configUSE_FILEMANAGER_MIDDLEWARE == 1)
/////////////////////////////////////////////// resource ///////////////////////////////////////////////
static MANAGER_RES_T MFile_res         = {0};
static TYPE_WRAPPER *Wrapper_Manager[] = {
// for tailor the file manager
#if (configUSE_BIN_FILE)
    [FILE_TYPE_BIN] = &BIN_Wrapper,
#else
    [FILE_TYPE_BIN] = NULL,
#endif
#if (configUSE_TXT_FILE)
    [FILE_TYPE_TXT] = &BIN_Wrapper,
#else
    [FILE_TYPE_TXT] = NULL,
#endif
#if (configUSE_WAV_FILE)
    [FILE_TYPE_WAV] = &WAV_Wrapper,
#else
    [FILE_TYPE_WAV] = NULL,
#endif
#if (configUSE_MP3_FILE)
    [FILE_TYPE_MP3] = &MP3_Wrapper,
#else
    [FILE_TYPE_MP3] = NULL,
#endif
#if (configUSE_A32_FILE)
    [FILE_TYPE_A32] = &A32_Wrapper,
#else
    [FILE_TYPE_A32] = NULL,
#endif
#if (configUSE_BMP_FILE)
    [FILE_TYPE_BMP] = &BMP_Wrapper,
#else
    [FILE_TYPE_BMP] = NULL,
#endif
#if (configUSE_JPG_FILE)
    [FILE_TYPE_JPG] = &JPG_Wrapper,
#else
    [FILE_TYPE_JPG] = NULL,
#endif
#if (configUSE_GIF_FILE)
    [FILE_TYPE_GIF] = &GIF_Wrapper,
#else
    [FILE_TYPE_GIF] = NULL,
#endif
#if (configUSE_AVI_FILE)
    [FILE_TYPE_AVI] = &AVI_Wrapper,
#else
    [FILE_TYPE_AVI] = NULL,
#endif
};

/////////////////////////////////////////////// Tool function ///////////////////////////////////////////////
static uint32_t murMurHash(const void *key, uint32_t len)
{
    const uint32_t m    = 0x5bd1e995;
    const uint32_t r    = 24;
    const uint32_t seed = 97;
    uint32_t       h    = seed ^ len;
    // Mix 4 bytes at a time into the hash
    const uint8_t *data = key;
    while (len >= 4)
    {
        uint32_t k = *(uint32_t *)data;
        k *= m;
        k ^= k >> r;
        k *= m;
        h *= m;
        h ^= k;
        data += 4;
        len -= 4;
    }
    // Handle the last few bytes of the input array
    switch (len)
    {
        case 3:
            h ^= data[2] << 16;
        case 2:
            h ^= data[1] << 8;
        case 1:
            h ^= data[0];
            h *= m;
    };
    // Do a few final mixes of the hash to ensure the last few
    // bytes are well-incorporated.
    h ^= h >> 13;
    h *= m;
    h ^= h >> 15;
    return h;
}

static int32_t _m_file_check_name(TCHAR *path_full, uint8_t dev, const TCHAR *path)
{
    sprintf(path_full, "%d:%s", dev, path);
    if (path_full[FILE_NAME_LEN - 1] != 0)
    {
        return M_FILE_ERROR_INVALID_PATH;
    }
    return HAL_OK;
}

static int32_t _m_file_uninit(M_FILE_HANDLE_T fh)
{
    int32_t    ret_sonix   = HAL_OK;
    FAT_RES_T *fat         = MFile_res.fat_set[fh.dev];
    FIL_RES_T *fp          = fat->fil_set[fh.index];
    fat->fil_set[fh.index] = NULL;
    for (uint16_t index = 1; index <= fat->alive[0]; index++)
    {
        if (fh.index == fat->alive[index])
        {
            fat->alive[index]         = fat->alive[fat->alive[0]];
            fat->alive[fat->alive[0]] = 0;
            fat->alive[0]--;
            break;
        }
    }

    // unmount fat
    uint8_t un_mount_flag = 1;
#if FF_MULTI_PARTITION
    FATFS_DISK_STATUS_T status = {FATFS_PHY_DEV_SD1};
    ret_sonix                  = HAL_MFile_GetDiskStatus(&status);
    CHECK_MIDDLEWARE_RET(ret_sonix);
    uint8_t phy_dev = VolToPart[fh.dev].pd;
    for (uint8_t index = 0; index < FATFS_LOGIC_PARTM; index++)
    {
        uint8_t logic_dev = status.part[phy_dev][index].logic;
        if (MFile_res.fat_set[logic_dev] && status.part[phy_dev][index].size && MFile_res.fat_set[logic_dev]->alive[0])
        {
            un_mount_flag = 0;
            break;
        }
    }
#else
    if (fat->alive[0] != 0)
    {
        un_mount_flag = 0;
    }
#endif

    if (un_mount_flag)
    {
        TAKE_SEM_WITH_COUNT(fat->sem, fat->sem_cnt);
        ret_sonix = _file_mount(NULL, fat->work, fp->name);
        GIVE_SEM_WITH_COUNT(fat->sem, fat->sem_cnt);
        DELETE_SEM_WITH_COUNT(fat->sem, fat->sem_cnt);
        MEMORY_FREE(fat);
        MFile_res.fat_set[fh.dev] = NULL;
    }
    MEMORY_FREE(fp);
    DELETE_SEM_WITH_COUNT(MFile_res.sem, MFile_res.sem_cnt);
    return ret_sonix;
}

static int32_t _m_file_init(M_FILE_HANDLE_T *fh, uint8_t dev, const TCHAR *path_name)
{
    FAT_RES_T *fat = MFile_res.fat_set[dev];
    if (fat == NULL)
    {
        MEMORY_ALLOC(fat, sizeof(FAT_RES_T), GFP_KERNEL);
        int32_t ret_sonix = _file_mount(&fat->fat, fat->work, path_name);
        if (ret_sonix != HAL_OK)
        {
            MEMORY_FREE(fat);
            return HAL_ERROR;
        }
        MFile_res.fat_set[dev] = fat;
    }
    CREATE_SEM_WITH_COUNT(fat->sem, fat->sem_cnt);

    uint16_t index = 0;
    uint32_t hash  = murMurHash(path_name, strlen(path_name));
    for (index = 1; index <= fat->alive[0]; index++)
    {
        uint16_t file_idx = fat->alive[index];
        if (file_idx == 0)
        {
            break;
        }

        FIL_RES_T *fp = fat->fil_set[file_idx];
        if (hash == fp->hash)
        {
            return M_FILE_ERROR_REOPEN;
        }
    }
    for (index = 1; index < FILE_OBJ_SIZE; index++)
    {
        FIL_RES_T *fp = fat->fil_set[index];
        if (fp == NULL)
        {
            MEMORY_ALLOC(fp, sizeof(FIL_RES_T), GFP_KERNEL);
            memcpy(fp->name, path_name, FILE_NAME_LEN);
            fp->hash            = hash;
            fat->fil_set[index] = fp;
            fat->alive[0]++;
            fat->alive[fat->alive[0]] = index;
            break;
        }
    }

    if (index == FILE_OBJ_SIZE)
    {
        return M_FILE_ERROR_OPEN_OVER_SIZE;
    }

    memset(fh, 0, sizeof(M_FILE_HANDLE_T));
    fh->dev   = dev;
    fh->index = index;
    return HAL_OK;
}

static int32_t _m_file_check_eof(M_FILE_HANDLE_T *fh, TYPE_WRAPPER *wrapper, FIL_RES_T *fp)
{
    int32_t ret_sonix = HAL_OK;
    switch (fp->format)
    {
        case FILE_TYPE_BIN:
        case FILE_TYPE_TXT:
        {
            BIN_STATUS_T bin_status = {0};
            ret_sonix               = wrapper->GetStatus(fp, &bin_status);
            CHECK_MIDDLEWARE_RET(ret_sonix);
            fh->bin_eof = M_FILE_IS_NOT_EOF;
            if (bin_status.is_eof)
            {
                fh->bin_eof = M_FILE_IS_EOF;
            }
            break;
        }
        case FILE_TYPE_WAV:
        {
            WAV_STATUS_T wav_status = {0};
            ret_sonix               = wrapper->GetStatus(fp, &wav_status);
            CHECK_MIDDLEWARE_RET(ret_sonix);
            fh->wav_eof = M_FILE_IS_NOT_EOF;
            if (wav_status.is_eof)
            {
                fh->wav_eof = M_FILE_IS_EOF;
            }
            break;
        }
        case FILE_TYPE_A32:
        {
            A32_STATUS_T a32_status = {0};
            ret_sonix               = wrapper->GetStatus(fp, &a32_status);
            CHECK_MIDDLEWARE_RET(ret_sonix);
            fh->a32_eof = M_FILE_IS_NOT_EOF;
            if (a32_status.is_eof)
            {
                fh->a32_eof = M_FILE_IS_EOF;
            }
            break;
        }
        case FILE_TYPE_MP3:
        {
            MP3_STATUS_T mp3_status = {MP3_LAYER_MAX};
            ret_sonix               = wrapper->GetStatus(fp, &mp3_status);
            CHECK_MIDDLEWARE_RET(ret_sonix);
            fh->mp3_eof = M_FILE_IS_NOT_EOF;
            if (mp3_status.is_eof)
            {
                fh->mp3_eof = M_FILE_IS_EOF;
            }
            break;
        }
        case FILE_TYPE_BMP:
        {
            BMP_STATUS_T bmp_status = {0};
            ret_sonix               = wrapper->GetStatus(fp, &bmp_status);
            CHECK_MIDDLEWARE_RET(ret_sonix);
            fh->bmp_eof = M_FILE_IS_NOT_EOF;
            if (bmp_status.is_eof)
            {
                fh->bmp_eof = M_FILE_IS_EOF;
            }
            break;
        }
        case FILE_TYPE_JPG:
        {
            JPG_STATUS_T jpg_status = {0};
            ret_sonix               = wrapper->GetStatus(fp, &jpg_status);
            CHECK_MIDDLEWARE_RET(ret_sonix);
            fh->jpg_eof = M_FILE_IS_NOT_EOF;
            if (jpg_status.is_eof)
            {
                fh->jpg_eof = M_FILE_IS_EOF;
            }
            break;
        }
        case FILE_TYPE_GIF:
        {
            GIF_STATUS_T gif_status = {0};
            ret_sonix               = wrapper->GetStatus(fp, &gif_status);
            CHECK_MIDDLEWARE_RET(ret_sonix);
            fh->gif_eof = M_FILE_IS_NOT_EOF;
            if (gif_status.is_eof)
            {
                fh->gif_eof = M_FILE_IS_EOF;
            }
            break;
        }
        case FILE_TYPE_AVI:
        {
            AVI_STATUS_T avi_status = {0};
            ret_sonix               = wrapper->GetStatus(fp, &avi_status);
            CHECK_MIDDLEWARE_RET(ret_sonix);
            fh->a_eof   = M_FILE_IS_NOT_EOF;
            fh->v_eof   = M_FILE_IS_NOT_EOF;
            fh->avi_eof = M_FILE_IS_NOT_EOF;
            if (avi_status.is_a_eof)
            {
                fh->a_eof = M_FILE_IS_EOF;
            }
            if (avi_status.is_v_eof)
            {
                fh->v_eof = M_FILE_IS_EOF;
            }
            if (avi_status.is_eof)
            {
                fh->avi_eof = M_FILE_IS_EOF;
            }
            break;
        }
        default:
            ret_sonix = HAL_ERROR;
    }

    return ret_sonix;
}

static int32_t _m_file_check_init(M_FILE_INIT_T *init_info)
{
#if FF_MULTI_PARTITION
    if ((init_info->dev & FILE_PHY_DISK_FLAG) == FILE_PHY_DISK_FLAG)
    {
        return HAL_ERROR;
    }
    if ((init_info->dev & ~FILE_PHY_DISK_FLAG) >= FF_VOLUMES)
    {
        return HAL_ERROR;
    }
#else
    if ((init_info->dev & FILE_PHY_DISK_FLAG) == FILE_PHY_DISK_FLAG)
    {
        switch (init_info->dev)
        {
            case FATFS_PHY_DEV_SD1:
            case FATFS_PHY_DEV_FLASH:
            case FATFS_PHY_DEV_SD0:
            case FATFS_PHY_DEV_USB:
                break;
            default:
                return HAL_ERROR;
        }
    }
#endif
    init_info->dev &= ~FILE_PHY_DISK_FLAG;

    switch (init_info->format)
    {
        case FILE_TYPE_BIN:
        case FILE_TYPE_TXT:
            break;
        case FILE_TYPE_WAV:
            break;
        case FILE_TYPE_A32:
            break;
        case FILE_TYPE_MP3:
            break;
        case FILE_TYPE_BMP:
            break;
        case FILE_TYPE_JPG:
            break;
        case FILE_TYPE_GIF:
            break;
        case FILE_TYPE_AVI:
            break;
        default:
            return HAL_ERROR;
    }

    switch (init_info->mode)
    {
        case FILE_OPEN_R:
        case FILE_OPEN_W:
        case FILE_OPEN_A:
        case FILE_OPEN_WX:
            break;
        default:
            return HAL_ERROR;
    }

    return HAL_OK;
}

/////////////////////////////////////////////// Disk operation ///////////////////////////////////////////////
static int32_t _m_file_disk(FATFS_DISK_T *disk)
{
    CHECK_POINTER_NULL(disk);
    int32_t ret_sonix = HAL_OK;
    CREATE_SEM_WITH_COUNT(MFile_res.sem, MFile_res.sem_cnt);
    TAKE_SEM_WITH_COUNT(MFile_res.sem, MFile_res.sem_cnt);
    disk->dev &= ~FILE_PHY_DISK_FLAG;
    ret_sonix = _file_disk(disk, &MFile_res.DiskMap);
    GIVE_SEM_WITH_COUNT(MFile_res.sem, MFile_res.sem_cnt);
    DELETE_SEM_WITH_COUNT(MFile_res.sem, MFile_res.sem_cnt);
    CHECK_MIDDLEWARE_RET(ret_sonix);
    return HAL_OK;
}

static int32_t _m_file_disk_find(FATFS_LOGIC_DEV_T *logic, FATFS_PHY_DEV_T dev, FATFS_LOGIC_PART_T part)
{
    CHECK_POINTER_NULL(logic);
    int32_t ret_sonix = HAL_OK;
    CREATE_SEM_WITH_COUNT(MFile_res.sem, MFile_res.sem_cnt);
    TAKE_SEM_WITH_COUNT(MFile_res.sem, MFile_res.sem_cnt);
    dev &= ~FILE_PHY_DISK_FLAG;
    ret_sonix = _file_disk_find(logic, dev, part, &MFile_res.DiskMap);
    GIVE_SEM_WITH_COUNT(MFile_res.sem, MFile_res.sem_cnt);
    DELETE_SEM_WITH_COUNT(MFile_res.sem, MFile_res.sem_cnt);
    CHECK_MIDDLEWARE_RET(ret_sonix);
    return HAL_OK;
}

static int32_t _m_file_disk_status(FATFS_DISK_STATUS_T *status)
{
    CHECK_POINTER_NULL(status);
    int32_t ret_sonix = HAL_OK;
    CREATE_SEM_WITH_COUNT(MFile_res.sem, MFile_res.sem_cnt);
    TAKE_SEM_WITH_COUNT(MFile_res.sem, MFile_res.sem_cnt);
    ret_sonix = _file_disk_status(status, &MFile_res.DiskMap);
    GIVE_SEM_WITH_COUNT(MFile_res.sem, MFile_res.sem_cnt);
    DELETE_SEM_WITH_COUNT(MFile_res.sem, MFile_res.sem_cnt);
    CHECK_MIDDLEWARE_RET(ret_sonix);
    return HAL_OK;
}

/////////////////////////////////////////////// file operation ///////////////////////////////////////////////
static int32_t _m_file_open(M_FILE_HANDLE_T *fh, M_FILE_INIT_T *init_info)
{
    CHECK_POINTER_NULL(fh);
    CHECK_POINTER_NULL(init_info);
    if ((init_info->format != FILE_TYPE_BIN && init_info->format != FILE_TYPE_TXT) && init_info->mode == FILE_OPEN_A)
    {
        return HAL_ERROR;
    }
    // check init info
    int32_t ret_sonix = HAL_OK;
    ret_sonix         = _m_file_check_init(init_info);
    CHECK_MIDDLEWARE_RET(ret_sonix);

    // check file name
    TCHAR path_name[FILE_NAME_LEN] = {0};
    ret_sonix                      = _m_file_check_name(path_name, init_info->dev, init_info->full_path);
    CHECK_MIDDLEWARE_RET(ret_sonix);

    // init MFile
    CREATE_SEM_WITH_COUNT(MFile_res.sem, MFile_res.sem_cnt);
    TAKE_SEM_WITH_COUNT(MFile_res.sem, MFile_res.sem_cnt);
    ret_sonix = _m_file_init(fh, init_info->dev, path_name);
    GIVE_SEM_WITH_COUNT(MFile_res.sem, MFile_res.sem_cnt);
    CHECK_MIDDLEWARE_RET(ret_sonix);

    // init file
    FAT_RES_T    *fat     = MFile_res.fat_set[fh->dev];
    FIL_RES_T    *fp      = fat->fil_set[fh->index];
    TYPE_WRAPPER *wrapper = Wrapper_Manager[init_info->format];
    CHECK_POINTER_NULL(wrapper);
    TAKE_SEM_WITH_COUNT(fat->sem, fat->sem_cnt);
    ret_sonix = _file_open(&fp->fp, fp->name, init_info->mode);
    GIVE_SEM_WITH_COUNT(fat->sem, fat->sem_cnt);
    CHECK_MIDDLEWARE_RET(ret_sonix);
    if ((init_info->mode & FA_WRITE) != FA_WRITE)
    {
        TAKE_SEM_WITH_COUNT(fat->sem, fat->sem_cnt);
        _file_fast_seek(&fp->fp, fp->name, &fat->fat);
        GIVE_SEM_WITH_COUNT(fat->sem, fat->sem_cnt);
    }

    // init resource
    fp->format = init_info->format;
    switch (fp->format)
    {
        case FILE_TYPE_BIN:
        case FILE_TYPE_TXT:
        {
            // common resource
            BIN_RES_T *bin_res = NULL;
            MEMORY_ALLOC(bin_res, sizeof(BIN_RES_T), GFP_KERNEL);
            bin_res->fp   = &fp->fp;
            bin_res->mode = init_info->mode;
            fp->res       = bin_res;
            // write resource
            break;
        }
        case FILE_TYPE_WAV:
        {
            // common resource
            WAV_FILE_RES_T *wav_res = NULL;
            MEMORY_ALLOC(wav_res, sizeof(WAV_FILE_RES_T), GFP_KERNEL);
            wav_res->fp   = &fp->fp;
            wav_res->mode = init_info->mode;
            fp->res       = wav_res;
            // write resource
            if ((init_info->mode & FA_WRITE) == FA_WRITE)
            {
                wav_res->info.audio_format    = init_info->wav.audio_format;
                wav_res->info.bits_per_sample = init_info->wav.bits_per_sample;
                wav_res->info.num_of_channels = init_info->wav.num_of_channels;
                wav_res->info.sample_rate     = init_info->wav.sample_rate;
                wav_res->info.block_align     = init_info->wav.block_align;
            }
            break;
        }
        case FILE_TYPE_A32:
        {
            // common resource
            A32_FILE_RES_T *a32_res = NULL;
            MEMORY_ALLOC(a32_res, sizeof(A32_FILE_RES_T), GFP_KERNEL);
            a32_res->fp   = &fp->fp;
            a32_res->mode = init_info->mode;
            fp->res       = a32_res;
            // write resource
            if ((init_info->mode & FA_WRITE) == FA_WRITE)
            {
                a32_res->info.bit_rate        = init_info->a32.bit_rate;
                a32_res->info.format          = init_info->a32.format;
                a32_res->info.num_of_channels = init_info->a32.num_of_channels;
                a32_res->info.sample_rate     = init_info->a32.sample_rate;
            }
            break;
        }
        case FILE_TYPE_MP3:
        {
            // common resource
            MP3_FILE_RES_T *mp3_res = NULL;
            MEMORY_ALLOC(mp3_res, sizeof(MP3_FILE_RES_T), GFP_KERNEL);
            mp3_res->fp   = &fp->fp;
            mp3_res->mode = init_info->mode;
            fp->res       = mp3_res;
            // read resource
            if ((init_info->mode & FA_READ) == FA_READ)
            {
            }
            // write resource
            if ((init_info->mode & FA_WRITE) == FA_WRITE)
            {
                mp3_res->info.bit_rate    = init_info->mp3.bit_rate;
                mp3_res->info.channel     = init_info->mp3.channel;
                mp3_res->info.layer       = init_info->mp3.layer;
                mp3_res->info.sample_rate = init_info->mp3.sample_rate;
                mp3_res->info.ver         = init_info->mp3.ver;
            }
            break;
        }
        case FILE_TYPE_BMP:
        {
            // common resource
            BMP_FILE_RES_T *bmp_res = NULL;
            MEMORY_ALLOC(bmp_res, sizeof(BMP_FILE_RES_T), GFP_KERNEL);
            bmp_res->fp   = &fp->fp;
            bmp_res->mode = init_info->mode;
            fp->res       = bmp_res;
            if ((init_info->mode & FA_WRITE) == FA_WRITE)
            {
                bmp_res->info.bit_count       = init_info->bmp.bit_count;
                bmp_res->info.bmp_height      = init_info->bmp.bmp_height;
                bmp_res->info.bmp_width       = init_info->bmp.bmp_width;
                bmp_res->info.compression     = init_info->bmp.compression;
                bmp_res->info.palette_data    = init_info->bmp.palette_data;
                bmp_res->info.palette_size    = init_info->bmp.palette_size;
                bmp_res->info.x_pels_permeter = init_info->bmp.x_pels_permeter;
                bmp_res->info.y_pels_permeter = init_info->bmp.y_pels_permeter;
            }
            // write resource
            break;
        }
        case FILE_TYPE_JPG:
        {
            // common resource
            JPG_FILE_RES_T *jpg_res = NULL;
            MEMORY_ALLOC(jpg_res, sizeof(BMP_FILE_RES_T), GFP_KERNEL);
            jpg_res->fp   = &fp->fp;
            jpg_res->mode = init_info->mode;
            fp->res       = jpg_res;
            if ((init_info->mode & FA_WRITE) == FA_WRITE)
            {
                jpg_res->info.head_data = init_info->jpg.head_data;
                jpg_res->info.head_size = init_info->jpg.head_size;
            }
            // write resource
            break;
        }
        case FILE_TYPE_GIF:
        {
            // common resource
            GIF_FILE_RES_T *gif_res = NULL;
            MEMORY_ALLOC(gif_res, sizeof(GIF_FILE_RES_T), GFP_KERNEL);
            gif_res->fp   = &fp->fp;
            gif_res->mode = init_info->mode;
            fp->res       = gif_res;
            if ((init_info->mode & FA_WRITE) == FA_WRITE)
            {
                gif_res->info.head_data = init_info->gif.head_data;
                gif_res->info.head_size = init_info->gif.head_size;
            }
            // write resource
            break;
        }
        case FILE_TYPE_AVI:
        {
            // common resource
            AVI_FILE_RES_T *avi_res = NULL;
            MEMORY_ALLOC(avi_res, sizeof(AVI_FILE_RES_T), GFP_KERNEL);
            avi_res->fp   = &fp->fp;
            avi_res->mode = init_info->mode;
            fp->res       = avi_res;
            if ((init_info->mode & FA_WRITE) == FA_WRITE)
            {
                avi_res->info.head_data = init_info->avi.head_data;
                avi_res->info.head_size = init_info->avi.head_size;
                avi_res->dev            = init_info->dev;
            }
            // write resource
            break;
        }
        default:
            return HAL_ERROR;
    }

    // init type
    TAKE_SEM_WITH_COUNT(fat->sem, fat->sem_cnt);
    ret_sonix = wrapper->Open(fp);
    GIVE_SEM_WITH_COUNT(fat->sem, fat->sem_cnt);
    CHECK_MIDDLEWARE_RET(ret_sonix);

    return HAL_OK;
}

static int32_t _m_file_close(M_FILE_HANDLE_T fh)
{
    int32_t    ret_sonix = HAL_OK;
    FAT_RES_T *fat       = MFile_res.fat_set[fh.dev];
    CHECK_POINTER_NULL(fat);
    FIL_RES_T *fp = fat->fil_set[fh.index];
    CHECK_POINTER_NULL(fp);
    TYPE_WRAPPER *wrapper = Wrapper_Manager[fp->format];
    CHECK_POINTER_NULL(wrapper);

    // release type
    TAKE_SEM_WITH_COUNT(fat->sem, fat->sem_cnt);
    ret_sonix = wrapper->Close(fp);
    GIVE_SEM_WITH_COUNT(fat->sem, fat->sem_cnt);
    CHECK_MIDDLEWARE_RET(ret_sonix);

    // release resource
    switch (fp->format)
    {
        case FILE_TYPE_BIN:
        case FILE_TYPE_TXT:
            break;
        case FILE_TYPE_WAV:
            break;
        case FILE_TYPE_A32:
            break;
        case FILE_TYPE_MP3:
            break;
        case FILE_TYPE_BMP:
            break;
        case FILE_TYPE_JPG:
            break;
        case FILE_TYPE_GIF:
            break;
        case FILE_TYPE_AVI:
            break;
        default:
            return HAL_ERROR;
    }
    MEMORY_FREE(fp->res);

    // release file
    TAKE_SEM_WITH_COUNT(fat->sem, fat->sem_cnt);
    ret_sonix = _file_close(&fp->fp, NULL);
    GIVE_SEM_WITH_COUNT(fat->sem, fat->sem_cnt);
    CHECK_MIDDLEWARE_RET(ret_sonix);

    // release MFile
    ret_sonix = _m_file_uninit(fh);
    CHECK_MIDDLEWARE_RET(ret_sonix);

    return HAL_OK;
}

static int32_t _m_file_read(M_FILE_HANDLE_T *fh, M_FILE_INFO_T *info, int8_t is_blocking)
{
    if (is_blocking == M_FILE_NON_BLOCKING)
    {
        return M_FILE_ERROR_NON_BLOCKING;
    }

    CHECK_POINTER_NULL(fh);
    CHECK_POINTER_NULL(info);
    int32_t    ret_sonix = HAL_OK;
    FAT_RES_T *fat       = MFile_res.fat_set[fh->dev];
    CHECK_POINTER_NULL(fat);
    FIL_RES_T *fp = fat->fil_set[fh->index];
    CHECK_POINTER_NULL(fp);
    TYPE_WRAPPER *wrapper = Wrapper_Manager[fp->format];
    CHECK_POINTER_NULL(wrapper);

    switch (fp->format)
    {
        case FILE_TYPE_BIN:
        case FILE_TYPE_TXT:
            break;
        case FILE_TYPE_WAV:
            break;
        case FILE_TYPE_A32:
            break;
        case FILE_TYPE_MP3:
            break;
        case FILE_TYPE_BMP:
            break;
        case FILE_TYPE_JPG:
            break;
        case FILE_TYPE_GIF:
            break;
        case FILE_TYPE_AVI:
            break;
        default:
            return HAL_ERROR;
    }

    FILE_FRAME_T file_frame = {
        .buffer     = info->cache,
        .length     = info->cache_len,
        .frame_info = info->frame_info,
    };
    TAKE_SEM_WITH_COUNT(fat->sem, fat->sem_cnt);
    ret_sonix = wrapper->Read(fp, &file_frame, is_blocking);
    GIVE_SEM_WITH_COUNT(fat->sem, fat->sem_cnt);
    MFILE_DEBUG_ERROR(ret_sonix != HAL_OK, "wrapper->Read %d", ret_sonix);
    CHECK_MIDDLEWARE_RET(ret_sonix);

    TAKE_SEM_WITH_COUNT(fat->sem, fat->sem_cnt);
    ret_sonix = _m_file_check_eof(fh, wrapper, fp);
    GIVE_SEM_WITH_COUNT(fat->sem, fat->sem_cnt);
    MFILE_DEBUG_ERROR(ret_sonix != HAL_OK, "_m_file_check_eof %d", ret_sonix);
    CHECK_MIDDLEWARE_RET(ret_sonix);

    info->data     = file_frame.buffer;
    info->data_len = file_frame.length;
    fp->busy       = 0;
    return HAL_OK;
}

static int32_t _m_file_write(M_FILE_HANDLE_T *fh, M_FILE_INFO_T *info, int8_t is_blocking)
{
    if (is_blocking == M_FILE_NON_BLOCKING)
    {
        return M_FILE_ERROR_NON_BLOCKING;
    }

    CHECK_POINTER_NULL(fh);
    CHECK_POINTER_NULL(info);
    int32_t    ret_sonix = HAL_OK;
    FAT_RES_T *fat       = MFile_res.fat_set[fh->dev];
    CHECK_POINTER_NULL(fat);
    FIL_RES_T *fp = fat->fil_set[fh->index];
    CHECK_POINTER_NULL(fp);
    TYPE_WRAPPER *wrapper = Wrapper_Manager[fp->format];
    CHECK_POINTER_NULL(wrapper);

    switch (fp->format)
    {
        case FILE_TYPE_BIN:
        case FILE_TYPE_TXT:
            break;
        case FILE_TYPE_WAV:
            break;
        case FILE_TYPE_A32:
            break;
        case FILE_TYPE_MP3:
            break;
        case FILE_TYPE_BMP:
            break;
        case FILE_TYPE_JPG:
            break;
        case FILE_TYPE_GIF:
            break;
        case FILE_TYPE_AVI:
            break;
        default:
            return HAL_ERROR;
    }

    FILE_FRAME_T file_frame = {
        .buffer     = info->data,
        .length     = info->data_len,
        .frame_info = info->frame_info,
    };
    TAKE_SEM_WITH_COUNT(fat->sem, fat->sem_cnt);
    ret_sonix = wrapper->Write(fp, &file_frame, is_blocking);
    GIVE_SEM_WITH_COUNT(fat->sem, fat->sem_cnt);
    CHECK_MIDDLEWARE_RET(ret_sonix);

    return HAL_OK;
}

static int32_t _m_file_wait(M_FILE_HANDLE_T fh)
{
    int32_t    ret_sonix = HAL_OK;
    FAT_RES_T *fat       = MFile_res.fat_set[fh.dev];
    CHECK_POINTER_NULL(fat);
    FIL_RES_T *fp = fat->fil_set[fh.index];
    CHECK_POINTER_NULL(fp);
    TYPE_WRAPPER *wrapper = Wrapper_Manager[fp->format];
    CHECK_POINTER_NULL(wrapper);

    TAKE_SEM_WITH_COUNT(fat->sem, fat->sem_cnt);
    ret_sonix = wrapper->Wait(fp);
    GIVE_SEM_WITH_COUNT(fat->sem, fat->sem_cnt);
    CHECK_MIDDLEWARE_RET(ret_sonix);

    return HAL_OK;
}

static int32_t _m_file_seek(M_FILE_HANDLE_T *fh, M_FILE_SEEK_T seek)
{
    CHECK_POINTER_NULL(fh);
    int32_t    ret_sonix = HAL_OK;
    FAT_RES_T *fat       = MFile_res.fat_set[fh->dev];
    CHECK_POINTER_NULL(fat);
    FIL_RES_T *fp = fat->fil_set[fh->index];
    CHECK_POINTER_NULL(fp);
    TYPE_WRAPPER *wrapper = Wrapper_Manager[fp->format];
    CHECK_POINTER_NULL(wrapper);

    TAKE_SEM_WITH_COUNT(fat->sem, fat->sem_cnt);
    ret_sonix = wrapper->Seek(fp, &seek);
    GIVE_SEM_WITH_COUNT(fat->sem, fat->sem_cnt);
    CHECK_MIDDLEWARE_RET(ret_sonix);

    TAKE_SEM_WITH_COUNT(fat->sem, fat->sem_cnt);
    ret_sonix = _m_file_check_eof(fh, wrapper, fp);
    GIVE_SEM_WITH_COUNT(fat->sem, fat->sem_cnt);
    CHECK_MIDDLEWARE_RET(ret_sonix);

    return HAL_OK;
}

static int32_t _m_file_delete(M_FILE_PATH_T del)
{
    int32_t         ret_sonix = HAL_OK;
    M_FILE_HANDLE_T fh        = {0};

    // check file name
    TCHAR path_name[FILE_NAME_LEN] = {0};
    ret_sonix                      = _m_file_check_name(path_name, del.dev, del.full_path);
    CHECK_MIDDLEWARE_RET(ret_sonix);

    CREATE_SEM_WITH_COUNT(MFile_res.sem, MFile_res.sem_cnt);
    TAKE_SEM_WITH_COUNT(MFile_res.sem, MFile_res.sem_cnt);
    ret_sonix = _m_file_init(&fh, del.dev, path_name);
    GIVE_SEM_WITH_COUNT(MFile_res.sem, MFile_res.sem_cnt);
    CHECK_MIDDLEWARE_RET(ret_sonix);

    FAT_RES_T *fat = MFile_res.fat_set[fh.dev];
    CHECK_POINTER_NULL(fat);
    FIL_RES_T *fp = fat->fil_set[fh.index];
    TAKE_SEM_WITH_COUNT(fat->sem, fat->sem_cnt);
    ret_sonix = _file_delete(fp->name);
    GIVE_SEM_WITH_COUNT(fat->sem, fat->sem_cnt);
    CHECK_MIDDLEWARE_RET(ret_sonix);

    ret_sonix = _m_file_uninit(fh);
    CHECK_MIDDLEWARE_RET(ret_sonix);

    return HAL_OK;
}

static int32_t _m_file_get_status(M_FILE_HANDLE_T fh, M_FILE_STATUS_T *status)
{
    CHECK_POINTER_NULL(status);
    int32_t    ret_sonix = HAL_OK;
    FAT_RES_T *fat       = MFile_res.fat_set[fh.dev];
    CHECK_POINTER_NULL(fat);
    FIL_RES_T *fp = fat->fil_set[fh.index];
    CHECK_POINTER_NULL(fp);
    TYPE_WRAPPER *wrapper = Wrapper_Manager[fp->format];
    CHECK_POINTER_NULL(wrapper);

    status->format = fp->format;
    status->length = f_size(&fp->fp);
    status->name   = (const TCHAR *)&fp->name;
    TAKE_SEM_WITH_COUNT(fat->sem, fat->sem_cnt);
    switch (fp->format)
    {
        case FILE_TYPE_BIN:
        case FILE_TYPE_TXT:
            ret_sonix = wrapper->GetStatus(fp, &status->bin);
            break;
        case FILE_TYPE_WAV:
            ret_sonix = wrapper->GetStatus(fp, &status->wav);
            break;
        case FILE_TYPE_A32:
            ret_sonix = wrapper->GetStatus(fp, &status->a32);
            break;
        case FILE_TYPE_MP3:
            ret_sonix = wrapper->GetStatus(fp, &status->mp3);
            break;
        case FILE_TYPE_BMP:
            ret_sonix = wrapper->GetStatus(fp, &status->bmp);
            break;
        case FILE_TYPE_JPG:
            ret_sonix = wrapper->GetStatus(fp, &status->jpg);
            break;
        case FILE_TYPE_GIF:
            ret_sonix = wrapper->GetStatus(fp, &status->gif);
            break;
        case FILE_TYPE_AVI:
            ret_sonix = wrapper->GetStatus(fp, &status->avi);
            break;
        default:
            ret_sonix = HAL_ERROR;
    }
    GIVE_SEM_WITH_COUNT(fat->sem, fat->sem_cnt);
    CHECK_MIDDLEWARE_RET(ret_sonix);

    return HAL_OK;
}

/////////////////////////////////////////////// dir operation ///////////////////////////////////////////////
static int32_t _m_file_opendir(M_DIR_HANDLE_T *dp, M_FILE_PATH_T *path)
{
    CHECK_POINTER_NULL(dp);
    CHECK_POINTER_NULL(path);
    int32_t ret_sonix                = HAL_OK;
    TCHAR   path_name[FILE_NAME_LEN] = {0};

    ret_sonix = _m_file_check_name(path_name, path->dev, path->full_path);
    CHECK_MIDDLEWARE_RET(ret_sonix);
    CREATE_SEM_WITH_COUNT(MFile_res.sem, MFile_res.sem_cnt);
    TAKE_SEM_WITH_COUNT(MFile_res.sem, MFile_res.sem_cnt);
    ret_sonix = _m_file_init(&dp->fh, path->dev, path_name);
    GIVE_SEM_WITH_COUNT(MFile_res.sem, MFile_res.sem_cnt);
    CHECK_MIDDLEWARE_RET(ret_sonix);

    FAT_RES_T *fat = MFile_res.fat_set[dp->fh.dev];
    CHECK_POINTER_NULL(fat);
    FIL_RES_T *fp = fat->fil_set[dp->fh.index];
    TAKE_SEM_WITH_COUNT(fat->sem, fat->sem_cnt);
    ret_sonix = _file_opendir(&dp->dir, fp->name);
    GIVE_SEM_WITH_COUNT(fat->sem, fat->sem_cnt);
    CHECK_MIDDLEWARE_RET(ret_sonix);
    return HAL_OK;
}

static int32_t _m_file_closedir(M_DIR_HANDLE_T *dp)
{
    CHECK_POINTER_NULL(dp);
    int32_t    ret_sonix = HAL_OK;
    FAT_RES_T *fat       = MFile_res.fat_set[dp->fh.dev];
    CHECK_POINTER_NULL(fat);

    TAKE_SEM_WITH_COUNT(fat->sem, fat->sem_cnt);
    ret_sonix = _file_closedir(&dp->dir);
    GIVE_SEM_WITH_COUNT(fat->sem, fat->sem_cnt);
    CHECK_MIDDLEWARE_RET(ret_sonix);

    ret_sonix = _m_file_uninit(dp->fh);
    CHECK_MIDDLEWARE_RET(ret_sonix);
    return HAL_OK;
}

static int32_t _m_file_makedir(M_FILE_PATH_T *path)
{
    CHECK_POINTER_NULL(path);
    int32_t         ret_sonix = HAL_OK;
    M_FILE_HANDLE_T fh        = {0};

    // check file name
    TCHAR path_name[FILE_NAME_LEN] = {0};
    ret_sonix                      = _m_file_check_name(path_name, path->dev, path->full_path);
    CHECK_MIDDLEWARE_RET(ret_sonix);

    CREATE_SEM_WITH_COUNT(MFile_res.sem, MFile_res.sem_cnt);
    TAKE_SEM_WITH_COUNT(MFile_res.sem, MFile_res.sem_cnt);
    ret_sonix = _m_file_init(&fh, path->dev, path_name);
    GIVE_SEM_WITH_COUNT(MFile_res.sem, MFile_res.sem_cnt);
    CHECK_MIDDLEWARE_RET(ret_sonix);

    FAT_RES_T *fat = MFile_res.fat_set[fh.dev];
    CHECK_POINTER_NULL(fat);
    FIL_RES_T *fp = fat->fil_set[fh.index];
    TAKE_SEM_WITH_COUNT(fat->sem, fat->sem_cnt);
    ret_sonix = _file_mkdir(fp->name);
    GIVE_SEM_WITH_COUNT(fat->sem, fat->sem_cnt);
    CHECK_MIDDLEWARE_RET(ret_sonix);

    ret_sonix = _m_file_uninit(fh);
    CHECK_MIDDLEWARE_RET(ret_sonix);
    return HAL_OK;
}

static int32_t _m_file_readdir(M_DIR_HANDLE_T *dp, M_DIR_ITEM_T *fno)
{
    CHECK_POINTER_NULL(dp);
    CHECK_POINTER_NULL(fno);
    int32_t    ret_sonix = HAL_OK;
    FAT_RES_T *fat       = MFile_res.fat_set[dp->fh.dev];
    CHECK_POINTER_NULL(fat);

    TAKE_SEM_WITH_COUNT(fat->sem, fat->sem_cnt);
    ret_sonix = _file_readdir(&dp->dir, &fno->fno);
    GIVE_SEM_WITH_COUNT(fat->sem, fat->sem_cnt);
    CHECK_MIDDLEWARE_RET(ret_sonix);

    if (fno->fno.fname[0] == 0)
    {
        return M_FILE_ERROR_NO_MORE_FILE;
    }
    if (fno->fno.fattrib & AM_DIR)
    {
        fno->attr = DIR_ATTR_DIR;
    }
    else
    {
        fno->attr = DIR_ATTR_FIL;
    }
    return HAL_OK;
}

static int32_t _m_file_find_first(M_DIR_HANDLE_T *dp, M_DIR_ITEM_T *fno, M_FILE_PATH_T *path, const TCHAR *pat)
{
    CHECK_POINTER_NULL(dp);
    CHECK_POINTER_NULL(fno);
    CHECK_POINTER_NULL(pat);
    CHECK_POINTER_NULL(path);
    CHECK_POINTER_NULL(path->full_path);
    int32_t ret_sonix = HAL_OK;

    TCHAR path_name[FILE_NAME_LEN] = {0};
    ret_sonix                      = _m_file_check_name(path_name, path->dev, path->full_path);
    CHECK_MIDDLEWARE_RET(ret_sonix);
    CREATE_SEM_WITH_COUNT(MFile_res.sem, MFile_res.sem_cnt);
    TAKE_SEM_WITH_COUNT(MFile_res.sem, MFile_res.sem_cnt);
    ret_sonix = _m_file_init(&dp->fh, path->dev, path_name);
    GIVE_SEM_WITH_COUNT(MFile_res.sem, MFile_res.sem_cnt);
    CHECK_MIDDLEWARE_RET(ret_sonix);

    FAT_RES_T *fat = MFile_res.fat_set[dp->fh.dev];
    CHECK_POINTER_NULL(fat);
    FIL_RES_T *fp = fat->fil_set[dp->fh.index];

    TAKE_SEM_WITH_COUNT(fat->sem, fat->sem_cnt);
    ret_sonix = _file_findfirst(&dp->dir, &fno->fno, fp->name, pat);
    GIVE_SEM_WITH_COUNT(fat->sem, fat->sem_cnt);
    CHECK_MIDDLEWARE_RET(ret_sonix);
    if (fno->fno.fname[0] == 0)
    {
        return M_FILE_ERROR_NO_MORE_FILE;
    }
    return HAL_OK;
}

static int32_t _m_file_find_next(M_DIR_HANDLE_T *dp, M_DIR_ITEM_T *fno)
{
    CHECK_POINTER_NULL(dp);
    CHECK_POINTER_NULL(fno);
    int32_t    ret_sonix = HAL_OK;
    FAT_RES_T *fat       = MFile_res.fat_set[dp->fh.dev];
    CHECK_POINTER_NULL(fat);

    TAKE_SEM_WITH_COUNT(fat->sem, fat->sem_cnt);
    ret_sonix = _file_findnext(&dp->dir, &fno->fno);
    GIVE_SEM_WITH_COUNT(fat->sem, fat->sem_cnt);
    CHECK_MIDDLEWARE_RET(ret_sonix);
    return HAL_OK;
}

static int32_t _m_file_change_time(M_FILE_PATH_T *path, M_DIR_ITEM_T *fno)
{
    CHECK_POINTER_NULL(path);
    CHECK_POINTER_NULL(fno);
    int32_t         ret_sonix = HAL_OK;
    M_FILE_HANDLE_T fh        = {0};

    // check file name
    TCHAR path_name[FILE_NAME_LEN] = {0};
    ret_sonix                      = _m_file_check_name(path_name, path->dev, path->full_path);
    CHECK_MIDDLEWARE_RET(ret_sonix);

    CREATE_SEM_WITH_COUNT(MFile_res.sem, MFile_res.sem_cnt);
    TAKE_SEM_WITH_COUNT(MFile_res.sem, MFile_res.sem_cnt);
    ret_sonix = _m_file_init(&fh, path->dev, path_name);
    GIVE_SEM_WITH_COUNT(MFile_res.sem, MFile_res.sem_cnt);
    CHECK_MIDDLEWARE_RET(ret_sonix);

    FAT_RES_T *fat = MFile_res.fat_set[fh.dev];
    CHECK_POINTER_NULL(fat);
    FIL_RES_T *fp = fat->fil_set[fh.index];
    TAKE_SEM_WITH_COUNT(fat->sem, fat->sem_cnt);
    fno->fno.fdate = (WORD)(((fno->year - 1980) * 512U) | fno->month * 32U | fno->mday);
    fno->fno.ftime = (WORD)(fno->hour * 2048U | fno->min * 32U | fno->sec / 2U);
    ret_sonix      = _file_utime(fp->name, &fno->fno);
    GIVE_SEM_WITH_COUNT(fat->sem, fat->sem_cnt);
    CHECK_MIDDLEWARE_RET(ret_sonix);

    ret_sonix = _m_file_uninit(fh);
    CHECK_MIDDLEWARE_RET(ret_sonix);
    return HAL_OK;
}

/////////////////////////////////////////////// interface ///////////////////////////////////////////////
int32_t HAL_MFile_DiskPart(FATFS_DISK_T *disk)
{
    return _m_file_disk(disk);
};
int32_t HAL_MFile_DiskFind(FATFS_LOGIC_DEV_T *logic, FATFS_PHY_DEV_T phy, FATFS_LOGIC_PART_T part)
{
    return _m_file_disk_find(logic, phy, part);
};
int32_t HAL_MFile_GetDiskStatus(FATFS_DISK_STATUS_T *status)
{
    return _m_file_disk_status(status);
};

int32_t HAL_MFile_OpenFile(M_FILE_HANDLE_T *fh, M_FILE_INIT_T *init)
{
    return _m_file_open(fh, init);
};
int32_t HAL_MFile_CloseFile(M_FILE_HANDLE_T fh)
{
    return _m_file_close(fh);
};
int32_t HAL_MFile_ReadFile(M_FILE_HANDLE_T *fh, M_FILE_INFO_T *info, int8_t is_blocking)
{
    return _m_file_read(fh, info, is_blocking);
};
int32_t HAL_MFile_WriteFile(M_FILE_HANDLE_T *fh, M_FILE_INFO_T *info, int8_t is_blocking)
{
    return _m_file_write(fh, info, is_blocking);
};
int32_t HAL_MFile_WaitFile(M_FILE_HANDLE_T fh)
{
    return _m_file_wait(fh);
};
int32_t HAL_MFile_SeekFile(M_FILE_HANDLE_T *fh, M_FILE_SEEK_T seek)
{
    return _m_file_seek(fh, seek);
};
int32_t HAL_MFile_GetFileStatus(M_FILE_HANDLE_T fh, M_FILE_STATUS_T *status)
{
    return _m_file_get_status(fh, status);
};

int32_t HAL_MFile_OpenDir(M_DIR_HANDLE_T *dp, M_FILE_PATH_T *path)
{
    return _m_file_opendir(dp, path);
};
int32_t HAL_MFile_CloseDir(M_DIR_HANDLE_T *dp)
{
    return _m_file_closedir(dp);
};
int32_t HAL_MFile_MakeDir(M_FILE_PATH_T *path)
{
    return _m_file_makedir(path);
};
int32_t HAL_MFile_ReadDir(M_DIR_HANDLE_T *dp, M_DIR_ITEM_T *fno)
{
    return _m_file_readdir(dp, fno);
};
int32_t HAL_MFile_FindFirst(M_DIR_HANDLE_T *dp, M_DIR_ITEM_T *fno, M_FILE_PATH_T *path, const TCHAR *pat)
{
    return _m_file_find_first(dp, fno, path, pat);
};
int32_t HAL_MFile_FindNext(M_DIR_HANDLE_T *dp, M_DIR_ITEM_T *fno)
{
    return _m_file_find_next(dp, fno);
};
int32_t HAL_MFile_DeleteDir(M_FILE_PATH_T del)
{
    return _m_file_delete(del);
};
int32_t HAL_MFile_ChangeDirTime(M_FILE_PATH_T *path, M_DIR_ITEM_T *fno)
{
    return _m_file_change_time(path, fno);
};

#endif
