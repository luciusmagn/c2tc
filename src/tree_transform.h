#ifndef c2_h
#define c2_h
#include "mpc.h"
#include "util.h"
#include "shared.h"

void cleanup_trg(target_t* trg);
void cleanup(mpc_ast_t* node);
void deluseless(mpc_ast_t* node);
void simplify_tags(mpc_ast_t* node);
#endif
