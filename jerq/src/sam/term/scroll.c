#include "samterm.h"
Rectangle	scrpos();
extern Texture	darkgrey;
scrdraw(l, tot)
	register Flayer *l;
	long tot;
{
	Rectangle r;
	register Bitmap *b;
	if(l->f.b==0)
		panic("scrdraw");
	r=l->scroll;
	r.origin.x+=1;	/* border between margin and bar */
	if(l->visible!=All || (b=balloc(r))==0)
		b=l->f.b;
	rectf(b, r, F_OR);
	texture(b, inset(r, 1), &darkgrey, F_STORE);
	rectf(b, scrpos(r, l->origin, l->origin+l->f.nchars, tot), F_CLR);
	if(b!=l->f.b){
		bitblt(b, b->rect, l->f.b, b->rect.origin, F_STORE);
		bfree(b);
	}
}
Rectangle
scrpos(r, p0, p1, tot)
	Rectangle r;
	register long p0, p1, tot;
{
	register long h;
	Rectangle q;
	q=inset(r, 1);
	h=q.corner.y-q.origin.y;
	if(tot==0)
		return q;
	if(tot>1024L*1024L)
		tot>>=10, p0>>=10, p1>>=10;
	if(p0>0)
		q.origin.y+=muldiv(h, p0, tot);
	if(p1<tot)
		q.corner.y-=muldiv(h, tot-p1, tot);
	if(q.corner.y<q.origin.y+2){
		if(q.origin.y+2<=r.corner.y)
			q.corner.y=q.origin.y+2;
		else
			q.origin.y=q.corner.y-2;
	}
	return q;
}
scroll(l, but)
	register Flayer *l;
{
	register in=0, oin;
	register long tot=scrtotal(l);
	Rectangle scr, r, s;
	register x, y, my, oy, h;
	register long p0;
	s=inset(l->scroll, 1);
	x=s.origin.x+FLSCROLLWID/2;
	scr=scrpos(l->scroll, l->origin, l->origin+l->f.nchars, tot);
	r=scr;
	y=scr.origin.y;
	my=mouse.xy.y;
	while(button(but)){
		oin=in;
		in=abs(x-mouse.xy.x)<=FLSCROLLWID/2;
		if(oin!=in)
			scrflip(l, r);
		if(in){
			oy=y;
			my=mouse.xy.y;
			if(my<s.origin.y)
				my=s.origin.y;
			if(my>=s.corner.y)
				my=s.corner.y;
			if(!eqpt(mouse.xy, Pt(x, my)))
				cursset(Pt(x, my));
			if(but==1){
				p0=l->origin-charofpt(&l->f, Pt(s.corner.x, my));
				y=scrpos(l->scroll, p0, p0+l->f.nchars, tot).origin.y;
			}else if(but==2){
				y=my;
				if(y>s.corner.y-2)
					y=s.corner.y-2;
			}else if(but==3){
				p0=l->origin+charofpt(&l->f, Pt(s.corner.x, my));
				y=scrpos(l->scroll, p0, p0+l->f.nchars, tot).origin.y;
			}
			if(y!=oy){
				scrflip(l, r);
				r=raddp(scr, Pt(0, y-scr.origin.y));
				scrflip(l, r);
			}
		}
		nap(2);
	}
	if(in){
		Text *t=(Text *)l->user1;
		h=s.corner.y-s.origin.y;
		scrflip(l, r);
		if(but==1)
			outTsll(Torigin, t->tag, l->origin, (long)(my-s.origin.y)/l->f.font->height+1);
		else if(but==2){
			if(tot>1024L*1024L)
				p0=muldiv(tot>>10, y-s.origin.y, h)<<10;
			else
				p0=muldiv(tot, y-s.origin.y, h);
			outTsll(Torigin, t->tag, p0, 1L);
		}else if(but==3){
			p0=l->origin+charofpt(&l->f, Pt(s.corner.x, my));
			if(p0>tot)
				p0=tot;
			horigin(t->tag, p0);
		}
	}
}
scrflip(l, r)
	Flayer *l;
	Rectangle r;
{
	if(rectclip(&r, l->scroll))
		rectf(l->f.b, r, F_XOR);
}
