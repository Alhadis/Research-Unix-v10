CC = PTCC
LIBOBJ = checksum.o decl.o dir.o domach.o impl.o locksubs.o param.o path.o
OBJ = $(LIBOBJ) cdaemon.o dkinstall.o mkspool.o udaemon.o
LIB = lib.a -lnstring -lipc
ASD = /usr/lib/asd
LIBPROGS = cdaemon dkinstall mkspool udaemon
CMDS = /usr/bin/ship /usr/bin/shipstat $(ASD)/dlist

all: $(LIBPROGS)

install: all
	cd $(ASD); rm -f $(LIBPROGS)
	cp $(LIBPROGS) rmlocks $(ASD)
	cd $(ASD); strip $(LIBPROGS)
	cd $(ASD); chown root,other $(LIBPROGS); chmod og-w $(LIBPROGS)
	chmod u+s $(ASD)/mkspool
	cp ship shipstat /usr/bin
	cp dlist $(ASD)
	chown bin,bin $(CMDS)
	chmod o-w,g+w $(CMDS)

shipall:
	cd $(ASD); ship $(LIBPROGS) /usr/bin/ship /usr/bin/shipstat

cdaemon:	lib.a cdaemon.o
	$(CC) -o cdaemon cdaemon.o $(LIB)

dkinstall:	lib.a dkinstall.o
	$(CC) -o dkinstall dkinstall.o $(LIB)

mkspool:	lib.a mkspool.o
	$(CC) -o mkspool mkspool.o $(LIB)

udaemon:	lib.a udaemon.o
	$(CC) -o udaemon udaemon.o $(LIB)

lib.a:	$(LIBOBJ)
	rm -f lib.a
	ar qc lib.a $(LIBOBJ)
	ranlib lib.a

$(OBJ):	decl.h

clean:
	rm -f *.o lib.a $(LIBPROGS)

push:
	dest=bowell ship -x 'cd /src/cmd/asd++; trap "" 1 2 3 15; \
		make 2>&1 | mail ark&' -D.=/src/cmd/asd++ *.[ch] Makefile
