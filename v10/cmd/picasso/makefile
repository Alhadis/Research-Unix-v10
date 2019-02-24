GWB	= /usr
OL	= $(GWB)/
CFLAGS	= -g
LDFLAGS	= 
YFLAGS	= -d
YACC	= yacc
TFLAGS	=
SPOOL	= i10send
PRT	= birdie
OFILES	= picassol.o \
	  ps_include.o \
	  print.o \
	  misc.o \
	  symtab.o \
	  blockgen.o \
	  boxgen.o \
	  circgen.o \
	  arcgen.o \
	  linegen.o \
	  movegen.o \
	  textgen.o \
	  troffgen.o \
	  picgen.o \
	  attrs.o \
	  xform.o \
	  objcopy.o \
	  fonts.o \
	  input.o \
	  for.o \
	  plps.o \
	  math.o \
	  font.o \
	  tmpfile.o \
	  xstubs.o
FILES	= main.o picassoy.o $(OFILES)
CFILES	= ps_include.c \
	  main.c \
	  print.c \
	  misc.c \
	  symtab.c \
	  blockgen.c \
	  boxgen.c \
	  circgen.c \
	  arcgen.c \
	  linegen.c \
	  movegen.c \
	  textgen.c \
	  troffgen.c \
	  picgen.c \
	  attrs.c \
	  xform.c \
	  objcopy.c \
	  fonts.c \
	  input.c \
	  for.c \
	  plps.c \
	  math.c \
	  font.c \
	  tmpfile.c \
	  xstubs.c
SRCFILES= picassoy.y picassol.l picasso.h font.h ps_include.h $(CFILES)
INS	= :
INSDIR	= $(OL)bin
MANDIR	= $(OL)man/u_man/man1
POSTLIB	= $(OL)lib/postscript
FONTDIR	= $(OL)lib/font

all:	picasso

notice:	;	@echo "\n\n\n++  MAKING  picasso \n++"

yacc:	y1.o y2.o y3.o y4.o
	$(CC) -o yacc y[1-4].o

picasso:	$(FILES)
	$(CC) -o picasso $(CFLAGS) $(FILES) -lm $(LDFLAGS)
	echo "++  picasso  CREATED"
	if [ "$(INS)" != ":" ]; then \
		$(INS) picasso $(INSDIR); \
		$(INS) picasso.ps $(POSTLIB); \
		$(INS) fonts.ps $(POSTLIB); \
		cd $(INSDIR); chmod 755 picasso ; \
		cd $(POSTLIB); chmod 744 fonts.ps picasso.ps; \
		echo "++  picasso  INSTALLED" ; \
	fi

main.o:		main.c picasso.h prevy.tab.h
	$(CC) -c $(CFLAGS) -DGWBFILES=\"$(POSTLIB)\" -DFONTDIR=\"$(FONTDIR)\" main.c

ps_include.o:	ps_include.h

$(OFILES):	picasso.h prevy.tab.h

y.tab.h:	picassoy.o

prevy.tab.h:	y.tab.h
	-cmp -s y.tab.h prevy.tab.h || cp y.tab.h prevy.tab.h

manpg:
	if [ "$(INS)" != ":" ]; then \
		$(INS) picasso.1 $(MANDIR); cd $(MANDIR); \
		chmod +r picasso.1; chown bin picasso.1; chgrp bin picasso.1; \
	fi

doc:	guide appx

guide:	guide.pic wine.harvest
	picasso -t guide.pic | troff -mm -mpictures -Tpost -rN1 $(TFLAGS) \
			     | dpost | $(SPOOL) -d$(PRT) -lpost

appx:	appa appb appc appd

appa:	guide.appa.mm
	troff -mm -mpictures -Tpost $(TFLAGS) $? | $(SPOOL) -d$(PRT) -ltroff

appb:	guide.appb.mm
	troff -mm -mpictures -Tpost $(TFLAGS) $? | $(SPOOL) -d$(PRT) -ltroff

appc:	guide.appc.mm
	troff -mm -mpictures -Tpost $(TFLAGS) $? | $(SPOOL) -d$(PRT) -ltroff

appd:	guide.appd.pic
	picasso -t $? | troff -mm -mpictures -Tpost $(TFLAGS) \
		      | dpost | $(SPOOL) -d$(PRT) -lpost

install:
	$(MAKE) INS=cp ROOT=$(ROOT) all

clean:
	rm -f *.o *y.tab.h

clip:
	rm -f picasso

clobber:	clean clip
