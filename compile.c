#include "AST.h"

void defineFunction(Symbol *sym, AST *body) {
    asmIni();
    funcAsm(sym->name);
}
