#include <stdio.h>
#include "imPRESS.h"

#define min(a,b)	((a<=b) ? a : b)

#define	LINES	32
#define	RASTSIZ	108
char rasters[LINES][RASTSIZ] = { 0 };

glyphpage(out, name, size, xoff, yoff)
FILE *out;
char *name;
int size, xoff, yoff;
{
	int nrasters, rastwid, rastbyt, nghoriz, ngvert, krast, i, iprev, j;
	int x1, y1, x2, y2;
	FILE *in;
	if ((in = fopen(name, "r")) == NULL) {
		fprintf(stderr, "cannot open %s\n", name);
		return -1;
	}

	if (getw(in) != 0) {
		fclose(in);
		return 1;
	}
	x1 = getw(in);
	y1 = getw(in);
	x2 = getw(in);
	y2 = getw(in);
	nrasters = (y2 - y1 < 0) ? y1 - y2 : y2 - y1;
	rastwid = (x2 - x1 < 0) ? x1 - x2 : x2 - x1;
	if (nrasters <= 0 || nrasters > 1024 || rastwid  <= 0 || rastwid  >  800) {
		fclose(in);
		return -1;
	}

	xoff >>= size;
	yoff >>= size;
	rastbyt = ((rastwid + xoff + 15) / 16) * 2;
	rastwid = (rastwid + 15) / 16;
	ngvert  = (nrasters + yoff + 31) / 32;
	nghoriz = (rastbyt + 3) / 4;

	putc(imP_SET_MAGN , out);
	putc(         size, out);
	putc(imP_BITMAP   , out);
	putc(         0x0F, out);
	putc(      nghoriz, out);
	putc(       ngvert, out);

	/* clear out old junk in array */
	for (i=0; i<32; i++)
		for (j=0; j<rastbyt+2; j++)
			rasters[i][j] = 0;
	/* send the rasters */
	for (nrasters+=yoff; nrasters>0; nrasters -= krast) {
		krast=min(nrasters,32);
		for (iprev=(yoff-1)&0x1f,i=yoff; i<krast; iprev=i++) {
			for (j=0; j<rastbyt+2; j++)
				rasters[i][j] = 0;
			if (readrast(rasters[i],rastwid,in) < 0){
				fclose(in);
				return -1;
			}
			shiftrast(rasters[i], xoff, rastbyt);
			for (j=0; j<rastbyt; j++)
				rasters[i][j] ^= rasters[iprev][j];
		}
		/* clear out old junk in array */
		for (i=krast; i<32; i++){
			for (j=0; j<rastbyt+2; j++) {
				rasters[i][j] = 0;
			}
		}
		for (j=0; j<rastbyt; j+=4) {
			for (i=0; i<32; i++)
				fwrite(&rasters[i][j],4,1,out);
		}
		yoff=0;
	}
	fclose(in);
	return 0;
}

static
readrast(p1,nw,in)
char *p1;
int nw;
FILE *in;
{
	int count, ctype;

	while (nw>0) {
		if ((count=getc(in)) <= 0)
			return -1;
		ctype = count & 0x80;
		count &= 0x7f;
		nw -= count;
		count *= 2;

		if (ctype) {
			if (fread(p1,2,1,in) <= 0)
				return -1;
			for (count-=2; count>0; count--) {
				*(p1+2) = *p1;
				p1 += 1;
			}
			p1 += 2;
		} else {
			if (fread(p1,count,1,in) <= 0)
				return -1;
			p1 += count;
		}
	}
	return (nw == 0 ? 0 : -1);
}

static
getw(stream)
FILE *stream;
{
	register l = getc(stream);
	return (getc(stream)<<8) | l;
}
iglyphpage(out, name, size, xoff, yoff)
FILE *out;
char *name;
int size, xoff, yoff;
{
	int nrasters, rastwid, rastbyt, nghoriz, ngvert, krast, i, iprev, j;
	int h=0, v, nbits;
	FILE *in;

	if ((in = fopen(name, "r")) == NULL) {
		fprintf(stderr, "cannot open %s\n", name);
		return -1;
	}

	for (v=0; nbits = nexticon(rasters, in); v++)
		if (nbits > h)
			h = nbits;
	rewind(in);

	nrasters = v;
	rastwid = h;
	if (nrasters <= 0 || nrasters > 1024 || rastwid  <= 0 || rastwid  >  800) {
		fclose(in);
		return -1;
	}

	xoff >>= size;
	yoff >>= size;

	rastbyt = ((rastwid + xoff + 15) / 16) * 2;
	rastwid = (rastwid + 15) / 16;
	ngvert  = (nrasters + yoff + 31) / 32;
	nghoriz = (rastbyt + 3) / 4;

	putc(imP_SET_MAGN, out);
	putc(        size, out);

	putc(imP_BITMAP  , out);
	putc(        0x0F, out);
	putc(     nghoriz, out);
	putc(      ngvert, out);

	/* clear out old junk in array */
	for (i=0; i<32; i++)
		for (j=0; j<rastbyt+2; j++)
			rasters[i][j] = 0;
	/* send the rasters */
	for (nrasters+=yoff; nrasters>0; nrasters -= krast) {
		krast=min(nrasters,32);
		for (iprev=(yoff-1)&0x1f,i=yoff; i<krast; iprev=i++) {
			for (j=0; j<rastbyt+2; j++)
				rasters[i][j] = 0;
			if (nexticon(rasters[i], in) == 0) {
				fclose(in);
				return -1;
			}
			shiftrast(rasters[i], xoff, rastbyt);
		}
		/* clear out old junk in array */
		for (i=krast; i<32; i++){
			for (j=0; j<rastbyt+2; j++) {
				rasters[i][j] = 0;
			}
		}
		for (j=0; j<rastbyt; j+=4) {
			for (i=0; i<32; i++){
				fwrite(&rasters[i][j],4,1,out);
			}
		}
		yoff=0;
	}
	fclose(in);
	return 0;
}
static int
nexticon(rast, in)
short *rast;
FILE *in;
{
	register j = 0, nbits; char delim[8], *p, c;
	do {
		if (fscanf(in, " 0x%4hx%[, \t\n]", (p = (char *)&rast[j++]), delim) <= 0)
			return 0;
		c = *p; *p = *(p+1); *(p+1) = c;	/* swap the bytes */
	} while (strchr(delim, '\n') == 0);
	nbits = 16*j;
	return nbits;
}

shiftrast(s, b, w)
unsigned char *s;
{
	register unsigned char *from, *to;
	register int i, bits, byte;
	i = b >> 3;
	if (i != 0) {
		for (to = &s[w], from = &s[w-i]; from != s; )
			*--to = *--from;
		while (to != s)
			*--to = 0;
	}
	bits = 0;
	i = b & 0x7;
	if (i != 0)
		for (to = s; to != &s[w]; to++) {
			byte = (bits | *to) >> i;
			bits = (*to & ((1 << i) - 1)) << 8;
			*to = byte;
		}
}
