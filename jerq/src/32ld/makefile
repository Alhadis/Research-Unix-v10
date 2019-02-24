BIN	= /usr/jerq/bin
CFLAGS	= -I/usr/jerq/include
RM	= /bin/rm -f

.c.o:
		$(CC) $(CFLAGS) -c $<

32ld:		32ld.o proto.o pcheck.o
		$(CC) $(LFLAGS) -o 32ld 32ld.o proto.o pcheck.o

32ld.o:		proto.h $(FRC)
proto.o:	proto.h $(FRC)

install:	32ld
		cp 32ld $(BIN)
		strip $(BIN)/32ld

clean:
	$(RM) *.o* 32ld

clobber:	clean
		$(RM) 32ld

lint:
		lint $(CFLAGS) *.c

pp:
		pp -t32ld makefile *.h *.c *.cm *.sm | dcan
