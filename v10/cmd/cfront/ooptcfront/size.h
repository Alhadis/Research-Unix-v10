/*ident	"@(#)ctrans:src/size.h	1.1.9.6" */
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

/* $Log:	size.h,v $
 * Revision 1.1  89/11/20  08:50:54  benson
 * Initial revision
 * 
 * Revision 1.3  89/10/26  16:30:55  benson
 * move all the initialization #ifdef's into here so they
 * can be changed without recompiling the whole bloody compiler.
 * Add ZB_BOUNDARY to represent the start of a bitfield 32 bit
 * throw as distinct from the "word", which is 32 bits on a sun.
 * 
 * Revision 1.2  89/06/28  12:59:47  benson
 * Port of changes from 2.0beta6 odi bugfixes
 * 
   end_log

*/


