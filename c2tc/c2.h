#ifndef c2_h
#define c2_h
#include "mpc.h"
#include "vector.h"
#include "inttypes.h"

#define ALIASTYPE 0x00
#define FUNCTYPE 0x01
#define STRUCTTYPE 0x02

typedef struct
{
	int8 kind;
	char* name;
	void* contents;
} type_t;

typedef struct
{
	type_t type;
	char* name;
	mpc_ast_t* value;
	int8 accessibility;
} symbol;

typedef struct
{
	vector* trees;
	vector* imports;
	char* target;
	char* name;
} module;
#endif