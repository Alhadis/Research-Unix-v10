#include <jerq.h>
Point spot, v, a;
int Topx, Topy;
int Botx, Boty;
Bitmap *ball;
int	R;
long	R2;
main()
{
	request(KBD);
	rectf(&display, Drect, F_XOR);
	Topx=10*Drect.corner.x-320;
	Topy=10*Drect.corner.y-320;
	Botx=10*Drect.origin.x+320;
	Boty=10*Drect.origin.y+320;
	a.x = 0;
	a.y = 1;
	spot.x = (Botx+Topx)/2;	
	spot.y = Boty;
	R=32;
	R2=R*(long)R;
	ball=balloc(Rect(0, 0, 2*R+1, 2*R+1));
	rectf(ball,ball->rect,F_STORE);
	sphere();
	drawball();
	v.x=5;
	v.y=0;
	while(kbdchar()==-1){
		drawball();
		v = add(v, a);
		spot = add(spot, v);
		if(spot.x >= Topx) {
			spot.x = 2*Topx - spot.x;
			v.x = -v.x;
		}
		else if(spot.x <= Botx) {
			spot.x = 2*Botx - spot.x;
			v.x = -v.x;
		}
		if(spot.y >= Topy) {
			spot.y = 2*Topy - spot.y;
			v.y = -v.y;
		}
		else if(spot.y <= Boty) {
			spot.y = 2*Boty - spot.y;
			v.y = -v.y;
		}
		drawball();
		sleep(4);
	}
	bfree(ball);
	exit();
}
drawball()
{	Point p;
	p = sub(div(spot, 10), Pt(R, R));
	bitblt(ball, ball->rect, &display, p, F_XOR);
}
int	illum[3]={5, 4, 3};	/* |illum|=7 (pretty close) */
int	view[3]={1, 0, 0};
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
sphere(){
	register x, y, z;
	register d;
	register I;
	rectf(ball, ball->rect, F_XOR);
	for(y= -R; y<=R; y++)	/* y across, pos to right */
		for(z= -R; z<=R; z++){	/* z pos up */
			if(z*z+y*y>R2)
				continue;
			x=sqrtryz(R, y, z);
			/* I=(illum.r)(view.r) */
			I=muldiv(x,	/* view.r */
			   (x*5+y*4+z*3), /* illum.r */
			    87);	/* a scale factor: ~RLV */
			if(I<=0)	/* unilluminated crescent */
				continue;
			if(I>dith[y&DITHMASK][z&DITHMASK])
				point(ball, Pt(y+R, -z+R), F_XOR);
		}
}
