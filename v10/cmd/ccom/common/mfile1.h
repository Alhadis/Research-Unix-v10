/* @(#) mfile1.h: 1.1 12/22/83				*/


# include "macdefs.h"
# include "manifest.h"

/*	storage classes  */
# define SNULL 0
# define AUTO 1
# define EXTERN 2
# define STATIC 3
# define REGISTER 4
# define EXTDEF 5
# define LABEL 6
# define ULABEL 7
# define MOS 8
# define PARAM 9
# define STNAME 10
# define MOU 11
# define UNAME 12
# define TYPEDEF 13
# define FORTRAN 14
# define ENAME 15
# define MOE 16
# define UFORTRAN 17
# define USTATIC 18
	/* field size is ORed in */
# define FIELD 0100
# define FLDSIZ 077
# ifndef NODBG
extern char *scnames();
# endif

/*	location counters */
# define PROG 0
# define ADATA 1
# define DATA 2
# define ISTRNG 3
# define STRNG 4

/* symbol table flags */
# define SMOS 01
# define SHIDDEN 02
# define SHIDES 04
# define SSET 010
# define SREF 020
# define SNONUNIQ 040
# define STAG 0100
# define SBSS 0200

typedef long OFFSZ;

struct symtab {
	char *sname;
	TWORD stype;  /* type word */

	char sclass;  /* storage class */
	char slevel;  /* scope level */
	char sflags;  /* flags for set, use, hidden, mos, etc. */
	int offset;  /* offset or value */
	short dimoff; /* offset into the dimension table */
	short sizoff; /* offset into the size table */
	short suse;  /* line number of last use of the variable */
	struct symtab *scopelink;	/* chain of symbols at same level */
};


struct sw {
	CONSZ sval;
	int slab;
};

extern struct sw swtab[];
extern struct sw *swp;
extern int swx;

extern int ftnno;
extern int blevel;
extern int instruct, stwart;

extern int lineno, slineno, nerrors;
typedef struct {
		union {
		int u_intval;
		NODE *u_nodep;
	} YY_un;
	int lineno;
} YYSTYPE;
#define intval YY_un.u_intval
#define nodep YY_un.u_nodep
extern YYSTYPE yylval;

extern CONSZ lastcon;
extern CONSZ ccast();
extern double dcon;

extern char ftitle[];
extern struct symtab stab[];
extern int curftn;
extern int strftn;
extern char *locnames[];
extern int curloc;
extern int curclass;
extern int curdim;
extern int *dimtab;
extern int paramstk[];
extern int paramno;
extern int argstk[];
extern int argsoff[];
extern TWORD argty[];
extern int argno;
extern int autooff, argoff, strucoff;
extern int regvar;
extern int nextrvar;
extern int brkflag;
extern char yytext[];

extern int strflg;

extern OFFSZ inoff;

extern int reached;

/*	tunnel to buildtree for name id's */

extern int idname;

extern NODE node[];
extern NODE *lastfree;

extern int cflag, hflag, pflag;

/* various labels */
extern int brklab;
extern int contlab;
#ifdef M32B
	extern int swregno;
#endif
extern int flostat;
extern int retlab;
extern int retstat;
extern int asavbc[], *psavbc;

/*	flags used in structures/unions */

# define SEENAME 01
# define INSTRUCT 02
# define INUNION 04
# define FUNNYNAME 010
# define TAGNAME 020

/*	flags used in the (elementary) flow analysis ... */

# define FBRK 02
# define FCONT 04
# define FDEF 010
# define FLOOP 020

/*
* These defines control "while" and "for" loop code generation.
* wloop_level and floop_level each must be set to one of these values.
*/
#define LL_TOP	0	/* test at loop top */
#define LL_BOT	1	/* test at loop bottom */
#define LL_DUP	2	/* duplicate loop test at top and bottom */

#ifdef M32B
	/* for register allocation optimization */
	extern int fordepth;
	extern int whdepth;
	extern int brdepth;
#   ifdef IMPREGAL
	extern void rainit(), radbl(), raname(), raua(), raftn();
#   endif
#endif

/*	flags used for return status */

# define RETVAL 1
# define NRETVAL 2

/*	used to mark a constant with no name field */

# define NONAME 040000

	/* mark an offset which is undefined */

# define NOOFFSET (-10201)

/*	declarations of various functions */

extern NODE
	*aadjust(),
	*bcon(),
	*bdty(),
	*block(),
	*bpsize(),
	*buildtree(),
#ifdef M32B
	*myand(),
	*setswreg(),
#endif
	*clocal(),
	*conval(),
	*convert(),
	*dclstruct(),
	*doszof(),
	*getstr(),
	*makety(),
	*mkty(),
	*oconvert(),
	*offcon(),
	*optim(),
	*pconvert(),
	*ptmatch(),
	*pvconvert(),
	*rstruct(),
	*sconvert(),
	*strargs(),
	*stref(),
	*talloc(),
	*tymatch(),
	*tymerge(),
	*unconvert(),
	*xicolon();

OFFSZ	tsize(),
	psize();

TWORD	types(),
	ctype();

char *exname(), *exdcon();

# define checkst(x)

/* type that is equivalent to pointers in size */
# ifndef PTRTYPE
# define PTRTYPE INT
# endif
