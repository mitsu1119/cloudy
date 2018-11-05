#include <stdio.h>
#include "compile.h"

#define MAX_CODES 100

struct _Code {
    int opcode, operand1, operand2, operand3;
} codes[MAX_CODES];
int codecnt;

// アセンブラ起動時の初期化関数
void initAssemble() {
    puts("global main");
}

// 関数のアセンブルの初期化関数
void asmIni() {
    codecnt = 0;
}

void genCode1(int opcode, int operand1) {
    codes[codecnt].opcode = opcode;
    codes[codecnt++].operand1 = operand1;
}

void genCode2(int opcode, int operand1, int operand2) {
    codes[codecnt].opcode = opcode;
    codes[codecnt].operand1 = operand1;
    codes[codecnt++].operand2 = operand2;
}

void genCode3(int opcode, int operand1, int operand2, int operand3) {
    codes[codecnt].opcode = opcode;
    codes[codecnt].operand1 = operand1;
    codes[codecnt].operand2 = operand2;
    codes[codecnt++].operand3 = operand3;
}

/* *** Assemble *** */

#define N_REG 4
#define N_SAVE 4

#define EAX 0
#define EBX 1
#define ECX 2
#define EDX 3

char *tmpRegName[N_REG] = {"eax", "ebx", "ecx", "edx"};
char *regAddressPrefix[4] = {"byte", "word", "unDefined", "dword"};

// 実際のレジスタにどの仮想レジスタが割り当てられているか(なし:-1)
int tmpRegState[N_REG];

// 退避領域にどの仮想レジスタの値が対比されているか
int tmpRegSave[N_SAVE];

void funcAsm(char *name) {
    int i;

    puts(".text");                                      /* .text */
    printf("%s:\n", name);                              /* name: */

    puts("push\tebp");      // push ebp
    puts("mov\tebp, esp");  // mov ebp, esp
    puts("sub\tesp, 4");    // sub esp, 4
}

void initTmpReg() {
    int i;
    for(i=0; i<N_REG; i++) tmpRegState[i] = -1;
    for(i=0; i<N_SAVE; i++) tmpRegSave[i] = -1;
}

int getFreeReg(int rs) {
    int i;
    for(i=0; i<N_REG; i++) {
        if(tmpRegState[i] < 0) {
            tmpRegState[i] = rs;
            return i;
        }
    }
    fprintf(stderr, "Free temp register is not found\n");
}

void assignReg(int rs, int reg) {
    if(tmpRegState[reg] = rs) return;
    saveReg(reg);
    tmpRegState[reg] = rs;
}

void freeReg(int reg) {
    tmpRegState[reg] = -1;
}

saveReg(int reg) {
    //レジスタの退避
    // あとで書く
}
