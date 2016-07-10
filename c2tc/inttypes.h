#ifndef inttypes_h
#define inttypes_h

#include <stdint.h>

#ifdef _WIN32
typedef __int64 int64;
typedef __int32 int32;
typedef __int16 int16;
typedef __int8 int8;
#else
typedef int64_t int64;
typedef int32_t int32;
typedef int16_t int16;
typedef int8_t int8;
#endif
typedef uint8_t  uint8;
typedef uint16_t uint16;
typedef uint32_t uint32;
typedef uint64_t uint64;

#endif
