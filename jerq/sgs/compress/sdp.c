/*
 *	static char ID_sdp[] = "@(#) sdp.c: 1.1 1/8/82";
 */

#include <stdio.h>
#include <fatal.h>
#include <signal.h>

/*  define dummy variables used by ld (may be useful later) */
long	SDP_read = 0;
long	SDP_write= 0;
long	LD_lock  = 0;
long	LD_unlock= 0;
int	dflag	 = 0;

extern char	*sname();
#include "syms.h"
#include "tagitem.h"
#include "sdpsrc/hd/define2.h"
#include "sdpsrc/hd/define3.h"
#include "sdpsrc/hd/define4.h"
#include "sdpsrc/hd/struct.h"
#include "sdpsrc/hd/globals.h"

#include "sdpsrc/lo/delist.c"
#include "sdpsrc/lo/enlist.c"
#include "sdpsrc/lo/fatal.c"
#include "sdpsrc/lo/getnxtpage.c"
#include "sdpsrc/lo/readpage.c"
#include "sdpsrc/lo/sojourn.c"
#include "sdpsrc/lo/repartition.c"
#include "sdpsrc/lo/writeframe.c"

#include "sdpsrc/hi/allocate.c"
#include "sdpsrc/hi/flush.c"
#include "sdpsrc/hi/init.c"
#include "sdpsrc/hi/lock.c"
#include "sdpsrc/hi/unlock.c"
lderror(lvl, ln,fln,strng, a1,a2,a3,a4,a5,a6,a7,a8,a9,aa)
int lvl;		/* error severity level */
int ln;			/* line in ifile containing the error */
char *fln;		/* name of the ifile containing the error */
char *strng;		/* error message format */
int a1,a2,a3,a4,a5,	/* error message format arguments */
    a6,a7,a8,a9,aa;
{

/*
 *  Prepend a filename and a line number to an error message.
 *  used for printing all parsing-related errors.
 */


	if (lvl != 2)
		return;

	if (fln != NULL)
		fprintf(stderr, " %s", sname(fln));
	if ( ln > 0 )
		fprintf(stderr, " %d", ln) ;

	switch(lvl) {
	case 0:
		fprintf(stderr, " warning: ");
		break;
	case 2:
		fprintf(stderr, " fatal: ");
		break;
	default:
		fprintf(stderr, ": ");
		break;
	}

	fprintf(stderr, strng, a1, a2, a3, a4, a5, a6, a7, a8, a9, aa);
	fprintf(stderr, "\n");
	fflush(stderr);

	if(lvl > 1)
		exit();
}
char *
sname(s)
char *s;
{

/*
 * Returns pointer to "simple" name of path name; that is,
 * pointer to first character after last '/'.  If no slashes,
 * returns pointer to first char of arg.
 *
 * If the string ends in a slash:
 *	1. replace the terminating slash with '\0'
 *	2. return a pointer to the first character after the last
 *		'/', or the first character in the string
 */

	register char *p;

	p = s;
	while( *p )
		if(*p++ == '/')
			if( *p == '\0' ) {
				*(p-1) = '\0';
				break;
				}
			else
				s = p;

	return(s);
}
