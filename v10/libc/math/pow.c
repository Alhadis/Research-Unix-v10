/*
	computes a^b.
	uses log and exp
*/

#include	<errno.h>
int errno;
double log(), exp();

double
pow(arg1,arg2)
double arg1, arg2;
{
	double temp;
	long l = arg2;

	if(l != arg2 || l>64 || l<-64) {
		if(arg1 <= 0.) {
			if(arg1 == 0.) {
				if(arg2 <= 0.)
					goto domain;
				return(0.);
			}
			if(l != arg2)
				goto domain;
			temp = exp(arg2 * log(-arg1));
			return l&1? -temp: temp;
		}
		return(exp(arg2 * log(arg1)));
	}
	temp = (l&1) ? arg1 : 1.0;
	for(l=l<0?-l:l; l >>= 1;) {
		arg1 *= arg1;
		if(l & 1)
			temp *= arg1;
	}
	return arg2 < 0? 1/temp: temp;

domain:
	errno = EDOM;
	return(0.);
}
