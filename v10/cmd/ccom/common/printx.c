#include <stdio.h>

#define BSIZE	4096
#define STRSIZE	1024
#define BTOTAL	(BSIZE+STRSIZE)

int nosharp, syncstdio;

static char buffer[BTOTAL];
static char *bufpt = buffer;

char *sprintxl(), *sprintd(), *sprinto(), *sprintX();

/*
main()
{
	printx("string: %s, decimals: %d, %d, %d, %d\npercent: %%, sharp: %#\n",
	    "this is it", 123456, 2147483647, -2147483647, -2147483648);
	printx("octal: %o, hex: %x, char: %c\n",
	    0123456, 0x123456, 'z');
	exit(0);
}
*/

printx(fmt, list)
char *fmt; long list;
{
	if (bufpt >= &buffer[BSIZE])
		flushx();
	bufpt = sprintxl(bufpt, fmt, &list);
	if (syncstdio)
		flushx();
}

printbuf(p, n)
register char *p; register int n;
{
	register int ncp;
	while (n > 0) {
		if (bufpt >= &buffer[BTOTAL])
			flushx();
		ncp = &buffer[BTOTAL] - bufpt;
		if (n < ncp)
			ncp = n;
		memcpy(bufpt, p, ncp);
		bufpt += ncp;
		p += ncp;
		n -= ncp;
	}
	if (syncstdio)
		flushx();
}

flushx()
{
	register int ncp;
	if (ncp = bufpt - buffer)
		if (syncstdio)
			fwrite(bufpt = buffer, sizeof(char), ncp, stdout);
		else
			write(1, bufpt=buffer, ncp);
}

char *
sprintx(str, fmt, list)
char *str, *fmt; long list;
{
	return sprintxl(str, fmt, &list);
}

char *
sprintxl(str, fmt, lp)
register char *str, *fmt; register long *lp;
{
	register c; register char *p; char *sprintd();
	while (c = *fmt++) switch (c) {
	case '%': casePCT:
		switch (c = *fmt++) {
		case 'd':
			str = sprintd(str, *lp++); break;
		case 's':
			for (p = (char *)(*lp++); *p;)
				*str++ = *p++;
			break;
		case 'o':
			str = sprinto(str, *lp++); break;
		case 'x':
			str = sprintX(str, *lp++); break;
		case 'c':
			*str++ = *lp++; break;
		case '%':
		case '#':
			*str++ = c; break;
		case 'l':
			goto casePCT;
		default:
			*str++ = '%'; *str++ = c;
			if (c == 0) return --str;
			break;
		}
		break;
	case '#':
		if (nosharp == 0) *str++ = c; break;
	default:
		*str++ = c; break;
	}
	*str = 0;
	return str;
}

static int pwrd[] = {
	0, 10, 100, 1000, 10000, 100000, 1000000,
	10000000, 100000000, 1000000000, 0xffffffff,
};

static char mostneg[] = "-2147483648";

char *
sprintd(str, x)
register char *str; register int x;
{
	register int *p = &pwrd[1], d;

	if (x < 0) {
		x = -x;
		if (x < 0) {
			memcpy(str, mostneg, sizeof mostneg);
			return str + (sizeof mostneg) - 1;
		}
		*str++ = '-';
	}
	while ((unsigned)(*p++) <= (unsigned)x)
		/* void */;
	--p;
	while (*--p) {
		d = x / *p;
		x -= d * *p;
		*str++ = d + '0';
	}
	*str++ = x + '0';
	*str = 0;
	return str;
}

static unsigned int pwro[] = {
	010, 0100, 01000, 010000, 0100000, 01000000,
	010000000, 0100000000, 01000000000, 010000000000, 0,
};

char *
sprinto(str, x)
register char *str; register unsigned int x;
{
	register int i = 0;
	while (pwro[i] && (pwro[i] <= x))
		++i;
	i *= 3;
	do
		*str++ = ((x>>i)&07) + '0';
	while ((i -= 3) >= 0);
	*str = 0;
	return str;
}

static unsigned int pwrx[] = {
	0x10, 0x100, 0x1000, 0x10000, 0x100000, 0x1000000,
	0x10000000, 0,
};

static char hexit[] = {
	'0', '1', '2', '3', '4', '5', '6', '7',
	'8', '9', 'a', 'b', 'c', 'd', 'e', 'f',
};

char *
sprintX(str, x)
register char *str; register unsigned int x;
{
	register int i = 0;
	while (pwrx[i] && (pwrx[i] <= x))
		++i;
	i *= 4;
	do
		*str++ = hexit[(x>>i)&0xf];
	while ((i -= 4) >= 0);
	*str = 0;
	return str;
}
