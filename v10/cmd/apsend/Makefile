apsend.mkhd: apsend.mkhd.c apsend.h
	cc -O apsend.mkhd.c -o apsend.mkhd

install: apsend apsend.mkhd /usr/lib/apsend
	cp apsend /usr/lib/apsend
	strip apsend.mkhd
	mv apsend.mkhd /usr/lib/apsend
	echo "exec /usr/lib/apsend/apsend" >/usr/bin/apsend
	chmod +x /usr/bin/apsend

/usr/lib/apsend:
	mkdir /usr/lib/apsend
