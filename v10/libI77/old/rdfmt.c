/*	@(#)rdfmt.c	1.4	*/

#include "fio.h"
#include "fmt.h"
extern int cursor;
rd_ed(p,ptr,len) char *ptr; struct syl *p; ftnlen len;
{	int ch;
	for(;cursor>0;cursor--) if((ch=(*getn)())<0) return(ch);
	if(cursor<0)
	{	if(recpos+cursor < 0) /*err(elist->cierr,110,"fmt")*/
			cursor = -recpos;	/* is this in the standard? */
		if(external == 0) {
			extern char *icptr;
			icptr += cursor;
		}
		else if(curunit->useek) (void) fseek(cf,(long) cursor,1);
		else err(elist->cierr,106,"fmt");
		recpos += cursor;
		cursor=0;
	}
	switch(p->op)
	{
	default: fprintf(stderr,"rd_ed, unexpected code: %d\n%s\n",
			p->op,fmtbuf);
		abort();
	case I: ch = (rd_I((uint *)ptr,p->p1,len, 10));
		break;
	case IM: ch = (rd_I((uint *)ptr,p->p1,len, 10));
		break;
	case O: ch = (rd_I((uint *)ptr, p->p1, len, 8));
		break;
	case L: ch = (rd_L((ftnint *)ptr,p->p1));
		break;
	case A:	ch = (rd_A(ptr,len));
		break;
	case AW:
		ch = (rd_AW(ptr,p->p1,len));
		break;
	case E: case EE:
	case D:
	case G:
	case GE:
	case F:	ch = (rd_F((ufloat *)ptr,p->p1,p->p2,len));
		break;
	}
	if(ch == 0) return(ch);
	else if(feof(cf)) return(EOF);
	clearerr(cf);
	return(errno);
}
rd_ned(p) struct syl *p;
{
	switch(p->op)
	{
	default: fprintf(stderr,"rd_ned, unexpected code: %d\n%s\n",
			p->op,fmtbuf);
		abort();
	case APOS:
		return(rd_POS(p->p1));
	case H:	return(rd_H(p->p1,p->p2));
	case SLASH: return((*donewrec)());
	case TR:
	case X:	cursor += p->p1;
		return(1);
	case T: cursor=p->p1-recpos - 1;
		return(1);
	case TL: cursor -= p->p1;
		if(cursor < -recpos)	/* TL1000, 1X */
			cursor = -recpos;
		return(1);
	}
}
rd_I(n,w,len, base) ftnlen len; uint *n; register int base;
{	long x;
	int sign,ch;
	char s[84], *ps, *qs;
	ps=s; x=0;
	while (w)
	{
		GET(ch);
		if (ch==',' || ch=='\n') break;
		*ps=ch; ps++; w--;
	}
	*ps='\0';
	ps=s;
	while (*ps==' ') ps++;
	if (*ps=='-') { sign=1; ps++; }
	else { sign=0; if (*ps=='+') ps++; }
loop:	while (*ps>='0' && *ps<='9') { x=x*base+(*ps-'0'); ps++; }
	if (*ps==' ') {if (cblank) x *= base; ps++; goto loop;}
	if(sign) x = -x;
	if(len==sizeof(short)) n->is=x;
	else if(len == sizeof(char)) n->ic = x;
	else n->il=x;
	if (*ps) return(errno=115); else return(0);
}
rd_L(n,w) ftnint *n;
{	int ch;
	char s[84], *ps;
	ps=s;
	while (w) {
		GET(ch);
		if (ch==','||ch=='\n') break;
		*ps=ch;
		ps++; w--;
		}
	*ps='\0';
	ps=s; while (*ps==' ') ps++;
	if (*ps=='.') ps++;
	if (*ps=='t' || *ps == 'T') { *n=1; return(0); }
	else if (*ps='f' || *ps == 'F') { *n=0; return(0); }
	else return(errno=116);
}
double ten_pow[]  = {1e1,1e2,1e3,1e4,1e5,1e6,1e7,1e8,1e9};
rd_F(p, w, d, len) 
ftnlen len; 
ufloat *p;
{	char	s[84], *sp;
	double	x;
	int	ch, nfrac, exp, dot, sx, se;
	sp = s;
	x = 0.0;
	dot = 1; /* no dot */
	while (w) {
		GET(ch); 
		w--;
		switch (ch) {
		case '\n':
		case ',':
			w = 0; 
			break;
		default:
			*sp++ = ch;
		}
	}
	*sp = '\0'; 
	sp = s;
	while (*sp == ' ') 
		sp++;
	if (*sp == '-') { 
		sx = 1; 
		sp++; 
	} else { 
		sx = 0; 
		if (*sp == '+') 
			sp++; 
	}
loop1:
	while (*sp >= '0' && *sp <= '9') {
		x = x * 10 + (*sp - '0');
		sp++;
	}
	if (*sp == ' ') { 
		if (cblank) 
			x *= 10; 
		sp++; 
		goto loop1;
	}
	nfrac = 0;
	if (*sp == '.') {
		sp++; 
		dot = 0;
loop2:
		while (*sp >= '0' && *sp <= '9') {
			x = x * 10 + (*sp - '0');
			nfrac--;
			sp++;
		}
		if (*sp == ' ') {
			if (cblank) {
				x *= 10; 
				nfrac--;
			} 
			sp++; 
			goto loop2;
		}
	}
	if (*sp == 'd' || *sp == 'e' || *sp == 'D' || *sp == 'E') { 
		sp++; 
	} else 
		nfrac -= scale;
	while (*sp == ' ') 
		sp++;
	if (*sp == '-') { 
		sp++; 
		se = 1; 
	} else { 
		se = 0; 
		if (*sp == '+') 
			sp++; 
	}
	exp = 0;
loop3:
	while (*sp >= '0' && *sp <= '9') {
		exp = exp * 10 + (*sp - '0');
		sp++;
	}
	if (*sp == ' ') {
		if (cblank) 
			exp *= 10; 
		sp++; 
		goto loop3;
	}
	if (se) 
		exp = nfrac - exp; 
	else 
		exp += nfrac;
	if (dot) 
		exp -= d;
	if (exp > 0) {
		while (exp > 9) { 
			x *= 1e9; 
			exp -= 9; 
		}
		x *= ten_pow[exp-1];
	} else if (exp < 0) {
		exp = -exp;
		while (exp > 9) { 
			x *= 1e-9; 
			exp -= 9; 
		}
		x /= ten_pow[exp-1];
	}
	if (sx)  
		x = -x;
	if (len == sizeof(float)) 
		p->pf = x; 
	else 
		p->pd = x;
	if (*sp) 
		return(errno = 115); 
	else 
		return(0);
}


rd_A(p,len) char *p; ftnlen len;
{	int i,ch;
	for(i=0;i<len;i++)
	{	GET(ch);
		*p++=VAL(ch);
	}
	return(0);
}
rd_AW(p,w,len) char *p; ftnlen len;
{	int i,ch;
	if(w>=len)
	{	for(i=0;i<w-len;i++)
			GET(ch);
		for(i=0;i<len;i++)
		{	GET(ch);
			*p++=VAL(ch);
		}
		return(0);
	}
	for(i=0;i<w;i++)
	{	GET(ch);
		*p++=VAL(ch);
	}
	for(i=0;i<len-w;i++) *p++=' ';
	return(0);
}
rd_H(n,s) char *s;
{	int i,ch;
	for(i=0;i<n;i++)
		if((ch=(*getn)())<0) return(ch);
		else *s++ = ch=='\n'?' ':ch;
	return(1);
}
rd_POS(s) char *s;
{	char quote;
	int ch;
	quote= *s++;
	for(;*s;s++)
		if(*s==quote && *(s+1)!=quote) break;
		else if((ch=(*getn)())<0) return(ch);
		else *s = ch=='\n'?' ':ch;
	return(1);
}
