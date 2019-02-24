#include	"libc.h"
#include	"fio.h"
#include	"regexp.h"

typedef struct Word
{
	char *s;
	struct Word *next;
} Word;
extern Word *newword(), *expand(), *wsubst(), *stow();

typedef struct Rule
{
	char *target;		/* one target */
	Word *tail;		/* constituents of targets */
	char *recipe;		/* do it ! */
	short attr;		/* attributes */
	short line;		/* source line */
	char *file;		/* source file */
	Word *alltargets;	/* all the targets */
	int rule;		/* rule number */
	regexp *pat;		/* reg exp goo */
	char *prog;		/* to use in out of date */
	struct Rule *chain;	/* hashed per target */
	struct Rule *next;
} Rule;
extern Rule *rules, *metarules, *patrule;

#define		META		0x0001
#define		SEQ		0x0002
#define		UPD		0x0004
#define		RED		0x0008
#define		QUIET		0x0010
#define		VIR		0x0020
#define		REGEXP		0x0040
#define		NOREC		0x0080
#define		DEL		0x0100

#define		NREGEXP		10

typedef struct Arc
{
	short flag;
	struct Node *n;
	Rule *r;
	char *stem;
	char *prog;
	regsubexp match[NREGEXP];
	struct Arc *next;
} Arc;
extern Arc *newarc();
#define		TOGO		1

typedef struct Node
{
	char *name;
	long time;
	unsigned short flags;
	Arc *prereqs;
	struct Node *next;	/* list for a rule */
} Node;
extern Node *graph();
#define		VIRTUAL		0x0001
#define		CYCLE		0x0002
#define		READY		0x0004
#define		CANPRETEND	0x0008
#define		PRETENDING	0x0010
#define		NOTMADE		0x0020
#define		BEINGMADE	0x0040
#define		MADE		0x0080
#define		MADESET(n,m)	n->flags = (n->flags&~(NOTMADE|BEINGMADE|MADE))|(m)
#define		PROBABLE	0x0100
#define		VACUOUS		0x0200
#define		NORECIPE	0x0400
#define		DELETE		0x0800

typedef struct Job
{
	Rule *r;		/* master rule for job */
	Node *n;		/* list of node targets */
	char *stem;
	regsubexp *match;
	Word *p;		/* prerequisites */
	Word *np;		/* new prerequisites */
	Word *t;		/* targets */
	Word *at;		/* all targets */
	int nproc;		/* slot number */
	int fd;			/* if redirecting */
	struct Job *next;
} Job;
extern Job *newjob(), *jobs;

typedef struct Symtab
{
	short space;
	char *name;
	char *value;
	struct Symtab *next;
} Symtab;
extern Symtab *symlook();
enum {
	S_VAR,		/* variable -> value */
	S_TARGET,	/* target -> rule */
	S_TIME,		/* file -> time */
	S_PID,		/* pid -> products */
	S_NODE,		/* target name -> node */
	S_AGG,		/* aggregate -> time */
	S_BITCH,	/* bitched about aggregate not there */
	S_NOEXPORT,	/* var -> noexport */
	S_OVERRIDE,	/* can't override */
	S_OUTOFDATE,	/* n1\377n2 -> 2(outofdate) or 1(not outofdate) */
	S_MAKEFILE,	/* target -> node */
	S_MAKEVAR,	/* dumpable mk variable */
};

extern int debug;
extern int nflag, tflag, iflag, kflag, aflag, mflag;
extern int inline;
extern char *infile;
extern char *Malloc(), *strndup();
extern long timeof();
extern void touch(), delete();
extern char *shname(), *vexpand();
extern int nproclimit;
extern int nreps;
extern char *explain;
extern char *envpr(), *wtos();
extern char *rulecnt();

#define	SYNERR(l)	(Fprint(2, "mk: %s:%d: syntax error; ", infile, (((l)>=0)?(l):inline)))
#define	RERR(r)	(Fprint(2, "mk: %s:%d: rule error; ", (r)->file, (r)->line))
#define	NAMEBLOCK	1000
#define	BIGBLOCK	20000

#ifndef	SHELL
#define	SHELL		"/bin/sh"		/* should be bourne-like */
#endif

#define	SEP(c)	(((c)==' ')||((c)=='\t')||((c)=='\n'))

#define	DEBUG(x)	(debug&(x))
#define		D_PARSE		0x01
#define		D_GRAPH		0x02
#define		D_EXEC		0x04

#define		EBIT		0x80	/* sorry japan */

#define	PERCENT(ch)	(((ch) == '%') || ((ch) == '&'))
