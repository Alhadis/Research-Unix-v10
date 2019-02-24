#include <stdio.h>

#define PAGELEN	66

#define CMASK	0x00ff	/* the ascii part	*/
#define FMASK	0x7f00	/* (most of) the rest	*/
#define UNDER	0x0100	/* char is underlined	*/
#define BOLD	0x0200	/* char is emboldened	*/
#define EXP	0x0400	/* pitch is expanded	*/
#define COMP	0x0800	/* pitch is compressed	*/
#define EIGHT	0x1000	/* 8 lines per inch	*/
#define OVER	0x8000	/* char is overstruck	*/

#define AGAIN	0x10000

#define CTL(c)	((c) & 0x1f)
#define ESC	033

#define Malloc(type,n)		(type *)malloc((n)*sizeof(type))
#define Realloc(type,ptr,n)	(type *)realloc(ptr,(n)*sizeof(type))

char *malloc(), *realloc();

#define WINSIZE	256
#define LINSIZE	128

typedef unsigned short Char;

typedef struct Line {
	Char *data, *cp, *np, *sizep;
	int col;
} Line;

static Line page[WINSIZE];

static int line, col, wintop, winbot, lastin, lastout, nlin, nlout;

static char escbuf[64];

#define Getc()	(--nchin > 0 ? *inp++ : (inp = getbuf()) ? *inp++ : -1)

extern int nchin; unsigned char *getbuf();

thinksort(inp)
register unsigned char *inp;
{
	register c = 0, d = 0, s = 0, flags = 0;
	register char *p;

	winbot = WINSIZE;
	nlin = 1;
	while ((c & AGAIN ? (c &= ~AGAIN) : (c = Getc())) >= 0) switch(c) {
	case 0:
		break;
	case '\b':	/* ^h */
		if (col > 0)
			col--;
		break;
	case '\t':	/* ^i */
		col += 8 - col%8;
		break;
	case '\n':	/* ^j */
		col = 0;
		nlin++;
		dline(2);
		break;
	case CTL('l'):
		dline(PAGELEN - line%PAGELEN);
		break;
	case '\r':	/* ^m */
		col = 0;
		break;
	case CTL('n'):
		flags |= BOLD;
		break;
	case CTL('o'):
		flags &= ~BOLD;
		break;
	case ESC:
		p = escbuf;
		if ((c = Getc()) < 0)
			break;
		*p++ = c;
		if (c != 'z')
			while ((c < '<' || c > 'Z') && (c = Getc()) >= 0)
				if (p < &escbuf[(sizeof escbuf) - 1])
					*p++ = c;
		*p = 0;
		if (strcmp(escbuf, "&dD") == 0)
			flags |= UNDER;
		else if (strcmp(escbuf, "&d@") == 0)
			flags &= ~UNDER;
		else if (strcmp(escbuf, "=") == 0)
			dline(1);
		else if (strcmp(escbuf, "<") == 0)
			dline(-1);
		else if (strcmp(escbuf, "&k0S") == 0)
			flags &= ~(EXP|COMP);
		else if (strcmp(escbuf, "&k1S") == 0)
			flags &= ~COMP, flags |= EXP;
		else if (strcmp(escbuf, "&k2S") == 0)
			flags &= ~EXP, flags |= COMP;
		else if (strcmp(escbuf, "&k3S") == 0)
			flags |= EXP|COMP;
		else if (strcmp(escbuf, "&l6D") == 0)
			flags &= ~EIGHT;
		else if (strcmp(escbuf, "&l8D") == 0)
			flags |= EIGHT;
		else if (strcmp(escbuf, "(s0B") == 0)
			flags &= ~BOLD;
		else if (strcmp(escbuf, "(s1B") == 0)
			flags |= BOLD;
		else
			fprintf(stderr,
			    "ignored escape sequence \"%s\", line %d\n",
			    escbuf, nlin);
		break;
	case ' ':
		col++;
		break;
	case '_':
		outc(' '|UNDER|flags), col++;
		break;
	default:
		if ((d = Getc()) == '\b') {
			if ((s = digram(c, (d = Getc()))) > 0)
				outc(s|flags), col++, d = 0;
			else
				outc(c|flags);
		} else
			outc(c|flags), col++;
		if (d > 0)
			c = d | AGAIN;
		break;
	}
	for (c = max(lastout, wintop); c <= lastin; c++)
		outl(c);
	if (nlout > 0) {
		wrts("\n");
		outflags(' '|flags, FMASK);
		wrtflush();
	}
}

dline(n)
register n;
{
	if (n > 0) while (--n >= 0) {
		if (++line > lastin)
			lastin = line;
		if (line >= winbot)
			outl(wintop++), winbot++;
	} else
		line += n;
}

outc(c)
{
	register Line *lp; register Char *cp;
	if (line < wintop)
		return;
	lp = page + line%WINSIZE;
	if (col < 0)
		col = lp->col + 1;
	if (lp->col > col)
		lp->cp = lp->data, lp->col = 0;
	while (lp->col < col) {
		if (lp->cp >= lp->sizep)
			lalloc(lp);
		if ((*lp->cp & OVER) == 0)
			++lp->col;
		++lp->cp;
	}
	if (lp->cp >= lp->sizep)
		lalloc(lp);
	if (lp->cp >= lp->np)
		lp->np = lp->cp + 1;
	if (*lp->cp) {
		if (lp->np >= lp->sizep)
			lalloc(lp);
		for (cp = lp->np - 1; cp >= lp->cp; cp--)
			cp[1] = cp[0];
		++lp->np;
		c |= OVER;
	}
	*lp->cp = c;
}

outl(line)
{
	int needcr = 1;
	register Line *lp; register Char *cp;
	register c;

	lp = page + line%WINSIZE;
	if (lp->np <= lp->data)
		return;
	if (nlout++ == 0)
		wrts("\033E");
	if (*(cp = lp->data) == 0)
		while (++cp < lp->np)
			if (*cp) {
				*lp->data = ' ' | (*cp & FMASK & ~UNDER);
				break;
			}
	outflags(*cp, EIGHT);
	while (lastout < line - 1)
		wrtc('\n'), lastout += 2, needcr = 0;
	if (lastout != line)
		wrts(needcr ? "\r\033=" : "\033="), lastout++;
	for (cp = lp->data; cp < lp->np; cp++) {
		c = outflags(*cp, FMASK);
		if (c & CMASK)
			wrtc(c & CMASK);
		if (c & OVER)
			wrtc('\b');
	}
	lfree(lp);
}

outflags(c, mask)
register c, mask;
{
	static int isunder, isbold, ispitch, isline;
	static char begunder[] = "\033&dD", endunder[] = "\033&d@";
	static char begbold[] = "\016\033(s1B", endbold[] = "\017\033(s0B";
	static char pitchctl[] = "\033&k0S";
	static char linectl[] = "\033&l0D";
	register i;

	if (c == 0)
		c = ' ' | isbold | ispitch | isline;
	if ((UNDER & mask) && (i = c & UNDER) != isunder)
		wrts(i ? begunder : endunder), isunder = i;
	if ((BOLD & mask) && (i = c & BOLD) != isbold)
		wrts(i ? begbold : endbold), isbold = i;
	if (((EXP|COMP) & mask) && (i = c & (EXP|COMP)) != ispitch) {
		pitchctl[3] = i/EXP + '0';
		wrts(pitchctl), ispitch = i;
	}
	if ((EIGHT & mask) && (i = c & EIGHT) != isline) {
		linectl[3] = 2*(i/EIGHT) + '6';
		wrts(linectl), isline = i;
	}
	return c;
}

lalloc(lp)
register Line *lp;
{
	register Char *dp; register size;
	size = (lp->sizep - lp->data) + LINSIZE;
	dp = lp->data ? Realloc(Char, lp->data, size) : Malloc(Char, size);
	lp->sizep = dp + size;
	lp->cp += dp - lp->data;
	lp->np += dp - lp->data;
	lp->data = dp;
	dp = lp->np;
	while (dp < lp->sizep)
		*dp++ = 0;
}

lfree(lp)
register Line *lp;
{
	static Line lnull;
	if (lp->data)
		free(lp->data);
	*lp = lnull;
}

static char *classes[] = {

 "`a\310e\311i\331o\312u\313A\241E\243I\346O\350U\255",

 "'a\304e\305i\325o\306u\307A\340E\334I\345O\347U\355",

 "^a\300e\301i\321o\302u\303A\242E\244I\246O\337U\256s\354S\353",

"\"a\314e\315i\335o\316u\317A\330E\245I\247O\332U\333y\357Y\356",

 "~a\342o\352n\267A\341O\351N\266!\270?\271",

 ",c\265C\264", "/o\326O\322", "\263a\324A\320",

 "ae\327", "AE\323", "<<\373", ">>\375",

 "-L\273", "=L\257Y\274",

 0 };

digram(c, d)
{
	register unsigned char **l, *p;
	for (l=(unsigned char **)&classes[0]; *l; l++)
		if (*(p = *l) == c)
			while (*++p)
				if (*p++ == d)
					return *p;
	return -1;
}
