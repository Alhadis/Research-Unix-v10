/*
 * b.c - new bouncing line
 */

#include <jerq.h>
#define N 25
Point foo[] = {
	312, 799,
	650, 650,
	7, -11
	-5, 3,
	13, 7,
	3, 11,
};

main()
{
	register i = 0, j;
	Point from[N], to[N];
	Point dfrom, dto;
	request(KBD);
	rectf(&display, Drect, F_XOR);
	for(j=0; j<N; j++){
		from[j] = Drect.origin;
		to[j] = Drect.origin;
	}
/*	from[0] = foo[0];
	from[1] = foo[0];
	to[0] = foo[1];
	to[1] = foo[1];*/
	dfrom = foo[2];
	dto = foo[3];
	for (; kbdchar()==-1; wait(CPU)){
		j = i;
		if (++i >= N)
			i = 0;
		segment(&display, from[i], to[i], F_XOR);
		from[i] = from[j];
		bump(&from[i], &dfrom);
		to[i] = to[j];
		bump(&to[i], &dto);
		segment(&display, from[i], to[i], F_XOR);
	}
	exit();
}

bump(p,dp)
register Point *p, *dp;
{
	if ((p->x += dp->x) > Drect.corner.x || p->x < Drect.origin.x) {
		dp->x = -dp->x;
		p->x += dp->x << 1;
	}
	if ((p->y += dp->y) > Drect.corner.y || p->y < Drect.origin.y) {
		dp->y = -dp->y;
		p->y += dp->y << 1;
	}
}
