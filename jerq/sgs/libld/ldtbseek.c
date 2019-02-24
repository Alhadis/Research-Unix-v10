#include	<stdio.h>
#include	"filehdr.h"
#include	"ldfcn.h"

int
ldtbseek(ldptr)

LDFILE		*ldptr;

{
    extern int		fseek( );

    extern int		vldldptr( );

    if (vldldptr(ldptr) == SUCCESS) {
	if (HEADER(ldptr).f_symptr != 0L) {
	    if (FSEEK(ldptr, HEADER(ldptr).f_symptr, BEGINNING) == OKFSEEK) {
		return(SUCCESS);
	    }
	}
    }

    return(FAILURE);
}

static char ID[ ] = "@(#) ldtbseek.c: 1.1 1/7/82";
