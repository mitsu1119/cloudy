/* AST.h */
#pragma once
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define FALSE 0
#define TRUE 1

// eqOp, addOp, subOp, mulOp, divOp, lessThanOp, greaterThanOp => [L] exp  [R] exp
// callOp => [L] symbol
// symOp => [L] symbol
// numOp => [L] value
// blockSt => [L] bodyStatements  [R] localvars
// returnSt => [L] retvalue
// listAST => [L] factor  [R] nextList
enum ASTtype {
    eqOp, addOp, subOp, mulOp, divOp, lessThanOp, greaterThanOp, eqeqOp, callOp, symOp, numOp, strOp, ifSt, whileSt, blockSt, returnSt, listAST
};

typedef struct _AST {
    enum ASTtype type;
    int val;
    char *str;
    struct _AST *right, *left;
    struct _Symbol *sym;
} AST;

typedef struct _Symbol {
    char *name;
    int val;
    int isBss;
    AST *funcBody;
} Symbol;

#define MAX_SYMBOLS 100

extern Symbol symTable[];
extern int symcnt;

void drawSymTable();

/* AST.c */
AST *makeAST(enum ASTtype type, AST *left, AST *right);
AST *makeSymAST(char *name);
AST *makeNumAST(int value);
AST *makeStrAST(char *str);
AST *addList(AST *ast, AST *p);
AST *getList(AST *ast, int num);
AST *getNext(AST *ast);
Symbol *searchSymbol(char *name);
Symbol *getSymbol(AST *ast);

/* compile.c */
void defineFunction(Symbol *sym, AST *body);
void declareVar(Symbol *sym, AST *value, int isBss);
