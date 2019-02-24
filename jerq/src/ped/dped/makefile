JCFILES=jped.h device.h jmain.c jcomm.c jplot.c jfill.c jresh.c jmke.c jdraw.c jsub.c jtext.c jmvc.c jscl.c   jmenu.c  jfont.c
JFILES=jmain.o jcomm.o jplot.o jfill.o jresh.o jmke.o jdraw.o jsub.o jtext.o jmvc.o jscl.o   jmenu.o  jfont.o
CC=/usr/jerq/bin/3cc
jped: $(JFILES)
	$(CC) $(JFILES) 
	mv a.out ped.m
$(JFILES): jped.h

install:
	cp ped.m /usr/jerq/mbin
clean:
	rm -f ped.m *.o
