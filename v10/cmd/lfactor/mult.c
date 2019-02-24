#include "mp.h"

mint
mult(a,b)
mint a, b;
{
	mint z;
	mint c;
	mint mtemp;
	int sign;
	double a3, a2, a1, a0;
	double b1, b0;
	double temp1, temp2;

	if((a.high==0) && (b.high==0)){
		c.high = 0.;
		c.low = a.low * b.low;
		if((c.low<e16) && (c.low > -e16)){
			return(c);
		}
	}

	sign = 1;

	if((a.high<0) || (a.low<0)){
		sign = -sign;
		a.high = -a.high;
		a.low = -a.low;
	}
	if((b.high<0) || (b.low<0)){
		sign = -sign;
		b.high = -b.high;
		b.low = -b.low;
	}
	if(b.high != 0){
		mtemp = a;
		a = b;
		b = mtemp;
	}
	if(b.high!=0) ofl("mult");

	modf(a.low/1e8 , &a1);
	a0 = a.low - a1*1e8;
	modf(b.low/1e8 , &b1);
	b0 = b.low - b1*1e8;

	modf(a.high/1e8 , &a3);
	a2 = a.high - a3*1e8;

	temp1 = a0*b1 + a1*b0;
	modf(temp1/1e8 , &temp2);
	z.low = a0*b0 + (temp1 - temp2*1e8)*1e8;
	z.high = a1*b1 + a2*b0 + (a2*b1 + a3*b0)*1e8 + temp2;
	while(z.low >= e16){
		z.low = z.low - e16;
		z.high = z.high + 1.;
	}
	if(z.high >= e16)
		ofl("mult");
	if(a3*b1!= 0)
		ofl("mult");

	if(sign<0){
		z.high = -z.high;
		z.low = -z.low;
	}
	return(z);
}

mint
smult(a,b)
mint a;
int b;
{
	return(mult(a, itom(b)));
}
ofl(s)
char *s;
{
	printf("%s: overflow\n",s);
	abort();
}
