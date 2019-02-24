/* d4ssol.f -- translated by f2c (version of 5 February 1993  1:37:14).
   You must link the resulting object file with the libraries:
	-lF77 -lI77 -lm -lc   (in that order)
*/

#include "f2c.h"

/* Table of constant values */

static integer c__37 = 37;
static integer c__19 = 19;
static integer c__1 = 1;

logical d4ssol_(e2, e, nx, m, kmax, pow, logrnd)
real *e2, *e;
integer *nx, *m, *kmax;
real *pow, *logrnd;
{
    /* System generated locals */
    integer e2_dim1, e2_offset, i__1, i__2;
    real r__1, r__2, r__3;
    logical ret_val;

    /* Builtin functions */
    double log();

    /* Local variables */
    static real loge;
    static logical temp;
    static integer i, j;
    static real v;
    extern /* Subroutine */ int seterr_();
    static integer jhi;

/* TO RETURN POW(J) TIMES THE LOGARITHM OF THE RATIO OF THE DESIRED */
/* TO THE ATTAINED ERROR, FOR EACH ELEMENT IN THE LOZENGE. */

/* D4SSOL = TRUE IF NOT SUCCESSFUL. */
/* D4SSOL = FALSE IF SUCCESSFUL. */
    /* Parameter adjustments */
    --pow;
    --e;
    e2_dim1 = *nx;
    e2_offset = e2_dim1 + 1;
    e2 -= e2_offset;

    /* Function Body */
    ret_val = FALSE_;
/* Computing MIN */
    i__1 = *m - 1;
    jhi = min(i__1,*kmax);
    i__1 = *nx;
    for (i = 1; i <= i__1; ++i) {
	temp = e[i] == (float)0.;
	if (temp) {
	    temp = e2[i + e2_dim1] != (float)0.;
	}
	if (! temp) {
	    temp = e[i] < (float)0.;
	}
	if (! temp) {
	    goto L1;
	}
/* /6S */
/*           CALL SETERR(37HDESSOM - E(I).LE.0 RETURNED BY SERROR, 37,
 */
/*    1         19, 1) */
/* /7S */
	seterr_("DESSOM - E(I).LE.0 RETURNED BY SERROR", &c__37, &c__19, &
		c__1, 37L);
/* / */
	ret_val = TRUE_;
	goto L6;
L1:
	if (e[i] > (float)0.) {
	    loge = log(e[i]);
	}
	i__2 = jhi;
	for (j = 1; j <= i__2; ++j) {
	    temp = e2[i + j * e2_dim1] != (float)0.;
	    if (temp) {
		temp = e2[i + e2_dim1] != (float)0.;
	    }
	    if (! temp) {
		goto L2;
	    }
/* Computing MIN */
	    r__2 = loge - log((r__1 = e2[i + j * e2_dim1], dabs(r__1))), r__3 
		    = -(doublereal)(*logrnd) - (float)4.6;
	    v = dmin(r__2,r__3);
	    goto L3;
L2:
	    v = -(doublereal)(*logrnd) - (float)4.6;
L3:
	    e2[i + j * e2_dim1] = pow[j] * v;
/* L4: */
	}
/* L5: */
    }
L6:
    return ret_val;
} /* d4ssol_ */

