# Regular make options:
#	nroff	make nroff
#	suftab	make suftab
#	clean	remove temporary object files
#	clobber	remove all object files
#	install	install nroff
#


CFLAGS = -O -DNROFF
NTSOURCE = hytab.c n1.c n10.c n[2-9i].c nii.c ntab.c strs.hd t10.c \
	 t6.c tab3.c tdef.hd tw.hd uns.hd def.nr.names def.rq.names
SMSOURCE = suftabmake.c suftab.c
SOURCE = ${NTSOURCE} ${SMSOURCE}
NFILES = n1.o n2.o n3.o n4.o n5.o n6.o n7.o n8.o n9.o n10.o ni.o \
		nii.o ntab.o hytab.o
TFILES = n1.o n2.o n3.o n4.o n5.o t6.o n7.o n8.o n9.o t10.o ni.o \
		nii.o tab3.o hytab.o

all:	suftab nroff
	cd terms.d; make all
	cd macros.d; make all

nroff:	$(NFILES)
	${CC} $(CFLAGS) -o nroff $(NFILES)

install:	all
	strip nroff
	mv nroff /usr/bin
	mv suftab /usr/lib
	cd terms.d; make install
	cd macros.d; make install

n[18].o::	tdef.hd strs.hd tw.hd uns.hd
	
n[2-79].o n10.o nii.o::	tdef.hd strs.hd tw.hd
	
t6.o t10.o::	tdef.hd strs.hd
	
ni.o::	tdef.hd nrtab.dt rqtab.dt
	
#	***** NOTE *****  the 300 in the next line is hardwired for (NN)
nrtab.dt: bldtab def.nr.names
	bldtab 300 def.nr.names > nrtab.dt

#	***** NOTE *****  the 350 in the next line is hardwired for (NM)
rqtab.dt:  bldtab def.rq.names
	bldtab 350 def.rq.names > rqtab.dt

suftab:	suftabmake.c suftab.c
	${CC} suftabmake.c
	a.out
	chmod 644 suftab

bldtab:	bldtab.c tdef.hd
	cc -s -O -o bldtab bldtab.c

clean:
	rm -f *.o a.out nroff suftab bldtab
	cd terms.d; make clean
	cd macros.d; make clean
