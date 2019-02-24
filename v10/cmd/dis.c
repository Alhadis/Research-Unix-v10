/*
**	Dis -- VDU page display program
**
**	"dis [-t<timeout>] [-c<refresh count>] [-u]"
*/

#include	<signal.h>
#include	<setjmp.h>
#ifdef	TERMCAP
#include	<sgtty.h>
#endif	TERMCAP
#include	<stdio.h>
#include	"/usr/jerq/include/jioctl.h"

/*
**	Parameters
*/

#define		MAXWID		132
#define		MAXLEN		80


/*
**	Screen buffer
*/

char *		Buf;

/*
**	Term Cap
*/

char		*CM, *CL;
short		amflag;

#ifdef	TERMCAP
extern short	ospeed;
extern char	PC, *BC, *UP;
#endif	TERMCAP

extern char *	tgoto();
extern char *	tgetstr();

/*
**	Screen macro
*/

#define		putcm(cp,p,c)		if ( *cp++ != c ) \
					{ \
						if ( move((--cp)-p) ) \
						{ \
							putc(c,stdout); \
							Flush = 1; \
						} \
						*cp++ = c; \
					}

/*
**	Miscellaneous
*/

short		Flush;
short		Length;		/* length - 1 */
short		Width;		/* width - 1 */
jmp_buf		alrmbuf;
short		length;
char *		name;
short		rcount;
short		width;
unsigned	timeout;
struct winsize	jwinsize;

void
		dis(),
		outc(),
		tcinit(),
		terror(),
		warn();

int		alrmcatch();

/*
**	Externals
*/

extern char	_sobuf[];
extern char *	tgetstr();
extern char *	malloc();
extern char *	getenv();



main(argc, argv)
	register int		argc;
	register char *		argv[];
{
	register char *		cp;
	register unsigned	size;

	name = *argv++;
	argc--;

	while ( argc )
	{
		switch( argv[0][0] )
		{
		 case '-':	argv[0]++;
				continue;
		 case 't':	timeout = atoi(&argv[0][1]);
				break;
		 case 'c':	rcount = atoi(&argv[0][1]);
				break;
		 case 'u':	setbuf(stdin, NULL);
				break;
		 default:	fprintf(stderr, "%s: bad arg - %s\n", name, argv[0] );
				return 1;
		}
		argc--;
		argv++;
	}

	setbuf(stdout, _sobuf);

	tcinit();

#	ifdef	JWINSIZE
	if ( ioctl(1, JWINSIZE, &jwinsize) != -1 )
	{
		length = jwinsize.bytesy;
		width = jwinsize.bytesx;
	}
#	endif

	size = length * width;
	if ( (Buf = malloc(size)) == (char *)0 )
	{
		fprintf(stderr, "No memory\n");
		exit(2);
	}
	for (cp = &Buf[size]; cp > Buf; *--cp = '\0');

	Length = length - 1;
	Width = width - 1;

	dis(size);

	setbuf(stdout, (char *)0);

	return 0;
}




void
dis(size)
	unsigned		size;
{
	register char *		ep;
	register char *		p = Buf;
	register int		c;
	register int		line;
	/** on stack to avoid setjmp **/
	char *			cp;
	char *			lastend;
	int			rc;

	lastend = p;
	rc = rcount;

	do
	{
		line = 0;
		cp = p;
		ep = cp+width;

		if ( rcount && --rc == 0 )
		{
			tputs(CL, 1, outc);
			for (c = size; c > 0; --c, *p++ = '\0');
			p = cp;
			rc = rcount;
		}

		if ( timeout == 0 || setjmp(alrmbuf) == 0 )
		{
			if ( timeout )
			{
				signal(SIGALRM, alrmcatch);
				alarm(timeout);
			}
			while ( (c = getchar()) != EOF )
			{
				if ( c < ' ' )
				{
					switch ( c )
					{
					 case '\f':	if ( cp != p )
								break;
							continue;
					 case '\t':	if ( cp >= ep )
								continue;
							c = cp - &p[line*width] + 1;
							putcm(cp, p, ' ');
							while ( c++&7 )
								putcm(cp, p, ' ');
							continue;
					 case '\n':	while ( cp < ep )
								putcm(cp, p, ' ');
							if ( line < Length )
							{
								cp = &p[(++line)*width];
								ep = cp+width;
							}
							continue;
					 default:
							if ( cp < ep )
								putcm(cp, p, '?');
							continue;
					}
				}
				else
				{
					if ( cp < ep )
						putcm(cp, p, c);
					continue;
				}
				break;
			}
			if ( timeout )
				alarm(0);
		}

		if ( !Flush )
			continue;
		Flush = 0;

		ep = cp - 1;
		while ( cp < lastend )
			putcm(cp, p, ' ');
		lastend = ep;
		if ( (line = (ep-p)/width) < Length )
			line++;
		(void)move(line*width);

		fflush(stdout);
	}
	while
		( c != EOF );
}




int
move(pos)
	register int	pos;
{
	register int	x = pos%width;
	register int	y = pos/width;
	register int	i;
	static int	oy, ox = -1;

	if ( oy == y )
	{
		if ( (i = x - ox) != 1 )
		{
			if ( i <= 3 && i > 0 )
			{
				i--;
				pos -= i;
				do
					putc(Buf[pos++], stdout);
				while
					( --i > 0 );
			}
			else
				tputs(tgoto(CM, x, y), 1, outc);

			Flush = 1;
		}
	}
	else
	{
		if ( oy == (y-1) && x == 0 )
		{
			if ( ox != Width || !amflag )
				putc('\n', stdout);
		}
		else
			tputs(tgoto(CM, x, y), oy<y?y-oy:oy-y, outc);

		Flush = 1;
	}

	ox = x; oy = y;
	if ( y==Length && x==Width && amflag )
		return 0;
	return 1;
}




int
alrmcatch()
{
	longjmp(alrmbuf, 1);
}




void
tcinit()
{
	register char *	cp;
#ifdef	TERMCAP
	struct sgttyb	gb;
#endif	TERMCAP
	char		bp[1024];
	static char	buf[100];
	char		*area = buf;

	if ( tgetent(bp, getenv("TERM")) != 1 )
		terror("no \"termcap\" entry");
	if ( (CL = tgetstr("cl", &area)) == (char *)0
	  || (CM = tgetstr("cm", &area)) == (char *)0 )
		terror("no cursor addressing");
#ifdef	TERMCAP
	UP = tgetstr("up", &area);
	BC = tgetstr("bc", &area);
#endif	TERMCAP
	if ( tgetflag("am") == 1 )
		amflag++;
	if ( (cp = getenv("WIDTH")) != (char *)0 )
		width = atoi(cp);
	else
		width = tgetnum("co");
	if ( width > MAXWID )
	{
		width = MAXWID;
		warn("width truncated");
	}
	if ( (length = tgetnum("li")) > MAXLEN )
	{
		length = MAXLEN;
		warn("length truncated");
	}

#ifdef	TERMCAP
	if ( (cp = tgetstr("pc", &area)) != (char *)0 )
		PC = *cp;
	gtty(1, &gb);
	ospeed = gb.sg_ospeed;
#endif	TERMCAP

	tputs(CL, 1, outc);
}




void
outc(c)
{
	putc(c, stdout);
}




void
warn(s)
	char *	s;
{
	fprintf(stderr, "Warning: %s\n", s);
	sleep(2);
}




void
terror(s)
	char *	s;
{
	fprintf(stderr, "Terminal capability error - %s\n", s);
	exit(1);
}
