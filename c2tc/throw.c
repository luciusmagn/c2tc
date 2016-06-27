#include "throw.h"

error_t error_new(int code, char *name)
{
	error_t err;
	err.code = code;
	err.name = name;
	return err;
}

void throw (void *type, char *msg)
{
	error_t *err = (error_t *)type;
	// default to `stderr` stream
	if (NULL == THROW_FD)
	{
		THROW_FD = stderr;
	}

	fprintf(THROW_FD, "%s (%d): %s\n", err->name, err->code, msg);
}