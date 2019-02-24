From graceland.ih.att.com!hastings Wed Nov  4 13:48:34 CST 1992
Received: by research.att.com; Wed Nov  4 14:48 EST 1992
Date: Wed, 4 Nov 92 13:48:34 CST
From: hastings@graceland.ih.att.com (Alan Hastings)
To: ches
Subject: sgi upas makefile
Content-Length: 1816

Bill -
	I had to make some changes to the upas sgi/makefile to get it to
	work with NIS here (basically so SLFLAGS can be set on the command
	line):

# cetus is an SGI box.  Directories are in slightly unusual places.
# also for hydra and pyxis.
#
#	Needs SGI_CC=-cckr

LIB=/v/lib/upas
MAIL=/usr/mail
SMTPQ=/v/spool/smtpq
SYSALIAS=namefiles
USERALIAS=/lib/names
MBOXMODE=0600
SLFLAGS=-lbsd


LIBC=sgilibc.a
all: config.o ../common/sys.h
	rm -f ../config/config.o;
	ln config.o ../config/config.o
	rm -f ../libc/libc.a;
	(cd ../libc;	make $(LIBC);	ln $(LIBC) libc.a)
	(cd ../common;  SCFLAGS="-I/usr/include/bsd" make)
	(cd ../send;	SLFLAGS="$(SLFLAGS)" make)
	(cd ../print;	SCFLAGS="-I/usr/include/bsd" SLFLAGS= make)
	(cd ../ns;	make)
	(cd ../ipc;	SCFLAGS="-I/usr/include/bsd" ERR=mips_err.o \
				make init tcp lib)
	(cd ../smtp;	SCFLAGS='-I/usr/include/bsd' SLFLAGS="$(SLFLAGS)" make)

config.c: makefile
	rm -f config.c
	echo 'char *UPASROOT = "$(LIB)/";' >> config.c
	echo 'char *MAILROOT = "$(MAIL)/";' >> config.c
	echo 'char *SMTPQROOT = "$(SMTPQ)/";' >> config.c
	echo 'char *SYSALIAS = "$(SYSALIAS)";' >> config.c
	echo 'char *USERALIAS = "$(USERALIAS)";' >> config.c
	echo 'int MBOXMODE = $(MBOXMODE);' >> config.c

../common/sys.h:	sys.h
	cp sys.h ../common/sys.h
	touch ../common/mail.h

install:
	-mkdir $(LIB)
	-mkdir $(MAIL)
	(cd ../send;	make install LIB=$(LIB))
	(cd ../ns;	make install LIB=$(LIB))
	(cd ../print;	make install LIB=$(LIB))
	(cd ../misc;	make install LIB=$(LIB))
	(cd ../smtp;	make install LIB=$(LIB))

clean:
	rm -f ../common/sys.h
	(cd ../libc;	make clean)
	(cd ../misc;	make clean)
	(cd ../common;	make clean)
	(cd ../send;	make clean)
	(cd ../ns;	make clean)
	(cd ../print;	make clean)
	(cd ../ipc;	make clean)
	(cd ../smtp;	make clean)
	rm -f config.o
	rm -f ../config/config.o

