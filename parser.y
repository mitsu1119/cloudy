/* parser.y */
%token SYMBOL NUMBER

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
