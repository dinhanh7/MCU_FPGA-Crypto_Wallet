/***********************
 * SN34F78X SDK V2.1.4 *
 ***********************/

#ifndef _BIN_WRAPPER_H_
#define _BIN_WRAPPER_H_

#include "file_wrapper.h"
#include "File_Bin.h"

//=============================================================================
//                  Structure Declaration
//=============================================================================
typedef struct _BIN_RES_T
{
    FIL    *fp;
    uint8_t mode;
    uint8_t is_eof;
} BIN_RES_T;

extern TYPE_WRAPPER BIN_Wrapper;

#endif /* _BIN_WRAPPER_H_ */
