/* parser.y */
%token SYMBOL

%{
#include "AST.h"
%}

%union {
    AST *val;
}

%type <val> block
%type <val> SYMBOL

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
    { defineFunction(getSymbol($1), NULL); }
	;

block:
    '{' '}'
    { $$ = makeAST(blockSt, NULL, NULL); }
	;

%%

#include "lexer.c"
