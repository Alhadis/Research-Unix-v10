#include "sky.h"

output()
{

	printf("%s ",object);

	if(!((flags&GEO)||(flags&HELIO))){
		prhms(ra,3);
		prdms(decl2,2);
		printf(" %8.4f %8.4f %9.4f\n", az, el, semi2);
	}
	if((flags&GEO)){
		prhms(alpha,3);
		prdms(delta,2);
		printf(" %13.7f\n", georad);
	}
	if((flags&HELIO)){
		while(lambda<0)
			lambda += 2.*pi;
		while(lambda > 2.*pi)
			lambda -= 2.*pi;
		prdms(lambda,3);
		prdms(beta,3);
		printf(" %14.8f\n", rad);
	}
}

prhms(arg,n)
double arg;
int n;
{
	int n1, n2;
	double x;
	char *format = " %2d %02d %07.4f";

	format[14] = n + '0';
	format[12] = n + '3';
	while(arg < 0.) arg += 2.*pi;
	arg /= 15.;
	x = arg/radian;
	n1 = floor(x);
	x = (x-n1)*60.;
	n2 = floor(x);
	x = x - n2;
	x = x * 60.;
	printf(format, n1, n2, x);
}

prdms(arg,n)
double arg;
int n;
{
	int n1, n2;
	double x;
	int sign;
	char *format1 = " %4d %02d %07.4f";
	char *format2 = "   -0 %02d %07.4f";

	format1[14] = n + '0';
	format1[12] = n + '3';
	format2[15] = n + '0';
	format2[13] = n + '3';
	if(arg<0.){
		arg = -arg;
		sign = -1;
	}else{
		sign = 1;
	}
	x = arg/radian;
	n1 = floor(x);
	x = (x-n1)*60.;
	n2 = floor(x);
	x -= n2;
	x *= 60.;
	n1 *= sign;
	if((n1==0)&&(sign== -1))
		printf(format2, n2, x);
	else
		printf(format1, n1, n2, x);
}
