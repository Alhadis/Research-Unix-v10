#include "mp.h"
mint mulmod();

/*
	returns 0 if pseudoprime
		1 if composite
		2 if composite pseudoprime
*/
int
pseudo(nn, arg2)
mint nn;
mint arg2;
{
	mint nm1, y, z;
	mint zero, one, two;
	mint rem;
	mint mtemp;
	mint lasty;
	int i;

	zero.low = zero.high = 0.0;
	one = itom(1);
	two = itom(2);

		if(mcmp(nn,zero) <= 0) abort(0);
		if(mcmp(nn,two) <=0) abort(0);
		mdiv(nn, two, &rem);
		if(mcmp(rem, zero) == 0.0) abort(0);

		nm1 = msub(nn, one);
		y = one;
		z = arg2;

		i = 0;
		while(1){
			mtemp = mdiv(nm1, two, &rem);
			if(mcmp(rem,zero) == 0){
				nm1 = mtemp;
				i = i + 1;
			}else break;
		}

		while(mcmp(nm1,zero) != 0){
			nm1 = mdiv(nm1, two, &rem);
			if(mcmp(rem, one) == 0){
				y = mulmod(y, z, nn);
			}
			z = mulmod(z, z, nn);
		}
		if(mcmp(y, one) == 0){
			return(0);
		}
		while(i--){
			lasty = y;
			y = mulmod(y,y,nn);
			if(mcmp(y, one) == 0) break;
		}
		if(mcmp(y, one) != 0){
			return(1);
		}
		if(mcmp(lasty, one) == 0){
			printf("pseudo: error.\n");
		}
		if(mcmp(lasty,msub(nn,one)) != 0){
			return(2);
		}
		return(0);
}
mint
mulmod(a,b,c)
mint a, b, c;
{

	mint mjunk;
	mint mtemp1, mtemp2;
	mint xy0, xy1, xy2, xy3;
	mint x1, x2, y1, y2;
	int i;
	double z0, z1, z2, z3;
	double s0, s1, s2;
	double tquot, dtemp1, dtemp2;
	mint zero;


	zero.low = 0.0;
	zero.high = 0.0;
	if(mcmp(c,zero) == 0){
		printf("mulmod: divide check\n");
		abort(0);
	}

	if((mcmp(a,zero)<0) || (mcmp(b,zero)<0)){
		printf("mulmod: negative argument.\n");
		abort(0);
	}
	if(mcmp(c,zero) < 0){
		printf("mulmod: negative modulus.\n");
		abort(0);
	}
	x1.low = a.low;
	x1.high = 0;
	x2.low = a.high;
	x2.high = 0;

	y1.low = b.low;
	y1.high = 0;
	y2.low = b.high;
	y2.high = 0;

	xy0 = mult(x1,y1);
	xy1 = mult(x1,y2);
	xy2 = mult(x2,y1);
	xy3 = mult(x2,y2);

	z0 = xy0.low;

	mtemp1.low = xy0.high;
	mtemp1.high = 0;
	mtemp2.low = xy1.low;
	mtemp2.high = 0;
	mtemp1 = madd(mtemp1, mtemp2);
	mtemp2.low = xy2.low;
	mtemp1 = madd(mtemp1, mtemp2);
	z1 = mtemp1.low;

	mtemp1.low = mtemp1.high;
	mtemp1.high = 0;
	mtemp2.low = xy1.high;
	mtemp2.high = 0;
	mtemp1 = madd(mtemp1, mtemp2);
	mtemp2.low = xy2.high;
	mtemp1 = madd(mtemp1, mtemp2);
	mtemp2.low = xy3.low;
	mtemp1 = madd(mtemp1, mtemp2);
	z2 = mtemp1.low;

	mtemp1.low = mtemp1.high;
	mtemp1.high = 0;
	mtemp2.low = xy3.high;
	mtemp2.high = 0;
	mtemp1 = madd(mtemp1, mtemp2);
	z3 = mtemp1.low;


	if(mtemp1.high != 0.0){
		printf("mulmod: error\n");
	}

	if(c.high == 0.0){
		mtemp1.high = 0.0;
		mtemp1.low = z3;
		mjunk = mdiv(mtemp1, c, &mtemp2);
		z3 = mtemp2.low;
		if(mtemp2.high != 0.0) trouble(12);
		mtemp1.high = z3;
		mtemp1.low = z2;
		mjunk = mdiv(mtemp1, c, &mtemp2);
		mtemp1.high = mtemp2.low;
		mtemp1.low = z1;
		mjunk = mdiv(mtemp1, c, &mtemp2);
		mtemp1.high = mtemp2.low;
		mtemp1.low = z0;
		mjunk = mdiv(mtemp1, c, &mtemp2);
		if(mcmp(mtemp2, c) >= 0) trouble(10);
		if(mcmp(mtemp2, zero) < 0) trouble(11);
		return(mtemp2);
	}

	mtemp1.high = z3;
	mtemp1.low = z2;
	if(mcmp(mtemp1, c) >= 0){
		mjunk = mdiv(mtemp1, c, &mtemp2);
		z3 = mtemp2.high;
		z2 = mtemp2.low;
	}
	dtemp1 = (z3*e16 + z2) + z1/e16;
	tquot = dtemp1/(c.high+c.low/e16);
	modf(tquot, &tquot);
	y1.low = tquot;
	y1.high = 0.0;
	x1.low = c.low;
	x1.high = 0.0;
	x2.low = c.high;
	x2.high = 0;
	xy0 = mult(x1, y1);
	xy1 = mult(x2, y1);

	s0 = xy0.low;

	mtemp1.low = xy0.high;
	mtemp1.high = 0.0;
	mtemp1 = madd(mtemp1, xy1);
	s1 = mtemp1.low;
	s2 = mtemp1.high;
	s0 = z1 - s0;
	s1 = z2 - s1;
	s2 = z3 - s2;

	if(s2 > 0.0) {s2 = s2 - 1; s1 = s1 + e16;}
	if(s2 < 0.0) {s2 = s2 + 1; s1 = s1 - e16;}
	if((s1<0.0) && (s0>0.0)){
		s1 = s1 + 1;
		s0 = s0 - e16;
	}else
	if((s1>0.0) && (s0<0.0)){
		s1 = s1 - 1;
		s0 = s0 + e16;
	}
	if((s1*e16+s0) < 0.0){
		mtemp1.low = s0;
		mtemp1.high = s1;
		mtemp1 = madd(mtemp1, c);
		s1 = mtemp1.high;
		s0 = mtemp1.low;
	}
	if(s2 != 0.0) trouble(1);
	mtemp1.low = s0;
	mtemp1.high = s1;
	if(mcmp(mtemp1,zero) < 0) trouble(2);
	if(mcmp(mtemp1,c) >= 0){
		mtemp1.high = s1;
		mtemp1.low = s0;
		mtemp1 = msub(mtemp1, c);
		s1 = mtemp1.high;
		s0 = mtemp1.low;
	}
	if(mcmp(mtemp1,c) >0) trouble(3);

	z3 = s2;
	z2 = s1;
	z1 = s0;

	dtemp1 = (z2*e16 + z1) + z0/e16;
	tquot = dtemp1/(c.high + c.low/e16);
	modf(tquot, &tquot);
	y1.low = tquot;
	y1.high = 0.0;
	x1.low = c.low;
	x1.high = 0.0;
	x2.low = c.high;
	x2.high = 0.0;
	xy0 = mult(x1, y1);
	xy1 = mult(x2, y1);

	s0 = xy0.low;

	mtemp1.low = xy0.high;
	mtemp1.high = 0.0;
	mtemp1 = madd(mtemp1, xy1);
	s1 = mtemp1.low;
	s2 = mtemp1.high;

	s0 = z0 - s0;
	s1 = z1 - s1;
	s2 = z2 - s2;
	if(s2 > 0.0) {s2 = s2 - 1; s1 = s1 + e16;}
	if(s2 < 0.0) {s2 = s2 + 1; s1 = s1 - e16;}
	if((s1<0.0) && (s0>0.0)){
		s1 = s1 + 1;
		s0 = s0 - e16;
	}else
	if((s1>0.0) && (s0<0.0)){
		s1 = s1 - 1;
		s0 = s0 + e16;
	}
	if((s1*e16+s0) < 0.0){
		mtemp1.low = s0;
		mtemp1.high = s1;
		mtemp1 = madd(mtemp1, c);
		s1 = mtemp1.high;
		s0 = mtemp1.low;
	}
	if(s2 != 0.0) trouble(4);
	mtemp1.low = s0;
	mtemp1.high = s1;
	if(mcmp(mtemp1,zero) < 0) trouble(5);
	if(mcmp(mtemp1,c) >= 0){
		mtemp1.high = s1;
		mtemp1.low = s0;
		mtemp1 = msub(mtemp1, c);
		s1 = mtemp1.high;
		s0 = mtemp1.low;
	}
	if(mcmp(mtemp1,c) > 0) trouble(6);

	z2 = s2;
	z1 = s1;
	z0 = s0;
	mtemp1.high = s1;
	mtemp1.low = s0;
	return(mtemp1);
}

trouble(i)
int i;
{
	printf("mulmod: error %d\n", i);
	abort(0);
}
