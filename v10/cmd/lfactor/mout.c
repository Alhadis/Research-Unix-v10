#include <stdio.h>
#include "mp.h"
double modf();
char obuf[200];


void
mout(a)
mint a;
{
	m_out(a, 10, stdout);
	printf("\n");
}
void
mout1(a)
mint a;
{
	m_out(a,10,stdout);
}

m_out(a,b,f)
mint a;
int b;
FILE *f;
{
	int sign;
	double carry, temp2;
	mint x;
	char *bp;

	x = a;
	sign = 1;
	bp = &obuf[199];
	if((x.high==0) && (x.low==0)){
		printf("0");
		return;
	}
	*bp-- = 0;
	if(x.high<0){
		x.high = -x.high;
		sign = -1;
	}
	if(x.low<0){
		x.low = -x.low;
		sign = -1;
	}

	while((x.high!=0) || (x.low!=0)){
		carry = 0;
		if(x.high != 0){
			carry = modf(x.high/10. , &x.high);
		}
		temp2 = modf(x.low/10. , &x.low);
		x.low += (int)(10.*carry+0.5) * e15;
		*bp-- = (int)(10.*temp2+0.5) + '0';
	}
	if(sign<0)
		*bp-- = '-';
	bp++;
	fprintf(f,"%s",bp);
}
