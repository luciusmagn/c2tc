#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "util.h"
#include "types.h"
#include "mpc.h"
#include "shared.h"
#include "errors.h"
#include "log.h"
#include "tree_transform.h"

void processrecipe()
{
    if(opts->print_info) printf("number of targets:%ld\n", recipe->target_count);
    for (int32 i = 0; i < recipe->target_count; i++)
        processtarget(vector_get(recipe->targets, i));
}

void processtarget(target_t* trg)
{
    trg->trees = malloc(sizeof(vector));
    vector_init(trg->trees);
    int success = 0;
    if(opts->test)
        printf(ANSI_YELLOW "target: " ANSI_GREEN "%s " ANSI_RESET "\n", trg->name);
    for(int32 i = 0; i < vector_total(trg->files); i++)
    {
        mpc_ast_t* ast = malloc(sizeof(mpc_ast_t*));
        ast = c2parse(vector_get(trg->files, i));

        if(ast)
        {
            vector_add(trg->trees, ast);
            if(opts->print_info)
                printf("module name: %s\n", MODULE_PRE(ast)->contents);
            if(opts->print_ast1)
                mpc_ast_print(ast);
            if(opts->test)
                printf(ANSI_MAGENTA "\tfile:" ANSI_CYAN " %s " ANSI_GREEN "[SUCCESS]" ANSI_RESET "\n",
                      (char*)vector_get(trg->files, i));
            success++;
        }
        else if(!opts->test)
        {
            throw(&eparsef, get_file_error(vector_get(trg->files, i)));
            log_info("compilation failed, exiting");
            exit(-1);
        }
        else
            printf(ANSI_MAGENTA "\tfile:" ANSI_CYAN " %s " ANSI_RED "[FAILURE]" ANSI_RESET "\n",
                  (char*)vector_get(trg->files, i));
    }
    if(opts->test)
        printf(ANSI_GREEN "TOTAL: " ANSI_YELLOW "%d/%d files parsed successfully" ANSI_RESET "\n",
               success, vector_total(trg->files));

    cleanup_trg(trg);
    for(int32 i = 0; i < vector_total(trg->trees); i++)
        analyse(vector_get(trg->trees, i));
}
