# docgen defaults to coversheet script mcsdata
# timestamp is used by tmac.cs to compute timestamp for itds
# cstrans is used by tmac.cs to package & ship coversheet
#	to itds database
all:	cstrans timestamp docgen
docgen: docgen.c
cstrans:	header.h trans.l getstamp.o
	lex trans.l
	cc lex.yy.c getstamp.o -ll -o cstrans
timestamp: timestamp.o getstamp.o
	cc timestamp.o getstamp.o -o timestamp
install: cstrans timestamp docgen
	cp docgen /usr/bin
	cp mcsdata /usr/lib/docgen
	cp cstrans /usr/lib/tmac
	cp timestamp /usr/lib/tmac
	cp name.sed /usr/lib/tmac
	cp sendcover.sh /usr/lib/tmac/sendcover
clean:
	rm cstrans lex.yy.c
