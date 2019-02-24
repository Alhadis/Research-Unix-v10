/* diodes.f -- translated by f2c (version of 5 February 1993  1:37:14).
   You must link the resulting object file with the libraries:
	-lF77 -lI77 -lm -lc   (in that order)
*/

#include "f2c.h"

/* Subroutine */ int diodes_(v, nv, tstart, tstop, dt, d, errpar, handle)
doublereal *v;
integer *nv;
doublereal *tstart, *tstop, *dt;
/* Subroutine */ int (*d) ();
real *errpar;
/* Subroutine */ int (*handle) ();
{
    extern /* Subroutine */ int diode1_();
    extern /* Subroutine */ int diodee_();
    static logical erputs;

/* THE FIRST LEVEL OF DIODES. */
    /* Parameter adjustments */
    --errpar;
    --v;

    /* Function Body */
    erputs = FALSE_;
    diode1_(&v[1], nv, tstart, tstop, dt, d, diodee_, &errpar[1], &erputs, 
	    handle);
    return 0;
} /* diodes_ */

