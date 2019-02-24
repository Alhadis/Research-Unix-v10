#include	<stdio.h>
#include	<ctype.h>
#include	"../comm.h"
#include	"host.h"

int hpos, vpos;
static blith, blitv;

readpage()
{
	register int c;
	int m, n, init = 1, gonow = 1;
	char str[4096], buf[300];

	while(gonow)
	{
		c = getch();
		switch(c)
		{
		case EOF:
			input.eof = 1;
			gonow = 0;
			break;
		case '\n':	/* when input is text */
		case ' ':
		case 0:		/* occasional noise creeps in */
			break;
		case '0': case '1': case '2': case '3': case '4':
		case '5': case '6': case '7': case '8': case '9':
			/* two motion digits plus a character */
			hpos += (c-'0')*10 + getch()-'0';
			buf[0] = getch();
			buf[1] = 0;
			dochar(buf);
			break;
		case 'c':	/* single ascii character */
			buf[0] = getch();
			buf[1] = 0;
			dochar(buf);
			break;
		case 'C':
			getstr(str);
			dochar(str);
			break;
		case 'N':
			str[0] = getn() & 0x7F;
			str[1] = 0;
			dochar(str);
			break;
		case 't':	/* straight text */
			eatline();
			break;
		case 'D':	/* draw function */
			xygoto();
			out(C_DRAW);
			switch(getch())
			{
			case 'l':	/* draw a line */
				out(LINE);
				outn(n = getn());
				outn(m = getn());
				hpos += n;
				vpos += m;
				break;
			case 'c':	/* circle */
				out(CIRCLE);
				outn(m = getn());
				hpos += 2*m;
				break;
			case 'e':	/* ellipse */
				out(ELLIPSE);
				outn(getn());
				outn(getn());
				break;
			case 'a':	/* arc */
				out(ARC);
				outn(n = getn());
				outn(m = getn());
				hpos += n;
				vpos += m;
				outn(n = getn());
				outn(m = getn());
				hpos += n;
				vpos += m;
				break;
			case '~':	/* wiggly line */
				wiggly();
				break;
			default:
				break;
			}
			eatline();
			break;
		case 's':
			n = getn();	/* ignore fractional sizes */
			if(cursize == n)
				break;
			cursize = n;
	setsize:
			if((n >= SIZE_ORIG) && (n <= (C_MAX-C_SIZE+SIZE_ORIG)))
				out(C_SIZE+n-SIZE_ORIG);
			else
			{
				out(C_BSIZE);
				out(n);
			}
			break;
		case 'f':
			curfont = getn();
			break;
		case 'H':	/* absolute horizontal motion */
			hpos = getn();
			break;
		case 'h':	/* relative horizontal motion */
			hpos += getn();
			break;
		case 'w':	/* word space */
			break;
		case 'V':
			vpos = getn();
			break;
		case 'v':
			vpos += getn();
			break;
		case 'p':	/* new page */
			if(init-- == 0)
			{
				ungetch('p');
				gonow = 0;
			}
			else
			{
				initpage(getn());
				yset();
				out(C_FONT+curfont);
				n = cursize;
				goto setsize;
			}
			break;
		case '#':	/* comment */
		case 'n':	/* end of line */
			eatline();
			break;
		case 'x':	/* device control */
			devcntrl();
			break;
		default:
			sprintf(buf, "unknown input character %o %c\n", c, c);
			quit(buf, 1);
		}
	}
	if(init > 0)
		lastpage = LARGE;
	else
		input.pages[lastpage].end = input.next;
}

wiggly()
{
	int pts[200], i, n;

	for(pts[0] = getn(), i = 1; (n = getch()) != '\n';)
	{
		ungetch(n);
		pts[i++] = getn();
	}
	out(SPLINE);
	out(i/2);
	for(n = 0; n != i; n++)
		outn(pts[n]);
}

devcntrl()	/* interpret device control functions */
{
        char str[4096];
	int n;

	getstr(str);
	switch (str[0]) {	/* crude for now */
	case 'i':	/* initialize */
		break;
	case 'T':	/* device name */
		getstr(devname);
		break;
	case 't':	/* trailer */
		break;
	case 'p':	/* pause -- can restart */
		break;
	case 's':	/* stop */
		break;
	case 'r':	/* resolution assumed when prepared */
		out(C_DEV);
		out(RES);
		outn(getn());
		break;
	case 'f':	/* font used */
		out(C_DEV);
		out(FONT);
		out(n = getn());
		getstr(str);
		outs(str);
		loadfont(n, str);
		break;
	/* these don't belong here... */
	case 'H':	/* char height */
		break;
	case 'S':	/* slant */
		break;
	case 'X':
		out(C_DEV);
		out(MODE);
		out(getn()%4);
		break;
	}
	eatline();
}

yset()
{
	vpos = 0;
	blitv = 0;
}

xygoto()
{
	if(blitv != vpos)
	{
		out(C_ABSV);
		outn(vpos);
		blitv = vpos;
	}
	if(blith != hpos)
	{
		if((blith < hpos) && (blith+99 >= hpos))
			out(C_HOR + hpos-blith);
		else
		{
			out(C_ABSH);
			outn(hpos);
		}
		blith = hpos;
	}
}

getch()
{
	return(getchar());
}

ungetch(c)
{
	(void)ungetc(c, stdin);
}

getstr(s)
	register char *s;
{
	for(*s = getch(); isspace(*s); *s = getch());
	for(; !isspace(*s); *++s = getch());
	ungetch(*s);
	*s = 0;
}

eatline()
{
	while(getch() != '\n');
}

getn()
{
	register n, c, sign;

	while(c = getch())
		if(!isspace(c)) break;
	if(c == '-')
	{
		sign = -1;
		c = getch();
	}
	else
		sign = 1;
	for(n = 0; isdigit(c); c = getch())
		n = n*10 + c - '0';
	while(c == ' ') c = getch();
	ungetch(c);
	return(n*sign);
}
