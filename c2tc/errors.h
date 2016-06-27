#ifndef errors_h
#define errors_h

#include "throw.h"

error_t erdfail; //error read failed
error_t emafail; //error memory alloc failed
error_t enoargs; //error no arguments
error_t enoaccs; //error no access to file
error_t ebadtok; //error bad token
error_t enotoks; //error not enough tokens

void init_errors();

#endif