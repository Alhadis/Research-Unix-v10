/* iode4.f -- translated by f2c (version of 5 February 1993  1:37:14).
   You must link the resulting object file with the libraries:
	-lF77 -lI77 -lm -lc   (in that order)
*/

#include "f2c.h"

/* Common Block Declarations */

extern struct {
    doublereal ds[500];
} cstak_;

#define cstak_1 cstak_

extern struct {
    real wv[30], rv[30];
    integer iv[40];
    logical lv[20];
} a10dey_;

#define a10dey_1 a10dey_

extern struct {
    real tgood;
} a10det_;

#define a10det_1 a10det_

extern struct {
    integer imem;
} a1odes_;

#define a1odes_1 a1odes_

extern struct {
    integer stats[9];
} a10der_;

#define a10der_1 a10der_

extern struct {
    real thetac, egivec;
    integer minitc, maxitc, keeacc, ircsc;
} a1odem_;

#define a1odem_1 a1odem_

extern struct {
    logical erptsc;
} a1odel_;

#define a1odel_1 a1odel_

extern struct {
    real aj, bj;
    logical getacc, neeumc;
} iodej_;

#define iodej_1 iodej_

extern struct {
    real tj, dtj;
    logical getjac, sepate, usegjc, usensc, usefdc;
} a1odeg_;

#define a1odeg_1 a1odeg_

extern struct {
    integer fnum;
} a1odef_;

#define a1odef_1 a1odef_

extern struct {
    logical failed;
} iodef_;

#define iodef_1 iodef_

/* Table of constant values */

static integer c__1 = 1;
static integer c__16 = 16;
static integer c__2 = 2;
static integer c__31 = 31;
static integer c__45 = 45;
static integer c__3 = 3;
static integer c__27 = 27;
static integer c__4 = 4;
static integer c__37 = 37;
static integer c__5 = 5;
static integer c__19 = 19;
static integer c__6 = 6;
static integer c__7 = 7;
static integer c__18 = 18;
static integer c__8 = 8;
static integer c__34 = 34;
static integer c__9 = 9;
static integer c__10 = 10;
static integer c__20 = 20;
static integer c__11 = 11;
static integer c__12 = 12;
static integer c__30 = 30;
static integer c__13 = 13;
static integer c__23 = 23;
static integer c__14 = 14;
static integer c__15 = 15;
static integer c__17 = 17;
static integer c__21 = 21;
static integer c__0 = 0;
static integer c_n1 = -1;
static integer c__52 = 52;
static real c_b88 = (float).9;
static integer c__1000 = 1000;
static integer c__32 = 32;
static integer c__1001 = 1001;
static integer c__1002 = 1002;
static integer c__46 = 46;
static integer c__1003 = 1003;
static integer c__36 = 36;
static integer c__1004 = 1004;
static integer c__1005 = 1005;
static integer c__26 = 26;
static integer c__1006 = 1006;
static integer c__1007 = 1007;
static integer c__42 = 42;
static integer c__1008 = 1008;

/* Subroutine */ int iode4_(v, nv, tstart, tstop, dt, d, theta, keejac, minit,
	 maxit, usengj, usenns, usenfd, beta, gamma, delta, n, mmax, hfract, 
	egive, kmax, xpoly, kinit, ircs, error, errpar, erputs, inmi, da, 
	handle)
real *v;
integer *nv;
real *tstart, *tstop, *dt;
/* Subroutine */ int (*d) ();
real *theta;
integer *keejac, *minit, *maxit;
logical *usengj, *usenns, *usenfd;
real *beta, *gamma, *delta;
integer *n, *mmax;
real *hfract, *egive;
integer *kmax;
logical *xpoly;
integer *kinit, *ircs;
/* Subroutine */ int (*error) ();
real *errpar;
logical *erputs;
/* Subroutine */ int (*inmi) ();
logical (*da) ();
/* Subroutine */ int (*handle) ();
{
    /* System generated locals */
    integer i__1;

    /* Local variables */
    static integer nerr;
    static logical temp;
    static integer i;
    extern /* Subroutine */ int a10dex_(), leave_();
    static integer ivold;
    extern /* Subroutine */ int enter_();
    extern /* Subroutine */ int a1odee_(), a1odeh_(), a1oden_(), a1odep_();
    extern /* Subroutine */ int a1odex_();
#define is ((integer *)&cstak_1)
#define ls ((logical *)&cstak_1)
#define rs ((real *)&cstak_1)
#define ws ((real *)&cstak_1)
    extern /* Subroutine */ int erroff_(), movefr_(), seterr_();
    extern integer istkgt_(), nerror_();

/* THE BOTTOM LEVEL OF IODES. */
/* SCRATCH SPACE ALLOCATED - */
/*     S( IODE3) = S(A1ODES) + */
/* REAL WORDS + */
/*                 MMAX */
/* INTEGER WORDS. */
/* THE PORT LIBRARY STACK AND ITS ALIASES. */
    /* Parameter adjustments */
    --errpar;
    --n;
    --v;

    /* Function Body */
    if (*tstart == *tstop) {
	return 0;
    }
    enter_(&c__1);
/* CHECK THE INPUT FOR ERRORS. */
/* /6S */
/*     IF (NV .LE. 0) CALL SETERR(16H IODE4 - NV.LE.0, 16, 1, 2) */
/*     IF (TSTART+DT .EQ. TSTART) CALL SETERR( */
/*    1   31H IODE4 - INPUT VALUE OF DT IS 0, 31, 2, 2) */
/*     IF ((DT/ABS(DT))*(TSTOP-TSTART) .LE. 0.) CALL SETERR( */
/*    1   45H IODE4 - INPUT VALUE OF DT HAS THE WRONG SIGN, 45, 3, 2) */
/*     IF (THETA .LT. 0. .OR. THETA .GT. 1.) CALL SETERR( */
/*    1   27H IODE4 - THETA NOT IN (0,1), 27, 4, 2) */
/*     IF (KEEJAC .LT. 0 .OR. KEEJAC .GT. 5) CALL SETERR( */
/*    1   37H IODE4 - KEEPJAC NOT ONE OF (0,...,5), 37, 5, 2) */
/*     IF (MINIT .LT. 1) CALL SETERR(19H IODE4 - MINIT.LT.1, 19, 6, 2) */
/*     IF (MAXIT .LT. 1) CALL SETERR(19H IODE4 - MAXIT.LT.1, 19, 7, 2) */
/*     IF (KMAX .LT. 1) CALL SETERR(18H IODE4 - KMAX.LT.1, 18, 8, 2) */
/* /7S */
    if (*nv <= 0) {
	seterr_(" IODE4 - NV.LE.0", &c__16, &c__1, &c__2, 16L);
    }
    if (*tstart + *dt == *tstart) {
	seterr_(" IODE4 - INPUT VALUE OF DT IS 0", &c__31, &c__2, &c__2, 31L);
    }
    if (*dt / dabs(*dt) * (*tstop - *tstart) <= (float)0.) {
	seterr_(" IODE4 - INPUT VALUE OF DT HAS THE WRONG SIGN", &c__45, &
		c__3, &c__2, 45L);
    }
    if (*theta < (float)0. || *theta > (float)1.) {
	seterr_(" IODE4 - THETA NOT IN (0,1)", &c__27, &c__4, &c__2, 27L);
    }
    if (*keejac < 0 || *keejac > 5) {
	seterr_(" IODE4 - KEEPJAC NOT ONE OF (0,...,5)", &c__37, &c__5, &c__2,
		 37L);
    }
    if (*minit < 1) {
	seterr_(" IODE4 - MINIT.LT.1", &c__19, &c__6, &c__2, 19L);
    }
    if (*maxit < 1) {
	seterr_(" IODE4 - MAXIT.LT.1", &c__19, &c__7, &c__2, 19L);
    }
    if (*kmax < 1) {
	seterr_(" IODE4 - KMAX.LT.1", &c__18, &c__8, &c__2, 18L);
    }
/* / */
    temp = *kinit < 1;
    if (! temp) {
	temp = *kinit > *kmax;
    }
/* /6S */
/*     IF (TEMP) CALL SETERR(34H IODE4 - KINIT NOT IN (1,...,KMAX), 34, 9 
*/
/*    1   , 2) */
/*     IF (BETA .LE. 0.) CALL SETERR(19H IODE4 - BETA .LE.0, 19, 10, 2) */
/*     IF (GAMMA .LE. 0.) CALL SETERR(20H IODE4 - GAMMA .LE.0, 20, 11, 2) 
*/
/*     IF (DELTA .LT. 0.) CALL SETERR(20H IODE4 - DELTA .LT.0, 20, 12, 2) 
*/
/*     IF (BETA+GAMMA-DELTA .LE. 0.) CALL SETERR( */
/*    1   30H IODE4 - BETA+GAMMA-DELTA.LE.0, 30, 13, 2) */
/*     IF (MMAX .LT. KMAX+2) CALL SETERR(23H IODE4 - MMAX.LT.KMAX+2, 23 */
/*    1   , 14, 2) */
/*     IF (N(1) .LT. 1) CALL SETERR(18H IODE4 - N(1).LT.1, 18, 15, 2) */
/* /7S */
    if (temp) {
	seterr_(" IODE4 - KINIT NOT IN (1,...,KMAX)", &c__34, &c__9, &c__2, 
		34L);
    }
    if (*beta <= (float)0.) {
	seterr_(" IODE4 - BETA .LE.0", &c__19, &c__10, &c__2, 19L);
    }
    if (*gamma <= (float)0.) {
	seterr_(" IODE4 - GAMMA .LE.0", &c__20, &c__11, &c__2, 20L);
    }
    if (*delta < (float)0.) {
	seterr_(" IODE4 - DELTA .LT.0", &c__20, &c__12, &c__2, 20L);
    }
    if (*beta + *gamma - *delta <= (float)0.) {
	seterr_(" IODE4 - BETA+GAMMA-DELTA.LE.0", &c__30, &c__13, &c__2, 30L);
    }
    if (*mmax < *kmax + 2) {
	seterr_(" IODE4 - MMAX.LT.KMAX+2", &c__23, &c__14, &c__2, 23L);
    }
    if (n[1] < 1) {
	seterr_(" IODE4 - N(1).LT.1", &c__18, &c__15, &c__2, 18L);
    }
/* / */
    i__1 = *mmax;
    for (i = 2; i <= i__1; ++i) {
/* /6S */
/*        IF (N(I) .LE. N(I-1)) CALL SETERR( */
/*    1      37H IODE4 - N IS NOT MONOTONE INCREASING, 37, 16, 2) */
/* /7S */
	if (n[i] <= n[i - 1]) {
	    seterr_(" IODE4 - N IS NOT MONOTONE INCREASING", &c__37, &c__16, &
		    c__2, 37L);
	}
/* / */
/* L1: */
    }
/* /6S */
/*     IF (HFRACT .LE. 0.) CALL SETERR(20H IODE4 - HFRACT.LE.0, 20, 17, 2 
*/
/*    1   ) */
/*     IF (EGIVE .LT. 1.) CALL SETERR(21H IODE4 - EGIVE .LT. 1, 21, 18, 2 
*/
/*    1   ) */
/* /7S */
    if (*hfract <= (float)0.) {
	seterr_(" IODE4 - HFRACT.LE.0", &c__20, &c__17, &c__2, 20L);
    }
    if (*egive < (float)1.) {
	seterr_(" IODE4 - EGIVE .LT. 1", &c__21, &c__18, &c__2, 21L);
    }
/* / */
    a1odel_1.erptsc = *erputs;
    a1odem_1.egivec = *egive;
    a1odem_1.ircsc = *ircs;
    a1odem_1.thetac = *theta;
    a1odem_1.minitc = *minit;
    a1odem_1.maxitc = *maxit;
    a1odem_1.keeacc = *keejac;
    if (*keejac == 1 && *maxit == 1) {
	a1odem_1.keeacc = 0;
    }
/* SAME AS L.B.E. */
    a1odeg_1.sepate = a1odem_1.keeacc > 1;
    a1odeg_1.usegjc = *usengj;
    a1odeg_1.usensc = *usenns;
    a1odeg_1.usefdc = *usenfd;
    if (a1odem_1.keeacc < 3) {
	goto L2;
    }
    a1odeg_1.getjac = TRUE_;
    a1odeg_1.tj = *tstart;
    goto L5;
L2:
    if (*keejac != 2) {
	goto L3;
    }
    a1odeg_1.tj = *tstop;
/* CANNOT BE TSTART. */
    goto L4;
L3:
    a1odeg_1.tj = *tstart;
/* CANNOT BE TSTART+THETA*DT/N. */
L4:
L5:
    a1odeg_1.dtj = (float)0.;
/* START WITH NO ERROR STATES. */
    a1odef_1.fnum = 0;
/* FLAG DA WORK-SPACE AS UN-ALLOCATED. */
    a1odes_1.imem = 0;
/* GET SPACE FOR DA. */
    if ((*da)(&v[1], &v[1], nv, tstart, dt, d, &v[1], &a1odeg_1.tj, &
	    a1odeg_1.dtj, &a1odeg_1.getjac, &a1odeg_1.sepate, usengj, usenns, 
	    usenfd, &c__0, &c_n1, &a1odef_1.fnum, theta, ircs, &
	    a1odem_1.keeacc)) {
    }
/* /6S */
/*     IF (IMEM .LE. 0) CALL SETERR( */
/*    1   51H IODE4 - IODED FAILED TO INITIALIZE COMMON /A1ODES/, 52, 19, 
*/
/*    2   2) */
/* /7S */
    if (a1odes_1.imem <= 0) {
	seterr_(" IODE4 - IODED FAILED TO INITIALIZE COMMON /A1ODES/", &c__52,
		 &c__19, &c__2, 51L);
    }
/* / */
    a10det_1.tgood = *tstart;
    ivold = istkgt_(nv, &c__3);
    movefr_(nv, &v[1], &ws[ivold - 1]);
/* TELL STATS ROUTINE IN IODE. */
    a1odex_(a10der_1.stats, &c__1);
    a10dex_(tstart, tstop, a1odep_, a1oden_, da, d, beta, gamma, delta, &ws[
	    ivold - 1], nv, dt, &n[1], kmax, mmax, xpoly, a1odee_, error, &
	    errpar[1], inmi, a1odeh_, handle, &c_b88, hfract, kinit);
/* TELL STATS ROUTINE OUT OF IODE. */
    a1odex_(a10der_1.stats, &c_n1);
    movefr_(nv, &ws[ivold - 1], &v[1]);
    *tstop = a10det_1.tgood;
/* CAPTURE THE ERROR NUMBER, IF ANY. */
    nerr = nerror_(&nerr);
    if (nerr != 15) {
	goto L6;
    }
    erroff_();
/* /6S */
/*        CALL SETERR(13H IODE4 - DT=0, 13, 1000, 1) */
/* /7S */
    seterr_(" IODE4 - DT=0", &c__13, &c__1000, &c__1, 13L);
/* / */
L6:
    if (nerr != 16) {
	goto L7;
    }
    erroff_();
/* /6S */
/*        CALL SETERR(32H IODE4 - DT=0 RETURNED BY HANDLE, 32, 1001, 1) */
/* /7S */
    seterr_(" IODE4 - DT=0 RETURNED BY HANDLE", &c__32, &c__1001, &c__1, 32L);
/* / */
L7:
    if (nerr != 17) {
	goto L8;
    }
    erroff_();
/* /6S */
/*        CALL SETERR(45H IODE4 - DT RETURNED BY HANDLE HAS WRONG SIGN, */
/*    1      45, 1002, 1) */
/* /7S */
    seterr_(" IODE4 - DT RETURNED BY HANDLE HAS WRONG SIGN", &c__45, &c__1002,
	     &c__1, 45L);
/* / */
L8:
    if (nerr != 18) {
	goto L9;
    }
    erroff_();
/* /6S */
/*        CALL SETERR(46H IODE4 - CANNOT RAISE DT IN HANDLE WHEN FAILED */
/*    1      , 46, 1003, 1) */
/* /7S */
    seterr_(" IODE4 - CANNOT RAISE DT IN HANDLE WHEN FAILED", &c__46, &
	    c__1003, &c__1, 46L);
/* / */
L9:
    if (nerr != 19) {
	goto L10;
    }
    erroff_();
/* /6S */
/*        CALL SETERR(36H IODE4 - E(I).LE.0 RETURNED BY ERROR, 36, 1004 */
/*    1      , 1) */
/* /7S */
    seterr_(" IODE4 - E(I).LE.0 RETURNED BY ERROR", &c__36, &c__1004, &c__1, 
	    36L);
/* / */
L10:
    if (nerr != 15) {
	goto L15;
    }
    if (a1odef_1.fnum != 1) {
	goto L11;
    }
    erroff_();
/* /6S */
/*           CALL SETERR(18H IODE4 - D FAILURE, 18, 1005, 1) */
/* /7S */
    seterr_(" IODE4 - D FAILURE", &c__18, &c__1005, &c__1, 18L);
/* / */
L11:
    if (a1odef_1.fnum != 2) {
	goto L12;
    }
    erroff_();
/* /6S */
/*           CALL SETERR(26H IODE4 - SINGULAR JACOBIAN, 26, 1006, 1) */
/* /7S */
    seterr_(" IODE4 - SINGULAR JACOBIAN", &c__26, &c__1006, &c__1, 26L);
/* / */
L12:
    if (a1odef_1.fnum != 3) {
	goto L13;
    }
    erroff_();
/* /6S */
/*           CALL SETERR( */
/*    1         45H IODE4 - TOO MANY NEWTON ITERATIONS PREDICTED, 45, */
/*    2         1007, 1) */
/* /7S */
    seterr_(" IODE4 - TOO MANY NEWTON ITERATIONS PREDICTED", &c__45, &c__1007,
	     &c__1, 45L);
/* / */
L13:
    if (a1odef_1.fnum != 4) {
	goto L14;
    }
    erroff_();
/* /6S */
/*           CALL SETERR(42H IODE4 - TOO MANY NEWTON ITERATIONS NEEDED, */
/*    1         42, 1008, 1) */
/* /7S */
    seterr_(" IODE4 - TOO MANY NEWTON ITERATIONS NEEDED", &c__42, &c__1008, &
	    c__1, 42L);
/* / */
L14:
L15:
    leave_();
    return 0;
} /* iode4_ */

#undef ws
#undef rs
#undef ls
#undef is


