L=libj.a
INCL=/usr/jerq/include
CC=3cc
AS=/usr/jerq/bin/vax/m32as
AR=3ar
DCAN=dcan
CFLAGS=-J

OBJ=Jrect.o addr.o alloc.o arc.o arith.o atan2.o balloc.o bitblt.o \
	circle.o clip.o clockroutine.o comm.o cosdeg.o data.o defont.o disc.o discture.o \
	display.o elarc.o eldisc.o ellipse.o exit.o fgets.o font.o \
	fopen.o gcalloc.o get.o getfont.o introutine.o line.o linesub.o \
	masks.o menuhit.o min.o norm.o point.o popen.o printf.o ptinrect.o \
	put.o rdwr.o rectXrect.o rectclip.o rectf.o rol.o screenswap.o sqrt.o \
	string.o strwidth.o texture.o texture32.o wait.o
CFILES=Jrect.c addr.s alloc.c arc.c arith.c arith.s atan2.c balloc.c bitblt.c \
	circle.c clip.c clockroutine.c comm.c cosdeg.c data.c defont.c disc.c discture.c \
	display.c elarc.c eldisc.c ellipse.c exit.c fgets.c font.c \
	fopen.c gcalloc.c get.c getfont.c introutine.c line.c linesub.c \
	masks.c menuhit.c min.c norm.c point.c popen.c printf.c ptinrect.c \
	put.c rdwr.c rectXrect.c rectclip.c rectf.c rol.c screenswap.c sqrt.c \
	string.c strwidth.c texture.c texture32.s wait.c


$L:	$(OBJ)
	$(AR) uv $L $^


new:	$(OBJ)
		$(AR) uv $L `mlorder $(FILES) | tsort`
		$(AR) ma clip.o $L line.o
		rm $(FILES)

print:
	@pr $(CFILES)

pp:
	pp -tGraphics -kjerq $(CFILES) | $(DCAN)

lint:
	lint -h -c -u -n -I/usr/jerq/include $(CFILES)

$L(clip.o):	$(INCL)/line.h
$L(comm.o):	$(INCL)/jerqio.h
$L(data.o):	$(INCL)/jerqio.h
$L(defont.o):	$(INCL)/font.h
$L(exit.o):	$(INCL)/jerqio.h
$L(fgets.o):	$(INCL)/jerqio.h
$L(font.o):	$(INCL)/jerqio.h
$L(font.o):	$(INCL)/font.h
$L(fopen.o):	$(INCL)/jerqio.h
$L(get.o):	$(INCL)/jerqio.h
$L(getfont.o):	$(INCL)/font.h
$L(getfont.o):	$(INCL)/jerqio.h
$L(jlineto.o):	$(INCL)/line.h
$L(jstring.o):	$(INCL)/font.h
$L(line.o):	$(INCL)/line.h
$L(menuhit.o):	$(INCL)/font.h
$L(menuhit.o):	$(INCL)/jerq.h
$L(menuhit.o):	$(INCL)/layer.h
$L(menuhit.o):	$(INCL)/jerqproc.h
$L(popen.o):	$(INCL)/jerqio.h
$L(printf.o):	$(INCL)/jerqio.h
$L(put.o):	$(INCL)/jerqio.h
$L(rdwr.o):	$(INCL)/jerqio.h
$L(string.o):	$(INCL)/font.h
$L(strwidth.o):	$(INCL)/font.h
$L(wait.o):	$(INCL)/queue.h

.PRECIOUS:	$L

