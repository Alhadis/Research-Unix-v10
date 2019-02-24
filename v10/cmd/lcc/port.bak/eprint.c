/* eprint.f -- translated by f2c (version of 5 February 1993  1:37:14).
   You must link the resulting object file with the libraries:
	-lF77 -lI77 -lm -lc   (in that order)
*/

#include "f2c.h"

/* Table of constant values */

static integer c__1 = 1;
static integer c__0 = 0;

/* Subroutine */ int eprint_()
{
    static char messg[1*1];
    extern /* Subroutine */ int e9rint_();


/*  THIS SUBROUTINE PRINTS THE LAST ERROR MESSAGE, IF ANY. */

/* /6S */
/*     INTEGER MESSG(1) */
/* /7S */
/* / */

    e9rint_(messg, &c__1, &c__1, &c__0, 1L);
    return 0;

} /* eprint_ */

