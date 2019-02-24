/*   "@(#) atof1.c:  V 2.13  6/29/83" */

#include <values.h>
#include <errno.h>

double	atof();

extern int	errno;


double
atof1(p)
char *p;
{
	char	*s1,
		*s2;
	double	num;

	s1 = s2 = p;
	while (*s2 != '\0')
		switch (*s2) {
			case ' ':
			case '\015':
			case '\012':
			case '\t':
				s2++;
				break;
			case 'd':
			case 'D':
				*s1++ = 'e';
				++s2;
				break;
			default:
				*s1++ = *s2++;
				break;
		}
	*s1 = '\0';
	errno = 0;		/* if errno = ERANGE, make sure it's from atof*/
	num = atof(p);
	if (errno == ERANGE && (num > 1.0 || num < -1.0))
	{
		errno = 0;
		return((num > 0.0) ? MAXDOUBLE : -MAXDOUBLE);
	}
	else
		return(num);
}
