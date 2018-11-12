#include "AST.h"

#define MAX_LVARS 100
#define MAX_GVARS 100

#define LOCAL_VAR 0 // not argument

typedef struct GlobalVariable {
    Symbol *var;
} GVar;

extern GVar gvars[];
extern int GVarp;

typedef struct LocalVariable {
    Symbol *var;
    int varType;
    int pos;
} LVar;

void pivotStatement(AST *stat);
void pivotBlock(AST *body);
void pivotExpr(int target, AST *p);

void initAssemble();

