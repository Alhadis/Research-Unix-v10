/* iodes.f -- translated by f2c (version of 5 February 1993  1:37:14).
   You must link the resulting object file with the libraries:
	-lF77 -lI77 -lm -lc   (in that order)
*/

#include "f2c.h"

/* Subroutine */ int iodes_(v, nv, tstart, tstop, dt, d, errpar, handle)
real *v;
integer *nv;
real *tstart, *tstop, *dt;
/* Subroutine */ int (*d) ();
real *errpar;
/* Subroutine */ int (*handle) ();
{
    extern /* Subroutine */ int iode1_();
    extern /* Subroutine */ int iodee_();
    static logical erputs;

/* THE FIRST LEVEL OF IODES. */
    /* Parameter adjustments */
    --errpar;
    --v;

    /* Function Body */
    erputs = FALSE_;
    iode1_(&v[1], nv, tstart, tstop, dt, d, iodee_, &errpar[1], &erputs, 
	    handle);
    return 0;
} /* iodes_ */

