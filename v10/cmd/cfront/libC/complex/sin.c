
#include "complex.h"
#include "const.h"

complex	sin(complex z)
/*
	sine of z: -i * sinh(i*z)
*/
{
	complex	y = complex(-z.im, z.re);	/*  i * z */
	y = sinh(y);				/*  csinh(y) */
	return complex(y.im, -y.re);		/*  -i * y */
}

complex sinh(complex z)
/*
	The hyperbolic sine
*/
{
	double cosh_x, sinh_x, cos_y, sin_y;

#define SINH_GOOD	1e7
#define SINH_HUGE	1e38

	if (z.re > MAX_EXPONENT) {
    		complex_error(C_SINH_RE, z.re);
    		cosh_x = sinh_x = SINH_HUGE;
    	}
	else if (z.re < MIN_EXPONENT) {
		complex_error(C_SINH_RE, z.re);
    		cosh_x = SINH_HUGE;
    		sinh_x = -SINH_HUGE;
    	}
	else {
    		double pos_exp = exp(z.re); 
    		double neg_exp = 1/pos_exp;
   		cosh_x = (pos_exp + neg_exp)/2;
   		sinh_x = (pos_exp - neg_exp)/2;
    	}

	if (ABS(z.im) > SINH_GOOD) { 
    		complex_error(C_SINH_IM, z.im);
    		cos_y = sin_y = 0;
    	}
	else  {
    		cos_y = cos(z.im);
    		sin_y = sin(z.im);
    	}

	return complex(cos_y*sinh_x, sin_y*cosh_x);
}
