#	makefile for diction
#
#	LIB is directory for storing dprog and data
#	BIN is directory for commands
#	DICT is the data file
#	CATCH is for collecting data - if you don't want
#		data, remove the definition of CATCH
#	MACS is a file of dummy .PP .SH etc
#		definitions to avoid the overhead
#		of loading the whole macro packages
#		in rewrite
#		only used with -c flag
#
BIN = /usr/bin
LIB=/usr/lib/style
DICT= -DDICT=\"$(LIB)/dict.d\"
CATCH= -DCATCH=\"/usr/llc/collect/catch\"
MACS= -DMACS=\"$(LIB)/macs.tr\"
all: diction rewrite
diction: diction.c
	cc -O $(MACS) $(DICT) $(CATCH) diction.c -o dprog
	strip dprog
rewrite: rewrite.l
	lex rewrite.l
	cc -Od2 lex.yy.c -ll -o rewrt
	strip rewrt
install:
	mv dprog $(LIB)/dprog
	chmod 755 $(LIB)/dprog
	cp diction.sh $(BIN)/diction
	chmod 755 $(BIN)/diction
	cp dict.d $(LIB)/dict.d
	chmod 644 $(LIB)/dict.d
	cp suggest.sh $(BIN)/suggest
	chmod 755 $(BIN)/suggest
	cp suggest.d $(LIB)/suggest.d
	chmod 644 $(LIB)/suggest.d
	mv rewrt $(LIB)/rewrt
	chmod 755 $(LIB)/rewrt
	cp rewrite.sh $(BIN)/rewrite
	chmod 755 $(BIN)/rewrite
	cp macs.tr $(LIB)/macs.tr
	chmod 644 $(LIB)/macs.tr
clean:
	rm -f dprog rewrt
	rm -f lex.yy.c
