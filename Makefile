run: main.o AST.o y.tab.o assembly.o compile.o pivotCode.o
	gcc -o run main.o AST.o y.tab.o assembly.o compile.o pivotCode.o
main.o: main.c
	gcc -c main.c
AST.o: AST.c AST.h
	gcc -c AST.c
y.tab.o: lexer.c parser.y
	yacc parser.y
	gcc -c y.tab.c
assembly.o: assembly.c compile.h AST.h
	gcc -c assembly.c
compile.o: compile.c AST.h pivotCode.h
	gcc -c compile.c
pivotCode.o: pivotCode.c pivotCode.h
	gcc -c pivotCode.c

clean:
	rm -f run main.o AST.o y.tab.o y.tab.c assembly.o compile.o pivotCode.o
