/* s3mach.f -- translated by f2c (version of 5 February 1993  1:37:14).
   You must link the resulting object file with the libraries:
	-lF77 -lI77 -lm -lc   (in that order)
*/

#include "f2c.h"

doublereal s3mach_(xd, base, exp__)
doublereal *xd;
integer *base, *exp__;
{
    /* System generated locals */
    doublereal ret_val;

    /* Local variables */
    static integer n;
    static doublereal tbase;


/* S3MACH = XD * BASE**EXP */

/*     (17-JUN-85) -- REVISED TO MAKE OVERFLOW LESS LIKELY */

    tbase = (real) (*base);
    ret_val = *xd;

    n = *exp__;
    if (n >= 0) {
	goto L20;
    }

    n = -n;
    tbase = 1. / tbase;

L20:
    if (n % 2 != 0) {
	ret_val *= tbase;
    }
    n /= 2;
    if (n < 2) {
	goto L30;
    }
    tbase *= tbase;
    goto L20;

L30:
    if (n == 1) {
	ret_val = ret_val * tbase * tbase;
    }
    return ret_val;

} /* s3mach_ */

