#include <stdio.h>
#include <math.h>
#include "tek.h"
#define pHEIGHT 3120.
#define pWIDTH  3120.
#define pSMALL    0.2
struct penvir E[9] = {
{ 0, 0,
0., 0., 1., 1., pWIDTH, pHEIGHT, 0., 0.,
pSMALL, 1., 31, 0., '`', '`'},
{ 0, 0,
0., 0., 1., 1., pWIDTH, pHEIGHT, 0., 0.,
pSMALL, 1., 31, 0., '`', '`'},
{ 0, 0,
0., 0., 1., 1., pWIDTH, pHEIGHT, 0., 0.,
pSMALL, 1., 31, 0., '`', '`'},
{ 0, 0,
0., 0., 1., 1., pWIDTH, pHEIGHT, 0., 0.,
pSMALL, 1., 31, 0., '`', '`'},
{ 0, 0,
0., 0., 1., 1., pWIDTH, pHEIGHT, 0., 0.,
pSMALL, 1., 31, 0., '`', '`'},
{ 0, 0,
0., 0., 1., 1., pWIDTH, pHEIGHT, 0., 0.,
pSMALL, 1., 31, 0., '`', '`'},
{ 0, 0,
0., 0., 1., 1., pWIDTH, pHEIGHT, 0., 0.,
pSMALL, 1., 31, 0., '`', '`'},
{ 0, 0,
0., 0., 1., 1., pWIDTH, pHEIGHT, 0., 0.,
pSMALL, 1., 31, 0., '`', '`'},
{ 0, 0,
0., 0., 1., 1., pWIDTH, pHEIGHT, 0., 0.,
pSMALL, 1., 31, 0., '`', '`'},
};
struct penvir *e0 = E, *e1 = &E[1];
struct penvir *esave;
int internal = 0;
char	bcolor(s)
char	*s;
{
	register char a;
	a = 0;
	while (*s != NULL) {
		switch (*s) {
		case 'l':
			if (!(strncmp(s, "long", 4)))
				a = 'd';
			break;
		case 'd':
			if (!(strncmp(s, "dotd", 4)))
				a = 'b';
			else if (!(strncmp(s, "dott", 4)))
				a = 'a';
			else a ='c';	/*dash default-short*/
			break;
		case 's':
			if (!(strncmp(s, "solid", 5)))
				a ='`';
			else if (!(strncmp(s, "short", 5)))
				a = 'c';
			break;
		case 'G':
			e1->pgap = atoi(s + 1);
			break;
		case 'A':
			e1->pslant = (180. - atof(s + 1)) / RADIAN;
			break;
		}
		while (*++s != NULL)
			if (*s == '/'){
				s++;
				break;
			}
	}
	return(a);
}
sscpy(a, b)
struct penvir *a, *b;
{ /* copy 'a' onto 'b' */
	b->left = a->left;
	b->bottom = a->bottom;
	b->xmin = a->xmin;
	b->ymin = a->ymin;
	b->scalex = a->scalex;
	b->scaley = a->scaley;
	b->sidex = a->sidex;
	b->sidey = a->sidey;
	b->copyx = a->copyx;
	b->copyy = a->copyy;
	b->quantum = a->quantum;
	b->grade = a->grade;
	b->ppink = a->ppink;
	b->pbrush = a->pbrush;
}
idle(){}
#define TRUNC(A) A>0.? A+0.5: A-0.5
normx(x)
double	x;
{
	double	xx;
	int	k;
	xx = SCX(x);
	k = TRUNC(xx);
	return(k);
}

normy(y)
double	y;
{
	double	yy;
	int	k;
	yy = SCY(y);
	k = TRUNC(yy);
	return(k);
}
ptype(){}
