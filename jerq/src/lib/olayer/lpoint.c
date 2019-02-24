#include <jerq.h>
#include <layer.h>

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
		for(o=l->obs; o; o=o->next)
			if(ptinrect(pt, o->rect)){
				b=o->bmap;
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
