char	dpd[]	= "/usr/spool/lab";
char	dfname[30] = "/usr/spool/lab/";
char	lock[]	= "/usr/spool/lab/lock";
char	lp[] = "/dev/lab";

/*
 *	lab - label maker dispatcher for DataSouth DS180 matrix printer
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
	tb.sg_flags |= XTABS|TANDEM;
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
	/* banner(lpf, &line[1]); */
}


FILE	*ibuf;

sascii(fc)
char	fc;
{
	register int c, nblank;

	if((ibuf = fopen(&line[1], "r")) == NULL)
		return(0);

	putc(ff, lpf);

	nblank = 0;
	while ((c = getc (ibuf)) != EOF) {
		switch (c) {
		case ' ':
			nblank++;
			break;

		case '-':
			if (nblank >= 1) {
				c = getc (ibuf);
				if (c == '\n') {
					putc ('\r', lpf);
					c = ff;
				} else
					putc ('-', lpf);
			}
			/* no break */
		default:
			nblank = 0;
		}
		putc (c, lpf);
	}

	for (c = 0; c < 6; c++)
		putc (ff, lpf);

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
	fprintf(pmail, "Your %s job for file %s is finished, Jim.\n", DAEMNAM, mailfname);
}
