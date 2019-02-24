/* istkst.f -- translated by f2c (version of 5 February 1993  1:37:14).
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
static integer c__33 = 33;
static integer c__1 = 1;
static integer c__2 = 2;

integer istkst_(nfact)
integer *nfact;
{
    /* Initialized data */

    static logical init = TRUE_;

    /* System generated locals */
    integer ret_val;

    /* Local variables */
    extern /* Subroutine */ int i0tk00_();
#define istak ((integer *)&cstak_1)
    extern /* Subroutine */ int seterr_();
#define istats ((integer *)&cstak_1)


/*  RETURNS CONTROL INFORMATION AS FOLLOWS */

/*  NFACT    ITEM RETURNED */

/*    1         LOUT,  THE NUMBER OF CURRENT ALLOCATIONS */
/*    2         LNOW,  THE CURRENT ACTIVE LENGTH */
/*    3         LUSED, THE MAXIMUM USED */
/*    4         LMAX,  THE MAXIMUM ALLOWED */





    if (init) {
	i0tk00_(&init, &c__500, &c__4);
    }

/* /6S */
/*     IF (NFACT.LE.0.OR.NFACT.GE.5) CALL SETERR */
/*    1   (33HISTKST - NFACT.LE.0.OR.NFACT.GE.5,33,1,2) */
/* /7S */
    if (*nfact <= 0 || *nfact >= 5) {
	seterr_("ISTKST - NFACT.LE.0.OR.NFACT.GE.5", &c__33, &c__1, &c__2, 
		33L);
    }
/* / */

    ret_val = istats[*nfact - 1];

    return ret_val;

} /* istkst_ */

#undef istats
#undef istak


