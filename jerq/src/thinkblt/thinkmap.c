#include <jerq.h>
#include "think.h"

static Word thinkdata[THWORDS];

static Bitmap thinkrast = { thinkdata, THWORDS, 0, 0, THBITS, 1 };

static char beggraph[] = "\033*rB\033*r640S\033*rA";
static char begraster[] = "\033*b80W";
static char endgraph[] = "\033*rB";

thinkmap(bp, r)
register Bitmap *bp; Rectangle r;
{
	register mrbytes, nrbytes; Rectangle rrast;

	mrbytes = (r.corner.x - r.origin.x + BYTESIZE - 1)/BYTESIZE;
	if (mrbytes <= 0)
		return;
	if (mrbytes > THBYTES)
		mrbytes = THBYTES;

	/* initialize printer for graphics mode  */
	thinknchars((sizeof beggraph)-1, beggraph);

	rectf(&thinkrast, thinkrast.rect, F_CLR);
	rectf(bp, r, F_XOR);
	for(rrast = r; rrast.origin.y < r.corner.y ; rrast.origin.y++) {
		if ((*thquitf)()) {
			rrast.corner.y = r.corner.y;
			rectf(bp, rrast, F_XOR);
			return thinkabort();
		}
		rrast.corner.y = rrast.origin.y+1;
		rectf(bp, rrast, F_XOR);
		bitblt(bp, rrast, &thinkrast, Pt(0,0), F_STORE);
		for (nrbytes=mrbytes; nrbytes > 1; nrbytes--)
			if (((char *)thinkdata)[nrbytes-1])
				break;
		/* write out the raster width */
		begraster[3] = nrbytes/10 + '0';
		begraster[4] = nrbytes%10 + '0';
		thinknchars((sizeof begraster)-1, begraster);
		/* put out the scanline */
		thinknchars(nrbytes, thinkdata);
	}
	/* flush the printer buffer */
	thinknchars((sizeof endgraph)-1, endgraph);
	return thinkflush();
}
