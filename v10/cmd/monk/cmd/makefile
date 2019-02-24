MHOME = /usr/lib/monk
MBIN = /usr/bin

MAKE = make

LDFLAGS = -s
FRC =

CFLAGS = -O -I.
INSDIR = $(MHOME)

PROGS = \
	cite \
	cw \
	tc

all:	$(PROGS) monkprog monkmergeprog demonkprog indexprog

cite: cite.o
	$(CC) cite.o $(LDFLAGS) -o cite

cw: cw.o
	$(CC) cw.o $(LDFLAGS) -o cw

tc: tc.o
	$(CC) tc.o $(LDFLAGS) -o tc

monkprog:
	cd monk; $(MAKE) MHOME=$(MHOME)

monkmergeprog:
	cd monkmerge; $(MAKE) MBIN=$(MBIN)

demonkprog:
	cd demonk; $(MAKE) MHOME=$(MHOME)

indexprog:
	cd index; $(MAKE) MHOME=$(MHOME)

clean:
	-rm -f *.o
	cd monk; $(MAKE) MHOME=$(MHOME) clean
	cd monkmerge; $(MAKE) MBIN=$(MBIN) clean
	cd demonk; $(MAKE) MHOME=$(MHOME) clean
	cd index; $(MAKE) MHOME=$(MHOME) clean

clobber: clean
	-rm -f $(PROGS)
	cd monk; $(MAKE) MHOME=$(MHOME) clobber
	cd monkmerge; $(MAKE) MBIN=$(MBIN) clobber
	cd demonk; $(MAKE) MHOME=$(MHOME) clobber
	cd index; $(MAKE) MHOME=$(MHOME) clobber

install: all
	cp $(PROGS) $(MHOME)
	cd monk; $(MAKE) MHOME=$(MHOME) install
	cd monkmerge; $(MAKE) MBIN=$(MBIN) install
	cd demonk; $(MAKE) MHOME=$(MHOME) install
	cd index; $(MAKE) MHOME=$(MHOME) install
	cp monk.sh $(MBIN)/monk
	cp monksample.sh $(MBIN)/monksample
	cp monkspell.sh $(MBIN)/monkspell
	cp demonk.sh $(MBIN)/demonk

