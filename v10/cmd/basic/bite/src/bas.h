#include	<signal.h>
#include	<stdio.h>

/* Copyright Bell Telephone Laboratories Whippany, N.J.

 *	*************************************
 *	*************************************
 *	********* BITE HEADER FILE **********
 *	**** R. B. Drake WH 8C-005 X4163 ****
 *	**************** and ////////////////
 *	/// J. P. Hawkins WH X4610 8C-001 ///
 *	///// Tue Aug 28 08:04:33 1979 //////
 *	/////////////////////////////////////
 *	/////////////////////////////////////

*/
/*   "@(#) bas.h:  V 1.6  3/4/81" */
/*
 * MOD for Strings, J.P.Hawkins, 31-JAN-81
 */

#define	STRINGS		/* Comment out if string handling to be omitted */
/*
 * REMOVE REMARK DEVICES IF COMPILING FOR LSX
 */

/*
 * classification codes for algebraic expression fields
 * as returned by routine class()
 */
#ifdef	LSX
#define PGSIZ 256
#define	PREC sizeof(float)
#endif
#ifndef	LSX
#define	PREC sizeof(double)
#define PGSIZ 1023	/* cannot exceed 1023 because "loader ld"
			    calculates 1024*32=MINUS it doesn't
			     handle 2's compliment correctly 32K is
			      enough anyway */
#endif
#define NMPAG 32
#define NOMAX 10 /* max size of ascii line number buffer */
#define OPMAX 10 /* max size of ascii operation code buffer */
#define LINMAX 80 /* max size of a single line buffer */
#define	MAXDIM	10	/* maximum number of dimensions of array */
#define	MAXFREE	40	/* Max num of remembered freed spaces for strings */

/* These constants used for FIELD TYPE identification by class() */

#define	OPCLASS	1	/* operator field ^ * / + - ( */
#define	NMCLASS	2	/* numeric field */
#define	VRCLASS	3	/* variable name */
#define	VACLASS	5	/* variable array name */
#define	FNCLASS	6	/* function name */
#define	STCLASS	7	/* STRING class */
#define	SVCLASS	8	/* STRING variable class */
#define	SFCLASS	9	/* STRING function class */
#define	SACLASS	10	/* STRING array class */

/* string freelist */

struct	FREELIST {
	int	size;		/* size of memory void left by removed str */
	char	*fraddr;	/* address of void */
	};

/* command table entry template */

struct	COMMAND {
	char	*cmdtxt;	/* pointer to command test string */
	int	(*function)();	/* pointer to correstponding service rtn */
	};

/* line number and command code structure aligned with 4 bytes for storage*/
union bascmd
	{
		struct
		{
			short linno;
			struct
			{
				char hibyte;
				char lobyte;
			} opcode;
		}thing;
		char byte[4];
	};

struct pages
	{
		int maxlin; /* max line number in a 512 byte page */
		char *begin; /* pointer to the start of the first line on a page */
	};

/*
 * FILE SLOT TABLE
 */
struct FILTBL {
	char	mode;	/* 'r', 'w' or 'a' */
	int	fildes; /* system assigned "file descriptor" */
	};
/* variable byte alignment for "no holes" storage */

union VARIABLE {double var ; char var4th[PREC];};

extern union bascmd inst;
extern union bascmd local;
extern char linbuf[];
extern char *expr;
extern char txtbuf[];
extern char *linptr;
extern struct pages index[];
extern union VARIABLE varbyts;
extern struct COMMAND cmdtbl[];
extern char *curptr,*lbdptr;
extern int pg,size;
/*
 * //////// BASIC ROUTINE DISPATCHER ////////
 *
 * it is really a call indirect for table driven calls.
 * calling format:
 *	bascall(code);
 *
 *	where: code = legal BASIC PSEUDO opcode
 */
#define bascall(opcode) ((*cmdtbl[opcode].function)(0))
/*
 *
 * //// RETURN POINTER TO COMMAND STRING FOR GIVEN OPCODE ////
 *
 *
 * calling format:
 *	ptr = decode(opcode);
 *
 *	where: ptr = returned pointer to command string
 *	       opcode = VALID opcode (integer value)
 *			the opcode is assumed to be valid since
 *			it was "compiled" from the same code referenced
 *
 */
#define decode(opcode) (cmdtbl[opcode].cmdtxt)
/*
#define logten(x) ((x) <= 0) ? 0. : (.4342944819 * log(x)))
*/

/* jimmy added */

/*
 * symbol table pointers
 */
char *symtab[26][11];
char *asymtab[26][11];
char *ssymtab[26][11];
char *sasymtab[26][11];

extern	char scratch[];
extern	int	endfcnt;
extern	char	bpath[];
extern int singflg;
extern int tv;
extern int rem;
extern int _sub;
extern int tel;
extern int stpflg;
extern int autflg,aut,inc;
extern	moreflag[];
extern struct FILTBL filtbl[];
extern char _string[];
extern int pg;
extern char temp[];
extern int start,finish;
extern int line;
extern	int	_comflg;	/* common flag */
extern	int	stpflg;
extern	char	*eoexpr;
extern	int	stpflg;		/* stop flag for fatal error */
char	*hicore;	/* highest free user location pointer */
extern	struct	FREELIST freelist[];
extern char filnam[];
extern char *curptr;
extern int singflg;
extern int rof;
extern int singst;
extern int nxt,stpflg;
extern char filnam[];
extern int singflg;
extern int singst;
extern char *ptr;
extern int stpflg;
extern int start;
extern char *ptr,*curptr;
extern	int	esle,fidne;	/* codes for "else" and "endif" */
extern char *eoexpr;	/* pointer to char after eval expr */
extern char ascop[];	/*use for variable name string*/
extern int stpflg;
extern char linbuf[];
extern char linbuf[];
extern int aut,autflg,inc;
extern char *lbdptr,*curptr;
extern char *keyword[];
extern int start,finish;
extern char filnam[];
extern char txtbuf[];
extern int stpflg;
extern	struct FILTBL filtbl[];
extern	int	ticks;		/* LTC 1/60th sec counter */
extern int atad;
extern int no;
extern char temp[];
extern char *endob;
extern int _fi,_togo,subog;
extern int tsil,tospgo;
extern int start,finish;
extern char _string[];
extern int stopl();
extern int singst;
extern int stpflg;
extern int endfcnt;	/* structured "if" nest count */
extern int forflg;	/* nested for count */
extern int bflag,cflag;	/* break count */
extern int quit();
extern int _comflg;	/* common flag */
extern char filnam[];
extern char linbuf[];
extern char _string[];
extern int stpflg;
extern char *ptr;
extern int subog;
extern int stpflg;
extern char _string[];
extern	int bflag,cflag;		/* break & continue flag */
extern	int esle,fidne;		/* else and endif codes */
extern	int _togo;		/* "goto" code */
extern	int _fi;
extern int stpflg;	/* stop flag */
extern char *ptr;
extern char	txtbuf[];
#define strcat	Strcat
char *strcat(), *carcat();
char *expand();
char *prncpy();
#define pause	Pause
#define rand	Rand
#define srand	Srand
long lseek();
char *strcpy();
