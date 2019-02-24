/* erroff.f -- translated by f2c (version of 5 February 1993  1:37:14).
   You must link the resulting object file with the libraries:
	-lF77 -lI77 -lm -lc   (in that order)
*/

#include "f2c.h"

/* Table of constant values */

static integer c__1 = 1;
static integer c__0 = 0;

/* Subroutine */ int erroff_()
{
    static integer i;
    extern integer i8save_();


/*  TURNS OFF THE ERROR STATE OFF BY SETTING LERROR=0. */

    i = i8save_(&c__1, &c__0, &c__1);
    return 0;

} /* erroff_ */

