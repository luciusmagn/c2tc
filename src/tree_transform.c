#include <stdio.h>

#include "tree_transform.h"
#include "util.h"
#include "shared.h"

void cleanup_trg(target* trg)
{
    for (int32 i = 0; i < vector_total(trg->nodes); i++)
    {
        cleanup(vector_get(trg->nodes, i));
        simplify_tags(vector_get(trg->nodes, i));
    }

    for (int32 i = 0; i < vector_total(trg->nodes); i++)
        if(opts->print_ast2)
            mpc_ast_print(vector_get(trg->nodes, i));
}

void cleanup(mpc_ast_t* node)
{
    deluseless(node);
    for (int32 i = 0; i < node->children_num; i++)
        cleanup(node->children[i]);
}

void r_elem(mpc_ast_t** arr, int index, int len)
{
    for(int i = index; i < len; i++)
        arr[i] = arr[i+1];
}

/*macros for deluseless()*/
#define REM  { found++;node->children_num--;r_elem(node->children, i, node->children_num); break; }
#define HAS_TAG(x)  (strcmp(node->children[i]->tag, x) == 0)
#define HAS_CONTENTS(x) (strcmp(node->children[i]->contents, x) == 0)

#define REM_BY_TAG(x)     if(HAS_TAG(x))                    REM
#define REM_BY_CONTENT(x) if(HAS_CONTENTS(x))               REM
#define REM_BY_BOTH(x,y)  if(HAS_TAG(x) && HAS_CONTENTS(y)) REM

#define REM_CHAR(x)       REM_BY_BOTH("char", x)
#define REM_STRING(x)     REM_BY_BOTH("string", x)

#define START_REM   int32 found = 0; for(;;) { for(int i = 0; i< node->children_num; i++)
#define END_REM     if(found==0) return; found = 0; }

void deluseless(mpc_ast_t* node)
{
    START_REM
    {
        REM_CHAR(":")
        REM_CHAR(";")
        REM_CHAR("\"")
        REM_CHAR("'")
        REM_CHAR("(")
        REM_CHAR(")")
        REM_CHAR("{")
        REM_CHAR("}")
        REM_STRING("module")
        REM_STRING("import")
        REM_STRING("as")
        REM_STRING("@(")
        REM_BY_TAG("start|regex")
        REM_BY_TAG("end|regex")
    }
    END_REM
}


#define NORMAL(x)       if(strcmp(node->tag, x "|>") == 0) { node->tag = x; found++; }
#define CHANGE(x,y)     if(strcmp(node->tag, x) == 0) { node->tag = y; found++; }
#define REGEX(x)        REPLACE(x "|regex", x)
#define STRING(x)       REPLACE(x "|string", x)
#define REPLACE(x,y)    { char* old = node->tag; node->tag = str_replace(old,x,y); if(strcmp(old, node->tag)) found++; }

#define SIMPLIFY_START  for(int32 found = 0;;) {
#define SIMPLIFY_END    if(found!=0)found = 0;else return; }

void simplify_tags(mpc_ast_t* node)
{
    for(int i = 0; i < node->children_num; i++)
        simplify_tags(node->children[i]);

    SIMPLIFY_START
    {
        NORMAL("head")
        NORMAL("body")
        NORMAL("module")
        NORMAL("import")

        REPLACE("body|func", "func")
        REPLACE("body|usertype", "usertype")
        REPLACE("body|decl", "decl")
        REPLACE("body|arrayincr", "arrayincr")

        NORMAL("index")

        NORMAL("decl")
        NORMAL("vardecl")
        NORMAL("cmpddecl")
        NORMAL("init")
        NORMAL("arrayincr")

        NORMAL("usertype")
        NORMAL("structure")
        NORMAL("memberblock")
        NORMAL("member")
        NORMAL("type")
        NORMAL("globalunion")
        NORMAL("uniontype")
        NORMAL("alias")
        NORMAL("enumtype")
        NORMAL("enumeration")

        NORMAL("attribute")
        NORMAL("attrtype")
        NORMAL("attrparam")

        NORMAL("args")
        NORMAL("arg")

        NORMAL("label")
        NORMAL("jump")
        NORMAL("expstmt")
        NORMAL("compound")
        NORMAL("declstmt")
        NORMAL("loop")
        NORMAL("branch")
        NORMAL("func")

        NORMAL("params")
        NORMAL("exp")
        NORMAL("asexp")
        NORMAL("elexp")
        NORMAL("lexp")
        NORMAL("bexp")
        NORMAL("eexp")
        NORMAL("rexp")
        NORMAL("sexp")
        NORMAL("aexp")
        NORMAL("mexp")
        NORMAL("cast")
        NORMAL("uexp")
        NORMAL("pfexp")
        NORMAL("pexP")

        NORMAL("ident")
        NORMAL("integer")
        NORMAL("floatn")
        NORMAL("character")
        NORMAL("stringlit")

        REGEX("ident")
        REGEX("integer")
        REGEX("floatn")
        REGEX("character")
        REGEX("stringlit")

        STRING("public")
        STRING("natives")
        STRING("attrtype")

        CHANGE("head|module|>", "module")
        CHANGE("type|natives", "natives")

        REPLACE("number|integer", "integer")
        REPLACE("number|floatn", "floatn")

        REPLACE("exp|asexp", "asexp")
        REPLACE("asexp|elexp", "elexp")
        REPLACE("elexp|lexp", "lexp")
        REPLACE("lexp|bexp", "bexp")
        REPLACE("bexp|eexp", "eexp")
        REPLACE("eexp|rexp", "rexp")
        REPLACE("rexp|sexp", "sexp")
        REPLACE("sexp|aexp", "aexp")
        REPLACE("aexp|mexp", "mexp")
        REPLACE("mexp|cast", "cast")
        REPLACE("cast|uexp", "uexp")
        REPLACE("uexp|pfexp", "pfexp")
        REPLACE("pfexp|pexp", "pexp")

        REPLACE("pexp|ident", "ident")
        REPLACE("pexp|integer", "integer")
        REPLACE("pexp|character", "character")
        REPLACE("pexp|stringlit", "stringlit")

        REPLACE("decl|vardecl", "vardecl")
        REPLACE("decl|cmpddecl", "cmpddecl")

        REPLACE("stmt|expstmt", "expstmt")
        REPLACE("stmt|label", "label")
        REPLACE("stmt|compound", "compound")
        REPLACE("stmt|declstmt", "declstmt")
        REPLACE("stmt|loop", "loop")
        REPLACE("stmt|branch", "branch")
        REPLACE("stmt|jump", "jump")

        REPLACE("member|uniontype", "uniontype")
        REPLACE("member|structlet", "structlet")
    }
    SIMPLIFY_END
}
