#include	"re.h"
#include	"lre.h"
#include	"hdr.h"

#define 	DEBUG

static Br *seq(Expr *);
static Br *spew(Br_type, Expr*, int);

static Expr *eop;

static char tabs[] = { '\t', '\t', '\t', '\t', '\t', '\t', '\t', '\t', '\t',
	'\t', '\t', '\t', '\t', '\t', '\t', '\t', '\t', '\t', '\t', '\t', '\t', 0
};
#define	SPACE(d)	(&tabs[sizeof tabs - (d) - 1])

static void
init1(register Br *br, re_re *r)
{
	switch(br->type)
	{
	case br_re:
		br->r = (re_re *)egmalloc(sizeof(re_re), "egbr");
#ifdef	DEBUG
		if(TRACE(3))
			PR "b@%ld->r = %ld\n", br, br->r);
#endif
		if(!br->r)
			return;
		memcpy((char *)br->r, (char *)r, sizeof(*r));
		if(br->e->type != EOP)
			br->e = eg_newexpr(EOP, '#', br->e, (Expr *)0);
		br->r->root = br->e;
		br->e->id = eop->id;
		br->r->br = br;
		br->r->backref = br->r->root->backref;
		br->r->parens = br->r->root->parens;
		eginit(br->r, br == r->br);
		break;
	case br_star:
	case br_plus:
	case br_quest:
	case br_group:
		init1(br->lb, r);
		break;
	case br_cat:
	case br_alt:
		init1(br->lb, r);
		init1(br->rb, r);
		break;
	}
}

void
egbr(re_re *r)
{
	eop = 0;
/*
#ifdef	DEBUG
	if(1||TRACE(3)){
		char buf[EPRINTSIZE];

		eg_epr(r->root, buf, 0);
		PR "egbr(%s) ->\n", buf);
		eg_brpr(seq(r->root));
	}
#endif
/**/
	egcanon(r->root);
	r->br = seq(r->root);
#ifdef	DEBUG
	if(TRACE(3)){
		char buf[EPRINTSIZE];

		eg_epr(r->root, buf, 0);
		PR "egbr(%s) ->\n", buf);
		eg_brpr(r->br);
	}
#endif
	init1(r->br, r);
}

#ifdef	DEBUG
static void
brpr1(Br *b, int depth)
{
	char buf[EPRINTSIZE];

	PR "%s%d@", SPACE(depth), (int)b);
	switch(b->type)
	{
	case br_br:
		PR "BR %d\n", b->group);
		break;
	case br_re:
		eg_epr((Expr *)b->e, buf, 0);
		if(((Expr *)b->e)->backref) PR "X");
		if(((Expr *)b->e)->parens) PR "()");
		PR "RE/%s/%d\n", buf, (int)b->r);
		break;
	case br_group:
		PR "GROUP %d\n", b->group);
		brpr1(b->lb, depth+1);
		break;
	case br_quest:
		PR "BR?");
		brpr1(b->lb, depth+1);
		break;
	case br_plus:
		PR "BR+\n");
		brpr1(b->lb, depth+1);
		break;
	case br_star:
		PR "BR*\n");
		brpr1(b->lb, depth+1);
		break;
	case br_cat:
		PR "BR CAT\n");
		brpr1(b->lb, depth+1);
		brpr1(b->rb, depth+1);
		break;
	case br_alt:
		PR "BR |\n");
		brpr1(b->lb, depth+1);
		brpr1(b->rb, depth+1);
		break;
	default:
		PR "BADTYPE/%d/\n", b->type);
		break;
	}
}

void
eg_brpr(Br *br)
{
	brpr1(br, 0);
}
#endif

static Br *
seq(Expr *e)
{
	Br *br;

	if(e->type == EOP)
		eop = e;
	if(!e->backref && !e->parens)
		return(spew(br_re, e, -1));
	switch(e->type)
	{
	case Cat:
		br = spew(br_cat, (Expr *)0, -1);
		br->lb = seq(e->l);
		br->rb = seq(e->r);
		return(br);
	case Alternate:
		br = spew(br_alt, (Expr *)0, -1);
		br->lb = seq(e->l);
		br->rb = seq(e->r);
		return(br);
	case Star:
		br = spew(br_star, (Expr *)0, -1);
		br->lb = seq(e->l);
		return(br);
	case Plus:
		br = spew(br_plus, (Expr *)0, -1);
		br->lb = seq(e->l);
		return(br);
	case Quest:
		br = spew(br_quest, (Expr *)0, -1);
		br->lb = seq(e->l);
		return(br);
	case Group:
		br = spew(br_group, (Expr *)0, e->lit);
		br->lb = seq(e->l);
		return(br);
	case Backref:
		return(spew(br_br, e->l, e->lit));
	case EOP:
		return(seq(e->l));
	default:
		return(spew(br_re, e, -1));
	}
}

static Br *
spew(Br_type t, Expr *d, int g)
{
	Br *b;

	b = (Br *)egmalloc(sizeof(Br), "back ref malloc");
	if(!b)
		return(0);
	b->type = t;
	b->e = d;
	b->group = g;
	b->r = 0;
	b->rb = b->lb = 0;
	return(b);
}
