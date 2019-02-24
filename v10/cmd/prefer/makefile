# Directories that need to exist:
#	PLIB
#	PBIN
#	PHOME (usually the same as PLIB)
#	PLIB/styles

PLIB = /usr/lib/prefer
PBIN = /usr/bin
PHOME = /usr/lib/prefer

#PLIB = /usr/mad/lib/prefer
#PBIN = /usr/mad/bin
#PHOME = /usr/mad/lib/prefer

CFLAGS = -O
#CFLAGS = -O -DSVR2
#CFLAGS = -O -DEXPTOOLS
#CFLAGS = -O -DBSD

LDFLAGS = -s
#LDFLAGS = -s ../libux3/libux3.a


all:	library prefer miscellaneous refstyles awk

prefer:
	cd pref; make PBIN=$(PBIN) PLIB=$(PLIB) PHOME=$(PHOME) CFLAGS="$(CFLAGS)" LDFLAGS="$(LDFLAGS)"

awk:
	cd prefawk; make PLIB=$(PLIB) CFLAGS="$(CFLAGS)"

miscellaneous:
	cd misc; make PBIN=$(PBIN) PLIB=$(PLIB) PHOME=$(PHOME) CFLAGS="$(CFLAGS)"

refstyles:
	cd styles; make PLIB=$(PLIB)

library:
#	cd libux3; make CFLAGS="$(CFLAGS)"

clean:
#	cd libux3; make CFLAGS="$(CFLAGS)" clean
	cd pref; make PBIN=$(PBIN) PLIB=$(PLIB) PHOME=$(PHOME) clean
	cd prefawk; make PLIB=$(PLIB) clean
	cd misc; make PBIN=$(PBIN) PLIB=$(PLIB) PHOME=$(PHOME) clean
	cd styles; make PLIB=$(PLIB) clean

clobber:
#	cd libux3; CFLAGS="$(CFLAGS)" clobber
	cd pref; make PBIN=$(PBIN) PLIB=$(PLIB) PHOME=$(PHOME) clobber
	cd prefawk; make PLIB=$(PLIB) clobber
	cd misc; make PBIN=$(PBIN) PLIB=$(PLIB) PHOME=$(PHOME) clobber
	cd styles; make PLIB=$(PLIB) clobber


install: all
	cd pref; make PBIN=$(PBIN) PLIB=$(PLIB) PHOME=$(PHOME) CFLAGS="$(CFLAGS)" LDFLAGS="$(LDFLAGS)" install
	cd prefawk; make PLIB=$(PLIB) CFLAGS="$(CFLAGS)" install
	cd misc; make PBIN=$(PBIN) PLIB=$(PLIB) CFLAGS="$(CFLAGS)" PHOME=$(PHOME) install
	cd styles; make PLIB=$(PLIB) install

print:
	cd pref; make print
	cd misc; make print
	cd styles; make print
