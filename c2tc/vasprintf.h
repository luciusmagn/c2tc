#ifndef HAVE_ASPRINTF
#ifndef ASPRINTF_H
#define ASPRINTF_H

/**
* `asprintf.h' - asprintf.c
*
* copyright (c) 2014 joseph werle <joseph.werle@gmail.com>, modified to own cause by Luk� Hozda <luk.hozda@gmail.com>
*/

#include <stdarg.h>
int vasprintf(char** str, const char* fmt, va_list args);

/// <summary>
///  This function works similarly to sprintf, but allocates the memory the string requires
/// </summary>
int asprintf(char** str, const char *fmt, ...);
#endif
#endif