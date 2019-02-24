#include <stdio.h>
#include "tek.h"
int	oloy = -1;
int	ohiy = -1;
int	ohix = -1;
int	oextra = -1;

vec(xx, yy) 
double	xx, yy; 
{
	int	x, y, hix, hiy, lox, loy, extra;
	int	n;
	e1->copyx = xx; 
	e1->copyy = yy;
	x = SCX(xx);
	y = SCY(yy);
	hix = (x >> 7) & 037;
	hiy = (y >> 7) & 037;
	lox = (x >> 2) & 037;
	loy = (y >> 2) & 037;
	extra = (x & 03) + ((y << 2) & 014);
	n = (abs(hix - ohix) + abs(hiy - ohiy) + 6) / 12;
	if (hiy != ohiy) {
		putchar(hiy | 040);
		ohiy = hiy;
	}
	if (hix != ohix) {
		if (extra != oextra) {
			putchar(extra | 0140);
			oextra = extra;
		}
		putchar(loy | 0140);
		putchar(hix | 040);
		ohix = hix;
		oloy = loy;
	} else {
		if (extra != oextra) {
			putchar(extra | 0140);
			putchar(loy | 0140);
			oextra = extra;
			oloy = loy;
		} else if (loy != oloy) {
			putchar(loy | 0140);
			oloy = loy;
		}
	}
	putchar(lox | 0100);
	while (n--)
		putchar(0);
}
