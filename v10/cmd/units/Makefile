units:		cdate.h
cdate.h:	units.y mkcdate
		./mkcdate >cdate.h
clean:
	rm -f *.o cdate.h mkcdate
install:
	strip units
	cp units /usr/bin
