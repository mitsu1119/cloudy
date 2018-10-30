#include <stdio.h>

#define MAX_CODES 100

struct _Code {
    int opcode, operand1, operand2, operand3;
} codes[MAX_CODES];
int codecnt;

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

void funcAsm(char *name) {
    int i;
    printf("*** functionAsm <%s> ***\n", name);
    puts(".text");
    printf(".global\t%s\n", name);
    printf(".type\t%s, @function\n", name);
    printf("%s:\n", name);

    puts("push\tebp");      // push ebp
    puts("mov\tebp, esp");  // mov ebp, esp
    puts("sub\tesp, 4");    // sub esp, 4
    
}
