#
/*
 *
 *	UNIX debugger
 *
 */

#include "defs.h"


int	infile = STDIN;
int	outfile = STDOUT;
int	maxpos = MAXPOS;
int	radix = 0;	/* magic multi-radix */

static	char	printbuf[MAXLIN];
static	char	*printptr = printbuf;
static	char	*digitptr;

printc(c)
char		c;
{
	char	d;
	register char 	*q;
	register int p;
	int	posn, tabs;

	if (mkfault)
		return;
	else if ((*printptr=c)==EOR) {
		tabs=0;
		posn=0;
		q=printbuf;
		for (p=0; p<printptr-printbuf; p++) {
			d=printbuf[p];
			if ((p&7)==0 && posn) {
				tabs++;
				posn=0;
			}
			if (d==SPC)
				posn++;
			else {
				while (tabs>0) {
					*q++=TB;
					tabs--;
				}
				while (posn>0) {
					*q++=SPC;
					posn--;
				}
				*q++=d;
			}
		}
		*q++=EOR;
		write(outfile,printbuf,q-printbuf);
		printptr=printbuf;
	} else if (c==TB) {
		*printptr++=SPC;
		while ((printptr-printbuf)&7)
			*printptr++=SPC;
	} else if (c)
		printptr++;
	if (printptr >= &printbuf[MAXLIN-9]) {
		write(outfile, printbuf, printptr - printbuf);
		printptr = printbuf;
	}
}

charpos()
{
	return(printptr-printbuf);
}

flushbuf()
{
	if (printptr!=printbuf)
		printc(EOR);
}

/*
 * our very own printf.  yes.
 * `int' things (like %d) expect ints
 * `long' things (like %D) expect WORDs
 * there is no %f, for reasons to horrible to explain here
 *
 * declare lots of silly arguments because of the cray's calling sequence
 */

printf(fmat, a1, a2, a3, a4, a5, a6, a7, a8, a9)
char *fmat;
int a1;
{
	register int *ap;
	register char *s;
	register int n, width;
	int prec;
	char *fptr;
	int *ip;
	WORD *lp;
	char **sp;
	char c, adj;
	char digits[64];

	fptr = fmat;
	ap = &a1;		/* hack */
	while (c = *fptr++) {
		if (c!='%') {
			printc(c);
			continue;
		}
		adj = 'r';
		if (*fptr=='-') {
			adj = 'l';
			fptr++;
		}
		width=convert(&fptr);
		prec = -1;
		if (*fptr=='.') {
			fptr++;
			prec=convert(&fptr);
		}
		digitptr=digits;
		s = NULL;
		ip = (int *)ap;
		lp = (long *)ap;
		sp = (char **)ap;
		switch (c = *fptr++) {

		case 'd':
			printsigned((WORD)*ip++, 10);
			ap = (int *)ip;
			break;

		case 'D':
			printsigned((WORD)*lp++, 10);
			ap = (int *)lp;
			break;

		case 'u':
			printus((WORD)*ip++, 10);
			ap = (int *)ip;
			break;

		case 'U':
			printus((WORD)*lp++, 10);
			ap = (int *)lp;
			break;

		case 'o':
			printus((WORD)*ip++, 8);
			ap = (int *)ip;
			break;

		case 'O':
			printus((WORD)*lp++, 8);
			ap = (int *)lp;
			break;

		case 'q':
			printsigned((WORD)*ip++, 8);
			ap = (int *)ip;
			break;

		case 'Q':
			printsigned((WORD)*lp++, 8);
			ap = (int *)lp;
			break;

		case 'x':
			printus((WORD)*ip++, 16);
			ap = (int *)ip;
			break;

		case 'X':
			printus((WORD)*lp++, 16);
			ap = (int *)lp;
			break;

		case 'r':
			printmaybe((WORD)*ip++, radix == 0 ? DEFRADIX : radix);
			ap = (int *)ip;
			break;

		case 'R':
			printmaybe((WORD)*lp++, radix == 0 ? DEFRADIX : radix);
			ap = (int *)lp;
			break;

		case 'c':
			printc((char)*ap++);
			break;

		case 's':
			s = *sp++;
			ap = (int *)sp;
			break;

		/* m is for `move' */
		case 'm':
			break;

		case 'M':
			width = *ap++;
			break;

		case 'T':
			width = *ap++;
			/* fall in */
		case 't':
			if (width)
				width -= charpos()%width;
			break;

		default:
			printc(c);
			break;
		}
		if (s == NULL) {
			*digitptr = 0;
			s = digits;
		}
		n = strlen(s);
		if (prec >= 0 && prec < n)
			n = prec;
		width -= n;
		if (adj=='r')
			while (width-- > 0)
				printc(SPC);
		while (n--)
			printc(*s++);
		while (width-- > 0)
			printc(SPC);
		digitptr=digits;
	}
}

prints(s)
char *s;
{
	printf("%s",s);
}

newline()
{
	printc(EOR);
}

convert(cp)
register char **cp;
{
	register char c;
	register int n;

	n=0;
	while (isdigit(c = *(*cp)++))
		n=n*10+c-'0';
	(*cp)--;
	return(n);
}

printmaybe(n, base)
WORD n;
{
	if (REALLYNEG(n))
		printsigned(n, base);
	else
		printus(n, base);
}

printsigned(n, base)
WORD n;
int base;
{

	if (n < 0) {
		n = -n;
		/* if (n < 0)  what? */
		*digitptr++ = '-';
	}
	printnum((unsigned long)n, base);
}

printus(n, base)
WORD n;
int base;
{

	printnum((unsigned long)n, base);
}

printnum(n, base)
unsigned long n;
int base;
{
	unsigned long d;
	register int r;
	static char digits[] = "0123456789abcdef";

	if ((d = n / base) != 0) {
		printnum(d, base);
		r = n - (d * base);
	}
	else {			/* hack for first digit */
		r = n;
		if (base == radix) {
			if (r >= 10)
				*digitptr++ = '0';
		}
		else if (radix == 0)
			switch (base) {
			case 16:
				*digitptr++ = '#';
				break;

			case 8:
				if (r)
					*digitptr++ = '0';
				break;
			}
	}
	if (r < sizeof(digits) - 1)
		*digitptr++ = digits[r];
	else
		*digitptr++ = '?';
}

#define	MAXIFD	5
struct {
	int	fd;
	int	r9;
} istack[MAXIFD];
int	ifiledepth;

iclose(stack, err)
{
	if (err) {
		if (infile) {
			close(infile);
			infile=STDIN;
		}
		while (--ifiledepth >= 0)
			if (istack[ifiledepth].fd)
				close(istack[ifiledepth].fd);
		ifiledepth = 0;
	} else if (stack == 0) {
		if (infile) {
			close(infile);
			infile=STDIN;
		}
	} else if (stack > 0) {
		if (ifiledepth >= MAXIFD)
			error("$<< nested too deeply");
		istack[ifiledepth].fd = infile;
		istack[ifiledepth].r9 = var[9];
		ifiledepth++;
		infile = STDIN;
	} else {
		if (infile) {
			close(infile); 
			infile=STDIN;
		}
		if (ifiledepth > 0) {
			infile = istack[--ifiledepth].fd;
			var[9] = istack[ifiledepth].r9;
		}
	}
}

oclose()
{
	if (outfile!=1) {
		flushbuf();
		close(outfile);
		outfile=STDOUT;
	}
}

endline()
{

	if (maxpos <= charpos())
		newline();
}
