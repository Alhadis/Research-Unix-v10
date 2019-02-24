
/*
 *
 * A few general purpose routines that can be used with any of the built
 * from these files.
 *
 */


#include <stdio.h>
#include <ctype.h>

#include "gen.h"			/* a few general purpose definitions */
#include "ext.h"			/* external variable declarations */


int	nolist = 0;			/* number of specified ranges */
int	olist[30];			/* processing range pairs */


/*****************************************************************************/


putint(n, fp)


    int		n;			/* number we want written out */
    FILE	*fp;			/* as next two bytes in this file */


{


/*
 *
 * Makes sure the value stored in the lower 16 bits of integer n is written
 * as the next two bytes in file *fp. Used to write Impress commands that
 * have 'word' values.
 *
 */


    putc(n >> BYTE, fp);
    putc(n & BMASK, fp);

}   /* End of putint */


/*****************************************************************************/


out_list(str)


    char	*str;			/* process ranges in this string */


{


    int		start, stop;		/* range end points */


/*
 *
 * Called to get the processing ranges that were specified by using the
 * -o option. Hopefully the range syntax is identical to the one used in
 * troff and nroff. Depending on the program the ranges may be used to
 * select pages we want printed or possibly glyphs that we want to display.
 *
 */


    while ( *str  &&  nolist < sizeof(olist) - 2 )  {
	start = stop = str_convert(&str, 0);

	if ( *str == '-'  &&  *str++ )
	    stop = str_convert(&str, 9999);

	if ( start > stop )
	    error(FATAL, "illegal range %d-%d", start, stop);

	olist[nolist++] = start;
	olist[nolist++] = stop;

	if ( *str != '\0' ) str++;

    }	/* End while */

    olist[nolist] = 0;

}   /* End of out_list */


/*****************************************************************************/


in_olist(num)


    int		num;			/* should we process this guy? */


{


    int		i;			/* just a loop index */


/*
 *
 * If num (may be a page or glyph) should be processed ON will be returned,
 * otherwise OFF is returned. If no ranges were initially selected nolist
 * will be zero and we'll assume everything is supposed to be processed.
 *
 */


    if ( nolist == 0 )			/* everything's included */
	return(ON);

    for ( i = 0; i < nolist; i += 2 )
	if ( num >= olist[i]  &&  num <= olist[i+1] )
	    return(ON);

    return(OFF);

}   /* End of in_olist */


/*****************************************************************************/


str_convert(str, err)


    char	**str;			/* get next number from this string */
    int		err;			/* value returned on error */


{


    int		i;			/* just a loop index */


/*
 *
 * Gets the next integer from **str and returns its value to the caller.
 * If **str isn't an integer err is returned. *str is updated after each
 * digit is processed.
 *
 */


    if ( ! isdigit(**str) )		/* something's wrong */
	return(err);

    for ( i = 0; isdigit(**str); *str += 1 )
	i = 10 * i + **str - '0';

    return(i);

}   /* End of str_convert */


/*****************************************************************************/


error(kind, mesg, a1, a2, a3)


    int		kind;			/* FATAL or NON_FATAL error */
    char	*mesg;			/* error message control string */
    unsigned	a1, a2, a3;		/* control string arguments */


{


/*
 *
 * Called when we've run into some kind of program error. First *mesg is
 * printed using the control string arguments a?. Then if kind is FATAL
 * and we're not ignoring errors the program will be terminated. Probably
 * should call a routine to clean things up for whatever program is using
 * this routine. Could use signal() to figure out who, if anyone, should
 * be called.
 *
 * If mesg is NULL or *mesg is the NULL string nothing will be printed.
 *
 */


    if ( mesg != NULL  &&  *mesg != '\0' )  {
	fprintf(stderr, "%s: ", prog_name);
	fprintf(stderr, mesg, a1, a2, a3);
	if ( lineno > 0 )
	    fprintf(stderr, " (line %d)", lineno);
	putc('\n', stderr);
    }	/* End if */

    if ( kind == FATAL  &&  ignore == OFF )
	exit(x_stat == 0 ? 01 : x_stat);

}   /* End of error */


/*****************************************************************************/


