#ifndef c2_h
#define c2_h
#include "mpc.h"
#include "util.h"
#include "recipe.h"

#define ALIASTYPE 0x00
#define FUNCTYPE 0x01
#define STRUCTTYPE 0x02

void cleanup_trg(target* trg);
void cleanup(mpc_ast_t* node);
void deluseless(mpc_ast_t* node);
void simplify_tags(mpc_ast_t* node);
#endif
