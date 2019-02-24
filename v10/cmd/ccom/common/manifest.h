/* @(#) manifest.h: 1.4 3/26/84				*/


# include <stdio.h>

	/* macro definitions for common cases of type collapsing */
# ifdef NOSHORT
# define SZSHORT SZINT
# define ALSHORT ALINT
# endif

# ifdef NOLONG
# define SZLONG SZINT
# define ALLONG ALINT
# endif

# ifdef NOFLOAT
# define SZFLOAT SZLONG
# define SZDOUBLE SZLONG
# define ALFLOAT ALLONG
# define ALDOUBLE ALLONG
# endif

# ifdef ONEFLOAT
# define SZFLOAT SZDOUBLE
# define ALFLOAT ALDOUBLE
# endif

/* define default assembly language comment starter */

# ifndef COMMENTSTR
# define COMMENTSTR	"#"
#endif

/*	manifest constant file for the lex/yacc interface */

# define ERROR 1
# define NAME 2
# define STRING 3
# define ICON 4
# define FCON 5
# define PLUS 6
# define MINUS 8
# define MUL 11
# define STAR (UNARY MUL)
# define AND 14
# define OR 17
# define ER 19
# define QUEST 21
# define COLON 22
# define ANDAND 23
# define OROR 24

/*	special interfaces for yacc alone */
/*	These serve as abbreviations of 2 or more ops:
	ASOP	=, = ops
	RELOP	LE,LT,GE,GT
	EQUOP	EQ,NE
	DIVOP	DIV,MOD
	SHIFTOP	LS,RS
	ICOP	INCR,DECR
	UNOP	NOT,COMPL
	STROP	DOT,STREF

	*/
# define ASOP 25
# define RELOP 26
# define EQUOP 27
# define DIVOP 28
# define SHIFTOP 29
# define INCOP 30
# define UNOP 31
# define STROP 32

/*	reserved words, etc */
# define TYPE 33
# define CLASS 34
# define STRUCT 35
# define RETURN 36
# define GOTO 37
# define IF 38
# define ELSE 39
# define SWITCH 40
# define BREAK 41
# define CONTINUE 42
# define WHILE 43
# define DO 44
# define FOR 45
# define DEFAULT 46
# define CASE 47
# define SIZEOF 48
# define ENUM 49

/*	little symbols, etc. */
/*	namely,

	LP	(
	RP	)

	LC	{
	RC	}

	LB	[
	RB	]

	CM	,
	SM	;

	*/

# define LP 50
# define RP 51
# define LC 52
# define RC 53
# define LB 54
# define RB 55
# define CM 56
# define SM 57
# define ASSIGN 58
	/* ASM returned only by yylex, and totally eaten by yyparse */
# define ASM 59

/*	END OF YACC */

/*	left over tree building operators */
# define COMOP 59
# define DIV 60
# define MOD 62
# define LS 64
# define RS 66
# define DOT 68
# define STREF 69
# define CALL 70
# define FORTCALL 73
# define NOT 76
# define COMPL 77
# define INCR 78
# define DECR 79
# define EQ 80
# define NE 81
# define LE 82
# define LT 83
# define GE 84
# define GT 85
# define ULE 86
# define ULT 87
# define UGE 88
# define UGT 89
# define SETBIT 90
# define TESTBIT 91
# define RESETBIT 92
# define ARS 93
# define REG 94
# define TEMP 95
# define CCODES 96
# define FREE 97
# define STASG 98
# define STARG 99
# define STCALL 100

/*	some conversion operators */
# define FLD 103
# define CONV 104
# define PMUL 105
# define PDIV 106

/*	special node operators, used for special contexts */
/* # define FORCE 107 */
# define GENLAB 108
# define CBRANCH 109
# define GENBR 110
# define CMP 111
# define GENUBR 112
# define INIT 113
# define CAST 114
# define FUNARG 115
# define VAUTO 116
# define VPARAM 117
# define RNODE 118
# define SNODE 119
# define QNODE 120
/*	a whole bunch of ops, done with unary; I don't need to tackle prec */
# define UOP0  121
# define UOP1  122
# define UOP2  123
# define UOP3  124
# define UOP4  125
# define UOP5  126
# define UOP6  127
# define UOP7  128
# define UOP8  129
# define UOP9  130
/*	op used in second pass */
#define LABELPT	131
# define MANY 132
#define PROLOG	133
#define EPILOG	134
#define SWCASE	135
#define SWDEF	136
#define SWEND	137
#define SWBEG	138

/*	node types */
# define LTYPE 02
# define UTYPE 04
# define BITYPE 010

	/* DSIZE is the size of the dope array */
# define DSIZE MANY+1

/*	type names, used in symbol table building */
# define TNULL 0
# define FARG 1
# define CHAR 2
# define SHORT 3
# define INT 4
# define LONG 5
# define FLOAT 6
# define DOUBLE 7
# define STRTY 8
# define UNIONTY 9
# define ENUMTY 10
# define MOETY 11
# define UCHAR 12
# define USHORT 13
# define UNSIGNED 14
# define ULONG 15
# define VOID 16
# define UNDEF 17

# define ASG 1+
# define UNARY 2+
# define NOASG (-1)+
# define NOUNARY (-2)+

/*	various flags */
# define NOLAB (-1)

/* type modifiers */

# define PTR  040
# define FTN  0100
# define ARY  0140

/* type packing constants */

# define MTMASK 03
# define BTMASK 037
# define BTSHIFT 5 
# define TSHIFT 2
# define TMASK (MTMASK<<BTSHIFT)
# define TMASK1 (MTMASK<<(BTSHIFT+TSHIFT))
# define TMASK2  (TMASK||MTMASK)

/*	macros	*/

# ifndef BITMASK
	/* beware 1's complement */
# define BITMASK(n) (((n)==SZLONG)?-1L:((1L<<(n))-1))
# endif
# define ONEBIT(n) (1L<<(n))
# define MODTYPE(x,y) x = (x&(~BTMASK))|y  /* set basic type of x to y */
# define BTYPE(x)  (x&BTMASK)   /* basic type of x */
# define ISUNSIGNED(x) ((x)<=ULONG&&(x)>=UCHAR)
# define UNSIGNABLE(x) ((x)<=LONG&&(x)>=CHAR)
# define ENUNSIGN(x) ((x)+(UNSIGNED-INT))
# define DEUNSIGN(x) ((x)+(INT-UNSIGNED))
# define ISPTR(x) ((x&TMASK)==PTR)
# define ISFTN(x)  ((x&TMASK)==FTN)  /* is x a function type */
# define ISARY(x)   ((x&TMASK)==ARY)   /* is x an array type */
# define INCREF(x) (((x&~BTMASK)<<TSHIFT)|PTR|(x&BTMASK))
# define DECREF(x) (((x>>TSHIFT)&~BTMASK)|(x&BTMASK))
# define SETOFF(x,y)   if( (x)%(y) != 0 ) x = ( ((x)/(y) + 1) * (y))
		/* advance x to a multiple of y */
# define NOFIT(x,y,z)   ( ((x)%(z) + (y)) > (z) )
	/* can y bits be added to x without overflowing z */
	/* pack and unpack field descriptors (size and offset) */
# define PKFIELD(s,o) (((o)<<6)|(s))
# define UPKFSZ(v)  ((v)&077)
# define UPKFOFF(v) ((v)>>6)

/*	operator information */

# define TYFLG 016
# define ASGFLG 01
# define LOGFLG 020

# define SIMPFLG 040
# define COMMFLG 0100
# define DIVFLG 0200
# define FLOFLG 0400
# define LTYFLG 01000
# define CALLFLG 02000
# define MULFLG 04000
# define SHFFLG 010000
# define ASGOPFLG 020000

# define SPFLG 040000

#define optype(o) (dope[o]&TYFLG)
#define asgop(o) (dope[o]&ASGFLG)
#define asgbinop(o) (dope[o]&ASGOPFLG)
#define logop(o) (dope[o]&LOGFLG)
#define callop(o) (dope[o]&CALLFLG)

/*	table sizes	*/

#ifdef M32B
# define BCSZ 125 /* size of the table to save break and continue labels */
#else
# define BCSZ 100 /* size of the table to save break and continue labels */
#endif
# define MAXNEST BCSZ	/* maximum nesting depth (for scopestack) */
# ifndef SYMTSZ
# define SYMTSZ 6000 /* size of the symbol table (was 2000) */
# endif
# define DIMTABSZ 300 /* size of the dimension/size table (dynamic) */
# define PARAMSZ 650 /* size of the parameter stack */
# define ARGSZ 50 /* size of the argument stack */
# ifndef TREESZ
# ifndef FORT
# define TREESZ 1500 /* space for building parse tree */
# else
# define TREESZ 1500
# endif
# endif
# define SWITSZ 4000 /* size of switch table */

	char		*hash();
	char		*savestr();
	char		*tstr();
	extern int	tstrused;
	extern char	*tstrbuf[];
	extern char	**curtstr;
#define freestr()	( curtstr = tstrbuf, tstrused = 0 )

#	define NCHNAM 8  /* number of characters in a truncated name */

/*	common defined variables */

extern int nerrors;  /* number of errors seen so far */

typedef union ndu NODE;
typedef unsigned int TWORD;
typedef long CONSZ;  /* size in which constants are converted */

	/* default is byte addressing */
	/* BITOOR(x) converts bit width x into address offset */
# ifndef BITOOR
# define BITOOR(x) ((x)/SZCHAR)
# endif

#if defined(TMPSRET) && !defined(AUXREG)
#define AUXREG (NRGS - 1)
#endif

# define NIL (NODE *)0

extern int dope[];  /* a vector containing operator information */
extern char *opst[];  /* a vector containing names for ops */

# define NCOSTS (NRGS+4)

	/* in one-pass operation, define the tree nodes */

union ndu {

	struct {
		int op;
		int goal;
		TWORD type;
		int cst[NCOSTS];
		char * name;
		char pad[NCHNAM-sizeof(char *)];	/* padding hack! */
		NODE *left;
		NODE *right;
	}in;	/* interior node */
	struct {
		int op;
		int goal;
		TWORD type;
		int cst[NCOSTS];
		char * name;
		char pad[NCHNAM-sizeof(char *)];	/* padding hack! */
		NODE *left;
		NODE *right;
		int lineno;
	}ln;	/* line number? */
	
	struct {
		int op;
		int goal;
		TWORD type;
		int cst[NCOSTS];
		char * name;
		char pad[NCHNAM-sizeof(char *)];	/* padding hack! */
		CONSZ lval;
		int rval;
	}tn;	/* terminal node */
	
	struct {
		int op;
		int goal;
		TWORD type;
		int cst[NCOSTS];
		int label;  /* for use with branching */
		int lop;  /* the opcode being branched on */
	}bn;	/* branch node */

	struct {
		int op;
		int goal;
		TWORD type;
		int cst[NCOSTS];
		int stsize;  /* sizes of structure objects */
		short stalign;  /* alignment of structure objects */
		short argsize;  /* size of argument list for call */
	}stn;	/* structure node */

	struct {
		int op;
		int goal;
		TWORD type;
		int cdim;
		int csiz;
	}fn;	/* front node */
	
	struct {
		/* this structure is used when a floating point constant
		   is being computed */
		int op;
		int goal;
		TWORD type;
		int cdim;
		int csiz;
		double dval;
	}fpn;	/* floating point node */

};
