#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#ifdef _WIN32
#include <io.h>
#include <direct.h>
#else
#include <unistd.h>
#endif

#include "util.h"
#include "mpc.h"
#include "shared.h"
#include "recipe.h"
#include "errors.h"

#ifndef R_OK
#define R_OK _A_NORMAL
#endif

#define parser mpc_parser_t*
mpc_ast_t* ex_c2parse(char* filename)
{
    //basic
    parser start = mpc_new("start");
    parser end = mpc_new("end");
    parser ptrop = mpc_new("ptrop");
    parser assigop = mpc_new("assigop");
    parser ident = mpc_new("ident");
    parser symbol = mpc_new("symbol");
    parser number = mpc_new("number");
    parser character = mpc_new("character");
    parser string = mpc_new("string");
    parser public_m = mpc_new("public");
    parser val = mpc_new("val");
    parser natives = mpc_new("natives");
    parser index = mpc_new("index");

    //function
    parser arg = mpc_new("arg");
    parser args = mpc_new("args");

    //types
    parser type = mpc_new("type");
    parser member = mpc_new("member");
    parser member_block = mpc_new("member_block");
    parser alias = mpc_new("alias");
    parser uniontype = mpc_new("union");
    parser global_union = mpc_new("global_union");
    parser functype = mpc_new("functype");
    parser structure = mpc_new("struct");
    parser usertype = mpc_new("usertype");

    //head
    parser module = mpc_new("module");
    parser import = mpc_new("import");

    //head and shoulders
    parser head = mpc_new("head");
    parser body = mpc_new("body");
    parser c2 = mpc_new("c2");

    mpc_err_t* err = mpca_lang(MPCA_LANG_DEFAULT,
        " start                             : /^/ ;                                                                \n"
        " end                               : /$/ ;                                                                \n"
        " ptrop        \"Pointer operator\" : '*' ;                                                                \n"
        " assigop   \"Assignment operator\" :( \"=\"  | \"+=\" | \"-=\" | \"*=\"                                   \n"
        "                                   |  \"/=\" | \"|=\" | \"~=\" | \"&=\" );                                \n"
        " ident              \"Identifier\" : /[a-zA-Z_][a-zA-Z0-9_]*/ ;                                           \n"
        " symbol                 \"Symbol\" : (<ident> '.')? <ident> ('.'<ident>)* ;                               \n"
        " number                 \"Number\" : /[0-9]+/ ;                                                           \n"
        " character       \"Any character\" : /'\\\\?.'/ ;                                                         \n"
        " string         \"String literal\" : /\"(\\\\.|[^\"])*\"/ ;                                               \n"
        " public                            : (\"public\")? ;                                                      \n"
        " val      \"Floating-point value\" : /[0-9]+(\\.[0-9]+)*[a-zA-Z_]*/ ;                                     \n"
        " natives           \"Native type\" : \"char\"    | \"bool\"                                               \n"
        "                                   | \"int8\"    | \"uint8\"                                              \n"
        "                                   | \"int16\"   | \"uint16\"                                             \n"
        "                                   | \"int32\"   | \"uint32\"                                             \n"
        "                                   | \"int64\"   | \"uint64\"                                             \n"
        "                                   | \"float32\" | \"float64\" ;                                          \n"
        " index                   \"Index\" : '[' ( '+' | <number> | <symbol> )? ']' ;                             \n"
        " arg                               : <type> <ident> ;                                                     \n"
        " args      \"Function parameters\" : '(' (<arg> (',' <arg>)*)* ')' ;                                      \n"
        " type                     \"Type\" : \"const\"? (<natives> | <symbol>) <ptrop>* <index>* ;                \n"
        " member                 \"Member\" : (<type> <ident> ';' | <union>) ;                                     \n"
        " member_block          \"Members\" : '{' <member>* '}' ;                                                  \n"
        " alias                   \"Alias\" : <ident> <type> ';' ;                                                 \n"
        " union                   \"Union\" : \"union\" <ident>? <member_block> ;                                  \n"
        " global_union       \"Union type\" : <ident> \"union\" <member_block> ;                                   \n"
        " functype        \"Function type\" : <ident> \"func\" <type> <args> ';' ;                                 \n"
        " struct          \"Struct\"        : <ident> \"struct\" <member_block> ;                                  \n"
        " usertype    \"User-defined type\" : <public> \"type\" ( <alias>  | <functype>                            \n"
        "                                                       | <struct> | <global_union>) ;                     \n"
        " module                 \"Module\" : \"module\" <ident> ';' ;                                             \n"
        " import                 \"Import\" : \"import\" <ident> (\"as\" <ident>)? (\"local\")? ';' ;              \n"
        " head                              : <module> <import>* ;                                                 \n"
        " body                              : (<usertype>)* ;                                                      \n"
        " c2                                : <start> <head> <body> <end> ;                                        \n",
        start, end, ptrop, assigop, ident, symbol, number, character, string, public_m, val, natives, index,
        arg, args, member, member_block,
        type, alias, uniontype, global_union, functype, structure, usertype,
        module, import,
        head, body, c2, NULL
        );
    mpc_optimise(c2);
    if (err != NULL)
    {
        mpc_err_print(err);
        mpc_err_delete(err);
        if(!opts->test)
            exit(1);
    }


    if (access(filename, R_OK) == 0)
    {
        current = fopen(filename, "rb");
        if (!current)
        {
            perror(filename);
            exit(1);
        }

        fseek(current, 0L, SEEK_END);
        lsize = ftell(current);
        rewind(current);

        currenttxt = calloc(1, lsize + 1);
        if (!currenttxt)
        {
            fclose(current);
            throw(&emafail, "Failed to alloc memory");
            exit(1);
        }

        if (fread(currenttxt, lsize, 1, current) != 1)
        {
            fclose(current);
            free(currenttxt);
            throw(&erdfail, "Failed to read file");
            exit(1);
        }
        fclose(current);

        //comment skip
        char* temp = calloc(sizeof(char) * strlen(currenttxt), 1);
        for (int32 i = 0, k = 0; i < strlen(currenttxt) + 1; i++, k++)
        {
            if(strncmp(&currenttxt[i], "//", 2) == 0)
                while(currenttxt[i] != '\n' && currenttxt[i] != '\0') i++;
            if(currenttxt[i] == '/' && currenttxt[i+1] == '*')
            {
                i += 2;
                while(currenttxt[i] != '*' && currenttxt[i+1] != '/' && currenttxt[i] != '\0') i++;
                if(currenttxt[i] != '\0')
                    i += 2;
            }
            temp[k] = currenttxt[i];
        }
        commentless = malloc(sizeof(char) * strlen(temp));
        strcpy(commentless, temp);
        free(currenttxt);
    }
    else
        throw(&enoaccs, "Cannot access file");

    mpc_result_t r;
    if (mpc_parse(filename, commentless, c2, &r))
        return r.output;
    else
    {
        mpc_err_print(r.error);
        mpc_err_delete(r.error);
    }

    mpc_cleanup(30, start, end, ptrop, assigop, ident, symbol, number, character, string,
                    public_m, val, natives, index,
                    arg, args, member, member_block,
                    type, alias, uniontype, global_union, functype, structure, usertype,
                    module, import,
                    head, body, c2);

    return NULL;
}
#undef parser
