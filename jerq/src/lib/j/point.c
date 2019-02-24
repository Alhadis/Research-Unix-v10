#include <jerq.h>
point(b, pt, f)
	register Bitmap *b;
	Point pt;
	register f;
{
	register bit;
	register Word *p=addr(b, pt);
	if(ptinrect(pt, b->rect)==0)
		return;
	bit=1<<(WORDSIZE-1)-(pt.x&WORDMASK);
	if (f == F_XOR)
		*p ^= bit;
	else
		if (f == F_CLR)
			*p &= ~bit;
		else
			*p |= bit;
}
