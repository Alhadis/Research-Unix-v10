/*% mcc -g % -o swar
 * swar -- jerq space war
 * td&rob 84.01.01
 */
#include <jerq.h>
#include <font.h>
int xc, yc;
#define	NOBJ	(1+2+6*2)
#define	MSPEED	(V/32)		/* speed of missile relative to ship */
#define	SZ	512		/* maximum scaled coordinate */
#define	V	256		/* velocity scale factor */
#define	G	((long)V*11585)	/* 11585 is SZ**(3./2.) */
#define	ALIVE	1
#define	DEAD	2
#define	SUN	3
#define	BOOM	4
#define	HYPER	5
#define	SLEEP	2		/* ticks per iteration */
#define	TUBA	(120/SLEEP)	/* ticks until born again */
#define	HYTIME	((40+rand()%40)/SLEEP)
short starbits[]={
#include "deathstar.icon"
};
Bitmap stardwg={(Word *)starbits, 1, 0, 0, 16, 16};
short p0bits[]={
#include "player0.icon"
};
Bitmap p0dwg={(Word *)p0bits, 2, 0, 0, 44, 44};
short p1bits[]={
#include "player1.icon"
};
Bitmap p1dwg={(Word *)p1bits, 2, 0, 0, 44, 44};
short misbits[]={
#include "missile.icon"
};
Bitmap misdwg={(Word *)misbits, 1, 0, 0, 32, 32};
short boombits[]={
#include "boom.icon"
};
Bitmap boomdwg={(Word *)boombits, 2, 0, 0, 64, 64};
struct obj{
	int x, y;
	long vx, vy;		/* scaled by V */
	int orientation;
	int state;
	int diameter;
	Bitmap *bp;
	int curdwg;
#define	wrapped	timer
	int timer;
}obj[NOBJ], iobj[]={
	{0, 0, 0, 0, 0, SUN, 16, &stardwg},
	{ 300, 0, 0,  5*V, 8, ALIVE, 11, &p0dwg, 0, TUBA},
	{-300, 0, 0, -5*V, 0, ALIVE, 11, &p1dwg, 0, TUBA},
	{0, 0, 0, 0, 0, ALIVE, 8, &misdwg},
};
#define	ATT	(&obj[0])
#define	P0	(&obj[1])
#define	P1	(&obj[2])
int score[3];
#define	NORIENTATION	16
struct dv{
	long x, y;
}dv[NORIENTATION]={
#include "accel.h"
};
int xc, yc, size;
#define	sq(x)	((long)(x)*(long)(x))
int min(a, b){return(a<b?a:b);}
long lmin(a, b)long a,b;{return(a<b?a:b);}
int abs(a){return(a<0?-a:a);}
main(){
	register i;
	register struct obj *o, *p;
	xc=(Drect.origin.x+Drect.corner.x)/2;
	yc=(Drect.origin.y+Drect.corner.y)/2;
	size=min(Drect.corner.x-Drect.origin.x,
		Drect.corner.y-Drect.origin.y)/2;
	rectf (&display, Drect, F_XOR);
	request(KBD);
	for(o=obj;o<=P1;o++)
		initobj(o);
	for(;o!=&obj[NOBJ];o++)
		o->state=DEAD;
	doscore();
	for(;;){
		for(o=obj;o!=&obj[NOBJ];o++){
			switch(o->state){
			case ALIVE:
			case SUN:
				for(p=o+1;p!=&obj[NOBJ];p++)
					if(p->state!=DEAD)
						collide(o, p);
				if(o>P1)
					left(o);
				move(o);
				break;
			case HYPER:
				if(--o->timer==0){
					blot(o, o->curdwg);
					o->state=ALIVE;
					if(rand()%4==0){
						deathto(o, 1);
						newscore(ATT, o);
					}
				}else
					move(o);
				break;
			case DEAD:
				if((o==P0 || o==P1) && --o->timer==0)
					initobj(o);
				break;
			case BOOM:
				shards(o);
				move(o);
				break;
			}
		}
		if(own()&KBD)
			kbdplayer(kbdchar());
		sleep(SLEEP);
	}
}
kbdplayer(c){
	switch(c){
	case 0xC0: left(P0);	break;
	case 0xD5: jerk(P0);	break;
	case 0xC6: right(P0);	break;
	case 0xC1: fire(P0);	break;
	case 0xC4:
	case 0xC2: hyper(P0);	break;
	case 'a':  left(P1);	break;
	case 'w':  jerk(P1);	break;
	case 'd':  right(P1);	break;
	case 's':  fire(P1);	break;
	case 'z':
	case 'x':  hyper(P1);	break;
	case 'Q':  exit();	break;
	}
}
hyper(o)
register struct obj *o;
{
	if(o->state!=ALIVE)
		return;
	o->state=HYPER;
	o->timer=HYTIME;
	blot(o, o->curdwg);
}
right(o)
register struct obj *o;
{
	if(++o->orientation==NORIENTATION)
		o->orientation=0;
}
left(o)
register struct obj *o;
{
	if(--o->orientation<0)
		o->orientation=NORIENTATION-1;
}
jerk(o)
register struct obj *o;
{
	o->vx+=dv[o->orientation].x/2;
	o->vy+=dv[o->orientation].y/2;
}
long isqrt(x)
register long x;
{
	register long s, u;
	if(x<=0)
		return(0);
	if(x>=32768L*(32768L/4))
		return(2*isqrt(x/4));	/* avoid overflow */
	for(s=2, u=4;u<x;s+=s, u*=4);
	while((u=((x+s*s)/s)>>1)<s)
		s=u;
	return(s);
}
fire(o)
register struct obj *o;
{
	register struct obj *m;
	register vx, vy, vl;

	if(o->state!=ALIVE)
		return;
	for(m=o+2;m<&obj[NOBJ];m+=2)
		if(m->state==DEAD){
			initobj(m);
			m->state=ALIVE;
			vl=isqrt(sq(o->vx)+sq(o->vy));
			if(vl==0)
				vl=V;
			vx=muldiv(vl, dv[o->orientation].x, V);
			vy=muldiv(vl, dv[o->orientation].y, V);
			m->x=o->x+muldiv(vx, (o->diameter+m->diameter), vl);
			m->y=o->y+muldiv(vy, (o->diameter+m->diameter), vl);
			m->vx=o->vx+MSPEED*dv[o->orientation].x;
			m->vy=o->vy+MSPEED*dv[o->orientation].y;
			blot(m, m->orientation);
			return;
		}
}
initobj(o)
	register struct obj *o;
{
	*o=(o>P1)?iobj[P1-obj+1]:iobj[o-obj];
	if(o<=P1)
		blot(o, o->orientation);
}
deathto(o, doboom)
register struct obj *o;
{
	o->state=DEAD;
	blot(o, o->curdwg);
	if(doboom)
		boom(o);
}
boom(o)
register struct obj *o;
{
	o->state=BOOM;
	o->bp=&boomdwg;
	o->diameter=boomdwg.rect.corner.x/4;
	blot(o, o->orientation=0);
}
shards(o)
register struct obj *o;
{
	if(++o->orientation==16){
		blot(o, o->curdwg);
		o->state=DEAD;
		o->timer=TUBA;
	}
}
move(o)
register struct obj *o;
{
	register long r32;
	register x, y;
	if(o->state==DEAD || o->state==SUN)
		return;
	r32=(long)o->x*o->x+(long)o->y*o->y;
	if(r32!=0){
		r32*=isqrt(r32);	/* pow(r, 3./2.) */
		if(r32!=0){
			o->vx-=G*o->x/r32;
			o->vy-=G*o->y/r32;
		}
	}
	x=o->x+o->vx/V;
	y=o->y+o->vy/V;
	if(x<-SZ || SZ<x){
		if(o>P1 && o->wrapped){
    Death:
			deathto(o, 0);
			return;
		}
		if(x<-SZ) x+=2*SZ; else x-=2*SZ;
		o->wrapped++;
	}
	if(y<-SZ || SZ<y){
		if(o>P1 && o->wrapped)
			goto Death;
		if(y<-SZ) y+=2*SZ; else y-=2*SZ;
		o->wrapped++;
	}
	if(o->state!=HYPER)
		blot(o, o->curdwg);
	o->x=x, o->y=y;
	if(o->state!=HYPER)
		blot(o, o->orientation);
}
#define	BLOTSIZE	5
#define	rescale(x)	muldiv(x, size, SZ)
blot(o, dwg)
register struct obj *o;
{
	register dx=dwg%4*o->diameter, dy=dwg/4*o->diameter;
	bitblt(o->bp, Rect(dx, dy, dx+o->diameter, dy+o->diameter),
		&display, Pt(rescale(o->x)+xc-o->diameter/2,
			rescale(o->y)+yc-o->diameter/2), F_XOR);
	o->curdwg=dwg;
}
collide(o, p)
register struct obj *o, *p;
{
	register doneboom;
	/* o<p always */
	if(o->state!=HYPER && p->state!=HYPER
	&& sq(rescale(o->x-p->x))+sq(rescale(o->y-p->y))<
		sq(o->diameter+p->diameter)/4){
		newscore(o, p);
		if(doneboom=o->state==ALIVE)
			deathto(o, 1);
		if(p->state==ALIVE)
			deathto(p, !doneboom || (o==P0 && p==P1));
	}
}
newscore(o, p)
register struct obj *o, *p;
{
	doscore();
	/* o<p always */
	score[2]++;
	if(o==P0 || p==P0)
		score[1]++;
	if(o==P1 || p==P1)
		score[0]++;
	doscore();
}
drawscore(str, sc, where)
	char *str;
{
	static char buf[16];
	register char *p=buf+6;
	register s;
	do; while(*p++=*str++);
	p=buf+6;
	s=abs(sc);
	do{
		*--p=s%10+'0';
		s/=10;
	}while(s);	/*mcc bug */
	if(sc<0)
		*--p='-';
	if(where==2)
		s=Drect.origin.x;
	else if(where==1)
		s=(Drect.origin.x+Drect.corner.x-strwidth(&defont, p))/2;
	else
		s=Drect.corner.x-strwidth(&defont, p);
	string(&defont, p, &display, Pt(s, Drect.origin.y+5), F_XOR);
}
doscore()
{
	drawscore(" MCI", score[0], 0);
	drawscore(" AT&T", score[2], 1);
	drawscore(" SPRINT", score[1], 2);
}
