#include	"re.h"
#include	"lre.h"
#include	"hdr.h"

static void traverse(Expr *);

#define	MAXLIT	256	/* is plenty big enough */
static unsigned char tmp[MAXLIT], best[MAXLIT];
static unsigned char *p;
static int bestlen;
#define	START	{ p = tmp ; }
#define	ADD(c)	{ if(p >= &tmp[MAXLIT]) p--; *p++ = c; }
#define	FINISH	{ ADD(0) if((p-tmp) > bestlen) memmove((char *)best, (char *)tmp, bestlen = p-tmp); }

re_lit(re_re *r, unsigned char **b, unsigned char **e)
{
	bestlen = 0;
	START
	traverse(r->root);
	FINISH
	if(bestlen < 3)
		return(0);
	*b = best;
	*e = best+bestlen-1;
	return(1);
}

static void
traverse(register Expr *e)
{
	switch(e->type)
	{
	case Literal:
		ADD(e->lit)
		break;
	case Charclass:
		if((int)e->l == 1)
			ADD(*(char *)e->r)
		else {
			FINISH
			START
		}
		break;
	case Cat:
		traverse(e->l);
		traverse(e->r);
		break;
	case Plus:
		traverse(e->l);
		FINISH	/* can't go on past a + */
		START	/* but we can start with one! */
		traverse(e->l);
		break;
	case EOP:
		FINISH
		START
		traverse(e->l);
		break;
	default:
		FINISH
		START
		break;
	}
}
