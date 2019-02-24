LIB=/usr/jerq/lib

all: foo

rom.o: rom.s
	3cc -J -c rom.s
l.o: l.s
	3cc -J -c l.s
jpcbs.o: jpcbs.c
	3cc -J -S jpcbs.c
	mv jpcbs.s jx.s
	sed -f :sed.comm-bss jx.s > jpcbs.s
	3cc -J -c jpcbs.s
	rm jx.s jpcbs.s
foo.o: foo.c
	3cc -c -J foo.c
foo: foo.o l.o jpcbs.o rom.o
	/usr/jerq/bin/3ld -m -o foo -L${LIB} jxmap foo.o rom.o l.o -llayer -lj -lsys -lj -lc
