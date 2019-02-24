FILES= access.o command.o expr.o format.o input.o main.o output.o pcs.o print.o runpcs.o sym.o regs.o mchpr.o mchframe.o mchsys.o trcrun.o mchinit.o mchio.o misc.o das.o optab.o setup.o syminit.o
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
regs.o: ../null/regs.c ../comm/defs.h 
	$(CC) $(CFLAGS) -c -I../null ../null/regs.c
mchpr.o: ../null/mchpr.c 
	$(CC) $(CFLAGS) -c -I../null ../null/mchpr.c
mchframe.o: ../null/mchframe.c 
	$(CC) $(CFLAGS) -c -I../null ../null/mchframe.c
mchsys.o: ../null/mchsys.c ../comm/defs.h 
	$(CC) $(CFLAGS) -c -I../null ../null/mchsys.c
trcrun.o: ../null/trcrun.c 
	$(CC) $(CFLAGS) -c -I../null ../null/trcrun.c
mchinit.o: ../null/mchinit.c 
	$(CC) $(CFLAGS) -c -I../null ../null/mchinit.c
mchio.o: ../68v/mchio.c 
	$(CC) $(CFLAGS) -c ../68v/mchio.c
misc.o: ../68v/misc.c 
	$(CC) $(CFLAGS) -c ../68v/misc.c
das.o: ../68v/das.c ../comm/defs.h 
	$(CC) $(CFLAGS) -c ../68v/das.c
optab.o: ../68v/optab.c 
	$(CC) $(CFLAGS) -c ../68v/optab.c
setup.o: ../68v/setup.c ../comm/defs.h 
	$(CC) $(CFLAGS) -c ../68v/setup.c
syminit.o: ../68v/syminit.c ../comm/defs.h 
	$(CC) $(CFLAGS) -c ../68v/syminit.c
