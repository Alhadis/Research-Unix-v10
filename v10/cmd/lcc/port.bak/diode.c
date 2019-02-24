/* diode.f -- translated by f2c (version of 5 February 1993  1:37:14).
   You must link the resulting object file with the libraries:
	-lF77 -lI77 -lm -lc   (in that order)
*/

#include "f2c.h"

/* Subroutine */ int diode_(v, nv, tstart, tstop, dt, d, errpar, handle)
doublereal *v;
integer *nv;
doublereal *tstart, *tstop, *dt;
/* Subroutine */ int (*d) ();
real *errpar;
/* Subroutine */ int (*handle) ();
{
    extern /* Subroutine */ int dioded_(), diodee_(), dioden_();
    extern /* Subroutine */ int dioder_();

/* THE FIRST LEVEL OF DIODE. */
    /* Parameter adjustments */
    --errpar;
    --v;

    /* Function Body */
    dioder_(&v[1], nv, tstart, tstop, dt, d, diodee_, &errpar[1], dioden_, 
	    dioded_, handle);
    return 0;
} /* diode_ */

