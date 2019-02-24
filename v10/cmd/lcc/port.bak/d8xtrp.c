/* d8xtrp.f -- translated by f2c (version of 5 February 1993  1:37:14).
   You must link the resulting object file with the libraries:
	-lF77 -lI77 -lm -lc   (in that order)
*/

#include "f2c.h"

/* Common Block Declarations */

extern struct {
    integer mc, koptc, icost, khic, ihopt, ircnt;
    real hup;
    integer ilozng, kmaxc;
} d9xtrp_;

#define d9xtrp_1 d9xtrp_

extern struct {
    doublereal ds[500];
} cstak_;

#define cstak_1 cstak_

/* Table of constant values */

static integer c__1 = 1;
static integer c__2 = 2;
static integer c__30 = 30;
static integer c__12 = 12;
static integer c__13 = 13;
static integer c__47 = 47;
static integer c__18 = 18;
static integer c__3 = 3;
static integer c__14 = 14;
static integer c__15 = 15;
static integer c__42 = 42;
static integer c__16 = 16;

/* Subroutine */ int d8xtrp_(tstart, tstop, xa, fcn, beta, gamma, delta, nx, 
	dt, n, kmax, mmax, xpoly, error, serror, errpar, output, sout, pespar,
	 logn, hopt, hopto, ng, x0, x1, loznge, f, pow, work, lnwork, cost)
doublereal *tstart, *tstop;
/* Subroutine */ int (*xa) (), (*fcn) ();
doublereal *beta, *gamma, *delta;
integer *nx;
doublereal *dt;
integer *n, *kmax, *mmax;
logical *xpoly;
logical (*error) (), (*serror) ();
real *errpar;
/* Subroutine */ int (*output) (), (*sout) ();
real *pespar;
doublereal *logn;
real *hopt, *hopto;
doublereal *ng, *x0, *x1, *loznge;
real *f, *pow, *work, *lnwork, *cost;
{
    /* Initialized data */

    static doublereal close = .01;
    static integer mrcnt = 3;
    static real hup0 = (float).1;
    static real hupmax = (float)7.;

    /* System generated locals */
    integer loznge_dim1, loznge_offset, i__1, i__2, i__3, i__4, i__5;
    real r__1, r__2, r__3, r__4;
    doublereal d__1, d__2, d__3;

    /* Builtin functions */
    double log(), exp(), sqrt();

    /* Local variables */
    static doublereal tbad;
    static integer mold;
    static doublereal temp;
    static integer kopt, i, k, l, m, ldone;
    static real loghi, loglo;
    static integer mused, itemp;
    static doublereal dtnew;
    static real rtemp;
    static integer koptm, kopto;
    extern doublereal d1mach_();
    static doublereal t0, t1;
    extern logical d8xtrd_();
    static integer ie, kf;
    static logical failed;
    extern logical d8xtrl_();
    static logical ok;
    extern /* Subroutine */ int d8xtro_();
    static logical logged;
    static integer mt;
#define rs ((real *)&cstak_1)
    static doublereal dtsave;
    extern integer istkmd_();
    static integer ie2, irtemp;
    extern /* Subroutine */ int seterr_();
    extern integer istkgt_();
    extern /* Subroutine */ int dxtrap_(), istkrl_();
    static logical prstrt;
    static integer khi, idx;
    static logical eeq0;



/*     REAL HOPT(KMAX+1),HOPTO(KMAX+1),COST(KMAX+1) */



/*  IF T0 AND T1 ARE ON THE SAME SIDE OF TSTOP AND T1 IS WITHIN */
/*  CLOSE*ABS(DT) OF TSTOP, THEN T1=TSTOP IS CHOSEN. */

/*  MRCNT LOGARITHMIC BISECTION STEPS ARE TAKEN AFTER EACH RESTART. */

/*  ALLOW HOPT(NEW)/HOPT(OLD) TO RISE BY AT MOST EXP(HUP) WHERE */
/*  HUP=HUP0 AT THE END OF A RESTART SEQUENCE AND EACH SUCCESSFUL STEP */
/*  THEREAFTER RESULTS IN HUP=MIN(2*HUP,HUPMAX). */

    /* Parameter adjustments */
    --cost;
    --lnwork;
    --work;
    --pow;
    --f;
    loznge_dim1 = *nx;
    loznge_offset = loznge_dim1 + 1;
    loznge -= loznge_offset;
    --x1;
    --x0;
    --ng;
    --hopto;
    --hopt;
    --logn;
    --errpar;
    --n;

    /* Function Body */

/* ... GET THE LOGARITHMS OF THE LARGEST AND SMALLEST NUMBERS IN THE */
/* ... MACHINE. */

    loglo = log(d1mach_(&c__1));
    loghi = log(d1mach_(&c__2));

/* /6S */
/*     IF ((DT/DABS(DT))*(TSTOP-TSTART).LE.0.0D0) CALL SETERR */
/*    1   (30HDSXTRP - DT HAS THE WRONG SIGN,30,12,2) */
/* /7S */
    if (*dt / abs(*dt) * (*tstop - *tstart) <= 0.) {
	seterr_("DSXTRP - DT HAS THE WRONG SIGN", &c__30, &c__12, &c__2, 30L);
    }
/* / */

/* ... INITIALIZE. */

    mused = 0;
    d9xtrp_1.koptc = 0;
    d9xtrp_1.ircnt = 0;
    kopto = 1;
    t1 = *tstart;
    d9xtrp_1.hup = hupmax;
    prstrt = FALSE_;

/* ... TAKE A TIME-STEP. */

L10:
    t0 = t1;
    t1 = t0 + *dt;

    if (*dt / abs(*dt) * (t1 - *tstop) >= 0.) {
	t1 = *tstop;
    }
    if ((d__1 = t1 - *tstop, abs(d__1)) <= close * abs(*dt)) {
	t1 = *tstop;
    }

    *dt = t1 - t0;
/* /6S */
/*        IF (T0+DT.EQ.T0) CALL SETERR */
/*    1      (13HDSXTRP - DT=0,13,13,1) */
/* /7S */
    if (t0 + *dt == t0) {
	seterr_("DSXTRP - DT=0", &c__13, &c__13, &c__1, 13L);
    }
/* / */
    if (t0 + *dt == t0) {
	goto L140;
    }

    i__1 = *mmax;
    for (mt = 1; mt <= i__1; ++mt) {

	failed = FALSE_;
	logged = FALSE_;
	m = mt;
	d9xtrp_1.mc = m;

/* ......... GET X1=T(DT/N(M)). */

	(*xa)(&t0, &x0[1], &t1, &x1[1], nx, &n[m], fcn, &ok);

	if (! ok) {
	    goto L130;
	}

/* ......... EXTRAPOLATE THE RESULTS. */

	itemp = d9xtrp_1.ilozng;
	if (min(m,*kmax) > mused) {
	    i__2 = *nx * min(m,*kmax);
	    itemp = istkmd_(&i__2);
	}
/* /6S */
/*           IF (ILOZNG.NE.ITEMP) CALL SETERR */
/*    1         (47HDSXTRP - SOMEBODY IS LEAVING STUFF ON THE STACK,47
, */
/*    2          18,2) */
/* /7S */
	if (d9xtrp_1.ilozng != itemp) {
	    seterr_("DSXTRP - SOMEBODY IS LEAVING STUFF ON THE STACK", &c__47,
		     &c__18, &c__2, 47L);
	}
/* / */
	mused = max(m,mused);

	ie = istkgt_(nx, &c__3);
/* Computing MAX */
/* Computing MIN */
	i__5 = m - 1;
	i__3 = 1, i__4 = *nx * min(i__5,*kmax);
	i__2 = max(i__3,i__4);
	ie2 = istkgt_(&i__2, &c__3);
	dxtrap_(&x1[1], &m, nx, &ng[1], kmax, xpoly, &loznge[loznge_offset], &
		rs[ie2 - 1], &rs[ie - 1]);

/* ......... CHECK FOR CONVERGENCE. */

	if (m == 1) {
	    goto L40;
	}

	if ((*serror)(&x1[1], nx, &t1, dt, &errpar[1], delta, &rs[ie - 1], 
		error)) {
	    goto L60;
	}

	if (m <= kopto) {
	    goto L40;
	}

/* ......... SEE IF A RE-START IS NECESSARY. */

	eeq0 = d8xtrl_(&rs[ie2 - 1], &rs[ie - 1], nx, &m, kmax, &pow[1], &
		loglo);
	logged = TRUE_;

	if (eeq0) {
	    istkrl_(&c__2);
	}
	if (eeq0) {
	    goto L140;
	}

/* ......... IF WILL NOT CONVERGE IN THIS LOZENGE, RESTART. */

	failed = d8xtrd_(&rs[ie2 - 1], nx, &m, mmax, kmax, &logn[1], beta, 
		gamma, delta, &f[1], &pow[1], &l, &ldone, &kopto);

	if (failed) {
	    goto L50;
	}

/* ......... SEE IF A RE-START WOULD BE MORE EFFICIENT. */

	if (m == kopto + 1) {
	    goto L40;
	}

	d8xtro_(&rs[ie2 - 1], nx, &m, mmax, kmax, &logn[1], gamma, &f[1], &
		pow[1], &kopt, dt, &hopt[1], &lnwork[1], &cost[1], &loglo, &
		loghi);

	rtemp = *dt / hopt[kopt + 1];
	if (rtemp > (float)1e3 && work[m] + work[kopt + 2] * rtemp > work[l]) 
		{
	    goto L40;
	}

	if (rtemp > (float)1e3) {
	    goto L30;
	}

	irtemp = (integer) rtemp;
	rtemp = (rtemp - (real) irtemp) * hopt[kopt + 1];
	kf = kopt + 1;

	i__2 = kopt;
	for (i = 1; i <= i__2; ++i) {
/* L20: */
	    if (dabs(rtemp) <= (r__1 = hopt[i], dabs(r__1)) && kf > kopt) {
		kf = i;
	    }
	}

	if (rtemp * (*dt / abs(*dt)) > (float)0. && work[m] + work[kopt + 2] *
		 (real) irtemp + work[kf + 1] > work[l]) {
	    goto L40;
	}
	if (rtemp * (*dt / abs(*dt)) <= (float)0. && work[m] + work[kopt + 2] 
		* (real) irtemp > work[l]) {
	    goto L40;
	}

/* ......... SIGNAL A RESTART. */

L30:
	t1 = t0;
	goto L70;

L40:
	if (m < *mmax) {
	    istkrl_(&c__2);
	}
    }

/* ...... DID NOT CONVERGE, TRY IT OVER AGAIN. */

L50:
    t1 = t0;

/* ...... FIND THE OPTIMAL DT AND M FOR THE NEXT TIME-STEP. */

L60:
    if (! logged) {
	eeq0 = d8xtrl_(&rs[ie2 - 1], &rs[ie - 1], nx, &m, kmax, &pow[1], &
		loglo);
    }

    if (eeq0) {
	istkrl_(&c__2);
    }
    if (eeq0) {
	goto L140;
    }

    d8xtro_(&rs[ie2 - 1], nx, &m, mmax, kmax, &logn[1], gamma, &f[1], &pow[1],
	     &kopt, dt, &hopt[1], &lnwork[1], &cost[1], &loglo, &loghi);

L70:
    istkrl_(&c__1);

/* ...... IF HAVE A RESTART, SAVE THE BAD VALUE OF T AND */
/* ...... SET IRCNT=MRCNT. IF STEP WAS SUCCESSFUL, SAVE DT. */

    if (t0 == t1) {
	tbad = t0 + *dt;
    }
    if (t0 == t1 && t0 != *tstart) {
	d9xtrp_1.ircnt = mrcnt;
    }
    if (t0 != t1) {
	dtsave = *dt;
    }

/* ...... GET THE DT FOR THE NEXT TIME-STEP. */

/* Computing MIN */
    i__1 = kopt, i__2 = kopto + 1;
    kopt = min(i__1,i__2);
    koptm = kopt;
    if (t0 == t1) {
	koptm = min(kopt,kopto);
    }
/* Computing MIN */
    r__3 = (r__1 = *dt, dabs(r__1)) * exp(d9xtrp_1.hup), r__4 = (r__2 = hopt[
	    koptm + 1], dabs(r__2));
    dtnew = *dt / abs(*dt) * *pespar * dmin(r__3,r__4);

/* ...... TWO RESTARTS IN A ROW CAUSE DT TO DECREASE BY AT LEAST A */
/* ...... FACTOR OF 10**3. */

    if (prstrt && t0 == t1) {
/* Computing MIN */
	d__1 = abs(dtnew), d__2 = abs(*dt) * .001;
	dtnew = *dt / abs(*dt) * min(d__1,d__2);
    }
    *dt = dtnew;

    if (t0 == t1) {
	prstrt = TRUE_;
    }

/* /6S */
/*        IF (T1+DT.EQ.T1) CALL SETERR */
/*    1      (13HDSXTRP - DT=0,13,13,1) */
/* /7S */
    if (t1 + *dt == t1) {
	seterr_("DSXTRP - DT=0", &c__13, &c__13, &c__1, 13L);
    }
/* / */
    if (t1 + *dt == t1) {
	istkrl_(&c__1);
    }
    if (t1 + *dt == t1) {
	goto L140;
    }

/* Computing MIN */
    i__1 = m, i__2 = *kmax + 1;
    khi = min(i__1,i__2);

/* ...... SET THE INFORMATION IN COMMON AND COMPUTE THE */
/* ...... COST/UNIT TIME-STEP FOR EACH LOZENGE SIZE. */

    d9xtrp_1.koptc = kopt;
    d9xtrp_1.khic = khi;

    i__1 = khi;
    for (k = 1; k <= i__1; ++k) {
/* L80: */
	cost[k] = work[k + 1] / (r__1 = hopt[k], dabs(r__1));
    }

    if (t0 == *tstart || t0 == t1) {
	goto L90;
    }

/* ...... IF ABS(HOPT(NEW)).LT.ABS(HOPT(OLD)), BE CONSERVATIVE. */

/* Computing MIN */
    i__1 = kopt + 1, i__2 = mold - 1, i__1 = min(i__1,i__2), i__2 = m - 1;
    idx = min(i__1,i__2);
    if (abs(*dt) > (r__1 = hopt[idx], dabs(r__1)) * .01) {
/* Computing MIN */
/* Computing MAX */
	r__4 = (r__2 = hopt[idx] / hopto[idx], dabs(r__2));
	r__3 = dmax(r__4,(float).01);
	*dt *= dmin(r__3,(float)1.);
    }
/* /6S */
/*        IF (T1+DT.EQ.T1) CALL SETERR */
/*    1      (13HDSXTRP - DT=0,13,13,1) */
/* /7S */
    if (t1 + *dt == t1) {
	seterr_("DSXTRP - DT=0", &c__13, &c__13, &c__1, 13L);
    }
/* / */
    if (t1 + *dt == t1) {
	istkrl_(&c__1);
    }
    if (t1 + *dt == t1) {
	goto L140;
    }

    if (d9xtrp_1.ircnt <= 0) {
	goto L90;
    }

/* ...... LOGARITHMIC BISECTION FOR MRCNT STEPS AFTER A RESTART. */

    --d9xtrp_1.ircnt;
    if (d9xtrp_1.ircnt == 0) {
	d9xtrp_1.hup = hup0 * (float).5;
    }
    temp = (tbad - t1) / dtsave;
    if (temp > .99) {
/* Computing MIN */
	d__2 = (d__1 = tbad - t1, abs(d__1)) * .5, d__3 = abs(*dt), d__2 = 
		min(d__2,d__3), d__3 = sqrt(temp) * abs(dtsave);
	*dt = *dt / abs(*dt) * min(d__2,d__3);
    }
/* /6S */
/*        IF (T1+DT.EQ.T1) CALL SETERR */
/*    1      (13HDSXTRP - DT=0,13,13,1) */
/* /7S */
    if (t1 + *dt == t1) {
	seterr_("DSXTRP - DT=0", &c__13, &c__13, &c__1, 13L);
    }
/* / */
    if (t1 + *dt == t1) {
	istkrl_(&c__1);
    }
    if (t1 + *dt == t1) {
	goto L140;
    }

/* ...... OUTPUT THE RESULTS FOR THIS TIME-STEP. */

L90:
    (*sout)(&t0, &x0[1], &t1, &x1[1], nx, dt, tstop, &ok, output, &rs[ie - 1])
	    ;

    istkrl_(&c__1);

/* /6S */
/*        IF (T1+DT.EQ.T1) CALL SETERR */
/*    1      (30HDSXTRP - DT=0 RETURNED BY SOUT,30,14,1) */
/* /7S */
    if (t1 + *dt == t1) {
	seterr_("DSXTRP - DT=0 RETURNED BY SOUT", &c__30, &c__14, &c__1, 30L);
    }
/* / */
    if (t1 + *dt == t1) {
	goto L140;
    }

/* /6S */
/*        IF ((DT/DABS(DT))*(TSTOP-T1).LT.0.0D0) CALL SETERR */
/*    1      (47HDSXTRP - DT RETURNED BY SOUT HAS THE WRONG SIGN,47,15,2) 
*/
/* /7S */
    if (*dt / abs(*dt) * (*tstop - t1) < 0.) {
	seterr_("DSXTRP - DT RETURNED BY SOUT HAS THE WRONG SIGN", &c__47, &
		c__15, &c__2, 47L);
    }
/* / */

    if (t0 == t1) {
	kopto = min(kopt,kopto);
    }
    if (t0 == t1) {
	goto L120;
    }

/* ...... UPDATE X0, HOPT AND HUP IF CONVERGED. */

    i__1 = *nx;
    for (i = 1; i <= i__1; ++i) {
/* L100: */
	x0[i] = x1[i];
    }

    i__1 = khi;
    for (k = 1; k <= i__1; ++k) {
/* L110: */
	hopto[k] = hopt[k];
    }

/* Computing MIN */
    r__1 = d9xtrp_1.hup * (float)2.;
    d9xtrp_1.hup = dmin(r__1,hupmax);

    mold = m;
    kopto = kopt;
    prstrt = FALSE_;

L120:
    if (t1 == *tstop) {
	goto L140;
    }

/* ...... GO DO THE NEXT TIME-STEP. */

    goto L10;

/* ...... THINGS ARE NOT OK. */

L130:
    t1 = t0;
    prstrt = TRUE_;

/* ...... DO NOT LET THE USER RAISE DT WHEN OK=.FALSE. */

    temp = *dt;

/* ...... GO INTO A FULL RESTART SEQUENCE. */

    if (t0 != *tstart) {
	d9xtrp_1.ircnt = mrcnt;
    }
    tbad = t0 + *dt;

    (*sout)(&t0, &x0[1], &t1, &x1[1], nx, dt, tstop, &ok, output, rs);

/* /6S */
/*        IF (T1+DT.EQ.T1) CALL SETERR */
/*    1      (30HDSXTRP - DT=0 RETURNED BY SOUT,30,14,1) */
/* /7S */
    if (t1 + *dt == t1) {
	seterr_("DSXTRP - DT=0 RETURNED BY SOUT", &c__30, &c__14, &c__1, 30L);
    }
/* / */
    if (t1 + *dt == t1) {
	goto L140;
    }

/* /6S */
/*        IF ((DT/DABS(DT))*(TSTOP-T1).LT.0.0D0) CALL SETERR */
/*    1      (47HDSXTRP - DT RETURNED BY SOUT HAS THE WRONG SIGN,47,15,2) 
*/
/* /7S */
    if (*dt / abs(*dt) * (*tstop - t1) < 0.) {
	seterr_("DSXTRP - DT RETURNED BY SOUT HAS THE WRONG SIGN", &c__47, &
		c__15, &c__2, 47L);
    }
/* / */

/* /6S */
/*        IF (DABS(TEMP).LT.DABS(DT)) CALL SETERR */
/*    1      (42HDSXTRP - DT RAISED BY SOUT WHEN OK=.FALSE.,42,16,2) */
/* /7S */
    if (abs(temp) < abs(*dt)) {
	seterr_("DSXTRP - DT RAISED BY SOUT WHEN OK=.FALSE.", &c__42, &c__16, 
		&c__2, 42L);
    }
/* / */

/* ...... THE DEFAULT RESPONSE IS TO LOWER DT BY 10**3. */

    if (abs(*dt) == abs(temp)) {
	*dt /= 1e3;
    }

/* /6S */
/*        IF (T0+DT.EQ.T0) CALL SETERR */
/*    1      (13HDSXTRP - DT=0,13,13,1) */
/* /7S */
    if (t0 + *dt == t0) {
	seterr_("DSXTRP - DT=0", &c__13, &c__13, &c__1, 13L);
    }
/* / */
    if (t0 + *dt == t0) {
	goto L140;
    }

    if (t1 != *tstop) {
	goto L10;
    }

L140:
    return 0;

} /* d8xtrp_ */

#undef rs


