// this file contains the throw library by joseph werle <joseph.werle@gmail.com> among other things
#ifndef errors_h
#define errors_h

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>

#include "util.h"

#define QUOTE_X(t)#t
#define QUOTE(t)QUOTE_X(t)
#define ERROR_NEW_CONSTANT(c) error_new(c, QUOTE_X(#c))

#ifdef __CYGWIN__
#define error_t _error_t
#endif
typedef struct
{
	int code;
	char *name;
} error_t;

FILE *THROW_FD;
void throw (void *type, char *msg);
error_t error_new(int code, char *name);
int32 errors;


error_t erdfail; //error read failed
error_t emafail; //error memory alloc failed
error_t enoargs; //error no arguments
error_t enoaccs; //error no access to file
error_t ebadtok; //error bad token
error_t enotoks; //error not enough tokens
error_t eparsef; //error parsing a file failed

void init_errors();
char* get_file_error(char* filename);
#endif
