/* drcsc.f -- translated by f2c (version of 5 February 1993  1:37:14).
   You must link the resulting object file with the libraries:
	-lF77 -lI77 -lm -lc   (in that order)
*/

#include "f2c.h"

/* Table of constant values */

static integer c__16 = 16;
static integer c__1 = 1;
static integer c__2 = 2;
static integer c__36 = 36;
static integer c__3 = 3;

/* Subroutine */ int drcsc_(a, m, n, r, c, rc)
doublereal *a;
integer *m, *n;
doublereal *r, *c;
integer *rc;
{
    /* Initialized data */

    static doublereal s = 0.;
    static doublereal l = 0.;

    /* System generated locals */
    integer a_dim1, a_offset, i__1, i__2;
    doublereal d__1, d__2;

    /* Local variables */
    static doublereal aaij;
    static integer i, j;
    static doublereal d1, d2;
    extern doublereal d1mach_();
    extern /* Subroutine */ int seterr_();
    static integer rd2;
    static doublereal aij;

    /* Parameter adjustments */
    --rc;
    --c;
    --r;
    a_dim1 = *m;
    a_offset = a_dim1 + 1;
    a -= a_offset;

    /* Function Body */
/* TO GET THE COLUMN SCALE FACTOR FOR (1/R)*A. */
/* /6S */
/*     IF (M .LT. 1) CALL SETERR(16HDRCSC - M .LT. 1, 16, 1, 2) */
/*     IF (N .LT. 1) CALL SETERR(16HDRCSC - N .LT. 1, 16, 2, 2) */
/* /7S */
    if (*m < 1) {
	seterr_("DRCSC - M .LT. 1", &c__16, &c__1, &c__2, 16L);
    }
    if (*n < 1) {
	seterr_("DRCSC - N .LT. 1", &c__16, &c__2, &c__2, 16L);
    }
/* / */
    if (s != 0.) {
	goto L1;
    }
    s = d1mach_(&c__1);
    l = d1mach_(&c__2);
L1:
    i__1 = *m;
    for (i = 1; i <= i__1; ++i) {
	if (r[i] == 0.) {
	    goto L2;
	}
/* /6S */
/*        IF (R(I) .LT. S .OR. R(I) .GT. L) CALL SETERR( */
/*    1      36HDRCSC - MUST HAVE S .LE. R(I) .LE. L, 36, 3, 2) */
/* /7S */
	if (r[i] < s || r[i] > l) {
	    seterr_("DRCSC - MUST HAVE S .LE. R(I) .LE. L", &c__36, &c__3, &
		    c__2, 36L);
	}
/* / */
L2:
	;
    }
    i__1 = *n;
    for (j = 1; j <= i__1; ++j) {
	d2 = 0.;
/* -1 = UNDERFLOW, 0 = IN-RANGE, +1 = OVERFLOW. */
	rd2 = -1;
	i__2 = *m;
	for (i = 1; i <= i__2; ++i) {
	    aij = a[i + j * a_dim1];
	    aaij = abs(aij);
	    d1 = r[i];
	    if (aij == 0. || d1 == 0.) {
		goto L17;
	    }
	    if (d1 >= 1.) {
		goto L9;
	    }
	    if (aaij <= d1 * l) {
		goto L3;
	    }
	    if (rd2 < 1) {
		d2 = 0.;
	    }
/* CHECK FOR OVERFLOW. */
/* OVERFLOW. */
	    rd2 = 1;
/* Computing MAX */
	    d__1 = d2, d__2 = aaij * (s / d1);
	    d2 = max(d__1,d__2);
	    goto L8;
L3:
	    if (rd2 <= 0) {
		goto L4;
	    }
	    goto L17;
/* THIS ELEMENT IS IN-RANGE. */
/* ALREADY OVERFLOWED, NO EFFECT. */
L4:
	    if (rd2 != 0) {
		goto L5;
	    }
/* Computing MAX */
	    d__1 = d2, d__2 = aaij / d1;
	    d2 = max(d__1,d__2);
	    goto L6;
L5:
	    rd2 = 0;
/* RD2 = -1. */
	    d2 = aaij / d1;
L6:
/* L7: */
L8:
	    goto L16;
L9:
	    if (aaij >= d1 * s) {
		goto L10;
	    }
	    if (rd2 >= 0) {
		goto L17;
	    }
/* ELEMENT UNDERFLOW, D1 >= 1. */
/* NO-EFFECT. */
/* Computing MAX */
	    d__1 = d2, d__2 = aaij * (l / d1);
	    d2 = max(d__1,d__2);
	    goto L15;
L10:
	    if (rd2 <= 0) {
		goto L11;
	    }
	    goto L17;
/* IN-RANGE. */
/* NO-EFFECT. */
L11:
	    if (rd2 != 0) {
		goto L12;
	    }
/* Computing MAX */
	    d__1 = d2, d__2 = aaij / d1;
	    d2 = max(d__1,d__2);
	    goto L13;
L12:
	    rd2 = 0;
/* UNDERFLOWED SO FAR. */
	    d2 = aaij / d1;
L13:
/* L14: */
L15:
L16:
L17:
	    ;
	}
	c[j] = d2;
	rc[j] = rd2;
/* L18: */
    }
    return 0;
} /* drcsc_ */

