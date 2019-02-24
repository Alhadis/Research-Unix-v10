/* sdump.f -- translated by f2c (version of 5 February 1993  1:37:14).
   You must link the resulting object file with the libraries:
	-lF77 -lI77 -lm -lc   (in that order)
*/

#include "f2c.h"

/* Subroutine */ int sdump_()
{
    extern /* Subroutine */ int fdump_(), stkdmp_();

/*   THIS IS THE STANDARD DUMP ROUTINE FOR THE PORT LIBRARY. */
/*   FIRST IT PROVIDES A FORMATTED DUMP OF THE PORT STACK. */
/*   THEN IT CALLS THE LOCAL (PREFERABLY SYMBOLIC) DUMP ROUTINE. */
    stkdmp_();
    fdump_();
    return 0;
} /* sdump_ */

