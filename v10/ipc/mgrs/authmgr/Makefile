OBJS=doconnect.o main.o
SECRETOBJS=secret.o
S5OBJS=${OBJS} ${SECRETOBJS} s5_tty.o
RESOBJS=${OBJS} ${SECRETOBJS} res_tty.o

# att-ms (old Sys V) version

auth:	ipclib ${S5OBJS}
	${CC} ${LDFLAGS} ${S5OBJS} ../ipc/libipc.a -lnet -lnsl_s -lc -o auth

ipclib:
	(cd ../ipc;  ERR=s5_err.o make init dk tcp lib)

# research version

res_auth:	${RESOBJS}
	${CC} ${LDFLAGS} ${RESOBJS} -lipc -o auth

doconnect.o:	auth.h
main.o:		auth.h
secret.o:	auth.h
s5_tty.o:
res_tty.o:


clean:
	rm -f *.o auth core

install:	auth
	cp auth /usr/add-on/bin/auth
