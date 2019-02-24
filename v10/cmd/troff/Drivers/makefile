CC = vcc
CFLAGS= -DINCORE -g
CFILES=n1.c n2.c n3.c n4.c n5.c t6.c n7.c n8.c n9.c t10.c ni.c nii.c hytab.c suftab.c
NCFILES = n6.c n10.c tab.37 tab.450
HFILES=tdef.h tw.h dev.h ext.h
TFILES=n1.o n2.o n3.o n4.o n5.o t6.o n7.o n8.o n9.o t10.o ni.o nii.o hytab.o suftab.o
NFILES=n1.o n2.o n3.o n4.o n5.o n6.o n7.o n8.o n9.o n10.o ni.o nii.o hytab.o suftab.o

a.out:	$(TFILES)
	$(CC) -n $(CFLAGS) $(TFILES) 

lcomp:	$(CFILES)
	lcomp $(CFILES)

nroff:
	make nroff1 CFLAGS="-DNROFF $(CFLAGS)" CC=vcc

nroff1:	$(NFILES)
	$(CC) -n -o na.out $(CFLAGS) $(NFILES)
	@echo output in na.out

n1.o:	tdef.h tw.h 
n2.o:	tdef.h tw.h 
n3.o:	tdef.h tw.h 
n5.o:	tdef.h tw.h 
n6.o:	tdef.h tw.h 
t6.o:	tdef.h tw.h dev.h
n7.o:	tdef.h tw.h 
n8.o:	tdef.h tw.h 
n9.o:	tdef.h tw.h 
n10.o:	tdef.h tw.h 
t10.o:	tdef.h tw.h dev.h
ni.o:	tdef.h tw.h 
nii.o:	tdef.h tw.h 

hytab.o:	hytab.c

suftab.o:	suftab.c

prcan:	$(CFILES) $(HFILES) $(NCFILES) README makefile makedev.c
	@prcan $?
	@touch prcan

lint:
	nlint -n $(CFILES)

dcat:	dcat.c dev.h
	cc -o dcat dcat.o draw.o -lm

d202:	d202.o draw.o dev.h
	cc -O -o d202 d202.o draw.o -lm

d8400:	d8400.o draw.o dev.h
	cc -O -o d8400 d8400.o draw.o -lm

dsort:	dsort.o dev.h
	cc -O -o dsort dsort.o

daps:	daps.o draw.o aps.h dev.h
	cc -o daps daps.o draw.o -lm

tc:	tc.o draw.o dev.h
	cc -O -o tc tc.o draw.o -lm

hc:	hc.o draw.o dev.h
	cc -O -o hc hc.o draw.o -lm

ta:	ta.o draw.o dev.h
	cc -O -o ta ta.o draw.o -lm

makedev:	makedev.c dev.h
	cc -o makedev makedev.c

dumpaps:	dumpaps.c
	cc dumpaps.c -o dumpaps

backup:	$(CFILES) $(HFILES) $(NCFILES) makefile
	push panther $? troff
	touch backup

bowell:	$(CFILES) $(HFILES) $(NCFILES) makefile makedev.c Drivers/ta.c Drivers/hc.c Drivers/tc.c
	push bowell $? /src/cmd/troff
	touch bowell

bundle:
	@bundle README $(HFILES) $(CFILES) $(NCFILES) makefile makedev.c Drivers/ta.c Drivers/hc.c Drivers/tc.c

bin:
	cp a.out /usr/bin/troff
