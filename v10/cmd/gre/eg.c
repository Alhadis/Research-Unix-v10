#include	"re.h"
#include	"lre.h"
#include	"hdr.h"

int re_debug = 0;

#define	RESET(r, ps)	COPY(r, ps, begin)
#define	SET(r, ps, n)	{if(r->ps.base[n] == 0) r->ps.count++, r->ps.base[n] = r->ps.last, r->ps.last = n; }
#define	GET(ps, n)	for(n = ps.last; n > 0; n = ps.base[n])
#define	COPY(r, to, from)	memmove((char *)r->to.base, (char *)r->from.base, r->maxid*sizeof(int)), r->to.count = r->from.count, r->to.last = r->from.last

static State *addstate(re_re *r, Positionset *ps);
static int first(re_re *, Expr *);
static int match(re_re *, Expr *, int);
static State *nextstate(re_re *, State *, int);
#ifdef	DEBUG
static void ppr(Positionset *, char *);
#endif

typedef enum { NORMAL, ACCEPT, ACCEPT_EOP, FAIL } Out;

static void
eptr(register re_re *r, register Expr *e)
{
	if((e->id < 0) || (e->id >= r->maxid)){
		EPR "eptr abort: r=%ld[maxid=%d] e=%ld[id=%d]\n", r, r->maxid, e, e->id);
		abort();
	}
	r->ptr[e->id] = e;
	if((e->type != Charclass) && (e->type != Compcharclass)){
		if(e->l)
			eptr(r, e->l);
		if(e->r)
			eptr(r, e->r);
	}
}

re_re *
egprep(enum Parsetype parser, unsigned char *b, unsigned char *e, unsigned char *map, int dotstar)
{
	register re_re *r;
	Expr *ecomp;
	int i;

	r = (re_re *)egmalloc(sizeof (re_re), "allocating re_re");
	if(r == 0)
		return 0;
	memset((char *)r, 0, sizeof (re_re));
	eg_lexinit((char *)b, (char *)e);
	if(map)
		memmove(r->mymap, (char *)map, 256);
	else
		for(i = 0; i < 256; i++)
			r->mymap[i] = i;
	eg_lex();
	if(dotstar){
		ecomp = eg_newexpr(Star, 0, eg_newexpr(Dot, '.', (Expr *)0, (Expr *)0), (Expr *)0);
		ecomp = eg_newexpr(Cat, 0, ecomp, eg_eall(parser, r->mymap));
	} else
		ecomp = eg_eall(parser, r->mymap);
	r->root = eg_newexpr(EOP, '#', ecomp, (Expr *)0);
	egpost(r);
	r->carat = 0;
	if(r->backref || r->parens)
		egbr(r);
	else
		eginit(r, dotstar);
	return(r);
}

void
eginit(register re_re *r, int dotstar)
{
	int n;

#ifdef	DEBUG
	if(TRACE(6))
		PR "eginit(r=%d, dotstar=%d)\n", r, dotstar);
#endif
#ifdef	DEBUG
	if(TRACE(10)){
		char buf[EPRINTSIZE];
		eg_epr(r->root, buf, 0);
		PR "eginit: r=%d expr='%s'\n", r, buf);
	}
#endif
	r->ptr = (Expr **)egmalloc(r->maxid*sizeof(Expr *), "ptr");
	if (!r->ptr)
		return;
	eptr(r, r->root);
	r->firstpos.base = (int *)egmalloc(n = r->maxid*sizeof(int), "first base");
	if (!r->firstpos.base){
		free((char*)r->ptr);
		return;
	}
	r->begin.base = (int *)egmalloc(n, "begin base");
	if (!r->begin.base){
		free((char*)r->firstpos.base);
		free((char*)r->ptr);
		return;
	}
	r->tmp.base = (int *)egmalloc(n, "tmp base");
	if (!r->tmp.base){
		free((char*)r->begin.base);
		free((char*)r->firstpos.base);
		free((char*)r->ptr);
		return;
	}
	memset((char *)r->begin.base, 0, n);
	r->begin.count = 0;
	r->begin.last = -1;
	r->carat = dotstar;
	RESET(r, firstpos);
	if(first(r, r->root->l) == 0){
		/*
			nullable, so include me
		*/
		SET(r, firstpos, r->root->id)
	}
	if(dotstar)
		COPY(r, begin, firstpos);
	eg_stateinit(r);
	eg_clrstates(r);
	eg_posinit(r);
	(void)addstate(r, &r->firstpos);
	eg_savestate(r, dotstar? nextstate(r, r->states, RE_CARAT):r->states);
	eg_posset(r);
}

unsigned char *
eg_quickmatch(register re_re *r, register unsigned char *b, register unsigned char *e, int endpts)
{
	register State *s, *t;

#ifdef	DEBUG
	if(TRACE(10)){
		char buf[EPRINTSIZE];
		eg_epr(r->root, buf, 0);
		PR "qm: r=%d expr='%s' endpts=%d\n", r, buf, endpts);
	}
#endif
	s = eg_startstate(r);
	if(endpts&RE_BEG){
		if(!r->carat){
			if(t = s->tab[RE_CARAT])
				s = t;
			else
				s = nextstate(r, s, RE_CARAT);
			if(s->out == FAIL)
				s = eg_startstate(r);
		}
		if(s->out){
#ifdef	DEBUG
			if(TRACE(6))
				PR "match at ^: '%s' out=%d\n", b, s->out);
#endif
			return((s->out == FAIL)? 0:b);
		}
	}
	while(b < e){
#ifdef	DEBUG
		if(TRACE(4))
			PR "state %d@%d[%d pos]: char '%c'\n", s-r->states, (int)s, s->npos, *b);
#endif
		if(t = s->tab[*(unsigned char *)b])
			s = t;
		else
			s = nextstate(r, s, *(unsigned char *)b);
		if(s->out){
#ifdef	DEBUG
			if(TRACE(6))
				PR "match at input '%s' out=%d\n", b, s->out);
#endif
			return((s->out == FAIL)? 0:b);
		}
		b++;
	}
	if(endpts&RE_END){
		if(t = s->tab[RE_DOLLAR])
			s = t;
		else
			s = nextstate(r, s, RE_DOLLAR);
	}
	if(s->out){
#ifdef	DEBUG
		if(TRACE(6))
			PR "match at $ '%s' out=%d\n", b, s->out);
#endif
		return((s->out == FAIL)? 0:b);
	}
#ifdef	DEBUG
	if(TRACE(3)){
		char buf[EPRINTSIZE];

		eg_epr(r->root, buf, 1);
		PR "pat = %s\n", buf);
	}
#endif
	return(0);
}

unsigned char *
eg_lquickmatch(register re_re *r, register unsigned char *b, register unsigned char *e, int endpts)
{
	register State *s, *t;
	int outedness = 0;

#ifdef	DEBUG
	if(TRACE(10)){
		char buf[EPRINTSIZE];
		eg_epr(r->root, buf, 0);
		PR "lqm: r=%d carat=%d expr='%s' endpts=%d\n", r, r->carat, buf, endpts);
	}
#endif
	s = eg_startstate(r);
	if(endpts&RE_BEG){
		if(!r->carat){
			if(t = s->tab[RE_CARAT])
				s = t;
			else
				s = nextstate(r, s, RE_CARAT);
			if(s->out == FAIL)
				s = eg_startstate(r);
		}
		if(s->out){
#ifdef	DEBUG
			if(TRACE(6))
				PR "match at ^: '%s' out=%d\n", b, s->out);
#endif
			if(s->out == ACCEPT_EOP)
				return(b);
			else if(s->out == FAIL)
				return(0);
			outedness = 1;
		}
	}
	/*
		look for first match
	*/
	if(outedness == 0)
	for(; b < e; b++){
#ifdef	DEBUG
		if(TRACE(4))
			PR "state %d@%d[%d pos]: char '%c'\n", s-r->states, (int)s, s->npos, *b);
#endif
		if(t = s->tab[*(unsigned char *)b])
			s = t;
		else
			s = nextstate(r, s, *(unsigned char *)b);
		if(s->out){
#ifdef	DEBUG
			if(TRACE(4))
				PR "	out=%d, state %d@%d\n", s->out,
					s-r->states, (int)s);
#endif
			b++;
			if((s->out == ACCEPT_EOP) || (s->out == FAIL))
				goto finish;
			outedness = 1;
			break;
		}
	}
	/*
		in the following loop, we have seen a match already
		because only way outedness is zero is if b>=e
	*/
	for(; b < e; b++){
#ifdef	DEBUG
		if(TRACE(4))
			PR "statex %d@%d[%d pos]: char '%c'\n", s-r->states, (int)s, s->npos, *b);
#endif
		if(t = s->tab[*(unsigned char *)b])
			s = t;
		else
			s = nextstate(r, s, *(unsigned char *)b);
		if(s->out == ACCEPT)
			continue;
		if((s->out == NORMAL) || (s->out == FAIL)){
#ifdef	DEBUG
			if(TRACE(6))
				PR "unmatch at input '%s' out=%d\n", b, s->out);
#endif
			return(b);
		}
	}
	if(endpts&RE_END){
		if(t = s->tab[RE_DOLLAR])
			s = t;
		else
			s = nextstate(r, s, RE_DOLLAR);
	}
finish:
	if((s->out && (s->out != FAIL)) || outedness){
#ifdef	DEBUG
		if(TRACE(6))
			PR "match at $ '%s' out=%d\n", b, s->out);
#endif
		return(b);
	}
#ifdef	DEBUG
	if(TRACE(3)){
		char buf[EPRINTSIZE];

		eg_epr(r->root, buf, 1);
		PR "lqm('%s') failed; out=%d, s->out=%d \n", buf, outedness, s->out);
	}
#endif
	return(0);
}

static
match(register re_re *r, register Expr *e, int a)
{
	switch(e->type)
	{
	case Dollar:	return(a == RE_DOLLAR);
	case Carat:	return(a == RE_CARAT);
	case Dot:	return(a < 256);
	case Literal:	return(r->mymap[a] == r->mymap[e->lit]);
	case Charclass:	if(a >= 256) return(0);
			return(memchr((char *)e->r, r->mymap[a], (int)e->l) != 0);
	case Compcharclass:
			if(a >= 256) return(0);
			return(memchr((char *)e->r, r->mymap[a], (int)e->l) == 0);
	}
	return(0);
}

	/*
		generates the followset for a node in firstpos
	*/

static void
follow(register re_re *r, register Expr *e)
{
	register Expr *p;

	if(e->type == EOP)
		return;
	else
		p = e->parent;
	switch(p->type)
	{
	case EOP:
		SET(r, firstpos, p->id)
		break;
	case Plus:
	case Star:
		(void)first(r, e);
		follow(r, p);
		break;
	case Quest:
	case Alternate:
		follow(r, p);
		break;
	case Cat:
		if(e == p->l){
			if(first(r, p->r) == 0){
				follow(r, p);
				break;
			}
		} else
			follow(r, p);
		break;
	case Group:
		follow(r, p);
		break;
	}
}

	/*
		first returns 0 if e is nullable and in the process,
		sets up firstpos.
	*/

static
first(register re_re *r, register Expr *e)
{
	int k;

	switch(e->type)
	{
	case Carat:
	case Dollar:
	case Literal:
	case Dot:
	case Charclass:
	case Compcharclass:
		SET(r, firstpos, e->id)
		return(1);
	case EOP:
	case Star:
	case Quest:
		(void)first(r, e->l);
		return(0);
	case Group:
	case Plus:
		return(first(r, e->l));
	case Cat:
		return(first(r, e->l) || first(r, e->r));
	case Alternate:
		k = first(r, e->r);
		return(first(r, e->l) && k);
	default:
		EPR "bad type %d\n", e->type);
		abort();
		return(0);
	}
}

static void
efollow(register re_re *r, register Expr *e)
{
	register i, *p;

	RESET(r, firstpos);
	follow(r, e);
	e->flen = r->firstpos.count;
	e->follow = (int *)malloc((unsigned)(e->flen*sizeof(int)));
	if(e->follow == 0){
		char buf[100];
		SPR buf, "efollow malloc fail(%ld)\n", e->flen);
		re_error(buf);
		return;
	}
	p = e->follow;
	GET(r->firstpos, i)
		*p++ = i;
	if(p != e->follow+e->flen){
		char buf[100];
		SPR buf, "efollow length error: %d %ld\n", p-e->follow, e->flen);
		re_error(buf);
		return;
	}
}

static State *
addstate(register re_re *r, register Positionset *ps)
{
	register *p, j;
	register State *s;

	s = r->states + eg_getstate(r);
	memset((char *)s->tab, 0, sizeof(s->tab));
	s->pos = eg_posalloc(r, (int)ps->count);
	s->npos = ps->count;
	p = r->posbase+s->pos;
	GET((*ps), j)
		*p++ = j;
	if(s->out = (ps->base[r->root->id] != 0)){
		if((ps->base[r->root->id] <= 0) && (ps->last == r->root->id))
			s->out = ACCEPT_EOP;	/* marker for only the EOP marker */
		else
			s->out = ACCEPT;
	}
	if(s->npos == 0)
		s->out = FAIL;
#ifdef	DEBUG
	if(TRACE(3)){
		char buf[2000];
		eg_spr(s->npos, s->pos+r->posbase, buf);
		PR "new state[%d]@%d %s,pos=%d out=%d\n", s-r->states, s, buf, s->pos, s->out);
	}
#endif
	return(s);
}

static State *
nextstate(register re_re *r, register State *s, int a)
{
	register int p, *q, *eq;
	register long i;
	State *news;
	Expr *e;

	RESET(r, tmp);
#ifdef	DEBUG
	if(TRACE(5)){
		char buf[2000];
		ppr(&r->tmp, buf);
		PR "nextstate: reset: %s\n", buf);
	}
#endif
	for(i = s->npos, p = s->pos; i > 0; i--){
		e = r->ptr[r->posbase[p++]];
#ifdef	DEBUG
		if(TRACE(11)){
			PR "i=%d e->type=%d\n", i, e->type);
		}
#endif
		if(e->type == EOP) continue;
		if(match(r, e, a)){
#ifdef	DEBUG
			if(TRACE(11)){PR "matched %c\n", a);}
#endif
			if(e->follow == 0)
				efollow(r, e);
			for(q = e->follow, eq = q+e->flen; q < eq; q++){
				SET(r, tmp, *q)
			}
		}
	}
#ifdef	DEBUG
	if(TRACE(5)){
		char buf[2000];
		ppr(&r->tmp, buf);
		PR "nextstate(%d, '%c'): found %s\n", s-r->states, a, buf);
	}
#endif
	if(news = eg_stateof(r, &r->tmp)){
		if(a <= RE_DOLLAR)
			s->tab[a] = news;
	} else
		news = addstate(r, &r->tmp);
#ifdef	DEBUG
	if(TRACE(5)){
		PR "nextstate(%d, '%c'): returning %ld %d out=%d\n", s-r->states, a,
			(long)news, news-r->states, news->out);
	}
#endif
	return(news);
}

void *
egmalloc(int n, char *s)
{
	char *x;
	char buf[256];

	if((x = malloc(n)) == 0){
		SPR buf, "malloc fail(%d): %s", n, s);
		re_error(buf);
		return 0;
	}
	return((void *)x);
}

void *
egrealloc(char *p, int n, char *s)
{
	char *x;
	char buf[256];

	if((x = realloc(p, n)) == 0){
		SPR buf, "realloc fail(%d): %s", n, s);
		re_error(buf);
		return 0;
	}
	return((void *)x);
}

#ifdef	DEBUG
static void
ppr(register Positionset *ps, register char *p)
{
	register n;

	if(ps->count < 1){
		SPR p, "{}");
		return;
	}
	*p++ = '{';
	GET((*ps), n){
		SPR p, "%d[=%d],", n, ps->base[n]);
		p = strchr(p, 0);
	}
	p[-1] = '}';
}
#endif
