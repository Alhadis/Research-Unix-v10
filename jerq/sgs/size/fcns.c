/* UNIX HEADER */
#include	<stdio.h>

/* COMMON SGS HEADERS */
#include	"filehdr.h"
#include	"scnhdr.h"
#include	"ldfcn.h"

/* SGS SPECIFIC HEADER */
#include	"sgs.h"

/* SIZE HEADER */
#include	"defs.h"


    /*  error(file, string)
     *
     *  simply prints the error message string
     *  simply returns
     */


error(file, string)

char	*file;
char	*string;

{
     /* UNIX FUNCTIONS CALLED */
    extern	fprintf( ),
		fflush( );

    fflush(stdout);
    fprintf(stderr, "%ssize:  %s:  %s\n", SGS, file, string);

    return;
}




#ifndef UNIX
    /*  findtotal( )
     *
     *  computes and returns the sum total of all section sizes
     *  returns TROUBLE if the object file is messed up
     */


long
findtotal( )

{
    /* COMMON OBJECT FILE ACCESS ROUTINE CALLED */
    extern int		ldshread( );

    /* EXTERNAL VARIABLE USED */
    extern LDFILE	*ldptr;

    /* LOCAL VARIABLES */
    long		size;
    unsigned short	section;
    SCNHDR		secthead;

    size = 0L;
    for (section = 1; section <= HEADER(ldptr).f_nscns; ++section) {
	if (ldshread(ldptr, section, &secthead) != SUCCESS) {
	    return(TROUBLE);
	}
	size += secthead.s_size;
    }

    return(size);
}
#endif	/*UNIX*/

/*
*     static char	ID_fcns[ ] = "@(#) fcns.c: 1.2 3/5/83";
*/
