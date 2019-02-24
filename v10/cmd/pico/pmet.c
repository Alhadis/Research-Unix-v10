#include <stdio.h>
#include <signal.h>
#include "pico.h"
#define NMBUF	2048

#define	mlong(a)	(mword((a)>>16), mword(a))
#define	mword(a)	(mbyte((a)>>8), mbyte(a))
#define	mbyte(a)	(mbufp==&mbuf[NMBUF] && mflush(), *mbufp++=(a))
/* versions of the above allowing moving checks out of the loop */
#define	mcheck(n)	(mbufp>=&mbuf[NMBUF-n] && mflush())
#define	mulong(a)	(muword((a)>>16), muword(a))
#define	muword(a)	(mubyte((a)>>8), mubyte(a))
#define	mubyte(a)	(*mbufp++=a)
#define	mcmd(a)		((mbufp-mbuf)&1 && (*mbufp++=0), mword(a))
#define	mflush()	((mbufp-mbuf)&1 && (*mbufp++=0),write(om_fd, mbuf, (mbufp-mbuf)*sizeof mbuf[0]),mbufp=mbuf)

#define	MOVP1		0x0052
#define	MOVP2		0x0053
#define	SETCLL		0x3061	/* l:color */
#define	WPIXEL		0x006d	/*  */
#define	CMAP		0x0051	/* b:index, b:red, b:grn, b:blu */
#define	WRMSKL		0x3023	/* l:mask */
#define	CMAPOL		0x301a	/* 8b:2 nibble-sized entries */
#define CMSEL		0x3015
#define CMLDM		0x3013
#define CMCOPY		0x3016
#define CMACT		0x3000
#define RGBA		0xe
#define RGB		0xd
#define A		0x1
#define R		0x2
#define G		0x4
#define B		0x8

extern short	CURSCRATCH, CUROLD;
extern struct	SRC src[MANY];
extern int	DEF_LL, DEF_NL;
extern void	onquit();

int om_fd;

om_open(dev)
{	char buf[16];
	unsigned char mbuf[16], *mbufp=mbuf;
	extern char metheus;

	sprintf(buf, "/dev/om%d", dev);
	if ((om_fd = open(buf, 2)) == -1)
	{	perror("om_open");
		metheus = 0;
		return 0;
	}
#if 1
	signal(SIGINT, SIG_IGN);	/* don't mess up the device */
	metinitcmap();
	muword(WRMSKL);			/* enable all 4 bitplanes */
	mubyte(255); mubyte(255);
	mubyte(255); mubyte(255);
	muword(CMAPOL);			/* clear the overlaymap   */
	muword(0); muword(0);
	muword(0); muword(0);
	write(om_fd, (char *)mbuf, 16);
	signal(SIGINT, onquit);
#endif
	return 1;
}

om_close()
{
	close(om_fd);
}

metputitall(k)
{	struct SRC *from = &src[k];
	register unsigned char *pr, *pg, *pb;
	register int i; int offset, delta;
	int sx = from->sx;
	int ex = from->ex;
	int sy = from->sy;
	int ey = from->ey;
	int chunk; if ((ex-sx)&1) ex++;

	signal(SIGINT, SIG_IGN);	/* don't mess up the device */
	merect(sx, sy, ex, ey);
	offset = sy*DEF_LL + sx;
	delta  = DEF_LL - ex + sx;
	chunk = (ex-sx)+delta;

	pr = from->pixred+offset;
	switch (from->nchan)
	{	case 1:	setbank(RGBA);
			for (i = sy; i < ey; i++, pr += chunk)
				write(om_fd, pr, ex-sx);
			break;
		case 3:	setbank(R);
			for (i = sy; i < ey; i++, pr += chunk)
				write(om_fd, pr, ex-sx);
			setbank(G); pg = from->pixgrn+offset;
			for (i = sy; i < ey; i++, pg += chunk)
				write(om_fd, pg, ex-sx);
			setbank(B); pb = from->pixblu+offset;
			for (i = sy; i < ey; i++, pb += chunk)
				write(om_fd, pb, ex-sx);
			break;
		default:
			fprintf(stderr, "undefined number of channels\n");
			break;
	}
	signal(SIGINT, onquit);
}

metgetitall(from)
	struct SRC *from;
{
	register unsigned char *pr, *pg, *pb;
	register int i; int offset, delta;
	int sx = from->sx;
	int ex = from->ex;
	int sy = from->sy;
	int ey = from->ey;
	int chunk; if ((ex-sx)&1) ex++;

	signal(SIGINT, SIG_IGN);	/* don't mess up the device */
	merect(sx, sy, ex, ey);
	offset = sy*DEF_LL + sx;
	delta  = DEF_LL - ex + sx;
	chunk = (ex-sx)+delta;

	if (from == Old)
		fprintf(stderr, "reading screen...");

	pr = from->pixred+offset;
	switch (from->nchan)
	{	case 1:	setbank(RGBA);
			for (i = sy; i < ey; i++, pr += chunk)
				read(om_fd, pr, ex-sx);
			break;
		case 3:	setbank(R);
			for (i = sy; i < ey; i++, pr += chunk)
				read(om_fd, pr, ex-sx);
			setbank(G); pg = from->pixgrn+offset;
			for (i = sy; i < ey; i++, pg += chunk)
				read(om_fd, pg, ex-sx);
			setbank(B); pb = from->pixblu+offset;
			for (i = sy; i < ey; i++, pb += chunk)
				read(om_fd, pb, ex-sx);
			break;
		default:
			fprintf(stderr, "undefined number of channels\n");
			break;
	}
	signal(SIGINT, onquit);
	RESIDENT;
}

metlyank(x, y)
{	register int i;
	register unsigned char *pr, *pg, *pb;
	extern char faster;
	int from=Old->sx, to=Old->ex;

	if (y <= Old->sy || y > Old->ey) return 1;

	if (!faster)
	{	metyank(x, y);
		return;
	} else
	{	if (x > from)
			return;
	}
	if ((to-from)&1) to++;
	i = x+DEF_LL*(y-1);
	signal(SIGINT, SIG_IGN);	/* don't mess up the device */
	merect(from, y-1, to, y);

	pr = scratchred+i;
	switch (Old->nchan)
	{	case 1:	setbank(RGBA); write(om_fd, pr, to-from);
			break;
		case 3: pg = scratchgrn+i;
			pb = scratchblu+i;
			setbank(R); write(om_fd, pr, to-from);
			setbank(G); write(om_fd, pg, to-from);
			setbank(B); write(om_fd, pb, to-from);
			break;
	}
	signal(SIGINT, onquit);
	return 1;
}

metyank(x, y)
{	register int i = DEF_LL*y+x;
	unsigned char *pg, *pb;
	unsigned char *pr = scratchred+i;

	signal(SIGINT, SIG_IGN);	/* don't mess up the device */
	merect(x, y, x, y);
	switch (Old->nchan)
	{	case 1:	setbank(RGBA); write(om_fd, pr, 1);
			break;
		case 3:	pg = scratchgrn+i;
			pb = scratchblu+i;
			setbank(R); write(om_fd, pr, 1);
			setbank(G); write(om_fd, pg, 1);
			setbank(B); write(om_fd, pb, 1);
			break;
	}
	signal(SIGINT, onquit);
	return 1;
}

unsigned char Rmap[256], Gmap[256], Bmap[256];

metinitcmap()
{	int i;

	for (i = 0; i < 256; i++)
		Rmap[i] = Gmap[i] = Bmap[i] = (unsigned char) i;
	
}

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
{	unsigned char mbuf[2048], *mbufp=mbuf;
	unsigned char cmap[256*3], *m;
	int i;

	signal(SIGINT, SIG_IGN);	/* don't mess up the device */

	for (i = 0; i < 256; i++)
	{	cmap[3*i]   = Rmap[i];
		cmap[3*i+1] = Gmap[i];
		cmap[3*i+2] = Bmap[i];
	}
	m = cmap;
	mcmd(CMSEL); mbyte(0);			/* write in cmap buffer 0 */
	mcmd(CMLDM);mword(0);mword(256);	/* load 256 entries at loc. 0 */
	for(i=0;i!=256*3;i++) mbyte(*m++);	/* data */
	mcmd(CMCOPY); mbyte(0); mbyte(0);	/* copy 256 entries to rest */
	mword(0); mword(256); mword(256*15);
	mcmd(CMACT);				/* make current buffer active */
	mflush();

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

merect(x0, y0, x1, y1)
{
	unsigned char mbuf[16], *mbufp=mbuf;

	muword(MOVP1); muword(x0);   muword(y0);
	muword(MOVP2); muword(x1-1); muword(y1-1);
	write(om_fd, (char *)mbuf, 12);
}

#define	WRBANK	0x003d
#define	WRR	0x006f

setbank(bank)
	unsigned char bank;
{
	unsigned char mbuf[8], *mbufp=mbuf;

	muword(WRBANK);
	mubyte(bank); mubyte(0);
	muword(WRR);		/* write 1 byte/pixel rectangle */
	write(om_fd, (char *)mbuf, 6);
}
