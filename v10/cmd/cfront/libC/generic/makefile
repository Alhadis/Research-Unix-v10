CC = nCC
CFLAGS = -O

OBJ = ../obj/generic.o

all: $(OBJ)

../obj/%.o: %.c
	$(CC) $(CFLAGS) -c $%.c && mv $%.o ../obj
