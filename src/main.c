#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#ifdef _WIN32
#include <io.h>
#else
#include <unistd.h>
#endif

#include "arg.h"
#include "util.h"
#include "shared.h"
#include "errors.h"
#include "types.h"

int8 option(char**, int32*);
void help();
void usage();

int32 main(int32 argc, char** argv)
{
	init_errors();

    opts = calloc(sizeof(options), 1);
    opts->wanted_targets = malloc(sizeof(vector)); vector_init(opts->wanted_targets);
    ARGBEGIN
    {
        FLAG('h', help())
        FLAG('d', chdir((++argv)[0]))
        FLAG('?', usage());
        FLAG('f', opts->file_mode = 1)
        FLAG('o', opts->target_name = (++argv)[0])
        FLAG('a', opts->print_ast1 = 1)
        FLAG('A', opts->print_ast2 = 1)
        FLAG('t', opts->test = 1)
        FLAG('e', opts->experiment = 1);
        FLAG('i', opts->print_info = 1);
        default:
            printf("unrecognized option: %c\n", ARGC());
            break;
    }
    ARGEND
    if(opts->file_mode)
    {
        target_t* trg = calloc(sizeof(target_t), 1);
        trg->files = malloc(sizeof(vector));
        trg->name = (opts->target_name ? opts->target_name : "dummy");
        trg->type = EXECUTABLE;

        vector_init(trg->files);

        for(int i = 0; i < vector_total(opts->wanted_targets); i++)
            vector_add(trg->files, vector_get(opts->wanted_targets, i));

        processtarget(trg);
    }
    else
    {
        if(vector_total(opts->wanted_targets) == 0)
            vector_add(opts->wanted_targets, "all");
        init_recipe(argc, argv);
    }
    return 0;
}

int8 option(char** argv, int32* argc)
{
    if(!argv[0]) return 1;
    if(argv[0][0] == '-' && strlen(argv[0]) > 1 && argv[0][1] != '-') return 1;
    START_OPTION("--help", help())
          OPTION("--usage", usage())
          OPTION("--file", opts->file_mode = 1)
          OPTION("--output", opts->target_name = (++argv)[0]; (*argc)++)
          OPTION("--ast0", opts->print_ast1 = 1)
          OPTION("--ast1", opts->print_ast2 = 1)
          OPTION("--test", opts->test = 1)
          OPTION("--experiment", opts->experiment = 1)
          PARAMETER("--dir", chdir(++argv[0]); (*argc)++)
    else if(strncmp(argv[0], "--", 2) == 0) printf("unrecognized option: %s", argv[0]);
    else
        vector_add(opts->wanted_targets, argv[0]);
    return 1;
}

void help()
{
    printf("OPTIONS:                                         \n"
           "--help -h                         print this text\n"
           "--dir -d                              chdir first\n"
           "--usage -?              print simple usage string\n"
           "--file -f                     switch to file-mode\n"
           "--ast0 -a                           print raw AST\n"
           "--ast1 -A                         print clean AST\n"
           "--output -o name     change file-mode target name\n"
           "--info -i                       print information\n"
    );
    exit(0);
}

void usage()
{
    puts("c2tc [-heidofatA?] [--ast0] [--ast1] [--test] [--info] "
         "[--experiment] [--output name] [--file] [--dir dir] targets...");
    puts("use -h for more info");
    exit(0);
}
