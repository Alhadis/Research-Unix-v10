/*% cyntax -Dlint -j % && 3cc -go # %
 */
#include <jerq.h>
#ifdef	lint
void rectf();
#endif
#define	NSPLIT	6
#define	NDOT	(NSPLIT*NSPLIT*NSPLIT+1)
struct dot{
	Point pos;
	Point vel;
	int size, life;
}dot[NDOT];
struct dot *edot=&dot[NDOT];
Point gravity={ 0, 1 };
Point dv[NSPLIT]={
	 5, 0,
	 3, 4,
	-3, 4,
	-5, 0,
	-3,-4,
	 3,-4,
};
#define	DISP	16
int life, disp;
#define	or origin
#define	co corner
upd(d)
struct dot *d;
{
	register i=d->size;
	rectf(&display, raddp(Rect(-i, -i, i, i), d->pos), F_XOR);
	d->pos=add(d->pos, d->vel);
	if(d->pos.x<Drect.or.x+d->size){
		d->pos.x=2*(Drect.or.x+d->size)-d->pos.x;
		d->vel.x=-d->vel.x;
	}
	else if(d->pos.x>=Drect.co.x-d->size){
		d->pos.x=2*(Drect.co.x-d->size)-d->pos.x;
		d->vel.x=-d->vel.x;
	}
	if(d->pos.y<Drect.or.y+d->size){
		d->pos.y=2*(Drect.or.y+d->size)-d->pos.y;
		d->vel.y=-d->vel.y;
	}
	else if(d->pos.y>=Drect.co.y-d->size){
		d->pos.y=2*(Drect.co.y-d->size)-d->pos.y;
		d->vel.y=-d->vel.y*4/5;
	}
	d->vel=add(d->vel, gravity);
	if(--d->life!=0)
		return;
	d->size/=2;
	if(d->size==0)
		return;
	d->life=life;
	for(i=1;i!=NSPLIT && edot!=&dot[NDOT];i++){
		*edot=*d;
		edot->vel=add(edot->vel, div(mul(dv[i], disp), DISP));
		edot++;
	}
	d->vel=add(d->vel, div(mul(dv[0], disp), DISP));
}
int rrand(lo, hi){
	return lo+rand()%(hi-lo+1);
}
main(argc, argv)
char *argv[];
{
	register struct dot *d;
	request(KBD|CPU);
	srand((unsigned)Drect.co.x);
	while(kbdchar()==-1){
		if(edot==&dot[NDOT]){
			edot=dot+1;
			dot[0].pos.x=(Drect.or.x+Drect.co.x)/2;
			dot[0].size=16;
			dot[0].pos.y=Drect.co.y-dot[0].size;
			dot[0].life=life=rrand(10, 24);
			dot[0].vel.x=rrand(-5, 5);
			dot[0].vel.y=rrand(20, 50);
			disp=rrand(DISP/4, 2*DISP);
			rectf(&display, Drect, F_OR);
		}
		for(d=dot;d!=edot;d++)
			upd(d);
		wait(CPU);
	}
}
