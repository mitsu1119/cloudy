#include <stdio.h>
#include "compile.h"
#include "pivotCode.h"

// #define __ASM_DEBUG_MODE

#define MAX_CODES 100

struct _Code {
    int opcode, operand1, operand2, operand3;
    char *operandStr;
} codes[MAX_CODES];
int codecnt;

int labelcnt = 0;

void asmGVar();

// アセンブラ起動時の初期化関数
void initAssemble() {
    puts("global main");
}

// 関数のアセンブルの初期化関数
void asmIni() {
    int i;

    codecnt = 0;
    LVarp = 0;

    // 関数のコードを生成する前にグローバル変数を宣言、定義
    if(GVarp > 0) {
        asmGVar();
    }
}

void asmGVar() {
    int i;

    puts("section .data");  // section .data
    for(i=0; i<GVarp; i++) {
        if(gvars[i].var->isBss == FALSE) printf("\t%s\tdd\t%d\n", gvars[i].var->name, gvars[i].var->val);   // varname dd value
    }
    puts("section .bss");   // section .bss
    for(i=0; i<GVarp; i++) {
        if(gvars[i].var->isBss == TRUE) printf("\t%s\tresb\t4\n", gvars[i].var->name);  // varname resb 4
    }
    GVarp = 0;
}

void asmString(char *s) {
    puts("section .rodata");
    printf(".LC%d:\n", labelcnt++);
    printf("\tdb\t'%s'\n",s);
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

void genCodeS(int opcode, int operand1, int operand2, char *operands) {
    codes[codecnt].opcode = opcode;
    codes[codecnt].operand1 = operand1;
    codes[codecnt].operand2 = operand2;
    codes[codecnt++].operandStr = operands;
}

/* *** Assemble *** */

#define N_REG 4
#define N_SAVE 4

#define EAX 0
#define EBX 1
#define ECX 2
#define EDX 3

// 変数のオフセット計算
// 退避領域は ebp-4, ebp-8, ebp-12, ebp-16, ..., ebp-(N_SAVE*4) まで 
#define LOCALVAR_OFF(pos) -(pos+1+N_SAVE)*4 // ebpから
#define TMPVAR_OFF(r) -(r+1)*4      // ローカル変数の先頭から

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
void funcAsm(char *name, int localvarSize);
void compilePivot(int opcode, int opd1, int opd2, int opd3, char *opdS, int returnLabel);

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
    if(tmpRegState[reg] == rs) return;
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
    exit(1);
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
            printf("\tmov\t%s, [ebp%d]\n", tmpRegName[r], TMPVAR_OFF(i));
            return r;
        }
    }
    fprintf(stderr, "register is not found\n");
    exit(1);
}

void funcAsm(char *name, int localvarSize) {
    int i;
    int returnLabel = labelcnt++;

    puts("section .text");                                      // section .text
    printf("%s:\n", name);                              // name: 

    // create stack frame
    puts("\tpush\tebp");      // push ebp
    puts("\tmov\tebp, esp");  // mov ebp, esp
    printf("\tsub\tesp, %d\n", (localvarSize+N_SAVE)*4);

    initTmpReg();
    for(i=0; i<codecnt; i++) {
        compilePivot(codes[i].opcode, codes[i].operand1, codes[i].operand2, codes[i].operand3, codes[i].operandStr, returnLabel);
    }

    // return
    printf(".L%d:\n", returnLabel);
    printf("\tmov\tesp, ebp\n");    // mov esp,ebp
    printf("\tpop\tebp\n");         // pop ebp
    printf("\tret\n");              // ret
}

void compilePivot(int opcode, int opd1, int opd2, int opd3, char *opdS, int returnLabel) {
    int reg1, reg2;
    
    #ifdef __ASM_DEBUG_MODE
    printf("[d] %s %d %d\n", getPivotName(opcode), opd1, opd2, opd3);
    #endif

    switch(opcode) {
    case LOADI:     // load integer
        if(opd1 < 0) return;
        reg1 = getFreeReg(opd1);
        printf("\tmov\t%s, %d\n", tmpRegName[reg1], opd2);
        return;
    case STOREL:    // store local variable
        reg1 = appReg(opd1);
        freeReg(reg1);
	    printf("\tmov\t[ebp%d], %s\n", LOCALVAR_OFF(opd2), tmpRegName[reg1]);
        return;
    case ADD:
        reg1 = appReg(opd2);
        reg2 = appReg(opd3);
        freeReg(reg1);
        freeReg(reg2);
        if(opd1 < 0) return;
        assignReg(opd1, reg1);
        printf("\tadd\t%s, %s\n", tmpRegName[reg1], tmpRegName[reg2]);
        return;
    case SUB:
        reg1 = appReg(opd2);
        reg2 = appReg(opd3);
        freeReg(reg1);
        freeReg(reg2);
        if(opd1 < 0) return;
        assignReg(opd1, reg1);
        printf("\tsub\t%s, %s\n", tmpRegName[reg1], tmpRegName[reg2]);
        return;
    case MUL:
        reg1 = appReg(opd2);
        reg2 = appReg(opd3);
        freeReg(reg1);
        freeReg(reg2);
        if(opd1 < 0) return;
	    assignReg(opd1, EAX);
        saveReg(EDX);
        if(reg1 != EAX) printf("\tmov\t%s,%s\n", tmpRegName[EAX], tmpRegName[reg1]);
        printf("\txor\t%s, %s\n", tmpRegName[EDX], tmpRegName[EDX]);
        printf("\tmul\t%s\n", tmpRegName[reg2]);
        return;
    case DIV:
        reg1 = appReg(opd2);
        reg2 = appReg(opd3);
        freeReg(reg1);
        freeReg(reg2);
        if(opd1 < 0) return;
	    assignReg(opd1, EAX);
        saveReg(EDX);
        if(reg1 != EAX) printf("\tmov\t%s,%s\n", tmpRegName[EAX], tmpRegName[reg1]);
        printf("\txor\t%s, %s\n", tmpRegName[EDX], tmpRegName[EDX]);
        printf("\tdiv\t%s\n", tmpRegName[reg2]);
        return;
    case CALL:
        saveAllReg();
        printf("\tcall\t%s\n", opdS);
        if(opd1 < 0) break; // 返り値を格納しない場合break
        return;
    case RET:
        reg1 = appReg(opd1);
        freeReg(reg1);
        if(reg1 != EAX) printf("\tmov\teax, %s\n", tmpRegName[reg1]);
        printf("\tjmp\t.L%d\n", returnLabel);
        return;
    }
    return;
}
