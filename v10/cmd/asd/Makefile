LFLAGS = -hcp

DEST = /usr/bin
ETC = /etc

LIB = alloc.o args.o chk.o crypt.o data.o ftw.o fullname.o \
	gid.o header.o mkcsum.o mkdir.o package.o path.o pwd.o \
	tmpname.o transname.o uid.o \
	closedir.o opendir.o readdir.o seekdir.o telldir.o

LIBSRC = alloc.c args.c chk.c crypt.c data.c ftw.c fullname.c \
	gid.c header.c mkcsum.c mkdir.c package.c path.c pwd.c \
	tmpname.c transname.c uid.c \
	closedir.c opendir.c readdir.c seekdir.c telldir.c

all:	seal unseal mkpkg inspkg asdrcv

config.h:	mkconfig
	./mkconfig >config.h

seal:	seal.o lib.a
	cc $(CFLAGS) -o seal seal.o `./libnames`

unseal:	unseal.o lib.a
	cc $(CFLAGS) -o unseal unseal.o `./libnames`

mkpkg:  mkpkg.o lib.a
	cc $(CFLAGS) -o mkpkg mkpkg.o `./libnames`

inspkg:	inspkg.o lib.a
	cc $(CFLAGS) -o inspkg inspkg.o `./libnames`

asdrcv:	asdrcv.o lib.a
	cc $(CFLAGS) -o asdrcv asdrcv.o `./libnames`

lib.a:	$(LIB)
	rm -f lib.a
	ar qc lib.a `lorder $(LIB) | tsort`
	./ranlib lib.a

lint:
	lint $(LFLAGS) mkpkg.c $(LIBSRC)
	lint $(LFLAGS) inspkg.c $(LIBSRC)
	lint $(LFLAGS) seal.c $(LIBSRC)
	lint $(LFLAGS) unseal.c $(LIBSRC)
	lint $(LFLAGS) asdrcv.c $(LIBSRC)

alloc.o args.o data.o fullname.o gid.o: asd.h ar.h config.h
header.o inspkg.o mkdir.o: asd.h ar.h config.h
mkpkg.o package.o path.o pwd.o seal.o tmpname.o: asd.h ar.h config.h
transname.o uid.o unseal.o: asd.h ar.h config.h
ftw.o mkdir.o package.c: ftw.h
mkcsum.o seal.o unseal.o: seal.h
closedir.o opendir.o readdir.o seekdir.o telldir.o: ndir.h config.h

clean:
	rm -f lib.a config.h asdrcv mkpkg inspkg seal unseal *.o

install: all
	./install.asd
