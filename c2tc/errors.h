#ifndef errors_h
#define errors_h

#include "throw.h"

error_t erdfail; //error read failed
error_t emafail; //error memory alloc failed
error_t enoargs; //error no arguments
error_t enoaccs; //error no access to file

void init_errors()
{
	erdfail = error_new(1, "ERDFAIL");
	emafail = error_new(2, "EMAFAIL");
	enoargs = error_new(3, "ENOARGS");
	enoaccs = error_new(4, "ENOACCS");
}

#endif