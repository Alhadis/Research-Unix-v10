#include <stdio.h>
poly(num, ff)
int num[];
double *ff[];
{
	register double *xp, *yp;
	double **fp;
	int i, *n;
	n = num;
	fp = ff;
	while((i = *n++)){
		xp = *fp++;
		yp = xp+1;
		move(*xp, *yp);
		while(--i){
			xp += 2;
			yp += 2;
			vec(*xp, *yp);
		}
	}
}
