/* drcsb.f -- translated by f2c (version of 5 February 1993  1:37:14).
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
static integer c__42 = 42;
static integer c__4 = 4;

/* Subroutine */ int drcsb_(b, m, n, r, c, rc)
doublereal *b;
integer *m, *n;
doublereal *r, *c;
integer *rc;
{
    /* Initialized data */

    static doublereal s = 0.;
    static doublereal l = 0.;

    /* System generated locals */
    integer b_dim1, b_offset, i__1;

    /* Local variables */
    static integer nerr, i;
    extern /* Subroutine */ int drcsc_();
    static integer irold;
    extern /* Subroutine */ int drcss_();
    extern doublereal d1mach_();
    extern /* Subroutine */ int erroff_(), entsrc_(), retsrc_(), seterr_();
    extern integer nerror_();

    /* Parameter adjustments */
    --rc;
    --c;
    --r;
    b_dim1 = *m;
    b_offset = b_dim1 + 1;
    b -= b_offset;

    /* Function Body */
/* TO GET THE COLUMN SCALED MATRIX (1/R)*B*(1/C), */
/* GIVEN THE ROW SCALE FACTOR, AND RETURN THE COLUMN FACTOR. */
/* /6S */
/*     IF (M .LT. 1) CALL SETERR(16HDRCSB - M .LT. 1, 16, 1, 2) */
/*     IF (N .LT. 1) CALL SETERR(16HDRCSB - N .LT. 1, 16, 2, 2) */
/* /7S */
    if (*m < 1) {
	seterr_("DRCSB - M .LT. 1", &c__16, &c__1, &c__2, 16L);
    }
    if (*n < 1) {
	seterr_("DRCSB - N .LT. 1", &c__16, &c__2, &c__2, 16L);
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
/*    1      36HDRCSB - MUST HAVE S .LE. R(I) .LE. L, 36, 3, 2) */
/* /7S */
	if (r[i] < s || r[i] > l) {
	    seterr_("DRCSB - MUST HAVE S .LE. R(I) .LE. L", &c__36, &c__3, &
		    c__2, 36L);
	}
/* / */
L2:
	;
    }
/* TURN ERROR RECOVERY ON AND SAVE OLD VALUE. */
    entsrc_(&irold, &c__1);
/* GET COLUMN SCALE FACTOR. */
    drcsc_(&b[b_offset], m, n, &r[1], &c[1], &rc[1]);
/* APPLY THEM. */
    drcss_(&b[b_offset], m, n, &r[1], &c[1], &rc[1]);
    if (nerror_(&nerr) == 0) {
	goto L3;
    }
    erroff_();
/* /6S */
/*        CALL SETERR(42HDRCSB - MUST HAVE 1/(S*L) IN MACHINE RANGE, 42 */
/*    1      , 4, 1) */
/* /7S */
    seterr_("DRCSB - MUST HAVE 1/(S*L) IN MACHINE RANGE", &c__42, &c__4, &
	    c__1, 42L);
/* / */
    return 0;
/* RESTORE OLD RECOVERY VALUE. */
L3:
    retsrc_(&irold);
    return 0;
} /* drcsb_ */

