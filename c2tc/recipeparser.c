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
#include "stringutils.h"
#include "microtest.h"

int32 recipemain(int32 argc, char** argv)
{
    char* recipepath = findrecipe();
    if (argc > 0)
    {
        char* commentlessrecipe;
        char* recipetxt;
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

            char* temp = calloc(sizeof(char) * strlen(recipetxt), 1);
            for (int32 i = 0, k = 0; i < strlen(recipetxt) + 1; i++, k++)
            {
                if(recipetxt[i] == '#') while(recipetxt[i] != '\n') i++;
                commentless[k] = recipetxt[i];
            }
            commentlessrecipe = malloc(sizeof(char) * strlen(temp));
            strcpy(commentlessrecipe, temp);
            parserecipe(commentlessrecipe);
            free(recipetxt);
        }
        else
            throw(&enoaccs, "Cannot access recipe file");
    }
    else
        throw(&enoaccs, "Not enough command-line arguments");
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
    getcwd(buf, 1024);
    while (strcmp(buf, "/") != 0)
#endif
    {
#ifdef _WIN32
        if (access("recipe.txt", _A_NORMAL) == 0)
#else
        if (access("recipe.txt", R_OK) == 0)
#endif
        {
            char* path = malloc(1024);
#ifdef _WIN32
            _getcwd(path, 1024);
#else
            getcwd(path, 1024);
#endif
            strcat(path, PATH_SEPARATOR "recipe.txt");
            return path;
        }
        chdir("../");
    }
    return 0;
}

void parserecipe(char* recipetext)
{
    state = START;
    char* nocarriage = str_replace(recipetext, "\r", " ");
    char** lines = strsplit((nocarriage ? nocarriage : recipetext), "\n");
    int32 lines_num = occurences(recipetext, '\n');
    recipe = malloc(sizeof(recipe_t));
    recipe->targets = malloc(sizeof(vector));
    recipe->count = 0;
    vector_init(recipe->targets);

    for (int32 i = 0; i <= lines_num; i++)
        handleline(lines[i]);

    processrecipe();
}

void handleline(char* line)
{
    if (issornull(line)) return;
    char** words = strsplit(line, " ");
    int32 words_num = occurences(line, ' ');
    if (!words) return;
    switch (state)
    {
        case START:
            if (words_num < 1)
                throw(&enotoks, "Not enough tokens");
            if (strcmp(words[0], "target") == 0)
            {
                currenttrg = malloc(sizeof(target));
                currenttrg->name = words[1];
                currenttrg->type = executable;
                currenttrg->files = malloc(sizeof(vector));
                vector_init(currenttrg->files);
                currenttrg->options = malloc(sizeof(vector));
                vector_init(currenttrg->options);
                state = INSIDE_TARGET;
            }
            else if (strcmp(words[0], "lib") == 0)
            {
                if (words_num < 2)
                    throw(&enotoks, "Not enough tokens");

                currenttrg = malloc(sizeof(target));
                currenttrg->name = words[1];
                if (strcmp(words[2], "static") == 0)
                {
                    currenttrg->type = libstatic;
                    currenttrg->files = malloc(sizeof(vector));
                    vector_init(currenttrg->files);
                    currenttrg->options = malloc(sizeof(vector));
                    vector_init(currenttrg->options);
                    state = INSIDE_TARGET;
                }
                else if (strcmp(words[2], "shared") == 0)
                {
                    currenttrg->type = libshared;
                    currenttrg->files = malloc(sizeof(vector));
                    vector_init(currenttrg->files);
                    currenttrg->options = malloc(sizeof(vector));
                    vector_init(currenttrg->options);
                    state = INSIDE_TARGET;
                }
                else
                    throw(&ebadtok, "Bad token in recipe");
            }
            else if(words[0][0] == '#')
                return;
            else
                throw(&ebadtok, "Bad token in recipe");
            break;
        case INSIDE_TARGET:
            if (strcmp(words[0], "end") == 0)
            {
                vector_add(recipe->targets, currenttrg);
                recipe->count++;
                state = START;
            }
            else if (words[0][0] == '$')
            {
                option* opt = malloc(sizeof(option));
                opt->name = malloc(strlen(words[0]) + 1);
                opt->name = words[0];
                opt->opts = malloc(sizeof(vector));
                vector_init(opt->opts);

                for (int32 i = 1; i < words_num; i++)
                    vector_add(opt->opts, words[i]);

                vector_add(currenttrg->options, opt);
            }
            else if (words_num >= 1)
                vector_add(currenttrg->files, words[0]);
            else
                throw(&ebadtok, "Unexpected token in recipe");
            break;
    }
}
