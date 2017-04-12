#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#ifdef _WIN32
#include <io.h>
#else
#include <unistd.h>
#endif

#include "mpc.h"
#include "integers.h"
#include "shared.h"
#include "throw.h"
#include "errors.h"
#include "log.h"
#include "stringutils.h"
#include "microtest.h"

mpc_ast_t* c2main(int32 argc, char** argv)
{
    if (argc > 1)
        return c2parse(argv[1]);
    else
        return NULL;
}

mpc_ast_t* c2parse(char* filename)
{
    mpc_parser_t* start = mpc_new("start");
    mpc_parser_t* end = mpc_new("end");
    mpc_parser_t* unaryop = mpc_new("unaryop");
    mpc_parser_t* ptrop = mpc_new("ptrop");
    mpc_parser_t* ident = mpc_new("ident");
    mpc_parser_t* number = mpc_new("number");
    mpc_parser_t* character = mpc_new("character");
    mpc_parser_t* string = mpc_new("string");
    mpc_parser_t* attrtype = mpc_new("attrtype");
    mpc_parser_t* attrwval = mpc_new("attrwval");
    mpc_parser_t* attribute = mpc_new("attribute");
    mpc_parser_t* val = mpc_new("val");
    mpc_parser_t* emptyindices = mpc_new("emptyindices");
    mpc_parser_t* indices = mpc_new("indices");
    mpc_parser_t* anyindices = mpc_new("anyindices");
    mpc_parser_t* module = mpc_new("module");
    mpc_parser_t* import = mpc_new("import");
    mpc_parser_t* define = mpc_new("define");
    mpc_parser_t* natives = mpc_new("natives");
    mpc_parser_t* typeident = mpc_new("typeident");
    mpc_parser_t* member = mpc_new("member");
    mpc_parser_t* memblock = mpc_new("memblock");
    mpc_parser_t* structure = mpc_new("structure");
    mpc_parser_t* locunion = mpc_new("locunion");
    mpc_parser_t* globunion = mpc_new("globunion");
    mpc_parser_t* alias = mpc_new("alias");
    mpc_parser_t* decltype = mpc_new("decltype");
    mpc_parser_t* functype = mpc_new("functype");//TODO
    mpc_parser_t* arg = mpc_new("arg");
    mpc_parser_t* args = mpc_new("args");
    mpc_parser_t* head = mpc_new("head");
    mpc_parser_t* funcdecl = mpc_new("funcdecl");
    mpc_parser_t* factor = mpc_new("factor");
    mpc_parser_t* term = mpc_new("term");
    mpc_parser_t* lexp = mpc_new("lexp");
    mpc_parser_t* vardecl = mpc_new("vardecl");
    mpc_parser_t* funccall = mpc_new("funccall");
    mpc_parser_t* whileloop = mpc_new("whileloop");
    mpc_parser_t* dowhile = mpc_new("dowhile");
    mpc_parser_t* forloop = mpc_new("forloop");
    mpc_parser_t* ifstmt = mpc_new("ifstmt");
    mpc_parser_t* switchcase = mpc_new("switchcase");
    mpc_parser_t* switchstmt = mpc_new("switchstmt");
    mpc_parser_t* stmt = mpc_new("stmt");
    mpc_parser_t* exp = mpc_new("exp");
    mpc_parser_t* logic = mpc_new("logic");
    mpc_parser_t* block = mpc_new("block");
    mpc_parser_t* function = mpc_new("function");

    mpc_parser_t* body = mpc_new("body");
    mpc_parser_t* c2 = mpc_new("c2");

    //TODO enums
    mpc_err_t* err = mpca_lang(MPCA_LANG_DEFAULT,
        //general
        " start                             : /^/ ;                                                                \n"
        " end                               : /$/ ;                                                                \n"
        " unaryop        \"Unary operator\" : '&' | '!' | '-' | '~' | '+' ;                                        \n"
        " ptrop        \"Pointer operator\" : '*' ;                                                                \n"
        " ident              \"Identifier\" : /[a-zA-Z_\\.\\#][a-zA-Z0-9_-#]*/ ;                                   \n"
        " number                 \"Number\" : /[0-9]+/ ;                                                           \n"
        " character       \"Any character\" : /'.'/ ;                                                              \n"
        " string         \"String literal\" : /\"\"(\\\\.|[^\"])*\"/ ;                                             \n"
        " attrtype       \"Attribute type\" :(\"export\"    | \"packed\"                                           \n"
        "                                   |  \"unused\"   | \"unused_params\"                                    \n"
        "                                   |  \"noreturn\" | \"inline\"                                           \n"
        "                                   |  \"weak\"     | \"opaque\") ;                                        \n"
        " attrwval \"Param attribute type\" : (\"aligned\" | \"section\") '=' (<string>|<number>) ;                \n"
        " attribute  \"Attribute notation\" : \"@(\" (<attrwal>|<attrtype>) (',' (<attrwval>                       \n"
        "                                   |<attrtype>))* ')' ;                                                   \n"
        " val      \"Floating-point value\" : /[0-9]+(\\.[0-9]+)*[a-zA-Z_]*/ ;                                     \n"
        " emptyindices      \"Empty index\" : \"[]\";                                                              \n"
        " indices        \"Nonempty index\" : '[' (<ident> | <number> | '+' ) ']' ;                                \n"
        " anyindices              \"Index\" : (<indices> | <emptyindices>) ;                                       \n"
        " module        \"Module notation\" : \"module\" <ident> ';' ;                                             \n"
        " import       \"Import statement\" : \"import\" <ident> (';'                                              \n"
        "                                   | (\"local\" ';')                                                      \n"
        "                                   | (\"as\" <ident> ';')                                                 \n"
        "                                   | (\"as\" <ident> \"local\" ';')) ;                                    \n"
        " define           \"Define macro\" : \"#define\" <ident> /((\\w*)\\s)/ ;                                  \n"
        " natives           \"Native type\" : \"char\"    | \"bool\"                                               \n"
        "                                   | \"int8\"    | \"uint8\"                                              \n"
        "                                   | \"int16\"   | \"uint16\"                                             \n"
        "                                   | \"int32\"   | \"uint32\"                                             \n"
        "                                   | \"int64\"   | \"uint64\"                                             \n"
        "                                   | \"float32\" | \"float64\" ;                                          \n"
        " typeident           \"Type name\" : <natives> | <ident> ;                                                \n"
        " member     \"Member declaration\" : <decltype> <ident> ';' ;                                             \n"
        " memblock         \"Member block\" : '{' (<member> | <locunion>)* '}' ;                                   \n"
        " structure      \"Structure type\" : \"type\" <ident> \"struct\" <memblock> <attribute>? ;                \n"
        " locunion                \"Union\" : \"union\" (<ident> <memblock> | <memblock>) ;                        \n"
        " globunion               \"Union\" : \"type\" <ident> \"union\" <memblock> ;                              \n"
        " decltype     \"Declaration type\" : (\"const\")? <typeident> <ptrop>* <anyindices>* ;                    \n"
        " functype        \"Function type\" : (\"public\")? <ident> \"func\" <decltype> ;                          \n"
        " alias              \"Alias type\" : \"type\" <ident> <decltype> ';' ;                                    \n"
        " arg                  \"Argument\" : <decltype> <ident>;                                                  \n"
        " args                \"Arguments\" : '(' (<arg> ',')* <arg>? ')' ;                                        \n"
        " vardecl  \"Variable declaration\" : (\"public\")? <decltype> <ident> ('=' <lexp>)? ';' ;                 \n"
        " funcdecl \"Function declaration\" : (\"public\")? \"func\" <decltype> <ident> <args>;                    \n"
        " whileloop          \"While loop\" : \"while\" '(' <logic> ')' <stmt> ;                                   \n"
        " dowhile         \"Do-while loop\" : \"do\" <stmt> \"while\" '(' <logic> ')' ';' ;                           \n"
        " forloop              \"For loop\" : \"for\" '('(<vardecl>|';') (<logic>';'|';') <factor>? ')' <stmt>;    \n"
        " ifstmt           \"If statement\" : \"if\" '(' <logic> ')' <stmt> (\"else\" <stmt>)*  ;                  \n"
        " switchcase        \"Switch case\" : ((\"case\" <factor>) | \"default\") ':' <stmt>* ;                    \n"
        " switchstmt   \"Switch statement\" : \"switch\" '(' <factor> ')' '{' <switchcase>* '}' ;                  \n"
        " factor                 \"Factor\" : '(' <lexp> ')'                                                       \n"
        "                                   | <number>                                                             \n"
        "                                   | <character>                                                          \n"
        "                                   | <string>                                                             \n"
        "                                   | <ident> (\"++\"|\"--\")                                              \n"
        "                                   | (\"++\"|\"--\") <ident>                                              \n"
        "                                   | '~' <term>                                                           \n"
        "                                   | '&' <ident>                                                          \n"
        "                                   | \"(->\"<decltype>')' <ident>                                         \n"
        "                                   | <ident> '(' <lexp>? (',' <lexp>)* ')'                                \n"
        "                                   | <ident> ;                                                            \n"
        " term                     \"Term\" : <factor> (('*' | '/' | '%') <factor>)* ;                             \n"
        " lexp     \"Left-side expression\" : <term> (('+' | '-') <term>)* ;                                       \n"
        " funccall        \"Function call\" : <ident> '(' <ident>? (',' <ident>)* ')' ';' ;                        \n"
        " stmt                \"Statement\" : '{' <stmt>* '}'                                                      \n"
        "                                   | <vardecl>                                                            \n"
        "                                   | <whileloop>                                                          \n"
        "                                   | <dowhile>                                                            \n"
        "                                   | <forloop>                                                            \n"
        "                                   | <ifstmt>                                                             \n"
        "                                   | <switchstmt>                                                         \n"
        "                                   | <ident> '=' <lexp> ';'                                               \n"
        "                                   | \"return\" <lexp>? ';'                                               \n"
        "                                   | \"continue\" ';'                                                     \n"
        "                                   | \"break\" ';'                                                        \n"
        "                                   | <funccall> ;                                                         \n"
        " exp        \"Boolean expression\" : <lexp> '>' <lexp>                                                    \n"
        "                                   | <lexp> '<' <lexp>                                                    \n"
        "                                   | '!'<lexp>                                                            \n"
        "                                   | <lexp>                                                               \n"
        "                                   | <lexp> \"<<\" <lexp>                                                 \n"
        "                                   | <lexp> \">>\" <lexp>                                                 \n"
        "                                   | <lexp> \">=\" <lexp>                                                 \n"
        "                                   | <lexp> \"<=\" <lexp>                                                 \n"
        "                                   | <lexp> \"!=\" <lexp>                                                 \n"
        "                                   | <lexp> \"==\" <lexp> ;                                               \n"
        " logic     \"Boolean expressions\" : <exp> ((\"&&\" | \"||\") <exp>)* ;                                   \n"
        " block              \"Code block\" : '{' <stmt>* '}' ;                                                    \n"
        " function             \"Function\" : <funcdecl> <block> ;                                                 \n"
        " head                \"File head\" : <module> <import>* ;                                                 \n"
        " body                \"File body\" : (<define> | <structure>                                              \n"
        "                                   | <globunion> | <alias>                                                \n"
        "                                   | <function> | <vardecl>)*;                                            \n"
        " c2                                : <start> <head> <body> <end> ;                                        \n",
        start, end, unaryop, ptrop, ident, number, character, string, attrtype, attrwval, attribute, val, emptyindices, indices, anyindices, module,
        import, define, natives, typeident, member, memblock, structure, locunion, globunion, decltype, functype,
        alias, arg, args, funcdecl, factor, term, lexp, vardecl, funccall, stmt, funccall, whileloop, dowhile, forloop, ifstmt, switchcase, switchstmt, exp, logic, block, function, head, body, c2, NULL
        );
    mpc_optimise(c2);
    if (err != NULL)
    {
        mpc_err_print(err);
        mpc_err_delete(err);
        exit(1);
    }


#ifdef R_OK
    if (access(filename, R_OK) == 0)
#else
    if (access(filename, _A_NORMAL) == 0)
#endif
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
        char* temp = malloc(sizeof(char) * strlen(currenttxt));
        int8 flag = 0;
        int32 cursor = 0;
        for (int32 i = 0; i < strlen(currenttxt) + 1; i++)
        {
            if (currenttxt[i] != '/' && flag == 0)
            {
                temp[cursor] = currenttxt[i];
                cursor++;
            }
            else if (currenttxt[i] == '*' && currenttxt[i + 1] == '/' && flag == 1)
            {
                flag = 0;
                i++;
            }
            else if (currenttxt[i] == '\n' && flag == 2)
            {
                flag = 0;
                temp[cursor] = currenttxt[i];
                cursor++;
            }
            else if (currenttxt[i] == '/' && currenttxt[i + 1] == '/' && flag == 0)
            {
                flag = 2;
            }
            else if (currenttxt[i] == '/' && currenttxt[i + 1] == '*' && flag == 0)
            {
                flag = 1;
            }
            else if (currenttxt[i] == '\n')
            {
                temp[cursor] = currenttxt[i];
                cursor++;
            }
        }
        commentless = malloc(sizeof(char) * strlen(temp));
        strcpy(commentless, temp);
        free(currenttxt);
    }
    else
    {
        throw(&enoaccs, "Cannot access file");
    }

    mpc_result_t r;
    if (mpc_parse(filename, commentless, c2, &r))
    {
        mpc_ast_print(r.output);
        return r.output;
    }
    else
    {
        mpc_err_print(r.error);
        printf(ANSI_COLOR_YELLOW "%s\n" ANSI_COLOR_RESET, get_line(commentless, r.error->state.row));
        int32 i = 0;
        printf(ANSI_COLOR_GREEN);
        while (i < r.error->state.col) { printf(" "); i++; }
        printf("^\n" ANSI_COLOR_RESET);
        mpc_err_delete(r.error);
    }

    mpc_cleanup(50, start, end, unaryop, ptrop, ident, number, character, string, attrtype, attrwval, attribute, val, emptyindices, indices, anyindices, module,
        import, define, natives, typeident, member, memblock, structure, locunion, globunion, decltype, functype, alias, arg, args, funcdecl, factor, term, lexp, vardecl, funccall,
        whileloop, dowhile, forloop, ifstmt, switchcase, switchstmt, stmt, exp, logic, block, function, head, body, c2);

    return NULL;
}
