MHOME = /usr/lib/monk

LIBS = \
	prefs \
	tmac.p

all:

clean:

clobber: clean

install: all
	cp $(LIBS) $(MHOME)
	cp sample/* $(MHOME)/sample
	-pinvert -p $(MHOME)/prefs $(MHOME)/prefs
