#ifndef shared_h
#define shared_h

#define debug

#include "types.h"
#include "inttypes.h"
#include "mpc.h"
#include "util.h"

#ifndef _WIN32
#define PATH_SEPARATOR "/"
#else
#define PATH_SEPARATOR "\\"
#endif

mpc_ast_t* c2parse(char* filename);
mpc_ast_t* ex_c2parse(char* filename);
void init_recipe(int32 argc, char** argv);

char* find_recipe();
void parserecipe(char* recipetext);
//void handleline(char* line);
void runtests();

void processrecipe();
void processtarget(target_t* trg);

extern void analyse(mpc_ast_t* ast);

#define test() do { printf("%s:%d\n", __FILE__, __LINE__); } while(0)

int32 lsize;
mpc_ast_t* currenttree;
mpc_ast_t* trees;
vector* modules;

typedef struct
{
    unsigned int file_mode  :1;
    unsigned int print_ast1 :1;
    unsigned int print_ast2 :1;
    unsigned int print_info :1;
    unsigned int library    :1; //if the file-mode target is a library
    unsigned int test       :1; //continue even when some errors occur
    unsigned int experiment :1; //experimental c2 parser
    vector* wanted_targets;
    char* target_name; //name for file-mode target
} options;

options* opts;
recipe_t* recipe;

#endif
