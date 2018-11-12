/* parser.y */
%token SYMBOL NUMBER
%token INT

%{
#include "AST.h"

#define __PARSER_DEBUG_MODE
%}

%union {
    AST *val;
}

%type <val> block
%type <val> SYMBOL NUMBER
%type <val> statements statement exp prim_exp

%start program

%%

program: /* empty */
	| external_definitions
	;

external_definitions:
	external_definition
	| external_definitions external_definition
	;

external_definition:
	SYMBOL '(' ')' block
    { defineFunction(getSymbol($1), $4);
	#ifdef __PARSER_DEBUG_MODE
	printf("[*] defined function <%s>\n",getSymbol($1)->name);
	#endif
	}
	| INT SYMBOL ';'
	{
		declareVar(getSymbol($2), NULL, TRUE);
		#ifdef __PARSER_DEBUG_MODE
		printf("[*] declared variable <%s>\n", getSymbol($2)->name);
		#endif
	}
	| INT SYMBOL '=' NUMBER ';'
	{
		declareVar(getSymbol($2), $4, FALSE);
		#ifdef __PARSER_DEBUG_MODE
		printf("[*] declared data variable <%s,%d>\n", getSymbol($2)->name, $4->val);
		#endif
	}
	;

block:
    '{' statements '}'
    { $$ = makeAST(blockSt, $2, NULL); }
	;

statements:
	statement
	{ $$ = makeAST(listAST, $1, NULL); }
	| statements statement
	{ $$ = addList($1, $2); }
	;

statement:
	exp ';'
	{ $$ = $1; }
	| block
	{ $$ = $1; }
	;

exp:
	prim_exp
	| SYMBOL '=' exp
	{ $$ = makeAST(eqOp, $1, $3); }
	;

prim_exp:
	SYMBOL
	| NUMBER
	;

%%

#include "lexer.c"
