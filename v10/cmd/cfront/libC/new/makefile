CC = nCC
CFLAGS = -O

OBJ = ../obj/_arr_map.o ../obj/_delete.o ../obj/_handler.o ../obj/_new.o ../obj/_vec.o ../obj/place.o

all: $(OBJ)

../obj/%.o: %.c
	$(CC) $(CFLAGS) -c $%.c && mv $%.o ../obj
