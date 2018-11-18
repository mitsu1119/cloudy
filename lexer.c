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
    case ',':
    case ';':
    case EOF:
    	return c;
    case '"':
        p = yytext;
        while((c = getc(stdin)) != '"') {
            *p++ = c;
        }
        *p = '\0';
        yylval.val = makeStrAST(strdup(yytext));
        return STRING;
    }

    if(isdigit(c)) {    // 数値
        n = 0;
        do {
            n = n*10 + c-'0';
            c = getc(stdin);
        } while(isdigit(c));
        ungetc(c, stdin);
        yylval.val = makeNumAST(n);
        return NUMBER;
    }

    if(isalpha(c)){        // / [A-Za-z]+ /
	    p = yytext;
	    do {
	        *p++ = c;
	        c = getc(stdin);
	    } while(isalpha(c));
	    *p = '\0';
	    ungetc(c, stdin);

        if(!strcmp(yytext, "int")) {
            return INT;
        }

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
