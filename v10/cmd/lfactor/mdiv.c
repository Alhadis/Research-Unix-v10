#include "mp.h"

mint
mdiv(a,b,r)
mint a, b, *r;
{
	mint c;
	mint num, den;
	mint quot, rem;
	mint temp, trial;
	mint zero, one;
	double tquot;
	int nsign, dsign;

	zero = itom(0);
	one = itom(1);
	nsign = 1;
	dsign = 1;
	if((b.low==0) && (b.high==0)){
		printf("mdiv: Zero divide.\n");
		abort();
	}
	num = a;
	den = b;
	if((num.low<0) || (num.high<0)){
		num.high = -num.high;
		num.low = -num.low;
		nsign = -1;
	}
	if((den.low<0) || (den.high<0)){
		den.high = -den.high;
		den.low = -den.low;
		dsign = -1;
	}
	if(mcmp(num,den) < 0){
		*r = a;
		return(zero);
	}
	tquot = (num.high*e16+num.low) / (den.high*e16+den.low);
	modf(tquot , &tquot);
	if(tquot < e16){
		c.high = 0.;
		c.low = tquot;
		temp = mult(c, den);
		*r = msub(num, temp);
		if(mcmp(*r,den) >= 0){
			c = madd(c, one);
			*r = msub(*r, den);
		}
		if(mcmp(*r,zero) < 0){
			c = msub(c,one);
			*r = madd(*r, den);
		}
		if(nsign == -1){
			c.low = -c.low;
			r->high = -r->high;
			r->low = -r->low;
		}
		if(dsign == -1){
			c.low = -c.low;
		}
	return(c);
	}
	modf( tquot/e16 , &trial.high);
	trial.low = tquot - trial.high*e16;
	temp = mult(trial, den);
	temp = msub(num, temp);
	quot = mdiv(temp, den, &rem);
	c = madd(trial, quot);
	if(mcmp(rem,den) >= 0){
		rem = msub(rem, den);
		c = madd(c, one);
	}
	if(mcmp(rem,zero) < 0){
		rem = madd(rem, den);
		c = msub(c, one);
	}
	*r = rem;
	if(nsign == -1){
		c.high = -c.high;
		c.low = -c.low;
		r->high = -r->high;
		r->low = -r->low;
	}
	if(dsign == -1){
		c.high = -c.high;
		c.low = -c.low;
	}
	return(c);
}

mint
sdiv(a,b,r)
mint a;
int b, *r;
{
	mint c;
	mint bb, rr;
	double temp1;

	if(a.high==0){
		c.high = 0.;
		modf(a.low/b , &temp1);
		*r = a.low - temp1*b;
		c.low = temp1;
		return(c);
	}
	bb = itom(b);
	c = mdiv(a,bb,&rr);
	*r = rr.low;
	return(c);
}

mint
msqrt(a,r)
mint a, *r;
{
	mint b;
	double temp, sqrt();
	mint dtemp;

	if((a.high<0) || (a.low<0)){
		printf("msqrt: Negative argument.\n");
		abort();
	}
	temp = sqrt(a.high*e16+a.low);
	modf(temp , &temp);
	b.high = 0.;
	b.low = temp;
	dtemp.high = 0.;
	dtemp.low = temp;
	dtemp = mult(dtemp, dtemp);
	*r = msub(a, dtemp);
	return(b);
}

mint
mcbrt(a,r)
mint a, *r;
{
	mint b;
	double temp;
	double log(), exp();
	mint dtemp;
	int sign = 1;
	mint zero;

	zero.low = 0.0;
	zero.high = 0.0;
	if((a.high<0) || (a.low<0)){
		sign = -1;
		a = mchs(a);
	}

	temp = exp(log(a.high*e16 + a.low)/3.0);
	modf(temp, &temp);
retry:
	b.high = 0;
	b.low = temp;

	dtemp.high = 0;
	dtemp.low = temp;
	dtemp = mult(dtemp,mult(dtemp,dtemp));
	*r = msub(a, dtemp);

	if(mcmp(*r, zero) < 0){
		temp = temp - 1;
		goto retry;
	}
	temp = temp + 1;
	dtemp.high = 0;
	dtemp.low = temp;
	dtemp = mult(dtemp,mult(dtemp,dtemp));
	if(mcmp(a,dtemp) >= 0){
		goto retry;
	}
	return(b);
}
