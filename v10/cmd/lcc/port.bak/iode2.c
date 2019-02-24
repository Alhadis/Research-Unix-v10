/* iode2.f -- translated by f2c (version of 5 February 1993  1:37:14).
   You must link the resulting object file with the libraries:
	-lF77 -lI77 -lm -lc   (in that order)
*/

#include "f2c.h"

/* Subroutine */ int iode2_(v, nv, tstart, tstop, dt, d, equil, kmax, xpoly, 
	kinit, error, errpar, erputs, handle)
real *v;
integer *nv;
real *tstart, *tstop, *dt;
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
    extern /* Subroutine */ int iode3_();
    extern /* Subroutine */ int ioded_(), ioden_();
    static real theta;
    static integer minit, maxit, keejac;

/* THE THIRD LEVEL OF IODES. */
/* CHECK THE INPUT FOR ERRORS. */
    /* Parameter adjustments */
    --errpar;
    --v;

    /* Function Body */
    if (! (*equil)) {
	goto L1;
    }
    theta = (float)1.;
    goto L2;
L1:
    theta = (float).5;
L2:
    keejac = 0;
    minit = 10;
    maxit = 15;
    iode3_(&v[1], nv, tstart, tstop, dt, d, &theta, &keejac, &minit, &maxit, 
	    kmax, xpoly, kinit, error, &errpar[1], erputs, ioden_, ioded_, 
	    handle);
    return 0;
} /* iode2_ */

