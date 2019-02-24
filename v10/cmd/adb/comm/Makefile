CFLAGS=	-g
LDFLAGS= -g
OFILES= access.o command.o expr.o format.o input.o main.o message.o \
	opset.o optab.o output.o pcs.o print.o regs.o runpcs.o setup.o sym.o \
	mchio.o mchrun.o mchsys.o mchframe.o

adb:	${OFILES}
	${CC} ${CFLAGS} -o adb ${OFILES}

message.o:
	${CC} -c -R message.c
optab.o:
	${CC} -c -R  optab.c

install:	adb
	strip adb
	cp adb ${DESTDIR}/bin

clean:
	rm -f adb ${OFILES}

defs.h: mtype.h

access.o: defs.h regs.h
command.o: defs.h regs.h
expr.o: defs.h regs.h
format.o: defs.h
input.o: defs.h
main.o: defs.h
mchframe.o: defs.h regs.h
mchio.o: defs.h
mchrun.o: defs.h regs.h
mchsys.o: defs.h machine.h
message.o: defs.h
opset.o: defs.h
optab.o: defs.h instrs
output.o: defs.h
pcs.o: defs.h
print.o: defs.h regs.h
regs.o: defs.h regs.h
runpcs.o: defs.h regs.h
setup.o: defs.h machine.h base.h
sym.o: defs.h
