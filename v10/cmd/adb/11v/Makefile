FILES= access.o command.o expr.o format.o input.o main.o output.o pcs.o print.o runpcs.o sym.o mchsys.o trcrun.o mchinit.o mchio.o opset.o syminit.o setup.o mchframe.o regs.o mchpr.o
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
trcrun.o: ../null/trcrun.c 
	$(CC) $(CFLAGS) -c -I../null ../null/trcrun.c
mchinit.o: ../null/mchinit.c 
	$(CC) $(CFLAGS) -c -I../null ../null/mchinit.c
mchio.o: ../11v/mchio.c ../comm/defs.h 
	$(CC) $(CFLAGS) -c ../11v/mchio.c
opset.o: ../11v/opset.c ../comm/defs.h 
	$(CC) $(CFLAGS) -c ../11v/opset.c
syminit.o: ../11v/syminit.c ../comm/defs.h 
	$(CC) $(CFLAGS) -c ../11v/syminit.c
setup.o: ../11v/setup.c ../comm/defs.h 
	$(CC) $(CFLAGS) -c ../11v/setup.c
mchframe.o: ../11v/mchframe.c ../comm/defs.h 
	$(CC) $(CFLAGS) -c ../11v/mchframe.c
regs.o: ../11v/regs.c ../comm/defs.h 
	$(CC) $(CFLAGS) -c ../11v/regs.c
mchpr.o: ../11v/mchpr.o 
	$(CC) $(CFLAGS) -c ../11v/mchpr.o
