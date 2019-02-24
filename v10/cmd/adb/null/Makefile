FILES= access.o command.o expr.o format.o input.o main.o output.o pcs.o print.o runpcs.o sym.o setup.o regs.o mchpr.o mchframe.o mchsys.o mchio.o trcrun.o mchinit.o das.o syminit.o
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
setup.o: ../null/setup.c ../comm/defs.h 
	$(CC) $(CFLAGS) -c -I../null ../null/setup.c
regs.o: ../null/regs.c ../comm/defs.h 
	$(CC) $(CFLAGS) -c -I../null ../null/regs.c
mchpr.o: ../null/mchpr.c 
	$(CC) $(CFLAGS) -c -I../null ../null/mchpr.c
mchframe.o: ../null/mchframe.c 
	$(CC) $(CFLAGS) -c -I../null ../null/mchframe.c
mchsys.o: ../null/mchsys.c ../comm/defs.h 
	$(CC) $(CFLAGS) -c -I../null ../null/mchsys.c
mchio.o: ../null/mchio.c 
	$(CC) $(CFLAGS) -c -I../null ../null/mchio.c
trcrun.o: ../null/trcrun.c 
	$(CC) $(CFLAGS) -c -I../null ../null/trcrun.c
mchinit.o: ../null/mchinit.c 
	$(CC) $(CFLAGS) -c -I../null ../null/mchinit.c
das.o: ../null/das.c 
	$(CC) $(CFLAGS) -c -I../null ../null/das.c
syminit.o: ../null/syminit.c 
	$(CC) $(CFLAGS) -c -I../null ../null/syminit.c
