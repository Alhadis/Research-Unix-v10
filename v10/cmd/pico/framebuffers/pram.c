#include <stdio.h>
#include <signal.h>
#include "pico.h"

extern short	CURSCRATCH, CUROLD;
extern struct	SRC src[MANY];
extern void	onquit();
extern int	DEF_LL, DEF_NL;

ramputitall(k)
{
	register unsigned char *pr, *pg, *pb;
	struct SRC *from = &src[k];
	register int j, ex = from->sx + from->ex;
	int i, offset, delta, sx = from->sx;
	int sy = from->sy, ey = from->sy + from->ey;
	short w[1024];		/* avoid malloc */

	offset = sy*DEF_LL + sx;
	delta  = DEF_LL - ex + sx;

	pr = from->pixred+offset;
	pg = from->pixgrn+offset;
	pb = from->pixblu+offset;
	signal(SIGINT, SIG_IGN);	/* don't mess up the ramtek */
	switch (from->nchan)
	{	case 1:
			for (i = sy; i < ey; i++, pr += delta)
			{	for (j = sx; j < ex; j++)
					w[j] = *pr++;
				wi(w, ex-sx, 1, sx, i);
			}
			break;
		case 4:
		case 3:
			for (i = sy; i < ey; i++, pr+=delta, pg+=delta, pb+=delta)
			{	for (j = sx; j < ex; j++)
				w[j] =  ((*pr++ & 224)<<1)|
					((*pg++ & 224)>>2)|
					((*pb++ & 224)>>5);
				wi(w, ex-sx, 1, sx, i);
			}
			break;
		default:
			fprintf(stderr, "undefined number of channels\n");
			break;
	}
	signal(SIGINT, onquit);
}

ramlamcl()
{	int cm[512], a, b, c, i;

	for (i = 0; i < 512; i++)
	{	a = (i & (7<<6)) << 3;
		b = (i & (7<<3)) << 2;
		c = (i & (7   )) << 1;
		cm[i] = a | b | c;
	}
	signal(SIGINT, SIG_IGN);
	lam(512, cm);
	signal(SIGINT, onquit);
	reloadmaps();
}

ramlambwram()
{	int cm[512], i;

	for (i = 0; i < 512; i++)
		cm[i] = i;
	signal(SIGINT, SIG_IGN);
	lam(512, cm);
	signal(SIGINT, onquit);
	reloadmaps();
}

ramgetitall(from)
	struct SRC *from;
{
	register unsigned char *pr, *pg, *pb;
	register int j, ex = from->sx + from->ex;
	int i, offset, delta, sx = from->sx;
	int sy = from->sy, ey = from->sy + from->ey;
	short w[1024];	/* avoid malloc */

	if (from == Old)
		fprintf(stderr, "reading screen...");

	offset = sy*DEF_LL + sx;
	delta  = DEF_LL - ex + sx;

	pr = from->pixred+offset;
	pg = from->pixgrn+offset;
	pb = from->pixblu+offset;

	signal(SIGINT, SIG_IGN);	/* don't mess up the ramtek */
	switch (from->nchan)
	{	case 1:
			for (i = sy; i < ey; i++, pr+=delta, pg+=delta, pb+=delta)
			{	ri(0, i, DEF_LL, w);
				for (j = sx; j < ex; j++)
				{	*pg = *pb = *pr = w[j];
					pg++; pb++; pr++;
				}
			}
			break;
		case 4:
		case 3:
			for (i = sy; i < ey; i++, pr+=delta, pg+=delta, pb+=delta)
			{	ri(0, i, DEF_LL, w);
				for (j = sx; j < ex; j++)
				{	*pr++ = ((w[j]>>6)&7)<<5;
					*pg++ = ((w[j]>>3)&7)<<5;
					*pb++ = ((w[j]   )&7)<<5;
			}	}
			break;
		default:
			fprintf(stderr, "undefined number of channels\n");
			break;
	}
	signal(SIGINT, onquit);
	if (from == Old)
		fprintf(stderr, "\n");
	RESIDENT;
}

/* a few extra builtins:
 */

ramlyank(x, y)
{	register int i;
	register unsigned char *pr, *pg, *pb;
	extern char faster;
	short w[1024];		/* avoid malloc */
	int from=Old->sx, to=Old->ex;

	if (y < Old->sy || y >= Old->ey) return 0;

	if (!faster)
	{	ramyank(x, y);
		return 0;
	} else
	{	if (x != 0)
			return 0;
	}
	i = (y < 1 || y >= DEF_LL) ? 0 : DEF_LL*(y-1);
	pr = scratchred+i;
	pg = scratchgrn+i;
	pb = scratchblu+i;

	if (Old->nchan == 1)
		for (i = from; i < to; i++)
		w[i] = *pr++;
	else
		for (i = from; i < to; i++)
		w[i] = ((*pr++ & 224)<<1) | ((*pg++ & 224)>>2) | (*pb++>>5);

	signal(SIGINT, SIG_IGN);
	wi(&w[from], to - from, 1, from, y-1);
	signal(SIGINT, onquit);

	return 1;
}

ramyank(x, y)
{	register int i = DEF_LL*y+x;
	unsigned char *pr = scratchred+i;
	unsigned char *pg = scratchgrn+i;
	unsigned char *pb = scratchblu+i;
	short ch;

	if (Old->nchan == 1)
		ch = *pr;
	else
		ch = ((*pr & 224)<<1)|((*pg & 224)>>2)|(*pb >>5);
	wii(&ch, x, y);
	return 1;
}

unsigned char Rmap[256], Gmap[256], Bmap[256];

reloadmaps()
{	register int i;
	for (i = 0; i < 256; i++)
		Rmap[i]=Gmap[i]=Bmap[i]=i;
}

ramredcmap(i, z)
{	int j = i%256;
	int k = z%256;

	Rmap[j] = k;
	if (j == 255) docmap();
	return k;
}

ramgrncmap(i, z)
{	int j = i%256;
	int k = z%256;

	Gmap[j] = k;
	if (j == 255) docmap();
	return k;
}

ramblucmap(i, z)
{	int j = i%256;
	int k = z%256;

	Bmap[j] = k;
	if (j == 255) docmap();
	return k;
}

docmap()
{	int i, cm[512], a, b, c;

	if (Old->nchan == 1)
	{	for (i = 0; i < 256; i++)
			cm[i] = Rmap[i];
	} else
	{	for (i = 0; i < 512; i++)
		{	a = ((Rmap[(i&(7<<6))>>1])&240)<<4;
			b = ((Gmap[(i&(7<<3))<<2])&240);
			c = ((Bmap[(i&(7   ))<<5])&240)>>4;
			cm[i] = a | b | c;
		}
	}
	signal(SIGINT, SIG_IGN);
	lam(512, cm);
	signal(SIGINT, onquit);
}

ramzap()
{
	fill(0, Old->sx, Old->sy, Old->ex, Old->ey);
}

ramgetcmap(i, r,g,b)
{	int h = 0, k = 0;

	if (r) { h++; k += Rmap[i]; }
	if (g) { h++; k += Gmap[i]; }
	if (b) { h++; k += Bmap[i]; }
	if (h > 1) k /= h;

	return k;
}
