all:	3cc 32reloc 3nm

32reloc:	32reloc.c
	cc -o 32reloc -Iinc 32reloc.c
3nm:	3nm.c
	cc -o 3nm -Iinc 3nm.c
3cc:	3cc.c
	cc -o 3cc 3cc.c

install:	3cc
	cp 3cc /usr/jerq/bin
	strip /usr/jerq/bin/3cc
	cp 32reloc /usr/jerq/bin
	strip /usr/jerq/bin/32reloc
	cp 3nm /usr/jerq/bin
	strip /usr/jerq/bin/3nm
clean:
	rm -f core 3cc 32reloc
