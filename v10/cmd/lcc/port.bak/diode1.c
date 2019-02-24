/* diode1.f -- translated by f2c (version of 5 February 1993  1:37:14).
   You must link the resulting object file with the libraries:
	-lF77 -lI77 -lm -lc   (in that order)
*/

#include "f2c.h"

/* Subroutine */ int diode1_(v, nv, tstart, tstop, dt, d, error, errpar, 
	erputs, handle)
doublereal *v;
integer *nv;
doublereal *tstart, *tstop, *dt;
/* Subroutine */ int (*d) (), (*error) ();
real *errpar;
logical *erputs;
/* Subroutine */ int (*handle) ();
{
    static integer kmax, kinit;
    static logical equil;
    extern /* Subroutine */ int diode2_();
    static logical xpoly;

/* THE SECOND LEVEL OF DIODES. */
    /* Parameter adjustments */
    --errpar;
    --v;

    /* Function Body */
    kmax = 10;
    xpoly = FALSE_;
    kinit = 2;
    equil = TRUE_;
    diode2_(&v[1], nv, tstart, tstop, dt, d, &equil, &kmax, &xpoly, &kinit, 
	    error, &errpar[1], erputs, handle);
    return 0;
} /* diode1_ */

