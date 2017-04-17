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

//to avoid conflicts with everything and anything
#define SYMBOL_TYPE(x) #x
static char* symbol_type[]
{
    SYMBOL_TYPE(func),
    SYMBOL_TYPE(typedef),
    SYMBOL_TYPE(declaration),
    SYMBOL_TYPE(aray_increment),
    SYMBOL_TYPE(type),
    SYMBOL_TYPE(index),
    SYMBOL_TYPE(parameter),
    SYMBOL_TYPE(statement),
    SYMBOL_TYPE(expression),
    SYMBOL_TYPE(struct),
    SYMBOL_TYPE(alias),
    SYMBOL_TYPE(enum),
    SYMBOL_TYPE(func_type),
    SYMBOL_TYPE(union),
    SYMBOL_TYPE(variable),
    SYMBOL_TYPE(literal),
    SYMBOL_TYPE(compound_decl),
    SYMBOL_TYPE(attribute)
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
    int* file_count;
    int* module_count;
    int* option_count;
    bin_type type;
    vector* files;
    vector* modules;
    vector* options;
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
