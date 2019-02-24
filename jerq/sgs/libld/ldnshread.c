#include	<stdio.h>
#include	"filehdr.h"
#include	"scnhdr.h"
#include	"ldfcn.h"

int
ldnshread(ldptr, sectname, secthdr)

LDFILE	*ldptr;
char	*sectname;
SCNHDR	*secthdr; 

{
    extern int		fseek( );
    extern int		fread( );

    extern int		vldldptr( );

    unsigned short	i;
    int			j;
    unsigned short	numsects;

    if (vldldptr(ldptr) == SUCCESS) {
	if (FSEEK(ldptr, (long) FILHSZ + HEADER(ldptr).f_opthdr,
            BEGINNING) == OKFSEEK) {
	    numsects = (HEADER(ldptr)).f_nscns;
	    for (i=0;
		(i < numsects) && (FREAD(secthdr,SCNHSZ,1,ldptr) == 1);
		 ++i) {
		    for (j = 0;
		        (j < 8) && (secthdr->s_name[j] == sectname[j]);
		     	++j) {
			    if (secthdr->s_name[j] == '\0') {
				return(SUCCESS);
			    }
		    }
		    if (j == 8) {
		    	return(SUCCESS);
		    }
	    }
	}
    }

    return(FAILURE);
}

static char ID[ ] = "@(#) ldnshread.c: 1.1 1/7/82";
