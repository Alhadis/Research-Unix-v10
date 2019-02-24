#include "frame.h"

frinit(f, r, ft, b)
	register Frame *f;
	Rectangle r;
	register Font *ft;
	Bitmap *b;
{
	register i, max;
	f->font=ft;
	f->maxtab=8*ft->info['1'].width;
	max=0;
	for(i=0; i<ft->n; i++)
		if(ft->info[i].width>max)
			max=ft->info[i].width;
	f->maxcharwid=max;
	f->nbox=f->nalloc=0;
	f->nchars=f->nlines=0;
	f->p0=f->p1=0;
	f->box=0;
	f->lastlinefull=1;
	frsetrects(f, r, b);
}
frsetrects(f, r, b)
	register Frame *f;
	Rectangle r;
	Bitmap *b;
{
	f->b=b;
	f->entire=f->r=r;
	f->r.corner.y-=(r.corner.y-r.origin.y)%f->font->height;
	f->left=r.origin.x+1;
	f->maxlines=(r.corner.y-r.origin.y)/f->font->height;
}
frclear(f)
	register Frame *f;
{
	if(f->nbox)
		delbox(f, 0, f->nbox-1);
	gcfree(f->box);
	f->box=0;
}
