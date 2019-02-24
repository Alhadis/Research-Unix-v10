# include	"trek.h"

/**
 **	photon torpedo control
 **
 **/


torped()
{
	register int		ix, iy;
	double			x, y, dx, dy;
	double			angle;
	int			course, course2, delta;
	int			burst, spread;
	int			n;

	if (Status.cloaked)
	{
		printf("Federation regulations do not permit attack while cloaked.\n");
		return;
	}
	if (Damage[TORPED])
	{
		printf("Photon tubes inoperable.\n");
		return;
	}
	if (Status.torped <= 0)
	{
		printf("All photon torpedos expended.\n");
		return;
	}
	if(getintpar("Torpedo course", &course)==0) return;
	fixco(&course);
	if(lineended() && Status.torped<3) {
		burst=0;
		printf("No burst mode selected.\n");
	} else {
		if(!digit(peekchar())) {
			if((burst=getynpar("Do you want a burst"))<0) return;
		} else
			burst=1;
		if (burst) {
			if(getintpar("Burst angle", &spread)==0) return;
			if (spread < 0 || spread > 15) {
				printf("Angle out of range.\n");
				return;
			}
			if(Status.torped<3) {
				printf("Not enough left for a burst.\n");
				burst=spread=0;
			}
			else {
				burst = 2;
				course =- spread;
			}
		}
	}
	for (n=0; n<=burst; n++) {
		delta = randcourse();
		course2 = course + delta;
		angle = course2 * 0.0174532925;			/* convert to radians */
		dx = -cos(angle);
		dy =  sin(angle);
		x = fabs(dx); y = fabs(dy);
		x = (x>y ? x : y);
		dx =/ x; dy =/ x;
		x = Sectx;
		y = Secty;
		if (Status.cond != DOCKED)
			Status.torped =- 1;
		if(burst)
			printf("Torpedo %d: ", n);
		printf("[%d] ", course2);
		printf("track");
		while (1)
		{
			ix = (x =+ dx)+0.5;
			iy = (y =+ dy)+0.5;
			if (x <= -0.5 || ix >= NSECTS || y <= -0.5 || iy >= NSECTS) {
				printf(" MISSED\n");
				break;
			}
			printf(" %d,%d", ix, iy);
			switch (Sect[ix][iy])
			{
			  case EMPTY:
				continue;
	
			  case KLINGON:
				printf("\n");
				hitkling(ix, iy, 500);
				break;
	
			  case STAR:
				printf("\n");
				nova(ix, iy);
				break;
	
			  case INHABIT:
				printf("\n");
				kills(ix, iy, -1);
				break;
	
			  case BASE:
				printf("\n");
				killb(Quadx, Quady); Game.killb++;
				break;
			  case BLACKHOLE:
				printf(" ...\n");
				break;
			}
			break;
		}
		if (Damage[TORPED]) {
			printf("Damages sustained");
			if (burst)
				printf("; remainder of burst aborted");
			printf("\n");
			break;
		}
		course =+ spread;
	}
	Move.free = 0;
}


randcourse()
{
	double			r;
	register int		d;

	d = ((franf() + franf()) - 1.0) * 20;
	if (abs(d) > 12)
	{
		printf("Photon tubes misfire.\n");
		if (!ranf(3))
		{
			damage(TORPED, 0.2 * abs(d));
		}
		d =* 1.0 + 2.0 * franf();
	}
	if (Status.shldup || Status.cond == DOCKED)
	{
		r = Status.shield;
		r = 1.0 + r / Initial.shield;
		if (Status.cond == DOCKED)
			r = 2.0;
		d =* r;
	}
	return (d);
}

hitkling(ix, iy, hit)
int		ix, iy, hit;
{
	register int	k;

	for (k = 0; k < Nkling; k++)
	{
		if (Kling[k].x != ix || Kling[k].y != iy)
			continue;
		Kling[k].power =- hit + ranf(hit+1);
		if (Kling[k].power > 0)
		{
			printf("*** Klingon hit at %d,%d: extensive damages.\n",
				ix, iy);
		} else
			killk(ix, iy);
		break;
	}
}
