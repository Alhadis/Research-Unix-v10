MESG	= SHORT
OPTS	= SWEET
SWAP	= LRU
CFLAGS	= -O -D$(MESG) -D$(OPTS) -D$(SWAP)
#
OBJECTS	=  allot.o change.o chstack.o connect.o copy.o destroy.o \
	disconnect.o generate.o hist.o house.o redeem.o stats.o \
	sync.o unuse.o use.o vacate.o pop_id.o push_id.o \
	fault.o rdSPACE.o readpage.o search.o switch.o writeframe.o \
	wtSPACE.o audit.o globals.o sdplib.o fatal.o
libsdp.a:	$(OBJECTS)
		ar r libsdp.a $(OBJECTS)
		ranlib libsdp.a

$(OBJECTS):	sdp.h define3.h
chstack.o copy.o destroy.o globals.o rdSPACE.o sdplib.o:	define2.h

install:	libsdp.a

clean:
	rm -f $(OBJECTS)

clobber:	clean
	rm -f libsdp.a
