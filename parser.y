/* parser.y */
%token SYMBOL

%{
#include "AST.h"

#define __DEBUG_MODE
%}

%union {
    AST *val;
}

%type <val> block
%type <val> SYMBOL
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
    { defineFunction(getSymbol($1), NULL);
	#ifdef __DEBUG_MODE
	printf("[*] define function <%s>\n",getSymbol($1)->name);
	#endif
	}
	;

block:
    '{' '}'
    { $$ = makeAST(blockSt, NULL, NULL); }
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
	;

exp:
	prim_exp
	;

prim_exp:
	SYMBOL
	;

%%

#include "lexer.c"
