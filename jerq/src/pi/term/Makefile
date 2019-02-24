OFILES=call.o step.o core.o memfault.o graphic.o jps.o main.o journal.o text.o cursor.o protocol.o lib.o
CC=3cc
INC=-I/usr/jerq/include 

pi.m:	$(OFILES)
	$(CC)  $(OFILES)
	cprs a.out pi.m
	rm a.out

%.o:	%.c
	$(CC) $(INC) -gc $*.c

clean:
	rm -f *.[os] junk* jim* dead.letter

core.o: term.h
core.o: ../protocol.h
cursor.o: term.h
graphic.o: term.h
graphic.o: ../protocol.h
journal.o: term.h
jps.o: term.h
jps.o: ../protocol.h
lib.o: term.h
main.o: term.h
main.o: ../protocol.h
protocol.o: term.h
protocol.o: ../protocol.h
step.o: term.h
step.o: ../protocol.h
text.o: term.h
