all:	clean links map.cpio
	-


links:
	ln ../map.c ../map.h ../plot.h ../iplot.h ../index.c ../symbol.c .
	ln ../libmap.a .
	ln -s ../libmap .
	ln -s /usr/maps mapdata
	ln -s /usr/man/man5/map.5 .
	ln -s /usr/man/man3/proj.3 .
	ln -s /usr/man/man5/plot.5 .
	ln -s /usr/man/man7/map.7 .
	newer map.sh /usr/bin/map  

map.cpio: map.man README Makefile plot.h iplot.h map.h \
	map.c index.c symbol.c plotPS.c
map.cpio: map.sh libmap mapdata route.c
	ls README Makefile plot.h iplot.h map.h \
	map.c index.c symbol.c plotPS.c map.sh \
	libmap/Makefile \
	libmap/aitoff.c libmap/albers.c libmap/azequalarea.c \
	libmap/azequidist.c libmap/bicentric.c libmap/bonne.c \
	libmap/ccubrt.c libmap/complex.c libmap/conic.c \
	libmap/cubrt.c libmap/cuts.c libmap/cylequalarea.c \
	libmap/cylindrical.c libmap/elco2.c libmap/elliptic.c \
	libmap/fisheye.c libmap/gall.c \
	libmap/guyou.c libmap/harrison.c libmap/hex.c \
	libmap/homing.c libmap/lagrange.c libmap/lambert.c \
	libmap/laue.c libmap/mercator.c libmap/mollweide.c \
	libmap/newyorker.c \
	libmap/orthographic.c libmap/perspective.c \
	libmap/polyconic.c libmap/rectangular.c libmap/simpleconic.c \
	libmap/sinusoidal.c libmap/tetra.c libmap/trapezoidal.c \
	libmap/twocirc.c libmap/zcoord.c \
	map.man mapdata/world*  \
	mapdata/counties* mapdata/states* route.c \
	map.7 proj.3 map.5 plot.5 route.1 test.results | cpio -oc >map.cpio

map.man: map.7 proj.3 map.5 plot.5 route.1
	nroff -man map.7 proj.3 map.5 plot.5 route.1 >map.man

clean:
	rm -f map.man *plot.h index.c symbol.c proj.3 map.[ch57] plot.5 
	rm -f mapdata libmap libmap.a *.o map new.results nohup.out
