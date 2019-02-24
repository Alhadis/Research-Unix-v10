# If routines are missing, try running with
#
#	SLFLAGS=-lresolv	if the resolver is used
#					(it's required by mx)

STUBS=stubdk.o stubtcp.o stubmx.o stubproxy.o
COBJS=ipcpath.o util.o ipcperror.o ${ERR}

CFLAGS=-g $(SCFLAGS)

all:
	@echo "usage: make init [dk] [tcp] [otcp] [utstcp] [mx] [proxy] lib"
	@exit 1

stublib.a:	$(STUBS)
	rm -f stublib.a
	ar cr stublib.a $(STUBS)


init:	$(COBJS)
	rm -f libipc.a reallib.a
	ar cr libipc.a $(COBJS)

	
lib:	ipcopen.o stublib.a
	-ranlib stublib.a reallib.a
	ld -r ipcopen.o reallib.a stublib.a -o a.out $(SLFLAGS) && mv a.out ipc.o
	ar cr libipc.a ipc.o
	-ranlib libipc.a
	
dk:	dkconnect.o
	ld -r dkconnect.o -ldk -o a.out && mv a.out dk.o
	ar cr reallib.a dk.o

sundk:	dkconnect.o
	ld -r dkconnect.o /usr/dk/lib/libdk.a -o a.out && mv a.out dk.o
	ar cr reallib.a dk.o

mipsdk:	mipsdkcon.o /v/lib/libipc.a
	ld -r mipsdkcon.o /v/lib/libipc.a -o a.out && mv a.out dk.o
	ar cr reallib.a dk.o

tcp:	tcpconnect.o
	ar cr reallib.a tcpconnect.o

otcp:	otcpconnect.o
	ar cr reallib.a otcpconnect.o

utstcp:	utcpconnect.o s5sockbcon.o
	ar cr libipc.a utsconnect.o s5sockbcon.o

mx:	tcp mxconnect.o
	ar cr reallib.a mxconnect.o

proxy:	proxio.o
	ar cr reallib.a proxio.o


proxio.o:	proxio.c proxy.h
	$(CC) $(CFLAGS) -c proxio.c

ipcopen.o:	ipcopen.c
	$(CC) $(CFLAGS) -c ipcopen.c

research:	res_err.o
	ld -r res_err.o -lipc -o a.out && mv a.out res.o
	ar cr libipc.a res.o
	ranlib libipc.a

mipsdkcon.o:
	$(CC) $(CFLAGS) -c mipsdkcon.c -I/v/include

clean:
	rm -f *.o *.a a.out
