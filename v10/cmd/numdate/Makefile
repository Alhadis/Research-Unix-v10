CFLAGS = -Od2
OBJ = numdate.o getdate.o

numdate: $(OBJ)
	cc $(CFLAGS) -o numdate $(OBJ)

install: numdate
	strip numdate
	mv numdate /usr/bin

clean:
	rm -f $(OBJ)
