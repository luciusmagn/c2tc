#ifndef recipe_h
#define recipe_h

#include "mpc.h"
#include "util.h"
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
	vector* nodes;	 //mpc_ast_t*
	vector* options; //target_option*
	vector* files;   //char*
	char* name;
	int8 type;
} target;

typedef struct
{
	char* name;
	vector* opts;
} target_option;

enum
{
	START,
	INSIDE_TARGET
};

target* currenttrg;
recipe_t* recipe;

int32 state;

void processrecipe();
void processtarget(target* trg);
#endif
