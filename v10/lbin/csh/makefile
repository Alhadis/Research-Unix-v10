#
#	makefile	4.1	10/9/80
#
# C Shell with process control; VM/UNIX VAX Makefile
# Bill Joy UC Berkeley; Jim Kulp IIASA, Austria
#
CFLAGS=	-DTELL -DVMUNIX -Ddebug -O
XSTR=	xstr
ED=	-ed
AS=	-as
RM=	-rm
CXREF=	cxref
VGRIND=	csh /usr/bin/vgrind
CTAGS=	ctags
# LIBES=	-ljobs 
LIBES=	../libjobs/libjobs.a
SCCS=	/usr/local/sccs

OBJS=	sh.o sh.dol.o sh.err.o sh.exec.o sh.exp.o sh.func.o sh.glob.o \
	sh.hist.o sh.lex.o sh.misc.o sh.parse.o sh.print.o sh.sem.o sh.set.o \
	sh.proc.o sh.dir.o sh.time.o alloc.o sh.init.o printf.o doprnt.o

csh: ${OBJS} sh.local.h
	rm -f csh
	cc ${OBJS} -o csh ${LIBES}

# need an old doprnt, whose output we can trap
doprnt.o: doprnt.c
	cc -E doprnt.c > doprnt.s
	as -o doprnt.o doprnt.s
	rm -f doprnt.s
	
lint:
	lint ${CFLAGS} sh*.c

clean:
	${RM} -f a.out strings x.c xs.c csh
	${RM} -f *.o sh.prof.c
