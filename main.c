#include "AST.h"


int main() {
    initAssemble();
    yyparse();
    return 0;
}
