LIB=/usr/lib/upas
MAIL=/usr/spool/mail
SMTPQ=/usr/spool/smtpq
SYSALIAS=namefiles
USERALIAS=/lib/names
MBOXMODE=0644
CC=../s5.3k/fakecc
AR=../s5.3k/fakear

LIBC=s5libc.a
all: config.o
	-rm ../config/config.o;
	ln config.o ../config/config.o
	-rm -f ../libc/libc.a;
	(cd ../libc;	make AR=../s5.3k/fakear CC=../s5.3k/fakecc $(LIBC); ln $(LIBC) libc.a)
	(cd ../common;	make AR=../s5.3k/fakear CC=../s5.3k/fakecc)
	(cd ../send;	make CC=../s5.3k/fakecc)
	(cd ../print;	make CC=../s5.3k/fakecc)
	(cd ../ns;	make CC=../s5.3k/fakecc)
	(cd ../rfc822;	make CC=../s5.3k/fakecc)

config.c: makefile
	-rm config.c
	echo 'char *UPASROOT = "$(LIB)/";' >> config.c
	echo 'char *MAILROOT = "$(MAIL)/";' >> config.c
	echo 'char *SMTPQROOT = "$(MAIL)/";' >> config.c
	echo 'char *SYSALIAS = "$(SYSALIAS)";' >> config.c
	echo 'char *USERALIAS = "$(USERALIAS)";' >> config.c
	echo 'int MBOXMODE = $(MBOXMODE);' >> config.c

install:
	-mkdir /usr/lib/upas
	-mkdir /usr/spool/mail
	(cd ../send;	make install LIB=$(LIB) CC=../s5.3k/fakecc)
	(cd ../ns;	make install LIB=$(LIB) CC=../s5.3k/fakecc)
	(cd ../print;	make install LIB=$(LIB) CC=../s5.3k/fakecc)
	(cd ../misc;	make install LIB=$(LIB) CC=../s5.3k/fakecc)

clean:
	(cd ../libc;	make clean)
	(cd ../misc;	make clean)
	(cd ../common;	make clean)
	(cd ../send;	make clean)
	(cd ../ns;	make clean)
	(cd ../print;	make clean)
	(cd ../rfc822;	make clean)
	-rm config.o
	-rm ../config/config.o
