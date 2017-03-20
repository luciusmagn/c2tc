#ifndef C2PARSER_H
#define C2PARSER_H
#include "mpc.h"
#include "integers.h"

/// <summary>
///  This function is the c2 parser main entrypoint
/// </summary>
mpc_ast_t* c2main(int32 argc, char** argv);
/// <summary>
///  This function parses C2 from a file name given in the argument and returns AST
/// </summary>
mpc_ast_t* c2parse(char* filename);
#endif