#	"@(#)makefile	1.3"	(1.9	12/28/82)
#
# There is currently no provision for local mods (reorder) or
# user mods ($TERMCAP).  These will be coming eventually.
#
# PARTS=	header \
# 	adds annarbor beehive cdc concept datamedia dec diablo general \
# 	hardcopy hazeltine heath homebrew hp ibm lsi microterm misc \
# 	pc perkinelmer print special \
# 	tektronix teleray teletype televideo ti visual \
# 	trailer

PARTS=	header *.ti trailer
COMPILE=../screen/tic

install: $(PARTS)
	$(COMPILE) $?
	make terminfo.src
	touch install

terminfo.src:	$(PARTS)
	cat $(PARTS) > terminfo.src

all:	terminfo.src
	$(COMPILE) -v terminfo.src
	touch install

clean:
	rm -f terminfo.src install

tabset:	/tmp
	-mkdir /usr/lib/tabset
	cp tabset/* /usr/lib/tabset

VGRIND=
vgrind:
	${VGRIND} -n -h "Terminal Information Data Base" $(PARTS)
