/* a4ssod.f -- translated by f2c (version of 5 February 1993  1:37:14).
   You must link the resulting object file with the libraries:
	-lF77 -lI77 -lm -lc   (in that order)
*/

#include "f2c.h"

logical a4ssod_(e, nx, m, mmax, kmax, slogn, beta, gamma, delta, f, pow, 
	ldone, ildone, kopto)
real *e;
integer *nx, *m, *mmax, *kmax;
real *slogn, *beta, *gamma, *delta, *f, *pow;
integer *ldone, *ildone, *kopto;
{
    /* Initialized data */

    static integer lgive = 1;

    /* System generated locals */
    integer e_dim1, e_offset, i__1, i__2, i__3;
    logical ret_val;

    /* Local variables */
    static integer lmax;
    static logical temp;
    static integer temp1, i, j, l, jsave, ldoneo, jhi, lmj, mmj;

    /* Parameter adjustments */
    --pow;
    --f;
    --slogn;
    e_dim1 = *nx;
    e_offset = e_dim1 + 1;
    e -= e_offset;

    /* Function Body */
/* RETURN LDONE = THE LEVEL WHERE CONVERGENCE IS EXPECTED. */

/* IF M = KOPTO+1, RETURN ILDONE=LDONE. */
/* IF M > KOPTO+1, DO NOT LET LDONE > ILDONE+1 HAPPEN IN THE FIRST */
/* KOPTO COLUMNS. */

/* A4SSOD = TRUE IF WILL NOT CONVERGE IN THIS LOZENGE. */
/* A4SSOD = FALSE IF WILL CONVERGE. */
    ret_val = TRUE_;
/* INITIALLY, FLAG NOT CONVERGENT. */
    *ldone = 0;
    if (*m <= *kopto + 1) {
	goto L1;
    }
/* Computing MIN */
    i__1 = *ildone + lgive;
    lmax = min(i__1,*mmax);
    goto L2;
L1:
    lmax = *mmax;
L2:
    if (*m >= lmax) {
	goto L10;
    }
    temp1 = *m + 1;
    i__1 = lmax;
    for (l = temp1; l <= i__1; ++l) {
	if (*ldone > 0 && (l < lmax || *m <= *kopto + 1)) {
	    goto L9;
	}
/*     LDONEO DETERMINES IF WILL CONVERGE IN THE FIRST KOPTO */
/*     COLUMNS. SET TRUE INITIALLY. */
	ldoneo = l;
/* Computing MIN */
	i__2 = *m - 1;
	jhi = min(i__2,*kmax);
/* COMPUTE THE FACTORS USED FOR CONVERGENCE CHECK. */
	i__2 = jhi;
	for (j = 1; j <= i__2; ++j) {
	    mmj = *m - j;
	    lmj = l - j;
	    f[j] = pow[j] * *gamma * (slogn[*m] - slogn[l] - (slogn[mmj] - 
		    slogn[lmj]));
/* L3: */
	}
/* SEE IF THE I-TH VARIABLE WILL CONVERGE AT M=L. */
	i__2 = *nx;
	for (i = 1; i <= i__2; ++i) {
/* FLAG NOT CONVERGENT. */
	    jsave = 0;
/* CHECK EACH COLUMN FOR CONVERGENCE. */
	    i__3 = jhi;
	    for (j = 1; j <= i__3; ++j) {
		if (e[i + j * e_dim1] < f[j]) {
		    goto L4;
		}
		jsave = j;
		goto L6;
L4:
/* L5: */
		;
	    }
L6:
	    if (jsave == 0) {
		goto L8;
	    }
/* NO CONVERGENCE. */
	    if (jsave > *kopto) {
		ldoneo = 0;
	    }
/* NO CONVERGENCE IN */
/* COLUMNS 1,...,KOPTO. */
/* L7: */
	}
L8:
	if (jsave == 0) {
	    goto L9;
	}
/* NO CONVERGENCE, TRY THE NEXT L. */
	if (*ldone == 0) {
	    *ldone = l;
	}
L9:
	;
    }
L10:
    temp = *ldone != 0;
    if (temp) {
	temp = ldoneo != 0;
    }
    if (! temp) {
	goto L11;
    }
    if (*m == *kopto + 1) {
	*ildone = *ldone;
    }
/* WILL CONVERGE AT M = LDONE. */
    ret_val = FALSE_;
L11:
    return ret_val;
} /* a4ssod_ */

