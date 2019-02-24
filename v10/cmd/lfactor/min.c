/* quad precision input routine.
 * reads and converts the next number on the
 * standard input after skipping over white space.
 */
#include <stdio.h>
#include "mp.h"

min(a)
mint *a;
{
return(m_in(a, 10, stdin));
}

m_in(a,b,f)
mint *a;
int b;
FILE *f;
{

	mint x;
	mint base;
	int sign, c;

	base = itom(b);
	sign = 1;
	x.high = 0.;
	x.low = 0;
	while((c = getc(f)) != EOF)
	switch(c)
	{
	case '\\':
		getc(f);
		continue;
	case '\t':
	case '\n':
		x.high *= sign;
		x.low *= sign;
		*a = x;
		return(0);
	case ' ':
		continue;
	case '-':
		sign = -sign;
		continue;
	default:
		if(c>='0' && c<='9'){
			c -= '0';
			x = mult(x, base);
			x = madd(x, itom(c));
			continue;
		}else{
			printf("min: bad number format\n");
			exit();
		}
	}
	*a = x;
	return(EOF);
}
