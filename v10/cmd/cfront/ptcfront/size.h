/*ident	"@(#)ctrans:src/size.h	1.2" */
/*************************************************************************

	C++ source for cfront, the C++ compiler front-end
	written in the computer science research center of Bell Labs

	Copyright (c) 1984 AT&T, Inc. All rigths Reserved
	THIS IS UNPUBLISHED PROPRIETARY SOURCE CODE OF AT&T, INC.

size.h:
	sizes and alignments used to calculate sizeofs

	table and butffer sizes

***************************************************************************/
#ifndef GRAM
extern BI_IN_WORD;
extern BI_IN_BYTE;
				/*	byte sizes */
extern SZ_CHAR;
extern AL_CHAR;

extern SZ_SHORT;
extern AL_SHORT;

extern SZ_INT;
extern AL_INT;

extern SZ_LONG;
extern AL_LONG;

extern SZ_FLOAT;
extern AL_FLOAT;

extern SZ_DOUBLE;
extern AL_DOUBLE;

extern SZ_LDOUBLE;
extern AL_LDOUBLE;

extern SZ_STRUCT;	/* minimum struct size */
extern AL_STRUCT;

//extern SZ_FRAME;
//extern AL_FRAME;

extern SZ_WORD;

extern SZ_WPTR;
extern AL_WPTR;

extern SZ_BPTR;
extern AL_BPTR;	

//extern SZ_TOP;
//extern SZ_BOTTOM;

extern char* LARGEST_INT;
extern int F_SENSITIVE;	// is field alignment sensitive to the type of the field?
extern int F_OPTIMIZED;	// can the compiler fit a small int field into a char?
#endif
				// default sizes:
	// Note: #if doesn't work on all systems
#ifdef u3b
#define Abbb
#endif
#ifdef u3b2
#define Abbb
#endif
#ifdef u3b5
#define Abbb
#endif
#ifdef u3b15
#define Abbb
#endif
#ifdef pyr
#define Abbb
#endif


#ifdef alliant
#define Am68
#endif
#ifdef apollo
#define Am68
#endif
#if defined(sun2) || defined(mc68010)
#define Am68
#endif
#if defined(sun3) || defined(mc68020)
#define Am68
#endif
#if defined(mc68k) || defined(hp9000s200) || defined(hp9000s300)
#define Am68
#endif

#ifdef iAPX286
#ifdef LARGE
#define Ai286l
#endif
#endif

#ifdef Abbb
				/* AT&T 3Bs */
#define DBI_IN_WORD 32
#define DBI_IN_BYTE 8
#define DSZ_CHAR 1
#define DAL_CHAR 1
#define DSZ_SHORT 2
#define DAL_SHORT 2
#define DSZ_INT 4
#define DAL_INT 4
#define DSZ_LONG 4
#define DAL_LONG 4
#define DSZ_FLOAT 4
#define DAL_FLOAT 4
#define DSZ_DOUBLE 8
#define DAL_DOUBLE 4
#define DSZ_LDOUBLE 8
#define DAL_LDOUBLE 4
#define DSZ_STRUCT 4
#define DAL_STRUCT 4
//#define DSZ_FRAME 4
//#define DAL_FRAME 4
#define DSZ_WORD 4
#define DSZ_WPTR 4
#define DAL_WPTR 4
#define DSZ_BPTR 4
#define DAL_BPTR 4
//#define DSZ_TOP 0
//#define DSZ_BOTTOM 0
#define DLARGEST_INT "2147483647"	/* 2**31 - 1 */
#define DF_SENSITIVE 0
#define DF_OPTIMIZED 1
#else
#ifdef Am68
				/* most M68K boxes */
#if defined(hp9000s200) || defined(hp9000s300)
#define DBI_IN_WORD 32 
#else
#define DBI_IN_WORD 16
#endif
#define DBI_IN_BYTE 8
#define DSZ_CHAR 1
#define DAL_CHAR 1
#define DSZ_SHORT 2
#define DAL_SHORT 2
#define DSZ_INT 4
#define DAL_INT 2
#define DSZ_LONG 4
#define DAL_LONG 2
#define DSZ_FLOAT 4
#define DAL_FLOAT 2
#define DSZ_DOUBLE 8
#define DAL_DOUBLE 2
#define DSZ_LDOUBLE 8
#define DAL_LDOUBLE 2
#define DSZ_STRUCT 2
#define DAL_STRUCT 2
//#define DSZ_FRAME 4
//#define DAL_FRAME 4
#define DSZ_WORD 2
#define DSZ_WPTR 4
#define DAL_WPTR 2
#define DSZ_BPTR 4
#define DAL_BPTR 2
//#define DSZ_TOP 0
//#define DSZ_BOTTOM 0
#define DLARGEST_INT "2147483647"	/* 2**31 - 1 */
#define DF_SENSITIVE 0
#define DF_OPTIMIZED 1
#else
#ifdef Ai286l
				/* Intel 80286 large model */
#define DBI_IN_WORD 16
#define DBI_IN_BYTE 8
#define DSZ_CHAR 1
#define DAL_CHAR 1
#define DSZ_SHORT 2
#define DAL_SHORT 2
#define DSZ_INT 2
#define DAL_INT 2
#define DSZ_LONG 4
#define DAL_LONG 2
#define DSZ_FLOAT 4
#define DAL_FLOAT 2
#define DSZ_DOUBLE 8
#define DAL_DOUBLE 2
#define DSZ_LDOUBLE 8
#define DAL_LDOUBLE 2
#define DSZ_STRUCT 2
#define DAL_STRUCT 2
//#define DSZ_FRAME 4
//#define DAL_FRAME 4
#define DSZ_WORD 2
#define DSZ_WPTR 4
#define DAL_WPTR 2
#define DSZ_BPTR 4
#define DAL_BPTR 2
//#define DSZ_TOP 0
//#define DSZ_BOTTOM 0
#define DLARGEST_INT "32767"	/* 2**15 - 1 */
#define DF_SENSITIVE 0
#define DF_OPTIMIZED 1
#else
#if defined(uts) || defined(hp9000s800) || defined(sun4) || defined(sparc)
				/* Amdahl running UTS, HP RISC */
#define DBI_IN_WORD 32
#define DBI_IN_BYTE 8
#define DSZ_CHAR 1
#define DAL_CHAR 1
#define DSZ_SHORT 2
#define DAL_SHORT 2
#define DSZ_INT 4
#define DAL_INT 4
#define DSZ_LONG 4
#define DAL_LONG 4
#define DSZ_FLOAT 4
#define DAL_FLOAT 4
#define DSZ_DOUBLE 8
#define DAL_DOUBLE 8
#define DSZ_LDOUBLE 8
#define DAL_LDOUBLE 8
#define DSZ_STRUCT 1
#define DAL_STRUCT 1
#define DSZ_WORD 4
#define DSZ_WPTR 4
#define DAL_WPTR 4
#define DSZ_BPTR 4
#define DAL_BPTR 4
#define DLARGEST_INT "2147483647"	/* 2**31 - 1 */
#if defined(uts)
#define DF_SENSITIVE 1
#else
#define DF_SENSITIVE 0
#endif
#define DF_OPTIMIZED 1
#else
#ifdef hpux 
                                /* hp */
#define DBI_IN_WORD 32
#define DBI_IN_BYTE 8
#define DSZ_CHAR 1
#define DAL_CHAR 1
#define DSZ_SHORT 2
#define DAL_SHORT 2
#define DSZ_INT 4
#define DAL_INT 4
#define DSZ_LONG 4
#define DAL_LONG 4
#define DSZ_FLOAT 4
#define DAL_FLOAT 4
#define DSZ_DOUBLE 8
#define DAL_DOUBLE 8 
#define DSZ_LDOUBLE 8
#define DAL_LDOUBLE 8
#define DSZ_STRUCT 1 
#define DAL_STRUCT 1 
#define DSZ_WORD 4
#define DSZ_WPTR 4
#define DAL_WPTR 4
#define DSZ_BPTR 4
#define DAL_BPTR 4
#define DLARGEST_INT "2147483647"       /* 2**31 - 1 */
#define DF_SENSITIVE 1 
#define DF_OPTIMIZED 1
#else
#if defined(vax) || defined(ibm032) || defined(i386)
				/* VAX, IBM 32, Intel 386 */
#define DBI_IN_WORD 32
#define DBI_IN_BYTE 8
#define DSZ_CHAR 1
#define DAL_CHAR 1
#define DSZ_SHORT 2
#define DAL_SHORT 2
#define DSZ_INT 4
#define DAL_INT 4
#define DSZ_LONG 4
#define DAL_LONG 4
#define DSZ_FLOAT 4
#define DAL_FLOAT 4
#define DSZ_DOUBLE 8
#define DAL_DOUBLE 4
#define DSZ_LDOUBLE 8
#define DAL_LDOUBLE 4
#define DSZ_STRUCT 1
#define DAL_STRUCT 1
//#define DSZ_FRAME 4
//#define DAL_FRAME 4
#define DSZ_WORD 4
#define DSZ_WPTR 4
#define DAL_WPTR 4
#define DSZ_BPTR 4
#define DAL_BPTR 4
//#define DSZ_TOP 0
//#define DSZ_BOTTOM 0
#define DLARGEST_INT "2147483647"	/* 2**31 - 1 */
#define DF_SENSITIVE 0
#define DF_OPTIMIZED 1
#else
#ifdef mc300
#define DBI_IN_WORD 32
#define DBI_IN_BYTE 8
#define DSZ_CHAR 1
#define DAL_CHAR 1
#define DSZ_SHORT 2
#define DAL_SHORT 2
#define DSZ_INT 4
#define DAL_INT 4
#define DSZ_LONG 4
#define DAL_LONG 4
#define DSZ_FLOAT 4
#define DAL_FLOAT 4
#define DSZ_DOUBLE 8
#define DAL_DOUBLE 4
#define DSZ_LDOUBLE 8
#define DAL_LDOUBLE 4
#define DSZ_STRUCT 2
#define DAL_STRUCT 2
//#define DSZ_FRAME 4
//#define DAL_FRAME 4
#define DSZ_WORD 4
#define DSZ_WPTR 4
#define DAL_WPTR 4
#define DSZ_BPTR 4
#define DAL_BPTR 4
//#define DSZ_TOP 0
//#define DSZ_BOTTOM 0
#define DLARGEST_INT "2147483647"       /* 2**31 - 1 */
#define DF_SENSITIVE 0
#define DF_OPTIMIZED 1
#else
#ifdef mips
#define DBI_IN_BYTE 8
#define DBI_IN_WORD 32
#define DSZ_WORD 4
#define DSZ_CHAR 1
#define DAL_CHAR 1
#define DSZ_SHORT 2
#define DAL_SHORT 2
#define DSZ_INT 4
#define DAL_INT 4
#define DLARGEST_INT "2147483647"
#define DSZ_LONG 4
#define DAL_LONG 4
#define DSZ_FLOAT 4
#define DAL_FLOAT 4
#define DSZ_DOUBLE 8
#define DAL_DOUBLE 8
#define DSZ_LDOUBLE 8 
#define DAL_LDOUBLE 8 
#define DSZ_BPTR 4
#define DAL_BPTR 4
#define DSZ_WPTR 4
#define DAL_WPTR 4
#define DSZ_STRUCT 1
#define DAL_STRUCT 1
#define DF_SENSITIVE 0
#define DF_OPTIMIZED 1
#else
				/* defaults: 0 => error */
#define DBI_IN_WORD 0
#define DBI_IN_BYTE 0
#define DSZ_CHAR 1
#define DAL_CHAR 1
#define DSZ_SHORT 0
#define DAL_SHORT 0
#define DSZ_INT 0
#define DAL_INT 0
#define DSZ_LONG 0
#define DAL_LONG 0
#define DSZ_FLOAT 0
#define DAL_FLOAT 0
#define DSZ_DOUBLE 0
#define DAL_DOUBLE 0
#define DSZ_LDOUBLE 0
#define DAL_LDOUBLE 0
#define DSZ_STRUCT 0
#define DAL_STRUCT 0
#define DSZ_WORD 0
#define DSZ_WPTR 0
#define DAL_WPTR 0
#define DSZ_BPTR 0
#define DAL_BPTR 0
#define DLARGEST_INT "0"
#define DF_SENSITIVE 0
#define DF_OPTIMIZED 0
#endif
#endif
#endif
#endif
#endif
#endif
#endif
#endif

#define KTBLSIZE	123	/*	initial keyword table size */
#define GTBLSIZE	257	/*	initial global name table size */
#define CTBLSIZE	12	/*	initial class table size */
#define TBLSIZE		20	/*	initial block table size */
#define BLMAX		50	/*	max block nesting */
#define MAXFILE		127	/*	max include file nesting */
#define MAXERR		13	/* maximum number of errors before terminating */

#ifndef GRAM
const CHUNK = 8*1024;
void* chunk(int);
#endif
