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

#define parser(x) mpc_parser_t* x = mpc_new(#x)
mpc_ast_t* ex_c2parse(char* filename)
{
    //basic
    parser(start);      parser(end);
    parser(ptrop);      parser(index);
    parser(ident);      parser(symbol);
    parser(integer);    parser(character);
    parser(string);     parser(public);
    parser(floatn);     parser(natives);
    parser(number);     parser(constant);

    //expressions - here be dragons, lmao
    parser(uop);        parser(asop);   //unary operators;          assignment operators
    parser(pexp);       parser(pfexp);  //primary expression;       prefix expression
    parser(params);     parser(cast);   //function call parameters; cast expression
    parser(uexp);       parser(mexp);   //unary expression;         multiplicative expression
    parser(aexp);       parser(sexp);   //addition expression;      shift expression
    parser(rexp);       parser(eexp);   //relation expression;      equivalence expression
    parser(bexp);       parser(lexp);   //bit-wise expression;      boolean expression
    parser(elexp);      parser(asexp);  //conditional expression;   assignment expression
    parser(cexp);       parser(exp);    //constant expression;      expression (cheers!)

    //function
    parser(arg);        parser(args);

    //types
    parser(type);       parser(member);
    parser(memberblock);parser(alias);
    parser(uniontype);  parser(globalunion);
    parser(functype);   parser(structure);
    parser(enumeration);parser(enumtype);
    parser(usertype);

    //declarations
    parser(vardecl);    parser(cmpddecl);
    parser(init);       parser(decl);

    //head
    parser(module);
    parser(import);

    //head and shoulders
    parser(head);       parser(body);
    parser(c2);

    mpc_err_t* err = mpca_lang(MPCA_LANG_DEFAULT,
       /**************************************************************************************************************
        *============================================================================================================*
        *----------------------------------------------BASIC PARSERS-------------------------------------------------*
        *============================================================================================================*
        **************************************************************************************************************/
        " start                             : /^/ ;                                                                \n"
        " end                               : /$/ ;                                                                \n"
        " ptrop        \"Pointer operator\" : '*' ;                                                                \n"
        " uop                               :  '&' | '*' | '+' | '-' | '~' | '!'  ;                                \n"
        " asop   \"Assignment operator\" :( \"=\"  | \"+=\" | \"-=\" | \"*=\" | \"/=\"                             \n"
        "                                   |  \"&=\" | \"|=\" | \"~=\" | \"<<=\"| \">>=\" );                      \n"
        " ident              \"Identifier\" : /[a-zA-Z_][a-zA-Z0-9_]*/ ;                                           \n"
        " symbol                 \"Symbol\" : (<ident> '.')? <ident> ;                                             \n"
        " integer               \"Integer\" : /[0-9]+/ ;                                                           \n"
        " character       \"Any character\" : '\'' /\\\\?./ '\'' ;                                                 \n"
        " string         \"String literal\" : /\"(\\\\.|[^\"])*\"/ ;                                               \n"
        " public                            : (\"public\")? ;                                                      \n"
        " floatn   \"Floating-point value\" : /[0-9]+\\.[0-9]+[a-zA-Z]*/ ;                                         \n"
        " natives           \"Native type\" : \"void\"                                                             \n"
        "                                   | \"char\"    | \"bool\"                                               \n"
        "                                   | \"int8\"    | \"uint8\"                                              \n"
        "                                   | \"int16\"   | \"uint16\"                                             \n"
        "                                   | \"int32\"   | \"uint32\"                                             \n"
        "                                   | \"int64\"   | \"uint64\"                                             \n"
        "                                   | \"float32\" | \"float64\" ;                                          \n"
        " index                   \"Index\" : '[' ( '+' | <integer> | <symbol> )? ']' ;                            \n"
        " number                 \"Number\" :  <floatn> | <integer>  ;                                             \n"
        " constant             \"Constant\" :  <number> | <string> | <ident> ;                                     \n"
       /**************************************************************************************************************
        *============================================================================================================*
        *%%%%%%%%%%%%%%%/------------------------------------------------------------------------------\%%%%%%%%%%%%%*
        *%%%%%%%%%%%%%%%|                           Here lie EXPRESSIONS                               |%%%%%%%%%%%%%*
        *%%%%%%%%%%%%%%%|Pain in the ass to make, but now finally 100% working. No regrets, I am happy.|%%%%%%%%%%%%%*
        *%%%%%%%%%%%%%%%|                Will eat your dog if you try to modify it                     |%%%%%%%%%%%%%*
        *%%%%%%%%%%%%%%%\------------------------------------------------------------------------------/%%%%%%%%%%%%%*
        *============================================================================================================*
        *Hours wasted: 8;                                                                       --Lukáš Hozda, 2017  *
        **************************************************************************************************************/
        " pexp                              : <ident> | <number> | <string> | <character> |'(' <exp> ')' ;         \n"
        " pfexp                             : <pexp>                                                               \n"
        "                                   ( <params>                                                             \n"
        "                                   | '[' <exp> ']'                                                        \n"
        "                                   | '[' <integer> ':' <integer> ']'                                      \n"
        "                                   | '.' <ident>                                                          \n"
        "                                   | (\"++\"|\"--\")                                                      \n"
        "                                   )* ;                                                                   \n"
        " uexp                              : <pfexp>                                                              \n"
        "                                   | (\"++\"|\"--\") <uexp>                                               \n"
        "                                   | <uop> <cast>                                                         \n"
        "                                   | \"sizeof\" ( <uexp> | '(' <type> ')' ) ;                             \n"
        " cast                              : ( \"(->\" <type> ')' )? <uexp> ;                                     \n"
        " mexp                              : <cast> (('*'|'/'|'%') <cast>)* ;                                     \n"
        " aexp                              : <mexp> (('+'|'-') <mexp>)* ;                                         \n"
        " sexp                              : <aexp> (( \"<<\" | \">>\" ) <aexp>)* ;                               \n"
        " rexp                              : <sexp> (( \"<=\" | \">=\" | \"<\" | \">\" ) <sexp>)* ;               \n"
        " eexp                              : <rexp> ((\"==\"|\"!=\") <rexp>)* ;                                   \n"
        " bexp                              : <eexp> (('|'|'^'|'&') <eexp>)* ;                                     \n"
        " lexp                              : <bexp> ((\"&&\"|\"||\") <bexp>)* ;                                   \n"
        " elexp                             : <lexp> ('?' <lexp> ':' <lexp>)* ;                                    \n"
        " asexp                             : <elexp> (<asop> <asexp>)* ;                                          \n"
        " exp                \"Expression\" : <asexp> (',' <asexp> )* ;                                            \n"
        " params                            : '(' (<elexp> (',' <elexp>)*)? ')' ;                                  \n"
       /**************************************************************************************************************
        *============================================================================================================*
        *---------------------------------------------FUNCTION STUFF-------------------------------------------------*
        *============================================================================================================*
        **************************************************************************************************************/
        " arg                               : <type> <ident> ;                                                     \n"
        " args      \"Function parameters\" : '(' (<arg> (',' <arg>)*)* ')' ;                                      \n"
       /**************************************************************************************************************
        *============================================================================================================*
        *--------------------------------------------------TYPES-----------------------------------------------------*
        *============================================================================================================*
        **************************************************************************************************************/
        " type                     \"Type\" : \"const\"? (<natives>|<symbol>) <ptrop>* <index>* ;                  \n"
        " member                 \"Member\" : <uniontype> | (<type> <ident> (':' <integer>)? ';') ;                \n"
        " memberblock           \"Members\" : '{' <member>+ '}' ;                                                  \n"
        " alias                   \"Alias\" : <ident> <type> ';' ;                                                 \n"
        " uniontype               \"Union\" : \"union\" <ident>? <memberblock> ;                                   \n"
        " globalunion        \"Union type\" : <ident> \"union\" <memberblock> ;                                    \n"
        " functype        \"Function type\" : <ident> \"func\" <type> <args> ';' ;                                 \n"
        " structure              \"Struct\" : <ident> \"struct\" <memberblock> ;                                   \n"
        " enumeration                       : '{' <ident> ('='<integer>)? (',' <ident> ('='<integer>)?)* ','? '}' ;\n"
        " enumtype          \"Enumeration\" : <ident> \"enum\" <type> <enumeration> ;                              \n"
        " usertype    \"User-defined type\" : <public> \"type\" ( <structure>   | <enumtype>                       \n"
        "                                                       | <globalunion> | <functype>  | <alias> ) ;        \n"
       /**************************************************************************************************************
        *============================================================================================================*
        *----------------------------------------------DECLARATIONS--------------------------------------------------*
        *============================================================================================================*
        **************************************************************************************************************/
        " vardecl  \"Variable declaration\" : <type> <ident> ('=' <exp>)? ';' ;                                    \n"
        " init                              : '{'(('.'<ident> '=')? (<init>|<elexp>)                               \n"
        "                                    (',' ('.'<ident> '=')? (<init>|<elexp>))* '.'? )? '}';                \n"
        " cmpddecl \"Compound declaration\" : <type> <ident> '=' <init> ;                                          \n"
        " decl              \"Declaration\" : <public> (<vardecl> | <cmpddecl>) ;                                  \n"
       /**************************************************************************************************************
        *============================================================================================================*
        *-----------------------------------------------FILE STUFF---------------------------------------------------*
        *============================================================================================================*
        **************************************************************************************************************/
        " module                 \"Module\" : \"module\" <ident> ';' ;                                             \n"
        " import                 \"Import\" : \"import\" <ident> (\"as\" <ident>)? (\"local\")? ';' ;              \n"
        " head                              : <module> <import>* ;                                                 \n"
        " body                              : (<usertype> | <decl>)* ;                                             \n"
        " c2                                : <start> <head> <body> <end> ;                                        \n",
        /*BASIC*/
        start, end, ptrop, ident, symbol, integer, character, string, public, floatn, natives, index, number, constant,
        /*EXPRESSIONS*/
        pexp, pfexp, params, cast, uexp, uop, mexp, aexp, sexp, rexp, eexp,bexp, lexp, elexp, asexp, asop, cexp, exp,
        /*FUNCTIONS*/
        arg, args,
        /*TYPES*/
        member, memberblock, type, alias, uniontype, globalunion, functype, structure, enumeration, enumtype, usertype,
        /*DECLARATIONS*/
        vardecl, init, cmpddecl, decl,
        /*FILE STUFF*/
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
                while(strncmp(&currenttxt[i], "*/", 2) != 0 && currenttxt[i] != '\0') i++;
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
                    /*BASIC*/
    mpc_cleanup(55, start, end, ptrop, ident, symbol, integer, character, string,
                    public, floatn, natives, index, number, constant,
                    /*EXPRESSIONS*/
                    pexp, pfexp, params, cast, uexp, uop, mexp,
                    aexp, sexp, rexp, eexp, bexp,
                    lexp, elexp, asexp, asop, cexp, exp,
                    /*FUNCTIONS*/
                    arg, args,
                    /*TYPES*/
                    member, memberblock, type, alias, uniontype, globalunion,
                    functype, structure, enumeration, enumtype, usertype,
                    /*DECLARATIONS*/
                    vardecl, init, cmpddecl, decl,
                    /*FILE STUFF*/
                    module, import,
                    head, body, c2);

    return NULL;
}
#undef parser
