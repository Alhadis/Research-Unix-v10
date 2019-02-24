
#include "complex.h"
#include "const.h"

#define	LOGWILD	-1000
#define	LOGDANGER	1e18
#define PERIL(t) (t > LOGDANGER || (t < 1/LOGDANGER && t != 0) )

complex log(complex z)
/*
	The complex natural logarithm of "z".
	If z = 0, then the answer LOGWILD + 0*i is returned.

	Stu Feldman says that the peril tests for the following function
	are "acceptable for now", but certain things like
	complex variables outside the over/underflow range 
	will cause floating exceptions.
*/
{
	complex	answer;	
	double  partial;

	if ( z.re == 0 && z.im == 0) {
    		complex_error(C_LOG_0,0);
    		answer.re = LOGWILD;
    	}
	else {
	/*
     		 Check for (over/under)flow, and fixup if necessary.
	*/
    		double x = ABS(z.re);
    		double y = ABS(z.im);
    
    		if ( x>y && PERIL(x) ) {
	 		z.im /=x;
         		z.re /= x;  /* z.re is replaced by 1 or -1 */
         		partial = log(x);
        	}
    		else if (PERIL(y)) {
         		z.im /= y;  /* roles of re, im reversed from previous */
	 		z.re /= y;
         		partial = log(y);
       		}
    		else partial = 0;
    
	/*
		z.re*z.re and z.im*z.im should not cause problems now.
	*/
    
    		answer.im = atan2(z.im,z.re); 
    		answer.re = log(z.re*z.re + z.im*z.im)/2 + partial;
    	}
	return answer;
}
