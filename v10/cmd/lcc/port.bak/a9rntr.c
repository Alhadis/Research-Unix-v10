/* a9rntr.f -- translated by f2c (version of 5 February 1993  1:37:14).
   You must link the resulting object file with the libraries:
	-lF77 -lI77 -lm -lc   (in that order)
*/

#include "f2c.h"

/* Table of constant values */

static integer c__10 = 10;
static integer c__12 = 12;
static integer c__13 = 13;
static integer c__2 = 2;
static integer c__1 = 1;

/* Subroutine */ int a9rntr_(a, nitems, iout, mcol, w, d)
real *a;
integer *nitems, *iout, *mcol, *w, *d;
{
    /* Initialized data */

    static char blank[1+1] = " ";
    static char star[1+1] = "*";
    static integer indw = 7;
    static integer expent = 0;
    static struct {
	char e_1[20];
	} equiv_0 = { {'(', '1', 'A', '1', ',', '5', 'X', ',', '2', 'A', '1', 
		',', ' ', ' ', 'X', ',', '2', 'A', '1', ')'} };

    static struct {
	char e_1[18];
	} equiv_1 = { {'(', '1', 'A', '1', ',', 'I', '7', ',', '1', 'P', ' ', 
		'E', ' ', ' ', '.', ' ', ' ', ')'} };


    /* System generated locals */
    integer i__1, i__2, i__3, i__4;
    real r__1;

    /* Builtin functions */
    double r_lg10();
    integer s_wsfe(), do_fio(), e_wsfe();

    /* Local variables */
    static real line[18];
    static integer emin, emax, ncol;
    static real last[18];
#define ifmt1 ((char *)&equiv_0)
#define ifmt2 ((char *)&equiv_1)
    static integer i, j, k;
    extern integer iceil_(), i10wid_();
    static integer iline, ilast, count;
    extern /* Subroutine */ int s88fmt_();
    extern integer i1mach_();
#define ifmt1c ((char *)&equiv_0)
#define ifmt2c ((char *)&equiv_1)
    static integer dd;
    static real logeta;
    static integer ww;
    static logical dup;

    /* Fortran I/O blocks */
    static cilist io___23 = { 0, 0, 0, ifmt1c, 0 };
    static cilist io___24 = { 0, 0, 0, ifmt2c, 0 };
    static cilist io___26 = { 0, 0, 0, ifmt2c, 0 };



/*  THIS IS THE DOCUMENTED ROUTINE APRNTR, BUT WITHOUT THE CALLS TO */
/*  SETERR - BECAUSE IT IS CALLED BY SETERR. */

/*  THIS SUBROUTINE PRINTS OUT NITEMS FROM THE REAL ARRAY, A, ON */
/*  OUTPUT UNIT IOUT, USING A MAXIMUM OF MCOL PRINT SPACES. */
/*  THE OUTPUT FORMAT IS 1PEW.D. */
/*  THE PROGRAM PUTS AS MANY VALUES ON A LINE AS POSSIBLE. */
/*  W SHOULD BE INPUT AS THE ACTUAL WIDTH +1 FOR A SPACE BETWEEN VALUES. 
*/

/*  DUPLICATE LINES ARE NOT ALL PRINTED, BUT ARE INDICATED BY ASTERISKS. 
*/

/*  WRITTEN BY DAN WARNER, REVISED BY PHYL FOX, OCTOBER 21, 1982. */

/*  THE LINE WIDTH IS COMPUTED AS THE MINIMUM OF THE INPUT MCOL AND 160. 
*/
/*  IF THE LINE WIDTH IS TO BE INCREASED ABOVE 160, THE BUFFERS LINE() */
/*  AND LAST(), WHICH THE VALUES TO BE PRINTED ON ONE LINE, MUST */
/*  BE DIMENSIONED ACCORDINGLY. */

/*  INPUT PARAMETERS - */

/*    A        - THE START OF THE REAL ARRAY TO BE PRINTED */

/*    NITEMS   - THE NUMBER OF ITEMS TO BE PRINTED */

/*    IOUT     - THE OUTPUT UNIT FOR PRINTING */

/*    MCOL     - THE NUMBER OF SPACES ACROSS THE LINE */

/*    W        - THE WIDTH OF THE PRINTED VALUE (1PEW.D) */

/*    D        - THE NUMBER OF DIGITS AFTER THE DECIMAL POINT (1PEW.D) */


/*  ERROR STATES - NONE.  LOWER LEVEL ROUTINE CALLED BY */
/*  SETERR, SO IT CANNOT CALL SETERR. */



/* /6S */
/*     INTEGER  IFMT1(20), IFMT1C(20), IFMT2(18), IFMT2C(18), BLANK, STAR 
*/
/*     EQUIVALENCE (IFMT1(1), IFMT1C(1)), (IFMT2(1), IFMT2C(1)) */
/* /7S */
/* / */

/* /6S */
/*     DATA BLANK/1H /, STAR/1H+/, INDW/7/, EXPENT/0/ */
/* /7S */
    /* Parameter adjustments */
    --a;

    /* Function Body */
/* / */

/*  IFMT1 IS FOR THE ASTERISK LINES- IFMT2 FOR THE DATA LINES */

/* /6S */
/*     DATA IFMT1( 1) /1H(/,  IFMT2( 1) /1H(/ */
/*     DATA IFMT1( 2) /1H1/,  IFMT2( 2) /1H1/ */
/*     DATA IFMT1( 3) /1HA/,  IFMT2( 3) /1HA/ */
/*     DATA IFMT1( 4) /1H1/,  IFMT2( 4) /1H1/ */
/*     DATA IFMT1( 5) /1H,/,  IFMT2( 5) /1H,/ */
/*     DATA IFMT1( 6) /1H5/,  IFMT2( 6) /1HI/ */
/*     DATA IFMT1( 7) /1HX/,  IFMT2( 7) /1H7/ */
/*     DATA IFMT1( 8) /1H,/,  IFMT2( 8) /1H,/ */
/*     DATA IFMT1( 9) /1H2/,  IFMT2( 9) /1H1/ */
/*     DATA IFMT1(10) /1HA/,  IFMT2(10) /1HP/ */
/*     DATA IFMT1(11) /1H1/,  IFMT2(11) /1H / */
/*     DATA IFMT1(12) /1H,/,  IFMT2(12) /1HE/ */
/*     DATA IFMT1(13) /1H /,  IFMT2(13) /1H / */
/*     DATA IFMT1(14) /1H /,  IFMT2(14) /1H / */
/*     DATA IFMT1(15) /1HX/,  IFMT2(15) /1H./ */
/*     DATA IFMT1(16) /1H,/,  IFMT2(16) /1H / */
/*     DATA IFMT1(17) /1H2/,  IFMT2(17) /1H / */
/*     DATA IFMT1(18) /1HA/,  IFMT2(18) /1H)/ */
/*     DATA IFMT1(19) /1H1/ */
/*     DATA IFMT1(20) /1H)/ */
/* /7S */
/* / */


/*     EXPENT IS USED AS A FIRST-TIME SWITCH TO SIGNAL IF THE */
/*     MACHINE-VALUE CONSTANTS HAVE BEEN COMPUTED. */

    if (expent > 0) {
	goto L10;
    }
    r__1 = (real) i1mach_(&c__10);
    logeta = r_lg10(&r__1);
    r__1 = logeta * (i__1 = i1mach_(&c__12) - 1, (real) abs(i__1));
    emin = iceil_(&r__1);
    r__1 = logeta * (real) i1mach_(&c__13);
    emax = iceil_(&r__1);
    i__1 = max(emin,emax);
    expent = i10wid_(&i__1);

/*     COMPUTE THE FORMATS. */

L10:
/* Computing MIN */
/* Computing MAX */
    i__3 = *w, i__4 = expent + 5;
    i__1 = 99, i__2 = max(i__3,i__4);
    ww = min(i__1,i__2);
    s88fmt_(&c__2, &ww, ifmt2 + 12, 1L);
/* Computing MIN */
    i__1 = *d, i__2 = ww - (expent + 5);
    dd = min(i__1,i__2);
    s88fmt_(&c__2, &dd, ifmt2 + 15, 1L);

/*  NCOL IS THE NUMBER OF VALUES TO BE PRINTED ACROSS THE LINE. */

/* Computing MAX */
/* Computing MIN */
    i__3 = 9, i__4 = (min(*mcol,160) - indw) / ww;
    i__1 = 1, i__2 = min(i__3,i__4);
    ncol = max(i__1,i__2);
    s88fmt_(&c__1, &ncol, ifmt2 + 10, 1L);
    ww += -2;

/*  THE ASTERISKS ARE POSITIONED RIGHT-ADJUSTED IN THE W-WIDTH SPACE. */
    s88fmt_(&c__2, &ww, ifmt1 + 12, 1L);

/*  I COUNTS THE NUMBER OF ITEMS TO BE PRINTED, */
/*  J COUNTS THE NUMBER ON A GIVEN LINE, */
/*  COUNT COUNTS THE NUMBER OF DUPLICATE LINES. */

    i = 1;
    j = 0;
    count = 0;

/*  THE LOGICAL OF THE FOLLOWING IS ROUGHLY THIS - */
/*  IF THERE ARE STILL MORE ITEMS TO BE PRINTED, A LINE- */
/*  FULL IS PUT INTO THE ARRAY, LINE. */
/*  WHENEVER A LINE IS PRINTED OUT, IT IS ALSO STUFFED INTO */
/*  THE ARRAY, LAST, TO COMPARE WITH THE NEXT ONE COMING IN */
/*  TO CHECK FOR REPEAT OR DUPLICATED LINES. */
/*  ALSO WHENEVER A LINE IS WRITTEN OUT, THE DUPLICATION */
/*  COUNTER, COUNT, IS SET TO ONE. */
/*  THE ONLY MILDLY TRICKY PART IS TO NOTE THAT COUNT HAS TO */
/*  GO TO 3 BEFORE A LINE OF ASTERISKS IS PRINTED BECAUSE */
/*  OF COURSE NO SUCH LINE IS PRINTED FOR JUST A PAIR OF */
/*  DUPLICATE LINES. */

/*  ILINE IS PRINTED AS THE INDEX OF THE FIRST ARRAY ELEMENT */
/*  IN A LINE. */

L20:
    if (i > *nitems) {
	goto L90;
    }
    ++j;
    line[j - 1] = a[i];
    if (j == 1) {
	iline = i;
    }
    if (j < ncol && i < *nitems) {
	goto L80;
    }
    if (count == 0) {
	goto L50;
    }
    dup = TRUE_;
    i__1 = ncol;
    for (k = 1; k <= i__1; ++k) {
/* L30: */
	if (last[k - 1] != line[k - 1]) {
	    dup = FALSE_;
	}
    }
    if (i == *nitems && j < ncol) {
	dup = FALSE_;
    }
    if (! dup && count == 1) {
	goto L50;
    }
    if (! dup) {
	goto L40;
    }
    ++count;
    if (count == 3) {
	io___23.ciunit = *iout;
	s_wsfe(&io___23);
	do_fio(&c__1, blank, 1L);
	do_fio(&c__1, star, 1L);
	do_fio(&c__1, star, 1L);
	do_fio(&c__1, star, 1L);
	do_fio(&c__1, star, 1L);
	e_wsfe();
    }
    if (i == *nitems) {
	goto L50;
    }
    goto L70;
L40:
    io___24.ciunit = *iout;
    s_wsfe(&io___24);
    do_fio(&c__1, blank, 1L);
    do_fio(&c__1, (char *)&ilast, (ftnlen)sizeof(integer));
    i__1 = ncol;
    for (k = 1; k <= i__1; ++k) {
	do_fio(&c__1, (char *)&last[k - 1], (ftnlen)sizeof(real));
    }
    e_wsfe();
L50:
    io___26.ciunit = *iout;
    s_wsfe(&io___26);
    do_fio(&c__1, blank, 1L);
    do_fio(&c__1, (char *)&iline, (ftnlen)sizeof(integer));
    i__1 = j;
    for (k = 1; k <= i__1; ++k) {
	do_fio(&c__1, (char *)&line[k - 1], (ftnlen)sizeof(real));
    }
    e_wsfe();
    count = 1;
    i__1 = ncol;
    for (k = 1; k <= i__1; ++k) {
/* L60: */
	last[k - 1] = line[k - 1];
    }
L70:
    ilast = iline;
    j = 0;
L80:
    ++i;
    goto L20;
L90:
    return 0;
} /* a9rntr_ */

#undef ifmt2c
#undef ifmt1c
#undef ifmt2
#undef ifmt1


