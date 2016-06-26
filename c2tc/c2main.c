#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <io.h>

#include "mpc.h"
#include "inttypes.h"
#include "shared.h"

int32 c2main(int32 argc, char** argv)
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
	mpc_parser_t* stmt = mpc_new("stmt");
	mpc_parser_t* exp = mpc_new("exp");
	mpc_parser_t* logic = mpc_new("logic");
	mpc_parser_t* block = mpc_new("block");
	mpc_parser_t* function = mpc_new("function");

	mpc_parser_t* body = mpc_new("body");
	mpc_parser_t* c2 = mpc_new("c2");

	mpc_err_t* err = mpca_lang(MPCA_LANG_DEFAULT,
		//general
		" start       : /^/ ;                                                                \n"
		" end         : /$/ ;                                                                \n"
		" unaryop     : '&' | '!' | '-' | '~' | '+' ;                                        \n"
		" ptrop       : '*' ;                                                                \n"
		" ident       : /[a-zA-Z_\\.\\#][a-zA-Z0-9_-]*/ ;                                    \n"
		" number      : /[0-9]+/ ;                                                           \n"
		" character   : /'.'/ ;                                                              \n"
		" string      : /\"\"(\\\\.|[^\"])*\"/ ;                                             \n"
		" attrtype    :(\"export\"    | \"packed\"                                           \n"
		"             |  \"unused\"   | \"unused_params\"                                    \n"
		"             |  \"noreturn\" | \"inline\"                                           \n"
		"             |  \"weak\"     | \"opaque\") ;                                        \n"
		" attrwval    : (\"aligned\" | \"section\") '=' (<string>|<number>) ;                \n"
		" attribute   : \"@(\" (<attrwal>|<attrtype>) (',' (<attrwval>                       \n"
		"             |<attrtype>))* ')' ;                                                   \n"
		" val         : /[0-9]+(\\.[0-9]+)*[a-zA-Z_]*/ ;                                     \n"
		" emptyindices: \"[]\";                                                              \n"
		" indices     : '[' (<ident> | <number> | '+' ) ']' ;                                \n"
		" anyindices  : (<indices> | <emptyindices>) ;                                       \n"
		" module      : \"module\" <ident> ';' ;                                             \n"
		" import      : \"import\" <ident> (';'                                              \n"
		"             | (\"local\" ';')                                                      \n"
		"             | (\"as\" <ident> ';')                                                 \n"
		"             | (\"as\" <ident> \"local\" ';')) ;                                    \n"
		" define      : \"#define\" <ident> /((\\w*)\\s)/ ;                                  \n"
		" natives     : \"char\"                                                             \n"
		"             | \"int8\"    | \"uint8\"                                              \n"
		"             | \"int16\"   | \"uint16\"                                             \n"
		"             | \"int32\"   | \"uint32\"                                             \n"
		"             | \"int64\"   | \"uint64\"                                             \n"
		"             | \"float32\" | \"float64\" ;                                          \n"
		" typeident   : <natives> | <ident> ;                                                \n"
		" member      : <typeident> <ident> ';' ;                                            \n"
		" memblock    : '{' (<member> | <locunion>)* '}' ;                                   \n"
		" structure   : \"type\" <ident> \"struct\" <memblock> <attribute>? ;                \n"
		" locunion    : \"union\" (<ident> <memblock> | <memblock>) ;                        \n"
		" globunion   : \"type\" <ident> \"union\" <memblock> ;                              \n"
		" decltype    : <typeident> <ptrop>* <indices>* ;                                    \n"
		" alias       : \"type\" <ident> <decltype> ';' ;                                    \n"
		" arg         : <decltype> <ident>;                                                  \n"
		" args        : '(' (<arg> ',')* <arg>? ')' ;                                        \n"
		" vardecl     : <decltype> <ident> ('=' <lexp>)? ';' ;                               \n"
		" funcdecl    : (\"public\")? \"func\" <decltype> <ident> <args>;                    \n"
		" whileloop   : \"while\" '(' <exp> ')' <stmt> ;                                     \n"
		" dowhile     : \"do\" <stmt> \"while\" '(' <exp> ')' ';' ;	                         \n"
		" forloop     : \"for\" '('(<vardecl>|';') (<logic>';'|';') <factor>? ')' <stmt>;    \n"
		" factor      : '(' <lexp> ')'                                                       \n"
		"             | <number>                                                             \n"
		"             | <character>                                                          \n"
		"             | <string>                                                             \n"
		"             | <ident> (\"++\"|\"--\")                                              \n"
		"             | (\"++\"|\"--\") <ident>                                              \n"
		"             | '~' <term>                                                           \n"
		"             | '&' <ident>                                                          \n"
		"             | \"(->\"<typedecl>')' <ident>                                         \n"
		"             | <ident> '(' <lexp>? (',' <lexp>)* ')'                                \n"
		"             | <ident> ;                                                            \n"
		" term        : <factor> (('*' | '/' | '%') <factor>)* ;                             \n"
		" lexp        : <term> (('+' | '-') <term>)* ;                                       \n"
		" funccall    : <ident> '(' <ident>? (',' <ident>)* ')' ';' ;                        \n"
		" stmt        : '{' <stmt>* '}'                                                      \n"
		"             | <vardecl>                                                            \n"
		"             | <whileloop>                                                          \n"
		"             | <dowhile>                                                            \n"
		"             | <forloop>                                                            \n"
		"             | \"if\"    '(' <exp> ')' <stmt>                                       \n"
		"             | <ident> '=' <lexp> ';'                                               \n"
		"             | \"return\" <lexp>? ';'                                               \n"
		"             | <funccall> ;                                                         \n"
		" exp         : <lexp> '>' <lexp>                                                    \n"
		"             | <lexp> '<' <lexp>                                                    \n"
		"             | '!'<lexp>                                                            \n"
		"             | <lexp> \"<<\" <lexp>                                                 \n"
		"             | <lexp> \">>\" <lexp>                                                 \n"
		"             | <lexp> \">=\" <lexp>                                                 \n"
		"             | <lexp> \"<=\" <lexp>                                                 \n"
		"             | <lexp> \"!=\" <lexp>                                                 \n"
		"             | <lexp> \"==\" <lexp> ;                                               \n"
		" logic       : <exp> ((\"&&\" | \"||\") <exp>)* ;                                   \n"
		" block       : '{' <stmt>* '}' ;                                                    \n"
		" function    : <funcdecl> <block> ;                                                 \n"
		" head        : <module> <import>* ;                                                 \n"
		" body        : (<define> | <structure>                                              \n"
		"             | <globunion> | <alias>                                                \n"
		"             | <function> | <vardecl>)*;                                            \n"
		" c2          : <start> <head> <body> <end> ;                                        \n",
		start, end, unaryop, ptrop, ident, number, string, attrtype, attrwval, attribute, val, emptyindices, indices, anyindices, module,
		import, define, natives, typeident, member, memblock, structure, locunion, globunion, decltype,
		alias, arg, args, funcdecl, factor, term, lexp, vardecl, funccall, stmt, funccall, whileloop, dowhile, forloop, exp, logic, block, function, head, body, c2, NULL
		);
	mpc_optimise(c2);
	if (err != NULL)
	{
		mpc_err_print(err);
		mpc_err_delete(err);
		exit(1);
	}

	if (argc > 1)
	{
#ifdef R_OK
		if (access(argv[1], R_OK) == 0)
#else
		if (access(argv[1], _A_NORMAL) == 0)
#endif
		{
			current = fopen(argv[1], "rb");
			if (!current)
			{
				perror(argv[1]);
				exit(1);
			}

			fseek(current, 0L, SEEK_END);
			lsize = ftell(current);
			rewind(current);

			currenttxt = calloc(1, lsize + 1);
			if (!currenttxt)
			{
				fclose(current);
				fputs("memory alloc fails", stderr);
				exit(1);
			}

			if (fread(currenttxt, lsize, 1, current) != 1)
			{
				fclose(current);
				free(currenttxt);
				fputs("entire read fails", stderr);
				exit(1);
			}
			fclose(current);

			//comment skip
			/*puts(currenttxt);
			puts("\n======================\n");*/
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
			puts(commentless);
			free(currenttxt);
		}

		mpc_result_t r;
		if (mpc_parse(argv[1], commentless, c2, &r))
		{
			mpc_ast_print(r.output);
			mpc_ast_delete(r.output);
		}
		else
		{
			mpc_err_print(r.error);
			mpc_err_delete(r.error);
		}
		//mpc_print(c2);

	}
	else
	{
		mpc_result_t r;
		if (mpc_parse_pipe("<stdin>", stdin, c2, &r))
		{
			mpc_ast_print(r.output);
			mpc_ast_delete(r.output);
		}
		else
		{
			mpc_err_print(r.error);
			mpc_err_delete(r.error);
		}
	}

	mpc_cleanup(45, start, end, unaryop, ptrop, ident, number, string, attrtype, attrwval, attribute, val, emptyindices, indices, anyindices, module,
	import, define, natives, typeident, member, memblock, structure, locunion, globunion, decltype, alias, arg, args, funcdecl, factor, term, lexp, vardecl, funccall,
	whileloop, dowhile, forloop, stmt, exp, logic, block, function, head, body, c2);

	return 0;
}