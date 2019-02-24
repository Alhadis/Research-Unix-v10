# "@(#)mx.mk	1.4 'attmail mail(1) command'"
#
# mailx -- a modified version of a University of California at Berkeley
#	mail program
#
# for standard Unix
#

ROOT=
VERSION=SVR3
HDR=	hdr
HELP=	help
DESTDIR= $(ROOT)/usr/bin

# If system == SVR3, use the following...
DESTLIB = $(ROOT)/usr/lib/mailx
CPPDEFS = -DpreSVr4 -I$(HDR) -I$(CRX)/usr/include -I$(ROOT)/usr/include 
CFLAGS  = -O $(CPPDEFS) 
#LD_LIBS = -L$(ROOT)/usr/lib -lmail
#SYMLINK = :
CC = cc

# If system == SVR4, use the following...
#DESTLIB = $(ROOT)/usr/share/lib/mailx
#CPPDEFS = -I$(HDR) -I$(CRX)/usr/include -I$(ROOT)/usr/include 
#CFLAGS  = -O -v -Xa  $(CPPDEFS) 
LD_FLAGS = -s $(LDFLAGS) $(PERFLIBS)
#LD_LIBS = -L$(ROOT)/usr/lib -lmail $(LDLIBS)
#SYMLINK = ln -s

MAILDIR = $(ROOT)/usr/mail

HOSTCC=	/bin/cc
INS=	install

SRCS=	myfopen.c aux.c cmd1.c cmd2.c cmd3.c cmd4.c cmdtab.c collect.c\
	config.c edit.c \
	init.c is.c fio.c genopen.c getname.c head.c hostname.c lex.c \
	list.c\
	lpaths.c main.c names.c optim.c popen.c quit.c receipt.c send.c \
	sigretro.c stralloc.c temp.c translate.c tty.c v9.local.c vars.c \
	version.c

OBJS=	myfopen.o aux.o cmd1.o cmd2.o cmd3.o cmd4.o cmdtab.o collect.o\
	config.o edit.o \
	init.o is.o fio.o genopen.o getname.o head.o hostname.o lex.o \
	list.o\
	lpaths.o main.o names.o optim.o popen.o quit.o receipt.o send.o \
	sigretro.o stralloc.o temp.o translate.o tty.o v9.local.o vars.o \
	version.o

HDRS=	$(HDR)/configdefs.h \
	$(HDR)/def.h \
	$(HDR)/glob.h \
	$(HDR)/local.h \
	$(HDR)/rcv.h \
	$(HDR)/sigretro.h \
	$(HDR)/uparm.h \
	$(HDR)/usg.local.h

S=	$(SRCS) version.c $(HDRS)

.c.o:
	$(CC) -c $(CFLAGS) $*.c

all:	mailx

mailx:	$S $(OBJS)
	-rm -f mailx
	$(CC) $(LD_FLAGS) -o mailx $(OBJS) $(LD_LIBS)

install: ckdirs all
	$(INS) -f $(DESTDIR) -m 2511 -g mail -u bin mailx
	grep -v '^#.*@(' $(HELP)/mailx.help > /tmp/mailx.help
	$(INS) -f $(DESTLIB) -m 644 -u bin -g bin /tmp/mailx.help
	rm /tmp/mailx.help
	grep -v '^#.*@(' $(HELP)/mailx.help.~ > /tmp/mailx.help.~
	$(INS) -f $(DESTLIB) -m 644 -u bin -g bin /tmp/mailx.help.~
	rm /tmp/mailx.help.~

version.o:	makefile version.c
	$(CC) -c version.c

version.c:
	echo \
	"char *version=\"mailx version $(VERSION)\";"\
		> version.c

clean:
	-rm -f *.o
	-rm -f version.c a.out core 

clobber:	clean
	-rm -f mailx

lint:	version.c
	$(PFX)lint $(CFLAGS) $(SRCS)

mailx.cpio:	$(SRCS) $(HDRS) mailx.mk 
	@echo $(SRCS) $(HDRS) mailx.mk | \
		tr " " "\012" | \
		cpio -oc >mailx.cpio

listing:
	pr mailx.mk hdr/*.h [a-l]*.c | lp
	pr [m-z]*.c | lp

ckdirs:
	if [ ! -d $(DESTLIB) ] ; then mkdir $(DESTLIB) ; fi
	$(SYMLINK) $(DESTLIB) /usr/lib/mailx

chgrp: 
	chgrp mail mailx
	chmod g+s mailx
