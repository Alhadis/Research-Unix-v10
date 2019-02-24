/* diodev.f -- translated by f2c (version of 5 February 1993  1:37:14).
   You must link the resulting object file with the libraries:
	-lF77 -lI77 -lm -lc   (in that order)
*/

#include "f2c.h"

/* Table of constant values */

static integer c__100 = 100;
static integer c__0 = 0;
static integer c__24 = 24;
static integer c__1 = 1;
static integer c__2 = 2;

/* Subroutine */ int diodev_(j, f, r, i, l)
integer *j;
doublereal *f;
real *r;
integer *i;
logical *l;
{
    /* Initialized data */

    static doublereal theta = 1.;
    static integer kmax = 10;
    static integer kinit = 4;
    static integer mmax = 15;
    static integer ircs = 1;
    static logical xpoly = FALSE_;
    static logical erputs = FALSE_;
    static logical usengj = FALSE_;
    static logical usenns = FALSE_;
    static logical usenfd = FALSE_;
    static integer n[100] = { 1,0,0 };
    static doublereal beta = 1.;
    static doublereal gamma = 1.;
    static doublereal delta = 0.;
    static real hfract = (float)1.;
    static real egive = (float)100.;
    static integer keejac = 0;
    static integer minit = 10;
    static integer maxit = 50;

    /* System generated locals */
    integer i__1, i__2;

    /* Builtin functions */
    double sqrt();

    /* Local variables */
    extern /* Subroutine */ int seti_();
    static integer temp;
    static logical temp1;
    static integer k, m;
    extern /* Subroutine */ int seterr_();

/* THE PARAMETER SETTING ROUTINE FOR IODE. */
/* THE VARIABLES ARE */
/* J = 1. */
/* J = 2. */
/* J = 3. */
/* J = 4. */
/* J = 1001. */
/* J = 1002. */
/* J = 2001. */
/* J = 2002. */
/* J = 2003. */
/* J = 2004. */
/* J = 2005. */
/* J = 2006. */
/* J = 2007. 0 DO NOT SCALE, +1 SCALE (DEFAULT). */
/* J = 3001. */
/* J = 3002. */
/* J = 3003. */
/* J = 3004. */
/* J = 3005. */
/* J = 4001, ... , 4100. */
    goto L58;
/*   EXPORT THE VARIABLES. */
L1:
    *f = theta;
    goto L59;
L2:
    *f = beta;
    goto L59;
L3:
    *f = gamma;
    goto L59;
L4:
    *f = delta;
    goto L59;
L5:
    *r = hfract;
    goto L59;
L6:
    *r = egive;
    goto L59;
L7:
    *i = keejac;
    goto L59;
L8:
    *i = minit;
    goto L59;
L9:
    *i = maxit;
    goto L59;
L10:
    *i = kmax;
    goto L59;
L11:
    *i = kinit;
    goto L59;
L12:
    *i = mmax;
    goto L59;
L13:
    *i = ircs;
    goto L59;
L14:
    *l = xpoly;
    goto L59;
L15:
    *l = erputs;
    goto L59;
L16:
    *l = usengj;
    goto L59;
L17:
    *l = usenns;
    goto L59;
L18:
    *l = usenfd;
    goto L59;
/* IODE VERSION NUMBER. */
L19:
    *f = 3.;
    goto L59;
/* SET THE VARIABLES TO THE DEFAULTS. */
L20:
    theta = 1.;
    beta = 1.;
    gamma = 1.;
    delta = 0.;
    hfract = (float)1.;
    egive = (float)100.;
    keejac = 0;
    minit = 10;
    maxit = 50;
    kmax = 10;
    kinit = 4;
    mmax = 15;
    ircs = 1;
    xpoly = FALSE_;
    erputs = FALSE_;
    usengj = FALSE_;
    usenns = FALSE_;
    usenfd = FALSE_;
    seti_(&c__100, &c__0, n);
    n[0] = 1;
/*   IMPORT THE VARIABLES. */
    goto L59;
L21:
    theta = *f;
    if (theta == .5) {
	goto L22;
    }
    gamma = 1.;
    hfract = (float)1.;
    goto L26;
L22:
    gamma = 2.;
    hfract = (float).5;
    n[0] = 2;
    n[1] = 4;
    n[2] = 6;
    m = 4;
    goto L24;
L23:
    ++m;
L24:
    if (m > mmax) {
	goto L25;
    }
    n[m - 1] = n[m - 3] << 1;
    goto L23;
L25:
L26:
    goto L59;
L27:
    beta = *f;
    goto L59;
L28:
    gamma = *f;
    goto L59;
L29:
    delta = *f;
    goto L59;
L30:
    hfract = *r;
    goto L59;
L31:
    egive = *r;
    goto L59;
L32:
    keejac = *i;
    goto L59;
L33:
    minit = *i;
    goto L59;
L34:
    maxit = *i;
    goto L59;
L35:
    kmax = *i;
    mmax = kmax + 5;
    goto L59;
L36:
    kinit = *i;
    goto L59;
L37:
    mmax = *i;
    goto L59;
L38:
    ircs = *i;
    goto L59;
L39:
    xpoly = *l;
    goto L59;
L40:
    erputs = *l;
    if (! erputs) {
	goto L41;
    }
    delta = 1.;
    goto L42;
L41:
    delta = 0.;
L42:
    goto L59;
L43:
    usengj = *l;
    goto L59;
L44:
    usenns = *l;
    goto L59;
L45:
    usenfd = *l;
    goto L59;
L46:
    temp1 = abs(*j) > 4100;
    if (! temp1) {
	temp1 = abs(*j) < 4001;
    }
/* /6S */
/*        IF (TEMP1) CALL SETERR(24HDIODEV - J OUT OF BOUNDS, 24, 1, 2) */
/* /7S */
    if (temp1) {
	seterr_("DIODEV - J OUT OF BOUNDS", &c__24, &c__1, &c__2, 24L);
    }
/* / */
    if (*j >= 0) {
	goto L56;
    }
    if (n[1] != 0) {
	goto L47;
    }
    n[1] = (integer) (sqrt(2.) * (doublereal) ((real) n[0]));
/* EXPORT N(ABS(J)-4000) */
/* ONLY N(1) IS GIVEN, USE SQRT(2) INCREASE. */
    if (n[1] == n[0]) {
	++n[1];
    }
    n[2] = (integer) (sqrt(2.) * (doublereal) ((real) n[1]));
    if (n[2] == n[1]) {
	++n[2];
    }
    n[3] = 0;
L47:
    temp = abs(*j);
    if (n[temp - 4001] != 0) {
	goto L55;
    }
    i__1 = mmax;
    for (k = 1; k <= i__1; ++k) {
/* FILL IN THE MISSING N(M). */
	if (n[k - 1] != 0) {
	    goto L52;
	}
	if (k != 3) {
	    goto L49;
	}
	i__2 = mmax;
	for (m = k; m <= i__2; ++m) {
	    n[m - 1] = n[1] * n[m - 2] / max(1,n[0]);
/* L48: */
	}
	goto L51;
L49:
	i__2 = mmax;
	for (m = k; m <= i__2; ++m) {
	    n[m - 1] = n[m - 3] << 1;
/* L50: */
	}
L51:
	goto L54;
L52:
/* L53: */
	;
    }
L54:
L55:
    temp = abs(*j);
    *i = n[temp - 4001];
    goto L57;
L56:
    n[*j - 4001] = *i;
/* IMPORT N(J-4000) */
    if (*j - 4000 < 100) {
	n[*j - 4000] = 0;
    }
L57:
    goto L59;
L58:
    if (*j == 3005) {
	goto L45;
    }
    if (*j == 3004) {
	goto L44;
    }
    if (*j == 3003) {
	goto L43;
    }
    if (*j == 3002) {
	goto L40;
    }
    if (*j == 3001) {
	goto L39;
    }
    if (*j == 2007) {
	goto L38;
    }
    if (*j == 2006) {
	goto L37;
    }
    if (*j == 2005) {
	goto L36;
    }
    if (*j == 2004) {
	goto L35;
    }
    if (*j == 2003) {
	goto L34;
    }
    if (*j == 2002) {
	goto L33;
    }
    if (*j == 2001) {
	goto L32;
    }
    if (*j == 1002) {
	goto L31;
    }
    if (*j == 1001) {
	goto L30;
    }
    if (*j == 4) {
	goto L29;
    }
    if (*j == 3) {
	goto L28;
    }
    if (*j == 2) {
	goto L27;
    }
    if (*j == 1) {
	goto L21;
    }
    if (*j == 0) {
	goto L20;
    }
    if (*j == -6000) {
	goto L19;
    }
    if (*j == -3005) {
	goto L18;
    }
    if (*j == -3004) {
	goto L17;
    }
    if (*j == -3003) {
	goto L16;
    }
    if (*j == -3002) {
	goto L15;
    }
    if (*j == -3001) {
	goto L14;
    }
    if (*j == -2007) {
	goto L13;
    }
    if (*j == -2006) {
	goto L12;
    }
    if (*j == -2005) {
	goto L11;
    }
    if (*j == -2004) {
	goto L10;
    }
    if (*j == -2003) {
	goto L9;
    }
    if (*j == -2002) {
	goto L8;
    }
    if (*j == -2001) {
	goto L7;
    }
    if (*j == -1002) {
	goto L6;
    }
    if (*j == -1001) {
	goto L5;
    }
    if (*j == -4) {
	goto L4;
    }
    if (*j == -3) {
	goto L3;
    }
    if (*j == -2) {
	goto L2;
    }
    if (*j == -1) {
	goto L1;
    }
    goto L46;
L59:
    return 0;
} /* diodev_ */

