L=libnetb.a
CFLAGS=-O
MAIN=disp.o funcs.o tag.o setupmain.o log.o panic.o\
	gread.o fsmkidmap.o setupmap.o perm.o
STUBS=\
	stubcreate.o stubdelete.o stubdirread.o \
	stubdone.o stublink.o stubmkdir.o stubread.o \
	stubrmdir.o stubstat.o stubupdate.o \
	stubwalk.o stubwrite.o
#CALLER=reswhocalled.o callsource.o
CALLER=reswhocalled.o callsource.o

FILES=$(MAIN) $(STUBS) $(CALLER)

all: $L

$L: $(FILES)
	ar ru $L $(FILES)
	ranlib $L

clean:
	rm -f *.o

install: $L
	cp rf.h /usr/include
	cp $L /usr/lib
	ranlib /usr/lib/$L

$(FILES): rf.h
funcs.o main.o tag.o: tag.h
perm.o setupmap.o: perm.h
disp.o funcs.o: netb.h nberrno.h

setup:
	cc -O -o setup setup.c -lipc
runfs:
	cc -O -o runfs runfs.c
