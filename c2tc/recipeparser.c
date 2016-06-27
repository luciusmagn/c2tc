#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#ifdef _WIN32
#include <io.h>
#include <direct.h>
#else
#include <unistd.h>
#endif

#include "mpc.h"
#include "inttypes.h"
#include "shared.h"
#include "recipe.h"
#include "throw.h"
#include "errors.h"

/// <summary>
///  Entry point for the recipe parser
/// </summary>
int32 recipemain(int32 argc, char** argv)
{
	char* recipepath = findrecipe();
	if (argc > 0)
	{
		char* commentlessrecipe;
		char* recipetxt;
		puts(recipepath);
#ifdef R_OK
		if (access(recipepath, R_OK) == 0)
#else
		if (access(recipepath, _A_NORMAL) == 0)
#endif
		{
			current = fopen(recipepath, "rb");
			if (!current)
			{
				perror(recipepath);
				exit(1);
			}

			fseek(current, 0L, SEEK_END);
			lsize = ftell(current);
			rewind(current);

			recipetxt = calloc(1, lsize + 1);
			if (!recipetxt)
			{
				fclose(current);
				throw(&emafail, "Failed to alloc memory");
				exit(1);
			}

			if (fread(recipetxt, lsize, 1, current) != 1)
			{
				fclose(current);
				free(recipetxt);
				throw(&erdfail, "Failed to read file");
				exit(1);
			}
			fclose(current);

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
					temp[cursor] = recipetxt[i];
					cursor++;
				}
				else if (recipetxt[i] == '#' && flag == 0)
				{
					flag = 1;
				}
			}
			commentlessrecipe = malloc(sizeof(char) * strlen(temp));
			strcpy(commentlessrecipe, temp);
			puts(commentlessrecipe);
			free(recipetxt);
		}
		else
		{
			throw(&enoaccs, "Cannot access recipe file");
		}

	}
	else
	{
		throw(&enoaccs, "Not enough command-line arguments");
	}
	return 0;
}

/// <summary>
///  This function loops through parent directories until it finds the project root directory
///  where a cwd is set and recipe.txt file is located. Returns path to recipe.txt
/// </summary>
char* findrecipe()
{
	char buf[1024];
#ifdef _WIN32
	_getcwd(buf, 1024);
	while (strlen(buf) != 3)
#else
	getcwd(buf, 1024)
	while (strcmp(buf, "/") != 0)
#endif
	{
#ifdef _WIN32
		if (access("recipe.txt", _A_NORMAL) == 0)
#else
		if (access("recipe.txt", R_OK) == 0)
#endif
		{
			char path[1024];
#ifdef _WIN32
			_getcwd(path, 1024);
#else
			getcwd(path, 1024);
#endif
			strcat(path, "\\recipe.txt");
			return path;
		}
		chdir("../");
	}
	return 0;
}

/// <summary>
///  This function parses the recipe file and builds a recipe structure from it.
/// </summary>
void parserecipe(char* recipetext)
{

}