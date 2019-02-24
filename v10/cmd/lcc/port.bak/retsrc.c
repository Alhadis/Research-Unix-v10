/* retsrc.f -- translated by f2c (version of 5 February 1993  1:37:14).
   You must link the resulting object file with the libraries:
	-lF77 -lI77 -lm -lc   (in that order)
*/

#include "f2c.h"

/* Table of constant values */

static integer c__31 = 31;
static integer c__1 = 1;
static integer c__2 = 2;
static integer c__0 = 0;

/* Subroutine */ int retsrc_(irold)
integer *irold;
{
    /* Builtin functions */
    /* Subroutine */ int s_stop();

    /* Local variables */
    static integer itemp;
    extern integer i8save_();
    extern /* Subroutine */ int eprint_(), seterr_();


/*  THIS ROUTINE SETS LRECOV = IROLD. */

/*  IF THE CURRENT ERROR BECOMES UNRECOVERABLE, */
/*  THE MESSAGE IS PRINTED AND EXECUTION STOPS. */

/*  ERROR STATES - */

/*    1 - ILLEGAL VALUE OF IROLD. */

/* /6S */
/*     IF (IROLD.LT.1 .OR. IROLD.GT.2) */
/*    1  CALL SETERR(31HRETSRC - ILLEGAL VALUE OF IROLD,31,1,2) */
/* /7S */
    if (*irold < 1 || *irold > 2) {
	seterr_("RETSRC - ILLEGAL VALUE OF IROLD", &c__31, &c__1, &c__2, 31L);
    }
/* / */

    itemp = i8save_(&c__2, irold, &c__1);

/*  IF THE CURRENT ERROR IS NOW UNRECOVERABLE, PRINT AND STOP. */

    if (*irold == 1 || i8save_(&c__1, &c__0, &c__0) == 0) {
	return 0;
    }

    eprint_();
    s_stop("", 0L);

} /* retsrc_ */

