/* dodese.f -- translated by f2c (version of 5 February 1993  1:37:14).
   You must link the resulting object file with the libraries:
	-lF77 -lI77 -lm -lc   (in that order)
*/

#include "f2c.h"

/* Common Block Declarations */

extern struct {
    integer igmax, igmaxo;
} dodesm_;

#define dodesm_1 dodesm_

extern struct {
    doublereal ds[500];
} cstak_;

#define cstak_1 cstak_

logical dodese_(x, nx, t, dt, errpar, erputs, e)
doublereal *x;
integer *nx;
doublereal *t, *dt;
real *errpar;
logical *erputs;
real *e;
{
    /* System generated locals */
    integer i__1;
    real r__1;
    logical ret_val;

    /* Local variables */
    static real temp;
    static integer i, j;
    static real dtpow;
#define rs ((real *)&cstak_1)


/*  STANDARD ERROR ROUTINE FOR DODES WITH THE OPTION FOR ERROR CONTROL */
/*  BASED ON EITHER THE LOCAL VALUE OR THE GLOBAL MAXIMUM OF EACH */
/*  COMPONENT. */

/*  THE OPTION FOR ERROR CONTROL ON AN ERROR PER UNIT-TIME-STEP OR */
/*  ERROR PER TIME-STEP BASIS IS ALSO PROVIDED. */

/*  INPUT */

/*    X      - X=X(T), THE APPROXIMATE SOLUTION FOR WHICH AN ERROR */
/*             CRITERION IS DESIRED. */
/*    NX     - THE LENGTH OF THE SOLUTION VECTOR X. */
/*    T      - CURRENT VALUE OF THE TIME VARIABLE. */
/*    DT     - CURRENT TIME-STEP. */
/*    ERRPAR - TWO PARAMETERS FOR USE IN DETERMINING THE DESIRED ERROR. */
/*    ERPUTS - IF ERPUTS=.TRUE., THEN THE ERROR IS TO BE */
/*             PROPORTIONAL TO DABS(DT). OTHERWISE IT WILL NOT. */
/*    E      - X(I) IS ACCURATE TO A REAL ABSOLUTE ERROR OF E(I), */
/*             I=1,...,NX, FOR THE SINGLE CURRENT TIME-STEP. */

/*  COMMON INPUT */

/*    IGMAX  - THE POINTER TO THE REAL VECTOR OF CURRENT MAXIMUM ABSOLUTE 
*/
/*             VALUES ATTAINED BY EACH COMPONENT OF THE SOLUTION. */
/*             IGMAX=0 MEANS THIS VECTOR IS NOT USED AND HAS NOT BEEN */
/*             ALLOCATED. */

/*  OUTPUT */

/*    E      - THE REAL ERROR VECTOR. E(I) IS THE ABSOLUTE ERROR */
/*             TOLERABLE IN X(I), FOR I=1,...,NX. */

/*             LET V(I) = ABS(X(I)) IF IGMAX=0 */
/*                           OTHERWISE */
/*                      = MAXIMUM(ABS(X(I)(T))) OVER ALL PREVIOUS TIME. */
/*                        THIS VALUE IS STORED IN THE REAL STACK */
/*                        POSITION RS(I+IGMAX-1). */

/*             AND EPS = 1 IF ERPUTS=.FALSE. */
/*                           OTHERWISE */
/*                     = DABS(DT), */

/*             THEN */

/*                     E(I) = EPS * (ERRPAR(1)*V(I) + ERRPAR(2)), */

/*             FOR I=1,...,NX. */

/*  FUNCTION VALUE */

/*    DODESE - .TRUE. IF EACH X(I) IS ACCURATE TO WITHIN AN */
/*             ABSOLUTE ERROR OF E(I), I=1,...,NX, OTHERWISE .FALSE. . */

/*  SCRATCH SPACE ALLOCATED - NONE. */

/*  ERROR STATES - NONE. */





    /* Parameter adjustments */
    --e;
    --errpar;
    --x;

    /* Function Body */
    dtpow = (float)1.;
    if (*erputs) {
	dtpow = abs(*dt);
    }

    ret_val = TRUE_;
    j = dodesm_1.igmax;

    i__1 = *nx;
    for (i = 1; i <= i__1; ++i) {

	if (dodesm_1.igmax > 0) {
	    temp = rs[j - 1];
	}
	if (dodesm_1.igmax == 0) {
	    temp = (r__1 = x[i], dabs(r__1));
	}
	temp = dtpow * (errpar[1] * temp + errpar[2]);

	if (e[i] > temp) {
	    ret_val = FALSE_;
	}

	e[i] = temp;

/* L10: */
	++j;
    }

    return ret_val;

} /* dodese_ */

#undef rs


