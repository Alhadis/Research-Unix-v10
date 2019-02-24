LIB=/usr/v9/lib/upas
MAIL=/usr/mail
SMTPQ=/usr/spool/smtpq
SYSALIAS=namefiles
USERALIAS=/lib/names
MBOXMODE=0600

#  The following library seems to have a different name on every
#  System 5 I know.
#
# SOCLIB=-lbsd		# for MIPS
# SOCLIB=-lsocket 	# for Microsoft on AT&T 6386
# SOCLIB=-lcmc		# for 3bvme
SOCKLIB=-lbsd

LIBC=s5libc.a
all: config.o
	rm -f ../config/config.o;
	ln config.o ../config/config.o
	rm -f ../libc/libc.a;
	(cd ../libc;	make UNIX=-DSYS5 $(LIBC);	ln $(LIBC) libc.a)
	(cd ../common;	make UNIX=-DSYS5)
	(cd ../send;	make UNIX=-DSYS5)
	(cd ../print;	make UNIX=-DSYS5)
	(cd ../ns;	make UNIX=-DSYS5)
	(cd ../smtp;	make "LIBS=${SOCKLIB}" "FUNNY=-DSYS5" s5)

config.c: makefile
	rm -f config.c
	echo 'char *UPASROOT = "$(LIB)/";' >> config.c
	echo 'char *MAILROOT = "$(MAIL)/";' >> config.c
	echo 'char *SMTPQROOT = "$(SMTPQ)/";' >> config.c
	echo 'char *SYSALIAS = "$(SYSALIAS)";' >> config.c
	echo 'char *USERALIAS = "$(USERALIAS)";' >> config.c
	echo 'int MBOXMODE = $(MBOXMODE);' >> config.c

install:
	-mkdir $(LIB)
	-mkdir $(MAIL)
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
	rm -f config.o
	rm -f ../config/config.o
