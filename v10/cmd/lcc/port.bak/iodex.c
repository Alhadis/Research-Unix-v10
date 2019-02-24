/* iodex.f -- translated by f2c (version of 5 February 1993  1:37:14).
   You must link the resulting object file with the libraries:
	-lF77 -lI77 -lm -lc   (in that order)
*/

#include "f2c.h"

/* Common Block Declarations */

extern struct {
    integer stats[9];
} a10der_;

#define a10der_1 a10der_

/* Table of constant values */

static integer c__0 = 0;
static integer c__2 = 2;
static integer c__9 = 9;

/* Subroutine */ int iodex_()
{
    /* Format strings */
    static char fmt_1[] = "(\002 IODE(J,F,TS,SS,E,NIT,ND,NF,R) =\002,9(i5))";

    /* Builtin functions */
    integer s_wsfe(), do_fio(), e_wsfe();

    /* Local variables */
    static integer temp;
    extern integer i1mach_();
    extern /* Subroutine */ int a1odex_();

    /* Fortran I/O blocks */
    static cilist io___2 = { 0, 0, 0, fmt_1, 0 };


/* TO PRINT THE RUN-TIME STATISTICS FOR IODE. */
    a1odex_(a10der_1.stats, &c__0);
    temp = i1mach_(&c__2);
    io___2.ciunit = temp;
    s_wsfe(&io___2);
    do_fio(&c__9, (char *)&a10der_1.stats[0], (ftnlen)sizeof(integer));
    e_wsfe();
    return 0;
} /* iodex_ */

