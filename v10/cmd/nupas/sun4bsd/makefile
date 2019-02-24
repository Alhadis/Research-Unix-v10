# shamash is a Sun 3 running Sun OS 4

LIB=/usr/lib/upas
MAIL=/usr/spool/mail
SMTPQ=/usr/spool/smtpq
SYSALIAS=namefiles
USERALIAS=/lib/names
MBOXMODE=0644

LIBC=4bsdlibc.a
all: config.o ../common/sys.h
	-rm ../config/config.o;
	ln config.o ../config/config.o
	-rm -f ../libc/libc.a;
	(cd ../libc;	make $(LIBC);	ln $(LIBC) libc.a)
	(cd ../common;	make)
	(cd ../send;	make)
	(cd ../print;	make)
	(cd ../ns;	make)
	(cd ../ipc;	ERR=sun_err.o make init otcp lib)
	(cd ../smtp;	make)

config.c: makefile
	-rm config.c
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
	-mkdir /usr/lib/upas
	-mkdir /usr/spool/mail
	(cd ../send;	make install LIB=$(LIB))
	(cd ../ns;	make install LIB=$(LIB))
	(cd ../print;	make install LIB=$(LIB))
	(cd ../misc;	make install LIB=$(LIB))
	(cd ../smtp;	make install LIB=$(LIB))

clean:
	(cd ../libc;	make clean)
	(cd ../misc;	make clean)
	(cd ../common;	make clean)
	(cd ../send;	make clean)
	(cd ../ns;	make clean)
	(cd ../print;	make clean)
	(cd ../ipc;	make clean)
	(cd ../smtp;	make clean)
	-rm -f config.o ../common/sys.h ../config/config.o
