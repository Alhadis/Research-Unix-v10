#include <jerq.h>
/*
 * Like bitblt, only magnifies.  Last arg is a point (xscale, yscale).
 * F_STORE assignment only.  Works properly if overlaps.
 */
magnify(b, r, tb, p, fac)
	register Bitmap *b, *tb;
	Rectangle r;
	Point p, fac;
{
	register i, shift;
	Point d;
	Rectangle s;
	if(fac.x<1 || fac.y<1)
		return;
	d=sub(r.corner, r.origin);
	s.origin=p;
	s.corner=add(p, Pt(fac.x*d.x, fac.y*d.y));
	/* Copy source into origin of dest */
	bitblt(b, r, tb, p, F_STORE);
	/* Clear rest of dest */
	rectf(tb, Rect(s.origin.x+d.x, s.origin.y, s.corner.x, s.corner.y), F_CLR);
	rectf(tb, Rect(s.origin.x, s.origin.y+d.y, s.origin.x+d.x, s.corner.y), F_CLR);
	/* Now we expand in place */
	/* 1: expand horizontally */
	if(fac.x > 1) for(i=d.x-1; i>0; i--){
		bitblt(tb, Rect(p.x+i, p.y, p.x+i+1, p.y+d.y),
			   tb, Pt(p.x+i*fac.x, p.y), F_OR);
		rectf(tb, Rect(p.x+i, p.y, p.x+i+1, p.y+d.y), F_CLR);
	}
	/* 2: smear horizontally */
	for(i=1; i<fac.x; i<<=1){
		shift=min(i, fac.x-i);
		bitblt(tb, Rect(p.x, p.y, s.corner.x-shift, p.y+d.y),
		       tb, Pt(p.x+shift, p.y), F_OR);
	}
	/* 3: expand vertically */	
	if(fac.y > 1) for(i=d.y-1; i>0; i--){
		bitblt(tb, Rect(p.x, p.y+i, s.corner.x, p.y+i+1),
		       tb, Pt(p.x, p.y+i*fac.y), F_OR);
		rectf(tb, Rect(p.x, p.y+i, s.corner.x, p.y+i+1), F_CLR);
	}
	/* 4: smear vertically */
	for(i=1; i<fac.y; i<<=1){
		shift=min(i, fac.y-i);
		bitblt(tb, Rect(p.x, p.y, s.corner.x, s.corner.y-shift),
		       tb, Pt(p.x, p.y+shift), F_OR);
	}	
}
