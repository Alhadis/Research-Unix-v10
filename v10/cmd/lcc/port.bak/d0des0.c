/* d0des0.f -- translated by f2c (version of 5 February 1993  1:37:14).
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

/* Table of constant values */

static integer c__49 = 49;
static integer c__4 = 4;
static integer c__2 = 2;
static integer c__8 = 8;

/* Subroutine */ int d0des0_(t0, x0, t1, x1, nx, dt, tstop, ok, handle, e)
doublereal *t0, *x0, *t1, *x1;
integer *nx;
doublereal *dt, *tstop;
logical *ok;
/* Subroutine */ int (*handle) ();
real *e;
{
    /* Local variables */
    static doublereal temp;
#define rs ((real *)&cstak_1)
    extern /* Subroutine */ int movefr_(), seterr_();


/*  OUTPUT FILTER FOR USE WITH GLOBAL MAXIMUM ERROR OPTION IN DODES. */





/* ... RAISE THE GLOBAL MAXIMUM, IF NECESSARY. */

    /* Parameter adjustments */
    --e;
    --x1;
    --x0;

    /* Function Body */
    if (*t0 != *t1 && dodesm_1.igmax > 0) {
	movefr_(nx, &rs[dodesm_1.igmax - 1], &rs[dodesm_1.igmaxo - 1]);
    }

/* ... OTHERWISE, RETURN IT TO ITS PREVIOUS VALUE. */

    if (*t0 == *t1 && dodesm_1.igmax > 0) {
	movefr_(nx, &rs[dodesm_1.igmaxo - 1], &rs[dodesm_1.igmax - 1]);
    }

    temp = *dt;

    (*handle)(t0, &x0[1], t1, &x1[1], nx, dt, tstop, &e[1]);

/* /6S */
/*     IF ((DT/DABS(DT))*(TSTOP-T1).LT.0.0D0) CALL SETERR */
/*    1   (49HDODES2 - DT RETURNED BY HANDLE HAS THE WRONG SIGN,49,4,2) */
/* /7S */
    if (*dt / abs(*dt) * (*tstop - *t1) < 0.) {
	seterr_("DODES2 - DT RETURNED BY HANDLE HAS THE WRONG SIGN", &c__49, &
		c__4, &c__2, 49L);
    }
/* / */

/* /6S */
/*     IF (.NOT.OK .AND. DABS(TEMP).LT.DABS(DT)) CALL SETERR */
/*    1   (49HDODES2 - CANNOT RAISE DT IN HANDLE WHEN .NOT.OKAY,49,8,2) */
/* /7S */
    if (! (*ok) && abs(temp) < abs(*dt)) {
	seterr_("DODES2 - CANNOT RAISE DT IN HANDLE WHEN .NOT.OKAY", &c__49, &
		c__8, &c__2, 49L);
    }
/* / */

    return 0;

} /* d0des0_ */

#undef rs


