#ifdef DMD630
#include <dmd.h>
#include <5620.h>
#include <research.h>
#else /*DMD630*/
#include <jerq.h>
#endif /*DMD630*/
#include <font.h>
#include "menu.h"

static flip();
static helpon();
static helpoff();

#ifdef DMD630
#define disp physical
#else /*DMD630*/
static Bitmap disp = { (Word *)0x700000, 25, 0, 0, XMAX, YMAX };
#endif /*DMD630*/

#define scale(x, inmin, inmax, outmin, outmax)\
	(outmin + muldiv(x-inmin,outmax-outmin,inmax-inmin))

#define bound(x, low, high) min(high, max( low, x ))

#define SPACING		(monfont->height+1)
#define DISPLAY		16
#define CHARWIDTH	(monfont->info['n'].width)
#define DELTA		6
#define BARWIDTH	18
#define	MAXDEPTH	3	/* don't use too much stack */
#define ARROWIDTH	20

static Texture tarrow =
{
	0x0000, 0x0000, 0x0080, 0x00C0, 0x00C0, 0x00E0, 0x00F8, 0xFFFE,
	0xFFFE, 0x00F8, 0x00E0, 0x00C0, 0x00C0, 0x0080, 0x0000, 0x0000,
};

static NMitem *
tablegen(i, table)
	NMitem *table;
{
	return &table[i];
}

static char fill[64];
extern Font *monfont;

NMitem *
mhit(m, but, depth)
register NMenu *m;
{
	register int width, mwidth, i, j, top, newtop, hit, newhit;
	register int items, lines, length, w, x;
	Point p, q, savep, baro, barc;
	Rectangle sr, tr, mr;	/* scroll, text, menu */
	Rectangle rside, rhit;
	register Bitmap *b;
	register char *from, *to;
	Bitmap *bhelp = 0, *arrow;
	NMitem *(*generator)(), *mi, *table, *ret = 0;
	int dohfn;

#define sro sr.origin
#define src sr.corner
#define tro tr.origin
#define trc tr.corner
#define mro mr.origin
#define mrc mr.corner
	generator = (table=m->item) ? tablegen : m->generator;
	arrow = balloc(Rect(0, 0, 16, 16));
	texture(arrow, arrow->rect, &tarrow, F_STORE);
	p = mouse.xy;
	w = x = length = 0;
	for(items = 0; (mi=(*generator)(items, table))->text; ++items) {
		register int s = strlen ((unsigned char *)mi->text);
		length = max(length, s);
		if (mi->next) {
			w = max (w, s);
		} else
			x = max (x, s);
	}
	if(items == 0)
		return(ret);
	width = length*CHARWIDTH+10;
	w *= CHARWIDTH;
	x *= CHARWIDTH;
	if (x <= w)
		mwidth = w + ARROWIDTH;
	else if (x >= w + 2*ARROWIDTH)
		mwidth = x;
	else
		mwidth = w + ARROWIDTH + (x - w) / 2;
	mwidth += 10;
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
	mrc = trc = add(tro, Pt(mwidth, min(items, lines)*SPACING));
	src.y = mrc.y-1;
	newtop = bound(m->prevtop, 0, items-lines);
	p.y -= bound(m->prevhit, 0, lines-1)*SPACING+SPACING/2;
	p.x = bound(p.x-(src.x+mwidth/2), 0, XMAX-mrc.x);
	p.y = bound(p.y, 0, YMAX-mrc.y);
	sr = raddp(sr, p);
	tr = raddp(tr, p);
	mr = raddp(mr, p);
	rside = mr;
	rside.origin.x = rside.corner.x-20;
	b = balloc(mr);
	cursinhibit();
	if(b)
		bitblt(&disp, mr, b, mro, F_STORE);
	rectf(&disp, mr, F_OR);
	cursallow();
PaintMenu:
	helpoff(&bhelp);
	cursinhibit();
	rectf(&disp, inset(mr, 1), F_CLR);
	cursallow();
	top = newtop;
	if(items > DISPLAY){
		baro.y = scale(top, 0, items, sro.y, src.y);
		baro.x = sr.origin.x;
		barc.y = scale(top+DISPLAY, 0, items, sro.y, src.y);
		barc.x = sr.corner.x;
		rectf(&disp, Rpt(baro,barc), F_XOR);
	}
	for(p=tro, i=top; i < min(top+lines, items); ++i){
		int justify = 0;
		q = p;
		mi = (*generator)(i, table);
		from = mi->text;
		for(to = &fill[0]; *from; ++from)
			if(*from & 0x80){
				for(j=length-(strlen((unsigned char *)from+1)+(to-&fill[0])); j-->0;)
					*to++ = *from & 0x7F;
				justify++;
			}else
				*to++ = *from;
		*to = '\0';
		if (justify)
			q.x += 5;
		else
			q.x += (width-strwidth(monfont, fill))/2;
		string(monfont, fill, &disp, q, F_XOR);
		if(mi->next)
			bitblt(arrow, arrow->rect, &disp, Pt(trc.x-18, p.y-9+SPACING/2), F_OR);
		p.y += SPACING;
	}
	savep = mouse.xy;
	mi = 0;
	for(newhit = hit = -1; bttn(but); nap(2)){
		p = mouse.xy;
		if(depth && ((p.x < mro.x && p.y >= mro.y && p.y <= mrc.y)
		    || bttn(5-but))){
			ret = 0;
			break;
		}
		if(ptinrect(p, sr)){
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
			rhit = tr;
			rhit.origin.y += newhit*SPACING-ASCEND/2;
			rhit.corner.y = rhit.origin.y + SPACING;
		}
		if(newhit == -1)
			ret = 0, dohfn = 0;
		else
			ret = mi = (*generator)(top+newhit, table), dohfn = 1;
		if(newhit == hit)
		{
			if((newhit != -1) && (bhelp == 0) && bttn1() && mi->help)
				helpon(mi->help, rhit, &bhelp);
			else if(bhelp && !bttn1())
				helpoff(&bhelp);
		}
		else
		{
			helpoff(&bhelp);
			flip(tr, hit);
			flip(tr, newhit);
			hit = newhit;
			if((newhit != -1) && bttn1() && mi->help)
				helpon(mi->help, rhit, &bhelp);
		}
		if((newhit != -1) && ptinrect(p, rside))
		{
			Point px;

			if(mi->dfn) (*mi->dfn)(mi);
			if(mi->next && (depth <= MAXDEPTH))
				ret = mhit(mi->next, but, depth+1), dohfn = 0;
			if(mi->bfn) (*mi->bfn)(mi);
			px = mouse.xy;
			/* going off the left edge of the menu with the button
			   still down will cause you to jump to the place you
			   left */
			if (ret==0 && bttn(but) && ptinrect(px, tr)==0
			    && px.y>=mro.y && px.y <= mrc.y) {
				p.x = rside.origin.x - 5;
				cursset(p);
/* ->->-> */			goto PaintMenu;
			}
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
#ifdef VOYDEMO
	if (bttn1())
		while(bttn1())
			sleep(30);
	while(!bttn1())
		sleep(30);
	while(bttn1())
		sleep(30);
#endif /*VOYDEMO*/
	if(bhelp)
		helpoff(&bhelp);
	if(b){
		cursinhibit();
		screenswap(b, b->rect, b->rect);
		cursallow();
		bfree(b);
	}
	if(hit>=0){
		m->prevhit = hit;
		m->prevtop = top;
		if(ret && ret->hfn && dohfn) (*ret->hfn)(mi);
	}
	return(ret);
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
	rectf(&disp, r, F_XOR);
}

static
helpon(msg, r, bhelp)
	char *msg;
	Rectangle r;
	Bitmap **bhelp;
{
	register Bitmap *b;
	register w;

	w = strwidth(monfont, msg)+10;
	if(r.corner.x+w < XMAX)
	{
		r.origin.x = r.corner.x;
		r.corner.x += w;
	}
	else if(r.origin.x-w>0)
	{
		r.corner.x = r.origin.x;
		r.origin.x -= w;
	}
	else
	{
		r.origin.x = 3;
		r.corner.x = w + 3;
	}
	if(*bhelp = b = balloc(r = inset(r, -1)))
	{
		rectf(b, r, F_OR);
		rectf(b, inset(r, 1), F_XOR);
		string(monfont, msg, b, add(b->rect.origin, Pt(5, 1)), F_XOR);
		screenswap(b, b->rect, b->rect);
	}
}

static
helpoff(bhelp)
	Bitmap **bhelp;
{
	Bitmap *bh;

	if(bh = *bhelp)
	{
		screenswap(bh, bh->rect, bh->rect);
		bfree(bh);
		*bhelp = 0;
	}
}
