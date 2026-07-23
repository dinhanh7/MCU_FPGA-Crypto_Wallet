/***********************
 * SN34F78X SDK V2.1.4 *
 ***********************/

#include "file_wrapper.h"

#ifndef SONIX_RTE_CFG_VER
#undef FILE_STORAGE_USAGE
#define FILE_STORAGE_USAGE ((1 << 1) | (1 << 2) | (1 << 4))
#else
#define FILE_STORAGE_USAGE (configUSE_FILE_STORAGE)
#endif
#define FILE_MERGE_BUFF_SIZE (0x400)
#define CHECK_FATFS_RET(ret)              \
    if (ret != FR_OK)                     \
    {                                     \
        return _file_dispatch_error(ret); \
    }

//=============================================================================
//                  Private Function Definition
//=============================================================================

//////////////////////////////////////////////////*** file operation ***//////////////////////////////////////////////////
static int32_t _file_dispatch_error(FRESULT ret_fatfs)
{
    switch (ret_fatfs)
    {
        case FR_OK:
            return HAL_OK;
        case FR_DISK_ERR:
        case FR_INT_ERR:
        case FR_NOT_READY:
        case FR_NO_FILE:
        case FR_NO_PATH:
        case FR_INVALID_NAME:
        case FR_DENIED:
        case FR_EXIST:
        case FR_INVALID_OBJECT:
        case FR_WRITE_PROTECTED:
        case FR_INVALID_DRIVE:
        case FR_NOT_ENABLED:
        case FR_NO_FILESYSTEM:
        case FR_MKFS_ABORTED:
        case FR_TIMEOUT:
        case FR_LOCKED:
        case FR_NOT_ENOUGH_CORE:
        case FR_TOO_MANY_OPEN_FILES:
        case FR_INVALID_PARAMETER:
            return ret_fatfs;

        default:
            return HAL_ERROR;
    }
}

static int32_t _file_disk_map(FAT_MAP_T *DiskMap)
{
#if (FF_MULTI_PARTITION)
    memset(&VolToPart, 0, sizeof(PARTITION) * FF_VOLUMES);
    for (uint8_t index = 0; index < DiskMap->part_num; index++)
    {
        if (DiskMap->part[index].logic < (FATFS_LOGIC_DEV_T)(FF_VOLUMES))
        {
            VolToPart[DiskMap->part[index].logic].pd = DiskMap->part[index].dev;
            VolToPart[DiskMap->part[index].logic].pt = DiskMap->part[index].part + DiskMap->part[index].multi;
            VolToPart[DiskMap->part[index].logic].mu = 1;
        }
    }
#endif
    return HAL_OK;
}

int32_t _file_get_free(FATFS *fat, uint8_t *fat_work, const TCHAR *name, DWORD *nclst)
{
    FRESULT result = FR_OK;
    result         = f_getfree(name, nclst, &fat);
    CHECK_FATFS_RET(result);
    return HAL_OK;
}

static int32_t _file_disk_info(FAT_MAP_T *DiskMap, TCHAR *disk_name, uint32_t logic)
{
    int32_t    ret_sonix = HAL_OK;
    FAT_RES_T *fat       = NULL;
    MEMORY_ALLOC(fat, sizeof(FAT_RES_T), GFP_KERNEL);
    ret_sonix = _file_mount(&fat->fat, fat->work, disk_name);
    if (ret_sonix != HAL_OK)
    {
        goto __EXIT_DISK_PART;
    }
    ret_sonix = _file_get_label(&fat->fat, fat->work, disk_name, (TCHAR *)&DiskMap->part[logic].label);
    if (ret_sonix != HAL_OK)
    {
        goto __EXIT_DISK_PART;
    }
    ret_sonix = _file_get_free(&fat->fat, fat->work, disk_name, &DiskMap->part[logic].free);
    if (ret_sonix != HAL_OK)
    {
        goto __EXIT_DISK_PART;
    }
    ret_sonix = _file_mount(NULL, fat->work, disk_name);
    if (ret_sonix != HAL_OK)
    {
        goto __EXIT_DISK_PART;
    }

    MEMORY_FREE(fat);
    return HAL_OK;
__EXIT_DISK_PART:
    MEMORY_FREE(fat);
    return ret_sonix;
}

static int32_t _file_disk_partition(uint8_t dev, FAT_MAP_T *DiskMap)
{
    if ((FILE_STORAGE_USAGE & (1 << dev)) == 0)
    {
        return HAL_ERROR;
    }
    if (DiskMap->is_init[dev])
    {
        return HAL_OK;
    }
    DiskMap->is_init[dev] = 1;

    FAT_MBR_DPT_T mbr_dpt[FATFS_LOGIC_PARTM] = {0};
    memset(&mbr_dpt, 0, sizeof(mbr_dpt));
    if (disk_get_partition(dev, (sizeof(FAT_MBR_DPT_T) / sizeof(uint8_t)) * FATFS_LOGIC_PARTM, (uint8_t *)&mbr_dpt) != RES_OK)
    {
        return HAL_ERROR;
    }
    uint32_t sector = 0;
    if (disk_ioctl(dev, GET_SECTOR_SIZE, &sector) != RES_OK)
    {
        return HAL_ERROR;
    }
    uint32_t sector_cnt = 0;
    if (disk_ioctl(dev, GET_SECTOR_COUNT, &sector_cnt) != RES_OK)
    {
        return HAL_ERROR;
    }
    TCHAR    disk_name[FILE_NAME_LEN] = {dev + '0', ':'};
    int32_t  ret_sonix                = HAL_OK;
    uint32_t sector_step              = 0;
    uint32_t logic                    = DiskMap->part_num;
    for (uint8_t part = 0; part < FATFS_LOGIC_PARTM; part++)
    {
        if (mbr_dpt[part].System != 0x00 && //
            mbr_dpt[part].System != 0x05 && //
            mbr_dpt[part].System != 0x0F && //
            mbr_dpt[part].System != 0x85)   // not support extended partition
        {
            ret_sonix = HAL_ERROR;
            if (FF_MULTI_PARTITION && (sector_step + mbr_dpt[part].LbaLen <= sector_cnt))
            {
                if (mbr_dpt[part].boot == 0x00 || mbr_dpt[part].boot == 0x80)
                {
                    VolToPart[0].pd           = dev;
                    VolToPart[0].pt           = part + 1;
                    ret_sonix                 = _file_disk_info(DiskMap, disk_name, logic);
                    DiskMap->part[logic].size = mbr_dpt[part].LbaLen;
                    sector_step += mbr_dpt[part].LbaLen;
                    DiskMap->part[logic].multi = 1;
                }
            }
            if (ret_sonix != HAL_OK)
            {
                logic -= part;
                memset(&disk_name, 0, sizeof(disk_name));
                sprintf(disk_name, "%d:", dev);
                DiskMap->part[logic].multi = 0;
                ret_sonix                  = _file_disk_info(DiskMap, disk_name, logic);
                DiskMap->part[logic].size  = sector_cnt;
                sector_step                = sector_cnt;
            }
            CHECK_MIDDLEWARE_RET(ret_sonix);
            DiskMap->part[logic].dev    = (FATFS_PHY_DEV_T)dev;
            DiskMap->part[logic].part   = (FATFS_LOGIC_PART_T)part;
            DiskMap->part[logic].sector = sector;
            DiskMap->part[logic].logic  = (FATFS_LOGIC_DEV_T)((logic < FF_VOLUMES) ? logic : FF_VOLUMES);
            logic++;
            if (sector_step >= sector_cnt)
            {
                break;
            }
        }
    }

    DiskMap->part_num = logic;
    _file_disk_map(DiskMap);
    return HAL_OK;
}

// static int32_t _file_disk_remap(FATFS_LOGIC_DEV_T *logic, FATFS_PHY_DEV_T dev, FATFS_LOGIC_PART_T part, FAT_MAP_T *DiskMap)
// {
// 	int32_t ret_sonix = HAL_OK;
// 	FATFS_LOGIC_DEV_T *logic_aim;
// 	ret_sonix = _file_disk_find(&logic_aim, dev, part, DiskMap);
// 	CHECK_MIDDLEWARE_RET(ret_sonix);
// 	FATFS_LOGIC_DEV_T *logic_free;
// 	ret_sonix = _file_disk_find_free(&logic_free, DiskMap);
// 	CHECK_MIDDLEWARE_RET(ret_sonix);
// 	ret_sonix = _file_disk_logic_ex(logic_aim, logic_free, DiskMap);
// 	CHECK_MIDDLEWARE_RET(ret_sonix);
// 	ret_sonix = _file_disk_map();
// 	CHECK_MIDDLEWARE_RET(ret_sonix);
// 	*logic = logic_free;
// 	return HAL_OK;
// }

//=============================================================================
//                  Public Function Definition
//=============================================================================

//////////////////////////////////////////////////*** file operation ***//////////////////////////////////////////////////
int32_t _file_seek(FIL *fp, uint64_t seek)
{
    FRESULT result = FR_OK;
    result         = f_lseek(fp, seek);
    CHECK_FATFS_RET(result);
    return HAL_OK;
}

int32_t _file_delete(const TCHAR *path)
{
    if (path)
    {
        DIR     dir                         = {0};
        FILINFO fno                         = {0};
        TCHAR   path_kid[FILE_NAME_LEN + 2] = {0};
        FRESULT result                      = f_opendir(&dir, path);
        if (result == FR_OK)
        {
            while (FR_OK == f_readdir(&dir, &fno))
            {
                if (0 == strlen(fno.fname))
                    break;
                if (0 == strcmp(fno.fname, "."))
                    continue;
                if (0 == strcmp(fno.fname, ".."))
                    continue;

                memset(path_kid, 0, sizeof(path_kid));
                sprintf(path_kid, "%s/%s", path, fno.fname);
                if (fno.fattrib & AM_DIR)
                {
                    int32_t ret_sonix = HAL_ERROR;
                    ret_sonix         = _file_delete(path_kid);
                    CHECK_MIDDLEWARE_RET(ret_sonix);
                }
                else
                {
                    result = f_unlink(path_kid);
                }
            }
            result = f_closedir(&dir);
            CHECK_FATFS_RET(result);
        }
        result = f_unlink(path);
    }
    return HAL_OK;
}

int32_t _file_close(FIL *fp, const TCHAR *name)
{
    MEMORY_FREE(fp->cltbl);
    if (f_close(fp) == FR_OK)
    {
        if (name)
        {
            f_unlink(name);
        }
    }
    return HAL_OK;
}

int32_t _file_open(FIL *fp, const TCHAR *name, uint8_t mode)
{
    FRESULT result = f_open(fp, name, mode);
    CHECK_FATFS_RET(result);
    return HAL_OK;
}

int32_t _file_create(FIL *fp, const TCHAR *name)
{
    FRESULT result = f_open(fp, name, FA_READ | FA_WRITE | FA_CREATE_ALWAYS);
    CHECK_FATFS_RET(result);
    return HAL_OK;
}

int32_t _file_seek_and_read(uint64_t seek, uint32_t *read_size, void *read_buff, FIL *fp)
{
    FRESULT result = FR_OK;
    if (FILE_NOT_SEEK != seek)
    {
        result = f_lseek(fp, seek);
        CHECK_FATFS_RET(result);
    }
    uint32_t length   = 0;
    uint32_t read_tmp = *read_size;
    while (read_tmp)
    {
        read_buff = (uint8_t *)read_buff + length;
        result    = f_read(fp, read_buff, read_tmp, &length);
        CHECK_FATFS_RET(result);
        if (length == 0)
        {
            if (f_eof(fp))
            {
                *read_size -= read_tmp;
                return HAL_OK;
            }
            else
            {
                return HAL_ERROR;
            }
        }
        read_tmp -= length;
    }
    return HAL_OK;
}

int32_t _file_seek_and_write(uint64_t seek, uint32_t write_size, void *write_buff, FIL *fp)
{
    if (write_buff == NULL)
    {
        return HAL_ERROR;
    }

    FRESULT result = FR_OK;
    if (FILE_NOT_SEEK != seek)
    {
        result = f_lseek(fp, seek);
        CHECK_FATFS_RET(result);
    }

    uint32_t length = 0;
    while (write_size)
    {
        write_buff = (uint8_t *)write_buff + length;
        result     = f_write(fp, write_buff, write_size, &length);
        CHECK_FATFS_RET(result);
        if (length == 0)
        {
            return FILE_ERROR_OVER_SIZE;
        }
        write_size -= length;
    }
    return HAL_OK;
}

int32_t _file_merge(FIL *des_file, FIL *src_file)
{
    FRESULT  result        = FR_OK;
    uint32_t src_file_size = f_size(src_file);
    result                 = f_lseek(src_file, 0);
    CHECK_FATFS_RET(result);
    uint32_t des_file_size = f_size(des_file);
    result                 = f_lseek(des_file, des_file_size);
    CHECK_FATFS_RET(result);
    uint32_t length      = 0;
    uint32_t left_size   = 0;
    uint32_t merge_size  = 0;
    uint32_t merged_size = 0;
    uint8_t *file_temp   = NULL;
    MEMORY_ALLOC(file_temp, FILE_MERGE_BUFF_SIZE, GFP_KERNEL);
    while (merged_size < src_file_size)
    {
        left_size = src_file_size - merged_size;
        if (left_size >= FILE_MERGE_BUFF_SIZE)
        {
            merge_size = FILE_MERGE_BUFF_SIZE;
        }
        else
        {
            merge_size = left_size;
        }
        result = f_read(src_file, file_temp, merge_size, &length);
        CHECK_FATFS_RET(result);
        result = f_write(des_file, file_temp, merge_size, &length);
        CHECK_FATFS_RET(result);
        merged_size += merge_size;
    }
    MEMORY_FREE(file_temp);

    return HAL_OK;
}

//////////////////////////////////////////////////*** file parser operation ***//////////////////////////////////////////////////
int32_t _file_get_nbyte_kernal(uint8_t *data, uint32_t size, FILE_PARSER_T *parser, FIL *fp)
{
    int32_t  ret_sonix = HAL_OK;
    uint16_t tail_num  = parser->buff_size - parser->buff_pos;
    if (tail_num <= size)
    {
        if (data)
        {
            memcpy(data, parser->buff_addr + parser->buff_pos, tail_num);
        }
        ret_sonix = _file_seek_and_read(FILE_NOT_SEEK, &parser->buff_size, parser->buff_addr, fp);
        CHECK_MIDDLEWARE_RET(ret_sonix);
        parser->buff_pos = 0;
        parser->buff_cnt++;
        if (parser->buff_size == 0)
        {
            return ret_sonix;
        }
    }
    else
    {
        tail_num = 0;
    }
    if (data)
    {
        memcpy(data + tail_num, parser->buff_addr + parser->buff_pos, size - tail_num);
    }
    parser->buff_pos += size - tail_num;
    return ret_sonix;
}

int32_t _file_rvt_nbyte(uint32_t size, FILE_PARSER_T *parser, FIL *fp)
{
    if (size <= parser->buff_pos)
    {
        parser->buff_pos -= size;
    }
    else
    {
        int32_t ret_sonix = HAL_OK;
        size -= parser->buff_pos;
        for (; size >= parser->buff_size; size -= parser->buff_size)
        {
            ret_sonix = _file_seek(fp, f_tell(fp) - parser->buff_size);
            CHECK_MIDDLEWARE_RET(ret_sonix);
            if (parser->buff_cnt)
            {
                parser->buff_cnt--;
            }
        }
        ret_sonix = _file_seek_and_read(f_tell(fp) - parser->buff_size, &parser->buff_size, parser->buff_addr, fp);
        CHECK_MIDDLEWARE_RET(ret_sonix);
        parser->buff_pos = parser->buff_size - size;
    }
    return HAL_OK;
}

int32_t _file_get_nbyte(uint8_t *data, uint32_t size, FILE_PARSER_T *parser, FIL *fp)
{
    CHECK_POINTER_NULL(parser);
    CHECK_POINTER_NULL(fp);
    if (parser->buff_addr == NULL)
    {
        parser->buff_size = FILE_PARSE_BUFF_SIZE;
        parser->buff_pos  = parser->buff_size;
        MEMORY_ALLOC(parser->buff_addr, parser->buff_size, GFP_SRAM);
        parser->buff_cnt = 0;
    }
    int32_t  ret_sonix = HAL_OK;
    uint16_t index     = 0;
    uint8_t *p_data    = data;
    for (; size > (index + 1) * parser->buff_size; index++)
    {
        if (p_data)
        {
            p_data = data + index * parser->buff_size;
        }
        ret_sonix = _file_get_nbyte_kernal(p_data, parser->buff_size, parser, fp);
        CHECK_MIDDLEWARE_RET(ret_sonix);
    }
    if (p_data)
    {
        p_data = data + index * parser->buff_size;
    }
    ret_sonix = _file_get_nbyte_kernal(p_data, size - index * parser->buff_size, parser, fp);
    CHECK_MIDDLEWARE_RET(ret_sonix);
    return HAL_OK;
}

int32_t _file_get_nword_ex(uint16_t *data, uint32_t size, FILE_PARSER_T *parser, FIL *fp)
{
    int32_t ret_sonix = HAL_OK;
    ret_sonix         = _file_get_nword(data, size, parser, fp);
    CHECK_MIDDLEWARE_RET(ret_sonix);
    for (uint16_t i = 0; i < size; i++)
    {
        data[i] = FILE_EXCHANGE_BYTE(data[i]);
    }
    return ret_sonix;
}

//////////////////////////////////////////////////*** Disk operation ***//////////////////////////////////////////////////
int32_t _file_fast_seek(FIL *fp, const TCHAR *name, FATFS *fat)
{
    /* Enable fast seek mode (cltbl != NULL) */
    if (fp && fp->cltbl == NULL)
    {
        uint32_t frag_size = (fat->csize + 1) * 2; /* ff suggest size ((number of the file fragments + 1) * 2) */
        fp->cltbl          = (DWORD *)pvPortMalloc(frag_size * sizeof(DWORD));
        CHECK_POINTER_NULL(fp->cltbl);
        memset(fp->cltbl, 0, frag_size * sizeof(DWORD));
        fp->cltbl[0]   = frag_size;                   /* Set table size */
        FRESULT result = f_lseek(fp, CREATE_LINKMAP); /* Create cluster link map table */
        if (result != FR_OK)
        {
            MEMORY_FREE(fp->cltbl);
            return _file_dispatch_error(result);
        }
    }
    return HAL_OK;
}

int32_t _file_make_fs(const TCHAR *name, const MKFS_PARM *opt)
{
#if (FF_USE_MKFS == 1)
    uint8_t work[FF_MAX_SS] = {0};
    FRESULT result          = f_mkfs(name, opt, work, FF_MAX_SS);
    CHECK_FATFS_RET(result);
    return HAL_OK;
#else
    return HAL_ERROR;
#endif
}

int32_t _file_set_label(FATFS *fat, uint8_t *fat_work, const TCHAR *name)
{
    FRESULT result    = FR_OK;
    int32_t ret_sonix = HAL_OK;
    ret_sonix         = _file_mount(fat, fat_work, name);
    CHECK_MIDDLEWARE_RET(ret_sonix);
    result = f_setlabel(name);
    CHECK_FATFS_RET(result);
    ret_sonix = _file_mount(NULL, fat_work, name);
    CHECK_MIDDLEWARE_RET(ret_sonix);
    return ret_sonix;
}

int32_t _file_get_label(FATFS *fat, uint8_t *fat_work, const TCHAR *name, TCHAR *label)
{
    FRESULT result = FR_OK;
    result         = f_getlabel(name, label, 0);
    CHECK_FATFS_RET(result);
    return HAL_OK;
}

int32_t _file_mount(FATFS *fat, uint8_t *fat_work, const TCHAR *name)
{
    FRESULT result            = FR_OK;
    uint8_t mount_immediately = 1;
    if (!fat)
    {
        result = f_unmount(name);
        CHECK_FATFS_RET(result);
    }
    else
    {
        result = f_mount(fat, name, mount_immediately);
        if (result != FR_OK)
        {
            if (result == FR_NO_FILESYSTEM)
            {
#if (FF_USE_MKFS == 1)
                int32_t ret_sonix = _file_make_fs(name, 0);
                CHECK_MIDDLEWARE_RET(ret_sonix);
                result = f_mount(fat, name, mount_immediately);
                CHECK_FATFS_RET(result);
#else
                return HAL_ERROR;
#endif
            }
            else
            {
                result = f_unmount(name);
                CHECK_FATFS_RET(result);
                return HAL_ERROR;
            }
        }
    }
    return HAL_OK;
}

int32_t _file_disk(FATFS_DISK_T *disk, FAT_MAP_T *DiskMap)
{
#if FF_MULTI_PARTITION && FF_USE_MKFS
    CHECK_POINTER_NULL(disk);
    uint8_t index                    = 0;
    LBA_t   plist[FATFS_LOGIC_PARTM] = {0};

    DSTATUS status = RES_OK;
    status         = disk_initialize(disk->dev);
    if (status != RES_OK)
    {
        return HAL_ERROR;
    }
    uint32_t sector_all  = 0;
    uint32_t sector_size = 0;
    status               = disk_ioctl(disk->dev, GET_SECTOR_COUNT, &sector_all);
    if (status != RES_OK)
    {
        return HAL_ERROR;
    }

    status = disk_ioctl(disk->dev, GET_SECTOR_SIZE, &sector_size);
    if (status != RES_OK)
    {
        return HAL_ERROR;
    }

    uint32_t sector_free = sector_all;
    for (uint8_t i = 0; i < FATFS_LOGIC_PARTM; i++)
    {
        switch (disk->part[i].method)
        {
            case FATFS_PART_METHOD_NULL:
                break;
            case FATFS_PART_METHOD_SECTOR:
                if (disk->part[i].size < (1024 * 512 / sector_size))
                {
                    return HAL_ERROR;
                }
                plist[index++] = disk->part[i].size;
                if (sector_free < disk->part[i].size)
                {
                    return HAL_ERROR;
                }
                sector_free -= disk->part[i].size;
                break;
            case FATFS_PART_METHOD_RATE:
                if (disk->part[i].size > 100)
                {
                    return HAL_ERROR;
                }
                plist[index++] = disk->part[i].size;
                if (sector_free < disk->part[i].size * sector_all / 100)
                {
                    return HAL_ERROR;
                }
                sector_free -= disk->part[i].size * sector_all / 100;
                break;
            case FATFS_PART_METHOD_REST:
                plist[index++] = 100;
                i              = FATFS_LOGIC_PARTM;
                sector_free    = 0;
                break;
            default:
                return HAL_ERROR;
        }
    }

    int32_t ret_sonix       = HAL_OK;
    FRESULT result          = FR_OK;
    uint8_t work[FF_MAX_SS] = {0};
    result                  = f_fdisk(disk->dev, plist, work);
    CHECK_FATFS_RET(result);

    TCHAR      disk_name[FILE_NAME_LEN] = {0};
    FAT_RES_T *fat                      = NULL;
    MEMORY_ALLOC(fat, sizeof(FAT_RES_T), GFP_KERNEL);
    for (uint8_t i = 0; i < index; i++)
    {
        VolToPart[0].pd = disk->dev;
        VolToPart[0].pt = i + 1;
        VolToPart[0].mu = 1;
        memset(&disk_name, 0, sizeof(disk_name));
        sprintf(disk_name, "0:%s", disk->part[i].label);
        ret_sonix = _file_make_fs(disk_name, 0);
        if (ret_sonix != HAL_OK)
        {
            MEMORY_FREE(fat);
            CHECK_MIDDLEWARE_RET(ret_sonix);
        }
        ret_sonix = _file_set_label(&fat->fat, fat->work, disk_name);
        if (ret_sonix != HAL_OK)
        {
            MEMORY_FREE(fat);
            CHECK_MIDDLEWARE_RET(ret_sonix);
        }
    }
    DiskMap->is_init[disk->dev] = 0;
    ret_sonix                   = _file_disk_partition(disk->dev, DiskMap);
    CHECK_MIDDLEWARE_RET(ret_sonix);
    return HAL_OK;
#else
    return HAL_ERROR;
#endif
}

int32_t _file_disk_find(FATFS_LOGIC_DEV_T *logic, FATFS_PHY_DEV_T dev, FATFS_LOGIC_PART_T part, FAT_MAP_T *DiskMap)
{
    CHECK_POINTER_NULL(logic);
#if FF_MULTI_PARTITION
    if (part > FATFS_LOGIC_PART4)
#else
    if (part > FATFS_LOGIC_PART1)
#endif
    {
        return HAL_ERROR;
    }
    *logic            = (FATFS_LOGIC_DEV_T)(FATFS_LOGIC_PARTM);
    int32_t ret_sonix = HAL_OK;
    ret_sonix         = _file_disk_partition(dev, DiskMap);
    CHECK_MIDDLEWARE_RET(ret_sonix);
    for (uint8_t index = 0; index < DiskMap->part_num; index++)
    {
        if (DiskMap->part[index].dev == dev && DiskMap->part[index].part == part)
        {
            *logic = DiskMap->part[index].logic;
            break;
        }
    }
    if (*logic >= (FATFS_LOGIC_DEV_T)(FATFS_LOGIC_PARTM))
    {
        return HAL_ERROR;
    }
    return HAL_OK;
}

int32_t _file_disk_status(FATFS_DISK_STATUS_T *status, FAT_MAP_T *DiskMap)
{
    CHECK_POINTER_NULL(status);
    memset(status, 0, sizeof(FATFS_DISK_STATUS_T));
    int32_t ret_sonix = HAL_OK;
    for (uint8_t dev = 0; dev < DEV_MAX; dev++)
    {
        if (((FILE_STORAGE_USAGE >> dev) & 1))
        {
            ret_sonix = _file_disk_partition(dev, DiskMap);
            if (ret_sonix != HAL_OK)
            {
                DiskMap->is_init[dev] = 0;
            }
        }
    }
    for (uint8_t index = 0; index < DiskMap->part_num; index++)
    {
        uint16_t dev                   = DiskMap->part[index].dev;
        uint16_t part                  = DiskMap->part[index].part;
        status->part[dev][part].dev    = DiskMap->part[index].dev;
        status->part[dev][part].part   = DiskMap->part[index].part;
        status->part[dev][part].size   = DiskMap->part[index].size;
        status->part[dev][part].sector = DiskMap->part[index].sector;
        status->part[dev][part].logic  = DiskMap->part[index].logic;
        status->part[dev][part].free   = DiskMap->part[index].free;
        memcpy(status->part[dev][part].label, DiskMap->part[index].label, strlen(DiskMap->part[index].label));
    }
    return HAL_OK;
}

//////////////////////////////////////////////////*** Dir operation ***//////////////////////////////////////////////////
int32_t _file_opendir(DIR *dp, const TCHAR *path)
{
    FRESULT result = FR_OK;
    result         = f_opendir(dp, path);
    CHECK_FATFS_RET(result);
    return HAL_OK;
}

int32_t _file_closedir(DIR *dp)
{
    FRESULT __attribute__((unused)) result = FR_OK;
    result                                 = f_closedir(dp);
    // CHECK_FATFS_RET(result);
    return HAL_OK;
}

int32_t _file_readdir(DIR *dp, FILINFO *fno)
{
    FRESULT result = FR_OK;
    result         = f_readdir(dp, fno);
    CHECK_FATFS_RET(result);
    return HAL_OK;
}

int32_t _file_findfirst(DIR *dp, FILINFO *fno, const TCHAR *path, const TCHAR *pattern)
{
    FRESULT result = FR_OK;
    result         = f_findfirst(dp, fno, path, pattern);
    CHECK_FATFS_RET(result);
    return HAL_OK;
}

int32_t _file_findnext(DIR *dp, FILINFO *fno)
{
    FRESULT result = FR_OK;
    result         = f_findnext(dp, fno);
    CHECK_FATFS_RET(result);
    if (fno->fname[0] == 0)
    {
        return HAL_ERROR;
    }
    return HAL_OK;
}

int32_t _file_mkdir(const TCHAR *path)
{
    FRESULT result = FR_OK;
    result         = f_mkdir(path);
    CHECK_FATFS_RET(result);
    return HAL_OK;
}

int32_t _file_utime(const TCHAR *path, const FILINFO *fno)
{
    FRESULT result = FR_OK;
    result         = f_utime(path, fno);
    CHECK_FATFS_RET(result);
    return HAL_OK;
}
