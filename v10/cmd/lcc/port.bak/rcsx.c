/* rcsx.f -- translated by f2c (version of 5 February 1993  1:37:14).
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
static integer c__6 = 6;
static integer c__42 = 42;
static integer c__7 = 7;
static integer c__33 = 33;
static integer c__8 = 8;

/* Subroutine */ int rcsx_(x, n, m, r, rr, c, rc)
real *x;
integer *n, *m;
real *r;
integer *rr;
real *c;
integer *rc;
{
    /* Initialized data */

    static real s = (float)0.;
    static real l = (float)0.;

    /* System generated locals */
    integer x_dim1, x_offset, i__1, i__2;

    /* Local variables */
    static integer i, j;
    static real d2, d3;
    extern doublereal r1mach_();
    static logical badnge;
    static real sl, xx;
    extern /* Subroutine */ int erroff_(), seterr_();
    static integer rd2, rd3;
    static logical ovelow;
    static real axx;

    /* Parameter adjustments */
    --rc;
    --c;
    --rr;
    --r;
    x_dim1 = *n;
    x_offset = x_dim1 + 1;
    x -= x_offset;

    /* Function Body */
/* TO SCALE */
/*   X = (1/R) * X * C. */
/* /6S */
/*     IF (N .LT. 1) CALL SETERR(16H RCSX - N .LT. 1, 16, 1, 2) */
/*     IF (M .LT. 1) CALL SETERR(16H RCSX - M .LT. 1, 16, 2, 2) */
/* /7S */
    if (*n < 1) {
	seterr_(" RCSX - N .LT. 1", &c__16, &c__1, &c__2, 16L);
    }
    if (*m < 1) {
	seterr_(" RCSX - M .LT. 1", &c__16, &c__2, &c__2, 16L);
    }
/* / */
    if (s != (float)0.) {
	goto L1;
    }
    s = r1mach_(&c__1);
    l = r1mach_(&c__2);
L1:
    sl = s * l;
    i__1 = *n;
    for (i = 1; i <= i__1; ++i) {
	if (r[i] == (float)0.) {
	    goto L2;
	}
/* /6S */
/*        IF (R(I) .LT. S .OR. R(I) .GT. L) CALL SETERR( */
/*    1      36H RCSX - MUST HAVE S .LE. R(I) .LE. L, 36, 3, 2) */
/*        IF (RR(I) .LT. (-1) .OR. RR(I) .GT. 1) CALL SETERR( */
/*    1      36H RCSX - MUST HAVE RR(I) IN (-1,0,+1), 36, 4, 2) */
/* /7S */
	if (r[i] < s || r[i] > l) {
	    seterr_(" RCSX - MUST HAVE S .LE. R(I) .LE. L", &c__36, &c__3, &
		    c__2, 36L);
	}
	if (rr[i] < -1 || rr[i] > 1) {
	    seterr_(" RCSX - MUST HAVE RR(I) IN (-1,0,+1)", &c__36, &c__4, &
		    c__2, 36L);
	}
/* / */
L2:
	;
    }
    i__1 = *m;
    for (i = 1; i <= i__1; ++i) {
	if (c[i] == (float)0.) {
	    goto L3;
	}
/* /6S */
/*        IF (C(I) .LT. S .OR. C(I) .GT. L) CALL SETERR( */
/*    1      36H RCSX - MUST HAVE S .LE. C(I) .LE. L, 36, 5, 2) */
/*        IF (RC(I) .LT. (-1) .OR. RC(I) .GT. 1) CALL SETERR( */
/*    1      36H RCSX - MUST HAVE RC(I) IN (-1,0,+1), 36, 6, 2) */
/* /7S */
	if (c[i] < s || c[i] > l) {
	    seterr_(" RCSX - MUST HAVE S .LE. C(I) .LE. L", &c__36, &c__5, &
		    c__2, 36L);
	}
	if (rc[i] < -1 || rc[i] > 1) {
	    seterr_(" RCSX - MUST HAVE RC(I) IN (-1,0,+1)", &c__36, &c__6, &
		    c__2, 36L);
	}
/* / */
L3:
	;
    }
/* CHECK 1/(S*L) RANGE. */
    badnge = FALSE_;
    if (sl > (float)1.) {
	goto L4;
    }
    if ((float)1. / l > sl) {
	badnge = TRUE_;
    }
    goto L5;
L4:
    if (sl > (float)1. / s) {
	badnge = TRUE_;
    }
/* S*L > 1. */
/* /6S */
/*  5  IF (BADNGE) CALL SETERR( */
/*    1   42H RCSX - MUST HAVE 1/(S*L) IN MACHINE RANGE, 42, 7, 1) */
/* /7S */
L5:
    if (badnge) {
	seterr_(" RCSX - MUST HAVE 1/(S*L) IN MACHINE RANGE", &c__42, &c__7, &
		c__1, 42L);
    }
/* / */
    ovelow = FALSE_;
    i__1 = *m;
    for (j = 1; j <= i__1; ++j) {
	d3 = c[j];
	rd3 = rc[j];
/* DO THE SCALING. */
	i__2 = *n;
	for (i = 1; i <= i__2; ++i) {
	    d2 = r[i];
	    rd2 = rr[i];
	    xx = x[i + j * x_dim1];
	    axx = dabs(xx);
	    if (d2 == (float)0.) {
		goto L85;
	    }
	    badnge = FALSE_;
	    if (xx != (float)0. && d3 != (float)0.) {
		goto L6;
	    }
	    x[i + j * x_dim1] = (float)0.;
	    goto L85;
L6:
	    if (rd2 != rd3) {
		goto L26;
	    }
	    if (d3 < d2) {
		goto L16;
	    }
	    if (d3 > (float)1.) {
		goto L7;
	    }
	    if (d3 * axx > d2 * l) {
		badnge = TRUE_;
	    }
/* WORRY ABOUT OVERFLOW. */
	    goto L10;
L7:
	    if (d2 > (float)1.) {
		goto L8;
	    }
	    if (axx > d2 * (l / d3)) {
		badnge = TRUE_;
	    }
/* D3 > 1. */
	    goto L9;
L8:
	    if (axx / d2 > l / d3) {
		badnge = TRUE_;
	    }
/* D2 > 1 & D3 > 1. */
L9:
L10:
	    if (badnge) {
		goto L15;
	    }
	    if (d3 > dmin(d2,(float)1.) * l) {
		goto L11;
	    }
	    x[i + j * x_dim1] *= d3 / d2;
/* IN-RANGE. */
	    goto L14;
L11:
	    if (d3 < (float)1. && axx > d2 * l) {
		goto L12;
	    }
	    x[i + j * x_dim1] = d3 * (xx / d2);
/* D3/D2 OVERFLOWS, IE, D3 > D2*L AND D2 < 1. */
	    goto L13;
L12:
	    x[i + j * x_dim1] = d3 * xx / d2;
L13:
L14:
L15:
	    goto L25;
L16:
	    if (d3 < (float)1.) {
		goto L17;
	    }
	    if (axx < d2 * s / d3) {
		x[i + j * x_dim1] = (float)0.;
	    }
/* D3 < D2, WORRY ABOUT UNDERFLOW. */
/* D2 > D3 >= 1. */
	    goto L20;
L17:
	    if (d2 < (float)1.) {
		goto L18;
	    }
	    if (d3 * axx < d2 * s) {
		x[i + j * x_dim1] = (float)0.;
	    }
/* D3 < 1. */
	    goto L19;
L18:
	    if (axx < s / d3 * d2) {
		x[i + j * x_dim1] = (float)0.;
	    }
/* D2, D3 < 1. */
L19:
L20:
	    if (x[i + j * x_dim1] == (float)0.) {
		goto L85;
	    }
/*       IN-RANGE. */
	    if (d3 < dmax(d2,(float)1.) * s) {
		goto L21;
	    }
	    x[i + j * x_dim1] *= d3 / d2;
	    goto L24;
L21:
	    if (d3 > (float)1. && axx < d2 * s) {
		goto L22;
	    }
	    x[i + j * x_dim1] = d3 * (xx / d2);
/* D3/D2 UNDERFLOWS, I.E., D3 < D2*S, D2 > 1 . */
	    goto L23;
L22:
	    x[i + j * x_dim1] = d3 * xx / d2;
L23:
L24:
L25:
	    goto L83;
L26:
	    if (rd2 <= 0 || rd3 >= 0) {
		goto L27;
	    }
	    x[i + j * x_dim1] = (float)0.;
	    goto L85;
/* UNDERFLOW. */
L27:
	    if (rd2 >= 0 || rd3 <= 0) {
		goto L28;
	    }
	    badnge = TRUE_;
/* OVERFLOW. */
	    goto L81;
L28:
	    if (rd2 >= 0 || rd3 != 0) {
		goto L42;
	    }
	    if (d3 > (float)1.) {
		goto L29;
	    }
	    if (d3 * axx > d2) {
		badnge = TRUE_;
	    }
/* WORRY ABOUT OVERFLOW. */
	    goto L30;
L29:
	    if (axx > d2 / d3) {
		badnge = TRUE_;
	    }
/* D3 > 1. */
L30:
	    if (badnge) {
		goto L41;
	    }
	    if (d3 > d2) {
		goto L33;
	    }
	    if (d3 > (float)1.) {
		goto L31;
	    }
	    x[i + j * x_dim1] = d3 * l / d2 * xx;
/* IN-RANGE. */
	    goto L32;
L31:
	    x[i + j * x_dim1] = d3 / d2 * l * xx;
L32:
	    goto L40;
L33:
	    if (d3 < (float)1. && axx > d2) {
		goto L36;
	    }
	    if (axx < d2 * s) {
		goto L34;
	    }
	    x[i + j * x_dim1] = d3 * (l * (xx / d2));
/* D3 > D2. */
/* D3*(X*L/D2). */
	    goto L35;
L34:
	    x[i + j * x_dim1] = d3 * (xx * l / d2);
L35:
	    goto L39;
L36:
	    if (axx > d2 * l) {
		goto L37;
	    }
	    x[i + j * x_dim1] = d3 * (xx / d2) * l;
/* D3 < 1 & |X| > D2. */
	    goto L38;
L37:
	    x[i + j * x_dim1] = d3 / d2 * xx * l;
L38:
L39:
L40:
L41:
	    goto L80;
L42:
	    if (rd2 <= 0 || rd3 != 0) {
		goto L55;
	    }
	    if (d3 < (float)1.) {
		goto L43;
	    }
	    if (axx < d2 / d3) {
		x[i + j * x_dim1] = (float)0.;
	    }
/* WORRY ABOUT UNDERFLOW. */
	    goto L44;
L43:
	    if (d3 * axx < d2) {
		x[i + j * x_dim1] = (float)0.;
	    }
L44:
	    if (x[i + j * x_dim1] == (float)0.) {
		goto L85;
	    }
/*       IN-RANGE. */
	    if (d3 < d2) {
		goto L47;
	    }
	    if (d2 > (float)1.) {
		goto L45;
	    }
	    x[i + j * x_dim1] = s / d2 * d3 * xx;
	    goto L46;
L45:
	    x[i + j * x_dim1] = d3 / d2 * s * xx;
L46:
	    goto L54;
L47:
	    if (d3 > (float)1. && axx < d2) {
		goto L50;
	    }
	    if (d2 > (float)1.) {
		goto L48;
	    }
	    x[i + j * x_dim1] = d3 * (xx * (s / d2));
/* D3 < D2. */
	    goto L49;
L48:
	    x[i + j * x_dim1] = d3 * (xx / d2 * s);
L49:
	    goto L53;
L50:
	    if (d3 < s * d2) {
		goto L51;
	    }
	    x[i + j * x_dim1] = d3 / d2 * xx * s;
/* D2 > D3 > 1 AND |X| < D2. */
	    goto L52;
L51:
	    x[i + j * x_dim1] = xx / d2 * d3 * s;
L52:
L53:
L54:
	    goto L79;
L55:
	    if (rd2 != 0 || rd3 >= 0) {
		goto L66;
	    }
	    if (d2 < (float)1.) {
		goto L56;
	    }
	    if (axx / d2 < sl / d3) {
		x[i + j * x_dim1] = (float)0.;
	    }
/* WORRY ABOUT UNDERFLOW. */
	    goto L57;
L56:
	    if (axx < sl / d3 * d2) {
		x[i + j * x_dim1] = (float)0.;
	    }
L57:
	    if (x[i + j * x_dim1] == (float)0.) {
		goto L85;
	    }
/*       IN-RANGE. */
	    if (d2 > (float)1.) {
		goto L62;
	    }
	    if (d3 < d2 * sl) {
		goto L58;
	    }
	    x[i + j * x_dim1] = d3 / d2 / l * xx;
	    goto L61;
L58:
	    if (axx < sl) {
		goto L59;
	    }
	    x[i + j * x_dim1] = d3 / d2 * (xx / l);
	    goto L60;
L59:
	    x[i + j * x_dim1] = d3 / d2 * xx / l;
L60:
L61:
	    goto L65;
L62:
	    if (axx / d2 < sl) {
		goto L63;
	    }
	    x[i + j * x_dim1] = xx / d2 / l * d3;
/* D2 > 1. */
	    goto L64;
L63:
	    x[i + j * x_dim1] = xx / d2 * d3 / l;
L64:
L65:
	    goto L78;
L66:
	    if (d2 < (float)1.) {
		goto L67;
	    }
	    if (axx / d2 > sl / d3) {
		badnge = TRUE_;
	    }
/* RD2 == 0 & RD3 > 0, WORRY ABOUT OVERFLOW. */
	    goto L68;
L67:
	    if (axx > d2 * (sl / d3)) {
		badnge = TRUE_;
	    }
L68:
	    if (badnge) {
		goto L77;
	    }
	    if (d2 < (float)1.) {
		goto L73;
	    }
	    if (d3 / d2 > sl) {
		goto L69;
	    }
	    x[i + j * x_dim1] = d3 / d2 / s * xx;
/* IN-RANGE. */
	    goto L72;
L69:
	    if (axx > sl) {
		goto L70;
	    }
	    x[i + j * x_dim1] = d3 / d2 * (xx / s);
	    goto L71;
L70:
	    x[i + j * x_dim1] = d3 / d2 * xx / s;
L71:
L72:
	    goto L76;
L73:
	    if (axx > d2 * sl) {
		goto L74;
	    }
	    x[i + j * x_dim1] = xx / d2 / s * d3;
/* D2 < 1. */
	    goto L75;
L74:
	    x[i + j * x_dim1] = xx / d2 * d3 / s;
L75:
L76:
L77:
L78:
L79:
L80:
L81:
/* L82: */
L83:
	    if (! badnge) {
		goto L84;
	    }
	    x[i + j * x_dim1] = l * (xx / axx);
	    ovelow = TRUE_;
L84:
L85:
	    ;
	}
/* L86: */
    }
    if (! ovelow) {
	goto L87;
    }
    erroff_();
/* /6S */
/*        CALL SETERR(33H RCSX - SOLUTION X HAS OVERFLOWED, 33, 8, 1) */
/* /7S */
    seterr_(" RCSX - SOLUTION X HAS OVERFLOWED", &c__33, &c__8, &c__1, 33L);
/* / */
L87:
    return 0;
} /* rcsx_ */

