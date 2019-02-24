
/*
 * Simulate the famous weird program
 */
#include	<jerq.h>

Bitmap	*ball;
struct balls	{
	int	x, y;
	int	dx, dy;
} b[100];

struct	balls	*maxball;
#define	MAXV	9

main()
{      
	register xmax, ymax;
	register struct balls *bp;
	register xbase, ybase;
	register struct balls *vbp;
	long epoch = 500;
	long big = 1000;
	int nball;

	rectf(&display, display.rect, F_XOR);
	ball = balloc(Rect(0, 0, 16, 16));
	rectf(ball, ball->rect, F_CLR);
	circle(ball, Pt(8, 8), 6, F_XOR);
	xbase = Drect.origin.x;
	ybase = Drect.origin.y;
	xmax = Drect.corner.x - Drect.origin.x - 16;
	ymax = Drect.corner.y - Drect.origin.y - 16;
	request(MOUSE);
	srand(mouse.xy);
	nball = ((long)xmax*ymax) / 2000;
	if (nball > 100)
		nball = 100;
	nball = 10;		/* temp */
	maxball = &b[nball];
	for (bp = b; bp < maxball; bp++) {
	   retry:
		bp->dx = (rand() % MAXV - MAXV/2);
		if (bp->dx == 0)
			bp->dx = (rand() % MAXV - MAXV/2);
		bp->dy = (rand() % MAXV - MAXV/2);
		if (bp->dy == 0)
			bp->dy = (rand() % MAXV - MAXV/2);
		for (vbp = b; vbp < bp; vbp++)
			if (bp->dx==vbp->dx && bp->dy==vbp->dy)
				goto retry;
		bp->x = (xmax/2 + big*xmax - epoch*bp->dx) % xmax;
		bp->y = (ymax/2 + big*ymax - epoch*bp->dy) % ymax;
		bitblt(ball, ball->rect, &display,
		   Pt(bp->x+xbase, bp->y+ybase), F_XOR);
	}
	request(KBD);
	while (kbdchar() != 'q') {
		for (bp = b; bp < maxball; bp++) {
			bitblt(ball, ball->rect, &display,
			   Pt(bp->x+xbase, bp->y+ybase), F_XOR);
			if ((bp->x += bp->dx) < 0)
				bp->x += xmax;
			if (bp->x >= xmax)
				bp->x -= xmax;
			if ((bp->y += bp->dy) < 0)
				bp->y += ymax;
			if (bp->y >= ymax)
				bp->y -= ymax;
			bitblt(ball, ball->rect, &display,
			   Pt(bp->x+xbase, bp->y+ybase), F_XOR);
		}
		sleep(1);
	}
	free(ball);
}
