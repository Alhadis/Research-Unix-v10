/* ceil.f -- translated by f2c (version of 5 February 1993  1:37:14).
   You must link the resulting object file with the libraries:
	-lF77 -lI77 -lm -lc   (in that order)
*/

#include "f2c.h"

doublereal ceil_(x)
real *x;
{
    /* System generated locals */
    real ret_val;


/*  CEIL RETURNS CEIL(X) */

    ret_val = (real) ((integer) (*x));
    if (*x <= (float)0.) {
	return ret_val;
    }
    if (ret_val != *x) {
	ret_val += (float)1.;
    }

    return ret_val;
} /* ceil_ */

