# This makefile must be run as super-user

SPOOL=/usr/spool/at

all:	at atrun

at:	at.c
	cc -Od2 -o at at.c

atrun:	atrun.c
	cc  -o atrun atrun.c

install:	all
	- rm /bin/at /usr/lib/atrun
	strip at atrun
	mv at /bin
	/etc/chown root /bin/at
	chmod u+s,og-w /bin/at
	cp atrun /usr/lib

clean:
	rm -f at atrun

dirs:
	rm -fr $(SPOOL)
	mkdir $(SPOOL) $(SPOOL)/past
	/etc/chown root $(SPOOL) $(SPOOL)/past
	chmod og-w $(SPOOL) $(SPOOL)/past
	>$(SPOOL)/lasttimedone
	chmod og-w $(SPOOL)/lasttimedone
