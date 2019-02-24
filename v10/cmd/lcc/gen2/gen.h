/* C compiler: back-end structures */

#define NODAG 0
#define RX		2		/* symbol holding target register */
#define MAXSYMS 	4		/* symbols/node */
#define MAXINTS 	5		/* codes/node */
#define MAXKIDS 	6		/* children/node */

#ifdef DEBUG
#define dumptree(x,y) dumptree1(x,y,-1,0)
#else
#define dumptree(x,y)
#endif

typedef struct regnode *Regnode;

typedef struct {		/* back end's block record */
	int off;			/* offset for locals */
	unsigned freemask[3];		/* free registers */
} Env;

typedef struct {
	char *exportfmt;
	unsigned twoop:1;
	unsigned char maxunalignedload;
	char *segnames[5];
	dclproto(void (*doarg),(Node));
	dclproto(void (*docall),(Node));
	dclproto(void (*blkfetch),(int, int, int, int));
	dclproto(void (*blkloop),(int, int, int, int, int, int[]));
	dclproto(void (*blkstore),(int, int, int, int));
	dclproto(void (*final),(Node));
	dclproto(void (*rewrite),(Node));
	dclproto(void (*emitspecial),(Node));
} Xinterface;

struct real {			/* canonical real */
	unsigned sign;
	unsigned exp;			/* unbiased exponent */
	unsigned msb;			/* most significant fraction bits */
	unsigned lsb;			/* least significant fraction bits */
};

struct regnode {		/* register */
	unsigned char regtype;		/* type extension for loads */
	unsigned char regset;		/* register set */
	unsigned char regnum;		/* register number */
	int count;			/* number of uses left; zero if free; negative if register vbl */
	unsigned mask;			/* underlying basic registers */
	Node node;			/* node assigned to this register */
	Symbol link;			/* next register in this class */
	Symbol next;			/* next register in any class */
};

typedef struct {		/* back end's node fields */
	unsigned loadaddr:1;
	unsigned sideeffect:1;
	unsigned unavailable:1;
	unsigned zap0:1;
	unsigned linearized:1;
	unsigned linfrontier:1;
	unsigned lininterior:1;
	unsigned registered:1;
	unsigned listed:1;
	unsigned iscall:1;
	unsigned isarg:1;
	unsigned isvararg:1;
	unsigned frontier:1;
	unsigned char ints[MAXINTS];	/* target-specific function, type and addressing codes */
	char perm[MAXKIDS];
	unsigned char delay;
	Symbol rtarget;
	Node args;
	Node next;			/* next node in output order */
} Xnode;

typedef struct {		/* back end's symbol fields */
	char *name;			/* name for back end */
	int offset;			/* offset for locals, formals */
	Symbol regvar;			/* register allocated to this symbol, if any */
	Symbol nextregvar;		/* links function's register variables */
	Regnode regnode;		/* mkregs's register id, if any */
	unsigned int eaddr;	/* effective address of symbol */
} Xsymbol;

#define atst(p) ((p)->x.loadaddr)
#define isregvar(p) ((p)->sclass == REGISTER)
#define setreg(p,r) ((p)->syms[RX]=r)
#define symeaddr(s) ((s)->x.eaddr)

/* gen.c */
dclproto(extern void blockbeg,(Env *));
dclproto(extern void blockend,(Env *));
dclproto(extern void export,(Symbol));
dclproto(extern Node gen,(Node));
dclproto(extern void progend,(void));

dclproto(extern int aset,(Node, int));
dclproto(extern int bitcount,(unsigned));
dclproto(extern void blkcopy,(int, int, int, int, int, int[]));
dclproto(extern void blkunroll,(int, int, int, int, int, int, int[]));
dclproto(extern void docall,(Node));
dclproto(extern void dumpregs,(char *, char *, char *));
dclproto(extern void dumptree1,(Node, int, int, int));
dclproto(extern void freeregvars,(void));
dclproto(extern void genascii,(char*, char*, int, char*));
dclproto(extern int getint,(Symbol));
dclproto(extern Regnode getregnode,(Symbol));
dclproto(extern int getregset,(Symbol));
dclproto(extern int getregvar,(Symbol, Symbol));
dclproto(extern unsigned getunsigned,(Symbol));
dclproto(extern void initfunc,(int, int, int));
dclproto(extern int lop,(Node, int, int));
dclproto(extern int mkactual,(int, int, int));
dclproto(extern int mkauto,(Symbol, int));
dclproto(extern Symbol mkregs,(char *, int, int, int, int, int, int, Symbol));
dclproto(extern void mvregvars,(void));
dclproto(extern void parseflags,(int, char **));
dclproto(extern int range,(int, int));
dclproto(extern int regchain,(Node));
dclproto(extern unsigned regloc,(Symbol));
dclproto(extern int rmcopy,(Node));
dclproto(extern void rtarget,(Node, int, Symbol));
dclproto(extern void setseg,(int));
dclproto(extern void spacen,(int, char *));
dclproto(extern void spill,(unsigned, int, Node));
extern int argbuildsize;
extern int argoffset;
extern char **asmstr[MAXINTS];
extern unsigned eemask[];
extern unsigned freemask[];
extern Node last;
extern int maxoffset;
extern int ncalls;
extern int offset;
extern int bflag, dflag, kflag, pflag, rflag;
extern int nregsets;
extern char **opcodes;
extern Symbol regs;
extern Symbol regvars;
extern Symbol rmap[];
extern int savebase[];
extern unsigned savemask[];
extern unsigned tempmask[];
extern unsigned tmask[];
extern unsigned usedmask[];
extern unsigned vmask[];
extern int dalign, salign;
extern int genx;
/* decode.c */
dclproto(struct real decode,(int, double));
#ifdef DEBUG
#define debug(f,x) if(f)x
#else
#define debug(f,x)
#endif
