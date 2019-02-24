/* iode3.f -- translated by f2c (version of 5 February 1993  1:37:14).
   You must link the resulting object file with the libraries:
	-lF77 -lI77 -lm -lc   (in that order)
*/

#include "f2c.h"

/* Common Block Declarations */

extern struct {
    doublereal ds[500];
} cstak_;

#define cstak_1 cstak_

/* Table of constant values */

static integer c__1 = 1;
static integer c__2 = 2;

/* Subroutine */ int iode3_(v, nv, tstart, tstop, dt, d, theta, keejac, minit,
	 maxit, kmax, xpoly, kinit, error, errpar, erputs, inmi, da, handle)
real *v;
integer *nv;
real *tstart, *tstop, *dt;
/* Subroutine */ int (*d) ();
real *theta;
integer *keejac, *minit, *maxit, *kmax;
logical *xpoly;
integer *kinit;
/* Subroutine */ int (*error) ();
real *errpar;
logical *erputs;
/* Subroutine */ int (*inmi) (), (*da) (), (*handle) ();
{
    /* Initialized data */

    static logical usengj = FALSE_;
    static logical usenns = FALSE_;
    static logical usenfd = FALSE_;

    /* System generated locals */
    integer i__1;

    /* Local variables */
    static real beta;
    static integer ircs, mmax, temp;
    extern /* Subroutine */ int iode4_();
    static integer temp1, i;
    static real gamma, delta;
    extern /* Subroutine */ int leave_();
    static real egive;
    extern /* Subroutine */ int enter_();
    static integer in;
#define is ((integer *)&cstak_1)
#define ls ((logical *)&cstak_1)
#define rs ((real *)&cstak_1)
    static real hfract;
#define ws ((real *)&cstak_1)
    extern integer istkgt_();

    /* Parameter adjustments */
    --errpar;
    --v;

    /* Function Body */
/* THE FOURTH LEVEL OF IODES. */
/* SCRATCH SPACE ALLOCATED - */
/*     S( IODE3) = S(A1ODES) + */
/* REAL WORDS + */
/*                 MMAX */
/* INTEGER WORDS. */
/* THE PORT LIBRARY STACK AND ITS ALIASES. */
    if (*tstart == *tstop) {
	return 0;
    }
    enter_(&c__1);
/* CHECK THE INPUT FOR ERRORS. */
    mmax = *kmax + 5;
    beta = (float)1.;
    if (*theta == (float).5) {
	goto L1;
    }
    gamma = (float)1.;
    goto L2;
L1:
    gamma = (float)2.;
L2:
    if (! (*erputs)) {
	goto L3;
    }
    delta = (float)1.;
    goto L4;
L3:
    delta = (float)0.;
/* GET N. */
L4:
    in = istkgt_(&mmax, &c__2);
    is[in - 1] = 1;
    is[in] = 2;
    is[in + 1] = 3;
    i = 4;
    goto L6;
L5:
    ++i;
L6:
    if (i > mmax) {
	goto L7;
    }
    temp1 = in + i;
    temp = in + i;
    is[temp1 - 2] = is[temp - 4] << 1;
    goto L5;
L7:
    if (*theta != (float).5) {
	goto L9;
    }
    hfract = (float).5;
    i__1 = mmax;
    for (i = 1; i <= i__1; ++i) {
	temp = in + i;
	is[temp - 2] <<= 1;
/* L8: */
    }
    goto L10;
L9:
    hfract = (float)1.;
L10:
    egive = (float)100.;
    ircs = 1;
    iode4_(&v[1], nv, tstart, tstop, dt, d, theta, keejac, minit, maxit, &
	    usengj, &usenns, &usenfd, &beta, &gamma, &delta, &is[in - 1], &
	    mmax, &hfract, &egive, kmax, xpoly, kinit, &ircs, error, &errpar[
	    1], erputs, inmi, da, handle);
    leave_();
    return 0;
} /* iode3_ */

#undef ws
#undef rs
#undef ls
#undef is


