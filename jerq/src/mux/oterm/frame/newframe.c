#include <jerq.h>
#include <layer.h>
#include <queue.h>
#include <jerqproc.h>
#include "frame.h"

Rectangle Null /* ={0,0,0,0} */;
Frame *
fralloc(r, m)
	Rectangle r;
{
	register Frame *t=(Frame *)Ualloc(sizeof(Frame));
	if(t){
		strzero(&t->str);
		t->margin=m;
		frsetrects(t, r);
	}
	return t;
}
frsetrects(f, r)
	register Frame *f;
	Rectangle r;
{
	f->totalrect=r;
	f->rect=inset(r, f->margin);
	f->rect.corner.y-=(f->rect.corner.y-f->rect.origin.y)%newlnsz;
	f->scrollrect=f->rect;
	f->scrollrect.origin.x+=2;
	f->scrollrect.corner.x=f->scrollrect.origin.x+SCROLLWIDTH;
	f->rect.origin.x+=3*SCROLLWIDTH/2;
	f->nlines=(f->rect.corner.y-f->rect.origin.y)/newlnsz;
	setcpl(f, 0, f->nlines-1);
}
frinit(t)
	register Frame *t;
{
	clear(t->totalrect, 1);
	border(P->layer, t->totalrect, t->margin, F_OR);
	strzero(&t->str);
	t->s1=t->s2=0;
	setcpl(t, 0, t->nlines-1);
}
frfree(f)
	register Frame *f;
{
	if(f==0 || f->str.s==0)
		return;
	gcfree(f->str.s);
	free(f);
}
drawframe(f)
	register Frame *f;
{
	int compl;
	clear(P->layer->rect, 1);
	border(P->layer, P->layer->rect, f->margin, F_OR);
	drawscrollbar(f);
	draw(f, f->rect.origin, f->str.s, f->str.n);
	compl=complete;
	selectf(f, F_XOR);
	complete=compl;
}
closeframe(t)
	register Frame *t;
{
	if(t==0 || t->str.s==0)
		return;
	gcfree(t->str.s);
	t->str.s=0;
	t->str.n=t->str.size=0;
	t->totalrect=Null;
	t->rect=Null;
	t->scrollrect=Null;
	t->str.s=0;	/* Mark it available */
}
setsel(t, n)
	register Frame *t;
	register n;
{
	t->s1=t->s2=n;
}
