/*
 *	lpd - Printronix line printer daemon dispatcher
 */

#include <ctype.h>
#include <sys/ttyio.h>
#include <sys/filio.h>

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
	struct ttydevb grime;
	static char iobuf[BUFSIZ];
	int ld = 0;
	extern int tty_ld;

	if((lpf = fopen(lp, "w")) == NULL)
		trouble("Can't open %s", lp);
	if (ioctl(fileno(lpf), FIOLOOKLD, &ld)<0 || ld!=tty_ld)
		ioctl(fileno(lpf), FIOPUSHLD, &tty_ld);
	if (ioctl(fileno(lpf), TIOCGETP, &tb) < 0)
		trouble("Can't do gtty(%s)\n", lp);
	ioctl(fileno(lpf), TIOCGDEV, &grime);
	tb.sg_ispeed = tb.sg_ospeed = B9600;	/* obsolescent */
	tb.sg_flags &= ~ALLDELAY;
	tb.sg_flags |= XTABS;
	if (ioctl(fileno(lpf), TIOCSETP, &tb) < 0)
		trouble("Can't do stty(%s)\n", lp);
	grime.ispeed = grime.ospeed = B9600;
	ioctl(fileno(lpf), TIOCSDEV, &grime);
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

/*
 *	how many rows of dots have appeared on this page.
 *
 *	there are ten rows per normal character, 20 per
 *	large character.  We count dots to know whether
 *	to emit a form feed at the end.
 */
int dots = 0;

/* Heights for various character modes */
#define NDOTS 10
#define LDOTS 20
#define PDOTS 1

/* vertical granularity in non-plot mode */
#define GRAIN 10

/* align x downward with granularity y */
#define align(x,y) (((x)/(y))*(y))

/* Page depth, in dots */
#define PAGE 660

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

		case '\033':	/* escape-backspace for expand */
			c = getc (ibuf);
			if (c == '\b')	
				lm = large;
			break;

		case '\005':	/* plot mode */
			lm = plot;
			break;

		default:
			if (p < LINESIZE) {
				if (p < blen && buf[p] != ' ') {
					if (c == '_' || buf[p] == '_') {
						ubuf[p] = '_';
						if (p >= ulen)
							ulen = p + 1;
					}
					if (c == '_')
						c = buf[p];
				}
				buf[p++] = c;
				if (p > blen)
					blen = p;
			}
			break;

		}
	}

	/* if we're not exactly at page end, start a new page */
	if (dots % PAGE)
		putc ('\f', lpf);

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
	fprintf(pmail, "Your %s job for file %s is finished, bignose.\n", DAEMNAM, mailfname);
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
		dots = align (dots, GRAIN) + LDOTS;
		putc ('\b', lpf);
		break;

	case plot:
		dots += PDOTS;
		putc ('\005', lpf);
		while (blen > 0 && (buf[blen-1] & 077) == 0)
			--blen;
		break;
	
	case normal:
		dots = align (dots, GRAIN) + NDOTS;
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
	if (c == '\f')
		dots = 0;

	if (ferror (lpf))
		trouble ("Output error on %s\n", lp);
}
