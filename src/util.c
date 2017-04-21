#include <stdio.h>
#include <string.h>
#include <stdarg.h>
#include <stdlib.h>
#include <stdbool.h>
#include <ctype.h>

#include "ooc.h"
#include "util.h"
#include "microtest.h"

/***********
 * STRINGS *
 ***********/
int8 endswith(char *string, char* suffix)
{
    char* loc = strstr(string, suffix);
    if (string) return !strcmp(loc, suffix);
    return(0);
}

char** strsplit(char* str, const char* delim)
{
    int32 i = 1;
    char* tok;
    char** result;
    llist* tokens;
    tok = strtok(str, delim);
    tokens = llist_new(tok);
    while( (tok = strtok(NULL, delim)) )
    {
        llist_put(tokens, tok);
        i++;
    }

    result = malloc(sizeof(char*) * i);
    for(int32 j = 0; j < llist_total(tokens, 0); j++)
        result[j] = llist_get(tokens, j, 0);
    return result;
}

//skip leading whitespace. proudly stolen from linux kernel in 2015
char *skip_spaces(const char *str)
{
    while (isspace(*str))
        ++str;
    return str;
}

//WARNING: does not count double occurrences at all
int32 occurences(char* str, char c)
{
    int32 i, count;
    for (i = 0, count = 0; str[i]; i++)
        count += (str[i] == c && str[i+1] != c);
    return count;
}

//replace every occurence of char* rep with char* with
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

//tests whether a string is whitespace or null
int8 issornull(char* test)
{
    if (!test) return 0;
    int32 i = 0;
    while (isspace(test[i])) i++;
    if (i != strlen(test)) return 0;
    return 1;
}

/************
 * ASPRINTF *
 ************/
#ifndef HAVE_ASPRINTF
/**
* `asprintf.c' - asprintf
* copyright (c) 2014 joseph werle <joseph.werle@gmail.com>,
* modified by Lukáš Hozda <luk.hozda@gmail.com>
*/

int asprintf(char **str, const char *fmt, ...)
{
	int size = 0;
	va_list args;
	va_start(args, fmt);
	size = vasprintf(str, fmt, args);
	va_end(args);
	return size;
}

int vasprintf(char **str, const char *fmt, va_list args)
{
	int size = 0;
	va_list tmpa;
	va_copy(tmpa, args);
	size = vsnprintf(NULL, size, fmt, tmpa);
	va_end(tmpa);
	if (size < 0) { return -1; }
	*str = (char *)malloc(size + 1);
	if (NULL == *str) { return -1; }
	size = vsprintf(*str, fmt, args);
	return size;
}
#endif

/**********
 * VECTOR *
 **********/

//Implementation of a vector. Based on a guide by Edd Mann
void vector_init(vector* v)
{
    v->capacity = 4;
    v->total = 0;
    v->items = malloc(sizeof(void *) * v->capacity);
}

int32 vector_total(vector* v) { return v->total; }

void vector_resize(vector* v, int32 capacity)
{
    void** items = realloc(v->items, sizeof(void *) * capacity);
    if (items != NULL)
    {
        v->items = items;
        v->capacity = capacity;
    }
}

void vector_add(vector* v, void* item)
{
    if (v->capacity == v->total)
        vector_resize(v, v->capacity * 2);
    v->items[v->total++] = item;
}

void vector_set(vector* v, int32 index, void* item)
{
    if (index >= 0 && index < v->total)
        v->items[index] = item;
}

void* vector_get(vector* v, int32 index)
{
    if (index >= 0 && index < v->total)
        return v->items[index];
    return NULL;
}

void vector_delete(vector* v, int32 index)
{
    if (index < 0 || index >= v->total)
        return;
    v->items[index] = NULL;
    for (int32 i = 0; i < v->total - 1; i++)
    {
        v->items[i] = v->items[i + 1];
        v->items[i + 1] = NULL;
    }
    v->total--;
    if (v->total > 0 && v->total == v->capacity / 4)
        vector_resize(v, v->capacity / 2);
}

void vector_free(vector* v)
{
    free(v->items);
    v->items = NULL;
}

/*********
 * TESTS *
 *********/
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

void testvector()
{
    vector* v = malloc(sizeof(vector));
    vector_init(v);
    tiny_file();
    vector_add(v, "test");
    tiny_assert("test if vector_add() works properly", vector_total(v) == 1);
    tiny_assert("test if vector_get() works properly", (strcmp(vector_get(v, 0), "test") == 0));
    vector_set(v, 0, "test2");
    tiny_assert("test if vector_set() works properly", (strcmp(vector_get(v, 0), "test2") == 0));
    tiny_assert("test if vector_total() works properly", vector_total(v) == 1);
    vector_delete(v, 0);
    tiny_assert("test if vector_delete() works properly", vector_total(v) == 0);
    vector_free(v);
    tiny_assert("test if vector_free() is working properly", !v->items);
}

