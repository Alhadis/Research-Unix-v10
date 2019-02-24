#	nroff terminal driving tables make file

INSDIR = /usr/lib/term
TFILES = code.300 code.lp tab2631.c tab2631-c.c tab2631-e.c tab300.c \
	 tab300-12.c tab300s.c tab300s-12.c tab37.c tab382.c \
	 tab4000A.c tab450.c tab450-12.c tab832.c taba1.c tablp.c tabtn300.c \
	 tabX.c tabavt.c
PFILES = maketerms.c terms.hd
FILES = tab2631 tab2631-c tab2631-e tab300 tab300-12 tab300s \
	tab300s-12 tab37 tab382 tab4000A tab450 \
	tab450-12 tab832 taba1 tablp tabtn300 tabX tabavt

maketerms compile all:  terms.hd
	${CC} -DPART1 -o maketerms maketerms.c
	./maketerms
	${CC} -DPART2 -o maketerms maketerms.c
	./maketerms
	${CC} -DPART3 -o maketerms maketerms.c
	./maketerms

install:  maketerms
	umask 113; cp ${FILES} ${INSDIR}
	cd ${INSDIR}; rm -f tab300S tab300S-12
	cd ${INSDIR};  ln tab300s tab300S;  ln tab300s-12 tab300S-12
	-cd ${INSDIR}; chown bin ${FILES} tab300S tab300S-12 ;\
		chgrp bin ${FILES} tab300S tab300S-12

clean:
	rm -f maketerms

clobber:  clean
	rm -f ${FILES}

delete:	clobber
	rm -f ${TFILES} ${PFILES}
