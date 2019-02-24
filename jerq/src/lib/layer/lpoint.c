#include <jerq.h>
#include "layer.h"

lpoint(l, pt, f)
	register Layer *l;
	Point pt;
	Code f;
{
	register bit;
	register Word *p;
	register Obscured *o;
	register Bitmap *b;
	b=(Bitmap *)l;
	if(ptinrect(pt, l->rect)){
		if (l->someobs == l->nonevis)
			b = (l->nonevis?l->obs:(Bitmap *)l);
		else
			for(o=lfront; o; o=o->back)
				if(ptinrect(pt, o->rect)){
					b=((o!=l)?l->obs:(Bitmap *)l);
					break;
				}
		p=addr(b, pt);
		bit=1<<(WORDSIZE-1)-(pt.x&WORDMASK);
		if (f == F_XOR)
			*p ^= bit;
		else
			if (f == F_CLR)
				*p &= ~bit;
			else
				*p |= bit;
	}
}
