/* a1odep.f -- translated by f2c (version of 5 February 1993  1:37:14).
   You must link the resulting object file with the libraries:
	-lF77 -lI77 -lm -lc   (in that order)
*/

#include "f2c.h"

/* Common Block Declarations */

extern struct {
    real wv[30], rv[30];
    integer iv[40];
    logical lv[20];
} a10dey_;

#define a10dey_1 a10dey_

extern struct {
    real tj, dtj;
    logical getjac, sepate, usengj, usenns, usenfd;
} a1odeg_;

#define a1odeg_1 a1odeg_

extern struct {
    real theta, egive;
    integer minit, maxit, keejac, ircs;
} a1odem_;

#define a1odem_1 a1odem_

extern struct {
    integer imem;
} a1odes_;

#define a1odes_1 a1odes_

extern struct {
    integer njs, nfs, ntss, nsss, nes, nnits, nnds, nnfs, nrs;
} a10der_;

#define a10der_1 a10der_

extern struct {
    integer fnum;
} a1odef_;

#define a1odef_1 a1odef_

/* Table of constant values */

static integer c__1 = 1;
static integer c__0 = 0;
static integer c_n2 = -2;

/* Subroutine */ int a1odep_(t0, v0, t1, v1, nv, n, nleqs, d, da, ok, error, 
	errpar, inmi)
real *t0, *v0, *t1, *v1;
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
    static real t;
    extern /* Subroutine */ int leave_(), enter_();
    static integer istep;
    static real dt;
    extern /* Subroutine */ int movefr_();
#define tstart ((real *)&a10dey_1)

/* TIME-STEPPING SCHEME FOR IODES. */
/* SCRATCH SPACE ALLOCATED - */
/*     S(A1ODEP) = NV**2 + S(NLEQS). */
/* REAL WORDS. */
    /* Parameter adjustments */
    --errpar;
    --v1;
    --v0;

    /* Function Body */
    enter_(&c__1);
    dt = (*t1 - *t0) / (real) (*n);
/* INITIAL APPROXIMATION FOR V1. */
    movefr_(nv, &v0[1], &v1[1]);
    if ((*da)(&v0[1], &v1[1], nv, t1, &dt, d, &v1[1], &a1odeg_1.tj, &
	    a1odeg_1.dtj, &a1odeg_1.getjac, &a1odeg_1.sepate, &
	    a1odeg_1.usengj, &a1odeg_1.usenns, &a1odeg_1.usenfd, &c__0, &c_n2,
	     &a1odef_1.fnum, &a1odem_1.theta, &a1odem_1.ircs, &
	    a1odem_1.keejac)) {
    }
    i__1 = *n;
    for (istep = 1; istep <= i__1; ++istep) {
	t = *t0 + ((real) (istep - 1) + a1odem_1.theta) * dt;
	++a10der_1.nsss;
	if (a1odem_1.keejac != 1) {
	    goto L1;
	}
	a1odeg_1.getjac = TRUE_;
	a1odeg_1.tj = t;
L1:
	if (dt > (float)0. && t > *t1 || dt < (float)0. && t < *t1) {
	    t = *t1;
	}
	*ok = (*nleqs)(&v1[1], nv, &t, &dt, d, da, error, inmi, &errpar[1]);
	if (a1odef_1.fnum < 3 || a1odem_1.keejac != 4) {
	    goto L2;
	}
	a1odeg_1.getjac = *t0 != a1odeg_1.tj;
	a1odeg_1.tj = *t0;
L2:
	if (! (*ok)) {
	    goto L4;
	}
/* L3: */
    }
L4:
    leave_();
    return 0;
} /* a1odep_ */

#undef tstart


