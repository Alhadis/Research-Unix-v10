ASRC = stubs.awk protocol 
HDR = commands.h io.h jerq.h balloc.h 
CSRC = stubs.c io.c balloc.c font.c menu.c spline.c track.c
JSRC = atan2.c arith.c cosdeg.c norm.c rect.c 
SRC = $(CSRC) $(ASRC)
OBJ = io.o stubs.o balloc.o atan2.o arith.o cosdeg.o norm.o font.o rect.o menu.o \
	spline.o track.o

all: libj.a 

libj.a: $(OBJ)
	ar r libj.a $(OBJ)
	ranlib libj.a

stubs.o: stubs.c commands.h io.h jerq.h
	cc -c -g stubs.c

arith.o: arith.c jerq.h
	cc -c -g arith.c 

spline.o: commands.h jerq.h io.h spline.c
	cc -c -g spline.c

track.o: commands.h jerq.h io.h track.c
	cc -c -g track.c

rect.o: rect.c jerq.h
	cc -c -g rect.c 

font.o: font.c jerq.h commands.h io.h
	cc -c -g font.c 

menu.o: menu.c jerq.h commands.h io.h
	cc -c -g menu.c 

commands.h stubs.c: protocol stubs.awk
	awk -f stubs.awk protocol 

io.o:	io.c $(HDR)
	cc -c -g io.c 

balloc.o: jerq.h balloc.h commands.h
	cc -c -g balloc.c 

install: libj.a commands.h jerq.h io.h
	cp libj.a /usr/lib
	ranlib /usr/lib/libj.a
	cp jerq.h /usr/include

clean: 
	rm $(OBJ) libj.a

print:
	pr $(HDR) $(CSRC) | can
