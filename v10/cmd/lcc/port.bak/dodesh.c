/* dodesh.f -- translated by f2c (version of 5 February 1993  1:37:14).
   You must link the resulting object file with the libraries:
	-lF77 -lI77 -lm -lc   (in that order)
*/

#include "f2c.h"

/* Subroutine */ int dodesh_(t0, x0, t1, x1, nx, dt, tstop, e)
doublereal *t0, *x0, *t1, *x1;
integer *nx;
doublereal *dt, *tstop;
real *e;
{

/*  THE DEFAULT OUTPUT ROUTINE FOR USE WITH DODES. */
/*  IT SIMPLY RETURNS. */

/*  SCRATCH SPACE ALLOCATED - NONE. */

/*  ERROR STATES - NONE. */


    /* Parameter adjustments */
    --e;
    --x1;
    --x0;

    /* Function Body */
    return 0;

} /* dodesh_ */

