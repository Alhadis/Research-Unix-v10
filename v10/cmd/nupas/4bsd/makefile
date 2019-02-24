LIB=/usr/lib/upas
MAIL=/usr/spool/mail
SMTPQ=/usr/spool/smtpq
SYSALIAS=namefiles
USERALIAS=/lib/names
MBOXMODE=0644

LIBC=4bsdlibc.a
all: config.o
	-rm ../config/config.o;
	ln config.o ../config/config.o
	-rm -f ../libc/libc.a;
	(cd ../libc;	make UNIX=-DBSD $(LIBC);	ln $(LIBC) libc.a)
	(cd ../common;	make UNIX=-DBSD)
	(cd ../send;	make UNIX=-DBSD)
	(cd ../print;	make UNIX=-DBSD)
	(cd ../ns;	make UNIX=-DBSD)
	(cd ../smtp;	make FUNNY="-DBSD -DSOCKET -DBIND -DSYSLOG" bsd)

config.c: makefile
	-rm config.c
	echo 'char *UPASROOT = "$(LIB)/";' >> config.c
	echo 'char *MAILROOT = "$(MAIL)/";' >> config.c
	echo 'char *SMTPQROOT = "$(SMTPQ)/";' >> config.c
	echo 'char *SYSALIAS = "$(SYSALIAS)";' >> config.c
	echo 'char *USERALIAS = "$(USERALIAS)";' >> config.c
	echo 'int MBOXMODE = $(MBOXMODE);' >> config.c

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
	(cd ../smtp;	make clean)
	-rm -f config.o
	-rm -f ../config/config.o
	
