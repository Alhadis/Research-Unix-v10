# for all OS --
OPT = -O
PROF = -p
PI = -g

PRODUCTION =  $(OPT)
DEBUG = $(PROF) $(PI)

# -- for UNIX 9th Ed --
CFLAGS = $(PRODUCTION)
LIBS = -lY3610 -lY
PORTLIB = -lport
LDFLAGS = 
F77LIBS = $(LIBS)
MLIB =
REGEX =
FTW =
METLIB = mlib.o
RASTLIB =
POSTLIB =
GRAPHICLIBS = 

# -- For Sun OS 4.x --
##GRAPHICLIBS = -lsuntool -lsunwindow -lpixrect
##MLIB = -lm
##LIBS = $(GRAPHICLIBS) $(MLIB) 
##PORTLIB = /usr/lib/libport3.a
##F77LIBS = -lF77 -lI77 -lc $(MLIB) $(GRAPHICLIBS)
##REGEX = regexec.o regcomp.o regerror.o
##FTW = myftw.o
##LDFLAGS = $(PROF) -fswitch
##CFLAGS = $(PROF) -fswitch -Bstatic
##METLIB = sunlib.o
##RASTLIB = rastlib.o
##POSTLIB = postlib.o 

all:	bcp
	touch bcp
clean:
	rm -rf *.[cho15] bcp bcp.cpio *.srcs makefile

# the next three targets are for use in 1127 software distribution on bowell

install: /usr/bin/bcp
	cp bcp.1 /usr/man/man1

/usr/bin/bcp:	bcp
	strip bcp
	cp bcp /usr/bin

ship:	/usr/bin/bcp
	ship /usr/bin/bcp

# If you want to use UNIX FILE_TREEs:
Path.o:  Path.c CPU.h myftw.h Path.h
	cc $(CFLAGS) -c Path.c
FTREE=Path.o

# If you DON'T want to use FILE_TREEs:
##FTREE=

bcp:	bcp.o CCITT.o rlelib.o riclib.o Coord.o piclib.o jslr.o Text.o $(FTREE) $(POSTLIB) $(RASTLIB)
	cc $(CFLAGS) bcp.o CCITT.o rlelib.o riclib.o Coord.o piclib.o jslr.o Text.o $(FTREE) $(POSTLIB) $(RASTLIB) $(MLIB) $(GRAPHICLIBS) -o bcp
bcp.o:	bcp.c CPU.h stdocr.h rle.h CCITT.h bitio.h pic.h Path.h Bitmap.h Text.h abort.h
	cc $(CFLAGS) $(MLIB) $(GRAPHICLIBS) -c bcp.c

stdocr.h:  boole.h limits.h Units.h ric.h fioi.h
	touch stdocr.h
CCITT.o: CCITT.c CPU.h stdocr.h rle.h Bitmap.h CCITT.h bitio.h
	cc $(CFLAGS) -c CCITT.c
rlelib.o:  rlelib.c CPU.h stdocr.h rle.h ric.h
	cc $(CFLAGS) -c rlelib.c
riclib.o:  riclib.c CPU.h stdocr.h
	cc $(CFLAGS) -c riclib.c
Coord.o:  Coord.c CPU.h boole.h Units.h Coord.h
	cc $(CFLAGS) -c Coord.c
Text.o:  Text.c font.h Bfeats.h stdocr.h
	cc $(CFLAGS) -c Text.c
piclib.o:  piclib.c CPU.h boole.h limits.h Units.h Coord.h pic.h
	cc $(CFLAGS) -c piclib.c
jslr.o:	jslr.c
	cc $(CFLAGS) -c jslr.c
rastlib.o:  rastlib.c
	cc $(CFLAGS) $(MLIB) $(GRAPHICLIBS) -c rastlib.c
postlib.o:  postlib.c
	cc $(CFLAGS) -c postlib.c
sunlib.o: sunlib.c
	cc $(CFLAGS) -c sunlib.c
myftw.o: myftw.c myftw.h
	cc $(CFLAGS) -c myftw.c
