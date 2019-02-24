MCC=/usr/jerq/bin/3cc
CFLAGS=-g
PROG= regrid view2d vdata rks level contour halft antial viewtd slice
MO=jerq.o fill.o hide.o

all:	$(PROG) view2d.m libview2d.a

view2d:	view2d.o rd.o sys.o view2d.h
	$(CC) $(CFLAGS) -o view2d view2d.o rd.o sys.o
	cp view2d /n/swift/usr/bin
view2d.m:	$(MO)
	$(MCC) -g $(MO) -o view2d.m
jerq.o:	jerq.c
	$(MCC) -c -g jerq.c
fill.o:	fill.c
	$(MCC) -c -g fill.c
hide.o:	hide.c
	$(MCC) -c -g hide.c

regrid: regrid.o wr.o rd.o sys.o view2d.h num.a
	$(CC) -o regrid regrid.o wr.o rd.o sys.o num.a -lF77 -lI77
	cp regrid /n/swift/usr/bin
vdata:	vdata.o num.a sys.o wr.o
	$(CC) -o vdata $(CFLAGS) vdata.o num.a sys.o wr.o -lF77 -lI77
antial: antial.o wr.o rd.o sys.o view2d.h
	$(CC) -o antial antial.o wr.o rd.o sys.o
viewtd: viewtd.o rd.o sys.o view2d.h
	$(CC) -o viewtd viewtd.o rd.o sys.o
	push swift viewtd /usr/lib/view2d
slice: slice.o rd.o sys.o view2d.h
	$(CC) -o slice slice.o rd.o sys.o
	push swift slice /usr/lib/view2d
rks:	rks.c wr.o sys.o view2d.h
	$(CC) -o rks $(CFLAGS) rks.c wr.o sys.o

level: level.o rd.o cvmap.o cv.o sys.o view2d.h
	$(CC) -o level level.o rd.o cvmap.o cv.o sys.o
	cp level /n/swift/usr/lib/view2d/level
contour: contour.o co.o g2.o rd.o sys.o
	$(CC) -o contour contour.o co.o g2.o rd.o sys.o -lF77 -lI77
halft: halft.o rd.o sys.o
	$(CC) -o halft halft.o rd.o sys.o
	cp halft /usr/lib/view2d /n/swift/usr/lib/view2d
panther:
	$(CC) -o half half_pan.c
	push panther half /usr/guest/ehg/bin

libview2d.a: wr.o moviefil.o
	ar ruv libview2d.a wr.o moviefil.o
num.a:
	cd num;make

wr.o: wr.c view2d.h
rd.o: rd.c view2d.h
regrid.o: regrid.c view2d.h

lint:
	lint view2d.c
	lint -DMPX -I/usr/jerq/include jerq.c hide.c fill.c
pp:
	pp -tView2d Makefile view2d.c jerq.c fill.c hide.c vdata.c | dcan

install:	Install Installm libview2d.a
	cp libview2d.a /usr/lib
	ranlib /usr/lib/libview2d.a
Install:	view2d regrid vdata
	cp $? /usr/bin
	touch Install
Installm:	rks contour halft view2d.m howto
	cp $? /usr/lib/view2d
	touch Installm
ship:	install
	ship /usr/bin/regrid /usr/bin/view2d /usr/bin/vdata\
	/usr/lib/view2d/rks /usr/lib/view2d/halft /usr/lib/view2d/contour\
	/usr/lib/view2d/view2d.m /usr/lib/view2d/howto\
	/usr/lib/libview2d.a

clean:
	rm -f core *.o *.m $(PROG) libview2d.a wedge makemap num.a
