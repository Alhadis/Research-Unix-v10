/* d8xtrd.f -- translated by f2c (version of 5 February 1993  1:37:14).
   You must link the resulting object file with the libraries:
	-lF77 -lI77 -lm -lc   (in that order)
*/

#include "f2c.h"

logical d8xtrd_(e, nx, m, mmax, kmax, logn, beta, gamma, delta, f, pow, ldone,
	 ildone, kopto)
real *e;
integer *nx, *m, *mmax, *kmax;
doublereal *logn, *beta, *gamma, *delta;
real *f, *pow;
integer *ldone, *ildone, *kopto;
{
    /* System generated locals */
    integer e_dim1, e_offset, i__1, i__2, i__3;
    logical ret_val;

    /* Local variables */
    static integer lmax;
    static doublereal temp;
    static integer i, j, l, jsave, ldoneo, mp1, lma, jhi, mma;


/*  RETURN LDONE = THE LEVEL WHERE CONVERGENCE IS EXPECTED. */

/*  IF M=KOPTO+1, RETURN ILDONE=LDONE. */
/*  IF M.GT.KOPTO+1, DO NOT LET LDONE.GT.ILDONE+1 HAPPEN IN THE FIRST */
/*  KOPTO COLUMNS. */

/*  D8XTRD = .TRUE. IF WILL NOT CONVERGE IN THIS LOZENGE. */
/*  D8XTRD = .FALSE. IF WILL CONVERGE. */



/* ... INITIALLY, FLAG NOT CONVERGENT. */

    /* Parameter adjustments */
    --pow;
    --f;
    --logn;
    e_dim1 = *nx;
    e_offset = e_dim1 + 1;
    e -= e_offset;

    /* Function Body */
    ret_val = TRUE_;
    *ldone = 0;

    lmax = *mmax;
    if (*m > *kopto + 1) {
/* Computing MIN */
	i__1 = *ildone + 1;
	lmax = min(i__1,*mmax);
    }
    mp1 = *m + 1;
    if (mp1 > lmax) {
	goto L60;
    }

    i__1 = lmax;
    for (l = mp1; l <= i__1; ++l) {

/* ...... IF M.LE.KOPTO+1, CHECK FOR CONVERGENCE AT LEVEL L ONLY IF NO
T */
/* ...... ALREADY ACHIEVED. OTHERWISE, CHECK AT L IF NOT ALREADY DONE 
*/
/* ...... OR L=LMAX=ILDONE+1. */

	if (*ldone > 0 && (l < lmax || *m <= *kopto + 1)) {
	    goto L50;
	}

/* ...... LDONEO DETERMINES IF WILL CONVERGE IN THE FIRST KOPTO */
/* ...... COLUMNS. */

	ldoneo = l;
/* Computing MIN */
	i__2 = *m - 1;
	jhi = min(i__2,*kmax);

/* ...... COMPUTE THE FACTORS NEEDED TO CHECK FOR CONVERGENCE. */

	i__2 = jhi;
	for (j = 1; j <= i__2; ++j) {

	    mma = *m;
	    lma = l;
	    temp = (float)0.;

	    i__3 = j;
	    for (i = 1; i <= i__3; ++i) {

		--lma;
		--mma;
/* L10: */
		temp += logn[mma] - logn[lma];
	    }

/* L20: */
	    f[j] = pow[j] * *gamma * temp;
	}

/* ...... SEE IF THE I-TH VARIABLE WILL CONVERGE AT M=L. */

	i__2 = *nx;
	for (i = 1; i <= i__2; ++i) {

/* ......... CHECK EACH COLUMN FOR CONVERGENCE. */

	    i__3 = jhi;
	    for (j = 1; j <= i__3; ++j) {

		jsave = j;

		if (e[i + j * e_dim1] >= f[j]) {
		    goto L40;
		}

/* L30: */
	    }

/* ......... NO CONVERGENCE HERE. */

	    goto L50;

/* ......... IF WILL NOT CONVERGE IN THE FIRST KOPTO COLUMNS, */
/* ......... FLAG LDONEO. */

L40:
	    if (jsave > *kopto) {
		ldoneo = 0;
	    }
	}

/* ...... HAVE CONVERGENCE HERE. */

	if (*ldone == 0) {
	    *ldone = l;
	}

L50:
	;
    }

L60:
    if (*ldone == 0 || ldoneo == 0) {
	goto L70;
    }

    if (*m == *kopto + 1) {
	*ildone = *ldone;
    }

    ret_val = FALSE_;

L70:
    return ret_val;

} /* d8xtrd_ */

