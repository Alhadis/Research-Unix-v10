CC = cc
CC = lcc -N -I/usr/include/lcc -A

CFLAGS = -g

CFILES = n1.c n2.c n3.c n4.c n5.c t6.c n6.c n7.c n8.c n9.c t10.c n10.c t11.c ni.c hytab.c suftab.c
NCFILES = tab.37 tab.450 tab.lp
HFILES = tdef.h ext.h fns.h
TFILES = n1.o n2.o n3.o n4.o n5.o t6.o n6.o n7.o n8.o n9.o t10.o n10.o t11.o ni.o hytab.o suftab.o
DFILES = dev202/DESC dev202/R dev202/I dev202/B dev202/S dev202/CW

a.out:	$(TFILES)
	$(CC) -n $(CFLAGS) $(TFILES) 

$(TFILES):	tdef.h fns.h ext.h 

d202:	d202.o draw.o dev.h
	$(CC) $(CFLAGS) -O -o d202 d202.o draw.o -lm

ta:	ta.o draw.o dev.h
	$(CC) $(CFLAGS) -O -o ta ta.o draw.o -lm

bowell:	$(CFILES) $(HFILES) $(NCFILES) makefile ta.c d202.c draw.c FIXES README cvt unansi
	push bowell $? /usr/src/cmd/troff
	touch bowell

bundle:
	@bundle README FIXES cvt unansi $(HFILES) $(CFILES) makefile 

install:
	cp a.out /usr/bin/troff
	# ln /usr/bin/troff /usr/bin/nroff

clean:
	rm -f a.out  *.o core foo* temp*
