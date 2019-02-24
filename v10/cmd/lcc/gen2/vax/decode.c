/* C compiler: VAX float/double decoding */

#include "c.h"

struct real decode(ty, x) double x; {
	struct real r;

	if (x == 0.0)
		r.exp = r.msb = r.lsb = r.sign = 0;
	else if (ty == D) {
		struct d { unsigned fract1:7, exp:8, sign:1, fract2:16,
			fract4:7, fract3:9, fract5:16; } *p = (struct d *) &x;
		r.sign = p->sign;
		r.exp = p->exp - 128 - 1;
		r.msb = (p->fract1<<25) | (p->fract2<<9) | p->fract3;
		r.lsb = (p->fract4<<25) | (p->fract5<<9);
	} else {
		float f = x;
		struct f { unsigned fract1:7, exp:8, sign:1, fract2:16; } *p = (struct f *) &f;
		r.sign = p->sign;
		r.exp = p->exp - 128 - 1;
		r.msb = (p->fract1<<25) | (p->fract2<<9);
		r.lsb = 0;
	}
	return r;
}
