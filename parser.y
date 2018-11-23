/* parser.y */
%token SYMBOL NUMBER STRING
%token INT
%token RETURN

%left '+' '-'
%left '*' '/'

%{
#include "AST.h"

//#define __PARSER_DEBUG_MODE
%}

%union {
    AST *val;
}

%type <val> lvars symbol_list block
%type <val> SYMBOL NUMBER STRING
%type <val> RETURN
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
	{ declareVar(getSymbol($2), NULL, TRUE);
	#ifdef __PARSER_DEBUG_MODE
	printf("[*] declared variable <%s>\n", getSymbol($2)->name);
	#endif
	}
	| INT SYMBOL '=' NUMBER ';'
	{ declareVar(getSymbol($2), $4, FALSE);
	#ifdef __PARSER_DEBUG_MODE
	printf("[*] declared data variable <%s,%d>\n", getSymbol($2)->name, $4->val);
	#endif
	}
	;

block:
    '{' lvars statements '}'
    { $$ = makeAST(blockSt, $3, $2); }
	;

lvars:
	/* NULL */
	{ $$ = NULL; }
	| INT symbol_list ';'
	{ $$ = $2; }
	;

symbol_list:
	SYMBOL
	{ $$ = makeAST(listAST, $1, NULL); }
	| symbol_list ',' SYMBOL
	{ $$ = addList($1,$3); }
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
	| RETURN ';'
	{ $$ = makeAST(returnSt, NULL, NULL); }
	;

exp:
	prim_exp
	| SYMBOL '=' exp
	{ $$ = makeAST(eqOp, $1, $3); }
	| exp '+' exp
	{ $$ = makeAST(addOp, $1, $3); }
	| exp '-' exp
	{ $$ = makeAST(subOp, $1, $3); }
	| exp '*' exp
	{ $$ = makeAST(mulOp, $1, $3); }
	| exp '/' exp
	{ $$ = makeAST(divOp, $1, $3); }
	;

prim_exp:
	SYMBOL
	| NUMBER
	| STRING
	{ printf("string %s\n", $1->str); }
	| SYMBOL '(' ')'
	{ $$ = makeAST(callOp, $1, NULL); }
	;

%%

#include "lexer.c"
