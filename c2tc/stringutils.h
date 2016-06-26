#ifndef stringutils_h
#define stringutils_h

#include "inttypes.h"
int8 endswith(char* str1,char* str2);
int8 startswith(char* str1, char* str2);
int8 split(char* dest[], const char* delimiter, const char* str);
char *skip_spaces(const char *str);

#endif