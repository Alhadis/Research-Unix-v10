/* n5err.f -- translated by f2c (version of 5 February 1993  1:37:14).
   You must link the resulting object file with the libraries:
	-lF77 -lI77 -lm -lc   (in that order)
*/

#include "f2c.h"

/* Subroutine */ int n5err_(messg, nmessg, nerr, iopt, messg_len)
char *messg;
integer *nmessg, *nerr, *iopt;
ftnlen messg_len;
{
    extern /* Subroutine */ int erroff_(), seterr_();

/* /6S */
/*     INTEGER MESSG(1) */
/* /7S */
/* / */
/*  N5ERR IS A PROCEDURE USED TO REDEFINE AN ERROR STATE. */
    /* Parameter adjustments */
    --messg;

    /* Function Body */
    erroff_();
    seterr_(messg + 1, nmessg, nerr, iopt, 1L);
    return 0;
} /* n5err_ */

