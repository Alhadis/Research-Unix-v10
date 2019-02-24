/* a0des0.f -- translated by f2c (version of 5 February 1993  1:37:14).
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

/* Table of constant values */

static integer c__49 = 49;
static integer c__4 = 4;
static integer c__2 = 2;
static integer c__8 = 8;

/* Subroutine */ int a0des0_(t0, x0, t1, x1, nx, dt, tstop, ok, handle, e)
real *t0, *x0, *t1, *x1;
integer *nx;
real *dt, *tstop;
logical *ok;
/* Subroutine */ int (*handle) ();
real *e;
{
    /* Local variables */
    static real temp;
#define rs ((real *)&cstak_1)
    extern /* Subroutine */ int movefr_(), seterr_();


/*  OUTPUT FILTER FOR USE WITH GLOBAL MAXIMUM ERROR OPTION IN ODES. */





/* ... RAISE THE GLOBAL MAXIMUM, IF NECESSARY. */

    /* Parameter adjustments */
    --e;
    --x1;
    --x0;

    /* Function Body */
    if (*t0 != *t1 && odesm_1.igmax > 0) {
	movefr_(nx, &rs[odesm_1.igmax - 1], &rs[odesm_1.igmaxo - 1]);
    }

/* ... OTHERWISE, RETURN IT TO ITS PREVIOUS VALUE. */

    if (*t0 == *t1 && odesm_1.igmax > 0) {
	movefr_(nx, &rs[odesm_1.igmaxo - 1], &rs[odesm_1.igmax - 1]);
    }

    temp = *dt;

    (*handle)(t0, &x0[1], t1, &x1[1], nx, dt, tstop, &e[1]);

/* /6S */
/*     IF ((DT/ABS(DT))*(TSTOP-T1).LT.0.0E0) CALL SETERR */
/*    1   (49H ODES2 - DT RETURNED BY HANDLE HAS THE WRONG SIGN,49,4,2) */
/* /7S */
    if (*dt / dabs(*dt) * (*tstop - *t1) < (float)0.) {
	seterr_(" ODES2 - DT RETURNED BY HANDLE HAS THE WRONG SIGN", &c__49, &
		c__4, &c__2, 49L);
    }
/* / */

/* /6S */
/*     IF (.NOT.OK .AND. ABS(TEMP).LT.ABS(DT)) CALL SETERR */
/*    1   (49H ODES2 - CANNOT RAISE DT IN HANDLE WHEN .NOT.OKAY,49,8,2) */
/* /7S */
    if (! (*ok) && dabs(temp) < dabs(*dt)) {
	seterr_(" ODES2 - CANNOT RAISE DT IN HANDLE WHEN .NOT.OKAY", &c__49, &
		c__8, &c__2, 49L);
    }
/* / */

    return 0;

} /* a0des0_ */

#undef rs


