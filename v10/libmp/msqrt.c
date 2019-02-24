#include "mp.h"
msqrt(a,b,r) mint *a,*b,*r;
{	mint x,junk,y;
	int j;
	x.len=junk.len=y.len=0;
	if(a->len<0) fatal("msqrt: neg arg");
	if(a->len==0)
	{	b->len=0;
		r->len=0;
		return(0);
	}
	if(a->len%2==1) x.len=(1+a->len)/2;
	else x.len=1+a->len/2;
	x.val=xalloc(x.len,"msqrt");
	for(j=0;j<x.len;x.val[j++]=0);
	if(a->len%2==1) x.val[x.len-1]=0400;
	else x.val[x.len-1]=1;
	xfree(b);
	xfree(r);
loop:
	mdiv(a,&x,&y,&junk);
	xfree(&junk);
	madd(&x,&y,&y);
	sdiv(&y,2,&y,(short *)&j);
	if(mcmp(&x,&y)>0)
	{	xfree(&x);
		move(&y,&x);
		xfree(&y);
		goto loop;
	}
	xfree(&y);
	move(&x,b);
	mult(&x,&x,&x);
	msub(a,&x,r);
	xfree(&x);
	return(r->len);
}

/* pathology: n<= 0 => r=rem=0, num <= 0, r=rem=0 */
/* this is a lazy implementation, assuming n>=3 => root is a legal double */
mroot(n, num, r, rem)
mint *num, *r, *rem;
{	extern double log(), exp();
	double z;
	int i;
	static mint *xn, *xn1, *top, *bot;
	static init;
	if(!init++) {
		xn = itom(0), xn1 = itom(0), top = itom(0), bot = itom(0);
	}
	if(n < 0 || num->len <= 0) {
		msub(r, r, r);
		move(r, rem);
		return;
	}
	if(n == 1) {
		move(num, r);
		msub(rem, rem, rem);
		return;
	}
	if(n == 2) {
		msqrt(num, r, rem);
		return;
	}
	/* compute approx bigger than root */
	for(z = 0, i = 0; i < num->len; i++)
		z = z/32768 + num->val[i];
	/* num = z * 2^15*(len-1) */
	z = exp((log(z) + 15*(num->len-1)*log(2.))/n);
	z = 1.01 * z + 1;	/* make sure it is bigger than root */
	dtom(z, r);
	/* ((n-1)*x^n+num)/(n*x^(n-1))*/
	for(;;) {
		rpow(r, n - 1, xn1);
		mult(r, xn1, xn);
		imult(n-1, xn, top);
		madd(top, num, top);
		imult(n, xn1, bot);
		mdiv(top, bot, xn1, rem);
		switch(mcmp(xn1, r)) {
		case -1:
			move(xn1, r);
			continue;
		case 0:
			move(xn1, r);
			msub(num, xn, rem);
			return;
		case 1:	/* since r was too large to start with */
			msub(num, xn, rem);
			return;
		}
	}
}

static
imult(n, a, b)
mint *a, *b;
{	mint *x;
	x = itom(n);
	mult(x, a, b);
	msub(x, x,x);
}
