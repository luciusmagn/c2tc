#include "errors.h"

void init_errors()
{
	erdfail = error_new(1, "ERDFAIL");
	emafail = error_new(2, "EMAFAIL");
	enoargs = error_new(3, "ENOARGS");
	enoaccs = error_new(4, "ENOACCS");
	ebadtok = error_new(5, "EBADTOK");
	enotoks = error_new(6, "ENOTOKS");
}