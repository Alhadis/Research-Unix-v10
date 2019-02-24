/*
 * AT&T Bell Laboratories
 *
 * convert string to long integer
 * if non-null e will point to first unrecognized char in s
 * if m>1 and no multipler was specified then the result is multiplied by m
 *
 * integer numbers are of the form:
 *
 *	[sign][base][number][multiplier]
 *
 *	base:		nnn#		base nnn
 *			0[xX]		hex
 *			0		octal
 *			[1-9]		decimal
 *
 *	number:		[0-9a-zA-Z]*
 *
 *	multiplier:	.		pseudo-float (100) + subsequent digits
 *			[bB]		block (512)
 *			[cC]		char (1)
 *			[gG]		giga (1024*1024*1024)
 *			[kK]		kilo (1024)
 *			[mM]		mega (1024*1024)
 */

long
strton(s, e, m)
register char*	s;
char**		e;
int		m;
{
	register long	n;
	register int	neg;
	char*		p;

	extern long	strtol();

	if (neg = (n = strtol(s, &p, 0)) < 0) n = -n;
	if (*p == '#') n = strtol(++p, &p, n);
	switch (*p++)
	{
	case 'b':
	case 'B':
		n *= 512;
		break;
	case 'c':
	case 'C':
		break;
	case 'g':
	case 'G':
		n *= 1024 * 1024 * 1024;
		break;
	case 'k':
	case 'K':
		n *= 1024;
		break;
	case 'm':
	case 'M':
		n *= 1024 * 1024;
		break;
	case '.':
		n *= 100;
		for (m = 10; *p >= '0' && *p <= '9'; m /= 10) 
			n += m * (*p++ - '0');
		break;
	default:
		p--;
		if (m > 1) n *= m;
		break;
	}
	if (e) *e = p;
	return(neg ? -n : n);
}
