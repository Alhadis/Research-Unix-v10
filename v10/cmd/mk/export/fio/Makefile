OBJ=fiofillbuf.o fioflush.o fiogetc.o fioinit.o fioprint.o fioputc.o\
	fiordline.o fioread.o fioseek.o fioundo.o fiowrite.o fiotie.o fioclose.o

fio.a:	$(OBJ)
	ar r fio.a $(OBJ)
	$(RANLIB) fio.a

cray:	fio.o

fio.o:	$(OBJ)
	cat $(OBJ) > fio.o
