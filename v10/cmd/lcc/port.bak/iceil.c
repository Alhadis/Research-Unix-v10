/* iceil.f -- translated by f2c (version of 5 February 1993  1:37:14).
   You must link the resulting object file with the libraries:
	-lF77 -lI77 -lm -lc   (in that order)
*/

#include "f2c.h"

integer iceil_(x)
real *x;
{
    /* System generated locals */
    integer ret_val;


/*  ICEIL RETURNS CEIL(X) */

    ret_val = (integer) (*x);
    if (*x <= (float)0.) {
	return ret_val;
    }
    if ((real) ret_val != *x) {
	++ret_val;
    }

    return ret_val;
} /* iceil_ */

