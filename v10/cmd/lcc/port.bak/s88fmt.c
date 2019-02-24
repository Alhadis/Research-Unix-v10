/* s88fmt.f -- translated by f2c (version of 5 February 1993  1:37:14).
   You must link the resulting object file with the libraries:
	-lF77 -lI77 -lm -lc   (in that order)
*/

#include "f2c.h"

/* Subroutine */ int s88fmt_(n, w, ifmt, ifmt_len)
integer *n, *w;
char *ifmt;
ftnlen ifmt_len;
{
    /* Initialized data */

    static char digits[1*10+1] = "0123456789";

    static integer nt, idigit, wt;


/*  S88FMT  REPLACES IFMT(1), ... , IFMT(N) WITH */
/*  THE CHARACTERS CORRESPONDING TO THE N LEAST SIGNIFICANT */
/*  DIGITS OF W. */

/* /6S */
/*     INTEGER IFMT(N) */
/* /7S */
/* / */


/* /6S */
/*     INTEGER DIGITS(10) */
/*     DATA DIGITS( 1) / 1H0 / */
/*     DATA DIGITS( 2) / 1H1 / */
/*     DATA DIGITS( 3) / 1H2 / */
/*     DATA DIGITS( 4) / 1H3 / */
/*     DATA DIGITS( 5) / 1H4 / */
/*     DATA DIGITS( 6) / 1H5 / */
/*     DATA DIGITS( 7) / 1H6 / */
/*     DATA DIGITS( 8) / 1H7 / */
/*     DATA DIGITS( 9) / 1H8 / */
/*     DATA DIGITS(10) / 1H9 / */
/* /7S */
    /* Parameter adjustments */
    --ifmt;

    /* Function Body */
/* / */

    nt = *n;
    wt = *w;

L10:
    if (nt <= 0) {
	return 0;
    }
    idigit = wt % 10;
    ifmt[nt] = digits[idigit];
    wt /= 10;
    --nt;
    goto L10;

} /* s88fmt_ */

