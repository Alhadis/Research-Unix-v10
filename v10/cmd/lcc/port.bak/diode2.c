/* diode2.f -- translated by f2c (version of 5 February 1993  1:37:14).
   You must link the resulting object file with the libraries:
	-lF77 -lI77 -lm -lc   (in that order)
*/

#include "f2c.h"

/* Subroutine */ int diode2_(v, nv, tstart, tstop, dt, d, equil, kmax, xpoly, 
	kinit, error, errpar, erputs, handle)
doublereal *v;
integer *nv;
doublereal *tstart, *tstop, *dt;
/* Subroutine */ int (*d) ();
logical *equil;
integer *kmax;
logical *xpoly;
integer *kinit;
/* Subroutine */ int (*error) ();
real *errpar;
logical *erputs;
/* Subroutine */ int (*handle) ();
{
    static doublereal theta;
    static integer minit, maxit;
    extern /* Subroutine */ int diode3_();
    static integer keejac;
    extern /* Subroutine */ int dioded_(), dioden_();

/* THE THIRD LEVEL OF DIODES. */
/* CHECK THE INPUT FOR ERRORS. */
    /* Parameter adjustments */
    --errpar;
    --v;

    /* Function Body */
    if (! (*equil)) {
	goto L1;
    }
    theta = 1.;
    goto L2;
L1:
    theta = .5;
L2:
    keejac = 0;
    minit = 10;
    maxit = 15;
    diode3_(&v[1], nv, tstart, tstop, dt, d, &theta, &keejac, &minit, &maxit, 
	    kmax, xpoly, kinit, error, &errpar[1], erputs, dioden_, dioded_, 
	    handle);
    return 0;
} /* diode2_ */

