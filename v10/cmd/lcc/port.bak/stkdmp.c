/* stkdmp.f -- translated by f2c (version of 5 February 1993  1:37:14).
   You must link the resulting object file with the libraries:
	-lF77 -lI77 -lm -lc   (in that order)
*/

#include "f2c.h"

/* Common Block Declarations */

extern struct {
    doublereal dstak[500];
} cstak_;

#define cstak_1 cstak_

/* Table of constant values */

static integer c__500 = 500;
static integer c__4 = 4;
static integer c__1 = 1;

/* Subroutine */ int stkdmp_()
{
    /* Initialized data */

    static integer mcol = 132;
    static logical init = TRUE_;

    /* Format strings */
    static char fmt_9901[] = "(\0021STACK DUMP\002)";
    static char fmt_9902[] = "(\0020STACK HEADING IS OVERWRITTEN\002)";
    static char fmt_9903[] = "(\002 UNSTRUCTURED DUMP OF THE DEFAULT STACK F\
OLLOWS\002)";
    static char fmt_9904[] = "(\0020STORAGE PARAMETERS\002)";
    static char fmt_9905[] = "(\002 LOGICAL          \002,i7,\002 STORAGE UN\
ITS\002)";
    static char fmt_9906[] = "(\002 INTEGER          \002,i7,\002 STORAGE UN\
ITS\002)";
    static char fmt_9907[] = "(\002 REAL             \002,i7,\002 STORAGE UN\
ITS\002)";
    static char fmt_9908[] = "(\002 DOUBLE PRECISION \002,i7,\002 STORAGE UN\
ITS\002)";
    static char fmt_9909[] = "(\002 COMPLEX          \002,i7,\002 STORAGE UN\
ITS\002)";
    static char fmt_9910[] = "(\0020STACK STATISTICS\002)";
    static char fmt_9911[] = "(\002 STACK SIZE            \002,i7)";
    static char fmt_9912[] = "(\002 MAXIMUM STACK USED    \002,i7)";
    static char fmt_9913[] = "(\002 CURRENT STACK USED    \002,i7)";
    static char fmt_9914[] = "(\002 NUMBER OF ALLOCATIONS \002,i7)";
    static char fmt_9915[] = "(\0020STACK PARTIALLY OVERWRITTEN\002)";
    static char fmt_9916[] = "(\002 UNSTRUCTURED DUMP OF REMAINING STACK FOL\
LOWS\002)";
    static char fmt_9917[] = "(\0020ALLOCATION =\002,i7,\002,          POINT\
ER =\002,i7,\002,          TYPE LOGICAL\002)";
    static char fmt_9918[] = "(\0020ALLOCATION =\002,i7,\002,          POINT\
ER =\002,i7,\002,          TYPE INTEGER\002)";
    static char fmt_9919[] = "(\0020ALLOCATION =\002,i7,\002,          POINT\
ER =\002,i7,\002,          TYPE REAL\002)";
    static char fmt_9920[] = "(\0020ALLOCATION =\002,i7,\002,          POINT\
ER =\002,i7,\002,          TYPE DOUBLE PRECISION\002)";
    static char fmt_9921[] = "(\0020ALLOCATION =\002,i7,\002,          POINT\
ER =\002,i7,\002,          TYPE COMPLEX\002)";
    static char fmt_9922[] = "(\0020END OF STACK DUMP\002)";

    /* Builtin functions */
    integer s_wsfe(), e_wsfe(), do_fio();

    /* Local variables */
#define lmax ((integer *)&cstak_1 + 3)
    static integer iptr;
#define lnow ((integer *)&cstak_1 + 1)
#define lout ((integer *)&cstak_1)
    extern /* Subroutine */ int i0tk00_();
    static logical trbl1, trbl2;
    extern /* Subroutine */ int u9dmp_();
    static integer i;
#define lbook ((integer *)&cstak_1 + 4)
#define istak ((integer *)&cstak_1)
#define lused ((integer *)&cstak_1 + 2)
#define lstak ((logical *)&cstak_1)
#define isize ((integer *)&cstak_1 + 5)
#define rstak ((real *)&cstak_1)
    static integer bpntr, lnext, itype, llout;
    extern integer i1mach_();
    extern /* Subroutine */ int a9rntc_(), a9rntd_(), a9rnti_();
    static integer dd;
    extern /* Subroutine */ int a9rntl_(), a9rntr_();
    static integer dr, wd, wi, wr;
    extern /* Subroutine */ int frmatd_();
#define cmstak ((complex *)&cstak_1)
    extern /* Subroutine */ int frmati_(), frmatr_();
    static integer nitems, errout, lng[5];

    /* Fortran I/O blocks */
    static cilist io___14 = { 0, 0, 0, fmt_9901, 0 };
    static cilist io___21 = { 0, 0, 0, fmt_9902, 0 };
    static cilist io___22 = { 0, 0, 0, fmt_9903, 0 };
    static cilist io___24 = { 0, 0, 0, fmt_9904, 0 };
    static cilist io___25 = { 0, 0, 0, fmt_9905, 0 };
    static cilist io___26 = { 0, 0, 0, fmt_9906, 0 };
    static cilist io___27 = { 0, 0, 0, fmt_9907, 0 };
    static cilist io___28 = { 0, 0, 0, fmt_9908, 0 };
    static cilist io___29 = { 0, 0, 0, fmt_9909, 0 };
    static cilist io___30 = { 0, 0, 0, fmt_9910, 0 };
    static cilist io___31 = { 0, 0, 0, fmt_9911, 0 };
    static cilist io___32 = { 0, 0, 0, fmt_9912, 0 };
    static cilist io___33 = { 0, 0, 0, fmt_9913, 0 };
    static cilist io___34 = { 0, 0, 0, fmt_9914, 0 };
    static cilist io___40 = { 0, 0, 0, fmt_9915, 0 };
    static cilist io___41 = { 0, 0, 0, fmt_9916, 0 };
    static cilist io___45 = { 0, 0, 0, fmt_9917, 0 };
    static cilist io___46 = { 0, 0, 0, fmt_9918, 0 };
    static cilist io___47 = { 0, 0, 0, fmt_9919, 0 };
    static cilist io___48 = { 0, 0, 0, fmt_9920, 0 };
    static cilist io___49 = { 0, 0, 0, fmt_9921, 0 };
    static cilist io___50 = { 0, 0, 0, fmt_9922, 0 };



/*  THIS PROCEDURE PROVIDES A DUMP OF THE PORT STACK. */

/*  WRITTEN BY D. D. WARNER. */

/*  MOSTLY REWRITTEN BY P. A. FOX, OCTOBER 13, 1982 */
/*  AND COMMENTS ADDED. */

/*  ALLOCATED REGIONS OF THE STACK ARE PRINTED OUT IN THE APPROPRIATE */
/*  FORMAT, EXCEPT IF THE STACK APPEARS TO HAVE BEEN OVERWRITTEN. */
/*  IF OVERWRITE SEEMS TO HAVE HAPPENED, THE ENTIRE STACK IS PRINTED OUT 
*/
/*  IN UNSTRUCTURED FORM, ONCE FOR EACH OF THE POSSIBLE */
/*  (LOGICAL, INTEGER, REAL, DOUBLE PRECISION, OR COMPLEX) FORMATS. */

/* /R */
/*     REAL CMSTAK(2,500) */
/* /C */
/* / */



/* /R */
/*     EQUIVALENCE (DSTAK(1), CMSTAK(1,1)) */
/* /C */
/* / */


/*  I0TK00 CHECKS TO SEE IF THE FIRST TEN, BOOKKEEPING, LOCATIONS OF */
/*  THE STACK HAVE BEEN INITIALIZED (AND DOES IT, IF NEEDED). */

    if (init) {
	i0tk00_(&init, &c__500, &c__4);
    }


/*  I1MACH(4) IS THE STANDARD ERROR MESSAGE WRITE UNIT. */

    errout = i1mach_(&c__4);
    io___14.ciunit = errout;
    s_wsfe(&io___14);
    e_wsfe();


/*  FIND THE MACHINE-DEPENDENT FORMATS FOR PRINTING - BUT ADD 1 TO */
/*  THE WIDTH TO GET SEPARATION BETWEEN ITEMS, AND SUBTRACT 1 FROM */
/*  THE NUMBER OF DIGITS AFTER THE DECIMAL POINT TO ALLOW FOR THE */
/*  1P IN THE DUMP FORMAT OF 1PEW.D */

/*  (NOTE, THAT ALTHOUGH IT IS NOT NECESSARY, 2 HAS BEEN ADDED TO */
/*   THE INTEGER WIDTH, WI, TO CONFORM WITH DAN WARNERS PREVIOUS */
/*   USAGE - SO PEOPLE CAN COMPARE DUMPS WITH ONES THEY HAVE HAD */
/*   AROUND FOR A LONG TIME.) */

    frmatr_(&wr, &dr);
    frmatd_(&wd, &dd);
    frmati_(&wi);

    ++wr;
    ++wd;
    wi += 2;
    --dr;
    --dd;

/*  CHECK, IN VARIOUS WAYS, THE BOOKKEEPING PART OF THE STACK TO SEE */
/*  IF THINGS WERE OVERWRITTEN. */

/*  LOUT  IS THE NUMBER OF CURRENT ALLOCATIONS */
/*  LNOW  IS THE CURRENT ACTIVE LENGTH OF THE STACK */
/*  LUSED IS THE MAXIMUM VALUE OF LNOW ACHIEVED */
/*  LMAX  IS THE MAXIMUM LENGTH OF THE STACK */
/*  LBOOK IS THE NUMBER OF WORDS USED FOR BOOK-KEEPING */

    trbl1 = *lbook != 10;
    if (! trbl1) {
	trbl1 = *lmax < 12;
    }
    if (! trbl1) {
	trbl1 = *lmax < *lused;
    }
    if (! trbl1) {
	trbl1 = *lused < *lnow;
    }
    if (! trbl1) {
	trbl1 = *lnow < *lbook;
    }
    if (! trbl1) {
	trbl1 = *lout < 0;
    }
    if (! trbl1) {
	goto L10;
    }

    io___21.ciunit = errout;
    s_wsfe(&io___21);
    e_wsfe();
    io___22.ciunit = errout;
    s_wsfe(&io___22);
    e_wsfe();

/*  SINCE INFORMATION IS LOST, SIMPLY SET THE USUAL DEFAULT VALUES FOR */
/*  THE LENGTH OF THE ENTIRE STACK IN TERMS OF EACH (LOGICAL, INTEGER, */
/*  ETC.,) TYPE. */

    lng[0] = 1000;
    lng[1] = 1000;
    lng[2] = 1000;
    lng[3] = 500;
    lng[4] = 500;


    u9dmp_(lng, &mcol, &wi, &wr, &dr, &wd, &dd);
    goto L110;

/*  WRITE OUT THE STORAGE UNITS USED BY EACH TYPE OF VARIABLE */

L10:
    io___24.ciunit = errout;
    s_wsfe(&io___24);
    e_wsfe();
    io___25.ciunit = errout;
    s_wsfe(&io___25);
    do_fio(&c__1, (char *)&isize[0], (ftnlen)sizeof(integer));
    e_wsfe();
    io___26.ciunit = errout;
    s_wsfe(&io___26);
    do_fio(&c__1, (char *)&isize[1], (ftnlen)sizeof(integer));
    e_wsfe();
    io___27.ciunit = errout;
    s_wsfe(&io___27);
    do_fio(&c__1, (char *)&isize[2], (ftnlen)sizeof(integer));
    e_wsfe();
    io___28.ciunit = errout;
    s_wsfe(&io___28);
    do_fio(&c__1, (char *)&isize[3], (ftnlen)sizeof(integer));
    e_wsfe();
    io___29.ciunit = errout;
    s_wsfe(&io___29);
    do_fio(&c__1, (char *)&isize[4], (ftnlen)sizeof(integer));
    e_wsfe();

/*  WRITE OUT THE CURRENT STACK STATISTICS (I.E. USAGE) */

    io___30.ciunit = errout;
    s_wsfe(&io___30);
    e_wsfe();
    io___31.ciunit = errout;
    s_wsfe(&io___31);
    do_fio(&c__1, (char *)&(*lmax), (ftnlen)sizeof(integer));
    e_wsfe();
    io___32.ciunit = errout;
    s_wsfe(&io___32);
    do_fio(&c__1, (char *)&(*lused), (ftnlen)sizeof(integer));
    e_wsfe();
    io___33.ciunit = errout;
    s_wsfe(&io___33);
    do_fio(&c__1, (char *)&(*lnow), (ftnlen)sizeof(integer));
    e_wsfe();
    io___34.ciunit = errout;
    s_wsfe(&io___34);
    do_fio(&c__1, (char *)&(*lout), (ftnlen)sizeof(integer));
    e_wsfe();

/*  HERE AT LEAST THE BOOKKEEPING PART OF THE STACK HAS NOT BEEN */
/*  OVERWRITTEN. */

/*  STACKDUMP WORKS BACKWARDS FROM THE END (MOST RECENT ALLOCATION) OF */
/*  THE STACK, PRINTING INFORMATION, BUT ALWAYS CHECKING TO SEE IF */
/*  THE POINTERS FOR AN ALLOCATION HAVE BEEN OVERWRITTEN. */

/*  LLOUT COUNTS THE NUMBER OF ALLOCATIONS STILL LEFT TO PRINT */
/*  SO LLOUT IS INITIALLY LOUT OR ISTAK(1). */

/*  THE STACK ALLOCATION ROUTINE PUTS, AT THE END OF EACH ALLOCATION, */
/*  TWO EXTRA SPACES - ONE FOR THE TYPE OF THE ALLOCATION AND THE NEXT */
/*  TO HOLD A BACK POINTER TO THE PREVIOUS ALLOCATION. */
/*  THE BACK POINTER IS THEREFORE INITIALLY LOCATED AT THE INITIAL END, */
/*  LNOW, OF THE STACK. */
/*  CALL THIS LOCATION BPNTR. */

    llout = *lout;
    bpntr = *lnow;

/*  IF WE ARE DONE, THE BACK POINTER POINTS BACK INTO THE BOOKKEEPING */
/*  PART OF THE STACK. */

/*  IF WE ARE NOT DONE, OBTAIN THE NEXT REGION TO PRINT AND GET ITS TYPE. 
*/

L20:
    if (bpntr <= *lbook) {
	goto L110;
    }

    lnext = istak[bpntr - 1];
    itype = istak[bpntr - 2];

/*  SEE IF ANY OF THESE NEW DATA ARE INCONSISTENT - WHICH WOULD SIGNAL */
/*  AN OVERWRITE. */

    trbl2 = lnext < *lbook;
    if (! trbl2) {
	trbl2 = bpntr <= lnext;
    }
    if (! trbl2) {
	trbl2 = itype < 0;
    }
    if (! trbl2) {
	trbl2 = 5 < itype;
    }
    if (! trbl2) {
	goto L40;
    }

/*  HERE THERE SEEMS TO HAVE BEEN A PARTIAL OVERWRITE. */
/*  COMPUTE THE LENGTH OF THE ENTIRE STACK IN TERMS OF THE VALUES GIVEN */
/*  IN THE BOOKKEEPING PART OF THE STACK (WHICH, AT LEAST, SEEMS NOT TO */
/*  HAVE BEEN OVERWRITTEN), AND DO AN UNFORMATTED DUMP, AND RETURN. */

    io___40.ciunit = errout;
    s_wsfe(&io___40);
    e_wsfe();
    io___41.ciunit = errout;
    s_wsfe(&io___41);
    e_wsfe();

    for (i = 1; i <= 5; ++i) {
	lng[i - 1] = (bpntr * isize[1] - 1) / isize[i - 1] + 1;
/* L30: */
    }

    u9dmp_(lng, &mcol, &wi, &wr, &dr, &wd, &dd);
    goto L110;


/*  COMES HERE EACH TIME TO PRINT NEXT (BACK) ALLOCATION. */

/*  AT THIS POINT BPNTR POINTS TO THE END OF THE ALLOCATION ABOUT TO */
/*  BE PRINTED, LNEXT = ISTAK(BPNTR) POINTS BACK TO THE END OF THE */
/*  PREVIOUS ALLOCATION, AND ITYPE = ISTAK(BPNTR-1) GIVES THE TYPE OF */
/*  THE ALLOCATION ABOUT TO BE PRINTED. */

/*  THE PRINTING ROUTINES NEED TO KNOW THE START OF THE ALLOCATION AND */
/*  THE NUMBER OF ITEMS. */
/*  THESE ARE COMPUTED FROM THE EQUATIONS USED WHEN THE FUNCTION ISTKGT */
/*  COMPUTED THE ORIGINAL ALLOCATION - THE POINTER TO THE */
/*  START OF THE ALLOCATION WAS COMPUTED BY ISTKGT FROM THE (THEN) */
/*  END OF THE PREVIOUS ALLOCATION VIA THE FORMULA, */

/*           ISTKGT = (LNOW*ISIZE(2)-1)/ISIZE(ITYPE) + 2 */

L40:
    iptr = (lnext * isize[1] - 1) / isize[itype - 1] + 2;

/*  THE FUNCTION ISTKGT THEN FOUND NEW END OF THE STACK, LNOW, FROM THE */
/*  FORMULA */

/*          I = ( (ISTKGT-1+NITEMS)*ISIZE(ITYPE) - 1 )/ISIZE(2) + 3 */

/*  HERE WE SOLVE THIS FOR NITEMS TO DETERMINE THE NUMBER OF LOCATIONS */
/*  IN THIS ALLOCATION. */

    nitems = 1 - iptr + ((bpntr - 3) * isize[1] + 1) / isize[itype - 1];


/*  USE THE TYPE (INTEGER, REAL, ETC.) TO DTERMINE WHICH PRINTING */
/*  ROUTINE TO USE. */

    if (itype == 1) {
	goto L50;
    }
    if (itype == 2) {
	goto L60;
    }
    if (itype == 3) {
	goto L70;
    }
    if (itype == 4) {
	goto L80;
    }
    if (itype == 5) {
	goto L90;
    }

L50:
    io___45.ciunit = errout;
    s_wsfe(&io___45);
    do_fio(&c__1, (char *)&llout, (ftnlen)sizeof(integer));
    do_fio(&c__1, (char *)&iptr, (ftnlen)sizeof(integer));
    e_wsfe();
    a9rntl_(&lstak[iptr - 1], &nitems, &errout, &mcol);
    goto L100;

L60:
    io___46.ciunit = errout;
    s_wsfe(&io___46);
    do_fio(&c__1, (char *)&llout, (ftnlen)sizeof(integer));
    do_fio(&c__1, (char *)&iptr, (ftnlen)sizeof(integer));
    e_wsfe();
    a9rnti_(&istak[iptr - 1], &nitems, &errout, &mcol, &wi);
    goto L100;

L70:
    io___47.ciunit = errout;
    s_wsfe(&io___47);
    do_fio(&c__1, (char *)&llout, (ftnlen)sizeof(integer));
    do_fio(&c__1, (char *)&iptr, (ftnlen)sizeof(integer));
    e_wsfe();
    a9rntr_(&rstak[iptr - 1], &nitems, &errout, &mcol, &wr, &dr);
    goto L100;

L80:
    io___48.ciunit = errout;
    s_wsfe(&io___48);
    do_fio(&c__1, (char *)&llout, (ftnlen)sizeof(integer));
    do_fio(&c__1, (char *)&iptr, (ftnlen)sizeof(integer));
    e_wsfe();
    a9rntd_(&cstak_1.dstak[iptr - 1], &nitems, &errout, &mcol, &wd, &dd);
    goto L100;

L90:
    io___49.ciunit = errout;
    s_wsfe(&io___49);
    do_fio(&c__1, (char *)&llout, (ftnlen)sizeof(integer));
    do_fio(&c__1, (char *)&iptr, (ftnlen)sizeof(integer));
    e_wsfe();
/* /R */
/*              CALL A9RNTC(CMSTAK(1,IPTR), NITEMS, ERROUT, MCOL, WR,DR) 
*/
/* /C */
    a9rntc_(&cmstak[iptr - 1], &nitems, &errout, &mcol, &wr, &dr);
/* / */

L100:
    bpntr = lnext;
    --llout;
    goto L20;

L110:
    io___50.ciunit = errout;
    s_wsfe(&io___50);
    e_wsfe();
    return 0;
} /* stkdmp_ */

#undef cmstak
#undef rstak
#undef isize
#undef lstak
#undef lused
#undef istak
#undef lbook
#undef lout
#undef lnow
#undef lmax


