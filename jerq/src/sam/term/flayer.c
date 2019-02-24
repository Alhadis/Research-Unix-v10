#include "flayer.h"

#define	DELTA	10

static Flayer	**llist;	/* front to back */
static int	nllist;
static int	nlalloc;
static Rectangle lDrect;

Vis		visibility();

flstart(r)
	Rectangle r;
{
	lDrect=r;
}
flnew(l, fn, u0, u1)
	register Flayer *l;
	uchar **(*fn)();
	char *u1;
{
	if(nllist==nlalloc)
		gcrealloc((char **)&llist, nllist*sizeof(Flayer **),
			  (nlalloc+=DELTA)*sizeof(Flayer **));
	l->textfn=fn;
	l->user0=u0;
	l->user1=u1;
	llinsert(l);
}
Rectangle
flrect(l, r)
	register Flayer *l;
	Rectangle r;
{
	l->entire=r;
	l->scroll=inset(r, FLMARGIN);
	r.origin.x=
	    l->scroll.corner.x=r.origin.x+FLMARGIN+FLSCROLLWID+(FLGAP-FLMARGIN);
	return r;
}
flinit(l, r, ft)
	register Flayer *l;
	Rectangle r;
	Font *ft;
{
	lldelete(l);
	llinsert(l);
	l->visible=All;
	l->origin=l->p0=l->p1=0;
	frinit(&l->f, inset(flrect(l, r), FLMARGIN), ft, D);
	newvisibilities(1);
	rectf(D, l->entire, F_CLR);
	scrdraw(l, 0L);
	flborder(l, 0);
}
flclose(l)
	register Flayer *l;
{
	if(l->visible==All)
		rectf(D, l->entire, F_CLR);
	else if(l->visible==Some){
		if(l->f.b==0)
			l->f.b=balloc(l->entire);
		if(l->f.b){
			rectf(l->f.b, l->entire, F_CLR);
			flrefresh(l, l->entire, 0);
		}
	}
	frclear(&l->f);
	lldelete(l);
	if(l->f.b && l->visible!=All)
		bfree(l->f.b);
	l->textfn=0;
	newvisibilities(1);
}
flborder(l, wide)
	register Flayer *l;
{
	if(flprepare(l)){
		border(l->f.b, l->entire, FLMARGIN, F_CLR);
		border(l->f.b, l->entire, wide? FLMARGIN : 1, F_XOR);
		if(l->visible==Some)
			flrefresh(l, l->entire, 0);
	}
}
Flayer *
flwhich(p)
	Point p;
{
	register i;
	if(p.x==0 && p.y==0)
		return nllist? llist[0] : 0;
	for(i=0; i<nllist; i++)
		if(ptinrect(p, llist[i]->entire))
			return llist[i];
	return 0;
}
flupfront(l)
	register Flayer *l;
{
	register v=l->visible;
	lldelete(l);
	llinsert(l);
	if(v!=All)
		newvisibilities(0);
}
newvisibilities(redraw)
	int redraw;	/* if false, we know it's a flupfront, and needn't
			 * redraw anyone becoming partially covered */
{
	register i;
	register Vis ov;
	register Flayer *l;
	for(i=0; i<nllist; i++){
		l=llist[i];
		ov=l->visible;
		l->visible=visibility(l);
#define	V(a, b)	(((a)<<2)|((b)))
		switch(V(ov, l->visible)){
		case V(Some, None):
			bfree(l->f.b);
		case V(All, None):
		case V(All, Some):
			l->f.b=0;
			frclear(&l->f);
			break;
		case V(Some, Some):
			if(l->f.b==0 && redraw)
		case V(None, Some):
				flprepare(l);
			if(l->f.b && redraw){
				flrefresh(l, l->entire, 0);
				bfree(l->f.b);
				l->f.b=0;
				frclear(&l->f);
			}
		case V(None, None):
		case V(All, All):
			break;
		case V(Some, All):
			if(l->f.b){
				bitblt(l->f.b, l->entire, D, l->entire.origin, F_STORE);
				bfree(l->f.b);
				l->f.b=D;
				break;
			}
		case V(None, All):
			flprepare(l);
			break;
		}
		if(ov==None && l->visible!=None)
			flnewlyvisible(l);
	}
}
llinsert(l)
	register Flayer *l;
{
	register i;
	for(i=nllist; i>0; --i)
		llist[i]=llist[i-1];
	llist[0]=l;
	nllist++;
}
lldelete(l)
	register Flayer *l;
{
	register i;
	for(i=0; i<nllist; i++)
		if(llist[i]==l){
			--nllist;
			for(; i<nllist; i++)
				llist[i]=llist[i+1];
			return;
		}
	panic("lldelete");
}
flinsert(l, sp, p0)
	register Flayer *l;
	uchar **sp;
	long p0;
{
	if(flprepare(l)){
		frinsert(&l->f, sp, (Posn)(p0-l->origin));
		scrdraw(l, scrtotal(l));
		if(l->visible==Some)
			flrefresh(l, l->entire, 0);
	}
}
fldelete(l, p0, p1)
	register Flayer *l;
	long p0, p1;
{
	if(flprepare(l)){
		frdelete(&l->f, (Posn)(p0-l->origin), (Posn)(p1-l->origin));
		scrdraw(l, scrtotal(l));
		if(l->visible==Some)
			flrefresh(l, l->entire, 0);
	}
}
flselect(l)
	register Flayer *l;
{
	register ret=0;
	if(l->visible!=All)
		flupfront(l);
	select(&l->f);
	if(l->f.p0==l->f.p1){
		if(realtime()-l->click<Clicktime && l->f.p0+l->origin==l->p0){
			ret=1;
			l->click=0;
		}else
			l->click=realtime();
	}else
		l->click=0;
	l->p0=l->f.p0+l->origin, l->p1=l->f.p1+l->origin;
	return ret;
}
flsetselect(l, p0, p1)
	register Flayer *l;
	long p0, p1;
{
	Posn fp0, fp1;
	l->click=0;
	if(l->visible==None || !flprepare(l)){
		l->p0=p0, l->p1=p1;
		return;
	}
	l->p0=p0, l->p1=p1;
	flfp0p1(l, &fp0, &fp1);
	if(fp0==l->f.p0 && fp1==l->f.p1)
		return;
	selectp(&l->f, F_XOR);
	l->f.p0=fp0, l->f.p1=fp1;
	selectp(&l->f, F_XOR);
	if(l->visible==Some)
		flrefresh(l, l->entire, 0);
}
flfp0p1(l, pp0, pp1)
	register Flayer *l;
	register Posn *pp0, *pp1;
{
	register long p0=l->p0-l->origin, p1=l->p1-l->origin;
	if(p0<0)
		p0=0;
	if(p1<0)
		p1=0;
	if(p0>l->f.nchars)
		p0=l->f.nchars;
	if(p1>l->f.nchars)
		p1=l->f.nchars;
	*pp0=p0, *pp1=p1;
}
Rectangle
rscale(r, old, new)
	Rectangle r;
	Point old, new;
{
	r.origin.x=muldiv(r.origin.x, new.x, old.x);
	r.origin.y=muldiv(r.origin.y, new.y, old.y);
	r.corner.x=muldiv(r.corner.x, new.x, old.x);
	r.corner.y=muldiv(r.corner.y, new.y, old.y);
	return r;
}
flreshape(dr)
	Rectangle dr;
{
	register i;
	register Flayer *l;
	register Frame *f;
	Rectangle r;
	for(i=0; i<nllist; i++){
		l=llist[i];
		f=&l->f;
		r=raddp(rscale(rsubp(l->entire, lDrect.origin),
			sub(lDrect.corner, lDrect.origin),
			sub(dr.corner, dr.origin)), dr.origin);
		if(l->visible==Some && f->b){
			bfree(f->b);
			frclear(f);
		}
		f->b=0;
		if(P->state&MOVED){
			if(l->visible==All)
				f->b=D;
		}else{
			if(l->visible!=None)
				frclear(f);
			if(!rectclip(&r, dr))
				panic("flreshape");
			if(r.corner.x-r.origin.x<100)
				r.origin.x=dr.origin.x;
			if(r.corner.x-r.origin.x<100)
				r.corner.x=dr.corner.x;
			if(r.corner.y-r.origin.y<40)
				r.origin.y=dr.origin.y;
			if(r.corner.y-r.origin.y<40)
				r.corner.x=dr.corner.x;
			l->visible=None;
		}
		frsetrects(f, inset(flrect(l, r), FLMARGIN), f->b);
	}
	lDrect=dr;
	if(!(P->state&MOVED))
		newvisibilities(1);
}
flprepare(l)
	register Flayer *l;
{
	register Frame *f=&l->f;
	register long n;
	if(l->visible==None)
		return 0;
	if(f->b==0){
		if(l->visible==All){
			f->b=D;
			rectf(D, l->entire, F_CLR);
		}else if((f->b=balloc(l->entire))==0)
			return 0;
		border(f->b, l->entire, l==llist[0]? FLMARGIN : 1, F_XOR);
		n=f->nchars;
		frinit(f, f->entire, f->font, f->b);
		frinsert(f, (*l->textfn)(l, n), (Posn)0);
		selectp(f, F_XOR);
		flfp0p1(l, &l->f.p0, &l->f.p1);
		selectp(f, F_XOR);
		scrdraw(l, scrtotal(l));
	}
	return 1;
}
static	somevis, someinvis, justvis;
Vis
visibility(l)
	register Flayer *l;
{
	somevis=someinvis=0;
	justvis=1;
	flrefresh(l, l->entire, 0);
	justvis=0;
	if(somevis==0)
		return None;
	if(someinvis==0)
		return All;
	return Some;
}
flrefresh(l, r, i)
	register Flayer *l;
	Rectangle r;
{
	register Flayer *t;
	Rectangle s;
    Top:
	if((t=llist[i++])==l){
		if(!justvis)
			bitblt(l->f.b, r, D, r.origin, F_STORE);
		somevis=1;
	}else{
		if(!rectXrect(t->entire, r))
			goto Top;	/* avoid stacking unnecessarily */
		if(t->entire.origin.x>r.origin.x){
			s=r;
			s.corner.x=t->entire.origin.x;
			flrefresh(l, s, i);
			r.origin.x=t->entire.origin.x;
		}
		if(t->entire.origin.y>r.origin.y){
			s=r;
			s.corner.y=t->entire.origin.y;
			flrefresh(l, s, i);
			r.origin.y=t->entire.origin.y;
		}
		if(t->entire.corner.x<r.corner.x){
			s=r;
			s.origin.x=t->entire.corner.x;
			flrefresh(l, s, i);
			r.corner.x=t->entire.corner.x;
		}
		if(t->entire.corner.y<r.corner.y){
			s=r;
			s.origin.y=t->entire.corner.y;
			flrefresh(l, s, i);
			r.corner.y=t->entire.corner.y;
		}
		/* remaining piece of r is blocked by t; forget about it */
		someinvis=1;
	}
}
