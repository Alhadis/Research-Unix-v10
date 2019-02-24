/* istkqu.f -- translated by f2c (version of 5 February 1993  1:37:14).
   You must link the resulting object file with the libraries:
	-lF77 -lI77 -lm -lc   (in that order)
*/

#include "f2c.h"

/* Common Block Declarations */

extern struct {
    doublereal dstak[500];
} cstak_;

#define cstak_1 cstak_

/* Table of constant values */

static integer c__500 = 500;
static integer c__4 = 4;
static integer c__47 = 47;
static integer c__1 = 1;
static integer c__2 = 2;
static integer c__33 = 33;

integer istkqu_(itype)
integer *itype;
{
    /* Initialized data */

    static logical init = TRUE_;

    /* System generated locals */
    integer ret_val, i__1;

    /* Local variables */
#define lmax ((integer *)&cstak_1 + 3)
#define lnow ((integer *)&cstak_1 + 1)
    extern /* Subroutine */ int i0tk00_();
#define lbook ((integer *)&cstak_1 + 4)
#define istak ((integer *)&cstak_1)
#define lused ((integer *)&cstak_1 + 2)
#define isize ((integer *)&cstak_1 + 5)
    extern /* Subroutine */ int seterr_();


/*  RETURNS THE NUMBER OF ITEMS OF TYPE ITYPE THAT REMAIN */
/*  TO BE ALLOCATED IN ONE REQUEST. */

/*  ERROR STATES - */

/*    1 - LNOW, LUSED, LMAX OR LBOOK OVERWRITTEN */
/*    2 - ITYPE .LE. 0 .OR. ITYPE .GE. 6 */






    if (init) {
	i0tk00_(&init, &c__500, &c__4);
    }

/* /6S */
/*     IF (LNOW.LT.LBOOK.OR.LNOW.GT.LUSED.OR.LUSED.GT.LMAX) CALL SETERR */
/*    1   (47HISTKQU - LNOW, LUSED, LMAX OR LBOOK OVERWRITTEN, */
/*    2    47,1,2) */
/* /7S */
    if (*lnow < *lbook || *lnow > *lused || *lused > *lmax) {
	seterr_("ISTKQU - LNOW, LUSED, LMAX OR LBOOK OVERWRITTEN", &c__47, &
		c__1, &c__2, 47L);
    }
/* / */

/* /6S */
/*     IF (ITYPE.LE.0.OR.ITYPE.GE.6) CALL SETERR */
/*    1   (33HISTKQU - ITYPE.LE.0.OR.ITYPE.GE.6,33,2,2) */
/* /7S */
    if (*itype <= 0 || *itype >= 6) {
	seterr_("ISTKQU - ITYPE.LE.0.OR.ITYPE.GE.6", &c__33, &c__2, &c__2, 
		33L);
    }
/* / */

/* Computing MAX */
    i__1 = (*lmax - 2) * isize[1] / isize[*itype - 1] - (*lnow * isize[1] - 1)
	     / isize[*itype - 1] - 1;
    ret_val = max(i__1,0);

    return ret_val;

} /* istkqu_ */

#undef isize
#undef lused
#undef istak
#undef lbook
#undef lnow
#undef lmax


