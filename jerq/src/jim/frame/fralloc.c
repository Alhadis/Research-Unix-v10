#include "frame.h"
#ifdef	commentedin
Frame *
fralloc(r, m)
	Rectangle r;
{
	register Frame *t=(Frame *)alloc(sizeof(Frame));
	if(t){
		strzero(&t->str);
		setrects(t, r, m);
	}
	return t;
}
setrects(t, r, m)
	register Frame *t;
	Rectangle r;
{
	t->totalrect = r;
	t->rect = inset(r, m);
	t->rect.corner.y -= (t->rect.corner.y-t->rect.origin.y)%newlnsz;
	t->nlines = (t->rect.corner.y-t->rect.origin.y)/newlnsz;
	t->scrollrect = t->rect;
	t->scrollrect.origin.x += 2;
	t->scrollrect.corner.x = t->scrollrect.origin.x+SCROLLWIDTH;
	t->rect.origin.x += SCROLLWIDTH+SCROLLWIDTH/2;
	setcpl(t, 0, t->nlines-1);
}
frinit(t)
	register Frame *t;
{
	box(t);
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
#endif
