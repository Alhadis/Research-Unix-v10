CFLAGS=${UNIX} -g -I. -I../libc -I../common -I/usr/include -I/usr/include/sys
AR=ar
.c.o: ; ${CC} -c ${CFLAGS} $*.c

V9OBJS=	getlog.o\
	v9sysname.o v9nsysfile.o mboxowner.o gendate.o genopen.o\
	fread.o fwrite.o
S5OBJS=	getlog.o\
	nsysfile.o s5sysname.o s5mboxowner.o s5date.o posixopen.o\
	setlogname.o regcomp.o regexec.o regerror.o\
	dup2.o
4BSDOBJS=getlog.o\
	nsysfile.o 4bsdsysname.o mboxowner.o gendate.o bsdopen.o\
	setlogname.o regcomp.o regexec.o regerror.o\
	strings.o
NFSOBJS=getlog.o\
	nsysfile.o 4bsdsysname.o mboxowner.o gendate.o genopen.o\
	setlogname.o regcomp.o regexec.o regerror.o\
	strings.o 4bsddirsel.o

all: v9libc.a

v9libc.a: ${V9OBJS}
	${AR} cr v9libc.a ${V9OBJS}
	-ranlib v9libc.a
s5libc.a: ${S5OBJS}
	${AR} cr s5libc.a ${S5OBJS}
sgilibc.a: ${S5OBJS} getlogin.o
	${AR} cr sgilibc.a ${S5OBJS} getlogin.o
3klibc.a: ${S5OBJS}
	cat ${S5OBJS} > 3klibc.a
4bsdlibc.a: ${4BSDOBJS}
	${AR} cr 4bsdlibc.a ${4BSDOBJS}
	-ranlib 4bsdlibc.a
nfslibc.a: ${NFSOBJS}
	${AR} cr nfslibc.a ${nfsOBJS}
	-ranlib nfslibc.a

clean:
	-rm -f *.[oOa] core *.sL
