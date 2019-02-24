/*
 *	lpd - Printronix line printer daemon dispatcher
 */

#include	<ctype.h>
#include	<sgtty.h>

#define	SPIDER	0
#define	PHONE	0
#define	LPD	1

#include	"daemon.c"

/*
 * The remaining part is the line printer interface.
 */

FILE	*lpf = NULL;

#define LINESIZE 1000

char buf[LINESIZE], ubuf[LINESIZE];
int ulen, blen;

dem_con()
{
	return(0);
}

dem_dis()
{
	FCLOSE(lpf);
	FCLOSE(dfb);
}

dem_open(file)
char	*file;
{
	struct sgttyb tb;
	static char iobuf[BUFSIZ];

	if((lpf = fopen(lp, "w")) == NULL)
		trouble("Can't open %s", lp);

	if (gtty(fileno(lpf),&tb) < 0)
		trouble("Can't do gtty(%s)\n", lp);
	
	tb.sg_ispeed = tb.sg_ospeed = B9600;
	tb.sg_flags &= ~ALLDELAY;
	tb.sg_flags |= XTABS;

	if (stty(fileno(lpf),&tb) < 0)
		trouble("Can't do stty(%s)\n", lp);

	setbuf (lpf, iobuf);
}


dem_close()
{
	dem_dis();
}

get_snumb()
{
}

lwrite()
{
	banner(lpf, &line[1]);
}


FILE	*ibuf;

enum linemode {normal, large, plot};

sascii(fc)
char	fc;
{
	register int c, p;
	register enum linemode lm;

	if((ibuf = fopen(&line[1], "r")) == NULL)
		return(0);
	if(fc == 'F')
		putc(ff, lpf);

	clr();
	p = 0;
	lm = normal;
	while ((c = getc (ibuf)) != EOF) {
		switch (c) {

		case ' ':
			++p;
			break;

		case '\b':
			if (p > 0)
				--p;
			break;

		case '\t':
			p = (p + 8) & -8;
			break;

		case '\r':
			p = 0;
			break;

		case '\n':
		case '\f':
			emit(c, lm);
			lm = normal;
			clr();
			p = 0;
			break;

		case '\033':	/* escape for expand */
			lm = large;
			break;

		case '\005':	/* plot mode */
			lm = plot;
			break;

		default:
			if (p < LINESIZE) {
				if (lm != plot && c == '_') {
					ubuf[p++] = c;
					if (p > ulen)
						ulen = p;
				} else {
					buf[p++] = c;
					if (p > blen)
						blen = p;
				}
			}
			break;

		}
	}
	fflush(lpf);
	fclose(ibuf);
	return(0);
}

etcp1()
{
}

/* VARARGS */
trouble(s, a1, a2, a3, a4)
char	*s;
{
	if(retcode != 0){
		dem_dis();
	}
	logerr(s, a1, a2, a3, a4);
	longjmp(env, 1);
}

/* VARARGS */
logerr(s, a1, a2, a3, a4)
char	*s;
int	a1, a2, a3, a4;
{
#ifdef DEBUG
	fprintf(stderr, s, a1, a2, a3, a4);
	putc('\n', stderr);
#endif
}

getowner()
{
}

maildname()
{
	fprintf(pmail, "Your %s job for file %s is finished.\n", DAEMNAM, mailfname);
}

clr()
{
	register int i;

	ulen = blen = 0;

	for (i = 0; i < LINESIZE; i++)
		buf[i] = ubuf[i] = ' ';
}

emit(c, mode)
	register int c;
	register enum linemode mode;
{
	register int i;

	switch (mode) {
	case large:
		putc ('\b', lpf);
		break;

	case plot:
		putc ('\005', lpf);
		while (blen > 0 && (buf[blen-1] & 077) == 0)
			--blen;
		break;
	}

	for (i = 0; i < blen; i++)
		putc (buf[i], lpf);
	
	if (ulen && mode != plot) {
		putc ('\r', lpf);
		for (i = 0; i < ulen; i++)
			putc (ubuf[i], lpf);
	}

	putc (c, lpf);

	if (ferror (lpf))
		trouble ("Output error on %s\n", lp);
}
