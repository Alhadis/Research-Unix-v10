CFLAGS = -g
LIB = libq.a
LIBO = $(LIB)(q.o) $(LIB)(gpib.o) $(LIB)(iberr.o) $(LIB)(library.o)

all:	qsnap

other:	qreset qbright qstatus qtest qreso

%:	%.c $(LIB)
	$(CC) $(CFLAGS) -o $% $%.c $(LIB)

qsnap:	Qsnap.c $(LIB) piclib.o
	$(CC) $(CFLAGS) -o qsnap Qsnap.c fsqrt.s $(LIB) piclib.o

qreso:	qreso.c $(LIB)
	$(CC) $(CFLAGS) -o qreso qreso.c $(LIB)

qtest:	qtest.c $(LIB)
	$(CC) $(CFLAGS) -o qtest qtest.c $(LIB)

$(LIB):	$(LIBO)

$(LIB)(%.o):		%.c
			$(CC) $(CFLAGS) -c $%.c
			ar rv $(LIB) $%.o
			ranlib $(LIB)
			rm $%.o
