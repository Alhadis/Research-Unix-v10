/* odesq.f -- translated by f2c (version of 5 February 1993  1:37:14).
   You must link the resulting object file with the libraries:
	-lF77 -lI77 -lm -lc   (in that order)
*/

#include "f2c.h"

/* Common Block Declarations */

extern struct {
    real tend, rerror;
} a0desq_;

#define a0desq_1 a0desq_

logical odesq_(x, nx, t, dt, errpar, erputs, e)
real *x;
integer *nx;
real *t, *dt, *errpar;
logical *erputs;
real *e;
{
    /* System generated locals */
    integer i__1;
    real r__1, r__2, r__3;
    logical ret_val;

    /* Local variables */
    static real emax, temp;
    static integer i;


/*  STANDARD QUADRATURE ERROR ROUTINE FOR ODES. */

/*  INPUT */

/*    X      - X=X(T), THE APPROXIMATE SOLUTION FOR WHICH AN ERROR */
/*             CRITERION IS DESIRED. */
/*    NX     - THE LENGTH OF THE SOLUTION VECTOR X. */
/*    T      - CURRENT VALUE OF THE TIME VARIABLE. */
/*    DT     - CURRENT TIME-STEP. */
/*    ERRPAR - TWO PARAMETERS FOR USE IN DETERMINING THE DESIRED ERROR. */
/*             THE FINAL INTEGRAL SHOULD BE COMPUTED ACCURATE TO A REAL */
/*             ABSOLUTE ERROR OF ERRPAR(2). */
/*    ERPUTS - THIS PARAMETER IS IGNORED. */
/*    E      - X(I) IS ACCURATE TO A REAL ABSOLUTE ERROR OF E(I), FOR THE 
*/
/*             SINGLE CURRENT TIME-STEP, I=1,...,NX. */

/*  COMMON INPUT - */

/*    TEND   - THE END OF THE INTEGRATION INTERVAL. */
/*    RERROR - THE REMAINDER OF THE INTEGRAL SHOULD BE DONE TO WITHIN */
/*             A REAL ABSOLUTE ERROR OF RERROR. */
/*             THE FIRST CALL SHOULD HAVE RERROR=ERRPAR(2). */

/*  OUTPUT */

/*    E      - E(I)=MAX(RERROR*ABS(DT/(TEND-(T-DT))), */
/*                      1.0E-3*ERRPAR(2)) ,   FOR I=1,...,NX. */
/*             THUS, THE FINAL INTEGRAL SHOULD BE ACCURATE TO WITHIN */
/*             A REAL ABSOLUTE ERROR OF ERRPAR(2). */

/*  FUNCTION VALUE - */

/*    ODESQ - .TRUE. IF EACH X(I) IS ACCURATE TO WITHIN AN */
/*             ABSOLUTE ERROR OF E(I), I=1,...,NX, OTHERWISE .FALSE. . */

/*  COMMON OUTPUT - */

/*    RERROR - IF (ODESQ) THEN */
/*             RERROR=RERROR - MAXIMUM(E(1),...,E(NX)), WHERE THE E */
/*             USED IS THE INPUT VALUE FOR THAT VARIABLE. */
/*             OTHERWISE, RERROR REMAINS UNCHANGED. */

/*  SCRATCH SPACE ALLOCATED - NONE. */

/*  ERROR STATES - NONE. */




    /* Parameter adjustments */
    --e;
    --errpar;
    --x;

    /* Function Body */
    ret_val = TRUE_;
    emax = (float)0.;

    i__1 = *nx;
    for (i = 1; i <= i__1; ++i) {

/* Computing MAX */
	r__2 = a0desq_1.rerror * (r__1 = *dt / (a0desq_1.tend - (*t - *dt)), 
		dabs(r__1)), r__3 = errpar[2] * (float).001;
	temp = dmax(r__2,r__3);

	if (e[i] > temp) {
	    ret_val = FALSE_;
	}

/* Computing MAX */
	r__1 = emax, r__2 = e[i];
	emax = dmax(r__1,r__2);

/* L10: */
	e[i] = temp;
    }

    if (ret_val) {
	a0desq_1.rerror -= emax;
    }

    return ret_val;

} /* odesq_ */

