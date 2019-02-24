/*
 * pax makefile
 */

ancestor = 3

DEBUG == 1

SYSTEM == "$(_release_:O=1) $(_version_:O=1)"

pax :: RELEASE HISTORY pax.1 pax.1.posix \
	pax.h bio.c convert.c copy.c pax.c \
	delta.c file.c format.c misc.c \
	-lodelta -lx

:INSTALLDIR: cpio tar

cpio :: cpio.sh

tar :: tar.sh

:: stubs.c
