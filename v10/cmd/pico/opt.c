#include "pico.h"

long	eval();
Node*	abase();
Node	*new();
#define	ARITH	1
#define	BOOL	0
#define	Z	((Node *)0)
#define	OK(x)	(x==0)
#define	EQ(x,y)	((x)==(y))
#define	IS(x,y)	EQ((x)->type,(y))
#define	OSIZE	50

rewrite(n, context)
Node *n;
{
	long v;

	if(n == Z)
		return;
	if(isconst(n)) {
		if(!IS(n,CONST) && !IS(n,CONSTB))
			mkconst(n, eval(n, context));
		return;
	}
	switch(n->type)
	{
	case LABL:
		rewrite(n->left, context);
		return;

	case GOTO:
	case VAR:
	case OARG:
	case REG:
		return;

	case CONDI:
		if(isconst(n->other)) {
			v = eval(n->other, BOOL);
			if(v) {
				rewrite(n->left, context);
				*n = *n->left;
				return;
			}
			rewrite(n->right, context);
			*n = *n->right;
			return;
		}
		rewrite(n->other, BOOL);
		iknown(n->other, n->left, 1);
		rewrite(n->left, context);
		if(n->right != Z) {
			iknown(n->other, n->right, 0);
			rewrite(n->right, context);
			if(compr(n->left, n->right) == 0)
				*n = *n->right;
		}
		return;

	case DEREFB:
		rewrite(n->left, ARITH);
		rewrite(n->right, ARITH);
		if(n->left->type != VAR)
			return;
		if(IS(n->right,CONST)) {
			n->left->arg += n->right->arg;
			n->right = Z;
			return;
		}
		if(IS(n->right,OADD))
		if(IS(n->right->right,CONST)) {
			n->left->arg += n->right->right->arg;
			n->right = n->right->left;
			return;
		}
		return;

	case DEREFS:
		rewrite(n->left, ARITH);
		rewrite(n->right, ARITH);
		if(n->left->type != VAR)
			return;
		if(IS(n->right,CONST)) {
			n->left->arg += 2*n->right->arg;
			n->right = Z;
			return;
		}
		if(IS(n->right,OADD))
		if(IS(n->right->right,CONST)) {
			n->left->arg += 2*n->right->right->arg;
			n->right = n->right->left;
			return;
		}
		return;

	case DEREFL:
		rewrite(n->left, ARITH);
		rewrite(n->right, ARITH);
		if(n->left->type != VAR)
			return;
		if(IS(n->right,CONST)) {
			n->left->arg += 4*n->right->arg;
			n->right = Z;
			return;
		}
		if(IS(n->right,OADD))
		if(IS(n->right->right,CONST)) {
			n->left->arg += 4*n->right->right->arg;
			n->right = n->right->left;
			return;
		}
		return;

	case OADD:
	case OSUB:
	case OISUB:
	case OMINUS:
		acan(n);
		return;

	case OMUL:
		scan(n, 1L);
		return;

	case ODIV:
		rewrite(n->left, ARITH);
		rewrite(n->right, ARITH);
		if(IS(n->right,CONST)) {
			v = n->right->arg;
			if(v == 1)
				*n = *n->left;
		}
		return;

	case OIDIV:
		rewrite(n->left, ARITH);
		rewrite(n->right, ARITH);
		if(IS(n->left,CONST)) {
			v = n->left->arg;
			if(v == 1)
				*n = *n->right;
		}
		return;

	case OPOW:
	case OBIC:
	case OLSH:
		rewrite(n->left, ARITH);
		rewrite(n->right, ARITH);
		return;

	case ORETURN:
		rewrite(n->left, ARITH);
		return;

	case OAND:
		scan(n, -1L);
		if(IS(n, OAND))
		if(context == ARITH)
			fixbic(n);
		return;

	case OOR:
	case OXOR:
		scan(n, 0L);
		return;

	case ONEG:
		rewrite(n->left, context);
		return;

	case OCOMMA:
	case ACOMMA:
		rewrite(n->left, ARITH);
		rewrite(n->right, context);
		return;

	case OCALL:
	case CCALL:
		rewrite(n->left, ARITH);
		return;

	case OASS:
		rewrite(n->left, context);
		rewrite(n->right, context);
		return;

	case COMP:
		rewrite(n->left, ARITH);
		return;

	case OGT:
	case OLT:
	case OLE:
	case OGE:
	case ONE:
	case OEQ:
		rewrite(n->left, ARITH);
		rewrite(n->right, ARITH);
		if(n->right && IS(n->right,CONST) && n->right->arg == 0) {
			n->right = Z;
			return;
		}
		if(n->left && IS(n->left,CONST) && n->left->arg == 0) {
			n->left = n->right;
			n->right = Z;
			n->type = comop(n->type);
			return;
		}
		return;

	case OOROR:
		rewrite(n->left, BOOL);
		rewrite(n->right, BOOL);
		if(IS(n->left,CONST)) {
			*n = *n->right;
			return;
		}
		if(IS(n->right,CONST)) {
			if(!eval(n->right, BOOL))
				*n = *n->right;
			return;
		}
		iknown(n->left, n->right, 0);
		return;

	case OANDAND:
		rewrite(n->left, BOOL);
		rewrite(n->right, BOOL);
		if(IS(n->left,CONST)) {
			*n = *n->right;
			return;
		}
		if(IS(n->right,CONST)) {
			if(eval(n->right, BOOL))
				*n = *n->right;
			return;
		}
		iknown(n->left, n->right, 1);
		return;

	case ONOT:
		rewrite(n->left, BOOL);
		return;
	}
	printf("type = %d\n", n->type);
}

comop(o)
{

	switch(o) {

	case OLT: o = OGE; break;
	case OLE: o = OGT; break;
	case OGT: o = OLE; break;
	case OGE: o = OLT; break;
	}
	return(o);
}

fixbic(n)
Node *n;
{

	if(!IS(n->right,CONST))
		return;
	n->right->arg = ~n->right->arg;
	n->type = OBIC;
}

iknown(x, n, f)
Node *n, *x;
{

	if (n == Z)
		return;

	known(x, n, f);
	switch(x->type) {

	case ONOT:
		iknown(x->left, n, !f);
		break;

	case OAND:
		if(f) {
			if(isconst(x->left))
				iknown(x->right, n, 1);
			if(isconst(x->right))
				iknown(x->left, n, 1);
		}
		break;

	case OOR:
		if(!f) {
			if(isconst(x->left))
				iknown(x->right, n, 0);
			if(isconst(x->right))
				iknown(x->left, n, 0);
		}
		break;
	}
}

known(x, n, f)
Node *n, *x;
{

	if(compr(x, n) == 0) {
		if(f == 0) {
			mkconst(n, 0L);
			return;
		}
	}
	switch(n->type) {

	case CONDI:
		known(x, n->other, f);
		if(compr(x, n->other) == 0) {
			if(f)
				*n = *n->left;
			else
				*n = *n->right;
			known(x, n, f);
			return;
		}
		break;

	case OANDAND:
		if(compr(x, n->left) == 0) {
			if(f)
				mkconst(n, 1L);
			else
				*n = *n->right;
			known(x, n, f);
			return;
		}
		if(compr(x, n->right) == 0) {
			if(f)
				mkconst(n->right, 1L);
			else
				*n = *n->left;
			known(x, n, f);
			return;
		}
		break;

	case OOROR:
		if(compr(x, n->left) == 0) {
			if(f)
				*n = *n->right;
			else
				mkconst(n, 0L);
			known(x, n, f);
			return;
		}
		if(compr(x, n->right) == 0) {
			if(f)
				*n = *n->left;
			else
				mkconst(n->right, 0L);
			known(x, n, f);
			return;
		}
		break;
	}
	if(n->right)
		known(x, n->right, f);
	if(n->left)
		known(x, n->left, f);
}

mkconst(n, v)
Node *n;
long v;
{

	n->type = CONST;
	n->other = Z;
	n->left = Z;
	n->right = Z;
	n->arg = v;
}


isconst(n)
Node *n;
{

	if(n == Z)
		return 0;
	switch(n->type)
	{

	case CONST:
	case CONSTB:
		return 1;

	case REG:
	case VAR:
	case OARG:
	case DEREFB:
	case DEREFS:
	case DEREFL:
	case OCALL:
	case CCALL:
	case OASS:
	case GOTO:
	case LABL:
	case OCOMMA:
	case ACOMMA:
	case ORETURN:
		return 0;

	case COMP:
		for (n = n->left; n->type == ACOMMA; n = n->left)
			if(!isconst(n->left))
				return 0;
		return isconst(n);

	case OANDAND:
		if(isconst(n->left)) {
			if(!eval(n->left, BOOL))
				return 1;
			return isconst(n->right);
		}
		return 0;

	case OOROR:
		if(isconst(n->left)) {
			if(eval(n->left, BOOL))
				return 1;
			return isconst(n->right);
		}
		return 0;

	case CONDI:
		if(!isconst(n->other))
			return 0;
		if(eval(n->other, BOOL))
			return isconst(n->left);
		return isconst(n->right);
	}
	if(n->left)
		if(!isconst(n->left))
			return 0;
	if(n->right)
		if(!isconst(n->right))
			return 0;
	return 1;
}

struct	O
{
	struct	l
	{
		Node	*n;
		long	f;
	} l[OSIZE];
	Node	*free;
	int	count;
} O;

cancmp1(p1, p2)
struct l *p1, *p2;
{

	return compr(p1->n, p2->n);
}

scan1(n, o)
	Node *n;
{

	if(IS(n,o)) {
		scan1(n->left, o);
		scan1(n->right, o);
		return;
	}
	rewrite(n, ARITH);
}

scan2(n, o)
	Node *n;
{

	if(IS(n,o)) {
		scan2(n->left, o);
		scan2(n->right, o);
		n->type = o;
		n->left = O.free;
		n->right = Z;
		n->arg = 0;
		O.free = n;
		return;
	}
	O.l[O.count].n = n;
	O.count++;
	if(O.count >= OSIZE)
		yyerror("OSIZE too small");
}

scan(n, z)
Node *n;
long z;
{
	Node t;
	int i, j, o;

	o = n->type;
	scan1(n, o);
	O.count = 0;
	O.free = Z;
	scan2(n, o);
	qsort(O.l, O.count, sizeof O.l[0], cancmp1);

	j = 0;
	for(i=0; i<O.count; i++) {
		if(j)
		if(IS(O.l[i].n,CONST)) {
			t.type = o;
			t.left = O.l[i].n;
			t.right = O.l[j-1].n;
			mkconst(O.l[i].n, eval(&t, ARITH));
			O.l[j-1].n = O.l[i].n;
			continue;
		}
		if(j)
		if(compr(O.l[j-1].n, O.l[i].n) == 0) {
			/* x^x => 0 */
			if(o == OXOR) {
				j--;
				continue;
			}
			/* x&x => x */
			/* x|x => x */
			if(o == OAND || o == OOR)
				continue;
		}
		O.l[j].n = O.l[i].n;
		if(IS(O.l[j].n,CONST))
		if(O.l[j].n->arg == z)
			continue;
		j++;
	}
	O.count = j;
	if(n != O.free)
		yyerror("scan smells");
	if(O.count == 0) {
		mkconst(n, z);
		return;
	}
	if(O.count == 1) {
		*n = *O.l[0].n;
		return;
	}
	for(i=0; i<O.count-2; i++) {
		O.free->right = O.l[i].n;
		O.free = O.free->left;
	}
	O.free->right = O.l[i].n;
	O.free->left = O.l[i+1].n;
}

acan1(n)
	Node *n;
{
	register t;

	t = n->type;
	if(EQ(t,OADD)||EQ(t,OSUB)||EQ(t,OMINUS)||EQ(t,OISUB)) {
		acan1(n->left);
		if(n->right)
			acan1(n->right);
		return;
	}
if (t == OISUB) yyerror("hit bug in optimizer (loop)");
	rewrite(n, ARITH);
}

acan2(n, f)
Node *n;
long f;
{
	register t;

	t = n->type;
	if(EQ(t,OADD)) {
		acan2(n->left, f);
		acan2(n->right, f);
		goto out;
	}
	if(EQ(t,OSUB)) {
		acan2(n->left, f);
		acan2(n->right, -f);
		goto out;
	}
	if(EQ(t,OISUB)) {
		acan2(n->left, -f);
		acan2(n->right, f);
		goto out;
	}
	if(EQ(t,OMINUS)) {
		acan2(n->left, -f);
		goto out;
	}
	if(EQ(t,OMUL))
	if(IS(n->right,CONST)) {
		f *= n->right->arg;
		acan2(n->left, f);
		goto out;
	}
	if(f != 1 && IS(n,CONST)) {
		mkconst(n, f*n->arg);
		f = 1;
	}
	O.l[O.count].n = n;
	O.l[O.count].f = f;
	O.count++;
	if(O.count >= OSIZE)
		yyerror("OSIZE too small");
	return;

out:
	n->type = OADD;
	n->left = O.free;
	n->right = Z;
	n->arg = 0;
	O.free = n;
}

acan3()
{
	int i, j;
	long nator, dator;
	Node *t;

	if(IS(O.l[0].n,CONST))
	if(O.l[0].n->arg < 0) {
		O.l[0].n->arg = -O.l[0].n->arg;
		O.l[0].f = -O.l[0].f;
	}
	for(i=0; i<O.count; i++) {
		if(IS(O.l[i].n,CONST))
			continue;
		dator = O.l[i].f;
		if(dator < 0)
			dator = -dator;
		if(dator <= 1)
			continue;
		for(j=0; j<O.count; j++) {
			if(i == j)
				continue;
			nator = O.l[j].f;
			if(IS(O.l[j].n,CONST))
				nator = O.l[j].n->arg;
			if(nator < 0)
				nator = -nator;
			if(nator%dator)
				continue;
			if(IS(O.l[j].n,CONST)) {
				O.l[j].n->arg = nator/dator;
				t = O.l[j].n;
			} else
			if(dator == nator) {
				t = O.l[j].n;
			} else {
				t = new(CONST, Z, Z, nator/dator);
				t = new(OMUL, O.l[j].n, t, 0L);
			}
			if(O.l[j].f < 0)
				t = new(OSUB, O.l[i].n, t, 0L);
			else
				t = new(OADD, O.l[i].n, t, 0L);
			O.l[i].n = t;
			O.l[j].f = 0;
		}
	}
	j = 0;
	for(i=0; i<O.count; i++) {
		if(O.l[i].f == 0)
			continue;
		O.l[j].n = O.l[i].n;
		O.l[j].f = O.l[i].f;
		j++;
	}
	O.count = j;
	for(i=O.count-1; i>=0; i--) {
		if(O.l[i].f < 0)
			continue;
	}
}

Node *
atemp(i)
{
	Node *t;
	int f;

	f = O.l[i].f;
	if(f < 0)
		f = -f;
	if(f != 1) {
		t = new(CONST, Z, Z, f);
		if(f)
			t = new(OMUL, O.l[i].n, t, 0L);
		return t;
	}
	return O.l[i].n;
}

acan(n)
Node *n;
{
	Node t;
	int i, j;

	acan1(n);
	O.count = 0;
	O.free = Z;
	acan2(n, 1L);
	qsort(O.l, O.count, sizeof O.l[0], cancmp1);

	j = 0;
	for(i=0; i<O.count; i++) {
		if(j)
		if(IS(O.l[i].n,CONST)) {
			t.type = OADD;
			t.left = O.l[i].n;
			t.right = O.l[j-1].n;
			mkconst(O.l[i].n, eval(&t, ARITH));
			O.l[j-1].n = O.l[i].n;
			continue;
		}
		if(j)
		if(compr(O.l[j-1].n, O.l[i].n) == 0) {
			O.l[j-1].f += O.l[i].f;
			if(O.l[j-1].f == 0)
				j--;
			continue;
		}
		O.l[j].n = O.l[i].n;
		O.l[j].f = O.l[i].f;
		if(IS(O.l[j].n,CONST))
		if(O.l[j].n->arg == 0)
			continue;
		j++;
	}
	O.count = j;
	acan3();
	if(n != O.free)
		yyerror("acan smells");
	if(O.count == 0) {
		mkconst(n, 0L);
		return;
	}
	j = -1;
	for(i=0; i<O.count; i++)
		if(O.l[i].f >= 0)
			j = i;
	if(j < 0) {
		if(IS(O.l[0].n,CONST)) {
			O.l[0].n->arg = -O.l[0].n->arg;
			O.l[0].f = -O.l[0].f;
			j = 0;
		} else {
			O.free->type = OMINUS;
			if(O.count == 1) {
				O.free->left = atemp(0);
				return;
			}
			O.free = O.free->left;
		}
	}
	for(i=j+1; i<O.count; i++)
		O.l[i].f = -O.l[i].f;
	if(O.count == 1) {
		*O.free = *atemp(0);
		return;
	}
	for(i=0; i<O.count-2; i++) {
		O.free->right = atemp(i);
		if(O.l[i].f < 0)
			O.free->type = OSUB;
		else
		if(i == j)
			O.free->type = OISUB;
		O.free = O.free->left;
	}
	O.free->right = atemp(i);
	if(O.l[i].f < 0)
		O.free->type = OSUB;
	else
	if(i == j)
		O.free->type = OISUB;
	O.free->left = atemp(i+1);
}

compr(n1, n2)
Node *n1, *n2;
{
	int v;

	v = n1->type - n2->type;
	if(v) {
		if(IS(n1,CONST))
			return -1;
		if(IS(n2,CONST))
			return 1;
		return v;
	}
	switch(n1->type)
	{

	case VAR:
	case CONST:
	case REG:
	case OARG:
		v = n1->arg - n2->arg;
		return v;

	case CONDI:
		v = compr(n1->other, n2->other);
		if(v)
			return v;

	case OCALL:
	case CCALL:
	case LABL:
	case GOTO:
		return -1;
	}
	v = compr(n1->left, n2->left);
	if(v)
		return v;
	if(n1->right)
		return compr(n1->right, n2->right);
	return 0;
}

dlist()
{
	int i;

	for(i=0; i<O.count; i++) {
		printf("[%2d]  ", i);
		printf("%3d ", O.l[i].f);
		prtree(O.l[i].n, 0, 5);
	}
	printf("\n");
}

long
eval(n, context)
Node *n;
{
	Node n1, n2;
	long v;


	if(context == BOOL) {
		n1.type = ONOT;
		n1.left = &n2;
		n1.right = Z;
		n1.other = Z;

		n2.type = ONOT;
		n2.left = n;
		n2.right = Z;
		n2.other = Z;

		n = &n1;
	}
	gencode(n);
	v = callit();
	return v;
}

callit()
{
	register x, y, z;
	extern program();

	x = y = z = 0;
asm("		jsb	*_program			");
asm("	x:	brb	x+9				");	
		x = program();
	return(x);
}

