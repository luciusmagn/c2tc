#ifndef types_h
#define types_h

#include "mpc.h"
#include "util.h"

typedef enum
{
    EXECUTABLE,
    LIB_STATIC,
    LIB_SHARED
} bin_type;

typedef enum
{
    START,
    INSIDE_TRG
} recipe_state;

typedef struct
{
    int64   target_count;
    char*   path;
    char*   contents;
    vector* targets;
} recipe_t;

typedef struct
{
    uint8 generate_c :1;
    uint8 generate_ir :1; //this option is ignored by c2tc, but might still come in useful to others
    uint8 refs :1;
    int* file_count;
    int* module_count;
    int* option_count;
    bin_type type;
    vector* files;
    vector* modules;
    vector* options;
    vector* warnings;
} target_t;

typedef struct
{
    char* name;
    vector imports;
    vector symbols;
    vector trees;
} module_t;

typedef struct
{
    unsigned int local :1;
    unsigned int with_alias :1;
    module_t* module;
} import_t;

#endif
