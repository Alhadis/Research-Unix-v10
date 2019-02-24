CC=3cc
CFLAGS=

HEADERS = jf.h
JFSRC = main.c spaceman.c charsubs.c sizesubs.c fontsubs.c filesubs.c magnify.c
UTLSRC = buttons.c kbdstring.c strcat2.c brotate.c rsupport.c \
	menuhit3.c lexit3.c reshaped.c mice.c
JFOBJ = main.o spaceman.o charsubs.o sizesubs.o fontsubs.o filesubs.o magnify.o
UTLOBJ = buttons.o kbdstring.o strcat2.o brotate.o rsupport.o \
	menuhit3.o lexit3.o reshaped.o mice.o
PFOBJ = pfont.o buttons.o mice.o

jf: jf.m
	@echo exec jx `pwd`/jf.m '$$*' >jf
	@chmod +x jf
jf.m: & $(JFOBJ) $(UTLOBJ)
	$(CC) $(JFOBJ) $(UTLOBJ) -o jf.m

$(JFOBJ): $(HEADERS)

pfont: pfont.m
	echo exec jx `pwd`/pfont.m '$$*' >pfont
	chmod +x pfont
pfont.m: & $(PFOBJ)
	$(CC) $(PFOBJ) -o pfont.m

install: jf.m
	cp jf.m /usr/jerq/mbin
	3strip -r /usr/jerq/mbin/jf.m
	-chmod a=r,ug+w /usr/jerq/mbin/jf.m

clean:
	rm -f $(JFOBJ) $(UTLOBJ) pfont.o jf.m pfont.m core nohup.out jim.recover
