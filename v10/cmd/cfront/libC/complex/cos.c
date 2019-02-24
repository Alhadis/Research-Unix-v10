
#include "complex.h"
#include "const.h"

complex cos(complex z)
/*
	The cosine of z: cos(z)=cosh(iz).
*/
{
	complex	y(-z.im, z.re);	
	return cosh(y);
}



complex cosh(complex z)
/*
	The complex hyperbolic cosine of z
*/
{
	double 	cosh_x, sinh_x, cos_y, sin_y;

#define	COSH_GOOD	1e7
#define	COSH_HUGE	1e38

	if (z.re > MAX_EXPONENT) {
   		complex_error(C_COSH_RE, z.re);
   		cosh_x = sinh_x = COSH_HUGE;
    	}
	else if (z.re < MIN_EXPONENT) {
    		complex_error(C_COSH_RE, z.re);
    		cosh_x = COSH_HUGE;
    		sinh_x = -COSH_HUGE;
    	}
	else {
    		double pos_exp = exp(z.re); 
    		double neg_exp = 1/pos_exp;
    		cosh_x = (pos_exp + neg_exp)/2;
    		sinh_x = (pos_exp - neg_exp)/2;
    	}

	if (ABS(z.im) > COSH_GOOD) {
   		complex_error(C_COSH_IM, z.im);
    		cos_y = sin_y = 0;
    	}
	else {
    		cos_y = cos(z.im);
    		sin_y = sin(z.im);
    	}

	return complex(cos_y*cosh_x, sin_y*sinh_x);
}
