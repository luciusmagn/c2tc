#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>
#include <ctype.h>

#include "util.h"
#include "microtest.h"

/// <summary>
///  This function checks if a string ends with the given suffix
/// </summary>
int8 endswith(char *string, char* suffix)
{
    char* loc = strstr(string, suffix);

    if (string != NULL)
        return !strcmp(loc, suffix);

    return(0);
}
/// <summary>
///  This function checks if a string starts with the given prefix
/// </summary>
int8 startswith(char* str1, char* str2)
{
    return !strncmp(str1, str2, strlen(str2));
}

/// <summary>
///  This function splits a string divided by a delimiter to an array of strings
/// </summary>
char** strsplit(char* str, const char* delim)
{
    char** res = NULL;
    char*  part;
    int i = 0;

    char* aux = strdup(str);

    part = strdup(strtok(aux, delim));

    while (part) {
        res = (char**)realloc(res, (i + 1) * sizeof(char*));
        *(res + i) = strdup(part);

        part = strdup(strtok(NULL, delim));
        i++;
    }

    res = (char**)realloc(res, i * sizeof(char*));
    *(res + i) = NULL;

    return res;
}

/// <summary>
///  This function skips leading whitespace
/// </summary>
char *skip_spaces(const char *str)
{
    while (isspace(*str))
        ++str;
    return str;
}
/// <summary>
///  This function returns a line given by index from a string. Zero-indexed
/// </summary>
char* get_line(char* src, int32 index)
{
    return strsplit(src, "\n")[index];
}
/// <summary>
///  This function returns a number of occurrences of given character in the string
/// </summary>
int32 occurences(char* str, char c)
{
    int32 i, count;
    for (i = 0, count = 0; str[i]; i++)
        count += (str[i] == c);
    return count;
}

/// <summary>
/// This function replaces every occurency of char* rep with char* in given string
/// </summary>
char *str_replace(char *orig, char *rep, char *with)
{
    char *result; // the return string
    char *ins;    // the next insert point
    char *tmp;    // varies
    int len_rep;  // length of rep
    int len_with; // length of with
    int len_front; // distance between rep and end of last rep
    int count;    // number of replacements

    if (!orig)
        return NULL;
    if (!rep)
        rep = "";
    len_rep = strlen(rep);
    if (!with)
        with = "";
    len_with = strlen(with);

    ins = orig;
    for (count = 0; (tmp = strstr(ins, rep)); ++count)
        ins = tmp + len_rep;

    // first time through the loop, all the variable are set correctly
    // from here on,
    //    tmp points to the end of the result string
    //    ins points to the next occurrence of rep in orig
    //    orig points to the remainder of orig after "end of rep"
    tmp = result = malloc(strlen(orig) + (len_with - len_rep) * count + 1);

    if (!result)
        return NULL;

    while (count--) {
        ins = strstr(orig, rep);
        len_front = ins - orig;
        tmp = strncpy(tmp, orig, len_front) + len_front;
        tmp = strcpy(tmp, with) + len_with;
        orig += len_front + len_rep; // move to next "end of rep"
    }
    strcpy(tmp, orig);
    return result;
}
/// <summary>
/// This function tests whether a string is whitespace or null
/// <summary>
int8 issornull(char* test)
{
    if (!test) return 0;
    int32 i = 0;
    while (isspace(test[i])) i++;
    if (i != strlen(test)) return 0;
    return 1;
}
/// <summary>
///  This function performs tests of stringutils' functions
/// </summary>
void teststrings()
{
    tiny_file();
    tiny_assert("test if endswith() works properly", endswith("test", "est"));
    tiny_assert("test if startswith() works properly", startswith("test", "tes"));
    tiny_assert("test if split() works properly", (strcmp(strsplit("This is a test", " ")[0], "This") == 0));
    char* str = "       test";
    tiny_assert("test if skip_spaces() works properly", (strcmp(skip_spaces(str), "test") == 0));
    tiny_assert("test if getline() works properly", (strcmp(get_line("a\nb\nc", 1), "b") == 0));
    tiny_assert("test if occurences() is working properly", occurences("bbb", 'b') == 3);
    tiny_assert("test if str_replace() is working properly", (strcmp(str_replace("abab", "a", "c"), "cbcb") == 0));
    tiny_assert("test if issornull()", issornull("      "));
}
