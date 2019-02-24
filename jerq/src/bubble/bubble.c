#include	<jerq.h>
#include	<font.h>
#include	<3d.h>
#include	"nodes.h"
#include	"textures.h"

Node *tab[CLIM];		/* lookup table */
short delta=100, ncount=0, step=10, skel=0;
short shadow=1, direction=1;
long Ro[3];
char buf[512];

extern struct BALLS store[NNODES];
struct LRot *frot;

main()
{	register int k;
	int id=0, sign=1, depth=0, tag='X', radius;
	Texture *TT; Point p;

	frot = (struct LRot *)NULL;
	request(SEND|RCV|KBD|MOUSE);

	TT = cursswitch(&coffee);
	Ro[0] = Ro[1] = Ro[2] = 0;

	setmidpt();
	makebubble();
	hoard();
	sendchar('1');
	while ((k = rcvchar()) != 'X')
	{	wait(CPU);
		if (k >= 'A' && k <= 'Z') { tag = k; continue; }
		switch (k)
		{
		case 'r':	radius = id; id = 0; sign = 1;
				another(p, radius, depth, tag); break;
		case 'x':	p.x   =  sign*id; id = 0; sign = 1; break;
		case 'y':	p.y   =  sign*id; id = 0; sign = 1; break;
		case 'z':	depth = -sign*id; id = 0; sign = 1; break;
		case '-':	id = 0; sign = -1; break;
		case '+':	id = 0; sign =  1; break;
		case '0': case '1': case '2': case '3': case '4':
		case '5': case '6': case '7': case '8': case '9':
				id = id*10 + k-'0';
		default:	break;
		}
	}

	announce("drawing");
	center(0);
	redraw();
	announce("making bonds");
	makebonds();
	announce("ready       ");
	(void)cursswitch(TT);

	while (wait(MOUSE))
	{	reshaped();
		if ((own()&MOUSE) && button123())
		{	if (button2()) restore();
			else
			if (button3()) nmenuhit(3);
			else
			{	int a, b;
				restore();
				a = pickone(-1);
				while (!button1())
				{	flash(tab[a]); sleep(5); flash(tab[a]);
				}
				b = pickone(a);
				flash(tab[a]);
				flash(tab[b]);
		}	}
		wait(CPU);
	}
	putchar('0');
}

compute(a, b)
{	int l; long l2, distance2();

	l2 = distance2(tab[a], tab[b]);
	l  = sqrt(l2);
	digit(4*l, buf);

	announce(buf);
}

pickone(j)
{	int i;

	do {	i = whatsphere(mouse.xy);
		flash(tab[i]);
		if (j >= 0)
		{	flash(tab[j]);
			compute(i, j);
			flash(tab[j]);
		}
		else sleep(5);
		flash(tab[i]);
	} while (button1());
	flash(tab[i]);

	return i;
}

digit(n, st)
	char *st;
{	char *str = st;
	int base=1000;

	if (n < 10) sprintf(str, "0.0%d", n);
	else if (n < 100) sprintf(str, "0.%d", n);
	else
	{	while (n/base > 0)
			base *= 10;
		for (base /= 10; base > 0; base /= 10)
		{	if (base == 10)
				*str++ = '.';
			if (n >= base)
			{	*str++ = n/base + '0';
				n %= base;
			} else
				*str++ = '0';
		}
		*str = 0;
	}
	strcat(st, " A    ");
}

compar(a, b)
	Node **a, **b;
{
	Node *m = *a, *n = *b;
	register int p = m->z + store[m->flag].radius;
	register int q = n->z + store[n->flag].radius;
	if (p <  q) return -1;
	if (p == q) return  0;
	return 1;
}

whatsphere(pt1)
	Point pt1;
{	Node *n;
	int i, best = 0;
	long closest = -1;
	long dx, dy, dist2;
	Point pt2, Norm();

	for (n = tab[i=0]; i < ncount; n = tab[++i])
	{	pt2 = Norm(n->p);
		dx = (long) (pt2.x - pt1.x)/10;
		dy = (long) (pt2.y - pt1.y)/10;
		dist2 = dx*dx + dy*dy;
		if (closest == -1
		||  dist2 < closest
		|| (abs(dist2-closest) < 3 && tab[best]->z < n->z))
		{	closest = dist2;
			best = i;
		}
	}
	return best;
}

struct LRot *
newrot(an, ax)
	unsigned short an, ax;
{	struct LRot *n;

	n = (struct LRot *) alloc(sizeof (struct LRot));
	if (!n) { outofmem(); return (struct LRot *) NULL; }
	n->link = (struct LRot *) NULL;
	n->angle = an;
	n->axis  = ax;
	return n;
}

Rdetach(angle)
	unsigned short angle;
{	struct LRot *n, *m;

	for (n = frot; n; m = n, n = n->link)
		;
	m->angle -= angle;
}

Rattach(angle, axis)
	unsigned short angle, axis;
{	struct LRot *n, *m, *newrot();

	if (frot)
	{	for (n = frot; n; m = n, n = n->link)
			;
		if (m->axis == axis)
			m->angle += angle;
		else
			m->link = newrot(angle, axis);
	}
	else
		frot = newrot(angle, axis);
}

Xform(n)
	Node *n;
{
	struct LRot *m;
	Hcoordl h;

	h.x = n->op.x;
	h.y = n->op.y;
	h.z = n->oz;
	h.w = 1;
	for (m = frot; m; m = m->link)
	{	init3(&display, 1, 1);
		rot3(m->angle, m->axis);
		h = xformp(hcoord(h.x/h.w, h.y/h.w, h.z/h.w, 1));
	}
	n->p.x = h.x/h.w;
	n->p.y = h.y/h.w;
	n->z   = h.z/h.w;
}

rotate()
{	Node *n;
	int i;

	for (n = tab[i=0]; i < ncount; n = tab[++i])
	{	Xform(n);
		wait(CPU);
	}
}

spin(j)
{	long i, k, dr = (PI*step)/180;
	long Dr = ((long)direction)*dr;
	unsigned short ddr;
	Texture *TT;

	TT = cursswitch(&wrap);
	if (Dr < 0) Dr += (2*PI);
	ddr = (unsigned short) Dr;
	for (i = 0; i < 2*PI; i += dr)
	{	Rattach(ddr, j);
		k = Ro[j];
		Ro[j] = (Ro[j] + ddr) % (2*PI);
		rotate();
		if (!redraw())
		{	Ro[j] = k;
			cursswitch(TT);
			Rdetach(ddr);
			rotate();	/* restore old rotation */
			break;
		}
		wait(CPU);
	}
	cursswitch(TT);
}

outofmem()
{
	sprintf(buf, "%d - out of memory", ncount);
	announce(buf);
}

another(p, radius, depth, which)
	Point p;
{
	Node *n;

	if (ncount >= CLIM)
	{	announce("too many bubbles...");
		return;
	}
	if (n = (Node *)alloc(sizeof *n))
	{	n->p = n->op = p;
		n->z = n->oz = depth;
		n->flag = which-'A';
		tab[ncount++] = n;
		makeone(which-'A', radius);
	} else
	{	outofmem();
		return;
	}
	store[which-'A'].n++;
	sprintf(buf, "%d        ", ncount);
	announce(buf);
}

redraw()
{	extern Bitmap *off; int r;

	rectf(off, Drect, F_CLR);
	qsort(tab, ncount, sizeof (Node *), compar);
	r = partial();
	if (r) blast();
	return r;
}

partial()
{	Node *n;
	short i;

	for (i = 0; i < ncount; i++)
	{	n = tab[i];
		if (store[n->flag].vis)
			drawbubble(n);
		if (button123()) return 0;
	}
	if (skel) drawbonds();
	return 1;
}

long
distance2(p, q)
	Node *p, *q;
{
	long x = (p->op.x - q->op.x);
	long y = (p->op.y - q->op.y);
	long z = (p->oz - q->oz);

	return (x*x + y*y + z*z);
}

moveit(w)
	Point w;
{	Node *n;
	Point q;
	int i;

	cursswitch(&coffee);
	q.x = muldiv(w.x, delta, 100);
	q.y = muldiv(w.y, delta, 100);
	for (n = tab[i=0]; i < ncount; n = tab[++i])
	{	n->op = n->p = add(n->p, q);
	}
}

center(how)
{	Node *n; Point q; int newz;
	int maxx=-30000, minx=30000;
	int maxy=-30000, miny=30000;
	int maxz=-30000, minz=30000;
	int i;

	if (ncount==0) return;
	for (n = tab[i=0]; i < ncount; n = tab[++i])
	{	if (n->p.x > maxx) maxx = n->p.x;
		if (n->p.x < minx) minx = n->p.x;
		if (n->p.y > maxy) maxy = n->p.y;
		if (n->p.y < miny) miny = n->p.y;
		if (n->z   > maxz) maxz = n->z;
		if (n->z   < minz) minz = n->z;
	}
	q.x  = -(maxx + minx)/2;
	q.y  = -(maxy + miny)/2;
	newz = -(maxz + minz)/2;
	for (n = tab[i=0]; i < ncount; n = tab[++i])
	{	n->op = n->p = add(n->p, q);
		n->oz = n->z += newz;
	}
	if (how) fit(maxx-minx, maxy-miny);

}

fit(n, m)
{	long dd = 100;	/* offset from edge */
	long dx = Drect.corner.x - Drect.origin.x;
	long dy = Drect.corner.y - Drect.origin.y;
	long Dx = dd+(100*n)/delta;
	long Dy = dd+(100*m)/delta;
	long i  = (Dx - dx > Dy - dy) ? (100L*Dx)/dx : (100L*Dy)/dy;
	delta = (short) (((long)delta * i)/100L);
}
