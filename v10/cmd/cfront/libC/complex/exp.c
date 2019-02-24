
#include "complex.h"
#include "const.h"


complex exp(complex z)
/*
	The complex exponentiation function: e**z, e being 2.718281828...

	In case of overflow, return EXPHUGE with the appropriate phase.
	In case of underflow return 0.
	In case of ridiculous input to "sin" and "cos", return 0.
*/
{
	complex	answer;
	double 	radius, sin_theta, cos_theta;

#define EXPHUGE	1e38
#define EXPGOOD	1e7

	if (z.re > MAX_EXPONENT) {
    		complex_error(C_EXP_RE_POS, z.re);
    		radius = EXPHUGE;
    	}
	else if (z.re < MIN_EXPONENT) {
    		complex_error(C_EXP_RE_NEG, z.re);
   		radius = 0;
   	}
	else {
    		radius = exp(z.re);
    	}

	if (z.im > EXPGOOD || z.im < -EXPGOOD) {
    		complex_error(C_EXP_IM, z.im);
    		sin_theta = cos_theta = 0;
    	}
	else {
    		sin_theta = sin(z.im);
    		cos_theta = cos(z.im);
    	}

	answer.re = radius * cos_theta;
	answer.im = radius * sin_theta;

	return answer;
}
