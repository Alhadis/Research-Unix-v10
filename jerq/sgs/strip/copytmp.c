/* UNIX HEADER */
#include	<stdio.h>

/* COMMON SGS HEADERS */
#include	"filehdr.h"
#include	"ldfcn.h"
/* STRIP HEADER */
#include	"defs.h"


    /*  copytmp(filename, filehead)
     *
     *  copies file header in filehead back to original file and
     *  copies the rest of the stripped file from temporary back to original
     *
     *  calls:
     *      - ignorsig( ) to turn off interrupts while recreating original file
     *      - resetsig( ) to turn signals back on after recreating original
     *      - error(file, string, level) if it cannot recreate original
     *
     *  returns SUCCESS or FAILURE
     */


int
copytmp(filename, filehead)

char	*filename;
FILHDR	*filehead;

{
    /* UNIX FUNCTIONS CALLED */
    extern int	fread( ),
		fwrite( );

    /* STRIP FUNCTIONS CALLED */
    extern	ignorsig( ),
		resetsig( ),
		error( );

    /* EXTERNAL VARIABLES USED */
    extern FILE	*strp1,
		*strp2,
		*stripout;
#if FLEXNAMES
	extern	long	string_size;
	extern	char	tmpnam5[];
#endif

    /* LOCAL VARIABLES */
    char	buffer[BUFSIZ];
    int		nitems;
#if FLEXNAMES
	FILE	*tmpfile;
#endif


    ignorsig( );

    if (fwrite(filehead, FILHSZ, 1, stripout) != 1) {
	error(filename, "cannot recreate fileheader", 5);
	resetsig( );
	return(FAILURE);
    }

    while ((nitems = fread(buffer, sizeof (char), BUFSIZ, strp1)) != 0) {
	if (fwrite(buffer, sizeof (char), nitems, stripout) != nitems) {
	    error(filename, "cannot recreate stripped file<tmp1>", 5);
	    resetsig( );
	    return(FAILURE);
	}
    }
    while ((nitems = fread(buffer, sizeof (char), BUFSIZ, strp2)) != 0) {
	if (fwrite(buffer, sizeof (char), nitems, stripout) != nitems) {
	    error(filename, "cannot recreate stripped file<tmp2>", 5);
	    resetsig( );
	    return(FAILURE);
	}
    }

#if FLEXNAMES
	if (string_size > 0L)
	{
		if ((tmpfile = fopen( tmpnam5, "r" )) == NULL)
		{
			fprintf( stderr, "cannot read temp file" );
			resetsig( );
			return( FAILURE );
		}

		if (fwrite( &string_size, sizeof( long ), 1, stripout ) != 1)
		{
			error( filename, "cannot recreate stripped file", 5 );
			resetsig( );
			return( FAILURE );
		}

		while ((nitems = fread( buffer, sizeof( char ), BUFSIZ, tmpfile)) != 0)
			if (fwrite( buffer, sizeof( char ), nitems, stripout ) != nitems)
			{
				error( filename, "cannot recreate stripped file", 5 );
				resetsig( );
				return( FAILURE );
			}

		fclose( tmpfile );
	}
#endif

    resetsig( );
    return(SUCCESS);
}
/*
 *	static char ID[] = "@(#) copytmp.c: 1.4 3/3/83";
 */
