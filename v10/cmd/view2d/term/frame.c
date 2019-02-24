#include	<CC/jerq.h>
#include	"frame.pri"
#include	"camera.pub"

Hcoord direct(short, short, short);

#define	Z(x,y)	data[(x)+m->nx*(y)]
#define	SEG(x1,y1,x2,y2)	{if((Z(x1,y1)>m->floor)&&(Z(x2,y2)>m->floor))seg(x1,y1,x2,y2);}

void
Frame.view(short vlat, short vlon, short vdist, short valat, short valon, short vpersp)
{
	register i;

	if((vlat != lat) || (vlon != lon) || (vdist != dist) || (valat != alat)
		|| (valon != alon) || (vpersp != persp))
	{
		lat = vlat;
		lon = vlon;
		dist = vdist;
		alat = valat;
		alon = valon;
		persp = vpersp;
		draw();
	}
	show();
}

void
Frame.draw()
{
	register x, y;

	transdata();
	rectf(b, b->rect, F_CLR);
	for(y = 0; y < m->ny; y++)
	{
		for(x = 0; x < m->nx; x++)
		{
			if(y) SEG(x, y-1, x, y);
			if(x) SEG(x-1, y, x, y);
		}
		wait(CPU);
	}
}

void
Frame.show()
{
	bitblt(b, b->rect, &display, m->ctr, F_STORE);
}

void
Frame.rd(int n)
{
	register short *s;
	extern int getn();

	alon = -1;
	gets(time);
	s = data = (short *)alloc(n*2);
	while(n--)
		*s++ = getn();
	b = balloc(m->bound);
}
/*
	the data will always fall in the cube 1000*1000*1000
	centered at the origin
*/

#define		SCALE		1000

void
Frame.transdata()
{
	register x, y;
	register Hcoord *h;
	short *d;
	int scaly;
	int scale = max(m->nx, m->ny)-1;
	Hcoord e = direct(lat, lon, dist*SCALE/100);
	Hcoord l = direct(alat, alon, ONE*2);
	extern long vscale;

	init3(b, 1, 1);
	look3(e, l, Hcoord((l.x == 0) && (l.y == 0), 0, 1, 1));
	clip3scale = (1-persp)*SCALE;
	scaly = muldiv(SCALE/2, vscale, 32768);
	h = (Hcoord *)m->tdata;
	for(y = 0, d = data; y < m->ny; y++)
		for(x = 0; x < m->nx; x++, h++)
		{
			*h = Hcoord(muldiv(x, SCALE, scale)-SCALE/2,
				muldiv(y, SCALE, scale)-SCALE/2,
				muldiv(*d++, scaly, 32768), 1);
		}
}

void
Frame.seg(short x1, short y1, short x2, short y2)
{
	register Hcoord *h = (Hcoord *)m->tdata;

	move3(h[x1+y1*m->nx]);
	line3(h[x2+y2*m->nx]);
}
