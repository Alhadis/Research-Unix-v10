/* Copyright AT&T Bell Laboratories, 1993 */
/* Convert string to unsigned long.
** Assumes ASCII properties - contiguity of 0-9, A-Z, a-z,
** digits less than letters, lower case maskable to upper.
*/
#include <ctype.h>
#include <errno.h>

#define DIGIT(x)	((x)<='9'? (x)-'0': TOUPPER(x)+10-'A')
#define TOUPPER(x)	((x) & ~('a'-'A'))
#define MBASE	('z' - 'a' + 1 + 10)
#define T_ULONG_MAX	~(unsigned long)0

unsigned long
strtoul(str, nptr, base)
char *str;
char **nptr;
int base;
{
	register unsigned long val;
	register int c;
	int xx;
	int overflow = 0, neg = 0;
	unsigned long	multmax;

	if (nptr)
		*nptr = str; /* in case no number is formed */
	c = *str;
	while (isspace(c))
		c = *++str;
	switch(c) {
	case '-':
		neg = 1;
	case'+':
		c = *++str;
	}
	if (base == 0)
		if (c != '0')
			base = 10;
		else if (TOUPPER(str[1]) == 'X')
			base = 16;
		else
			base = 8;
	if (base < 2 || base > MBASE)
		return (0); /* base is invalid -- should be a fatal error */
	if (!isalnum(c) || DIGIT(c) >= base)
		return (0); /* no number formed */
	if (base == 16 && c == '0' && (TOUPPER(str[1]) == 'X') &&
		isxdigit(str[2]))
		c = *(str += 2); /* skip over leading "0x" or "0X" */

	multmax = T_ULONG_MAX / (unsigned)base;
	for (val = DIGIT(c); isalnum(c = *++str) && (xx = DIGIT(c)) < base; )
		if (val > multmax || (val = val*base + xx) < xx)
			overflow = 1;
	if (nptr)
		*nptr = str;
	if (overflow) {
		errno = ERANGE;
		return T_ULONG_MAX;
	} else
		return neg? -val: val;
}
