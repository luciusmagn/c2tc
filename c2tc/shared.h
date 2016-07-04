#ifndef shared_h
#define shared_h

#define debug

#include "inttypes.h"
#include "mpc.h"
#include "vector.h"
#include "dict.h"

/// <summary>
///  This function is the c2 parser main entrypoint
/// </summary>
mpc_ast_t* c2main(int32 argc, char** argv);
/// <summary>
///  This function parses C2 from a file name given in the argument and returns AST
/// </summary>
mpc_ast_t* c2parse(char* filename);
/// <summary>
///  Entry point for the recipe parser
/// </summary>
int32 recipemain(int32 argc, char** argv);

/// <summary>
///  This function loops through parent directories until it finds the project root directory
///  where a cwd is set and recipe.txt file is located. Returns path to recipe.txt
/// </summary>
char* findrecipe();

/// <summary>
///  This function parses the recipe file and builds a recipe structure from it.
/// </summary>
void parserecipe(char* recipetext);

/// <summary>
///  This function handles each line in recipe
/// </summary>
void handleline(char* line);

/// <summary>
///  This function runs all tests and prints verdict to stdout
/// </summary>
void runtests();

#define test() do { testf(__FILE__, __LINE__); } while(0)
_inline void testf(char* file, int32 line)
{
	printf("%s:%d\n", file, line);
}

_inline void remove_element(mpc_ast_t** arr, int32 index, int32 array_length)
{
	int i;
	for (i = index; i < array_length - 1; i++) arr[i] = arr[i + 1];
}

int32 lsize;
FILE* current;
char* currenttxt;
char* commentless;
mpc_ast_t* currenttree;
mpc_ast_t* trees;
vector* modules;
dict* knownmodules;


#endif