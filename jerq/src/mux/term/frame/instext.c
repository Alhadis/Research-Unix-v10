#include <jerq.h>
#include <layer.h>
#include <queue.h>
#include <jerqproc.h>
#include "frame.h"
int inscomplete;
#define	MAXEXTRA	1024	/* max # chars off end of screen */
instext(t, s, sn)
	register Frame *t;
	register String *s;
	register sn;	/* char number at which to insert */
{
	register char *p, *q, *r;
	register n, y1, y2;
	Point sp;	/* point on screen */
	sp=ptofchar(t, sn);
	p=t->str.s+sn;
	r=t->str.s+t->str.n;
	for(n=0,q=p; q<r && *q!='\n'; q++)
		n++;
	/* n is #chars to next '\n' */
	frameop(t, opnull, sp, s->s, s->n);
	frameop(t, opnull, endpoint, p, n);
	y2=endpoint.y;		/* where p[n] will end up */
	draw(t, sp, p, n);	/* undraws, leaves endpoint */
	y1=endpoint.y;		/* where p[n] is now */
	insstring(&t->str, sn, s);	/* build new string */
	if(y1 != y2){		/* bitblt up a hole */
		y1+=newlnsz;	/* BOTTOM of char */
		y2+=newlnsz;
		Ubitblt(D, Rect(t->rect.origin.x, y1,
			t->rect.corner.x, t->rect.corner.y-(y2-y1)),
			D, Pt(t->rect.origin.x, y2), F_STORE);
		clear(Rect(t->rect.origin.x, y1, t->rect.corner.x, y2), 1);
		y1=(y1-t->rect.origin.y)/newlnsz-1;
		y2=(y2-t->rect.origin.y)/newlnsz-1;
		scrollcpl(t, t->nlines-1-(y2-y1), y1, y2-y1);
	}else
		y2=(y2-t->rect.origin.y)/newlnsz;
	draw(t, sp, t->str.s+sn, n+s->n);	/* redraw this line */
	inscomplete=complete;	/* cough! */
	setcpl(t, (ptofchar(t, sn).y-t->rect.origin.y)/newlnsz, y2);
#ifdef	garbage
	n=charofpt(t, Pt(XMAX, YMAX));	/* Re-use of n */
	if(t->str.n-n > MAXEXTRA)
		delstring(&t->str, n+MAXEXTRA/2, t->str.n);
#endif
}
