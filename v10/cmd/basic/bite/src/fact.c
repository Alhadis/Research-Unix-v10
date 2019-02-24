
/* Copyright Bell Telephone Laboratories Whippany, N.J.

 *	/////////////////////////////////////
 *	/////////////////////////////////////
 *	////////////// fact.c ///////////////
 *	/// J. P. Hawkins WH X4610 8C-001 ///
 *	///// Tue Apr 21 16:39:20 1981 //////
 *	/////////////////////////////////////
 *	/////////////////////////////////////
 *	@(#) fact.c: V1.2 4/22/81
 * Factorial function

*/

#include	"bas.h"
double
fact(n)
double  n;
{
        long    l;
        double  fact();
        l = n;
	if(l>33 || l<0L)
	{
		error(inst.thing.linno, 56);
		return(0.0);
	}
        if(l == 0 || l == 1)
                return(1.0);
        else
                return(l*fact(l-1.0));
}
