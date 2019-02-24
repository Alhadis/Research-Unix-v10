/* a8xtro.f -- translated by f2c (version of 5 February 1993  1:37:14).
   You must link the resulting object file with the libraries:
	-lF77 -lI77 -lm -lc   (in that order)
*/

#include "f2c.h"

/* Subroutine */ int a8xtro_(e, nx, m, mmax, kmax, logn, gamma, f, pow, kopt, 
	dt, hopt, lnwork, cost, loglo, loghi)
real *e;
integer *nx, *m, *mmax, *kmax;
real *logn, *gamma, *f, *pow;
integer *kopt;
real *dt, *hopt, *lnwork, *cost, *loglo, *loghi;
{
    /* Initialized data */

    static real htol = (float).05;

    /* System generated locals */
    integer e_dim1, e_offset, i__1, i__2, i__3;
    real r__1, r__2;

    /* Builtin functions */
    double log(), exp();

    /* Local variables */
    static real temp;
    static integer jhim1, khip1, i, j, k;
    static real logdt, hoptk, romax, hoptm;
    static integer ks;
    static real rhomax;
    static integer lma, jhi, khi, mma;


/*  COMPUTE THE OPTIMAL K AND H. */

/*     REAL HOPT(KMAX+1),COST(KMAX+1) */


/*  HTOL IS THE RELATIVE TOLERANCE TO WHICH LOG(HOPT) WILL BE COMPUTED. */

    /* Parameter adjustments */
    --cost;
    --lnwork;
    --hopt;
    --pow;
    --f;
    --logn;
    e_dim1 = *nx;
    e_offset = e_dim1 + 1;
    e -= e_offset;

    /* Function Body */

    logdt = log((dabs(*dt)));
/* Computing MIN */
    i__1 = *m - 1;
    khi = min(i__1,*kmax);
    khip1 = khi + 1;
    *kopt = 1;

/* ... COMPUTE HOPT(K), K=1,...,MIN(M,KMAX+1). */

    i__1 = khip1;
    for (k = 1; k <= i__1; ++k) {

	jhi = min(k,khi);
	jhim1 = jhi - 1;

/* ...... COMPUTE THE FACTORS WHICH CONVERT ERRORS INTO STEP-SIZES. */

	i__2 = jhi;
	for (j = 1; j <= i__2; ++j) {

	    mma = *m;
	    lma = k + 1;
	    temp = (float)0.;

	    i__3 = j;
	    for (i = 1; i <= i__3; ++i) {

		--lma;
		--mma;
/* L10: */
		temp += logn[lma] - logn[mma];
	    }

/* L20: */
	    f[j] = *gamma * temp * pow[j];
	}

/* ...... HOPTK IS THE OPTIMAL STEP-SIZE FOR THE K-COLUMN LOZENGE. */
/* ...... HOPTM IS THE OPTIMAL STEP-SIZE FOR THE FIRST (K-1)-COLUMNS 
*/
/* ...... OF THE FULL K-COLUMN LOZENGE. */

	hoptk = *loghi;
	hoptm = *loghi;

	i__2 = *nx;
	for (i = 1; i <= i__2; ++i) {

	    rhomax = *loglo;
	    romax = *loglo;

	    i__3 = jhi;
	    for (j = 1; j <= i__3; ++j) {

/* Computing MAX */
		r__1 = rhomax, r__2 = f[j] + e[i + j * e_dim1];
		rhomax = dmax(r__1,r__2);

/* ............ SAVE THE OPTIMAL FACTOR FOR THE (K-1) COLUMN S
UB-LOZENGE. */

/* L30: */
		if (j == jhim1) {
		    romax = rhomax;
		}
	    }

	    hoptm = dmin(hoptm,romax);
/* L40: */
	    hoptk = dmin(hoptk,rhomax);
	}

	cost[k] = lnwork[k + 1] - hoptk;
	if (hoptk > hoptm + htol && k < khip1) {
	    *kopt = k;
	}

/* L50: */
/* Computing MIN */
	r__1 = hoptk + logdt, r__2 = *loghi * (float).9999;
	hopt[k] = exp((dmin(r__1,r__2))) * (*dt / dabs(*dt));
    }

/* ... SEE IF A LOWER K IS CHEAPER THAN KOPT. IF SO, USE IT. */

    i__1 = *kopt;
    for (k = 1; k <= i__1; ++k) {
	ks = k;
	if (cost[k] <= cost[*kopt]) {
	    goto L70;
	}
/* L60: */
    }

L70:
    *kopt = ks;

    return 0;

} /* a8xtro_ */

