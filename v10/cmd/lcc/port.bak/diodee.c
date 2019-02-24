/* diodee.f -- translated by f2c (version of 5 February 1993  1:37:14).
   You must link the resulting object file with the libraries:
	-lF77 -lI77 -lm -lc   (in that order)
*/

#include "f2c.h"

logical diodee_(v, nv, t, dt, errpar, erputs, ev)
doublereal *v;
integer *nv;
doublereal *t, *dt;
real *errpar;
logical *erputs;
real *ev;
{
    /* System generated locals */
    integer i__1;
    doublereal d__1;
    logical ret_val;

    /* Local variables */
    static real temp;
    static integer i;
    static real dtpow;
    static logical conged;

/* THE STANDARD ERROR PROCEDURE FOR DIODES. */
/* SCRATCH SPACE ALLOCATED - NONE. */
    /* Parameter adjustments */
    --ev;
    --errpar;
    --v;

    /* Function Body */
    if (! (*erputs)) {
	goto L1;
    }
    dtpow = abs(*dt);
    goto L2;
L1:
    dtpow = (float)1.;
L2:
    conged = TRUE_;
/* ERROR FOR V. */
    i__1 = *nv;
    for (i = 1; i <= i__1; ++i) {
	temp = dtpow * (errpar[1] * (d__1 = v[i], abs(d__1)) + errpar[2]);
	if (temp < ev[i]) {
	    conged = FALSE_;
	}
	ev[i] = temp;
/* L3: */
    }
    ret_val = conged;
    return ret_val;
} /* diodee_ */

