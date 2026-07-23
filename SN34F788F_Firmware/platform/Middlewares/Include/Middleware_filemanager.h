/***********************
 * SN34F78X SDK V2.1.4 *
 ***********************/

#ifndef _MIDDLEWARE_FILEMANAGER_H_
#define _MIDDLEWARE_FILEMANAGER_H_

#ifdef __cplusplus
extern "C" {
#endif

#include "File_Fat.h"
#include "File_Bin.h"
#include "File_WAV.h"
#include "File_MP3.h"
#include "File_A32.h"
#include "File_BMP.h"
#include "File_JPG.h"
#include "File_GIF.h"
#include "File_AVI.h"
#include "FreeRTOS.h"
#include "task.h"

#define M_FILE_NON_BLOCKING (0x00)
#define M_FILE_BLOCKING     (0x01)

/**
 * \brief enum for M_FILE_MANAGER eof status
 * \ingroup filemanager_common_para
 */
typedef enum _M_FILE_EOF_STATUS
{
    M_FILE_IS_NOT_EOF, /**< file is not at end */
    M_FILE_IS_EOF,     /**< file is at end     */
} M_FILE_EOF_STATUS;

/**
 * \brief   Negative values indicate errors of M_FileManager.
 * \ingroup filemanager_error_code_2
 * \details The M_FILE_MANAGER has additional status error codes that are listed below.
 * \note    that the M_FILE_MANAGER also returns the common \ref execution_status .
 */
typedef enum _M_FILE_ERROR
{
    M_FILE_ERROR_OVER_SIZE = FILE_ERROR_OVER_SIZE, /**< file size cannot reach 4 GiB at FAT volume */
    M_FILE_ERROR_BEG       = -0x200,
    M_FILE_ERROR_INVALID_PATH,   /**< file path invalid */
    M_FILE_ERROR_NON_BLOCKING,   /**< file non-blocking read/write, currently not supported */
    M_FILE_ERROR_OPEN_OVER_SIZE, /**< file alive count overflow */
    M_FILE_ERROR_NO_MORE_FILE,   /**< no more file */
    M_FILE_ERROR_REOPEN,         /**< file reopen */
    M_FILE_ERROR_END,
} M_FILE_ERROR;

/**
 * \brief struct for file handler
 * \ingroup filemanager_common_struct
 * \details The structure define the M_FILE_MANAGER handler information
 */
typedef struct _M_FILE_HANDLE_T
{
    uint8_t dev;   /**< Logical drive index, \ref HAL_MFile_OpenFile will fix it */
    uint8_t index; /**< Logical file index,  \ref HAL_MFile_OpenFile will fix it */
    union
    {
        uint8_t bin_eof : 1;  /**< EOF flag of bin type, \ref HAL_MFile_WriteFile or \ref HAL_MFile_ReadFile will fix it */
        uint8_t wav_eof : 1;  /**< EOF flag of wav type, \ref HAL_MFile_WriteFile or \ref HAL_MFile_ReadFile will fix it */
        uint8_t jpeg_eof : 1; /**< EOF flag of jpegtype, \ref HAL_MFile_WriteFile or \ref HAL_MFile_ReadFile will fix it */
        uint8_t mp3_eof : 1;  /**< EOF flag of mp3 type, \ref HAL_MFile_WriteFile or \ref HAL_MFile_ReadFile will fix it */
        uint8_t a32_eof : 1;  /**< EOF flag of a32 type, \ref HAL_MFile_WriteFile or \ref HAL_MFile_ReadFile will fix it */
        uint8_t bmp_eof : 1;  /**< EOF flag of bmp type, \ref HAL_MFile_WriteFile or \ref HAL_MFile_ReadFile will fix it */
        uint8_t jpg_eof : 1;  /**< EOF flag of jpg type, \ref HAL_MFile_WriteFile or \ref HAL_MFile_ReadFile will fix it */
        uint8_t gif_eof : 1;  /**< EOF flag of gif type, \ref HAL_MFile_WriteFile or \ref HAL_MFile_ReadFile will fix it */
        struct                /**< EOF flag of bmp type, \ref HAL_MFile_WriteFile or \ref HAL_MFile_ReadFile will fix it */
        {
            uint8_t a_eof : 1;   /**< EOF flag of avi:audio:stream, \ref HAL_MFile_WriteFile or \ref HAL_MFile_ReadFile will fix it */
            uint8_t v_eof : 1;   /**< EOF flag of avi:video:stream, \ref HAL_MFile_WriteFile or \ref HAL_MFile_ReadFile will fix it */
            uint8_t avi_eof : 1; /**< EOF flag of avi, \ref HAL_MFile_WriteFile or \ref HAL_MFile_ReadFile will fix it */
        };
    }; /**< EOF flag of type, \ref HAL_MFile_WriteFile or \ref HAL_MFile_ReadFile will fix it */
    uint8_t eof : 1; /**< EOF flag of file, \ref HAL_MFile_WriteFile or \ref HAL_MFile_ReadFile will fix it */
} M_FILE_HANDLE_T;

/**
 * \brief struct for dir handler
 * \ingroup filemanager_common_struct
 * \details The structure define the M_FILE_MANAGER dir handler information
 */
typedef struct _M_DIR_HANDLE_T
{
    DIR             dir; /**< ff DIR info, \ref HAL_MFile_OpenFile will fix it */
    M_FILE_HANDLE_T fh;  /**< fm DIR info, \ref HAL_MFile_OpenFile will fix it */
} M_DIR_HANDLE_T;

/**
 * \brief struct for dir item infomation
 * \ingroup filemanager_common_struct
 * \details The structure define the M_FILE_MANAGER dir item information
 */
typedef struct _M_DIR_ITEM_T
{
    FILINFO    fno;   /**< ff DIR item info */
    DIR_ATTR_T attr;  /**< DIR item attr    */
    int32_t    year;  /**< file time: year  */
    int32_t    month; /**< file time: month */
    int32_t    mday;  /**< file time: mday  */
    int32_t    hour;  /**< file time: hour  */
    int32_t    min;   /**< file time: min   */
    int32_t    sec;   /**< file time: sec   */
} M_DIR_ITEM_T;

/**
 * \brief struct for seek information
 * \ingroup filemanager_common_struct
 * \details The structure define the M_FILE_MANAGER seek information, will be checked in \ref HAL_MFile_SeekFile
 */
typedef struct _M_FILE_SEEK_T
{
    union
    {
        BIN_SEEK_T seek_bin; /**< File seek info of bin  file \ref BIN_SEEK_T  */
        WAV_SEEK_T seek_wav; /**< File seek info of wav  file \ref WAV_SEEK_T  */
        MP3_SEEK_T seek_mp3; /**< File seek info of mp3  file \ref MP3_SEEK_T  */
        A32_SEEK_T seek_a32; /**< File seek info of a32  file \ref A32_SEEK_T  */
        BMP_SEEK_T seek_bmp; /**< File seek info of a32  file \ref BMP_SEEK_T  */
        AVI_SEEK_T seek_avi; /**< File seek info of a32  file \ref AVI_SEEK_T  */
    };
} M_FILE_SEEK_T;

/**
 * \brief struct for open information
 * \ingroup filemanager_common_struct
 * \details The structure define the M_FILE_MANAGER open information, will be checked in \ref HAL_MFile_OpenFile
 */
typedef struct _M_FILE_INIT_T
{
    uint8_t          dev;       /**< Physical/Logical drive index */
    FILE_FORMAT_T    format;    /**< File format, see \ref FILE_FORMAT_T  */
    FILE_OPEN_MODE_T mode;      /**< Open Mode, see \ref FILE_OPEN_MODE_T */
    const TCHAR     *full_path; /**< File full path */
    union
    {
        WAV_INIT_T wav; /**< The unique init of the wav  file \ref WAV_INIT_T  */
        MP3_INIT_T mp3; /**< The unique init of the mp3  file \ref MP3_INIT_T  */
        A32_INIT_T a32; /**< The unique init of the a32  file \ref A32_INIT_T  */
        BMP_INIT_T bmp; /**< The unique init of the bmp  file \ref BMP_INIT_T  */
        JPG_INIT_T jpg; /**< The unique init of the jpg  file \ref JPG_INIT_T  */
        GIF_INIT_T gif; /**< The unique init of the gif  file \ref GIF_INIT_T  */
        AVI_INIT_T avi; /**< The unique init of the gif  file \ref AVI_INIT_T  */
    };
} M_FILE_INIT_T;

/**
 * \brief struct for read/write information
 * \ingroup filemanager_common_struct
 * \details The structure define the M_FILE_MANAGER setting information, will be checked in \ref HAL_MFile_ReadFile or \ref HAL_MFile_WriteFile
 */
typedef struct _M_FILE_INFO_T
{
    uint8_t *data;       /**< buffer of read/write real data */
    uint32_t data_len;   /**< length of read/write real data */
    uint8_t *cache;      /**< buffer of read data cache */
    uint32_t cache_len;  /**< length of read data cache */
    void    *frame_info; /**< file frame extra info */
} M_FILE_INFO_T;

/**
 * \brief struct for path information
 * \ingroup filemanager_common_struct
 * \details The structure define the M_FILE_MANAGER path information
 */
typedef struct _M_FILE_PATH_T
{
    uint8_t      dev;       /**< Logical drive index */
    const TCHAR *full_path; /**< File full path */
} M_FILE_PATH_T;

/**
 * \brief struct for status info
 * \ingroup filemanager_common_struct
 * \details The structure define the M_FILE_MANAGER status information, will be got in \ref HAL_MFile_GetFileStatus
 */
typedef struct _M_FILE_STATUS_T
{
    const TCHAR  *name;   /**< file full name */
    uint64_t      length; /**< file total length, unint:byte */
    FILE_FORMAT_T format; /**< file parsing format, \ref FILE_FORMAT_T */
    union
    {
        BIN_STATUS_T bin; /**< The unique state of the \b bin  file \ref BIN_STATUS_T  */
        WAV_STATUS_T wav; /**< The unique state of the \b wav  file \ref WAV_STATUS_T  */
        MP3_STATUS_T mp3; /**< The unique state of the \b mp3  file \ref MP3_STATUS_T  */
        A32_STATUS_T a32; /**< The unique state of the \b a32  file \ref A32_STATUS_T  */
        BMP_STATUS_T bmp; /**< The unique state of the \b bmp  file \ref BMP_STATUS_T  */
        JPG_STATUS_T jpg; /**< The unique state of the \b jpg  file \ref JPG_STATUS_T  */
        GIF_STATUS_T gif; /**< The unique state of the \b gif  file \ref GIF_STATUS_T  */
        AVI_STATUS_T avi; /**< The unique state of the \b avi  file \ref AVI_STATUS_T  */
    };
} M_FILE_STATUS_T;

// ========================================== Disk Operation ==========================================
int32_t HAL_MFile_DiskPart(FATFS_DISK_T *disk); /* note: Be Cautious to use this function! will format the Physical disk, include all Logical disk on it.*/
int32_t HAL_MFile_DiskFind(FATFS_LOGIC_DEV_T *logic, FATFS_PHY_DEV_T phy, FATFS_LOGIC_PART_T part);
int32_t HAL_MFile_GetDiskStatus(FATFS_DISK_STATUS_T *status);

// ========================================== FILE Operation ==========================================
int32_t HAL_MFile_OpenFile(M_FILE_HANDLE_T *fp, M_FILE_INIT_T *init);
int32_t HAL_MFile_CloseFile(M_FILE_HANDLE_T fp);
int32_t HAL_MFile_ReadFile(M_FILE_HANDLE_T *fp, M_FILE_INFO_T *info, int8_t is_blocking);
int32_t HAL_MFile_WriteFile(M_FILE_HANDLE_T *fp, M_FILE_INFO_T *info, int8_t is_blocking);
int32_t HAL_MFile_WaitFile(M_FILE_HANDLE_T fp);
int32_t HAL_MFile_SeekFile(M_FILE_HANDLE_T *fp, M_FILE_SEEK_T seek);
int32_t HAL_MFile_GetFileStatus(M_FILE_HANDLE_T fp, M_FILE_STATUS_T *status);

// ==========================================  Dir Operation ==========================================
int32_t HAL_MFile_OpenDir(M_DIR_HANDLE_T *dp, M_FILE_PATH_T *path);
int32_t HAL_MFile_CloseDir(M_DIR_HANDLE_T *dp);
int32_t HAL_MFile_MakeDir(M_FILE_PATH_T *path);
int32_t HAL_MFile_ReadDir(M_DIR_HANDLE_T *dp, M_DIR_ITEM_T *fno);
int32_t HAL_MFile_FindFirst(M_DIR_HANDLE_T *dp, M_DIR_ITEM_T *fno, M_FILE_PATH_T *path, const TCHAR *pat);
int32_t HAL_MFile_FindNext(M_DIR_HANDLE_T *dp, M_DIR_ITEM_T *fno);
int32_t HAL_MFile_DeleteDir(M_FILE_PATH_T del);
int32_t HAL_MFile_ChangeDirTime(M_FILE_PATH_T *path, M_DIR_ITEM_T *fno);

#ifdef __cplusplus
}
#endif

#endif // _MIDDLEWARE_FILEMANAGER_H_
