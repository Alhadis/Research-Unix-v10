#include	<stdio.h>
#include	"filehdr.h"
#include	"scnhdr.h"
#include	"ldfcn.h"

int
ldshread(ldptr, sectnum, secthdr)

LDFILE	*ldptr;
unsigned short	sectnum;
SCNHDR	*secthdr; 

{
    extern int		fseek( );
    extern int		fread( );

    extern int		vldldptr( );

    if (vldldptr(ldptr) == SUCCESS) {
	if ((sectnum != 0) && (sectnum <= HEADER(ldptr).f_nscns)) {
	    if (FSEEK(ldptr,
		FILHSZ + HEADER(ldptr).f_opthdr + (sectnum - 1L) * SCNHSZ,
		BEGINNING) == OKFSEEK) {
		if (FREAD(secthdr, SCNHSZ, 1, ldptr) == 1) {
		    return(SUCCESS);
		}
	    }
	}
    }

    return(FAILURE);
}

static char ID[ ] = "@(#) ldshread.c: 1.1 1/7/82";
