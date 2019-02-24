/* frmati.f -- translated by f2c (version of 5 February 1993  1:37:14).
   You must link the resulting object file with the libraries:
	-lF77 -lI77 -lm -lc   (in that order)
*/

#include "f2c.h"

/* Table of constant values */

static integer c__7 = 7;
static integer c__8 = 8;

/* Subroutine */ int frmati_(iwidth)
integer *iwidth;
{
    /* System generated locals */
    real r__1, r__2;

    /* Builtin functions */
    double r_lg10();

    /* Local variables */
    extern integer iceil_(), i1mach_();


/*  THIS SUBROUTINE COMPUTES THE WIDTH, W=IWIDTH, IN THE FORMAT */
/*  SPECIFICATION FOR INTEGER VARIABLES. */

/*  FRMATI SETS IWIDTH TO THE NUMBER OF CHARACTER POSITIONS NEEDED */
/*  FOR WRITING OUT THE LARGEST INTEGER PLUS ONE POSITION FOR THE SIGN. */

/*  I1MACH(7) IS THE BASE, A, FOR INTEGER REPRESENTATION IN THE MACHINE. 
*/
/*  I1MACH(8) IS THE (MAXIMUM) NUMBER OF BASE A DIGITS. */


    r__2 = (real) i1mach_(&c__7);
    r__1 = r_lg10(&r__2) * (real) i1mach_(&c__8);
    *iwidth = iceil_(&r__1) + 1;

    return 0;
} /* frmati_ */

