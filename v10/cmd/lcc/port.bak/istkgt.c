/* istkgt.f -- translated by f2c (version of 5 February 1993  1:37:14).
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
static integer c__33 = 33;
static integer c__47 = 47;
static integer c__3 = 3;
static integer c__69 = 69;

integer istkgt_(nitems, itype)
integer *nitems, *itype;
{
    /* Initialized data */

    static logical init = TRUE_;

    /* System generated locals */
    integer ret_val;

    /* Local variables */
#define lmax ((integer *)&cstak_1 + 3)
#define lnow ((integer *)&cstak_1 + 1)
#define lout ((integer *)&cstak_1)
    extern /* Subroutine */ int i0tk00_();
    static integer i;
#define lbook ((integer *)&cstak_1 + 4)
#define istak ((integer *)&cstak_1)
#define lused ((integer *)&cstak_1 + 2)
#define isize ((integer *)&cstak_1 + 5)
    extern /* Subroutine */ int seterr_();


/*  ALLOCATES SPACE OUT OF THE INTEGER ARRAY ISTAK (IN COMMON */
/*  BLOCK CSTAK) FOR AN ARRAY OF LENGTH NITEMS AND OF TYPE */
/*  DETERMINED BY ITYPE AS FOLLOWS */

/*    1 - LOGICAL */
/*    2 - INTEGER */
/*    3 - REAL */
/*    4 - DOUBLE PRECISION */
/*    5 - COMPLEX */

/*  ON RETURN, THE ARRAY WILL OCCUPY */

/*    STAK(ISTKGT), STAK(ISTKGT+1), ..., STAK(ISTKGT-NITEMS+1) */

/*  WHERE STAK IS AN ARRAY OF TYPE ITYPE EQUIVALENCED TO ISTAK. */

/*  (FOR THOSE WANTING TO MAKE MACHINE DEPENDENT MODIFICATIONS */
/*  TO SUPPORT OTHER TYPES, CODES 6,7,8,9,10,11 AND 12 HAVE */
/*  BEEN RESERVED FOR 1/4 LOGICAL, 1/2 LOGICAL, 1/4 INTEGER, */
/*  1/2 INTEGER, QUAD PRECISION, DOUBLE COMPLEX AND QUAD */
/*  COMPLEX, RESPECTIVELY.) */

/*  THE ALLOCATOR RESERVES THE FIRST TEN INTEGER WORDS OF THE STACK */
/*  FOR ITS OWN INTERNAL BOOK-KEEPING. THESE ARE INITIALIZED BY */
/*  THE INITIALIZING SUBPROGRAM I0TK00 UPON THE FIRST CALL */
/*  TO A SUBPROGRAM IN THE ALLOCATION PACKAGE. */

/*  THE USE OF THE FIRST FIVE WORDS IS DESCRIBED BELOW. */

/*    ISTAK( 1) - LOUT,  THE NUMBER OF CURRENT ALLOCATIONS. */
/*    ISTAK( 2) - LNOW,  THE CURRENT ACTIVE LENGTH OF THE STACK. */
/*    ISTAK( 3) - LUSED, THE MAXIMUM VALUE OF ISTAK(2) ACHIEVED. */
/*    ISTAK( 4) - LMAX,  THE MAXIMUM LENGTH THE STACK. */
/*    ISTAK( 5) - LBOOK, THE NUMBER OF WORDS USED FOR BOOKEEPING. */

/*  THE NEXT FIVE WORDS CONTAIN INTEGERS DESCRIBING THE AMOUNT */
/*  OF STORAGE ALLOCATED BY THE FORTRAN SYSTEM TO THE VARIOUS */
/*  DATA TYPES.  THE UNIT OF MEASUREMENT IS ARBITRARY AND MAY */
/*  BE WORDS, BYTES OR BITS OR WHATEVER IS CONVENIENT.  THE */
/*  VALUES CURRENTLY ASSUMED CORRESPOND TO AN ANS FORTRAN */
/*  ENVIRONMENT.  FOR SOME MINI-COMPUTER SYSTEMS THE VALUES MAY */
/*  HAVE TO BE CHANGED (SEE I0TK00). */

/*    ISTAK( 6) - THE NUMBER OF UNITS ALLOCATED TO LOGICAL */
/*    ISTAK( 7) - THE NUMBER OF UNITS ALLOCATED TO INTEGER */
/*    ISTAK( 8) - THE NUMBER OF UNITS ALLOCATED TO REAL */
/*    ISTAK( 9) - THE NUMBER OF UNITS ALLOCATED TO DOUBLE PRECISION */
/*    ISTAK(10) - THE NUMBER OF UNITS ALLOCATED TO COMPLEX */

/*  ERROR STATES - */

/*    1 - NITEMS .LT. 0 */
/*    2 - ITYPE .LE. 0 .OR. ITYPE .GE. 6 */
/*    3 - LNOW, LUSED, LMAX OR LBOOK OVERWRITTEN */
/*    4 - STACK OVERFLOW */






    if (init) {
	i0tk00_(&init, &c__500, &c__4);
    }

/* /6S */
/*     IF (NITEMS.LT.0) CALL SETERR(20HISTKGT - NITEMS.LT.0,20,1,2) */
/* /7S */
    if (*nitems < 0) {
	seterr_("ISTKGT - NITEMS.LT.0", &c__20, &c__1, &c__2, 20L);
    }
/* / */

/* /6S */
/*     IF (ITYPE.LE.0 .OR. ITYPE.GE.6) CALL SETERR */
/*    1   (33HISTKGT - ITYPE.LE.0.OR.ITYPE.GE.6,33,2,2) */
/* /7S */
    if (*itype <= 0 || *itype >= 6) {
	seterr_("ISTKGT - ITYPE.LE.0.OR.ITYPE.GE.6", &c__33, &c__2, &c__2, 
		33L);
    }
/* / */

/* /6S */
/*     IF (LNOW.LT.LBOOK.OR.LNOW.GT.LUSED.OR.LUSED.GT.LMAX) CALL SETERR */
/*    1   (47HISTKGT - LNOW, LUSED, LMAX OR LBOOK OVERWRITTEN, */
/*    2    47,3,2) */
/* /7S */
    if (*lnow < *lbook || *lnow > *lused || *lused > *lmax) {
	seterr_("ISTKGT - LNOW, LUSED, LMAX OR LBOOK OVERWRITTEN", &c__47, &
		c__3, &c__2, 47L);
    }
/* / */

    ret_val = (*lnow * isize[1] - 1) / isize[*itype - 1] + 2;
    i = ((ret_val - 1 + *nitems) * isize[*itype - 1] - 1) / isize[1] + 3;

/*  STACK OVERFLOW IS AN UNRECOVERABLE ERROR. */

/* /6S */
/*     IF (I.GT.LMAX) CALL SETERR(69HISTKGT - STACK TOO SHORT. ENLARGE IT 
*/
/*    1 AND CALL ISTKIN IN MAIN PROGRAM.,69,4,2) */
/* /7S */
    if (i > *lmax) {
	seterr_("ISTKGT - STACK TOO SHORT. ENLARGE IT AND CALL ISTKIN IN MAI\
N PROGRAM.", &c__69, &c__4, &c__2, 69L);
    }
/* / */

/*  ISTAK(I-1) CONTAINS THE TYPE FOR THIS ALLOCATION. */
/*  ISTAK(I  ) CONTAINS A POINTER TO THE END OF THE PREVIOUS */
/*             ALLOCATION. */

    istak[i - 2] = *itype;
    istak[i - 1] = *lnow;
    ++(*lout);
    *lnow = i;
    *lused = max(*lused,*lnow);

    return ret_val;

} /* istkgt_ */

#undef isize
#undef lused
#undef istak
#undef lbook
#undef lout
#undef lnow
#undef lmax


