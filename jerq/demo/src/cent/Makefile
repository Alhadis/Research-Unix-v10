#
#	'Makefile' for CENTIPEDE by Pat Autilio	11/3/83
#	DMD 5620 mods: Jim Reeds, 2 Aug 1984
#
#
CFLAGS=
CC=/usr/jerq/bin/3cc
CFILES = 	digits.c globals.c player.c\
		chk_coll.c chk_mecoll.c error.c exit_game.c\
		init_game.c init_round.c killplayer.c personchoose.c\
		playdisp.c put_chunk.c init_scr.c scorpchoose.c\
		shotchoose.c slugchoose.c spiderchoose.c start_stuff.c\
		wormchoose.c
OFILES = 	digits.o globals.o player.o\
		chk_coll.o chk_mecoll.o error.o exit_game.o\
		init_game.o init_round.o killplayer.o personchoose.o\
		playdisp.o put_chunk.o init_scr.o scorpchoose.o\
		shotchoose.o slugchoose.o spiderchoose.o start_stuff.o\
		wormchoose.o
#
# 'make' is all you have to make for this to work.
#
cent.m: playdefs.h $(OFILES)
	$(CC) $(LDFLAGS) -o cent.m $(OFILES)
lint:
	lint -I/usr/jerq/include $(CFILES)
#
# 'make print' will print all source on the local line printer.
#
pp:
	pp -fR Makefile playdefs.h $(CFILES) | 8dcan
print:
	pr -n Makefile playdefs.h $(CFILES) | opr -fstandard
#
# 'make xprint' will print all source on a xerox printer if
#	available.
#
xprint:
	pr -n Makefile playdefs.h $(CFILES) | opr -txr -pland -fhole

shrink:
	-rm -f *.o
#
# 'make cpiofile' packs the source up for sending it somewhere.
#
cpiofile:
	ls cent.README Makefile playdefs.h *.c | cpio -oc > cent.cpio
#
# End of 'Makefile'
