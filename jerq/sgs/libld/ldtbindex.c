#include	<stdio.h>
#include	"filehdr.h"
#include	"syms.h"
#include	"ldfcn.h"

long
ldtbindex(ldptr)

LDFILE	*ldptr;

{
    extern long		ftell( );

    extern int		vldldptr( );

    long		position;


    if (vldldptr(ldptr) == SUCCESS) {
	if ((position = FTELL(ldptr) - OFFSET(ldptr) - HEADER(ldptr).f_symptr) 
	    >= 0) {

	    if ((position % SYMESZ) == 0) {
		return(position / SYMESZ);
	    }
	}
    }

    return(BADINDEX);
}

static char	ID[ ] = "@(#) ldtbindex.c: 1.1 1/7/82";
