#include    <stdio.h>
#include    "filehdr.h"
#include    "ldfcn.h"

int
ldaclose(ldptr)

LDFILE    *ldptr; 

{
    extern 		fclose( );

    extern int		vldldptr( );
    extern	    	freeldptr( );

    if (vldldptr(ldptr) == FAILURE) {
	return(FAILURE);
    }

    fclose(IOPTR(ldptr));
    freeldptr(ldptr);

    return(SUCCESS);
}

static char ID[ ] = "@(#) ldaclose.c: 1.1 1/7/82";
