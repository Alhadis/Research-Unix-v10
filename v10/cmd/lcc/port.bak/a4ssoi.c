/* a4ssoi.f -- translated by f2c (version of 5 February 1993  1:37:14).
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

static integer c__18 = 18;
static integer c__1 = 1;
static integer c__2 = 2;
static integer c__19 = 19;
static integer c__3 = 3;
static integer c__16 = 16;
static integer c__4 = 4;
static integer c__22 = 22;
static integer c__5 = 5;
static integer c__6 = 6;
static integer c__7 = 7;
static integer c__23 = 23;
static integer c__8 = 8;
static integer c__28 = 28;
static integer c__9 = 9;
static integer c__20 = 20;
static integer c__10 = 10;
static integer c__34 = 34;
static integer c__11 = 11;
static integer c__30 = 30;
static integer c__12 = 12;
static integer c__37 = 37;
static integer c__13 = 13;
static integer c__14 = 14;

logical a4ssoi_(wv, rv, iv, lv, tstart, tstop, beta, gamma, delta, nx, dt, n, 
	kmax, mmax, xpoly, errpar, pespar, hfract, kinit)
real *wv, *rv;
integer *iv;
logical *lv;
real *tstart, *tstop, *beta, *gamma, *delta;
integer *nx;
real *dt;
integer *n, *kmax, *mmax;
logical *xpoly;
real *errpar, *pespar, *hfract;
integer *kinit;
{
    /* Initialized data */

    static real close = (float).01;
    static integer mrcnt = 3;
    static real hup0 = (float).1;

    /* System generated locals */
    integer i__1;
    real r__1;
    doublereal d__1, d__2;
    logical ret_val;

    /* Builtin functions */
    double log(), pow_dd();

    /* Local variables */
    static logical temp;
    static integer ipow;
    static logical temp1;
    static integer i, j;
    static real loghi, loglo;
    static integer mused, ircnt, icost, ihopt, iwork, kopto;
    static real t0, t1;
    extern doublereal r1mach_();
#define is ((integer *)&cstak_1)
    static integer iw;
#define ls ((logical *)&cstak_1)
#define rs ((real *)&cstak_1)
#define ws ((real *)&cstak_1)
    static real rnding, logrnd;
    static integer islogn, ihopto, ilozng;
    static real hupmax;
    extern integer istkgt_();
    extern /* Subroutine */ int seterr_();
    static integer ilwork, ix1;
    static logical prstrt;
    static integer ifa, ing, ilw;
    static real hup;

    /* Parameter adjustments */
    --errpar;
    --n;
    --lv;
    --iv;
    --rv;
    --wv;

    /* Function Body */
/* INITIALIZATION. */
    if (*tstart != *tstop) {
	goto L1;
    }
    ret_val = TRUE_;
    return ret_val;
L1:
    ret_val = FALSE_;
/* CHECK THE INPUT. */
/* /6S */
/*  2  IF (BETA .LE. 0.) CALL SETERR(18H ESSOM - BETA.LE.0, 18, 1, 2) */
/*     IF (GAMMA .LE. 0.) CALL SETERR(19H ESSOM - GAMMA.LE.0, 19, 2, 2) */
/*     IF (DELTA .LT. 0.) CALL SETERR(19H ESSOM - DELTA.LT.0, 19, 3, 2) */
/*     IF (NX .LT. 1) CALL SETERR(16H ESSOM - NX.LT.1, 16, 4, 2) */
/*     IF (TSTART+DT .EQ. TSTART) CALL SETERR(22H ESSOM - DT=0 ON INPUT */
/*    1   , 22, 5, 2) */
/*     IF (N(1) .LT. 1) CALL SETERR(18H ESSOM - N(1).LT.1, 18, 6, 2) */
/*     IF (KMAX .LT. 1) CALL SETERR(18H ESSOM - KMAX.LT.1, 18, 7, 2) */
/* /7S */
/* L2: */
    if (*beta <= (float)0.) {
	seterr_(" ESSOM - BETA.LE.0", &c__18, &c__1, &c__2, 18L);
    }
    if (*gamma <= (float)0.) {
	seterr_(" ESSOM - GAMMA.LE.0", &c__19, &c__2, &c__2, 19L);
    }
    if (*delta < (float)0.) {
	seterr_(" ESSOM - DELTA.LT.0", &c__19, &c__3, &c__2, 19L);
    }
    if (*nx < 1) {
	seterr_(" ESSOM - NX.LT.1", &c__16, &c__4, &c__2, 16L);
    }
    if (*tstart + *dt == *tstart) {
	seterr_(" ESSOM - DT=0 ON INPUT", &c__22, &c__5, &c__2, 22L);
    }
    if (n[1] < 1) {
	seterr_(" ESSOM - N(1).LT.1", &c__18, &c__6, &c__2, 18L);
    }
    if (*kmax < 1) {
	seterr_(" ESSOM - KMAX.LT.1", &c__18, &c__7, &c__2, 18L);
    }
/* / */
    temp = *mmax < *kmax + 2;
    if (! temp) {
	goto L3;
    }
    temp1 = *kmax > 1;
    if (! temp1) {
	temp1 = *mmax != 1;
    }
    temp = temp1;
/* /6S */
/*  3  IF (TEMP) CALL SETERR(23H ESSOM - MMAX.LT.KMAX+2, 23, 8, 2) */
/* /7S */
L3:
    if (temp) {
	seterr_(" ESSOM - MMAX.LT.KMAX+2", &c__23, &c__8, &c__2, 23L);
    }
/* / */
    temp = *pespar <= (float)0.;
    if (! temp) {
	temp = *pespar > (float)1.;
    }
/* /6S */
/*     IF (TEMP) CALL SETERR(28H ESSOM - PESPAR NOT IN (0,1), 28, 9, 2) */
/*     IF (HFRACT .LE. 0.) CALL SETERR(20H ESSOM - HFRACT.LE.0, 20, 10, 2 
*/
/*    1   ) */
/* /7S */
    if (temp) {
	seterr_(" ESSOM - PESPAR NOT IN (0,1)", &c__28, &c__9, &c__2, 28L);
    }
    if (*hfract <= (float)0.) {
	seterr_(" ESSOM - HFRACT.LE.0", &c__20, &c__10, &c__2, 20L);
    }
/* / */
    temp = *kinit < 1;
    if (! temp) {
	temp = *kinit > *kmax;
    }
/* /6S */
/*     IF (TEMP) CALL SETERR(34H ESSOM - KINIT NOT IN (1,...,KMAX), 34, */
/*    1   11, 2) */
/*     IF (BETA-DELTA+GAMMA .LE. 0.) CALL SETERR( */
/*    1   30H ESSOM - BETA-DELTA+GAMMA.LE.0, 30, 12, 2) */
/* /7S */
    if (temp) {
	seterr_(" ESSOM - KINIT NOT IN (1,...,KMAX)", &c__34, &c__11, &c__2, 
		34L);
    }
    if (*beta - *delta + *gamma <= (float)0.) {
	seterr_(" ESSOM - BETA-DELTA+GAMMA.LE.0", &c__30, &c__12, &c__2, 30L);
    }
/* / */
/* ALLOCATE AND LOAD THE ARRAY SLOGN WITH LOG(N(1))+ ... +LOG(N(I-1)). */
    i__1 = *mmax + 1;
    islogn = istkgt_(&i__1, &c__3);
    ws[islogn - 1] = (float)0.;
    j = 2;
    goto L5;
L4:
    ++j;
L5:
    if (j > *mmax) {
	goto L6;
    }
/* /6S */
/*        IF (N(J-1) .GE. N(J)) CALL SETERR( */
/*    1      37H ESSOM - N IS NOT MONOTONE INCREASING, 37, 13, 2) */
/* /7S */
    if (n[j - 1] >= n[j]) {
	seterr_(" ESSOM - N IS NOT MONOTONE INCREASING", &c__37, &c__13, &
		c__2, 37L);
    }
/* / */
    i = islogn + j - 1;
    ws[i - 1] = ws[i - 2] + log((real) n[j - 1]);
    goto L4;
L6:
    i = islogn + *mmax;
    ws[i - 1] = ws[i - 2] + log((real) n[*mmax]);
/* /6S */
/*     IF (DT/ABS(DT)*(TSTOP-TSTART) .LE. 0.) CALL SETERR( */
/*    1   30H ESSOM - DT HAS THE WRONG SIGN, 30, 14, 2) */
/* /7S */
    if (*dt / dabs(*dt) * (*tstop - *tstart) <= (float)0.) {
	seterr_(" ESSOM - DT HAS THE WRONG SIGN", &c__30, &c__14, &c__2, 30L);
    }
/* / */
/* ALLOCATE CURRENT AND OLD OPTIMAL STEP-SIZE ARRAYS. */
    i__1 = *kmax + 1;
    ihopt = istkgt_(&i__1, &c__3);
    i__1 = *kmax + 1;
    ihopto = istkgt_(&i__1, &c__3);
/* ALLOCATE AND LOAD THE ARRAY NG WITH N(J)**GAMMA. */
    ing = istkgt_(mmax, &c__3);
    i__1 = *mmax;
    for (j = 1; j <= i__1; ++j) {
	i = ing + j - 1;
	d__1 = (doublereal) ((real) n[j]);
	d__2 = (doublereal) (*gamma);
	ws[i - 1] = pow_dd(&d__1, &d__2);
/* L7: */
    }
/* ALLOCATE SPACE FOR X1 (THE SOLUTION AT TIME T1) AND A SCRATCH ARRAY F. 
*/
    ix1 = istkgt_(nx, &c__3);
    ifa = istkgt_(kmax, &c__3);
/* ALLOCATE AND LOAD POW(J) WITH 1/(BETA-DELTA+J*GAMMA). */
    ipow = istkgt_(kmax, &c__3);
    i__1 = *kmax;
    for (j = 1; j <= i__1; ++j) {
	i = ipow + j - 1;
	rs[i - 1] = (float)1. / (*beta - *delta + (real) j * *gamma);
/* L8: */
    }
/* ALLOCATE AND LOAD ARRAYS WORK AND LWORK WITH */
/* SUM(I=1,...,J)(N(I)) AND LOG(WORK(J)), RESPECTIVELY. */
    iwork = istkgt_(mmax, &c__3);
    ilwork = istkgt_(mmax, &c__3);
    rs[iwork - 1] = (real) n[1];
    rs[ilwork - 1] = log(rs[iwork - 1]);
    j = 2;
    goto L10;
L9:
    ++j;
L10:
    if (j > *mmax) {
	goto L11;
    }
    iw = iwork + j - 1;
    rs[iw - 1] = rs[iw - 2] + (real) n[j];
    ilw = ilwork + j - 1;
    rs[ilw - 1] = log(rs[iw - 1]);
    goto L9;
/* ALLOCATE THE COST/UNIT TIME-STEP ARRAY. */
L11:
    i__1 = *kmax + 1;
    icost = istkgt_(&i__1, &c__3);
/* ALLOCATE THE EXTRAPOLATION LOZENGE LAST SO THAT ISTKMD CAN */
/* BE USED TO LET IT GROW ONLY AS NEEDED. */
    ilozng = istkgt_(&c__1, &c__3);
/* GET THE LOGARITHMS OF THE LARGEST AND SMALLEST NUMBERS, */
/* AS WELL AS THE ROUNDING ERROR LEVEL, OF THE MACHINE. */
    loglo = log(r1mach_(&c__1));
    loghi = log(r1mach_(&c__2));
    rnding = r1mach_(&c__4);
    logrnd = log(rnding);
    hupmax = -(doublereal)(logrnd + (float)4.6) / (*beta + *gamma);
    mused = 0;
    ircnt = 0;
    kopto = *kinit;
    hup = hupmax;
    prstrt = FALSE_;
    t0 = *tstart;
    t1 = t0 + *dt;
    temp = *dt / dabs(*dt) * (t1 - *tstop) >= (float)0.;
    if (! temp) {
	temp = (r__1 = t1 - *tstop, dabs(r__1)) <= close * dabs(*dt);
    }
    if (temp) {
	t1 = *tstop;
    }
    *dt = t1 - t0;
/* LOAD THE VARIABLE ARRAYS WITH THE APPROPRIATE VALUES. */
    wv[1] = *tstart;
    wv[2] = *tstop;
    wv[3] = *beta;
    wv[4] = *gamma;
    wv[5] = *delta;
    wv[6] = *dt;
    wv[7] = close;
    wv[9] = *dt;
    wv[10] = t0;
    wv[11] = t1;
    rv[1] = errpar[1];
    rv[2] = errpar[2];
    rv[3] = *pespar;
    rv[4] = *hfract;
    rv[5] = hup0;
    rv[6] = hupmax;
    rv[7] = hup;
    rv[8] = loglo;
    rv[9] = loghi;
    rv[10] = logrnd;
    rv[11] = rnding;
    rv[12] = (float).01;
    iv[1] = *nx;
    iv[2] = *kmax;
    iv[3] = *mmax;
    iv[4] = *kinit;
    iv[5] = icost;
    iv[6] = ihopt;
    iv[7] = ihopto;
    iv[8] = iwork;
    iv[9] = ilwork;
    iv[10] = ipow;
    iv[11] = islogn;
    iv[12] = ix1;
    iv[13] = ilozng;
    iv[14] = mrcnt;
    iv[15] = 0;
    iv[16] = kopto;
    iv[17] = kopto;
    iv[20] = mused;
    iv[25] = ircnt;
    iv[26] = ifa;
    iv[27] = ing;
    lv[1] = *xpoly;
    lv[3] = prstrt;
    return ret_val;
} /* a4ssoi_ */

#undef ws
#undef rs
#undef ls
#undef is


