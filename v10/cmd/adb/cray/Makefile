FILES= access.o command.o expr.o format.o input.o main.o output.o pcs.o print.o runpcs.o sym.o mchsys.o mchio.o mchinit.o setup.o hksyminit.o regs.o mchpr.o mchframe.o das.o mchrun.o trcrun.o trace.o
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
mchsys.o: ../null/mchsys.c ../comm/defs.h 
	$(CC) $(CFLAGS) -c -I../null ../null/mchsys.c
mchio.o: ../null/mchio.c 
	$(CC) $(CFLAGS) -c -I../null ../null/mchio.c
mchinit.o: ../null/mchinit.c 
	$(CC) $(CFLAGS) -c -I../null ../null/mchinit.c
setup.o: ../cray/setup.c 
	$(CC) $(CFLAGS) -c -I../cray ../cray/setup.c
hksyminit.o: ../cray/hksyminit.c 
	$(CC) $(CFLAGS) -c -I../cray ../cray/hksyminit.c
regs.o: ../cray/regs.c 
	$(CC) $(CFLAGS) -c -I../cray ../cray/regs.c
mchpr.o: ../cray/mchpr.c 
	$(CC) $(CFLAGS) -c -I../cray ../cray/mchpr.c
mchframe.o: ../cray/mchframe.c 
	$(CC) $(CFLAGS) -c -I../cray ../cray/mchframe.c
das.o: ../cray/das.c 
	$(CC) $(CFLAGS) -c -I../cray ../cray/das.c
mchrun.o: ../cray/mchrun.c 
	$(CC) $(CFLAGS) -c -I../cray ../cray/mchrun.c
trcrun.o: ../cray/trcrun.c 
	$(CC) $(CFLAGS) -c -I../cray ../cray/trcrun.c
trace.o: ../cray/trace.o 
	$(CC) $(CFLAGS) -c -I../cray ../cray/trace.o
