#include	"re.h"
#include	"lre.h"
#include	"hdr.h"

typedef struct Link
{
	unsigned char l;
	struct Node *node;
	struct Link *next;
} Link;
static Link *froot;

#define	NEW(N)	(froot?(t = froot, froot = froot->next, t->next = 0, t->node = N, t): newlink(c, 0, N))
#define	ADD(N)	if(qtail) qtail = qtail->next = NEW(N);\
			else qtail = qhead = NEW(N)
#define	DEL()	{ Link *tmp = qhead;if((qhead = qhead->next) == 0)\
			qtail = 0; tmp->next = froot; froot = tmp;}

typedef struct Node
{
	short out;
	short d;
	short shift1;
	short shift2;
	long id;
	struct Node **tab;
	Link *alts;
	struct Node *fail;
} Node;

static Link *newlink(re_cw*, int, Node*);
static Node *newnode(re_cw*, int);
static void zeroroot(Node*, Node*);
static void shifttab(Node*);
static void shiftprop(re_cw*,Node*);
#ifdef	DEBUG
static void cwpr(register Node *n);
#endif

re_cw *
re_cwinit(unsigned char *cmap)
{
	register i;
	register re_cw *c;

	if((c = (re_cw *)egmalloc(sizeof *c, "malloc failed in re_cwinit")) == 0)
		return((re_cw *)0);
	c->nodeid = 0;
	c->maxdepth = 0;
	c->mindepth = 10000;
	c->seenerror = 0;
	c->root = newnode(c, 0);
	if(cmap)
		memmove((char *)c->map, (char *)cmap, sizeof c->map);
	else
		for(i = 0; i < sizeof c->map; i++)
			c->map[i] = i;
	return(c);
}

void
re_cwadd(register re_cw *c, register unsigned char *s, register unsigned char *e)
{
	register Node *p, *state;
	register Link *l;
	int depth;

	for(state = c->root; s <= --e;){
		for(l = state->alts; l; l = l->next)
			if(l->l == c->map[*e]) break;
		if(l == 0)
			break;
		else
			state = l->node;
	}
	if(s <= e){
		depth = state->d+1;
		l = newlink(c, *e--, p = newnode(c, depth++));
		if((l == 0) || (p == 0)){
			c->seenerror = 1;
			return;
		}
		l->next = state->alts;
		state->alts = l;
		state = p;
		while(s <= e){
			state->alts = newlink(c, *e--, p = newnode(c, depth++));
			if((state->alts == 0) || (p == 0)){
				c->seenerror = 1;
				return;
			}
			state = p;
		}
	}
	if(c->mindepth > state->d)
		c->mindepth = state->d;
	state->out = 1;
}

#ifdef	DEBUG
static void
cwpr(register Node *n)
{
	register Link *l;

	Fprint(1, "%d[%d,%d]: ", n->id, n->shift1, n->shift2);
	for(l = n->alts; l; l = l->next){
		Fprint(1, "edge from \"%d\" to \"%d\" label {\"%c\"};",
			n->id, l->node->id, l->l);
		if(l->node->out) Fprint(1, " draw \"%d\" as Doublecircle;", l->node->id);
		if(l->node->fail)
			Fprint(1, " edge from \"%d\" to \"%d\" dashed;", l->node->id, l->node->fail->id);
		Fprint(1, "\n");
		cwpr(l->node);
	}
}
#endif

static void
fail(re_cw *c, Node *root)
{
	Link *qhead = 0, *qtail = 0;
	register Link *l, *ll;
	register Link *t;
	register Node *state, *r, *s;
	int a;

	for(l = root->alts; l; l = l->next){
		ADD(l->node);
		l->node->fail = root;
	}
	while(qhead){
		r = qhead->node;
		DEL();
		for(l = r->alts; l; l = l->next){
			s = l->node;
			a = l->l;
			ADD(s);
			for(state = r->fail; state;){
				for(ll = state->alts; ll; ll = ll->next)
					if(ll->l == a)
						break;
				if(ll || (state == root)){
					if(ll)
						state = ll->node;
					/*
						do it here as only other exit is
						state 0
					*/
					if(state->out){
						s->out = 1;
					}
					break;
				} else
					state = state->fail;
			}
			s->fail = state;
		}
	}
	zeroroot(root, root);
}

static void
zeroroot(register Node *root, register Node *n)
{
	register Link *l;

	if(n->fail == root)
		n->fail = 0;
	for(l = n->alts; l; l = l->next)
		zeroroot(root, l->node);
}

static void
shift(re_cw *c)
{
	Link *qhead = 0, *qtail = 0;
	register Link *l;
	register Link *t;
	register Node *state, *r, *s;
	int k;

	for(k = 0; k < 256; k++)
		c->step[k] = c->mindepth+1;
	c->root->shift1 = 1;
	c->root->shift2 = c->mindepth;
	for(l = c->root->alts; l; l = l->next){
/*		l->node->shift2 = c->root->shift2;/**/
		ADD(l->node);
		l->node->fail = 0;
	}
	while(qhead){
		r = qhead->node;
		DEL();
		r->shift1 = c->mindepth;
		r->shift2 = c->mindepth;
		if(state = r->fail){
			do {
				k = r->d - state->d;
				if(k < state->shift1){
					state->shift1 = k;
				}
				if(r->out && (k < state->shift2)){
					state->shift2 = k;
				}
			} while(state = state->fail);
		}
		for(l = r->alts; l; l = l->next){
			s = l->node;
			ADD(s);
		}
	}
	shiftprop(c, c->root);
	shifttab(c->root);
	c->step[0] = 1;
}

static void
shifttab(register Node *n)
{
	register Link *l;
	register Node **nn;

	n->tab = nn = (Node **)calloc(256, sizeof(Node *));
	for(l = n->alts; l; l = l->next)
		nn[l->l] = l->node;
}

static void
shiftprop(register re_cw *c, register Node *n)
{
	register Link *l;
	register Node *nn;

	for(l = n->alts; l; l = l->next){
		if(c->step[l->l] > l->node->d)
			c->step[l->l] = l->node->d;
		nn = l->node;
		if(n->shift2 < nn->shift2)
			nn->shift2 = n->shift2;
		shiftprop(c, nn);
	}
}

void
re_cwcomp(re_cw *c)
{
	if(c->seenerror)
		return;
	fail(c, c->root);
	shift(c);
#ifdef	DEBUG
	cwpr(c->root);
#endif
}

re_cwexec(void *re_c, RDFN rdfn, MATCHFN matchfn)
{
	re_cw *c = (re_cw*)re_c;
	register Node *state;
	register Link *l;
	register unsigned char *sp;
	register unsigned char *e, *s;
	register Node *ostate;
	register k;
	unsigned char *rs, *re;
	unsigned char fake[1];
	unsigned char mappedsp;

	if(c->seenerror)
		return(-1);
	fake[0] = 0;
	state = c->root;
	for(rs = 0; (k = (*rdfn)((char**)&rs,(char**) &re)) > 0;){
doneline:
		s = rs+c->mindepth-1;
		e = re;
		while(s < e){
			/* scan */
			for(sp = s; ostate = state;){
				if(state->tab){
					if((state = state->tab[c->map[*sp]]) == 0)
						goto nomatch;
				} else {
					mappedsp = c->map[*sp];
					for(l = state->alts; ; l = l->next){
						if(l == 0)
							goto nomatch;
						if(l->l == mappedsp){
							state = l->node;
							break;
						}
					}
				}
#ifdef	DEBUG
print("state %d -0x%x-> state %d (out=%d)\n", ostate->id, *sp&0xFF, state->id, state->out);
#endif
				if(state->out){
					unsigned char *bm = sp, *em = s+1;
					if((k = (*matchfn)((char**)&bm, (char**)&em)) <= 0)
						return(k);
					rs = bm;
					re = em;
					state = c->root;
					goto doneline;
				}
				if(--sp < rs){
					sp = fake;
					break;
				}
			}
		nomatch:
			k = c->step[c->map[*sp]]-ostate->d-1;
			if(k < ostate->shift1)
				k = ostate->shift1;
			if(k > ostate->shift2)
				k = ostate->shift2;
			s += k;
			state = c->root;
		}
#ifdef	DEBUG
print("end of s<e loop: s=%d e=%d, rs=%d, re=%d\n", s, e, rs, re);
#endif
		rs = re;	/* we have analysed evrything up to here */
	}
	return(k? -1:0);
}

static void
freenode(Node *n)
{
	register Link *l, *ll;

	if(n->tab)
		free((char *)n->tab);
	for(l = n->alts; l; l = ll){
		ll = l->next;
		freenode(l->node);
	}
	free((char *)n);
}

void
re_cwfree(re_cw *cw)
{
	register Link *l;

	while(froot){
		l = froot->next;
		free((char *)froot);
		froot = l;
	}
	freenode(cw->root);
	free((char *)cw);
}

static Node *
newnode(re_cw *c, int d)
{
	static Node *next = 0, *lim = 0;
	static incr = 1000;

	if(next == lim){
		next = (Node *)malloc(incr*sizeof(Node));
		if(next == 0){
			re_error("node malloc fail");
			return 0;
		}
		lim = next+incr;
	}
	next->d = d;
	if(d > c->maxdepth) c->maxdepth = d;
	next->id = c->nodeid++;
	next->alts = 0;
	next->tab = 0;
	next->out = 0;
	return(next++);
}

static Link *
newlink(re_cw *c, int l, Node *n)
{
	static Link *next = 0, *lim = 0;
	static incr = 1000;

	if(next == lim){
		next = (Link *)malloc(incr*sizeof(Node));
		if(next == 0){
			re_error("link malloc fail");
			return 0;
		}
		lim = next+incr;
	}
	next->l = c->map[l];
	next->node = n;
	next->next = 0;
	return(next++);
}
