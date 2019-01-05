#include "AST.h"
#include "compile.h"
#include "pivotCode.h"

GVar gvars[MAX_GVARS];
int GVarp = 0;

LVar lvars[MAX_LVARS];
int LVarp = 0;
int LVarpFunc;

int tmpCnt = 0;

void defineFunction(Symbol *sym, AST *body) {
    asmIni();

    LVarp = 0;
    LVarpFunc = 0;

    pivotStatement(body);
    funcAsm(sym->name, LVarpFunc);

    LVarp = 0;
}

void declareVar(Symbol *sym, AST *value, int isBss) {
    gvars[GVarp].var = sym;
    gvars[GVarp].var->isBss = isBss;

    // token NUMBER
    if(isBss == FALSE) gvars[GVarp++].var->val = value->val;
    else GVarp++;
}

void pivotStoreVar(Symbol *var, int r) {
    int i;

    for(i = 0; i<LVarp; i++) {
        if(lvars[i].var == var) {
            switch(lvars[i].varType) {
            case LOCAL_VAR:
                genCode2(STOREL, r, lvars[i].pos);
                return;
            }
        }
    }
    fprintf(stderr, "undefinded variable\n");
}

void pivotLoadvar(int target, Symbol *var) {
    int i;
    for(i=0; i<LVarp; i++) {
        if(lvars[i].var == var) {
            switch(lvars[i].varType) {
            case LOCAL_VAR:
                genCode2(LOADL, target, lvars[i].pos);
                return;
            }
        }
    }
    fprintf(stderr, "wrong loadvar");
    exit(1);
}

void pivotStatement(AST *stat) {
    if(stat == NULL) return;
    switch(stat->type) {
    case ifSt:
        pivotIf(stat->left, stat->right);
        break;
    case blockSt:
        pivotBlock(stat->left, stat->right);
        break;
    case returnSt:
        pivotReturn(stat->left);
        break;
    default:
        pivotExpr(-1, stat);
    };
}

void pivotBlock(AST *body, AST *localvars) {
    int LVarpStart = LVarp;

    // ローカル変数の処理
    while(localvars != NULL) {
        lvars[LVarp].var = getSymbol(getList(localvars,0));
        lvars[LVarp].varType = LOCAL_VAR;
        // printf("[*] declare localvar: %s\n", lvars[LVarp].var->name);
        lvars[LVarp++].pos = LVarpFunc++;
        localvars = getNext(localvars);
    }

    while(body != NULL) {
        pivotStatement(getList(body,0));
        body = getNext(body);
    }
    LVarp = LVarpStart;
}

void pivotReturn(AST *retValue) {
    int reg;
    if(retValue == NULL) reg = -1;
    else {
        reg = tmpCnt++;
        pivotExpr(reg, retValue);
    }
    genCode1(RET, reg);
}

void pivotCall(int target, Symbol *sym) {
    genCodeS(CALL, target, 0, sym->name);
}

void pivotIf(AST *condition, AST *body) {
    int reg = tmpCnt++;
    int label1;

    pivotExpr(reg, condition);
    label1 = labelcnt++;
    genCode2(BEQ0, reg, label1);
    pivotStatement(body);
    genCode1(LABEL, label1);
}

void pivotExpr(int target, AST *p) {
    int r1, r2; // buf
    if(p == NULL) return;

    switch(p->type) {
    case numOp:
        genCode2(LOADI, target, p->val);
        return;
    case symOp:
        pivotLoadvar(target, getSymbol(p));
        return;
    case addOp:
        r1 = tmpCnt++;
        r2 = tmpCnt++;
        pivotExpr(r1, p->left);
        pivotExpr(r2, p->right);
        genCode3(ADD, target, r1, r2);
        return;
    case subOp:
        r1 = tmpCnt++;
        r2 = tmpCnt++;
        pivotExpr(r1, p->left);
        pivotExpr(r2, p->right);
        genCode3(SUB, target, r1, r2);
        return;
    case mulOp:
        r1 = tmpCnt++;
        r2 = tmpCnt++;
        pivotExpr(r1, p->left);
        pivotExpr(r2, p->right);
        genCode3(MUL, target, r1, r2);
        return;
    case divOp:
        r1 = tmpCnt++;
        r2 = tmpCnt++;
        pivotExpr(r1, p->left);
        pivotExpr(r2, p->right);
        genCode3(DIV, target, r1, r2);
        return;
    case lessThanOp:
        r1 = tmpCnt++;
        r2 = tmpCnt++;
        pivotExpr(r1, p->left);
        pivotExpr(r2, p->right);
        genCode3(LT, target, r1, r2);
        return;
    case greaterThanOp:
        r1 = tmpCnt++;
        r2 = tmpCnt++;
        pivotExpr(r1, p->left);
        pivotExpr(r2, p->right);
        genCode3(GT, target, r1, r2);
        return;
    case eqOp:
        if(target != -1) fprintf(stderr, "assign has no value");
        r1 = tmpCnt++;
        pivotExpr(r1, p->right);
        pivotStoreVar(getSymbol(p->left), r1);
        return;
    case callOp:
        pivotCall(target, getSymbol(p->left));
        return;
    default:
        fprintf(stderr, "wrong expr\n");
        exit(1);
    }
}
