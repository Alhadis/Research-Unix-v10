/* frmatd.f -- translated by f2c (version of 5 February 1993  1:37:14).
   You must link the resulting object file with the libraries:
	-lF77 -lI77 -lm -lc   (in that order)
*/

#include "f2c.h"

/* Table of constant values */

static integer c__10 = 10;
static integer c__14 = 14;
static integer c__15 = 15;
static integer c__16 = 16;

/* Subroutine */ int frmatd_(wwidth, ewidth)
integer *wwidth, *ewidth;
{
    /* System generated locals */
    integer i__1, i__2;
    real r__1, r__2;

    /* Builtin functions */
    double r_lg10();

    /* Local variables */
    static real base;
    extern integer iflr_(), iceil_();
    static integer demin, demax;
    extern integer i1mach_();
    static integer expwid;


/*  THIS SUBROUTINE COMPUTES, FOR THE FORMAT SPECIFICATION, DW.E, THE */
/*  NUMBER OF DIGITS TO THE RIGHT OF THE DECIMAL POINT, E=EWIDTH, AND */
/*  THE FIELD WIDTH, W=WWIDTH. */

/*  WWIDTH INCLUDES THE FIVE POSITIONS NEEDED FOR THE SIGN OF THE */
/*  MANTISSA, THE SIGN OF THE EXPONENT, THE 0, THE DECIMAL POINT AND THE 
*/
/*  CHARACTER IN THE OUTPUT - +0.XXXXXXXXXD+YYYY */

/*  THE FOLLOWING MACHINE-DEPENDENT VALUES ARE USED - */

/*  I1MACH(10) - THE BASE, B */
/*  I1MACH(14) - THE NUMBER OF BASE-B DIGITS IN THE MANTISSA */
/*  I1MACH(15) - THE SMALLEST EXPONENT, EMIN */
/*  I1MACH(16) - THE LARGEST EXPONENT, EMAX */


    base = (real) i1mach_(&c__10);

    r__1 = r_lg10(&base) * (real) i1mach_(&c__14);
    *ewidth = iceil_(&r__1);

    r__1 = r_lg10(&base) * (real) (i1mach_(&c__15) - 1);
    demin = iflr_(&r__1) + 1;
    r__1 = r_lg10(&base) * (real) i1mach_(&c__16);
    demax = iceil_(&r__1);
/* Computing MAX */
    i__1 = abs(demin), i__2 = abs(demax);
    r__2 = (real) max(i__1,i__2);
    r__1 = r_lg10(&r__2);
    expwid = iflr_(&r__1) + 1;
    *wwidth = *ewidth + expwid + 5;

    return 0;
} /* frmatd_ */

