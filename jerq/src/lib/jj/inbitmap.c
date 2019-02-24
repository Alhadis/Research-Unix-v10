#include <jerq.h>

#define SHORTSIZE	16

#define inpoint(pp)	(inword(&(pp)->x) < 0 ? -1 : inword(&(pp)->y))
#define inrect(rp)	(inpoint(&(rp)->origin) < 0 ? -1 : inpoint(&(rp)->corner))

static Word buffer[XMAX/WORDSIZE];

static Bitmap bbuffer = { buffer, XMAX/WORDSIZE, 0, 0, XMAX, 1, };

static int (*inch)();

Bitmap *
inbitmap(incharg)
int (*incharg)();
{
	register Bitmap *bp;
	register y;
	int rastwid, dummy;
	Rectangle r;

	inch = incharg;

	if (inword(&dummy) < 0 || inrect(&r) < 0 || (bp = balloc(r)) == 0)
		return 0;
	rastwid = (r.corner.x - r.origin.x + SHORTSIZE - 1)/SHORTSIZE;
	for (y = r.origin.y; y < r.corner.y; y++) {
		if (getrast((short *)buffer, rastwid) < 0) {
			bfree(bp);
			return 0;
		}
		bitblt(&bbuffer, bbuffer.rect, bp, Pt(r.origin.x, y), F_STORE);
		if (y > r.origin.y)
			bitblt(bp, Rect(r.origin.x, y-1, r.corner.x, y),
			       bp, Pt(r.origin.x, y), F_XOR);
	}
	return bp;
}

static
getrast(p, nwords)	/* receive single compressed raster */
register short *p; int nwords;
{
	int count, same;

	while (nwords > 0) {
		if ((count = (*inch)()) <= 0)
			return -1;
		same = count&128; count &= 127;
		nwords -= count;

		if (same) {
			if (innchars(sizeof(short), (char *)p) < 0)
				return -1;
			for (count--; count>0; count--) {
				*(p+1) = *p; p++;
			}
			p++;
		} else {
			if (innchars(sizeof(short)*count, (char *)p) < 0)
				return -1;
			p += count;
		}
	}
	return (nwords == 0 ? 0 : -1);
}

static
inword(wp)
short *wp;
{
	register lo, hi;
	if ((lo = (*inch)()) < 0 || (hi = (*inch)()) < 0)
		return -1;
	*wp = (hi << 8) | lo;
	return 0;
}

static
innchars(n, p)
register n; register char *p;
{
	register c;
	while (--n >= 0)
		if ((c = (*inch)()) < 0)
			return -1;
		else
			*p++ = c;
	return 0;
}
