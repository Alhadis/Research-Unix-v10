expr:	regexp.h expr.y
	yacc expr.y
	cc -O -s y.tab.c -o expr
install:	expr
	cp expr /usr/bin
clean:
	-rm -f expr y.tab.c
