#include "AST.h"
#define MAX_LVARS 100

typedef struct LocalVariable {
    Symbol *var;
    int varType;
    int pos;
} LVar;

void pivotStatement(AST *stat);
void pivotBlock(AST *body);
void pivotExpr(int target, AST *p);

void initAssemble();
