#include <jerq.h>
#include <font.h>
#include "MENUHIT.h"

#undef	cursinhibit
#undef	cursallow
#define cursinhibit()
#define cursallow()

struct udata {
	Rectangle Drect;
	Layer	 * JDisplayp;
	int	argc;
	char	**argv;
	struct Mouse mouse;
};

#define Pmouse		(((struct udata *)P->data)->mouse)
#define Pbutton(i)	(Pmouse.buttons&(8>>i))

Bitmap *lalloc(), *Displayp;

#define scale(x, inmin, inmax, outmin, outmax)\
	(outmin + muldiv(x-inmin,outmax-outmin,inmax-inmin))

#define bound(x, low, high) min(high, max( low, x ))

#define SPACING		(defont.height)
#define DISPLAY		16
#define CHARWIDTH	(defont.info['n'].width)
#define DELTA		6
#define BARWIDTH	18

static char **table;
static char *
tablegen(i)
{
	return table[i];
}

static Rectangle sr, tr, mr;	/* scroll, text, menu */
static items, iselect;

demohit(m, but)
register Menu *m;
{
	register int width, i, j, top, newtop, hit, newhit, lines, length;
	Point p, q, savep, baro, barc;
	register char *s, *(*generator)(), *from, *to, fill[64];

#define sro sr.origin
#define src sr.corner
#define tro tr.origin
#define trc tr.corner
#define mro mr.origin
#define mrc mr.corner

	menulift();
	generator = (table=m->item) ? tablegen : m->generator;
	p = Pmouse.xy;
	for(length = items = 0; s=(*generator)(items); ++items)
		length = max(length, strlen(s));
	if(items == 0){
		while(Pbutton(but));
		return -1;
	}
	width = length*CHARWIDTH+10;
	sro.x = sro.y = src.x = tro.x = mro.x = mro.y = 0;
	if(items <= DISPLAY)
		lines = items;
	else{
		lines = DISPLAY;
		tro.x = src.x = BARWIDTH;
		sro.x = sro.y = 1;
	}
#define ASCEND 2
	tro.y = ASCEND;
	mrc = trc = add(tro, Pt(width, min(items, lines)*SPACING));
	src.y = mrc.y-1;
	newtop = bound(m->prevtop, 0, items-lines);
	p.y -= bound(m->prevhit, 0, lines-1)*SPACING+SPACING/2;
	p.x = bound(p.x-(src.x+width/2), 0, XMAX-mrc.x);
	p.y = bound(p.y, 0, YMAX-mrc.y);
	sr = raddp(sr, p);
	tr = raddp(tr, p);
	mr = raddp(mr, p);
	Displayp = lalloc(mr);
PaintMenu:
	cursinhibit();
	rectf(Displayp, inset(mr, 1), F_CLR);
	cursallow();
	top = newtop;
	if(items > DISPLAY){
		baro.y = scale(top, 0, items, sro.y, src.y);
		baro.x = sr.origin.x;
		barc.y = scale(top+DISPLAY, 0, items, sro.y, src.y);
		barc.x = sr.corner.x;
		rectf(Displayp, Rpt(baro,barc), F_XOR);
	}
	for(p=tro, i=top; i < min(top+lines, items); ++i){
		q = p;
		from = generator(i);
		for(to = &fill[0]; *from; ++from)
			if(*from & 0x80)
				for(j=length-(strlen(from+1)+(to-&fill[0])); j-->0;)
					*to++ = *from & 0x7F;
			else
				*to++ = *from;
		*to = '\0';
		q.x += (width-jstrwidth(fill))/2;
		string(&defont, fill, Displayp, q, F_XOR);
		p.y += SPACING;
	}
	savep = Pmouse.xy;
	for(newhit = hit = -1; Pbutton(but); nap(2)){
		if(ptinrect(p = Pmouse.xy, sr)){
			if(ptinrect(savep,tr)){
				p.y = (baro.y+barc.y)/2;
				cursset(p);
			}
			newtop = scale(p.y, sro.y, src.y, 0, items);
			newtop = bound(newtop-DISPLAY/2, 0, items-DISPLAY);
			if(newtop != top)
/* ->->-> */			goto PaintMenu;
		}else if(ptinrect(savep,sr)){
			register dx, dy;
			if(abs(dx = p.x-savep.x) < DELTA)
				dx = 0;
			if(abs(dy = p.y-savep.y) < DELTA)
				dy = 0;
			if(abs(dy) >= abs(dx))
				dx = 0;
			else
				dy = 0;
			cursset(p = add(savep, Pt(dx,dy)));
		}
		savep = p;
		newhit = -1;
		if(ptinrect(p, tr)){
			newhit = bound((p.y-tro.y)/SPACING, 0, lines-1);
			if(newhit!=hit && hit>=0
			 && abs(tro.y+SPACING*newhit+SPACING/2-p.y) > SPACING/3)
				newhit = hit;
		}
		if(newhit != hit){
			flip(tr, hit);
			flip(tr, hit = newhit);
		}
		if(newhit==0 && top>0){
			newtop = top-1;
			p.y += SPACING;
			cursset(p);
/* ->->-> */		goto PaintMenu;
		}
		if(newhit==DISPLAY-1 && top<items-lines){
			newtop = top+1;
			p.y -= SPACING;
			cursset(p);
/* ->->-> */		goto PaintMenu;
		}
	}
	if(hit>=0){
		m->prevhit = hit;
		m->prevtop = top;
		return (iselect = hit+top);
	}else
		return -1;
}

static
flip(r,n)
	Rectangle r;
{
	if(n<0)
		return;
	++r.origin.x;
	r.corner.y = (r.origin.y += SPACING*n-1) + SPACING;
	--r.corner.x;
	rectf(Displayp, r, F_XOR);
}

int (*muxhit)();

menudemo(flag)
{
	if (muxhit == 0)
		muxhit = Sys[MENUHIT];
	Sys[MENUHIT] = flag ? demohit : muxhit;
}

menulift()
{
	docursor(-1);
	if (Displayp) {
		lfree(Displayp);
		Displayp = 0;
	}
	iselect = -1;
}

menuselect(i)
{
	flip(tr, iselect);
	if (i >= 0 && i < min(DISPLAY, items))
		flip(tr, iselect = i);
	else
		iselect = -1;
}
