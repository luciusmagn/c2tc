#ifndef shared_h
#define shared_h

#include "inttypes.h"
#include "mpc.h"
#include "vector.h"

/// <summary>
///  This function is the c2 parser main entrypoint
/// </summary>
int32 c2main(int32 argc, char** argv);

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

int32 lsize;
FILE* current;
char* currenttxt;
char* commentless;
mpc_ast_t* currenttree;
vector* trees;



#endif