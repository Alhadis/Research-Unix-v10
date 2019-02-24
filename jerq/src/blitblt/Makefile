CC=3cc
CFLAGS=-g
HOSTCC=cc

all: blitblt blitblt.m

blitblt: blitblt.c
	$(HOSTCC) $(CFLAGS) blitblt.c -o blitblt

BBTERM=bbterm.o flash.o sendbitmap.o sendch.o \
	reshape.o buttons.o kbdstring.o mice.o
blitblt.m:  $(BBTERM)
	$(CC) $(CFLAGS) -o $@ $(BBTERM)

wsize: wsize.m
	@echo exec 32ld `pwd`/$@.m '"$$@"' >$@
	@chmod +x $@
WSIZE=wsize.o canon.o reshape.o
wsize.m:  $(WSIZE)
	$(CC) $(CFLAGS) -o $@ $(WSIZE)

wmove: wmove.m
	@echo exec 32ld `pwd`/$@.m '"$$@"' >$@
	@chmod +x $@
WMOVE=wmove.o canon.o reshape.o
wmove.m:  $(WMOVE)
	$(CC) $(CFLAGS) -o $@ $(WMOVE)

getbits: getbits.m
	@echo exec jx `pwd`/$@.m '<$$1' >$@
	@chmod +x $@
GETBITS=getbits.o canon.o reshape.o
getbits.m:  $(GETBITS)
	$(CC) $(CFLAGS) -o $@ $(GETBITS)

bcan.out: bcan.c
	$(HOSTCC) $(CFLAGS) -o $@ bcan.c

bdump: bdump.c
	$(HOSTCC) $(CFLAGS) -o $@ bdump.c

b202: b202.c
	$(HOSTCC) $(CFLAGS) -o $@ b202.c

clean:
	rm -f *.o *.m *.out bdump b202 blitblt

HPROG=/usr/jerq/bin/blitblt
TPROG=/usr/jerq/mbin/blitblt.m

install: blitblt blitblt.m
	cp blitblt	$(HPROG)
	-chmod a=rx,ug+w	$(HPROG)
	strip		$(HPROG)
	cp blitblt.m	$(TPROG)
	-chmod a=r,ug+w	$(TPROG)
	3strip -r	$(TPROG)
	touch install

ship: install
	ship $(HPROG) $(TPROG)
	touch ship
