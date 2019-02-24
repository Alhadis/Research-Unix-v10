# cetus is an SGI box.  Directories are in slightly unusual places.
# also for hydra and pyxis.
#
#	Needs SGI_CC=-cckr

LIB=/v/lib/upas
LOCKS=/usr/spool/locks
MAIL=/usr/mail
SMTPQ=/v/spool/smtpq
SYSALIAS=namefiles
USERALIAS=/lib/names
MBOXMODE=0600


LIBC=sgilibc.a
all: config.o ../common/sys.h
	rm -f ../config/config.o;
	ln config.o ../config/config.o
	rm -f ../libc/libc.a;
	(cd ../libc;	make $(LIBC);	ln $(LIBC) libc.a)
	(cd ../common;  SCFLAGS="-I/usr/include/bsd" make)
	(cd ../send;	SLFLAGS=-lbsd make)
	(cd ../print;	SCFLAGS="-I/usr/include/bsd" SLFLAGS= make)
	(cd ../ns;	make)
	(cd ../ipc;	SCFLAGS="-I/usr/include/bsd" ERR=mips_err.o \
				make init tcp lib)
	(cd ../smtp;	SCFLAGS='-I/usr/include/bsd' SLFLAGS=-lbsd make)

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
	-mkdir $(LOCKS)
	chmod 777 $(LOCKS)
	-mkdir $(MAIL)
	(cd ../send;	make install LIB=$(LIB))
	(cd ../ns;	make install LIB=$(LIB))
	(cd ../print;	make install LIB=$(LIB))
	(cd ../misc;	make install LIB=$(LIB))
	(cd ../smtp;	make install LIB=$(LIB))

ship:
	ship /bin/mail /bin/rmail /v/lib/upas/edmail /v/lib/upas/gone.fishing /v/lib/upas/gone.msg /v/lib/upas/send /v/lib/upas/smtp /v/lib/upas/smtpd /v/lib/upas/smtpsched /v/lib/upas/translate
	dest='blue1 blue2 blue3 blue4 fiber1 fiber2' ship /bin/mail /bin/rmail

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
