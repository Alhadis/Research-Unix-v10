MHOME = /usr/lib/monk
MBIN = /usr/bin

MAKE = make

FRC =

all:	command library database

command:
	cd cmd; $(MAKE) MHOME=$(MHOME) MBIN=$(MBIN)

library:
	cd lib; $(MAKE) MHOME=$(MHOME)

database:
	cd db; $(MAKE) MHOME=../cmd/monk

clean:
	cd cmd; $(MAKE) MHOME=$(MHOME) MBIN=$(MBIN) clean
	cd lib; $(MAKE) MHOME=$(MHOME) clean
	cd db; $(MAKE) MHOME=$(MHOME) clean

clobber: clean
	cd cmd; $(MAKE) MHOME=$(MHOME) MBIN=$(MBIN) clobber
	cd lib; $(MAKE) MHOME=$(MHOME) clobber
	cd db; $(MAKE) MHOME=$(MHOME) clobber

install: all
	cd cmd; $(MAKE) MHOME=$(MHOME) MBIN=$(MBIN) install
	cd lib; $(MAKE) MHOME=$(MHOME) install
	cd db; $(MAKE) MHOME=$(MHOME) install
