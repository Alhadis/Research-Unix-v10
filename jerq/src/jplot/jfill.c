#include <jerq.h>
#include <layer.h>
#include "jjplot.h"
int fmode;
int place();
jjfill(so, s, yymin)
int	so, yymin;
struct seg *s;
{
	register struct seg *sp;
	struct seg *s1, *s2, *st;
	register int	yb;
	struct seg *sfin;

	sfin = s1 = s + so - 1;
	for (sp=s; sp<=sfin; sp++){
		if (sp->stat){
			if (sp->j1.y == sp->j2.y){
				if (sp < sfin) {
					s2 = sp+1;
					while(s2<=sfin && s2->j1.y==s2->j2.y){
						if (sp->j2.x == s2->j1.x)
							sp->j2.x = s2->j2.x;
						else if (sp->j1.x == s2->j2.x)
							sp->j1.x = s2->j1.x;
						s2++->stat = 0;
					}
				}
				else s2 = sp;
				if(s1->j1.y==sp->j1.y && s2->j1.y==sp->j1.y)
					sp->stat = 0;
				else if(s1->j2.y==sp->j1.y && s2->j2.y==sp->j1.y)
					sp->stat = 0;
			}
		}
		s1 = sp;
	}

	qsort(s, so, sizeof(struct seg), place);
	sfin++;
	s1 = s;
	yb = s1->j1.y;
	for(sp=s; yb<=sp->j1.y; sp++);
	s2 = sp - 1;
	for (; yb >= yymin; yb--) {
		draw(yb, s1, s2);
		for (st = s1; st <= s2; st++) {
			if (st->stat)
				if (yb <= st->j2.y)
					st->stat = 0;
		}
		if (sp >= sfin)
			continue;
		for (; yb <= (sp->j1.y + 1) && sp < sfin; sp++) {
			for (st = s1; st <= s2; st++) {
				if (sp->j1.y == st->j2.y) {
					if (sp->j1.x == st->j2.x)
						st->stat = 0;
					else if(sp->j2.y==sp->j1.y && sp->j2.x==st->j2.x)
						st->stat = 0;
				}
			}
		}

		s2 = sp - 1;
		while (!(s1->stat))
			s1++;
	}
}
draw(y, pr1, pr2)
int	y;
struct seg *pr1, *pr2;
{
	register struct seg *pr;
	register int *xp;
	int	x[32], yi, yii, xi, xii, xj;
	int *xpe;
	for(pr=pr1, xp=x; pr <= pr2;pr++){
		if (pr->stat)
			if (pr->j1.y != pr->j2.y){
				*xp++ = xcut(y, pr->j1.x, pr->j1.y, pr->j2.x, pr->j2.y);
				if(xp > &x[31])
					return;
			}
	}
	xpe = xp;
	if(xpe < &x[1])
		return;
	do {
		xj = 0;
		for(xp=x+1;xp<xpe;xp++){
			if(*(xp-1) > *xp){
				xj = *(xp-1);
				*(xp-1) = *xp;
				*xp = xj;
			}
		}
	} while(xj);
	for(xp=x; xp<xpe;xp++){
		xj = *xp++;
		texture(&display,Rect(xj, y, *xp, y+1),&t, fmode);
	}
}
xcut(y, x1, y1, x2, y2)
{
	long 	dx, dy, xy;
	int x;
	if (y1 == y2) {
		x = x1 > x2 ? x1 : x2;
		return(x);
	}
	dy = y1 - y2;
	dx = x1 - x2;
	xy = (long)y1 * x2 - x1 * (long)y2;
	x = (y * dx + xy) / dy;
	return(x);
}
place(p1, p2)
struct seg *p1, *p2;
{
	if(p1->j1.y > p2->j1.y)
		return(-1);
	else if(p1->j1.y < p2->j1.y)
		return(1);
	else if(p1->j1.x > p2->j1.x)
		return(-1);
	else if(p1->j1.x < p2->j1.x)
		return(1);
	else if(p1->j2.x > p2->j2.x)
		return(-1);
	else if(p1->j2.x < p2->j2.x)
		return(1);
	else if(p1->j2.y > p2->j2.y)
		return(-1);
	else if(p1->j2.y < p2->j2.y)
		return(1);
	else return(0);
}
