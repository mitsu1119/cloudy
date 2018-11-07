#include <stdio.h>
#include "compile.h"

#define __ASM_DEBUG_MODE

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

// 新しく変数を作るときにスタックのどこに作るかのオフセット
#define TMPVAR_OFF(r) -((i+1)+1)*4

char *tmpRegName[N_REG] = {"eax", "ebx", "ecx", "edx"};
char *regAddressPrefix[4] = {"byte", "word", "unDefined", "dword"};

// 実際のレジスタにどの仮想レジスタが割り当てられているか(なし:-1)
// tmpRegState[0] == 2  : eax は退避領域の2番の値を指している
int tmpRegState[N_REG];

// 退避領域の仮想レジスタ
int tmpRegSave[N_SAVE];

// prototype
void initTmpReg();
int getFreeReg(int rs);
void assignReg(int rs, int reg);
void freeReg(int reg);
void saveReg(int reg);
void saveAllReg();
int appReg(int rs);
void funcAsm(char *name, int codeSize);
void compilePivot(int opcode, int opd1, int opd2, int opd3);

void initTmpReg() {
    int i;
    for(i=0; i<N_REG; i++) tmpRegState[i] = -1;
    for(i=0; i<N_SAVE; i++) tmpRegSave[i] = -1;
}

// 空いてるレジスタを探す
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

// 仮想レジスタrsを実レジスタregに無理やり割当
// 特定のレジスタが必要な場合に使用(システムコールなど)
void assignReg(int rs, int reg) {
    if(tmpRegState[reg] = rs) return;
    saveReg(reg);
    tmpRegState[reg] = rs;
}

void freeReg(int reg) {
    tmpRegState[reg] = -1;
}

// 実レジスタregを退避
void saveReg(int reg) {
    int i;

    if(tmpRegState[reg] < 0) return;
    for(i=0; i<N_SAVE; i++) {
        if(tmpRegSave[i] < 0) {
            // mov  [ebp-TMPVAR_OFF(reg)], edx など
            printf("\tmov\t[ebp%d], %s\n", TMPVAR_OFF(reg), tmpRegName[reg]);
            tmpRegSave[i] = tmpRegState[reg];
            tmpRegState[reg] = -1;
            return;
        }
    }
    fprintf(stderr, "Free tmp register was not found\n");
}

void saveAllReg() {
    int i;
    for(i=0; i<N_REG; i++) saveReg(i);
}

// 仮想レジスタrsがどの実レジスタと対応しているか調べる
// rsが退避領域にある => 実レジスタも入れ込む
int appReg(int rs) {
    int i, r;

    for(i=0; i<N_REG; i++) if(tmpRegState[i] == rs) return i;

    for(i=0; i<N_SAVE; i++) {
        if(tmpRegSave[i] == rs) {
            r = getFreeReg(rs);
            tmpRegSave[i] = -1;
            printf("\tmov\t%s,[ebp%d]\n", TMPVAR_OFF(i), tmpRegName[r]);
            return r;
        }
    }
    fprintf(stderr, "register is not found\n");
}

void funcAsm(char *name, int codeSize) {
    int i;

    puts(".text");                                      /* .text */
    printf("%s:\n", name);                              /* name: */

    puts("\tpush\tebp");      // push ebp
    puts("\tmov\tebp, esp");  // mov ebp, esp
    puts("\tsub\tesp, 4");    // sub esp, 4

    initTmpReg();
    for(i=0; i<codeSize; i++) {
        compilePivot(codes[i].opcode, codes[i].operand1, codes[i].operand2, codes[i].operand3);
    }

    // return
    printf("\tleave\n");
    printf("\tret\n");
}

void compilePivot(int opcode, int opd1, int opd2, int opd3) {
    #ifdef __ASM_DEBUG_MODE
    printf("[d] %s %d %d\n", getPivotName(opcode), opd1, opd2, opd3);
    #endif
    return;
}
