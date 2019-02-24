#include "mp.h"
mgcd(a,b,c) mint *a,*b,*c;
{	mint *x,*y,*z,*w;
	x = itom(0), y = itom(0), z = itom(0), w = itom(0);
	move(a,x);
	move(b,y);
	while(y->len!=0){
		mdiv(x,y,w,z);
		move(y,x);
		move(z,y);
	}
	move(x,c);
	mfree(x);
	mfree(y);
	mfree(z);
	mfree(w);
}
invert(a, b, c) mint *a, *b, *c;
{	mint x, y, z, w, Anew, Aold;
	int i = 0;
	x.len = y.len = z.len = w.len = Aold.len = 0;
	Anew.len = 1;
	Anew.val = xalloc(1, "invert");
	*Anew.val = 1;
	move(b, &x);
	move(a, &y);
	while(y.len != 0)
	{	mdiv(&x, &y, &w, &z);
		move(&Anew, &x);
		mult(&w, &Anew, &Anew);
		madd(&Anew, &Aold, &Anew);
		move(&x, &Aold);
		move(&y, &x);
		move(&z, &y);
		i++;
	}
	move(&Aold, c);
	if( (i&01) == 0) msub(b, c, c);
	xfree(&x);
	xfree(&y);
	xfree(&z);
	xfree(&w);
	xfree(&Aold);
	xfree(&Anew);
}

lineq(a, b, x, y, u)	/* ax + by = u */
mint *a, *b, *x, *u, *y;
{	mint *at, *bt, *xo, *yo, *q, *r, *z;
	static mint *zero, *one;
	int i;
	at = itom(0), bt = itom(0), xo = itom(0);
	yo = itom(0), q = itom(0), r = itom(0), z = itom(0);
	if(zero == 0) zero = itom(0);
	if(one == 0) one = itom(1);
	move(a, at);
	move(b, bt);
	move(zero, x);
	move(one, xo);
	move(zero, yo);
	move(one, y);
	if(bt->len == 0) {
		move(one, x);
		move(zero, y);
		move(a, u);
		goto out;
	}
	for(i = 0; ; i++) {
		mdiv(at, bt, q, r);
		if(r->len == 0)
			break;
		move(xo, z);
		move(x, xo);
		mult(q, xo, x);
		madd(z, x, x);
		move(yo, z);
		move(y, yo);
		mult(q, yo, y);
		madd(z, y, y);
		move(bt, at);
		move(r, bt);
	}
	move(bt, u);
	if(i & 1)
		y->len = -y->len;
	else
		x->len = -x->len;
out:
	mfree(z), mfree(r), mfree(q), mfree(yo);
	mfree(xo), mfree(bt), mfree(at);
}
