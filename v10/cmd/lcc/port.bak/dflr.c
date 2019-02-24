/* dflr.f -- translated by f2c (version of 5 February 1993  1:37:14).
   You must link the resulting object file with the libraries:
	-lF77 -lI77 -lm -lc   (in that order)
*/

#include "f2c.h"

doublereal dflr_(x)
doublereal *x;
{
    /* System generated locals */
    doublereal ret_val;


/*  DFLR RETURNS FLR(X) */


    ret_val = (doublereal) ((real) ((integer) (*x)));
    if (*x >= 0.) {
	return ret_val;
    }
    if (ret_val != *x) {
	ret_val += -1.;
    }

    return ret_val;
} /* dflr_ */

