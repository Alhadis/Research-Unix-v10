/* istkrl.f -- translated by f2c (version of 5 February 1993  1:37:14).
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
static integer c__20 = 20;
static integer c__1 = 1;
static integer c__2 = 2;
static integer c__47 = 47;
static integer c__55 = 55;
static integer c__3 = 3;

/* Subroutine */ int istkrl_(number)
integer *number;
{
    /* Initialized data */

    static logical init = TRUE_;

    /* Local variables */
#define lmax ((integer *)&cstak_1 + 3)
#define lnow ((integer *)&cstak_1 + 1)
#define lout ((integer *)&cstak_1)
    extern /* Subroutine */ int i0tk00_();
#define lbook ((integer *)&cstak_1 + 4)
#define istak ((integer *)&cstak_1)
#define lused ((integer *)&cstak_1 + 2)
    static integer in;
    extern /* Subroutine */ int seterr_();


/*  DE-ALLOCATES THE LAST (NUMBER) ALLOCATIONS MADE IN THE STACK */
/*  BY ISTKGT. */

/*  ERROR STATES - */

/*    1 - NUMBER .LT. 0 */
/*    2 - LNOW, LUSED, LMAX OR LBOOK OVERWRITTEN */
/*    3 - ATTEMPT TO DE-ALLOCATE NON-EXISTENT ALLOCATION */
/*    4 - THE POINTER AT ISTAK(LNOW) OVERWRITTEN */





    if (init) {
	i0tk00_(&init, &c__500, &c__4);
    }

/* /6S */
/*     IF (NUMBER.LT.0) CALL SETERR(20HISTKRL - NUMBER.LT.0,20,1,2) */
/* /7S */
    if (*number < 0) {
	seterr_("ISTKRL - NUMBER.LT.0", &c__20, &c__1, &c__2, 20L);
    }
/* / */

/* /6S */
/*     IF (LNOW.LT.LBOOK.OR.LNOW.GT.LUSED.OR.LUSED.GT.LMAX) CALL SETERR */
/*    1   (47HISTKRL - LNOW, LUSED, LMAX OR LBOOK OVERWRITTEN, */
/*    2    47,2,2) */
/* /7S */
    if (*lnow < *lbook || *lnow > *lused || *lused > *lmax) {
	seterr_("ISTKRL - LNOW, LUSED, LMAX OR LBOOK OVERWRITTEN", &c__47, &
		c__2, &c__2, 47L);
    }
/* / */

    in = *number;
L10:
    if (in == 0) {
	return 0;
    }

/* /6S */
/*        IF (LNOW.LE.LBOOK) CALL SETERR */
/*    1   (55HISTKRL - ATTEMPT TO DE-ALLOCATE NON-EXISTENT ALLOCATION, */
/*    2    55,3,2) */
/* /7S */
    if (*lnow <= *lbook) {
	seterr_("ISTKRL - ATTEMPT TO DE-ALLOCATE NON-EXISTENT ALLOCATION", &
		c__55, &c__3, &c__2, 55L);
    }
/* / */

/*     CHECK TO MAKE SURE THE BACK POINTERS ARE MONOTONE. */

/* /6S */
/*        IF (ISTAK(LNOW).LT.LBOOK.OR.ISTAK(LNOW).GE.LNOW-1) CALL SETERR 
*/
/*    1   (47HISTKRL - THE POINTER AT ISTAK(LNOW) OVERWRITTEN, */
/*    2    47,4,2) */
/* /7S */
    if (istak[*lnow - 1] < *lbook || istak[*lnow - 1] >= *lnow - 1) {
	seterr_("ISTKRL - THE POINTER AT ISTAK(LNOW) OVERWRITTEN", &c__47, &
		c__4, &c__2, 47L);
    }
/* / */

    --(*lout);
    *lnow = istak[*lnow - 1];
    --in;
    goto L10;

} /* istkrl_ */

#undef lused
#undef istak
#undef lbook
#undef lout
#undef lnow
#undef lmax


