HFILES=device.h hped.h 
CFILES=main.c plot.c sub.c rot.c hread.c fspline.c
FILES=main.o sub.o plot.o rot.o hread.o fspline.o
LFILES=hrange.o hsub.o hwide.o hblit.o hspline.o
tped: $(HFILES) $(FILES) $(LFILES) 
	cc -g $(FILES) $(LFILES) -lm -o tped

install:
	strip tped
	cp tped /usr/bin
hped.h:
	ln ../hped/hped.h
clean:
	rm -f tped *.o
LFILES: ../hped/hrange.c ../hped/hsub.c ../hped/hblit.c ../hped/hspline.c
