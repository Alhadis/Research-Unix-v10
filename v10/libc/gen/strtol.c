#include <errno.h>
#include <ctype.h>

#define T_LONG_MAX (long)(~(unsigned long)0>>1)
#define T_LONG_MIN (~T_LONG_MAX)
#define POS 0
extern unsigned long strtoul();

long
strtol(iptr, endptr, base)
char *iptr;
register char **endptr;
{
	register char *nptr = iptr;
	register sign = POS;
	register long result;

	if(endptr)
		*endptr = iptr;
	while(isspace(*nptr))
		nptr++;
	switch(*nptr) {
	case '-':
		sign = !POS;
	case '+':
		nptr++;
	}
	if(!isalnum(*nptr))
		return 0;
	result = (long)strtoul(nptr, &iptr, base);
	if(endptr && iptr!=nptr)
		*endptr = iptr;
	if(result < 0) {
		if(sign==POS || result!=T_LONG_MIN)
			errno = ERANGE;
		return sign==POS? T_LONG_MAX: T_LONG_MIN;
	}
	return sign==POS? result: -result;
}
