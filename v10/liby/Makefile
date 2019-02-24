CFLAGS = -O

OBJ = libmai.o libzer.o

liby.a: $(OBJ)
	rm -f liby.a
	ar qc liby.a $(OBJ)

install: liby.a
	cp liby.a /usr/lib/liby.a
	ranlib /usr/lib/liby.a

clean:
	rm -f *.[ao]
