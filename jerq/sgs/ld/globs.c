static char ID[] = "@(#) globs.c: 1.11 8/18/83";
#include "system.h"

#include <stdio.h>
#include "structs.h"

#if !ONEPROC || TRVEC
#include "tv.h"
#include "ldtv.h"
#endif

#include "paths.h"
#include "params.h"

/*
 * This include file is the complement of "extrns.h" and
 * should be changed whenever extrns.h is changed.
 */



int lineno;		/* current line number in ifile being parsed */
char *curfilnm;		/* ptr to current input file name	     */

char	aflag = 0,	/* file is to be "absolute"		*/
	cflag = 0,	/* perform only a partial execution	*/
	dflag = 0,	/* turn on debug output			*/
	iflag = 0,	/* ** b16ld/x86ld *** separate I and D	*/
	mflag = 0,	/* generate a memory map		*/
	rflag = 0,	/* mark the object file as relocatable	*/
	sflag = 0,	/* strip relocation/line/symbol info	*/
	tvflag = 0,	/* * b16ld/x86ld/n3bld * tv linkage	*/
#if UNIX || XL
	xflag = 0,	/* do not preserve local symbols	*/
#endif
#if COMMON
	tflag = 0,	/* do type checking, if on		*/
	Mflag = 0,	/* msg for multiply defnd external defs */
#endif
#if PAGING
	Fflag = 0,	/* do blocking, memory configuration, etc
			   as needed for paging, if on		*/
#endif
#if PAGING
	zflag = 0,	/* do not use memory around zero, if on */
#endif
#if ILDOPT
	ildflag = 0,	/* do not output .i_l_dxx sections	*/
#endif
	Hflag = 0,	/* "Hide" global symbols by converting them
			   into variables of type "static"	*/
	Nflag = 0,	/* squash text and data together, turn off -n */
	Sflag = 0,	/* "silent" running mode: issue messages
			   only for fatal errors		*/
	Vflag = 0;	/* echo version information about ld    */
int	hflag = 0,	/* size reserved for optional header	*/
	pflag = 0,	/* size reserved for patch list		*/
	Bflag = 0;	/* generate "padding" output sections	*/
unsigned short VSflag = 0;	/* put version stamp in optional hdr */

short usrdeftv = 0;	/* .tv input or SECTION			*/
short globfill = 0;	/* default fill value */
short errlev = 0;	/* error level generated during the ld run */

#if PAGING
short bond_t_d = 0;	/* set by dfltsec to indicate to pboslist that text
				and data must be properly bonded for paging */
#endif

#if FLEXNAMES		/* this implies PORTAR */
long stroffset = 4L;	/* offset of string in string table (4 is for length) */
long tnumsyms;			/* total number of symbols in output file */
#endif
#if PORTAR
long ar_size;			/* size of archive .o */
#endif


/*
 * Lists built during PASS 1, from user input
 */

struct listown symlist;		/* head of the symbol list	*/
struct listown memlist;		/* head of the MEMLIST		*/
struct listown reglist;		/* head of the REGLIST		*/
struct listown ldfilist;	/* head of the LDFILE List	*/
struct listown bldoutsc;	/* head of the BLDOUTSEC List	*/
struct listown explist;		/* head of the expression List	*/

#if !ONEPROC
int symcnt, memcnt, regcnt,	/* list counts, used in  */
    ldfcnt, bldcnt, expcnt, tvslotcnt;	/* transfering the lists */
#endif

#if !ONEPROC || TRVEC
TVASSIGN *tvslot1;		/* ptr to head of tv slot list */
#endif


/*
 * Lists built during PASS 2, from information obtained in PASS 1
 */

struct listown bondlist;	/* head of the BONDLIST		*/
struct listown ownlist;		/* head of the OWNLIST		*/
struct listown atownlst;	/* head of the ATTOWN List	*/
struct listown infilist;	/* head of the INFILE List	*/
struct listown outsclst;	/* head of the OUTSEC List	*/
struct listown avlist;		/* head of available space list */
struct listown dsectlst;	/* head of DSECT list		*/

char outfilnm[128] = A_OUT;	/* name of output object file */

#if !ONEPROC
char trnname[sizeof(TMPDIR)+20];	/* name of PASS1 -> PASS2 */
FILE *trnfdes;				/* "transfer" file	  */
#if AR16WR
char *trnfbuf = NULL;
#endif
#endif

char *strbase,			/* global save area for input ifile  */
     *strnext,			/* names, input *.o and archive file */
     *strlimit;			/* names			     */
int strleng;

char epsymbol[8] = "";		/* a.out header entry point symbol */

int numoutsc;			/* number of output sections */
long cur_dot = -1L;		/* current value of "dot" symbol
				 *	-1L indicates no value.
				 */

ACTITEM *curexp;		/* current expression being evaluated */

long maxlcsms;			/* max no. of local symbols for an input file */

long symtborg;			/* file address of symbol table in output file*/

long	allocspc = 0L;		/* amount of space allocated by myalloc */
long	maxalloc = 0L;		/* maximum allocated at any time	*/
#if DMERT || XL
int	dmertplib = 0;		/* DMERT public library			*/
#endif

ADDRESS memorg = MEMORG;		/* beginning of configured memory */
char	stat_name[9]	=	{ '\0','\0','\0','\0','\0','\0','\0','\0','\0'};
