CC=2c
LD=2l
FILES= access.2 command.2 expr.2 format.2 input.2 main.2 output.2 pcs.2 print.2 runpcs.2 sym.2 mchsys.2 trcrun.2 mchinit.2 gtou.2 mchio.2 das.2 optab.2 setup.2 syminit.2 regs.2 mchpr.2 mchframe.2
CFLAGS=-I. -I../comm

adb: $(FILES)
	$(LD) -o adb $(FILES)

clean:
	rm adb $(FILES)

access.2: ../comm/access.c ../comm/defs.h 
	$(CC) $(CFLAGS) ../comm/access.c
command.2: ../comm/command.c ../comm/defs.h 
	$(CC) $(CFLAGS) ../comm/command.c
expr.2: ../comm/expr.c ../comm/defs.h 
	$(CC) $(CFLAGS) ../comm/expr.c
format.2: ../comm/format.c ../comm/defs.h 
	$(CC) $(CFLAGS) ../comm/format.c
input.2: ../comm/input.c ../comm/defs.h 
	$(CC) $(CFLAGS) ../comm/input.c
main.2: ../comm/main.c ../comm/defs.h 
	$(CC) $(CFLAGS) ../comm/main.c
output.2: ../comm/output.c ../comm/defs.h 
	$(CC) $(CFLAGS) ../comm/output.c
pcs.2: ../comm/pcs.c ../comm/defs.h 
	$(CC) $(CFLAGS) ../comm/pcs.c
print.2: ../comm/print.c ../comm/defs.h 
	$(CC) $(CFLAGS) ../comm/print.c
runpcs.2: ../comm/runpcs.c ../comm/defs.h 
	$(CC) $(CFLAGS) ../comm/runpcs.c
sym.2: ../comm/sym.c ../comm/defs.h 
	$(CC) $(CFLAGS) ../comm/sym.c
mchsys.2: ../null/mchsys.c ../comm/defs.h 
	$(CC) $(CFLAGS) -I../null ../null/mchsys.c
trcrun.2: ../null/trcrun.c 
	$(CC) $(CFLAGS) -I../null ../null/trcrun.c
mchinit.2: ../null/mchinit.c 
	$(CC) $(CFLAGS) -I../null ../null/mchinit.c
gtou.2: ../g/gtou.c
mchio.2: ../g/mchio.c
das.2: ../g/das.c ../comm/defs.h
optab.2: ../g/optab.c
setup.2: ../g/setup.c ../comm/defs.h
syminit.2: ../g/syminit.c ../comm/defs.h
regs.2: ../g/regs.c
mchpr.2: ../g/mchpr.c
mchframe.2: ../g/mchframe.c

%.2:	%.c
	$(CC) $(CFLAGS) $%.c
