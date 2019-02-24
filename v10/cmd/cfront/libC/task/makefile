CC = nCC
CFLAGS = -O

OBJ = ../obj/erand.o ../obj/event.o ../obj/fudge.o ../obj/hw_stack.o \
	../obj/obj.o ../obj/qhead.o ../obj/qtail.o ../obj/sched.o \
	../obj/sim.o ../obj/swap_supp.o ../obj/task.o ../obj/timer.o

all: $(OBJ) ../obj/swap.o

../obj/%.o: %.c task.h
	$(CC) $(CFLAGS) -c $%.c && mv $%.o ../obj

../obj/swap.o: swap.s
	as -o ../obj/swap.o swap.s
