#include "jerq.h"

#define c corner
#define o origin

int rectXrect(r, s) Rectangle r, s;
{
	return(r.o.x<s.c.x && s.o.x<r.c.x && r.o.y<s.c.y && s.o.y<r.c.y);
}

int ptinrect(p, r) Point p; Rectangle r;
{
	return(p.x>=r.o.x && p.x<r.c.x && p.y>=r.o.y && p.y<r.c.y);
}

rectclip(rp, b)	register Rectangle *rp; Rectangle b;
{	register Rectangle *bp= &b;
	/* Expand rectXrect() in line for speed */
	if ((rp->o.x < bp->c.x && bp->o.x < rp->c.x &&
	     rp->o.y < bp->c.y && bp->o.y < rp->c.y)==0) return 0;
	/* They must overlap */
	if (rp->origin.x < bp->origin.x) rp->origin.x = bp->origin.x;
	if (rp->origin.y < bp->origin.y) rp->origin.y = bp->origin.y;
	if (rp->corner.x > bp->corner.x) rp->corner.x = bp->corner.x;
	if (rp->corner.y > bp->corner.y) rp->corner.y = bp->corner.y;
	return 1;
}
