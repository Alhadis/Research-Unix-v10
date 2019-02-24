/* istkmd.f -- translated by f2c (version of 5 February 1993  1:37:14).
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

static integer c__1 = 1;
static integer c__35 = 35;
static integer c__2 = 2;

integer istkmd_(nitems)
integer *nitems;
{
    /* System generated locals */
    integer ret_val;

    /* Local variables */
#define lnow ((integer *)&cstak_1 + 1)
#define istak ((integer *)&cstak_1)
    static integer itype, lnowo;
    extern /* Subroutine */ int seterr_();
    extern integer istkgt_();
    extern /* Subroutine */ int istkrl_();


/*  CHANGES THE LENGTH OF THE FRAME AT THE TOP OF THE STACK */
/*  TO NITEMS. */

/*  ERROR STATES - */

/*    1 - LNOW OVERWRITTEN */
/*    2 - ISTAK(LNOWO-1) OVERWRITTEN */




    lnowo = *lnow;
    istkrl_(&c__1);

    itype = istak[lnowo - 2];

/* /6S */
/*     IF (ITYPE.LE.0.OR.ITYPE.GE.6) CALL SETERR */
/*    1   (35HISTKMD - ISTAK(LNOWO-1) OVERWRITTEN,35,1,2) */
/* /7S */
    if (itype <= 0 || itype >= 6) {
	seterr_("ISTKMD - ISTAK(LNOWO-1) OVERWRITTEN", &c__35, &c__1, &c__2, 
		35L);
    }
/* / */

    ret_val = istkgt_(nitems, &itype);

    return ret_val;

} /* istkmd_ */

#undef istak
#undef lnow


