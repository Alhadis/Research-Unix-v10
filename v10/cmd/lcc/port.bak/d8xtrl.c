/* d8xtrl.f -- translated by f2c (version of 5 February 1993  1:37:14).
   You must link the resulting object file with the libraries:
	-lF77 -lI77 -lm -lc   (in that order)
*/

#include "f2c.h"

/* Table of constant values */

static integer c__36 = 36;
static integer c__17 = 17;
static integer c__1 = 1;

logical d8xtrl_(e2, e, nx, m, kmax, pow, loglo)
real *e2, *e;
integer *nx, *m, *kmax;
real *pow, *loglo;
{
    /* System generated locals */
    integer e2_dim1, e2_offset, i__1, i__2;
    real r__1;
    logical ret_val;

    /* Builtin functions */
    double log();

    /* Local variables */
    static real loge;
    static integer i, j;
    static real v;
    extern /* Subroutine */ int seterr_();
    static integer jhi;


/*  TO RETURN POW(J) TIMES THE LOGARITHM OF THE RATIO OF THE DESIRED */
/*  TO THE ATTAINED ERROR, FOR EACH ELEMENT IN THE LOZENGE. */

/*  D8XTRL = .TRUE. IF NOT SUCCESSFUL. */
/*  D8XTRL = .FALSE. IF SUCCESSFUL. */



    /* Parameter adjustments */
    --pow;
    --e;
    e2_dim1 = *nx;
    e2_offset = e2_dim1 + 1;
    e2 -= e2_offset;

    /* Function Body */
    ret_val = FALSE_;
/* Computing MIN */
    i__1 = *m - 1;
    jhi = min(i__1,*kmax);

    i__1 = *nx;
    for (i = 1; i <= i__1; ++i) {

	if (e[i] <= (float)0.) {
	    goto L30;
	}

	loge = log(e[i]);

	i__2 = jhi;
	for (j = 1; j <= i__2; ++j) {

	    v = loge - *loglo;
	    if (e2[i + j * e2_dim1] != (float)0.) {
		v = loge - log((r__1 = e2[i + j * e2_dim1], dabs(r__1)));
	    }
/* L10: */
	    e2[i + j * e2_dim1] = pow[j] * v;
	}

/* L20: */
    }

    goto L40;

/* ... HERE FOR A NON-POSITIVE ERROR REQUEST. */

/* /6S */
/* 30   CALL SETERR(36HDSXTRP - E(I).LE.0 RETURNED BY ERROR,36,17,1) */
/* /7S */
L30:
    seterr_("DSXTRP - E(I).LE.0 RETURNED BY ERROR", &c__36, &c__17, &c__1, 
	    36L);
/* / */
    ret_val = TRUE_;

L40:
    return ret_val;

} /* d8xtrl_ */

