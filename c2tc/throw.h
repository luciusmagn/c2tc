#ifndef THROW_H
#define THROW_H

//
// throw.h
// copyright (c) 2013 joseph werle <joseph.werle@gmail.com>
//

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>

#define QUOTE_X(t)#t
#define QUOTE(t)QUOTE_X(t)

#define ERROR_NEW_CONSTANT(c) error_new(c, QUOTE_X(#c))

typedef struct
{
	int code;
	char *name;
} error_t;

FILE *THROW_FD;

void throw (void *type, char *msg);

error_t error_new(int code, char *name);

#endif