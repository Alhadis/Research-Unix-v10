/*
/   This header file defines the interface between the Macro SPITBOL compiler
/   written in assembly langauge and the OS interface written in C.
/   Communication between the two is handled via a set of global variables 
/   defined as externals below.
*/

/*
/   Set up externals for all the compiler's registers.
*/

extern float	reg_ra;
extern long	reg_cp, reg_ia, reg_wa, reg_wb, reg_wc, reg_xr, reg_xl;

/*
/   Macros to fetch a value of appropriate type from a compiler register
*/

#define RA(type)	((type) reg_ra)
#define CP(type)	((type) reg_cp)
#define IA(type)	((type) reg_ia)
#define WA(type)	((type) reg_wa)
#define WB(type)	((type) reg_wb)
#define WC(type)	((type) reg_wc)
#define XR(type)	((type) reg_xr)
#define XL(type)	((type) reg_xl)

/*
/   Macros to set a value of appropriate type into a compiler register.
*/

#define SET_RA(val)	reg_ra = (float) (val)
#define SET_CP(val)	reg_cp = (long) (val)
#define SET_IA(val)	reg_ia = (long) (val)
#define SET_WA(val)	reg_wa = (long) (val)
#define SET_WB(val)	reg_wb = (long) (val)
#define SET_WC(val)	reg_wc = (long) (val)
#define SET_XR(val)	reg_xr = (long) (val)
#define SET_XL(val)	reg_xl = (long) (val)

/*
/   Return values to take exit N from interface
*/

#define EXIT_1		0
#define EXIT_2		4
#define EXIT_3		8
#define EXIT_4		12
#define EXIT_5		16
#define EXIT_6		20
#define EXIT_7		24
#define EXIT_8		28
#define EXIT_9		32

/*
/    Return value to do a normal return from interface.
*/

#define NORMAL_RETURN	(-1)

/*
/   Define length of temporary string in tscblk.  (Must be coordinated with
/   vaxinter.s.
*/

#define TSCBLK_LENGTH	120
