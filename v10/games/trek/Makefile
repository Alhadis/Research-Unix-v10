trek:	abandon.o attack.o checkcond.o compkl.o
trek:	computer.o damage.o destruct.o dock.o dump.o
trek:	ssradio.o events.o externs.o extra.o getcodi.o
trek:	getpar.o help.o impulse.o input.o initquad.o kill.o
trek:	klmove.o log.o lose.o lrscan.o main.o move.o
trek:	nova.o phaser.o play.o printf.o query.o ram.o ranf.o
trek:	report.o rest.o schedule.o score.o setup.o setwarp.o
trek:	shell.o shield.o snova.o srscan.o torped.o visual.o warp.o win.o
	cc -o trek -i *.o -lm

install:
	strip trek
	rm otrek
	mv /usr/games/trek otrek
	mv trek /usr/games/trek
	chmod 711 /usr/games/trek

.c.o:; cc -c -O $<

shrink:
	rm *.o
