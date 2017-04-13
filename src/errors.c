#include "log.h"
#include "errors.h"

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
        THROW_FD = stderr;
    if (THROW_FD != stderr)
        fprintf(THROW_FD, "%s (%d): %s\n", err->name, err->code, msg);
    else
        log_error("%s (%d): %s\n", err->name, err->code, msg);

    errors++;
}

void init_errors()
{
	erdfail = error_new(1, "ERDFAIL");
	emafail = error_new(2, "EMAFAIL");
	enoargs = error_new(3, "ENOARGS");
	enoaccs = error_new(4, "ENOACCS");
	ebadtok = error_new(5, "EBADTOK");
	enotoks = error_new(6, "ENOTOKS");
	eparsef = error_new(7, "EPARSEF");
}

char* get_file_error(char* filename)
{
	char* str; asprintf(&str, "Failed to parse file" ANSI_GREEN " %s" ANSI_RESET ".", filename);
	return str;
}
