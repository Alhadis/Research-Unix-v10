#include    <stdio.h>
#include    "filehdr.h"
#include    "ldfcn.h"

LDFILE *
ldaopen(filename, oldptr)
	char *filename;
	LDFILE *oldptr; 
{
	extern FILE *fopen();
	extern int vldldptr();
	extern LDFILE *allocldptr();
	extern int freeldptr();
	FILE *ioptr;
	LDFILE *nldptr;

	if (vldldptr(oldptr) == FAILURE || (nldptr = allocldptr()) == NULL)
		return (NULL);
	if ((ioptr = fopen(filename, "r")) == NULL)
	{
		freeldptr(nldptr);
		return (NULL);
	}
	IOPTR(nldptr) = ioptr;
	OFFSET(nldptr) = OFFSET(oldptr);
	TYPE(nldptr) = TYPE(oldptr);
	hdrassign(oldptr, nldptr);
	nldptr->_fnum_ = oldptr->_fnum_;	/* use same string table */
	return (nldptr);
}



static int
hdrassign(oldptr, nldptr)
	LDFILE *oldptr;
	LDFILE *nldptr;
{
	(HEADER(nldptr)).f_magic = (HEADER(oldptr)).f_magic;
	(HEADER(nldptr)).f_nscns = (HEADER(oldptr)).f_nscns;
	(HEADER(nldptr)).f_timdat = (HEADER(oldptr)).f_timdat;
	(HEADER(nldptr)).f_symptr = (HEADER(oldptr)).f_symptr;
	(HEADER(nldptr)).f_nsyms = (HEADER(oldptr)).f_nsyms;
	(HEADER(nldptr)).f_opthdr = (HEADER(oldptr)).f_opthdr;
	(HEADER(nldptr)).f_flags = (HEADER(oldptr)).f_flags;
	return;
}

static char ID[] = "@(#) ldaopen.c: 1.2 2/16/83";
