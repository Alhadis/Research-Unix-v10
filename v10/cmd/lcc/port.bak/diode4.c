/* diode4.f -- translated by f2c (version of 5 February 1993  1:37:14).
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
    doublereal wv[30];
    real rv[30];
    integer iv[40];
    logical lv[20];
} d10dey_;

#define d10dey_1 d10dey_

extern struct {
    doublereal tgood;
} d10det_;

#define d10det_1 d10det_

extern struct {
    integer imem;
} d1odes_;

#define d1odes_1 d1odes_

extern struct {
    integer stats[9];
} d10der_;

#define d10der_1 d10der_

extern struct {
    doublereal thetac;
    real egivec;
    integer minitc, maxitc, keeacc, ircsc;
} d1odem_;

#define d1odem_1 d1odem_

extern struct {
    logical erptsc;
} d1odel_;

#define d1odel_1 d1odel_

extern struct {
    doublereal aj, bj;
    logical getacc, neeumc;
} diodej_;

#define diodej_1 diodej_

extern struct {
    doublereal tj, dtj;
    logical getjac, sepate, usegjc, usensc, usefdc;
} d1odeg_;

#define d1odeg_1 d1odeg_

extern struct {
    integer fnum;
} d1odef_;

#define d1odef_1 d1odef_

extern struct {
    logical failed;
} diodef_;

#define diodef_1 diodef_

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

/* Subroutine */ int diode4_(v, nv, tstart, tstop, dt, d, theta, keejac, 
	minit, maxit, usengj, usenns, usenfd, beta, gamma, delta, n, mmax, 
	hfract, egive, kmax, xpoly, kinit, ircs, error, errpar, erputs, inmi, 
	da, handle)
doublereal *v;
integer *nv;
doublereal *tstart, *tstop, *dt;
/* Subroutine */ int (*d) ();
doublereal *theta;
integer *keejac, *minit, *maxit;
logical *usengj, *usenns, *usenfd;
doublereal *beta, *gamma, *delta;
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
    extern /* Subroutine */ int d10dex_(), leave_();
    static integer ivold;
    extern /* Subroutine */ int enter_();
    extern /* Subroutine */ int d1odee_(), d1odeh_(), d1oden_(), d1odep_();
    extern /* Subroutine */ int d1odex_();
#define is ((integer *)&cstak_1)
#define ls ((logical *)&cstak_1)
#define rs ((real *)&cstak_1)
#define ws ((doublereal *)&cstak_1)
    extern /* Subroutine */ int movefd_(), erroff_(), seterr_();
    extern integer istkgt_(), nerror_();

/* THE BOTTOM LEVEL OF IODES. */
/* SCRATCH SPACE ALLOCATED - */
/*     S(DIODE3) = S(D1ODES) + */
/* LONG REAL WORDS + */
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
/*     IF (NV .LE. 0) CALL SETERR(16HDIODE4 - NV.LE.0, 16, 1, 2) */
/*     IF (TSTART+DT .EQ. TSTART) CALL SETERR( */
/*    1   31HDIODE4 - INPUT VALUE OF DT IS 0, 31, 2, 2) */
/*     IF ((DT/DABS(DT))*(TSTOP-TSTART) .LE. 0D0) CALL SETERR( */
/*    1   45HDIODE4 - INPUT VALUE OF DT HAS THE WRONG SIGN, 45, 3, 2) */
/*     IF (THETA .LT. 0D0 .OR. THETA .GT. 1D0) CALL SETERR( */
/*    1   27HDIODE4 - THETA NOT IN (0,1), 27, 4, 2) */
/*     IF (KEEJAC .LT. 0 .OR. KEEJAC .GT. 5) CALL SETERR( */
/*    1   37HDIODE4 - KEEPJAC NOT ONE OF (0,...,5), 37, 5, 2) */
/*     IF (MINIT .LT. 1) CALL SETERR(19HDIODE4 - MINIT.LT.1, 19, 6, 2) */
/*     IF (MAXIT .LT. 1) CALL SETERR(19HDIODE4 - MAXIT.LT.1, 19, 7, 2) */
/*     IF (KMAX .LT. 1) CALL SETERR(18HDIODE4 - KMAX.LT.1, 18, 8, 2) */
/* /7S */
    if (*nv <= 0) {
	seterr_("DIODE4 - NV.LE.0", &c__16, &c__1, &c__2, 16L);
    }
    if (*tstart + *dt == *tstart) {
	seterr_("DIODE4 - INPUT VALUE OF DT IS 0", &c__31, &c__2, &c__2, 31L);
    }
    if (*dt / abs(*dt) * (*tstop - *tstart) <= 0.) {
	seterr_("DIODE4 - INPUT VALUE OF DT HAS THE WRONG SIGN", &c__45, &
		c__3, &c__2, 45L);
    }
    if (*theta < 0. || *theta > 1.) {
	seterr_("DIODE4 - THETA NOT IN (0,1)", &c__27, &c__4, &c__2, 27L);
    }
    if (*keejac < 0 || *keejac > 5) {
	seterr_("DIODE4 - KEEPJAC NOT ONE OF (0,...,5)", &c__37, &c__5, &c__2,
		 37L);
    }
    if (*minit < 1) {
	seterr_("DIODE4 - MINIT.LT.1", &c__19, &c__6, &c__2, 19L);
    }
    if (*maxit < 1) {
	seterr_("DIODE4 - MAXIT.LT.1", &c__19, &c__7, &c__2, 19L);
    }
    if (*kmax < 1) {
	seterr_("DIODE4 - KMAX.LT.1", &c__18, &c__8, &c__2, 18L);
    }
/* / */
    temp = *kinit < 1;
    if (! temp) {
	temp = *kinit > *kmax;
    }
/* /6S */
/*     IF (TEMP) CALL SETERR(34HDIODE4 - KINIT NOT IN (1,...,KMAX), 34, 9 
*/
/*    1   , 2) */
/*     IF (BETA .LE. 0D0) CALL SETERR(19HDIODE4 - BETA .LE.0, 19, 10, 2) 
*/
/*     IF (GAMMA .LE. 0D0) CALL SETERR(20HDIODE4 - GAMMA .LE.0, 20, 11, 2 
*/
/*    1   ) */
/*     IF (DELTA .LT. 0D0) CALL SETERR(20HDIODE4 - DELTA .LT.0, 20, 12, 2 
*/
/*    1   ) */
/*     IF (BETA+GAMMA-DELTA .LE. 0D0) CALL SETERR( */
/*    1   30HDIODE4 - BETA+GAMMA-DELTA.LE.0, 30, 13, 2) */
/*     IF (MMAX .LT. KMAX+2) CALL SETERR(23HDIODE4 - MMAX.LT.KMAX+2, 23 */
/*    1   , 14, 2) */
/*     IF (N(1) .LT. 1) CALL SETERR(18HDIODE4 - N(1).LT.1, 18, 15, 2) */
/* /7S */
    if (temp) {
	seterr_("DIODE4 - KINIT NOT IN (1,...,KMAX)", &c__34, &c__9, &c__2, 
		34L);
    }
    if (*beta <= 0.) {
	seterr_("DIODE4 - BETA .LE.0", &c__19, &c__10, &c__2, 19L);
    }
    if (*gamma <= 0.) {
	seterr_("DIODE4 - GAMMA .LE.0", &c__20, &c__11, &c__2, 20L);
    }
    if (*delta < 0.) {
	seterr_("DIODE4 - DELTA .LT.0", &c__20, &c__12, &c__2, 20L);
    }
    if (*beta + *gamma - *delta <= 0.) {
	seterr_("DIODE4 - BETA+GAMMA-DELTA.LE.0", &c__30, &c__13, &c__2, 30L);
    }
    if (*mmax < *kmax + 2) {
	seterr_("DIODE4 - MMAX.LT.KMAX+2", &c__23, &c__14, &c__2, 23L);
    }
    if (n[1] < 1) {
	seterr_("DIODE4 - N(1).LT.1", &c__18, &c__15, &c__2, 18L);
    }
/* / */
    i__1 = *mmax;
    for (i = 2; i <= i__1; ++i) {
/* /6S */
/*        IF (N(I) .LE. N(I-1)) CALL SETERR( */
/*    1      37HDIODE4 - N IS NOT MONOTONE INCREASING, 37, 16, 2) */
/* /7S */
	if (n[i] <= n[i - 1]) {
	    seterr_("DIODE4 - N IS NOT MONOTONE INCREASING", &c__37, &c__16, &
		    c__2, 37L);
	}
/* / */
/* L1: */
    }
/* /6S */
/*     IF (HFRACT .LE. 0.) CALL SETERR(20HDIODE4 - HFRACT.LE.0, 20, 17, 2 
*/
/*    1   ) */
/*     IF (EGIVE .LT. 1.) CALL SETERR(21HDIODE4 - EGIVE .LT. 1, 21, 18, 2 
*/
/*    1   ) */
/* /7S */
    if (*hfract <= (float)0.) {
	seterr_("DIODE4 - HFRACT.LE.0", &c__20, &c__17, &c__2, 20L);
    }
    if (*egive < (float)1.) {
	seterr_("DIODE4 - EGIVE .LT. 1", &c__21, &c__18, &c__2, 21L);
    }
/* / */
    d1odel_1.erptsc = *erputs;
    d1odem_1.egivec = *egive;
    d1odem_1.ircsc = *ircs;
    d1odem_1.thetac = *theta;
    d1odem_1.minitc = *minit;
    d1odem_1.maxitc = *maxit;
    d1odem_1.keeacc = *keejac;
    if (*keejac == 1 && *maxit == 1) {
	d1odem_1.keeacc = 0;
    }
/* SAME AS L.B.E. */
    d1odeg_1.sepate = d1odem_1.keeacc > 1;
    d1odeg_1.usegjc = *usengj;
    d1odeg_1.usensc = *usenns;
    d1odeg_1.usefdc = *usenfd;
    if (d1odem_1.keeacc < 3) {
	goto L2;
    }
    d1odeg_1.getjac = TRUE_;
    d1odeg_1.tj = *tstart;
    goto L5;
L2:
    if (*keejac != 2) {
	goto L3;
    }
    d1odeg_1.tj = *tstop;
/* CANNOT BE TSTART. */
    goto L4;
L3:
    d1odeg_1.tj = *tstart;
/* CANNOT BE TSTART+THETA*DT/N. */
L4:
L5:
    d1odeg_1.dtj = 0.;
/* START WITH NO ERROR STATES. */
    d1odef_1.fnum = 0;
/* FLAG DA WORK-SPACE AS UN-ALLOCATED. */
    d1odes_1.imem = 0;
/* GET SPACE FOR DA. */
    if ((*da)(&v[1], &v[1], nv, tstart, dt, d, &v[1], &d1odeg_1.tj, &
	    d1odeg_1.dtj, &d1odeg_1.getjac, &d1odeg_1.sepate, usengj, usenns, 
	    usenfd, &c__0, &c_n1, &d1odef_1.fnum, theta, ircs, &
	    d1odem_1.keeacc)) {
    }
/* /6S */
/*     IF (IMEM .LE. 0) CALL SETERR( */
/*    1   52HDIODE4 - DIODED FAILED TO INITIALIZE COMMON /D1ODES/, 52, */
/*    2   19, 2) */
/* /7S */
    if (d1odes_1.imem <= 0) {
	seterr_("DIODE4 - DIODED FAILED TO INITIALIZE COMMON /D1ODES/", &
		c__52, &c__19, &c__2, 52L);
    }
/* / */
    d10det_1.tgood = *tstart;
    ivold = istkgt_(nv, &c__4);
    movefd_(nv, &v[1], &ws[ivold - 1]);
/* TELL STATS ROUTINE IN IODE. */
    d1odex_(d10der_1.stats, &c__1);
    d10dex_(tstart, tstop, d1odep_, d1oden_, da, d, beta, gamma, delta, &ws[
	    ivold - 1], nv, dt, &n[1], kmax, mmax, xpoly, d1odee_, error, &
	    errpar[1], inmi, d1odeh_, handle, &c_b88, hfract, kinit);
/* TELL STATS ROUTINE OUT OF IODE. */
    d1odex_(d10der_1.stats, &c_n1);
    movefd_(nv, &ws[ivold - 1], &v[1]);
    *tstop = d10det_1.tgood;
/* CAPTURE THE ERROR NUMBER, IF ANY. */
    nerr = nerror_(&nerr);
    if (nerr != 15) {
	goto L6;
    }
    erroff_();
/* /6S */
/*        CALL SETERR(13HDIODE4 - DT=0, 13, 1000, 1) */
/* /7S */
    seterr_("DIODE4 - DT=0", &c__13, &c__1000, &c__1, 13L);
/* / */
L6:
    if (nerr != 16) {
	goto L7;
    }
    erroff_();
/* /6S */
/*        CALL SETERR(32HDIODE4 - DT=0 RETURNED BY HANDLE, 32, 1001, 1) */
/* /7S */
    seterr_("DIODE4 - DT=0 RETURNED BY HANDLE", &c__32, &c__1001, &c__1, 32L);
/* / */
L7:
    if (nerr != 17) {
	goto L8;
    }
    erroff_();
/* /6S */
/*        CALL SETERR(45HDIODE4 - DT RETURNED BY HANDLE HAS WRONG SIGN, */
/*    1      45, 1002, 1) */
/* /7S */
    seterr_("DIODE4 - DT RETURNED BY HANDLE HAS WRONG SIGN", &c__45, &c__1002,
	     &c__1, 45L);
/* / */
L8:
    if (nerr != 18) {
	goto L9;
    }
    erroff_();
/* /6S */
/*        CALL SETERR(46HDIODE4 - CANNOT RAISE DT IN HANDLE WHEN FAILED */
/*    1      , 46, 1003, 1) */
/* /7S */
    seterr_("DIODE4 - CANNOT RAISE DT IN HANDLE WHEN FAILED", &c__46, &
	    c__1003, &c__1, 46L);
/* / */
L9:
    if (nerr != 19) {
	goto L10;
    }
    erroff_();
/* /6S */
/*        CALL SETERR(36HDIODE4 - E(I).LE.0 RETURNED BY ERROR, 36, 1004 */
/*    1      , 1) */
/* /7S */
    seterr_("DIODE4 - E(I).LE.0 RETURNED BY ERROR", &c__36, &c__1004, &c__1, 
	    36L);
/* / */
L10:
    if (nerr != 15) {
	goto L15;
    }
    if (d1odef_1.fnum != 1) {
	goto L11;
    }
    erroff_();
/* /6S */
/*           CALL SETERR(18HDIODE4 - D FAILURE, 18, 1005, 1) */
/* /7S */
    seterr_("DIODE4 - D FAILURE", &c__18, &c__1005, &c__1, 18L);
/* / */
L11:
    if (d1odef_1.fnum != 2) {
	goto L12;
    }
    erroff_();
/* /6S */
/*           CALL SETERR(26HDIODE4 - SINGULAR JACOBIAN, 26, 1006, 1) */
/* /7S */
    seterr_("DIODE4 - SINGULAR JACOBIAN", &c__26, &c__1006, &c__1, 26L);
/* / */
L12:
    if (d1odef_1.fnum != 3) {
	goto L13;
    }
    erroff_();
/* /6S */
/*           CALL SETERR( */
/*    1         45HDIODE4 - TOO MANY NEWTON ITERATIONS PREDICTED, 45, */
/*    2         1007, 1) */
/* /7S */
    seterr_("DIODE4 - TOO MANY NEWTON ITERATIONS PREDICTED", &c__45, &c__1007,
	     &c__1, 45L);
/* / */
L13:
    if (d1odef_1.fnum != 4) {
	goto L14;
    }
    erroff_();
/* /6S */
/*           CALL SETERR(42HDIODE4 - TOO MANY NEWTON ITERATIONS NEEDED, */
/*    1         42, 1008, 1) */
/* /7S */
    seterr_("DIODE4 - TOO MANY NEWTON ITERATIONS NEEDED", &c__42, &c__1008, &
	    c__1, 42L);
/* / */
L14:
L15:
    leave_();
    return 0;
} /* diode4_ */

#undef ws
#undef rs
#undef ls
#undef is


