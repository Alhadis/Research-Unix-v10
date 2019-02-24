#include	<jerq.h>

#define		ONE		512

Point buf1[128], buf2[128], *old = buf2, *new = buf1;

jdraw(b)
	Bitmap *b;
{
}
#ifdef	AGH
/*
	for(j = nx+ny-3; j > 0; j--)
	{
		y = min(ny-1, j);
		lasty = x = j-y;
		while(y != lasty)
		{
			POLY(x, y, x, y-1, x+1, y-1, x+1, y);
			x++;
			y--;
		}
		wait(CPU);
	}
*/
	Point left, right, p, *np, *op;
	int wleft;

	old[0].y = -1;
	np = new;
	op = old;
	nextpt(np++);
	nextpt(np++);
	left = np[-1];
	segment(b, new[0], new[1], F_OR);

	while(nextpt(&right))
	{
		if(left.x > right.x)
		{
			left = right;
			np->y = -1;
			np = old; old = new; new = np; op = old;
			continue;
		}
		if(left.x < op->x)
		{
			*np++ = left;
			*np++ = right;
			segment(b, left, right, F_OR);
		}
		else if(left.x > op->x)
			*np++ = *op++;
		else
		{
			
		}
		left = right;
	}
}

addseg(b, l, ls, r, rs)
	Bitmap *b;
	Point l, r;
	register Segment *ls, *rs;
{
	register Segment *s;

	if(l.x != ls->l.x)
	{
		s = newseg(l, ls->r);
		ls->r = l;
		s->next = ls->next;
		ls->next = s;
		if(ls == rs) rs = s;
	}
	else
		ls->l.y = l.y;
	recalc(ls);
	if(r.x != rs->r.x)
	{
		s = newseg(r, rs->r);
		rs->r = r;
		s->next = rs->next;
		rs->next = s;
	}
	else
		rs->r.y = r.y;
	recalc(rs);
	segment(b, l, r, F_OR);
}

static Segment *
newseg(l, r)
	Point l, r;
{
	register Segment *s;

	s = (Segment *)alloc(sizeof *s);
	s->next = 0;
	s->l = l;
	s->r = r;
	recalc(s);
	return(s);
}

recalc(s)
	register Segment *s;
{
	s->m = muldiv(s->r.y-s->l.y, ONE, s->r.x-s->l.x);
	s->b = s->l.y - muldiv(s->m, s->l.x, ONE);
}

crosses(s, l, r, p)
	Segment *s;
	Point l, r, *p;
{
	register m, b;

	m = muldiv(r.y-l.y, ONE, r.x-l.x);
	if(m == s->m) return(0);
	b = l.y - s->m*l.x;
	p->x = muldiv(s->b-b, ONE, m-s->m);
	p->y = m*p->x + b;
	return((p->x >= s->l.x) && (p->x <= s->r.x) && (p->x >= l.x) && (p->x <= r.x));
}

ptrels(p, s)
	Point p;
	Segment *s;
{
	register Segment *s;

	if((s->l.y < 0) || (s->r.y < 0)) return(1);
	return((muldiv(s->m, p.x, ONE)+s->b) - p.y);
}
#endif
