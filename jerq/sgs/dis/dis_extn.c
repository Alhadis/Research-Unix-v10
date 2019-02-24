/*
 *	static	char	dis_extn_ID[] = "@(#)dis_extn.c	1.7 11/11/83";
 */
#include	<stdio.h>
#include	"dis.h"
#include	"scnhdr.h"
#include	"filehdr.h"
#include	"ldfcn.h"
#include	"linenum.h"
#include	"syms.h"
#include	"structs.h"
/*
 *	This file contains those global variables that are used in more
 *	than one source file. This file is meant to be an
 *	aid in keeping track of the variables used.  Note that in the
 *	other source files, the global variables used are declared as
 *	'static' to make sure they will not be referenced outside of the
 *	containing source file.
 */

unsigned short	curbyte;	/* for storing the results of 'getbyte()' */
unsigned short	cur1byte;	/* for storing the results of 'get1byte()' */
unsigned short	cur2bytes;	/* for storing the results of 'get2bytes()' */
#ifdef AR32WR
	unsigned long	cur4bytes;		/* for storing the results of 'get4bytes()' */
#else
	long	cur4bytes;
#endif
char	bytebuf[4];

/* aflag, oflag, trace, Lflag, and lflag are flags which are set to
 * 1 if specified by the user as options for the disassembly run.
 *
 * aflag	(set by the -da option) indicates that when disassembling
 *		a section as data, the actual address of the data should be 
 *		printed rather than the offset within the data section.
 * oflag	indicates that output is to be in octal rather than hex.
 * trace	is for debugging of the disassembler itself.
 * Lflag	is for looking up labels.
 * lflag	is for disassembling a named library.
 * fflag	is for disassembling named functions.
 *		fflag is incremented for each named function on the command line.
 * ffunction	contains information about each named function
 */

int	oflag = 0;
int	trace = 0;
int	Lflag = 0;
int	lflag = 0;
short	aflag = 0;
#ifndef OLIST
int	fflag = 0;
NFUNC	ffunction[FFUNC];
#endif

long	 loc;		/* byte location in section being disassembled	*/
			/* IMPORTANT: remember that loc is incremented	*/
			/* only by the getbyte routine			*/
char	object[NHEX];	/* array to store object code for output	*/
char	mneu[NLINE];	/* array to store mnemonic code for output	*/
#if OLIST
char	linebuf[NNUM];	/* array to store line numbers for output	*/
#endif

/*
 *	These pointers to structures are for each readpoint desired in the
 *	object file for disassembly purposes.  Initially the file
 *	will be opened (f_ptr) and section headers read.
 *	Either text or data sections (t_ptr) will
 *	be disassembled. Line number information (l_ptr) 
 *	and symbol table information (symb and 
 *	symb_ptr) will also be needed.
 */

LDFILE	*f_ptr = NULL;		/* for file being processed information */
LDFILE  *t_ptr = NULL;		/* for section body being processed	*/
LDFILE	*l_ptr = NULL;		/* for line # associated with section   */
LDFILE	*symb = NULL;		/* for accessing the symbol table in	*/
				/* large blocks at a time in get_sym()	*/
LDFILE	*symb_ptr = NULL;	/* for accessing the symbol table in	*/
				/* line_nums function			*/
int	nosyms = FALSE;		/* TRUE if the symbol table has been 	*/
				/* stripped from object file		*/
/*
 *	The following structures contain the file
 *	header and section header information respectively
 */

FILHDR	filhdr;
SCNHDR scnhdr;

/*	One may disassemble a total of 10 libraries */
#define		LIBS	10

/* this array will be for pointers to allocated memory to hold the	*/
/* library names to be disassembled					*/

char	*libs[LIBS];

char	*fname;		/* to save and pass name of file being processed*/	
char	sec_name[NCPS+1]; /* in order for a section name to have its	*/
			/* own storage, this array is used and its	*/
			/* address given to 'sname'. This is the only	*/
			/* place in these routines that the reader	*/
			/* will find 'sec_name'.			*/
char	*sname = sec_name; /* to save and pass name of section		*/
unsigned short	snumb;	/* the number of the section being disassembled */
int	save_Lflag = 0;	/* for svaing the value of Lflag		*/


char	*namedsec[NSECS];	/* contains names of sections to be disassembled */
int	namedtype[NSECS];	/* specifies whether the corresponding section
				 * is to be disassembled as text or data
				 */
int	nsecs = -1;		/* number of sections in the above arrays */

FUNCLIST	*next_function;	/* structure containing name and address  */
				/* of the next function in a text section */
