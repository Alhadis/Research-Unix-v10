#include "univ.h"

#define scale( x, inmin, inmax, outmin, outmax )\
	( outmin + muldiv(x-inmin,outmax-outmin,inmax-inmin) )

#define bound( x, low, high ) ( min( high, max( low, x ) ) )

#define SPACING		(defont.height)		/* was 14 */
#define DISPLAY		13
#define CHARWIDTH	9

#define DELTA		6
#define BARWIDTH	18

int MenuNest;
Bitmap physical = { (Word *) 0x700000, 25, 0, 0, XMAX, YMAX };

Entry *EntryGen(i, s)
register Script *s;
{
	register Entry *e;

	for( ; s; s = s->more ){
		CIndex = s->cindex;
		if( i < s->items )
			return (s->generator)(i);
		i -= s->items;
	}
	return 0;
}

Limits(s)
register Script *s;
{
	register i, l;
	register Entry *e;

	s->items = s->width = 0;
	for( i = 0; e = (s->generator)(i); ++i ){
		++s->items;
		l = strlen(e->text);
		if( l > s->width ) s->width = l;
	}
}

Entry *ScriptHit(sh, but, rl)
register Script *sh;
RectList *rl;
{
	register int width, i, j, top, newtop, hit, newhit, items, lines, charswide;
	Point p, q, savep, baro, barc;
	Rectangle sr, tr, mr;	/* scroll, text, menu */
	register Bitmap *b;
	register char *s, *from, *to, fill[64];
	register Script *m;
	register Entry *e;
	RectList lrl, *l;

#define sro sr.origin
#define src sr.corner
#define tro tr.origin
#define trc tr.corner
#define mro mr.origin
#define mrc mr.corner

	charswide = items = 0;
	for( m = sh; m; m = m->more ){
		CIndex = m->cindex;
		m->items = m->width = 0;
		if( m->limits )
			m->limits(m);
		else
			Limits(m);
		items += m->items;
		if( m->width > charswide ) charswide = m->width;
	}
	p = mouse.xy;
	if( items == 0 ) return 0;
	width = charswide*CHARWIDTH+10;
	sro.x = sro.y = src.x = tro.x = mro.x = mro.y = 0;
	if( items <= DISPLAY ) lines = items;
	else {
		lines = DISPLAY;
		tro.x = src.x = BARWIDTH;
		sro.x = sro.y = 1;
	}
#define ASCEND 2
	tro.y = ASCEND;
	mrc = trc = add(tro, Pt(width,min(items,lines)*SPACING) );
	src.y = mrc.y-1;
	newtop = bound(sh->prevtop, 0, items-lines );
	p.y -= bound(sh->prevhit, 0, lines-1)*SPACING+SPACING/2;
	p.x = bound(p.x-(src.x+width/2), 0, XMAX-mrc.x);
	p.y = bound(p.y, 0, YMAX-mrc.y);
	sr = raddp(sr,p);
	tr = raddp(tr, p);
	mr = raddp(mr, p);
	b = balloc(mr);
	if( b ) bitblt(&physical, mr, b, mro, F_STORE);
	rectf( &physical, mr, F_STORE );
PaintMenu:
	rectf( &physical, inset(mr,1), F_CLR );
	top = newtop;
	if( items > DISPLAY ){
		baro.y = scale(top, 0, items, sro.y, src.y);
		baro.x = sr.origin.x;
		barc.y = scale(top+DISPLAY, 0, items, sro.y, src.y);
		barc.x = sr.corner.x;
		rectf(&physical, Rpt(baro,barc), F_XOR);
	}
	for( p = tro, i = top; i < min(top+lines,items); ++i ){
		q = p;
		e = EntryGen(i, sh);
		from = e->text;
		for( to = &fill[0]; *from; ++from ){
			if( *from & 0x80 )
				for(j=charswide-(strlen(from+1)+(to-&fill[0])); j-->0;)
					*to++ = *from & 0x7F;
			else
				*to++ = *from;
		}
		if( e->script ){
			static Bitmap *bb;
			Point qq;
			qq.y = q.y+1;
			qq.x = q.x+width-15;
			if( !bb ){
				bb = balloc(Rect(0,0,15,15));
				assert(bb);
				texture(bb, bb->rect, &Arrow, F_STORE);
			}
			bitblt(bb, bb->rect, &physical, qq, F_XOR);
		}
		*to = '\0';
		q.x += ( width-jstrwidth(fill) )/2;
		string( &defont, fill, &physical, q, F_XOR );
		p.y += SPACING;
	}
	savep = mouse.xy;
	newhit = hit = -1;
	e = 0;
	for( ; button(but); nap(2) ){
		p = mouse.xy;
		if( ptinrect(p, sr) ){
			if( ptinrect(savep,tr) ){
				p.y = (baro.y+barc.y)/2;
				cursset( p );
			}
			newtop = scale( p.y, sro.y, src.y, 0, items );
			newtop = bound( newtop-DISPLAY/2, 0, items-DISPLAY );
			if( newtop != top )
				goto PaintMenu;
		} else if( ptinrect(savep,sr) ){
			register dx, dy;
			if( abs(dx = p.x-savep.x) < DELTA ) dx = 0;
			if( abs(dy = p.y-savep.y) < DELTA ) dy = 0;
			if( abs(dy) >= abs(dx) ) dx = 0; else dy = 0;
			cursset( p = add(savep, Pt(dx,dy)) );
		}
		savep = p;
		newhit = -1;
		if( ptinrect(p, tr) ){
			newhit = bound((p.y-tro.y)/SPACING, 0, lines-1);
			if( hit >= 0
			 && abs(tro.y+SPACING*newhit+SPACING/2-p.y) > SPACING/3 )
				newhit = hit;
		}
		if( newhit != hit ){
			flip(tr, hit);
			flip(tr, hit = newhit);
			e = hit>=0 ? EntryGen(hit+top, sh) : 0;
		}
		if( !ptinrect(p, mr) ){
			assert(!e);
			for( l = rl; l; l = l->more )
				if( ptinrect(p, *l->rp) )
					goto Done;
		}
		if(newhit==0 && top>0){
			newtop = top-1;
			p.y += SPACING;
			cursset(p);
			goto PaintMenu;
		}
		if(newhit==DISPLAY-1 && top<items-lines){
			newtop = top+1;
			p.y -= SPACING;
			cursset(p);
			goto PaintMenu;
		}
 		if( e && e->script
		 && p.x > trc.x-CHARWIDTH ){
			p.x = trc.x;
			cursset(p);
			nap(1);
			lrl.rp = &mr;
			lrl.more = rl;
			++MenuNest;
			e = ScriptHit(e->script, but, &lrl);
			--MenuNest;
			if( e ) goto Done;
			goto PaintMenu;
		}
	}
Done:
	if( b ){
		screenswap( b, b->rect, b->rect );
		bfree(b);
	}
	sh->prevhit = hit;
	sh->prevtop = top;
	return e;
}

static flip(r,n)
Rectangle r;
{
	if( n<0 ) return;
	++r.origin.x;
	r.corner.y = (r.origin.y += SPACING*n-1) + SPACING;
	--r.corner.x;
	rectf( &physical, r, F_XOR );
}
