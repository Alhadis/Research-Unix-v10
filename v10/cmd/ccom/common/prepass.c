/* prepass tries to turn the trees into basic blocks */
/* eventually this is where p2compile will be */
#include "mfile2.h"
NODE *dup(), *chng1(), *seq(), *buildtree(), *dlabel();

NODE *
prepass(p)
NODE *p;
{	int o;
	NODE *r, *l;
	if(!p)
		return(p);
	o = p->in.op;
	if(o == ASSIGN && (r = p->in.right)->in.op == QUEST) {
		if((l = p->in.left)->in.op == SNODE || l->in.op == RNODE)
			return(p);
		p = chng1(p);
	}
	return(p);		
}
/* =(x ?(t :(a b))) -> (cbranch t l0), l0(jbr l1 =(x a)), l1(=(x b) */
NODE *
chng1(p)
NODE *p;
{	NODE *t, *a, *b, *x, *c1, *c2, *u, *v;
	int l0, l1;
	t = p->in.right->in.left;
	a = p->in.right->in.right->in.left;
	b = p->in.right->in.right->in.right;
	x = p->in.left;
	l0 = getlab();
	l1 = getlab();
	u = dup(p);
	u->in.left = dup(x);
	u->in.right = b;
	v = dup(p);
	v->in.left = dup(x);
	v->in.right = a;
	c2 = seq(dlabel(genbr(0, l1, v), l0), dlabel(u, l1));
	c1 = seq(buildtree(CBRANCH, t, bcon(l0)), c2);
	p->in.op = x->in.op = FREE;
	p->in.right->in.op = p->in.right->in.right->in.op = FREE;
	return(c1);
}

NODE *
dup(p)
NODE *p;
{	NODE *t;
	t = talloc();
	*t = *p;
	return(t);
}
