/*	static char  ID[] = "@(#) optim.h: 1.7 1/20/84";	*/

/*	machine independent include file for code improver */

#include <stdio.h>
#include <ctype.h>
#include "defs"

#ifndef MEMFCN	/* remove after 5.0 when memcpy is in all UNIX releases */
#define memcpy	strncpy
#endif

#ifndef MAXOPS
#define MAXOPS	1
#endif

/* booleans */

typedef int boolean;
#define false	0
#define true	1

/* predefined "opcodes" for various nodes */

#define GHOST	0		/* temporary, to prevent linking node in */
#define TAIL	0		/* end of text list */
#define MISC	1		/* miscellaneous instruction */
#define FILTER	2		/* nodes to be filtered before optim */

#ifdef LIVEDEAD
extern void ldanal();
#if LIVEDEAD - 0 < 2
#undef LIVEDEAD
#define LIVEDEAD	16
#endif
#endif

/* structure of each text node */

typedef struct node {
	struct node *forw;	/* forward link */
	struct node *back;	/* backward link */
	char *ops[MAXOPS + 1];	/* opcode or label and operand field strings */
#ifdef IDVAL
	IDTYPE uniqid;		/* unique identification for this node */
#endif
	unsigned short op;	/* operation code */
#ifdef LIVEDEAD
	unsigned
	    nlive:LIVEDEAD,	/* registers used by instruction */
	    ndead:LIVEDEAD;	/* registers set but not used by instruction */
#endif
#ifdef USERDATA
	USERTYPE userdata;	/* user-defined data for this node */
#endif
} NODE;

#define opcode	ops[0]
#define op1	ops[1]
#if MAXOPS > 1
#define op2	ops[2]
#if MAXOPS > 2
#define op3	ops[3]
#if MAXOPS > 3
#define op4	ops[4]
#if MAXOPS > 4
#define op5	ops[5]
#if MAXOPS > 5
#define op6	ops[6]
#endif
#endif
#endif
#endif
#endif

/* structure of non-branch text reference node */

typedef struct ref {
	char *lab;		/* label referenced */
	struct ref *nextref;	/* link to next ref */
} REF;

/* externals */

extern NODE n0;			/* header node of text list */
extern NODE ntail;		/* trailer node of text list */
extern NODE *lastnode;		/* pointer to last node on text list */
extern REF *lastref;		/* pointer to last label reference */

extern int dflag;		/* display live-dead info */
extern int sflag;		/* statistics (on stderr) */
extern int ndisc;		/* # of instructions discarded */
extern int ninst;		/* total # of instructions */
extern int npass;		/* # of times through this function */
extern int fnum;		/* # of this function */

extern NODE *Saveop();
extern boolean same(), sameaddr();
extern char *getspace(), *strcpy(), *memcpy(), *xalloc(), *memset();
extern void addref(), fatal(), filter(), init(), optim(), prtext(), xfree();

/* user-supplied functions or macros */

#ifndef getp
extern char *getp();
#endif
#ifndef newlab
extern char *newlab();
#endif

#define saveop(opn, str, len, op) \
	    (void) Saveop((opn), (str), (unsigned)(len), (op))
#define addtail(p)		/* superfluous */
#define appinst()		/* superfluous */
#define appmisc(str, len)	saveop(0, (str), (len), MISC)
#define appfl(str, len)		saveop(0, (str), (len), FILTER)
#define applbl(str, len) \
	(setlab(Saveop(0, (str), (unsigned)(len), MISC)), --ninst)
#define ALLN(p)			p = n0.forw; p != &ntail; p = p->forw
#define PRINTF			(void) printf
#define FPRINTF			(void) fprintf
#define SPRINTF			(void) sprintf
#define PUTCHAR(c)		(void) putchar(c)
#define DELNODE(p)		((p)->back->forw = (p)->forw, \
				    (p)->forw->back = (p)->back)
#define APPNODE(p, q)		PUTNODE((p), (q), back, forw)
#define INSNODE(p, q)		PUTNODE((p), (q), forw, back)
#define PUTNODE(p, q, f, b)	((p)->f = (q), (p)->b = (q)->b, \
				    (q)->b = (q)->b->f = (p))
#define GETSTR(type)		(type *) getspace(sizeof(type))
#define COPY(str, len)	((len) != 0 ? memcpy(getspace(len), str, (int)(len)) : str)
#ifndef ISLABREF
#define ISLABREF(p)	0
#endif
