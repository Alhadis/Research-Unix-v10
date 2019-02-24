#include	<jerq.h>
#include	<jerqio.h>
typedef struct
{
	short x;		/* left edge of bits */
	char top;		/* first non-zero scan-line */
	char bottom;		/* last non-zero scan-line */
	char left;		/* offset of baseline */
	char width;		/* width of baseline */
} Fontchar;

typedef struct
{
	short n;		/* number of chars in font */
	char height;		/* height of bitmap */
	char ascent;		/* top of bitmap to baseline */
	long unused;		/* in case we think of more stuff */
	Bitmap *bits;		/* where the characters are */
	Fontchar info[128];		/* n+1 character descriptors */
} Font;

#include	"char.h"

Font font;
int x[1000];	/* do not move !!!! */
extern int newlnsz;
int nbytes = 0;
int offs = 0;

main(argc, argv)
char **argv;
{
	register i;
	short int x;
	Bitmap *bmap;
	extern putchar();

	jinit();
	/*
		first find max height, set most vars
	*/

	for(i = 0; i < 128; i++)
	{
		font.info[i].x = offs;
		font.info[i].width = dispatch[i].c_wid;
		font.info[i].left = 0;
		font.info[i].top = dispatch[i].c_blank;
		font.info[i].bottom = dispatch[i].c_blank + dispatch[i].c_ht;
		if(dispatch[i].c_addr)
			offs += dispatch[i].c_wid;
	}
/*
	for(i = 0; i < 128; i++)
	{
		fprintf(stderr, "[%d] x=%d, top=%d bot=%d left=%d width=%d\n", i,
			font.info[i].x, font.info[i].top,
			font.info[i].bottom, font.info[i].left,
			font.info[i].width);
	}
*/
	font.height = newlnsz;
	font.n = 0131;
	font.ascent = font.height;
	bmap = balloc(Rect(0,0, offs, newlnsz));
	rectf(bmap, bmap->rect, F_CLR);
	if(bmap == (Bitmap *)0)
	{
		fprintf(stderr, "BALLOC failed ht=%d,offs=d\n",newlnsz,offs);
		exit();
	}
	/*
		adjust blank and write bits
	*/
	for(i = 0; i < 128; i++)
	{
		if(dispatch[i].c_addr)
			bltchar(&dispatch[i], bmap, Pt(font.info[i].x,
				font.info[i].top));
		bitblt(bmap, Rect(font.info[i].x , 0, font.info[i+1].x, font.height),
			&display, add(Drect.origin, Pt(100, 100)), F_STORE);
		sleep(10);
	}

	/*
		write it out
	*/

	font.bits = bmap;
	outfont(&font, putchar);
	bfree(bmap);
	exit();
}

bltchar(dp, bm, pt)
	register struct dispatch *dp;
	register Bitmap *bm;
	Point pt;
{
	static Bitmap font;
	register Rectangle *rp= &font.rect;
	register Point *pp= &pt;

	font.base = dp->c_addr;
	rp->origin.x=0;
	rp->origin.y=0;
	rp->corner.x = dp->c_wid;
	rp->corner.y = dp->c_ht;
	font.width = (dp->c_wid - 1)/16 + 1;

	bitblt(&font, *rp, bm, *pp, F_STORE);
}
