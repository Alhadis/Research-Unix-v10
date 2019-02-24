#include <stdio.h>
#include <signal.h>
#include "pico.h"

#define lowbyte(A) (A)&0377
#define higbyte(A) ((A) >> 8) & 07

extern short	CURSCRATCH, CUROLD;
extern struct	SRC src[MANY];
extern void	onquit();
extern int	DEF_LL, DEF_NL;

metputitall(k)
{	struct SRC *from = &src[k];
	register unsigned char *pr, *pg, *pb, *w;
	register int i, j;
	int ex = from->sx + from->ex;
	int offset, delta, sx = from->sx;
	int sy = from->sy, ey = from->sy + from->ey;
	unsigned char mcode[16];
	unsigned char W[1024];
	extern int om_fd;

	mcode[0] = 0x4F;
	mcode[1] = 0377;	/* WRMASK */
	mcode[2]  = 0x52;	/* set P1 */
	mcode[7]  = 0x53;	/* set P2 */
	mcode[12] = 0;
	mcode[13] = 'o';
	mcode[3]  = lowbyte(sx);
	mcode[4]  = higbyte(sx);
	mcode[8]  = lowbyte(ex);
	mcode[9]  = higbyte(ex);

	offset = sy*DEF_LL + sx;
	delta  = DEF_LL - ex + sx;

	pr = from->pixred+offset;
	pg = from->pixgrn+offset;
	pb = from->pixblu+offset;

	switch (from->nchan)
	{	case 1:
			signal(SIGINT, SIG_IGN);
			for (i = sy; i < ey; i++, pr += (ex-sx)+delta)
			{	mcode[5] = mcode[10] = lowbyte(i);
				mcode[6] = mcode[11] = higbyte(i);
				write(om_fd, mcode, 14);
				write(om_fd, pr, ex-sx);
			}
			signal(SIGINT, onquit);
			break;
		case 3: for (i = sy; i < ey; i++, pr+=delta, pg+=delta, pb+=delta)
			{	mcode[5] = mcode[10] = lowbyte(i);
				mcode[6] = mcode[11] = higbyte(i);
				for (j = sx, w = W; j < ex; j++)
					*w++ =  ((*pr++ & 224))|
						((*pg++ & 224)>>3)|
						((*pb++ & 192)>>6);
				signal(SIGINT, SIG_IGN);
				write(om_fd, mcode, 14);
				write(om_fd, W, ex-sx);
				signal(SIGINT, onquit);
			}
			break;
		default:
			fprintf(stderr, "undefined number of channels\n");
			break;
	}
}

metgetitall(from)
	struct SRC *from;
{
	register unsigned char *pr, *pg, *pb, *w;
	register int j, ex = from->sx + from->ex;
	unsigned char W[1024];
	int i, offset, delta, sx = from->sx;
	int sy = from->sy, ey = from->sy + from->ey;

	if (from == Old)
		fprintf(stderr, "reading screen...");

	offset = sy*DEF_LL + sx;
	delta  = DEF_LL - ex + sx;

	pr = from->pixred+offset;
	pg = from->pixgrn+offset;
	pb = from->pixblu+offset;

	switch (from->nchan)
	{	case 1:
			for (i = sy; i < ey; i++, pr+=delta, pg+=delta, pb+=delta)
			{	metri(pr, 0, i, DEF_LL);
				for (j = sx; j < ex; j++)
				{	*pg++ = *pb++ = *pr++;
				}
			}
			break;
		case 3:
			for (i = sy; i < ey; i++, pr+=delta, pg+=delta, pb+=delta)
			{	metri(W, 0, i, DEF_LL);
				for (j = sx, w = W; j < ex; j++, w++)
				{	*pr++ = (*w&224);
					*pg++ = (*w& 28)<<3;
					*pb++ = (*w&  3)<<6;
			}	}
			break;
		default:
			fprintf(stderr, "undefined number of channels\n");
			break;
	}
	if (from == Old)
		fprintf(stderr, "\n");
	RESIDENT;
}

metlyank(x, y)
{	register int i;
	register unsigned char *pr, *pg, *pb, *w;
	unsigned char W[1024];
	extern char faster;
	int from=Old->sx, to=Old->ex;

	if (y < Old->sy || y >= Old->ey) return;

	if (!faster)
	{	metyank(x, y);
		return;
	} else
	{	if (x != 0)
			return;
	}
	i = (y < 1 || y >= DEF_LL) ? 0 : DEF_LL*(y-1);
	pr = scratchred+i;

	if (Old->nchan ==1)
		w = pr;
	else
	{	pg = scratchgrn+i;
		pb = scratchblu+i;
		for (i = from, w = W; i < to; i++)
			*w++ =  ((*pr++ & 224))|
				((*pg++ & 224)>>3)|
				((*pb++ & 192)>>6);
		w = W;
	}
	metwi(w, to-from, from, y);

	return 1;
}

metyank(x, y)
{	register int i = DEF_LL*y+x;
	unsigned char *pr = scratchred+i;
	unsigned char *pg = scratchgrn+i;
	unsigned char *pb = scratchblu+i;
	unsigned char w;

	if (Old->nchan == 1)
		w = *pr;
	else
		w = ((*pr & 224))| ((*pg & 224)>>3)| ((*pb & 192)>>6);

	metwi(&w, 1, x, y);

	return 1;
}

extern unsigned char Rmap[256], Gmap[256], Bmap[256];

metredcmap(i, z)
{	int j = i%256;
	int k = z%256;

	Rmap[j] = k;
	if (j == 255) dometcmap();
	return k;
}

metgrncmap(i, z)
{	int j = i%256;
	int k = z%256;

	Gmap[j] = k;
	if (j == 255) dometcmap();
	return k;
}

metblucmap(i, z)
{	int j = i%256;
	int k = z%256;

	Bmap[j] = k;
	if (j == 255) dometcmap();
	return k;
}

dometcmap()
{	struct scolor {
		char tag, loc;
		char red, grn, blu;
		char zero;
	} color[256];
	char b[8];
	register struct scolor *cp = color;
	register int i;
	extern int om_fd;

	if (Old->nchan == 1)
	for (i = 0; i < 256; i++, cp++)
	{	cp->tag = 'Q';
		cp->loc = i;
		cp->red = Rmap[i];
		cp->grn = Gmap[i];
		cp->blu = Bmap[i];
		cp->zero = 0;
	}
	else
	for (i = 0; i < 256; i++, cp++)
	{	cp->tag = 'Q';
		cp->loc = i;
		cp->red = (Rmap[i]&224) ? (Rmap[i]&224)    : 0;
		cp->grn = (Gmap[i]& 28) ? (Gmap[i]& 28)<<3 : 0;
		cp->blu = (Bmap[i]&  3) ? (Bmap[i]&  3)<<6 : 0;
		cp->zero = 0;
	}

	b[0] = 'O'; b[1] = 0377;
	signal(SIGINT, SIG_IGN);
	write(om_fd, b, 2);
	write(om_fd, color, sizeof(color));
	signal(SIGINT, onquit);
}

metgetcmap(i, r,g,b)
{	int h = 0, k = 0;

	if (r) { h++; k += Rmap[i]; }
	if (g) { h++; k += Gmap[i]; }
	if (b) { h++; k += Bmap[i]; }
	if (h > 1) k /= h;

	return k;
}
