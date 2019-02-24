#include	<jerq.h>
#include	<font.h>
#define Edge		(defont.height)

Bitmap *frames[512];
int maxframe=0;
int nFrames=0;

char unwound[2];
Rectangle Dor;

main()
{	request(RCV|SEND|MOUSE|KBD);
	Dor = inset(Drect, Edge);
	unwound[0] = unwound[1] = 1;

	sleep(50);
	sendchar(103);
	nFrames = getn();
	run();
}

getframe()
{	int x, y;

	x = getn();
	y = getn();

	if (!(frames[maxframe] = balloc(Rect(0, 0, x, y))))
	{	spr(1, "                   out of mem");
		sendchar(101); for (;;) sleep(100);
		/* return 0; */
	}
	spr(1, "frame %   ", maxframe+1);
	sendchar(103);
	read(maxframe, x, y);

	return 1;
}

read(m, a, b)
{	long i = 0, j = 0, k, n;
	long z = 0;

	n = (long) ((a+31)&(~31))/8;	/* 32bit allignment */
	k = n*8;
	n *= b;
	while (n--)
	{	((char *)(frames[m]->base))[z++] = rerecv();
		if (++i >= k)
		{	i = 0;
			BB(maxframe, j, j+9);
			j += 8;
			wait(CPU);
		}
	}
}

Blast(x)
{	BB(x, 0, frames[x]->rect.corner.y);
}

BB(k, n, m)
{	Rectangle sr;

	if (P->state & (RESHAPED|MOVED))
	{	n = 0;
		Dor = inset(Drect, Edge);
		P->state &= ~(RESHAPED|MOVED);
	}
	sr = frames[k]->rect;		/* local  coordinates */
	sr.origin.y = n;
	sr.corner.y = m;

	if (rectclip(&sr, rsubp(Dor, Dor.origin)))
	bitblt(frames[k], sr, &display, add(Dor.origin, sr.origin), F_STORE);
}

winder(n) {}
