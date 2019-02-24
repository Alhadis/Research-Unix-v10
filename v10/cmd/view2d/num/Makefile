FFLAGS=-O
OBJ=interp.o tl2fit.o l2fit.o\
	isamax.o saxpy.o scopy.o sdot.o sgefa.o sgesl.o snrm2.o\
	sqrdc.o sqrsl.o sscal.o sswap.o

num.a: $(OBJ)
	ar ruv ../num.a $?
	ranlib ../num.a
	rm $(OBJ)
