BIN=/usr/dud/bin
CC=$(BIN)/3cc
AS=$(BIN)/3as
clean:
	rm -f *.o

fake:	fake.o
	mld -o fake -b 256 /usr/jerq/lib/l.o fake.o -lsys -lj -lc

bounce:	bounce.o
	mld -o bounce -b 256 /usr/jerq/lib/l.o bounce.o -lsys -lj -lc

elevator:	elevator.o
	mld -o elevator -b 256 /usr/jerq/lib/l.o elevator.o -lsys -lj -lc

ack:	ack.o
	mcc -o ack -b 256 ack.o

asteroids:	asteroids.o
	mcc -o asteroids -b 256 asteroids.o -lj

spheres: spheres.o norm.o
	mld -o spheres -b 256 /usr/jerq/lib/l.o spheres.o norm.o -lsys -lj -lc

win: win.o balloc.o
	mld -o win -b 256 /usr/jerq/lib/l.o win.o balloc.o -lsys -llayer -lj -lc

w: w.o newlayer.o
	mcc -o w -j w.o newlayer.o -llayer

balloc.o:	balloc.c
	mcc -S balloc.c
	sed '/divs.w/s/.*/	asr.l	\&1, %d0/' < balloc.s > goo
	mv goo balloc.s
	mcc -c balloc.s

bltdemo:	bltdemo.o
	mld -o bltdemo -b 256 /usr/jerq/lib/l.o bltdemo.o -lsys -lj -lc

disc:	disc.o
	mld -o disc -b 256 /usr/jerq/lib/l.o disc.o -lsys -lj -lc

moire:
	mcc -j moire.c -o moire


rae:
	mcc -j rae.c -o rae
