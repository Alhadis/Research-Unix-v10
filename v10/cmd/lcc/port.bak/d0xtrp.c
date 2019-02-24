/* d0xtrp.f -- translated by f2c (version of 5 February 1993  1:37:14).
   You must link the resulting object file with the libraries:
	-lF77 -lI77 -lm -lc   (in that order)
*/

#include "f2c.h"

/* Subroutine */ int d0xtrp_(tm, m, nvar, ng, kmax, xpoly, t, error, ebest, 
	rhg, emag, esave)
doublereal *tm;
integer *m, *nvar;
doublereal *ng;
integer *kmax;
logical *xpoly;
doublereal *t;
real *error, *ebest;
doublereal *rhg;
real *emag;
logical *esave;
{
    /* System generated locals */
    integer t_dim1, t_offset, error_dim1, error_offset, i__1, i__2;
    real r__1;

    /* Local variables */
    static doublereal temp;
    static integer i, j;
    static doublereal u, v;
    static integer jbest;
    static doublereal ti;
    static integer mr;
    static doublereal tv;
    static integer mmj;
    static real err;


/*     DOUBLE PRECISION RHG(MIN(M-1,KMAX)) */
/*     REAL ERROR(NVAR,MIN(M-1,KMAX)),EMAG(MIN(M-1,KMAX)) */


    /* Parameter adjustments */
    --emag;
    --rhg;
    --ebest;
    error_dim1 = *nvar;
    error_offset = error_dim1 + 1;
    error -= error_offset;
    t_dim1 = *nvar;
    t_offset = t_dim1 + 1;
    t -= t_offset;
    --ng;
    --tm;

    /* Function Body */
    if (*m > 1) {
	goto L20;
    }

/* ... INITIALIZE T. */

    i__1 = *nvar;
    for (i = 1; i <= i__1; ++i) {
/* L10: */
	t[i + t_dim1] = tm[i];
    }

    goto L80;

L20:
/* Computing MIN */
    i__1 = *m - 1;
    mr = min(i__1,*kmax);

    i__1 = mr;
    for (j = 1; j <= i__1; ++j) {
	mmj = *m - j;
	rhg[j] = ng[*m] / ng[mmj];
	emag[j] = 1. / (rhg[j] - 1.) + 1.;
	if (*xpoly) {
	    rhg[j] += -1.;
	}
/* L30: */
    }

    i__1 = *nvar;
    for (i = 1; i <= i__1; ++i) {

	v = 0.;
	u = t[i + t_dim1];
	ti = tm[i];
	t[i + t_dim1] = ti;

	i__2 = mr;
	for (j = 1; j <= i__2; ++j) {

/* ......... OBTAIN SIGNED ERROR ESTIMATE. */

	    err = (t[i + j * t_dim1] - u) * emag[j];
	    if (*esave) {
		error[i + j * error_dim1] = err;
	    }
	    err = dabs(err);
	    if (j == 1) {
		ebest[i] = err;
	    }
/* Computing MIN */
	    r__1 = ebest[i];
	    ebest[i] = dmin(r__1,err);
	    if (ebest[i] == err) {
		jbest = j;
	    }

	    if (j == *kmax) {
		goto L60;
	    }

	    if (*xpoly) {
		goto L40;
	    }

/* ......... RATIONAL EXTRAPOLATION. */

	    tv = ti - v;
	    temp = rhg[j] * (u - v) - tv;
	    if (temp != 0.) {
		ti += (ti - u) * (tv / temp);
	    }
	    v = u;
	    goto L50;

/* ......... POLYNOMIAL EXTRAPOLATION. */

L40:
	    ti += (ti - u) / rhg[j];

L50:
	    u = t[i + (j + 1) * t_dim1];
	    t[i + (j + 1) * t_dim1] = ti;
L60:
	    ;
	}

/* L70: */
	tm[i] = t[i + jbest * t_dim1];
    }

L80:
    return 0;

} /* d0xtrp_ */

