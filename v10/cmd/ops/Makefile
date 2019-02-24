CC=cc
CFLAGS= -DDMMIN=32

SRC=ps.h ps.c doselect.c printp.c fdprint.c \
getargs.c gettty.c getuname.c getfs.c sort.c nalloc.c

OBJ=ps.o doselect.o printp.o fdprint.o \
getargs.o gettty.o getuname.o getfs.o sort.o nalloc.o

ps:	& $(OBJ)
	rm -f $@
	$(CC) $(CFLAGS) -o $@ $(OBJ)
	chmod a=rx,u+ws $@

$(OBJ): ps.h

install: ps
	cp ps /bin
	strip /bin/ps
	/etc/chown root /bin/ps
	chmod a=rx,u+ws /bin/ps

clean:
	rm -f *.o ps nohup.out core jim.recover
