LIB=/v/lib/upas
MAIL=/usr/mail
SMTPQ=/usr/spool/smtpq
SYSALIAS=namefiles
USERALIAS=/lib/names
MBOXMODE=0600


LIBC=s5libc.a
all: config.o ../common/sys.h
	rm -f ../config/config.o;
	ln config.o ../config/config.o
	rm -f ../libc/libc.a;
	(cd ../libc;	make $(LIBC);	ln $(LIBC) libc.a)
	(cd ../common;  SCFLAGS="-I/usr/include/bsd" make)
	(cd ../send;	SLFLAGS=-lbsd make)
	(cd ../print;	SCFLAGS="-I/usr/include/bsd" SLFLAGS= make)
	(cd ../ns;	make)
	(cd ../ipc;     SCFLAGS="-I/usr/include/bsd" ERR=cray_err.o make craylib)
	(cd ../smtp;	SCFLAGS="-I/usr/include/bsd" SLFLAGS="" make)

config.c: research.mk
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
