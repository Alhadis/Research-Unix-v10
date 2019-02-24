/* UNIX HEADERS */
#include	<stdio.h>
#include	<signal.h>

/* COMMON SGS HEADERS */
#include	"filehdr.h"
#include	"ldfcn.h"

/* SGS SPECIFIC HEADER */
#include	"sgs.h"

/* STRIP HEADER */
#include	"defs.h"


    /*  error(file, message, level)
     *
     *  prints an error message
     *  and closes what ever files are open associated with the error level and
     *  the current state of the lister
     *
     *  simply returns
     */


error(file, message, level)

char	*file;
char	*message;
int	level;

{
	/* UNIX FUNCTIONS CALLED */
	extern		fprintf( ),
			fclose( ),
			unlink( );

	/* COMMON OBJECT FILE ACCESS ROUTINE CALLED */
	extern int	ldaclose( );

	/* EXTERNAL VARIABLES USED */
	extern LDFILE	*ldptr,
			*fwdptr;
	extern FILE	*strp1,
			*strp2,
			*stripout;
#if AR16WR
	extern FILE	*tempfil,
			*readtmp;
#endif
	extern char	tmpnam1[ ],
			tmpnam2[ ];


	fprintf(stderr, "%sstrip:  %s:  %s\n", SGS, file, message);

	switch(level) {
	    case 0:
		break;

	    case 1:
		ldaclose(ldptr);
		break;

	    case 2:
		ldaclose(ldptr);
		if (strp1 != NULL)
			fclose(strp1);
		if (strp2 != NULL)
			fclose(strp2);
#if AR16WR
		if (tempfil != NULL)
			fclose(tempfil);
		if (readtmp != NULL)
			fclose(readtmp);
#endif
		break;

	    case 3:
		ldaclose(ldptr);
		ldaclose(fwdptr);
		if (strp1 != NULL)
			fclose(strp1);
		if (strp2 != NULL)
			fclose(strp2);
#if AR16WR
		if (tempfil != NULL)
			fclose(tempfil);
		if (readtmp != NULL)
			fclose(readtmp);
#endif
		break;

	    case 4:
		if (stripout != NULL)
			fclose(stripout);
		unlink(tmpnam1);
		break;

	    case 5:
		if (strp1 != NULL)
			fclose(strp1);
		if (strp2 != NULL)
			fclose(strp2);
#if AR16WR
		if (tempfil != NULL)
			fclose(tempfil);
		if (readtmp != NULL)
			fclose(readtmp);
#endif
		if (stripout != NULL)
			fclose(stripout);
		unlink(file);
		break;

	    default:
		break;
	}
	return;
}




    /*  catchsig( )
     *
     *  prepares strip to field interrupts (via function onintr( ))
     *  so that if interrupted strip can remove the temporary files it has
     *  created
     *
     *  catchsig simply returns
     */


catchsig( )

{
	/* UNIX FUNCTION CALLED */
	extern int	(*signal( ))( );

	/* EXTERNAL VARIABLE USED */
	extern		onintr( );

	if ((signal(SIGINT, SIG_IGN)) == SIG_DFL)
		signal(SIGINT, onintr);

	if ((signal(SIGHUP, SIG_IGN)) == SIG_DFL)
		signal(SIGHUP, onintr);

	if ((signal(SIGQUIT, SIG_IGN)) == SIG_DFL)
		signal(SIGQUIT, onintr);

	if ((signal(SIGTERM, SIG_IGN)) == SIG_DFL)
		signal(SIGTERM, onintr);

	return;

}




/* STATIC VARIABLES USED */
static int	(*oldint)( );
static int	(*oldhup)( );
static int	(*oldquit)( );
static int	(*oldterm)( );


    /*  ignorsig( )
     *
     *  turns off interrupts but saves their former state in static variables
     *  so that previous state of interrupts can be restored later
     *
     *  ignorsig simply returns
     */


ignorsig( )

{
	/* UNIX FUNCTION CALLED */
	extern int	(*signal( ))( );

	oldint = signal(SIGINT, SIG_IGN);
	oldhup = signal(SIGHUP, SIG_IGN);
	oldquit = signal(SIGQUIT, SIG_IGN);
	oldterm = signal(SIGTERM, SIG_IGN);

	return;
}




    /*  resetsig( )
     *
     *  restores state of interrupts to what they were before ignorsig was
     *  called
     *  uses static variables old-whatever to do the restoration
     *
     *  simply returns
     */


resetsig( )

{
	/* UNIX FUNCTION CALLED */
	extern int	(*signal( ))( );

	signal(SIGINT, oldint);
	signal(SIGHUP, oldhup);
	signal(SIGQUIT, oldquit);
	signal(SIGTERM, oldterm);

	return;
}




    /*  onintr( )
     *
     *  is strip's interrupt handling routine
     *  onintr turns off interrupts while it closes all files that may be open
     *  and unlinks strip's temporary file
     *
     *  onintr always exits fatally
     */


onintr( )

{
	/* UNIX FUNCTIONS CALLED */
	extern int	(*signal( ))( );
	extern		fclose( ),
			unlink( ),
			exit( );

	/* COMMON OBJECT FILE ACCESS ROUTINE CALLED */
	extern int	ldaclose( );

	/* EXTERNAL VARIABLES USED */
	extern LDFILE	*ldptr,
			*fwdptr;
	extern FILE	*strp1,
			*strp2,
			*stripout;
#if AR16WR
	extern FILE	*tempfil,
			*readtmp;
#endif
	extern char	tmpnam1[ ],
			tmpnam2[ ],
			tmpnam3[ ];


	/* ignore signals */
	signal(SIGINT, SIG_IGN);
	signal(SIGHUP, SIG_IGN);
	signal(SIGQUIT, SIG_IGN);
	signal(SIGTERM, SIG_IGN);

	ldaclose(ldptr);
	ldaclose(fwdptr);
	if (strp1 != NULL)
		fclose(strp1);
	if (strp2 != NULL)
		fclose(strp2);
#if AR16WR
	if (tempfil != NULL)
		fclose(tempfil);
	if (readtmp != NULL)
		fclose(readtmp);
#endif
	unlink(tmpnam1);
	unlink(tmpnam2);
	unlink(tmpnam3);
	if (stripout != NULL)
		fclose(stripout);

	exit(FATAL);

}

/*
 *	static char ID[] = "@(#) fcns.c: 1.5 2/13/83";
 */
