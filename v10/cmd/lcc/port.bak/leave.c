/* leave.f -- translated by f2c (version of 5 February 1993  1:37:14).
   You must link the resulting object file with the libraries:
	-lF77 -lI77 -lm -lc   (in that order)
*/

#include "f2c.h"

/* Common Block Declarations */

extern struct {
    doublereal dstack[500];
} cstak_;

#define cstak_1 cstak_

/* Table of constant values */

static integer c_n1 = -1;
static integer c__43 = 43;
static integer c__1 = 1;
static integer c__2 = 2;
static integer c__41 = 41;
static integer c__59 = 59;
static integer c__3 = 3;
static integer c__4 = 4;
static integer c__5 = 5;

/* Subroutine */ int leave_()
{
    /* System generated locals */
    integer i__1;

    /* Local variables */
    static integer inow;
#define lout ((integer *)&cstak_1)
    static integer itemp;
    extern integer i8tsel_();
#define istack ((integer *)&cstak_1)
    extern /* Subroutine */ int retsrc_(), seterr_(), istkrl_();


/*  THIS ROUTINE */

/*    1) DE-ALLOCATES ALL SCRATCH SPACE ALLOCATED SINCE THE LAST ENTER, */
/*       INCLUDING THE LAST ENTER-BLOCK. */
/*    2) RESTORES THE RECOVERY LEVEL TO ITS VALUE */
/*       AT THE TIME OF THE LAST CALL TO ENTER. */

/*  ERROR STATES - */

/*    1 - CANNOT LEAVE BEYOND THE FIRST ENTER. */
/*    2 - ISTACK(INOW) HAS BEEN OVERWRITTEN. */
/*    3 - TOO MANY ISTKRLS OR ISTACK(1 AND/OR INOW) CLOBBERED. */
/*    4 - ISTACK(INOW+1) HAS BEEN OVERWRITTEN. */
/*    5 - ISTACK(INOW+2) HAS BEEN OVERWRITTEN. */


/*  GET THE POINTER TO THE CURRENT ENTER-BLOCK. */

    inow = i8tsel_(&c_n1);

/* /6S */
/*     IF (INOW.EQ.0) */
/*    1  CALL SETERR(43HLEAVE - CANNOT LEAVE BEYOND THE FIRST ENTER,43, */
/*    2              1,2) */
/*     IF (ISTACK(INOW).LT.1) */
/*    1  CALL SETERR(41HLEAVE - ISTACK(INOW) HAS BEEN OVERWRITTEN,41,2,2) 
*/
/*     IF (LOUT.LT.ISTACK(INOW)) CALL SETERR( */
/*    1  59HLEAVE - TOO MANY ISTKRLS OR ISTACK(1 AND/OR INOW) CLOBBERED, 
*/
/*    2  59,3,2) */
/*     IF (ISTACK(INOW+1).LT.1 .OR. ISTACK(INOW+1).GT.2) */
/*    1  CALL SETERR(43HLEAVE - ISTACK(INOW+1) HAS BEEN OVERWRITTEN, */
/*    2              43,4,2) */
/*     IF (ISTACK(INOW+2).GT.INOW-3 .OR. ISTACK(INOW+2).LT.0) */
/*    1  CALL SETERR(43HLEAVE - ISTACK(INOW+2) HAS BEEN OVERWRITTEN, */
/*    2              43,5,2) */
/* /7S */
    if (inow == 0) {
	seterr_("LEAVE - CANNOT LEAVE BEYOND THE FIRST ENTER", &c__43, &c__1, 
		&c__2, 43L);
    }
    if (istack[inow - 1] < 1) {
	seterr_("LEAVE - ISTACK(INOW) HAS BEEN OVERWRITTEN", &c__41, &c__2, &
		c__2, 41L);
    }
    if (*lout < istack[inow - 1]) {
	seterr_("LEAVE - TOO MANY ISTKRLS OR ISTACK(1 AND/OR INOW) CLOBBERED",
		 &c__59, &c__3, &c__2, 59L);
    }
    if (istack[inow] < 1 || istack[inow] > 2) {
	seterr_("LEAVE - ISTACK(INOW+1) HAS BEEN OVERWRITTEN", &c__43, &c__4, 
		&c__2, 43L);
    }
    if (istack[inow + 1] > inow - 3 || istack[inow + 1] < 0) {
	seterr_("LEAVE - ISTACK(INOW+2) HAS BEEN OVERWRITTEN", &c__43, &c__5, 
		&c__2, 43L);
    }
/* / */

/*  DE-ALLOCATE THE SCRATCH SPACE. */

    i__1 = *lout - istack[inow - 1] + 1;
    istkrl_(&i__1);

/*  RESTORE THE RECOVERY LEVEL. */

    retsrc_(&istack[inow]);

/*  LOWER THE BACK-POINTER. */

    itemp = i8tsel_(&istack[inow + 1]);

    return 0;

} /* leave_ */

#undef istack
#undef lout


