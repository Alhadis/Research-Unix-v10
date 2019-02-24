#include <stdio.h>
#include "mp.h"
m_in(a,b,f) mint *a; FILE *f;
{	mint x,y,ten;
	int sign,c;
	short qten,qy;
	xfree(a);
	sign=1;
	ten.len=1;
	ten.val= &qten;
	qten=b;
	x.len=0;
	y.len=1;
	y.val= &qy;
	while((c=getc(f))!=EOF)
	switch(c)
	{
	case '\\':	getc(f);
		continue;
	case '\t':
	case '\n':
		goto done;
	case ' ':
		continue;
	case '-': sign = -sign;
		continue;
	default: if(c>='0' && c<= '9')
		{	qy=c-'0';
			mult(&x,&ten,a);
			madd(a,&y,a);
			move(a,&x);
			continue;
		}
		else
		{	(void) ungetc(c,stdin);
done:
			a->len *= sign;
			mcan(a);
			return(0);
		}
	}
	return(EOF);
}
m_out(a,b,f) mint *a; FILE *f;
{	int sign,xlen,i;
	short r;
	mint x;
	char *obuf;
	register char *bp;
	sign=1;
	xlen=a->len;
	if(xlen<0)
	{	xlen= -xlen;
		sign= -1;
	}
	if(xlen==0)
	{	fprintf(f,"0\n");
		return;
	}
	x.len=xlen;
	x.val=xalloc(xlen,"m_out");
	for(i=0;i<xlen;i++) x.val[i]=a->val[i];
	obuf=(char *)malloc(7*xlen);
	bp=obuf+7*xlen-1;
	*bp--=0;
	while(x.len>0)
	{	for(i=0;i<10&&x.len>0;i++)
		{	sdiv(&x,b,&x,&r);
			*bp--=r+'0';
		}
		if(x.len>0) *bp--=' ';
	}
	if(sign==-1) *bp--='-';
	fprintf(f,"%s\n",bp+1);
	free(obuf);
	xfree(&x);
	return;
}
sdiv(a,n,q,r) mint *a,*q; short *r;
{	mint x,y;
	int sign;
	sign=1;
	x.len=a->len;
	x.val=a->val;
	if(n<0) {
		sign= -sign;
		n= -n;
	}
	if(x.len<0) {
		sign = -sign;
		x.len= -x.len;
	}
	else if(x.len == 0) {
		xfree(q);
		*r = 0;
		return;
	}
	s_div(&x,n,&y,r);
	xfree(q);
	q->val=y.val;
	q->len=sign*y.len;
	*r = *r*sign;
	return;
}
s_div(a,n,q,r) mint *a,*q; short *r;
{	int qlen,i;
	long int x;
	short *qval;
	x=0;
	qlen=a->len;
	qval=xalloc(qlen,"s_div");
	for(i=qlen-1;i>=0;i--)
	{
		x=x*0100000L+a->val[i];
		qval[i]=x/n;
		x=x%n;
	}
	*r=x;
	if(qval[qlen-1]==0) qlen--;
	q->len=qlen;
	q->val=qval;
	if(qlen==0) shfree(qval);
	return;
}
min(a) mint *a;
{
	return(m_in(a,10,stdin));
}
omin(a) mint *a;
{
	return(m_in(a,8,stdin));
}
mout(a) mint *a;
{
	m_out(a,10,stdout);
}
omout(a) mint *a;
{
	m_out(a,8,stdout);
}
fmout(a,f) mint *a; FILE *f;
{	m_out(a,10,f);
}
fmin(a,f) mint *a; FILE *f;
{
	return(m_in(a,10,f));
}
