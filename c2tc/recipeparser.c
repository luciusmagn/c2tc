#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <io.h>

#include "mpc.h"
#include "inttypes.h"
#include "shared.h"
#include "recipe.h"

int32 recipemain(int32 argc, char** argv)
{
	mpc_parser_t* spaces = mpc_new("spaces");
	mpc_parser_t* ident = mpc_new("ident");
	mpc_parser_t* option = mpc_new("option");
	mpc_parser_t* type = mpc_new("type");
	mpc_parser_t* libtype = mpc_new("libtype");
	mpc_parser_t* file = mpc_new("file");
	mpc_parser_t* target = mpc_new("target");
	mpc_parser_t* recipe = mpc_new("recipe");

	mpc_err_t* err = mpca_lang(MPCA_LANG_WHITESPACE_SENSITIVE,
		//Hours wasted: 7
		" spaces      : /([ ]|\\t)*/ ;                                                                          \n"//
		" ident       : <spaces> /[a-zA-Z][a-zA-Z0-9_-]*/ <spaces> ;                                            \n"//
		" option      : <spaces> /\\$/ <ident>+ /\\s/+ ;                                                        \n"//
		" type        : \"target\" | \"lib\" ;                                                                  \n"//
		" libtype     : \"shared\" | \"static\" ;                                                               \n"//
		" file        : <spaces> /[a-zA-Z0-9_-\\.\\\\\\/]+/ <spaces> /\\s/+ ;                                   \n"//
		" target      : <spaces> <type> <ident> <libtype>? /\\s/+ (<option> | <file>)+ \"end\" <spaces> ;       \n"//
		" recipe      : /^/ <target>+ /$/;                        \n",
		spaces, ident, option, type, libtype, file, target,
		recipe, NULL
		);
	mpc_optimise(recipe);
	if (err != NULL)
	{
		mpc_err_print(err);
		mpc_err_delete(err);
		exit(1);
	}

	if (argc > 1)
	{
		char* commentlessrecipe;
		char* recipetxt;
#ifdef R_OK
		if (access(argv[1], R_OK) == 0)
#else
		if (access(argv[1], _A_NORMAL) == 0)
#endif
		{
			current = fopen(argv[1], "rb");
			if (!current)
			{
				perror(argv[1]);
				exit(1);
			}

			fseek(current, 0L, SEEK_END);
			lsize = ftell(current);
			rewind(current);

			recipetxt = calloc(1, lsize + 1);
			if (!recipetxt)
			{
				fclose(current);
				fputs("memory alloc fails", stderr);
				exit(1);
			}

			if (fread(recipetxt, lsize, 1, current) != 1)
			{
				fclose(current);
				free(recipetxt);
				fputs("entire read fails", stderr);
				exit(1);
			}
			fclose(current);

			//comment skip
			//puts(recipetxt);
			//puts("\n======================\n");
			char* temp = malloc(sizeof(char) * strlen(recipetxt));
			int8 flag = 0;
			int32 cursor = 0;
			for (int32 i = 0; i < strlen(recipetxt) + 1; i++)
			{
				if (recipetxt[i] != '#' && flag == 0)
				{
					temp[cursor] = recipetxt[i];
					cursor++;
				}
				else if (recipetxt[i] == '\n' && flag == 1)
				{
					flag = 0;
				}
				else if (recipetxt[i] == '#' && flag == 0)
				{
					flag = 1;
				}
			}
			commentlessrecipe = malloc(sizeof(char) * strlen(temp));
			strcpy(commentlessrecipe, temp);
			//puts(commentlessrecipe);
			free(recipetxt);
		}
		mpc_result_t r;
		if (mpc_parse(argv[1], commentlessrecipe, recipe, &r))
		{
			mpc_ast_print(r.output);
			recipe_walk(r.output, NULL);
			mpc_ast_delete(r.output);
		}
		else
		{
			mpc_err_print(r.error);
			puts("dump");
			puts("=======================");
			printf("failure:%s\nfilename:%s\n", r.error->failure, r.error->filename);
			mpc_err_delete(r.error);
		}
		//mpc_print(recipe);

	}
	else
	{
		mpc_result_t r;
		if (mpc_parse_pipe("<stdin>", stdin, recipe, &r))
		{
			mpc_ast_print(r.output);
			mpc_ast_delete(r.output);
		}
		else
		{
			mpc_err_print(r.error);
			mpc_err_delete(r.error);
		}
	}

	mpc_cleanup(6, spaces, ident, option, file, target, recipe);

	return 0;
}