CC = nCC
CFLAGS = -O

OBJ = ../obj/event.o ../obj/fudge.o ../obj/obj.o ../obj/qhead.o \
	../obj/qtail.o ../obj/sched.o ../obj/sim.o ../obj/task.o \
	../obj/timer.o

all: $(OBJ) ../obj/swap.o

../obj/%.o: %.c
	$(CC) $(CFLAGS) -c $%.c && mv $%.o ../obj

../obj/swap.o: swap.s
	as -o ../obj/swap.o swap.s

../obj/event.o: event.c task.h
../obj/fudge.o: fudge.c task.h
../obj/obj.o: obj.c task.h
../obj/qhead.o: qhead.c task.h
../obj/qtail.o: qtail.c task.h
../obj/sched.o: sched.c task.h
../obj/sim.o: sim.c task.h
../obj/task.o: task.c task.h
../obj/timer.o: timer.c task.h
