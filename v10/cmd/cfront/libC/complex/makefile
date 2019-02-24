CC = nCC
CFLAGS = -O

OBJ = ../obj/abs.o ../obj/arg.o ../obj/cos.o ../obj/error.o \
	../obj/exp.o ../obj/io.o ../obj/log.o ../obj/oper.o \
	../obj/polar.o ../obj/pow.o ../obj/sin.o ../obj/sqrt.o

all: $(OBJ)

../obj/%.o: %.c
	$(CC) $(CFLAGS) -c $%.c && mv $%.o ../obj

../obj/abs.o: abs.c complex.h
../obj/arg.o: arg.c complex.h
../obj/cos.o: cos.c complex.h
../obj/error.o: error.c complex.h
../obj/exp.o: exp.c complex.h
../obj/io.o: io.c complex.h
../obj/log.o: log.c complex.h
../obj/oper.o: oper.c complex.h
../obj/polar.o: polar.c complex.h
../obj/pow.o: pow.c complex.h
../obj/sin.o: sin.c complex.h
../obj/sqrt.o: sqrt.c complex.h
