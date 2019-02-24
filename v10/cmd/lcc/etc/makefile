HOST=?
LDFLAGS=-s
CFLAGS=

a.out:	$(HOST).o lcc.o
	$(CC) $(LDFLAGS) $(HOST).o lcc.o; rm -f $(HOST).o lcc.o

bprint:	bprint.c ../c/profio.c
	$(CC) -o bprint $(LDFLAGS) bprint.c

clean:
	-rm -f *.o a.out core bprint
