#include	<stdio.h>
#include	"filehdr.h"
#include	"syms.h"
#include	"ldfcn.h"

int
ldtbread(ldptr, symnum, symentry)

LDFILE	*ldptr;
long	symnum;
SYMENT	*symentry;

{
    extern int		fseek( );
    extern int		fread( );

    extern int		vldldptr( );

    if (vldldptr(ldptr) == SUCCESS) {
	if ((symnum >= 0) && (symnum <= (HEADER(ldptr)).f_nsyms)) {
	    if (FSEEK(ldptr,
		HEADER(ldptr).f_symptr + symnum * SYMESZ, BEGINNING)
		== OKFSEEK) {
		if (FREAD(symentry,SYMESZ,1,ldptr) == 1) {
		    return(SUCCESS);
		}
	    }
	}
    }

    return(FAILURE);
}

static char ID[ ] = "@(#) ldtbread.c: 1.1 1/7/82";
