/* drcss.f -- translated by f2c (version of 5 February 1993  1:37:14).
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
static integer c__4 = 4;
static integer c__5 = 5;
static integer c__42 = 42;
static integer c__6 = 6;

/* Subroutine */ int drcss_(a, m, n, r, c, rc)
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

    /* Local variables */
    static integer i, j;
    static doublereal d1, d2;
    extern doublereal d1mach_();
    static logical badnge;
    extern /* Subroutine */ int seterr_();
    static integer rd2;

    /* Parameter adjustments */
    --rc;
    --c;
    --r;
    a_dim1 = *m;
    a_offset = a_dim1 + 1;
    a -= a_offset;

    /* Function Body */
/* TO SCALE ((1/R)*A)*(1/C). */
/* /6S */
/*     IF (M .LT. 1) CALL SETERR(16HDRCSS - M .LT. 1, 16, 1, 2) */
/*     IF (N .LT. 1) CALL SETERR(16HDRCSS - N .LT. 1, 16, 2, 2) */
/* /7S */
    if (*m < 1) {
	seterr_("DRCSS - M .LT. 1", &c__16, &c__1, &c__2, 16L);
    }
    if (*n < 1) {
	seterr_("DRCSS - N .LT. 1", &c__16, &c__2, &c__2, 16L);
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
/*    1      36HDRCSS - MUST HAVE S .LE. R(I) .LE. L, 36, 3, 2) */
/* /7S */
	if (r[i] < s || r[i] > l) {
	    seterr_("DRCSS - MUST HAVE S .LE. R(I) .LE. L", &c__36, &c__3, &
		    c__2, 36L);
	}
/* / */
L2:
	;
    }
    i__1 = *n;
    for (i = 1; i <= i__1; ++i) {
	if (c[i] == 0.) {
	    goto L3;
	}
/* /6S */
/*        IF (C(I) .LT. S .OR. C(I) .GT. L) CALL SETERR( */
/*    1      36HDRCSS - MUST HAVE S .LE. C(I) .LE. L, 36, 4, 2) */
/*        IF (RC(I) .LT. (-1) .OR. RC(I) .GT. 1) CALL SETERR( */
/*    1      36HDRCSS - MUST HAVE RC(I) IN (-1,0,+1), 36, 5, 2) */
/* /7S */
	if (c[i] < s || c[i] > l) {
	    seterr_("DRCSS - MUST HAVE S .LE. C(I) .LE. L", &c__36, &c__4, &
		    c__2, 36L);
	}
	if (rc[i] < -1 || rc[i] > 1) {
	    seterr_("DRCSS - MUST HAVE RC(I) IN (-1,0,+1)", &c__36, &c__5, &
		    c__2, 36L);
	}
/* / */
L3:
	;
    }
/* CHECK 1/(S*L) RANGE. */
    badnge = FALSE_;
    if (s * l > 1.) {
	goto L4;
    }
    if (1. / l > s * l) {
	badnge = TRUE_;
    }
    goto L5;
L4:
    if (s * l > 1. / s) {
	badnge = TRUE_;
    }
/* S*L > 1. */
/* /6S */
/*  5  IF (BADNGE) CALL SETERR( */
/*    1   42HDRCSX - MUST HAVE 1/(S*L) IN MACHINE RANGE, 42, 6, 1) */
/* /7S */
L5:
    if (badnge) {
	seterr_("DRCSX - MUST HAVE 1/(S*L) IN MACHINE RANGE", &c__42, &c__6, &
		c__1, 42L);
    }
/* / */
    i__1 = *m;
    for (i = 1; i <= i__1; ++i) {
	d1 = r[i];
	if (d1 == 0.) {
	    goto L33;
	}
	i__2 = *n;
	for (j = 1; j <= i__2; ++j) {
	    d2 = c[j];
	    rd2 = rc[j];
	    if (a[i + j * a_dim1] != 0. && d2 != 0.) {
		goto L6;
	    }
	    goto L32;
L6:
	    if (d1 < 1.) {
		goto L19;
	    }
	    if (rd2 <= 0) {
		goto L11;
	    }
	    if (d2 < 1.) {
		goto L7;
	    }
	    a[i + j * a_dim1] = s * (a[i + j * a_dim1] / d1 / d2);
/* D2 OVERFLOWED. */
	    goto L10;
L7:
	    if (d1 * d2 < 1.) {
		goto L8;
	    }
	    a[i + j * a_dim1] = s * (a[i + j * a_dim1] / (d1 * d2));
/* D2 < 1. */
	    goto L9;
L8:
	    a[i + j * a_dim1] *= s / (d1 * d2);
L9:
L10:
	    goto L18;
L11:
	    if (d2 < 1.) {
		goto L12;
	    }
	    a[i + j * a_dim1] = a[i + j * a_dim1] / d1 / d2;
	    goto L17;
L12:
	    if (rd2 >= 0) {
		goto L15;
	    }
	    if (d2 < 1. / d1) {
		goto L13;
	    }
	    a[i + j * a_dim1] *= l / d1 / d2;
/* D2 UNDERFLOWED. */
	    goto L14;
L13:
	    a[i + j * a_dim1] = l * (a[i + j * a_dim1] / (d1 * d2));
L14:
	    goto L16;
L15:
	    a[i + j * a_dim1] /= d1 * d2;
/* D2 < 1. */
L16:
L17:
L18:
	    goto L30;
L19:
	    if (rd2 <= 0) {
		goto L22;
	    }
	    if (d1 * d2 < 1.) {
		goto L20;
	    }
	    a[i + j * a_dim1] = s * (a[i + j * a_dim1] / (d1 * d2));
/* D1 < 1. */
/* D2 OVERFLOWED. */
	    goto L21;
L20:
	    a[i + j * a_dim1] *= s / d1 / d2;
L21:
	    goto L29;
L22:
	    if (d2 < 1.) {
		goto L23;
	    }
	    a[i + j * a_dim1] /= d1 * d2;
	    goto L28;
L23:
	    if (rd2 >= 0) {
		goto L26;
	    }
	    if (d1 * d2 > 1.) {
		goto L24;
	    }
	    a[i + j * a_dim1] = l * (a[i + j * a_dim1] / (d1 * d2));
/* D2 UNDERFLOWED. */
	    goto L25;
L24:
	    a[i + j * a_dim1] *= l / (d1 * d2);
L25:
	    goto L27;
L26:
	    a[i + j * a_dim1] = a[i + j * a_dim1] / d1 / d2;
/* D2 < 1. */
L27:
L28:
L29:
L30:
/* L31: */
L32:
	    ;
	}
L33:
	;
    }
    return 0;
} /* drcss_ */

