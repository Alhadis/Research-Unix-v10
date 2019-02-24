#
#	Blit Version (stand alone only)
#
#
#CFLAGS=-O -j
#CC=/usr/blit/bin/mcc
#I=/usr/blit/include
#CYFLAGS=
#
#*********************************************
#
#	5620 Version (mux)
#
#
#CFLAGS=-DJERQ
#CC=/usr/jerq/bin/3cc
#I=/usr/jerq/include
#CYFLAGS=-DJERQ -jm -Dlint
#LDFLAGS=-g
#
#*********************************************
#
#	5620 Version (stand alone)
#
#
CFLAGS=-J -DJERQ
CC=/usr/jerq/bin/3cc
I=/usr/jerq/include
CYFLAGS=-DJERQ
#
CFILES	= bitmap.c chars.c digits.c endgame.c event.c \
		events.c globals.c header.c init.c level.c \
		mouse.c pengo.c player.c \
		score.c screen.c shapes.c \
		sno_bee.c walrus.c water.c
CYFILES	= bitmap.O chars.O digits.O endgame.O event.O \
		events.O globals.O header.O init.O level.O \
		mouse.O pengo.O player.O \
		score.O screen.O shapes.O \
		sno_bee.O walrus.O water.O
OFILES	= bitmap.o chars.o digits.o endgame.o event.o \
		events.o globals.o header.o init.o level.o \
		mouse.o pengo.o player.o \
		score.o screen.o shapes.o \
		sno_bee.o walrus.o water.o

ICONS	= \
	icons/0diamond	icons/conf1	icons/pengo1	icons/sq_0left \
	icons/100	icons/conf12	icons/pengo2	icons/sq_0right \
	icons/1600	icons/conf2	icons/pengo3	icons/sq_0up \
	icons/3200	icons/dead0	icons/pengo4	icons/sq_1down \
	icons/400	icons/dead1	icons/pengo5	icons/sq_1left \
	icons/500	icons/egg	icons/pengo6	icons/sq_1right \
	icons/6400	icons/fblock	icons/pengo7	icons/sq_1up \
	icons/bee0	icons/hula0	icons/pengo8	icons/still \
	icons/bee1	icons/hula1	icons/playerl0	icons/video0 \
	icons/beede	icons/hula2	icons/playerl1	icons/wiggle0 \
	icons/blank	icons/hula3	icons/playerr0	icons/wiggle1 \
	icons/block	icons/hula4	icons/playerr1 \
	icons/chairl	icons/null	icons/salute \
	icons/chairr	icons/pback	icons/sq_0down

pengo: pengo.h $(OFILES)
	$(CC) $(CFLAGS) $(LDFLAGS) $(OFILES) -o pengo

install:	pengo
	cp pengo /usr/jerq/demo/mux

shapes.o:	screen.c $(ICONS)
		$(CC) $(CFLAGS) -c shapes.c

walrus.o:	walrus.c
		$(CC) $(CFLAGS) -g -c walrus.c
player.o:	player.c
		$(CC) $(CFLAGS) -g -c player.c
globals.o:	globals.c
		$(CC) $(CFLAGS) -g -c globals.c

%.O:	%.c
	cyntax -c -I$(I) $(CYFLAGS) $%.c

cyntax:	pengo.h $(CYFILES)
	cyntax -jm $(CYFILES) && touch cyntax

clean:
	rm -f $(OFILES) $(CYFILES) core jim.recover cyntax

print:
	@pr -n Makefile pengo.h $(CFILES) $(ICONS) 
