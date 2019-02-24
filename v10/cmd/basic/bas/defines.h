/*
 * macros and global declarations for basic
 *
 *
 *
 * stack frame types:
 *
 * NOTE: the EXPR types must be distinguishable from all others
 *	 by the presence of the single bit defined by EXPR
 *	 Also note that INT, FLOAT and STRING are used as indices
 *	 into the 'chains' array (cf. sym.c) and that the value
 *	 of MAXTYPES must be at least 1 greater than that of the
 *	 highest-numbered basic type.
 */

#define ANYTYPE 	0
#define INT		1
#define FLOAT		2
#define STRING		3
#define STK_DEF 	5
#define STK_FOR 	6
#define STK_GOSUB	7
#define INTEXPR 	INT+EXPR
#define FLOATEXPR	FLOAT+EXPR
#define STRINGEXPR	STRING+EXPR
#define EXPR		010

#define MAXTYPES 	4


/*
 * macros and abbreviations:
 */

#define ALL_LINES(p)	(p=(Linep)lines;isline(p);p=nextline(p))
#define endchk()	if(!endtest()) badsyn()
#define endtest()	(*inptr==0||*inptr==COLON||*inptr==ELSE)
#define expectc(c)	if(*inptr!=c) badsyn(); else ++inptr
#define fexpr() 	(expr(),popfloat())
#define isexpr(s)	((s)->k_type&EXPR)
#define isline(p)	((p)->l_len)
#define isstring(s)	((s)>=strspace&&(s)<endstring)
#define istoken(c)	((c)<0)
#ifdef NULL
#undef NULL
#endif
#define NULL		(char *)0
#define pushint(i)	pushfloat((double)(i))
#define skipbl()	while(*inptr==' '||*inptr=='\t')++inptr
#define SINGLE		(typelens[FLOAT]==4)
#define UNTOKEN(c)	(-1-(c))

#define endstring	strspace+MAXSTRSPACE
#define line		immed.l_line
#define stkbase 	stkspace
#define stktop		stkspace+MAXSTACK


/*
 * sizes and limits:
 */

#define DEFSIZE 	10	/* default number of array elements */
#define EXPANDLINES	4096	/* expansion size */
#define EXPANDSYM	4096	/* symbol table expansion increment */
#define LEN_FUZZ	6	/* close enough to not move */
#define MAXFILENAME	32
#define MAXLINELEN	255	/* maximum length of an input line */
#define MAXLINES	4096	/* initial size of line buffer */
#define MAXSTACK	2048	/* size of expression stack */
#define MAXSTRING	255	/* max string len. (must fit in a char) */
#define MAXSTRSPACE	2048	/* size of string table */
#define MAXSUBS 	3	/* max number of array dimensions */
#define MAXSYMSPACE	4096	/* initial size of symbol table */
#define TOKEN		0177600
#define TOKENMASK	0177

#define FLOATSIZE	(sizeof(double))
#define INTSIZE 	(sizeof(int))
#define STRINGSIZE	(sizeof(String))

#define MINLNR		0
#define MAXLNR		(unsigned)65535
#define MAXINT		32767


/*
 * status and action symbols:
 */

#define LISTLNRS	1	/* getlnrs: expect lnrs in input */
#define DEFAULTLNRS	0	/* getlnrs: use defaults if no lnrs found */
#define EXACTLNR	1	/* findline: error if line not found */
#define NEXTLNR 	0	/* findline: return next higher line */
#define YES		1
#define NO		0
