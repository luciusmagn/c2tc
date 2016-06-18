#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "inttypes.h"
#include "recipe.h"
#include "mpc.h"
#include "vector.h"

void recipe_walk(mpc_ast_t* tree, mpc_ast_t* parent)
{
	if (!parent)
	{
		puts("1");
		recipetree = malloc(sizeof(tree));
		puts("a");
		recipetree = tree;
		puts("b");
		recipe = malloc(sizeof(recipe_t));
		recipe->targets = malloc(sizeof(vector));
		puts("c");
		vector_init(recipe->targets);
		puts("d");
		recipe->count = 0;
		puts("e");
		puts("2");
	}
	else
	{
		puts("3");
		puts(tree->tag);
		if (strcmp(tree->tag, "target|>") == 0)
		{
			puts("4");
			target* trg = malloc(sizeof(target));
			trg->files = malloc(sizeof(vector));
			trg->options = malloc(sizeof(vector));
			currenttrg = trg;
			vector_init(currenttrg->files);
			vector_init(currenttrg->options);
			vector_add(recipe->targets, trg);
			recipe->count++;
			puts("5");
		}
		if (strcmp(tree->tag, "ident|>") == 0 && strcmp(parent->tag, "target|>") == 0)
		{
			puts("6");
			currenttrg->name = malloc(sizeof(char) * strlen(tree->children[1]->contents));
			currenttrg->name = strcpy(currenttrg->name, tree->children[1]->contents);
			puts("7");
		}
		if (strcmp(tree->tag, "ident|>") == 0 && strcmp(parent->tag, "file|>") == 0)
		{
			puts("8");
			vector_add(currenttrg->files, tree->children[1]->contents);
			puts("9");
		}
		if (strcmp(tree->tag, "type|string") == 0)
		{
			puts("10");
			if (strcmp(tree->contents, "target") == 0)
			{
				currenttrg->type = executable;
			}
			puts("11");
		}
		if (strcmp(tree->tag, "libtype|string") == 0 && strcmp(parent->children[0]->contents, "lib") == 0)
		{
			puts("12");
			if (strcmp(tree->children[0]->contents, "static") == 0)
			{
				currenttrg->type = libstatic;
			}
			else
			{
				currenttrg->type = libshared;
			}
			puts("13");
		}
		if (strcmp(tree->tag, "option|>") == 0 && strcmp(parent->tag, "target|>") == 0)
		{
			puts("14");
			option* opt = malloc(sizeof(option));
			opt->opts = malloc(sizeof(vector));
			opt->name = tree->children[2]->children[1]->contents;
			puts("15");
			if (tree->children_num > 4)
			{
				vector_init(opt->opts);
				puts("16");
				for (int32 i = 0; i < tree->children_num; i++)
				{
					puts("17");
					if (strcmp(tree->children[i]->tag, "ident|>") == 0)
					{
						vector_add(opt->opts, tree->children[i]->children[1]->contents);
						puts(tree->children[i]->children[1]->contents);
					}
					puts("18");
				}
			}
			vector_add(currenttrg->options, opt);
			puts("19");
		}
	}
	puts("20");
	for (int32 i = 0; i < tree->children_num; i++)
	{
		recipe_walk(tree->children[i], tree);
	}
	puts("21");
	if (!parent)
	{
		puts("22");
		for (int32 i = 0; i < recipe->count; i++)
		{
			for (int32 j = 0; j < ((target*)vector_get(recipe->targets, i))->options->total; j++)
			{
				puts("23");
				vector* trg = malloc(sizeof(vector));
				puts(((target*)vector_get(recipe->targets, i))->name);
				trg = ((target*)vector_get(recipe->targets, i))->options;
				for (int32 x = 0; x < trg->total; x++)
				{
					printf("   %s\n", vector_get(trg, x));
				}
				puts("24");
			}
		}
	}
}