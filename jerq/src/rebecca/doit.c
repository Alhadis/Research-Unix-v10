#include	<jerq.h>
#include	"textures.h"
#include	"rebecca.h"
#include	"instruments.h"

extern Rectangle Image, dr;
extern unsigned char *photo[SZ];
extern short have[SZ], l_side[SZ], r_side[SZ];
extern int res, magn;
extern int Gity, Gitres, Gitmag, Sem0;

int xoff=0, yoff=0, gridon=0, Sem4=1, Sem8=0;
int sos=0, eos=255, minres=512;
Bitmap *image;

startdoit()
{	image = balloc(Rect(0, 0, SZ, SZ+80));
	if (!image) return 0;
	return 1;
}

#define ineed(sx,ex,yy)	((omagn && (sx < l_side[yy] || ex > r_side[yy])) || \
			 (!omagn && have[yy] > curres))

doit()
{	Point pt, tp;
	Rectangle r;
	int curres=res;
	int omagn = magn;
	int dx, dy=yoff, ox=xoff, ax, sem8=Sem8;
	extern int *ALIVE, QUIT;

	ax = ox + SZ/curres;
	if (!Sem4) return;
	if (!omagn && curres < minres) minres = curres;
	tp.x = tp.y = curres;
	Sem4=0;
	drawruler();
	for (pt.y = 0; pt.y < SZ; pt.y += curres, dy += (omagn)?1:curres)
	{	if (ineed(ox, ax, dy))
		{	Gity=dy; Gitres=curres; Gitmag = omagn;
			Psem(&Sem0);
		} else
		{	if (sem8) continue;
		}
		dx = ox;
		for (pt.x = 0; pt.x < SZ; pt.x += curres, dx += (omagn)?1:curres)
		{	r.origin = pt;
			r.corner = add(pt, tp);
			r.corner.y = (r.corner.y > SZ)?SZ:r.corner.y;
			if (dy >= SZ)
			{	r.corner.y = SZ;
				r.corner.x = SZ;
				burp(0, r, SHOW);
				goto done;
			} else
			burp((dx<SZ)?photo[dy][dx]:0,  r, BLIND);
			if (Sem4 || !OK) goto done;
		}
		Blast(pt.y, pt.y+curres);
		if (Sem4 || !OK) goto done;
		wait(CPU);
	}
done:	if (OK) imagekader();
	if (gridon) grid(curres);
	Sem8=0;
	return;
}

makeimage(pt)
	Point pt;
{
	Image.origin = pt;
	Image.corner = add(pt, Pt(SZ+2, SZ+2));
	imagekader();
}

imagekader()
{	Rectangle box;

	box = realbox(Image);
	outline(box, F_STORE);
}

#define MAXGREY	255

burp(val, r, how)
	Rectangle r;
{
	Texture *t;

	if (val <= sos)
		t = Grey[0];
	else if (val >= eos)
		t = Grey[MAXGREY];
	else
		t = Grey[((val-sos)*(MAXGREY+1))/(eos-sos)];
	texture(image, r, t, F_STORE);

	if (how) blast(r);
}

Blast(n, m)
{	Rectangle sr;

	sr = image->rect;	/* local  coordinates */
	sr.origin.y = n;
	sr.corner.y = m;
	blast(sr);
}

blast(r)
	Rectangle r;
{
	Rectangle sr; sr = r;

	if (!OK) return;

	if (rectclip(&sr, rsubp(dr, dr.origin)))
	bitblt(image, sr, &display, add(dr.origin, sr.origin), F_STORE);
}

update()
{	int i;
	for (i = 0; i < SZ; i++)
		Blast(i, i+1);
	imagekader();
}
