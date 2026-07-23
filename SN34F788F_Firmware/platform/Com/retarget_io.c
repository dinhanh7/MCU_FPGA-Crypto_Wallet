#include <stdio.h>

#ifndef __MICROLIB

#if (defined(__CC_ARM) && defined(__ARMCC_VERSION))
#if (__ARMCC_VERSION < 6010050)
#pragma import(__use_no_semihosting)
#elif (__ARMCC_VERSION >= 6010050)
__asm(".global __use_no_semihosting");
FILE __stdout __attribute((used)); /* Prevent from optimized */
FILE __stdin __attribute((used));  /* Prevent from optimized */
FILE __stderr __attribute((used)); /* Prevent from optimized */
#endif /* __ARMCC_VERSION */
#endif /* __CC_ARM */

__weak void _sys_exit(int returncode)
{
}

__weak void _ttywrch(int ch)
{
}

__weak _ARMABI FILE *fopen(const char *filename, const char *mode)
{
    /* Your implementation of fopen(). */
    return (FILE *)-1;
}

__weak _ARMABI FILE *freopen(const char *filename, const char *mode, FILE *stream)
{
    /* Your implementation of freopen(). */
    return (FILE *)-1;
}

__weak int fgetc(FILE *f)
{
    /* Your implementation of fgetc(). */
    return -1;
}

__weak int fputc(int c, FILE *stream)
{
    /* Your implementation of fputc(). */
    return -1;
}

__weak int ferror(FILE *stream)
{
    /* Your implementation of ferror(). */
    return -1;
}

__weak long int ftell(FILE *stream)
{
    /* Your implementation of ftell(). */
    return -1;
}

__weak int fclose(FILE *f)
{
    /* Your implementation of fclose(). */
    return -1;
}

__weak int fseek(FILE *f, long nPos, int nMode)
{
    /* Your implementation of fseek(). */
    return -1;
}

__weak int fflush(FILE *f)
{
    /* Your implementation of fflush(). */
    return -1;
}

#endif /* __MICROLIB */
