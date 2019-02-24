#include "mp.h"
int mpdivdebug;
mdiv(a,b,q,r) mint *a,*b,*q,*r;
{	mint *x,*y, *z;
	int alen, blen;
	x = itom(0), y = itom(0);
	z = itom(0);
	move(a, x);
	move(b, y);
	alen = x->len;
	if(x->len<0) {x->len= -x->len;}
	blen = y->len;
	if(y->len<0) {y->len= -y->len;}
	xfree(q);
	xfree(r);
	m_div(x,y,q,r);
	if(mpdivdebug) {
		mult(y, q, z);
		madd(z, r, z);
		if(mcmp(z, x) != 0) {
			mout(a);
			mout(b);
			fatal("mdiv err");
		}
	}
	if(alen < 0) {
		mint o;
		short i = 1;
		o.len = 1;
		o.val = &i;
		if(r->len == 0) {
			if(blen > 0)
				q->len = -q->len;
			goto out;
		}
		msub(r, y, r);
		r->len = -r->len;
		madd(q, &o, q);
		if(blen > 0)
			q->len = -q->len;
		goto out;
	}
	if(blen < 0)
		q->len = -q->len;
out:
	mfree(z);
	mfree(y);
	mfree(x);
}
#ifdef vax
union zz {
	long a;
	struct {
		unsigned int lo:15, hi:15;
	} b;
};
#else
union zz {
	long a;
	struct {
		unsigned int :2, hi:15,lo:15;
	} b;
};
#endif
m_dsb(q,n,a,b) short *a,*b;
{	long int qx, u;
	union zz x;
	int borrow,j;
	qx=q;
	x.a = 0;
	for(j = 0; j < n; j++) {
		x.a = qx * a[j] + x.b.hi;
		if((b[j] -= x.b.lo) < 0) {
			b[j] += (1 << 15);
			x.b.hi += 1;
		}
	}
	if((b[j] -= x.b.hi) >= 0)
		return(0);
	borrow=0;
	for(j=0;j<n;j++)
	{	u=a[j]+b[j]+borrow;
		if(u & 0100000)
			borrow = 1;
		else borrow=0;
		b[j]=u&077777;
	}
	{ return(1);}
}
m_trq(v1,v2,u1,u2,u3)
{	long int d;
	long int x1;
	if(u1==v1) d=077777;
	else d=(u1*0100000L+u2)/v1;
	while(1)
	{	x1=u1*0100000L+u2-v1*d;
		x1=x1*0100000L+u3-v2*d;
		if(x1<0) d=d-1;
		else {return(d);}
	}
}
m_div(a,b,q,r) mint *a,*b,*q,*r;
{	mint u,v,x,w;
	short d,*qval;
	int qq,n,v1,v2,j;
	u.len=v.len=x.len=w.len=0;
	if(b->len==0) { fatal("mdiv divide by zero"); return;}
	if(b->len==1)
	{	r->val=xalloc(1,"m_div1");
		sdiv(a,b->val[0],q,r->val);
		if(r->val[0]==0)
		{	shfree(r->val);
			r->len=0;
		}
		else r->len=1;
		return;
	}
	if(a->len < b->len)
	{	q->len=0;
		r->len=a->len;
		r->val=xalloc(r->len,"m_div2");
		for(qq=0;qq<r->len;qq++) r->val[qq]=a->val[qq];
		return;
	}
	x.len=1;
	x.val = &d;
	n=b->len;
	d=0100000L/(b->val[n-1]+1L);
	mult(a,&x,&u); /*subtle: relies on fact that mult allocates extra space */
	mult(b,&x,&v);
	v1=v.val[n-1];
	v2=v.val[n-2];
	qval=xalloc(a->len-n+1,"m_div3");
	for(j=a->len-n;j>=0;j--)
	{	qq=m_trq(v1,v2,u.val[j+n],u.val[j+n-1],u.val[j+n-2]);
		if(m_dsb(qq,n,v.val,&(u.val[j]))) qq -= 1;
		qval[j]=qq;
	}
	x.len=n;
	x.val=u.val;
	mcan(&x);
	sdiv(&x,d,&w,(short *)&qq);
	r->len=w.len;
	r->val=w.val;
	q->val=qval;
	qq=a->len-n+1;
	if(qq>0 && qval[qq-1]==0) qq -= 1;
	q->len=qq;
	if(qq==0) shfree(qval);
	if(x.len!=0) xfree(&u);
	xfree(&v);
	return;
}
