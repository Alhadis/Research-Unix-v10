#include "frame.h"

frdelete(f, p0, p1)
	register Frame *f;
	Posn p0, p1;
{
	Point pt0, pt1, ppt0;
	register Box *b;
	register n0, n1, n;
	Rectangle r;
	int nn0;
	if(p0>=f->nchars || p0==p1 || f->b==0)
		return 0;
	if(p1>f->nchars)
		p1=f->nchars;
	n0=findbox(f, 0, 0, p0);
	n1=findbox(f, n0, p0, p1);
	pt0=ptofcharnb(f, p0, n0);
	pt1=ptofchar(f, p1);
	if(f->p0!=p0 || f->p1!=p1)	/* likely they ARE equal */
		selectp(f, F_XOR);	/* can do better some day */
	selectf(f, pt0, pt1, F_CLR);
	if(n0==f->nbox)
		panic("off end in frdelete");
	nn0=n0;
	ppt0=pt0;
	freebox(f, n0, n1-1);
	/*
	 * Invariants:
	 *  pt0 points to beginning, pt1 points to end
	 *  n0 is box containing beginning of stuff being deleted
	 *  n1, b are box containing beginning of stuff to be kept after deletion
	 *  region between pt0 and pt1 is clear
	 */
	b=&f->box[n1];
	while(pt1.x!=pt0.x && n1<f->nbox){
		cklinewrap0(f, &pt0, b);
		cklinewrap(f, &pt1, b);
		if(b->len>0){
			n=canfit(f, pt0, b);
			if(n==0)
					panic("canfit==0");
			if(n!=b->len){
				splitbox(f, n1, n);
				b=&f->box[n1];
			}
			r.origin=r.corner=pt1;
			r.corner.x+=b->wid, r.corner.y+=f->font->height;
			bitblt(B, r, B, pt0, F_STORE);
			if(pt0.y==pt1.y)
				r.origin.x=r.corner.x-(pt1.x-pt0.x);
			rectf(B, r, F_CLR);
		}
		advance(f, &pt1, b);
		pt0.x+=newwid(f, pt0, b);
		f->box[n0++]=f->box[n1++];
		b++;
	}
	if(pt1.y!=pt0.y){
		Point pt2;
		pt2=ptofcharptb(f, 32767, pt1, n1);
		if(pt2.y>f->r.corner.y)
			panic("ptofchar in frdelete");
		if(n1<f->nbox){
			int q0, q1, q2;
			q0=pt0.y+f->font->height, q1=pt1.y+f->font->height;
			q2=pt2.y+f->font->height;
			bitblt(B, Rect(pt1.x, pt1.y, f->r.corner.x, q1),
			       B, pt0, F_STORE);
			bitblt(B, Rect(f->r.origin.x, q1, f->r.corner.x, q2),
			       B, Pt(f->r.origin.x, q0), F_STORE);
			selectf(f, Pt(pt2.x, pt2.y-(pt1.y-pt0.y)), pt2, F_CLR);
		}else
			selectf(f, pt0, pt2, F_CLR);
	}
	closebox(f, n0, n1-1);
	if(nn0>0 && f->box[nn0-1].len>=0 && ppt0.x-f->box[nn0-1].wid>=(int)f->left){
		--nn0;
		ppt0.x-=f->box[nn0].wid;
	}
	clean(f, ppt0, nn0, n0<f->nbox-1? n0+1 : n0);
	if(f->p1>p1)
		f->p1-=p1-p0;
	else if(f->p1>p0)
		f->p1=p0;
	if(f->p0>p1)
		f->p0-=p1-p0;
	else if(f->p0>p0)
		f->p0=p0;
	selectp(f, F_XOR);
	f->nchars-=p1-p0;
	if(pt0.x!=pt1.x)
		f->lastlinefull=pt0.x==f->left;
	pt0=ptofchar(f, f->nchars);
	n=f->nlines;
	f->nlines=(pt0.y-f->r.origin.y)/f->font->height+(pt0.x>f->left);
	return n-f->nlines;
}
