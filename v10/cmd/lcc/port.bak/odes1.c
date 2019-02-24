/* odes1.f -- translated by f2c (version of 5 February 1993  1:37:14).
   You must link the resulting object file with the libraries:
	-lF77 -lI77 -lm -lc   (in that order)
*/

#include "f2c.h"

/* Common Block Declarations */

extern struct {
    logical okay;
} odesf_;

#define odesf_1 odesf_

/* Table of constant values */

static integer c__10 = 10;
static integer c__16 = 16;

/* Subroutine */ int odes1_(f, x, nx, tstart, tstop, dt, error, errpar, 
	handle, glbmax, erputs)
/* Subroutine */ int (*f) ();
real *x;
integer *nx;
real *tstart, *tstop, *dt;
/* Subroutine */ int (*error) ();
real *errpar;
/* Subroutine */ int (*handle) ();
logical *glbmax, *erputs;
{
    extern /* Subroutine */ int odes2_();


/*  TO SOLVE THE INITIAL VALUE PROBLEM FOR */

/*          DX(T)/DT = F(T,X(T)). */

/*  METHOD - RATIONAL EXTRAPOLATION OF GRAGGS MODIFIED MID-POINT RULE. */

/*  THE 3 EXTRA ARGUMENTS IN THIS SUBROUTINE PROVIDE MORE USER */
/*  CONTROL OVER THE ACCURACY OF THE INTEGRATION PROCESS. */

/*  INPUT */

/*    F      - CALL F(T,X,NX,FTX) SHOULD RETURN FTX(I)=F(T,X)(I), */
/*             FOR I=1,...,NX. IF IT CANNOT, IT SHOULD RETURN */
/*             OKAY=.FALSE. IN COMMON /ODESF/OKAY . */
/*             F SHOULD BE DECLARED EXTERNAL IN THE SUBPROGRAM */
/*             CALLING ODES. */
/*    X      - THE INITIAL VALUES FOR THE SOLUTION. */
/*    NX     - THE LENGTH OF THE SOLUTION VECTOR X. */
/*    TSTART - THE INITIAL TIME. */
/*    TSTOP  - THE FINAL TIME. */
/*    DT     - THE INITIAL TIME-STEP TO BE USED. */
/*             THE PERFORMANCE OF ODES IS SUBSTANTIALLY */
/*             INDEPENDENT OF THE VALUE OF DT CHOSEN BY THE USER. */
/*             IT IS SUFFICIENT THAT THE USERS CHOICE FOR DT MERELY BE */
/*             WITHIN SEVERAL ORDERS OF MAGNITUDE OF BEING CORRECT. */
/*             THE VALUE OF DT WILL BE AUTOMATICALLY CHANGED BY ODES */
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
/*               ERRPAR - TWO PARAMETERS, AS GIVEN TO ODES1. */
/*               ERPUTS - THIS VARIABLE HAS THE SAME VALUE AS ERPUTS IN */
/*                        THE CALL TO ODES1. */
/*               E      - THE REAL ABSOLUTE ERROR IN X(I) IS E(I), */
/*                        I=1,...,NX, FOR THE SINGLE CURRENT TIME-STEP. */

/*             THE OUTPUT IS */

/*               ERRPAR - MAY BE ALTERED IF DESIRED. */
/*               E      - E(I) IS THE REAL TOLERABLE ABSOLUTE ERROR IN */
/*                        X(I), FOR I=1,...,NX. ALL THE E(I) MUST BE */
/*                        POSITIVE. */

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
/*             SUBPROGRAM CALLING ODES. */
/*    GLBMAX - IF (GLBMAX) THEN THE GLOBAL MAXIMUM OF THE ABSOLUTE VALUE 
*/
/*             OF THE SOLUTION IS TO BE RECORDED. */
/*             THE GLOBAL MAXIMUM INFORMATION IS STORED IN COMMON, */
/*             AS DESCRIBED IN ODES2. */

/*             IF THE ERROR SUBPROGRAM SUPPLIED BY THE USER IS ODESE, */
/*             THEN GLBMAX DETERMINES WHETHER OR NOT THE GLOBAL */
/*             MAXIMUM ABSOLUTE VALUE OF THE SOLUTION WILL BE USED IN */
/*             THAT SUBPROGRAM. */
/*    ERPUTS - IF (ERPUTS) THEN THE ERROR PER UNIT TIME-STEP CRITERION */
/*             IS TO BE USED IN THE ERROR ROUTINE. */
/*             OTHERWISE, THE ERROR PER TIME-STEP CRITERION IS TO BE */
/*             USED IN THE ERROR ROUTINE. */

/*             IF THE ERROR SUBPROGRAM SUPPLIED BY THE USER IS ODESE, */
/*             THEN ERPUTS DETERMINES WHETHER OR NOT THE ERROR */
/*             PER UNIT-TIME-STEP OR ERROR PER TIME-STEP ERROR */
/*             CRITERION WILL BE USED BY THAT SUBPROGRAM. */

/*  OUTPUT */

/*    X      - X=X(TSTOP), THE FINAL VALUE FOR THE SOLUTION. */
/*    TSTOP  - MAY BE ALTERED BY USER SUPPLIED ROUTINE HANDLE. */
/*    DT     - PROPOSED TIME-STEP FOR THE NEXT STEP, IF ANY. */
/*    ERRPAR - MAY BE ALTERED BY USER SUPPLIED ROUTINE ERROR. */

/*  SCRATCH SPACE OF LENGTH */

/*                    S(ODES1) .LE. */

/*    32 + 12*NX */

/*  REAL WORDS + */

/*    101 + ( IF (GLBMAX) THEN 2*NX , OTHERWISE 0 ) + */

/*    MAX( 2*NX REAL + S(F) , */

/*         11*NX + MAX ( 10 REAL + 10 , S(ERROR) ) , */

/*         NX + S(HANDLE) ) */

/*  INTEGER WORDS IS ALLOCATED. */

/*  ERROR STATES */

/*    1 - NX.LT.1. */
/*    2 - DT HAS THE WRONG SIGN ON INPUT. */
/*    3 - DT=0 ON INPUT. */
/*    4 - DT RETURNED BY HANDLE HAS THE WRONG SIGN. */
/*    5 - DT=0 WAS RETURNED BY HANDLE. (RECOVERABLE) */
/*    6 - THE ERROR DESIRED IN X(I) IS NOT POSITIVE. (RECOVERABLE) */
/*    7 - DT=0. (RECOVERABLE) */
/*    8 - CANNOT RAISE DT IN HANDLE WHEN .NOT.OKAY. */



    /* Parameter adjustments */
    --errpar;
    --x;

    /* Function Body */
    if (*tstart == *tstop) {
	goto L10;
    }

    odes2_(f, &x[1], nx, tstart, tstop, dt, error, &errpar[1], handle, glbmax,
	     erputs, &c__10, &c__16);

L10:
    return 0;

} /* odes1_ */

