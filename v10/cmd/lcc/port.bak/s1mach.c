/* s1mach.f -- translated by f2c (version of 5 February 1993  1:37:14).
   You must link the resulting object file with the libraries:
	-lF77 -lI77 -lm -lc   (in that order)
*/

#include "f2c.h"

/* Table of constant values */

static integer c__2 = 2;
static integer c__1 = 1;
static integer c__14 = 14;
static integer c__19 = 19;
static real c_b192 = (float)1.;
static doublereal c_b273 = 1.;

/* Subroutine */ int s1mach_()
{
    /* Initialized data */

    static char ccplus[1+1] = "+";
    static struct {
	char e_1[12];
	} equiv_0 = { {'(', 'A', '1', ',', '5', '1', 'X', ',', 'I', ' ', ' ', 
		')'} };

    static struct {
	char e_1[15];
	} equiv_1 = { {'(', 'A', '1', ',', '3', '2', 'X', ',', 'E', ' ', ' ', 
		'.', ' ', ' ', ')'} };

    static struct {
	char e_1[15];
	} equiv_2 = { {'(', 'A', '1', ',', '3', '2', 'X', ',', 'D', ' ', ' ', 
		'.', ' ', ' ', ')'} };


    /* Format strings */
    static char fmt_900[] = "(//\002 FORMAT CONVERSION FOR INTEGERS IS - \
\002,3a1/\002 INTEGER CONSTANTS FOLLOW\002///)";
    static char fmt_1001[] = "(\002 THE STANDARD INPUT UNIT\002)";
    static char fmt_1002[] = "(\002 THE STANDARD OUTPUT UNIT\002)";
    static char fmt_1003[] = "(\002 THE STANDARD PUNCH UNIT\002)";
    static char fmt_1004[] = "(\002 THE STANDARD ERROR MESSAGE UNIT\002)";
    static char fmt_1005[] = "(\002 THE NUMBER OF BITS PER WORD\002)";
    static char fmt_1006[] = "(\002 THE NUMBER OF CHARACTERS PER WORD\002)";
    static char fmt_1007[] = "(\002 A, THE BASE OF AN S-DIGIT INTEGER\002)";
    static char fmt_1008[] = "(\002 S, THE NUMBER OF BASE-A DIGITS\002)";
    static char fmt_1009[] = "(\002 A**S - 1, THE LARGEST MAGNITUDE\002)";
    static char fmt_1010[] = "(\002 B, THE BASE OF A T-DIGIT FLOATING-POINT \
NUMBER\002)";
    static char fmt_1011[] = "(\002 T, THE NUMBER OF BASE-B DIGITS IN SINGLE\
-PRECISION\002)";
    static char fmt_1012[] = "(\002 EMIN, THE SMALLEST SINGLE-PRECISION EXPO\
NENT\002)";
    static char fmt_1013[] = "(\002 EMAX, THE LARGEST SINGLE-PRECISION EXPON\
ENT\002)";
    static char fmt_1014[] = "(\002 T, THE NUMBER OF BASE-B DIGITS IN DOUBLE\
-PRECISION\002)";
    static char fmt_1015[] = "(\002 EMIN, THE SMALLEST DOUBLE-PRECISION EXPO\
NENT\002)";
    static char fmt_1016[] = "(\002 EMAX, THE LARGEST DOUBLE-PRECISION EXPON\
ENT\002)";
    static char fmt_1900[] = "(//\002 FORMAT CONVERSION FOR SINGLE-PRECISION\
 IS - \002,6a1/\002 SINGLE-PRECISION CONSTANTS FOLLOW\002///)";
    static char fmt_2001[] = "(\002 THE SMALLEST POSITIVE MAGNITUDE\002)";
    static char fmt_2002[] = "(\002 THE LARGEST MAGNITUDE\002)";
    static char fmt_2003[] = "(\002 THE SMALLEST RELATIVE SPACING\002)";
    static char fmt_2004[] = "(\002 THE LARGEST RELATIVE SPACING\002)";
    static char fmt_2005[] = "(\002 LOG10 OF THE BASE\002)";
    static char fmt_2900[] = "(//\002 FORMAT CONVERSION FOR DOUBLE-PRECISION\
 IS - \002,6a1/\002 DOUBLE-PRECISION CONSTANTS FOLLOW\002///)";
    static char fmt_3001[] = "(\002 THE SMALLEST POSITIVE MAGNITUDE\002)";
    static char fmt_3002[] = "(\002 THE LARGEST MAGNITUDE\002)";
    static char fmt_3003[] = "(\002 THE SMALLEST RELATIVE SPACING\002)";
    static char fmt_3004[] = "(\002 THE LARGEST RELATIVE SPACING\002)";
    static char fmt_3005[] = "(\002 LOG10 OF THE BASE\002)";
    static char fmt_4001[] = "(\0020I1MACH(11) EXCEEDS I1MACH(14)\002)";
    static char fmt_4002[] = "(\002 I1MACH(11) =\002)";
    static char fmt_4003[] = "(\002 I1MACH(14) =\002)";
    static char fmt_4011[] = "(\0020WARNING - I1MACH(13) EXCEEDS I1MACH(16\
)\002)";
    static char fmt_4012[] = "(\002 I1MACH(13) =\002)";
    static char fmt_4013[] = "(\002 I1MACH(16) =\002)";
    static char fmt_4021[] = "(\0020WARNING - I1MACH(13) - I1MACH(12)\002)";
    static char fmt_4022[] = "(\002 EXCEEDS I1MACH(16) - I1MACH(15)\002)";
    static char fmt_4023[] = "(\002 I1MACH(12) =\002)";
    static char fmt_4024[] = "(\002 I1MACH(13) =\002)";
    static char fmt_4025[] = "(\002 I1MACH(15) =\002)";
    static char fmt_4026[] = "(\002 I1MACH(16) =\002)";
    static char fmt_4031[] = "(\0021IMACH(9) IS NOT IMACH(7)**IMACH(8) - \
1\002)";
    static char fmt_4032[] = "(\002 I1MACH(7) =\002)";
    static char fmt_4034[] = "(\002 I1MACH(8) =\002)";
    static char fmt_4035[] = "(\002 I1MACH(9) =\002)";
    static char fmt_5001[] = "(\0020R1MACH(1) DOES NOT AGREE WITH CALCULATED\
 VALUE\002)";
    static char fmt_5002[] = "(\002 R1MACH(1) =\002)";
    static char fmt_5003[] = "(\002 CALCULATED VALUE =\002)";
    static char fmt_5004[] = "(\002 DIFFERENCE = \002)";
    static char fmt_5011[] = "(\0020R1MACH(2) DOES NOT AGREE WITH CALCULATED\
 VALUE\002)";
    static char fmt_5012[] = "(\002 R1MACH(2) =\002)";
    static char fmt_5013[] = "(\002 CALCULATED VALUE =\002)";
    static char fmt_5014[] = "(\002 DIFFERENCE = \002)";
    static char fmt_5021[] = "(\0020R1MACH(3) DOES NOT AGREE WITH CALCULATED\
 VALUE\002)";
    static char fmt_5022[] = "(\002 R1MACH(3) =\002)";
    static char fmt_5023[] = "(\002 CALCULATED VALUE =\002)";
    static char fmt_5024[] = "(\002 DIFFERENCE = \002)";
    static char fmt_5031[] = "(\0020R1MACH(4) DOES NOT AGREE WITH CALCULATED\
 VALUE\002)";
    static char fmt_5032[] = "(\002 R1MACH(4) =\002)";
    static char fmt_5033[] = "(\002 CALCULATED VALUE =\002)";
    static char fmt_5034[] = "(\002 DIFFERENCE = \002)";
    static char fmt_5041[] = "(\0020R1MACH(5) DOES NOT AGREE WITH CALCULATED\
 VALUE\002)";
    static char fmt_5042[] = "(\002 R1MACH(5) =\002)";
    static char fmt_5043[] = "(\002 CALCULATED VALUE =\002)";
    static char fmt_5044[] = "(\002 DIFFERENCE = \002)";
    static char fmt_6001[] = "(\0020D1MACH(1) DOES NOT AGREE WITH CALCULATED\
 VALUE\002)";
    static char fmt_6002[] = "(\002 D1MACH(1) =\002)";
    static char fmt_6003[] = "(\002 CALCULATED VALUE =\002)";
    static char fmt_6004[] = "(\002 DIFFERENCE = \002)";
    static char fmt_6011[] = "(\0020D1MACH(2) DOES NOT AGREE WITH CALCULATED\
 VALUE\002)";
    static char fmt_6012[] = "(\002 D1MACH(2) =\002)";
    static char fmt_6013[] = "(\002 CALCULATED VALUE =\002)";
    static char fmt_6014[] = "(\002 DIFFERENCE = \002)";
    static char fmt_6021[] = "(\0020D1MACH(3) DOES NOT AGREE WITH CALCULATED\
 VALUE\002)";
    static char fmt_6022[] = "(\002 D1MACH(3) =\002)";
    static char fmt_6023[] = "(\002 CALCULATED VALUE =\002)";
    static char fmt_6024[] = "(\002 DIFFERENCE = \002)";
    static char fmt_6031[] = "(\0020D1MACH(4) DOES NOT AGREE WITH CALCULATED\
 VALUE\002)";
    static char fmt_6032[] = "(\002 D1MACH(4) =\002)";
    static char fmt_6033[] = "(\002 CALCULATED VALUE =\002)";
    static char fmt_6034[] = "(\002 DIFFERENCE = \002)";
    static char fmt_6041[] = "(\0020D1MACH(5) DOES NOT AGREE WITH CALCULATED\
 VALUE\002)";
    static char fmt_6042[] = "(\002 D1MACH(5) =\002)";
    static char fmt_6043[] = "(\002 CALCULATED VALUE =\002)";
    static char fmt_6044[] = "(\002 DIFFERENCE = \002)";
    static char fmt_7001[] = "(\0020-(-R1MACH(1)) .NE. R1MACH(1)\002)";
    static char fmt_7002[] = "(\002    R1MACH(1)  =\002)";
    static char fmt_7003[] = "(\002 -(-R1MACH(1)) =\002)";
    static char fmt_7011[] = "(\0020-(-R1MACH(2)) .NE. R1MACH(2)\002)";
    static char fmt_7012[] = "(\002    R1MACH(2)  =\002)";
    static char fmt_7013[] = "(\002 -(-R1MACH(2)) =\002)";
    static char fmt_8001[] = "(\0020-(-D1MACH(1)) .NE. D1MACH(1)\002)";
    static char fmt_8002[] = "(\002    D1MACH(1)  =\002)";
    static char fmt_8003[] = "(\002 -(-D1MACH(1)) =\002)";
    static char fmt_8011[] = "(\0020-(-D1MACH(2)) .NE. D1MACH(2)\002)";
    static char fmt_8012[] = "(\002    D1MACH(2)  =\002)";
    static char fmt_8013[] = "(\002 -(-D1MACH(2)) =\002)";
    static char fmt_9001[] = "(\002 SQRT(R1MACH(1)) =\002)";
    static char fmt_9003[] = "(\002 EXCESSIVE ERROR IN SQRT(R1MACH(1))\002\
/\002 REL. ERROR =\002)";
    static char fmt_9005[] = "(\002 SQRT(R1MACH(2)) =\002)";
    static char fmt_9007[] = "(\002 EXCESSIVE ERROR IN SQRT(R1MACH(2))\002\
/\002 REL. ERROR =\002)";
    static char fmt_9009[] = "(\002 DSQRT(D1MACH(1)) =\002)";
    static char fmt_9011[] = "(\002 EXCESSIVE ERROR IN DSQRT(D1MACH(1))\002\
/\002 REL. ERROR =\002)";
    static char fmt_9013[] = "(\002 DSQRT(D1MACH(2)) =\002)";
    static char fmt_9015[] = "(\002 EXCESSIVE ERROR IN DSQRT(D1MACH(2))\002\
/\002 REL. ERROR =\002)";

    /* System generated locals */
    integer i__1, i__2;
    real r__1, r__2;
    doublereal d__1;
    static integer equiv_6[16];

    /* Builtin functions */
    double r_lg10();
    integer s_wsfe(), do_fio(), e_wsfe();
    double d_lg10(), sqrt();

    /* Local variables */
#define dfmt ((char *)&equiv_2)
#define efmt ((char *)&equiv_1)
    extern integer iflr_();
#define ifmt ((char *)&equiv_0)
#define dfmt1 ((char *)&equiv_2)
#define efmt1 ((char *)&equiv_1)
#define ifmt1 ((char *)&equiv_0)
    static integer i;
    static doublereal dmach[5], dbase;
    static integer n;
#define imach (equiv_6)
    extern integer iceil_();
#define digdp (equiv_6 + 13)
    static real rmach[5];
    static integer demin;
    static real sbase;
    static integer demax;
#define digsp (equiv_6 + 10)
    extern /* Subroutine */ int s88fmt_();
    extern doublereal d1mach_();
    extern integer i1mach_();
    extern doublereal r1mach_(), s2mach_();
    extern doublereal s3mach_();
    static doublereal dbasem, xd, yd;
    static integer ibasem;
    static real xr, yr, sbasem;
#define digint (equiv_6 + 7)
    static integer dwidth, ewidth, wwidth;
#define stdout (equiv_6 + 1)

    /* Fortran I/O blocks */
    static cilist io___17 = { 0, 0, 0, fmt_900, 0 };
    static cilist io___18 = { 0, 0, 0, fmt_1001, 0 };
    static cilist io___19 = { 0, 0, 0, ifmt, 0 };
    static cilist io___20 = { 0, 0, 0, fmt_1002, 0 };
    static cilist io___21 = { 0, 0, 0, ifmt, 0 };
    static cilist io___22 = { 0, 0, 0, fmt_1003, 0 };
    static cilist io___23 = { 0, 0, 0, ifmt, 0 };
    static cilist io___24 = { 0, 0, 0, fmt_1004, 0 };
    static cilist io___25 = { 0, 0, 0, ifmt, 0 };
    static cilist io___26 = { 0, 0, 0, fmt_1005, 0 };
    static cilist io___27 = { 0, 0, 0, ifmt, 0 };
    static cilist io___28 = { 0, 0, 0, fmt_1006, 0 };
    static cilist io___29 = { 0, 0, 0, ifmt, 0 };
    static cilist io___30 = { 0, 0, 0, fmt_1007, 0 };
    static cilist io___31 = { 0, 0, 0, ifmt, 0 };
    static cilist io___32 = { 0, 0, 0, fmt_1008, 0 };
    static cilist io___33 = { 0, 0, 0, ifmt, 0 };
    static cilist io___34 = { 0, 0, 0, fmt_1009, 0 };
    static cilist io___35 = { 0, 0, 0, ifmt, 0 };
    static cilist io___36 = { 0, 0, 0, fmt_1010, 0 };
    static cilist io___37 = { 0, 0, 0, ifmt, 0 };
    static cilist io___38 = { 0, 0, 0, fmt_1011, 0 };
    static cilist io___39 = { 0, 0, 0, ifmt, 0 };
    static cilist io___40 = { 0, 0, 0, fmt_1012, 0 };
    static cilist io___41 = { 0, 0, 0, ifmt, 0 };
    static cilist io___42 = { 0, 0, 0, fmt_1013, 0 };
    static cilist io___43 = { 0, 0, 0, ifmt, 0 };
    static cilist io___44 = { 0, 0, 0, fmt_1014, 0 };
    static cilist io___45 = { 0, 0, 0, ifmt, 0 };
    static cilist io___46 = { 0, 0, 0, fmt_1015, 0 };
    static cilist io___47 = { 0, 0, 0, ifmt, 0 };
    static cilist io___48 = { 0, 0, 0, fmt_1016, 0 };
    static cilist io___49 = { 0, 0, 0, ifmt, 0 };
    static cilist io___54 = { 0, 0, 0, fmt_1900, 0 };
    static cilist io___55 = { 0, 0, 0, fmt_2001, 0 };
    static cilist io___56 = { 0, 0, 0, efmt, 0 };
    static cilist io___57 = { 0, 0, 0, fmt_2002, 0 };
    static cilist io___58 = { 0, 0, 0, efmt, 0 };
    static cilist io___59 = { 0, 0, 0, fmt_2003, 0 };
    static cilist io___60 = { 0, 0, 0, efmt, 0 };
    static cilist io___61 = { 0, 0, 0, fmt_2004, 0 };
    static cilist io___62 = { 0, 0, 0, efmt, 0 };
    static cilist io___63 = { 0, 0, 0, fmt_2005, 0 };
    static cilist io___64 = { 0, 0, 0, efmt, 0 };
    static cilist io___65 = { 0, 0, 0, fmt_2900, 0 };
    static cilist io___66 = { 0, 0, 0, fmt_3001, 0 };
    static cilist io___67 = { 0, 0, 0, dfmt, 0 };
    static cilist io___68 = { 0, 0, 0, fmt_3002, 0 };
    static cilist io___69 = { 0, 0, 0, dfmt, 0 };
    static cilist io___70 = { 0, 0, 0, fmt_3003, 0 };
    static cilist io___71 = { 0, 0, 0, dfmt, 0 };
    static cilist io___72 = { 0, 0, 0, fmt_3004, 0 };
    static cilist io___73 = { 0, 0, 0, dfmt, 0 };
    static cilist io___74 = { 0, 0, 0, fmt_3005, 0 };
    static cilist io___75 = { 0, 0, 0, dfmt, 0 };
    static cilist io___76 = { 0, 0, 0, fmt_4001, 0 };
    static cilist io___77 = { 0, 0, 0, fmt_4002, 0 };
    static cilist io___78 = { 0, 0, 0, ifmt, 0 };
    static cilist io___79 = { 0, 0, 0, fmt_4003, 0 };
    static cilist io___80 = { 0, 0, 0, ifmt, 0 };
    static cilist io___81 = { 0, 0, 0, fmt_4011, 0 };
    static cilist io___82 = { 0, 0, 0, fmt_4012, 0 };
    static cilist io___83 = { 0, 0, 0, ifmt, 0 };
    static cilist io___84 = { 0, 0, 0, fmt_4013, 0 };
    static cilist io___85 = { 0, 0, 0, ifmt, 0 };
    static cilist io___86 = { 0, 0, 0, fmt_4021, 0 };
    static cilist io___87 = { 0, 0, 0, fmt_4022, 0 };
    static cilist io___88 = { 0, 0, 0, fmt_4023, 0 };
    static cilist io___89 = { 0, 0, 0, ifmt, 0 };
    static cilist io___90 = { 0, 0, 0, fmt_4024, 0 };
    static cilist io___91 = { 0, 0, 0, ifmt, 0 };
    static cilist io___92 = { 0, 0, 0, fmt_4025, 0 };
    static cilist io___93 = { 0, 0, 0, ifmt, 0 };
    static cilist io___94 = { 0, 0, 0, fmt_4026, 0 };
    static cilist io___95 = { 0, 0, 0, ifmt, 0 };
    static cilist io___98 = { 0, 0, 0, fmt_4031, 0 };
    static cilist io___99 = { 0, 0, 0, fmt_4032, 0 };
    static cilist io___100 = { 0, 0, 0, ifmt, 0 };
    static cilist io___101 = { 0, 0, 0, fmt_4034, 0 };
    static cilist io___102 = { 0, 0, 0, ifmt, 0 };
    static cilist io___103 = { 0, 0, 0, fmt_4035, 0 };
    static cilist io___104 = { 0, 0, 0, ifmt, 0 };
    static cilist io___106 = { 0, 0, 0, fmt_5001, 0 };
    static cilist io___107 = { 0, 0, 0, fmt_5002, 0 };
    static cilist io___108 = { 0, 0, 0, efmt, 0 };
    static cilist io___109 = { 0, 0, 0, fmt_5003, 0 };
    static cilist io___110 = { 0, 0, 0, efmt, 0 };
    static cilist io___111 = { 0, 0, 0, fmt_5004, 0 };
    static cilist io___112 = { 0, 0, 0, efmt, 0 };
    static cilist io___115 = { 0, 0, 0, fmt_5011, 0 };
    static cilist io___116 = { 0, 0, 0, fmt_5012, 0 };
    static cilist io___117 = { 0, 0, 0, efmt, 0 };
    static cilist io___118 = { 0, 0, 0, fmt_5013, 0 };
    static cilist io___119 = { 0, 0, 0, efmt, 0 };
    static cilist io___120 = { 0, 0, 0, fmt_5014, 0 };
    static cilist io___121 = { 0, 0, 0, efmt, 0 };
    static cilist io___122 = { 0, 0, 0, fmt_5021, 0 };
    static cilist io___123 = { 0, 0, 0, fmt_5022, 0 };
    static cilist io___124 = { 0, 0, 0, efmt, 0 };
    static cilist io___125 = { 0, 0, 0, fmt_5023, 0 };
    static cilist io___126 = { 0, 0, 0, efmt, 0 };
    static cilist io___127 = { 0, 0, 0, fmt_5024, 0 };
    static cilist io___128 = { 0, 0, 0, efmt, 0 };
    static cilist io___129 = { 0, 0, 0, fmt_5031, 0 };
    static cilist io___130 = { 0, 0, 0, fmt_5032, 0 };
    static cilist io___131 = { 0, 0, 0, efmt, 0 };
    static cilist io___132 = { 0, 0, 0, fmt_5033, 0 };
    static cilist io___133 = { 0, 0, 0, efmt, 0 };
    static cilist io___134 = { 0, 0, 0, fmt_5034, 0 };
    static cilist io___135 = { 0, 0, 0, efmt, 0 };
    static cilist io___136 = { 0, 0, 0, fmt_5041, 0 };
    static cilist io___137 = { 0, 0, 0, fmt_5042, 0 };
    static cilist io___138 = { 0, 0, 0, efmt, 0 };
    static cilist io___139 = { 0, 0, 0, fmt_5043, 0 };
    static cilist io___140 = { 0, 0, 0, efmt, 0 };
    static cilist io___141 = { 0, 0, 0, fmt_5044, 0 };
    static cilist io___142 = { 0, 0, 0, efmt, 0 };
    static cilist io___144 = { 0, 0, 0, fmt_6001, 0 };
    static cilist io___145 = { 0, 0, 0, fmt_6002, 0 };
    static cilist io___146 = { 0, 0, 0, dfmt, 0 };
    static cilist io___147 = { 0, 0, 0, fmt_6003, 0 };
    static cilist io___148 = { 0, 0, 0, dfmt, 0 };
    static cilist io___149 = { 0, 0, 0, fmt_6004, 0 };
    static cilist io___150 = { 0, 0, 0, dfmt, 0 };
    static cilist io___153 = { 0, 0, 0, fmt_6011, 0 };
    static cilist io___154 = { 0, 0, 0, fmt_6012, 0 };
    static cilist io___155 = { 0, 0, 0, dfmt, 0 };
    static cilist io___156 = { 0, 0, 0, fmt_6013, 0 };
    static cilist io___157 = { 0, 0, 0, dfmt, 0 };
    static cilist io___158 = { 0, 0, 0, fmt_6014, 0 };
    static cilist io___159 = { 0, 0, 0, dfmt, 0 };
    static cilist io___160 = { 0, 0, 0, fmt_6021, 0 };
    static cilist io___161 = { 0, 0, 0, fmt_6022, 0 };
    static cilist io___162 = { 0, 0, 0, dfmt, 0 };
    static cilist io___163 = { 0, 0, 0, fmt_6023, 0 };
    static cilist io___164 = { 0, 0, 0, dfmt, 0 };
    static cilist io___165 = { 0, 0, 0, fmt_6024, 0 };
    static cilist io___166 = { 0, 0, 0, dfmt, 0 };
    static cilist io___167 = { 0, 0, 0, fmt_6031, 0 };
    static cilist io___168 = { 0, 0, 0, fmt_6032, 0 };
    static cilist io___169 = { 0, 0, 0, dfmt, 0 };
    static cilist io___170 = { 0, 0, 0, fmt_6033, 0 };
    static cilist io___171 = { 0, 0, 0, dfmt, 0 };
    static cilist io___172 = { 0, 0, 0, fmt_6034, 0 };
    static cilist io___173 = { 0, 0, 0, dfmt, 0 };
    static cilist io___174 = { 0, 0, 0, fmt_6041, 0 };
    static cilist io___175 = { 0, 0, 0, fmt_6042, 0 };
    static cilist io___176 = { 0, 0, 0, dfmt, 0 };
    static cilist io___177 = { 0, 0, 0, fmt_6043, 0 };
    static cilist io___178 = { 0, 0, 0, dfmt, 0 };
    static cilist io___179 = { 0, 0, 0, fmt_6044, 0 };
    static cilist io___180 = { 0, 0, 0, dfmt, 0 };
    static cilist io___181 = { 0, 0, 0, fmt_7001, 0 };
    static cilist io___182 = { 0, 0, 0, fmt_7002, 0 };
    static cilist io___183 = { 0, 0, 0, efmt, 0 };
    static cilist io___184 = { 0, 0, 0, fmt_7003, 0 };
    static cilist io___185 = { 0, 0, 0, efmt, 0 };
    static cilist io___186 = { 0, 0, 0, fmt_7011, 0 };
    static cilist io___187 = { 0, 0, 0, fmt_7012, 0 };
    static cilist io___188 = { 0, 0, 0, efmt, 0 };
    static cilist io___189 = { 0, 0, 0, fmt_7013, 0 };
    static cilist io___190 = { 0, 0, 0, efmt, 0 };
    static cilist io___191 = { 0, 0, 0, fmt_8001, 0 };
    static cilist io___192 = { 0, 0, 0, fmt_8002, 0 };
    static cilist io___193 = { 0, 0, 0, dfmt, 0 };
    static cilist io___194 = { 0, 0, 0, fmt_8003, 0 };
    static cilist io___195 = { 0, 0, 0, dfmt, 0 };
    static cilist io___196 = { 0, 0, 0, fmt_8011, 0 };
    static cilist io___197 = { 0, 0, 0, fmt_8012, 0 };
    static cilist io___198 = { 0, 0, 0, dfmt, 0 };
    static cilist io___199 = { 0, 0, 0, fmt_8013, 0 };
    static cilist io___200 = { 0, 0, 0, dfmt, 0 };
    static cilist io___201 = { 0, 0, 0, fmt_9001, 0 };
    static cilist io___202 = { 0, 0, 0, efmt, 0 };
    static cilist io___204 = { 0, 0, 0, fmt_9003, 0 };
    static cilist io___205 = { 0, 0, 0, efmt, 0 };
    static cilist io___206 = { 0, 0, 0, fmt_9005, 0 };
    static cilist io___207 = { 0, 0, 0, efmt, 0 };
    static cilist io___208 = { 0, 0, 0, fmt_9007, 0 };
    static cilist io___209 = { 0, 0, 0, efmt, 0 };
    static cilist io___210 = { 0, 0, 0, fmt_9009, 0 };
    static cilist io___211 = { 0, 0, 0, dfmt, 0 };
    static cilist io___213 = { 0, 0, 0, fmt_9011, 0 };
    static cilist io___214 = { 0, 0, 0, efmt, 0 };
    static cilist io___215 = { 0, 0, 0, fmt_9013, 0 };
    static cilist io___216 = { 0, 0, 0, efmt, 0 };
    static cilist io___217 = { 0, 0, 0, fmt_9015, 0 };
    static cilist io___218 = { 0, 0, 0, efmt, 0 };



/*  S1MACH TESTS THE CONSISTENCY OF THE MACHINE CONSTANTS IN */
/*  I1MACH, R1MACH AND D1MACH. */


/* /6S */
/*     INTEGER IFMT(12) */
/*     INTEGER EFMT(15) */
/*     INTEGER DFMT(15) */
/*     INTEGER CCPLUS */
/* /7S */
/* / */


/* /6S */
/*     DATA CCPLUS   / 1H+ / */
/* /7S */
/* / */

/* /6S */
/*     DATA IFMT(1 ) / 1H( / */
/*     DATA IFMT(2 ) / 1HA / */
/*     DATA IFMT(3 ) / 1H1 / */
/*     DATA IFMT(4 ) / 1H, / */
/*     DATA IFMT(5 ) / 1H5 / */
/*     DATA IFMT(6 ) / 1H1 / */
/*     DATA IFMT(7 ) / 1HX / */
/*     DATA IFMT(8 ) / 1H, / */
/*     DATA IFMT(9 ) / 1HI / */
/*     DATA IFMT(10) / 1H  / */
/*     DATA IFMT(11) / 1H  / */
/*     DATA IFMT(12) / 1H) / */
/* /7S */
/* / */

/* /6S */
/*     DATA EFMT( 1) / 1H( /,    DFMT( 1) / 1H( / */
/*     DATA EFMT( 2) / 1HA /,    DFMT( 2) / 1HA / */
/*     DATA EFMT( 3) / 1H1 /,    DFMT( 3) / 1H1 / */
/*     DATA EFMT( 4) / 1H, /,    DFMT( 4) / 1H, / */
/*     DATA EFMT( 5) / 1H3 /,    DFMT( 5) / 1H3 / */
/*     DATA EFMT( 6) / 1H2 /,    DFMT( 6) / 1H2 / */
/*     DATA EFMT( 7) / 1HX /,    DFMT( 7) / 1HX / */
/*     DATA EFMT( 8) / 1H, /,    DFMT( 8) / 1H, / */
/*     DATA EFMT( 9) / 1HE /,    DFMT( 9) / 1HD / */
/*     DATA EFMT(10) / 1H  /,    DFMT(10) / 1H  / */
/*     DATA EFMT(11) / 1H  /,    DFMT(11) / 1H  / */
/*     DATA EFMT(12) / 1H. /,    DFMT(12) / 1H. / */
/*     DATA EFMT(13) / 1H  /,    DFMT(13) / 1H  / */
/*     DATA EFMT(14) / 1H  /,    DFMT(14) / 1H  / */
/*     DATA EFMT(15) / 1H) /,    DFMT(15) / 1H) / */
/* /7S */
/* / */

/*  FETCH ALL CONSTANTS INTO LOCAL ARRAYS */

    for (i = 1; i <= 16; ++i) {
	imach[i - 1] = i1mach_(&i);
/* L10: */
    }

    for (i = 1; i <= 5; ++i) {
	rmach[i - 1] = r1mach_(&i);
	dmach[i - 1] = d1mach_(&i);
/* L20: */
    }

/*  COMPUTE NUMBER OF CHARACTER POSITIONS NEEDED FOR WRITING */
/*  OUT THE LARGEST INTEGER ALLOWING FOR ONE SPACE AND A SIGN */
/*  AND PLUG THE FIELD WIDTH IN THE FORMAT. */

    r__2 = (real) imach[6];
    r__1 = r_lg10(&r__2) * (real) imach[7];
    wwidth = iceil_(&r__1) + 2;
/* /6S */
/*     CALL S88FMT( 2, WWIDTH, IFMT(10) ) */
/*     WRITE( STDOUT, 900 ) ( IFMT(I), I = 9, 11 ) */
/* /7S */
    s88fmt_(&c__2, &wwidth, ifmt1 + 9, 1L);
    io___17.ciunit = *stdout;
    s_wsfe(&io___17);
    for (i = 9; i <= 11; ++i) {
	do_fio(&c__1, ifmt1 + (i - 1), 1L);
    }
    e_wsfe();
/* / */

/*  NOW WRITE OUT THE INTEGER CONSTANTS */

    io___18.ciunit = *stdout;
    s_wsfe(&io___18);
    e_wsfe();
    io___19.ciunit = *stdout;
    s_wsfe(&io___19);
    do_fio(&c__1, ccplus, 1L);
    do_fio(&c__1, (char *)&imach[0], (ftnlen)sizeof(integer));
    e_wsfe();

    io___20.ciunit = *stdout;
    s_wsfe(&io___20);
    e_wsfe();
    io___21.ciunit = *stdout;
    s_wsfe(&io___21);
    do_fio(&c__1, ccplus, 1L);
    do_fio(&c__1, (char *)&imach[1], (ftnlen)sizeof(integer));
    e_wsfe();

    io___22.ciunit = *stdout;
    s_wsfe(&io___22);
    e_wsfe();
    io___23.ciunit = *stdout;
    s_wsfe(&io___23);
    do_fio(&c__1, ccplus, 1L);
    do_fio(&c__1, (char *)&imach[2], (ftnlen)sizeof(integer));
    e_wsfe();

    io___24.ciunit = *stdout;
    s_wsfe(&io___24);
    e_wsfe();
    io___25.ciunit = *stdout;
    s_wsfe(&io___25);
    do_fio(&c__1, ccplus, 1L);
    do_fio(&c__1, (char *)&imach[3], (ftnlen)sizeof(integer));
    e_wsfe();

    io___26.ciunit = *stdout;
    s_wsfe(&io___26);
    e_wsfe();
    io___27.ciunit = *stdout;
    s_wsfe(&io___27);
    do_fio(&c__1, ccplus, 1L);
    do_fio(&c__1, (char *)&imach[4], (ftnlen)sizeof(integer));
    e_wsfe();

    io___28.ciunit = *stdout;
    s_wsfe(&io___28);
    e_wsfe();
    io___29.ciunit = *stdout;
    s_wsfe(&io___29);
    do_fio(&c__1, ccplus, 1L);
    do_fio(&c__1, (char *)&imach[5], (ftnlen)sizeof(integer));
    e_wsfe();

    io___30.ciunit = *stdout;
    s_wsfe(&io___30);
    e_wsfe();
    io___31.ciunit = *stdout;
    s_wsfe(&io___31);
    do_fio(&c__1, ccplus, 1L);
    do_fio(&c__1, (char *)&imach[6], (ftnlen)sizeof(integer));
    e_wsfe();

    io___32.ciunit = *stdout;
    s_wsfe(&io___32);
    e_wsfe();
    io___33.ciunit = *stdout;
    s_wsfe(&io___33);
    do_fio(&c__1, ccplus, 1L);
    do_fio(&c__1, (char *)&imach[7], (ftnlen)sizeof(integer));
    e_wsfe();

    io___34.ciunit = *stdout;
    s_wsfe(&io___34);
    e_wsfe();
    io___35.ciunit = *stdout;
    s_wsfe(&io___35);
    do_fio(&c__1, ccplus, 1L);
    do_fio(&c__1, (char *)&imach[8], (ftnlen)sizeof(integer));
    e_wsfe();

    io___36.ciunit = *stdout;
    s_wsfe(&io___36);
    e_wsfe();
    io___37.ciunit = *stdout;
    s_wsfe(&io___37);
    do_fio(&c__1, ccplus, 1L);
    do_fio(&c__1, (char *)&imach[9], (ftnlen)sizeof(integer));
    e_wsfe();

    io___38.ciunit = *stdout;
    s_wsfe(&io___38);
    e_wsfe();
    io___39.ciunit = *stdout;
    s_wsfe(&io___39);
    do_fio(&c__1, ccplus, 1L);
    do_fio(&c__1, (char *)&imach[10], (ftnlen)sizeof(integer));
    e_wsfe();

    io___40.ciunit = *stdout;
    s_wsfe(&io___40);
    e_wsfe();
    io___41.ciunit = *stdout;
    s_wsfe(&io___41);
    do_fio(&c__1, ccplus, 1L);
    do_fio(&c__1, (char *)&imach[11], (ftnlen)sizeof(integer));
    e_wsfe();

    io___42.ciunit = *stdout;
    s_wsfe(&io___42);
    e_wsfe();
    io___43.ciunit = *stdout;
    s_wsfe(&io___43);
    do_fio(&c__1, ccplus, 1L);
    do_fio(&c__1, (char *)&imach[12], (ftnlen)sizeof(integer));
    e_wsfe();

    io___44.ciunit = *stdout;
    s_wsfe(&io___44);
    e_wsfe();
    io___45.ciunit = *stdout;
    s_wsfe(&io___45);
    do_fio(&c__1, ccplus, 1L);
    do_fio(&c__1, (char *)&imach[13], (ftnlen)sizeof(integer));
    e_wsfe();

    io___46.ciunit = *stdout;
    s_wsfe(&io___46);
    e_wsfe();
    io___47.ciunit = *stdout;
    s_wsfe(&io___47);
    do_fio(&c__1, ccplus, 1L);
    do_fio(&c__1, (char *)&imach[14], (ftnlen)sizeof(integer));
    e_wsfe();

    io___48.ciunit = *stdout;
    s_wsfe(&io___48);
    e_wsfe();
    io___49.ciunit = *stdout;
    s_wsfe(&io___49);
    do_fio(&c__1, ccplus, 1L);
    do_fio(&c__1, (char *)&imach[15], (ftnlen)sizeof(integer));
    e_wsfe();

/*  COMPUTE THE NUMBER OF CHARACTER POSITIONS NEEDED FOR WRITING */
/*  OUT A SINGLE-PRECISION NUMBER ALLOWING FOR ONE SPACE AND */
/*  A SIGN AND PLUG THE FIELDS IN THE FORMAT. */

    r__2 = (real) imach[9];
    r__1 = r_lg10(&r__2) * (real) imach[10];
    dwidth = iceil_(&r__1);
/* /6S */
/*     CALL S88FMT( 2, DWIDTH, EFMT(13) ) */
/* /7S */
    s88fmt_(&c__2, &dwidth, efmt1 + 12, 1L);
/* / */
    r__2 = (real) imach[9];
    r__1 = r_lg10(&r__2) * (real) (imach[11] - 1);
    demin = iflr_(&r__1) + 1;
    r__2 = (real) imach[9];
    r__1 = r_lg10(&r__2) * (real) imach[12];
    demax = iceil_(&r__1);
/* Computing MAX */
    i__1 = abs(demin), i__2 = abs(demax);
    r__2 = (real) max(i__1,i__2);
    r__1 = r_lg10(&r__2);
    ewidth = iflr_(&r__1) + 1;
    wwidth = dwidth + ewidth + 6;
/* /6S */
/*     CALL S88FMT( 2, WWIDTH, EFMT(10) ) */
/*     WRITE( STDOUT, 1900 ) ( EFMT(I), I = 9, 14 ) */
/* /7S */
    s88fmt_(&c__2, &wwidth, efmt1 + 9, 1L);
    io___54.ciunit = *stdout;
    s_wsfe(&io___54);
    for (i = 9; i <= 14; ++i) {
	do_fio(&c__1, efmt1 + (i - 1), 1L);
    }
    e_wsfe();
/* / */

/*  NOW WRITE OUT THE SINGLE-PRECISION CONSTANTS */

    io___55.ciunit = *stdout;
    s_wsfe(&io___55);
    e_wsfe();
    io___56.ciunit = *stdout;
    s_wsfe(&io___56);
    do_fio(&c__1, ccplus, 1L);
    do_fio(&c__1, (char *)&rmach[0], (ftnlen)sizeof(real));
    e_wsfe();

    io___57.ciunit = *stdout;
    s_wsfe(&io___57);
    e_wsfe();
    io___58.ciunit = *stdout;
    s_wsfe(&io___58);
    do_fio(&c__1, ccplus, 1L);
    do_fio(&c__1, (char *)&rmach[1], (ftnlen)sizeof(real));
    e_wsfe();

    io___59.ciunit = *stdout;
    s_wsfe(&io___59);
    e_wsfe();
    io___60.ciunit = *stdout;
    s_wsfe(&io___60);
    do_fio(&c__1, ccplus, 1L);
    do_fio(&c__1, (char *)&rmach[2], (ftnlen)sizeof(real));
    e_wsfe();

    io___61.ciunit = *stdout;
    s_wsfe(&io___61);
    e_wsfe();
    io___62.ciunit = *stdout;
    s_wsfe(&io___62);
    do_fio(&c__1, ccplus, 1L);
    do_fio(&c__1, (char *)&rmach[3], (ftnlen)sizeof(real));
    e_wsfe();

    io___63.ciunit = *stdout;
    s_wsfe(&io___63);
    e_wsfe();
    io___64.ciunit = *stdout;
    s_wsfe(&io___64);
    do_fio(&c__1, ccplus, 1L);
    do_fio(&c__1, (char *)&rmach[4], (ftnlen)sizeof(real));
    e_wsfe();
/* /6S */
/*     CALL S88FMT( 2, WWIDTH+1, EFMT(10) ) */
/*     CALL S88FMT( 2, DWIDTH+1, EFMT(13) ) */
/* /7S */
    i__1 = wwidth + 1;
    s88fmt_(&c__2, &i__1, efmt1 + 9, 1L);
    i__1 = dwidth + 1;
    s88fmt_(&c__2, &i__1, efmt1 + 12, 1L);
/* / */
/*  COMPUTE THE NUMBER OF CHARACTER POSITIONS NEEDED FOR WRITING */
/*  OUT A DOUBLE-PRECISION NUMBER ALLOWING FOR ONE SPACE AND */
/*  A SIGN AND PLUG THE FIELDS IN THE FORMAT. */

    r__2 = (real) imach[9];
    r__1 = r_lg10(&r__2) * (real) imach[13];
    dwidth = iceil_(&r__1);
/* /6S */
/*     CALL S88FMT( 2, DWIDTH, DFMT(13) ) */
/* /7S */
    s88fmt_(&c__2, &dwidth, dfmt1 + 12, 1L);
/* / */
    r__2 = (real) imach[9];
    r__1 = r_lg10(&r__2) * (real) (imach[14] - 1);
    demin = iflr_(&r__1) + 1;
    r__2 = (real) imach[9];
    r__1 = r_lg10(&r__2) * (real) imach[15];
    demax = iceil_(&r__1);
/* Computing MAX */
    i__1 = abs(demin), i__2 = abs(demax);
    r__2 = (real) max(i__1,i__2);
    r__1 = r_lg10(&r__2);
    ewidth = iflr_(&r__1) + 1;
    wwidth = dwidth + ewidth + 6;
/* /6S */
/*     CALL S88FMT( 2, WWIDTH, DFMT(10) ) */
/*     WRITE( STDOUT, 2900 ) ( DFMT(I), I = 9, 14 ) */
/* /7S */
    s88fmt_(&c__2, &wwidth, dfmt1 + 9, 1L);
    io___65.ciunit = *stdout;
    s_wsfe(&io___65);
    for (i = 9; i <= 14; ++i) {
	do_fio(&c__1, dfmt1 + (i - 1), 1L);
    }
    e_wsfe();
/* / */

/*  NOW WRITE OUT THE DOUBLE-PRECISION CONSTANTS */

    io___66.ciunit = *stdout;
    s_wsfe(&io___66);
    e_wsfe();
    io___67.ciunit = *stdout;
    s_wsfe(&io___67);
    do_fio(&c__1, ccplus, 1L);
    do_fio(&c__1, (char *)&dmach[0], (ftnlen)sizeof(doublereal));
    e_wsfe();

    io___68.ciunit = *stdout;
    s_wsfe(&io___68);
    e_wsfe();
    io___69.ciunit = *stdout;
    s_wsfe(&io___69);
    do_fio(&c__1, ccplus, 1L);
    do_fio(&c__1, (char *)&dmach[1], (ftnlen)sizeof(doublereal));
    e_wsfe();

    io___70.ciunit = *stdout;
    s_wsfe(&io___70);
    e_wsfe();
    io___71.ciunit = *stdout;
    s_wsfe(&io___71);
    do_fio(&c__1, ccplus, 1L);
    do_fio(&c__1, (char *)&dmach[2], (ftnlen)sizeof(doublereal));
    e_wsfe();

    io___72.ciunit = *stdout;
    s_wsfe(&io___72);
    e_wsfe();
    io___73.ciunit = *stdout;
    s_wsfe(&io___73);
    do_fio(&c__1, ccplus, 1L);
    do_fio(&c__1, (char *)&dmach[3], (ftnlen)sizeof(doublereal));
    e_wsfe();

    io___74.ciunit = *stdout;
    s_wsfe(&io___74);
    e_wsfe();
    io___75.ciunit = *stdout;
    s_wsfe(&io___75);
    do_fio(&c__1, ccplus, 1L);
    do_fio(&c__1, (char *)&dmach[4], (ftnlen)sizeof(doublereal));
    e_wsfe();
/* /6S */
/*     CALL S88FMT( 2, WWIDTH+1, DFMT(10) ) */
/*     CALL S88FMT( 2, DWIDTH+1, DFMT(13) ) */
/* /7S */
    i__1 = wwidth + 1;
    s88fmt_(&c__2, &i__1, dfmt1 + 9, 1L);
    i__1 = dwidth + 1;
    s88fmt_(&c__2, &i__1, dfmt1 + 12, 1L);
/* / */
/*  NOW CHECK CONSISTENCY OF INTEGER CONSTANTS */
/* /6S */
/*     CALL S88FMT( 2, 14, IFMT(5) ) */
/* /7S */
    s88fmt_(&c__2, &c__14, ifmt1 + 4, 1L);
/* / */
    if (imach[10] <= imach[13]) {
	goto L4009;
    }
    io___76.ciunit = *stdout;
    s_wsfe(&io___76);
    e_wsfe();
    io___77.ciunit = *stdout;
    s_wsfe(&io___77);
    e_wsfe();
    io___78.ciunit = *stdout;
    s_wsfe(&io___78);
    do_fio(&c__1, ccplus, 1L);
    do_fio(&c__1, (char *)&imach[10], (ftnlen)sizeof(integer));
    e_wsfe();
    io___79.ciunit = *stdout;
    s_wsfe(&io___79);
    e_wsfe();
    io___80.ciunit = *stdout;
    s_wsfe(&io___80);
    do_fio(&c__1, ccplus, 1L);
    do_fio(&c__1, (char *)&imach[13], (ftnlen)sizeof(integer));
    e_wsfe();
L4009:

    if (imach[12] <= imach[15]) {
	goto L4019;
    }
    io___81.ciunit = *stdout;
    s_wsfe(&io___81);
    e_wsfe();
    io___82.ciunit = *stdout;
    s_wsfe(&io___82);
    e_wsfe();
    io___83.ciunit = *stdout;
    s_wsfe(&io___83);
    do_fio(&c__1, ccplus, 1L);
    do_fio(&c__1, (char *)&imach[12], (ftnlen)sizeof(integer));
    e_wsfe();
    io___84.ciunit = *stdout;
    s_wsfe(&io___84);
    e_wsfe();
    io___85.ciunit = *stdout;
    s_wsfe(&io___85);
    do_fio(&c__1, ccplus, 1L);
    do_fio(&c__1, (char *)&imach[15], (ftnlen)sizeof(integer));
    e_wsfe();
L4019:

    if (imach[15] - imach[14] >= imach[12] - imach[11]) {
	goto L4029;
    }
    io___86.ciunit = *stdout;
    s_wsfe(&io___86);
    e_wsfe();
    io___87.ciunit = *stdout;
    s_wsfe(&io___87);
    e_wsfe();
    io___88.ciunit = *stdout;
    s_wsfe(&io___88);
    e_wsfe();
    io___89.ciunit = *stdout;
    s_wsfe(&io___89);
    do_fio(&c__1, ccplus, 1L);
    do_fio(&c__1, (char *)&imach[11], (ftnlen)sizeof(integer));
    e_wsfe();
    io___90.ciunit = *stdout;
    s_wsfe(&io___90);
    e_wsfe();
    io___91.ciunit = *stdout;
    s_wsfe(&io___91);
    do_fio(&c__1, ccplus, 1L);
    do_fio(&c__1, (char *)&imach[12], (ftnlen)sizeof(integer));
    e_wsfe();
    io___92.ciunit = *stdout;
    s_wsfe(&io___92);
    e_wsfe();
    io___93.ciunit = *stdout;
    s_wsfe(&io___93);
    do_fio(&c__1, ccplus, 1L);
    do_fio(&c__1, (char *)&imach[14], (ftnlen)sizeof(integer));
    e_wsfe();
    io___94.ciunit = *stdout;
    s_wsfe(&io___94);
    e_wsfe();
    io___95.ciunit = *stdout;
    s_wsfe(&io___95);
    do_fio(&c__1, ccplus, 1L);
    do_fio(&c__1, (char *)&imach[15], (ftnlen)sizeof(integer));
    e_wsfe();
L4029:

    n = 0;
    ibasem = imach[6] - 1;
    i__1 = *digint;
    for (i = 1; i <= i__1; ++i) {
	n = n * imach[6] + ibasem;
/* L4030: */
    }

    if (imach[8] == n) {
	goto L4039;
    }
    io___98.ciunit = *stdout;
    s_wsfe(&io___98);
    e_wsfe();
    io___99.ciunit = *stdout;
    s_wsfe(&io___99);
    e_wsfe();
    io___100.ciunit = *stdout;
    s_wsfe(&io___100);
    do_fio(&c__1, ccplus, 1L);
    do_fio(&c__1, (char *)&imach[6], (ftnlen)sizeof(integer));
    e_wsfe();
    io___101.ciunit = *stdout;
    s_wsfe(&io___101);
    e_wsfe();
    io___102.ciunit = *stdout;
    s_wsfe(&io___102);
    do_fio(&c__1, ccplus, 1L);
    do_fio(&c__1, (char *)&imach[7], (ftnlen)sizeof(integer));
    e_wsfe();
    io___103.ciunit = *stdout;
    s_wsfe(&io___103);
    e_wsfe();
    io___104.ciunit = *stdout;
    s_wsfe(&io___104);
    do_fio(&c__1, ccplus, 1L);
    do_fio(&c__1, (char *)&imach[8], (ftnlen)sizeof(integer));
    e_wsfe();
L4039:

/* NOW CHECK CONSISTENCY OF SINGLE-PRECISION CONSTANTS */
/* /6S */
/*     CALL S88FMT( 2, 19, EFMT(5) ) */
/* /7S */
    s88fmt_(&c__2, &c__19, efmt1 + 4, 1L);
/* / */
    i__1 = imach[11] - 1;
    xr = s2mach_(&c_b192, &imach[9], &i__1);
    if (xr == rmach[0]) {
	goto L5009;
    }
    io___106.ciunit = *stdout;
    s_wsfe(&io___106);
    e_wsfe();
    io___107.ciunit = *stdout;
    s_wsfe(&io___107);
    e_wsfe();
    io___108.ciunit = *stdout;
    s_wsfe(&io___108);
    do_fio(&c__1, ccplus, 1L);
    do_fio(&c__1, (char *)&rmach[0], (ftnlen)sizeof(real));
    e_wsfe();
    io___109.ciunit = *stdout;
    s_wsfe(&io___109);
    e_wsfe();
    io___110.ciunit = *stdout;
    s_wsfe(&io___110);
    do_fio(&c__1, ccplus, 1L);
    do_fio(&c__1, (char *)&xr, (ftnlen)sizeof(real));
    e_wsfe();
    io___111.ciunit = *stdout;
    s_wsfe(&io___111);
    e_wsfe();
    xr = rmach[0] - xr;
    io___112.ciunit = *stdout;
    s_wsfe(&io___112);
    do_fio(&c__1, ccplus, 1L);
    do_fio(&c__1, (char *)&xr, (ftnlen)sizeof(real));
    e_wsfe();
L5009:

    xr = (float)0.;
    sbase = (real) imach[9];
    sbasem = (real) (imach[9] - 1);
    i__1 = *digsp;
    for (i = 1; i <= i__1; ++i) {
	xr = (xr + sbasem) / sbase;
/* L5010: */
    }

    xr = s2mach_(&xr, &imach[9], &imach[12]);
    if (xr == rmach[1]) {
	goto L5019;
    }
    io___115.ciunit = *stdout;
    s_wsfe(&io___115);
    e_wsfe();
    io___116.ciunit = *stdout;
    s_wsfe(&io___116);
    e_wsfe();
    io___117.ciunit = *stdout;
    s_wsfe(&io___117);
    do_fio(&c__1, ccplus, 1L);
    do_fio(&c__1, (char *)&rmach[1], (ftnlen)sizeof(real));
    e_wsfe();
    io___118.ciunit = *stdout;
    s_wsfe(&io___118);
    e_wsfe();
    io___119.ciunit = *stdout;
    s_wsfe(&io___119);
    do_fio(&c__1, ccplus, 1L);
    do_fio(&c__1, (char *)&xr, (ftnlen)sizeof(real));
    e_wsfe();
    io___120.ciunit = *stdout;
    s_wsfe(&io___120);
    e_wsfe();
    xr = rmach[1] - xr;
    io___121.ciunit = *stdout;
    s_wsfe(&io___121);
    do_fio(&c__1, ccplus, 1L);
    do_fio(&c__1, (char *)&xr, (ftnlen)sizeof(real));
    e_wsfe();
L5019:

    i__1 = -imach[10];
    xr = s2mach_(&c_b192, &imach[9], &i__1);
    if (xr == rmach[2]) {
	goto L5029;
    }
    io___122.ciunit = *stdout;
    s_wsfe(&io___122);
    e_wsfe();
    io___123.ciunit = *stdout;
    s_wsfe(&io___123);
    e_wsfe();
    io___124.ciunit = *stdout;
    s_wsfe(&io___124);
    do_fio(&c__1, ccplus, 1L);
    do_fio(&c__1, (char *)&rmach[2], (ftnlen)sizeof(real));
    e_wsfe();
    io___125.ciunit = *stdout;
    s_wsfe(&io___125);
    e_wsfe();
    io___126.ciunit = *stdout;
    s_wsfe(&io___126);
    do_fio(&c__1, ccplus, 1L);
    do_fio(&c__1, (char *)&xr, (ftnlen)sizeof(real));
    e_wsfe();
    io___127.ciunit = *stdout;
    s_wsfe(&io___127);
    e_wsfe();
    xr = rmach[2] - xr;
    io___128.ciunit = *stdout;
    s_wsfe(&io___128);
    do_fio(&c__1, ccplus, 1L);
    do_fio(&c__1, (char *)&xr, (ftnlen)sizeof(real));
    e_wsfe();
L5029:

    i__1 = 1 - imach[10];
    xr = s2mach_(&c_b192, &imach[9], &i__1);
    if (xr == rmach[3]) {
	goto L5039;
    }
    io___129.ciunit = *stdout;
    s_wsfe(&io___129);
    e_wsfe();
    io___130.ciunit = *stdout;
    s_wsfe(&io___130);
    e_wsfe();
    io___131.ciunit = *stdout;
    s_wsfe(&io___131);
    do_fio(&c__1, ccplus, 1L);
    do_fio(&c__1, (char *)&rmach[3], (ftnlen)sizeof(real));
    e_wsfe();
    io___132.ciunit = *stdout;
    s_wsfe(&io___132);
    e_wsfe();
    io___133.ciunit = *stdout;
    s_wsfe(&io___133);
    do_fio(&c__1, ccplus, 1L);
    do_fio(&c__1, (char *)&xr, (ftnlen)sizeof(real));
    e_wsfe();
    io___134.ciunit = *stdout;
    s_wsfe(&io___134);
    e_wsfe();
    xr = rmach[3] - xr;
    io___135.ciunit = *stdout;
    s_wsfe(&io___135);
    do_fio(&c__1, ccplus, 1L);
    do_fio(&c__1, (char *)&xr, (ftnlen)sizeof(real));
    e_wsfe();
L5039:

    r__1 = (real) imach[9];
    xr = r_lg10(&r__1);
    if (xr == rmach[4]) {
	goto L5049;
    }
    io___136.ciunit = *stdout;
    s_wsfe(&io___136);
    e_wsfe();
    io___137.ciunit = *stdout;
    s_wsfe(&io___137);
    e_wsfe();
    io___138.ciunit = *stdout;
    s_wsfe(&io___138);
    do_fio(&c__1, ccplus, 1L);
    do_fio(&c__1, (char *)&rmach[4], (ftnlen)sizeof(real));
    e_wsfe();
    io___139.ciunit = *stdout;
    s_wsfe(&io___139);
    e_wsfe();
    io___140.ciunit = *stdout;
    s_wsfe(&io___140);
    do_fio(&c__1, ccplus, 1L);
    do_fio(&c__1, (char *)&xr, (ftnlen)sizeof(real));
    e_wsfe();
    io___141.ciunit = *stdout;
    s_wsfe(&io___141);
    e_wsfe();
    xr = rmach[4] - xr;
    io___142.ciunit = *stdout;
    s_wsfe(&io___142);
    do_fio(&c__1, ccplus, 1L);
    do_fio(&c__1, (char *)&xr, (ftnlen)sizeof(real));
    e_wsfe();
L5049:

/* NOW CHECK CONSISTENCY OF DOUBLE-PRECISION CONSTANTS */
/* /6S */
/*     CALL S88FMT( 2, 19, DFMT(5) ) */
/* /7S */
    s88fmt_(&c__2, &c__19, dfmt1 + 4, 1L);
/* / */
    i__1 = imach[14] - 1;
    xd = s3mach_(&c_b273, &imach[9], &i__1);
    if (xd == dmach[0]) {
	goto L6009;
    }
    io___144.ciunit = *stdout;
    s_wsfe(&io___144);
    e_wsfe();
    io___145.ciunit = *stdout;
    s_wsfe(&io___145);
    e_wsfe();
    io___146.ciunit = *stdout;
    s_wsfe(&io___146);
    do_fio(&c__1, ccplus, 1L);
    do_fio(&c__1, (char *)&dmach[0], (ftnlen)sizeof(doublereal));
    e_wsfe();
    io___147.ciunit = *stdout;
    s_wsfe(&io___147);
    e_wsfe();
    io___148.ciunit = *stdout;
    s_wsfe(&io___148);
    do_fio(&c__1, ccplus, 1L);
    do_fio(&c__1, (char *)&xd, (ftnlen)sizeof(doublereal));
    e_wsfe();
    io___149.ciunit = *stdout;
    s_wsfe(&io___149);
    e_wsfe();
    xd = dmach[0] - xd;
    io___150.ciunit = *stdout;
    s_wsfe(&io___150);
    do_fio(&c__1, ccplus, 1L);
    do_fio(&c__1, (char *)&xd, (ftnlen)sizeof(doublereal));
    e_wsfe();
L6009:

    xd = 0.;
    dbase = (doublereal) ((real) imach[9]);
    dbasem = (doublereal) ((real) (imach[9] - 1));
    i__1 = *digdp;
    for (i = 1; i <= i__1; ++i) {
	xd = (xd + dbasem) / dbase;
/* L6010: */
    }

    xd = s3mach_(&xd, &imach[9], &imach[15]);
    if (xd == dmach[1]) {
	goto L6019;
    }
    io___153.ciunit = *stdout;
    s_wsfe(&io___153);
    e_wsfe();
    io___154.ciunit = *stdout;
    s_wsfe(&io___154);
    e_wsfe();
    io___155.ciunit = *stdout;
    s_wsfe(&io___155);
    do_fio(&c__1, ccplus, 1L);
    do_fio(&c__1, (char *)&dmach[1], (ftnlen)sizeof(doublereal));
    e_wsfe();
    io___156.ciunit = *stdout;
    s_wsfe(&io___156);
    e_wsfe();
    io___157.ciunit = *stdout;
    s_wsfe(&io___157);
    do_fio(&c__1, ccplus, 1L);
    do_fio(&c__1, (char *)&xd, (ftnlen)sizeof(doublereal));
    e_wsfe();
    io___158.ciunit = *stdout;
    s_wsfe(&io___158);
    e_wsfe();
    xd = dmach[1] - xd;
    io___159.ciunit = *stdout;
    s_wsfe(&io___159);
    do_fio(&c__1, ccplus, 1L);
    do_fio(&c__1, (char *)&xd, (ftnlen)sizeof(doublereal));
    e_wsfe();
L6019:

    i__1 = -imach[13];
    xd = s3mach_(&c_b273, &imach[9], &i__1);
    if (xd == dmach[2]) {
	goto L6029;
    }
    io___160.ciunit = *stdout;
    s_wsfe(&io___160);
    e_wsfe();
    io___161.ciunit = *stdout;
    s_wsfe(&io___161);
    e_wsfe();
    io___162.ciunit = *stdout;
    s_wsfe(&io___162);
    do_fio(&c__1, ccplus, 1L);
    do_fio(&c__1, (char *)&dmach[2], (ftnlen)sizeof(doublereal));
    e_wsfe();
    io___163.ciunit = *stdout;
    s_wsfe(&io___163);
    e_wsfe();
    io___164.ciunit = *stdout;
    s_wsfe(&io___164);
    do_fio(&c__1, ccplus, 1L);
    do_fio(&c__1, (char *)&xd, (ftnlen)sizeof(doublereal));
    e_wsfe();
    io___165.ciunit = *stdout;
    s_wsfe(&io___165);
    e_wsfe();
    xd = dmach[2] - xd;
    io___166.ciunit = *stdout;
    s_wsfe(&io___166);
    do_fio(&c__1, ccplus, 1L);
    do_fio(&c__1, (char *)&xd, (ftnlen)sizeof(doublereal));
    e_wsfe();
L6029:

    i__1 = 1 - imach[13];
    xd = s3mach_(&c_b273, &imach[9], &i__1);
    if (xd == dmach[3]) {
	goto L6039;
    }
    io___167.ciunit = *stdout;
    s_wsfe(&io___167);
    e_wsfe();
    io___168.ciunit = *stdout;
    s_wsfe(&io___168);
    e_wsfe();
    io___169.ciunit = *stdout;
    s_wsfe(&io___169);
    do_fio(&c__1, ccplus, 1L);
    do_fio(&c__1, (char *)&dmach[3], (ftnlen)sizeof(doublereal));
    e_wsfe();
    io___170.ciunit = *stdout;
    s_wsfe(&io___170);
    e_wsfe();
    io___171.ciunit = *stdout;
    s_wsfe(&io___171);
    do_fio(&c__1, ccplus, 1L);
    do_fio(&c__1, (char *)&xd, (ftnlen)sizeof(doublereal));
    e_wsfe();
    io___172.ciunit = *stdout;
    s_wsfe(&io___172);
    e_wsfe();
    xd = dmach[3] - xd;
    io___173.ciunit = *stdout;
    s_wsfe(&io___173);
    do_fio(&c__1, ccplus, 1L);
    do_fio(&c__1, (char *)&xd, (ftnlen)sizeof(doublereal));
    e_wsfe();
L6039:

    d__1 = (doublereal) ((real) imach[9]);
    xd = d_lg10(&d__1);
    if (xd == dmach[4]) {
	goto L6049;
    }
    io___174.ciunit = *stdout;
    s_wsfe(&io___174);
    e_wsfe();
    io___175.ciunit = *stdout;
    s_wsfe(&io___175);
    e_wsfe();
    io___176.ciunit = *stdout;
    s_wsfe(&io___176);
    do_fio(&c__1, ccplus, 1L);
    do_fio(&c__1, (char *)&dmach[4], (ftnlen)sizeof(doublereal));
    e_wsfe();
    io___177.ciunit = *stdout;
    s_wsfe(&io___177);
    e_wsfe();
    io___178.ciunit = *stdout;
    s_wsfe(&io___178);
    do_fio(&c__1, ccplus, 1L);
    do_fio(&c__1, (char *)&xd, (ftnlen)sizeof(doublereal));
    e_wsfe();
    io___179.ciunit = *stdout;
    s_wsfe(&io___179);
    e_wsfe();
    xd = dmach[4] - xd;
    io___180.ciunit = *stdout;
    s_wsfe(&io___180);
    do_fio(&c__1, ccplus, 1L);
    do_fio(&c__1, (char *)&xd, (ftnlen)sizeof(doublereal));
    e_wsfe();
L6049:

/* NOW SEE IF SINGLE-PRECISION IS CLOSED UNDER NEGATION */

    xr = -(doublereal)rmach[0];
    xr = -(doublereal)xr;
    if (xr == rmach[0]) {
	goto L7009;
    }
    io___181.ciunit = *stdout;
    s_wsfe(&io___181);
    e_wsfe();
    io___182.ciunit = *stdout;
    s_wsfe(&io___182);
    e_wsfe();
    io___183.ciunit = *stdout;
    s_wsfe(&io___183);
    do_fio(&c__1, ccplus, 1L);
    do_fio(&c__1, (char *)&rmach[0], (ftnlen)sizeof(real));
    e_wsfe();
    io___184.ciunit = *stdout;
    s_wsfe(&io___184);
    e_wsfe();
    io___185.ciunit = *stdout;
    s_wsfe(&io___185);
    do_fio(&c__1, ccplus, 1L);
    do_fio(&c__1, (char *)&xr, (ftnlen)sizeof(real));
    e_wsfe();
L7009:

    xr = -(doublereal)rmach[1];
    xr = -(doublereal)xr;
    if (xr == rmach[1]) {
	goto L7019;
    }
    io___186.ciunit = *stdout;
    s_wsfe(&io___186);
    e_wsfe();
    io___187.ciunit = *stdout;
    s_wsfe(&io___187);
    e_wsfe();
    io___188.ciunit = *stdout;
    s_wsfe(&io___188);
    do_fio(&c__1, ccplus, 1L);
    do_fio(&c__1, (char *)&rmach[1], (ftnlen)sizeof(real));
    e_wsfe();
    io___189.ciunit = *stdout;
    s_wsfe(&io___189);
    e_wsfe();
    io___190.ciunit = *stdout;
    s_wsfe(&io___190);
    do_fio(&c__1, ccplus, 1L);
    do_fio(&c__1, (char *)&xr, (ftnlen)sizeof(real));
    e_wsfe();
L7019:

/* NOW SEE IF DOUBLE-PRECISION IS CLOSED UNDER NEGATION */

    xd = -dmach[0];
    xd = -xd;
    if (xd == dmach[0]) {
	goto L8009;
    }
    io___191.ciunit = *stdout;
    s_wsfe(&io___191);
    e_wsfe();
    io___192.ciunit = *stdout;
    s_wsfe(&io___192);
    e_wsfe();
    io___193.ciunit = *stdout;
    s_wsfe(&io___193);
    do_fio(&c__1, ccplus, 1L);
    do_fio(&c__1, (char *)&dmach[0], (ftnlen)sizeof(doublereal));
    e_wsfe();
    io___194.ciunit = *stdout;
    s_wsfe(&io___194);
    e_wsfe();
    io___195.ciunit = *stdout;
    s_wsfe(&io___195);
    do_fio(&c__1, ccplus, 1L);
    do_fio(&c__1, (char *)&xd, (ftnlen)sizeof(doublereal));
    e_wsfe();
L8009:

    xd = -dmach[1];
    xd = -xd;
    if (xd == dmach[1]) {
	goto L8019;
    }
    io___196.ciunit = *stdout;
    s_wsfe(&io___196);
    e_wsfe();
    io___197.ciunit = *stdout;
    s_wsfe(&io___197);
    e_wsfe();
    io___198.ciunit = *stdout;
    s_wsfe(&io___198);
    do_fio(&c__1, ccplus, 1L);
    do_fio(&c__1, (char *)&dmach[1], (ftnlen)sizeof(doublereal));
    e_wsfe();
    io___199.ciunit = *stdout;
    s_wsfe(&io___199);
    e_wsfe();
    io___200.ciunit = *stdout;
    s_wsfe(&io___200);
    do_fio(&c__1, ccplus, 1L);
    do_fio(&c__1, (char *)&xd, (ftnlen)sizeof(doublereal));
    e_wsfe();
L8019:

/* CHECK THAT SQRT AND DSQRT WORK NEAR OVER- AND UNDERFLOW LIMITS. */

    n = imach[10] / 2 + 1;
    xr = sqrt(rmach[0]);
    if (xr > (float)0.) {
	goto L9002;
    }
    io___201.ciunit = *stdout;
    s_wsfe(&io___201);
    e_wsfe();
    io___202.ciunit = *stdout;
    s_wsfe(&io___202);
    do_fio(&c__1, ccplus, 1L);
    do_fio(&c__1, (char *)&xr, (ftnlen)sizeof(real));
    e_wsfe();
    goto L9004;
/* SCALE TO AVOID TROUBLE FROM UNDERFLOW... */
L9002:
    xr = s2mach_(&xr, &imach[9], &n);
    i__1 = n << 1;
    yr = s2mach_(rmach, &imach[9], &i__1);
    yr = (r__1 = xr * xr - yr, dabs(r__1)) / yr;
    if (yr < rmach[3] * (float)2.) {
	goto L9004;
    }
    io___204.ciunit = *stdout;
    s_wsfe(&io___204);
    e_wsfe();
    io___205.ciunit = *stdout;
    s_wsfe(&io___205);
    do_fio(&c__1, ccplus, 1L);
    do_fio(&c__1, (char *)&yr, (ftnlen)sizeof(real));
    e_wsfe();
L9004:
    xr = sqrt(rmach[1]);
    if (xr > (float)0.) {
	goto L9006;
    }
    io___206.ciunit = *stdout;
    s_wsfe(&io___206);
    e_wsfe();
    io___207.ciunit = *stdout;
    s_wsfe(&io___207);
    do_fio(&c__1, ccplus, 1L);
    do_fio(&c__1, (char *)&xr, (ftnlen)sizeof(real));
    e_wsfe();
    goto L9008;
/* SCALE TO AVOID TROUBLE FROM OVERFLOW... */
L9006:
    i__1 = -n;
    xr = s2mach_(&xr, &imach[9], &i__1);
    i__1 = n * -2;
    yr = s2mach_(&rmach[1], &imach[9], &i__1);
    yr = (r__1 = xr * xr - yr, dabs(r__1)) / yr;
    if (yr < rmach[3] * (float)2.) {
	goto L9008;
    }
    io___208.ciunit = *stdout;
    s_wsfe(&io___208);
    e_wsfe();
    io___209.ciunit = *stdout;
    s_wsfe(&io___209);
    do_fio(&c__1, ccplus, 1L);
    do_fio(&c__1, (char *)&yr, (ftnlen)sizeof(real));
    e_wsfe();

L9008:
    n = imach[13] / 2 + 1;
    xd = sqrt(dmach[0]);
    if (xd > 0.) {
	goto L9010;
    }
    io___210.ciunit = *stdout;
    s_wsfe(&io___210);
    e_wsfe();
    io___211.ciunit = *stdout;
    s_wsfe(&io___211);
    do_fio(&c__1, ccplus, 1L);
    do_fio(&c__1, (char *)&xd, (ftnlen)sizeof(doublereal));
    e_wsfe();
    goto L9012;
/* AGAIN SCALE TO AVOID TROUBLE FROM UNDERFLOW... */
L9010:
    xd = s3mach_(&xd, &imach[9], &n);
    i__1 = n << 1;
    yd = s3mach_(dmach, &imach[9], &i__1);
    yd = (d__1 = xd * xd - yd, abs(d__1)) / yd;
    if (yd < dmach[3] * 2.) {
	goto L9012;
    }
    io___213.ciunit = *stdout;
    s_wsfe(&io___213);
    e_wsfe();
    io___214.ciunit = *stdout;
    s_wsfe(&io___214);
    do_fio(&c__1, ccplus, 1L);
    do_fio(&c__1, (char *)&yd, (ftnlen)sizeof(doublereal));
    e_wsfe();
L9012:
    xd = sqrt(dmach[1]);
    if (xd > 0.) {
	goto L9014;
    }
    io___215.ciunit = *stdout;
    s_wsfe(&io___215);
    e_wsfe();
    io___216.ciunit = *stdout;
    s_wsfe(&io___216);
    do_fio(&c__1, ccplus, 1L);
    do_fio(&c__1, (char *)&xd, (ftnlen)sizeof(doublereal));
    e_wsfe();
    goto L9016;
/* AGAIN SCALE TO AVOID TROUBLE FROM OVERFLOW... */
L9014:
    i__1 = -n;
    xd = s3mach_(&xd, &imach[9], &i__1);
    i__1 = n * -2;
    yd = s3mach_(&dmach[1], &imach[9], &i__1);
    yd = (d__1 = xd * xd - yd, abs(d__1)) / yd;
    if (yd < dmach[3] * 2.) {
	goto L9016;
    }
    io___217.ciunit = *stdout;
    s_wsfe(&io___217);
    e_wsfe();
    io___218.ciunit = *stdout;
    s_wsfe(&io___218);
    do_fio(&c__1, ccplus, 1L);
    do_fio(&c__1, (char *)&yd, (ftnlen)sizeof(doublereal));
    e_wsfe();
L9016:
    return 0;

} /* s1mach_ */

#undef stdout
#undef digint
#undef digsp
#undef digdp
#undef imach
#undef ifmt1
#undef efmt1
#undef dfmt1
#undef ifmt
#undef efmt
#undef dfmt


