/*
 * moire.c - new bouncing line
 */

#include <jerq.h>
#define N 150
bump();
Point foo[] = {
	312, 799,
	650, 650,
	400,400,
	3,-2,
	-4, 3,
	2, 1,
};

main()
{
	register i = 0, j;
	Point from[N], to[N], mid[N];
	Point dfrom, dto, dmid;
	cursinhibit();
	request(KBD|CPU);
	rectf(&display,display.rect,F_XOR);
	for(j=0; j<N; j++){
		from[j]=Drect.origin;
		to[j]=Drect.origin;
		mid[j]=Drect.origin;
	}
	dfrom = foo[3];
	dto = foo[4];
	dmid = foo[5];
	for (; kbdchar()==-1; wait(CPU)){
		j = i;
		if (++i >= N)
			i = 0;
		segment(&display, from[i], to[i], F_XOR);
		segment(&display, to[i], mid[i], F_XOR);
		segment(&display, mid[i], from[i], F_XOR);
		from[i] = from[j];
		bump(&from[i], &dfrom);
		to[i] = to[j];
		bump(&to[i], &dto);
		mid[i] = mid[j];
		bump(&mid[i], &dmid);
		segment(&display, from[i], to[i], F_XOR);
		segment(&display, to[i], mid[i], F_XOR);
		segment(&display, mid[i], from[i], F_XOR);
	}
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
introutine(){
	if(button3())
		exit(0);
}
