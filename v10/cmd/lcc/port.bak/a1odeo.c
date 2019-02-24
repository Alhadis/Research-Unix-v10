/* a1odeo.f -- translated by f2c (version of 5 February 1993  1:37:14).
   You must link the resulting object file with the libraries:
	-lF77 -lI77 -lm -lc   (in that order)
*/

#include "f2c.h"

/* Common Block Declarations */

extern struct {
    integer njs, nfs, ntss, nsss, nes, nnits, nnds, nnfs, nrs;
} a10der_;

#define a10der_1 a10der_

extern struct {
    real tj, dtj;
    logical getjac, sepate, usengj, usenns, usenfd;
} a1odeg_;

#define a1odeg_1 a1odeg_

extern struct {
    logical erputs;
} a1odel_;

#define a1odel_1 a1odel_

extern struct {
    integer fnum;
} a1odef_;

#define a1odef_1 a1odef_

/* Table of constant values */

static real c_b2 = (float)0.;
static integer c__37 = 37;
static integer c__19 = 19;
static integer c__1 = 1;

logical a1odeo_(v, nv, t, dt, d, da, error, inmi, errpar, vtheta, vt, vold, 
	ev, ev1, ev2, dv, theta, minit, maxit, keejac, ircs, egive)
real *v;
integer *nv;
real *t, *dt;
/* Subroutine */ int (*d) ();
logical (*da) (), (*error) ();
/* Subroutine */ int (*inmi) ();
real *errpar, *vtheta, *vt, *vold, *ev, *ev1, *ev2, *dv, *theta;
integer *minit, *maxit, *keejac, *ircs;
real *egive;
{
    /* System generated locals */
    integer i__1, i__2, i__3;
    real r__1;
    logical ret_val;

    /* Builtin functions */
    double pow_ri();

    /* Local variables */
    static logical done;
    static integer iter;
    static real prod, temp;
    extern /* Subroutine */ int setr_();
    static integer temp1;
    static logical temp2;
    static integer i, j;
    static real power;
    extern /* Subroutine */ int movefr_(), seterr_();
    static real rho;

    /* Parameter adjustments */
    --dv;
    --ev2;
    --ev1;
    --ev;
    --vold;
    --vt;
    --vtheta;
    --errpar;
    --v;

    /* Function Body */
    movefr_(nv, &v[1], &vold[1]);
    setr_(nv, &c_b2, &vt[1]);
/* GET INITIAL NEWTON METHOD GUESS. */
    (*inmi)(nv, t, dt, &vold[1], &v[1], &vt[1]);
    i__1 = *maxit;
    for (iter = 1; iter <= i__1; ++iter) {
	if (*keejac != 0) {
	    goto L1;
	}
	a1odeg_1.getjac = TRUE_;
	a1odeg_1.tj = *t;
L1:
	if (a1odeg_1.getjac) {
	    ++a10der_1.njs;
	}
	++a10der_1.nnits;
	i__2 = *nv;
	for (i = 1; i <= i__2; ++i) {
	    vtheta[i] = *theta * (v[i] - vold[i]) + vold[i];
/* L2: */
	}
	if (a1odeg_1.getjac) {
	    a1odeg_1.dtj = (float)0.;
	}
	done = (*da)(&vtheta[1], &vt[1], nv, t, dt, d, &dv[1], &a1odeg_1.tj, &
		a1odeg_1.dtj, &a1odeg_1.getjac, &a1odeg_1.sepate, &
		a1odeg_1.usengj, &a1odeg_1.usenns, &a1odeg_1.usenfd, &
		a10der_1.nes, &a10der_1.nfs, &a1odef_1.fnum, theta, ircs, 
		keejac);
	if (! done) {
	    goto L3;
	}
	done = FALSE_;
	goto L24;
L3:
	a1odeg_1.getjac = FALSE_;
	a1odeg_1.dtj = *dt;
	i__2 = *nv;
	for (i = 1; i <= i__2; ++i) {
	    v[i] += dv[i];
	    ev[i] = *egive * (r__1 = dv[i], dabs(r__1));
/* L4: */
	}
	if (*maxit != 1) {
	    goto L5;
	}
	done = TRUE_;
	goto L24;
L5:
	movefr_(nv, &ev[1], &ev2[1]);
	done = (*error)(&v[1], nv, t, dt, &errpar[1], &a1odel_1.erputs, &ev[1]
		);
/* CHECK FOR NEGATIVE ERROR REQUESTS. */
	i__2 = *nv;
	for (i = 1; i <= i__2; ++i) {
	    temp2 = ev[i] == (float)0.;
	    if (temp2) {
		temp2 = ev2[i] != (float)0.;
	    }
	    if (! temp2) {
		temp2 = ev[i] < (float)0.;
	    }
	    if (! temp2) {
		goto L6;
	    }
/* /6S */
/*              CALL SETERR(37H ESSOM - E(I).LE.0 RETURNED BY SERR
OR, 37, */
/*    1            19, 1) */
/* /7S */
	    seterr_(" ESSOM - E(I).LE.0 RETURNED BY SERROR", &c__37, &c__19, &
		    c__1, 37L);
/* / */
	    ret_val = FALSE_;
	    return ret_val;
L6:
/* L7: */
	    ;
	}
	if (! done) {
	    goto L8;
	}
	goto L24;
L8:
	if (iter != *maxit) {
	    goto L9;
	}
	a1odef_1.fnum = 4;
	++a10der_1.nnfs;
	goto L24;
L9:
/* L10: */
	i__2 = *nv;
	for (i = 1; i <= i__2; ++i) {
	    if (iter <= *minit || ev[i] >= ev2[i]) {
		goto L20;
	    }
	    if (ev1[i] <= ev2[i]) {
		goto L11;
	    }
	    rho = ev2[i] / ev1[i];
/* CAN CHECK CONVERGENCE RATE. */
	    goto L12;
L11:
	    rho = (float)1.;
L12:
	    if (rho < (float)1.) {
		goto L13;
	    }
	    ++a10der_1.nnds;
/* DIVERGING. */
	    a1odef_1.fnum = 3;
	    ret_val = done;
	    return ret_val;
L13:
	    if (*keejac != 0) {
		goto L17;
	    }
	    prod = (float)1.;
/* CHECK QUADRATIC CONVERGENCE RATE. */
/* Computing 2nd power */
	    r__1 = rho;
	    power = r__1 * r__1;
/* < 1. */
	    temp = ev[i] / ev2[i];
	    temp1 = *maxit - iter;
	    i__3 = temp1;
	    for (j = 1; j <= i__3; ++j) {
		prod *= power;
/* Computing 2nd power */
		r__1 = power;
		power = r__1 * r__1;
		if (prod <= temp) {
		    goto L15;
		}
/* L14: */
	    }
L15:
	    if (prod <= temp) {
		goto L16;
	    }
	    ++a10der_1.nnds;
/* SLOW CONVERGENCE. */
	    a1odef_1.fnum = 3;
	    ret_val = done;
	    return ret_val;
L16:
	    goto L19;
L17:
	    i__3 = *maxit - iter;
	    if (pow_ri(&rho, &i__3) * ev2[i] <= ev[i]) {
		goto L18;
	    }
	    ++a10der_1.nnds;
/* CHECK LINEAR CONVERGENCE RATE. */
/* SLOW CONVERGENCE. */
	    a1odef_1.fnum = 3;
	    ret_val = done;
	    return ret_val;
L18:
L19:
L20:
	    ev1[i] = ev2[i];
/* L21: */
	}
	i__2 = *nv;
	for (i = 1; i <= i__2; ++i) {
	    vt[i] += dv[i] / *dt;
/* L22: */
	}
/* L23: */
    }
L24:
    ret_val = done;
    return ret_val;
} /* a1odeo_ */

