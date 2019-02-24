CC = nCC
CFLAGS = -O

OBJ = ../obj/_ctor.o ../obj/_dtor.o ../obj/_main.o \
	../obj/dtors.o ../obj/open.o ../obj/pure.o ../obj/Bits.o ../obj/Pool.o

all: $(OBJ)

../obj/%.o: %.c
	$(CC) $(CFLAGS) -c $%.c && mv $%.o ../obj
