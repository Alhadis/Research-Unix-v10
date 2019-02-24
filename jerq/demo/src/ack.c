#include <jerq.h>
/* benchmark program number 1 */

/* This benchmark is the property of Bell Laboratories and is proprietary.
 * It is intended for internal use and is not to be used otherwise,
 * reproduced or published without Bell Laboratories approval. */

/* ackermann's function */

main() {

	WonB();
	*DADDR=56*(1024L/4);
	ack(3,9);
	}

/* ackermann's function */
ack(m,n) int m,n; {

	if (m == 0)
		return(n+1);
	    else
		if (n == 0)
			return(ack(m-1,1));
		    else
			return(ack(m-1,ack(m,n-1)));
	}
