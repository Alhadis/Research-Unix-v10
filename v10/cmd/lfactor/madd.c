#include "mp.h"

mint
madd(a,b)
mint a,b;
{
	mint x;
	int i;

	x.low = a.low + b.low;
	x.high = a.high + b.high;
	if(x.low > e16){
		x.low -= e16;
		x.high += 1.;
	}else
	if(x.low < -e16){
		x.low += e16;
		x.high -= 1.;
	}
	if(x.high >= e16) ofl("madd");
	if((x.high>0) && (x.low<0)){
		x.high -= 1.;
		x.low += e16;
	}
	if((x.high<0)  && (x.low>0)){
		x.high += 1.;
		x.low -= e16;
	}
	return(x);
}

mint
msub(a,b)
mint a,b;
{
	b.high = -b.high;
	b.low = -b.low;
	return(madd(a,b));
}

int
mcmp(a,b)
mint a, b;
{
	if(a.high > b.high)
		return(1);
	if(a.high < b.high)
		return(-1);
	if(a.low > b.low)
		return(1);
	if(a.low < b.low)
		return(-1);
	return(0);
}


mint
itom(i)
int i;
{
	mint x;

	x.high = 0.;
	x.low = i;
	return(x);
}
mint
mchs(a)
mint a;
{
	a.high = -a.high;
	a.low = -a.low;
	return(a);
}
