#include "errors.h"
#include "vasprintf.h"
#include "microtest.h"

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
	char* str; asprintf(&str, "Failed to parse file" ANSI_COLOR_GREEN " %s" ANSI_COLOR_RESET ".", filename);
	return str;
}