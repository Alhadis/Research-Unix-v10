/*
	floating point Bessel's function of
	the first and second kinds and of
	integer order.

	int n;
	double x;
	besjn(n,x);

	returns the value of Jn(x) for all
	integer values of n and all real values
	of x.

	There are no error returns.
	Calls besj0, besj1.

	For n=0, besj0(x) is called,
	for n=1, besj1(x) is called,
	for n<x, forward recursion us used starting
	from values of besj0(x) and besj1(x).
	for n>x, a continued fraction approximation to
	j(n,x)/j(n-1,x) is evaluated and then backward
	recursion is used starting from a supposed value
	for j(n,x). The resulting value of j(0,x) is
	compared with the actual value to correct the
	supposed value of j(n,x).

	yn(n,x) is similar in all respects, except
	that forward recursion is used for all
	values of n>1.
*/

#include <math.h>
#include <errno.h>

int	errno;

double
besjn(n,x) int n; double x;{
	int i;
	double a, b, temp;
	double xsq, t;
	double besj0(), besj1();

	if(n<0){
		n = -n;
		x = -x;
	}
	if(n==0) return(besj0(x));
	if(n==1) return(besj1(x));
	if(x == 0.) return(0.);
	if(n>x) goto recurs;

	a = besj0(x);
	b = besj1(x);
	for(i=1;i<n;i++){
		temp = b;
		b = (2.*i/x)*b - a;
		a = temp;
	}
	return(b);

recurs:
	xsq = x*x;
	for(t=0,i=n+16;i>n;i--){
		t = xsq/(2.*i - t);
	}
	t = x/(2.*n-t);

	a = t;
	b = 1;
	for(i=n-1;i>0;i--){
		temp = b;
		b = (2.*i/x)*b - a;
		a = temp;
	}
	return(t*besj0(x)/b);
}

double
besyn(n,x) int n; double x;{
	int i;
	int sign;
	double a, b, temp;
	double besy0(), besy1();

	if (x <= 0) {
		errno = EDOM;
		return(-HUGE);
	}
	sign = 1;
	if(n<0){
		n = -n;
		if(n%2 == 1) sign = -1;
	}
	if(n==0) return(besy0(x));
	if(n==1) return(sign*besy1(x));

	a = besy0(x);
	b = besy1(x);
	for(i=1;i<n;i++){
		temp = b;
		b = (2.*i/x)*b - a;
		a = temp;
	}
	return(sign*b);
}
