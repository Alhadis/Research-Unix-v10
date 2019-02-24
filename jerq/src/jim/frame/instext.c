#include "frame.h"
int inscomplete;
#define	MAXEXTRA	1024	/* max # chars off end of screen */
frinsert(t, s, sn)
	register Frame *t;
	register String *s;
	register sn;	/* char number at which to insert */
{
	register char *p, *q;
	register n, y1, y2;
	Point sp;	/* point on screen */
	sp=ptofchar(t, sn);
	p=t->str.s+sn;
	for(n=0,q=p; sn+n<t->str.n && *q!='\n'; q++)
		n++;
	/* n is #chars to next '\n' */
	frameop(t, opnull, sp, s->s, s->n);
	frameop(t, opnull, endpoint, p, n);
	y2=endpoint.y;		/* where p[n] will end up */
	draw(t, sp, p, n);	/* undraws, leaves endpoint */
	y1=endpoint.y;		/* where p[n] is now */
	strinsert(&t->str, sn, s);	/* build new string */
	if(y1 != y2){		/* bitblt up a hole */
		y1+=newlnsz;	/* BOTTOM of char */
		y2+=newlnsz;
		bitblt(D, Rect(t->rect.origin.x, y1,
			t->rect.corner.x, t->rect.corner.y-(y2-y1)),
			D, Pt(t->rect.origin.x, y2), F_STORE);
		rectf(D, Rect(t->rect.origin.x, y1, t->rect.corner.x, y2), F_CLR);
		y1=(y1-t->rect.origin.y)/newlnsz-1;
		y2=(y2-t->rect.origin.y)/newlnsz-1;
		scrollcpl(t, t->nlines-1-(y2-y1), y1, y2-y1);
	}else
		y2=(y2-t->rect.origin.y)/newlnsz;
	draw(t, sp, t->str.s+sn, n+s->n);	/* redraw this line */
	inscomplete=complete;	/* cough! */
	setcpl(t, (ptofchar(t, sn).y-t->rect.origin.y)/newlnsz, y2);
	n=charofpt(t, Pt(XMAX, YMAX));	/* Re-use of n */
	if(t->str.n-n > MAXEXTRA)
		strdelete(&t->str, n+MAXEXTRA/2, t->str.n);
}
