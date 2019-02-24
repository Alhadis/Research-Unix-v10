#include <jerq.h>
#include <layer.h>
#include <queue.h>
#include <jerqproc.h>
#include "frame.h"

#define	INFINITY	32767

deltext(t, s1, s2)
	register Frame *t;
{
	register char *p, *q, *r;
	register n, y1, y2;
	Point pt;
	frameop(t, opclear, ptofchar(t, s1), t->str.s+s1, s2-s1);
	p=t->str.s+s2;
	r=t->str.s+t->str.n;
	for(n=0,q=p; q<r && *q!='\n'; q++)
		n++;
	/* Found the '\n' at the end of this line; clear out */
	frameop(t, opclear, ptofchar(t, s2), p, n);
	y1=endpoint.y;	/* current y position of '\n' at p[n] */
	/* Draw rest of line at new place */
	pt=ptofchar(t, s1);
	draw(t, pt, p, n);
	y2=endpoint.y;
	if(y1 != y2){	/* More housekeeping */
		/* NOTE: y1 > y2 */
		y1+=newlnsz;
		y2+=newlnsz;
		/* Scroll up */
		Ubitblt(D, Rect(t->rect.origin.x, y1,
			t->rect.corner.x, t->rect.corner.y),
			D, Pt(t->rect.origin.x, y2), F_STORE);
		/* Clear the rest */
		clear(Rpt(Pt(t->rect.origin.x, t->rect.corner.y-(y1-y2)),
			t->rect.corner), 1);
		y1-=newlnsz;
		y2-=newlnsz;
	}
	y1=lineno(t, y1);
	y2=lineno(t, y2);
	delstring(&t->str, s1, s2);
	if(y1 == y2)
		setcpl(t, lineno(t, pt.y), y1);
	else
		setcpl(t, 0, t->nlines-1);	/* SLOW, correct */
	if(y1 != y2){
		n=charofpt(t, Pt(0, (t->nlines-(y1-y2))
			*newlnsz+t->rect.origin.y));	/* Re-use of n */
		/* n is last visible char */
		draw(t, ptofchar(t, n), t->str.s+n, t->str.n-n);
		if(complete)
			return TRUE;
	}
	return FALSE;
}
