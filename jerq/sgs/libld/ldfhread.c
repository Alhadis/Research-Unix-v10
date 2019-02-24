#include    <stdio.h>
#include    "filehdr.h"
#include    "ldfcn.h"

int
ldfhread(ldptr, filehead)

LDFILE    *ldptr;
FILHDR    *filehead; 

{
    extern int		fread( );
    extern int		fseek( );

    extern int		vldldptr( );

    if (vldldptr(ldptr) == SUCCESS) {
	if (FSEEK(ldptr, 0L, BEGINNING) == OKFSEEK) {
	    if (FREAD(filehead, FILHSZ, 1, ldptr) == 1) {
	    	return(SUCCESS);
	    }
	}
    }

    return(FAILURE);
}

static char ID[ ] = "@(#) ldfhread.c: 1.1 1/7/82";
