CC=cc
OBJS=small.o regcomp.o regerror.o regexec.o regsub.o
SRC=small.c regcomp.c regerror.c regexec.c regprog.h regsub.c regexp.h
all: $(OBJS)
	$(CC) $(OBJS) -lm -o small
clean:
	rm -f $(OBJS) small

install:
	mv small /usr/games/lib/word_clout
	cp word_clout /usr/games
