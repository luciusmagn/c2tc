#ifndef recipe_h
#define recipe_h

#include "mpc.h"
#include "vector.h"
#include "inttypes.h"
#include "shared.h"

#define executable 0x00
#define libstatic 0x01
#define libshared 0x02

/// <summary>
///  This structure contains a vector with targets and their count
/// </summary>
typedef struct
{
	vector* targets;
	int32 count;
} recipe_t;

/// <summary>
///  This structure contains information about each target
/// </summary>
typedef struct
{
	mpc_ast_t* node;
	vector* options;
	vector* files;
	char* name;
	int8 type;
} target;

/// <summary>
///  This structure contains information about each option and its parameters
/// </summary>
typedef struct
{
	char* name;
	vector* opts;
} option;

//currently built target
target* currenttrg;
recipe_t* recipe;

#endif