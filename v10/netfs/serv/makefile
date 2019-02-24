CFLAGS=-O -I../libnetb
L=../libnetb/libnetb.a

all: zarf

ZFILES=zarf.o except.o libdir.o
zarf: $(ZFILES) $L
	cc -o zarf $(ZFILES) $L
zarf.o resdir.o bsdlibdir.o libdir.o: zarf.h
f6: f6.o
	cc -o f6 f6.o $L
f512: f512.o
	cc -o f512 f512.o $L
f7: f7.o
	cc -o f7 f7.o $L
d7: d7.o
	cc -o d7 d7.o $L
ffs: ffs.o
	cc -o ffs ffs.o $L
ffs.o: bsdffs.h
F11FILES=f11.o f11sub.o r50.o
f11: $(F11FILES)
	cc -o f11 $(F11FILES) $L
f11.o f11sub.o: f11.h files11.h
