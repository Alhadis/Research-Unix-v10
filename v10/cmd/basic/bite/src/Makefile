SRC=asc.c atof1.c auto.c avars.c basic.c bed.c belse.c bfiles.c \
bseek.c bsys.c call.c chksec.c chr.c class.c clear.c common.c copen.c \
corleft.c cover.c delete.c dim.c encode.c equal.c error.c evals.c evalx.c expunge.c \
ext.c f.c fact.c fetch.c for.c getdims.c getfnam.c gosub.c goto.c if.c \
input.c iprintf.c isthere.c left.c len.c let.c list.c lnpars.c load.c minus.c \
more.c new.c no_op.c old.c on.c pathopen.c pause.c postb.c print.c put.c \
rand.c random.c read.c reseq.c right.c run.c savars.c save.c sing.c \
stop.c str.c strcat.c string.c sub.c svars.c sysls.c sysrm.c untilcod.c up-lo.c \
update.c val.c variable.c

OBJ=asc.o atof1.o auto.o avars.o basic.o bed.o belse.o bfiles.o \
bseek.o bsys.o call.o chksec.o chr.o class.o clear.o common.o copen.o \
corleft.o cover.o delete.o dim.o encode.o equal.o error.o evals.o evalx.o expunge.o \
ext.o f.o fact.o fetch.o for.o getdims.o getfnam.o gosub.o goto.o if.o \
input.o iprintf.o isthere.o left.o len.o let.o list.o lnpars.o load.o minus.o \
more.o new.o no_op.o old.o on.o pathopen.o pause.o postb.o print.o put.o \
rand.o random.o read.o reseq.o right.o run.o savars.o save.o sing.o \
stop.o str.o strcat.o string.o sub.o svars.o sysls.o sysrm.o untilcod.o up-lo.o \
update.o val.o variable.o

CFLAGS=-O

bite:	$(OBJ)
	cc -o bite $(OBJ)

$(OBJ):	bas.h

tags:	$(SRC)
	ctags $(SRC)

lint:
	@lint $(SRC)

print:
	@pr Makefile bas.h 
	@pr $(SRC)
