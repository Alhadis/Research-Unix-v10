#include <jerq.h>
#include "jerqio.h"
#include "rock.h"
#include "event.h"
#include "ship.h"
extern Point pos, vel;
extern int xflag;
extern Bitmap *rockseq[];
/* batcomp calls are spread around, but all functions are here */

int curthreat;	/* rocknum of worst guy */
extern int death;
extern int costheta, sintheta;
int curtime = 10000;	/* and when he hits us */
long curdist = 1000000;		/* if not threat, dist of worst guy */
extern int score;
int computer, compbut;
int screens;

batcomp(r)	/* called from moverock */
register Rock *r;
{	int a, b, c, d;
	long dist, u, t;
	if(death)
		return;
	a = r->x.x - pos.x;
	b = r->x.y - pos.y;
	c = r->v.x - vel.x;	/* our velocity has to be scaled */
	d = r->v.y - vel.y;
/* only understands current region, not lattice */
	t = (long)a * c + (long)b * d;
	u = (long)c * c + (long)d * d + 1;	/* don't divide by zero */
	dist = (long)b * c - (long)a * d;
	dist *= dist;
	dist /= u;
	if(curthreat != -1 && rock[curthreat].hash == -1)
		curthreat = -1, curtime = 10000, curdist = 1000000;
	t = -t/u;
	if(t >= -3 && dist <= r->r * r->r + RADSHIP * RADSHIP *2) {
		if(curthreat == -1 || t < curtime && t >= 0) {
			curtime = t;
			curthreat = r - rock;
		}
		return;
	}
	if(curthreat == r - rock)
		curdist = dist;
	dist = (long)a * a + (long)b * b;
	if(curthreat == -1 || dist < curdist && t >= 0) {
		curdist = dist;
		curthreat = r - rock;
	}
	if(screens & curtime < 20) {
		curtime = 10000;
		split(r);
		explode(r->x);
		killrock(r);
		score -= 6;
	}
}
int velb = 24;
int invb = 1;
#define RET(n)	return(compbut = dir[n])
#define LEFT 6
#define RIGHT 3
char dir[16] = {LEFT, LEFT, RIGHT, RIGHT, LEFT, RIGHT, LEFT, RIGHT, LEFT, LEFT, RIGHT};
strat()
{	register Rock *r = rock + curthreat;
	int a, b, c, d, x;
	int tbul, trock;
	if(r->hash == -1) {
		curthreat = -1;
		curtime = 10000;
		curdist = 1000000;
	}
	compbut = 0;
	if(curthreat == -1)
		return(0);
	a = r->x.x - pos.x + r->v.x;
	b = r->x.y - pos.y + r->v.y;
	c = r->v.x;	/* we'd better be at rest */
	d = r->v.y;
	if(m(a, costheta) + m(b, sintheta) < 0)
		RET(0);		/* not close */
	x = m(c, sintheta) - m(d, costheta);
	tbul = ((long)b * c - (long)a * d);
	/* tbul / xVELB/INV is the actual time */
	trock = m(b, costheta) - m(a, sintheta);
	/* trock / x is the actual time */
	if(x < 0) {
		if(tbul >= 0)
			RET(1);
		else if(trock > 0)
			RET(2);
		else if(trock * velb > tbul * invb)
			RET(3);
		else
			RET(4);
	}
	else if(x > 0) {
		if(tbul <= 0 && trock > 0)
			RET(5);
		else if(trock < 0)
			RET(6);
		else if(trock * velb > tbul * invb)
			RET(7);
		else
			RET(8);
	}
	else if(trock < 0)
		RET(9);
	else
		RET(10);
right:
	return(compbut = 3);
left:
	return(compbut = 6);
}
