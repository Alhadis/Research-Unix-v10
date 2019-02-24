CC = nCC
CFLAGS = -O

OBJ = ../obj/bignum.o

all: $(OBJ)

../obj/%.o: %.c
	$(CC) $(CFLAGS) -c $%.c && mv $%.o ../obj

../obj.bignum.o: bignum.c bignum.h
