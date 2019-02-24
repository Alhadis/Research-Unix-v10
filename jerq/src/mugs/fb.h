#include <stdio.h>
#define	ITIADDR	(('F'<<8)|0)	/* get Unibus device address */
/*
 * hardware registers
 */
typedef struct fbreg{
	struct chanreg{
		short x, y, pan, scroll;
		unsigned char vid, csr, j0, j1, mask, j2, z, j3;
	}grn, alf, red, blu;
	struct mapreg{
		unsigned char data, addr, sele, csr;
	}map;
}fbreg;
fbreg *snare();
typedef struct point{
	int x, y;
}Point;
typedef struct rectangle{
	Point origin, corner;
}Rectangle;
typedef struct color{
	unsigned char r, g, b, a;
}Color;
#ifdef lint
Color Rgb(), Rgba();
#else
#define	Rgb(r, g, b) Rgba(r, g, b, 255)
#define	Rgba(r, g, b, a) (((r)&255)|(((g)&255)<<8)|(((b)&255)<<16)|((a)<<24))
#endif
typedef struct hsva{
	short h;			/* won't fit in a char.  nuts. */
	unsigned char s, v, a;
}Hsva;
Color hsv2rgb();
Hsva rgb2hsv();
#define	fbr	fb.rect
#define	or	origin
#define	co	corner
struct fb{
	Rectangle rect;
	int colors;
	char gotflags;
	char cheat;
	fbreg *addr;
	int fd;
}fb;
char *progname;
#define	F_RED	1
#define	F_GRN	2
#define	F_BLU	4
#define	F_ALF	8
/*
 * [red,grn,blu,alf].csr defines
 */
#define	F_IX	0x5
#define	F_DX	0x4
#define	F_IY	0xa
#define	F_DY	0x8
#define	F_IR	0x10
#define	F_IW	0x20
#define	F_IXW	(F_IX|F_IW)
#define	F_IYW	(F_IY|F_IW)
#define	F_IXR	(F_IX|F_IR)
#define	F_IYR	(F_IY|F_IR)
#define	F_DXW	(F_DX|F_IW)
#define	F_DYW	(F_DY|F_IW)
#define	F_DXR	(F_DX|F_IR)
#define	F_DYR	(F_DY|F_IR)
#define	F_CLR	0x40
/*
 * [red,grn,blu,alf].vid defines
 */
#define	F_MAG1	0		/* magnify x1 */
#define	F_MAG2	6		/* magnify x2 */
/*
 * constructors for points and rectangles -- good only in argument lists
 */
#ifdef	lint
Point Pt();
Rectangle Rect(), Rpt();
#else
#define	Pt(x, y)		(x), (y)
#define	Rect(a, b, c, d)	(a), (b), (c), (d)
#define	Rpt(a, b)		a, b
#endif
Point add(), sub(), mul(), div();
Rectangle inset(), raddp(), rsubp(), rcanon(), rdiv(), rmul(), rdivp(), rmulp();
#define	F_SY	32		/* Y scale for hotdog lines, etc. */
#define	F_SX	(F_SY*5/4)	/* X scale for hotdog lines, etc. */
/*
 * A font file contains a struct fonthead, followed by fonthead.nchr copies
 * of struct chr, followed by run-coded descriptions of each character.
 * The runs are stored as an 8 bit count (actually 1 less than the count) and
 * an 8 bit value.  No run spans the end of a scanline.
 */
#define	FONTMAGIC 1138		/* magic number */
struct fonthead{
	short magic;		/* FONTMAGIC */
	short nfontchar;	/* normally 137 */
	short size;		/* nominal size in pixels */
	char name[16];		/* name[15]=='\0' */
};
struct fontchar{
	short xlo, ylo, xhi, yhi;/* bounding box of character */
	long addr;		/* address in file of runs, 0 if no char */
};
/*
 * incore structure used by fbfontopen, fbfontdraw
 */
struct fbfont{
	short nfontchar;
	struct fbfontchar{
		Rectangle r;
		unsigned char *bits;
	}*fontchar;
};
struct fbfont *fbfontopen();
/*
 * new picture file definitions
 * used by openpic[rw], readpic, writepic, etc.
 */
typedef struct picfile{
	FILE *fd;
	char *file;
	Rectangle r;
	int nchan;
	int type;
	unsigned char *cmap;
	char **argv;
}PICFILE;
/*
 * PICFILE types
 */
#define	F_RUNCODE	1
#define	F_DUMP		2
#define	F_PICO		3
PICFILE *openpicr(), *openpicw();
#define	F_UNCOVERED	1.701411733192644270e38	/* large value to indicate no z */
