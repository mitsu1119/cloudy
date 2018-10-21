/* parser.y */
%token SYMBOL

%{
#include "AST.h"
%}

%union {
    AST *val;
}

// %type <val> block
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
    { printf("extdef\n"); }
	;

block:
    '{' '}'
    { printf("block\n"); }
	;

%%

#include "lexer.c"
