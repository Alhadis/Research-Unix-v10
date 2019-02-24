/* iode.f -- translated by f2c (version of 5 February 1993  1:37:14).
   You must link the resulting object file with the libraries:
	-lF77 -lI77 -lm -lc   (in that order)
*/

#include "f2c.h"

/* Subroutine */ int iode_(v, nv, tstart, tstop, dt, d, errpar, handle)
real *v;
integer *nv;
real *tstart, *tstop, *dt;
/* Subroutine */ int (*d) ();
real *errpar;
/* Subroutine */ int (*handle) ();
{
    extern /* Subroutine */ int ioded_(), iodee_(), ioden_();
    extern /* Subroutine */ int ioder_();

/* THE FIRST LEVEL OF IODE. */
    /* Parameter adjustments */
    --errpar;
    --v;

    /* Function Body */
    ioder_(&v[1], nv, tstart, tstop, dt, d, iodee_, &errpar[1], ioden_, 
	    ioded_, handle);
    return 0;
} /* iode_ */

