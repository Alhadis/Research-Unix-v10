/* i10wid.f -- translated by f2c (version of 5 February 1993  1:37:14).
   You must link the resulting object file with the libraries:
	-lF77 -lI77 -lm -lc   (in that order)
*/

#include "f2c.h"

integer i10wid_(ix)
integer *ix;
{
    /* System generated locals */
    integer ret_val;

    /* Local variables */
    static integer iy, digits;

/*     THIS FUNCTION RETURNS THE NUMBER OF DECIMAL */
/*     DIGITS REQUIRED TO REPRESENT THE INTEGER, IX. */
    digits = 0;
    iy = abs(*ix);
L1:
    if (iy < 1) {
	goto L2;
    }
    ++digits;
    iy /= 10;
    goto L1;
L2:
    ret_val = digits;
    return ret_val;
} /* i10wid_ */

