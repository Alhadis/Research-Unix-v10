extern char *malloc();
#include "stdio.h"
#include "mp.h"
move(a,b) mint *a,*b;
{	int i,j;
	if(a == b)
		return;
	xfree(b);
	b->len=a->len;
	if((i=a->len)<0)
		i = -i;
	if(i==0)
		return;
	b->val=xalloc(i,"move");
	for(j=0;j<i;j++)
		b->val[j]=a->val[j];
	return;
}
dummy(){ }
/*ARGSUSED*/
short *xalloc(nint,s) char *s;
{	short *i;
	extern short *halloc();
	i = halloc(nint);
#ifdef DBG
	i=(short *)malloc(2*(unsigned)nint+4);
	if(dbg) fprintf(stderr, "%s: %o\n",s,i);
#endif
	if(i!=NULL) return(i);
	fatal("mp: no free space");
	return(0);
}
fatal(s) char *s;
{
	fprintf(stderr,"%s\n",s);
	(void) fflush(stdout);
	sleep(2);
	abort();
}
xfree(c) mint *c;
{
#ifdef DBG
	if(dbg) fprintf(stderr, "xfree ");
#endif
	if(c->len==0) return;
	/*shfree(c->val);*/
	hfree(c->val);
	c->len=0;
	return;
}

mfree(a)
mint *a;
{
	xfree(a);
	hfree(a);
}
mcan(a) mint *a;
{	int i,j;
	if((i=a->len)==0) return;
	else if(i<0) i= -i;
	for(j=i;j>0 && a->val[j-1]==0;j--);
	if(j==i) return;
	if(j==0) {	
		xfree(a);
		return;
	}
	if(a->len > 0) a->len=j;
	else a->len = -j;
}
mint *itom(n)
{	mint *a;
	a=(mint *)xalloc(2,"itom");
	if(n>0) {	
		a->len=1;
		a->val=xalloc(1,"itom1");
		*a->val=n;
		return(a);
	}
	else if(n<0) {	
		a->len = -1;
		a->val=xalloc(1,"itom2");
		*a->val= -n;
		return(a);
	}
	else {	
		a->len=0;
		return(a);
	}
}
mcmp(a,b) mint *a,*b;
{	mint c;
	int res;
	if(a->len < b->len)
		return(-1);
	if(a->len > b->len)
		return(1);
	c.len=0;
	msub(a,b,&c);
	res=c.len;
	xfree(&c);
	if(res < 0)
		return(-1);
	else if(res == 0)
		return(0);
	else
		return(1);
}

dtom(z, r)
double z;
mint *r;
{	int i, sgn;
	static mint *c;
	if(!c) {
		c = itom(16384);
		madd(c, c, c);
	}
	if(z < 0) {
		sgn = 1;
		z = -z;
	}
	else
		sgn = 0;
	for(i = 0; z >= 32768; i++)
		z /= 32768;
	move(c, r);
	r->len = 1;
	r->val[0] = z;
	while(--i >= 0) {
		z -= r->val[0];
		z *= 32768;
		mult(r, c, r);
		r->val[0] = z;
	}
	if(sgn)
		r->len = -r->len;
}
