

/*
 *
 * The stuff in this file used to be part of di10.c. I've taken it out of
 * the main part of the post-processor because we're now accepting two
 * different formats for the raster files. The original raster files had
 * some rather severe limitations. They were machine dependent and used
 * shorts etc. quite extensively, especially as offsets to locate glyph
 * bitmaps. That put a limit on the point sizes that could be offered
 * even at 240 dots per inch, but at a resolution of 300 dots an inch
 * we probably wouldn't be able to access the bitmaps in a complete font
 * past size 24 or so. In addition nobody's supporting the old style
 * raster files or digitizing new characters for us. I've decided to
 * let the new post-processor read the old style raster files along with
 * the ones supplied by Imagen, although the old style won't be the
 * default.
 *
 *
 *
 *	The following things manage raster font information.
 *	The big problem is mapping desired font + size into
 *	available font + size.  For now, a file RASTERLIST
 *	contains entries like
 *		R 6 8 10 14 999
 *		I 8 10 12 999
 *		...
 *	This data is used to create an array "fontdata" that
 *	describes legal fonts and sizes, and pointers to any
 *	data from files that has actually been loaded.
 *
 */


#include <stdio.h>

#include "gen.h"			/* some general purpose definitions */
#include "ext.h"			/* external variable definitions */
#include "glyph.h"			/* raster file structures */
#include "impcodes.h"			/* Impress 2.0 opcode definitions */
#include "dimpress.h"			/* we'll need a few of these defs */


#define MAXFONT		60		/* max number of fonts forever */


struct fontdata {
	char	name[4];		/* e.g., "R" or "PA" */
	int	size[10];		/* e.g., 6 8 10 14 0 */
	struct	fontset	*fsp[10];	/* either NULL or block of data */
};



struct	Fontheader	fh;
struct	fontset {
	int	size;
	int	family;
	struct	Charparam *chp;
	unsigned char	*cdp;		/* char data pointer */
	unsigned char	*chused;	/* bit-indexed; 1 if char downloaded */
};


struct fontdata		fontdata[MAXFONT];
struct fontset		*fs;		/* current font+size */
int			maxfonts = 0;	/* number actually used */
int			nfamily = 0;	/* next available family number */
int			lastfam = -1;	/* last family we told Impress about */

extern int		res;		/* resolution assumed in input file */
extern int		pres;		/* printer resolution */
extern int		rres;		/* raster file resolution */
extern int		hpos;		/* troff's position variables */
extern int		vpos;
extern float		xfac;		/* scaling factors */
extern float		yfac;
extern int		xoffset;	/* hor and vert offsets in pixels */
extern int		yoffset;
extern int		font;		/* using font in this position */
extern int		size;		/* and want this point size */
extern int		lastfont;	/* got raster files for guy last */
extern int		lastsize;	/* and in this point size */
extern int		lastw;		/* width of the last character */
extern int		lastx;		/* Impress coordinates right now */
extern int		lasty;
extern int		center;		/* try and improve placement? */


/*****************************************************************************/


initfontdata()	/* read RASTERLIST information */
{
	char name[100];
	FILE *fp;
	int i, j, n;

	sprintf(name, "%s/RASTERLIST", rastdir);
	if ((fp = fopen(name, "r")) == NULL)
		error(FATAL, "can't open %s", name);
	maxfonts = 0;
	while (fscanf(fp, "%s", fontdata[maxfonts].name) != EOF) {
		i = 0;
		while (fscanf(fp, "%d", &n) != EOF && n < 100) {
			fontdata[maxfonts].size[i] = n;
			fontdata[maxfonts].fsp[i] = NULL;
			i++;
		}
		fontdata[maxfonts].size[i] = 999;
		if (++maxfonts > MAXFONT)
			error(FATAL, "Too many fonts in RASTERLIST");
	}
	fclose(fp);
	if (debug) {
		fprintf(stderr, "initfontdata():  maxfonts=%d", maxfonts);
		for (i = 0; i < maxfonts; i++) {
			fprintf(stderr, "%.4s ", fontdata[i].name);
			for (j = 0; fontdata[i].size[j] < 100; j++)
				fprintf(stderr, " %3d", fontdata[i].size[j]);
			fprintf(stderr, "\n");
		}
	}
}

getfontdata(f, s)	/* causes loading of font information if needed */
	char *f;
	int s;
{
	int fd, n, i, j;
	char name[100];
	static int first = 1;

	if (first) {
		initfontdata();
		first = 0;
	}

	for (i = 0; i < maxfonts; i++)
		if (strcmp(f, fontdata[i].name) == 0)
			break;
	if (i >= maxfonts)	/* the requested font wasn't there */
		i = 0;		/* use the first one (probably R) */

	s = (s * pres) / rres;	/* scale the requested point size */

	/* find the best approximation to size s */
	for (j = 1; s >= fontdata[i].size[j]; j++)
		;
	j--;

	/* open file if necessary */
	if (fontdata[i].fsp[j] == NULL) {
		fs = (struct fontset *) malloc(sizeof(struct fontset));
		fontdata[i].fsp[j] = fs;
		fs->chp = (struct Charparam *) malloc(256*sizeof(struct Charparam));
		sprintf(name, "%s/%s.%d", rastdir, f, fontdata[i].size[j]);
		fd = open(name, 0);
		if (fd == -1)
			error(FATAL, "can't open %s", name);
		read(fd, &fh, sizeof(struct Fontheader));
		read(fd, fs->chp, 256*sizeof(struct Charparam));
		fs->size = fontdata[i].size[j];
		fs->family = nfamily;
		nfamily += 2;	/* even-odd leaves room for big fonts */
		fs->cdp = (unsigned char *) malloc(fh.f_size);
		fs->chused = (unsigned char *) malloc(256/8);
		for (n = 0; n < 256/8; n++)
			fs->chused[n] = 0;
		n = read(fd, fs->cdp, fh.f_size);
		close(fd);
	}
	fs = fontdata[i].fsp[j];
}

xychar(c, f, s, chwid, tf)
	register int	c;		/* print this character */
	char		*f;		/* using this font */
	int		s;		/* and this point size */
	int		chwid;		/* use this as the char width? */
	FILE		*tf;		/* output written to this FILE */

{
	register unsigned char *p;
	register struct Charparam *par;
	register int x, y;
	char hold;
	int i, n, rwid, ht, fam;
	int	w;

	x = hpos * xfac + 0.5;
	x += xoffset;
	y = vpos * yfac + 0.5;
	y += yoffset;

	if (font != lastfont || size != lastsize) {
		getfontdata(f, s);
		lastsize = size;
		lastfont = font;
	}
	par = fs->chp + c;
	p = fs->cdp + par->c_addr;

	fam = fs->family;
	if (c > 127)
		fam++;
	if (fam != lastfam) {
		putc(ASF, tf);
		putc(lastfam = fam, tf);
	}

	/* first cut:  ship each glyph as needed. */
	/* ignore memory use, efficiency, etc. */

	if ( !checkbit(fs->chused, c) ) {	/* 1st use of this character */
		setbit(fs->chused, c);
		putc(ASGLY, tf);
		putint((fam << 7) | c, tf);
		par->c_width = (char)((lastw * pres) / res);
 		putc(par->c_width, tf);	/* character width */
		hold=(char) (w = CONVINT(par->c_left) + CONVINT(par->c_right) + 1);
		putc(hold,tf);
		if ( center )
			putc((w - CONVINT(par->c_width)) / 2, tf);
		else putc(par->c_left, tf);
  /* this nonsense fixes a bug in output produced by rec.c: */
  /* when up is < 0 (and = 0?) size is one too big */
		rwid = (1 + CONVINT(par->c_left) + CONVINT(par->c_right) + BYTE-1) / BYTE;
		ht = par->c_size / rwid;
		par->c_down = (char)(ht - CONVINT(par->c_up));
		hold=(char)(CONVINT(par->c_down) + CONVINT(par->c_up));
		putc(hold, tf);
		putc(par->c_up, tf);
		for (i = par->c_size; i--; )
			putc(*p++, tf);
	}

	if (y != lasty) {
		putc(ASETAV, tf);
		putint(y, tf);
		lasty = y;
	}

	if (ABS(x-lastx) > SLOP) {
		putc(ASETAH, tf);
		putint(x, tf);
		lastx = x + (par->c_width & BMASK);
	} else {
		lastx += (par->c_width & BMASK);
	}

	if (c <= 127)
		putc(c, tf);	/* fails if c > 127, probably disastrously */
	else
		putc(c-128, tf);
}
