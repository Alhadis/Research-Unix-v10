#include <stdio.h>
#include "pico.h"
#include "pico2.h"
#include "y.tab.h"

extern struct	SRC src[MANY];
extern short	CURSCRATCH, CUROLD;
extern char	faster, frameb, metheus;
extern int	DEF_LL, DEF_NL;

#define	NNODE	8000

Node	nodearray[NNODE];
int	nalloc=0;

Node *
Index(k)
	Node *k;
{
	switch(k->type) {
	case ACOMMA:	if (commacount(k) == 2)
			return
			new(OADD, new(OMUL, k->right, VR(&DEF_LL), Z), k->left, Z);
			break;
	case COMP:	return new(ODIV, sumchans(k->left), NR(3), Z);
	}
	return k;
}

Node *
ncopy(n)
	Node *n;
{
	if (n == Z)
		return n;

	n = new(n->type, ncopy(n->left), ncopy(n->right), n->other);

	if (n->type == CONDI)
		n->other = ncopy(n->other);

	return n;
}

int spare;

Node *
modulo(p, q)
	Node *p, *q;
{ Node *tmp1, *tmp2;

	if (p->type == OCOMMA || p->type == ACOMMA)
		yyerror("lefthand-side of modulo is a composite");

	tmp1 = new(OASS, VR(&spare), ncopy(p), Z);
	tmp2 = new(OSUB, VR(&spare),
		new(OMUL, new(ODIV, VR(&spare), ncopy(q), Z), ncopy(q), Z), Z);

	return	new(OCOMMA, tmp1, new(CONDI, tmp2, VR(&spare), q), Z);
}

Node *
divide(p, q)
	Node *p, *q;
{ Node *tmp  =	ncopy(q);
	extern Node zero;
	return new(CONDI, new(ODIV, p, q, Z), &zero, tmp);
}

int fixspot;
		/* encodes the assignment of a composite to a composite */
Node *
fixit(n, m)
	Node *n, *m;
{	Node *tmp1 = new(OASS, VR(&fixspot), ncopy(m), Z);

	Node *tmp2 = ncopy(n->left->left);		/* r */
	Node *tmp3 = ncopy(n->left->right->left);	/* g */
	Node *tmp4 = ncopy(n->left->right->right);	/* b */

	Node *low = new(OAND, VR(&fixspot), NR(255), Z);
	Node *med = new(OLSH, notnew(OAND, VR(&fixspot), NR(255<<8)),
				new(OMINUS, NR(8), Z, Z), Z);
	Node *hig = new(OLSH, VR(&fixspot),
				new(OMINUS, NR(16), Z, Z), Z);

	Node *tmp5 = new(OASS, tmp2, low, Z);
	Node *tmp6 = new(OASS, tmp3, med, Z);
	Node *tmp7 = new(OASS, tmp4, hig, Z);

	Node *tmp8 = new(OCOMMA, tmp5, tmp6, Z);
	Node *tmp9 = new(OCOMMA, tmp7, tmp8, Z);

	return new(OCOMMA, tmp1, tmp9, Z);
}

Node *
catch(what, n, m)
	int what;
	Node *n, *m;
{
	switch (what)
	{
	case DIVV:	return divide(n, m);
	case MODU:	return modulo(n, m);
	case OASS:	if (n->type == COMP)
			{	if (m->type == CONDI)
					return fixit(n, m);
				else
			return new(OCOMMA, par(what, n->left, m->left), Z, Z);
			}
	default:	return new(what, n, m, Z);
	}
}

Node *
new(type, left, right, oth)
	Node *left, *right, *oth;
{
	register Node *n;

	if (nalloc >= NNODE)
		yyerror("NNODE too small; recompile pico");
	n = nodearray+nalloc++;
	n->type  = type;
	n->left  = left;
	n->right = right;
	n->other = oth;

	return n;
}

Node *
sumchans(n)
	Node *n;
{
	if (n == Z || n->type != ACOMMA)
		return n;
	return new(OADD, sumchans(n->left), sumchans(n->right), Z);
}

Node *
cast(n)
	Node *n;
{
	if (n == Z || n->type != ACOMMA)
		return n;

	return new(ODIV, sumchans(n->left), NR(3), Z);
}

#define Par(side)	par(what, n1->side, n2->side)

Node *
par(what, n1, n2)
	int what;
	Node *n1, *n2;
{
	if (n1 == Z || n2 == Z)
		return Z;

	if (n1->type != ACOMMA || n2->type != ACOMMA)
		return catch(what, ncopy(n1), ncopy(n2));

	if (what == OASS)
		return new(OCOMMA, Par(left), Par(right), Z);
	else
		return new(ACOMMA, Par(left), Par(right), Z);
}

Node *
promote(m)
	Node *m;
{
	Node *n;

	if (m->type == CONDI)
		return new(CONDI, promote(m->left), promote(m->right), m->other);
	if (m->type == COMP)
		return m;
	n = new(ACOMMA, ncopy(m), ncopy(m), Z);
	n = new(ACOMMA, ncopy(m), n, Z);

	return new(COMP, n, Z, Z);	
}

Node *
disp(what, n, e)
	int what;
	Node *n, *e;
{
	if (n == Z)
		return Z;

	if (n->type != ACOMMA)
		return catch(what, ncopy(n), ncopy(e));

	return new(OCOMMA, disp(what, n->left, e), disp(what, n->right, e), Z);
}

Node *
notnew(what, n1, n2)
	int what;
	Node *n1, *n2;
{
	int how = 0;

	if (n1->type == COMP) how += 1;
	if (n2->type == COMP) how += 2;

	switch (how) {
	case 1: n2 = promote(n2);	/* fall through */
	case 0:	return catch(what, n1, n2);
	case 2:	if (what == OASS)
		return catch(what, n1, new(ODIV, sumchans(n2->left), NR(3), Z));
		n1 = promote(n1);	/* fall through */
	case 3: if (what == OASS)
		return catch(what, n1, n2);	/* else: */
		return new(COMP, par(what, n1->left, n2->left), Z, Z);
	}
}

Node *
threechans(q, r)
	struct SRC *q;
	Node *r;
{
	Node *n;

	if (q->nchan >= 3)
	{	n = new(ACOMMA, DOLGRN(q, r), DOLBLU(q, r), Z);
		n = new(ACOMMA, DOLRED(q, r), n, Z);
		return new(COMP, n, Z, Z);
	} else
		return DOLRED(q, r);
}

SNode *
nsup(n, x, y, i)
	Node *n, *x, *y;
	char i;
{
	SNode *tmp;

	tmp = (SNode *) Emalloc(sizeof(SNode));
	tmp->n = n;
	tmp->x = x;
	tmp->y = y;
	tmp->i = i;

	return tmp;
}

Node *
splatter(n)
	SNode *n;
{
	extern int ramlyank(), fblyank(), metlyank();

	if (metheus)
		return new(CCALL, new(ACOMMA, n->x, n->y, Z), Z, metlyank);
	else if (frameb)
		return new(CCALL, new(ACOMMA, n->x, n->y, Z), Z, fblyank);
}

Node *
weird(n, m)
	SNode *n;
	 Node *m;
{
	Node *tmp;

	if (m == Z)
		return n->n;

	tmp = notnew(OASS, n->n, m);
	if (n->i != CURSCRATCH
	|| (!frameb && !metheus)
	|| (faster && notafunc() && metheus)
	|| (faster && Old->nchan==1 && notafunc() && frameb))
		return tmp;
	else
		return new(OCOMMA, tmp, splatter(n), Z);
}

Node *
getx(n)
	Node *n;
{
	if (n == Z)
		return AREG(XREG);
	if (n->type == ACOMMA)
	{	if (commacount(n) == 2)
			return n->left;
		else
			yyerror("bad index");
	}
	return modulo(n, VR(&DEF_LL));
}

Node *
gety(n)
	Node *n;
{
	if (n == Z)
		return AREG(YREG);
	if (n->type == ACOMMA)
	{	if (commacount(n) == 2)
			return n->right;
		else
			yyerror("bad index");
	}
	return divide(n, VR(&DEF_LL));
}

SNode *
super(a, b, t)
	Node *b;
{
	Node *tmp;
	Node *n = (b == Z)? DII: Index(b);

	switch (t) {
	case   RGB:	tmp = threechans(&src[a], n); break;
	case    BW:	if (src[a].nchan == 1)
				tmp = DOLRED((&src[a]), n);
		  	else
				tmp = cast(threechans(&src[a], n));
			break;
	case RCHAN:	tmp = DOLRED((&src[a]), n); break;
	case GCHAN:	tmp = DOLGRN((&src[a]), n); break;
	case BCHAN:	tmp = DOLBLU((&src[a]), n); break;
	}

	if (b == Z)
		return nsup(tmp, AREG(XREG), AREG(YREG), a);
	else
		return nsup(tmp, getx(b), gety(b), a);
}
