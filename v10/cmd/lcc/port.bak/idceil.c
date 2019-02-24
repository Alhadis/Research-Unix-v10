/* idceil.f -- translated by f2c (version of 5 February 1993  1:37:14).
   You must link the resulting object file with the libraries:
	-lF77 -lI77 -lm -lc   (in that order)
*/

#include "f2c.h"

integer idceil_(x)
doublereal *x;
{
    /* System generated locals */
    integer ret_val;


/*  IDCEIL RETURNS CEIL(X) */


    ret_val = (integer) (*x);
    if (*x <= 0.) {
	return ret_val;
    }
    if ((doublereal) ((real) ret_val) != *x) {
	++ret_val;
    }

    return ret_val;
} /* idceil_ */

