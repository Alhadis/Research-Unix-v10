town: town.o
	cc town.o -lcbt -lm -o town
apmemory: apmem.o
	cc apmem.o -lcbt -o apmemory
mark: mark.o
	cc mark.o -lcbt -o mark
counties:
	rx r70 cat /crp/dict/counties >xx1
	/usr/mel/towns/dx <xx1 | /usr/mel/towns/patch | sort >counties
	rm xx1
	/usr/mel/towns/loc <counties >indcounties
	: the input has fields
	: 1 unknown
	: 2 unknown
	: 3 state A
	: 4 county A
	: 5 state B
	: 6 county B
	: 7 lat 1
	: 8 long 1
	: 9 lat 2
	: 10 long 2
drnp: drnp.o
	cc drnp.o /usr/theo/PLOT/libplot.a -lcbt -lm -o drnp
