#include "mpc.h"
#include "inttypes.h"

int32 c2mainbackup(int32 argc, char** argv)
{
	//general
	mpc_parser_t* op = mpc_new("op");
	mpc_parser_t* ptrop = mpc_new("ptrop");
	mpc_parser_t* ident = mpc_new("ident");
	mpc_parser_t* number = mpc_new("number");
	mpc_parser_t* character = mpc_new("character");
	mpc_parser_t* string = mpc_new("string");
	mpc_parser_t* val = mpc_new("val");
	mpc_parser_t* emptyindices = mpc_new("emptyindices");
	mpc_parser_t* indices = mpc_new("indices");

	//words, idents
	mpc_parser_t* numkw = mpc_new("numkw");
	mpc_parser_t* textkw = mpc_new("textkw");
	mpc_parser_t* type = mpc_new("type");
	mpc_parser_t* ptr = mpc_new("ptr");
	mpc_parser_t* typeident = mpc_new("typeident");
	mpc_parser_t* args = mpc_new("args");
	mpc_parser_t* memblock = mpc_new("memblock");
	mpc_parser_t* valblock = mpc_new("valblock");

	//comments, labels
	mpc_parser_t* comment = mpc_new("comment");
	mpc_parser_t* oneliner = mpc_new("oneliner");
	mpc_parser_t* label = mpc_new("label");

	//module notation
	mpc_parser_t* module = mpc_new("module");

	//import stmt
	mpc_parser_t* import = mpc_new("import");

	//expressions
	mpc_parser_t* factor = mpc_new("factor");
	mpc_parser_t* term = mpc_new("term");
	mpc_parser_t* lexp = mpc_new("lexp");
	mpc_parser_t* exp = mpc_new("exp");
	mpc_parser_t* stmt = mpc_new("stmt");

	//vars, types, unions
	mpc_parser_t* single = mpc_new("single");
	mpc_parser_t* arr = mpc_new("arr");
	mpc_parser_t* datatype = mpc_new("datatype");
	mpc_parser_t* uniontype = mpc_new("uniontype");
	mpc_parser_t* attr = mpc_new("attr");
	mpc_parser_t* var = mpc_new("var");
	mpc_parser_t* incrarr = mpc_new("incrarr");
	mpc_parser_t* member = mpc_new("member");

	//preprocessor
	mpc_parser_t* define = mpc_new("define");

	//function type
	mpc_parser_t* fptr = mpc_new("fptr");

	//compound
	mpc_parser_t* body = mpc_new("body");


	mpc_parser_t* c2 = mpc_new("c2");

	mpc_err_t* err = mpca_lang(MPCA_LANG_DEFAULT,
		//general
		" op          : '+' | '>' | '<' ;                                              \n"
		" ptrop       : '*' ;                                                          \n"
		" ident       : /[a-zA-Z_\\.\\#][a-zA-Z0-9_-]*/ ;                              \n"
		" number      : /[0-9]+/ ;                                                     \n"
		" character   : /'.'/ ;                                                        \n"
		" string      : /\"\"(\\\\.|[^\"])*\"/ ;                                       \n"
		" val         : /[0-9]+(\\.[0-9]+)*[a-zA-Z_]*/ ;                               \n"
		" emptyindices: \"[]\";                                                        \n"
		" indices     : '[' (<ident> | <number> | '+' ) ']' ;                          \n"

		//words, idents
		" numkw       : \"int8\"    | \"uint8\"                                        \n"
		"             | \"int16\"   | \"uint16\"                                       \n"
		"             | \"int32\"   | \"uint32\"                                       \n"
		"             | \"int64\"   | \"uint64\"                                       \n"
		"             | \"float32\" | \"float64\" ;                                    \n"
		" textkw      : \"char\" ;                                                     \n"
		" type        : <numkw> | <textkw> | <ident> ;                                 \n"
		" ptr         : <type> <ptrop>* ;                                              \n"
		" typeident   : <type> | <ptr> ;                                               \n"
		" args        : (<typeident> <ident>)? (',' <typeident> <ident>)*;             \n"
		" memblock    : '{' <member>* '}' ;                                            \n"
		" valblock    : '{' ((<number> | <character> | <val> | <ident>                 \n"
		"             | <valblock>) (',' <number> | <character> | <val> | <ident>      \n"
		"             | <valblock>)*  '}' ;                                            \n"

		//comments, labels
		" comment     : /\\/\\*((\\w*)\\s)*\\*\\//;                                    \n"
		" oneliner    : /\\/\\/\\w*\\s/;                                               \n"
		" label       : /\\s/ <ident> ':' ;                                            \n"

		//module notation
		" module      : \"module\" <ident> ';' ;                                       \n"

		//import statement
		" import      : \"import\" <ident> (((\"local\" ';')                           \n"
		"             | ( (\"as\" <ident> ';')                                         \n"
		"             | (\"as\" <ident> \"local\" ';') ) | ';') ;                      \n"

		//expressions
		" factor      : '(' <lexp> ')'                                                   \n"
		"             | <number>                                                         \n"
		"             | <character>                                                      \n"
		"             | <string>                                                         \n"
		"             | <ident> '(' <lexp>? (',' <lexp>)* ')'                            \n"
		"             | <ident> ;                                                        \n"
		" term        : <factor> (('*' | '/' | '%') <factor>)* ;                         \n"
		" lexp        : <term> (('+' | '-') <term>)* ;                                   \n"
		" exp         : <lexp> '>' <lexp>                                                \n"
		"             | <lexp> '<' <lexp>                                                \n"
		"             | <lexp> \">=\" <lexp>                                             \n"
		"             | <lexp> \"<=\" <lexp>                                             \n"
		"             | <lexp> \"!=\" <lexp>                                             \n"
		"             | <lexp> \"==\" <lexp> ;                                           \n"
		" stmt        : '{' <stmt>* '}'                                                  \n"
		"             | \"while\" '(' <exp> ')' <stmt>                                   \n"
		"             | \"if\"    '(' <exp> ')' <stmt>                                   \n"
		"             | <ident> '=' <lexp> ';'                                           \n"
		"             | \"return\" <lexp>? ';'                                           \n"
		"             | <ident> '(' <ident>? (',' <ident>)* ')' ';' ;                    \n"

		//vars, types, unions
		" single      : <typeident> <ident> (';'                                         \n"
		"             | '=' <term>) ;                                                    \n"
		" arr         : <typeident> (<emptyindices>? | <indices>?) <ident> (';'          \n"
		"             | '=' <ident> ';'                                                  \n"
		"             | '=' <valblock> ;                                                 \n"


		" body        : '{' <stmt>+ '}' ;                                                \n"
		" c2          : /^/ <module> <import>* /$/ ;                                     \n",
		op, ptrop, ident, number, string, val, emptyindices, indices, numkw, textkw, type, ptr,
		typeident, args, memblock, valblock, comment, oneliner, label, module, import, factor,
		term, lexp, exp, stmt, single, arr, type, uniontype, attr, var, incrarr, member, define,
		fptr, body, c2, NULL
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
		mpc_result_t r;
		if (mpc_parse_contents(argv[1], c2, &r))
		{
			mpc_ast_print(r.output);
			mpc_ast_delete(r.output);
		}
		else
		{
			mpc_err_print(r.error);
			mpc_err_delete(r.error);
		}
		mpc_print(c2);

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

	mpc_cleanup(38, op, ptrop, ident, number, string, val, emptyindices, indices, numkw, textkw, type, ptr,
		typeident, args, memblock, valblock, comment, oneliner, label, module, import, factor,
		term, lexp, exp, stmt, single, arr, type, uniontype, attr, var, incrarr, member, define,
		fptr, body, c2);

	return 0;
}