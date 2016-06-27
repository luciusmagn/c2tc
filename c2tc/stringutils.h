#ifndef stringutils_h
#define stringutils_h

#include "inttypes.h"
/// <summary>
///  This function checks if a string ends with the given suffix
/// </summary>
int8 endswith(char* str1,char* str2);
/// <summary>
///  This function checks if a string starts with the given prefix
/// </summary>
int8 startswith(char* str1, char* str2);
/// <summary>
///  This function checks splits a string divided by a delimiter to an array of strings
/// </summary>
char** strsplit(char* dest[], const char* delimiter, const char* str);
/// <summary>
///  This function checks skips leading whitespace
/// </summary>
char *skip_spaces(const char *str);
/// <summary>
///  This function performs tests of stringutils' functions
/// </summary>
void teststrings();

#endif