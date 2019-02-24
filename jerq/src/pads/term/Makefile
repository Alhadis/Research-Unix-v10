CC32=/usr/jerq/bin/3cc -g -Dmac32  -I..
OFILES= paint.o buttons.o lib.o pad.o lineops.o cursor.o\
	master.o host.o protocol.o cache.o scripthit.o

pads.m:$(OFILES)
	$(CC32) $(OFILES)
	cprs a.out pads.m
	rm a.out

%.o:	%.c univ.h ../pads.pri
	$(CC32) -c $*.c

clean:
	rm -f *junk* core jim.recover

install: pads.m
	cp pads.m /usr/jerq/mbin
