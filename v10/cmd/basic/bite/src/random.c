/* Copyright Bell Telephone Laboratories Whippany, N.J.

 *	/////////////////////////////////////
 *	/////////////////////////////////////
 *	///////////// random.c //////////////
 *	/// J. P. Hawkins WH X4610 8C-001 ///
 *	///// Fri Aug 24 17:31:02 1979 //////
 *	/////////////////////////////////////
 *	/////////////////////////////////////

 * Pland a random seed (RANDOMIZE COMMAND)
 */
/*   "@(#) random.c:  V 1.3  3/22/81" */
#include	"bas.h"

#ifdef	LSX
extern	int	ticks;		/* LTC 1/60th sec counter */
#endif

_random()
{
#ifndef	LSX
	long tm;
	time(&tm);
	srand(tm);	/* seed random gen with system time */
#endif
#ifdef	LSX
	srand(ticks);	/* seed with current clock for LSX */
#endif
}
