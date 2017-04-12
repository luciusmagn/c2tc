#ifndef shared_h
#define shared_h

#define debug

#include "inttypes.h"
#include "mpc.h"
#include "vector.h"

#ifndef _WIN32
#define __inline inline
#define PATH_SEPARATOR "/"
#else
#define PATH_SEPARATOR "\\"
#endif

int c2main(int32 argc, char** argv);
mpc_ast_t* c2parse(char* filename);
int32 recipemain(int32 argc, char** argv);

char* findrecipe();
void parserecipe(char* recipetext);
void handleline(char* line);
void runtests();

static __inline void _test(char* file, int32 line)
{
	printf("%s:%d\n", file, line);
}
#define test() do { _test(__FILE__, __LINE__); } while(0)

static __inline void remove_element(mpc_ast_t** arr, int32 index, int32 array_length)
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

#endif
