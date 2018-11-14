/* AST.c */
#include "AST.h"

Symbol symTable[MAX_SYMBOLS];
int symcnt = 0;

void drawSymTable() {
    for(int i=0; i<symcnt; i++) printf("[symbol(%d)] %s\n", i, symTable[i].name);
}

AST *makeAST(enum ASTtype type, AST *left, AST *right) {
    AST *p;
    p = (AST *)malloc(sizeof(AST));
    p->type = type;
    p->right = right;
    p->left = left;
    return p;
}

AST *addList(AST *ast, AST *p) {
    if(ast->type != listAST) {
        fprintf(stderr, "bad list accessing\n");
        exit(1);
    }
    AST *ret = ast;

    // retの一番下へ
    while(ret->right != NULL) ret = ret->right;
    ret->right = makeAST(listAST, p, NULL);

    return ast;
}

AST *getList(AST *ast, int num) {
    if(ast->type != listAST) {
        fprintf(stderr, "bad list accessing\n");
        exit(1);
    }
    if(num == 0) return ast->left;
    else return getList(ast->right, num-1);
}

AST *getNext(AST *ast) {
    if(ast->type != listAST) {
        fprintf(stderr, "bad list acccessing\n");
        exit(1);
    }
    return ast->right;
}

AST *makeSymAST(char *name) {
    AST *p;

    p = (AST *)malloc(sizeof(AST));
    p->type = symOp;
    p->sym = searchSymbol(name);
    return p;
}

AST *makeNumAST(int value) {
    AST *p;

    p = (AST *)malloc(sizeof(AST));
    p->type = numOp;
    p->val = value;
    return p;
}

Symbol *searchSymbol(char *name) {
    Symbol *sp = NULL;

    for(int i = 0; i < symcnt; i++){
	    if(strcmp(symTable[i].name, name) == 0){
	        sp = &symTable[i];
	        break;
	    }
    }
    if(sp == NULL){
	    sp = &symTable[symcnt++];
	    sp->name = strdup(name);
    }
    return sp;
}

Symbol *getSymbol(AST *ast) {
    if(ast->type != symOp){
	    fprintf(stderr,"bad accessing\n");
    	exit(1);
    }
    return ast->sym;
}
