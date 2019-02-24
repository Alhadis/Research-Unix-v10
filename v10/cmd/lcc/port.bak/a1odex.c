/* a1odex.f -- translated by f2c (version of 5 February 1993  1:37:14).
   You must link the resulting object file with the libraries:
	-lF77 -lI77 -lm -lc   (in that order)
*/

#include "f2c.h"

/* Table of constant values */

static integer c__9 = 9;
static integer c__0 = 0;

/* Subroutine */ int a1odex_(xstats, iflag)
integer *xstats, *iflag;
{
    /* Initialized data */

    static integer stats[9] = { 0,0,0,0,0,0,0,0,0 };
    static logical inpost = FALSE_;

    extern /* Subroutine */ int seti_(), movefi_();

    /* Parameter adjustments */
    --xstats;

    /* Function Body */
/* INTERNAL SAVING OF STATISTICS FOR IODE. */
/* FOR IFLAG = 0, THE STATS ARE SIMPLY REPORTED. */
/* FOR IFLAG > 0, IT ENTERS IODE. */
/* FOR IFLAG < 0, IT EXITS IODE. */
    if (*iflag != 0) {
	goto L1;
    }
    if (! inpost) {
	movefi_(&c__9, stats, &xstats[1]);
    }
    goto L4;
L1:
    if (*iflag <= 0) {
	goto L2;
    }
    inpost = TRUE_;
    seti_(&c__9, &c__0, stats);
    movefi_(&c__9, stats, &xstats[1]);
    goto L3;
L2:
    inpost = FALSE_;
/* IFLAG < 0. */
    movefi_(&c__9, &xstats[1], stats);
L3:
L4:
    return 0;
} /* a1odex_ */

