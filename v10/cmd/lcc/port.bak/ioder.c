/* ioder.f -- translated by f2c (version of 5 February 1993  1:37:14).
   You must link the resulting object file with the libraries:
	-lF77 -lI77 -lm -lc   (in that order)
*/

#include "f2c.h"

/* Table of constant values */

static integer c_n1 = -1;
static integer c_n2 = -2;
static integer c_n3 = -3;
static integer c_n4 = -4;
static integer c_n1001 = -1001;
static integer c_n1002 = -1002;
static integer c_n2001 = -2001;
static integer c_n2002 = -2002;
static integer c_n2003 = -2003;
static integer c_n2004 = -2004;
static integer c_n2005 = -2005;
static integer c_n2006 = -2006;
static integer c_n2007 = -2007;
static integer c_n3001 = -3001;
static integer c_n3002 = -3002;
static integer c_n3003 = -3003;
static integer c_n3004 = -3004;
static integer c_n3005 = -3005;
static integer c__18 = 18;
static integer c__8 = 8;
static integer c__2 = 2;
static integer c__23 = 23;
static integer c__14 = 14;
static integer c__37 = 37;
static integer c__16 = 16;

/* Subroutine */ int ioder_(v, nv, tstart, tstop, dt, d, error, errpar, inmi, 
	da, handle)
real *v;
integer *nv;
real *tstart, *tstop, *dt;
/* Subroutine */ int (*d) (), (*error) ();
real *errpar;
/* Subroutine */ int (*inmi) (), (*da) (), (*handle) ();
{
    /* System generated locals */
    integer i__1, i__2;

    /* Local variables */
    static real beta;
    static integer kmax, ircs, mmax, izap;
    static real fzap;
    static logical lzap;
    static real rzap;
    extern /* Subroutine */ int iode4_();
    static integer i, n[100];
    static real gamma, delta, egive, theta;
    extern /* Subroutine */ int iodev_();
    static integer kinit, minit, maxit;
    static logical xpoly;
    static integer keejac;
    static real hfract;
    static logical usenfd, usengj;
    extern /* Subroutine */ int seterr_();
    static logical usenns, erputs;

/* THE SECOND, ROUTINE, LEVEL OF IODE. */
/* RETRIEVE THE VALUES TO BE USED. */
    /* Parameter adjustments */
    --errpar;
    --v;

    /* Function Body */
    iodev_(&c_n1, &theta, &rzap, &izap, &lzap);
    iodev_(&c_n2, &beta, &rzap, &izap, &lzap);
    iodev_(&c_n3, &gamma, &rzap, &izap, &lzap);
    iodev_(&c_n4, &delta, &rzap, &izap, &lzap);
    iodev_(&c_n1001, &fzap, &hfract, &izap, &lzap);
    iodev_(&c_n1002, &fzap, &egive, &izap, &lzap);
    iodev_(&c_n2001, &fzap, &rzap, &keejac, &lzap);
    iodev_(&c_n2002, &fzap, &rzap, &minit, &lzap);
    iodev_(&c_n2003, &fzap, &rzap, &maxit, &lzap);
    iodev_(&c_n2004, &fzap, &rzap, &kmax, &lzap);
    iodev_(&c_n2005, &fzap, &rzap, &kinit, &lzap);
    iodev_(&c_n2006, &fzap, &rzap, &mmax, &lzap);
    iodev_(&c_n2007, &fzap, &rzap, &ircs, &lzap);
    iodev_(&c_n3001, &fzap, &rzap, &izap, &xpoly);
    iodev_(&c_n3002, &fzap, &rzap, &izap, &erputs);
    iodev_(&c_n3003, &fzap, &rzap, &izap, &usengj);
    iodev_(&c_n3004, &fzap, &rzap, &izap, &usenns);
    iodev_(&c_n3005, &fzap, &rzap, &izap, &usenfd);
/* TEST FOR ERRORS. */
/* /6S */
/*     IF (KMAX .LT. 1) CALL SETERR(18H IODE4 - KMAX.LT.1, 18, 8, 2) */
/*     IF (MMAX .LT. KMAX+2) CALL SETERR(23H IODE4 - MMAX.LT.KMAX+2, 23 */
/*    1   , 14, 2) */
/* /7S */
    if (kmax < 1) {
	seterr_(" IODE4 - KMAX.LT.1", &c__18, &c__8, &c__2, 18L);
    }
    if (mmax < kmax + 2) {
	seterr_(" IODE4 - MMAX.LT.KMAX+2", &c__23, &c__14, &c__2, 23L);
    }
/* / */
    i__1 = mmax;
    for (i = 1; i <= i__1; ++i) {
	i__2 = -(i + 4000);
	iodev_(&i__2, &fzap, &rzap, &n[i - 1], &lzap);
/* L1: */
    }
/* TEST N FOR MONOTONICITY. */
    i__1 = mmax;
    for (i = 2; i <= i__1; ++i) {
/* /6S */
/*        IF (N(I) .LE. N(I-1)) CALL SETERR( */
/*    1      37H IODE4 - N IS NOT MONOTONE INCREASING, 37, 16, 2) */
/* /7S */
	if (n[i - 1] <= n[i - 2]) {
	    seterr_(" IODE4 - N IS NOT MONOTONE INCREASING", &c__37, &c__16, &
		    c__2, 37L);
	}
/* / */
/* L2: */
    }
    iode4_(&v[1], nv, tstart, tstop, dt, d, &theta, &keejac, &minit, &maxit, &
	    usengj, &usenns, &usenfd, &beta, &gamma, &delta, n, &mmax, &
	    hfract, &egive, &kmax, &xpoly, &kinit, &ircs, error, &errpar[1], &
	    erputs, inmi, da, handle);
    return 0;
} /* ioder_ */

