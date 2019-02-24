CC=3cc
CFLAGS=-g
HOSTCC=cc

all: think thinkblt thinkblt.m

THINK=think.o namelink.o wrtc.o thinksort.o
think:& $(THINK)
	$(HOSTCC) $(CFLAGS) -o $@ $(THINK) -ldk
think.o: think.c
	$(HOSTCC) $(CFLAGS) -c think.c
wrtc.o: wrtc.c
	$(HOSTCC) $(CFLAGS) -c wrtc.c

THINKHOST=thinkhost.o namelink.o namept.o
thinkblt: & $(THINKHOST)
	$(HOSTCC) $(CFLAGS) -o $@ $(THINKHOST) 
thinkhost.o: thinkhost.c
	$(HOSTCC) $(CFLAGS) -c thinkhost.c
namept.o: namept.c
	$(HOSTCC) $(CFLAGS) -c namept.c
namelink.o: namelink.c
	$(HOSTCC) $(CFLAGS) -c namelink.c

THINKTERM=thinkterm.o thinkprint.o thinkmap.o thinkchar.o \
	thinkshare.o flash.o reshape.o buttons.o mice.o
thinkblt.m: & $(THINKTERM)
	$(CC) $(CFLAGS) $(THINKTERM) -o $@
thinkchar.o thinkprint.o thinkmap.o: think.h
thinkhost.o thinkterm.o: thinkblt.h

THINKCOL=thinkcol.o thinksort.o
thinkcol: & $(THINKCOL)
	$(HOSTCC) $(CFLAGS) -o $@ $(THINKCOL) -ldk
thinkcol.o: thinkcol.c
	$(HOSTCC) $(CFLAGS) -c thinkcol.c
thinksort.o: thinksort.c
	$(HOSTCC) $(CFLAGS) -c thinksort.c

clean:
	rm -f think thinkblt *.m *.o

HNAME=thinkblt
TNAME=thinkblt.m
HNAME2=think
HPROG=/usr/jerq/bin/$(HNAME)
TPROG=/usr/jerq/mbin/$(TNAME)
HPROG2=/usr/jerq/bin/$(HNAME2)

install: $(HNAME) $(TNAME) $(HNAME2)
	cp $(HNAME)	$(HPROG)
	-chmod a=rx,ug+w	$(HPROG)
	strip		$(HPROG)
	cp $(TNAME)	$(TPROG)
	3strip -r	$(TPROG)
	-chmod a=r,ug+w	$(TPROG)
	cp $(HNAME2)	$(HPROG2)
	-chmod a=rx,ug+w	$(HPROG2)
	strip		$(HPROG2)
	touch install

ship: install
	ship $(HPROG) $(TPROG) $(HPROG2)
	touch ship
