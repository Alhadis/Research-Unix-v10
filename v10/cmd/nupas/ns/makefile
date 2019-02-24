OBJS=translate.o ../config/config.o ../common/common.a ../libc/libc.a
SRCS=translate.c ../common/aux.c ../common/string.c ../common/mail.c
CFLAGS=${UNIX} -g -I. -I../libc -I../common ${SCFLAGS}
LFLAGS=-g ${SLFLAGS}
LIB=/usr/lib/upas

.c.o: ; $(CC) -c $(CFLAGS) $*.c

all: translate

translate: $(OBJS)
	$(CC) $(LFLAGS) $(OBJS) -o translate

translate.o: ../common/mail.h ../common/string.h ../common/aux.h

cyntax: 
	cyntax $(CFLAGS) $(SRCS)

install: translate
	mv translate $(LIB)

clean:
	-rm -f *.[oO] translate core a.out *.sL
