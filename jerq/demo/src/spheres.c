#include <jerq.h>
#define	R	50
#define	R2	(R*R)

Rectangle sphererect={
	0, 0, 2*R, 2*R
};
main(){
	register i, j;
	rectf(&display, Drect, F_XOR);	/* WonB() */
	cursinhibit();
	sphere();
	for(i=0; i<(XMAX/(2*R)); i++){
		for(j=0; j<(YMAX/(2*R)); j++){
			if(i==0 && j==0)
				continue;
			bitblt(&display, sphererect, &display,
				Pt(2*R*i, 2*R*j), F_STORE);
		}
	}
	for(;;){
		sleep(60);
	}
}
introutine(){
	if(button123())
		exit(0);
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
	for(y= -R; y<=R; y++)	/* y across, pos to right */
		for(z= -R; z<=R; z++){	/* z pos up */
			if(z*z+y*y>R2)
				continue;
			x=sqrtryz(R, y, z);
			/* I=(illum.r)(view.r) */
			I=muldiv(x,	/* view.r */
			   (x*5+y*4+z*3), /* illum.r */
			   (850*R2)/10000);	/* a scale factor: RLV */
			if(I<=0)	/* unilluminated crescent */
				continue;
			if(I>dith[y&DITHMASK][z&DITHMASK])
				jpoint(Pt(y+R, -z+R), F_XOR);
		}
}
