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

int c2main(int32 argc, char** argv)
{
    if (argc > 1)
        c2parse(argv[1]);
    else
        recipemain(argc, argv);
     return 0;
}

mpc_ast_t* c2parse(char* filename)
{
    //general
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

    //TODO: Parse enums
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
        import, natives, typeident, member, memblock, structure, locunion, globunion, decltype, functype,
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
        //puts(currenttxt);
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
        puts(commentless);
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
        //mpc_print(c2);

    mpc_cleanup(49, start, end, unaryop, ptrop, ident, number, character, string, attrtype, attrwval, attribute, val, emptyindices, indices, anyindices, module,
        import, natives, typeident, member, memblock, structure, locunion, globunion, decltype, functype, alias, arg, args, funcdecl, factor, term, lexp, vardecl, funccall,
        whileloop, dowhile, forloop, ifstmt, switchcase, switchstmt, stmt, exp, logic, block, function, head, body, c2);

    return NULL;
}


int32 recipemain(int32 argc, char** argv)
{
    char* recipepath = findrecipe();
    if (argc > 0)
    {
        char* commentlessrecipe;
        char* recipetxt;
#ifdef R_OK
        if (access(recipepath, R_OK) == 0)
#else
        if (access(recipepath, _A_NORMAL) == 0)
#endif
        {
            current = fopen(recipepath, "rb");
            if (!current)
            {
                perror(recipepath);
                exit(1);
            }

            fseek(current, 0L, SEEK_END);
            lsize = ftell(current);
            rewind(current);

            recipetxt = calloc(1, lsize + 1);
            if (!recipetxt)
            {
                fclose(current);
                throw(&emafail, "Failed to alloc memory");
                exit(1);
            }

            if (fread(recipetxt, lsize, 1, current) != 1)
            {
                fclose(current);
                free(recipetxt);
                throw(&erdfail, "Failed to read file");
                exit(1);
            }
            fclose(current);

            char* temp = calloc(sizeof(char) * strlen(recipetxt), 1);
            for (int32 i = 0, k = 0; i < strlen(recipetxt) + 1; i++, k++)
            {
                if(recipetxt[i] == '#') while(recipetxt[i] != '\n') i++;
                commentless[k] = recipetxt[i];
            }
            commentlessrecipe = malloc(sizeof(char) * strlen(temp));
            strcpy(commentlessrecipe, temp);
            parserecipe(commentlessrecipe);
            free(recipetxt);
        }
        else
            throw(&enoaccs, "Cannot access recipe file");
    }
    else
        throw(&enoaccs, "Not enough command-line arguments");
    return 0;
}

/// <summary>
///  This function loops through parent directories until it finds the project root directory
///  where a cwd is set and recipe.txt file is located. Returns path to recipe.txt
/// </summary>
char* findrecipe()
{
    char buf[1024];
#ifdef _WIN32
    _getcwd(buf, 1024);
    while (strlen(buf) != 3)
#else
    getcwd(buf, 1024);
    while (strcmp(buf, "/") != 0)
#endif
    {
#ifdef _WIN32
        if (access("recipe.txt", _A_NORMAL) == 0)
#else
        if (access("recipe.txt", R_OK) == 0)
#endif
        {
            char* path = malloc(1024);
#ifdef _WIN32
            _getcwd(path, 1024);
#else
            getcwd(path, 1024);
#endif
            strcat(path, PATH_SEPARATOR "recipe.txt");
            return path;
        }
        chdir("../");
    }
    return 0;
}

void parserecipe(char* recipetext)
{
    state = START;
    char* nocarriage = str_replace(recipetext, "\r", " ");
    char** lines = strsplit((nocarriage ? nocarriage : recipetext), "\n");
    int32 lines_num = occurences(recipetext, '\n');
    recipe = malloc(sizeof(recipe_t));
    recipe->targets = malloc(sizeof(vector));
    recipe->count = 0;
    vector_init(recipe->targets);

    for (int32 i = 0; i <= lines_num; i++)
        handleline(lines[i]);

    processrecipe();
}

void handleline(char* line)
{
    if (issornull(line)) return;
    char** words = strsplit(line, " ");
    int32 words_num = occurences(line, ' ');
    if (!words) return;
    switch (state)
    {
        case START:
            if (words_num < 1)
                throw(&enotoks, "Not enough tokens");
            if (strcmp(words[0], "target") == 0)
            {
                currenttrg = malloc(sizeof(target));
                currenttrg->name = words[1];
                currenttrg->type = executable;
                currenttrg->files = malloc(sizeof(vector));
                vector_init(currenttrg->files);
                currenttrg->options = malloc(sizeof(vector));
                vector_init(currenttrg->options);
                state = INSIDE_TARGET;
            }
            else if (strcmp(words[0], "lib") == 0)
            {
                if (words_num < 2)
                    throw(&enotoks, "Not enough tokens");

                currenttrg = malloc(sizeof(target));
                currenttrg->name = words[1];
                if (strcmp(words[2], "static") == 0)
                {
                    currenttrg->type = libstatic;
                    currenttrg->files = malloc(sizeof(vector));
                    vector_init(currenttrg->files);
                    currenttrg->options = malloc(sizeof(vector));
                    vector_init(currenttrg->options);
                    state = INSIDE_TARGET;
                }
                else if (strcmp(words[2], "shared") == 0)
                {
                    currenttrg->type = libshared;
                    currenttrg->files = malloc(sizeof(vector));
                    vector_init(currenttrg->files);
                    currenttrg->options = malloc(sizeof(vector));
                    vector_init(currenttrg->options);
                    state = INSIDE_TARGET;
                }
                else
                    throw(&ebadtok, "Bad token in recipe");
            }
            else if(words[0][0] == '#')
                return;
            else
                throw(&ebadtok, "Bad token in recipe");
            break;
        case INSIDE_TARGET:
            if (strcmp(words[0], "end") == 0)
            {
                vector_add(recipe->targets, currenttrg);
                recipe->count++;
                state = START;
            }
            else if (words[0][0] == '$')
            {
                option* opt = malloc(sizeof(option));
                opt->name = malloc(strlen(words[0]) + 1);
                opt->name = words[0];
                opt->opts = malloc(sizeof(vector));
                vector_init(opt->opts);

                for (int32 i = 1; i < words_num; i++)
                    vector_add(opt->opts, words[i]);

                vector_add(currenttrg->options, opt);
            }
            else if (words_num >= 1)
                vector_add(currenttrg->files, words[0]);
            else
                throw(&ebadtok, "Unexpected token in recipe");
            break;
    }
}
