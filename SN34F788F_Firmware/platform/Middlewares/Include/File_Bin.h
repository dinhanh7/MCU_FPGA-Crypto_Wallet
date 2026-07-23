/***********************
 * SN34F78X SDK V2.1.4 *
 ***********************/

#ifndef _FILE_BIN_H_
#define _FILE_BIN_H_

#include "sn34f78x_hal_def.h"
//=============================================================================
//                  Macro Definition
//=============================================================================
/**
 * \brief BIN_STATUS_T
 * \ingroup filemanager_bin_struct
 */
typedef struct _BIN_STATUS_T
{
    uint64_t bin_pos;  /**< bin pointer position */
    uint64_t bin_size; /**< bin total size */
    uint8_t  is_eof;   /**< bin is end or not */
} BIN_STATUS_T;

/**
 * \brief BIN_SEEK_T
 * \ingroup filemanager_bin_struct
 */
typedef struct _BIN_SEEK_T
{
    uint64_t pos; /**< seek to position */
} BIN_SEEK_T;

#endif /* _FILE_BIN_H_ */
