#include "AST.h"

#define MAX_LVARS 100
#define MAX_GVARS 100

#define LOCAL_VAR 0 // not argument

extern int labelcnt;

typedef struct GlobalVariable {
    Symbol *var;
} GVar;

typedef struct LocalVariable {
    Symbol *var;
    int varType;
    int pos;
} LVar;

extern GVar gvars[];
extern int GVarp;
extern LVar lvars[];
extern int LVarp;

void pivotStatement(AST *stat);
void pivotBlock(AST *body, AST *localvars);
void pivotExpr(int target, AST *p);
void pivotIf(AST *condition, AST *body);

void initAssemble();
