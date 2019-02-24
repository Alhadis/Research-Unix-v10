/* d1odee.f -- translated by f2c (version of 5 February 1993  1:37:14).
   You must link the resulting object file with the libraries:
	-lF77 -lI77 -lm -lc   (in that order)
*/

#include "f2c.h"

logical d1odee_(v, nv, t, dt, errpar, delta, ev, error)
doublereal *v;
integer *nv;
doublereal *t, *dt;
real *errpar;
doublereal *delta;
real *ev;
logical (*error) ();
{
    /* System generated locals */
    logical ret_val;

    /* Local variables */
    static logical erputs;

/* THE ERROR FILTER FOR NLEQS. */
/* SCRATCH SPACE ALLOCATED - S(D1ODEE) = S(ERROR). */
    /* Parameter adjustments */
    --ev;
    --errpar;
    --v;

    /* Function Body */
    erputs = *delta == 1.;
    ret_val = (*error)(&v[1], nv, t, dt, &errpar[1], &erputs, &ev[1]);
    return ret_val;
} /* d1odee_ */

