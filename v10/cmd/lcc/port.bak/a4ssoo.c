/* a4ssoo.f -- translated by f2c (version of 5 February 1993  1:37:14).
   You must link the resulting object file with the libraries:
	-lF77 -lI77 -lm -lc   (in that order)
*/

#include "f2c.h"

/* Subroutine */ int a4ssoo_(e, nx, m, mmax, kmax, slogn, gamma, f, pow, kopt,
	 dt, hopt, lnwork, cost, loglo, loghi, costol)
real *e;
integer *nx, *m, *mmax, *kmax;
real *slogn, *gamma, *f, *pow;
integer *kopt;
real *dt, *hopt, *lnwork, *cost, *loglo, *loghi, *costol;
{
    /* System generated locals */
    integer e_dim1, e_offset, i__1, i__2, i__3;
    real r__1, r__2;

    /* Builtin functions */
    double exp();

    /* Local variables */
    static integer temp, i, j, k;
    static real hoptk, rhomax, minost;
    static integer jhi, khi, kmj, mmj;

/* COMPUTE THE OPTIMAL K AND H. */
/* REAL HOPT(KMAX+1),COST(KMAX+1) */
    /* Parameter adjustments */
    --cost;
    --lnwork;
    --hopt;
    --pow;
    --f;
    --slogn;
    e_dim1 = *nx;
    e_offset = e_dim1 + 1;
    e -= e_offset;

    /* Function Body */
    *kopt = 1;
/* Computing MIN */
    i__1 = *m - 1;
    khi = min(i__1,*kmax);
/* COMPUTE HOPT(K), K=1,...,MIN(M,KMAX+1). */
    temp = khi + 1;
    i__1 = temp;
    for (k = 1; k <= i__1; ++k) {
	jhi = min(k,khi);
/*   COMPUTE THE FACTORS WHICH CONVERT ERRORS INTO STEP-SIZES. */
	i__2 = jhi;
	for (j = 1; j <= i__2; ++j) {
	    kmj = k - j;
	    mmj = *m - j;
	    f[j] = *gamma * pow[j] * (slogn[k + 1] - slogn[*m] - (slogn[kmj + 
		    1] - slogn[mmj]));
/* L1: */
	}
/*   HOPTK IS THE OPTIMAL STEP-SIZE FOR THE K-COLUMN LOZENGE. */
	hoptk = *loghi;
	i__2 = *nx;
	for (i = 1; i <= i__2; ++i) {
	    rhomax = *loglo;
	    i__3 = jhi;
	    for (j = 1; j <= i__3; ++j) {
/* Computing MAX */
		r__1 = rhomax, r__2 = f[j] + e[i + j * e_dim1];
		rhomax = dmax(r__1,r__2);
/* L2: */
	    }
	    hoptk = dmin(hoptk,rhomax);
/* L3: */
	}
	cost[k] = lnwork[k + 1] - hoptk;
	if (k != 1) {
	    goto L4;
	}
	minost = cost[1];
	goto L5;
L4:
	if (k <= khi) {
/* Computing MIN */
	    r__1 = minost, r__2 = cost[k];
	    minost = dmin(r__1,r__2);
	}
L5:
	hopt[k] = exp(hoptk) * *dt;
/* L6: */
    }
/* OPTIMIZE THE COST TO WITHIN A RELATIVE TOLERANCE OF COSTTOL. */
    i__1 = khi;
    for (k = 1; k <= i__1; ++k) {
	if (cost[k] > minost + *costol) {
	    goto L7;
	}
	*kopt = k;
	goto L9;
L7:
/* L8: */
	;
    }
L9:
    return 0;
} /* a4ssoo_ */

