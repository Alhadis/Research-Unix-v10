/* dqrd.f -- translated by f2c (version of 5 February 1993  1:37:14).
   You must link the resulting object file with the libraries:
	-lF77 -lI77 -lm -lc   (in that order)
*/

#include "f2c.h"

/* Common Block Declarations */

extern struct {
    doublereal ds[500];
} cstak_;

#define cstak_1 cstak_

/* Table of constant values */

static integer c__13 = 13;
static integer c__1 = 1;
static integer c__2 = 2;
static integer c__4 = 4;
static integer c__22 = 22;
static integer c__3 = 3;

/* Subroutine */ int dqrd_(m, n, a, alfa, pivot)
integer *m, *n;
doublereal *a, *alfa;
integer *pivot;
{
    /* Initialized data */

    static doublereal rnding = 0.;

    /* System generated locals */
    integer a_dim1, a_offset, i__1, i__2;
    doublereal d__1;

    /* Builtin functions */
    double sqrt();

    /* Local variables */
    static doublereal beta;
    static integer jbar;
    extern doublereal ddot_();
    static integer temp, isum, temp1, i, j, k;
    static doublereal alfak, sigma;
    extern doublereal d1mach_();
#define is ((integer *)&cstak_1)
#define ls ((logical *)&cstak_1)
#define rs ((real *)&cstak_1)
#define ws ((doublereal *)&cstak_1)
    extern /* Subroutine */ int seterr_();
    extern integer istkgt_();
    extern /* Subroutine */ int istkrl_();
    static doublereal akk, eps;

    /* Parameter adjustments */
    --pivot;
    --alfa;
    a_dim1 = *m;
    a_offset = a_dim1 + 1;
    a -= a_offset;

    /* Function Body */
/* TO OBTAIN THE QR DECOMPOSITION OF A MATRIX. */
/* MNEMONIC - DOUBLE PRECISION QR DECOMPOSITION OF A MATRIX. */
/* INPUT - */
/*   M - THE NUMBER OF ROWS IN THE MATRIX A. */
/*   N - THE NUMBER OF COLUMNS IN THE MATRIX A. */
/*   A - THE MATRIX. */
/* OUTPUT - */
/*   A     - THE UPPER TRIANGULAR PORTION OF A ABOVE THE DIAGONAL */
/*           IS THE R OF THE QR DECOMPOSITION, WITH THE DIAGONAL */
/*           ELEMENTS OF R IN ALFA. */
/*           THE LOWER TRIANGULAR PORTION OF A STORES THE Q IN */
/*           FACTORED HOUSEHOLDER FORM. Q*A = R. */
/*   ALFA  - THE DIAGONAL OF R. */
/*   PIVOT - PIVOT(J) IS THE POSITION OF THE J-TH VARIABLE, J = 1,...,N, 
*/
/*           CHOSEN SO THAT THE MAXIMAL COLUMN IS ELIMINATED AT EACH */
/*           STEP. */
/* SCRATCH SPACE ALLOCATED - N*MU WORDS. */
/* ERROR STATES - */
/*   1 - M.LT.1. */
/*   2 - N.LT.1. */
/*   3 - SINGULAR MATRIX. (RECOVERABLE) */
/* ALFA(MIN(M,N)). */
/* THE PORT LIBRARY STACK AND ITS ALIASES. */
/* DEFINE SUM(J) WS(ISUM-1+J) */
/* CHECK THE INPUT FOR ERRORS. */
/* /6S */
/*     IF (M .LT. 1) CALL SETERR(13HDQRD - M.LT.1, 13, 1, 2) */
/*     IF (N .LT. 1) CALL SETERR(13HDQRD - N.LT.1, 13, 2, 2) */
/* /7S */
    if (*m < 1) {
	seterr_("DQRD - M.LT.1", &c__13, &c__1, &c__2, 13L);
    }
    if (*n < 1) {
	seterr_("DQRD - N.LT.1", &c__13, &c__2, &c__2, 13L);
    }
/* / */
    if (rnding == 0.) {
	rnding = d1mach_(&c__4);
    }
/* Computing 3rd power */
    i__1 = min(*m,*n), i__2 = i__1;
/* Computing 2nd power */
    d__1 = rnding * (doublereal) ((real) (i__2 * (i__1 * i__1)));
    eps = d__1 * d__1;
    isum = istkgt_(n, &c__4);
    j = 1;
    goto L2;
L1:
    ++j;
L2:
    if (j > *n) {
	goto L3;
    }
/* GET THE L2 NORM OF THE J-TH COLUMN. */
    temp1 = isum - 1 + j;
    ws[temp1 - 1] = ddot_(m, &a[j * a_dim1 + 1], &c__1, &a[j * a_dim1 + 1], &
	    c__1);
    pivot[j] = j;
    goto L1;
L3:
    k = 1;
    goto L5;
L4:
    ++k;
L5:
    if (k > min(*m,*n)) {
	goto L19;
    }
/* ELIMINATE K-TH COLUMN. */
    temp1 = isum - 1 + k;
    sigma = ws[temp1 - 1];
/* FIND THE PIVOT COLUMN. */
    jbar = k;
    j = k + 1;
    goto L7;
L6:
    ++j;
L7:
    if (j > *n) {
	goto L9;
    }
    temp1 = isum - 1 + j;
    if (sigma >= ws[temp1 - 1]) {
	goto L8;
    }
    temp = isum - 1 + j;
    sigma = ws[temp - 1];
    jbar = j;
L8:
    goto L6;
L9:
    if (jbar == k) {
	goto L11;
    }
    i = pivot[k];
/* NEED TO INTERCHANGE THE COLUMNS. */
    pivot[k] = pivot[jbar];
    pivot[jbar] = i;
    temp1 = isum - 1 + jbar;
    temp = isum - 1 + k;
    ws[temp1 - 1] = ws[temp - 1];
    temp = isum - 1 + k;
    ws[temp - 1] = sigma;
    i__1 = *m;
    for (i = 1; i <= i__1; ++i) {
	sigma = a[i + k * a_dim1];
	a[i + k * a_dim1] = a[i + jbar * a_dim1];
	a[i + jbar * a_dim1] = sigma;
/* L10: */
    }
L11:
    i__1 = *m - k + 1;
    sigma = ddot_(&i__1, &a[k + k * a_dim1], &c__1, &a[k + k * a_dim1], &c__1)
	    ;
    if (sigma > eps * ws[isum - 1]) {
	goto L12;
    }
/* /6S */
/*           CALL SETERR(22HDQRD - SINGULAR MATRIX, 22, 3, 1) */
/* /7S */
    seterr_("DQRD - SINGULAR MATRIX", &c__22, &c__3, &c__1, 22L);
/* / */
    goto L19;
L12:
    akk = a[k + k * a_dim1];
    if (akk >= 0.) {
	goto L13;
    }
    alfak = sqrt(sigma);
    goto L14;
L13:
    alfak = -sqrt(sigma);
L14:
    alfa[k] = alfak;
    beta = 1. / (sigma - akk * alfak);
    a[k + k * a_dim1] = akk - alfak;
    j = k + 1;
    goto L16;
L15:
    ++j;
L16:
    if (j > *n) {
	goto L18;
    }
    i__1 = *m + 1 - k;
    sigma = beta * ddot_(&i__1, &a[k + k * a_dim1], &c__1, &a[k + j * a_dim1],
	     &c__1);
    i__1 = *m;
    for (i = k; i <= i__1; ++i) {
	a[i + j * a_dim1] -= a[i + k * a_dim1] * sigma;
/* L17: */
    }
    temp = isum - 1 + j;
/* Computing 2nd power */
    d__1 = a[k + j * a_dim1];
    ws[temp - 1] -= d__1 * d__1;
    goto L15;
L18:
    goto L4;
L19:
    istkrl_(&c__1);
    return 0;
} /* dqrd_ */

#undef ws
#undef rs
#undef ls
#undef is


