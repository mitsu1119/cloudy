#include "AST.h"
#include "compile.h"
#include "pivotCode.h"

int LVarp;
LVar lvars[MAX_LVARS];

int tmpCnt = 0;

void defineFunction(Symbol *sym, AST *body) {
    asmIni();
    pivotStatement(body);
    funcAsm(sym->name);
}

void pivotStatement(AST *stat) {
    if(stat == NULL) return;
    switch(stat->type) {
    case blockSt:
        pivotBlock(stat->left);
        break;
    default:
        pivotExpr(-1, stat);
    };
}

void pivotBlock(AST *body) {
    while(body != NULL) {
        pivotStatement(getList(body,0));
        getNext(body);
    }
}

void pivotExpr(int target, AST *p) {
    int r1, r2; // buf
    if(p == NULL) return;

    switch(p->type) {
    case numOp:
        genCode2(LOADI, target, p->val);
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
    default:
        fprintf(stderr, "wrong expr\n");
        exit(1);
    }
}
