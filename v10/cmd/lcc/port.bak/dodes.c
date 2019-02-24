/* dodes.f -- translated by f2c (version of 5 February 1993  1:37:14).
   You must link the resulting object file with the libraries:
	-lF77 -lI77 -lm -lc   (in that order)
*/

#include "f2c.h"

/* Common Block Declarations */

extern struct {
    logical okay;
} dodesf_;

#define dodesf_1 dodesf_

/* Table of constant values */

static integer c__0 = 0;

/* Subroutine */ int dodes_(f, x, nx, tstart, tstop, dt, errpar, handle)
/* Subroutine */ int (*f) ();
doublereal *x;
integer *nx;
doublereal *tstart, *tstop, *dt;
real *errpar;
/* Subroutine */ int (*handle) ();
{
    extern /* Subroutine */ int dodes1_();
    extern /* Subroutine */ int dodese_();


/*  TO SOLVE THE INITIAL VALUE PROBLEM FOR */

/*          DX(T)/DT = F(T,X(T)). */

/*  METHOD - RATIONAL EXTRAPOLATION OF GRAGGS MODIFIED MID-POINT RULE. */

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
/*    ERRPAR - EACH COMPONENT X(I) OF THE SOLUTION IS TO BE COMPUTED */
/*             TO WITHIN AN ABSOLUTE ERROR OF */

/*                     ERRPAR(1) * ABS(X(I)) + ERRPAR(2) */

/*             FOR I=1,...,NX, AT EACH TIME-STEP. THIS ERROR REQUEST MUST 
*/
/*             ALWAYS BE POSITIVE. */
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

/*  OUTPUT */

/*    X      - X=X(TSTOP), THE FINAL VALUE FOR THE SOLUTION. */
/*    TSTOP  - MAY BE ALTERED BY USER SUPPLIED ROUTINE HANDLE. */
/*    DT     - PROPOSED TIME-STEP FOR THE NEXT STEP, IF ANY. */

/*  SCRATCH SPACE OF LENGTH */

/*                   S(DODES) .LE. */

/*    32 + 12*NX */

/*  DOUBLE PRECISION WORDS + */

/*    101 + MAX( 2*NX DOUBLE PRECISION + S(F) , */

/*               11*NX + 10 DOUBLE PRECISION + 10 , */

/*               NX + S(HANDLE) ) */

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

    dodes1_(f, &x[1], nx, tstart, tstop, dt, dodese_, &errpar[1], handle, &
	    c__0, &c__0);

L10:
    return 0;

} /* dodes_ */

