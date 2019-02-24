#define	SIZE	1024
#define	FUNSIGN	4
#define	FSHORT	2
#define	FLONG	1
#define	PTR	sizeof (char *)
#define	SHORT	sizeof (int)
#define	INT	sizeof (int)
#define	LONG	sizeof (long)
#define	FLOAT	sizeof (double)
#define	FDIGIT	30
#define	FDEFLT	8
#define	IDIGIT	40
#define	MAXCONV	30

static char	*out, *eout;
static		convcount  = { 13 };

static	noconv();
static	cconv(), dconv(), hconv(), lconv();
static	oconv(), sconv(), uconv(), xconv();

static	econv(), fconv(), gconv(), percent();
int	printcol;
static
int	(*fmtconv[MAXCONV])() =
{
	noconv,
	cconv, dconv, hconv, lconv,
	oconv, sconv, uconv, xconv,
	econv, fconv, gconv, percent,
};
static
char	fmtindex[128] =
{
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0,12, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 1, 2, 9,10,11, 3, 0, 0, 0, 4, 0, 0, 5,
	0, 0, 0, 6, 0, 7, 0, 0, 8, 0, 0, 0, 0, 0, 0, 0,
};

fmtinstall(c, f)
char c;
int (*f)();
{

	c &= 0177;
	if(fmtindex[c] == 0) {
		if(convcount >= MAXCONV)
			return 1;
		fmtindex[c] = convcount++;
	}
	fmtconv[fmtindex[c]] = f;
	return 0;
}

char*
donprint(s, es, fmt, argp)
char *s, *es;
register char *fmt;
char *argp;
{
	register int f1, f2, f3, sf1, c;
	char *sout, *seout;

	sout = out;
	seout = eout;
	out = s;
	eout = es-1;
loop:
	c = *fmt++;
	if(c != '%') {
		if(c == 0) {
			*out = 0;
			s = out;
			out = sout;
			eout = seout;
			return s;
		}
		if(out < eout)
			*out++ = c;
		printcol++;
		if(c == '\n')
			printcol = 0; else
		if(c == '\t')
			printcol = (printcol+7) & ~7;
		goto loop;
	}
	f1 = 0;
	f2 = -1;
	f3 = 0;
	c = *fmt++;
	sf1 = 0;
	if(c == '-') {
		sf1 = 1;
		c = *fmt++;
	}
	while(c >= '0' && c <= '9') {
		f1 = f1*10 + c-'0';
		c = *fmt++;
	}
	if(sf1)
		f1 = -f1;
	if(c != '.')
		goto l1;
	c = *fmt++;
	while(c >= '0' && c <= '9') {
		if(f2 < 0)
			f2 = 0;
		f2 = f2*10 + c-'0';
		c = *fmt++;
	}
l1:
	if(c == 0)
		fmt--;
	c = (*fmtconv[fmtindex[c&0177]])(argp, f1, f2, f3);
	if(c < 0) {
		f3 |= -c;
		c = *fmt++;
		goto l1;
	}
	argp += c;
	goto loop;
}

numbconv(o, f1, f2, f3, b)
char *o;
{
	char s[IDIGIT];
	register long v;
	register int i, f, n, r;

	switch(f3 & (FLONG|FSHORT|FUNSIGN)) {
	case FLONG:
		v = *(long*)o;
		r = LONG;
		break;

	case FUNSIGN|FLONG:
		v = *(unsigned long*)o;
		r = LONG;
		break;

	case FSHORT:
		v = *(short*)o;
		r = SHORT;
		break;

	case FUNSIGN|FSHORT:
		v = *(unsigned short*)o;
		r = SHORT;
		break;

	default:
		v = *(int*)o;
		r = INT;
		break;

	case FUNSIGN:
		v = *(unsigned*)o;
		r = INT;
		break;
	}
	f = 0;
	if(!(f3 & FUNSIGN) && v < 0) {
		v = -v;
		f = 1;
	}
	s[IDIGIT-1] = 0;
	for(i = IDIGIT-2; i >= 1; i--) {
		n = (unsigned long)v % b;
		n += '0';
		if(n > '9')
			n += 'a' - ('9'+1);
		s[i] = n;
		v = (unsigned long)v / b;
		if(f2 >= 0 && i >= IDIGIT-f2)
			continue;
		if(v <= 0)
			break;
	}
	if(f)
		s[--i] = '-';
	strconv(s+i, f1, -1);
	return r;
}

char*
doprint(s, fmt, argp)
char *s, *fmt, *argp;
{

	return donprint(s, s+SIZE, fmt, argp);
}

/*
	if you change this, change chconv
*/

strconv(o, f1, f2)
char *o;
{
	register int n, c;
	register char *s;

	n = 0;
	for(s=o; *s++;)
		n++;
	if(f1 >= 0)
		while(n < f1) {
			if(out < eout)
				*out++ = ' ';
			printcol++;
			n++;
		}
	for(s=o; c = *s++;)
		if(f2 != 0) {
			if(out < eout)
				*out++ = c;
			printcol++;
			if(c == '\n')
				printcol = 0; else
			if(c == '\t')
				printcol = (printcol+7) & ~7;
			f2--;
		}
	if(f1 < 0) {
		f1 = -f1;
		while(n < f1) {
			if(out < eout)
				*out++ = ' ';
			printcol++;
			n++;
		}
	}
}

chconv(o, f1)
char o;
{
	register int n;
	register char *s;

	n = 1;
	if(f1 >= 0)
		while(n < f1) {
			if(out < eout)
				*out++ = ' ';
			printcol++;
			n++;
		}
	if(out < eout)
		*out++ = o;
	printcol++;
	if(o == '\n')
		printcol = 0; else
	if(o == '\t')
		printcol = (printcol+7) & ~7;
	if(f1 < 0) {
		f1 = -f1;
		while(n < f1) {
			if(out < eout)
				*out++ = ' ';
			printcol++;
			n++;
		}
	}
}

static
noconv(o, f1, f2, f3)
char *o;
{

	strconv("***", 0, -1);
	return 0;
}

static
cconv(o, f1, f2, f3)
int *o;
{
	chconv(*o, f1);
	return INT;
}

static
dconv(o, f1, f2, f3)
char *o;
{
	int r;

	r = numbconv(o, f1, f2, f3, 10);
	return r;
}

static
hconv(o, f1, f2, f3)
{
	return -FSHORT;
}

static
lconv(o, f1, f2, f3)
{

	return -FLONG;
}

static
oconv(o, f1, f2, f3)
char *o;
{
	int r;

	r = numbconv(o, f1, f2, f3, 8);
	return r;
}

static
sconv(o, f1, f2, f3)
char **o;
{

	strconv(*o, f1, f2);
	return PTR;
}

static
uconv(o, f1, f2, f3)
{
	return -FUNSIGN;
}

static
xconv(o, f1, f2, f3)
char *o;
{
	int r;

	r = numbconv(o, f1, f2, f3, 16);
	return r;
}

double	pow10(), frexp();
fltconv(f, f1, f2, f3, c)
register double f;
{
	char s1[FDIGIT+10], s2[FDIGIT+10];
	register double g;
	register int d, i, n, s;
	double h;
	int e;
	int c1, c2, c3;

	s = 0;
	if(f < 0) {
		f = -f;
		s++;
	}

loop:
	e = 0;
	g = 0;
	if(f != 0) {
		g = frexp(f, &e);
		e = e * .30103;
		d = e/2;
		h = f * pow10(-d);		/* 10**-e in 2 parts */
		g = h * pow10(d-e);
		while(g < 1) {
			e--;
			g = h * pow10(d-e);
		}
		while(g >= 10) {
			e++;
			g = h * pow10(d-e);
		}
	}
	if(f2 < 0)
		f2 = FDEFLT;
	if(c == 'g' && f2 > 0)
		f2--;
	if(f2 > FDIGIT)
		f2 = FDIGIT;
	/*
	 * n is number of digits to convert
	 * 1 before, f2 after, 1 extra for rounding
	 */
	n = f2 + 2;
	if(c == 'f') {
		/*
		 * e+1 before, f2 after, 1 extra
		 */
		n += e;
		if(n <= 0) {
			n = 1;
			g = 0;
		}
	}
	if(n >= FDIGIT+2) {
		if(c == 'e')
			f2 = -1;
		c = 'e';
		goto loop;
	}
	/*
	 * convert n digits
	 */
	for(i=0; i<n; i++) {
		d = g;
		if(d > g)
			d--;
		g -= d;
		s1[i+1] = d + '0';
		g *= 10;
	}
	/*
	 * round by adding .5 into extra digit
	 */
	d = 5;
	for(i=n-1; i>=0; i--) {
		s1[i+1] += d;
		d = 0;
		if(s1[i+1] > '9') {
			s1[i+1] -= 10;
			d++;
		}
	}
	i = 1;
	if(d) {
		s1[0] = '1';
		e++;
		i = 0;
	} 
	/*
	 * copy into final place
	 * c1 digits of leading '0'
	 * c2 digits from conversion
	 * c3 digits after '.'
	 */
	d = 0;
	if(s)
		s2[d++] = '-';
	c1 = 0;
	c2 = f2 + 1;
	c3 = f2;
	if(c == 'g')
	if(e >= -5 && e <= f2) {
		c1 = -e - 1;
		c3 = c1;
		if(c1 < 0)
			c1 = 0;
		c3 = f2 - e;
		c = 'h';
	}
	if(c == 'f') {
		c1 = -e;
		if(c1 < 0)
			c1 = 0;
		if(c1 > f2)
			c1 = c2;
		c2 += e;
		if(c2 < 0)
			c2 = 0;
	}
	while(c1 > 0) {
		if(c1+c2 == c3)
			s2[d++] = '.';
		s2[d++] = '0';
		c1--;
	}
	while(c2 > 0) {
		if(c1+c2 == c3)
			s2[d++] = '.';
		s2[d++] = s1[i++];
		c2--;
	}
	/*
	 * strip trailing '0' on g conv
	 */
	if(c == 'g' || c == 'h') {
		for(n=d-1; n>=0; n--)
			if(s2[n] != '0')
				break;
		for(i=n; i>=0; i--)
			if(s2[i] == '.') {
				d = n;
				if(i != n)
					d++;
				break;
			}
	}
	if(c == 'e' || c == 'g') {
		s2[d++] = 'e';
		s2[d++] = '+';
		c1 = e;
		if(c1 < 0) {
			s2[d-1] = '-';
			c1 = -c1;
		}
		if(c1 >= 100) {
			s2[d++] = c1/100 + '0';
			c1 %= 100;
		}
		s2[d++] = c1/10 + '0';
		s2[d++] = c1%10 + '0';
	}

out:
	s2[d] = 0;
	strconv(s2, f1, -1);
	return FLOAT;
}

static
econv(o, f1, f2, f3)
double *o;
{

	return fltconv(*o, f1, f2, f3, 'e');
}

static
fconv(o, f1, f2, f3)
double *o;
{

	return fltconv(*o, f1, f2, f3, 'f');
}

static
gconv(o, f1, f2, f3)
double *o;
{

	return fltconv(*o, f1, f2, f3, 'g');
}

static
percent()
{

	if(out < eout)
		*out++ = '%';
	return 0;
}
