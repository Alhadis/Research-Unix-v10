#include <math.h>
#include <errno.h>
extern	int	errno;
double	errcheck();

double Log(x)
	double x;
{
	return errcheck(log(x), "log");
}
double Log10(x)
	double x;
{
	return errcheck(log10(x), "log10");
}

double Sqrt(x)
	double x;
{
	return errcheck(sqrt(x), "sqrt");
}

double Gamma(x)
	double x;
{
	double y;
	extern int signgam;
	y=errcheck(gamma(x), "gamma");
	if(y>88.0)
		execerror("gamma result out of range", (char *)0);
	return signgam*exp(y);
}

double Exp(x)
	double x;
{
	return errcheck(exp(x), "exp");
}

double Asin(x)
	double x;
{
	return errcheck(asin(x), "asin");
}

double Acos(x)
	double x;
{
	return errcheck(acos(x), "acos");
}

double Sinh(x)
	double x;
{
	return errcheck(sinh(x), "sinh");
}

double Cosh(x)
	double x;
{
	return errcheck(cosh(x), "cosh");
}

double Pow(x, y)
	double x, y;
{
	return errcheck(pow(x,y), "exponentiation");
}

double integer(x)
	double x;
{
	return (double)(long)x;
}

double errcheck(d, s)	/* check result of library call */
	double d;
	char *s;
{
	if (errno == EDOM) {
		errno = 0;
		execerror(s, "argument out of domain");
	} else if (errno == ERANGE) {
		errno = 0;
		execerror(s, "result out of range");
	}
	return d;
}
