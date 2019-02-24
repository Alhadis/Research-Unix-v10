/* dodes2.f -- translated by f2c (version of 5 February 1993  1:37:14).
   You must link the resulting object file with the libraries:
	-lF77 -lI77 -lm -lc   (in that order)
*/

#include "f2c.h"

/* Common Block Declarations */

extern struct {
    logical okay;
} dodesf_;

#define dodesf_1 dodesf_

extern struct {
    integer igmax, igmaxo;
} dodesm_;

#define dodesm_1 dodesm_

extern struct {
    doublereal tend;
    real rerror;
} d0desq_;

#define d0desq_1 d0desq_

extern struct {
    integer iftx0;
} d0desp_;

#define d0desp_1 d0desp_

extern struct {
    doublereal ds[500];
} cstak_;

#define cstak_1 cstak_

/* Table of constant values */

static integer c__1 = 1;
static integer c__16 = 16;
static integer c__2 = 2;
static integer c__39 = 39;
static integer c__22 = 22;
static integer c__3 = 3;
static integer c__18 = 18;
static integer c__9 = 9;
static integer c__23 = 23;
static integer c__10 = 10;
static integer c__4 = 4;
static doublereal c_b28 = 1.;
static doublereal c_b29 = 2.;
static integer c__0 = 0;
static real c_b31 = (float).95;
static integer c__13 = 13;
static integer c__7 = 7;
static integer c__32 = 32;
static integer c__5 = 5;
static integer c__50 = 50;
static integer c__6 = 6;

/* Subroutine */ int dodes2_(f, x, nx, tstart, tstop, dt, error, errpar, 
	handle, glbmax, erputs, kmax, mmax)
/* Subroutine */ int (*f) ();
doublereal *x;
integer *nx;
doublereal *tstart, *tstop, *dt;
/* Subroutine */ int (*error) ();
real *errpar;
/* Subroutine */ int (*handle) ();
logical *glbmax, *erputs;
integer *kmax, *mmax;
{
    /* System generated locals */
    integer i__1;

    /* Local variables */
    static integer nerr, i, j;
    extern /* Subroutine */ int d0des0_();
    static doublereal delta;
    extern /* Subroutine */ int leave_(), enter_();
    extern /* Subroutine */ int d0dese_(), d0desg_();
    extern /* Subroutine */ int d0desl_();
    static integer in;
#define is ((integer *)&cstak_1)
#define rs ((real *)&cstak_1)
    extern /* Subroutine */ int erroff_(), seterr_();
    extern integer istkgt_(), nerror_();
    extern /* Subroutine */ int dsxtrp_();


/*  TO SOLVE THE INITAL VALUE PROBLEM FOR */

/*          DX(T)/DT = F(T,X(T)). */

/*  METHOD - RATIONAL EXTRAPOLATION OF GRAGGS MODIFIED MID-POINT RULE. */

/*  THE 2 EXTRA ARGUMENTS IN THIS SUBROUTINE PROVIDE USER CONTROL */
/*  OVER THE MAXIMUM ORDER AND MAXIMUM LEVEL OF EXTRAPOLATION TO BE USED 
*/
/*  BY THE PROCEDURE. */

/*  INPUT */

/*    F      - CALL F(T,X,NX,FTX) SHOULD RETURN FTX(I)=F(T,X)(I), */
/*             FOR I=1,...,NX. IF IT CANNOT, IT SHOULD RETURN */
/*             OKAY=.FALSE. IN COMMON /DODESF/OKAY . */
/*             F SHOULD BE DECLARED EXTERNAL IN THE SUBPROGRAM */
/*             CALLING DODES. */
/*    X      - THE INITIAL VALUES FOR THE SOLUTION. */
/*    NX     - THE LENGTH OF THE SOLUTION VECTOR X. */
/*    TSTART - THE INITIAL TIME. */
/*    TSTOP  - THE FINAL TIME. */
/*    DT     - THE INITIAL TIME-STEP TO BE USED. */
/*             THE PERFORMANCE OF DODES IS SUBSTANTIALLY */
/*             INDEPENDENT OF THE VALUE OF DT CHOSEN BY THE USER. */
/*             IT IS SUFFICIENT THAT THE USERS CHOICE FOR DT MERELY BE */
/*             WITHIN SEVERAL ORDERS OF MAGNITUDE OF BEING CORRECT. */
/*             THE VALUE OF DT WILL BE AUTOMATICALLY CHANGED BY DODES */
/*             DURING THE INTEGRATION PROCESS IN SUCH A WAY AS TO GET */
/*             THE SOLUTION, TO THE DESIRED ACCURACY, AT THE LEAST */
/*             POSSIBLE COST. */
/*    ERROR  - ERROR TOLERANCE ROUTINE. */

/*                 LOGICAL FUNCTION ERROR(X,NX,T,DT,ERRPAR,ERPUTS,E) */

/*             HAS AS INPUT */

/*               X,T    - X=X(T), THE APPROXIMATE SOLUTION FOR WHICH */
/*                        AN ERROR CRITERION IS DESIRED. */
/*               NX     - THE LENGTH OF X. */
/*               DT     - THE TIME-STEP USED TO OBTAIN X(T). */
/*               ERRPAR - TWO PARAMETERS, AS GIVEN TO DODES2. */
/*               ERPUTS - THIS VARIABLE HAS THE SAME VALUE AS ERPUTS */
/*                        IN THE CALL TO DODES2. */
/*               E      - THE REAL ABSOLUTE ERROR IN X(I) IS E(I), */
/*                        I=1,...,NX, FOR THE SINGLE CURRENT TIME-STEP. */

/*             THE OUTPUT IS */

/*               ERRPAR - MAY BE ALTERED IF DESIRED. */
/*               E      - E(I) IS THE REAL TOLERABLE ABSOLUTE ERROR IN */
/*                        X(I), I=1,...,NX. ALL E(I) MUST BE POSITIVE. */

/*             FUNCTION VALUE */

/*               ERROR  - ERROR=.TRUE. IF CONVERGED. */
/*                        ERROR=.FALSE. IF NOT. */
/*    ERRPAR - TWO PARAMETERS TO BE PASSED TO ERROR. */
/*    HANDLE - OUTPUT ROUTINE WITH A CALLING SEQUENCE OF THE FORM */

/*                     HANDLE(T0,X0,T1,X1,NX,DT,TSTOP,E) */

/*             HANDLE WILL BE CALLED AT THE END OF EACH TIME-STEP. */

/*             THE INPUT TO HANDLE IS AS FOLLOWS */

/*               X0,X1  - X0=X(T0) AND X1=X(T1). */
/*               T0,T1  - T0=T1 INDICATES A RESTART AND X1 IS FULL OF */
/*                        GARBAGE. */
/*               NX     - THE LENGTH OF THE SOLUTION VECTOR X. */
/*               DT     - THE PROPOSED TIME-STEP FOR THE NEXT STEP. */
/*               TSTOP  - THE CURRENT FINAL TIME. */
/*               E      - E(I) GIVES THE REAL ABSOLUTE ERROR IN X1(I), */
/*                        I=1,...,NX, FOR THE SINGLE CURRENT TIME-STEP. */

/*             THE OUTPUT FROM HANDLE MAY BE ANY OF */

/*               X1     - MAY BE ALTERED IF DESIRED. */
/*               DT     - THE PROPOSED TIME-STEP FOR THE NEXT STEP. */
/*               TSTOP  - THE FINAL TIME VALUE. */

/*             HANDLE SHOULD BE DECLARED EXTERNAL IN THE */
/*             SUBPROGRAM CALLING DODES. */
/*    GLBMAX - IF (GLBMAX) THEN THE GLOBAL MAXIMUM ABSOLUTE VALUE OF */
/*             THE SOLUTION IS TO BE RECORDED, */
/*             SEE COMMON /DODESM/ BELOW. */

/*             IF THE ERROR SUBPROGRAM SUPPLIED BY THE USER IS DODESE, */
/*             THEN GLBMAX DETERMINES WHETHER OR NOT THE GLOBAL */
/*             MAXIMUM ABSOLUTE VALUE OF THE SOLUTION WILL BE USED IN */
/*             THAT SUBPROGRAM. */
/*    ERPUTS - IF (ERPUTS) THEN THE ERROR PER UNIT TIME-STEP CRITERION */
/*             IS TO BE USED IN THE ERROR ROUTINE. */
/*             OTHERWISE, THE ERROR PER TIME-STEP CRITERION IS TO BE */
/*             USED IN THE ERROR ROUTINE. */

/*             IF THE ERROR SUBPROGRAM SUPPLIED BY THE USER IS DODESE, */
/*             THEN ERPUTS DETERMINES WHETHER OR NOT THE ERROR */
/*             PER UNIT-TIME-STEP OR ERROR PER TIME-STEP ERROR */
/*             CRITERION WILL BE USED BY THAT SUBPROGRAM. */
/*    KMAX   - THE MAXIMUM NUMBER OF COLUMNS ALLOWED IN THE */
/*             EXTRAPOLATION PROCESS. */
/*    MMAX   - THE MAXIMUM NUMBER OF LEVELS OF EXTRAPOLATION PERMITTED. */
/*             MMAX.GE.KMAX+2 IS REQUIRED. */

/*  OUTPUT */

/*    X      - X=X(TSTOP), THE FINAL VALUE FOR THE SOLUTION. */
/*    TSTOP  - MAY BE ALTERED BY USER SUPPLIED ROUTINE HANDLE. */
/*    DT     - PROPOSED TIME-STEP FOR THE NEXT STEP, IF ANY. */
/*    ERRPAR - MAY BE ALTERED BY USER SUPPLIED ROUTINE ERROR. */

/*  SCRATCH SPACE OF LENGTH */

/*                      S(DODES2) .LE. */

/*    2*MMAX + NX*(KMAX+2) */

/*  DOUBLE PRECISION WORDS + */

/*    5*KMAX + 3*MMAX + 3 + */

/*    ( IF (GLBMAX) THEN 2*NX , OTHERWISE 0 ) + */

/*    MAX ( 2*NX DOUBLE PRECISION + S(F) , */

/*          NX*(KMAX+1) + */

/*          MAX( KMAX DOUBLE PRECISION + KMAX , S(ERROR) ) , */

/*          NX + S(HANDLE) ) */

/*  INTEGER WORDS IS ALLOCATED. */

/*  ERROR STATES */

/*     1 - NX.LT.1. */
/*     2 - DT HAS THE WRONG SIGN ON INPUT. */
/*     3 - DT=0 ON INPUT. */
/*     4 - DT RETURNED BY HANDLE HAS THE WRONG SIGN. */
/*     5 - DT=0 WAS RETURNED BY HANDLE. (RECOVERABLE) */
/*     6 - THE ERROR DESIRED IN X(I) IS NOT POSITIVE. (RECOVERABLE) */
/*     7 - DT=0. (RECOVERABLE) */
/*     8 - CANNOT RAISE DT IN HANDLE WHEN .NOT.OKAY. */
/*     9 - KMAX.LT.1. */
/*    10 - KMAX.GT.MMAX-2. */

/*  INTERNAL NAMED COMMON USAGE - */

/*     DODESM HOLDS THE POINTER, IGMAX, TO THE REAL */
/*            VECTOR OF CURRENT MAXIMUM ABSOLUTE VALUES ATTAINED BY EACH 
*/
/*            COMPONENT OF THE SOLUTION, AND THE POINTER, IGMAXO, TO THE 
*/
/*            REAL VECTOR OF MAXIMUM ABSOLUTE VALUES ATTAINED BY EACH */
/*            COMPONENT OF THE SOLUTION AS OF THE LAST TIME STEP. */
/*            IGMAX=0 MEANS THESE VECTORS ARE NOT USED AND HAVE NOT BEEN 
*/
/*            ALLOCATED. */
/*     D0DESQ HOLDS THE DOUBLE PRECISION END-POINT VALUE AND THE REAL */
/*            REMAINING ERROR FOR USE IN DODESQ. */
/*     D0DESP HOLDS THE POINTER TO THE DOUBLE PRECISION VECTOR */
/*            F(T0,X(T0)). */





    /* Parameter adjustments */
    --errpar;
    --x;

    /* Function Body */
    if (*tstart == *tstop) {
	goto L20;
    }

    enter_(&c__1);

/* ... CHECK FOR ERRORS IN THE INPUT. */

/* /6S */
/*     IF (NX.LT.1) CALL SETERR(16HDODES2 - NX.LT.1,16,1,2) */
/*     IF ((DT/DABS(DT))*(TSTOP-TSTART).LT.0.0D0) */
/*    1   CALL SETERR(39HDODES2 - DT HAS THE WRONG SIGN ON INPUT,39,2,2) 
*/
/*     IF (TSTART+DT.EQ.TSTART) */
/*    1   CALL SETERR(22HDODES2 - DT=0 ON INPUT,22,3,2) */
/*     IF (KMAX.LT.1) CALL SETERR(18HDODES2 - KMAX.LT.1,18,9,2) */
/*     IF (KMAX.GT.MMAX-2) */
/*    1   CALL SETERR(23HDODES2 - KMAX.GT.MMAX-2,23,10,2) */
/* /7S */
    if (*nx < 1) {
	seterr_("DODES2 - NX.LT.1", &c__16, &c__1, &c__2, 16L);
    }
    if (*dt / abs(*dt) * (*tstop - *tstart) < 0.) {
	seterr_("DODES2 - DT HAS THE WRONG SIGN ON INPUT", &c__39, &c__2, &
		c__2, 39L);
    }
    if (*tstart + *dt == *tstart) {
	seterr_("DODES2 - DT=0 ON INPUT", &c__22, &c__3, &c__2, 22L);
    }
    if (*kmax < 1) {
	seterr_("DODES2 - KMAX.LT.1", &c__18, &c__9, &c__2, 18L);
    }
    if (*kmax > *mmax - 2) {
	seterr_("DODES2 - KMAX.GT.MMAX-2", &c__23, &c__10, &c__2, 23L);
    }
/* / */

/* ... ALLOCATE AND LOAD N WITH THE SEQUENCE 1,2,3,4,6,8,12,... . */

    in = istkgt_(mmax, &c__2);

    i__1 = *mmax;
    for (i = 1; i <= i__1; ++i) {
	j = in + i - 1;
	if (i < 4) {
	    is[j - 1] = i;
	}
/* L10: */
	if (i > 3) {
	    is[j - 1] = is[j - 3] << 1;
	}
    }

/* ... DECIDE IF HAVE ERROR PER UNIT-TIME-STEP OR PER TIME-STEP. */

    delta = 0.;
    if (*erputs) {
	delta = 1.;
    }

/* ... LOAD THE GLOBAL MAXIMUM ABSOLUTE VALUES OF THE SOLUTION, */
/* ... IF NECESSARY. */

    dodesm_1.igmax = 0;
    if (*glbmax) {
	i__1 = *nx << 1;
	dodesm_1.igmax = istkgt_(&i__1, &c__3);
    }
    dodesm_1.igmaxo = dodesm_1.igmax + *nx;
    if (*glbmax) {
	d0desl_(&x[1], nx, &rs[dodesm_1.igmax - 1], &rs[dodesm_1.igmaxo - 1]);
    }

    d0desq_1.tend = *tstop;
    d0desq_1.rerror = errpar[2];

    d0desp_1.iftx0 = istkgt_(nx, &c__4);

    dsxtrp_(tstart, tstop, d0desg_, f, &c_b28, &c_b29, &delta, &x[1], nx, dt, 
	    &is[in - 1], kmax, mmax, &c__0, error, d0dese_, &errpar[1], 
	    handle, d0des0_, &c_b31);

    if (nerror_(&nerr) != 0) {
	erroff_();
    }
/* /6S */
/*     IF (NERR.EQ.13) CALL SETERR */
/*    1   (13HDODES2 - DT=0,13,7,1) */
/*     IF (NERR.EQ.14) CALL SETERR */
/*    1   (32HDODES2 - DT=0 RETURNED BY HANDLE,32,5,1) */
/*     IF (NERR.EQ.17) CALL SETERR */
/*    1   (50HDODES2 - THE ERROR DESIRED IN X(I) IS NOT POSITIVE,50,6,1) 
*/
/* /7S */
    if (nerr == 13) {
	seterr_("DODES2 - DT=0", &c__13, &c__7, &c__1, 13L);
    }
    if (nerr == 14) {
	seterr_("DODES2 - DT=0 RETURNED BY HANDLE", &c__32, &c__5, &c__1, 32L)
		;
    }
    if (nerr == 17) {
	seterr_("DODES2 - THE ERROR DESIRED IN X(I) IS NOT POSITIVE", &c__50, 
		&c__6, &c__1, 50L);
    }
/* / */

    leave_();

L20:
    return 0;

} /* dodes2_ */

#undef rs
#undef is


