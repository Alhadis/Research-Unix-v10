/* d0desu.f -- translated by f2c (version of 5 February 1993  1:37:14).
   You must link the resulting object file with the libraries:
	-lF77 -lI77 -lm -lc   (in that order)
*/

#include "f2c.h"

/* Subroutine */ int d0desu_(x, nx, glbmax)
doublereal *x;
integer *nx;
real *glbmax;
{
    /* System generated locals */
    integer i__1;
    real r__1, r__2, r__3;

    /* Local variables */
    static integer i;


/*  TO RAISE THE GLOBAL MAXIMUM. */


    /* Parameter adjustments */
    --glbmax;
    --x;

    /* Function Body */
    i__1 = *nx;
    for (i = 1; i <= i__1; ++i) {
/* L10: */
/* Computing MAX */
	r__2 = glbmax[i], r__3 = (r__1 = x[i], dabs(r__1));
	glbmax[i] = dmax(r__2,r__3);
    }

    return 0;

} /* d0desu_ */

