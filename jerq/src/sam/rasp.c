#include "sam.h"
/*
 * GROWDATASIZE must be big enough that all errors go out as Hgrowdata's,
 * so they will be scrolled into visibility in the ~~sam~~ window (yuck!).
 */
#define	GROWDATASIZE	50	/* if size is > this, send data with grow */

toterminal(f, toterm)
	register File *f;
{
	register Buffer *t=f->transcript;
	register Posn n, p0, p1, p2, nbytes, delta=0, deltacmd=0;
	Range r;
	uchar buf[16+GROWDATASIZE];
	if(f->rasp==0)
		return;
	nbytes=f->nbytes;
	if(f->marked)
		p0=f->markp+sizeof(Mark);
	else
		p0=0;
	while(Bread(t, buf, sizeof buf, p0)>0){
		switch(buf[0]){
		default:
			panic("unknown in toterminal");
		case 'd':
			GETPOSN(p1, buf+1);
			GETPOSN(p2, buf+1+sizeof(Posn));
			p1+=delta, p2+=delta;
			if(p2<=p1)
				panic("toterminal delete 0");
			if(f==cmd && p1<cmdpt){
				if(p2<=cmdpt)
					deltacmd-=(p2-p1);
				else
					deltacmd-=cmdpt-p1;
			}
			p0+=1+2*sizeof(Posn);
			if(toterm)
				outTsll(Hcut, f->tag, p1, p2-p1);
			rcut(f->rasp, p1, p2);
			delta-=p2-p1;
			nbytes-=p2-p1;
			break;
		case 'f':
			n=buf[1]&0xFF;
			n|=buf[2]<<8;
			p0+=1+SS+n;
			break;
		case 'i':
			n=buf[1]&0xFF;
			n|=buf[2]<<8;
			if(n<=0)
				panic("toterminal insert 0");
			GETPOSN(p1, (buf+1+SS));
			p1+=delta;
			p0+=1+SS+sizeof(Posn)+n;
			if(f==cmd && p1<cmdpt)
				deltacmd+=n;
			if(toterm){
				if(n>GROWDATASIZE || !rterm(f->rasp, p1)){
					rgrow(f->rasp, p1, n);
					outTsll(Hgrow, f->tag, p1, n);
				}else{
					uchar *s=buf+1+SS+sizeof(Posn);
					rgrow(f->rasp, p1, n);
					r=rdata(f->rasp, p1, n);
					if(r.p1!=p1 || r.p2!=p1+n)
						panic("rdata in toterminal");
					s[n]=0;
					outTsllS(Hgrowdata, f->tag, p1, n, s);
				}
			}else{
				rgrow(f->rasp, p1, n);
				r=rdata(f->rasp, p1, n);
				if(r.p1!=p1 || r.p2!=p1+n)
					panic("rdata in toterminal");
			}
			delta+=n;
			nbytes+=n;
			break;
		}
	}
	if(toterm)
		outTs(Hcheck0, f->tag);
	if(f==cmd){
		cmdpt+=deltacmd+cmdptadv;
		cmdptadv=0;
	}
}
#define	M	0x80000000L
#define	P(i)	r->ptr[i]
#define	T(i)	(P(i)&M)	/* in terminal */
#define	L(i)	(P(i)&~M)	/* length of this piece */
rcut(r, p1, p2)
	register List *r;
	register Posn p1, p2;
{
	register Posn p, x;
	register i;
	if(p1==p2)
		panic("rcut 0");
	for(p=0,i=0; i<r->nused && p+L(i)<=p1; p+=L(i++))
		;
	if(i==r->nused)
		panic("rcut 1");
	if(p<p1){	/* chop this piece */
		if(p+L(i)<p2){
			x=p1-p;
			p+=L(i);
		}else{
			x=L(i)-(p2-p1);
			p=p2;
		}
		if(T(i))
			P(i)=x|M;
		else
			P(i)=x;
		i++;
	}
	while(i<r->nused && p+L(i)<=p2){
		p+=L(i);
		dellist(r, i);
	}
	if(p<p2){
		if(i==r->nused)
			panic("rcut 2");
		x=L(i)-(p2-p);
		if(T(i))
			P(i)=x|M;
		else
			P(i)=x;
	}
	/* can we merge i and i-1 ? */
	if(i>0 && i<r->nused && T(i-1)==T(i)){
		x=L(i-1)+L(i);
		dellist(r, i--);
		if(T(i))
			P(i)=x|M;
		else
			P(i)=x;
	}
}
rgrow(r, p1, n)
	register List *r;
	register Posn p1, n;
{
	register Posn p;
	register i;
	if(n==0)
		panic("rgrow 0");
	for(p=0,i=0; i<r->nused && p+L(i)<=p1; p+=L(i++))
		;
	if(i==r->nused){	/* stick on end of file */
		if(p!=p1)
			panic("rgrow 1");
		if(i>0 && !T(i-1))
			P(i-1)+=n;
		else
			inslist(r, i, n);
	}else if(!T(i))		/* goes in this empty piece */
		P(i)+=n;
	else if(p==p1 && i>0 && !T(i-1))	/* special case; simplifies life */
		P(i-1)+=n;
	else if(p==p1)
		inslist(r, i, n);
	else{			/* must break piece in terminal */
		inslist(r, i+1, (L(i)-(p1-p))|M);
		inslist(r, i+1, n);
		P(i)=(p1-p)|M;
	}
}
rterm(r, p1)
	register List *r;
	register Posn p1;
{
	register Posn p;
	register i;
	for(p=0,i=0; i<r->nused && p+L(i)<=p1; p+=L(i++))
		;
	if(i==r->nused && (i==0 || !T(i-1)))
		return 0;
	return T(i);
}
Range
rdata(r, p1, n)
	register List *r;
	register Posn p1, n;
{
	register Posn p;
	register i;
	Range rg;
	if(n==0)
		panic("rdata 0");
	for(p=0,i=0; i<r->nused && p+L(i)<=p1; p+=L(i++))
		;
	if(i==r->nused)
		panic("rdata 1");
	if(T(i)){
		n-=L(i)-(p1-p);
		if(n<=0){
			rg.p1=rg.p2=p1;
			return rg;
		}
		p+=L(i++);
		p1=p;
	}
	if(T(i) || i==r->nused)
		panic("rdata 2");
	if(p+L(i)<p1+n)
		n=L(i)-(p1-p);
	rg.p1=p1;
	rg.p2=p1+n;
	if(p!=p1){
		inslist(r, i+1, L(i)-(p1-p));
		P(i)=p1-p;
		i++;
	}
	if(L(i)!=n){
		inslist(r, i+1, L(i)-n);
		P(i)=n;
	}
	P(i)|=M;
	/* now i is set; can we merge? */
	if(i<r->nused-1 && T(i+1)){
		P(i)=(n+=L(i+1))|M;
		dellist(r, i+1);
	}
	if(i>0 && T(i-1)){
		P(i)=(n+L(i-1))|M;
		dellist(r, i-1);
	}
	return rg;
}
