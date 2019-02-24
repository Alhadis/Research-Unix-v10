/* d1odep.f -- translated by f2c (version of 5 February 1993  1:37:14).
   You must link the resulting object file with the libraries:
	-lF77 -lI77 -lm -lc   (in that order)
*/

#include "f2c.h"

/* Common Block Declarations */

extern struct {
    doublereal wv[30];
    real rv[30];
    integer iv[40];
    logical lv[20];
} d10dey_;

#define d10dey_1 d10dey_

extern struct {
    doublereal tj, dtj;
    logical getjac, sepate, usengj, usenns, usenfd;
} d1odeg_;

#define d1odeg_1 d1odeg_

extern struct {
    doublereal theta;
    real egive;
    integer minit, maxit, keejac, ircs;
} d1odem_;

#define d1odem_1 d1odem_

extern struct {
    integer imem;
} d1odes_;

#define d1odes_1 d1odes_

extern struct {
    integer njs, nfs, ntss, nsss, nes, nnits, nnds, nnfs, nrs;
} d10der_;

#define d10der_1 d10der_

extern struct {
    integer fnum;
} d1odef_;

#define d1odef_1 d1odef_

/* Table of constant values */

static integer c__1 = 1;
static integer c__0 = 0;
static integer c_n2 = -2;

/* Subroutine */ int d1odep_(t0, v0, t1, v1, nv, n, nleqs, d, da, ok, error, 
	errpar, inmi)
doublereal *t0, *v0, *t1, *v1;
integer *nv, *n;
logical (*nleqs) ();
/* Subroutine */ int (*d) ();
logical (*da) ();
logical *ok;
/* Subroutine */ int (*error) ();
real *errpar;
/* Subroutine */ int (*inmi) ();
{
    /* System generated locals */
    integer i__1;

    /* Local variables */
    static doublereal t;
    extern /* Subroutine */ int leave_(), enter_();
    static integer istep;
    static doublereal dt;
    extern /* Subroutine */ int movefd_();
#define tstart ((doublereal *)&d10dey_1)

/* TIME-STEPPING SCHEME FOR IODES. */
/* SCRATCH SPACE ALLOCATED - */
/*     S(D1ODEP) = NV**2 + S(NLEQS). */
/* LONG REAL WORDS. */
    /* Parameter adjustments */
    --errpar;
    --v1;
    --v0;

    /* Function Body */
    enter_(&c__1);
    dt = (*t1 - *t0) / (doublereal) ((real) (*n));
/* INITIAL APPROXIMATION FOR V1. */
    movefd_(nv, &v0[1], &v1[1]);
    if ((*da)(&v0[1], &v1[1], nv, t1, &dt, d, &v1[1], &d1odeg_1.tj, &
	    d1odeg_1.dtj, &d1odeg_1.getjac, &d1odeg_1.sepate, &
	    d1odeg_1.usengj, &d1odeg_1.usenns, &d1odeg_1.usenfd, &c__0, &c_n2,
	     &d1odef_1.fnum, &d1odem_1.theta, &d1odem_1.ircs, &
	    d1odem_1.keejac)) {
    }
    i__1 = *n;
    for (istep = 1; istep <= i__1; ++istep) {
	t = *t0 + ((doublereal) ((real) (istep - 1)) + d1odem_1.theta) * dt;
	++d10der_1.nsss;
	if (d1odem_1.keejac != 1) {
	    goto L1;
	}
	d1odeg_1.getjac = TRUE_;
	d1odeg_1.tj = t;
L1:
	if (dt > 0. && t > *t1 || dt < 0. && t < *t1) {
	    t = *t1;
	}
	*ok = (*nleqs)(&v1[1], nv, &t, &dt, d, da, error, inmi, &errpar[1]);
	if (d1odef_1.fnum < 3 || d1odem_1.keejac != 4) {
	    goto L2;
	}
	d1odeg_1.getjac = *t0 != d1odeg_1.tj;
	d1odeg_1.tj = *t0;
L2:
	if (! (*ok)) {
	    goto L4;
	}
/* L3: */
    }
L4:
    leave_();
    return 0;
} /* d1odep_ */

#undef tstart


