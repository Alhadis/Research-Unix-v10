/* qrqtb.f -- translated by f2c (version of 5 February 1993  1:37:14).
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

static integer c__15 = 15;
static integer c__1 = 1;
static integer c__2 = 2;
static integer c__16 = 16;
static integer c__3 = 3;
static integer c__18 = 18;
static integer c__4 = 4;
static integer c__5 = 5;
static integer c__36 = 36;
static integer c__6 = 6;

/* Subroutine */ int qrqtb_(m, n, qr, alfa, pivot, nb, b, x)
integer *m, *n;
real *qr, *alfa;
integer *pivot, *nb;
real *b, *x;
{
    /* System generated locals */
    integer qr_dim1, qr_offset, b_dim1, b_offset, x_dim1, x_offset, i__1, 
	    i__2, i__3;

    /* Local variables */
    static integer temp;
    extern doublereal sdot_();
    static integer temp1, i, j;
    static real gamma;
    static integer jb;
#define is ((integer *)&cstak_1)
#define ls ((logical *)&cstak_1)
    static integer iz;
#define rs ((real *)&cstak_1)
#define ws ((real *)&cstak_1)
    extern /* Subroutine */ int seterr_();
    extern integer istkgt_();
    extern /* Subroutine */ int istkrl_();

/* TO SOLVE R*X = Q*B. */
/* MNEMONIC - QR Q*B AND BACK-SOLVE. */
/* INPUT - */
/*   M     - THE NUMBER OF ROWS IN THE MATRIX. */
/*   N     - THE NUMBER OF COLUMNS IN THE MATRIX. */
/*   QR    - THE QR FACTORIZATION OF A MATRIX, AS DESCRIBED IN  QRD. */
/*   ALFA  - THE DIAGONAL OF R, AS DESCRIBED IN  QRD. */
/*   PIVOT - THE PIVOTING ARRAY, AS DESCRIBED IN  QRD. */
/*   NB    - THE NUMBER OF RIGHT-HAND-SIDES. */
/*   B     - THE RIGHT-HAND-SIDES. */
/* OUTPUT - */
/*   B - HAS BEEN CLOBBERED. */
/*   X - THE SOLUTION VECTORS. */
/* SCRATCH SPACE ALLOCATED - N*MU WORDS. */
/* ERROR STATES - */
/*   1 - N.LT.1. */
/*   2 - M.LT.N. */
/*   3 - NB.LT.1. */
/*   4 - ALFA(J)=0. */
/*   5 - QR(J,J)=0. */
/*   6 - PIVOT(I) NOT ONE OF 1,...,N. */
/* THE PORT LIBRARY STACK AND ITS ALIASES. */
/* DEFINE Z(J) WS(IZ-1+J) */
/* CHECK THE INPUT FOR ERRORS. */
/* /6S */
/*     IF (N .LT. 1) CALL SETERR(15H QRQTB - N.LT.1, 15, 1, 2) */
/*     IF (M .LT. N) CALL SETERR(15H QRQTB - M.LT.N, 15, 2, 2) */
/*     IF (NB .LT. 1) CALL SETERR(16H QRQTB - NB.LT.1, 16, 3, 2) */
/* /7S */
    /* Parameter adjustments */
    x_dim1 = *n;
    x_offset = x_dim1 + 1;
    x -= x_offset;
    b_dim1 = *m;
    b_offset = b_dim1 + 1;
    b -= b_offset;
    --pivot;
    --alfa;
    qr_dim1 = *m;
    qr_offset = qr_dim1 + 1;
    qr -= qr_offset;

    /* Function Body */
    if (*n < 1) {
	seterr_(" QRQTB - N.LT.1", &c__15, &c__1, &c__2, 15L);
    }
    if (*m < *n) {
	seterr_(" QRQTB - M.LT.N", &c__15, &c__2, &c__2, 15L);
    }
    if (*nb < 1) {
	seterr_(" QRQTB - NB.LT.1", &c__16, &c__3, &c__2, 16L);
    }
/* / */
    i__1 = *n;
    for (j = 1; j <= i__1; ++j) {
/* /6S */
/*        IF (ALFA(J) .EQ. 0.) CALL SETERR(18H QRQTB - ALFA(J)=0, 18, 
4 */
/*    1      , 2) */
/* /7S */
	if (alfa[j] == (float)0.) {
	    seterr_(" QRQTB - ALFA(J)=0", &c__18, &c__4, &c__2, 18L);
	}
/* / */
/* L1: */
    }
    temp = min(*m,*n);
    i__1 = temp;
    for (j = 1; j <= i__1; ++j) {
/* /6S */
/*        IF (QR(J, J) .EQ. 0.) CALL SETERR(18H QRQTB - QR(J,J)=0, 18,
 5, */
/*    1      2) */
/* /7S */
	if (qr[j + j * qr_dim1] == (float)0.) {
	    seterr_(" QRQTB - QR(J,J)=0", &c__18, &c__5, &c__2, 18L);
	}
/* / */
/* L2: */
    }
    i__1 = *n;
    for (i = 1; i <= i__1; ++i) {
/* /6S */
/*        IF (PIVOT(I) .LT. 1 .OR. PIVOT(I) .GT. N) CALL SETERR( */
/*    1      36H QRQTB - PIVOT(I) NOT ONE OF 1,...,N, 36, 6, 2) */
/* /7S */
	if (pivot[i] < 1 || pivot[i] > *n) {
	    seterr_(" QRQTB - PIVOT(I) NOT ONE OF 1,...,N", &c__36, &c__6, &
		    c__2, 36L);
	}
/* / */
/* L3: */
    }
/* FORM Q*B. */
/* MULTIPLY ALL THE VECTORS. */
    i__1 = *nb;
    for (jb = 1; jb <= i__1; ++jb) {
/* APPLY THE J-TH TRANSFORMATION. */
	temp = min(*m,*n);
	i__2 = temp;
	for (j = 1; j <= i__2; ++j) {
	    i__3 = *m - j + 1;
	    gamma = sdot_(&i__3, &qr[j + j * qr_dim1], &c__1, &b[j + jb * 
		    b_dim1], &c__1) / (alfa[j] * qr[j + j * qr_dim1]);
	    i__3 = *m;
	    for (i = j; i <= i__3; ++i) {
		b[i + jb * b_dim1] += gamma * qr[i + j * qr_dim1];
/* L4: */
	    }
/* L5: */
	}
/* L6: */
    }
/* SOLVE R*X = Q*B. */
    iz = istkgt_(n, &c__3);
/* DO ALL THE RIGHT-HAND-SIDES. */
    i__1 = *nb;
    for (jb = 1; jb <= i__1; ++jb) {
	temp = iz + *n;
	ws[temp - 2] = b[*n + jb * b_dim1] / alfa[*n];
	i = *n - 1;
	goto L8;
L7:
	--i;
L8:
	if (i < 1) {
	    goto L9;
	}
	temp = iz - 1 + i;
	temp1 = iz + i;
	i__2 = *n - i;
	ws[temp - 1] = -(doublereal)(sdot_(&i__2, &qr[i + (i + 1) * qr_dim1], 
		m, &ws[temp1 - 1], &c__1) - b[i + jb * b_dim1]) / alfa[i];
	goto L7;
L9:
	i__2 = *n;
	for (i = 1; i <= i__2; ++i) {
	    temp1 = pivot[i];
	    temp = iz + i;
	    x[temp1 + jb * x_dim1] = ws[temp - 2];
/* L10: */
	}
/* L11: */
    }
    istkrl_(&c__1);
    return 0;
} /* qrqtb_ */

#undef ws
#undef rs
#undef ls
#undef is


