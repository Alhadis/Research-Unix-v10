/* r1mach.f -- translated by f2c (version of 5 February 1993  1:37:14).
   You must link the resulting object file with the libraries:
	-lF77 -lI77 -lm -lc   (in that order)
*/

#include "f2c.h"

/* Table of constant values */

static integer c__24 = 24;
static integer c__1 = 1;
static integer c__2 = 2;

doublereal r1mach_(i)
integer *i;
{
    /* Initialized data */

    static integer sc = 987;
    static struct {
	integer e_1[5];
	integer fill_2[1];
	real e_3;
	} equiv_4 = { 128, -32769, 13440, 13568, 547045274, {0}, (float)0. };


    /* System generated locals */
    real ret_val;

    /* Builtin functions */
    /* Subroutine */ int s_stop();

    /* Local variables */
#define log10 ((integer *)&equiv_4 + 4)
#define large ((integer *)&equiv_4 + 1)
#define rmach ((real *)&equiv_4)
#define small ((integer *)&equiv_4)
#define diver ((integer *)&equiv_4 + 3)
#define right ((integer *)&equiv_4 + 2)
    extern /* Subroutine */ int seterr_();


/*  SINGLE-PRECISION MACHINE CONSTANTS */

/*  R1MACH(1) = B**(EMIN-1), THE SMALLEST POSITIVE MAGNITUDE. */

/*  R1MACH(2) = B**EMAX*(1 - B**(-T)), THE LARGEST MAGNITUDE. */

/*  R1MACH(3) = B**(-T), THE SMALLEST RELATIVE SPACING. */

/*  R1MACH(4) = B**(1-T), THE LARGEST RELATIVE SPACING. */

/*  R1MACH(5) = LOG10(B) */

/*  TO ALTER THIS FUNCTION FOR A PARTICULAR ENVIRONMENT, */
/*  THE DESIRED SET OF DATA STATEMENTS SHOULD BE ACTIVATED BY */
/*  REMOVING THE C FROM COLUMN 1. */

/*  FOR IEEE-ARITHMETIC MACHINES (BINARY STANDARD), THE FIRST */
/*  SET OF CONSTANTS BELOW SHOULD BE APPROPRIATE. */

/*  WHERE POSSIBLE, DECIMAL, OCTAL OR HEXADECIMAL CONSTANTS ARE USED */
/*  TO SPECIFY THE CONSTANTS EXACTLY.  SOMETIMES THIS REQUIRES USING */
/*  EQUIVALENT INTEGER ARRAYS.  IF YOUR COMPILER USES HALF-WORD */
/*  INTEGERS BY DEFAULT (SOMETIMES CALLED INTEGER*2), YOU MAY NEED TO */
/*  CHANGE INTEGER TO INTEGER*4 OR OTHERWISE INSTRUCT YOUR COMPILER */
/*  TO USE FULL-WORD INTEGERS IN THE NEXT 5 DECLARATIONS. */

/*  COMMENTS JUST BEFORE THE END STATEMENT (LINES STARTING WITH *) */
/*  GIVE C SOURCE FOR R1MACH. */




/*     MACHINE CONSTANTS FOR IEEE ARITHMETIC MACHINES, SUCH AS THE AT&T */
/*     3B SERIES, MOTOROLA 68000 BASED MACHINES (E.G. SUN 3 AND AT&T */
/*     PC 7300), AND 8087 BASED MICROS (E.G. IBM PC AND AT&T 6300). */

/*      DATA SMALL(1) /     8388608 / */
/*      DATA LARGE(1) /  2139095039 / */
/*      DATA RIGHT(1) /   864026624 / */
/*      DATA DIVER(1) /   872415232 / */
/*      DATA LOG10(1) /  1050288283 /, SC/987/ */

/*     MACHINE CONSTANTS FOR AMDAHL MACHINES. */

/*      DATA SMALL(1) /    1048576 / */
/*      DATA LARGE(1) / 2147483647 / */
/*      DATA RIGHT(1) /  990904320 / */
/*      DATA DIVER(1) / 1007681536 / */
/*      DATA LOG10(1) / 1091781651 /, SC/987/ */

/*     MACHINE CONSTANTS FOR THE BURROUGHS 1700 SYSTEM. */

/*      DATA RMACH(1) / Z400800000 / */
/*      DATA RMACH(2) / Z5FFFFFFFF / */
/*      DATA RMACH(3) / Z4E9800000 / */
/*      DATA RMACH(4) / Z4EA800000 / */
/*      DATA RMACH(5) / Z500E730E8 /, SC/987/ */

/*     MACHINE CONSTANTS FOR THE BURROUGHS 5700/6700/7700 SYSTEMS. */

/*      DATA RMACH(1) / O1771000000000000 / */
/*      DATA RMACH(2) / O0777777777777777 / */
/*      DATA RMACH(3) / O1311000000000000 / */
/*      DATA RMACH(4) / O1301000000000000 / */
/*      DATA RMACH(5) / O1157163034761675 /, SC/987/ */

/*     MACHINE CONSTANTS FOR FTN4 ON THE CDC 6000/7000 SERIES. */

/*      DATA RMACH(1) / 00564000000000000000B / */
/*      DATA RMACH(2) / 37767777777777777776B / */
/*      DATA RMACH(3) / 16414000000000000000B / */
/*      DATA RMACH(4) / 16424000000000000000B / */
/*      DATA RMACH(5) / 17164642023241175720B /, SC/987/ */

/*     MACHINE CONSTANTS FOR FTN5 ON THE CDC 6000/7000 SERIES. */

/*      DATA RMACH(1) / O"00564000000000000000" / */
/*      DATA RMACH(2) / O"37767777777777777776" / */
/*      DATA RMACH(3) / O"16414000000000000000" / */
/*      DATA RMACH(4) / O"16424000000000000000" / */
/*      DATA RMACH(5) / O"17164642023241175720" /, SC/987/ */

/*     MACHINE CONSTANTS FOR CONVEX C-1. */

/*      DATA RMACH(1) / '00800000'X / */
/*      DATA RMACH(2) / '7FFFFFFF'X / */
/*      DATA RMACH(3) / '34800000'X / */
/*      DATA RMACH(4) / '35000000'X / */
/*      DATA RMACH(5) / '3F9A209B'X /, SC/987/ */

/*     MACHINE CONSTANTS FOR THE CRAY 1, XMP, 2, AND 3. */

/*      DATA RMACH(1) / 200034000000000000000B / */
/*      DATA RMACH(2) / 577767777777777777776B / */
/*      DATA RMACH(3) / 377224000000000000000B / */
/*      DATA RMACH(4) / 377234000000000000000B / */
/*      DATA RMACH(5) / 377774642023241175720B /, SC/987/ */

/*     MACHINE CONSTANTS FOR THE DATA GENERAL ECLIPSE S/200. */

/*     NOTE - IT MAY BE APPROPRIATE TO INCLUDE THE FOLLOWING LINE - */
/*     STATIC RMACH(5) */

/*      DATA SMALL/20K,0/,LARGE/77777K,177777K/ */
/*      DATA RIGHT/35420K,0/,DIVER/36020K,0/ */
/*      DATA LOG10/40423K,42023K/, SC/987/ */

/*     MACHINE CONSTANTS FOR THE HARRIS SLASH 6 AND SLASH 7. */

/*      DATA SMALL(1),SMALL(2) / '20000000, '00000201 / */
/*      DATA LARGE(1),LARGE(2) / '37777777, '00000177 / */
/*      DATA RIGHT(1),RIGHT(2) / '20000000, '00000352 / */
/*      DATA DIVER(1),DIVER(2) / '20000000, '00000353 / */
/*      DATA LOG10(1),LOG10(2) / '23210115, '00000377 /, SC/987/ */

/*     MACHINE CONSTANTS FOR THE HONEYWELL DPS 8/70 SERIES. */

/*      DATA RMACH(1) / O402400000000 / */
/*      DATA RMACH(2) / O376777777777 / */
/*      DATA RMACH(3) / O714400000000 / */
/*      DATA RMACH(4) / O716400000000 / */
/*      DATA RMACH(5) / O776464202324 /, SC/987/ */

/*     MACHINE CONSTANTS FOR THE IBM 360/370 SERIES, */
/*     THE XEROX SIGMA 5/7/9 AND THE SEL SYSTEMS 85/86. */

/*      DATA RMACH(1) / Z00100000 / */
/*      DATA RMACH(2) / Z7FFFFFFF / */
/*      DATA RMACH(3) / Z3B100000 / */
/*      DATA RMACH(4) / Z3C100000 / */
/*      DATA RMACH(5) / Z41134413 /, SC/987/ */

/*     MACHINE CONSTANTS FOR THE INTERDATA 8/32 */
/*     WITH THE UNIX SYSTEM FORTRAN 77 COMPILER. */

/*     FOR THE INTERDATA FORTRAN VII COMPILER REPLACE */
/*     THE Z'S SPECIFYING HEX CONSTANTS WITH Y'S. */

/*      DATA RMACH(1) / Z'00100000' / */
/*      DATA RMACH(2) / Z'7EFFFFFF' / */
/*      DATA RMACH(3) / Z'3B100000' / */
/*      DATA RMACH(4) / Z'3C100000' / */
/*      DATA RMACH(5) / Z'41134413' /, SC/987/ */

/*     MACHINE CONSTANTS FOR THE PDP-10 (KA OR KI PROCESSOR). */

/*      DATA RMACH(1) / "000400000000 / */
/*      DATA RMACH(2) / "377777777777 / */
/*      DATA RMACH(3) / "146400000000 / */
/*      DATA RMACH(4) / "147400000000 / */
/*      DATA RMACH(5) / "177464202324 /, SC/987/ */

/*     MACHINE CONSTANTS FOR PDP-11 FORTRANS SUPPORTING */
/*     32-BIT INTEGERS (EXPRESSED IN INTEGER AND OCTAL). */

/*      DATA SMALL(1) /    8388608 / */
/*      DATA LARGE(1) / 2147483647 / */
/*      DATA RIGHT(1) /  880803840 / */
/*      DATA DIVER(1) /  889192448 / */
/*      DATA LOG10(1) / 1067065499 /, SC/987/ */

/*      DATA RMACH(1) / O00040000000 / */
/*      DATA RMACH(2) / O17777777777 / */
/*      DATA RMACH(3) / O06440000000 / */
/*      DATA RMACH(4) / O06500000000 / */
/*      DATA RMACH(5) / O07746420233 /, SC/987/ */

/*     MACHINE CONSTANTS FOR PDP-11 FORTRANS SUPPORTING */
/*     16-BIT INTEGERS  (EXPRESSED IN INTEGER AND OCTAL). */

/*      DATA SMALL(1),SMALL(2) /   128,     0 / */
/*      DATA LARGE(1),LARGE(2) / 32767,    -1 / */
/*      DATA RIGHT(1),RIGHT(2) / 13440,     0 / */
/*      DATA DIVER(1),DIVER(2) / 13568,     0 / */
/*      DATA LOG10(1),LOG10(2) / 16282,  8347 /, SC/987/ */

/*      DATA SMALL(1),SMALL(2) / O000200, O000000 / */
/*      DATA LARGE(1),LARGE(2) / O077777, O177777 / */
/*      DATA RIGHT(1),RIGHT(2) / O032200, O000000 / */
/*      DATA DIVER(1),DIVER(2) / O032400, O000000 / */
/*      DATA LOG10(1),LOG10(2) / O037632, O020233 /, SC/987/ */

/*     MACHINE CONSTANTS FOR THE SEQUENT BALANCE 8000. */

/*      DATA SMALL(1) / $00800000 / */
/*      DATA LARGE(1) / $7F7FFFFF / */
/*      DATA RIGHT(1) / $33800000 / */
/*      DATA DIVER(1) / $34000000 / */
/*      DATA LOG10(1) / $3E9A209B /, SC/987/ */

/*     MACHINE CONSTANTS FOR THE UNIVAC 1100 SERIES. */

/*      DATA RMACH(1) / O000400000000 / */
/*      DATA RMACH(2) / O377777777777 / */
/*      DATA RMACH(3) / O146400000000 / */
/*      DATA RMACH(4) / O147400000000 / */
/*      DATA RMACH(5) / O177464202324 /, SC/987/ */

/*     MACHINE CONSTANTS FOR THE VAX UNIX F77 COMPILER. */


/*     MACHINE CONSTANTS FOR THE VAX-11 WITH */
/*     FORTRAN IV-PLUS COMPILER. */

/*      DATA RMACH(1) / Z00000080 / */
/*      DATA RMACH(2) / ZFFFF7FFF / */
/*      DATA RMACH(3) / Z00003480 / */
/*      DATA RMACH(4) / Z00003500 / */
/*      DATA RMACH(5) / Z209B3F9A /, SC/987/ */

/*     MACHINE CONSTANTS FOR VAX/VMS VERSION 2.2. */

/*      DATA RMACH(1) /       '80'X / */
/*      DATA RMACH(2) / 'FFFF7FFF'X / */
/*      DATA RMACH(3) /     '3480'X / */
/*      DATA RMACH(4) /     '3500'X / */
/*      DATA RMACH(5) / '209B3F9A'X /, SC/987/ */

/*  ***  ISSUE STOP 778 IF ALL DATA STATEMENTS ARE COMMENTED... */
    if (sc != 987) {
	s_stop("778", 3L);
    }
/* /6S */
/*     IF (I .LT. 1  .OR.  I .GT. 5) */
/*    1   CALL SETERR(24HR1MACH - I OUT OF BOUNDS,24,1,2) */
/* /7S */
    if (*i < 1 || *i > 5) {
	seterr_("R1MACH - I OUT OF BOUNDS", &c__24, &c__1, &c__2, 24L);
    }
/* / */

    ret_val = rmach[*i - 1];
    return ret_val;

/* /+ C source for R1MACH -- remove the * in column 1 +/ */
/* #include <stdio.h> */
/* #include <float.h> */
/* #include <math.h> */

/* float r1mach_(long *i) */
/* { */
/* 	switch(*i){ */
/* 	  case 1: return FLT_MIN; */
/* 	  case 2: return FLT_MAX; */
/* 	  case 3: return FLT_EPSILON/FLT_RADIX; */
/* 	  case 4: return FLT_EPSILON; */
/* 	  case 5: return log10(FLT_RADIX); */
/* 	  } */

/* 	fprintf(stderr, "invalid argument: r1mach(%ld)\n", *i); */
/* 	exit(1); */
/* 	return 0; /+ for compilers that complain  +/ */
/* 		  /+ about missing return values. +/ */
/* 	} */
} /* r1mach_ */

#undef right
#undef diver
#undef small
#undef rmach
#undef large
#undef log10


