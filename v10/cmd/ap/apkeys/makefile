CFLAGS=-g
all:	distribute flist fuser install
keydo: apsel.o y.tab.o par.o shell.o suft.o
	cc -g apsel.o y.tab.o par.o shell.o suft.o -o keydo
par.o: key..c par.c
y.tab.o: y.tab.c
y.tab.c: lex.yy.c kpar.y
	yacc kpar.y
lex.yy.c: dlex.l
	lex dlex.l
distribute: dist.o dread.o plex.o suft.o
	cc -g dist.o dread.o plex.o suft.o -o distribute
dread.o: dread.c key..c
flist: flist.o
	cc -g flist.o -o flist
fuser: fuser.o
	cc -g fuser.o -o fuser
window: wind1.o wind2.o wind3.o keys.o mkey3.o getargs.o
	cc -g wind1.o wind2.o wind3.o keys.o mkey3.o getargs.o -o window
backup:
	uucp *.c *.y *.l apselect /usr/ap/progs/*.c  research!~/mel
apnews: apnews1.o apnews2.o apnews3.o apnews4.o suft.o getargs.o tpos.o
	cc -g apnews1.o apnews2.o apnews3.o apnews4.o suft.o getargs.o \
	 tpos.o -lcbt -ltermlib -o apnews
install: distribute flist fuser
	cp distribute /usr/spool/ap
	cp flist /usr/spool/ap
	cp fuser /usr/spool/ap
