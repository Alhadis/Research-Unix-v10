#include <stdio.h>
#include "pico.h"

extern struct	SRC src[MANY];
extern short	CUROLD;
extern int	DEF_LL, DEF_NL;

getwindow(into, dest)
	struct SRC *into;
	char *dest;
{
	register char *z = dest + (into->sy)*DEF_LL + (into->sx);
	register int i;
	register int w = into->ex - into->sx;
	register int d = into->ey - into->sy;
	int ll = into->ll, nl = into->nl;
	int  x = into->ox,  y = into->oy;
	int fd = into->fd;

	lseek(fd, (y*ll + x), 1);
	for(i = 0; i < d; i++, z += DEF_LL)
	{	if (!eread(fd, z, w))
			break;
		lseek(fd, ll-w, 1);
	}
	lseek(fd, ll-x, 1);
	lseek(fd, (nl-d-y-1)*ll, 1);

}

putwindow(fd, source, x, y, width, depth)
	char *source;
{
  register char *z = source + y*DEF_LL + x;
  register int i;

	for (i = 0; i < depth; i++, z += DEF_LL)
		if (!ewrite(fd, z, width))
			break;
}

eopen(infile)
	char *infile;
{ int from;
	if ((from = open(infile, 0)) <= 0)
	{	fprintf(stderr, "pico: cannot open %s\n", infile);
		return 0;
	}
	return from;
}

eread(n, m, p)
	char *m;
{ int k;
	if ((k=read(n, m, p)) != p)
	{	fprintf(stderr, "pico: read error..wanted %d got %d\n", p, k);
		return 0;
	}
	return 1;
}

ewrite(n, m, p)
	char *m;
{ int k;
	if ((k=write(n, m, p)) != p)
	{	fprintf(stderr, "pico: write error..wrote %d iso %d\n", k, p);
		return 0;
	}
	return 1;
}

getpix(into, infile)
	struct SRC *into;
	char *infile;
{
	fprintf(stderr, "reading %s...", into->fname);
	if (into->incore == 0)
	{	into->pixred = (unsigned char *) Emalloc(DEF_LL*DEF_NL);
		if (into->nchan >= 3)
		{	into->pixgrn = (unsigned char *) Emalloc(DEF_LL*DEF_NL);
			into->pixblu = (unsigned char *) Emalloc(DEF_LL*DEF_NL);
		}
		into->incore = 1;
	}
	if (into->type == PICO || (into->type == DUMP && into->nchan == 1))
		getfile(into);
	else {
		fprintf(stderr, "sorry, unsupported picture format\n");
		return;
	}
	into->incore = 2;
	fprintf(stderr, "\n");
}

getfile(into)
	struct SRC *into;
{
	getwindow(into, into->pixred);
	if (into->nchan == 1)
		into->pixgrn = into->pixblu = into->pixred;
	else
	{	getwindow(into, into->pixgrn);
		getwindow(into, into->pixblu);
	}
}
