ofiles=bitfile.o fill.o image.o main.o menu.o paintfunc.o state.o util.o
Ofiles=bitfile.O fill.O image.O main.O menu.O paintfunc.O state.O util.O
cfiles=bitfile.c fill.c image.c main.c menu.c paintfunc.c state.c util.c
paint.m:	$(ofiles)
	cyntax -j $(Ofiles) -ljj
	3cc -o paint.m $(ofiles) -ljj
$(ofiles):	paint.h
listing:
	pr paint.h $(cfiles)|can
clean:
	rm -f *.[oO] paint.m core
install:	paint.m
	cp paint.m /usr/jerq/mbin
%.o:	%.c
	cyntax -c -j -Dlint $%.c
	3cc -c $%.c
