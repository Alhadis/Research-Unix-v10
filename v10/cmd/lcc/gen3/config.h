/* C compiler: configuration parameters for VAX subset code generator */

/* include config */
typedef int Xinterface;
/* end config */

/* include Env */
typedef struct {
	unsigned rmask;
	int offset;
} Env;
/* end Env */
/* include Xnode */
typedef struct {
	short lev;          /* node nesting level (omit) */
	short id;           /* node identification number (omit) */
	int state;
	unsigned visited:1; /* 1 if dag has been linearized */
	int reg;            /* register number */
	unsigned rmask;     /* unshifted register mask */
	unsigned busy;      /* busy regs */
	int argoffset;      /* ARG: argument offset */
	Node next;          /* next node on emit list */
} Xnode;
/* end Xnode */
/* include Xsymbol */
typedef struct {
	char *name;		/* name for back end */
	int offset;		/* frame offset */
} Xsymbol;
/* end Xsymbol */

#ifdef vax
dclproto(extern double atof,(char *));
#define strtod(a,b) atof(a)
#endif
