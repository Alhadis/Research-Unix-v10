/* C compiler: SPARC float/double decoding */

#include "c.h"

struct real decode(ty, x) double x; {
	struct real r;

	if (ty == D) {
		struct d { unsigned sign:1, exp:11, fract1:20, fract2; }
			*p = (struct d *) &x;
		r.sign = p->sign;
		r.exp = x ? p->exp - 1023 : 0;
		r.msb = (p->fract1<<12) | (p->fract2>>20);
		r.lsb = p->fract2<<12;
	} else {
		float f = x;
		struct f { unsigned sign:1, exp:8, fract:23; }
			*p = (struct f *) &f;
		r.sign = p->sign;
		r.exp = f ? p->exp - 127 : 0;
		r.msb = p->fract<<9;
		r.lsb = 0;
	}
	return r;
}
