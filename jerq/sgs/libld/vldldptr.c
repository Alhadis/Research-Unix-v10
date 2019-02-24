#include	<stdio.h>
#include	"filehdr.h"
#include	"ldfcn.h"
#include	"lddef.h"

int
vldldptr(ldptr)

LDFILE	*ldptr;

{
    extern LDLIST	*_ldhead;

    LDLIST		*ldindx;

    for (ldindx = _ldhead; ldindx != NULL; ldindx = ldindx->ld_next) {
	if (ldindx == (LDLIST *) ldptr) {
	    return(SUCCESS);
	}
    }

    return(FAILURE);
}

static char ID[ ] = "@(#) vldldptr.c: 1.1 1/8/82";
