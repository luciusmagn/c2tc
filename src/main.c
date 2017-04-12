#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "arg.h"
#include "util.h"
#include "shared.h"
#include "errors.h"

int8 option(char**, int32);
void help();
void usage();

int32 main(int32 argc, char** argv)
{
	init_errors();
	wanted_targets = malloc(sizeof(vector)); vector_init(desired_targets);
    ARGBEGIN
    {
        FLAG('h', help())
        FLAG('d', chdir((++argv)[0]))
        FLAG('?', usage());
        FLAG('f', mpc_ast_print(c2parse((++argv)[0])); return 0;)
        default:
            printf("unrecognized option: %c\n" ARGC());
            break;
    }
    ARGEND
    return 0;
}

int8 option(char** argv, int32* argc)
{
    if(!argv[0]) return 1;
    if(argv[0][0] == '-' && strlen(argv[0]) > 1 && argv[0][1] != '-') return 1;
    START_OPTION("--help", help())
         _OPTION("--usage", usage())
         _PARAMETER("--dir", chdir(++argv[0]); (*argc)++)
         _PARAMETER("--file", mpc_ast_print(c2parse((++argv[0])); exit(0);)
    else if(strncmp(argv[0], "--", 2) printf("unrecognized option: %s", argv[0]);
    else
        vector_add(wanted_targets, argv[0]);
}

void help()
{
    printf("not gonna lie, there is not many flags as of yet:\n"
           "--help -h                         print this text\n"
           "--dir -d                              chdir first\n"
           "--usage -?              print simple usage string\n");
    exit(0);
}

void usage()
{
    puts("c2tc [-hdf?] [--file file] [--dir dir] target1 target2...");
    puts("use -h for more info");
    exit(0);
}
