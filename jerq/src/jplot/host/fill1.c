#include "jcom.h"
#include <stdio.h>
#include <math.h>
#include "jplot.h"
#define SEGS 256
fill(num,ff)
int num[];
double	*ff[];
{
	register struct seg *sp, *s1;
	register int x, y, X, Y;
	struct seg *in, *spend;
	int fx, fy, flag;
	int	so;
	double	  **fp;
	int ymin;
	int	M, *n;
	double	*xpp, *ypp;
	in = (struct seg *)malloc((unsigned)(SEGS*sizeof(struct seg)));
	spend = in + (SEGS-2);
	n = num;
	fp = ff;
	sp = in;
	ymin = 4000;
	while((M = *n++)){
		xpp = *fp++;
		ypp = xpp + 1;
		x = fx = normx(*xpp);
		y = fy = normy(*ypp);
		flag = 0;
		while(--M){
			xpp += 2;
			ypp += 2;
			X = normx(*xpp);
			Y = normy(*ypp);
	check:
			if ( (Y > y) || (Y == y && x < X)){
				sp->x = X;
				sp->y = Y;
				sp->X = x;
				sp->Y = y;
			} else {
				sp->x = x;
				sp->y = y;
				sp->X = X;
				sp->Y = Y;
			}
			x=X;
			y=Y;
			if(Y < ymin)
				ymin = Y;
			if ( sp->y == sp->Y) {
				if(sp > in){
				if ((sp-1)->y == (sp-1)->Y && (sp-1)->y == sp->y) {
					if (sp->x == (sp - 1)->X) {
						(sp - 1)->X = sp->X;
						sp--;
					} else if (sp->X == (sp - 1)->x) {
						(sp - 1)->x = sp->x;
						sp--;
					}
				}
				}
			}
			if(sp++ > spend){
				return;
			}
			if((M==1) && (flag == 0)){
				X = fx;
				Y = fy;
				flag = 1;
				goto check;
			}
		}
	}
	so = sp - in;
	graphic(FILL);
	intout(so);
	intout(ymin);
	for(s1=in;s1<sp;s1++){
		xy(s1->x, s1->y);
		xy(s1->X, s1->Y);
	}
	free(in);
	cmd_sent();
}
