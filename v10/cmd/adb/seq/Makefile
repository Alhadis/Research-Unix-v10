FILES= access.o command.o expr.o format.o input.o main.o output.o pcs.o print.o runpcs.o sym.o mchio.o trace.o trcrun.o das.o mchframe.o mchinit.o mchrun.o mchpr.o mchsys.o module.o regs.o setup.o syminit.o
CFLAGS=-I. -I../comm

adb: $(FILES)
	$(CC) -o adb $(FILES)

clean:
	rm adb $(FILES)

access.o: ../comm/access.c ../comm/defs.h 
	$(CC) $(CFLAGS) -c ../comm/access.c
command.o: ../comm/command.c ../comm/defs.h 
	$(CC) $(CFLAGS) -c ../comm/command.c
expr.o: ../comm/expr.c ../comm/defs.h 
	$(CC) $(CFLAGS) -c ../comm/expr.c
format.o: ../comm/format.c ../comm/defs.h 
	$(CC) $(CFLAGS) -c ../comm/format.c
input.o: ../comm/input.c ../comm/defs.h 
	$(CC) $(CFLAGS) -c ../comm/input.c
main.o: ../comm/main.c ../comm/defs.h 
	$(CC) $(CFLAGS) -c ../comm/main.c
output.o: ../comm/output.c ../comm/defs.h 
	$(CC) $(CFLAGS) -c ../comm/output.c
pcs.o: ../comm/pcs.c ../comm/defs.h 
	$(CC) $(CFLAGS) -c ../comm/pcs.c
print.o: ../comm/print.c ../comm/defs.h 
	$(CC) $(CFLAGS) -c ../comm/print.c
runpcs.o: ../comm/runpcs.c ../comm/defs.h 
	$(CC) $(CFLAGS) -c ../comm/runpcs.c
sym.o: ../comm/sym.c ../comm/defs.h 
	$(CC) $(CFLAGS) -c ../comm/sym.c
mchio.o: ../null/mchio.c 
	$(CC) $(CFLAGS) -c -I../null ../null/mchio.c
trace.o: ../v7/trace.c ../comm/defs.h 
	$(CC) $(CFLAGS) -c -I../v7 ../v7/trace.c
trcrun.o: ../v7/trcrun.c ../comm/defs.h 
	$(CC) $(CFLAGS) -c -I../v7 ../v7/trcrun.c
das.o: ../seq/das.c ../comm/defs.h 
	$(CC) $(CFLAGS) -c ../seq/das.c
mchframe.o: ../seq/mchframe.c ../comm/defs.h 
	$(CC) $(CFLAGS) -c ../seq/mchframe.c
mchinit.o: ../seq/mchinit.c ../comm/defs.h 
	$(CC) $(CFLAGS) -c ../seq/mchinit.c
mchrun.o: ../seq/mchrun.c ../comm/defs.h 
	$(CC) $(CFLAGS) -c ../seq/mchrun.c
mchpr.o: ../seq/mchpr.c ../comm/defs.h 
	$(CC) $(CFLAGS) -c ../seq/mchpr.c
mchsys.o: ../seq/mchsys.c ../comm/defs.h 
	$(CC) $(CFLAGS) -c ../seq/mchsys.c
module.o: ../seq/module.c ../comm/defs.h 
	$(CC) $(CFLAGS) -c ../seq/module.c
regs.o: ../seq/regs.c ../comm/defs.h 
	$(CC) $(CFLAGS) -c ../seq/regs.c
setup.o: ../seq/setup.c ../comm/defs.h 
	$(CC) $(CFLAGS) -c ../seq/setup.c
syminit.o: ../seq/syminit.c ../comm/defs.h 
	$(CC) $(CFLAGS) -c ../seq/syminit.c
