OBJ=pow10.o

math.a:	$(OBJ)
	ar r math.a $(OBJ)
	$(RANLIB) math.a

cray:	math.o

math.o:	$(OBJ)
	cat $(OBJ) > math.o
