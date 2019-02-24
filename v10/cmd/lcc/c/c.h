#ifndef C_H_INCLUDED
#define C_H_INCLUDED
/* C compiler: definitions */

/* default sizes */
#define MAXLINE   512		/* maximum input/output line length */
#define MAXTOKEN   32		/* maximum token length */
#define	BUFSIZE	 4096		/* input buffer size */
#define HASHSIZE  128		/* default hash table size */
#define MEMINCR    10		/* blocks (1kb) allocated per arena */

#ifdef __LCC__
#ifndef __STDC__
#define __STDC__
#endif
#endif

#include "ops.h"

#ifdef __STDC__
#include <stdarg.h>
#define va_init va_start

typedef enum tokencode {
#define xx(a,b,c,d,e,f,g) a=b,
#define yy(a,b,c,d,e,f,g)
#include "token.h"
	NTOKENS
} Typeop;			/* type operators are a subset of tokens */

#define dclproto(func,args) func args
typedef void *Generic;
#else
#include <varargs.h>
#define va_init(a,b) va_start(a)

#define xx(a,b,c,d,e,f,g)
#include "token.h"
typedef int Typeop;

#define dclproto(func,args) func()
typedef char *Generic;
#endif

typedef struct list {		/* lists: */
	Generic x;			/* element */
	struct list *link;		/* next node */
} *List;

typedef struct coord {	/* source coordinates: */
	char *file;		/* file name */
	unsigned short x, y;	/* x,y position in file */
} Coordinate;
typedef union value {	/* constant values: */
	char sc;		/* signed */
	short ss;		/* signed */
	int i;			/* signed */
	unsigned char uc;
	unsigned short us;
	unsigned int u;
	float f;
	double d;
	char *p;		/* pointer to anything */
} Value;

typedef struct symbol *Symbol;	/* symbol table entries */
typedef struct table *Table;	/* symbol tables */
typedef struct tynode *Type;	/* type nodes */
typedef struct node *Node;	/* dag nodes */
typedef struct tree *Tree;	/* tree nodes */
typedef struct field *Field;	/* struct/union fields */
typedef struct swtch *Swtch;	/* switch data */

typedef struct metrics {
	unsigned char size, align, inline;
} Metrics;

#include "config.h"
typedef struct interface {
	char *target;
	Metrics charmetric;
	Metrics shortmetric;
	Metrics intmetric;
	Metrics floatmetric;
	Metrics doublemetric;
	Metrics ptrmetric;
	Metrics structmetric;
	unsigned left_to_right:1;
	unsigned little_endian:1;
	unsigned jump_on_return:1;
	unsigned mulops_are_calls:1;
	unsigned compl_band:1;
	unsigned no_argb:1;
	unsigned no_dag:1;
	dclproto(void (*address),(Symbol, Symbol, int));
	dclproto(void (*blockbeg),(Env *));
	dclproto(void (*blockend),(Env *));
	dclproto(void (*defaddress),(Symbol));
	dclproto(void (*defconst),(int, Value));
	dclproto(void (*defstring),(int, char *));
	dclproto(void (*defsymbol),(Symbol));
	dclproto(void (*emit),(Node));
	dclproto(void (*export),(Symbol));
	dclproto(void (*function),(Symbol, Symbol [], Symbol [], int));
	dclproto(Node (*gen),(Node));
	dclproto(void (*global),(Symbol));
	dclproto(void (*import),(Symbol));
	dclproto(void (*local),(Symbol));
	dclproto(void (*progbeg),(int, char **));
	dclproto(void (*progend),(void));
	dclproto(void (*segment),(int));
	dclproto(void (*space),(int));

	dclproto(void (*stabblock),(int, int, Symbol*));
	dclproto(void (*stabend),(Coordinate *, Symbol, Coordinate **, Symbol *, Symbol *));
	dclproto(void (*stabfend),(Symbol, int));
	dclproto(void (*stabinit),(char *, int, char *[]));
	dclproto(void (*stabline),(Coordinate *));
	dclproto(void (*stabsym),(Symbol));
	dclproto(void (*stabtype),(Symbol));
	Xinterface x;
}  Interface;
extern Interface *interfaces[], *IR;

#ifndef MAXKIDS
#define MAXKIDS 2
#endif
#ifndef MAXSYMS
#define MAXSYMS 3
#endif

struct node {		/* dag nodes: */
	Opcode op;		/* operator */
	short count;		/* reference count */
 	Symbol syms[MAXSYMS];	/* symbols */
	Node kids[MAXKIDS];	/* operands */
	Node link;		/* next dag in the forest */
	Xnode x;		/* back-end's type extension */
};
struct tree {		/* tree nodes: */
	Opcode op;		/* operator */
	Type type;		/* type of result */
	Tree kids[2];		/* operands */
	Node node;		/* associated dag node */
	union {
		Symbol sym;	/* associated symbol */
		Value v;	/* associated value */
		Field field;	/* associated struct/union bit field */
	} u;
};
typedef struct code *Code;
struct code {		/* code list entries: */
#ifdef __STDC__
	enum {
		Blockbeg, Blockend, Local, Address, Defpoint,
		Label, Start, Asm, Gen, Jump, Switch } kind;
#else
	int kind;
#define Blockbeg	0
#define Blockend	1
#define Local		2
#define Address		3
#define Defpoint	4
#define Label		5
#define Start		6
#define Asm		7
#define Gen		8
#define Jump		9
#define Switch		10
#endif
	Code prev;			/* previous code node */
	Code next;			/* next code node */
	union {
		struct {		/* Asm: assembly language */
			char *code;		/* assembly code */
			Symbol *argv;		/* %name arguments */
		} acode;
		struct {		/* Blockbeg: */
			Code prev;		/* previous Blockbeg */
			short bnumber;		/* block number */
			short level;		/* block level */
			Symbol *locals;		/* locals */
			Table identifiers, types;/* symbol tables; used for -g */
			Env x;			/* value filled in by blockbeg() */
		} block;
		Symbol var;		/* Local: local variable */
		struct {		/* Address: */
			Symbol sym;		/* created symbol */
			Symbol base;		/* local or parameter */
			int offset;		/* offset from sym */
		} addr;
		struct {		/* Defpoint: execution point */
			Coordinate src;		/* source location */
			int point;		/* execution point number */
		} point;
		Node node;		/* Label, Gen, Jump: a dag node */
		struct swselect {	/* Switch: swselect data */
			Symbol sym;		/* temporary holding value */
			Symbol table;		/* branch table */
			Symbol deflab;		/* default label */
			int size;		/* size of value & label arrays */
			int *values;		/* value, label pairs */
			Symbol *labels;
		} swtch;
	} u;
};
typedef struct arena *Arena;
struct arena {			/* storage allocation arena: */
	int m;				/* size of current allocation request */
	char *avail;			/* next available location */
	char *limit;			/* address 1 past end of arena */
	Arena first;			/* pointer to first arena */
	Arena next;			/* link to next arena */
};
struct symbol {		/* symbol structures: */
	Xsymbol x;		/* back-end's type extension */
	char *name;		/* name */
	unsigned short scope;	/* scope level */
	unsigned char sclass;	/* storage class */
	unsigned defined:1;	/* 1 if defined */
	unsigned temporary:1;	/* 1 if a temporary */
	unsigned generated:1;	/* 1 if a generated identifier */
	unsigned computed:1;	/* 1 if an address computation identifier */
	unsigned addressed:1;	/* 1 if its address is taken */
	unsigned structarg:1;	/* 1 if parameter is a struct */
	float ref;		/* # of references */
	Type type;		/* data type */
	Coordinate src;		/* definition coordinate */
	Coordinate **uses;	/* array of Coordinate *'s for uses (omit) */
	Symbol up;		/* next symbol in this or outer scope */
	union {
		struct {		/* labels: */
			int label;		/* label number */
			Symbol equatedto;	/* equivalent label */
		} l;
		struct {	/* struct/union types: */
			unsigned cfields:1; 	/* 1 if >= 1 const fields */
			unsigned vfields:1;	/* 1 if >= 1 volatile fields */
			Table ftab;		/* if xref != 0, table of field names */
			Field flist;		/* field list */
		} s;
		int value;	/* enumeration identifiers: value */
		Symbol *idlist;	/* enumeration types: identifiers */
		struct {	/* constants: */
			Value v;	/* value */
			Symbol loc;	/* out-of-line location */
		} c;
		struct {	/* functions: */
			Coordinate pt[3];/* source code coordinates */
			int label;	/* exit label */
			int ncalls;	/* # calls in this function */
			Symbol *callee;	/* parameter symbols */
		} f;
		int seg;	/* globals, statics: definition segment */
	} u;
#ifdef Ysymbol
		Ysymbol y;
#endif
};
#ifdef __STDC__
enum { CONSTANTS=1, LABELS, GLOBAL, PARAM, LOCAL };
#else
#define CONSTANTS 1
#define LABELS	2
#define GLOBAL	3
#define PARAM	4
#define LOCAL	5
#endif
typedef struct {
	unsigned printed:1;
	unsigned marked:1;
	unsigned short typeno;
} Xtype;

struct tynode {		/* type nodes: */
	Typeop op;		/* operator */
	short align;		/* alignment in storage units */
	int size;		/* size in storage units */
	Type type;		/* operand */
	union {
		Symbol sym;		/* associated symbol */
		struct {		/* function types */
			unsigned oldstyle:1;	/* 1 if an old-style function type */
			Type *proto;		/* prototype */
		} f;
	} u;
	Xtype x;		/* symbol table information */
#ifdef Ytype
	Ytype y;
#endif
};
struct field {		/* struct/union fields: */
	char *name;		/* field name */
	Type type;		/* data type */
	int offset;		/* field offset */
	short bitsize;		/* field size in bits */
	short lsb;		/* lsb bit of the field; rightmost bit is bit 1 */
	Field link;		/* next field in this type */
};

/* limits */
#ifdef __LCC__
#include <limits.h>
#include <float.h>
#else
/*
 * The magnitudes of the values below are greater than or equal to the minimum
 * permitted by the standard (see Appendix D) and are typical for byte-addressed
 * machines with 32-bit integers. These values are suitable for bootstrapping.
 */
#define CHAR_BIT	8
#define MB_LEN_MAX	1

#define UCHAR_MAX	0xff
#define USHRT_MAX	0xffff
#define UINT_MAX	0xffffffff
#define ULONG_MAX	0xffffffffL

#define CHAR_MAX	SCHAR_MAX
#define SCHAR_MAX	0x7f
#define SHRT_MAX	0x7fff
#define INT_MAX		0x7fffffff
#define LONG_MAX	0x7fffffffL

#define CHAR_MIN	SCHAR_MIN
#define SCHAR_MIN	(-SCHAR_MAX-1)
#define SHRT_MIN	(-SHRT_MAX-1)
#define INT_MIN		(-INT_MAX-1)
#define LONG_MIN	(-LONG_MAX-1)

#define FLT_MAX		1e37
#define DBL_MAX		1e37
#endif

#ifdef __STDC__
enum { CODE=1, BSS, DATA, LIT, SYM };	/* logical segments */
#else
#define CODE	1
#define BSS	2
#define DATA	3
#define LIT	4
#define SYM	5
#define CONSTANTS 1
#define LABELS	2
#define GLOBAL	3
#define PARAM	4
#define LOCAL	5
#endif

/* misc. macros */
#define roundup(x,n) (((x)+((n)-1))&(~((n)-1)))
#define utod(x)	(2.*(int)((unsigned)(x)>>1)+(int)((x)&1))
#ifdef NDEBUG
#define assert(c)
#else
#define assert(c) ((c) || fatal(__FILE__,"assertion failure at line %d\n",\
	__LINE__))
#endif

/* C library */
#ifndef strtod
dclproto(extern double strtod,(char *, char **));
#endif
dclproto(extern int atoi,(char *));
dclproto(extern int close,(int));
dclproto(extern int creat,(char *, int));
dclproto(extern void exit,(int));
dclproto(extern Generic malloc,(unsigned));
dclproto(extern int open,(char *, int));
dclproto(extern int read,(int, char *, int));
dclproto(extern long strtol,(char *, char **, int));
dclproto(extern int sprintf,(char *, const char *, ...));
dclproto(extern char *strchr,(const char *, int));
dclproto(extern int strcmp,(const char *, const char *));
dclproto(extern unsigned strlen,(const char *));
dclproto(extern char *strncmp,(const char *, const char *, unsigned));
dclproto(extern char *strncpy,(char *, const char *, unsigned));
dclproto(extern int write,(int, char *, int));

extern int bnumber;
extern Table constants;
extern Table externals;
extern Table globals;
extern Table identifiers;
extern Table labels[2];
/*G*/extern Table types;
extern int level;
extern List symbols;
#define islabel(p) ((p) && (p)->op == LABEL+V && (p)->syms[0])
#define yyalloc(n,ap) (ap->m = roundup(n,sizeof(double)), \
	ap->avail + ap->m >= ap->limit ? allocate(ap->m, &ap) : \
	(ap->avail += ap->m, ap->avail - ap->m))
#define alloc(n)  yyalloc(n, permanent)
#define talloc(n) yyalloc(n, transient)
#define BZERO(p,t) \
	{ unsigned *q1 = (unsigned *)(p), *q2 = q1 + ((sizeof (t)/sizeof (unsigned))&~(8-1)); \
	for ( ; q1 < q2; q1 += 8) \
		q1[0] = q1[1] = q1[2] = q1[3] = q1[4] = q1[5] = q1[6] = q1[7] = 0; \
	sizeof (t)/sizeof (unsigned)%8 >= 1 ? q1[0] = 0 : 0; \
	sizeof (t)/sizeof (unsigned)%8 >= 2 ? q1[1] = 0 : 0; \
	sizeof (t)/sizeof (unsigned)%8 >= 3 ? q1[2] = 0 : 0; \
	sizeof (t)/sizeof (unsigned)%8 >= 4 ? q1[3] = 0 : 0; \
	sizeof (t)/sizeof (unsigned)%8 >= 5 ? q1[4] = 0 : 0; \
	sizeof (t)/sizeof (unsigned)%8 >= 6 ? q1[5] = 0 : 0; \
	sizeof (t)/sizeof (unsigned)%8 >= 7 ? q1[6] = 0 : 0; \
	sizeof (t)%sizeof (unsigned) >= 1 ? ((char *)(q1 + sizeof (t)/sizeof (unsigned)%8))[0] = 0 : 0; \
	sizeof (t)%sizeof (unsigned) >= 2 ? ((char *)(q1 + sizeof (t)/sizeof (unsigned)%8))[1] = 0 : 0; \
	sizeof (t)%sizeof (unsigned) >= 3 ? ((char *)(q1 + sizeof (t)/sizeof (unsigned)%8))[2] = 0 : 0; \
	}
#define fieldsize(p) (p)->bitsize
#define fieldright(p) ((p)->lsb - 1)
#define fieldmask(p) (~(~(unsigned)0<<fieldsize(p)))
#define fieldleft(p) (8*(p)->type->size - fieldsize(p) - fieldright(p))
/*
 * type-checking macros.
 * the operator codes are defined in token.h
 * to permit the range tests below; don't change them.
 */
#define isqual(t)	((t)->op >= CONST)
#define isvolatile(t)	((t)->op == VOLATILE || (t)->op == CONST+VOLATILE)
#define isconst(t)	((t)->op == CONST    || (t)->op == CONST+VOLATILE)
#define unqual(t)	(isqual(t) ? (t)->type : t)
#define	isarray(t)	(unqual(t)->op == ARRAY)
#define	isstruct(t)	(unqual(t)->op == STRUCT || unqual(t)->op == UNION)
#define isunion(t)	(unqual(t)->op == UNION)
#define	isfunc(t)	(unqual(t)->op == FUNCTION)
#define	isptr(t)	(unqual(t)->op == POINTER)
#define ischar(t)	(unqual(t)->op == CHAR)
#define isint(t)	(unqual(t)->op >= CHAR && unqual(t)->op <= UNSIGNED)
#define isfloat(t)	(unqual(t)->op <= DOUBLE)
#define isarith(t)	(unqual(t)->op <= UNSIGNED)
#define isunsigned(t)	(unqual(t)->op == UNSIGNED)
#define isdouble(t)	(unqual(t)->op == DOUBLE)
#define isscalar(t)	(unqual(t)->op <= POINTER || unqual(t)->op == ENUM)
#define isenum(t)	(unqual(t)->op == ENUM)
#define widen(t)	(isint(t) || isenum(t) ? INT : ttob(t))
dclproto(extern List append,(Generic, List));
dclproto(extern int length,(List));
dclproto(extern Generic *ltoa,(List, Generic []));
/*G*/dclproto(extern char *string,(char *));
/*G*/dclproto(extern char *stringd,(int));
dclproto(extern char *stringn,(char *, int));

dclproto(extern Symbol constant,(Type, Value));
dclproto(extern void enterscope,(void));
dclproto(extern void exitscope,(void));
dclproto(extern void fielduses,(Symbol, Generic));
dclproto(extern Symbol findlabel,(int));
dclproto(extern Symbol findtype,(Type));
/*G*/dclproto(extern void foreach,(Table, int, void (*)(Symbol, Generic), Generic));
dclproto(extern Symbol genident,(int, Type, int));
/*G*/dclproto(extern int genlabel,(int));
dclproto(extern Symbol install,(char *, Table *, int));
dclproto(extern Symbol intconst,(int));
dclproto(extern void locus,(Table, Coordinate *));
dclproto(extern Symbol lookup,(char *, Table));
/*G*/dclproto(extern Symbol newtemp,(int, int));
dclproto(extern void rmtemps,(int, int));
dclproto(extern void release,(Symbol));
dclproto(extern void setuses,(Table));
dclproto(extern Table table,(Table, int));
dclproto(extern Symbol temporary,(int, Type));
dclproto(extern void use,(Symbol, Coordinate));

dclproto(extern void addlocal,(Symbol));
dclproto(extern Type btot,(int));
dclproto(extern Code code,(int));
/*G*/dclproto(extern void emitcode,(void));
/*G*/dclproto(extern void gencode,(Symbol [], Symbol []));
dclproto(extern Node listnodes,(Tree, int, int));
dclproto(extern Node jump,(int));
/*G*/dclproto(extern Node newnode,(int, Node, Node, Symbol));
dclproto(extern Node node,(int, Node, Node, Symbol));
dclproto(extern void printdag,(Node, int));
dclproto(extern void walk,(Tree, int, int));
extern struct code codehead;
extern Code codelist;
extern int nodecount;
dclproto(extern void compound,(int, Swtch, int));
dclproto(extern void finalize,(void));
dclproto(extern void program,(void));
dclproto(extern Type typename,(void));
extern Symbol cfunc;
extern char *fname;
extern Symbol retv;
dclproto(extern int genconst,(Tree, int));
dclproto(extern int hascall,(Tree));
dclproto(extern int nodeid,(Tree));
dclproto(extern char *opname,(int));
dclproto(extern int *printed,(int));
dclproto(extern void printtree,(Tree, int));
dclproto(extern Tree retype,(Tree, Type));
dclproto(extern Tree root,(Tree));
dclproto(extern Tree texpr,(Tree (*)(int), int));
dclproto(extern void tfree,(void));
dclproto(extern Tree tree,(int, Type, Tree, Tree));
extern int ntree;
dclproto(extern Tree addrof,(Tree));
dclproto(extern Tree asgn,(Symbol, Tree));
dclproto(extern Type assign,(Type, Tree));
dclproto(extern Tree cast,(Tree, Type));
dclproto(extern Tree cond,(Tree));
dclproto(extern Tree conditional,(int));
dclproto(extern Tree constexpr,(int));
dclproto(extern Tree expr0,(int));
dclproto(extern Tree expr,(int));
dclproto(extern Tree expr1,(int));
dclproto(extern Tree field,(Tree, char *));
dclproto(extern char *funcname,(Tree));
dclproto(extern Tree idnode,(Symbol));
dclproto(extern Tree incr,(int, Tree, Tree));
dclproto(extern int intexpr,(int, int));
dclproto(extern Tree lvalue,(Tree));
dclproto(extern Tree pointer,(Tree));
dclproto(extern Type promote,(Type));
dclproto(extern Tree right,(Tree, Tree));
dclproto(extern Tree rvalue,(Tree));
dclproto(extern Tree cvtconst,(Tree));
dclproto(extern void defglobal,(Symbol, int));
dclproto(extern void defpointer,(Symbol));
dclproto(extern void doconst,(Symbol, Generic));
dclproto(extern int genconst,(Tree, int));
dclproto(extern void initglobal,(Symbol, int));
dclproto(extern Type initializer,(Type, int));
dclproto(extern Tree structexp,(Type, Symbol));
dclproto(extern void swtoseg,(int));
dclproto(extern void inputInit,(int));
dclproto(extern void inputstring,(char *));
dclproto(extern void fillbuf,(void));
dclproto(extern void nextline,(void));
extern unsigned char *cp;
extern char *file;
extern char *firstfile;
extern unsigned char *limit;
extern char *line;
extern int lineno;
dclproto(extern int getchr,(void));
dclproto(extern int gettok,(void));
extern char kind[];
extern Coordinate src;
#ifdef __STDC__
extern enum tokencode t;
#else
extern int t;
#endif
extern char *token;
extern Symbol tsym;
dclproto(extern int main,(int, char **));
dclproto(extern Symbol mkstr,(char *));
dclproto(extern Symbol mksymbol,(int, char *,Type));
extern int Aflag;
extern int Pflag;
extern Symbol YYnull;
/*G*/extern int glevel;
extern int xref;
dclproto(void bbinit,(char *));
extern int ncalled;
extern int npoints;
dclproto(void traceinit,(char *));
typedef struct {
	List entry;
	List exit;
	List returns;
	List points;
	List calls;
	List end;
} Events;
extern Events events;
dclproto(typedef void (*Apply),(Generic, Generic, Generic));
dclproto(extern void attach,(Apply, Generic, List *));
dclproto(extern void apply,(List, Generic, Generic));
/*G*/dclproto(extern void fprint,(int, char *, ...));
/*G*/dclproto(extern void print,(char *, ...));
/*G*/dclproto(extern char *stringf,(char *, ...));
/*G*/dclproto(extern void outflush,(void));
dclproto(extern void outputInit,(int));
/*G*/dclproto(extern void outs,(char *));
dclproto(extern void vfprint,(int, char *, va_list));
dclproto(extern void vprint,(char *, va_list));
/*G*/extern char *bp;
dclproto(extern void error,(char *, ...));
/*G*/dclproto(extern int fatal,(char *, char *, int));
dclproto(extern void warning,(char *, ...));
dclproto(extern int expect,(int));
dclproto(extern void skipto,(int, char *));
dclproto(extern void test,(int, char *));
extern int errcnt;
extern int errlimit;
extern int wflag;
dclproto(extern int process,(char *));
dclproto(extern int findfunc,(char *, char *));
dclproto(extern int findcount,(char *, int, int));
dclproto(extern Tree asgnnode,(int, Tree, Tree));
dclproto(extern Tree bitnode,(int, Tree, Tree));
dclproto(extern Tree callnode,(Tree, Type, Tree));
dclproto(extern Tree condnode,(Tree, Tree, Tree));
dclproto(extern Tree constnode,(unsigned int, Type));
dclproto(extern Tree eqnode,(int, Tree, Tree));
dclproto(extern Tree shnode,(int, Tree, Tree));
dclproto(extern void typeerror,(int, Tree, Tree));
dclproto(extern Tree (*opnode[]),(int, Tree, Tree));
dclproto(extern Tree simplify,(int, Type, Tree, Tree));
dclproto(extern int ispow2,(unsigned u));
dclproto(extern char *vtoa,(Type, Value));
extern int needconst;
dclproto(extern void definelab,(int));
dclproto(extern Code definept,(Coordinate *));
dclproto(extern void equatelab,(Symbol, Symbol));
dclproto(extern void flushequ,(void));
dclproto(extern void retcode,(Tree, int));
dclproto(extern void statement,(int, Swtch, int));
extern float density;
extern float refinc;
/*G*/dclproto(extern char *allocate,(int, Arena *));
dclproto(extern void deallocate,(Arena *));
/*G*/extern Arena permanent;
/*G*/extern Arena transient;
dclproto(extern void typeInit,(void));
dclproto(extern Type array,(Type, int, int));
dclproto(extern Type atop,(Type));
dclproto(extern void checkfields,(Type));
dclproto(extern Type composite,(Type, Type));
dclproto(extern Symbol deftype,(char *, Type, Coordinate *));
dclproto(extern Type deref,(Type));
dclproto(extern int eqtype,(Type, Type, int));
dclproto(extern Field extends,(Type, Type));
/*G*/dclproto(extern Field fieldlist,(Type));
dclproto(extern Field fieldref,(char *, Type));
/*G*/dclproto(extern Type freturn,(Type));
dclproto(extern Type func,(Type, Type *, int));
dclproto(extern int hasproto,(Type));
dclproto(extern Field newfield,(char *, Type, Type));
dclproto(extern Type newstruct,(int, char *));
dclproto(extern void outtype,(Type));
dclproto(extern void printdecl,(Symbol, Type));
dclproto(extern void printproto,(Symbol, Symbol *));
dclproto(extern void printtype,(Type, int));
dclproto(extern Type ptr,(Type));
dclproto(extern Type qual,(int, Type));
dclproto(extern void rmtypes,(void));
/*G*/dclproto(extern int ttob,(Type));
dclproto(extern char *typestring,(Type, char *));
/*G*/dclproto(extern int variadic,(Type));
/*G*/extern Type chartype;
extern Type doubletype;
extern Type floattype;
/*G*/extern Type inttype;
extern Type longdouble;
extern Type longtype;
extern Type shorttype;
extern Type signedchar;
/*G*/extern Type unsignedchar;
extern Type unsignedlong;
/*G*/extern Type unsignedshort;
extern Type unsignedtype;
extern Type voidptype;
extern Type voidtype;
#endif
