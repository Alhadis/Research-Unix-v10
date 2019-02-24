OBJ=doprint.o exit.o getfields.o onexit.o print.o strdup.o regcomp.o regexec.o regsub.o dup2.o

libc.a:	$(OBJ)
	ar r libc.a $(OBJ)
	$(RANLIB) libc.a

cray:	libc.o

libc.o:	$(OBJ)
	cat $(OBJ) > libc.o
