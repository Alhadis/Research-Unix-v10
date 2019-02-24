#include "../frame/frame.h"

#define	MINCHARS 64	/* for efficiency; min size of a string we'll alloc */
strzero(p)
	String *p;
{
	if(p->s && p->size>MINCHARS){
		p->n=0;
		p->size=0;	/* forces strinsure to free the old stuff */
	}
	strinsure(p, MINCHARS);
	p->n = 0;
}
straddc(p, c)
	register String *p;
	char c;
{
	strinsure(p, p->n+1);
	p->s[p->n++] = c;
}
snarf(p, i, j, q)
	register String *p, *q;
	register short i, j;
{
	register n=j-i;
	strinsure(q, n);
	movstring(n, p->s+i, q->s);
	q->n=n;
}
