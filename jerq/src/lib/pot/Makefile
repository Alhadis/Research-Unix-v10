L=libpot.a
X=/usr/jerq/lib/libpot.a
INCL=/usr/jerq/include
CC=3CC
AR=3ar
DCAN=dcan
CFLAGS=-g
PRI=pot.pri

OBJ=pot.o rule.o bmap.o globe.o moved.o elarc.o
CFILES=bmap.c globe.c moved.c pot.c rule.c


%.pri:	%.h
	@cleave -I/usr/jerq/include $*
	@touch $*.pri

$L:	$(OBJ)
	$(AR) uv $L $^

install:	$L
	rm -f $X
	ln $L $X
	cp pot.pub /usr/jerq/include

tst:	$(PRI) tst.o libpot.a
	$(CC) $(CFLAGS) -o tst tst.o libpot.a -ljj

new:	$(OBJ)
	rm $L
	$(AR) uv $L $(OBJ)

pp:
	pp -tGraphics -kjerq $(CFILES) | $(DCAN)

lint:
	lint -h -c -u -n -I/usr/jerq/include $(CFILES)

clean:
	rm -f *.o tst

pot.o: pot.pri /usr/jerq/include/CC/jerq.h
rule.o: pot.pri /usr/jerq/include/CC/jerq.h
bmap.o: pot.pri /usr/jerq/include/CC/jerq.h
globe.o: pot.pri /usr/jerq/include/CC/jerq.h
moved.o: /usr/jerq/include/CC/jerq.h /usr/jerq/include/CC/jerqproc.h
elarc.o: /usr/jerq/include/CC/jerq.h
tst.o: pot.pub up.icon down.icon b1.icon b2.icon b3.icon b4.icon /usr/jerq/include/CC/jerq.h
proto.o: pot.pub up.icon down.icon down.icon up.icon b1.icon b2.icon b3.icon b4.icon /usr/jerq/include/CC/jerq.h

.PRECIOUS:	$L

