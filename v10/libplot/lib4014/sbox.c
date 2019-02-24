#include <stdio.h>
#include "tek.h"
sbox(x1, y1, x2, y2)
double	x1, y1, x2, y2;
{
	double ff[8], *fp[1];
	int i[2];
	ff[0] = ff[6] = x1;
	ff[1] = ff[3] = y1;
	ff[2] = ff[4] = x2;
	ff[5] = ff[7] = y2;
	i[0] = 4;
	i[1] = 0;
	fp[0] = ff;
	internal = 1;
	fill(i, fp);
	internal = 0;
}
