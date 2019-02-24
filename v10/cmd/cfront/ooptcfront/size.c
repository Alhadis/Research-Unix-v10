/*ident	"@(#)ctrans:src/size.c	1.1.4.3" */
/*********************************************************************

	C++ source for cfront, the C++ compiler front-end
	written in the computer science research center of Bell Labs

	Copyright (c) 1984 AT&T, Inc. All rigths Reserved
	THIS IS UNPUBLISHED PROPRIETARY SOURCE CODE OF AT&T, INC.

size.c:

	initialize alignment and sizeof "constants"

**********************************************************************/

#include "cfront.h"
#include "size.h"

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
#if defined(hp9000s200) || defined(hp9000s300) || defined(sun)
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
#ifdef sun
#define DSZ_WORD 4
#define DZB_BOUNDARY 2
#else
#define DSZ_WORD 2
#define DZB_BOUNDARY 2
#endif
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
#define DF_SENSITIVE 0
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

#ifndef DZB_BOUNDARY
// #define ZB_BOUNDARY DSZ_WORD
#define DZB_BOUNDARY DSZ_WORD
#endif

int BI_IN_WORD = DBI_IN_WORD;
int BI_IN_BYTE = DBI_IN_BYTE;
int SZ_CHAR = DSZ_CHAR;
int AL_CHAR = DAL_CHAR;
int SZ_SHORT = DSZ_SHORT;
int AL_SHORT = DAL_SHORT;
int SZ_INT = DSZ_INT;
int AL_INT = DAL_INT;
int SZ_LONG = DSZ_LONG;
int AL_LONG = DAL_LONG;
int SZ_FLOAT = DSZ_FLOAT;
int AL_FLOAT = DAL_FLOAT;
int SZ_DOUBLE = DSZ_DOUBLE;
int AL_DOUBLE = DAL_DOUBLE;
int SZ_LDOUBLE = DSZ_LDOUBLE;
int AL_LDOUBLE = DAL_LDOUBLE;
int SZ_STRUCT = DSZ_STRUCT;
int AL_STRUCT = DAL_STRUCT;
//int SZ_FRAME = DSZ_FRAME;
//int AL_FRAME = DAL_FRAME;
int SZ_WORD = DSZ_WORD;
int ZB_BOUNDARY = DZB_BOUNDARY;
int SZ_WPTR = DSZ_WPTR;
int AL_WPTR = DAL_WPTR;
int SZ_BPTR = DSZ_BPTR;
int AL_BPTR = DAL_BPTR;
//int SZ_TOP = DSZ_TOP;
//int SZ_BOTTOM = DSZ_BOTTOM;
char* LARGEST_INT = DLARGEST_INT;
int F_SENSITIVE = DF_SENSITIVE;
int F_OPTIMIZED =  DF_OPTIMIZED;

int arg1 = 0;
int arg2 = 0;

int get_line(FILE* fp)
{
	char s[32];

	if (fscanf(fp," %s %d %d",s,&arg1,&arg2) == EOF) return 0;
// fprintf(stderr,"xxx %s %d %d",s,arg1,arg2);
	if (strcmp("char",s) == 0) {
		SZ_CHAR = arg1;
		AL_CHAR = arg2;
		return 1;
	}
	if (strcmp("short",s) == 0) {
		SZ_SHORT = arg1;
		AL_SHORT = arg2;
		return 1;
	}
	if (strcmp("int",s) == 0) {
		SZ_INT = arg1;
		AL_INT = arg2;
		if (fscanf(fp," %s",s) == EOF) return 0;
		LARGEST_INT = new char[strlen(s)+1];
		strcpy(LARGEST_INT,s);
		return 1;
	}
	if (strcmp("long",s) == 0) {
		SZ_LONG = arg1;
		AL_LONG = arg2;
		return 1;
	}
	if (strcmp("float",s) == 0) {
		SZ_FLOAT = arg1;
		AL_FLOAT = arg2;
		return 1;
	}
	if (strcmp("double",s) == 0) {
		SZ_DOUBLE = arg1;
		AL_DOUBLE = arg2;
		return 1;
	}
	if (strcmp("ldouble",s) == 0) {
		SZ_LDOUBLE = arg1;
		AL_LDOUBLE = arg2;
		return 1;
	}
	if (strcmp("bit",s) == 0) {
		BI_IN_BYTE = arg1;
		BI_IN_WORD = arg2;
		return 1;
	}
	if (strcmp("struct",s) == 0) {
		SZ_STRUCT = arg1;
		AL_STRUCT = arg2;
		return 1;
	}
	if (strcmp("struct2",s) == 0) {
		F_SENSITIVE = arg1;
		F_OPTIMIZED = arg2;
		return 1;
	}
	if (strcmp("frame",s) == 0) {
//		SZ_FRAME = arg1;
//		AL_FRAME = arg2;
		return 1;
	}
	if (strcmp("word",s) == 0) {
		SZ_WORD = arg1;
		return 1;
	}
	if (strcmp("zero_bitfield_boundary",s) == 0) {
		ZB_BOUNDARY = arg1;
		return 1;
	}
	if (strcmp("wptr",s) == 0) {
		SZ_WPTR = arg1;
		AL_WPTR = arg2;
		return 1;
	}
	if (strcmp("bptr",s) == 0) {
		SZ_BPTR = arg1;
		AL_BPTR = arg2;
		return 1;
	}
	if (strcmp("top",s) == 0) {
//		SZ_TOP = arg1;
//		SZ_BOTTOM = arg2;
		return 1;
	}
	return 0;
}

int read_align(char* f)
{
	char* p = f;
	if (*p == 0) {
		fprintf(stderr,"size/align file missing\n");
		ext(1);
	}
	FILE* fp = fopen(f,"r");
	if (fp == 0) return 1;
	while (get_line(fp)) ;
	return 0;
}
/*
print_align(char* s)
{
	fprintf(stderr,"%s sizes and alignments\n\n",s);

	fprintf(stderr,"	size	align	largest\n");
	fprintf(stderr,"char	%d	%d\n",SZ_CHAR,AL_CHAR);
	fprintf(stderr,"short	%d	%d\n",SZ_SHORT,AL_SHORT);
	fprintf(stderr,"int	%d	%d	%s\n",SZ_INT,AL_INT,LARGEST_INT);
	fprintf(stderr,"long	%d	%d\n",SZ_LONG,AL_LONG);
	fprintf(stderr,"float	%d	%d\n",SZ_FLOAT,AL_FLOAT);
	fprintf(stderr,"double	%d	%d\n",SZ_DOUBLE,AL_DOUBLE);
	fprintf(stderr,"ldouble	%d	%d\n",SZ_LDOUBLE,AL_LDOUBLE);
	fprintf(stderr,"bptr	%d	%d\n",SZ_BPTR,AL_BPTR);
	fprintf(stderr,"wptr	%d	%d\n",SZ_WPTR,AL_WPTR);
	fprintf(stderr,"struct	%d	%d\n",SZ_STRUCT,AL_STRUCT);
	fprintf(stderr,"struct2	%d	%d\n",F_SENSITIVE,F_OPTIMIZED);
//	fprintf(stderr,"frame	%d	%d\n",SZ_FRAME,AL_FRAME);

	fprintf(stderr,"%d bits in a byte, %d bits in a word, %d bytes in a word zero bitfield aligned to %d bytes\n",
			BI_IN_BYTE, BI_IN_WORD, SZ_WORD, ZB_BOUNDARY);
	return 1;
}
*/

int c_strlen(const char* s)
/*
	return sizeof(s) with escapes processed
	sizeof("") == 1		the terminating 0
	sizeof("a") == 2
	sizeof("\0x") == 3	0 x 0
	sizeof("\012") == 2	'\012'
	sizeof("\01")		'\001'
	sizeof("\x") == 2 	\ ignored

*/
{
	int i = 1;
        for (const char* p = s; *p; i++,p++) {
		if (*p == '\\')	{					// '\?
			switch (*++p) {
			case '0':
				switch (p[1]) {		// '\01' or '\012'
				case '0': case '1': case '2': case '3':
				case '4': case '5': case '6': case '7':
					break;
				default:
					continue;	// '\0'
				}
				/* no break */
			case '1': case '2': case '3':
			case '4': case '5': case '6': case '7':		// '\123'
				switch (*++p) {
				case '0': case '1': case '2': case '3':
				case '4': case '5': case '6': case '7':
					switch (*++p) {
					case '0': case '1': case '2': case '3':
					case '4': case '5': case '6': case '7':
						break;
					default:
						--p;
					}
					break;
				default:
					--p;
				}
			}
		}
	}
	return i;
}
/* ODI notes --

 * move all the initialization #ifdef's into here so they
 * can be changed without recompiling the whole compiler.
 * Add ZB_BOUNDARY to represent the start of a bitfield 32 bit
 * throw as distinct from the "word", which is 32 bits on a sun.
 * 

*/
