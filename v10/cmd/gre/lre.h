#ifndef	LRE_H
#define LRE_H

# if defined(__cplusplus)
extern "C" {	/* C++ 2.0 */
# endif

#include	"io.h"

#ifndef	MEMMOVE
#define	memmove(to, from, n)	memcpy(to, from, n)
#endif

#define		TRACE(n)	(n < re_debug)
#define		EPRINTSIZE	32767
extern int re_debug;

# if defined(__STDC__) || defined(c_plusplus) || defined(__cplusplus)
extern void eg_clrstates(re_re*);
extern Expr *eg_eall(enum Parsetype, unsigned char*);
extern void egbr(re_re*);
extern int egdfabr(re_re*, unsigned char*, unsigned char*, unsigned char**, unsigned char**);
extern int eg_match(re_re*, unsigned char*, unsigned char*, unsigned char**, unsigned char**);
extern void eginit(re_re*, int);
extern void *egmalloc(int, char*);
extern void egpost(re_re*);
extern void egcanon(Expr *);
extern re_re *egprep(enum Parsetype, unsigned char*, unsigned char*, unsigned char*, int);
extern void *egrealloc(char*, int, char*);
extern void eg_epr(Expr*, char*, int);
extern void eg_brpr(Br *);
extern int eg_getstate(re_re*);
extern void eg_lexinit(char*, char*);
extern void eg_lex(void);
extern Expr *eg_newexpr(Exprtype, int, Expr*, Expr*);
extern int eg_posalloc(re_re*, int);
extern void eg_posinit(re_re*);
extern void eg_posset(re_re*);
State *eg_startstate(re_re*);
State *eg_stateof(re_re*, Positionset*);
extern void eg_savestate(re_re*, State*);
extern void eg_spr(long, int*, char*);
extern void eg_stateinit(re_re*);
extern unsigned char *eg_quickmatch(re_re *, unsigned char *, unsigned char *, int);
extern unsigned char *eg_lquickmatch(re_re *, unsigned char *, unsigned char *, int);
#else
extern void clrstates();
extern Expr *eg_eall();
extern void egbr();
extern int egdfabr();
extern int eg_match();
extern void eginit();
extern char *egmalloc();
extern void egpost();
extern void egcanon();
extern re_re *egprep();
extern char *egrealloc();
extern void eg_epr();
extern void eg_brpr();
extern int eg_getstate();
extern void eg_lex();
extern void eg_lexinit();
extern Expr *eg_newexpr();
extern int eg_posalloc();
extern void eg_posinit();
extern void eg_posset();
extern void eg_savestate();
extern void eg_spr();
extern State *eg_startstate();
extern void eg_stateinit();
extern State *eg_stateof();
extern unsigned char *eg_quickmatch();
extern unsigned char *eg_lquickmatch();
# endif

# if defined(__cplusplus)
}		/* C++ 2.0 */
# endif
#endif
