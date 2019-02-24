/* C compiler: MIPS float/double decoding */

#include "c.h"

struct real decode(ty, x) double x; {
	struct real r;

	if (ty == D) {
#ifdef MIPSEL
		struct d { unsigned fract2, fract1:20, exp:11, sign:1;  }
#else
		struct d { unsigned sign:1, exp:11, fract1:20, fract2; }
#endif
			*p = (struct d *) &x;
		r.sign = p->sign;
		r.exp = x ? p->exp - 1023 : 0;
		r.msb = (p->fract1<<12) | (p->fract2>>20);
		r.lsb = p->fract2<<12;
	} else {
		float f = x;
#ifdef MIPSEL
		struct f { unsigned fract:23, exp:8, sign:1; }
#else
		struct f { unsigned sign:1, exp:8, fract:23; }
#endif
			*p = (struct f *) &f;
		r.sign = p->sign;
		r.exp = f ? p->exp - 127 : 0;
		r.msb = p->fract<<9;
		r.lsb = 0;
	}
	return r;
}
