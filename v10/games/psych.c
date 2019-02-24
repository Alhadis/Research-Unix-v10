/*
	psych - draw pretty pictures on 4014

	compile as cc psych.c -lm -l4014
		could use other plot-type library

*/
#include "stdio.h"

char	obuf[BUFSIZ];

main() {
	int c;
	float g, fact, d, z, b;
	float x, y, limit = 0.25;
	int xx, yy;
	double sin(), cos();
	int i, first;
	long time(), t;

	setbuf(stdout, obuf);
	time(&t);
	srand(t&0177777);
  top:
	g = ((float)rand()/32767.) * 10;
	fact = ((float)rand()/32767.) * 20;
	erase();
	d = 0.1;
	z = 0;
	first = 0;
	for (i = 1; i <= 500; i++) {
		b = (z-110) / 1.57;
		z += g;
		g = (0-g) * fact;
		fact = 1 / fact;
		d *= 1.01;
		x = d * cos(b);
		y = d * sin(b);
		if (x*x > limit || y*y > limit)
			break;
		xx = (x+0.5) * 3120 + 500;
		yy = (y+0.5) * 3120;
		if (first == 0)
			move(xx, yy);
		cont(xx, yy);
		first = 1;
	}
	printf("\015");	/* out of plot mode */
	fflush(stdout);
	while ((c=getchar()) != '\n' && c!=EOF)
		;
	if(c == '\n')
		goto top;
}
