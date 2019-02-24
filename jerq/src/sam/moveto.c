#include "sam.h"

moveto(f, r)
	register File *f;
	Range r;
{
	register Posn p1=r.p1, p2=r.p2;
	f->dot.r.p1=p1;
	f->dot.r.p2=p2;
	if(f->rasp){
		telldot(f);
		outTsl(Hmoveto, f->tag, f->dot.r.p1);
	}
}
telldot(f)
	register File *f;
{
	if(f->rasp==0)
		panic("telldot");
	if(f->dot.r.p1==f->tdot.p1 && f->dot.r.p2==f->tdot.p2)
		return;
	outTsll(Hsetdot, f->tag, f->dot.r.p1, f->dot.r.p2);
	f->tdot=f->dot.r;
}
tellpat()
{
	uchar buf[16];
	bcopy(lastpat.s, lastpat.s+15, buf, 1);
	buf[15]=0;
	outTS(Hsetpat, buf);
	patset=FALSE;
}
#define	CHARSHIFT	128
lookorigin(f, p0, ls)
	register File *f;
	register Posn p0, ls;
{
	register nl, nc, c;
	register Posn oldp0;
	if(p0>f->nbytes)
		p0=f->nbytes;
	oldp0=p0;
	Fgetcset(f, p0);
	for(nl=nc=c=0; c!=-1 && nl<ls && nc<ls*CHARSHIFT; nc++)
		if((c=Fbgetc(f))=='\n'){
			nl++;
			oldp0=p0-nc;
		}
	if(c==-1)
		p0=0;
	else if(nl==0){
		if(p0>=CHARSHIFT/2)
			p0-=CHARSHIFT/2;
		else
			p0=0;
	}else
		p0=oldp0;
	outTsl(Horigin, f->tag, p0);
}
char *left[]= {
	"{[(<",
	"\n",
	"'\"`",
	0
};
char *right[]= {
	"}])>",
	"\n",
	"'\"`",
	0
};
doubleclick(f, p1)
	register File *f;
	register Posn p1;
{
	register c, i;
	register char *r, *l;
	if(p1>f->nbytes)
		return;
	f->dot.r.p1=f->dot.r.p2=p1;
	for(i=0; left[i]; i++){
		l=left[i];
		r=right[i];
		/* try left match */
		if(p1==0){
			Fgetcset(f, p1);
			c='\n';
		}else{
			Fgetcset(f, p1-1);
			c=Fgetc(f);
		}
		if(strchr(l, c)){
			if(clickmatch(f, c, r[strchr(l, c)-l], 1)){
				f->dot.r.p1=p1;
				f->dot.r.p2=f->getcp-(c!='\n');
			}
			return;
		}
		/* try right match */
		if(p1==f->nbytes){
			Fbgetcset(f, p1);
			c='\n';
		}else{
			Fbgetcset(f, p1+1);
			c=Fbgetc(f);
		}
		if(strchr(r, c)){
			if(clickmatch(f, c, l[strchr(r, c)-r], -1)){
				f->dot.r.p1=f->getcp;
				if(c!='\n' || f->getcp!=0 ||
				   (Fgetcset(f, (Posn)0),Fgetc(f))=='\n')
					f->dot.r.p1++;
				f->dot.r.p2=p1+(p1<f->nbytes && c=='\n');
			}
			return;
		}
	}
	/* try filling out word to right */
	Fgetcset(f, p1);
	while(alnum(Fgetc(f)))
		f->dot.r.p2++;
	/* try filling out word to left */
	Fbgetcset(f, p1);
	while(alnum(Fbgetc(f)))
		f->dot.r.p1--;
}
alnum(c)
	register c;
{
	return ('0'<=c && c<='9') || (c=='_') ||
	       ('a'<=c && c<='z') || ('A'<=c && c<='Z')  ||
/*	       ('À'<=c && c<='ÿ');  many have trouble with latin-1*/
	       (0300<=c && c<=0377);
}
clickmatch(f, cl, cr, dir)
	register File *f;
	register cl, cr;
	int dir;
{
	register c;
	register nest=1;
	while((c=(dir>0? Fgetc(f) : Fbgetc(f)))>0)
		if(c==cr){
			if(--nest==0)
				return 1;
		}else if(c==cl)
			nest++;
	return cl=='\n' && --nest==0;
}
