/* lexer.c */
#include <ctype.h>

char yytext[100];

int yylex() {
    int c,n;
    char *p;
again:
    c = getc(stdin);
    if(isspace(c)) goto again;
    switch(c){
    case '+':
    case '-':
    case '*':
    case '/':
    case '=':
    case '(':
    case ')':
    case '{':
    case '}':
    case ';':
    case EOF:
    	return c;
    }

    if(isalpha(c)){        // / [A-Za-z]+ /
	    p = yytext;
	    do {
	        *p++ = c;
	        c = getc(stdin);
	    } while(isalpha(c));
	    *p = '\0';
	    ungetc(c, stdin);
	    yylval.val = makeSymAST(yytext);
	    return SYMBOL;
    }
    fprintf(stderr, "bad char '%c'\n", c);
    exit(1);
}

void yyerror() {
    fprintf(stderr, "syntax error\n");
    exit(1);
}
