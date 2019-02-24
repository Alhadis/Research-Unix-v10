#ifndef	RE_H
#define RE_H

# if defined(__cplusplus)
extern "C" {	/* C++ 2.0 */
# endif

typedef struct re_bm
{
	int delta0[256], *delta2;
	unsigned char cmap[256];
	char *bmpat;
	int patlen;
} re_bm;

typedef struct re_cw
{
	int maxdepth, mindepth;
	char seenerror;		/* set if we called re_error */
	long nodeid;
	int step[256];
	unsigned char map[256];
	struct Node *root;
} re_cw;

typedef enum
{
	Literal, Dot, Carat, Dollar, Charclass, Compcharclass,		/* 0-5 */
	Cat, Alternate, Star, Plus, Quest, Backref, Group, EOP,		/* 6-13 */
	/* not in grammar, just helping */
	Lpar, Rpar, Backslash, Null
} Exprtype;

typedef struct Expr
{
	Exprtype type;
	char reallit;		/* just for dollar and -G, dammit! */
	char backref;		/* backref used here or below */
	char parens;		/* parens used here or below */
	char seenerror;		/* set if we called re_error */
	int id;
	unsigned int lit;
	long flen;
	int *follow;
	struct Expr *l, *r, *parent;
} Expr;
typedef enum Parsetype { greparse, grepparse, egrepparse } Parsetype;

#define		RE_DOLLAR	256
#define		RE_CARAT	257
#define		RE_HIGH		258	/* always 1+last constant */

typedef struct State
{
	struct State *tab[RE_HIGH];
	char out;	/* matched */
	char init;	/* inital state */
	long npos;
	int pos;	/* index into posbase */
} State;

typedef struct Positionset
{
	long count;
	int last;
	int *base;
} Positionset;

typedef enum {
	br_re, br_group, br_br, br_cat, br_alt, br_star, br_plus, br_quest
} Br_type;

typedef struct Br
{
	Br_type type;
	Expr *e;
	int group;
	struct re_re *r;
	struct Br *lb, *rb;
} Br;

typedef struct re_re
{
	int *posbase;
	int nposalloc, posnext, posreset;
	int maxid;
	Expr *root;
	Expr **ptr;
	unsigned char mymap[256];
	Positionset firstpos, begin, tmp;
	int nstates, statelim;
	State *states;
	State istate;
	int initialstate;
	int carat;
	int flushed;
	int threshhold;		/* resize cache every threshhold flushes */
	int backref;
	int parens;
	Br *br;
} re_re;

/*
	matching routine endpoint markers
*/
#define		RE_BEG		1		/* beginning matches ^ */
#define		RE_END		2		/* end matches $ */

# ifdef USE_STDIO
#  include <stdio.h>
#  if defined(__STDC__) || defined(c_plusplus) || defined(__cplusplus)
extern void re_refile(re_re*, FILE*);
extern re_re *re_filere(FILE*);
#  else
extern void re_refile();
extern re_re *re_filere();
#  endif
# endif /* USE_STDIO */

# if defined(__STDC__) || defined(c_plusplus) || defined(__cplusplus)
# define VOID void
typedef int (*RDFN)(char**, char**);
typedef int (*MATCHFN)(char**,char**);
typedef int (*PROCFN)(VOID*, RDFN, MATCHFN);
extern re_bm *re_bmcomp(char*, char*, unsigned char*);
extern int re_bmexec(VOID*, RDFN, MATCHFN);
extern void re_bmfree(re_bm*);
extern void re_cwadd(re_cw*, unsigned char*, unsigned char*);
extern void re_cwcomp(re_cw*);
extern int re_cwexec(VOID*, RDFN, MATCHFN);
extern void re_cwfree(re_cw*);
extern re_cw *re_cwinit(unsigned char*);
extern void re_error(char*);
extern int re_paren(re_re *e);
extern re_re *re_recomp(char*, char*, unsigned char*);
extern re_cw *re_recw(re_re*, unsigned char*);
extern int re_reexec(re_re*, char*, char*, char*[10][2]);
extern void re_refree(re_re*);
#else
# define VOID char
typedef int (*RDFN)();
typedef int (*MATCHFN)();
typedef int (*PROCFN)();
extern re_bm *re_bmcomp();
extern int re_bmexec();
extern void re_bmfree();
extern void re_cwadd();
extern void re_cwcomp();
extern int re_cwexec();
extern void re_cwfree();
extern re_cw *re_cwinit();
extern void re_error();
extern int re_paren();
extern re_re *re_recomp();
extern re_cw *re_recw();
extern int re_reexec();
extern void re_refree();
# endif

# if defined(__cplusplus)
}		/* C++ 2.0 */
# endif
#endif
