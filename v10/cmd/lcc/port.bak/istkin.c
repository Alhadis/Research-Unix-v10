/* istkin.f -- translated by f2c (version of 5 February 1993  1:37:14).
   You must link the resulting object file with the libraries:
	-lF77 -lI77 -lm -lc   (in that order)
*/

#include "f2c.h"

/* Table of constant values */

static integer c__20 = 20;
static integer c__1 = 1;
static integer c__2 = 2;
static integer c__33 = 33;

/* Subroutine */ int istkin_(nitems, itype)
integer *nitems, *itype;
{
    /* Initialized data */

    static logical init = TRUE_;

    extern /* Subroutine */ int i0tk00_(), seterr_();


/*  INITIALIZES THE STACK ALLOCATOR, SETTING THE LENGTH OF THE STACK. */

/*  ERROR STATES - */

/*    1 - NITEMS .LE. 0 */
/*    2 - ITYPE .LE. 0 .OR. ITYPE .GE. 6 */



/* /6S */
/*     IF (NITEMS.LE.0) CALL SETERR(20HISTKIN - NITEMS.LE.0,20,1,2) */
/* /7S */
    if (*nitems <= 0) {
	seterr_("ISTKIN - NITEMS.LE.0", &c__20, &c__1, &c__2, 20L);
    }
/* / */

/* /6S */
/*     IF (ITYPE.LE.0.OR.ITYPE.GE.6) CALL SETERR */
/*    1   (33HISTKIN - ITYPE.LE.0.OR.ITYPE.GE.6,33,2,2) */
/* /7S */
    if (*itype <= 0 || *itype >= 6) {
	seterr_("ISTKIN - ITYPE.LE.0.OR.ITYPE.GE.6", &c__33, &c__2, &c__2, 
		33L);
    }
/* / */

    if (init) {
	i0tk00_(&init, nitems, itype);
    }

    return 0;

} /* istkin_ */

