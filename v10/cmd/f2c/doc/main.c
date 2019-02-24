/* main.f -- translated by f2c (version of 15 December 1989  17:36:19).
   You must link the resulting object file with the libraries:
	-lF77 -lI77 -lm -lc   (in that order)
*/

#include "f2c.h"

/* Common Block Declarations */

struct cmname_1_ {
    complex c[10];
};

#define cmname_1 (*(struct cmname_1_ *) &cmname_)

/* Initialized data */

struct {
    complex e_1;
    doublereal e_2;
    real e_3;
    integer e_4[2];
    struct { integer filler[13]; } e_5;
    } cmname_ = { (float)1., (float)0., 2., (float)3., 4, 1 };


/* Table of constant values */

static integer c__14 = 14;

/* Main program */ MAIN__()
{

    /* Local variables */

#define d ((doublereal *)&cmname_1)
#define i ((integer *)&cmname_1)
#define l ((logical *)&cmname_1)
#define r ((real *)&cmname_1)
    extern /* Subroutine */ int sam_();

    sam_(cmname_1.c, d, &r[1], &i[2], &l[3], "some hollerith", &c__14, 14L);
} /* MAIN__ */

#undef r
#undef l
#undef i
#undef d


