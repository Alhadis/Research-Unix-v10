OBJ = ../spitv35.o ../spitv35sub.o \
	arg2scb.o cpys2sc.o doexec.o doset.o dosys.o fakexit.o flush.o \
        gethost.o lenfnm.o osclose.o osopen.o \
	osint.o ospipe.o osread.o oswait.o oswrite.o \
	rdenv.o sigs.o sioarg.o swcinp.o swcoup.o testty.o wrtaout.o \
	interface.o stubs.o sysax.o sysbx.o sysdt.o sysem.o sysep.o \
	sysou.o sysrw.o

COBJ = arg2scb.o doexec.o dosys.o fakexit.o flush.o lenfnm.o \
	osclose.o osopen.o \
	osread.o oswrite.o rdenv.o sigs.o sioarg.o swcinp.o swcoup.o testty.o \
	wrtaout.o stubs.o sysax.o sysbx.o sysdt.o sysem.o sysep.o \
	sysou.o sysrw.o

CFLAGS = -O

newspit: $(OBJ)
	cc -o newspit $(OBJ)

clean:
	rm -f spitbol newspit $(OBJ)
