#include "frame.h"

#define	DELTA	25
static Frame		frame;
Point
bxscan(f, sp, ppt)
	Frame *f;
	uchar **sp;
	Point *ppt;
{
	register w, c, nb;
	register uchar *p;
	register Box *b;
	register uchar *s;
	int n;
	frame.r=f->r;
	frame.b=f->b;
	frame.font=f->font;
	frame.maxcharwid=f->maxcharwid;
	frame.maxtab=f->maxtab;
	frame.left=f->left;
	frame.nbox=0;
	for(n=0, nb=0; (*sp)[n]; nb++,frame.nbox++){
		if(nb==frame.nalloc)
			growbox(&frame, DELTA);
		s=&(*sp)[n];
		b=&frame.box[nb];
		if(*s=='\t' || *s=='\n'){
			b->bc=*s;
			b->wid=5000;
			b->minwid=(*s=='\n')? 0 : frame.font->info[' '].width;
			b->len=-1;
			n++;
		}else{
			for(w=0,p=s; (c=*s) && c!='\t' && c!='\n'; s++)
				w+=frame.font->info[c].width;
			*s=0;
			b->ptr=allocstr((int)(s-p+1));
			copystr(p, b->ptr);
			*s=c;
			b->wid=w;
			n+=b->len=s-p;
		}
	}
	frame.nchars=n;
	b=&frame.box[0];
	cklinewrap0(f, ppt, b);
	return draw(&frame, *ppt);
}
frinsert(f, sp, p0)
	register Frame *f;
	uchar **sp;
	Posn p0;
{
	Point pt0, pt1, ppt0, ppt1, pt;
	register Box *b;
	register n, n0, nn0;
	Rectangle r;
	int y;
	static struct{
		Point pt0, pt1;
	}*pts;
	static int nalloc=0;
	int npts;
	if(p0>f->nchars || **sp==0 || f->b==0)
		return;
	nn0=n0=findbox(f, 0, 0, p0);
	ppt0=pt0=ptofcharnb(f, p0, n0);
	ppt1=pt1=bxscan(f, sp, &ppt0);
	if(n0<f->nbox){
		cklinewrap(f, &pt0, b=&f->box[n0]);	/* for selectf() */
		cklinewrap0(f, &ppt1, b);
	}
	/*
	 * ppt0 and ppt1 are start and end of insertion as they will appear when
	 * insertion is complete. pt0 is current location of insertion position
	 * (p0); pt1 is terminal point (without line wrap) of insertion.
	 */
	if(p0==f->p0 && p0==f->p1)		/* quite likely */
		selectf(f, pt0, pt0, F_XOR);
	else
		selectp(f, F_XOR);
	/*
	 * Find point where old and new x's line up
	 * Invariants:
	 *	pt0 is where the next box (b, n0) is now
	 *	pt1 is where it will be after then insertion
	 * If pt1 goes off the rectangle, we can toss everything from there on
	 */
	for(b=&f->box[n0],npts=0;
	     pt1.x!=pt0.x && pt1.y!=f->r.corner.y && n0<f->nbox; b++,n0++,npts++){
		cklinewrap(f, &pt0, b);
		cklinewrap0(f, &pt1, b);
		if(b->len>0){
			n=canfit(f, pt1, b);
			if(n==0)
				panic("canfit==0");
			if(n!=b->len){
				splitbox(f, n0, n);
				b=&f->box[n0];
			}
		}
		if(npts==nalloc){
			gcrealloc((char **)&pts,
				npts*sizeof(pts[0]), (npts+DELTA)*sizeof(pts[0]));
			nalloc+=DELTA;
			b=&f->box[n0];
		}
		pts[npts].pt0=pt0, pts[npts].pt1=pt1;
		/* has a text box overflowed off the frame? */
		if(pt1.y==f->r.corner.y)
			break;
		advance(f, &pt0, b);
		pt1.x+=newwid(f, pt1, b);
	}
	if(pt1.y>f->r.corner.y)
		panic("frinsert pt1 too far");
	if(pt1.y==f->r.corner.y && n0<f->nbox){
		f->nchars-=frstrlen(f, n0);
		delbox(f, n0, f->nbox-1);
	}
	if(n0==f->nbox)
		f->nlines=(pt1.y-f->r.origin.y)/f->font->height+(pt1.x>f->left);
	else if(pt1.y!=pt0.y){
		int q0, q1;
		y=f->r.corner.y;
		q0=pt0.y+f->font->height, q1=pt1.y+f->font->height;
		f->nlines+=(q1-q0)/f->font->height;
		if(f->nlines>f->maxlines)
			chopframe(f, ppt1, p0, nn0);
		if(pt1.y<y){
			r=f->r, r.origin.y=q0, r.corner.y=y-(q1-q0);
			if(q1<y)
				bitblt(B, r, B, Pt(f->r.origin.x, q1), F_STORE);
			r.origin=pt0; r.corner.y=q0;
			bitblt(B, r, B, pt1, F_STORE);
		}
	}
	/*
	 * Move the old stuff down to make room.  The loop will move the stuff
	 * between the insertion and the point where the x's lined up.
	 * The bitblts above moved everything down after the point they lined up.
	 */
	for((y=pt1.y==f->r.corner.y?pt1.y:0),b=&f->box[n0-1]; --npts>=0; --b){
		pt=pts[npts].pt1;
		if(b->len>0){
			r.origin=r.corner=pts[npts].pt0;
			r.corner.x+=b->wid, r.corner.y+=f->font->height;
			bitblt(B, r, B, pt, F_STORE);
			if(pt.y<y){	/* clear bit hanging off right */
				r.origin=r.corner=pt;
				r.origin.x+=b->wid;
				r.corner.x=f->r.corner.x;
				r.corner.y+=f->font->height;
				rectf(B, r, F_CLR);
			}
			y=pt.y;
		}else{
			r.origin=r.corner=pt;
			r.corner.x+=b->wid, r.corner.y+=f->font->height;
			if(r.corner.x>=f->r.corner.x)
				r.corner.x=f->r.corner.x;
			rectf(B, r, F_CLR);
			y=pt.x==f->left? pt.y : 0;
		}
	}
	selectf(f, ppt0, ppt1, F_CLR);
	redraw(&frame, ppt0);
	addbox(f, nn0, frame.nbox);
	for(n=0; n<frame.nbox; n++)
		f->box[nn0+n]=frame.box[n];
	if(nn0>0 && f->box[nn0-1].len>=0 && ppt0.x-f->box[nn0-1].wid>=(int)f->left){
		--nn0;
		ppt0.x-=f->box[nn0].wid;
	}
	n0+=frame.nbox;
	clean(f, ppt0, nn0, n0<f->nbox-1? n0+1 : n0);
	f->nchars+=frame.nchars;
	if(f->p0>=p0)
		f->p0+=frame.nchars;
	if(f->p0>f->nchars)
		f->p0=f->nchars;
	if(f->p1>=p0)
		f->p1+=frame.nchars;
	if(f->p1>f->nchars)
		f->p1=f->nchars;
	if(pt0.x!=pt1.x)
		f->lastlinefull=pt0.x==f->left;
	selectp(f, F_XOR);
}
chopframe(f, pt, p, bn)
	register Frame *f;
	Point pt;
	register Posn p;
	int bn;
{
	register Box *b;
	for(b=&f->box[bn]; ; b++){
		if(b>=&f->box[f->nbox])
			panic("endofframe");
		cklinewrap(f, &pt, b);
		if(pt.y>=f->r.corner.y)
			break;
		p+=LEN(b);
		advance(f, &pt, b);
	}
	f->nchars=p;
	f->nlines=f->maxlines;
if(b<&f->box[f->nbox])				/* BUG */
	delbox(f, (int)(b-f->box), f->nbox-1);
}
