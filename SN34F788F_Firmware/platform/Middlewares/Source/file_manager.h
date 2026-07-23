/***********************
 * SN34F78X SDK V2.1.4 *
 ***********************/

#ifndef _SN34F78X_FILE_MANAGER_
#define _SN34F78X_FILE_MANAGER_

#include "sn34f78x_hal.h"
#include ".\file_manager\file_wrapper.h"
#include ".\file_manager\bin_wrapper.h"
#include ".\file_manager\wav_wrapper.h"
#include ".\file_manager\mp3_wrapper.h"
#include ".\file_manager\a32_wrapper.h"
#include ".\file_manager\bmp_wrapper.h"
#include ".\file_manager\jpg_wrapper.h"
#include ".\file_manager\gif_wrapper.h"
#include ".\file_manager\avi_wrapper.h"

#if (configUSE_FILEMANAGER_MIDDLEWARE == 1)
#define FAT_COUNT (FF_MULTI_PARTITION ? FF_VOLUMES : DEV_MAX)

typedef struct _MANAGER_RES_T
{
    uint16_t          sem_cnt;            /**< semaphore reference count */
    SemaphoreHandle_t sem;                /**< semaphore for init */
    FAT_RES_T        *fat_set[FAT_COUNT]; /**< fat set */
    FAT_MAP_T         DiskMap;            /**< fat mapping */
} MANAGER_RES_T;
#endif /* configUSE_FILEMANAGER_MIDDLEWARE == 1 */

#endif /* _SN34F78X_FILE_MANAGER_ */
