/***********************
 * SN34F78X SDK V2.1.4 *
 ***********************/

#ifndef _FILE_WRAPPER_H_
#define _FILE_WRAPPER_H_

#include "File_Fat.h"
#include "FreeRTOS.h"
#include "semphr.h"

#if configUSE_AVI_FILE
#undef configUSE_WAV_FILE
#undef configUSE_JPEG_FILE
#define configUSE_WAV_FILE  (0x1) /**< avi file needed wav  file manager */
#define configUSE_JPEG_FILE (0x1) /**< avi file needed jpeg file manager */
#endif

#define GFP_KERNEL (NULL)
#define GFP_SRAM   (NULL)

#define FILE_IS_NOT_EOF        (0)
#define FILE_IS_EOF            (1)
#define FILE_IS_NON_BLOCKING   (0)
#define FILE_IS_BLOCKING       (1)
#define FILE_NOT_SEEK          ((uint64_t)(-1))
#define FILE_EXCHANGE_BYTE(x)  ((((x) & 0xff) << 8) | (((x) >> 8) & 0xff))                   // exchange motorola/intel  word ending
#define FILE_EXCHANGE_DWORD(x) ((FILE_EXCHANGE_BYTE(x) << 16) | FILE_EXCHANGE_BYTE(x >> 16)) // exchange motorola/intel dword ending
#define FILE_PARSE_BUFF_SIZE   (0x400)

#define CHECK_POINTER_NULL(ptr) \
    if (ptr == NULL)            \
    {                           \
        return HAL_ERROR;       \
    }
#define CHECK_MIDDLEWARE_RET(ret) \
    if (ret != HAL_OK)            \
    {                             \
        return ret;               \
    }

#define MFILE_DEBUG_ERROR(flag, fmt, args...)                                                 \
    if (flag)                                                                                 \
    {                                                                                         \
        printf("\r\n[MFILE_ERROR]" fmt ", Line:%d, File:%s\r\n", ##args, __LINE__, __FILE__); \
    }

#define MFILE_DEBUG_INFO(flag, fmt, args...)           \
    if (flag)                                          \
    {                                                  \
        printf("\r\n[MFILE_INFO]" fmt "\r\n", ##args); \
    }

#if configUSE_FILE_DEBUG
#else
#undef MFILE_DEBUG_ERROR
#undef MFILE_DEBUG_INFO
#define MFILE_DEBUG_ERROR(args...)
#define MFILE_DEBUG_INFO(args...)
#endif

#define PRINTF_HEAP_FREE_SIZE(str) MFILE_DEBUG_INFO(false, "[MEMORY_FREE]SRAM:0x%x, DRAM:0x%x, PTR:%x", xPortGetFreeHeapSize(GFP_SRAM), xPortGetFreeHeapSize(GFP_KERNEL), str)
#define MEMORY_FREE(ptr)            \
    if (ptr)                        \
    {                               \
        vPortFree(ptr);             \
        ptr = NULL;                 \
        PRINTF_HEAP_FREE_SIZE(ptr); \
    }

#define PRINTF_HEAP_ALLOC_ERROR(ptr, size) MFILE_DEBUG_ERROR(ptr == NULL, "[MFILE_ALLOC]SRAM:0x%x, DRAM:0x%x, PTR:%x, SIZE:%4d", xPortGetFreeHeapSize(GFP_SRAM), xPortGetFreeHeapSize(GFP_KERNEL), ptr, size);
#define MEMORY_ALLOC(ptr, size, where)      \
    if (ptr == NULL)                        \
    {                                       \
        ptr = pvPortMalloc(size);           \
        PRINTF_HEAP_ALLOC_ERROR(ptr, size); \
        CHECK_POINTER_NULL(ptr);            \
        memset(ptr, 0, size);               \
    }

// semaphore with reference count
#define TAKE_SEM_WITH_COUNT(sem, cnt)       \
    if (sem)                                \
    {                                       \
        cnt++;                              \
        xSemaphoreTake(sem, portMAX_DELAY); \
    }

#define GIVE_SEM_WITH_COUNT(sem, cnt) \
    if (sem)                          \
    {                                 \
        xSemaphoreGive(sem);          \
        cnt--;                        \
    }

#define CREATE_SEM_WITH_COUNT(sem, cnt) \
    if (sem == NULL)                    \
    {                                   \
        sem = xSemaphoreCreateBinary(); \
        CHECK_POINTER_NULL(sem);        \
        xSemaphoreGive(sem);            \
        cnt = 0;                        \
    }

#define DELETE_SEM_WITH_COUNT(sem, cnt) \
    if (sem && cnt == 0)                \
    {                                   \
        vSemaphoreDelete(sem);          \
        sem = NULL;                     \
    }

typedef enum _FILE_SRC_T
{
    FILE_SRC_FROM_FATFS, /**< source from fatfs */
    FILE_SRC_FROM_BUFF,  /**< source from buffer */
} FILE_SRC_T;

typedef struct _FAT_MBR_DPT_T
{
    uint8_t  boot;
    uint8_t  StHead;
    uint8_t  StSec;
    uint8_t  StCyl;
    uint8_t  System;
    uint8_t  EdHead;
    uint8_t  EdSec;
    uint8_t  EdCyl;
    uint32_t LbaBeg;
    uint32_t LbaLen;
} FAT_MBR_DPT_T;
typedef FATFS_PART_STATUS_T FATFS_PART_RES_T;

typedef struct _FAT_MAP_T
{
    uint8_t          is_init[DEV_MAX];                  /**< init flag */
    uint32_t         part_num;                          /**< Partition total number of all Physical drive */
    FATFS_PART_RES_T part[DEV_MAX * FATFS_LOGIC_PARTM]; /**< Partition info */
} FAT_MAP_T;

typedef struct _FIL_RES_T
{
    FIL           fp;                  /**< file handler of the file */
    void         *res;                 /**< file unique resource     */
    int8_t        busy;                /**< file is busy or not      */
    uint32_t      hash;                /**< file identification info */
    FILE_FORMAT_T format;              /**< file format of the file  */
    TCHAR         name[FILE_NAME_LEN]; /**< file name of the file    */
} FIL_RES_T;

typedef struct _FAT_RES_T
{
    FATFS             fat;                    /**< fat object */
    uint16_t          sem_cnt;                /**< fat semaphore access count */
    SemaphoreHandle_t sem;                    /**< fat access only once at the same time */
    uint8_t           work[FF_MAX_SS];        /**< fat working area */
    uint8_t           alive[FILE_OBJ_SIZE];   /**< file alive status, [0]:alive count */
    FIL_RES_T        *fil_set[FILE_OBJ_SIZE]; /**< file alive object */
} FAT_RES_T;

typedef struct _FILE_FRAME_T
{
    uint8_t *buffer;     /**< file frame buffer */
    uint32_t length;     /**< file frame buffer length */
    void    *frame_info; /**< file frame extra info */
} FILE_FRAME_T;

typedef struct _FILE_PARSER_T
{
    uint8_t *data_addr; /* buffer_data addr for user data */
    uint32_t data_size; /* buffer_data size for user data */
    uint8_t *tail_addr; /* buffer_tail addr for before buffer */
    uint32_t tail_size; /* buffer_tail size for before buffer */
    uint32_t tail_used; /* buffer_tail used for before buffer */

    uint8_t *buff_addr;  /* buffer_curr addr for parser working area */
    uint32_t buff_size;  /* buffer_curr size for parser working area */
    uint32_t buff_pos;   /* buffer_curr used position for parser working area */
    uint32_t buff_cnt;   /* buffer_curr used count for parser working area */
    uint32_t buff_total; /* buffer_curr size total for parser working area */
} FILE_PARSER_T;

typedef struct _TYPE_WRAPPER
{
    int32_t (*Open)(const void *res);                                          /**< open the file  */
    int32_t (*Close)(const void *res);                                         /**< close the file */
    int32_t (*Read)(const void *res, void *frame, uint8_t is_blocking);        /**< read data from the file by frame */
    int32_t (*Write)(const void *res, const void *frame, uint8_t is_blocking); /**< write data to the file by frame  */
    int32_t (*Wait)(const void *res);                                          /**< wait the end of Read/Write       */
    int32_t (*Seek)(const void *res, const void *seek);                        /**< Seek to a specify position       */
    int32_t (*GetStatus)(const void *res, void *status);                       /**< get runtime status of the file   */
} const TYPE_WRAPPER;

//////////////////////////////////////////////////////////////////////////////
// file operation
extern int32_t _file_create(FIL *fp, const TCHAR *name);
extern int32_t _file_close(FIL *fp, const TCHAR *name);
extern int32_t _file_open(FIL *fp, const TCHAR *name, uint8_t mode);
extern int32_t _file_fast_seek(FIL *fp, const TCHAR *name, FATFS *fat);
extern int32_t _file_seek_and_read(uint64_t seek, uint32_t *read_size, void *read_buff, FIL *fp);
extern int32_t _file_seek_and_write(uint64_t seek, uint32_t write_size, void *write_buff, FIL *fp);
extern int32_t _file_merge(FIL *des_file, FIL *src_file);
extern int32_t _file_seek(FIL *fp, uint64_t seek);
extern int32_t _file_delete(const TCHAR *name);

//  dir operation
extern int32_t _file_opendir(DIR *dp, const TCHAR *path);
extern int32_t _file_closedir(DIR *dp);
extern int32_t _file_readdir(DIR *dp, FILINFO *fno);
extern int32_t _file_findfirst(DIR *dp, FILINFO *fno, const TCHAR *path, const TCHAR *pattern);
extern int32_t _file_findnext(DIR *dp, FILINFO *fno);
extern int32_t _file_mkdir(const TCHAR *path);
extern int32_t _file_utime(const TCHAR *path, const FILINFO *fno);

// disk operation
extern int32_t _file_mount(FATFS *fat, uint8_t *fat_work, const TCHAR *name);
extern int32_t _file_set_label(FATFS *fat, uint8_t *fat_work, const TCHAR *name);
extern int32_t _file_get_label(FATFS *fat, uint8_t *fat_work, const TCHAR *name, TCHAR *label);
extern int32_t _file_get_free(FATFS *fat, uint8_t *fat_work, const TCHAR *name, DWORD *nclst);
extern int32_t _file_make_fs(const TCHAR *name, const MKFS_PARM *opt);
extern int32_t _file_disk_status(FATFS_DISK_STATUS_T *status, FAT_MAP_T *DiskMap);
extern int32_t _file_disk_find(FATFS_LOGIC_DEV_T *logic, FATFS_PHY_DEV_T dev, FATFS_LOGIC_PART_T part, FAT_MAP_T *DiskMap);
extern int32_t _file_disk(FATFS_DISK_T *disk, FAT_MAP_T *DiskMap);

// parse file by byte or word
extern int32_t _file_get_nbyte(uint8_t *data, uint32_t size, FILE_PARSER_T *parser, FIL *fp);
extern int32_t _file_get_nword_ex(uint16_t *data, uint32_t size, FILE_PARSER_T *parser, FIL *fp);
extern int32_t _file_rvt_nbyte(uint32_t size, FILE_PARSER_T *parser, FIL *fp);
#define _file_get_nword(data, size, parser, fp) _file_get_nbyte((uint8_t *)data, size * sizeof(uint16_t), parser, fp)

//////////////////////////////////////////////////////////////////////////////
// no fat operation
typedef struct _DEV_FIL_T
{
    uint8_t  dev;      /**< Physical device */
    uint8_t  init;     /**< init or not */
    uint32_t beg_addr; /**< begin address */
    uint32_t end_addr; /**< ended address */
    uint32_t cur_addr; /**< current address */
} DEV_FIL_T;

typedef struct _PARSE_WRAPPER
{
    int32_t (*Init)(void *res);                  /**< init the stream */
    int32_t (*Uninit)(void *res);                /**< uninit the stream */
    int32_t (*ReadDev)(void *res, void *info);   /**< read data from the storage */
    int32_t (*GetFrame)(void *res, void *frame); /**< read data from the stream by frame */
} const PARSE_WRAPPER;

#endif /* _FILE_WRAPPER_H_ */
