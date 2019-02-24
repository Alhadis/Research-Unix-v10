/* dsxtrp.f -- translated by f2c (version of 5 February 1993  1:37:14).
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
static integer c__18 = 18;
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
static integer c__30 = 30;
static integer c__10 = 10;
static integer c__37 = 37;
static integer c__11 = 11;

/* Subroutine */ int dsxtrp_(tstart, tstop, xa, f, beta, gamma, delta, x, nx, 
	dt, n, kmax, mmax, xpoly, error, serror, errpar, output, sout, pespar)
doublereal *tstart, *tstop;
/* Subroutine */ int (*xa) (), (*f) ();
doublereal *beta, *gamma, *delta, *x;
integer *nx;
doublereal *dt;
integer *n, *kmax, *mmax;
logical *xpoly;
logical (*error) (), (*serror) ();
real *errpar;
/* Subroutine */ int (*output) (), (*sout) ();
real *pespar;
{
    /* System generated locals */
    integer i__1;
    doublereal d__1;

    /* Builtin functions */
    double log(), pow_dd();

    /* Local variables */
    static integer ipow, i, j;
    extern /* Subroutine */ int leave_();
    static integer ilogn;
    extern /* Subroutine */ int enter_();
    static integer iwork, if__;
    extern /* Subroutine */ int d8xtrp_();
    static integer iw;
#define rs ((real *)&cstak_1)
#define ws ((doublereal *)&cstak_1)
    static integer ihopto;
    extern /* Subroutine */ int seterr_();
    extern integer istkgt_();
    static integer ilwork, ix1, ing, ilw;


/*  LET A VECTOR VALUED FUNCTION A(H) OF LENGTH NX PRODUCE AN */
/*  APPROXIMATION TO X(T1) WHEN GIVEN T0, X(T0) AND H=(T1-T0)/N WHERE */
/*  N IS AN INTEGER AND X(T) IS SOME UNKNOWN VECTOR-VALUED FUNCTION */
/*  OF TIME. */

/*  ASSUME THAT */

/*  A(H) = X(T1) + */

/*         ABS(T1-T0)**BETA * SUM(J=1,...,INFINITY)(C(J)*H**(J*GAMMA)) */

/*  WHERE THE C(J) ARE UNKNOWN VECTORS INDEPENDENT OF H. */

/*  THIS ROUTINE THEN TAKES THE VALUE X=X(TSTART) AND, USING AN INITIAL */
/*  VALUE OF T1=TSTART+DT, SEQUENTIALLY EVALUATES X(T1) UNTIL T1=TSTOP. */

/*  THE EVALUATION OF X(T1) IS ACCOMPLISHED USING EXTRAPOLATION TO */
/*  THE LIMIT OF THE RESULTS OF A(H) FOR H=DT/N(M), M=1,...,MMAX. */

/*  INPUT */

/*    TSTART - THE INITIAL VALUE FOR TIME. */
/*    TSTOP  - THE FINAL VALUE FOR TIME. */
/*    XA     - CALL XA(T0,X0,T1,X1,NX,N,F,OK) SHOULD RETURN THE */
/*             APPROXIMATION X1=A(H) TO X(T1) GIVEN T0, X0=X(T0) AND N. */
/*             OK=.TRUE. SHOULD BE RETURNED IF X1 HAS BEEN SUCCESSFULLY */
/*             COMPUTED. OTHERWISE, OK=.FALSE. SHOULD BE RETURNED. */
/*             THIS WILL CAUSE A RESTART OF THE PROCESS FROM TIME T=T0, */
/*             WITH A DEFAULT LOWERING OF DT BY 10**3. */
/*             F IS A SUBPROGRAM NAME, AS PASSED TO DSXTRP. */
/*    F      - A SUBPROGRAM NAME WHICH IS PASSED TO XA. */
/*    BETA   - THE POWER SERIES FOR THE ERROR IN A(H) HAS A */
/*             MULTIPLICATIVE FACTOR OF ABS(T1-T0)**BETA IN FRONT OF IT. 
*/
/*    GAMMA  - THE POWER SERIES FOR THE ERROR IN A(H) IS IN THE */
/*             VARIABLE H**GAMMA. */
/*    DELTA  - THE ERROR CRITERION IS PROPORTIONAL TO */
/*             ABS(T1-T0)**DELTA. */
/*    X      - THE INITIAL VALUES X=X(TSTART). */
/*    NX     - THE LENGTH OF THE SOLUTION VECTOR X. */
/*    DT     - THE INITIAL TIME-STEP TO BE USED. */
/*             THE PERFORMANCE OF DSXTRP IS SUBSTANTIALLY */
/*             INDEPENDENT OF THE VALUE OF DT CHOSEN BY THE USER. */
/*             IT IS SUFFICIENT THAT THE USERS CHOICE FOR DT MERELY BE */
/*             WITHIN SEVERAL ORDERS OF MAGNITUDE OF BEING CORRECT. */
/*             THE VALUE OF DT WILL BE AUTOMATICALLY CHANGED BY DSXTRP */
/*             DURING THE INTEGRATION PROCESS IN SUCH A WAY AS TO GET */
/*             THE SOLUTION, TO THE DESIRED ACCURACY, AT THE LEAST */
/*             POSSIBLE COST. */
/*    N      - H=(T1-T0)/N(M) WILL BE USED AT THE M-TH LEVEL OF */
/*             EXTRAPOLATION, M=1,...,MMAX. */
/*    KMAX   - THE MAXIMAL NUMBER OF COLUMNS KEPT IN THE EXTRAPOLATION */
/*             PROCESS. */
/*    MMAX   - THE MAXIMUM LEVEL OF EXTRAPOLATION TO BE USED. */
/*             MMAX.GE.KMAX+2 IS REQUIRED. */
/*    XPOLY  - IF (XPOLY) THEN USE POLYNOMIAL EXTRAPOLATION. */
/*             IF (.NOT.XPOLY) THEN USE RATIONAL EXTRAPOLATION. */
/*    ERROR  - A SUBPROGRAM NAME WHICH IS PASSED TO SERROR. */
/*    SERROR - A LOGICAL FUNCTION OF THE FORM */

/*              LOGICAL FUNCTION SERROR(X1,NX,T1,DT,ERRPAR,DELTA,E,ERROR) 
*/

/*             THE INPUT TO SERROR IS */

/*               X1     - X1=X(T1), THE APPROXIMATE SOLUTION FOR WHICH */
/*                        AN ERROR CRITERION IS DESIRED. */
/*               NX     - THE LENGTH OF THE SOLUTION VECTOR. */
/*               T1     - THE CURRENT VALUE OF TIME, X1=X(T1). */
/*               DT     - DT=T1-T0. */
/*               ERRPAR - TWO PARAMETERS, AS PASSED TO DSXTRP. */
/*               DELTA  - AS PASSED TO DSXTRP. */
/*               E      - E(I) IS THE REAL ABSOLUTE ERROR IN X1(I), */
/*                        I=1,...,NX, FOR THE SINGLE CURRENT TIME-STEP. */
/*               ERROR  - THE NAME OF A SUBPROGRAM, AS PASSED TO DSXTRP. 
*/

/*             THE OUTPUT FROM SERROR IS */

/*               E      - E(I) GIVES THE DESIRED REAL ABSOLUTE ERROR */
/*                        IN THE I-TH COMPONENT OF X1=X(T1), I=1,...,NX. 
*/
/*               ERRPAR - MAY BE ALTERED IF DESIRED. */

/*             FUNCTION VALUE - */

/*               SERROR - SERROR.TRUE. IF CONVERGED. */
/*                        SERROR=.FALSE. IF NOT. */

/*    ERRPAR - A VECTOR OF LENGTH TWO TO BE PASSED TO ERROR. */
/*    OUTPUT - A SUBPROGRAM NAME TO BE PASSED TO SOUT. */
/*    SOUT   - THE OUTPUT SUBROUTINE */

/*                 SOUT(T0,X0,T1,X1,NX,DT,TSTOP,OK,OUTPUT,E) */

/*             WILL BE CALLED AT THE END OF EACH TIME STEP. */

/*             THE INPUT TO SOUT IS */

/*               T0     - THE OLD VALUE OF T1. */
/*               X0     - X0=X(T0) */
/*               T1     - CURRENT VALUE OF TIME. */
/*               X1     - X1=X(T1). */
/*               NX     - THE LENGTH OF THE SOLUTION VECTOR. */
/*               DT     - THE PROPOSED TIME-STEP FOR THE NEXT STEP. */
/*               TSTOP  - THE CURRENT VALUE OF THE FINAL-TIME. */
/*               OK     - AS RETURNED BY XA. */
/*               OUTPUT - A SUBPROGRAM NAME, AS PASSED TO DSXTRP. */
/*               E      - THE REAL ABSOLUTE ERROR IN X1(I)=X(T1)(I) */
/*                        IS E(I), I=1,...,NX, FOR THE SINGLE TIME-STEP */
/*                        FROM T0 TO T1. */

/*             THE OUTPUT FROM SOUT MAY BE ANY OF */

/*               X1     - X1=X(T1). */
/*               DT     - THE PROPOSED TIME-STEP FOR THE NEXT STEP. */
/*               TSTOP  - THE FINAL-TIME VALUE. */

/*    PESPAR - THE OPTIMAL TIME-STEP DT IS MULTIPLIED BY PESPAR */
/*             BEFORE BEING USED FOR THE NEXT STEP. */
/*             0.LT.PESPAR.LE.1 IS REQUIRED. */

/*  OUTPUT */

/*    X      - X=X(TSTOP), THE FINAL VALUE FOR THE SOLUTION. */
/*    DT     - THE PROPOSED TIME-STEP FOR THE NEXT STEP, IF ANY. */
/*    TSTOP  - MAY BE ALTERED BY USER SUPPLIED ROUTINE SOUT. */
/*    ERRPAR - MAY BE ALTERED BY USER SUPPLIED ROUTINE ERROR. */

/*  SCRATCH SPACE OF LENGTH */

/*                   S(DSXTRP) .LE. */

/*    2*MMAX + NX*(KMAX+1) */

/*  DOUBLE PRECISION WORDS + */

/*    5*KMAX + 2*MMAX + 3 + */

/*    MAX( S(XA), NX*(KMAX+1) + */

/*         MAX( KMAX DOUBLE PRECISION + KMAX , S(ERROR) ) , */

/*         NX + S(SOUT) ) */

/*  INTEGER WORDS IS ALLOCATED. */

/*  ERROR STATES */

/*     1 - BETA.LT.0. */
/*     2 - GAMMA.LE.0. */
/*     3 - DELTA.LT.0. */
/*     4 - NX.LT.1. */
/*     5 - DT=0 ON INPUT. */
/*     6 - N(1).LT.1. */
/*     7 - KMAX.LT.1. */
/*     8 - MMAX.LT.KMAX+2. */
/*     9 - PESPAR NOT IN (0,1). */
/*    10 - BETA-DELTA+GAMMA.LE.0. */
/*    11 - N IS NOT MONOTONE INCREASING. */
/*    12 - DT HAS THE WRONG SIGN. */
/*    13 - DT=0. (RECOVERABLE) */
/*    14 - DT=0 RETURNED BY SOUT. (RECOVERABLE) */
/*    15 - DT RETURNED BY SOUT HAS THE WRONG SIGN. */
/*    16 - DT RAISED BY SOUT WHEN OK=.FALSE.. */
/*    17 - E(I).LE.0 RETURNED BY ERROR. (RECOVERABLE) */
/*    18 - SOMEBODY IS LEAVING STUFF ON THE STACK. */

/*  WHILE DSXTRP IS EXECUTING, COMMON /D9XTRP/ CONTAINS THE FOLLOWING */
/*  INFORMATION - */

/*    MC     - THE CURRENT LEVEL OF EXTRAPOLATION. */
/*    KOPTC  - THE OPTIMAL NUMBER OF COLUMNS IN THE LOZENGE. */
/*             IF KOPTC IS ZERO, THEN THE NEXT THREE ITEMS ARE */
/*             MEANINGLESS. */
/*    ICOST  - THE POINTER TO THE REAL COST/UNIT TIME-STEP ARRAY. */
/*    KHIC   - THE ACTIVE LENGTH OF THE COST ARRAY. */
/*    IHOPT  - THE POINTER TO THE REAL ARRAY OF OPTIMAL STEP-SIZES */
/*             FOR A GIVEN NUMBER OF COLUMNS, ITS LENGTH IS KHIC. */
/*    IRCNT  - IRCNT LOGARITHMIC BISECTION STEPS ARE TO BE DONE. */
/*    HUP    - DT CANNOT GROW BY MORE THAN EXP(HUP) PER STEP. */
/*             HUP WILL BE MULTIPLIED BY 2 AFTER EACH SUCCESSFUL */
/*             TIME STEP. THIS VALUE IS TYPE REAL. */
/*    ILOZNG - THE POINTER TO THE LOWER EDGE OF THE DOUBLE PRECISION */
/*             EXTRAPOLATION LOZENGE. */
/*    KMAXC  - THE LENGTH OF THE BOTTOM EDGE OF THE LOZENGE IS */
/*             MIN(KMAXC,MC). */






    /* Parameter adjustments */
    --errpar;
    --n;
    --x;

    /* Function Body */
    if (*tstart == *tstop) {
	goto L50;
    }

    enter_(&c__1);

/* ... CHECK THE INPUT. */

/* /6S */
/*     IF (BETA.LT.0.0D0) CALL SETERR(18HDSXTRP - BETA.LT.0,18,1,2) */
/*     IF (GAMMA.LE.0.0D0) CALL SETERR(19HDSXTRP - GAMMA.LE.0,19,2,2) */
/*     IF (DELTA.LT.0.0D0) CALL SETERR(19HDSXTRP - DELTA.LT.0,19,3,2) */
/*     IF (NX.LT.1) CALL SETERR(16HDSXTRP - NX.LT.1,16,4,2) */
/*     IF (DT.EQ.0.0D0) CALL SETERR(22HDSXTRP - DT=0 ON INPUT,22,5,2) */
/*     IF (N(1).LT.1) CALL SETERR(18HDSXTRP - N(1).LT.1,18,6,2) */
/*     IF (KMAX.LT.1) CALL SETERR(18HDSXTRP - KMAX.LT.1,18,7,2) */
/*     IF (MMAX.LT.KMAX+2) CALL SETERR(23HDSXTRP - MMAX.LT.KMAX+2,23,8,2) 
*/
/*     IF (PESPAR.LE.0.0E0.OR.PESPAR.GT.1.0E0) CALL SETERR */
/*    1   (28HDSXTRP - PESPAR NOT IN (0,1),28,9,2) */
/*     IF (BETA-DELTA+GAMMA.LE.0.0D0) CALL SETERR */
/*    1   (30HDSXTRP - BETA-DELTA+GAMMA.LE.0,30,10,2) */
/* /7S */
    if (*beta < 0.) {
	seterr_("DSXTRP - BETA.LT.0", &c__18, &c__1, &c__2, 18L);
    }
    if (*gamma <= 0.) {
	seterr_("DSXTRP - GAMMA.LE.0", &c__19, &c__2, &c__2, 19L);
    }
    if (*delta < 0.) {
	seterr_("DSXTRP - DELTA.LT.0", &c__19, &c__3, &c__2, 19L);
    }
    if (*nx < 1) {
	seterr_("DSXTRP - NX.LT.1", &c__16, &c__4, &c__2, 16L);
    }
    if (*dt == 0.) {
	seterr_("DSXTRP - DT=0 ON INPUT", &c__22, &c__5, &c__2, 22L);
    }
    if (n[1] < 1) {
	seterr_("DSXTRP - N(1).LT.1", &c__18, &c__6, &c__2, 18L);
    }
    if (*kmax < 1) {
	seterr_("DSXTRP - KMAX.LT.1", &c__18, &c__7, &c__2, 18L);
    }
    if (*mmax < *kmax + 2) {
	seterr_("DSXTRP - MMAX.LT.KMAX+2", &c__23, &c__8, &c__2, 23L);
    }
    if (*pespar <= (float)0. || *pespar > (float)1.) {
	seterr_("DSXTRP - PESPAR NOT IN (0,1)", &c__28, &c__9, &c__2, 28L);
    }
    if (*beta - *delta + *gamma <= 0.) {
	seterr_("DSXTRP - BETA-DELTA+GAMMA.LE.0", &c__30, &c__10, &c__2, 30L);
    }
/* / */

/* ... ALLOCATE AND LOAD THE ARRAY LOGN WITH LOG(N(I)). */

    ilogn = istkgt_(mmax, &c__4);
    ws[ilogn - 1] = log((doublereal) ((real) n[1]));
    i = ilogn + 1;
    i__1 = *mmax;
    for (j = 2; j <= i__1; ++j) {
/* /6S */
/*        IF (N(J-1).GE.N(J)) CALL SETERR */
/*    1      (37HDSXTRP - N IS NOT MONOTONE INCREASING,37,11,2) */
/* /7S */
	if (n[j - 1] >= n[j]) {
	    seterr_("DSXTRP - N IS NOT MONOTONE INCREASING", &c__37, &c__11, &
		    c__2, 37L);
	}
/* / */
	ws[i - 1] = log((doublereal) ((real) n[j]));
/* L10: */
	++i;
    }

/* ... ALLOCATE CURRENT AND OLD OPTIMAL STEP-SIZE ARRAYS. */

    i__1 = *kmax + 1;
    d9xtrp_1.ihopt = istkgt_(&i__1, &c__3);
    i__1 = *kmax + 1;
    ihopto = istkgt_(&i__1, &c__3);

/* ... ALLOCATE AND LOAD THE ARRAY NG WITH N(J)**GAMMA. */

    ing = istkgt_(mmax, &c__4);
    i = ing;
    i__1 = *mmax;
    for (j = 1; j <= i__1; ++j) {
	d__1 = (doublereal) ((real) n[j]);
	ws[i - 1] = pow_dd(&d__1, gamma);
/* L20: */
	++i;
    }

/* ... ALLOCATE SPACE FOR X1 (THE SOLUTION AT TIME T1), */
/* ... AND A SCRATCH ARRAY F. */

    ix1 = istkgt_(nx, &c__4);
    d9xtrp_1.kmaxc = *kmax;
    if__ = istkgt_(kmax, &c__3);

/* ... ALLOCATE AND LOAD POW(J) WITH 1/(BETA-DELTA+J*GAMMA). */

    ipow = istkgt_(kmax, &c__3);
    i = ipow;
    i__1 = *kmax;
    for (j = 1; j <= i__1; ++j) {
	rs[i - 1] = (float)1. / (*beta - *delta + (real) j * *gamma);
/* L30: */
	++i;
    }

/* ... ALLOCATE AND LOAD ARRAYS WORK AND LWORK WITH */
/* ... SUM(I=1,...,J)(N(I)) AND LOG(WORK(J)), RESPECTIVELY. */

    iwork = istkgt_(mmax, &c__3);
    ilwork = istkgt_(mmax, &c__3);
    iw = iwork;
    ilw = ilwork;
    rs[iw - 1] = (real) n[1];
    rs[ilw - 1] = log(rs[iw - 1]);
    i__1 = *mmax;
    for (j = 2; j <= i__1; ++j) {
	++iw;
	++ilw;
	rs[iw - 1] = rs[iw - 2] + (real) n[j];
/* L40: */
	rs[ilw - 1] = log(rs[iw - 1]);
    }

/* ... ALLOCATE THE COST/UNIT TIME-STEP ARRAY. */

    i__1 = *kmax + 1;
    d9xtrp_1.icost = istkgt_(&i__1, &c__3);

/* ... ALLOCATE THE EXTRAPOLATION LOZENGE SO THAT ISTKMD CAN */
/* ... BE USED TO LET IT GROW ONLY AS NEEDED. */

    d9xtrp_1.ilozng = istkgt_(&c__1, &c__4);

    d8xtrp_(tstart, tstop, xa, f, beta, gamma, delta, nx, dt, &n[1], kmax, 
	    mmax, xpoly, error, serror, &errpar[1], output, sout, pespar, &ws[
	    ilogn - 1], &rs[d9xtrp_1.ihopt - 1], &rs[ihopto - 1], &ws[ing - 1]
	    , &x[1], &ws[ix1 - 1], &ws[d9xtrp_1.ilozng - 1], &rs[if__ - 1], &
	    rs[ipow - 1], &rs[iwork - 1], &rs[ilwork - 1], &rs[d9xtrp_1.icost 
	    - 1]);

    leave_();

L50:
    return 0;

} /* dsxtrp_ */

#undef ws
#undef rs


