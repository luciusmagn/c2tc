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
///  This function splits a string divided by a delimiter to an array of strings
/// </summary>
char** strsplit(char* str, const char* delim);
/// <summary>
///  This function skips leading whitespace
/// </summary>
char *skip_spaces(const char *str);
/// <summary>
///  This function returns a line given by index from a string
/// </summary>
char* get_line(char* src, int32 index);
/// <summary>
///  This function returns a number of occurrences of given character in the string
/// </summary>
int32 occurences(char* str, char c);
/// <summary>
/// This function replaces every occurency of char* rep with char* in given string
/// </summary>
char *str_replace(char *orig, char *rep, char *with);
/// <summary>
/// This function tests whether a string is whitespace or null
/// <summary>
int8 issornull(char* test);
///  This function performs tests of stringutils' functions
/// </summary>
void teststrings();
#endif
