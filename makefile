all: compile run

run: 2048.out
	./2048.out

compile: y.tab.c lex.yy.c main.c
	gcc y.tab.c lex.yy.c main.c -o 2048.out

lex.yy.c: lexer.l
	flex lexer.l

y.tab.c: parser.y
	bison -y -d --warnings=none parser.y

clean:
	rm -f lex.yy.c y.tab.* *.out
