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

int8 endswith(char* str1,char* str2);
int8 startswith(char* str1, char* str2);
char** strsplit(char* str, const char* delim);
char *skip_spaces(const char *str);
char* get_line(char* src, int32 index);
int32 occurences(char* str, char c);
char *str_replace(char *orig, char *rep, char *with);
int8 issornull(char* test);
void teststrings();


#endif
