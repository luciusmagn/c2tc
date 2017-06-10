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
    uint8 generate_c;
    uint8 generate_ir; //this option is ignored by c2tc, but might still come in useful to others
    uint8 refs;
    int file_count;
    int module_count;
    char* name;
    bin_type type;
    vector* use;
    vector* deps;
    vector* trees;
    vector* files;
    vector* config;
    vector* modules;
    vector* warnings;
} target_t;
#endif
