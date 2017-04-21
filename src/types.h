#ifndef recipe_h
#define recipe_h

#include "mpc.h"
#include "util.h"
#include "shared.h"

enum bin_type
{
    EXECUTABLE,
    LIB_STATIC,
    LIB_SHARED
}


typedef struct
{
    int     module_count;
    char*   path;
    char*   contents;
    vector* targets;
} recipe;

typedef struct
{
    uint8 generate_c :1;
    uint8 generate_ir :1; //this option is ignored by c2tc, but still come in useful to others
    uint8 refs :1;
    int* file_count;
    int* module_count;
    int* option_count;
    bin_type type;
    vector* files;
    vector* modules;
    vector* options;
    vector* warnings;
} target;

typedef struct
{
    char* name;
    vector imports;
    vector symbols;
    vector trees;
} module;

typedef struct
{
    unsigned int local :1;
    unsigned int with_alias :1;
    module module;
}

#endif
