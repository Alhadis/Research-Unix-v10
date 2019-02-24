/* d0desr.f -- translated by f2c (version of 5 February 1993  1:37:14).
   You must link the resulting object file with the libraries:
	-lF77 -lI77 -lm -lc   (in that order)
*/

#include "f2c.h"

/* Common Block Declarations */

extern struct {
    logical okay;
} dodesf_;

#define dodesf_1 dodesf_

extern struct {
    doublereal ds[500];
} cstak_;

#define cstak_1 cstak_

/* Subroutine */ int d0desr_(t0, x0, t1, x1, nx, nt, f, ftx, xl, ftx0, igmax)
doublereal *t0, *x0, *t1, *x1;
integer *nx, *nt;
/* Subroutine */ int (*f) ();
doublereal *ftx, *xl, *ftx0;
integer *igmax;
{
    /* System generated locals */
    integer i__1, i__2;

    /* Local variables */
    static doublereal temp;
    static integer i;
    static doublereal t;
    extern /* Subroutine */ int d0desu_();
    static doublereal dt;
    static integer it;
#define rs ((real *)&cstak_1)
    static doublereal dt2;
    static integer nt2;


/*  ACTUAL GRAGGS MODIFIED MID-POINT RULE. */





    /* Parameter adjustments */
    --ftx0;
    --xl;
    --ftx;
    --x1;
    --x0;

    /* Function Body */
    dodesf_1.okay = TRUE_;
    dt = (*t1 - *t0) / (real) (*nt);
    dt2 = dt * .5;
    nt2 = *nt << 1;

/* ... IF THIS IS THE FIRST CALL, COMPUTE FTX0=F(T0,X(T0)) */

    if (*nt == 1) {
	(*f)(t0, &x0[1], nx, &ftx0[1]);
    }
    if (! dodesf_1.okay) {
	goto L50;
    }

/* ... COMPUTE FORWARD-EULER INITIAL TIME-STEP. */

    i__1 = *nx;
    for (i = 1; i <= i__1; ++i) {
	xl[i] = x0[i];
/* L10: */
	x1[i] = x0[i] + dt2 * ftx0[i];
    }

/* ... GET THE APPROXIMATE SOLUTION AT THE REST OF THE MESH-POINTS */
/* ... USING THE MID-POINT RULE. */

    i__1 = nt2;
    for (it = 2; it <= i__1; ++it) {
	t = *t0 + (real) (it - 1) * dt2;
	(*f)(&t, &x1[1], nx, &ftx[1]);
	if (! dodesf_1.okay) {
	    goto L50;
	}

	i__2 = *nx;
	for (i = 1; i <= i__2; ++i) {
	    temp = x1[i];
	    x1[i] = xl[i] + dt * ftx[i];
/* L20: */
	    xl[i] = temp;
	}

/* ...... IF NEED TO, RAISE THE GLOBAL MAXIMUM. */

/* L30: */
	if (*igmax > 0) {
	    d0desu_(&x1[1], nx, &rs[*igmax - 1]);
	}
    }

/* ... SMOOTH AT THE END POINT. */

    (*f)(t1, &x1[1], nx, &ftx[1]);
    if (! dodesf_1.okay) {
	goto L50;
    }

    i__1 = *nx;
    for (i = 1; i <= i__1; ++i) {
/* L40: */
	x1[i] = (x1[i] + (xl[i] + dt2 * ftx[i])) * .5;
    }

/* ... IF NEED TO, RAISE THE GLOBAL MAXIMUM. */

    if (*igmax > 0) {
	d0desu_(&x1[1], nx, &rs[*igmax - 1]);
    }

L50:
    return 0;

} /* d0desr_ */

#undef rs


