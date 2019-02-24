CFLAGS=-gJDMUXTERM
CC=3cc
FILES=deltext.o frameop.o instext.o newframe.o ptofchar.o select.o string.o
CFILES=deltext.c frameop.c instext.c newframe.c ptofchar.c  select.c string.c

frame.a:	$(FILES)
	3ar uv frame.a $(FILES)

print:
	@pr frame.h $(CFILES)

lint:
	lint -I/usr/jerq/include $(CFILES) -lj

$(FILES):	frame.h

clean:
	rm -f *.o core
