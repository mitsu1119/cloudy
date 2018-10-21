#include "AST.h"

int main() {
    AST *sym1 = makeSymAST("sym1");
    AST *sym2 = makeSymAST("sym2");

    drawSymTable();
    yyparse();
    return 0;
}
