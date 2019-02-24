/* nerror.f -- translated by f2c (version of 5 February 1993  1:37:14).
   You must link the resulting object file with the libraries:
	-lF77 -lI77 -lm -lc   (in that order)
*/

#include "f2c.h"

/* Table of constant values */

static integer c__1 = 1;
static integer c__0 = 0;

integer nerror_(nerr)
integer *nerr;
{
    /* System generated locals */
    integer ret_val;

    /* Local variables */
    extern integer i8save_();


/*  RETURNS NERROR = NERR = THE VALUE OF THE ERROR FLAG LERROR. */

    ret_val = i8save_(&c__1, &c__0, &c__0);
    *nerr = ret_val;
    return ret_val;

} /* nerror_ */

