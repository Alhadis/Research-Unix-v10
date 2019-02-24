#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include "pico.h"

extern struct	SRC src[MANY];
extern short	CURSCRATCH, CUROLD;
extern int	nsrc, DEF_LL, DEF_NL;
extern char	frameb, usedold, metheus;

prepare(infile)
	char *infile;
{
	struct SRC *into = &src[nsrc];
	int from;

	into->ex = into->ll = DEF_LL;
	into->ey = into->nl = DEF_NL;
	into->nchan = DEF_CH;
	into->sx = into->sy = 0;

	if ((from = whatarray(infile)) != -1)
	{	fprintf(stderr, "sorry: base name clash of %s with %s\n",
						infile, src[from].fname);
		return 0;
	}
	strcpy(into->fname, infile);
	if ((from = eopen(infile)) > 0 && readheader(into, from))
	{	into->fd = from;
		into->used = 0;
		into->pixred = (unsigned char *) Emalloc(DEF_LL*DEF_NL);
		if (into->nchan >= 3)
		{	into->pixgrn = (unsigned char *) Emalloc(DEF_LL*DEF_NL);
			into->pixblu = (unsigned char *) Emalloc(DEF_LL*DEF_NL);
		} else
		{	into->pixgrn = into->pixblu = into->pixred;
		}
		into->incore = 1;
		nsrc++;
		return nsrc-1;
	} else
	{	into->fname[0] = '\0';
		into->fd = -1;
		into->incore = into->used = 0;
		return 0;
	}
}

interpret(infile)
	char *infile;
{
	struct SRC *into = &src[nsrc];
	int from, hh;

	into->ex = into->ll = DEF_LL;
	into->ey = into->nl = DEF_NL;
	into->nchan = DEF_CH;
	into->sx = into->sy = 0;

	if ((from = eopen(infile)) > 0 && (hh = rawheader(into, from)))
	{	fprintf(stderr, "%s: ", &infile[findbase(infile)]);
		if (hh == 2)
		{	hh = less(from);
			fprintf(stderr, "headerless file, ");
			fprintf(stderr, "ll %d nl %d, ", into->ll, into->nl);
			fprintf(stderr, "(%d = %dx512 bytes)\n", hh, hh/512);
		} else
		{	fprintf(stderr, "%s ", (into->nchan == 1)?"b&w  ":"color");
			tpfile(into->type);
			fprintf(stderr, "ll %d nl %d ", into->ll, into->nl);
			wsize(into->sx, into->sy, into->ex, into->ey);
		}
		close(from);
	}
	into->fd = -1;
	into->incore = into->used = 0;
}

tpfile(n)
{
	switch (n) {
	case PICO:	fprintf(stderr, "pico-format "); break;
	case DUMP:	fprintf(stderr, "dump-format "); break;
	case RUNCODE:	fprintf(stderr, "runcode-format "); break;
	}
}

less(fd)
{ 	struct stat bam;
	if (fstat(fd, &bam)==0)
		return bam.st_size;
	return 0;
}

nopix(into)
	struct SRC *into;
{
	into->pixred = (unsigned char *) Emalloc(DEF_LL*DEF_NL);
/*
	into->pixgrn = (unsigned char *) Emalloc(DEF_LL*DEF_NL);
	into->pixblu = (unsigned char *) Emalloc(DEF_LL*DEF_NL);
*/
	into->pixgrn = (unsigned char *) 0;
	into->pixblu = (unsigned char *) 0;
	into->ex = into->ll = DEF_LL;
	into->ey = into->nl = DEF_NL;
	into->nchan = DEF_CH;
	into->sx = into->sy = 0;
	into->ox = into->oy = 0;
	strcpy(into->fname, "_workspace_");
	into->incore = 1;
}

checkpix(into)
	struct SRC *into;
{
	if (!into->pixgrn)
		into->pixgrn = (unsigned char *) Emalloc(DEF_LL*DEF_NL);
	if (!into->pixblu)
		into->pixblu = (unsigned char *) Emalloc(DEF_LL*DEF_NL);
}

discard(s)
	char *s;
{	int nr;

	if ((nr = ungetpix(s)) < 2)
		return 0;

	close(src[nr].fd);
	src[nr].fd = -1;
	src[nr].sx = src[nr].sy = src[nr].nchan = 0;
	src[nr].ex = src[nr].ey = src[nr].ll = 0;
	strcpy(src[nr].fname, " empty");
	src[nr].used = 0;

	return nr;
}

ungetpix(s)
	char *s;
{	int nr;

	if ((nr = whatarray(s)) < 2)
	{	fprintf(stderr, "unknown file %s\n", s);
		return 0;
	}

	if (src[nr].incore == 0)
		return nr;

	free(src[nr].pixred);
	if (src[nr].nchan >= 3)
	{	free(src[nr].pixgrn);
		free(src[nr].pixblu);
	}
	src[nr].incore = 0;
	lseek(src[nr].fd, src[nr].sop, 0);

	return nr;
}

newscreen(infile)
	char *infile;
{
	if ((Old->fd = eopen(infile)) > 0 && readheader(Old, Old->fd))
	{	getpix(Old, infile);
		putscreen(CUROLD);
		setscratch(Old, Scratch);
		return 1;
	}	/* else reset */
	Old->ex = Old->ll = DEF_LL;
	Old->ey = Old->nl = DEF_NL;
	Old->nchan = DEF_CH;
	Old->sx = Old->sy = 0;
	return 0;
}

readheader(into, from)
	struct SRC *into;
{
	int i, ret = rawheader(into, from);
	int a = into->sx, b = into->sy;
	int c = into->ex, d = into->ey;
	int n = into->nchan;

	if (a > c) i = a, a = c, c = i;		/* flip */
	if (b > d) i = b, b = d, d = i;
	if (a < 0) c -= a, a = 0;		/* normalize  */
	if (b < 0) d -= b, b = 0;
	into->ll = c-a; into->nl = d-b;		/* scan ll,nl */
			
	a = min(DEF_LL, a); c = min(DEF_LL, c);	/* clip */
	b = min(DEF_NL, b); d = min(DEF_NL, d);

	into->sx = a; into->sy = b;
	into->ex = c; into->ey = d;
	into->ox = 0; into->oy = 0;

	if (ret == 0) return 0;

	if (a >= c || b >= d)
	{	fprintf(stderr, "pico: file window is outside screen area\n");
		return 0;
	}
	if (n < 1 || n > 4)
	{	fprintf(stderr, "pico: bad nr of channels (%d)\n", n);
		return 0;
	}

	return ret;
}

rawheader(into, from)
	struct SRC *into;
{ char line[512];
  int i, j, a, b, c, d, N, sawtype=0;
  extern double sqrt();

	into->type = PICO;	/* default */
	for (j = 0;;)
	{	for (i = 0; i < 127; i++, j++)
		{	read(from, &line[i], 1);
			if (line[i] == '\n')
			{	line[i] = '\0';
				break;
		}	}
		if (i == 0)			/* saw end of header */
		{	if (sawtype)
			{	into->sop = j;
				return 1;
			} else
				i = 127;
		}
		if (i == 127)			/* no header */
		{	into->nchan = 1;	/* assume b&w, square image */
			N = (int) sqrt((double)less(from)+1.0);
			into->ex = into->ey = N;
			into->ll = into->nl = N;
			lseek(from, 0L, 0);
			into->sop = 0;
			return 2;
		}
		if (sscanf(line, "WINDOW=%d %d %d %d", &a, &b, &c, &d) == 4)
		{	into->sx = a; into->sy = b;
			into->ex = c; into->ey = d;
			into->ll = c-a; into->nl = d-b;
		} else if (sscanf(line, "NCHAN=%d", &a) == 1)
			into->nchan = a;
		   else if (strcmp(line, "TYPE=dump") == 0)
		 {	into->type = DUMP; sawtype = 1;
		 } else if (strcmp(line, "TYPE=runcode") == 0)
		 {	into->type = RUNCODE; sawtype = 1;
		 } else if (strcmp(line, "TYPE=pico") == 0)
		 {	into->type = PICO; sawtype = 1;
		 } else if (strncmp(line, "TYPE", 4) == 0)
		{	fprintf(stderr, "pico: unknown format: %s..\n", line);
			return 0;
		}
	}
}

putheader(where, n, a, b, c, d)
{	char line[128];

	sprintf(line, "TYPE=pico\nWINDOW=%d %d %d %d\nNCHAN=%d\n\n", a,b,c,d,n);
	write(where, line, strlen(line));
}

putdpix(which, how)
	char *which;
{
	int fd, a, b, c, d;

	a = Old->sx; b = Old->sy;
	c = Old->ex; d = Old->ey;

	checkit();		/* make sure we have screen incore */
	if ((fd = creat(which, 0666)) <= 0)
		yyerror("cannot create file %s", which);

	if (!how)
		putwindow(fd, Old->pixred, a, b, c-a, d-b);
	else
	{	if (Old->nchan != 1 || c-a != d-b)
			putheader(fd, min(3, Old->nchan), a, b, c, d);
		putfile(fd, a, b, c-a, d-b);
	}
	close(fd);
}

putfile(where, x, y, w, d)
{
	putwindow(where, Old->pixred, x, y, w, d);
	if (Old->nchan >= 3)
	{	putwindow(where, Old->pixgrn, x, y, w, d);
		putwindow(where, Old->pixblu, x, y, w, d);
	}
	close(where);
}

checkit()
{	register int i;

	if (usedold && src[CUROLD].incore != 2 && (frameb || metheus))
		fprintf(stderr, "warning: picture on screen wasn't read\n");

	for (i = 2; i < nsrc; i++)
	if (src[i].used && src[i].incore != 2)
	{	src[i].used = 0;
		getpix(&src[i], src[i].fname);
	}
}

setscratch(from, to)
	struct SRC *from, *to;
{
	register unsigned char *qr = from->pixred, *pr = to->pixred;
	register unsigned char *qg = from->pixgrn, *pg = to->pixgrn;
	register unsigned char *qb = from->pixblu, *pb = to->pixblu;
	memcpy(pr, qr, DEF_NL*DEF_LL);
	if (pg && qg) memcpy(pg, qg, DEF_NL*DEF_LL);
	if (pb && qb) memcpy(pb, qb, DEF_NL*DEF_LL);
}

getter(n)
{
	if (n <= 0)
		return;
	if (n >  1)
	{	if (src[n].incore != 2)
		getpix(&src[n], src[n].fname);
		setscratch(&src[n], Old);
	}
	putscreen(CUROLD);
	if (n >  1)
		setscratch(Old, Scratch);
	RESIDENT;
}
