#include <jerq.h>
#include <jerqio.h>
#include <font.h>

#define UP	0	/* button state */
#define DOWN	1	/* button state */

#define SIGNED(ch)	((ch) & 0x80 ? (ch) | 0xffffff00 : (ch))

extern Texture deadmouse;

main(argc,argv)
int argc; char *argv[];
{
	Rectangle rf; Point pf; int i, j, dx; Font *fp;

	request(MOUSE); cursswitch(&deadmouse);
	if (argc <= 1)
		fp = &defont;
	else if ((fp=getfont(argv[1])) == (Font *)0) {
		printf("getfont failed\n");
		exit();
	}

	cursswitch((Texture *)0);

	rf=fp->bits->rect; pf=add(Drect.origin, Pt(3,3));
	dx = Drect.corner.x - 3 - pf.x;
	for (i = 0; i < fp->n; i = j) {
		for (j=i; j < fp->n; j++)
			if (fp->info[j+1].x > dx)
				break;
		rf.corner.x = fp->info[j].x;
		bitblt(fp->bits, rf, &display, pf, F_XOR);
		dx += rf.corner.x - rf.origin.x;
		rf.origin.x = rf.corner.x;
		pf.y       += fp->height+3;
	}

	buttons(DOWN); buttons(UP); rectf(&display,Drect,F_XOR);

	printf("n = %d, height = %d, ascent = %d, unused = %D\n",
	fp->n,			/* number of chars in font */
	(int)fp->height,	/* height of bitmap */
	(int)fp->ascent,	/* top of bitmap to baseline */
	fp->unused);		/* in case we think of more stuff */

	printf("bits->width = %d, org.x = %d, org.y = %d, cor.x = %d, cor.y = %d\n",
	fp->bits->width,
	fp->bits->rect.origin.x,fp->bits->rect.origin.y,
	fp->bits->rect.corner.x,fp->bits->rect.corner.y);

	printf("\ni:\tc x t b l w");
	for (i=0; i <= fp->n; i++) {
		if (i%4==0) printf("\n%d:",i);
		printf("\t%c %d %d %d %d %d",
		(i > ' ' && i < 0177 ? i : ' '),
		fp->info[i].x,			/* left edge of bits */
		(int)fp->info[i].top,		/* first non-zero scan-line */
		(int)fp->info[i].bottom,	/* last non-zero scan-line */
		SIGNED(fp->info[i].left),	/* offset of baseline */
		(int)fp->info[i].width);	/* width of baseline */
	}
	printf("\nputfont: %d\n", putfont(fp, "font.out"));

	exit();
}

FILE *outf;

static
cput(c)
{
	return(putc(c,outf));
}

putfont(f, s)
	Font *f;
	char *s;
{
	int status;

	if((outf = fopen(s, "w")) == (FILE *)0)
	{
		return -1;
	}

	status = outfont(f, cput);
	fclose(outf);
	return status;
}
