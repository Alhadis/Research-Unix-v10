#include	<stdio.h>
#include	<jstructs.h>
#include	"pico.h"

#define	DITHSIZE	16
#define	DITHMASK	(DITHSIZE-1)
#define DITHMAX		DITHSIZE*DITHSIZE-1

int dith[DITHSIZE][DITHSIZE]={
#include "udither.data"
};
FILE *ofd;

extern short	CURSCRATCH, CUROLD;
extern struct	SRC src[MANY];
extern int DEF_NL, DEF_LL;

put0bitmap(name)
	char *name;
{
	register byte, mask, nout;
	register unsigned char *s;
	register int *dth;
	unsigned char *zbuf;
	int ll, len, ht, x, y, i;
	short data;
	Bitmap bp;

	if ((ofd = fopen(name, "w")) == NULL)
	{	fprintf(stderr, "pico: cannot open %s\n", name);
		return;
	}

	ll = DEF_LL;	/* full size */
	ht = DEF_NL;
	/* len = 32*((ll+31)/32); */
	len = 32*((2*ll+31)/32);	/* 2*ll for 2bit/pel */
	zbuf = (unsigned char *) Emalloc(ht*len/8);
	nout = 0;
	for (y = 0; y < ht; y++)
	{	byte = 0; mask = 0x80;
		dth = dith[y&DITHMASK];
		s = &(Old->pixred[2*y*DEF_LL]);
		for (x = 0; x < ll; x++)
		{	data = *s++;
		/*	if ((data & DITHMAX) < dth[x&DITHMASK])
				byte |= mask;
			mask >>= 1;
			if (mask == 0)
			{	zbuf[nout++] = byte;
				byte = 0;
				mask = 0x80;
			}
		*/
			data += dth[x&DITHMASK];	/* value between 0 - 511 */
			data /= 128;			/* value between 0 - 3 */
			for (i = 2; i > 0; i = i/2)
			{	if (data&i)
					byte |= mask;
				mask >>= 1;
				if (mask == 0)
				{	zbuf[nout++] = byte;
					byte = 0;
					mask = 0x80;
				}
			}
		}
	/*	for ( ; x < len; x++)
	*/	for (x = 2*x ; x < len; x++)
		{	if (dth[x&DITHMASK])
				byte |= mask;
			mask >>= 1;
			if (mask == 0)
			{	zbuf[nout++] = byte;
				byte = 0;
				mask = 0x80;
		}	}
	
	}
	bp.base = (Word *)zbuf;
	bp._null = (char *) 0;
	bp.width = len/32;	/* why not 16? */
	bp.rect.origin.x = bp.rect.origin.y = 0;
	bp.rect.corner.x = len; bp.rect.corner.y = ht;
	outbitmap(&bp, bp.rect);
	fflush(ofd);
	fclose(ofd);
	fprintf(stderr, "+: %s\n", name);
}

putbitmap(name)
	char *name;
{
	register byte, mask, nout;
	register unsigned char *s;
	register int *dth;
	unsigned char *zbuf;
	int ll, len, ht, x, y;
	short data;
	Bitmap bp;

	if ((ofd = fopen(name, "w")) == NULL)
	{	fprintf(stderr, "pico: cannot open %s\n", name);
		return;
	}

	ll = DEF_LL/2;	/* medium size */
	ht = DEF_NL/2;
	len = 32*((ll+31)/32);
	zbuf = (unsigned char *) Emalloc(ht*len/8);
	nout = 0;
	for (y = 0; y < ht; y++)
	{	byte = 0; mask = 0x80;
		dth = dith[y&DITHMASK];
		s = &(Old->pixred[2*y*DEF_LL]);
		for (x = 0; x < ll; x++)
		{	data  = *s + *(s+DEF_LL); s++;
			data += *s + *(s+DEF_LL); s++;
			data >>= 2;
			if ((data & DITHMAX) < dth[x&DITHMASK])
				byte |= mask;
			mask >>= 1;
			if (mask == 0)
			{	zbuf[nout++] = byte;
				byte = 0;
				mask = 0x80;
			}
		}
		for ( ; x < len; x++)
		{	if (dth[x&DITHMASK])
				byte |= mask;
			mask >>= 1;
			if (mask == 0)
			{	zbuf[nout++] = byte;
				byte = 0;
				mask = 0x80;
	}	}	}
	bp.base = (Word *)zbuf;
	bp._null = (char *) 0;
	bp.width = len/32;	/* why not 16? */
	bp.rect.origin.x = bp.rect.origin.y = 0;
	bp.rect.corner.x = len; bp.rect.corner.y = ht;
	outbitmap(&bp, bp.rect);
	fflush(ofd);
	fclose(ofd);
	fprintf(stderr, "+: %s\n", name);
}

#define SHORTSIZE	16

#define outword(w)	(putc(w,ofd) < 0 ? -1 : putc((w)>>8,ofd))
#define outpoint(p)	(outword((p).x) < 0 ? -1 : outword((p).y))
#define outrect(r)	(outpoint((r).origin) < 0 ? -1 : outpoint((r).corner))

static int ctype, count, rastwid; static short *p1;

static short *buf; static bufwid;

outbitmap(bp, r)
	Bitmap *bp; Rectangle r;
{
	register short *pr;
	register y, i;

	rastwid = (r.corner.x-r.origin.x+SHORTSIZE-1)/SHORTSIZE;

	if (rastwid > bufwid) {
		if (buf)
			free(buf), bufwid = 0;
		buf = (short *)Emalloc(rastwid*sizeof(short));
		if (buf == 0)
			return -1;
		bufwid = rastwid;
	}
	if (outword(0) < 0 || outrect(r) < 0)
		return -1;
	pr = (short *)bp->base;
	for (i=0; i<rastwid; i++)
		buf[i] = 0;
	for (y=r.origin.y; y<r.corner.y; y++)
	{	for (i=0; i<rastwid; i++)
			buf[i] ^= pr[i];
		if (outrast(buf) < 0)
			return -1;
		for (i=0; i<rastwid; i++)
			buf[i] = pr[i];
		pr = (short *)((Word *)pr + bp->width);
	}
	return 0;
}

outrast(p2)
	register short *p2;
{
	short *endraster;
	p1 = p2;
	endraster = p2+rastwid-1;

	do {
		if (p1 >= p2) {
			p2=p1+1; count=2;
			ctype=(*p1 == *p2);

		} else if ((*p2 == *(p2+1)) == ctype) {
			if (++count >= 127) {
				if (outbits() < 0)
					return -1;
				p1=p2+2;
			} else p2++;

		} else if (ctype) {
			if (outbits() < 0)
				return -1;
			p1=p2+1;
			ctype=0;

		} else {
			count--;
			if (outbits() < 0)
				return -1;
			p1=p2;
			ctype=1;
		}
	} while (p2 < endraster);

	if (p1 > endraster)
		return 0;
	if (p2 > endraster)
		count--;
	if (outbits() < 0)
		return -1;
	return 0;
}

outbits()
{	register c;
	register char *pout;

	c = count;
	if (ctype)
		c += 128, count=1;
	if (putc(c, ofd) < 0)
		return -1;
	c = sizeof(short)*count;
	pout = (char *)p1;
	while (--c >= 0)
		if (putc(*pout++, ofd) < 0)
			return -1;
	return 0;
}
