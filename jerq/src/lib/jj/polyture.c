#include	<jerq.h>

static emax(a,b) { return((a<b)? b:a); }
static emin(a,b) { return((a>b)? b:a); }
static ycmp(), xcmp(), xcut();

typedef struct Edge
{
	Point p1, p2;
	short dolow;
} Edge;
static Edge edges[300];
static Edge *canon();

polyture(bp, pts, n, t, mode)
	Bitmap *bp;
	Point *pts;
	Texture *t;
{
	register Edge *eend, *e, *estart;
	register y;
	int ymin, ymax, flag;
	Point p;
	int xcuts[64], *xp, *x;

	eend = canon(pts, n, &ymax);
	qsort(edges, eend-edges, sizeof(edges[0]), ycmp);
	ymin = edges->p1.y;
	eend->p1.y = eend->p2.y = ymax+1;
	/*
		traditional scanline
	*/
	for(estart = eend = edges, y = ymin; y <= ymax; y++)
	{
		while(estart->p2.y < y) estart++;
		while(eend->p1.y <= y) eend++;
		for(e = estart, xp = xcuts; e < eend; e++)
			if((y < e->p2.y) || (e->dolow && (y == e->p2.y)))
				*xp++ = xcut(y, e);
		qsort(xcuts, xp-xcuts, sizeof(*xp), xcmp);
		for(x = xcuts; x < xp; x += 2)
			texture(bp, Rect(*x, y, x[1]+1, y+1), t, mode);
	}
}

static Edge *
canon(pts, n, ymax)
	register Point *pts;
	register *ymax;
{
	register Edge *le, *e;
	register Point *p, *lp, pt;
	int vert;

	/*
		delete horizontal edges; compact vertical edges into one
	*/
	p = pts;
	lp = p+n;
	e = edges;
	*ymax = p->y;
	e->p1 = *p++;
	for(vert = 0; p < lp; p++)
	{
		if(*ymax < p->y) *ymax = p->y;
		if(p->y != p[-1].y)
			e++->p2 = *p;
		e->p1 = *p;
	}
	if(vert) e++;
	le = e;
	*le = edges[0];
	le[1] = edges[1];
	for(e = edges+1; e <= le; e++)
	{
		if(e->p1.y < e->p2.y)	/* going down */
			e->dolow = (e[1].p1.y-e[1].p2.y) > 0;
		else
			e->dolow = (e[-1].p1.y-e[-1].p2.y) < 0;
	}
	edges[0] = *le;
	/*
		canonise the edges so that p1.y < p2.y
		or if p1.y == p2.y then p1.x < p2.x.
	*/
	for(e = edges; e < le; e++)
	{
		if((e->p1.y > e->p2.y) || ((e->p1.y == e->p2.y)&&(e->p1.x > e->p2.x)))
			pt = e->p1, e->p1 = e->p2, e->p2 = pt;
	}
	return(le);
}

static
ycmp(a, b)
	register Edge *a, *b;
{
	register k;

	if(k = a->p1.y - b->p1.y)
		return(k);
	return(a->p2.y - b->p2.y);
}

static
xcmp(a, b)
	register int *a, *b;
{
	return(*a - *b);
}

static
xcut(y, e)
	register Edge *e;
{
	return(e->p1.x + muldiv(e->p2.x-e->p1.x, y-e->p1.y, e->p2.y-e->p1.y));
}

