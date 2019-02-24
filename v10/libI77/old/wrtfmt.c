/*	@(#)wrtfmt.c	1.3	*/
/*	3.0 SID #	1.2	*/
#include "fio.h"
#include "fmt.h"
extern int cursor;
extern char *icvt(), *ecvt();
int hiwater;
icilist *svic;
char *icptr;
mv_cur()	/* shouldn't use fseek because it insists on calling fflush */
		/* instead we know too much about stdio */
{
	if(external == 0) {
		if(cursor < 0) {
			if(hiwater < recpos)
				hiwater = recpos;
			recpos += cursor;
			icptr += cursor;
			cursor = 0;
			if(recpos < 0)
				err(elist->cierr, 110, "left off");
		}
		else if(cursor > 0) {
			if(recpos + cursor >= svic->icirlen)
				err(elist->cierr, 110, "recend");
			if(hiwater <= recpos)
				for(; cursor > 0; cursor--)
					(*putn)(' ');
			else if(hiwater <= recpos + cursor) {
				cursor -= hiwater - recpos;
				icptr += hiwater - recpos;
				recpos = hiwater;
				for(; cursor > 0; cursor--)
					(*putn)(' ');
			}
			else {
				icptr += cursor;
				recpos += cursor;
			}
			cursor = 0;
		}
		return(0);
	}
	if(cursor > 0) {
		if(hiwater <= recpos)
			for(;cursor>0;cursor--) (*putn)(' ');
		else if(hiwater <= recpos + cursor) {
			if(cf->_ptr + hiwater - recpos < _bufend(cf))
				cf->_ptr += hiwater - recpos;
			else
				(void) fseek(cf, (long) (hiwater - recpos), 1);
			cursor -= hiwater - recpos;
			recpos = hiwater;
			for(; cursor > 0; cursor--)
				(*putn)(' ');
		}
		else {
			if(cf->_ptr + cursor < _bufend(cf))
				cf->_ptr += cursor;
			else
				(void) fseek(cf, (long)cursor, 1);
			recpos += cursor;
		}
	}
	if(cursor<0)
	{
		if(cursor+recpos<0) err(elist->cierr,110,"left off");
		if(cf->_ptr + cursor >= cf->_base)
			cf->_ptr += cursor;
		else if(curunit->useek) (void) fseek(cf,(long)cursor,1);
		else err(elist->cierr,106,"fmt");
		if(hiwater < recpos)
			hiwater = recpos;
		recpos += cursor;
		cursor=0;
	}
	return(0);
}
w_ed(p,ptr,len) char *ptr; struct syl *p; ftnlen len;
{
	if(cursor && mv_cur()) return(mv_cur());
	switch(p->op)
	{
	default:
		fprintf(stderr,"w_ed, unexpected code: %d\n%s\n",
			p->op,fmtbuf);
		abort();
	case I:	return(wrt_I((uint *)ptr,p->p1,len, 10));
	case IM:
		return(wrt_IM((uint *)ptr,p->p1,p->p2,len));
	case O:	return(wrt_I((uint *)ptr, p->p1, len, 8));
	case L:	return(wrt_L((uint *)ptr,p->p1, len));
	case A: return(wrt_A(ptr,len));
	case AW:
		return(wrt_AW(ptr,p->p1,len));
	case D:
	case E:
	case EE:
		return(wrt_E((ufloat *)ptr,p->p1,p->p2,p->p3,len));
	case G:
	case GE:
		return(wrt_G((ufloat *)ptr,p->p1,p->p2,p->p3,len));
	case F:	return(wrt_F((ufloat *)ptr,p->p1,p->p2,len));
	}
}
w_ned(p) struct syl *p;
{
	switch(p->op)
	{
	default: fprintf(stderr,"w_ned, unexpected code: %d\n%s\n",
			p->op,fmtbuf);
		abort();
	case SLASH:
		return((*donewrec)());
	case T: cursor = p->p1-recpos - 1;
		return(1);
	case TL: cursor -= p->p1;
		if(cursor < -recpos)	/* TL1000, 1X */
			cursor = -recpos;
		return(1);
	case TR:
	case X:
		cursor += p->p1;
		return(1);
	case APOS:
		return(wrt_AP(p->p1));
	case H:
		return(wrt_H(p->p1,p->p2));
	}
}
wrt_I(n,w,len, base) uint *n; ftnlen len; register int base;
{	int ndigit,sign,spare,i;
	long x;
	char *ans;
	if(len==sizeof(short)) x=n->is;
	else if(len == sizeof(char)) x = n->ic;
	else x=n->il;
	ans=icvt(x,&ndigit,&sign, base);
	spare=w-ndigit;
	if(sign || cplus) spare--;
	if(spare<0)
		for(i=0;i<w;i++) (*putn)('*');
	else
	{	for(i=0;i<spare;i++) (*putn)(' ');
		if(sign) (*putn)('-');
		else if(cplus) (*putn)('+');
		for(i=0;i<ndigit;i++) (*putn)(*ans++);
	}
	return(0);
}
wrt_IM(n,w,m,len) uint *n; ftnlen len;
{	int ndigit,sign,spare,i,xsign;
	long x;
	char *ans;
	if(sizeof(short)==len) x=n->is;
	else if(len == sizeof(char)) x = n->ic;
	else x=n->il;
	ans=icvt(x,&ndigit,&sign, 10);
	if(sign || cplus) xsign=1;
	else xsign=0;
	if(ndigit+xsign>w || m+xsign>w)
	{	for(i=0;i<w;i++) (*putn)('*');
		return(0);
	}
	if(x==0 && m==0)
	{	for(i=0;i<w;i++) (*putn)(' ');
		return(0);
	}
	if(ndigit>=m)
		spare=w-ndigit-xsign;
	else
		spare=w-m-xsign;
	for(i=0;i<spare;i++) (*putn)(' ');
	if(sign) (*putn)('-');
	else if(cplus) (*putn)('+');
	for(i=0;i<m-ndigit;i++) (*putn)('0');
	for(i=0;i<ndigit;i++) (*putn)(*ans++);
	return(0);
}
wrt_AP(n)
{	char *s,quote;
	if(cursor && mv_cur()) return(mv_cur());
	s=(char *)n;
	quote = *s++;
	for(;*s;s++)
	{	if(*s!=quote) (*putn)(*s);
		else if(*++s==quote) (*putn)(*s);
		else return(1);
	}
	return(1);
}
wrt_H(a,b)
{	char *s=(char *)b;
	if(cursor && mv_cur()) return(mv_cur());
	while(a--) (*putn)(*s++);
	return(1);
}
wrt_L(n,len, sz) uint *n; ftnlen sz;
{	int i;
	long x;
	if(sizeof(short)==sz) x=n->is;
	else if(sz == sizeof(char)) x = n->ic;
	else x=n->il;
	for(i=0;i<len-1;i++)
		(*putn)(' ');
	if(x) (*putn)('t');
	else (*putn)('f');
	return(0);
}
wrt_A(p,len) char *p; ftnlen len;
{
	while(len-- > 0) (*putn)(*p++);
	return(0);
}
wrt_AW(p,w,len) char * p; ftnlen len;
{
	while(w>len)
	{	w--;
		(*putn)(' ');
	}
	while(w-- > 0)
		(*putn)(*p++);
	return(0);
}
wrt_E(p,w,d,e,len) ufloat *p; ftnlen len;
{	char *s;
	int dp,sign,i, delta;
	char *ecvt();
	double dd;
	if(e == 0)
		e = 2;
	if(scale) {
		if(scale >= d + 2 || scale <= -d)
			goto nogood;
		if(scale > 0)
			d++;
	}
	if (len == sizeof(float))
		dd = p->pf;
	else
		dd = p->pd;
	s=ecvt( dd ,d,&dp,&sign);
	delta = 1 /* for the . */ + 2 /* for the E+ */ + (sign || cplus);
	if(w < d + e + delta) {
nogood:
		for(i=0;i<w;i++) (*putn)('*');
		return(0);
	}
	if(p->pf != 0) dp -= scale;
	else	dp = 0;
	switch(e) {	/* grumph */
	case 1:
		if(dp <= -10 || dp >= 10)
			goto nogood;
		break;
	case 2:
		if(dp <= -100 || dp >= 100)
			goto nogood;
		break;
	case 3:
		if(dp <= -1000 || dp >= 1000)
			goto nogood;
		break;
	default:
		/* probably ok */
		break;
	}
	for(i=0;i<w-(delta+d+e);i++) (*putn)(' ');
	if(sign) (*putn)('-');
	else if(cplus) (*putn)('+');
	if(scale<0)
	{
		(*putn)('.');
		for(i=0;i<-scale;i++)
			(*putn)('0');
		for(i=0;i<d+scale;i++)
			(*putn)(*s++);
	}
	else if(scale>0 && scale<d+2)
	{	for(i=0;i<scale;i++)
			(*putn)(*s++);
		(*putn)('.');
		for(i=0;i<d-scale;i++)
			(*putn)(*s++);
	}
	else
	{	(*putn)('.');
		for(i=0;i<d;i++) (*putn)(*s++);
	}
	if(dp < 100 && dp > -100) (*putn)('e');
	if(dp<0)
	{	(*putn)('-');
		dp = -dp;
	}
	else	(*putn)('+');
	for(; e >= 4; e--)
		(*putn)('0');
	if(e>=3 || dp >= 100)
	{	(*putn)(dp/100 + '0');
		dp = dp % 100;
	}
	if(e!=1) (*putn)(dp/10+'0');
	(*putn)(dp%10+'0');
	return(0);
}
wrt_G(p,w,d,e,len) ufloat *p; ftnlen len;
{	double up = 1,x;
	int i,oldscale=scale,n,j;
	x= len==sizeof(float)?p->pf:p->pd;
	if(x < 0 ) x = -x;
	if(x<.1) return(wrt_E(p,w,d,e,len));
	for(i=0;i<=d;i++,up*=10)
	{	if(x>=up) continue;
		scale=0;
		if(e==0) n=4;
		else	n=e+2;
		i=wrt_F(p,w-n,d-i,len);
		for(j=0;j<n;j++) (*putn)(' ');
		scale=oldscale;
		return(i);
	}
	return(wrt_E((ufloat *)p,w,d,e,len));
}
wrt_F(p,w,d,len) ufloat *p; ftnlen len;
{	int i,delta,dp,sign,n;
	double x;
	char *s,*fcvt();
	x= (len==sizeof(float)?p->pf:p->pd);
	if(scale)
	{	if(scale>0)
			for(i=0;i<scale;i++) x*=10;
		else	for(i=0;i<-scale;i++) x/=10;
	}
	s=fcvt(x,d,&dp,&sign);
	if(-dp>=d) sign=0;
	if(sign || cplus) delta=2;
	else delta=1;
	n= w - (d+delta+(dp>0?dp:0));
	if(n<0)
	{
		for(i=0;i<w;i++) PUT('*');
		return(0);
	}
	for(i=0;i<n;i++) PUT(' ');
	if(sign) PUT('-');
	else if(cplus) PUT('+');
	for(i=0;i<dp;i++) PUT(*s++);
	PUT('.');
	for(i=0;i< -dp && i<d;i++) PUT('0');
	for(;i<d;i++)
	{	if(*s) PUT(*s++);
		else PUT('0');
	}
	return(0);
}
