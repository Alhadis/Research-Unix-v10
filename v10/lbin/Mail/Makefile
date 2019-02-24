#	Makefile	2.15	83/08/18
#
# Berkeley Mail
#
# CFLAGS options:
#	-DVMUNIX	Virtual memory
#			Supports job control and vfork()
#	-DV7		Supports Version 7 UNIX
#	-DNODATE	Don't check date format
#	-DBSD4_2	Supports sigblock() and sigsetmask() system calls
#	-DUSGFROM	Ignore silly USG `FROM:' header field
#
CFLAGS=	-g -DVMUNIX -DV7 -DNODATE -DUSGFROM
XSTR=	/usr/bin/xstr
DESTDIR=
CTAGS=	/usr/bin/ctags -w
VPRINT=	/usr/bin/print
CGRAF=	/usr/ber/bin/cgraf
CGRAPH=	/usr/ber/bin/cgraph
NMKINDEX=/usr/ber/bin/nmkindex
DIRTCLOD=/usr/ber/bin/dirtclod
LIBES=-ljobs					## not officially in Version 9
LIBES=/n/bowell/usr/src/lbin/libjobs/libjobs.a	## mjm: unofficial "-ljobs"
ED=	-ed
AS=	-as
RM=	-rm
OBJS=	version.o aux.o cmd1.o cmd2.o cmd3.o cmdtab.o collect.o \
	config.o edit.o fio.o \
	getname.o head.o v7.local.o lock.o lex.o list.o main.o \
	names.o optim.o popen.o quit.o send.o strings.o temp.o tty.o \
	vars.o str.o 4.2sys.o
SRCS=	aux.c cmd1.c cmd2.c cmd3.c cmdtab.c collect.c config.c edit.c fio.c \
	getname.c head.c v7.local.c lock.c lex.c list.c main.c \
	names.c optim.c popen.c quit.c send.c strings.c temp.c tty.c vars.c \
	version.c 4.2sys.c
HDRS=	rcv.h configdefs.h def.h glob.h v7.local.h local.h
ALL=	Mail fmt
S = $(SRCS) $(HDRS)

#
# Special massaging of C files for sharing of strings
#
.SUFFIXES: .o .c

.c.o:
	${CC} -E ${CFLAGS} $*.c | ${XSTR} -c -
	${CC} -c ${CFLAGS} x.c 
	mv x.o $*.o
	@$(RM) -f x.c

all:	${ALL}

Mail:	$S $(OBJS)
	$(RM) -f Mail
	@echo Loading ...
	@$(CC) -n -o Mail $(OBJS) $(LIBES)
	@size Mail

mail.g:	$(SRCS)
	$(CGRAF) $(SRCS) > mail.g

install: all
	mv Mail fmt ${DESTDIR}/usr/bin
	strip  ${DESTDIR}/usr/bin/Mail ${DESTDIR}/usr/bin/fmt

version.o:	version.c
	$(CC) -c -R version.c

$S:
	sccs get $@;

srcs:	$S

tags:	$(SRCS)
	${CTAGS} $(SRCS);

clean:
	$(RM) -f *.o
	rm -f Mail a.out fmt x.c xs.c tags core
	cp /dev/null strings

lint:
	lint $(CFLAGS) $(SRCS)

fmt:	fmt.c head.c
	$(CC) fmt.c head.c -o fmt
	$(RM) fmt.o head.o
	size fmt

str.o: strings
	$(XSTR)
	$(CC) -R -c xs.c
	mv xs.o str.o
	$(RM) xs.c

cmdtab.o: cmdtab.c
	$(CC) -R -c $(CFLAGS) cmdtab.c

print:	$S fmt.c
	$(CGRAPH) > mail.g;
	$(NMKINDEX) > mail.i;
	$(DIRTCLOD) -x mail.i -p mail.g > mail.x
	@$(VPRINT) Makefile mail.x rcv.h def.h glob.h local.h v7.local.h \
	    $(SRCS) fmt.c;

wc:
	@wc rcv.h def.h glob.h local.h v7.local.h $(SRCS)

sc:
	@grep -c \; rcv.h def.h glob.h local.h v7.local.h $(SRCS)

strings:
	echo "making strings"
