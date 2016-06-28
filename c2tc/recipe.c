#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "inttypes.h"
#include "recipe.h"
#include "mpc.h"
#include "vector.h"
#include "shared.h"
#include "errors.h"
#include "throw.h"
#include "log.h"

void processrecipe()
{
	puts("test");
	printf("%d\n", recipe->count);
	printf("%d\n", vector_total(recipe->targets));
	for (int32 i = 0; i < recipe->count; i++)
	{
		processtarget(vector_get(recipe->targets, i));
	}
}

void processtarget(target* trg)
{
	for (int32 i = 0; i < vector_total(trg->files); i++)
	{
		mpc_ast_t* ast = malloc(sizeof(mpc_ast_t*));
		ast = c2parse(vector_get(trg->files, i));

		if (ast)
		{
			puts(ast->children[1]->children[0]->children[1]->contents);
		}
		else
		{
			throw(&eparsef, get_file_error(vector_get(trg->files, i)));
			log_info("compilation halted, exiting");
			exit(-1);
		}
	}
}