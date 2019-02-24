CFLAGS = -g
OBJS = malloc_t.o cmalloc.o trace.expr.o trace1.o trace2.o trace3.o trace4.o trace5.o trace6.o trace7.o trace8.o trace9.o assert.o

#	FILES:
#		trace.h		constants and macro definitions
#		trace.d		data structures
#		trace1.c	reads in the tables, sets up data structures
#		trace2.c	memory allocation	
#		trace3.c	the symbolic execution procedures
#		trace4.c	state space handler
#		trace5.c	lookup table for system states
#		trace6.c	lookup table for queue states
#		trace7.c	lookup table for leaf-states
#		trace8.c	lookup table for variable & parameter states
#		trace9.c	lookup table for process states
#		trace.expr.c	evaluation of expressions
#		malloc_t.c	memory allocation interface to cmalloc.c
#		assert.c	assertion primitives

trace:	$(OBJS)
	cc $(CFLAGS) $(OBJS) -o trace

%.o:	%.c trace.d trace.h
	cc $(CFLAGS) -c $%.c

clean:
	rm -f *.o pret.* mon.out core a.out

install:
	cp trace /usr/bin

