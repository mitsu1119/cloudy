/* AST.h */
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

enum ASTtype {
    symOp, blockSt, retSt
};

typedef struct _AST {
    enum ASTtype type;
    struct _AST *right, *left;
    struct _Symbol *sym;
} AST;

typedef struct _Symbol {
    char *name;
    AST *funcBody;
} Symbol;

#define MAX_SYMBOLS 100

extern Symbol symTable[];
extern int symcnt;

void drawSymTable();

/* AST.c */
AST *makeAST(enum ASTtype type, AST *left, AST *right);
AST *makeSymAST(char *name);
Symbol *searchSymbol(char *name);
Symbol *getSymbol(AST *ast);

/* compile.c */
void defineFunction(Symbol *sym, AST *body);
