#include	<jerq.h>
#include	<font.h>
#include	<msgs.h>

#define UPD	4
#define LSIZE	256

struct Bitmap *files;
Rectangle dr;	/* screen coordinates */
Rectangle sr;	/* local  coordinates */
Point pt;

extern char *genfn();
Menu menu = { (char **)0, genfn };

main()
{	int x, y; Word *w;
	unsigned char *s;

	request(RCV|MOUSE);
	files = balloc(Rect(0, 0, 512+8, 512+8));
	dr = inset(Drect, 4);
	sr.origin.x = 0;
	sr.corner.x = LSIZE;
	pt.x = pt.y = 0;
	standard();
	hmm();
	for (;;)
	{	while (!(own()&RCV)) wait(CPU);
		pt.y = getn();
		w = addr(files, pt);
		s = (unsigned char *) w;
		for(x = 0; x < LSIZE/8; x++) *s++ = (unsigned char) get();
		if ((++pt.y)%UPD == 0)
		{	reshaped();
			Blast((pt.y > UPD)?(pt.y - UPD): 0, pt.y);
		}
		if (pt.y == LSIZE) pt.y = 0;
		wait(CPU);
	}
}

Blast(n, m)
{
	sr.origin.y = n;
	sr.corner.y = m;

	if (rectclip(&sr, rsubp(dr, dr.origin)))
	bitblt(files, sr, &display, add(dr.origin, sr.origin), F_STORE);
}

get()
{	int k;

	while ((k = rcvchar()) == -1)
	{	wait(CPU);
		reshaped();
	}
	return (k);
}

char *
genfn(i)
{	char buf[64];

	switch(i)
	{
	case 0:	strcpy(buf, "reset"); break;
	case 1: strcpy(buf, "512x512"); break;
	default: return (char *)0;
	}
	return (buf);
}

standard()
{	Rectangle r;
	register struct Layer *l;

	r.origin = P->layer->rect.origin;
	r.corner = add(P->layer->rect.origin, Pt(LSIZE+8, LSIZE+8));
	if (r.corner.x >= (XMAX-5)) {
		r.origin.x -= r.corner.x - (XMAX-5);
		r.corner.x = (XMAX-5);
	}
	if (r.corner.y >= (YMAX-5)) {
		r.origin.y -= r.corner.y - (YMAX-5);
		r.corner.y = (YMAX-5);
	}
	dellayer(P->layer);
	l = newlayer(r);
	if (l == 0) {
		r.corner = add(r.origin, Pt(100,50));
		l = newlayer(r);
		if (l == 0) exit();	/* The Oh Shit case */
	}
	P->layer = l;
	P->rect = r;
	rectf(P->layer, r, F_STORE);
	P->rect = inset(r, 4);
	P->state |= RESHAPED;
	muxnewwind(P, C_RESHAPE);

	Drect = r;
	dr = inset(Drect, 4);
	sr.origin.x = 0;
	sr.corner.x = LSIZE;
	Blast(0, LSIZE);
}

reshaped()
{
	if (P->state & (RESHAPED|MOVED))
	{	P->state &= ~(RESHAPED|MOVED);
		dr = inset(Drect, 4);
		sr.origin.x = 0;
		sr.corner.x = LSIZE;
		Blast(0, LSIZE);
	}
	if ((own()&MOUSE) && button3())
	{	switch (menuhit(&menu, 3))
		{
		case 0: pt.y = 0; rectf(P->layer, dr, F_STORE); break;
		case 1: standard(); break;
		default: break;	
	}	}
}

hmm()
{	while (!(own()&RCV)) wait(CPU);
	get();	/* get rid of one initial fantom character */
}

getn()
{	int n = (get()&255);

	n <<= 8;
	n |= (get()&255);
	return n;
}
