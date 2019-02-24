/* a0dese.f -- translated by f2c (version of 5 February 1993  1:37:14).
   You must link the resulting object file with the libraries:
	-lF77 -lI77 -lm -lc   (in that order)
*/

#include "f2c.h"

/* Common Block Declarations */

extern struct {
    integer igmax, igmaxo;
} odesm_;

#define odesm_1 odesm_

extern struct {
    doublereal ds[500];
} cstak_;

#define cstak_1 cstak_

logical a0dese_(x, nx, t, dt, errpar, delta, e, error)
real *x;
integer *nx;
real *t, *dt, *errpar, *delta, *e;
logical (*error) ();
{
    /* System generated locals */
    logical ret_val, L__1;

    /* Local variables */
    extern /* Subroutine */ int a0desu_();
#define rs ((real *)&cstak_1)


/*  ERROR FILTER FOR KEEPING TRACK OF THE MAXIMUM ABSOLUTE VALUE OF THE */
/*  SOLUTION, IF IT IS NECESSARY. */




/* ... RAISE THE GLOBAL MAXIMUM, IF NECESSARY. */

    /* Parameter adjustments */
    --e;
    --errpar;
    --x;

    /* Function Body */
    if (odesm_1.igmax > 0) {
	a0desu_(&x[1], nx, &rs[odesm_1.igmax - 1]);
    }

    L__1 = *delta == (float)1.;
    ret_val = (*error)(&x[1], nx, t, dt, &errpar[1], &L__1, &e[1]);

    return ret_val;

} /* a0dese_ */

#undef rs


