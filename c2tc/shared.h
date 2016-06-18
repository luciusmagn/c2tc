#ifndef shared_h
#define shared_h

#include "inttypes.h"
#include "mpc.h"
#include "vector.h"
int32 c2main(int32 argc, char** argv);
int32 recipemain(int32 argc, char** argv);

int32 lsize;
FILE* current;
char* currenttxt;
char* commentless;
mpc_ast_t* currenttree;
mpc_ast_t* recipetree;
vector* trees;



#endif