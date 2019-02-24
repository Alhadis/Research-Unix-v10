/* dot.f -- translated by f2c (version of 17 January 1992  0:17:58).
   You must link the resulting object file with the libraries:
	-lF77 -lI77 -lm -lc   (in that order)
*/

#include "f2c.h"

doublereal dot_(n, x, y)
integer *n;
real *x, *y;
{
    /* System generated locals */
    integer i__1;
    real ret_val;

    /* Local variables */
    static integer i;

    /* Parameter adjustments */
    --y;
    --x;

    /* Function Body */
    ret_val = (float)0.;
    i__1 = *n;
    for (i = 1; i <= i__1; ++i) {
/* L10: */
	ret_val += x[i] * y[i];
    }
    return ret_val;
} /* dot_ */

