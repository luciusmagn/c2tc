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
#include "c2.h"

void processrecipe()
{
	test();
	printf("number of targets:%d\n", recipe->count);
	for (int32 i = 0; i < recipe->count; i++)
	{
		processtarget(vector_get(recipe->targets, i));
	}
}

void processtarget(target* trg)
{
	trg->nodes = malloc(sizeof(vector));
	vector_init(trg->nodes);
	for (int32 i = 0; i < vector_total(trg->files); i++)
	{
		mpc_ast_t* ast = malloc(sizeof(mpc_ast_t*));
		ast = c2parse(vector_get(trg->files, i));

		if (ast)
		{
			vector_add(trg->nodes, ast);
			printf("module name: %s\n", ast->children[1]->children[0]->children[1]->contents);
		}
		else
		{
			throw(&eparsef, get_file_error(vector_get(trg->files, i)));
			log_info("compilation halted, exiting");
			exit(-1);
		}
	}
	cleanup_trg(trg);
}