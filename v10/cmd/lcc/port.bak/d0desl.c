/* d0desl.f -- translated by f2c (version of 5 February 1993  1:37:14).
   You must link the resulting object file with the libraries:
	-lF77 -lI77 -lm -lc   (in that order)
*/

#include "f2c.h"

/* Subroutine */ int d0desl_(x, nx, glbmax, glbmxo)
doublereal *x;
integer *nx;
real *glbmax, *glbmxo;
{
    /* System generated locals */
    integer i__1;
    real r__1;

    /* Local variables */
    static integer i;


/*  TO LOAD THE GLOBAL MAXIMUM. */


    /* Parameter adjustments */
    --glbmxo;
    --glbmax;
    --x;

    /* Function Body */
    i__1 = *nx;
    for (i = 1; i <= i__1; ++i) {
	glbmax[i] = (r__1 = x[i], dabs(r__1));
/* L10: */
	glbmxo[i] = glbmax[i];
    }

    return 0;

} /* d0desl_ */

