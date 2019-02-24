#include <jerq.h>
#include <font.h>

#define WIDTH	9
#define HEIGHT	14
#define HMARG	3
#define WMARG	4
#define HTOT	(HEIGHT+2*HMARG)
#define CMARG	2

ident(bp, dr, s)
register Bitmap *bp; Rectangle dr; register char *s;
{
	register Bitmap *sb; int xw, xb, yw;
	Point d, dn;
	Rectangle r;

	d = sub(dr.corner, dr.origin);
	dn.x = ((d.x - WMARG)/WIDTH)*WIDTH;
	dn.y = ( d.y         /HTOT )*HTOT;
	r.origin = add(dr.origin, div(sub(d, dn), 2));
	r.corner = add(r.origin, dn);

	xw = (strlen(s) + CMARG)*WIDTH;
	sb = balloc(Rect(0, 0, xw, HTOT));
	string(&defont, s, sb, Pt(0, HMARG), F_STORE);
	cbitblt(sb, sb->rect, bp, r, F_STORE);
	for (xb = xw; (d.x = r.origin.x + xb) < r.corner.x; xb *= 2)
		cbitblt(bp, Rpt(r.origin, Pt(d.x, r.origin.y+HTOT)),
		    bp, Rpt(Pt(d.x, r.origin.y), r.corner), F_STORE);

	yw = r.origin.y;
	xb = dn.x%xw;
	while ((yw += HTOT) < r.corner.y) {
		cbitblt(bp, Rect(r.origin.x+WIDTH, yw-HTOT, r.corner.x, yw),
		    bp, Rpt(Pt(r.origin.x, yw), r.corner), F_STORE);
		cbitblt(sb, Rect(xb, 0, xw, HTOT),
		    bp, Rpt(Pt(r.corner.x-WIDTH, yw), r.corner), F_STORE);
		if ((xb += WIDTH) >= xw)
			xb = 0;
	}
	bfree(sb);
}

cbitblt(sb, sr, db, dr, f)
Bitmap *sb, *db; Rectangle sr, dr; int f;
{
	if (rectclip(&sr, raddp(dr, sub(sr.origin, dr.origin))))
		bitblt(sb, sr, db, dr.origin, f);
}
