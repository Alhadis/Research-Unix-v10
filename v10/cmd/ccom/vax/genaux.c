#include "gencode.h"
#define NDNUM 800
mnod myt[NDNUM];
extern mnod *svq;
int ntree;
char *regnames[] = {"r0", "r1", "r2", "r3", "r4", "r5", "r6", "r7", "r8",
	"r9", "r10", "r11", "r12", "r13", "r14", "r15"};
char *frameptr = "fp";
char *argptr = "ap";
#define CHARCHAR 'b'
#define SHORTCHAR 'w'
#define LONGCHAR 'l'
#define FLOATCHAR 'f'
#define DOUBLECHAR 'd'

mnod *
gimmemnod()
{	mnod *p;
	p = myt + ntree++;
	if(ntree > NDNUM)
		cerror("out of temporary trees");
	return(p);
}

mnod *
tempnode(p, flag)
mnod *p;
{	mnod *x, *q;
	int n;
	extern int minrvar;
	if(p->op == Conv)
		q = p;	/* Conv to double of float, versus (CMP double double) */
	else
		q = (flag & RIGHT)? p->right: p->left;
	x = gimmemnod();
	x->type = q->type;
	n = incrsize(q) == 8? 2: 1;
	if(!(flag & ASADDR) && regvar >= REGVAR - 1 + n) {
		x->op = Reg;
		x->rval = regvar + 1 - n;
		x->lval = 1;	/* SCRATCH marker !!!!! */
		regvar -= n;
		if(minrvar > regvar)
			minrvar = regvar;	/* in case current routine recursive */
		return(x);
	}
	x->op = Auto;
	x->lval = gimmetemp(n);
	/* scratch marker? */
	return(x);
}
ret
alloctmp(p)
mnod *p;
{	ret s;
	sprintf(buf, "%d(%s)", gimmetemp(incrsize(p) == 8? 2: 1), frameptr);
	done(s, CANINDIR|SCRATCH, 0);
}

ret
allocreg(p, regmask)
mnod *p;
{	int i, n;
	ret s;
	mnod *x;
	if(p->type == Tdouble)
		n = 2;
	else
		n = 1;
	for(i = 0; i < REGVAR; i++) {
		if(!(regmask & (1 << i)))
			continue;
		if(n == 2 & !(regmask & (1 << (1+i))))
			continue;
		sprintx(buf, "%s", regnames[i]);
		regmask = (1 << i);
		if(n == 2)
			regmask |= (1 << (i+1));
		done(s, SCRATCH|ISREG, regmask);
	}
	x = tempnode(p, 0);
	if(x->op == Reg) {
		sprintx(buf, "%s", regnames[x->rval]);
		done(s, SCRATCH|ISREG, 0);
	}
	sprintx(buf, "%d(%s)", x->lval, frameptr);
	done(s, SCRATCH|CANINDIR, 0);
}

isfloat(p)
mnod *p;
{
	return(p->type == Tfloat || p->type == Tdouble);
}

childtype(p)
mnod *p;
{
	return(type(p->left));
}

char typearr[] = {LONGCHAR, CHARCHAR, CHARCHAR, SHORTCHAR, SHORTCHAR,
	LONGCHAR, LONGCHAR, LONGCHAR, LONGCHAR,
	FLOATCHAR, DOUBLECHAR, '?', '?'};
type(p)
mnod *p;
{
	return(typearr[p->type]);
}

int shiftarr[] = {2, 0, 0, 1, 1, 2, 2, 2, 2, 2, 3, -99, 0};
shiftsize(p)
mnod *p;
{
	return(shiftarr[p->type]);
}	

int incrarr[] = {4, 1, 1, 2, 2, 4, 4, 4, 4, 4, 8, -99, 0};
incrsize(p)
mnod *p;
{
	return(incrarr[p->type]);
}

int unsarr[] = {1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 0, 0, 0};	
isunsigned(p)
mnod *p;
{
	return(unsarr[p->type]);
}

mnod *
copymnod(p)
mnod *p;
{	mnod *a, *b, *c;
	c = gimmemnod();
	switch(p->op) {
	case Andeq: case And: case Cmp: case Comop: case Decr: 
	case Diveq: case Div: case Xoreq: case Xor:
	case Incr: case Lseq: case Ls: case Minuseq: case Minus:
	case Modeq: case Mod: case Muleq: case Mul: case Oreq:
	case Or: case Pluseq: case Plus: case Rseq: case Rs:
	case Asg: case Cm:
	case Call: case Stcall:
	case Stasg:
		a = copymnod(p->left);
		b = copymnod(p->right);
		*c = *p;
		c->left = a;
		c->right = b;
		return(c);
	case Compl: case Conv: case Genbr: case Genlab:
	case Genubr: case Star: case Addr: case Ucall:
	case Uminus: case Ustcall: case Init: case Funarg:
	case Fld:case Starg:
		a = copymnod(p->left);
		*c = *p;
		c->left = a;
		return(c);
	case Auto: case Reg: case Name: case Param: case Icon:
	case Snode: case Rnode: case Qnode:
		*c = *p;
		return(c);
	default:
		cerror("unk mnod in copymnod");
	}
}

/* rewrit A op B into (T = A, T) op B or A op (T = B, T) */
totemp(p, flag)
mnod *p;
{	mnod *a, *b, *c;
	a = gimmemnod();
	b = tempnode(p, flag);
	c = gimmemnod();
	if(flag & LEFT)
		*a = *p->left;
	else
		*a = *p->right;
	a->op = Asg;
	a->left = b;
	if(flag & LEFT)
		a->right = p->left;
	else
		a->right = p->right;
	c->op = Comop;
	c->left = a;
	c->right = b;
	c->type = a->type;
	if(flag & LEFT)
		p->left = c;
	else
		p->right = c;
}

funargs(p, regmask)
mnod *p;
{	ret s, t;
	int i;
	switch(p->op) {
	case Cm:	/* order depends on way stack grows */
#ifdef LTORARGS
		i = funargs(p->left, regmask);
		i |= funargs(p->right, regmask);
#else
		i = funargs(p->right, regmask);
		i |= funargs(p->left, regmask);
#endif
		return(i);
	case Funarg:
		t = tostack();
		s = doit(p->left, VALUE|TOSTACK, t, regmask);
		return(s.flag & FAIL);
	case Starg:
		/* this has to have same cases as STASG */
		if(regmask != REGMASK)
			return(FAILX);
		s = doit(p->left, (ASADDR|VALUE|FSTASG), 0, regmask);
		/* this generates expensive code for small structures */
		i = p->stsize / 8;
		if(p->left->op == Stasg) {
			if(i != 4 && i != 8) {
				pr("#\tsubl2\t$%d,sp\n", i);
				pr("#\tsubl2\t$%d,r3\n", i);
				pr("#\tmovc3\t$%d,(r3),(sp)\n", p->stsize/8);
				return(0);
			}
		}
		if(i == 4)
			pr("#\tpushl\t%s\n", str(s));
		else if(i == 8)
			pr("#\tmovq\t%s,-(sp)\n", str(s));
		else {
			pr("#\tsubl2\t$%d,sp\n", i);
			pr("#\tmovc3\t$%d,%s,(sp)\n", p->stsize/8, str(s));
		}
		return(s.flag & FAIL);
	}
}
ret
specialreg(p, regmask)
mnod *p;
{	ret s;
	int n, i;
	n = p->type == Tdouble? 2: 1;
	i = 1;
	if(n == 2)
		i = 3;
	sprintx(buf, "%s", regnames[0]);
	if(i & ~regmask)
		pr("#\specialreg not free\n");
	done(s, SCRATCH|ISREG, i);
}
/* these guys rewrite a1 = a2 = ... an = x
 * as t = x; an = t; an-1 = an */
mnod *
fromtemp(p, temp)
mnod *p, *temp;
{	mnod *q;
	q = gimmemnod();
	*q = *p;
	q->left = p;
	q->op = Asg;
	q->right = temp;
	return(q);
}

mnod *
to(p, temp)
mnod *p, *temp;
{	mnod *q;
	q = gimmemnod();
	*q = *p;
	q->right = p;
	q->op = Asg;
	q->left = temp;
	return(q);
}

mnod *
commnod(a, p)
mnod *a, *p;
{	mnod *q;
	q = gimmemnod();
	*q = *p;
	q->op = Comop;
	q->right = p;
	q->left = a;
	return(q);
}

asgwrite(p)
mnod *p;
{	mnod *q, *a, *temp;
	debugpr("#REWRITEasgwrite ");
	temp = tempnode(p, 0);
	a = fromtemp(p->left, temp);
	for(q = p->right; q->op == Asg; q = q->right)
		a = commnod(fromtemp(q->left, temp), a);
	a = commnod(to(q, temp), a);
	*p = *a;
}

lsconv(p)
mnod *p;
{	mnod *lft, *right;
	debugpr("#REWRITElsconv ");
	lft = gimmemnod();
	right = gimmemnod();
	lft->op = right->op = Conv;
	lft->type = right->type = Tlong;
	lft->left = p->left;
	right->left = p->right;
	if(p->left->op != Icon)	/* Icon are longs anyway (see RS) */
		p->left = lft;
	p->right = right;
}
/* only some ops have to be rewritten (addb and addl are the same at the bottom) */
rewriteasgop(p)
mnod *p;
{	mnod *a, *newop;
	if(p->left->op != Conv)
		return(0);
	if(p->left->left->op == Star)
		longjmp(back, awfulstar(p));
	debugpr("#REWRITE rewriteasgop ");
	newop = gimmemnod();
	*newop = *p;
	switch(p->op) {
	case Diveq:
		newop->op = Div;
		break;
	case Lseq:
		newop->op = Ls;
		break;
	case Modeq:
		newop->op = Mod;
		break;
	case Rseq:
		newop->op = Rs;
		break;
	case Pluseq:
		if(!isfloat(p->left))
			return(0);
		newop->op = Plus;
		break;
	case Minuseq:
		if(!isfloat(p->left))
			return(0);
		newop->op = Minus;
		break;
	case Muleq:
		if(!isfloat(p->left))
			return(0);
		newop->op = Mul;
		break;

	default:
		cerror("codegen: rewriting asgop");
	}
	a = gimmemnod();
	*a = *p->left->left;
	p->op = Asg;
	p->left = a;
	p->right = newop;
	return(1);
}

rewriteconv(p)	/* uns to float or double */
mnod *p;
{	mnod *a;
	debugpr("#REWRITEconv ");
	a = gimmemnod();
	*a = *p;
	a->type = Tlong;
	p->left = a;
}

/* =(FLD(*(A)),B) -> ,(=(T,A), =(FLD(*(T)),B) */
fldstar(p)
mnod *p;
{	mnod *A, *B, *T, *newtop;
	debugpr("#REWRITE fldstar ");write(2, "fldstar\n", 8);
	A = p->left->left->left;
	B = p->right;
	T = tempnode(A, 0);
	newtop = gimmemnod();
	newtop->op = Comop;
	newtop->type = p->type;
	newtop->left = gimmemnod();
	newtop->left->op = Asg;
	newtop->left->type = A->type;
	newtop->left->left = T;
	newtop->left->right = A;
	newtop->right = gimmemnod();
	*newtop->right = *p;
	p->left->left->left = T;
	*p = *newtop;
}

mediumstar(p)
mnod *p;
{	mnod *newtop, *tmp, *x, *y;
	debugpr("#REWRITE mediumstar ");
	newtop = gimmemnod();
	newtop->op = Comop;
	newtop->type = p->type;
	x = gimmemnod();
	x->op = Asg;
	x->type = Tpoint;
	x->right = p->left->left;
	newtop->left = x;
	tmp = tempnode(newtop, 0);
	tmp->type = Tpoint;
	x->left = tmp;
	y = gimmemnod();
	y->op = Comop;
	y->type = p->type;
	x = gimmemnod();
	*x = *p;
	newtop->right = y;
	y->left = x;
	y->right = x->left;
	x->left->left = tmp;
	*p = *newtop;
	return(1);
}

awfulstar(p)
mnod *p;
{	mnod *newtop, *doleft, *x, *op, *equals;
	debugpr("#REWRITE awfulstar ");
	newtop = gimmemnod();
	newtop->op = Comop;
	newtop->type = p->type;
	doleft = gimmemnod();
	doleft->op = Asg;
	doleft->type = Tpoint;
	doleft->right = p->left->left->left;
	x = tempnode(doleft, RIGHT);
	doleft->left = x;
	op = gimmemnod();
	*op = *p;
	op->op += 1;	/* cheap hack */
	op->left->left->left = x;	/* so it's a dag, but x is harmless */
	equals = gimmemnod();
	*equals = *p;
	equals->right = op;
	equals->left = p->left->left;
	equals->op = Asg;
	*p = *newtop;
	p->left = doleft;
	p->right = equals;
	return(1);
}
rewritefld(p)
mnod *p;
{	mnod *q, *tmp, *left, *comop;
	debugpr("#REWRITEfld ");
	if((q = p->left->left)->op != Star) {
		rewfld(p);
		return;
	}
	q = q->left;
	tmp = tempnode(q, 0);
	left = gimmemnod();
	left->type = q->type;
	left->op = Asg;
	left->left = tmp;
	left->right = gimmemnod();
	left->right = copymnod(q);
	*q = *tmp;
	comop = gimmemnod();
	comop->op = Comop;
	comop->left = left;
	comop->right = copymnod(p);
	*p = *comop;
	rewfld(p->right);
	longjmp(back, 1);
}
rewfld(p)
mnod *p;
{	mnod *x, *y, *z;
	x = gimmemnod();
	*x = *copymnod(p);
	y = gimmemnod();
	*y = *copymnod(p);
	z = gimmemnod();
	*z = *p;
	z->left = x;
	x->op = Asg;
	x->right = y;
	if(p->op == Decr || p->op == Minuseq)
		z->op = Plus;
	else
		z->op = Minus;
	if(p->op == Decr)
		y->op = Minus;
	else if(p->op == Incr)
		y->op = Plus;
	else
		y->op += 1;	/* cheap hack */
	if(p->op == Decr || p->op == Incr)
		*p = *z;
	else
		*p = *x;
}
/* not int = (...? int exprs) have a bogus tree */
extracheck(p)
mnod *p;
{	mnod *a;
	if(p->right->op != Genlab && p->right->op != Comop)
		return;
	debugpr("#REWRITE extracheck ");
	a = gimmemnod();
	*a = *p;
	a->left = p->right;
	a->op = Conv;
	p->right = a;
}

starasg(p)
mnod *p;
{	mnod *tmp, *asg, *x;
	debugpr("#REWRITE starasg ");
	tmp = tempnode(p->left->left, 0);
	x = gimmemnod();
	*x = *p;
	asg = gimmemnod();
	*asg = *p->left->left;
	asg->op = Asg;
	asg->left = tmp;
	asg->right = p->left->left;
	x->left->left = tmp;
	p->op = Comop;
	p->left = asg;
	p->right = x;
}
/* *A op= B => (T=A, *T op= B) if T has Incr or Asg */
fixuns(p)
mnod *p;
{	mnod *tmp, *asg, *new;
	if(!sideffects(p->left->left))
		return(0);
	debugpr("#REWRITE fixuns");
	tmp = tempnode(p->left->left, 0);
	asg = gimmemnod();
	asg->type = tmp->type;
	asg->op = Asg;
	asg->left = tmp;
	asg->right = p->left->left;
	new = gimmemnod();
	*new = *p;
	new->left->left = tmp;
	p->op = Comop;
	p->left = asg;
	p->right = new;
	return(1);
}

stasgrewrite(p)
mnod *p;
{	mnod *qa, *qb, *n, *left, *right;
	debugpr("#REWRITE starasgrewrite ");
	totemp(p, RIGHT);
	longjmp(back, 1);
}
