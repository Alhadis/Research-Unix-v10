/* a8xtrp.f -- translated by f2c (version of 5 February 1993  1:37:14).
   You must link the resulting object file with the libraries:
	-lF77 -lI77 -lm -lc   (in that order)
*/

#include "f2c.h"

/* Common Block Declarations */

extern struct {
    integer mc, koptc, icost, khic, ihopt, ircnt;
    real hup;
    integer ilozng, kmaxc;
} a9xtrp_;

#define a9xtrp_1 a9xtrp_

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

/* Subroutine */ int a8xtrp_(tstart, tstop, xa, fcn, beta, gamma, delta, nx, 
	dt, n, kmax, mmax, xpoly, error, serror, errpar, output, sout, pespar,
	 logn, hopt, hopto, ng, x0, x1, loznge, f, pow, work, lnwork, cost)
real *tstart, *tstop;
/* Subroutine */ int (*xa) (), (*fcn) ();
real *beta, *gamma, *delta;
integer *nx;
real *dt;
integer *n, *kmax, *mmax;
logical *xpoly;
logical (*error) (), (*serror) ();
real *errpar;
/* Subroutine */ int (*output) (), (*sout) ();
real *pespar, *logn, *hopt, *hopto, *ng, *x0, *x1, *loznge, *f, *pow, *work, *
	lnwork, *cost;
{
    /* Initialized data */

    static real close = (float).01;
    static integer mrcnt = 3;
    static real hup0 = (float).1;
    static real hupmax = (float)7.;

    /* System generated locals */
    integer loznge_dim1, loznge_offset, i__1, i__2, i__3, i__4, i__5;
    real r__1, r__2, r__3, r__4;

    /* Builtin functions */
    double log(), exp(), sqrt();

    /* Local variables */
    static real tbad;
    static integer mold;
    static real temp;
    static integer kopt, i, k, l, m, ldone;
    static real loghi, loglo;
    static integer mused, itemp;
    static real dtnew, rtemp;
    static integer koptm, kopto;
    extern /* Subroutine */ int xtrap_();
    static real t0, t1;
    extern doublereal r1mach_();
    extern logical a8xtrd_();
    static integer ie, kf;
    extern logical a8xtrl_();
    static logical failed, ok;
    extern /* Subroutine */ int a8xtro_();
    static logical logged;
    static integer mt;
#define rs ((real *)&cstak_1)
    static real dtsave;
    extern integer istkmd_();
    static integer ie2, irtemp;
    extern /* Subroutine */ int seterr_();
    extern integer istkgt_();
    extern /* Subroutine */ int istkrl_();
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

    loglo = log(r1mach_(&c__1));
    loghi = log(r1mach_(&c__2));

/* /6S */
/*     IF ((DT/ABS(DT))*(TSTOP-TSTART).LE.0.0E0) CALL SETERR */
/*    1   (30H SXTRP - DT HAS THE WRONG SIGN,30,12,2) */
/* /7S */
    if (*dt / dabs(*dt) * (*tstop - *tstart) <= (float)0.) {
	seterr_(" SXTRP - DT HAS THE WRONG SIGN", &c__30, &c__12, &c__2, 30L);
    }
/* / */

/* ... INITIALIZE. */

    mused = 0;
    a9xtrp_1.koptc = 0;
    a9xtrp_1.ircnt = 0;
    kopto = 1;
    t1 = *tstart;
    a9xtrp_1.hup = hupmax;
    prstrt = FALSE_;

/* ... TAKE A TIME-STEP. */

L10:
    t0 = t1;
    t1 = t0 + *dt;

    if (*dt / dabs(*dt) * (t1 - *tstop) >= (float)0.) {
	t1 = *tstop;
    }
    if ((r__1 = t1 - *tstop, dabs(r__1)) <= close * dabs(*dt)) {
	t1 = *tstop;
    }

    *dt = t1 - t0;
/* /6S */
/*        IF (T0+DT.EQ.T0) CALL SETERR */
/*    1      (13H SXTRP - DT=0,13,13,1) */
/* /7S */
    if (t0 + *dt == t0) {
	seterr_(" SXTRP - DT=0", &c__13, &c__13, &c__1, 13L);
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
	a9xtrp_1.mc = m;

/* ......... GET X1=T(DT/N(M)). */

	(*xa)(&t0, &x0[1], &t1, &x1[1], nx, &n[m], fcn, &ok);

	if (! ok) {
	    goto L130;
	}

/* ......... EXTRAPOLATE THE RESULTS. */

	itemp = a9xtrp_1.ilozng;
	if (min(m,*kmax) > mused) {
	    i__2 = *nx * min(m,*kmax);
	    itemp = istkmd_(&i__2);
	}
/* /6S */
/*           IF (ILOZNG.NE.ITEMP) CALL SETERR */
/*    1         (47H SXTRP - SOMEBODY IS LEAVING STUFF ON THE STACK,47
, */
/*    2          18,2) */
/* /7S */
	if (a9xtrp_1.ilozng != itemp) {
	    seterr_(" SXTRP - SOMEBODY IS LEAVING STUFF ON THE STACK", &c__47,
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
	xtrap_(&x1[1], &m, nx, &ng[1], kmax, xpoly, &loznge[loznge_offset], &
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

	eeq0 = a8xtrl_(&rs[ie2 - 1], &rs[ie - 1], nx, &m, kmax, &pow[1], &
		loglo);
	logged = TRUE_;

	if (eeq0) {
	    istkrl_(&c__2);
	}
	if (eeq0) {
	    goto L140;
	}

/* ......... IF WILL NOT CONVERGE IN THIS LOZENGE, RESTART. */

	failed = a8xtrd_(&rs[ie2 - 1], nx, &m, mmax, kmax, &logn[1], beta, 
		gamma, delta, &f[1], &pow[1], &l, &ldone, &kopto);

	if (failed) {
	    goto L50;
	}

/* ......... SEE IF A RE-START WOULD BE MORE EFFICIENT. */

	if (m == kopto + 1) {
	    goto L40;
	}

	a8xtro_(&rs[ie2 - 1], nx, &m, mmax, kmax, &logn[1], gamma, &f[1], &
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

	if (rtemp * (*dt / dabs(*dt)) > (float)0. && work[m] + work[kopt + 2] 
		* (real) irtemp + work[kf + 1] > work[l]) {
	    goto L40;
	}
	if (rtemp * (*dt / dabs(*dt)) <= (float)0. && work[m] + work[kopt + 2]
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
	eeq0 = a8xtrl_(&rs[ie2 - 1], &rs[ie - 1], nx, &m, kmax, &pow[1], &
		loglo);
    }

    if (eeq0) {
	istkrl_(&c__2);
    }
    if (eeq0) {
	goto L140;
    }

    a8xtro_(&rs[ie2 - 1], nx, &m, mmax, kmax, &logn[1], gamma, &f[1], &pow[1],
	     &kopt, dt, &hopt[1], &lnwork[1], &cost[1], &loglo, &loghi);

L70:
    istkrl_(&c__1);

/* ...... IF HAVE A RESTART, SAVE THE BAD VALUE OF T AND */
/* ...... SET IRCNT=MRCNT. IF STEP WAS SUCCESSFUL, SAVE DT. */

    if (t0 == t1) {
	tbad = t0 + *dt;
    }
    if (t0 == t1 && t0 != *tstart) {
	a9xtrp_1.ircnt = mrcnt;
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
    r__2 = dabs(*dt) * exp(a9xtrp_1.hup), r__3 = (r__1 = hopt[koptm + 1], 
	    dabs(r__1));
    dtnew = *dt / dabs(*dt) * *pespar * dmin(r__2,r__3);

/* ...... TWO RESTARTS IN A ROW CAUSE DT TO DECREASE BY AT LEAST A */
/* ...... FACTOR OF 10**3. */

    if (prstrt && t0 == t1) {
/* Computing MIN */
	r__1 = dabs(dtnew), r__2 = dabs(*dt) * (float).001;
	dtnew = *dt / dabs(*dt) * dmin(r__1,r__2);
    }
    *dt = dtnew;

    if (t0 == t1) {
	prstrt = TRUE_;
    }

/* /6S */
/*        IF (T1+DT.EQ.T1) CALL SETERR */
/*    1      (13H SXTRP - DT=0,13,13,1) */
/* /7S */
    if (t1 + *dt == t1) {
	seterr_(" SXTRP - DT=0", &c__13, &c__13, &c__1, 13L);
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

    a9xtrp_1.koptc = kopt;
    a9xtrp_1.khic = khi;

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
    if (dabs(*dt) > (r__1 = hopt[idx], dabs(r__1)) * (float).01) {
/* Computing MIN */
/* Computing MAX */
	r__4 = (r__2 = hopt[idx] / hopto[idx], dabs(r__2));
	r__3 = dmax(r__4,(float).01);
	*dt *= dmin(r__3,(float)1.);
    }
/* /6S */
/*        IF (T1+DT.EQ.T1) CALL SETERR */
/*    1      (13H SXTRP - DT=0,13,13,1) */
/* /7S */
    if (t1 + *dt == t1) {
	seterr_(" SXTRP - DT=0", &c__13, &c__13, &c__1, 13L);
    }
/* / */
    if (t1 + *dt == t1) {
	istkrl_(&c__1);
    }
    if (t1 + *dt == t1) {
	goto L140;
    }

    if (a9xtrp_1.ircnt <= 0) {
	goto L90;
    }

/* ...... LOGARITHMIC BISECTION FOR MRCNT STEPS AFTER A RESTART. */

    --a9xtrp_1.ircnt;
    if (a9xtrp_1.ircnt == 0) {
	a9xtrp_1.hup = hup0 * (float).5;
    }
    temp = (tbad - t1) / dtsave;
    if (temp > (float).99) {
/* Computing MIN */
	r__2 = (r__1 = tbad - t1, dabs(r__1)) * (float).5, r__3 = dabs(*dt), 
		r__2 = min(r__2,r__3), r__3 = sqrt(temp) * dabs(dtsave);
	*dt = *dt / dabs(*dt) * dmin(r__2,r__3);
    }
/* /6S */
/*        IF (T1+DT.EQ.T1) CALL SETERR */
/*    1      (13H SXTRP - DT=0,13,13,1) */
/* /7S */
    if (t1 + *dt == t1) {
	seterr_(" SXTRP - DT=0", &c__13, &c__13, &c__1, 13L);
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
/*    1      (30H SXTRP - DT=0 RETURNED BY SOUT,30,14,1) */
/* /7S */
    if (t1 + *dt == t1) {
	seterr_(" SXTRP - DT=0 RETURNED BY SOUT", &c__30, &c__14, &c__1, 30L);
    }
/* / */
    if (t1 + *dt == t1) {
	goto L140;
    }

/* /6S */
/*        IF ((DT/ABS(DT))*(TSTOP-T1).LT.0.0E0) CALL SETERR */
/*    1      (47H SXTRP - DT RETURNED BY SOUT HAS THE WRONG SIGN,47,15,2) 
*/
/* /7S */
    if (*dt / dabs(*dt) * (*tstop - t1) < (float)0.) {
	seterr_(" SXTRP - DT RETURNED BY SOUT HAS THE WRONG SIGN", &c__47, &
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
    r__1 = a9xtrp_1.hup * (float)2.;
    a9xtrp_1.hup = dmin(r__1,hupmax);

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
	a9xtrp_1.ircnt = mrcnt;
    }
    tbad = t0 + *dt;

    (*sout)(&t0, &x0[1], &t1, &x1[1], nx, dt, tstop, &ok, output, rs);

/* /6S */
/*        IF (T1+DT.EQ.T1) CALL SETERR */
/*    1      (30H SXTRP - DT=0 RETURNED BY SOUT,30,14,1) */
/* /7S */
    if (t1 + *dt == t1) {
	seterr_(" SXTRP - DT=0 RETURNED BY SOUT", &c__30, &c__14, &c__1, 30L);
    }
/* / */
    if (t1 + *dt == t1) {
	goto L140;
    }

/* /6S */
/*        IF ((DT/ABS(DT))*(TSTOP-T1).LT.0.0E0) CALL SETERR */
/*    1      (47H SXTRP - DT RETURNED BY SOUT HAS THE WRONG SIGN,47,15,2) 
*/
/* /7S */
    if (*dt / dabs(*dt) * (*tstop - t1) < (float)0.) {
	seterr_(" SXTRP - DT RETURNED BY SOUT HAS THE WRONG SIGN", &c__47, &
		c__15, &c__2, 47L);
    }
/* / */

/* /6S */
/*        IF (ABS(TEMP).LT.ABS(DT)) CALL SETERR */
/*    1      (42H SXTRP - DT RAISED BY SOUT WHEN OK=.FALSE.,42,16,2) */
/* /7S */
    if (dabs(temp) < dabs(*dt)) {
	seterr_(" SXTRP - DT RAISED BY SOUT WHEN OK=.FALSE.", &c__42, &c__16, 
		&c__2, 42L);
    }
/* / */

/* ...... THE DEFAULT RESPONSE IS TO LOWER DT BY 10**3. */

    if (dabs(*dt) == dabs(temp)) {
	*dt /= (float)1e3;
    }

/* /6S */
/*        IF (T0+DT.EQ.T0) CALL SETERR */
/*    1      (13H SXTRP - DT=0,13,13,1) */
/* /7S */
    if (t0 + *dt == t0) {
	seterr_(" SXTRP - DT=0", &c__13, &c__13, &c__1, 13L);
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

} /* a8xtrp_ */

#undef rs


