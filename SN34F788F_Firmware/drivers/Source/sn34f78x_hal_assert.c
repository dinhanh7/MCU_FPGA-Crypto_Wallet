/***********************
 * SN34F78X SDK V2.1.4 *
 ***********************/

#include "sn34f78x_hal.h"

#if (configUSE_ASSERT == 1)
__weak void AssertParaFalse(uint8_t *file, uint32_t line)
{
    // printf("Error: file@ $s, line@ %d", file, line);
    while (1)
    {
        /* Delay 1ms */
    }
}

#endif
