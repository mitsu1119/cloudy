#include "pivotCode.h"
#include <stdio.h>
#include <string.h>

struct pivot {
    char *name;
    int num;
} pivots[] = {
    { "LOADI", LOADI},
    { "LOADA", LOADA},
    { "LOADL", LOADL},
    { "STOREA", STOREA},
    { "STOREL", STOREL},
    { "ADD", ADD},
    { "SUB", SUB},
    { "MUL", MUL},
    { "DIV", DIV},
    { "GT", GT},
    { "LT", LT},
    { "BEQ0", BEQ0},
    { "JUMP", JUMP},
    { "ARG", ARG},
    { "CALL", CALL},
    { "RET", RET},
    { "PRINTLN", PRINTLN},
    { "LABEL", LABEL},
    { "LOADS", LOADS},
    { "LOADADDR", LOADADDR},
    { "LOAD", LOAD},
    { "STORE", STORE},
    { 0, 0}
};

char *getPivotName(int codenum) {
    int i;
    for(i=0; pivots[i].name != 0; i++)
        if(pivots[i].num == codenum)
            return pivots[i].name;

    fprintf(stderr, "unknown pivot codenum = %d\n", codenum);
}

int getPivotNum(char *codename) {
    int i;
    for(i=0; pivots[i].name != 0; i++)
        if(!strcmp(pivots[i].name, codename))
            return pivots[i].num;
    fprintf(stderr, "unknown pivot name = %s\n", codename);
}
