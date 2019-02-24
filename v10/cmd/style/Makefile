#	make file for style
#
#	LIB is directory for storing object code
#		for 3 style programs
#	if defined SCATCH is for collecting data
#	if defined SNOM is for collecting nominalizations
#
LIB = /usr/lib/style
BIN = /usr/bin
CFLAGS = -Od2
SCATCH= -DSCATCH=\"/usr/llc/collect/scatch\"
SNOM= -DSNOM=\"/usr/llc/snom\"
style: style1 style2 style3
style3: prtlex.o pscan.o outp.o
	cc $(CFLAGS) prtlex.o pscan.o outp.o -ll -o style3
	strip style3
prtlex.c: part.l
	lex part.l
	mv lex.yy.c prtlex.c
prtlex.o: names.h prtlex.c
	cc $(CFLAGS) -c -O $(SCATCH) $(SNOM) prtlex.c
pscan.o: names.h conp.h
outp.o: names.h conp.h style.h 
	cc $(CFLAGS) -c -O outp.c
style1: wdslex.o
	cc $(CFLAGS) wdslex.o -ll -o style1
	strip style1
wdslex.o: wdslex.c nhash.c dict.c ydict.c names.h abbrev.c
	cc -O -c wdslex.c
wdslex.c: nwords.l
	lex nwords.l
	mv lex.yy.c wdslex.c
style2: endlex.o
	cc $(CFLAGS) endlex.o -ll -o style2
	strip style2
endlex.o: names.h endlex.c ehash.c edict.c
	cc -O -c endlex.c
endlex.c: end.l
	lex end.l
	mv lex.yy.c endlex.c
clean:
	rm -f prtlex.c prtlex.o pscan.o outp.o
	rm -f wdslex.c wdslex.o
	rm -f endlex.c endlex.o
install:
	mv  style1 $(LIB)/style1
	mv  style2 $(LIB)/style2
	mv  style3 $(LIB)/style3
	cp style.sh $(BIN)/style
	cp parts.sh $(BIN)/parts
	cp topic.sh $(BIN)/topic
	chmod 755 $(LIB)/style1
	chmod 755 $(LIB)/style2
	chmod 755 $(LIB)/style3
	chmod 755 $(BIN)/style
	chmod 755 $(BIN)/parts
	chmod 755 $(BIN)/topic
