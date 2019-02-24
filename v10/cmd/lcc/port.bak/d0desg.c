/* d0desg.f -- translated by f2c (version of 5 February 1993  1:37:14).
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
    integer iftx0;
} d0desp_;

#define d0desp_1 d0desp_

extern struct {
    logical okay;
} dodesf_;

#define dodesf_1 dodesf_

extern struct {
    doublereal ds[500];
} cstak_;

#define cstak_1 cstak_

/* Table of constant values */

static integer c__4 = 4;
static integer c__1 = 1;

/* Subroutine */ int d0desg_(t0, x0, t1, x1, nx, nt, f, ok)
doublereal *t0, *x0, *t1, *x1;
integer *nx, *nt;
/* Subroutine */ int (*f) ();
logical *ok;
{
    /* System generated locals */
    integer i__1;

    /* Local variables */
    static integer iftx;
    extern /* Subroutine */ int d0desr_();
#define ws ((doublereal *)&cstak_1)
    extern integer istkgt_();
    extern /* Subroutine */ int istkrl_();
    static integer ixl;


/*  GRAGG'S MODIFIED MID-POINT RULE FOR Y' = F(T,Y). */

/*  INPUT */

/*    T0     - INITIAL TIME. */
/*    X0     - X0=X(T0). */
/*    T1     - FINAL TIME. */
/*    NX     - THE LENGTH OF THE SOLUTION VECTOR. */
/*    NT     - THE NUMBER OF TIME STEPS TO BE USED IS 2*NT. */
/*    F      - THE RIGHT-HAND SIDE. */
/*             THE CALL F(T,X,NX,FTX) SHOULD SET FTX=F(T,X). */
/*             IF IT CANNOT COMPUTE F(T,X), OKAY=.FALSE. SHOULD BE */
/*             RETURNED IN COMMON /DODESF/OKAY . */

/*  COMMON INPUT - */

/*    IGMAX  - THE POINTER TO THE REAL VECTOR OF CURRENT MAXIMUM ABSOLUTE 
*/
/*             VALUES ATTAINED BY EACH COMPONENT OF THE SOLUTION. */
/*             IGMAX=0 MEANS THIS VECTOR IS NOT USED. */
/*    IFTX0  - THE POINTER TO THE DOUBLE PRECISION VECTOR F(T0,X0). */

/*  OUTPUT */

/*    X1     - THE APPROXIMATE SOLUTION AT TIME T1. */

/*  COMMON OUTPUT - */

/*    OK     - OK=.TRUE., IF SUCCESSFUL. OTHERWISE, OK=.FALSE. . */
/*    IGMAX  - THE UPDATED MAXIMUM VALUES. */
/*    IFTX0  - THE VECTOR FTX0 IS LOADED IF NT=1. */

/*  SCRATCH SPACE ALLOCATED - 2*NX DOUBLE PRECISION WORDS. */

/*  ERROR STATES - NONE. */




/* ... ALLOCATE SPACE FOR THE FUNCTION VALUES FTX AND SCRATCH VALUES XL. 
*/

    /* Parameter adjustments */
    --x1;
    --x0;

    /* Function Body */
    i__1 = *nx << 1;
    iftx = istkgt_(&i__1, &c__4);
    ixl = iftx + *nx;

    d0desr_(t0, &x0[1], t1, &x1[1], nx, nt, f, &ws[iftx - 1], &ws[ixl - 1], &
	    ws[d0desp_1.iftx0 - 1], &dodesm_1.igmax);

    *ok = dodesf_1.okay;

    istkrl_(&c__1);

    return 0;

} /* d0desg_ */

#undef ws


