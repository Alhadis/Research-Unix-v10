/*	"@(#) bas.h: V 2.40 11/14/83"	*/

#define PGSIZ 512
#ifndef	DFSIZ
#define	DFSIZ	(8 * 1024)
#endif
#define OPMAX 		50 	/* max size of ascii operation code buffer */
#define LINMAX 		255 	/* max size of a single program line */
#define ENCLINMAX 	(3*LINMAX)/* max size of encoded program line */
#define STRMAX 		255	/* maximum string length */
#define	MAXDIM		10	/* maximum number of dimensions of array */
#define	MAXARGS		10	/* maximum # of user-def. function arguments */
#define DEFDIM  	10	/* default value for each dimension */
#define	MAXLINNO	65529	/* maximum line number allowed */

/* stack and list sizes */
#define	BREAKSIZE	15
#define	LOOPSTACKSIZE	20
#define CONTEXTSIZE	60
#define CACHESIZE	10

/* constants for symbol table and data areas */
#define		IDSIZE		42	/* size of BASIC identifier in chars */
#define		HTSIZE		97	/* size of hash table */
#define		INTSIZE		sizeof(long) /* for use by hash function */

#define		EXTFUNC		255	/*type for user external subroutines;
					  all other types are yacc tokens
					  ( > 255).			   */

struct  sym 				/*symbol table entry*/
	{
		char 		*name;
		struct sym	*next;
		unsigned	symtype : 14;
		unsigned	isbufvar: 1;   /* set if the variable is
						  a file buffer variable */
		unsigned	scratch : 1;   /* used for functions to signal
					          "executing", and for data 
						  items to mark as "commoned".*/
		union
		{
			double		dval;		/* DBLID */
			float		fval;		/* SNGID */
			int		ival;		/* INTID */
			char		*string;	/* STRID */
			unsigned long	faddr;		/* EXTFUNC */
			struct 				/* DBLAID */
			{
				short 	*header;
				double  *arblok;
			} darr;
			struct 				/* SNGAID */
			{
				short 	*header;
				float  	*arblok;
			} farr;
			struct				/* STRAID */
			{
				short   	*header;
				char		**arblok;
			} sarr;
			struct				/* INTAID */
			{
				short   	*header;
				int		*arblok;
			} iarr;
			struct	/* DBLUID, SNGUID, INTUID and STRUID */
			{
				char		*funcexp; /*pointer to function
							 body in string space*/
				struct sym	*arg; /* pointer to arg list */
			} udef;
			struct /* buffer variables */
			{
				char	*start;
				int	length;
			} bufvar;
		} data;
	};

/*	structure of predefined function table			*/
struct FUNCTAB {
	char	*name;
	int	(*handler)();
	};



/*	expression semantic stack element	*/
typedef struct
	{
		unsigned char	type;
		union
		{
			double	dval;
			float	fval;
			int	ival;
			char	*sval;
		} val;
	} STAKREC;

/*	expression parse stack element	*/
typedef union  {
	int		ival;
	struct sym	*symptr;
	} YYSTYPE;

/*	definitions for expression evaluator interface		*/

#define	STRING	0
#define INTEGER	1
#define	SINGLE	2
#define	DOUBLE	3

struct value 
	{
	double	dval;
	float	fval;
	int	ival;
	char	sval[STRMAX+1];
	};

extern struct value	exprval;
extern short		exprtype;
extern char		*eoexpr;


/* command table entry */
struct	COMMAND {
	char	*cmdtxt;	/* pointer to command test string */
	int	(*function)();	/* pointer to correstponding service rtn */
	};


typedef	unsigned short LINNO;
typedef	unsigned char UCHAR;

/* statement information header */
struct CMDCODE {
	LINNO	linno;		/* line number of statement */
	UCHAR	stmtno;		/* position of statement on line */
	UCHAR	opcode;		/* code for command (cmdtbl index) */
	};

#define CMDSIZE	sizeof(struct CMDCODE)

/* structure for holding CMDCODE and permitting byte by byte access */
union bascmd
	{
		struct CMDCODE thing;
		char byte[CMDSIZE];
	};

/* record for program page pointers - used to speed line/statement fetches */
struct pages
	{
		LINNO 	maxlin; /* maximum line number on page */
		char 	*begin;	/* pointer to start of the first line on page */
	};

struct conrec {
	short	fornest;
	short	whilenest;
	short	ifnest;
	LINNO	nextno;
	UCHAR	stmtno;
	LINNO	gosublin;
	UCHAR	gosubstmt;
	char	*ptr;
	};


/* constants for calls to fetch (see comment on fetch.c) */
#define	FIRST	0
#define	NEXT	(MAXLINNO+1)
#define	IMMED	65535
#define LINE	0
#define	STMT	1

extern LINNO atoln();		/* ascii to line number conversion */
extern float maxfloat,
	     mkfloat();
extern char *lclfgets();
extern char *txtbuf;
extern long NMPAG;
extern char *linptr;
extern char *endofprog;
extern struct COMMAND cmdtbl[];
extern union bascmd inst;
extern char *expr;
extern char *curptr,*lbdptr;
extern char nullstrng;

/*
 *          BASIC routine dispatcher
 *
 * call indirect using cmdtbl[].
 * calling format:
 *	bascall(code);
 *
 *	where: code = legal BASIC opcode
 */
#define bascall(opcode) ((*cmdtbl[opcode].function)(0))

/*
 *
 *      return pointer to command string for given opcode
 *
 * calling format:
 *	ptr = decode(opcode);
 *
 *	where: ptr = returned pointer to command string
 *
 *	       opcode = valid opcode (integer value)
 */
#define decode(opcode) (cmdtbl[opcode].cmdtxt)

#define skip00(X) { while (**(X) == ' ' || **(X) =='\t')\
			(*(X))++;\
		  }

#define ALPHA(C) (((C) >= 'a' && (C) <= 'z') || ((C) >= 'A' && (C) <= 'Z'))

#define NUM(C) ((C) >= '0' && (C) <= '9')

/* codes for embedded keywords */
#define	LTcode		0200
#define	LEcode		0201
#define	NEcode		0202
#define	EQcode		0203
#define	ELcode		0204
#define	EGcode		0205
#define	GTcode		0206
#define ENcode		0207
#define	GEcode		0210
#define	ANDcode		0211
#define	AScode		0212
#define	EQVcode		0213
#define ERRORcode	0214
#define	GOSUBcode	0215
#define	GOTOcode	0216
#define	IMPcode		0217
#define	INPUTcode	0220
#define	INPUTPcode	0221
#define	MODcode		0222
#define NEXTcode	0223
#define	NOTcode		0224
#define	ORcode		0225
#define	SPCcode		0226
#define	STEPcode	0227
#define	TABcode		0230
#define	THENcode	0231
#define	TOcode		0232
#define	USINGcode	0233
#define	XORcode		0234

#if	vax	||	u3b	||	u3b5	||	u3b2
#undef	MAXSHORT
#define	MAXSHORT	32767
#endif
