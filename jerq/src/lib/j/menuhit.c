#include <jerq.h>
#include <layer.h>
#include <jerqproc.h>
#include <font.h>

#define scale(x, inmin, inmax, outmin, outmax)\
	(outmin + muldiv(x-inmin,outmax-outmin,inmax-inmin))

#define bound(x, low, high) min(high, max( low, x ))

#define DISPLAY		16
#define DELTA		6
#define BARWIDTH	18

static char **table;
static char *
tablegen(i)
{
	return table[i];
}

menuhit(m, but)
register Menu *m;
{
	register int width, i, j, top, newtop, hit, newhit, items, lines, length;
	Point p, q, savep, baro, barc;
	Rectangle sr, tr, mr;	/* scroll, text, menu */
	register Bitmap *b;
	register char *s, *(*generator)(), *from, *to, fill[64];
#ifdef	MUXTERM	#kludge to make defont substitutable in mux
	Font *font = (!P||!P->defaultfont)? &defont:
		P->defaultfont;
#else
	Font *font = &defont;
#endif	MUX
	int spacing = font->height;
	int charwidth = font->info['n'].width;

#define sro sr.origin
#define src sr.corner
#define tro tr.origin
#define trc tr.corner
#define mro mr.origin
#define mrc mr.corner

	generator = (table=m->item) ? tablegen : m->generator;
	p = mouse.xy;
	length = width = items = 0;
	for( ; s=(*generator)(items); ++items) {
		length = max(length, strlen(s));
		width = max(width, strwidth(font,s));
	}
	if(items == 0){
		while(button(but));
		return -1;
	}
	width += 10;
	sro.x = sro.y = src.x = tro.x = mro.x = mro.y = 0;
	if(items <= DISPLAY)
		lines = items;
	else{
		lines = DISPLAY;
		tro.x = src.x = BARWIDTH;
		sro.x = sro.y = 1;
	}
	tro.y = 1;
	mrc = trc = add(Pt(tro.x, mro.y), Pt(width, min(items, lines)*spacing+2));
	trc.y = src.y = mrc.y-1;
	newtop = bound(m->prevtop, 0, items-lines);
	p.y -= bound(m->prevhit, 0, lines-1)*spacing+spacing/2;
	p.x = bound(p.x-(src.x+width/2), 0, XMAX-mrc.x);
	p.y = bound(p.y, 0, YMAX-mrc.y);
	sr = raddp(sr, p);
	tr = raddp(tr, p);
	mr = raddp(mr, p);
	b = balloc(mr);
	cursinhibit();
	if(b)
		bitblt(&display, mr, b, mro, F_STORE);
	rectf(&display, mr, F_OR);
	cursallow();
PaintMenu:
	cursinhibit();
	rectf(&display, inset(mr, 1), F_CLR);
	cursallow();
	top = newtop;
	if(items > DISPLAY){
		baro.y = scale(top, 0, items, sro.y, src.y);
		baro.x = sr.origin.x;
		barc.y = scale(top+DISPLAY, 0, items, sro.y, src.y);
		barc.x = sr.corner.x;
		rectf(&display, Rpt(baro,barc), F_XOR);
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
		q.x += (width-strwidth(font,fill))/2;
		string(font, fill, &display, q, F_XOR);
		p.y += spacing;
	}
	savep = mouse.xy;
	for(newhit = hit = -1; button(but); nap(2)){
		if(ptinrect(p = mouse.xy, sr)){
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
			newhit = bound((p.y-tro.y)/spacing, 0, lines-1);
			if(newhit!=hit && hit>=0
			 && abs(tro.y+spacing*newhit+spacing/2-p.y) > spacing/3)
				newhit = hit;
		}
		if(newhit != hit){
			flip(tr, hit, spacing);
			flip(tr, hit = newhit, spacing);
		}
		if(newhit==0 && top>0){
			newtop = top-1;
			p.y += spacing;
			cursset(p);
/* ->->-> */		goto PaintMenu;
		}
		if(newhit==DISPLAY-1 && top<items-lines){
			newtop = top+1;
			p.y -= spacing;
			cursset(p);
/* ->->-> */		goto PaintMenu;
		}
	}
	if(b){
		cursinhibit();
		screenswap(b, b->rect, b->rect);
		cursallow();
		bfree(b);
	}
	if(hit>=0){
		m->prevhit = hit;
		m->prevtop = top;
		return hit+top;
	}else
		return -1;
}

static
flip(r,n,spacing)
	Rectangle r;
{
	if(n<0)
		return;
	++r.origin.x;
	r.corner.y = (r.origin.y += spacing*n) + spacing;
	--r.corner.x;
	rectf(&display, r, F_XOR);
}
