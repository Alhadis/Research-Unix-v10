ALL = wdefine ldefine odefine colthlook colthread slanglook  \
	oedread namelook namelook1 nameread

all:	$(ALL)

install: $(ALL)
	strip $(ALL)
	cp $(ALL) /usr/lib/dict
	cp dict.sh /usr/bin/dict

clean:
	rm -f *.o $(ALL) lex.yy.c
