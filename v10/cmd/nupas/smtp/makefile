# COBJ is for sysname_read()
COBJ=log.o ../common/common.a ../libc/libc.a ../config/config.o

QEROBJ=smtpqer.o to822addr.o qlib.o $(COBJ)
SMTPOBJ=smtp.o converse.o netio.o to822addr.o to822.o header.o $(COBJ)
SMTPDOBJ=smtpd.o conversed.o netio.o qlib.o from822addr.o from822.o header.o $(COBJ)

CFLAGS=${UNIX} -g -I. -I../libc -I../common ${SCFLAGS}
LIB=/usr/lib/upas


all:	smtp smtpd smtpqer smtpsched


smtp:	../ipc/libipc.a $(SMTPOBJ)
	$(CC) $(SMTPOBJ) ../ipc/libipc.a -o smtp $(SLFLAGS)

smtpd:	$(SMTPDOBJ)
	$(CC) $(SMTPDOBJ) -o smtpd $(SLFLAGS)

smtpqer: $(QEROBJ)
	$(CC) $(QEROBJ) -o smtpqer $(SLFLAGS)

smtpsched: smtpsched.o qlib.o
	$(CC) smtpsched.o qlib.o $(COBJ) -o smtpsched $(SLFLAGS)


from822.o:	header.h ../common/mail.h ../common/string.h ../common/aux.h
from822addr.o:	../common/string.h
header.o:	header.h ../common/mail.h ../common/string.h ../common/aux.h
converse.o:	smtp.h ../common/mail.h ../common/string.h
conversed.o:	smtp.h cmds.h ../common/mail.h ../common/string.h
smtp.o:		smtp.h ../common/string.h ../common/aux.h
netio.o:	smtp.h
qlib.o:		smtp.h ../common/string.h
smtpqer.o:	../common/string.h ../common/mail.h
smtpsched.o:	smtp.h ../common/string.h
to822.o:	header.h addrformat.h ../common/mail.h ../common/string.h \
		../common/aux.h
to822addr.o:	addrformat.h ../common/string.h


install:	dirs $(LIB)/smtp $(LIB)/smtpd $(LIB)/smtpqer $(LIB)/smtpsched

dirs:
	-mkdir /usr/spool/smtpq
	-chown uucp /usr/spool/smtpq
	-chmod 775 /usr/spool/smtpq

$(LIB)/smtp:	smtp
	chmod 755 smtp
	mv $(LIB)/smtp $(LIB)/osmtp && cp smtp $(LIB)/smtp
	strip $(LIB)/smtp

$(LIB)/smtpd:	smtpd
	chmod 755 smtpd
	cp smtpd $(LIB)/smtpd
	strip $(LIB)/smtpd

$(LIB)/smtpqer:	smtpqer
	cp smtpqer $(LIB)/smtpqer
	strip $(LIB)/smtpqer
	chown uucp $(LIB)/smtpqer
	chmod 6775 $(LIB)/smtpqer

$(LIB)/smtpsched:	smtpsched
	chmod 755 smtpsched
	cp smtpsched $(LIB)/smtpsched
	strip $(LIB)/smtpsched


clean:
	rm -f smtp smtpd smtpqer smtpsched *.o
