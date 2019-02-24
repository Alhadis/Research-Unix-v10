#include <jerq.h>
#include "ship.h"
#include "rock.h"
extern Bitmap *buildimage();
extern Rock *collision();
extern death;
extern notdrawn;

Bitmap *ship;

char nothrust[]={
	'R', -20, -20, 21, 21,
	'm', 20, 0, 	/* must be tip of ship, where bombs go from */
	'l',  -11, 7,
	'l',  -11, -7,
	'l',  20, 0,
	0,
};
char thrust[]={
	'R', -20, -20, 21, 21,
	'm',  20, 0,
	'l',  -11, 7,
	'l',  -11, -7,
	'l',  20, 0,
	'm',  -11, 5,
	'l',  -20, 0,
	'l',  -11, -5,
	0
};
char shipim[sizeof thrust];	/* rotated into position */
int	angle=270;	/* up, actually */
int	costheta=0, sintheta= -1024;
int	oldangle;
int	thrusting;
int	oldthrusting;
Point	pos={XMAX/2, YMAX/2};
Point	oldpos={XMAX/2, YMAX/2};
int	reloading;
Point	vel;
Point	resid;
shipfree()
{
	bfree(ship);
}
initship()
{
	pos.x=XMAX/2;
	pos.y=YMAX/2;
	oldpos=pos;
	vel.x=0;
	vel.y=0;
	angle=270;
	thrusting=0;
	oldangle=angle;
	costheta=cos(angle);
	sintheta=sin(angle);
	oldthrusting=0;
	reloading=0;
	resid=vel;
	genship(nothrust);
	drawship();
}
genship(p)
	register char *p;
{
	register char *q=shipim;
	register i;
	bfree(ship);	/* safe if ship==0 */
	for(i=0; i<5; i++)
		*q++ = *p++;	/* copy bounding rectangle */
	while(*q++ = *p++){	/* copy opcode */
		*q++=m(sext(p[0]), costheta)-m(sext(p[1]), sintheta);
		*q++=m(sext(p[1]), costheta)+m(sext(p[0]), sintheta);
		p+=2;
	}
	ship=buildimage(shipim);
}
quickcheck()
{	register but=mouse.buttons&7;	/* sleazy, but avoid button1, etc. */
	if(death)
		return 0;
	if(computer && curthreat>=0)
		strat(), but = compbut;
	if((but&(4|1))==(4|1))	/* button1() && button3() ==> thrust */
		thrusting=1;
	else{
		thrusting=0;
		if(but&(4|1)){
			if(but&4){	/* button1() ==> turn left */
				angle-=DTHETA;
				if(angle < 0)
					angle += 360;
			}else{
				angle+=DTHETA;
				if(angle > 360)
					angle -= 360;
			}
			costheta=cos(angle);
			sintheta=sin(angle);
		}
	}
	if(!reloading && (but&2)){
		shoot();
		reloading=RELOAD;
	}
	if(reloading>0)
		--reloading;
	if(thrusting){
		vel.x+=m(THRUST, costheta);
		vel.y+=m(THRUST, sintheta);
	}
	pos.x+=(vel.x+resid.x)/VSCALE;
	pos.y+=(vel.y+resid.y)/VSCALE;
	resid.x=(resid.x+vel.x)&(VSCALE-1);
	resid.y=(resid.y+vel.y)&(VSCALE-1);
	onscreen(&pos);
	if(collision(pos, hash(pos), RADSHIP)){
		allover(pos);
		return 0;
	}else
		return 1;
}
shoot()
{
	/* VSCALE/3 reflects the difference in updates between bombs and ship */
	startbomb(add(pos, Pt(sext(shipim[6]), sext(shipim[7]))), div(vel, VSCALE/3), angle);
}
longcheck()
{
	if(death)
		return 0;
	drawship();
	if(angle!=oldangle || thrusting!=oldthrusting){
		genship(thrusting? thrust : nothrust);
		notdrawn-=2;
	}
	oldpos=pos;
	oldangle=angle;
	oldthrusting=thrusting;
	drawship();
	if(vel.x > C)
		vel.x = C;
	if(vel.y > C)
		vel.y = C;
	if(vel.x < -C)
		vel.x = -C;
	if(vel.y < -C)
		vel.y = -C;
	return 1;
}
drawship()
{
	bitblt(ship, ship->rect, &display, add(transform(oldpos), ship->rect.origin), F_XOR);
	if(notdrawn>-20)
		--notdrawn;
}
