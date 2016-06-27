#ifndef __LOG_h__
#define __LOG_h__

//Copyright 2013 Thorsten Lorenz.
//All rights reserved.
//
//Permission is hereby granted, free of charge, to any person
//obtaining a copy of this software and associated documentation
//files(the "Software"), to deal in the Software without
//restriction, including without limitation the rights to use,
//copy, modify, merge, publish, distribute, sublicense, and / or sell
//copies of the Software, and to permit persons to whom the
//Software is furnished to do so, subject to the following
//conditions :
//
//The above copyright notice and this permission notice shall be
//included in all copies or substantial portions of the Software.
//
//THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
//EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES
//OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
//NONINFRINGEMENT.IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT
//HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY,
//WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
//FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR
//OTHER DEALINGS IN THE SOFTWARE.

#include <stdio.h>
#include <errno.h>
#include <string.h>

#ifndef LOGLEVEL
#define LOGLEVEL 4
#endif

#define __FILENAME__ (strrchr(__FILE__, '/') ? strrchr(__FILE__, '/') + 1 : __FILE__)

#if LOGLEVEL < 3
#define NDEBUG 1
#endif

#ifdef NDEBUG
/* compile with all debug messages removed */
#define log_debug(M, ...)
#else
#ifdef LOG_NOCOLORS
#define log_debug(M, ...) fprintf(stderr, "DEBUG " M " at %s (%s:%d) \n", ##__VA_ARGS__, __FUNCTION__, __FILE__, __LINE__)
#else
#define log_debug(M, ...) fprintf(stderr, "\33[34mDEBUG\33[39m " M "  \33[90m at %s (%s:%d) \33[39m\n", ##__VA_ARGS__, __FUNCTION__, __FILE__, __LINE__)
#endif /* NOCOLORS */
#endif /* NDEBUG */

/* safe readable version of errno */
#define clean_errno() (errno == 0 ? "None" : strerror(errno))

#ifdef LOG_NOCOLORS
#define log_error(M, ...) fprintf(stderr,  "ERR   " M " in (%s:%d) errno:%s\n", ##__VA_ARGS__,  __FILE__, __LINE__, clean_errno())
#define log_warn(M, ...) fprintf(stderr, "WARN  " M " in (%s:%d) errno:%s\n", ##__VA_ARGS__,  __FILE__, __LINE__, clean_errno())
#define log_info(M, ...) fprintf(stderr, "INFO  " M " in (%s:%d)\n", ##__VA_ARGS__, __FILENAME__, __LINE__)
#else
#define log_error(M, ...) fprintf(stderr,  "\33[31mERR\33[39m   " M "  \33[90m in (%s:%d) \33[94merrno: %s\33[39m\n", ##__VA_ARGS__, __FILE__, __LINE__, clean_errno())
#define log_warn(M, ...) fprintf(stderr, "\33[91mWARN\33[39m  " M "  \33[90m in (%s:%d) \33[94merrno: %s\33[39m\n", ##__VA_ARGS__, __FILE__, __LINE__, clean_errno())
#define log_info(M, ...) fprintf(stderr, "\33[32mINFO\33[39m  " M "  \33[90m in (%s:%d) \33[39m\n", ##__VA_ARGS__, __FILENAME__, __LINE__)
#endif /* NOCOLORS */

#if LOGLEVEL < 4
#undef log_info
#define log_info(M, ...)
#endif

#if LOGLEVEL < 2
#undef log_warn
#define log_warn(M, ...)
#endif

#if LOGLEVEL < 1
#undef log_error
#define log_error(M, ...)
#endif

#endif