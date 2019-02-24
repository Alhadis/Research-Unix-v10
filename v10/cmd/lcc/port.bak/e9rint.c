/* e9rint.f -- translated by f2c (version of 5 February 1993  1:37:14).
   You must link the resulting object file with the libraries:
	-lF77 -lI77 -lm -lc   (in that order)
*/

#include "f2c.h"

/* Table of constant values */

static integer c__1 = 1;
static integer c__0 = 0;
static integer c__4 = 4;
static integer c__2 = 2;
static integer c__6 = 6;

/* Subroutine */ int e9rint_(messg, nw, nerr, save, messg_len)
char *messg;
integer *nw, *nerr;
logical *save;
ftnlen messg_len;
{
    /* Initialized data */

    static char messgp[1*72+1] = "1";
    static integer nwp = 0;
    static integer nerrp = 0;
    static struct {
	char e_1[10];
	} equiv_0 = { {'(', '3', 'X', ',', '7', '2', 'A', 'X', 'X', ')'} };


    /* Format strings */
    static char fmt_9000[] = "(\002 ERROR \002,i4,\002 IN \002)";

    /* System generated locals */
    integer i__1;

    /* Builtin functions */
    integer s_wsfe(), do_fio(), e_wsfe();

    /* Local variables */
#define fmt10 ((char *)&equiv_0)
    static integer i;
    extern /* Subroutine */ int s88fmt_();
    extern integer i1mach_(), i8save_();
    static integer iwunit;
#define fmt ((char *)&equiv_0)

    /* Fortran I/O blocks */
    static cilist io___8 = { 0, 0, 0, fmt_9000, 0 };
    static cilist io___9 = { 0, 0, 0, fmt10, 0 };



/*  THIS ROUTINE STORES THE CURRENT ERROR MESSAGE OR PRINTS THE OLD ONE, 
*/
/*  IF ANY, DEPENDING ON WHETHER OR NOT SAVE = .TRUE. . */

/*  CHANGED, BY P.FOX, MAY 18, 1983, FROM THE ORIGINAL VERSION IN ORDER */
/*  TO GET RID OF THE FORTRAN CARRIAGE CONTROL LINE OVERWRITE */
/*  CHARACTER +, WHICH HAS ALWAYS CAUSED TROUBLE. */
/*  FOR THE RECORD, THE PREVIOUS VERSION HAD THE FOLLOWING ARRAY */
/*  AND CALLS -   (WHERE CCPLUS WAS DECLARED OF TYPE INTEGER) */

/*      DATA CCPLUS  / 1H+ / */

/*      DATA FMT( 1) / 1H( / */
/*      DATA FMT( 2) / 1HA / */
/*      DATA FMT( 3) / 1H1 / */
/*      DATA FMT( 4) / 1H, / */
/*      DATA FMT( 5) / 1H1 / */
/*      DATA FMT( 6) / 1H4 / */
/*      DATA FMT( 7) / 1HX / */
/*      DATA FMT( 8) / 1H, / */
/*      DATA FMT( 9) / 1H7 / */
/*      DATA FMT(10) / 1H2 / */
/*      DATA FMT(11) / 1HA / */
/*      DATA FMT(12) / 1HX / */
/*      DATA FMT(13) / 1HX / */
/*      DATA FMT(14) / 1H) / */

/*        CALL S88FMT(2,I1MACH(6),FMT(12)) */
/*        WRITE(IWUNIT,FMT) CCPLUS,(MESSGP(I),I=1,NWP) */

/* /6S */
/*     INTEGER MESSG(NW) */
/* /7S */
/* / */

/*  MESSGP STORES AT LEAST THE FIRST 72 CHARACTERS OF THE PREVIOUS */
/*  MESSAGE. ITS LENGTH IS MACHINE DEPENDENT AND MUST BE AT LEAST */

/*       1 + 71/(THE NUMBER OF CHARACTERS STORED PER INTEGER WORD). */

/* /6S */
/*     INTEGER MESSGP(36),FMT(10), FMT10(10) */
/*     EQUIVALENCE (FMT(1),FMT10(1)) */
/* /7S */
/* / */

/*  START WITH NO PREVIOUS MESSAGE. */

/* /6S */
/*     DATA MESSGP(1)/1H1/, NWP/0/, NERRP/0/ */
/* /7S */
    /* Parameter adjustments */
    --messg;

    /* Function Body */
/* / */

/*  SET UP THE FORMAT FOR PRINTING THE ERROR MESSAGE. */
/*  THE FORMAT IS SIMPLY (A1,14X,72AXX) WHERE XX=I1MACH(6) IS THE */
/*  NUMBER OF CHARACTERS STORED PER INTEGER WORD. */

/* /6S */
/*     DATA FMT( 1) / 1H( / */
/*     DATA FMT( 2) / 1H3 / */
/*     DATA FMT( 3) / 1HX / */
/*     DATA FMT( 4) / 1H, / */
/*     DATA FMT( 5) / 1H7 / */
/*     DATA FMT( 6) / 1H2 / */
/*     DATA FMT( 7) / 1HA / */
/*     DATA FMT( 8) / 1HX / */
/*     DATA FMT( 9) / 1HX / */
/*     DATA FMT(10) / 1H) / */
/* /7S */
/* / */

    if (! (*save)) {
	goto L20;
    }

/*  SAVE THE MESSAGE. */

    nwp = *nw;
    nerrp = *nerr;
    i__1 = *nw;
    for (i = 1; i <= i__1; ++i) {
/* L10: */
	messgp[i - 1] = messg[i];
    }

    goto L30;

L20:
    if (i8save_(&c__1, &c__0, &c__0) == 0) {
	goto L30;
    }

/*  PRINT THE MESSAGE. */

    iwunit = i1mach_(&c__4);
    io___8.ciunit = iwunit;
    s_wsfe(&io___8);
    do_fio(&c__1, (char *)&nerrp, (ftnlen)sizeof(integer));
    e_wsfe();

    i__1 = i1mach_(&c__6);
    s88fmt_(&c__2, &i__1, fmt + 7, 1L);
    io___9.ciunit = iwunit;
    s_wsfe(&io___9);
    i__1 = nwp;
    for (i = 1; i <= i__1; ++i) {
	do_fio(&c__1, messgp + (i - 1), 1L);
    }
    e_wsfe();

L30:
    return 0;

} /* e9rint_ */

#undef fmt
#undef fmt10


