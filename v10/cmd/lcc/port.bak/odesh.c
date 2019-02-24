/* odesh.f -- translated by f2c (version of 5 February 1993  1:37:14).
   You must link the resulting object file with the libraries:
	-lF77 -lI77 -lm -lc   (in that order)
*/

#include "f2c.h"

/* Subroutine */ int odesh_(t0, x0, t1, x1, nx, dt, tstop, e)
real *t0, *x0, *t1, *x1;
integer *nx;
real *dt, *tstop, *e;
{

/*  THE DEFAULT OUTPUT ROUTINE FOR USE WITH ODES. */
/*  IT SIMPLY RETURNS. */

/*  SCRATCH SPACE ALLOCATED - NONE. */

/*  ERROR STATES - NONE. */


    /* Parameter adjustments */
    --e;
    --x1;
    --x0;

    /* Function Body */
    return 0;

} /* odesh_ */

