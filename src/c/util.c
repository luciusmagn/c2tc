#include <stdio.h>
#include <string.h>
#include <stdarg.h>
#include <stdlib.h>
#include <ctype.h>

#include "util.h"
#include "types.h"

/***********
 * STRINGS *
 ***********/

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

char* bin_type_str(int32 type)
{
	switch((bin_type)type)
	{
		case EXECUTABLE: return "executable";
		case LIB_STATIC: return "static library";
		case LIB_SHARED: return "shared library";
	}
	return NULL; //should never happen
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
 * LLIST *
 *********/
llist* llist_new(void* first)
{
	if (first == NULL) return NULL;
	llist* l = malloc(sizeof(llist));
	l->data = first;
	l->n = NULL;
	return l;
}

void llist_put(llist* l, void* data)
{
	if (!l) return;
	if (l && l->n == NULL)
	{
		l->n = malloc(sizeof(llist));
		l->n->data = data;
		l->n->n = NULL;
	}
	else llist_put(l->n, data);
}

int32_t llist_total(llist* l, int32_t carry)
{
	if (!l) return 0;
	if (l->n != NULL) return llist_total(l->n, carry + 1);
	else return carry + 1;
}

void* llist_get(llist* l, int32_t index, int32_t carry)
{
	if (!l) return NULL;
	if (carry > index) return NULL;
	if (carry < index && l->n) return llist_get(l->n, index, carry + 1);
	if (carry == index && l->data != NULL) return l->data;
	return NULL;
}
