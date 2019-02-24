CFLAGS=-O -g
HCFILES=hped.h Unix.h device.h dmain.c hedit.c hcomm.c  hread.c hwrite.c hsub.c  hfnt.c  hwork.c hbmap.c hrange.c hblit.c help.c  hwide.c hspline.c fspline.c huser.c
HFILES=dmain.o hedit.o hcomm.o  hread.o hwrite.o hsub.o  hfnt.o  hwork.o hbmap.o hrange.o hblit.o help.o  hwide.o hspline.o fspline.o huser.o
BFILES=hmain.o hedit.o hcomm.o  hread.o hwrite.o hsub.o  hfnt.o  hwork.o hbmap.o hrange.o hblit.o help.o  hwide.o hspline.o fspline.o huser.o
BCFILES=beauty.h locus.h where.h dep.h hform.c aux.c solve.c dep.c hclust.c
BOFILES=hform.o aux.o solve.o dep.o hclust.o
ped: $(HFILES) $(BOFILES)
	cc $(HFILES) $(BOFILES) -lm -o ped
blit: $(BFILES)
	cc $(BFILES) $(BOFILES) -lm -o blitped
$(HFILES): hped.h
$(BOFILES): beauty.h locus.h where.h dep.h
hread.o fspline.o: device.h
dmain.o: Unix.h

install:
	strip ped
	cp ped /usr/jerq/bin
clean:
	rm -f ped *.o
