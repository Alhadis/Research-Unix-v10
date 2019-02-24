idfilt.o	:idfilt.c idfilt.h

4014.o	:4014.c idfilt.h

4filt	:idfilt.o 4014.o
	cc -o 4filt 4014.o idfilt.o -l4014 -lm

pout.o	:pout.c

pfilt	:idfilt.o pout.o
	cc -o pfilt pout.o idfilt.o -lplot -lm

tfilt	:idfilt.o tfilt.o
	cc -o tfilt tfilt.o idfilt.o

texfilt	:idfilt.o texfilt.o
	cc -o texfilt idfilt.o texfilt.o

cv.o	:cv.c idfilt.h

cvfilt	:idfilt.o cv.o
	cc -o cvfilt cv.o idfilt.o

install: 4filt tfilt pfilt texfilt
