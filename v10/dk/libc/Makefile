CFLAGS = -O
LIB = tdkdial.o tdkexec.o tdklogin.o tdkmgr.o dkproto.o  \
	dkctlchan.o pwsearch.o

%.o:	%.c
	$(CC) $(CFLAGS) -I../cmd -c $%.c

all: libdk.a

libdk.a:	$(LIB)
	rm -f libdk.a
	ar rc libdk.a $(LIB)

clean:
	rm -f *.o libdk.a

install:	libdk.a
	cp libdk.a /usr/lib
	ranlib /usr/lib/libdk.a
