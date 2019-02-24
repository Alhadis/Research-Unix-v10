#include "frame.h"

canfit(f, pt, b)
	register Frame *f;
	register Box *b;
	Point pt;
{
	register left=f->r.corner.x-pt.x;
	register uchar *p;
	if(b->len<0)
		return b->minwid<=left;
	if(left>=b->wid)
		return b->len;
	for(p=b->ptr; *p; p++)
		if((left-=(int)f->font->info[*p].width)<0) /* (int) for 3cc bug */
			return p-b->ptr;
	panic("canfit can't");
	return 0;	/* for cyntax */
}
cklinewrap(f, p, b)
	register Frame *f;
	register Point *p;
	register Box *b;
{
	if((b->len<0? b->minwid : b->wid)>f->r.corner.x-p->x)
		p->x=f->left, p->y+=f->font->height;
}
cklinewrap0(f, p, b)
	register Frame *f;
	register Point *p;
	register Box *b;
{
	if(canfit(f, *p, b)==0)
		p->x=f->left, p->y+=f->font->height;
}
advance(f, p, b)
	register Frame *f;
	register Point *p;
	register Box *b;
{
	if(b->len<0 && b->bc=='\n')
		p->x=f->left, p->y+=f->font->height;
	else
		p->x+=b->wid;
}
newwid(f, pt, b)
	register Frame *f;
	Point pt;
	Box *b;
{
	register c=f->r.corner.x;
	register x=pt.x;
	if(b->len>=0)
		return b->wid;
	if(b->bc=='\t'){
		if(x+b->minwid>c)
			x=pt.x=f->left;
		x+=f->maxtab;
		x-=(x-f->left)%f->maxtab;
		if(x-pt.x<b->minwid)
			x=pt.x+b->minwid;
		b->wid=x-pt.x;
	}
	return b->wid;
}
clean(f, pt, n0, n1)	/* look for mergeable boxes */
	register Frame *f;
	Point pt;
{
	register nb;
	register Box *b;
	register c=f->r.corner.x;
	for(nb=n0; nb<n1-1; nb++){
		b=&f->box[nb];
		cklinewrap(f, &pt, b);
		while(b[0].len>=0 && nb<n1-1 && b[1].len>=0 &&
		       pt.x+b[0].wid+b[1].wid<c){
				mergebox(f, nb);
				n1--;
		}
		advance(f, &pt, b);
	}
}
