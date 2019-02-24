BIN=rogind rshd tcpd telnetd ftpd
CFLAGS=-g -I../etc -I../libin

all	:	${BIN}

rogind	:	rogind.o
		cc ${CFLAGS} -o rogind rogind.o -lin

rshd	:	rshd.o
		cc ${CFLAGS} -o rshd rshd.o -lin

tcpd	:	tcpd.o
		cc ${CFLAGS} -o tcpd tcpd.o -lin

telnetd	:	telnetd.o telnetaux.o
		cc ${CFLAGS} -o telnetd telnetd.o telnetaux.o -lin

ftpd	:	ftpd.o ftpcmd.o ftpaux.o
		${CC} -g -o ftpd ftpd.o ftpcmd.o ftpaux.o -lin

ftpcmd.o:	ftpcmd.y
		yacc ftpcmd.y
		cc -c ${CFLAGS} y.tab.c
		mv y.tab.o ftpcmd.o

install	:	${BIN}
		cp ${BIN} /usr/inet/etc
		chmod 755 /usr/inet/etc/*
		chown root,bin /usr/inet/etc/*

clean:
	rm -f *.[oO] y.tab.c rogind rshd tcpd telnetd ftpd
