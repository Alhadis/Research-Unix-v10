#include <jerq.h>
#include <font.h>
#include <3d.h>
#include "nodes.h"

struct BALLS store[NNODES];	/* information on spheres    */
Bitmap *off;			/* off screen scratch bitmap */
Bitmap *hog[109];		/* allow up to 3*36+1 frames */
int NN=0, NM=0, MAX=0;		/* for cycling through hog   */

hoard()
{	int i;
	char str[128];

	for (i = 0; i < 109 && (hog[i] = balloc(Drect)); i++)
		hog[i]->rect = inset(Drect, 5);
	if (i > 0) bfree(hog[--i]);	/* be nice  */
	if (i > 0) bfree(hog[--i]);	/* be nicer */
	MAX = i;
}

unhoard()
{	int i;

	for (i = 0; i < MAX; i++) bfree(hog[i]);
	MAX = NM = NN = 0;
}

playback(how)
{	int i=NM-1, di=-1;

	while ((!button123()))
	{	nap(10);
		i = stepper(i, di, how);
		if (how == 1) di = -di;
		i += di;
	}
}

stepper(from, da, how)
{	Point p;
	Bitmap *boo;
	int i, j;
	int b1, b2;
	int di = da;

	p = add(Pt(5, 5), Drect.origin);

	for (j = 0, i = from; i >= 0 && i < NM && j < MAX; j++, i += di)
	{	boo = hog[i%MAX];
		bitblt(boo, boo->rect, &display, p, F_STORE);
		wait(CPU);
		if (how == 2)
		{	while (!button123()) nap(2);
			b1 = button1();
			b2 = button2();
			while (button12()) nap(2);
			if (b1) { di = -1; continue; }
			if (b2) { di =  1; continue; }
		}
		if (button3()) return -2;
		nap(5);
	}
	return i;
}

makebubble()
{	int i;
	if (!(off = balloc(Drect)))
	{	outofmem();
		exit(0);
	}
	off->rect = inset(Drect, 5);
	for (i = 0; i < NNODES; i++)
	{	store[i].n = 0;
		store[i].vis = 1;
	}
	origin();	/* something to stare at */
	bitblt(off, off->rect, &display, add(Pt(5, 5), Drect.origin), F_STORE);
}

makeone(tag, radius)
{
	if (store[tag].n > 0) return;

	store[tag].name[0] = (char) (tag+'A');
	store[tag].name[1] = '\0';
	store[tag].n = 0;
	store[tag].oradius = radius;
	crunch(radius, tag);
	wait(CPU);
}

crunch(r, i)
{	long R2;
	Bitmap *ball;
	int R = (r < 6)?6:r;

	R2 = R*(long)R;
	if (!(ball = balloc(Rect(0, 0, 2*R+1, 2*R+1))))
	{	outofmem();
		nap(200);
		exit(0);
	}

	rectf(ball, ball->rect, F_STORE);
	sphere(R, R2, ball, 3*R-22);
	string(&defont, store[i].name, ball, Pt(R-3, R-7), F_XOR);
	store[i].Ball = ball;
	store[i].radius = R;
}

resize(i)
{	extern short delta;
	if (store[i].n > 0)
	{	bfree(store[i].Ball);
		crunch((store[i].oradius*100)/delta, i);
	}
}

origin()
{	int ox, oy, qh, qw;
	int i, j, ang;		/* ang: screen size of one Angstrom */
	char buf[64];
	extern long Ro[];

	ox = Drect.origin.x + 8;
	oy = Drect.origin.y + 8;
	qw = Drect.corner.x - Drect.origin.x - 16;
	qh = Drect.corner.y - Drect.origin.y - 16;
	segment(off, Pt(ox, oy), Pt(ox+qw, oy), F_XOR);
	segment(off, Pt(ox, oy), Pt(ox, oy+qh), F_XOR);

	ang = (20*100)/delta; if (ang == 0) ang = 1;
	for (i = ox+ang, j = oy; i < ox+qw; i += ang)
		segment(off, Pt(i,j), Pt(i,j+8), F_XOR);
	for (j = oy+ang, i = ox; j < oy+qh; j += ang)
		segment(off, Pt(i,j), Pt(i+8,j), F_XOR);
	sprintf(buf, "(%d,%d,%d)", (Ro[0]*180)/PI, (Ro[1]*180)/PI, (Ro[2]*180)/PI);
	string(&defont, buf, off, add(Drect.origin, Pt(12, 12)), F_XOR);
}

reshaped()
{	Texture *TT;
	extern Texture coffee;

	if (P->state & (RESHAPED|MOVED))
	{	TT = cursswitch(&coffee);
		bfree(off);
		P->state &= ~(RESHAPED|MOVED);
		if (!(off = balloc(Drect)))
		{	outofmem();
			exit(0);
		}
		off->rect = inset(Drect, 5);
		unhoard();
		hoard();
		setmidpt(); 
		center(0);
		redraw();
		cursswitch(TT);
	}
}

drawbubble(n)
	Node *n;
{
	Point Norm(), p, at;
	int i, depth, sum;
	extern short skel, shadow;

	i = n->flag;
	depth = n->z;
	sum = depth+store[i].radius;
	at = Norm(n->p);
	if (skel)
		string(&defont, store[i].name, off, at, F_XOR);
	else if (sum < 0 && shadow)
		disc(off, at, store[i].radius, F_OR);
	else
	{	p = sub(at, Pt(store[i].radius, store[i].radius));
		disc(off, at, store[i].radius, F_CLR);		/* mask   */
		bitblt(store[i].Ball, store[i].Ball->rect, off, p, F_XOR);
	}
	wait(CPU);
}

flash(n)
	Node *n;
{	Point Norm();

	disc(&display, Norm(n->p), store[n->flag].radius, F_XOR);
}

connect(a, b)
	Node *a, *b;
{
	Point Norm();
	segment(off, Norm(a->p), Norm(b->p), F_OR);
}

blast()
{	Point p;

	p = add(Pt(5, 5), Drect.origin);
	origin();
	bitblt(off, off->rect, &display, p, F_STORE);
	if (MAX > 0)
	{	bitblt(off, off->rect, hog[NN], p, F_STORE);
		NN = (NN+1)%MAX; NM++;
	}
}

restore()
{	Bitmap *boo;

	if (MAX > 0)
	{	boo = hog[(NM-1)%MAX];
		bitblt(boo,boo->rect, &display,add(Pt(5,5),Drect.origin),F_STORE);
	}
}

#define	DITHSIZE	8
#define	DITHMASK	(DITHSIZE-1)

int dith[DITHSIZE][DITHSIZE]={
	0,	32,	8,	40,	2,	34,	10,	42,
	48,	16,	56,	24,	50,	18,	58,	26,
	12,	44,	4,	36,	14,	46,	6,	38,
	60,	28,	52,	20,	62,	30,	54,	22,
	3,	35,	11,	43,	1,	33,	9,	41,
	51,	19,	59,	27,	49,	17,	57,	25,
	15,	47,	7,	39,	13,	45,	5,	37,
	63,	31,	55,	23,	61,	29,	53,	21,
};

sphere(R, R2, ball, scale)
	long R2;
	Bitmap *ball;
{
	register x, y, z, I;

	rectf(ball, ball->rect, F_XOR);
	disc(ball, Pt(R,R), R, F_XOR);
	for (y = -R; y <= R; y++, wait(CPU))	/* y across, pos to right */
	for (z = -R; z <= R; z++)	/* z pos up */
	{	if (z*z + y*y > R2)
			continue;
		x = sqrtryz(R, y, z);
					/* I = (illum.r)(view.r)  */
		I = muldiv(x,		/* view.r   */
		   (x*5+y*4+z*3),	/* illum.r  */
		    scale);		/* a scale factor: ~RLV   */
		if (I <= 0)		/* unilluminated crescent */
			continue;
		if (I > dith[y&DITHMASK][z&DITHMASK])
			point(ball, Pt(y+R, -z+R), F_XOR);
	}
}
