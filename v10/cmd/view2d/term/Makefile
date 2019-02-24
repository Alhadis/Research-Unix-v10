OBJ=main.o movie.o frame.o camera.o clock.o
CFILES=main.c camera.c clock.c frame.c movie.c
CC=3CC
CFLAGS=-g
PRI=frame.pri camera.pri clock.pri
HFILES=camera.h clock.h frame.h

%.pri:	%.h
	@cleave -I/usr/jerq/include $*
	@touch $*.pri

view2d.m:	$(PRI) $(OBJ)
	$(CC) $(CFLAGS) -o view2d.m $(OBJ) -lpot -ljj
	@3size view2d.m

3d.o:	3d.c
	3cc -c -g 3d.c

install:	view2d.m
	cp view2d.m /usr/lib/view2d/view2d.m

pp:
	pp -fPO -t"View2d: Terminal and Host" Makefile ../view2d.h ../view2d.c $(HFILES) $(CFILES) | dcan

clean:
	rm -f *.o view2d.m *.pri *.pub


# object dependencies!!
depend:
	(echo '/^#dstart/+1,$$c'; cdepend $(OBJ) $(PRI); echo '.'; echo 'w'; echo 'q')|ed - Makefile

#dstart
main.o: frame.pri clock.pub /usr/jerq/include/jerq.h
movie.o: frame.pri camera.pub /usr/jerq/include/jerq.h
frame.o: frame.pri camera.pub /usr/jerq/include/jerq.h
camera.o: frame.pri camera.pri clock.pub up.icon down.icon up.icon down.icon b1.icon b2.icon b3.icon b4.icon /usr/jerq/include/jerq.h
clock.o: frame.pri clock.pri /usr/jerq/include/jerq.h
frame.pri: /usr/jerq/include/pot.pub /usr/jerq/include/jerq.h
camera.pri: 3d.h /usr/jerq/include/pot.pub /usr/jerq/include/jerq.h
clock.pri: /usr/jerq/include/pot.pub /usr/jerq/include/jerq.h
