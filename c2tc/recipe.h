#ifndef recipe_h
#define recipe_h

#include "mpc.h"
#include "vector.h"
#include "inttypes.h"
#include "shared.h"

#define executable 0x00
#define libstatic 0x01
#define libshared 0x02

typedef struct
{
	vector* targets;
	int32 count;
} recipe_t;

typedef struct
{
	mpc_ast_t* node;
	vector* options;
	vector* files;
	char* name;
	int8 type;
} target;

typedef struct
{
	char* name;
	vector* opts;
} option;
target* currenttrg;
recipe_t* recipe;

void recipe_walk(mpc_ast_t* tree, mpc_ast_t* parent);
#endif