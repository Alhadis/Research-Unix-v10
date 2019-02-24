#include	<stdio.h>
#include	"filehdr.h"
#include	"scnhdr.h"
#include	"ldfcn.h"

int
ldsseek(ldptr, sectnum)

LDFILE		*ldptr;
unsigned short	sectnum; 

{
	extern	int	ldshread( );
	extern	int	fseek( );

	SCNHDR	shdr;

	if (ldshread(ldptr, sectnum, &shdr) == SUCCESS) {
		if (shdr.s_scnptr != 0) {
		    if (FSEEK(ldptr, shdr.s_scnptr, BEGINNING) == OKFSEEK) {
			    return(SUCCESS);
		    }
		}
	}

	return(FAILURE);
}

static char ID[ ] = "@(#) ldsseek.c: 1.1 1/7/82";
